/* 
psprint.c  -- server process waiting on socket requests from WINDS process for
converting a WINDS raster image to PostScript and subsequent sending of output
file to designated PostScript printer.
*/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/param.h>
#include <netdb.h>

#include <postscript.h>
#include <printers.h>
#include <colors.h>
#include <socket.h>
#include <lengths.h>
#include <files.h>
#include <constants.h>
#include <help.h>
#include <proto.h>

static char computehost[MAXHOSTNAMELEN];      /* name of comute host */
static char imagedir[MAX_FLNM_LNGTH];      /* absolute pathname of image dir */
static void catchint();
char *ProjectDirectory;         /* home dir for PROJ_DIR */
char *progName = "psprint:";


/********************************  catchint()  ******************************/

void
catchint()
/*
catch SIGINT - useful for ctl-C'ing dbxtool sessions w/o killing
child processes like this one.
*/
{
}
/************************  NotifyWINDS()  *****************************/
 
NotifyWINDS(message,exitstatus)
char *message;
int exitstatus;
{
 NotifyWINDSProcess(message);
 if (exitstatus) {
  PostScriptServerCleanup();
  exit(0);
 }
}
/**********************************  MAIN()  *********************************/

/*ARGSUSED*/
main(argc,argv)
int argc;
char **argv;
{
static char *ProjectDirectory;		/* home dir for WINDS */
char PrinterName[MAXPRINTERNAMELENGTH]; /* name of printer to whom output goes */

 (void)gethostname(computehost,MAXHOSTNAMELEN);
 if (strchr(computehost, '.'))
   *(strchr(computehost, '.')) = '\0';
 if ( (ProjectDirectory = (char *)getenv("PROJ_DIR"))==NULL) {
  ProjectDirectory=(char *)GetDefaultProjDir();
 }
/*
setup socket for exit status information. Must connect to this one as early
as possible in case something bad happens, then process can tell winds what
it was. We always use the same host as the one on which psprint process is
running, since that's where the winds process is running also.
*/
 talk_to_socket("PSPRINT-GENERAL_LISTENING",GENERAL_LISTENING,
  GENERAL_LISTENING,computehost);
 if ( (ProjectDirectory=(char *)getenv("PROJ_DIR"))==NULL) {
  NotifyWINDS("WARNING: PostScript server exiting because WINDS environment variable is not set. Contact a WINDS expert about this message.",TRUE);
 }
/*
establish socket from which to read
*/
 if (WaitForSocket("PSPRINT",POSTSCRIPT,POSTSCRIPT)!=OK) {
  (void)fprintf(stderr,
   "PSPRINT FATAL: socket bind problem; server probably already running\n");
  exit (0);
 }
/*
change directory to location assumed used by print servers and WINDS
image processing routines.
*/
 (void)sprintf(imagedir,HOSTPATH,ProjectDirectory);
 if (SetCurrentDir(imagedir)==ERROR) {
  NotifyWINDS("WARNING: PostScript server exiting because of directory change problem. Contact a WINDS expert about this message.",TRUE);
 }

 if ((int)sigset(SIGINT,catchint) < 0) {
  perror("sigset, psprint catchint");
 };

 /* status message - put out AFTER everything that might cause termination has
  * been successfully completed
  */
 (void)fprintf(stderr,"PSPRINT: waiting on color AND b-w images\n");

  while (1) {
    /* expecting a string designating window, color or greyscale imaging, 
     * input image filename, and a port # and address for status messaging
     */
    int window,length;
    char *buffer;
    char PSMode[MAX_SOCKET_BUFF_LNGTH];                             
    char ImageFilename[MAX_FLNM_LNGTH];               	
    int port,address;

    PSMode[0] = '\0';
    length = MAX_SOCKET_BUFF_LNGTH;
    buffer = (char *)read_from_socket(POSTSCRIPT,&length,BLOCKING);
    sscanf(buffer,IMAGEHEADERFORMAT, PSMode,&window,ImageFilename,&port,&address);

fprintf(stderr, "PSPRINT: %s\n", buffer);
fprintf(stderr, "PSPRINT: in loop, appears something has arrived, %s.\n", PSMode);

    if (!(strcmp(PSMode, PSCOLORIMAGE))) {
      CvtWindsRaster2PS(ImageFilename,PSCOLOR,window,port,address);
    } else
    if (!(strcmp(PSMode, PSGREYSCALEIMAGE))) {
      CvtWindsRaster2PS(ImageFilename,PSGREYSCALE,window,port,address);
    } else
    if (!(strcmp(PSMode, PSBWIMAGE))) {
      CvtWindsRaster2PS(ImageFilename,PSBLACKNWHITE,window,port,address);
    } else
    if (!(strcmp(PSMode, TEXTFILE))) {
      length=MAX_SOCKET_BUFF_LNGTH;
      buffer=(char *)read_from_socket(POSTSCRIPT,&length,BLOCKING);
      sscanf(buffer, "%s", PrinterName);
      IssueLPRequest(window,port,address,PrinterName,ImageFilename);
    } else
    if (!(strcmp(PSMode,PARTYISOVER))) {
      PostScriptServerCleanup();
      strcpy(buffer, "PSPRINT");
      PrintResponseToSender(BYEBYE,port,address,computehost,buffer);
      exit (0);
    }
  }

}

/*************  PostScriptServerCleanup()  ********************************/
PostScriptServerCleanup()
{
  close_socket(POSTSCRIPT);
  close_socket(GENERAL_LISTENING);

}

/*************************  GetRGBValue()  *******************************/
static unsigned int pixel[NUMCOLORS];	/* color index table from XView side */
static FILE *PSOutputFile;		/* file pointer for output file */
static unsigned char *Image;            /* memory buffer for image file */

/*
Return a hex value for the rgb of a given index into color table...
These values are directly associated with the RGB values given for each
color map in xdraw.c (Xv_singlecolor BlueMap[]/BlackMap[] initializations)...
i.e., convert the RGB #'s for each color to a single hex #, e.g. the
color RED at (255,0,0) maps onto 0xf00.

Sometimes an xor effect can produce a color value different than those 
manually mapped -- default these values so as not to confuse the output.
*/
/*
#define GetRGBValue(PixelValue,BlackMap)			\
(PixelValue==	pixel[BKGND]?	0xfff:       			\
(PixelValue==	pixel[BLU]?	0x7ff:       			\
(PixelValue==  	pixel[GRN]? 	0x0f0:       			\
(PixelValue==  	pixel[RED]? 	0xf00:       			\
(PixelValue==  	pixel[VIO]? 	0x83d:       			\
(PixelValue==  	pixel[BRN]? 	0xf80:       			\
(PixelValue==  	pixel[LTG]? 	0x080:       			\
(PixelValue==  	pixel[CYN]? 	0xff0:       			\
(PixelValue==  	pixel[PUR]? 	0xf0f:       			\
(PixelValue==  	pixel[IND]? 	0xfa0:       			\
(PixelValue==  	pixel[ORG]? 	0xf85:       			\
(PixelValue==  	pixel[G3]? 	0x666:       			\
(PixelValue==  	pixel[G4]? 	0xfff:       			\
(PixelValue==  	pixel[G5]? 	0xaaa:       			\
(PixelValue==  	pixel[WHT]? 	0x000:       			\
(PixelValue==  	pixel[BLACK]? 	0x8ff:       			\
  			(0x000)					\
))))))))))))))))
*/

int GetRGBValue(unsigned int PixelValue, int depth)
{
  if (depth == 8)
    PixelValue &= 0x000000ff;

  if (depth == 16)
    PixelValue &= 0x0000ffff;

  if (PixelValue == pixel[BKGND])
    return(0xfff);
  else
  if (PixelValue == pixel[BLU])
    return(0x7ff);
  else
  if (PixelValue == pixel[GRN])
    return(0x0f0);
  else
  if (PixelValue == pixel[RED])
    return(0xf00);
  else
  if (PixelValue == pixel[VIO])
    return(0x83d);
  else
  if (PixelValue == pixel[BRN])
    return(0xf80);
  else
  if (PixelValue == pixel[LTG])
    return(0x080);
  else
  if (PixelValue == pixel[CYN])
    return(0xff0);
  else
  if (PixelValue == pixel[PUR])
    return(0xf0f);
  else
  if (PixelValue == pixel[IND])
    return(0xfa0);
  else
  if (PixelValue == pixel[ORG])
    return(0xf85);
  else
  if (PixelValue == pixel[G3])
    return(0x666);
  else
  if (PixelValue == pixel[G4])
    return(0xfff);
  else
  if (PixelValue == pixel[G5])
    return(0xaaa);
  else
  if (PixelValue == pixel[BLACK])
    return(0x8ff);

  return(0x000);

}
/***************************  GetBWValue()  *********************************/

/*
Return a hex value for the black or white of a given color index into table...
*/
#define GetBWValue(PixelValue)	(PixelValue == pixel[BKGND] ? 0xf : 0x0)

/*************************  GetGreyValue()  *******************************/

/*
Return a hex value for the grey-scale of a given color index into table...

These values are somewhat arbitrarily assigned. Some effort is made to create
maximum contrast in adjacent colors, assuming they are most likely to be seen
together.

Sometimes an xor effect can produce a color value different than those 
manually mapped -- default these values so as not to confuse the output.
*/

int GetGreyValue(unsigned int PixelValue, int depth)
{
  if (depth == 8)
    PixelValue &= 0x000000ff;

  if (depth == 16)
    PixelValue &= 0x0000ffff;

  if (PixelValue == pixel[BKGND])
    return(0x0f);
  else
  if (PixelValue == pixel[BLU])
    return(0x00);
  else
  if (PixelValue == pixel[GRN])
    return(0x05);
  else
  if (PixelValue == pixel[RED])
    return(0x0a);
  else
  if (PixelValue == pixel[VIO])
    return(0x0d);
  else
  if (PixelValue == pixel[BRN])
    return(0x01);
  else
  if (PixelValue == pixel[LTG])
    return(0x06);
  else
  if (PixelValue == pixel[CYN])
    return(0x0b);
  else
  if (PixelValue == pixel[PUR])
    return(0x02);
  else
  if (PixelValue == pixel[IND])
    return(0x07);
  else
  if (PixelValue == pixel[ORG])
    return(0x0c);
  else
  if (PixelValue == pixel[G3])
    return(0x3);
  else
  if (PixelValue == pixel[G4])
    return(0x8);
  else
  if (PixelValue == pixel[G5])
    return(0xe);
  else
  if (PixelValue == pixel[BLACK])
    return(0x9);

  return(0x0f);

}

/*********************  CvtWindsRaster2PS()  ***************************/

CvtWindsRaster2PS(InputFilename,colormode,window,port,address)
char *InputFilename;
int colormode,port,address;
/*
given the filename of a WINDS-format raster file, convert it to a bitmapped
color image PostScript file. Respond to port and address via socket with status.
*/
{
/* 
read in from input file:
*/
 int ImageWidth;		/* actual # of pixels per row in image */
 int depth;
 int BlackMap;			/* TRUE if WINDS BlackMap is in use */
/* 
declare buffer unsigned to insure getting the 8th bit for graphics purposes
...this results in lint complaining profusely about "arg. 1 used inconsistently"
in the sprintf()'s, etc., that use buff[] as an argument.  Ignore it.
*/
 int color;			/* utility index */
 int preamble;			/* whether image is the WINDS preamble */
 FILE *InputFile;		/* file pointer for input file */
 FILE *PSCodeFile;		/* file pointer for PS commands file */
 char buffer [PSCODELINELENGTH];	/* transfer command from PSCodeFile to
					   PSOutputFile */
 char PrinterName[MAXPRINTERNAMELENGTH];
 
 char CodeFilename[MAX_FLNM_LNGTH];	/* ps-code input filename */
 int CenterPosition=TRUE;
 int PageDone=TRUE;

 int totalrows,	totalcolumns;

 static int DefineMode=TRUE;
 static int image_number = 0;		/* increment to maintain unique output
					   filenames */
 static int preamblerows, preamblecols;
 static char OutputFilename[MAX_FLNM_LNGTH];	/* ps-code output filename */
 static int PreambleMode=FALSE;		/* waiting on image file before can
					   complete preamble stuff */

// preamblerows = preamblecols = 0;

  /* open input PostScript commands file and output PostScript file. 
   */
 if (DefineMode) {
  sprintf(CodeFilename, PSCODEFILE, (char *)getenv("PROJ_DIR"));
  if ((PSCodeFile = fopen(CodeFilename,"r")) == NULL) {
   fprintf(stderr,"PSPRINT: can't open POSTSCRIPT CodeFile [%s]\n", CodeFilename);
   return;
  }

  sprintf(OutputFilename, PSOUTPUTFILE, computehost, image_number++);
  if ((PSOutputFile = fopen(OutputFilename,"w")) == NULL) {
   fprintf(stderr,"PSPRINT: ERROR on FOPEN of POSTSCRIPT OUTPUT file\n");
   return;
  }
 }


/* 
open image input filename as specified on socket
*/
 if ((InputFile = fopen(InputFilename,"r")) == NULL) {
  (void)fprintf(stderr,"PSPRINT: trouble with FOPEN of IMAGE_FILENAME file\n");
  return;
 }

 fscanf(InputFile,"%s %d %d %d %d ",
	PrinterName, &BlackMap, &ImageWidth, &totalrows, &depth);

 for (color = 0; color < NUMCOLORS; color++) {
  fscanf(InputFile,"%d ", &pixel[color]);
 }

 fscanf(InputFile,"%d ",&preamble);

 if (!PreambleMode)
  PreambleMode=preamble;
/* 
MUST send an even number of bytes to PostScript processor, else undefined 
results appear on stack. Users of this process are advised to adjust their
images BEFOREHAND to preclude right-end truncation on printout. 
*/
 totalcolumns=(((ImageWidth/BYTE_LEN)%2)?ImageWidth-BYTE_LEN:ImageWidth);

 if (DefineMode) {
/*
print output commands to complete the code definitions
*/
  while ( fgets(buffer,PSCODELINELENGTH,PSCodeFile) != NULL) {
   fprintf(PSOutputFile,"%s",buffer);
  }
  if (colormode==PSCOLOR)
   fprintf(PSOutputFile,"false\n3\ncolorimage\n} bind def\n");
  else
   fprintf(PSOutputFile,"\nimage\n} bind def\n");
  DefineMode=FALSE;
 } else {
  if (PreambleMode) {
/*
finish up what's needed to describe the preamble and draw it
*/
   PrintOutCallParms(PreambleMode,CenterPosition,preamblerows,preamblecols);
   fprintf(PSOutputFile,"\n%%\n%% Preamble needs width and height of image to follow it: \n%%\n");
   fprintf(PSOutputFile,"/imageheight %d def\n",totalrows);
   fprintf(PSOutputFile,"/imagewidth %d def\n",totalcolumns);
   PrintOutWINDSImage(ImageWidth,colormode,preamblerows,preamblecols,depth,BlackMap);
   PreambleMode=FALSE;
  } 

  PrintOutCallParms(PreambleMode,CenterPosition,totalrows,totalcolumns);
 }

 Image=(unsigned char *)malloc( (unsigned int)ImageWidth*totalrows*(depth>>3));
 fread(Image, ImageWidth*(depth>>3), totalrows, InputFile);


 if (PreambleMode) {
/*
can't continue until next call arrives with needed info for completing the
description of the preamble
*/
  preamblerows=totalrows; preamblecols=totalcolumns;
  unlink(InputFilename);
  return;
 } 

 PrintOutWINDSImage(ImageWidth,colormode,totalrows,totalcolumns,depth,BlackMap);

 if (PageDone) {

/* done with raster file -- cleanup time
*/
  fprintf(PSOutputFile,"\nshowpage\n");
  fclose(PSOutputFile);

/* Rename file to name of input file, which we assume reflects some
title or attribute about the file, so as to make it more easily identifiable
when in lpr's queue.
*/
/*** remove this 11/1/96: we're just writing it to /home/tmp and not 
worrying anymore about job-names in printer queue. This is part of fix for
problem where lpr AND lp suddenly started putting out error messages 
intermittently instead of printout plots.

  if (rename(OutputFilename,InputFilename)==ERROR) {
   fprintf(stderr,"PSPRINT WARNING: problem renaming %s to %s: ",
    OutputFilename,InputFilename);
   perror("");
  }
**/
  IssueLPRequest(window,port,address,PrinterName,OutputFilename/***InputFilename***/);
  DefineMode=TRUE;
 }

 unlink(InputFilename);
 fclose(InputFile);
 return;
}

/****************************************************************************/

IssueLPRequest(window,port,address,printer,filename)
int window,port,address;
char *printer,*filename;
/*
call lpr (default) with given printer, suppressing burst page and removing file 
on completion. Want to use lpr instead of lp because with lp you must use -c
option to copy file before unlinking so lp gets it before its gone, but then
the lpq shows an arbitrarily numbered entry instead of the title, which is
something we went to a lot of trouble to get in the queue listing...don't
want to skip unlinking either or can run out of disk. With lpr you can
unlink immediately and for some reason it's not a problem. However, when
UCB support goes away we'll have to rethink this.
*/
{
 FILE *LpPipe;				/* pipe cmd to print PS file to lpr */
 char command[256];		
 char buffer[256];		
 char InitFilename[256];		

/*
set default command
*/
#ifdef SVR4
 sprintf(command, "/bin/lp -c -d %s -o nobanner \"%s\"", printer, filename);
#else
 sprintf(command, "/usr/bin/lpr -h -P%s \"%s\"", printer, filename);
#endif

fprintf(stderr, "PSPRINT: issuing command:\n    %s\n", command);

/*
note that we've changed back to lp -c again. This is to hopefully get around
an alledged mismatch with lpr and Solaris 2.4 where now there IS a problem
with removing file immediately -- apparently lpr's link to it is not enough
anymore to keep it. So we do a copy with lp instead to see if we can eliminate
problem with error message pages coming out instead of plots. (11/1/96 gh)

Later same day: lp alone didn't fix problem. Something in system must have
changed but we don't know what it is; instead we don't rename file to that
original input raster file (which gets removed), we send it to /home/tmp 
instead where (at least at RAF) a scrubber will remove the file automatically. 
This of course is a bad idea for all outside users of WINDS if they don't 
have the same scrubber.
*/

/*
look for configuration file that may contain user-specified lp print command
*/
 (void)sprintf(InitFilename,USERINIT,(char *)getenv("HOME"));
 if ((LpPipe=fopen(InitFilename,"r"))) {
/*
it's there; search for keyword indicating user preference
*/
  while  (fgets(buffer,80,LpPipe)!=NULL) {
   if (StringInString("LpPrintCommand",buffer,TRUE)) {
/*
got it; eliminate the keyword from it and append printer and filename
*/
    buffer[strlen(buffer)-1] = '\0';
    sprintf(command,"%s",buffer);
    sprintf(command,"%s %s \"%s\"",&command[nindex(command,' ')],
     printer,filename);
    break;
   }
  }
  (void)fclose(LpPipe);
 }

 /* execute command and notify sender
 */
 LpPipe=popen(command,"w");
 pclose(LpPipe);
 sprintf(buffer,"PSPRINT requests execution of: '%s'\n",command);
 PrintResponseToSender(window,port,address,computehost,buffer);
}

/*********************  PrintOutCallParms()  ********************************/

PrintOutCallParms(preamble,CenterPosition,rows,columns)
{
  (void)fprintf(PSOutputFile,"\n%%\n%% Preamble or not;\n%% whether position is centered or top-down on page;\n%% total rows and columns of this image;\n%% draw it\n%%\n");
  (void)fprintf(PSOutputFile,"initgraphics\n");
  (void)fprintf(PSOutputFile,"/preamble %d def\n",preamble);
  (void)fprintf(PSOutputFile,"/CenterPosition %d def\n",CenterPosition);
  (void)fprintf(PSOutputFile,"/totalrows %d def\n",rows);
  (void)fprintf(PSOutputFile,"/totalcolumns %d def\n",columns);
}

/************************  PrintOutWINDSImage()  ******************************/

PrintOutWINDSImage(imagewidth,colormode,rows,cols,depth,BlackMap)
int imagewidth,colormode,rows,cols,depth,BlackMap;
{
  int	x,y;	/*coordinates being examined in input image file raster map */
  int	y_coord;

  /* print it out one row at a time. Value imagewidth is width of actual
   * image, and thus is used as an index into the image buffer. Value cols
   * is the desired (adjusted to a multiple of BYTE_LEN) # of rows to be
   * printed out.
   */ 
  fprintf(PSOutputFile,"DrawImage\n");

  for (y = 0; y < rows; y++) {
    y_coord = y * imagewidth;

    for (x = 0; x < cols; x++) {
/*
each value of Image[] is an index into the color table
*/
      if (!(x%(cols/4)))
        fprintf(PSOutputFile,"\n");

      switch (colormode) {
        case PSCOLOR:
          if (depth == 8)
            fprintf(PSOutputFile, "%03x",
			GetRGBValue((int)Image[y_coord+x],depth));
          else
          if (depth == 16)
            fprintf(PSOutputFile, "%03x",
			GetRGBValue(((short *)Image)[y_coord+x],depth));

          break;

        case PSBLACKNWHITE:
          if (depth == 8)
            fprintf(PSOutputFile, "%x", GetBWValue((int)Image[y_coord+x]));
          else
          if (depth == 16)
            fprintf(PSOutputFile, "%x",
			GetBWValue(((short *)Image)[y_coord+x]));
          break;

        case PSGREYSCALE:
          if (depth == 8)
            fprintf(PSOutputFile, "%x",
		GetGreyValue((int)Image[y_coord+x], depth));
          else
          if (depth == 16)
            fprintf(PSOutputFile, "%x",
		GetGreyValue(((short *)Image)[y_coord+x], depth));
          break;
      }
    } 

    fprintf(PSOutputFile,"\n");
  }

  free(Image);
}
