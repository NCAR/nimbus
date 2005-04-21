//#define SQL
/*
-------------------------------------------------------------------------
OBJECT NAME:	pms2_data.c

FULL NAME:	Translate PMS2D records to look like 1D histograms

ENTRY POINTS:	Add2DtoList()
		xlTwodInit()
		xl2dDeadTime1()
		xl2dDeadTime2()
		xlTwoD()
		xlOneD()

STATIC FNS:	AddMore2dData()

DESCRIPTION:	This sums up the data bins from SampleRate to 1hz.

INPUT:		

OUTPUT:		

NOTES:		Order of raw variables is determined alphabetically.  There
		are some not so raw (more like derived) variables being
		created here.  Watch out changing variable names (A1DC, etc).

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1999-2000
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include "queue.h"

#define OVERLOAD	(0xffffffff)

static Queue	*probes[MAX_PMS2];

/* Data from the file is only marked as 'C1', 'C2', 'P1', 'P2', we'll
 * use this array to determine which records go with which Queue above.
 */
static short	probeIDorder[MAX_PMS2D] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static long	startTime[MAX_PMS2];
static short	startMilliSec[MAX_PMS2];
static NR_TYPE	twoD[MAX_PMS2][BINS_64];

#ifdef SQL
#include "psql.h"

extern PostgreSQL	*psql;

RAWTBL	*cur_rp;
#endif


/* Exported to amlib/std/pms2d.c */

NR_TYPE	deadTime[MAX_PMS2D][2];	/* Probe down time (subtract from 1 second) */
size_t	overFlowCnt[MAX_PMS2];	/* Number of particles greater than 64 bins */


struct particle
  {
  long	time;		/* Seconds since mid-night	*/
  long	msec;

  size_t	w, h;
  bool		edge;		/* particle touched either edge		*/
  ulong		timeWord;
  ushort	x1, x2;		/* for particles that touch both edges.	*/
  NR_TYPE	deltaTime;	/* Amount of time between prev & this particle*/
  NR_TYPE	liveTime;	/* Amount of time consumed by particle	*/
  ulong		*p;		/* Pointer into P2d_rec			*/
  size_t	nSlices;
  };

typedef struct particle Particle;


static void AddMore2dData(Queue *, long, int);
void	Process(Queue *, P2d_rec *, int probeCnt), sTwodInit(RAWTBL *varp);
void	ProcessHVPS(Queue *, P2d_rec *, int probeCnt), sTwodInitH(RAWTBL *varp);

bool	Next2dRecord(P2d_rec *, int, short);


/* -------------------------------------------------------------------- */
void Add2DtoList(RAWTBL *varp)	/* Called by hdr_decode.c */
{
  size_t	cCnt, pCnt, hCnt;
  static size_t	piCnt = 0;

  probes[piCnt] = CreateQueue();
  
  if (strstr(varp->name, "HVPS"))
    probeIDorder[varp->ProbeCount>>1] |= 'H' << 8;
  else
    probeIDorder[varp->ProbeCount>>1] |= varp->name[3] << 8;

  ++piCnt;

//printf("name=%s, cnt=%d\n", varp->name, varp->ProbeCount>>1);

  /* Set up ordering and ID's so we now which 2D records go with which
   * probe index
   */
  cCnt = pCnt = hCnt = '1';
  for (size_t i = 0; i < piCnt; ++i)
    {
    if ((probeIDorder[i] & 0x4300) == 0x4300)   /* 2D-C */
      probeIDorder[i] |= cCnt++;

    if ((probeIDorder[i] & 0x5000) == 0x5000)   /* 2D-P */
      probeIDorder[i] |= pCnt++;

    if ((probeIDorder[i] & 0x4800) == 0x4800)   /* HVPS */
      probeIDorder[i] |= hCnt++;

//printf("%x\n", probeIDorder[i]);
    }

}	/* END ADD2DTOLIST */

/* -------------------------------------------------------------------- */
void xlTwodInit(RAWTBL *varp)
{
  size_t	probeCnt = varp->ProbeCount;
  P2d_rec	rec;


  /* Acquire time stamps from first record, and throw data away.
   */
  do
    {
    if (Next2dRecord(&rec, probeCnt, probeIDorder[probeCnt]) == false)
      {
      startTime[probeCnt] = startMilliSec[probeCnt] = 0;
      break;
      }

    startTime[probeCnt]	= ntohs(rec.hour)*3600+ntohs(rec.minute)*60+ntohs(rec.second);
    startMilliSec[probeCnt] = ntohs(rec.msec);

//printf("  %x startTime = %d.%d\n", ntohs(rec.id), startTime[probeCnt], startMilliSec[probeCnt]);
//printf("  %02d:%02d:%02d\n", ntohs(rec.hour), ntohs(rec.minute), ntohs(rec.second));
    }
  while (startTime[probeCnt] < 0 || startTime[probeCnt] > 86400);

  if (strstr(varp->name, "HVPS") == 0)
    sTwodInit(varp);
  else
    sTwodInitH(varp);

}	/* END INITTWOD */

/* -------------------------------------------------------------------- */
void xl2dDeadTime1(RAWTBL *varp, void *in, NR_TYPE *np)
{
  size_t	probeCnt = varp->ProbeCount >> 1;

  /* Return Milliseconds.
   */
  *np = deadTime[probeCnt][0] / 1000;
//printf("dt[%d][0] = %f\n", probeCnt, deadTime[probeCnt][0]);
}	/* END XLTWOD */

/* -------------------------------------------------------------------- */
void xl2dDeadTime2(RAWTBL *varp, void *in, NR_TYPE *np)
{
  size_t	probeCnt = varp->ProbeCount >> 1;

  /* Return Milliseconds.
   */
  *np = deadTime[probeCnt][1] / 1000;
//printf("dt[%d][1] = %f\n", probeCnt, deadTime[probeCnt][1]);
}	/* END XLTWOD */

/* -------------------------------------------------------------------- */
void xlTwoD(RAWTBL *varp, void *in, NR_TYPE *np)
{
  size_t	probeCnt = varp->ProbeCount >> 1;

  for (size_t i = 0; i < varp->Length; ++i)
    np[i] = twoD[probeCnt][i];

}	/* END XLTWOD */

/* -------------------------------------------------------------------- */
void xlOneD(RAWTBL *varp, void *in, NR_TYPE *np)
{
  size_t	n, probeCount;
  int		thisTime;
  bool		rejected;
  Queue		*probe;
  Particle	*p;

  probeCount = varp->ProbeCount >> 1;

  overFlowCnt[probeCount] = 0;
  deadTime[probeCount][0] = 0.0;
  deadTime[probeCount][1] = 0.0;
  memset((void *)np, 0, NR_SIZE * varp->Length);
  memset((void *)twoD[probeCount], 0, NR_SIZE * BINS_64);

#ifdef SQL
  cur_rp = varp;
#endif

/*
np[16] = 13.25 * 49;
deadTime[probeCount][0] = 13.25 * 0.036;
deadTime[probeCount][1] = 13.25 * 0.036;
twoD[probeCount][17] = 13.25 * 49;
return;
*/
//printf("------ %02d:%02d:%02d -----------------------------------\n",
//	((short *)in)[1], ((short *)in)[2], ((short *)in)[3]);

  thisTime = ntohs(((short *)in)[1])*3600 + ntohs(((short *)in)[2])*60 + ntohs(((short *)in)[3]);

  probe = probes[probeCount];
//printf("xlTwoD entered %x, thisTime = %d\n", probeIDorder[probeCount], thisTime);

  /* Make sure we have enough processed data to carry us through this second
   */
  AddMore2dData(probe, thisTime, probeCount);


  if (FrontQueue(probe) == NULL)	/* No data left in 2d file? */
{
//printf(" Queue is empty, leaving.\n");
    return;
}

  /* If first time available in Q is later, bail out
   */
  if (thisTime < ((Particle *)FrontQueue(probe))->time)
{
//printf(" Front Q has later time, leaving.  this=%d, Q=%d\n", thisTime, ((Particle *)FrontQueue(probe))->time);
    return;
}


  /* Skip prior particles.  In theory this shouldn't happen.
   */
  while (FrontQueue(probe) && ((Particle *)FrontQueue(probe))->time < thisTime)
    delete (Particle *)DeQueue(probe);


  while (FrontQueue(probe) && ((Particle *)FrontQueue(probe))->time == thisTime)
    {
    p = (Particle *)DeQueue(probe);
//printf("   %d.%d w=%d, h=%d, e=%d - %d\n", ((Particle *)p)->time,
//  ((Particle *)p)->msec, ((Particle *)p)->w, ((Particle *)p)->h,
//  ((Particle *)p)->edge, ((Particle *)FrontQueue(probe))->time);

    if (p->timeWord == OVERLOAD)
      {
//printf("OVERLOAD, incr deadTime by %d\n", p->deltaTime);
      deadTime[probeCount][0] += p->deltaTime;
      deadTime[probeCount][1] += p->deltaTime;
      }
    else
      {
      rejected = FALSE;

      if (p->w > 121 ||
         (p->h < 24 && p->w > 6 * p->h) ||
         (p->h < 6 && p->w > 3 * p->h) ||
         (p->edge && (float)p->h / p->w < 0.2))
        rejected = TRUE;

      /* A2DC */
      if (!rejected && (!p->edge || (p->edge && p->w <= p->h * 2))) // Center-in
        {
        n = MAX(p->w, p->h);

        if (n < BINS_64)
          twoD[probeCount][n] += 1.0;
        else
          ++overFlowCnt[probeCount];
        }
      else
        deadTime[probeCount][1] += p->liveTime;

/*    if (1) // Reconstruction
        {
        if (!p->edge || (p->edge && p->w <= p->h * 2))
          n = MAX(p->w, p->h);
        else
        if (p->edge && p->edge != 0xffff) // One edge, but not both.
          n = (pow(p->w >> 1, 2.0) + pow(p->h, 2.0)) / p->h;
        else
        if (p->edge == 0xffff)
          n = sqrt(pow(p->h + ((pow(p->x2, 2.0) + pow(p->x1, 2.0)) / 4 * p->h),
					2.0) + pow(p->x1, 2.0));

        else
          printf("amlib/xlate/pms2_data.c: Not all cases for particles covered.\n");

        if (n < BINS_64)
          twoD[probeCount][n] += 1.0;
        else
          ++overFlowCnt[probeCount];
        }
      else
        deadTime[probeCount][1] += p->liveTime;
*/

      /* A1DC */
      if (p->h > 0 && p->h < 4 * p->w && !p->edge)
        np[p->h] += 1.0;
      else
        deadTime[probeCount][0] += p->liveTime;
      }

    delete p;
    }
//printf(" leaving\n");
}	/* END XLONED */

/* -------------------------------------------------------------------- */
void xlHVPS(RAWTBL *varp, void *in, NR_TYPE *np)
{
  size_t	probeCount;
  int		thisTime;
  bool          rejected;
  Queue         *probe;
  Particle      *p;

  probeCount = varp->ProbeCount >> 1;

  overFlowCnt[probeCount] = 0;
  deadTime[probeCount][0] = 0.0;
  deadTime[probeCount][1] = 0.0;
  memset((void *)np, 0, NR_SIZE * varp->Length);
/*
printf("------ %02d:%02d:%02d - %x -------------\n",
	ntohs(((short *)in)[1]), ntohs(((short *)in)[2]),
	ntohs(((short *)in)[3]), probeIDorder[probeCount]);
*/
  thisTime = ntohs(((short *)in)[1])*3600 + ntohs(((short *)in)[2])*60 + ntohs(((short *)in)[3]);

  probe = probes[probeCount];

  /* Make sure we have enough processed data to carry us through this second
   */
  AddMore2dData(probe, thisTime, probeCount);


  if (FrontQueue(probe) == NULL)        /* No data left in 2d file? */
{
//printf(" Queue is empty, leaving.\n");
    return;
}

  /* If first time available in Q is later, bail out
   */
  if (thisTime < ((Particle *)FrontQueue(probe))->time)
{
//printf(" Front Q has later time, leaving.  this=%d, Q=%d\n", thisTime, ((Particle *)FrontQueue(probe))->time);
    return;
}


  /* Skip prior particles.  In theory this shouldn't happen.
   */
  while (FrontQueue(probe) && ((Particle *)FrontQueue(probe))->time < thisTime)
    delete (Particle *)DeQueue(probe);


  while (FrontQueue(probe) && ((Particle *)FrontQueue(probe))->time == thisTime)
    {
    p = (Particle *)DeQueue(probe);
//printf("   %d.%d w=%d, h=%d, e=%d - %d\n", ((Particle *)p)->time,
//  ((Particle *)p)->msec, ((Particle *)p)->w, ((Particle *)p)->h,
//  ((Particle *)p)->edge, ((Particle *)FrontQueue(probe))->time);

    if (p->timeWord == OVERLOAD)
      {
//printf("OVERLOAD, incr deadTime by %d\n", p->deltaTime);
      deadTime[probeCount][0] += p->deltaTime;
      deadTime[probeCount][1] += p->deltaTime;
      }
    else
      {
      rejected = FALSE;
/*
      if (p->w > 121 ||
         (p->h < 24 && p->w > 6 * p->h) ||
         (p->h < 6 && p->w > 3 * p->h) ||
         (p->edge && (float)p->h / p->w < 0.2))
        rejected = TRUE;
*/
      /* MAX Y vs. Y */
      if (p->h > 0 && p->h < 4 * p->w && !p->edge)
        np[p->h] += 1.0;
//        np[MAX(p->h, p->w)] += 1.0;
      else
        deadTime[probeCount][0] += p->liveTime;
      }

    delete p;
    }

//printf(" leaving %d %d\n", partCnt1, partCnt2);

}       /* END XLHVPS */

/* -------------------------------------------------------------------- */
static void AddMore2dData(Queue *probe, long thisTime, int probeCnt)
{
  P2d_rec	rec;
  bool		debug = false;

//  if (probeIDorder[probeCnt] == 0x4831)
//    debug = true;

  /* No more data? */
  while (probe->tail == NULL)
    {
    if (debug) printf("mo data\n");
    if (Next2dRecord(&rec, probeCnt, probeIDorder[probeCnt]) == FALSE)
      {
      if (debug)
        printf("AddMore2dData: returning empty handed, no more data\n");
      return;
      }
    else
      Process(probe, &rec, probeCnt);
    }


  /* Keep adding records until we have all for this second.  We'll need to
   * get at the tail to make sure last particle is greater than current time.
   */
  while (((Particle *)probe->tail->datum)->time <= thisTime &&
		Next2dRecord(&rec, probeCnt, probeIDorder[probeCnt]))
    {
    if (debug)
      printf("  %x thisTime=%d, newRec=%d tail=%d, ol=%d\n", rec.id, thisTime, rec.hour*3600+rec.minute*60+rec.second, ((Particle *)probe->tail->datum)->time, rec.overld);

    Process(probe, &rec, probeCnt);

    if (debug)
      printf("    %02d:%02d:%02d  tail=%d\n", rec.hour, rec.minute, rec.second, ((Particle *)probe->tail->datum)->time);
    }

}	/* END ADDMORE2DDATA */

/* -------------------------------------------------------------------- */
void Process(Queue *probe, P2d_rec *rec, int probeCnt)
{
  size_t	i, j, h, partCnt;
  long		endTime, oload;
  bool		firstParticleAfter512;
  ulong		*p, slice, pSlice, ppSlice, tBarElapsedtime, DASelapsedTime;
  NR_TYPE	tas, frequency, overLap;
  ushort	t;
  Particle	*part[512], *cp; /* cp stands for "currentParticle" */

  static int	overLoad = 0;

  /* If it's an HVPS record, process elsewhere, and return.
   */
  if (((char *)&rec->id)[0] == 'H')
    {
    ProcessHVPS(probe, rec, probeCnt);
    return;
    }


  /* Perform byte swapping on whole [data] record if required.
   */
  if (1 != ntohs(1))
    {
    short       *sp = (short *)rec;

    for (i = 1; i < 10; ++i, ++sp)
      *sp = ntohs(*sp);


    p = (ulong *)rec->data;

    for (i = 0; i < 1024; ++i, ++p)
      *p = ntohl(*p);
    }


  endTime = rec->hour * 3600 + rec->minute * 60 + rec->second;

  tas = (NR_TYPE)rec->tas * 125 / 255;

  /* Compute frequency, which is used to convert timing words from TAS clock
   * pulses to microseconds.
   */
  if (((char *)&rec->id)[0] == 'C')
    frequency = 25 / tas;
  else
    frequency = 200 / tas;


  DASelapsedTime = (((endTime * 1000) + rec->msec) -
	((startTime[probeCnt] * 1000) + startMilliSec[probeCnt])) * 1000;

  if (DASelapsedTime == 0)	/* Duplicate record */
    return;


  p = (ulong *)rec->data;
  partCnt = 0;
  tBarElapsedtime = 1024;
  firstParticleAfter512 = TRUE;

  /* Locate each particle and characteristics (e.g. h & w, touched edge).
   */
  for (i = 0; i < 1024; ++i, ++p)
    {
    if ((slice = *p) == 0xffffffff)
      --tBarElapsedtime;

    /* Ok, found start of particle.
     */
//    if (slice == 0xff000000 && ppSlice == 0xffffffff && pSlice != 0xffffffff)
    if (slice == 0x55000000 && ppSlice == 0xffffffff && pSlice != 0xffffffff)
      {
      if (i > 512 && firstParticleAfter512 && overLoad > 0)
        {
        /* Overload is treated as a particle.
         */
        cp = part[partCnt++] = new Particle;

        cp->time = startTime[probeCnt];
        cp->msec = startMilliSec[probeCnt];
        cp->deltaTime = (NR_TYPE)overLoad * 1000; /* microseconds */
        cp->timeWord = OVERLOAD;
        cp->w = 0;
        cp->h = 0;

        firstParticleAfter512 = FALSE;
        }

      cp = part[partCnt++] = new Particle;
      cp->time = startTime[probeCnt];
      cp->msec = startMilliSec[probeCnt];
      cp->timeWord = pSlice & 0x00ffffff;
      cp->deltaTime = (NR_TYPE)cp->timeWord * frequency;
      cp->w = 1;	/* first slice of particle is in sync word */
      cp->h = 1;
      cp->edge = FALSE;
      cp->x1 = 0;
      cp->x2 = 0;
      cp->p = p;
      cp->nSlices = 0;

      if ((ulong)cp->deltaTime < DASelapsedTime)
        tBarElapsedtime += cp->timeWord;

      /* Determine height of particle.
       */
      ++p; ++i; ++cp->nSlices;
      for (; i < 1024 && *p != 0xffffffff; ++p, ++i)
        {
        ++cp->w;

        slice = ~(*p);

        /* Potential problem/bug with computing of x1, x2.  Works good if all
         * edge touches are contigious (water), not so good for snow, where
         * it will all get bunched up.  Counts total number of contacts for
         * each edge.
         */
        if (slice & 0x80000000)	/* touched edge */
          {
          cp->edge |= 0xF0;
          cp->x1++;
          }

        if (slice & 0x00000001) /* touched edge */
          {
          cp->edge |= 0x0F;
          cp->x2++;
          }

        h = 32;
        for (	j = 0, slice = *p;
		j < 32 && (slice & 0x80000000); slice <<= 1, ++j)
          --h;
        for (	j = 0, slice = *p;
		j < 32 && (slice & 0x00000001); slice >>= 1, ++j)
          --h;

        cp->h = MAX(cp->h, h);
        }

      /* If the particle becomes rejected later, we need to now much time the
       * particle consumed, so we can add it to the deadTime, so sampleVolume
       * can be reduced accordingly.
       */
      cp->liveTime = (NR_TYPE)((cp->w + 3) * frequency);

      t = MAX(cp->x1, cp->x2);
      cp->x1 = MIN(cp->x1, cp->x2);
      cp->x2 = t;
      cp->nSlices = p - cp->p;
      }

    ppSlice = p[-1];
    pSlice = p[0];
    }


  /* Determine number milliseconds to add/subtract to each particle.
   */
  tBarElapsedtime = (int)((float)tBarElapsedtime * frequency);	/* Convert to microseconds */
  tBarElapsedtime += overLoad * 1000;
  overLap = (NR_TYPE)tBarElapsedtime / DASelapsedTime;


//printf("DASeTime = %d, tBarEtime=%d, %f\n", DASelapsedTime, tBarElapsedtime, overLap);


  /* Determine whether to reject this record.
   *  - too few particles, indicates something wrong with probe/datasystem.
   *  - too much DAS elaplsed time, means timing words are rolling over and
   *    we can't accuratly compute time for those particles.
   *  - if timeBar total is way out of line with DAS elapsed time.
   */
  if (partCnt < 20)
    {
    sprintf(buffer, "Rejecting 2D record: %x %02d:%02d:%02d.%d, too few particles, nPart=%d", rec->id, rec->hour, rec->minute, rec->second, rec->msec, partCnt);
    Log2dXlateMsg(rec, buffer);
    goto cleanup;
    }

  if (DASelapsedTime > 8000000)	/* 8 seconds */
    {
    sprintf(buffer, "Rejecting 2D record: %x %02d:%02d:%02d.%d, too much elapsed time, %ld seconds.", rec->id, rec->hour, rec->minute, rec->second, rec->msec, DASelapsedTime / 1000000);
    Log2dXlateMsg(rec, buffer);
    goto cleanup;
    }

  if (overLap > 2 && tas < 70.0)
    {
    sprintf(buffer, "Rejecting 2D record: %x %02d:%02d:%02d.%d, oLap=%f, tas=%5.1f", rec->id, rec->hour, rec->minute, rec->second, rec->msec, overLap, tas);
    Log2dXlateMsg(rec, buffer);
    goto cleanup;
    }



  /* Go back through particles and add in the timing word to get accurate
   * time for each particle.
   */
  tBarElapsedtime = 0;
  oload = 0;
  overLap = 1.0 / overLap;

#ifdef SQL
  if (Mode == REALTIME && partCnt > 0)
  {
    psql->Start2dSQL();
//    SQLcommand("BEGIN");
  }

//  if (partCnt > 100)
//    fprintf(stderr, "2d particle cnt for 1 second = %d\n", partCnt);
#endif

  for (i = 0; i < partCnt; ++i)
    {
    cp = part[i];

    if (cp->deltaTime < DASelapsedTime)
      {
      if (cp->timeWord == OVERLOAD)
        oload = (int)(cp->deltaTime / 1000);
      else
        tBarElapsedtime += cp->timeWord;
      }

    cp->msec = startMilliSec[probeCnt] +
	(long)((((float)tBarElapsedtime * frequency / 1000) + oload) * overLap);

    while (cp->msec < 0)
      {
      cp->time--;
      cp->msec += 1000;
      }

    while (cp->msec > 999)
      {
      cp->time++;
      cp->msec -= 1000;
      }

    /* Add in number of slices in particle, since timing word is in reset
     * state while diode array is shadowed.
     */
    tBarElapsedtime += cp->w + 1;

    EnQueue(probe, cp);
/*
printf("----------\n");
for (i = 0; i < cp->nSlices+1; ++i)
  printf("%x\n", cp->p[i]);
*/
#ifdef SQL
    if (Mode == REALTIME)
      psql->Write2dSQL(cur_rp, cp->time, cp->msec, cp->p, cp->w);
#endif
    }

#ifdef SQL
  if (Mode == REALTIME && partCnt > 0)
  {
    psql->Submit2dSQL();
  }
#endif

cleanup:
  startTime[probeCnt] = endTime;
  startMilliSec[probeCnt] = rec->msec;

  /* Overload should rarely exceed 60 milliseconds for 1 probe, 200 ms for 2 probes
   * on one interface card.  The new 2d interface card starting in 1996 (Sheba) has
   * some bugs regarding this.
   */
  overLoad = rec->overld;
  if (overLoad == 1000)
    overLoad = 0;
  else
  if (overLoad > 200)
    overLoad = 50;


}	/* END PROCESS */

/* -------------------------------------------------------------------- */
static const float	TAS_COMPENSATE = 2.5;
static const size_t	lower_mask = 215, upper_mask = 40;

void ProcessHVPS(Queue *probe, P2d_rec *rec, int probeCnt)
{
  size_t	i, partCnt, shaded, unshaded;
  long          endTime, oload;
  bool          firstParticleAfter512;
  ulong		tBarElapsedtime, DASelapsedTime;
  ushort	*p, slice, pSlice, ppSlice;
  NR_TYPE       tas, frequency, overLap;
  ushort        t;
  Particle      *part[1024], *cp; /* cp stands for "currentParticle" */

  static int    overLoad = 0;

  /* Perform byte swapping on whole [data] record if required.
   */
  if (1 != ntohs(1))
    {
    short       *sp = (short *)rec;

    for (i = 1; i < 10; ++i, ++sp)
      *sp = ntohs(*sp);


    p = (ushort *)rec->data;

    for (i = 0; i < 2048; ++i, ++p)
      *p = ntohs(*p);
    }

  if (p[0] == 0xcaaa)		/* Diagnostic record	*/
    return;

  if (rec->hour > 23 || rec->hour < 0 ||
      rec->minute > 59 || rec->minute < 0 ||
      rec->second > 59 || rec->second < 0)
    return;

//printf("ProcessHVPS: %02d:%02d:%02d\n", rec->hour, rec->minute, rec->second);
  endTime	= rec->hour * 3600 + rec->minute * 60 + rec->second;
  tas		= (NR_TYPE)rec->tas * 125 / 255;
  frequency	= 200 / tas;

  DASelapsedTime = (((endTime * 1000) + rec->msec) -
        ((startTime[probeCnt] * 1000) + startMilliSec[probeCnt])) * 1000;

  if (DASelapsedTime == 0)      /* Duplicate record	*/
    return;


  p = (ushort *)rec->data;
  partCnt = 0;
  tBarElapsedtime = 0;
  firstParticleAfter512 = TRUE;


  /* Locate each particle and characteristics (e.g. h & w, touched edge).
   */
  for (i = 0; i < 2048; ++i, ++p)
    {
    slice = *p;

    /* Ok, found start of particle.
     */
    if (slice & 0x8000)
      {
      /* Reject particle if there are not two timing words.  Usually will happen
       * when start of particle crosses record boundaries.
       */
      if ((p[1] & 0x8000) != 0x8000)
        continue;

      if (i > 512 && firstParticleAfter512 && overLoad > 0)
        {
        /* Overload is treated as a particle.
         */
        cp = part[partCnt++] = new Particle;

        cp->time = startTime[probeCnt];
        cp->msec = startMilliSec[probeCnt];
        cp->deltaTime = (NR_TYPE)overLoad * 1000; /* microseconds */
        cp->timeWord = OVERLOAD;
        cp->w = 0;
        cp->h = 0;

        firstParticleAfter512 = FALSE;
        }

      cp = part[partCnt++] = new Particle;
      cp->time = startTime[probeCnt];
      cp->msec = startMilliSec[probeCnt];
      cp->timeWord = (((ulong)p[0] << 14) & 0x0fffc000);
      cp->timeWord += (ulong)(p[1] & 0x3fff);
      cp->deltaTime = (NR_TYPE)cp->timeWord * frequency;
      cp->w = 0;
      cp->h = 0;
      cp->edge = FALSE;
      cp->x1 = 0;
      cp->x2 = 0;

      if ((ulong)cp->deltaTime < DASelapsedTime)
        tBarElapsedtime += cp->timeWord;
//printf("Start particle = %04x %04x\n", slice, p[1]);
      /* Determine height of particle.
       */
      p += 2; i += 2;
      for (; i < 2048 && (*p & 0x8000) != 0x8000; ++p, ++i)
        {
        ++cp->w;
        slice = *p;

        unshaded = shaded = 0;

        if (slice == 0x4000)    // Starts in lower half of diode array.
          {
          ++p; ++i;
          slice = *p;

          unshaded = 128;
          }

        shaded = (slice & 0x3f80) >> 7;
        unshaded += slice & 0x007f;

//printf("  slice = %04x - %d %d\n", slice, unshaded, shaded);
        /* Loop through any additional words that make up this slice - e.g.
         * multiple particles in one array.
         */
        while ((p[1] & 0xc000) == 0)
          {
          ++p; ++i;
          slice = *p;

          shaded += (slice & 0x3f80) >> 7;
          shaded += slice & 0x007f;
//printf("        = %04x - %d %d\n", slice, unshaded, shaded);
          }


        /* Potential problem/bug with computing of x1, x2.  Works good if all
         * edge touches are contigious (water), not so good for snow, where
         * it will all get bunched up.  Counts total number of contacts for
         * each edge.
         */
//if (unshaded < 40 || unshaded+shaded > 216)
//  printf("%02d:%02d:%02d.%d - %d - %d %d - %04x\n", rec->hour, rec->minute, rec->second, rec->msec, partCnt, unshaded, shaded, slice);
        if (unshaded <= upper_mask) /* touched edge */
          {
          cp->edge |= 0xF0;
          cp->x1++;
          }

        if (unshaded + shaded >= lower_mask) /* touched lower edge (assuming 40 masked out) */
          {
          cp->edge |= 0x0F;
          cp->x2++;
          }

        cp->h = MAX(cp->h, shaded);
        }

      --p; --i;

      /* If the particle becomes rejected later, we need to now much time the
       * particle consumed, so we can add it to the deadTime, so sampleVolume
       * can be reduced accordingly.
       */
      cp->liveTime = (ulong)((float)(cp->w + 1) * frequency);
      cp->w = (int)((float)cp->w * TAS_COMPENSATE);

      t = MAX(cp->x1, cp->x2);
      cp->x1 = MIN(cp->x1, cp->x2);
      cp->x2 = t;
      }

    ppSlice = p[-1];
    pSlice = p[0];
    }

  /* Determine number milliseconds to add/subtract to each particle.
   */
  tBarElapsedtime = (int)((float)tBarElapsedtime * frequency); /* Convert to microseconds */
  tBarElapsedtime += overLoad * 1000;
  overLap = (NR_TYPE)tBarElapsedtime / DASelapsedTime;


  /* Go back through particles and add in the timing word to get accurate
   * time for each particle.
   */
  tBarElapsedtime = 0;
  oload = 0;
  overLap = 1.0 / overLap;

  for (i = 0; i < partCnt; ++i)
    {
    cp = part[i];
                                                                                
    if (cp->deltaTime < DASelapsedTime)
      {
      if (cp->timeWord == OVERLOAD)
        oload = (int)(cp->deltaTime / 1000);
      else
        tBarElapsedtime += cp->timeWord;
      }

    cp->msec = startMilliSec[probeCnt] +
	(long)((((float)tBarElapsedtime * frequency / 1000) + oload) * overLap);

    while (cp->msec < 0)
      {
      cp->time--;
      cp->msec += 1000;
      }

    while (cp->msec > 999)
      {
      cp->time++;
      cp->msec -= 1000;
      }

    /* Add in number of slices in particle, since timing word is in reset
     * state while diode array is shadowed.  NOT CONFIRMED FOR HVPS!
     */
    tBarElapsedtime += cp->w + 1;

    EnQueue(probe, cp);
    }

  startTime[probeCnt]	= endTime;
  startMilliSec[probeCnt] = rec->msec;
  overLoad		= rec->overld;

}	/* END PROCESSHVPS */

/* END PMS2_DATA.C */
