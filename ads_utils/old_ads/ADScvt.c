/*
-------------------------------------------------------------------------
OBJECT NAME:	ADScvt.c

FULL NAME:	ADS Convert

ENTRY POINTS:	main

STATIC FNS:	

DESCRIPTION:	Convert old HP style ADS tape to the New ADS format.

INPUT:		Old HP ADS file

OUTPUT:		New ADS file.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <fcntl.h>
#include <stdio.h>
#include "raf/header.h"
#include "oldhdr.h"


struct old_hdr	OldHdr;
struct Fl	f;
struct Blk	b[8];
struct Ins	ins;
struct Dme	dme;
struct Pms1	pms[4];
struct Sh	sdi[120];
struct Sh	hskp[] =
	{
	"HSKP",sizeof(struct Sh),0,2,1,0, "V10",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "V10R",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "TADS",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "TV10",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "FLOADS",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "FZV",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "FZVR",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "VDREF",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XIDICE",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XIU400",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XI60U",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XI28U",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XID400",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XI60D",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XI28D",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XILGEN",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XIRGEN",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "XIEXT",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "VP15D",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "V28",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "VP15A",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "VM15A",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "TCBADS",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "FCBADS",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP1A",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP2A",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP3A",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP4A",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP5A",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP6A",	"A",0,0.001223,2,{0.0,-1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "CCKPIT",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "HCPY1",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "EV1",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "HCPY2",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "EV2",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "HCPY3",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "EV3",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "HCPY4",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "EV4",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "HCPY5",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "TA2D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "TS2D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "TLSI",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP1D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP2D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP3D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP4D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP5D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP6D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0,
	"HSKP",sizeof(struct Sh),0,2,1,0, "SP7D",	"D",0,1.0,2,{0.0,1.0},1,0,"",0
	};

int	pcnt, hcnt = 50;
int	PMS1Dstart;
char	OldADSrecord[32000], NewADSrecord[32000];
char	buffer[1024];

/* -------------------------------------------------------------------- */
main(argc, argv)
int	argc;
char	*argv[];
{
  int	in, out;
  int	rc = 1;
  int	len;
  char	prevOAR[20];

  if (argc < 3)
    {
    fprintf(stderr, "Usage: hdrcvt HP_ADS_file new_ADS_file [CalCoeOverride_File]\n");
    exit(rc);
    }


  if ((in = open(argv[1], O_RDONLY)) < 0)
    {
    fprintf(stderr, "hdrcvt: can't open input file %s\n", argv[1]);
    exit(rc);
    }

  if ((out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC), 644) < 0)
    {
    fprintf(stderr, "hdrcvt: can't open output file %s\n", argv[2]);
    close(in);
    exit(rc);
    }


  read(in, &OldHdr, sizeof(struct old_hdr));
  ConvertHeader();

  if (argv[3])
    GetOverRideCalCoes(argv[3]);

  WriteNewHeader(out);


  while (1)
    {
    do
      {
      if (read(in, OldADSrecord, 2) != 2)
				goto exit;
      }
    while (((ushort *)OldADSrecord)[0] != 0x8681);

    len = OldHdr.lrlen * 2 - 2;
    if (read(in, &OldADSrecord[2], len) != len)
      {
      fprintf(stderr, "Unable to complete read.\n");
      goto exit;
      }

    if (memcmp(OldADSrecord, prevOAR, 12) == 0)
      continue;

    if (((struct Hdr_blk *)OldADSrecord)->hour < 0 ||
	((struct Hdr_blk *)OldADSrecord)->hour > 24)
      {
      printf("Bad time stamp detected, probably end of tape.\n");
      printf("Do have another tape/file? ");
      gets(buffer);

      if (toupper(buffer[0]) == 'Y')
        {
        close(in);
        printf("Enter name of new file : ");
        gets(buffer);

        if ((in = open(buffer, O_RDONLY)) < 0)
          {
          fprintf(stderr, "can't open input file %s\n", buffer);
          goto exit;
          }

        continue;
        }
      else
        {
        rc = 0;
        goto exit;
        }
      }

    ConvertLogicalRecord(OldADSrecord, NewADSrecord);
    write(out, NewADSrecord, f.lrlen);
    memcpy(prevOAR, OldADSrecord, 12);
    }


exit:
  close(in);
  close(out);
  return(rc);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
ConvertHeader()
{
  int	i, j, Start, RateCount[500], last_rate;

  Start = pcnt = last_rate = 0;
  ins.start = 0;
  dme.start = 0;

  /* Convert HP floating point cal coe's to IEEE
   */
  {
  int	n = 120;

  fphp2mc(OldHdr.cals[0], &n);
  fphp2mc(OldHdr.cals[1], &n);
  fphp2mc(OldHdr.cals[2], &n);
  }


  /* Fill in Flight header info
   */
  strcpy(f.thdr, TAPEHDR_STR);
  f.item_len = sizeof(struct Fl);
  strcpy(f.version, VERSION_STR);
  strncpy(f.prnum, OldHdr.projname, 4); f.prnum[3] = '\0';
  sprintf(f.fltnum, "%d", OldHdr.flightnum);
  sprintf(f.tpnum, "%d", OldHdr.tapenum);
  sprintf(f.date, "%02d/%02d/19%02d",	OldHdr.yymm & 0x00ff,
		(OldHdr.ddhh >> 8) & 0x00ff, (OldHdr.yymm >> 8) & 0x00ff);

  sprintf(f.time, "%d:%d:%d", OldHdr.ddhh & 0x00ff,
		(OldHdr.mmss >> 8) & 0x00ff, OldHdr.mmss & 0x00ff);

  strcpy(f.tzone, "GMT");
  strncpy(f.acraft, OldHdr.aircraft, 4);
  f.acraft[4] = '\0';

  f.n_items = 0;
  f.thdrlen = sizeof(struct Fl);
  f.lrppr = OldHdr.lrppr;


  /* Create Block Probe structs
   */
  for (i = 0; i < 8; ++i)
    {
    if (strcmp(OldHdr.blkName[i], "") == 0 ||
        strncmp(OldHdr.blkName[i], "SDI", 3) == 0)
      {
      strcpy(b[i].item_type, "");
      continue;
      }

    if (strncmp(OldHdr.blkName[i], "PMS1", 4) == 0)
      {
      PMS1Dstart = OldHdr.startWord[i];
      continue;
      }
    else
    if (strncmp(OldHdr.blkName[i], "HDR", 3) == 0)
      {
      strcpy(b[i].item_type, "HDR");
      b[i].item_len= sizeof(struct Blk);
      b[i].start	= Start;
      b[i].length	= sizeof(struct Hdr_blk);
      Start += sizeof(struct Hdr_blk);
      f.thdrlen += sizeof(struct Blk);
      }
    else
    if (strncmp(OldHdr.blkName[i], "HSKP", 4) == 0)
      {
      for (j = 0; j < hcnt; ++j)
        {
        hskp[j].start = Start;
        Start += 2;
        }

      f.n_items += hcnt - 1;
      f.thdrlen += hcnt * sizeof(struct Sh);
      }
    else
    if (strncmp(OldHdr.blkName[i], "INS", 3) == 0)
      {
      strcpy(ins.item_type, "INS");
      ins.item_len= sizeof(struct Ins);
      ins.start	= Start;
      ins.length	= sizeof(struct Ins_blk);
      ins.rate	= 1;
      Start += sizeof(struct Ins_blk);
      f.thdrlen += sizeof(struct Ins);
      }
    else
    if (strncmp(OldHdr.blkName[i], "DME", 3) == 0)
      {
      strcpy(dme.item_type, "DME");
      dme.item_len= sizeof(struct Dme);
      dme.start	= Start;
      dme.length	= sizeof(struct Dme_blk);
      dme.rate	= 1;
      Start += sizeof(struct Dme_blk);
      f.thdrlen += sizeof(struct Dme);
      }
    else
    if (strncmp(OldHdr.blkName[i], "GPS", 3) == 0)
      {
      strcpy(b[i].item_type, "GPS");
      b[i].item_len	= sizeof(struct Blk);
      b[i].start	= Start;
      b[i].length	= sizeof(struct Gps_blk);
      Start += sizeof(struct Gps_blk);
      f.thdrlen += sizeof(struct Blk);
      }
    else
    if (strncmp(OldHdr.blkName[i], "LRNC", 4) == 0)
      {
      strcpy(b[i].item_type, "LRNC");
      b[i].item_len	= sizeof(struct Blk);
      b[i].start	= Start;
      b[i].length	= sizeof(struct Lrn_blk);
      Start += sizeof(struct Lrn_blk);
      f.thdrlen += sizeof(struct Blk);
      }
    else
      {
      fprintf(stderr, "Unknown block probe, %-4.4s\n", OldHdr.blkName[i]);
      continue;
      }

    ++f.n_items;
    }


  /* Fill in data for SDI's
   */
  for (i = 0; i < 500; ++i)
    RateCount[i] = 0;

  for (i = 0; i < OldHdr.nFull; ++i)
    {
    strcpy(sdi[i].item_type, "SDI");
    sdi[i].item_len	= sizeof(struct Sh);
    sdi[i].length	= 2;
    sdi[i].rate		= OldHdr.sampleRate[i];
    sdi[i].start	= Start + RateCount[sdi[i].rate] * 2;
    sdi[i].adsaddr	= OldHdr.locate[i][0];
    strncpy(sdi[i].name, OldHdr.SDIname[i], 6); sdi[i].name[6] = '\0';

    for (j = 3; j < 6; ++j)
      if (sdi[i].name[j] == ' ')
        {
        sdi[i].name[j] = '\0';
        break;
        }


    sdi[i].type[0] = OldHdr.type[i][0];
    sdi[i].type[1] = '\0';

    if (sdi[i].type[0] == 'D')
      sdi[i].convert = 1.0;
    else
      sdi[i].convert = 0.001223;

    sdi[i].order	= 2;
    sdi[i].cof[0]	= OldHdr.cals[0][i];
    sdi[i].cof[1]	= OldHdr.cals[1][i];

    if (OldHdr.cals[2][i] != 0.0)
      {
      sdi[i].cof[2]	= OldHdr.cals[2][i];
      sdi[i].order	= 3;
      }

    ++RateCount[sdi[i].rate];
    ++f.n_items;
    f.thdrlen += sizeof(struct Sh);

    if (last_rate != sdi[i].rate)
      {
      Start += last_rate * RateCount[last_rate] * 2;
      sdi[i].start = Start;
      }

    last_rate = sdi[i].rate;
    }


  /* Compute offset field for all SDI's
   */
  for (i = 0; i < OldHdr.nFull; ++i)
    sdi[i].offset = RateCount[sdi[i].rate] * 2;


  /* Fix PSFD & PSFD2.
   */
  for (i = 0; i < OldHdr.nFull; ++i)
    if (strncmp(sdi[i].name, "PSFD", 4) == 0)
      break;

  if (i < OldHdr.nFull)
    {
    if (strncmp(sdi[i+1].name, "PSFD", 4) != 0)
      {
      fprintf(stderr, "PSFD & PSFD2 not contiguous, fatal.\n");
      exit(1);
      }

    if (strcmp(sdi[i].name, "PSFD") == 0)
      {
      short	tmp;

      strcat(sdi[i].name, "2");
      sdi[i+1].name[4] = '\0';

      tmp = OldHdr.locate[i][1];
      OldHdr.locate[i][1] = OldHdr.locate[i+1][1];
      OldHdr.locate[i+1][1] = tmp;
      }
    }


  /* Create PMS1D blocks
   */
  for (i = 0; i < 4; ++i)
    {
    if (strncmp(OldHdr.pmsName[i], " ", 1) == 0)
			break;

    strcpy(pms[pcnt].item_type, "PMS1D");
    pms[pcnt].item_len	= sizeof(struct Pms1);
    pms[pcnt].start		= Start;
    pms[pcnt].rate		= OldHdr.pmsRate[i];
    strncpy(pms[pcnt].name, OldHdr.pmsName[i], 4);
    pms[pcnt].name[4] = '\0';
    strcpy(pms[pcnt].locn, "");
		
    if (strncmp(pms[pcnt].name, "260X", 4) == 0)
      {
      Start += sizeof(struct P260_blk) * pms[pcnt].rate;
      pms[pcnt].length = sizeof(struct P260_blk);
      }
    else
      {
      Start += sizeof(struct P16_blk) * pms[pcnt].rate;
      pms[pcnt].length = sizeof(struct P16_blk);
      }

    pms[pcnt].length *= pms[pcnt].rate;
    ++pcnt;
    ++f.n_items;
    f.thdrlen += sizeof(struct Pms1);
    }

  f.lrlen = Start;

}	/* END CONVERTHEADER */

/* -------------------------------------------------------------------- */
ConvertLogicalRecord(OldRecord, NewRecord)
char	OldRecord[];
char	NewRecord[];
{
  int		i, j;

  /* Hdr_blk
   */
  memcpy(NewRecord, OldRecord, sizeof(struct Hdr_blk));

  if (((struct Hdr_blk *)NewRecord)->second == 0)
    {
    printf("0x%02X - %02d:%02d:%02d\r",
			((struct Hdr_blk *)NewRecord)->id,
			((struct Hdr_blk *)NewRecord)->hour,
			((struct Hdr_blk *)NewRecord)->minute,
			((struct Hdr_blk *)NewRecord)->second);
    fflush(stdout);
    }


  /* SDI
   */
  for (i = 0; i < OldHdr.nFull; ++i)
    for (j = 0; j < sdi[i].rate; ++j)
      {
      ((ushort *)NewRecord)[sdi[i].start/2 + j*sdi[i].offset/2] =
      ((ushort *)OldRecord)[OldHdr.locate[i][1]-1 + j*sdi[i].offset/2];
      }


  /* PMS1D
   */
  for (i = 0; i < pcnt; ++i)
    for (j = 0; j < pms[i].rate; ++j)
      memcpy(	&NewRecord[pms[i].start + (j*pms[i].length/pms[i].rate)],
		&OldRecord[(PMS1Dstart-1 +
		OldHdr.pmsStart[i] +
		(j*OldHdr.probeSize[i]/OldHdr.pmsRate[i])) * 2],
		pms[i].length / pms[i].rate);


  /* Block probes
   */
  for (i = 0; i < 8; ++i)
    {
    if (strcmp(OldHdr.blkName[i], "") == 0)
      continue;

    if (strncmp(OldHdr.blkName[i], "INS", 3) == 0)
      memcpy(	&NewRecord[ins.start],
		&OldRecord[OldHdr.startWord[i] * 2 - 2],
		sizeof(struct Ins_blk));
    else
    if (strncmp(OldHdr.blkName[i], "HSKP", 4) == 0)
      memcpy(	&NewRecord[hskp[0].start],
		&OldRecord[OldHdr.startWord[i] * 2 - 2],
		100);
    else
    if (strncmp(OldHdr.blkName[i], "DME", 3) == 0)
      {
      ((struct Dme_blk *)&NewRecord[dme.start])->freq =
		*(ushort *)&OldRecord[OldHdr.startWord[i] * 2 - 2];

      ((struct Dme_blk *)&NewRecord[dme.start])->dist =
		*(long *)&OldRecord[OldHdr.startWord[i] * 2];
      }
    else
    if (strncmp(OldHdr.blkName[i], "LRN", 3) == 0)
      memcpy(	&NewRecord[b[i].start],
		&OldRecord[OldHdr.startWord[i] * 2 - 2],
		sizeof(struct Lrn_blk));
    else
    if (strncmp(OldHdr.blkName[i], "GPS", 3) == 0)
      {
      struct Gps_blk	*gps;
      char		*ogps;
      int		pos = 0;

      gps = (struct Gps_blk *)&NewRecord[b[i].start];
      ogps = &OldRecord[OldHdr.startWord[i] * 2 - 2];

      gps->ghealth = *(ushort *)ogps;
      pos += 2;

      memcpy((char *)&gps->glat, &ogps[pos], 10 * sizeof(float));
      pos += 10 * sizeof(float);

      gps->addstat = (long)ogps[pos++];
      gps->gpsmode = (long)ogps[pos++];
      memcpy((char *)gps->gpssats, &ogps[pos], 8);
      pos += 8;
      gps->gpsweek = (long)*(short *)&ogps[pos];
      pos += 2;
      gps->gpsutc = (*(short *)&ogps[pos] << 16) + *(short *)&ogps[pos+2];
      }
    }

}	/* END CONVERTLOGICALRECORD */

/* -------------------------------------------------------------------- */
WriteNewHeader(out)
int		out;
{
  int		i;

  write(out, &f, sizeof(struct Fl));

  write(out, &b[0], sizeof(struct Blk));

  for (i = 0; i < OldHdr.nFull; ++i)
    write(out, &sdi[i], sizeof(struct Sh));

  for (i = 0; i < hcnt; ++i)
    write(out, &hskp[i], sizeof(struct Sh));

  for (i = 0; i < pcnt; ++i)
    write(out, &pms[i], sizeof(struct Pms1));

  for (i = 1; i < 8; ++i)
    if (strncmp(OldHdr.blkName[i], "INS", 3) == 0)
      write(out, &ins, sizeof(struct Ins));
    else
    if (strncmp(OldHdr.blkName[i], "HSKP", 3) == 0)
      ;
    else
    if (strncmp(OldHdr.blkName[i], "DME", 3) == 0)
      write(out, &dme, sizeof(struct Dme));
    else
    if (strcmp(b[i].item_type, "") == 0)
      continue;
    else
      write(out, &b[i], sizeof(struct Blk));

}	/* END WRITENEWHEADER */

/* -------------------------------------------------------------------- */
GetOverRideCalCoes(file_name)
char	file_name[];
{
  FILE	*fp;
  int	i, j;
  char	name[16];
  float	cals[3];


  if ((fp = fopen(file_name, "r")) == NULL)
    {
    fprintf(stderr, "Unable to open Cal Coe override file, continuing.\n");
    return;
    }

  for (i = 0; fgets(buffer, 1024, fp); ++i)
    {
    sscanf(buffer, "%s %f %f %f\n", name, &cals[0], &cals[1], &cals[2]);

    for (j = 0; j < OldHdr.nFull; ++j)
      if (strcmp(sdi[j].name, name) == 0)
        {
        sdi[j].cof[0] = cals[0];
        sdi[j].cof[1] = cals[1];
        sdi[j].cof[2] = cals[2];
        break;
        }
    }

  fclose(fp);

}	/* END GETOVERRIDECALCOES */

/* -------------------------------------------------------------------- */
fphp2mc(a, n)
int     a[], *n;
{
  int	signfr,frac,expon,signex,i;

  for (i = 0; i < *n; i++)
    {
    if (a[i] == 0x00000000)
      continue; 

    signfr	= a[i] & 0x80000000;
    frac	= a[i] & 0x3fffff00;
    expon	= a[i] & 0xfe;
    signex	= a[i] & 1;
    frac	= frac >> 7;
    if (signfr != 0)
      frac = (frac ^ 0x7fffff) + 1;
    expon	= expon >> 1;
    expon	= expon + 126 - signex * 128;
    expon	= expon << 23;
    a[i]	= signfr + expon + frac;
    }

}	/* END FPHP2MC */

/* END ADSCVT.C */
