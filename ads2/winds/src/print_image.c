/* print_image.c -- write an image of a window to disk file for 
processing by printer server, for either PostScript printing or HP PaintJet
*/
#include <stdio.h>
#include <xview/xview.h>
#include <X11/Xlib.h>

/****************************  WINDS INCLUDES  ******************************/

#include <lengths.h>
#include <constants.h>
#include <files.h>
#include <colors.h>
#include <help.h>
#include <printers.h>
#include <socket.h>
#include <display.h>
#include <proto.h>

FILE *WINDSfopen();

/****************************  GLOBAL IN FILE  ********************************/

static FILE *stream;
static char ImageFilename[MAX_FLNM_LNGTH];
char *GetImagePrinterName();	/* list of user-selected image printers */
char *GetTextPrinterName();	/* list of user-selected text printers */
/*
system-wide printers available for selection by user
*/
static int TotalNumPrinters;	/* total number of all printers */
static int CurrentImagePrinter=ERROR;/* user-selected current image printer */
static int CurrentTextPrinter=ERROR; /* user-selected current text printer */
static int CurrentImageMode=ERROR;   /* user-selected current image mode */

static char *EnabledNames[NUMENABLEDNAMES]={ENABLEDNAME,DISABLEDNAME};
static char *ImageModeName[NUMIMAGEMODES]={BWMODE,GREYMODE,COLORMODE};
static char *PrinterTypeName[NUMPRINTERTYPES]={PJETTYPE,PSTYPE,TEXTONLYTYPE};
static char *BackgroundName[NUMBKGNDVALUES]={ASISBKGND,BLUEBKGND,BLACKBKGND,WHITEBKGND};
static char *NumListColName[NUMLISTCOLVALUES]={PAINTJETCOLS,EPSONCOLS};
static char *OrientationName[NUMORIENTATIONS]={OPTIMAL,LANDSCAPE,PORTRAIT};
static char *NumPerPageName[NUMPERPGVALUES]={ONEPERPAGE,TWOPERPAGE,THREEPERPAGE,FOURPERPAGE};

static struct printconfig PrintConfig[MAXNUMPRINTERS];

/************************  GetPrinterName()  *********************************/

char *
GetPrinterName(int which)
/*
return name of which'th printer read in from printconfig file
*/
{
 if (which==ERROR)
  return (char *)NULL;
 return PrintConfig[which].printername;
}

/*********************  GetImagePrinterName()  ******************************/

char *
GetImagePrinterName(int which)
/*
return the name of the which'th enabled image printer
*/
{
int printernum,printer;

 for (printernum=printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (GetPrinterEnabled(printer) && GetPrinterType(printer)!=TEXTONLY)
   if ((printernum++)==which)
   return GetPrinterName(printer);
 }
 return (char *)NULL;
}

/*********************  GetTextPrinterName()  ******************************/

char *
GetTextPrinterName(which)
int which;
/*
return the name of the which'th enabled text printer
*/
{
int printer,printernum;

 for (printernum=printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (GetPrinterEnabled(printer) && GetPrinterType(printer)==TEXTONLY)
   if ((printernum++)==which)
   return GetPrinterName(printer);
 }
 return (char *)NULL;
}

/********************  GetDisabledPrinterName()  **********************/
 
char *
GetDisabledPrinterName(int which)
/*
return the name of the which'th disabled printer
*/
{
int printer,printernum;

 for (printernum=printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (GetPrinterEnabled(printer)==FALSE)
   if ((printernum++)==which)
   return GetPrinterName(printer);
 }
 return (char *)NULL;
}

/********************  GetEnabledPrinterName()  **********************/
 
char *
GetEnabledPrinterName(int which)
/*
return the name of the which'th enabled printer
*/
{
int printer,printernum;

 for (printernum=printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (GetPrinterEnabled(printer)==TRUE)
   if ((printernum++)==which)
   return GetPrinterName(printer);
 }
 return (char *)NULL;
}

/*******************  GetCurrentTextPrinterNami)  **************************/

char *
GetCurrentTextPrinterName()  
/*
return the name of the current text printer
*/
{
 return (char *)GetPrinterName(GetCurrentTextPrinter());
}

/*******************  GetCurrentImagePrinterName()  **************************/
char *GetCurrentImagePrinterName()  
/*
return the name of the current image printer
*/
{
 return (char *)GetPrinterName(GetCurrentImagePrinter());
}

/******************  PrinterNameinPrinterList()  *****************************/
PrinterNameinPrinterList(printername)
char *printername;
/*
return TRUE if printername is in either the enabled or available lists
*/
{
 return PrinterNameinDisabledList(printername) || 
        PrinterNameinEnabledList(printername);
}

/*****************  PrinterNameinDisabledList()  *****************************/
PrinterNameinDisabledList(printername)  
char *printername;
/*
return TRUE if printername is in the list of available (system) printers
*/
{
int printer;

 for (printer=0; printer<GetNumDisabledPrinters(); printer++)
  if (!(strcmp(printername,GetDisabledPrinterName(printer))))
   return TRUE;
 return FALSE;
}

/*****************  PrinterNameinEnabledList()  *****************************/
PrinterNameinEnabledList(printername)  
char *printername;
/*
return TRUE if printername is in the list of enabled printers
*/
{
int printer;
 for (printer=0; printer<GetNumEnabledPrinters(); printer++)
  if (!(strcmp(printername,GetEnabledPrinterName(printer))))
   return TRUE;
 return FALSE;
}

/********************  GetCurrentImageMode()  ********************************/

GetCurrentImageMode()  
/*
return value of current user-selected image mode, i.e. color, grey, or bw
*/
{
 return CurrentImageMode;
}

/**********************  SetCurrentImageMode()  ******************************/

SetCurrentImageMode(which)  
int which;
/*
set value of current user-selected image mode to which, i.e. color, grey, or bw
*/
{
 CurrentImageMode=which;
}

/**********************  SetCurrentImagePrinter()  ****************************/

SetCurrentImagePrinter(which)  
int which;
/*
set value (xref'd to PrintConfig entries) of user-selected current image printer
*/
{
 CurrentImagePrinter=GetPrinterIndex(GetImagePrinterName(which));
}

/**********************  GetCurrentImagePrinter()  ****************************/

GetCurrentImagePrinter()  
/*
return value of user-selected current image printer
*/
{
 return CurrentImagePrinter;
}

/**********************  SetCurrentTextPrinter()  ****************************/

SetCurrentTextPrinter(which)  
int which;
/*
set value (xref'd to PrintConfig entries) of user-selected current text printer
*/
{
 CurrentTextPrinter=GetPrinterIndex(GetTextPrinterName(which));
}

/**********************  GetCurrentTextPrinter()  ****************************/

GetCurrentTextPrinter()  
/*
return value of user-selected current text printer
*/
{
 return CurrentTextPrinter;
}

/*********************  GetTotalNumPrinters()  *********************************/

GetTotalNumPrinters()
/*
return the total number of printers, either enabled or not
*/
{
 return TotalNumPrinters;
}

/*********************  SetTotalNumPrinters()  *********************************/

SetTotalNumPrinters(value)
int value;
/*
set the total number of printers to value
*/
{
 if (value<MAXNUMPRINTERS)
  TotalNumPrinters=value;
}

/************************  GetNumImagePrinters()  **************************/
 
GetNumImagePrinters()
/*
return number of enabled image printers
*/
{
int printer;
int NumImagePrinters;	/* number of image printers available */

 for (NumImagePrinters=printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (GetPrinterEnabled(printer))
   if (GetPrinterType(printer)==PSPRINTER || GetPrinterType(printer)==PAINTJET)
    NumImagePrinters++;
 }
 return NumImagePrinters;
}

/************************  GetNumTextPrinters()  **************************/
 
GetNumTextPrinters()
/*
return number of enabled text printers
*/
{
int printer;
int NumTextPrinters;	/* number of text printers */

 for (NumTextPrinters=printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (GetPrinterEnabled(printer))
   if (GetPrinterType(printer)==TEXTONLY)
    NumTextPrinters++;
 }
 return NumTextPrinters;
}

/************************  GetNumDisabledPrinters()  **************************/
 
GetNumDisabledPrinters()  
{
int printer;
int NumDisabledPrinters;	/* number of printers available on the net */

 for (NumDisabledPrinters=printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (GetPrinterEnabled(printer)==FALSE)
   NumDisabledPrinters++;
 }
 return NumDisabledPrinters;
}

/*******************  GetNumEnabledPrinters()  *********************************/

GetNumEnabledPrinters()
/*
return the total number of user-designated printers
*/
{
int printer;
int NumEnabledPrinters;	/* number of printers selected by user */

 for (NumEnabledPrinters=printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (GetPrinterEnabled(printer)==TRUE)
   NumEnabledPrinters++;
 }
 return NumEnabledPrinters;
}

/*******************  GetCurrentImagePrinterMode()  **************************/

GetCurrentImagePrinterMode()  
/*
return the current image printing mode, i.e. COLORPRINT, BWPRINT, or GREYPRINT
*/
{
 if (GetCurrentImagePrinter()==ERROR)
  return ERROR;
 return GetPrinterMode(GetCurrentImagePrinter());
}

/*******************  GetCurrentImagePrinterType()  **************************/

GetCurrentImagePrinterType()  
/*
return the current image printing type, i.e. PAINTJET, PSPRINTER, or TEXTONLY
*/
{
 if (GetCurrentImagePrinter()==ERROR)
  return ERROR;
 return GetPrinterType(GetCurrentImagePrinter());
}

/********************  SetPrinterEnabled()  *********************************/

SetPrinterEnabled(printer,value)
int printer,value;
/*
set the value of the enabled field for the printer'th printer to value
*/
{
 PrintConfig[printer].enabled=value;
}

/********************  GetPrinterEnabled()  *********************************/

GetPrinterEnabled(printer)
int printer;
/*
return the enabled value for the printer'th printer
*/
{
 return PrintConfig[printer].enabled;
}

/************************  SetPrinterType()  ********************************/

SetPrinterType(which,type)
int which;
char *type;
/*
set the which'th in list of printer types to type
*/
{
 if (!(strcmp(type,PJETTYPE)))
  PrintConfig[which].type=PAINTJET;
 else if (!(strcmp(type,TEXTONLYTYPE)))
  PrintConfig[which].type=TEXTONLY;
 else if (!(strcmp(type,PSTYPE)))
  PrintConfig[which].type=PSPRINTER;
}

/************************  SetPrinterMode()  ********************************/

SetPrinterMode(which,mode)
int which;
char *mode;
/*
set the which'th in list of printer modes to mode
*/
{
 if (!(strcmp(mode,COLORMODE)))
  PrintConfig[which].mode=COLORPRINT;
 else if (!(strcmp(mode,BWMODE)))
  PrintConfig[which].mode=BWPRINT;
 else if (!(strcmp(mode,GREYMODE)))
  PrintConfig[which].mode=GREYPRINT;
}

/*********************  GetPrinterType()  *****************************/

GetPrinterType(which)
int which;
/*
return the which'th in list of printer types
*/
{
 return PrintConfig[which].type;
}

/*********************  GetPrinterMode()  *****************************/

GetPrinterMode(which)
int which;
/*
return the which'th in list of printer modes
*/
{
 return PrintConfig[which].mode;
}

/********************  GetNumPrinterTypes()  *********************************/

GetNumPrinterTypes()
/*
return the number of possible printer types
*/
{
 return NUMPRINTERTYPES;
}

/*************************  GetOrientationName()  *****************************/

char *
GetOrientationName(int which)
/*
return the which'th in list of possible names of printout orientation
*/
{
 return OrientationName[which];
}

/*************************  GetBackgroundName()  *****************************/

char *
GetBackgroundName(int which)
/*
return the which'th in list of possible names of printout backgrounds
*/
{
 return BackgroundName[which];
}

/*************************  GetNumPerPageName()  *****************************/

char *
GetNumPerPageName(int which)
/*
return the which'th in list of possible names of printouts per page values
*/
{
 return NumPerPageName[which];
}

/*************************  GetNumListColName()  *****************************/

char *
GetNumListColName(int which)
/*
return the which'th in list of possible names of printout list columns values
*/
{
 return NumListColName[which];
}

/*************************  GetImageModeName()  *****************************/

char *
GetImageModeName(int which)
/*
return the which'th in list of possible names of printout modes
*/
{
 return ImageModeName[which];
}

/*************************  GetPrinterTypeName()  *****************************/

char *
GetPrinterTypeName(int which)
/*
return the which'th in list of possible names of printer types
*/
{
 return PrinterTypeName[which];
}

/********************  GetPrinterIndex()  *********************************/

GetPrinterIndex(printername)
char *printername;
/*
return the position in the overall printer list of a printer named printername,
if found; else, return ERROR
*/
{
int printer;
 for (printer=0; printer<GetTotalNumPrinters(); printer++) {
  if (!(strcmp(printername,PrintConfig[printer].printername)))
   return printer;
 }
 return ERROR;
}

/***********************  ConfigurePrinter()  *********************************/

ConfigurePrinter(enabled,printername,typename,modename,
bkgndname,orientname,numperpage)
char *printername,*typename,*modename,*bkgndname,*orientname;
int enabled,numperpage;
/*
if already present in overall list of printers, update the parameters associated
with printername to those given as arguments. If not present, add a printer and
associate it with printername.
*/
{
int type,index;
/*
safeguard against a printconfig file entry trying to override a netconfig
file entry. 
*/
 if (!(strcmp(printername,PAINTJETNAME)) && get_color_image_port()!=PJETSOCKET)
  return;
 if (!(strcmp(printername,EPSONNAME)) && get_lstat_port()!=EPSONSOCKET)
  return;
/*
proceed with a valid request
*/
 if ( (index=GetPrinterIndex(printername)) == ERROR) {
/*
printername not found in list; increment number of printers
*/
  index=GetTotalNumPrinters();
  SetTotalNumPrinters(GetTotalNumPrinters()+1);
 } 
/*
else, index is value of previously configured printer. Proceed to update fields
of overall printer list.
*/
 PrintConfig[index].enabled=enabled;
 (void)sprintf(PrintConfig[index].printername,"%s",printername);
 (void)sprintf(PrintConfig[index].typename,"%s",typename);
 (void)sprintf(PrintConfig[index].modename,"%s",modename);
 SetPrinterType(index,PrintConfig[index].typename);
 SetPrinterMode(index,PrintConfig[index].modename);
 type=PrintConfig[index].type;
 if (type==PSPRINTER) {
/*
background, orientation, and #/pg are irrelevant for the PaintJet and
text-only printers
*/
  (void)sprintf(PrintConfig[index].bkgndname,"%s",bkgndname);
  (void)sprintf(PrintConfig[index].orientname,"%s",orientname);
  PrintConfig[index].numperpage=numperpage;
 }
 if (enabled) {
  if (type==PSPRINTER || type==PAINTJET) {
/*
image mode printer
*/
   if (GetCurrentImagePrinter()==ERROR)
/*
set FIRST enabled printer encountered as current image/text printer if none 
set so far
*/
    CurrentImagePrinter=index;
  } else {
/*
textonly printer
*/
   if (GetCurrentTextPrinter()==ERROR)
    CurrentTextPrinter=index;
  }
 } 
}

/*************************  GetPrintConfig()  *********************************/

GetPrintConfig()
/*
called AFTER set_printer_ports()
*/
{
static char filename[MAX_FLNM_LNGTH];
static char *PrintConfigRecord;
char *string;
char printername[MAXPRINTERNAMELENGTH],
     typename[MAXNAMELENGTH],
     modename[MAXNAMELENGTH],
     bkgndname[MAXNAMELENGTH],
     orientname[MAXNAMELENGTH];
int enabled,defaultnumperpage;
int stringposn;

/*
initialize un-implemented fields so as to suppress Purify reports of umr. This
becomes unnecessary when these fields are implemented and thus initialized.
*/
 (void)sprintf(bkgndname,"\0");
 (void)sprintf(orientname,"\0");
 defaultnumperpage=1;
/*
initialize
*/
 SetTotalNumPrinters(0);
/*
Special cases: check for printers on serial ports.  All relevant 
info on serial line printers is maintained in the netconfig file. Info
in the netconfig file overrides any info maintained in the princonfig
file.
*/
 if (get_color_image_port()==PJETSOCKET)
  ConfigurePrinter(TRUE,PAINTJETNAME,PJETTYPE,COLORMODE,
   (char *)NULL,(char *)NULL,0);
 if (get_lstat_port()==EPSONSOCKET)
  ConfigurePrinter(TRUE,EPSONNAME,TEXTONLYTYPE,BWMODE,
   (char *)NULL,(char *)NULL,0);
/*
open and process the printconfig file
*/
 (void)sprintf(&filename[0],PRINTCONFIG,ProjectDirectory);
 if (ReadFileIntoEntryArray(filename,ErrorReport)!=ERROR) {
  int entry;
  for (entry=0; entry<GetNumFileEntries(); entry++) {   
   PrintConfigRecord=(char *)GetEntryString(entry);
   if (PrintConfigRecord[0]!='#') {
    stringposn=0;
    while ((string=(char *)GetNextString(&stringposn,PrintConfigRecord))!=NULL) {
     switch (GetPrintConfigMode(string)) {
      case PRINTCONFIGPRINTERNAME:
       (void)sprintf(printername,"%s",string);
       break;
      case PRINTCONFIGMODE:
       (void)sprintf(modename,"%s",string);
       break;
      case PRINTCONFIGTYPE:
       (void)sprintf(typename,"%s",string);
       break;
      case PRINTCONFIGENABLED:
       enabled=(!strcmp(string,ENABLEDNAME));
       break;
     }
    }
/*
preclude duplicate entries
*/
    if (!PrinterNameinPrinterList(printername))
     ConfigurePrinter(enabled,
     printername,typename,modename,bkgndname,orientname,defaultnumperpage);
   }
  }
 }
/*
If no printers configured as enabled yet, set a textonly printer
to user environment var values, if there is one.
*/
 if (GetNumEnabledPrinters()==0) {
  if ( ((char *)getenv(PRINTERENV))!=NULL) {
   ConfigurePrinter(TRUE,(char *)getenv(PRINTERENV),TEXTONLYTYPE,BWMODE,
    (char *)NULL,(char *)NULL,0);
  }
 }
/*
add any unix system printers not yet seen to list as well, to provide for
enabling later by user.
*/
 AddSystemPrintersToList();
/*
see ../include/printers.h: set default print to integer that corresponds to
string in ImageModeName[0], as initialized at top of this file.
*/
 SetCurrentImageMode(BWPRINT);
}

/***********************  GetPrintConfigMode()  **************************/
 
GetPrintConfigMode(string)
char *string;
/*
Check value of string against various lists of keywords to determine what
input file mode the string represents.
*/
{
 if (GetStringPosition()==0)
  return PRINTCONFIGPRINTERNAME;
 if (StringInList(string,PrinterTypeName,NUMPRINTERTYPES))
  return PRINTCONFIGTYPE;
 if (StringInList(string,ImageModeName,NUMIMAGEMODES))
  return PRINTCONFIGMODE;
 if (StringInList(string,EnabledNames,NUMENABLEDNAMES))
  return PRINTCONFIGENABLED;
 return ERROR;
}

/***********************  AddSystemPrintersToList()  **************************/
 
AddSystemPrintersToList()
/*
open a pipe to the Unix lpc process and filter its output to retrieve a list
of system printer names, appending it to the full list of printers
*/
{
char DisabledPrinterName[MAXPRINTERNAMELENGTH];
FILE *lpcpipe;

#ifdef SVR4
 lpcpipe=popen("/usr/ucb/lpc status all |  grep : | sed 's/://'","r");
#else
 lpcpipe=popen("/usr/sbin/lpc status all |  grep : | sed 's/://'","r");
#endif
 while ( GetNumDisabledPrinters()<MAXNUMPRINTERS &&
         (fscanf(lpcpipe,"%s",DisabledPrinterName)!=EOF)) {
/*
in each case, check for printer name already enabled by user; if not, you
can add it to list of available system printers
*/
  if (!PrinterNameinPrinterList(DisabledPrinterName)) {
   ConfigurePrinter(FALSE,DisabledPrinterName,TEXTONLYTYPE,DEFAULTMODE,
                    DEFAULTBKGND,DEFAULTORIENT,1/*defaultnumperpage*/);
  }
 }
 (void)pclose(lpcpipe);
}

/****************************  SNAPSHOT()  **********************************/

snapshot(quadrant)
int quadrant;
/* send black and white or greyscale image of quadrant to printer
*/
{
/* if (xy_window_type(quadrant) && plot_type[quadrant]==LISTS) 
  return FALSE; Linux port, lists go to network printer now */

 if (GetVideoShowing(quadrant)) {
/*
video printing not yet enabled. Delete following block when video printing 
is in place: 
*/
  show_op_msg(quadrant,"Sorry...can't print video yet...");
  return FALSE;
 }

 print_image_header(quadrant);
 print_image(quadrant,GetCurrentImageMode(),pw_xid[quadrant],0,0,window_width[quadrant],
  window_height[quadrant],TRUE);

 SetWaitTimeForImage(BWPRINT,get_geom(quadrant));
/*
flush buffer if needed as per printer type
...commented out 6/30, used only on 1200C; determined better to let 
portserver manage this so it could print 2 per page.
/***
 if (GetFlushOnImagePrint())
  (void)write_to_socket(PJETSOCKET,FLUSHBUFFER,strlen(FLUSHBUFFER));
***/

 if (get_lstat_port()==get_bw_image_port())

/* header for lists will be needed on next list startup
*/
  SetHeaderNeeded(TRUE);
 return TRUE;
}
 
/***********************  COLOR_SNAPSHOT()  **********************************/
 
color_snapshot(quadrant)
int quadrant;
/* send image of requesting window to printer, in graphics mode
*/
{
int image_printed;

 image_printed=ERROR;

 /* video printing not yet enabled. Delete following block when video printing 
  * is in place: 
  */
 if (GetVideoShowing(quadrant)) {
  show_op_msg(quadrant,"Sorry...can't print video yet...");
  return FALSE;
 }


 if (xy_window_type(quadrant)) {
  if (xy_window_type(quadrant) && plot_type[quadrant]==LISTS) 
   return FALSE;

  if (GetCurrentImagePrinterType()==PAINTJET &&
      window_type[quadrant]==DISPTYPE_FIXEDFORMAT) {
/* 
b/w image for fixed format should suffice on PAINTJET printers
*/
   print_image_header(quadrant);
   print_image(quadrant,BWPRINT,pw_xid[quadrant],0,0,window_width[quadrant],window_height[quadrant],TRUE);
   image_printed=BWPRINT;
  } else {
/* 
not LISTS or FIXED_FORMAT on a PAINTJET; print a color or greyscale image. 
Requests for color FIXED_FORMAT prints on PostScript printers will be honored.
*/
   print_image_header(quadrant);
   print_image(quadrant,GetCurrentImageMode(),pw_xid[quadrant],0,0,window_width[quadrant],window_height[quadrant],TRUE);
   image_printed=COLORPRINT;
  }
 } else {

/* non-xy window type: color or greyscale image time
*/
  print_image_header(quadrant);
  print_image(quadrant,GetCurrentImageMode(),
    pw_xid[quadrant],
    0,0,window_width[quadrant],window_height[quadrant],TRUE);
  image_printed=COLORPRINT;
 }


 if (image_printed!=ERROR) {
  if (GetCurrentImagePrinterType()==PAINTJET && get_lstat_port()==PJETSOCKET) {

/* paintjet printer has a difficult time catching up with
   ongoing lists after an image has been printed.  Relieve it of this burden
   by telling lists to wait some additional amount of time before restarting.
*/
   SetWaitTimeForImage(image_printed,get_geom(quadrant));
/*
flush buffer if needed as per printer type
...commented out 6/30, used only on 1200C; determined better to let 
portserver manage this so it could print 2 per page.
/***
   if (GetFlushOnImagePrint())
    (void)write_to_socket(PJETSOCKET,FLUSHBUFFER,strlen(FLUSHBUFFER));
***/
   SetHeaderNeeded(TRUE);
  }
  return TRUE;
 } else
  return FALSE;

}

/**************************  PRINT_IMAGE_HEADER()  **************************/

print_image_header(quadrant)
int quadrant;
{
static Server_image image_header;
static Drawable image_xid;
static int image_height=BYTE_LEN*2;
static int image_width;
char header[160];

 image_width=window_width[quadrant];
 (void)sprintf(header,"'%s' (%s@%s) %s %s %s %s",
  (char *)GetUnixCleanFilename(get_description(quadrant)),(char *)getenv("USER"),
  (char *)GetDisplayHost(),project,(char *)get_fltno(),CurrentDate,current_time);
 (*InitMemDraw[GetDrawType()])
   (&image_header,image_width,image_height,BYTE_LEN);
 image_xid=(Drawable)xv_get(image_header,XV_XID);
 (*FillArea[GetDrawType()])(image_xid,0,0,image_width,image_height,BKGND);
 (*SetContext[GetDrawType()])(image_xid,GXcopy,BLACK,BKGND,0,
  GetFontID(GetImageHeaderFont()));
 (*DrawText[GetDrawType()])(0,image_height-GetFontDescent(GetImageHeaderFont()),
  header);
 if (GetCurrentImagePrinterType()==PAINTJET)
  print_image(PREAMBLE,BWPRINT,image_xid,0,0,image_width,image_height,FALSE);
 else
  print_image(PREAMBLE,GetCurrentImageMode(),image_xid,0,0,image_width,
  image_height,FALSE);
}

/**************************  WriteImageFile()  ********************************/

WriteImageFile(int quadrant, int width, int height, int depth, char *data)
{
  int	color;
  char	imagedir[MAX_FLNM_LNGTH];
  static int image_number=0;      /* used to create unique image filenames */

  /* open output image file
   */
  sprintf(ImageFilename,IMAGE_FILENAME,(char *)GetDisplayHost(),
	quadrant==PREAMBLE?"preamble":
	(char *)GetUnixCleanFilename(get_description(quadrant)),image_number++);

  /* change directory to location assumed used by print servers
   */
  sprintf(imagedir, HOSTPATH, ProjectDirectory);
  if (SetCurrentDir(imagedir) == ERROR) {
    time_to_log();
    fprintf(stderr,"WARNING: can't chdir to %s in print_image.c\n",imagedir);
    return ERROR;
  }

  if ((stream = WINDSfopen(ImageFilename,"w")) == NULL) {
    time_to_log();
    fprintf(stderr,"WARNING: can't open %s in print_image.c\n",ImageFilename);
    return ERROR;
  } 

  if (GetCurrentImagePrinterType()==PSPRINTER) {
    /* psprint server process expects the name of the printer as first arg
     * in file.
     */
    fprintf(stream,"%s %d %d %d %d ", (char *)GetCurrentImagePrinterName(),
	(*GetColorMap[GetDrawType()])() == BLACK_MAP, depth*width, height, depth);
  } else {
    /* write out image parameters, color table, etc. to file for PAINTJET
     * printer.
     */
    fprintf(stream,"%d %d %d %d ", depth*width, height, depth, cms_size);
  }

  for (color = 0; color < NUMCOLORS; color++) 
    fprintf(stream, "%d ", xcolors[color+index_offset].pixel);

  fprintf(stream,"%d ",
	GetCurrentImagePrinterType()==PSPRINTER?(quadrant==PREAMBLE):
	(quadrant!=PREAMBLE?window_type[quadrant]:ERROR));

  if (depth == 16)
    width <<= 1;

  fwrite((char *)data, (unsigned)(depth*width*height), 1, stream);
fprintf(stderr, "print_im: wrote %d bytes\n", (depth*width*height));
  WINDSfclose(stream);
  return OK;
}

/*****************************************************************************/

PrintTextFile(filename)
char *filename;
{
 if (GetCurrentImagePrinter()==ERROR)
  return ERROR;
 if (GetCurrentImagePrinterType()==PAINTJET) {
  return NotifyPrinter(TEXTFILEPRINTED,PJETSOCKET,TEXTFILE,filename);
 } 
 if (GetCurrentImagePrinterType()==PSPRINTER) { 
  return NotifyPrinter(TEXTFILEPRINTED,POSTSCRIPT,TEXTFILE,filename);
 } 
 return ERROR;
}

/****************************  PRINT_IMAGE()  ********************************/

print_image(quadrant,image_type,xid,x,y,width,num_rows,show_msg)
int quadrant,image_type;
Drawable xid;
int x,y,width,num_rows,show_msg;

/* write an image of pixwin of a window to disk file for processing by 
   server designated for image_type (b/w, greyscale, or color)

   NOTE that with y-res (in netconfig file) set at 880, print_image_header()
   image_height at 16, op-panel height at 64, and control panel height at 80, 
   that very close to precisely 3 images per HP PJet page can be accomplished.  
*/
{
  int	bytes_per_row;
  char	printmsg[MAXPRINTERNAMELENGTH*3];
  int	numrows, depth;
  extern Display *win_display;

  depth = XDefaultDepth(win_display, DefaultScreen(win_display));

fprintf(stderr, "w = %d, h = %d, d = %d\n", width, num_rows, depth);

  if (GetCurrentImagePrinterType() == PAINTJET) {
    /* check valid request, adjust color if appropriate, and truncate as per 
     * PaintJet printer width
     */
    if (image_type == GREYPRINT || quadrant == PREAMBLE || quadrant == PLOTS ||
		window_type[quadrant] == DISPTYPE_FIXEDFORMAT)
      image_type = BWPRINT;

    if (((image_type == GREYPRINT || image_type == BWPRINT) 
		&& get_bw_image_port() == ERROR) ||
		(image_type == COLORPRINT && get_color_image_port() == ERROR))
      return;

    bytes_per_row = MIN((width / depth), PJ_MAX_BYTES_PER_ROW);
  } else
    bytes_per_row = width / depth;


/* BUG in OW3: must subtract IMAGE_MARGIN pixels from width & num_rows or 
 * BadMatch error occurs in X_GetImage().
 * ...so, set numrows (# of rows) by deducting IMAGE_MARGIN from num_rows.
 * But, must send integral # of bytes (length of each row) to printer server
 * process; if current # of bytes doesn't provide the IMAGE_MARGIN pixel
 * margin, deduct one more.
 */
  numrows=num_rows-IMAGE_MARGIN;
  if (width < bytes_per_row*depth + IMAGE_MARGIN)
    --bytes_per_row;

/* in case of overlapping geometries, explicitly SHOW the window to ensure it
 * is completely visible for printing purposes, and for that matter a crash is 
 * averted from trying to grab a hidden image. As well, hide any panel menus 
 * that are showing.
 */
  if (quadrant!=PREAMBLE && quadrant != PLOTS) {
    SetWINDSShowWindow(quadrant,TRUE);
    HideCurrentPanelMenu();
  }

  /* write out image to file
   */
  if (quadrant!=PREAMBLE && quadrant != PLOTS)
    show_op_msg(quadrant,"Grabbing image...");

  if (quadrant!=PREAMBLE && GetVideoShowing(quadrant)) {
/*
note: this is the first step in processing a video image...you must also
adjust above width/height values as per the differences here to get it
right. I left it undone because we also need significant adjustments in
how to proceed in psprint process, converting the WINDS raster to PostScript.
Need more info about color table, etc...also would be nice to be able
to adjust dimensions of Video image, but xv_set with VIDEO_WIDTH and
VIDEO_HEIGHT don't seem to have any effect. Consult with Parallax to
proceed from here...

If all fails, simply remove the PRT button from video display, since it
might not be too important anyway.
*/
/*
we have a basic problem with following: SEGV in fwrite() call in WriteImageFile()
given video image parameters...and I don't know why yet.
*/
/***
  XImage *image = (XImage *)GetVideoImage(GetQuadrantVideoSource(quadrant));
  if(WriteImageFile(quadrant,
   image->bytes_per_line,image->height,depth,image->data)
   ==ERROR){
   if (show_msg && quadrant!=PREAMBLE)
    show_op_msg(quadrant,"Can't print file...see error log");
   DestroyVideoImage();
   return;
  }
  DestroyVideoImage();
***/
  } else {
    /* either quadrant is PREAMBLE or Video is NOT showing in this quadrant
     * or both
     */
    if (WriteImageFile(quadrant,bytes_per_row,numrows,depth, (char *)(*GetImage[GetDrawType()])(xid,x,y,(depth*bytes_per_row),numrows)) == ERROR) {
      if (show_msg && quadrant!=PREAMBLE && quadrant!=PLOTS)
        show_op_msg(quadrant,"Can't print file...see error log");
      (*DestroyImage[GetDrawType()])();
      return;
    }
    (*DestroyImage[GetDrawType()])();
  }
  ShowCurrentPanelMenu();

  /* notify print server and display feedback to user of progress
   */
  if (NotifyPrintServer(quadrant,image_type)!=ERROR) {
    if (show_msg && quadrant!=PREAMBLE && quadrant!=PLOTS) {
      sprintf(printmsg,"%s print request issued to %s...",
	(image_type == BWPRINT) ? "B/W" :
	(image_type == COLORPRINT ? "Color" : "Greyscale"),
	(char *)GetCurrentImagePrinterName());
      show_op_msg(quadrant,printmsg);
    }
    if (quadrant!=PREAMBLE)
      fprintf(stderr,"Requested %s print of %s to %s\n", 
	(image_type == BWPRINT) ? "b/w" :
	(image_type == COLORPRINT ? "color" : "greyscale"),
	ImageFilename,(char *)GetCurrentImagePrinterName());
    } else {
      if (show_msg && quadrant!=PREAMBLE && quadrant!=PLOTS)
        show_op_msg(quadrant,"Image not printed...see error log");
    return;
  }

  /* add form feed for calibration plot images only
   */
  if (quadrant==PLOTS && GetCurrentImagePrinterType()==PAINTJET ) {
    if (get_bw_image_port()==ERROR)
      return;
    if (write_to_socket(get_bw_image_port(),"\f",strlen("\f"))==EBADWRITE) {
      perror("Form feed problem");
    }
  }

  return;
}

/***********************  NotifyPrintServer()  *********************************/

NotifyPrintServer(quadrant,image_type) 
int quadrant, image_type;
{
 switch (image_type) {
 case BWPRINT:
  return NotifyPrinter(quadrant,
   GetCurrentImagePrinterType()==PAINTJET?get_bw_image_port():POSTSCRIPT,
   GetCurrentImagePrinterType()==PAINTJET?PJETBWIMAGE:PSBWIMAGE,ImageFilename);

 case GREYPRINT:

  return NotifyPrinter(quadrant,
   GetCurrentImagePrinterType()==PAINTJET?get_bw_image_port():POSTSCRIPT,
   GetCurrentImagePrinterType()==PAINTJET?PJETBWIMAGE:PSGREYSCALEIMAGE,
    ImageFilename);

 case COLORPRINT:

  return NotifyPrinter(quadrant,
   GetCurrentImagePrinterType()==PAINTJET?PJETSOCKET:POSTSCRIPT,
   GetCurrentImagePrinterType()==PAINTJET?PJETCOLORIMAGE:PSCOLORIMAGE,
    ImageFilename);
 } 
}

/**************************  NotifyPrinter()  *********************************/

NotifyPrinter(quadrant,printer,printmode,filename)
int printer;
char *printmode,*filename;
/* 
send info to print server: quadrant requesting print (so status message can
be directed to that window's op-panel when print server responds), which
printer is desired, type of print request (i.e., b/w, color, etc.), and the
filename of image file to be processed.
*/
{
  char buffer[MAX_SOCKET_BUFF_LNGTH];

  sprintf(buffer,IMAGEHEADERFORMAT, printmode,quadrant,filename,
	INTERWINDS_LISTEN+get_session_number(GetDisplayHost()),
	NUM_PORTS+get_session_number(GetDisplayHost()));

  if (write_to_socket(printer,buffer,strlen(buffer))==EBADWRITE) {
    perror("NotifyPrinter: image write_to_socket");
    fprintf(stderr,"Problem with %s\n",buffer);
    return ERROR;
  }

  if (!strcmp(printmode,TEXTFILE) && printer==POSTSCRIPT) {
/*
need to append name of printer to socket message, since it is not pre-pended in
text files as is done with image files
*/
    sprintf(buffer,"%s",GetCurrentImagePrinterName());
    write_to_socket(printer,buffer,strlen(buffer));
  }

  return OK;

}
