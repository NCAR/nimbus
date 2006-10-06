/*
-------------------------------------------------------------------------
OBJECT NAME:	adsIO.c

FULL NAME:	ADS Record IO routines

ENTRY POINTS:	ExtractHeaderIntoFile(char *ADSfileName)
		FindFirstLogicalADS2(char *record, long starttime)
		FindNextLogicalADS2(char *record, long endtime)
		FindNextDataRecord(char *record)
		CloseADSfile()
		Open2dFile()
		Next2dRecord()

STATIC FNS:	GetNextADSfile()
		GetNext2Dfile()
		check_rico_half_buff()

DESCRIPTION:	These routines locate data records that start with the
		ID = 0x8681.  (i.e. skips all PMS2D records).

INPUT:		Pointer to where to place the record
		Time of first record desired / Time last record desired

OUTPUT:		Length of record or ERR

REFERENCES:	tapeIO.c

REFERENCED BY:	lrloop.c, hrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <cctype>
#include <fcntl.h>
#include <unistd.h>

#include "nimbus.h"
#include "decode.h"
#include "ctape.h"
#include "raf.h"
#include "portable.h"

/* Values for DiskData, 1st one matches false.	*/
enum FileType { TAPE_DATA, RAW_ADS, COS_BLOCKED };

/* ADS image record types (sans pms2d).	*/
#define ADS_WORD	0x4144
#define HDR_WORD	0x5448
#define SDI_WORD	SDIWRD
#define MCR_WORD	0x4d43

#define FIRST_DATA_RECORD	((long)3)


static FileType	DiskData = RAW_ADS;
static char	phys_rec[MX_PHYS] = "";
static char	*adsFileName;
static long	lrlen, lrppr, currentLR;
static int	infd;

extern size_t	LITTON51_start;

static long	FindNextDataRecord(char buff[]);
static bool	IsThisAnAsyncRecord(short buff[]);
static void	check_rico_half_buff(P2d_rec *buff, size_t beg, size_t end);
static int	GetNext2Dfile();

char	*ExtractHeaderIntoFile(char *);
void	WriteAsyncData(char record[]);

#ifdef __cplusplus
extern "C" {
#endif
int crayopen(char *flnm, int *rwflags, int *mode, int plen);
int crayread(int *index, void *buf, int *size, int *conv);
int crayclose(int *index);
#ifdef __cplusplus
}
#endif


/* -------------------------------------------------------------------- */
long FindFirstLogicalADS2(
	char	record[],	/* First Data Record, for start time	*/
	long	startTime)	/* User specified start time		*/
{
  long		recTime, nbytes;
  static int	firstTime = true;

  if (firstTime)
    {
    int		rc;

//    firstTime = false;

    get_lrlen(&lrlen);
    get_lrppr(&lrppr);

    switch (DiskData)
      {
      case RAW_ADS:	rc = lseek(infd, 0L, SEEK_SET); break;
      case COS_BLOCKED:	rc = 0; break;
      default:		rc = TapeSeek(FIRST_DATA_RECORD);
      }

//rc = lseek(infd, 133874376, SEEK_SET);
    if (rc == ERR)
      return(ERR);

    if ((nbytes = FindNextDataRecord(phys_rec)) <= 0)
      return(nbytes);

    if (startTime == BEG_OF_TAPE)
      return(FindNextLogicalADS2(record, startTime));
    }
  else
    currentLR = 0;


  recTime = (long)HdrBlkTimeToSeconds((struct Hdr_blk  *)phys_rec);

  /*	     12:00:00		   23:59:59	*/
  if (recTime < 43200L && startTime > 86399L)
    recTime += 86399L;

  while (startTime > (recTime + lrppr))
    {
    if ((nbytes = FindNextDataRecord(phys_rec)) <= 0)
      return(nbytes);

    recTime = HdrBlkTimeToSeconds((struct Hdr_blk  *)phys_rec);

    /*	     12:00:00		   23:59:59	*/
    if (recTime < 43200L && startTime > 86399L)
      recTime += 86399L;

    FlushXEvents();
    }


  /* Cover the case if start time is before first record on file
   */
  if (startTime < recTime)
    startTime = recTime;

  while ((nbytes = FindNextLogicalADS2(record, startTime)) > 0)
    {
    recTime = HdrBlkTimeToSeconds((struct Hdr_blk *)record);

    /*	     12:00:00		   23:59:59	*/
    if (recTime < 43200L && startTime > 86399L)
      recTime += 86399L;

    if (recTime >= startTime)
      break;
    }

  return(nbytes);

}	/* END FINDFIRSTLOGICALRECORD */

/* -------------------------------------------------------------------- */
long FindNextLogicalADS2(char record[], long endtime)
{
  int	nbytes;
  long	TansStart, rectime;
  Hdr_blk *ADShdr = (Hdr_blk *)record;

  rectime = HdrBlkTimeToSeconds(&phys_rec[currentLR * lrlen]);

  if (endtime != END_OF_TAPE)
    {
    /*	     12:00:00		 23:59:59	*/
    if (rectime < 43200L && endtime > 86399L)
      rectime += 86399L;

    if (rectime > endtime)
      return(0);	/* End Of Time Segment	*/
    }

  memcpy(record, &phys_rec[currentLR * lrlen], lrlen);

  if (++currentLR >= lrppr)
    {
    if ((nbytes = FindNextDataRecord(phys_rec)) <= 0)
      return(nbytes);

    currentLR = 0;
    }


  /* Indoex had some munged records, so let's remove trashed stuff. */
  if (endtime == END_OF_TAPE && (rectime < 0 || rectime > 86399L || ntohs((ushort)((Hdr_blk*)record)->id) != SDI_WORD))
    {

    sprintf(buffer, "Record found id = %x @ %02d:%02d:%02d, throwing away.\n",
		ntohs(ADShdr->id), ntohs(ADShdr->hour),
		ntohs(ADShdr->minute), ntohs(ADShdr->second));
    LogMessage(buffer);
    FindNextLogicalADS2(record, endtime);
    }


  if (cfg.ProcessingMode() == Config::RealTime)
    return(lrlen);

  /* Lag the Litton 51 INS one second.
   */
  if (cfg.InertialShift() && LITTON51_present)
    memcpy( &record[LITTON51_start],
            &phys_rec[currentLR * lrlen + LITTON51_start],
            sizeof(struct Ins_blk));


  /* Lag GPS position & velocity, only if off by 1 second.
   */
  if (cfg.InertialShift() && GetStart(GPS_TANS3_STR, &TansStart) != ERR)
    {
    Gps_blk	*gp_src = (Gps_blk *)&phys_rec[currentLR * lrlen + TansStart],
                *gp_dst = (Gps_blk *)&record[TansStart];

    if ((int)ntohf(gp_dst->gpstime) == (int)ntohf(gp_dst->postime) + 1)
      {
      gp_dst->glat = gp_src->glat;
      gp_dst->glon = gp_src->glon;
      gp_dst->galt = gp_src->galt;
      gp_dst->postime = gp_src->postime;
      }

    if ((int)ntohf(gp_dst->gpstime) == (int)ntohf(gp_dst->veltime) + 1)
      {
      gp_dst->veleast = gp_src->veleast;
      gp_dst->velnrth = gp_src->velnrth;
      gp_dst->velup = gp_src->velup;
      gp_dst->veltime = gp_src->veltime;
      }
    }

  if (cfg.InertialShift() && GetStart(GPS_GARMIN_STR, &TansStart) != ERR)
    {
    Garmin_blk
	*gp_src = (Garmin_blk *)&phys_rec[currentLR * lrlen + TansStart],
	*gp_dst = (Garmin_blk *)&record[TansStart];

    gp_dst->ground_speed = gp_src->ground_speed;
    gp_dst->course = gp_src->course;
    }

  if (cfg.InertialShift() && GetStart(CMIGITS3_STR, &TansStart) != ERR)
    {
    Cmigits3_blk
	*gp_src = (Cmigits3_blk *)&phys_rec[currentLR * lrlen + TansStart],
	*gp_dst = (Cmigits3_blk *)&record[TansStart];

    memcpy(gp_dst, gp_src, sizeof(Cmigits3_blk));
    }

  return(lrlen);

}	/* END FINDNEXTLOGICALRECORD */

/* -------------------------------------------------------------------- */
char *ExtractHeaderIntoFile(const char fileName[])
{
  int	nBytes, rc, outFD;
  int	iflag = 0, mode = 0, nWords = 4096, iconv = 0;

  static char	tmpFile[MAXPATHLEN];

  adsFileName = (char *)fileName;

  strcpy(tmpFile, "/tmp/nimbusXXXXXX");
  outFD = mkstemp(tmpFile);

  if (strncmp(adsFileName, "/dev/rmt/", 9) == 0)
    {
    DiskData = TAPE_DATA;
    TapeOpen(adsFileName);

    nBytes = TapeRead(phys_rec);	/* Skip "ADS_DATA_TAPE" record	*/
    nBytes = TapeRead(phys_rec);
    write(outFD, phys_rec, nBytes);
    close(outFD);
    return(tmpFile);
    }

  if ((infd = open(adsFileName, O_RDONLY)) < 0)
    {
    sprintf(buffer, "adsIO: Failure opening input file %s.\n", adsFileName);
    perror(buffer);
    exit(1);
    }

  if (lseek(infd, 0, SEEK_END) < 200) // 200 is kinda random.
    {
    fprintf(stderr, "adsIO: File %s is empty???.\n", adsFileName);
    exit(1);
    }

  close(infd);

  if ((infd = crayopen(adsFileName, &iflag, &mode, strlen(adsFileName))) < 0)
    DiskData = RAW_ADS;
  else
    {
    DiskData = COS_BLOCKED;

    if ((rc = crayread(&infd, phys_rec, &nWords, &iconv)) < 0)
      {
      crayclose(&infd);
      DiskData = RAW_ADS;
      }
    else
      {
      if ((rc = crayread(&infd, phys_rec, &nWords, &iconv)) < 0) {
        fprintf(stderr, "adsIO: crayread: read error %d\n", rc);
        exit(1);
        }

      /* 8 is cray word size, see man page before adjusting.	*/
      write(outFD, phys_rec, (rc * 8) + 8);
      crayread(&infd, phys_rec, &nWords, &iconv);
      }
    }


  if (DiskData == RAW_ADS)
    {
    Fl	fi;

    if ((infd = open(adsFileName, O_RDONLY)) < 0)
      {
      sprintf(buffer, "Failure opening input file %s.\n", adsFileName);
      perror(buffer);
      exit(1);
      }

    read(infd, phys_rec, 200);

    if (strncmp(FIRST_REC_STRING, (char *)phys_rec, strlen(FIRST_REC_STRING)) != 0)
      {
      memcpy((char *)&fi, phys_rec, sizeof(Fl));
      lseek(infd, 0L, SEEK_SET);
      }
    else
      {
      memcpy((char *)&fi, &phys_rec[20], sizeof(Fl));
      lseek(infd, 20L, SEEK_SET);
      }

    nBytes = ntohl(fi.thdrlen);

    read(infd, phys_rec, nBytes);
    write(outFD, phys_rec, nBytes);
    }

  if (DiskData == COS_BLOCKED)
    printf("adsIO: File is COS blocked.\n");

  close(outFD);
  return(tmpFile);

}	/* END EXTRACTHEADERINTOFILE */

/* -------------------------------------------------------------------- */
int CloseADSfile()
{
  switch (DiskData)
    {
    case TAPE_DATA:
      TapeClose();
      return(0);

    case RAW_ADS:
      return(close(infd));

    case COS_BLOCKED:
      return(crayclose(&infd));
    }

  return(ERR);

}	/* END CLOSEADSFILE */

/* -------------------------------------------------------------------- */
static int GetNextADSfile()
{
  char    *dot = strchr(adsFileName, '.');
  int	iflag = 0, mode = 0;

  static char seq;

  if (DiskData == TAPE_DATA || dot == NULL || islower(dot[-1]) == false)
    return(false);

  seq = dot[-1];
  dot[-1] = ++seq;

  if (access(adsFileName, R_OK) == ERR)
    return(false);

  if (DiskData == RAW_ADS)
    close(infd);
  else
    crayclose(&infd);

  if (DiskData == RAW_ADS && (infd = open(adsFileName, O_RDONLY)) == ERR)
    {
    LogMessage("Can't open next file in sequence.\n");
    return(false);
    }

  if (DiskData == COS_BLOCKED && (infd = crayopen(adsFileName, &iflag, &mode,
      strlen(adsFileName))) < 0)
    {
    LogMessage("Can't open next file in sequence.\n");
    return(false);
    }

  fprintf(stderr, "Switching ADS file to %s.\n", adsFileName);
  GetNext2Dfile();
  return(true);

}	/* END GETNEXTADSFILE */

/* -------------------------------------------------------------------- */
static long FindNextDataRecord(char buff[])
{
  long	nbytes = 0;

  do
    {
    if (DiskData == RAW_ADS)
      {
      long	len;
      int	size = ONE_WORD;
//Hdr_blk *p1 = (Hdr_blk *)buff;

      if ((nbytes = read(infd, buff, size)) == 0 && GetNextADSfile())
        nbytes = read(infd, buff, size);

      if (nbytes <= 0)
        break;

      switch (ntohs(*((ushort *)buff)))
        {
        case SDI_WORD:
          size = lrppr * lrlen - ONE_WORD;
          break;

        case ADS_WORD:
          size = 18;
          break;

        case HDR_WORD:
          get_thdrlen(&len);
          size = len - ONE_WORD;
          break;

        case PMS2DC1: case PMS2DC2: /* PMS2D */
        case PMS2DP1: case PMS2DP2:
        case PMS2DH1: case PMS2DH2:	/* HVPS */
          size = PMS2_RECSIZE - ONE_WORD;
          break;

        case PMS2DG1: case PMS2DG2: /* GrayScale */
          size = 32000 - ONE_WORD;
          break;

        case MCR_WORD:
          size = sizeof(Mcr_rec) - ONE_WORD;
          break;

        default:
          size = 0;
        }

      if ((nbytes += read(infd, &buff[ONE_WORD], size)) != size + 2)
        {
        /* This happens typically when the data system goes down.  The last
         * record in the file is incomplete.  So fake that it didn't exist
         * so we go back through the loop and open the next data file.
         */
        nbytes = 2;
        memset(buff, 0, 24);
        lseek(infd, 0, SEEK_END);
        }
      }
    else
    if (DiskData == TAPE_DATA)
      {
      nbytes = TapeRead(buff);
      }
    else
    if (DiskData == COS_BLOCKED)
      {
      int	nWords = 4096, iconv = 0;

      if ((nbytes = crayread(&infd, buff, &nWords, &iconv)) < 0 &&
							GetNextADSfile())
        nbytes =  crayread(&infd, buff, &nWords, &iconv);
      }

    if (cfg.AsyncFileEnabled() && IsThisAnAsyncRecord((short *)buff))
      WriteAsyncData(buff);
    }
  while (nbytes > 0 && ntohs(*((ushort *)buff)) != SDI_WORD);

  currentLR = 0;
  return(nbytes);

}	/* END FINDNEXTDATARECORD */

/* -------------------------------------------------------------------- */
static bool IsThisAnAsyncRecord(short buff[])
{
  if (buff[0] == PMS2DC1 || buff[0] == PMS2DC2 ||
      buff[0] == PMS2DP1 || buff[0] == PMS2DP2 ||
      buff[0] == PMS2DG1 || buff[0] == PMS2DG2 ||
      buff[0] == PMS2DH1 || buff[0] == PMS2DH2)
    return(true);
  else
    return(false);

}	/* END ISTHISANASYNCRECORD */


/* PMS 2D file stuff.  Have a file descriptor for each 2D probe in the
 * header.  This is done to aleviate inter-mixed times.
 */

static int	twoDfd[] = { -1, -1, -1, -1, -1, -1 };
static char	twoDfile[1024];

static const unsigned long StandardSyncWord = 0x55000000;
static const unsigned long SyncWordMask = 0xff000000;
static const size_t RecordLen = 1024;

/* -------------------------------------------------------------------- */
bool Open2dFile(const char file[], int probeCnt)
{
  char	*p;

  strcpy(twoDfile, file);
  p = strstr(twoDfile, ".ads");
  strcpy(p, ".2d");

  printf("adsIO:Open2dFile: %s ....", twoDfile);

  if (access(twoDfile, R_OK) == ERR)
    {
    if (p > 0 && isalpha(p[-1]))
      --p;
    strcpy(p, ".2d");

    if (access(twoDfile, R_OK) == ERR)
      {
      printf("  Failed to locate.\n");
      return(false);
      }
    }

  if ((twoDfd[probeCnt] = open(twoDfile, O_RDONLY)) < 0)
    {
    printf("  Failed to open.\n");
    return(false);
    }

  printf("  Opened.\n");
  return(true);

}	/* OPEN2DFILE */

/* -------------------------------------------------------------------- */
static int GetNext2Dfile()
{
  int	i;
  char	*dot = strchr(twoDfile, '.');

  static char seq;

  if (dot == NULL || islower(dot[-1]) == false)
    return(false);

  seq = dot[-1];
  dot[-1] = ++seq;

  if (access(twoDfile, R_OK) == ERR)
    return(false);

  printf("adsIO:GetNext2dFile: Switching 2d file to %s\n", twoDfile);

  for (i = 0; twoDfd[i] != -1; ++i)
    {
    close(twoDfd[i]);

    if ((twoDfd[i] = open(twoDfile, O_RDONLY)) == ERR)
      {
      LogMessage("  Can't open next 2d file in sequence.\n");
      return(false);
      }
    }

  return(true);

}	/* END GETNEXT2DFILE */

/* -------------------------------------------------------------------- */
bool Next2dRecord(P2d_rec *record, int probeCnt, short id)
{
  long	len;
  int	nbytes, size;
  char	buff[32000];

  do
    {
    size = ONE_WORD;

    if ((nbytes = read(twoDfd[probeCnt], buff, size)) == 0)
      nbytes = read(twoDfd[probeCnt], buff, size);

    if (nbytes <= 0)
      break;

    switch (ntohs(*((ushort *)buff)))
      {
      case PMS2DC1: case PMS2DC2:	/* PMS2D */
      case PMS2DP1: case PMS2DP2:
      case PMS2DH1: case PMS2DH2:	/* HVPS */
        size = PMS2_SIZE - ONE_WORD;
        break;

      case ADS_WORD:
        size = 18;
        break;

      case HDR_WORD:
        get_thdrlen(&len);
        size = len - ONE_WORD;
        break;

      case SDI_WORD:
        size = lrppr * lrlen - ONE_WORD;
        break;

      case MCR_WORD:
        size = sizeof(Mcr_rec) - ONE_WORD;
        break;

      default:
        size = 0;
      }

    nbytes += read(twoDfd[probeCnt], &buff[ONE_WORD], size);
    }
  while (ntohs(*((ushort *)buff)) != id);

  if (nbytes <= 0)
    return(false);

  /* RICO stuck bit cleanup.
   */
  if (cfg.ProjectNumber() == "135")
  {
    check_rico_half_buff((P2d_rec *)buff, 0, RecordLen/2);
    check_rico_half_buff((P2d_rec *)buff, RecordLen/2, RecordLen);
  }

  memcpy((void *)record, (void *)buff, PMS2_SIZE);
  return(true);

}	/* END NEXT2DRECORD */

/* -------------------------------------------------------------------- */
static void check_rico_half_buff(P2d_rec *buff, size_t beg, size_t end)
{
  std::vector<size_t> spectra, sorted_spectra;

  for (size_t j = 0; j < 32; ++j)
    spectra.push_back(0);

  // Generate spectra.
  unsigned long *p = (unsigned long *)buff->data;
  bool firstSyncWord = beg == 0 ? false : true;

  for (size_t i = beg; i < end; ++i, ++p)
  {
    // There seemed to be lots of splatter at the start of the buffer,
    // skip until first sync word appears.
    if (!firstSyncWord)
      if ((*p & SyncWordMask) == 0x55000000)
        firstSyncWord = true;
      else
        continue;

    if ((*p & SyncWordMask) == 0x55000000 || *p == 0xffffffff)
      continue;

    unsigned long slice = ~(*p);
    for (size_t j = 0; j < 32; ++j)
      if (((slice >> j) & 0x01) == 0x01)
        ++spectra[j];
  }

  // Sort the spectra and compare the last bin to the one next too it and see
  // if there is a large descrepency.  If so, probably a bad 1/2 buffer.
  sorted_spectra = spectra;
  sort(sorted_spectra.begin(), sorted_spectra.end());

  if ((sorted_spectra[30] * 2 < sorted_spectra[31]))
  {
    int stuck_bin = -1;
    for (size_t j = 0; j < 32; ++j)
      if (spectra[j] == sorted_spectra[31])
        stuck_bin = j;

    fprintf(stderr,
	"DataFile.cc: %02d:%02d:%02d.%d - Performing stuck bit correction, bit %d, ",
	buff->hour, buff->minute, buff->second, buff->msec, stuck_bin);

    if (beg == 0)
      fprintf(stderr, "first half.\n");
    else
      fprintf(stderr, "second half.\n");

    if (stuck_bin == -1)
    {
      fprintf(stderr, "DataFile.cc:  Impossible.\n");
      exit(1);
    }

    unsigned long mask1 = 0x01 << stuck_bin;
    unsigned long mask2 = 0x07 << stuck_bin-1;
    unsigned long *p = (unsigned long *)buff->data;
    for (size_t i = beg; i < end; ++i, ++p)
    {
      if ((*p & SyncWordMask) == 0x55000000 || *p == 0xffffffff)
        continue;

      unsigned long slice = ~(*p);
      if ((slice & mask2) == mask1)
        *p = ~(slice & ~mask1);
    }
  }
}
/* END ADSIO.C */
