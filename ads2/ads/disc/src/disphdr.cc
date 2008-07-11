/* disphdr.cc
   Displays a tape header

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <stdio.h>
#include <header.h>

#include <TapeHeader.h>

void dispFl (Fl*);
void dispSh (Sh*);
void dispBlk (Blk*);
void dispEvt (Evt*);
void dispIrs (Irs*);
void dispIns (Ins*);
void dispPms1v2 (Pms1v2*);
void dispPms1 (Pms1*);
void dispSerial (Serial*);
void dispUvhyg (Uvhyg*);
void dispPms2 (Pms2*);
void dispAsy (Asy*);

class TapeHeader hdr;

main(int argc, char *argv[])
{
  char *desc;

  if (argc < 2) {
    printf ("\nUsage: disphdr header_file\n");
    exit (0);
  }

// Read the header file. 
  if (hdr.readFile (argv[1]) == ERROR)
    exit(0);

// Display the flight descriptor.
  desc = hdr.getDesc();
  dispFl((Fl*)desc);

// Display the remaining descriptors.
  while (hdr.nextDesc()) {
    desc = hdr.getDesc();

    if (hdr.shType())
      dispSh ((Sh*)desc);
    else if (hdr.blkType())
      dispBlk ((Blk*)desc);
    else if (hdr.evtType())
      dispEvt ((Evt*)desc);
    else if (hdr.irsType())
      dispIrs ((Irs*)desc);
    else if (hdr.insType())
      dispIns ((Ins*)desc);
    else if (hdr.pms1v2Type())
      dispPms1v2 ((Pms1v2*)desc);
    else if (hdr.pms1Type())
      dispPms1 ((Pms1*)desc);
    else if (hdr.serialType())
      dispSerial ((Serial*)desc);
    else if (hdr.uvhygType())
      dispUvhyg ((Uvhyg*)desc);
    else if (hdr.pms2Type())
      dispPms2 ((Pms2*)desc);
    else if (hdr.asyType())
      dispAsy ((Asy*)desc);
  }

}
/*****************************************************************************/

void dispFl (Fl *fl)

{
  printf ("\nVersion 2 flight descriptor.\n");
  printf ("fl->thdr = %s\n", fl->thdr);
  printf ("fl->version = %s\n", fl->version);
  printf ("fl->prnum = %s\n", fl->prnum);
  printf ("fl->fltnum = %s\n", fl->fltnum);
  printf ("fl->tpnum = %s\n", fl->tpnum);
  printf ("fl->date = %s\n", fl->date);
  printf ("fl->time = %s\n", fl->time);
  printf ("fl->tzone = %s\n", fl->tzone);
  printf ("fl->acraft = %s\n", fl->acraft);
  printf ("fl->n_items = %d\n", fl->n_items);
  printf ("fl->lrlen = %d\n", fl->lrlen);
  printf ("fl->lrppr = %d\n", fl->lrppr);
  printf ("fl->thdrlen = %d\n", fl->thdrlen);
}
/*****************************************************************************/

void dispSh (Sh* v3)

{
  printf ("itm=%6s", v3->item_type);
  printf (" strt=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" rte=%3d", v3->rate);
  printf (" adr=%4X", v3->adsaddr);
  printf (" nam=%6s", v3->name);
  printf (" typ=%s", v3->type);
  printf (" ofst=%3d", v3->offset);
  printf (" ch_gain=%1d", v3->ch_gain);
  printf (" ch_ofst=%1d", v3->ch_offset);
  printf (" convert=%f", v3->convert);
  printf (" convert_offset=%3d", v3->convert_offset);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void dispBlk (Blk* v3)

{
  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" locn=%s", v3->locn);
  printf (" dsm =%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void dispEvt (Evt* v3)
 
{
  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" locn=%s", v3->locn);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void dispIrs (Irs* v3)

{
  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" locn=%s", v3->locn);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/
 
void dispIns (Ins* v3)
 
{
  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" rte=%3d", v3->rate);
  printf (" adr1=%4X", v3->adsadr1);
  printf (" adr2=%4X", v3->adsadr2);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void dispPms1v2 (Pms1v2* v3)

{
  printf ("item=%s", v3->item_type);
  printf (" name=%6s,", v3->name);
  printf (" start=%4d,", v3->start);
  printf (" len=%1d", v3->length);
  printf (" ps_start=%4d,", v3->ps_start);
  printf (" ps_len=%1d", v3->ps_length);
  printf (" intf_num=%1d", v3->intf_num);
  printf (" intf_chan=%1d", v3->intf_chan);
  printf (" rate=%3d,", v3->rate);
  printf (" locn =%s", v3->locn);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/
 
void dispPms1 (Pms1* v3)
 
{
  printf ("item=%s", v3->item_type);
  printf (" name=%6s,", v3->name);
  printf (" start=%4d,", v3->start);
  printf (" len=%1d", v3->length);
  printf (" rate=%3d,", v3->rate);
  printf (" adr=%4X", v3->adsaddr);
  printf (" locn =%s", v3->locn);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void dispSerial (Serial* v3)

{
  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" rate=%2d,", v3->rate);
  printf (" name=%6s,", v3->name);
  printf (" port=%1d,", v3->port_num);
  printf (" flow=%1d,", v3->flow_ctl);
  printf (" baud=%4d,", v3->baud_rate);
  printf (" bits=%1d,", v3->data_bits);
  printf (" stop=%1f,", v3->stop_bits);
  printf (" term_str=0x%04x,", *(short*)v3->term_str);
  printf (" align_str=0x%04x,", *(short*)v3->align_str);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void dispUvhyg (Uvhyg* v3)

{
  printf ("item=%s", v3->item_type);
  printf (" start=%4d", v3->start);
  printf (" len=%1d", v3->length);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/

void dispPms2 (Pms2* v3)

{
  printf ("item=%s", v3->item_type);
  printf (" name=%6s", v3->name);
  printf (" lrlen=%d", v3->lrlen);
  printf (" lrppr=%d", v3->lrppr);
  printf (" locn =%s", v3->locn);
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/
 
void dispAsy (Asy* v3)
{
  printf ("item=%s", v3->item_type);
  printf (" name=%6s,", v3->name); 
  printf (" lrlen=%d,", v3->lrlen); 
  printf (" lrppr=%d\n", v3->lrppr); 
  printf (" dsm=%s\n", v3->dsm_locn);
}
/*****************************************************************************/
