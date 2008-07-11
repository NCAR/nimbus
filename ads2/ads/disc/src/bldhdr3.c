/* bldhdr3
   Build a version 3 test header from a version 2 header. 

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

/* Set NUM_DSMS to the number of dsms desired in the version 3 header. */
#define NUM_DSMS	4

#include <stdio.h>
#include <header.h>

void bld_Sh (struct Sh*, struct Sh*);
void bld_Blk (struct Blk*, struct Blk*);
void bld_Evt (struct Evt*, struct Evt*);
void bld_Irs (struct Irs*, struct Irs*);
void bld_Pms1v2 (struct Pms1v2*, struct Pms1v2*);
void bld_Serial (struct Serial*, struct Serial*);
void bld_Uvhyg (struct Uvhyg*, struct Uvhyg*);
void bld_Pms2 (struct Pms2*, struct Pms2*);
void bld_Asy (struct Asy*, struct Asy*);

static unsigned char ary2[MX_PHYS];      	/* version 2 header */
static unsigned char ary3[MX_PHYS];     	/* version 3 header */
static struct Fl *fl2;
static struct Fl *fl3;
static str8 dsm_names[7] = {DSM_CTL_STR, DSM_1_STR, DSM_2_STR, DSM_3_STR, 
                            DSM_4_STR, DSM_5_STR, DSM_6_STR};
                           
int ridx = 0;					/* dsm names index */
int start_corr = 0;

main(argc, argv)
int argc;
char *argv[];
{
  static FILE *fd;                        	/* file descriptor */
  int idx2;					/* v2 header array index */
  int idx3;					/* v3 header array index */
  int cnt250 = 0;
  int cnt50 = 0;
  int cnt5 = 0;
  int cnt1 = 0;
  int size;
  int j;

  if (argc < 2) {
    printf ("\nUsage: bldhdr3 header_file\n");
    exit (0);
  }

/* Open the header file. */
  if ((int)((fd = fopen ((const char*)argv[1], "rw"))) == (int)NULL) {
    perror ("bldhdr3: fopen:");
    exit(1);
  }

/* Read the header file. */
  size = fread(ary2, sizeof(char), MX_PHYS, fd);
  printf ("header size = %d\n", size);
  fclose (fd);

/* Assign the flight descriptor pointers. */
  fl2 = (struct Fl*)ary2;
  fl3 = (struct Fl*)ary3;

  printf ("\nVersion 2 flight descriptor.\n"); 
  printf ("fl2->thdr = %s\n", fl2->thdr);
  printf ("fl2->version = %s\n", fl2->version);
  printf ("fl2->prnum = %s\n", fl2->prnum);
  printf ("fl2->fltnum = %s\n", fl2->fltnum);
  printf ("fl2->tpnum = %s\n", fl2->tpnum);
  printf ("fl2->date = %s\n", fl2->date);
  printf ("fl2->time = %s\n", fl2->time);
  printf ("fl2->tzone = %s\n", fl2->tzone);
  printf ("fl2->acraft = %s\n", fl2->acraft);
  printf ("fl2->n_items = %d\n", fl2->n_items);
  printf ("fl2->lrlen = %d\n", fl2->lrlen);
  printf ("fl2->lrppr = %d\n", fl2->lrppr);
  printf ("fl2->thdrlen = %d\n", fl2->thdrlen);

/* Parse the header and determine the number of analog channels at each
   sample rate. */
  for (j = 0, idx2 = sizeof(struct Fl); j < fl2->n_items; 
       j++, idx2 += ((struct Sh*)&ary2[idx2])->item_len) {
    if (!strcmp (&ary2[idx2], SDI_STR)) {
      switch (((struct Sh*)&ary2[idx2])->rate) {
        case RATE_250:
          cnt250++;
          break;
        case RATE_50:
          cnt50++;
          break;
        case RATE_5:
          cnt5++;
          break;
        case RATE_1:
          cnt1++;
          break;
        default:
          printf ("Unknown sdi sample rate, idx2 = %d\n", idx2);
      }
    }
  }

/* Now parse through the header building the version 3 header. */

  *fl3 = *fl2;				/* copy the flight descriptor */

/* Create a HDR descriptor for the AFT time-master dsm. */
  idx2 = sizeof(struct Fl);
  idx3=idx2;
  bld_Blk ((struct Blk*)&ary2[idx2], (struct Blk*)&ary3[idx3]);
  idx2 += ((struct Sh*)&ary2[idx2])->item_len;
  idx3 += ((struct Sh*)&ary3[idx3])->item_len;

/* Process the 250 hz analog and digital sampling descriptors. */
  for (j = 0; j < cnt250; j++) {
       
    ridx = (j * NUM_DSMS) / cnt250 + 1; 

    if (!strcmp (&ary2[idx2], SDI_STR)) {
      bld_Sh ((struct Sh*)&ary2[idx2], (struct Sh*)&ary3[idx3]);
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      idx3 += ((struct Sh*)&ary3[idx3])->item_len;
    }
    else {
      printf ("Expected 250 hz SDI descriptor not found.\n");
      exit(0);
    }
  }

/* Process the 50 hz analog and digital sampling descriptors. 
   Convert to 25 hertz sample rate. */
  for (j = 0; j < cnt50; j++) {
    ridx = (j * NUM_DSMS) / cnt50 + 1;
    if (!strcmp (&ary2[idx2], SDI_STR)) {
      bld_Sh ((struct Sh*)&ary2[idx2], (struct Sh*)&ary3[idx3]);
      ((struct Sh*)&ary3[idx3])->rate = RATE_25;	/* convert to 25 hz */
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      idx3 += ((struct Sh*)&ary3[idx3])->item_len;
    }
    else {
      printf ("Expected 50 hz SDI descriptor not found.\n");
      exit(0);
    }
  }

/* The 50 hz channels are converted to 25 hz, so adjust the start. */
  start_corr -= cnt50 * RATE_50;

/* Process the 5 hz analog and digital sampling descriptors. */
  for (j = 0; j < cnt5; j++) {
    ridx = (j * NUM_DSMS) / cnt5 + 1;
    if (!strcmp (&ary2[idx2], SDI_STR)) {
      bld_Sh ((struct Sh*)&ary2[idx2], (struct Sh*)&ary3[idx3]);
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      idx3 += ((struct Sh*)&ary3[idx3])->item_len;
    }
    else {
      printf ("Expected 5 hz SDI descriptor not found.\n");
      exit(0);
    }
  }

/* Process the 1 hz analog and digital sampling descriptors. */
  for (j = 0; j < cnt1; j++) {
    ridx = (j * NUM_DSMS) / cnt1 + 1;
    if (!strcmp (&ary2[idx2], SDI_STR)) {
      bld_Sh ((struct Sh*)&ary2[idx2], (struct Sh*)&ary3[idx3]);
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      idx3 += ((struct Sh*)&ary3[idx3])->item_len;
    }
    else {
      printf ("Expected 1 hz SDI descriptor not found.\n");
      exit(0);
    }
  }

/* Process the remaining descriptors. */
  for (j = cnt250 + cnt50 + cnt5 + cnt1 + 1; j < fl2->n_items; j++) {
    ridx = j % NUM_DSMS + 1;

/* Throw away HSKP, the dummy SDI, DME, INS, PMS1 descriptors. */
    if (!strcmp (&ary2[idx2], HSKP_STR)) {
      printf ("%s descriptor discarded.\n", &ary2[idx2]);
      start_corr -= 2;
      fl3->n_items--;
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      continue;
    }
    if (!strcmp (&ary2[idx2], SDI_STR)) {
      printf ("%s descriptor discarded.\n", &ary2[idx2]);
      start_corr -= 2;
      fl3->n_items--;
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      continue;
    }
    if (!strcmp (&ary2[idx2], DME_STR)) {
      printf ("%s descriptor discarded.\n", &ary2[idx2]);
      start_corr -= ((struct Dme*)&ary2[idx2])->length;
      fl3->n_items--;
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      continue;
    }
    if (!strcmp (&ary2[idx2], INS_STR)) {
      printf ("%s descriptor discarded.\n", &ary2[idx2]);
      start_corr -= ((struct Ins*)&ary2[idx2])->length;
      fl3->n_items--;
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      continue;
    }
    if (!strcmp (&ary2[idx2], PMS1D_STR)) {
      printf ("%s descriptor discarded.\n", &ary2[idx2]);
      start_corr -= ((struct Pms1*)&ary2[idx2])->length;
      fl3->n_items--;
      idx2 += ((struct Sh*)&ary2[idx2])->item_len;
      continue;
    }

    if (!strcmp (&ary2[idx2], DIGOUT_STR)) { 
      bld_Sh ((struct Sh*)&ary2[idx2], (struct Sh*)&ary3[idx3]);
    }
    else if (!strcmp (&ary2[idx2], GPS_TRIM_STR) ||
             !strcmp (&ary2[idx2], HDR_STR) ||
             !strcmp (&ary2[idx2], LRNC_STR) ||
             !strcmp (&ary2[idx2], OPHIR3_STR)) {
      bld_Blk ((struct Blk*)&ary2[idx2], (struct Blk*)&ary3[idx3]);
    }
    else if (!strcmp (&ary2[idx2], EVNT_STR)) {
      bld_Evt ((struct Evt*)&ary2[idx2], (struct Evt*)&ary3[idx3]);
    }
    else if (!strcmp (&ary2[idx2], IRS_STR)) {
      bld_Irs ((struct Irs*)&ary2[idx2], (struct Irs*)&ary3[idx3]);
    }
    else if (!strcmp (&ary2[idx2], PMS1V2_STR)) {
      bld_Pms1v2 ((struct Pms1v2*)&ary2[idx2], (struct Pms1v2*)&ary3[idx3]);
    }
    else if (!strcmp (&ary2[idx2], SER_STR)) {
      bld_Serial ((struct Serial*)&ary2[idx2], (struct Serial*)&ary3[idx3]);
    }
    else if (!strcmp (&ary2[idx2], UVHYG_STR)) {
      bld_Uvhyg ((struct Uvhyg*)&ary2[idx2], (struct Uvhyg*)&ary3[idx3]);
    }
    else if (!strcmp (&ary2[idx2], PMS2D_STR)) {
      bld_Pms2 ((struct Pms2*)&ary2[idx2], (struct Pms2*)&ary3[idx3]);
    }
    else if (!strcmp (&ary2[idx2], ASYNC_STR)) {
      bld_Asy ((struct Asy*)&ary2[idx2], (struct Asy*)&ary3[idx3]);
    }
    else {
      printf ("Invalid fl->type = %s, idx = %d\n", &ary2[idx2], idx2);
      exit(0);
    }
    idx2 += ((struct Sh*)&ary2[idx2])->item_len;
    idx3 += ((struct Sh*)&ary3[idx3])->item_len;
  } 

  strcpy (fl3->version, "3.0");
  fl3->lrlen += start_corr;
  fl3->lrppr = MX_PHYS / fl3->lrlen;
  fl3->thdrlen = idx3;

  printf ("\nVersion 3 flight descriptor.\n");
  printf ("fl3->thdr = %s\n", fl3->thdr);
  printf ("fl3->version = %s\n", fl3->version);
  printf ("fl3->prnum = %s\n", fl3->prnum);
  printf ("fl3->fltnum = %s\n", fl3->fltnum);
  printf ("fl3->tpnum = %s\n", fl3->tpnum);
  printf ("fl3->date = %s\n", fl3->date);
  printf ("fl3->time = %s\n", fl3->time);
  printf ("fl3->tzone = %s\n", fl3->tzone);
  printf ("fl3->acraft = %s\n", fl3->acraft);
  printf ("fl3->n_items = %d\n", fl3->n_items);
  printf ("fl3->lrlen = %d\n", fl3->lrlen);
  printf ("fl3->lrppr = %d\n", fl3->lrppr);
  printf ("fl3->thdrlen = %d\n", fl3->thdrlen);

/* Open the output header file. */
  if ((int)((fd = fopen ((const char*)strcat (argv[1], "3"), "w"))) ==
      (int)NULL) {
    perror ("bldhdr3: fopen:");
    exit(1);
  }

/* Write the header file. */
  size = fwrite(ary3, sizeof(char), idx3, fd);
  printf ("Output header size = %d\n", size);
  fclose (fd);

}
/*****************************************************************************/

void bld_Sh (struct Sh* v2, struct Sh* v3)

{
  *v3 = *v2;
  strcpy (v3->dsm_locn, dsm_names[ridx]);
  v3->item_len = sizeof(struct Sh);
  v3->start += start_corr;
  v3->ch_gain = 4;
  v3->ch_offset = 0;

  if (!strcmp (v3->type, DIGITAL_STR)) {
    if (!strcmp (v3->name, "PSFD")) {
      strcpy (v3->dsm_locn, "AFT");
      v3->adsaddr = 0x0000;
    }
    else if (!strcmp (v3->name, "PSFD2")) {
      strcpy (v3->dsm_locn, "AFT");
      v3->adsaddr = 0x0008;
    }
    else if (!strcmp (v3->name, "TWCH1")) {
      strcpy (v3->name, "MCRSTAT");
      strcpy (v3->dsm_locn, "AFT");
      v3->adsaddr = 0x0002;
    }
    else if (!strcmp (v3->name, "SDWP1")) {
      strcpy (v3->dsm_locn, "AFT");
      v3->adsaddr = 0x0006;
    }
    else if (!strcmp (v3->name, "XCVCNT")) {
      strcpy (v3->type, COUNTER_STR);
      strcpy (v3->dsm_locn, "AFT");
      v3->adsaddr = 0x0005;
    }
    else if (!strcmp (v3->name, "CNTS")) {
      strcpy (v3->type, COUNTER_STR);
      strcpy (v3->dsm_locn, "AFT");
      v3->adsaddr = 0x0105;
    }
    else
      strcpy (v3->dsm_locn, "PODLW");
  }

  if (!strncmp (v3->item_type, "DIGOUT", 6)) {
    strcpy (v3->item_type, SDI_STR);
    strcpy (v3->type, DIGITAL_OUT_STR);
    if (!strcmp (v3->name, "DOUT1")) {
      strcpy (v3->name, "MCROUT");
      strcpy (v3->dsm_locn, "AFT");
      v3->adsaddr = 0x0004;
    }
  }

  printf ("itm=%6s", v3->item_type);
  printf (" strt=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" rte=%3d", v3->rate);
  printf (" adr=%4X", v3->adsaddr);
  printf (" nam=%6s", v3->name);
  printf (" typ=%s", v3->type);
  printf (" ofst=%3d", v3->offset);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void bld_Blk (struct Blk* v2, struct Blk* v3)

{
  *v3 = *v2;
  strcpy (v3->locn, "FWD");
  strcpy (v3->dsm_locn, "AFT");		/* put all Blks in the AFT */

/* Make gps into tans 3 gps */
  if (!strcmp (v3->item_type, "GPS")) 
    strcpy (v3->item_type, GPS_TANS3_STR);	
  v3->item_len = sizeof(struct Blk);
  v3->start += start_corr;

  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" locn=%s", v3->locn);
  printf (" dsm =%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void bld_Evt (struct Evt* v2, struct Evt* v3)
 
{
  *v3 = *v2;
  strcpy (v3->dsm_locn, DSM_CTL_STR);
  v3->item_len = sizeof(struct Evt);
  v3->start += start_corr;
 
  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" locn=%s", v3->locn);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void bld_Irs (struct Irs* v2, struct Irs* v3)

{
  *v3 = *v2;
  strcpy (v3->dsm_locn, "AFT");
  strcpy (v3->locn, "CAB");
  v3->item_len = sizeof(struct Irs);
  v3->start += start_corr;

  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" locn=%s", v3->locn);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void bld_Pms1v2 (struct Pms1v2* v2, struct Pms1v2* v3)

{
  *v3 = *v2;
  strcpy (v3->dsm_locn, "AFT");
  if (!strcmp (v3->name, "PCAS"))
    strcpy (v3->name, "ASAS");		/* make an asas for testing */
  v3->item_len = sizeof(struct Pms1v2);
  v3->start += start_corr;

  if (v3->rate == RATE_10) {
    v3->rate = RATE_25;
    start_corr += (v3->length * 3) / 2;
    v3->length += (v3->length * 3) / 2;
  }

  if (v3->ps_start) {
    v3->ps_start += start_corr;

    if (v3->rate == RATE_25) {
      start_corr += (v3->ps_length * 3) / 2;
      v3->ps_length += (v3->ps_length * 3) / 2;
    }
  }

  printf ("item=%s", v3->item_type);
  printf (" name=%6s,", v3->name);
  printf (" start=%4d,", v3->start);
  printf (" len=%1d", v3->length);
  printf (" ps_start=%4d,", v3->ps_start);
  printf (" ps_len=%1d", v3->ps_length);
  printf (" rate=%3d,", v3->rate);
  printf (" locn =%s", v3->locn);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void bld_Serial (struct Serial* v2, struct Serial* v3)

{
  *v3 = *v2;
  strcpy (v3->dsm_locn, dsm_names[ridx]);
  v3->item_len = sizeof(struct Serial);
  v3->start += start_corr;

  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void bld_Uvhyg (struct Uvhyg* v2, struct Uvhyg* v3)

{
  *v3 = *v2;
  strcpy (v3->dsm_locn, dsm_names[ridx]);
  v3->item_len = sizeof(struct Uvhyg);
  v3->start += start_corr;

  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void bld_Pms2 (struct Pms2* v2, struct Pms2* v3)

{
  *v3 = *v2;
  strcpy (v3->dsm_locn, "AFT");
  v3->item_len = sizeof(struct Pms2);

  printf ("item=%s", v3->item_type);
  printf (" name=%6s", v3->name);
  printf (" lrlen=%d", v3->lrlen);
  printf (" lrppr=%d", v3->lrppr);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/
 
void bld_Asy (struct Asy* v2, struct Asy* v3)
{
  *v3 = *v2;
  strcpy (v3->dsm_locn, dsm_names[ridx]);
  v3->item_len = sizeof(struct Asy); 
 
  printf ("item=%s", v3->item_type);
  printf (" name=%6s,", v3->name); 
  printf (" lrlen=%d,", v3->lrlen); 
  printf (" lrppr=%d\n", v3->lrppr); 
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/
