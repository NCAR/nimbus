/* 
prt_srvr.c -- 'port server' for images, lists, and statistics output to printer
via serial port. Supports HP PaintJet printer in color and b/w.
*/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/param.h>
#include <netdb.h>
#include <termio.h>
#ifdef SVR4
#include <ttold.h>
#endif

#include <colors.h>
#include <constants.h>
#include <socket.h>
#include <lengths.h>
#include <printers.h>
#include <help.h>
#include <network.h>
#include <files.h>
#include <proto.h>


static FILE *file,			/* image file */
     *stream;				/* output serial port */
static int PortFD; 			/* port file descriptor */
static char port[MAX_FLNM_LNGTH];	/* on which port, if any, is printer */
static int printer_type; 		/* printer being served at port */
static int read_socket;			/* which socket to read from */
static int *colortable,			/* lookup table for paintjet colormap */
    pixel[NUMCOLORS],			/* color index table from XView side */
    height,width,			/* dimensions of image */
    color_table_size;			/* max. size needed for color indices */
static short int mask[BYTE_LEN<<1]= {  	/* or'ing values */
1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768
};
static char top_planes[16],		/* command for 1st two color planes */
     last_plane[16];			/* command for third color plane */
static char raster_print[MAX_LEN_ESC];
static char errmsg[MAX_MSG_LNGTH]; 	/* error messages to error log server */
static char printer_name[MAX_FLNM_LNGTH];/* string for printer being served */
static unsigned char *image;             /* memory buffer for image file */
static char PortServerComputeHost[MAXHOSTNAMELEN],/* host on which server is running */
     WINDSComputeHost[MAXHOSTNAMELEN],	/* compute host for WINDS sessions */
     *buffer,                           /* read from socket */
     imagefilename[MAX_FLNM_LNGTH],      /* relative pathname of image file */
     imagedir[MAX_FLNM_LNGTH];          /* absolute pathname of image dir */
static int debug=0;			/* used in debugger: set to get
					debug printf's */
/*
maintain counter of how many WINDS images have been printed to a page on
HP1200C before sending a form feed to flush page.
*/
static int num1200images=0;
/*
whether HP1200C is at top of page or not
*/
static int AtTopOfPage=TRUE;
static int Parallel=FALSE;              /* TRUE if port out is parallel */
/*
whether this output is going to a printer or not, which determines various
other management tasks
*/
static int OutputToPrinter=FALSE;

char *progName = "prt_srvr:";
char *ProjectDirectory;		/* home dir for PROJ_DIR */
static void catchint();

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
  PortServerCleanup();
  exit(0);
 }
}

/**********************************  MAIN()  *********************************/

/*ARGSUSED*/
main(argc,argv)
int argc;
char **argv;
{
  int baudrate;
  char mode[MAX_SOCKET_BUFF_LNGTH];
  int portNum,address;
  int length,window;
  int maxchars=DEFAULTPJETCHARSPERLINE;

/* 
get hostname of WINDS compute host, associated display host, and 
path of project directory
*/
 (void)gethostname(PortServerComputeHost,MAXHOSTNAMELEN);
 if (strchr(PortServerComputeHost, '.'))
   *(strchr(PortServerComputeHost, '.')) = '\0';
/*
if startup command has a second argument, it is to be the name of the
WINDS compute host for this network session. Else, the compute host
on which this process is running is the WINDS compute host. This is
the host on which a listening socket is bound for communication between
these processes.
*/
 if (argc>2) {
  (void)sprintf(WINDSComputeHost,"%s",argv[2]);
 } else {
  (void)sprintf(WINDSComputeHost,"%s",PortServerComputeHost);
 }
 if ( (ProjectDirectory = (char *)getenv("PROJ_DIR"))==NULL) {
  ProjectDirectory=(char *)GetDefaultProjDir();
 }
/*
setup socket for exit status information. Must connect to this one as early
as possible in case something bad happens, then process can tell winds what
it was.
*/
 talk_to_socket("PORTSERVER-GENERAL_LISTENING",GENERAL_LISTENING,
  GENERAL_LISTENING,WINDSComputeHost);
/*
get WINDS home dir
*/
 if ( (ProjectDirectory=(char *)getenv("PROJ_DIR"))==NULL) {
  NotifyWINDS("WARNING: port server exiting because WINDS environment variable is not set. Contact a WINDS expert about this message.",TRUE);
 }
/* 
determine which output type is being served by consulting the netconfig file.  
If none, done.
*/
 get_entry_by_name(PortServerComputeHost);
 switch (printer_type=get_printer(argv[1])) {
  case HP1200C:
/*
serial port HP1200C DeskJet
*/
   read_socket=PJETSOCKET;
   baudrate=HP1200C_BAUD;
   (void)sprintf(printer_name,"HP1200C");
   OutputToPrinter=TRUE;
   break;
  case SPJET:
/*
serial port interfaced PaintJet
*/
   read_socket=PJETSOCKET;
   baudrate=SPJET_BAUD;
   (void)sprintf(printer_name,"SPJET");
   OutputToPrinter=TRUE;
   break;
  case PHP1200C:
/*
parallel port PHP1200C DeskJet
*/
   read_socket=PJETSOCKET;
   baudrate=ERROR;
   (void)sprintf(printer_name,"PHP1200C");
   Parallel=TRUE;
   OutputToPrinter=TRUE;
   break;
  case PPJET:
/*
parallel port interfaced PaintJet
*/
   read_socket=PJETSOCKET;
   baudrate=ERROR;
   (void)sprintf(printer_name,"PPJET");
   Parallel=TRUE;
   OutputToPrinter=TRUE;
   break;
  case PJET:
/*
parallel interfaced PaintJet
*/
   read_socket=PJETSOCKET;
   baudrate=PJET_BAUD;
   (void)sprintf(printer_name,"PJET");
   OutputToPrinter=TRUE;
   break;
  case EPSON:
/*
serial port Epson wide-carriage
*/
   read_socket=EPSONSOCKET;
   baudrate=EPSON_BAUD;
   (void)sprintf(printer_name,"EPSON");
   OutputToPrinter=TRUE;
   break;
  case LCD:
/*
LCD type lcd display, formatted ascii output
*/
   read_socket=FLAT_PANEL_DATA;
   baudrate=LCD_BAUD;
   (void)sprintf(printer_name,"LCD");
   OutputToPrinter=FALSE;
   break;
  case FLIP:
/*
FLIP type lcd display, formatted ascii output
*/
   read_socket=FLAT_PANEL_DATA;
   baudrate=FLIP_BAUD;
   (void)sprintf(printer_name,"FLIP");
   OutputToPrinter=FALSE;
   break;
  case ASCIIFEED:
/*
ascii data values only
*/
   read_socket=ASCIIVALUES;
   baudrate=ASCII_BAUD;
   (void)sprintf(printer_name,"ASCII");
   OutputToPrinter=FALSE;
   break;
  case NONE:
  default:
   (void)fprintf(stderr,
    "PRT_SRVR: Unknown or no printer at %s: exiting...\n", argv[1]);
   exit (0);
 }
/*
establish socket from which to read
*/
 if (WaitForSocket(printer_name,read_socket,read_socket)!=OK) {
   (void)fprintf(stderr,
   "PRT_SRVR on %s FATAL: socket bind pblm; server probably already running\n",
   argv[1]);
  exit (0);
 }
/* 
open stream to serial or parallel port
*/
 if (Parallel)
#ifdef SVR4
  (void)sprintf(port,"/dev/bpp0");
#else
  (void)sprintf(port,"/dev/lp0");
#endif
 else
  (void)sprintf(port,"/dev/%s",argv[1]);
 if ( (stream=WINDSfopen(port,"w"))==NULL) {
  (void)WINDSfclose(stream);
  (void)sprintf(errmsg,"WARNING: port server on %s (called with argument %s) exiting because of trouble opening port %s (error #: %d). Contact a WINDS expert about this message.",
   PortServerComputeHost,argv[1],port,errno);
  perror("PRT_SRVR: opening stream");
  NotifyWINDS(errmsg,TRUE);
 }
 init_port(port,baudrate);
/* 
change directory to location assumed used by print servers and WINDS
image processing routines.
*/
 (void)sprintf(imagedir,HOSTPATH,ProjectDirectory);
 if (SetCurrentDir(imagedir)==ERROR) {
  (void)fprintf(stderr,
   "PRT_SRVR on %s FATAL: chdir pblm...\n",argv[1]);
   NotifyWINDS("WARNING: port server exiting because of directory change problem. Contact a WINDS expert about this message.",TRUE);
 }
 if ((int)sigset(SIGINT,catchint) < 0) {
  perror("sigset, psprint catchint");
 };
/*
status message - put out AFTER everything that might cause termination has
been successfully completed
*/
 if (printer_type==EPSON) {
  (void)fprintf(stderr,
   "PRT_SRVR: sends data to EPSON, port %s on %s\n",
   port,PortServerComputeHost);
 } else {
/***
  (void)fprintf(stderr,
***/
  printf(
   "PRT_SRVR: sends data to %s, port %s on %s\n",
   printer_name,port,PortServerComputeHost);
 }

 config_port(ASCII);


 /* Main loop.
  */
 do {
  length=MAX_SOCKET_BUFF_LNGTH;
  buffer=(char *)read_from_socket(read_socket,&length,BLOCKING);
  strcpy(mode, buffer);

  if (!(strcmp(mode,PJETCOLORIMAGE))) {
/* 
color image
*/
   sscanf(buffer,IMAGEHEADERFORMAT,mode,&window,imagefilename,&portNum,&address);
   if (OutputToPrinter==TRUE && printer_type!=EPSON ) {
    sprintf(buffer,"PRT_SRVR: printing color image from %s for user %s\n",
		imagefilename,(char *)getenv("USER"));
    PrintResponseToSender(window,portNum,address,WINDSComputeHost,buffer);
    if ( (printer_type==HP1200C || printer_type==PHP1200C) 
      && num1200images==0 && !AtTopOfPage)
     send_reset();
    PjetPrintColorImage();
/*
flush after 4th 1200 image - each image consists of 2 counts, a header and
the image to follow, and allow 2 of these sets per page
*/
    if (printer_type==HP1200C || printer_type==PHP1200C) {
     if (++num1200images==4) 
      send_reset();
     else
      AtTopOfPage=FALSE;
    }
   }
  } else if (!(strcmp(mode,PJETBWIMAGE))) {
/* 
b/w image
*/
   (void)sscanf(buffer,IMAGEHEADERFORMAT,
    mode,&window,imagefilename,&portNum,&address);
   if (OutputToPrinter==TRUE && printer_type!=EPSON ) {
    (void)sprintf(buffer,"PRT_SRVR: printing b/w image from %s for user %s\n",
     imagefilename,(char *)getenv("USER"));
    PrintResponseToSender(window,portNum,address,WINDSComputeHost,buffer);
    if ((printer_type==HP1200C || printer_type==PHP1200C)
      && num1200images==0 && !AtTopOfPage)
     send_reset();
    PjetPrintBWImage(); 
    if (printer_type==HP1200C || printer_type==PHP1200C) {
     if (++num1200images==4) 
      send_reset();
     else
      AtTopOfPage=FALSE;
    }
   }
  } else if (!(strcmp(mode,TEXTFILE))) {
/* 
general ascii text output
*/
   (void)sscanf(buffer,IMAGEHEADERFORMAT,
    mode,&window,imagefilename,&portNum,&address);
/***
   (void)fprintf(stderr,"PRT_SRVR: printing text file %s for user %s\n",
     imagefilename,(char *)getenv("USER"));
***/
   if (( printer_type==HP1200C || printer_type==PHP1200C) && !AtTopOfPage)
    send_reset();
   Print80CharLines(FALSE,portNum,address); 
   send_reset();

  } else if (!(strcmp(mode,STATISTICS))) {
/* 
statistics output
*/
   (void)sscanf(buffer,IMAGEHEADERFORMAT,
    mode,&window,imagefilename,&portNum,&address);
   (void)sprintf(buffer,"PRT_SRVR: printing stats from %s for user %s\n",
     PortServerComputeHost,(char *)getenv("USER"));
   PrintResponseToSender(TEXTFILEPRINTED,portNum,address,WINDSComputeHost,buffer);
   if (( printer_type==HP1200C || printer_type==PHP1200C) && !AtTopOfPage)
    send_reset();
   Print80CharLines(FALSE,portNum,address); 
   send_reset();

  } else if (!(strcmp(mode,PARTYISOVER))) {
   (void)sscanf(buffer,IMAGEHEADERFORMAT,
    mode,&window,imagefilename,&portNum,&address);
   PortServerCleanup();
   (void)sprintf(buffer,"PORTSERVER");
   PrintResponseToSender(BYEBYE,portNum,address,WINDSComputeHost,buffer);
   if (printer_type==HP1200C || printer_type==PHP1200C)
    send_reset();
   exit (0);
  } else if (!(strcmp(mode,FLUSHBUFFER))) {
/* 
request for initialization/flush buffers on paintjet
*/
   if (OutputToPrinter==TRUE && printer_type != EPSON) {
    send_reset();
   }
  } else {
/* 
lists or other miscellaneous
*/
   if (OutputToPrinter==TRUE && printer_type != EPSON) {
    if (num1200images) {
/*
there is one image already printed to page; flush that page before starting
lists on new one
*/
     send_reset();
    }
/*
examine each line for # of characters to determine font size
*/ 
    (void)fputs("\033\&k4S",stream);
    maxchars=DEFAULTPJETCHARSPERLINE;
    if (strlen(buffer)>=maxchars) {
     (void)fputs("\033\&k2S",stream);
     maxchars=MAXPJETCHARSPERLINE;
    }
   } else if (printer_type == EPSON) {
     maxchars=MAXEPSONCHARSPERLINE;
   }
   if (OutputToPrinter==TRUE && strlen(buffer)>=maxchars) {
     (void)sprintf(&buffer[maxchars-1],"\n");
   }
   (void)fputs(buffer,stream);
   (void)fflush(stream);
   AtTopOfPage=FALSE;
   if (debug)
    printf("%s",buffer);
  }
 } while (buffer==buffer);


 PortServerCleanup();
 return 0;

}

/*********************  PortServerCleanup()  ********************************/

PortServerCleanup()
/*
clean up resources, send reset command to flush printer
*/
{
 if (OutputToPrinter==TRUE && printer_type!=EPSON ) {
  send_reset();
 }
 close_socket(read_socket);
 close_socket(GENERAL_LISTENING);
/***
 if (!Parallel) 
***/
  (void)close(PortFD);
 (void)WINDSfclose(stream);
}

/***********************  Print80CharLines()  ********************************/

Print80CharLines(delete,port,address)
int delete,port,address;
/*
read lines of maximum 80 characters from filename designated in imagefilename 
and print out presumed ascii text to the stream
*/
{
char buffer[128];

/* open filename as specified on socket
*/
 if ( (file=WINDSfopen(imagefilename,"r"))==NULL) {
  (void)WINDSfclose(file);
  (void)fprintf(stderr,"PRT_SRVR on %s: %s FOPEN of TextFile %s\n",
   PortServerComputeHost,printer_name,imagefilename);
  return;
 }
 config_port(ASCII);
 if (OutputToPrinter==TRUE && printer_type!=EPSON ) {
/*
set to DEFAULTPJETCHARSPERLINE chars/line
*/
  (void)fputs("\033\&k4S",stream);
 }
 while  (fgets(buffer,80,file)!=NULL) {
  (void)fputs(buffer,stream);
 }
 (void)fflush(stream);
 (void)WINDSfclose(file);
 if (delete)
  (void)unlink(imagefilename);
 (void)sprintf(buffer,"PRT_SRVR: printing text file %s for user %s\n",
   imagefilename,(char *)getenv("USER"));
 PrintResponseToSender(TEXTFILEPRINTED,port,address,WINDSComputeHost,buffer);
}

/**********************  PjetPrintColorImage()  ******************************/

PjetPrintColorImage()
/*
take a winds raster format file and send it in graphics mode to the color
PaintJet printer
*/
{
int bit,  	/* bit in current byte to be masked according to pixel value */
    x,y,  	/* coordinates being examined in image */
    index;	/* where in buffer to start writing values */

/* declare buffer unsigned to insure getting the 8th bit for graphics purposes
...this results in lint complaining profusely about "arg. 1 used inconsistently"
in the sprintf()'s, etc., that use buff[] as an argument.  Ignore it.
*/
unsigned char buff[PLANES][PJ_MAX_BYTES_PER_ROW], 
              value;    		/* value of current pixel */
int plane,				/* current color plane */
    bytes_per_row,			/* # of bytes per each row to print */
    depth,
    color,				/* utility index */
    process,				/* type of process */
    y_coord;				/* memory buffer index */


/* open filename as specified on socket
*/
  if ( (file=WINDSfopen(imagefilename,"r"))==NULL) {
    (void)WINDSfclose(file);
    (void)fprintf(stderr,"PRT_SRVR on %s: %s FOPEN of IMAGE_FILENAME file\n",
     PortServerComputeHost,printer_name);
    return;
   } 

/* read in parameters of image 
*/
  fscanf(file,"%d %d %d %d", &width,&height,&depth,&color_table_size);
  for (color=0; color<NUMCOLORS; color++)  {
    fscanf(file,"%d ",&pixel[color]);
    if (debug)
      (void)fprintf(stderr,"color %d = %d ",color,pixel[color]);
    color_table_size=MAX(color_table_size,pixel[color]+1);
  }
  fscanf(file,"%d ",&process);

/* truncate bits beyond byte boundary via integer arithmetic
*/
  bytes_per_row=MIN((width/depth),PJ_MAX_BYTES_PER_ROW);
/* printf("prt_srvr color: bytes/row: %d; %d by %d image to be read...\n", bytes_per_row,width,height);
*/

/* 
configure serial port, initialize printer
*/
  config_port(GRAPHICS);
  init_colors(color_table_size,bytes_per_row);
/* 
since NUMCOLORS is assumed to be larger than NUM_PJ_COLORS, must custom-set
palette for each process according to subset of colors needed
*/
  SetPjetPalette(process);

  /* initialize first byte of buffer (in all planes ) to all white space WHY?
   */
  for (plane=0; plane<PLANES; plane++)
    buff[plane][0]=0;

/* read in image 
*/
  image=(unsigned char *)malloc( (unsigned int)depth*bytes_per_row*height);
  fread(image, depth*bytes_per_row*height, 1, file);

/*printf("read in color image of %d by %d, which = %d pixels\n",
depth*bytes_per_row,height,depth*bytes_per_row*height);
*/
  for (y=0; y<height; y++) {
    bit=depth;
    y_coord=y*bytes_per_row*depth;
    index=0;
    /* MUST send an even number of bytes to printer
     */
    for (x=0; x<bytes_per_row*depth; x++) {
/* 
determine which bit is to be masked next 
*/
      if (depth == 8)
        value = colortable[(int)image[y_coord+x]];
      else
      if (depth == 16)
        value = colortable[(int)((short *)image)[y_coord+x]];
      else
      if (depth == 32)
        value = colortable[(int)((long *)image)[y_coord+x]];
/* 
do all three planes, one row at at time. Leave buff[] unchanged (from
all 0's) on value PJ_WHITE to minimize processing and send
fewest non-zeros to printer to speed printing. Assumption is made here
that image[0] is the same color as the general background color, and
we want to not use ink on that either. 
*/
      if (/***value!=colortable[(int)image[0]] && ***/value!=PJ_WHITE) {
/* 
apply pixel value to all planes of buffer
*/
        for (plane=0; plane<PLANES; plane++) {
          if (value & mask[plane])
            buff[plane][index] |= mask[bit-1];
        }
      }
      if ((bit--)==1)  {  /* leftmost bit of next byte */
        bit=depth;
/* 
initialize time again
*/
        if (++index<bytes_per_row) {
          for (plane=0; plane<PLANES; plane++) 
            buff[plane][index]=0;
        } 
      }
    }
/* 
write out each row in graphics mode 
*/
    for (plane=0; plane<PLANES; plane++) {
     fputs((plane<PLANES-1?top_planes:last_plane),stream);
     fwrite(buff[plane],1,index,stream);
    }
  }


  graphics_done(imagefilename);
  return;

}

/**************************  PjetPrintBWImage()  *****************************/

PjetPrintBWImage()
/*
take a winds raster format file and send it in graphics mode to the 
black-and-white PaintJet printer
*/
{
int bit,        /* bit in current byte to be masked according to pixel value */
    depth,
    x,y,        /* coordinates being examined in image */
    index;      /* where in buffer to start writing values */
/* 
declare buffer unsigned to insure getting the 8th bit for graphics purposes
...this results in lint complaining profusely about "arg. 1 used inconsistently"in the sprintf()'s, etc., that use buff[] as an argument.  Ignore it.
*/
unsigned char buff[PJ_MAX_BYTES_PER_ROW],
              value;                    /* value of current pixel */
int bytes_per_row,                      /* # of bytes per each row to print */
    color,                              /* utility index */
    process,                            /* type of process */
    y_coord;                            /* memory buffer index */
/* 
open filename as specified on socket
*/
   if ( (file=WINDSfopen(imagefilename,"r"))==NULL) {
    (void)WINDSfclose(file);
    (void)fprintf(stderr,"PRT_SRVR on %s: %s FOPEN of IMAGE_FILENAME file\n",
     PortServerComputeHost,printer_name);
    return;
   }
/* 
read in parameters of image
*/
   fscanf(file,"%d %d %d %d ",
           &width,&height,&depth,&color_table_size);
   for (color=0; color<NUMCOLORS; color++) {
    fscanf(file,"%d ",&pixel[color]);
    color_table_size=MAX(color_table_size,pixel[color]);
   }
   fscanf(file,"%d ",&process);
/* 
truncate bits beyond byte boundary via integer arithmetic
*/
   bytes_per_row=MIN((width/depth),PJ_MAX_BYTES_PER_ROW);
/*printf("prt_srvr b/w: bytes/row: %d; %d by %d image to be read...\n",
 bytes_per_row,width,height);
*/
/* 
configure serial port, initialize printer
*/
   config_port(GRAPHICS);
   init_bw(bytes_per_row);
/* 
read in image
*/
   image=(unsigned char *)malloc( (unsigned int)depth*bytes_per_row*height);
/*printf("read in b/w image of %d by %d, which = %d pixels\n",
depth*bytes_per_row,height,depth*bytes_per_row*height);
*/
   (void)fread(&image[0],depth*bytes_per_row,height,file);
 
   for (y=0; y<height; y++) {
/* 
initialize first byte of buffer to all white space
*/
    bit=depth;
    index=0;
    buff[index]=0;
    y_coord=y*bytes_per_row*depth;
/* 
MUST send an even number of bytes to printer
*/
    for (x=0; x<bytes_per_row*depth; x++) {
/* 
determine which bit is to be masked next 
*/
     value=(int)image[y_coord+x];
/* 
Leave buff[] unchanged (from all 0's) on value of PJ_WHITE to minimize 
processing, and send fewest non-zeros to printer to speed printing. Assumption 
is made here that pixel[0] is the same color as the general background color,
and we want to not use any ink on that either.
*/
     if (value!=pixel[0] && value!=PJ_WHITE) {
      buff[index] |= mask[bit-1];
     }
     if ((bit--)==1)  {  /* leftmost bit of next byte */
      bit=depth;
/* 
initialize time again
*/
      if (++index<bytes_per_row) {
        buff[index]=0;
      }
     } 
    } 
/* 
write out each row in graphics mode 
*/
    (void)fputs(raster_print,stream);
    (void)fwrite(buff,1,index,stream);
   }
/* 
done with raster file
*/
   graphics_done(imagefilename);
   return;
}

/***************************  GRAPHICS_DONE()  *******************************/

graphics_done(filename)
char filename[];
/*
send flush command to PaintJet, free malloc's, close/remove files, reset port
to ASCII mode
*/
{
  (void)fputs("\033\*rB",stream);
  (void)fflush(stream);
  (void)free(image);
  if (colortable) {
   (void)free(colortable);
/*
believe it or not, free()'ing colortable does not set it back to nil. Do so
explicitly to preclude SEGV after a b/w print that did not re-allocate it
*/
   colortable=0;
  }
  (void)unlink(filename);
  (void)WINDSfclose(file);
/* 
reset port to normal post-processing
*/
  config_port(ASCII);
}

/****************************  SetPjetPalette()  ******************************/

SetPjetPalette(process)
int process;
/* 
set the color table palette as per process type and printer type. 
*/
{
/* 
set color 7 to black to provide for borders, time stamps, etc.  Set 
colortable[] index to PJ_BLACK, since its value is 7.

ABC are the RGB values, I is the index.  RGB of 4-4-6 is the first color,
90-88-85 is the last.
*/
/* 
HP PaintJet
*/
 if (printer_type==PJET || printer_type==SPJET || printer_type==PPJET
    ) {
  (void)fputs("\033\*v4A\033\*v4B\033\*v6C\033\*v7I",stream);
   colortable[pixel[BLACK]]=PJ_BLACK;
 }
/* 
HP 1200C DeskJet
*/
 if (printer_type==HP1200C || printer_type==PHP1200C) {
  (void)fputs("\033\*v4A\033\*v4B\033\*v6C\033\*v0I",stream);
  colortable[pixel[BLACK]]=PJ_WHITE;
 }
 if (debug)
  (void)fprintf(stderr,"colortable[BLACK-index %d=%d]=PJ_BLACK,%d\n",BLACK,pixel[BLACK],PJ_BLACK);

/* 
set color 0 to white: since buff[] in PjetPrintColorImage() is init'd to
zeroes, and BKGND is the most predominant color, this minimizes the
loop processing in that routine.  Set colortable[] index to PJ_WHITE, since
its value is 0.
*/
/* 
HP PaintJet
*/
 if (printer_type==PJET || printer_type==SPJET || printer_type==PPJET
    ) {
  (void)fputs("\033\*v90A\033\*v88B\033\*v85C\033\*v0I",stream);
   colortable[pixel[BKGND]]=PJ_WHITE;
 }
/* 
HP 1200C DeskJet
*/
 if (printer_type==HP1200C || printer_type==PHP1200C) {
  (void)fputs("\033\*v90A\033\*v88B\033\*v85C\033\*v7I",stream);
  colortable[pixel[BKGND]]=PJ_BLACK;
 }
 if (debug)
  (void)fprintf(stderr,"colortable[BKGND-index %d=%d]=PJ_WHITE,%d\n",BKGND,pixel[BKGND],PJ_WHITE);

/* 
use same colors as provided by printer in 3-plane colormap 
*/
 colortable[pixel[BLU]]=PJ_BLUE;
 if (debug)
  (void)fprintf(stderr,"colortable[BLU-index %d=%d]=PJ_BLUE,%d\n",BLU,pixel[BLU],PJ_BLUE);
 colortable[pixel[GRN]]=PJ_GREEN;
 if (debug)
  (void)fprintf(stderr,"colortable[GRN-index %d=%d]=PJ_GREEN,%d\n",GRN,pixel[GRN],PJ_GREEN);
 colortable[pixel[RED]]=PJ_RED;
 if (debug)
  (void)fprintf(stderr,"colortable[RED-index %d=%d]=PJ_RED,%d\n",RED,pixel[RED],PJ_RED);
 colortable[pixel[VIO]]=PJ_PURPLE;
 if (debug)
  (void)fprintf(stderr,"colortable[VIO-index %d=%d]=PJ_PURPLE,%d\n",VIO,pixel[VIO],PJ_PURPLE);
/* 
set up special colors as per process type 
*/
 switch (process) {
  case DISPTYPE_XYPLOT:
  case WTYPE_SKEWT:
  case WTYPE_CALIBDISP:
/* 
brown 
*/
   (void)fputs("\033\*v31A\033\*v18B\033\*v13C\033\*v3I",stream);
   colortable[pixel[BRN]]=PJ_YELLOW;
   if (debug)
    (void)fprintf(stderr,"colortable[BRN-index %d=%d]=PJ_YELLOW,%d\n",BRN,pixel[BRN],PJ_YELLOW);
/* 
orange too
*/
   colortable[pixel[ORG]]=PJ_YELLOW;
   if (debug)
    (void)fprintf(stderr,"colortable[ORG-index %d=%d]=PJ_YELLOW,%d\n",ORG,pixel[ORG],PJ_YELLOW);
/* 
light green 
*/
   (void)fputs("\033\*v29A\033\*v57B\033\*v28C\033\*v5I",stream);
   colortable[pixel[LTG]]=PJ_MAGENTA;
   if (debug)
    (void)fprintf(stderr,"colortable[LTG-index %d=%d]=PJ_MAGENTA,%d\n",LTG,pixel[LTG],PJ_MAGENTA);
   break;

  case DISPTYPE_PMS2D:
   break;

  case DISPTYPE_PMS1D:
/* 
purple
*/
   colortable[pixel[PUR]]=PJ_PURPLE;
   if (debug)
    (void)fprintf(stderr,"colortable[PUR-index %d=%d]=PJ_PURPLE,%d\n",PUR,pixel[PUR],PJ_PURPLE);
/* 
light blue -- asas probe 
*/
   (void)fputs("\033\*v38A\033\*v58B\033\*v78C\033\*v3I",stream);
   colortable[pixel[CYN]]=PJ_YELLOW;
   if (debug)
    (void)fprintf(stderr,"colortable[CYN-index %d=PJ_YELLOW,%d]=%d\n",CYN,pixel[CYN],PJ_YELLOW);
/* 
non-descript gray -- old data 
*/
   (void)fputs("\033\*v43A\033\*v43B\033\*v45C\033\*v5I",stream);
   colortable[pixel[IND]]=PJ_MAGENTA;
   if (debug)
    (void)fprintf(stderr,"colortable[IND-index %d=PJ_MAGENTA,%d]=%d\n",IND,pixel[IND],PJ_MAGENTA);
   break;
 }
}

/****************************  CONFIG_PORT() ******************************/
 
config_port(mode)
int mode;
/*
set mode to ASCII or GRAPHICS mode
*/
{
struct termios graphics_chars;
 
 if (!Parallel) {
/*
TCGETS ioctl()'s are "inappropriate ioctl for device" on /dev/bpp0
*/
  if (ioctl(PortFD, TCGETS, &graphics_chars)==ERROR) {
   (void)fprintf(stderr,"PRT_SRVR on %s: %s config_port, ioctl TCGETS\n",
      PortServerComputeHost,printer_name);
   perror("PRT_SRVR ioctl");
  }
 }
 if (OutputToPrinter==TRUE && printer_type!=EPSON ) {
/* 
12 char. per inch ==> 200 cps
*/
  (void)fputs("\033\&k4S",stream);
 }
/* 
special config'n for flip type display
*/
 if (printer_type==FLIP) {
  memset ((char*)&graphics_chars, 0, sizeof (struct termios));
  graphics_chars.c_iflag = BRKINT | IGNPAR;
  graphics_chars.c_oflag = 0;
  graphics_chars.c_cflag |= B9600 | CS7 | PARENB | PARODD;
  graphics_chars.c_lflag = ISIG | NOFLSH;
 } else {
/* 
disable handshaking for either ASCII or GRAPHICS
*/
  if (Parallel) {
  } else {
   graphics_chars.c_cflag &= (~CRTSCTS);
  }
/* 
disable or enable post-processing
*/
  if (mode==GRAPHICS) {
   if (Parallel) {
   } else {
    graphics_chars.c_oflag &= (~OPOST);
   }
  } else {
/*
ASCII
*/
   if (Parallel) {
/*
disable raster printing ... this may not be needed ...
*/
    (void)fputs("\033\*r0B",stream);
/*
add CR's to LF's
*/
/***
    (void)fputs("\033\&k1G",stream);
/*
add CR's to LF's, and CR's to FF's
*/
/***
    (void)fputs("\033\&k2G",stream);
/*
add CR's to LF's, LF's to CR's, and CR's to FF's
*/
    (void)fputs("\033\&k3G",stream);
   } else {
/*
serial
*/
    graphics_chars.c_oflag |= (OPOST);
   }
  }
 }
 if (!Parallel) {
/*
TCSETS ioctl()'s are "inappropriate ioctl for device" on /dev/bpp0
*/
  if (ioctl(PortFD, TCSETS, &graphics_chars)==ERROR) {
   (void)fprintf(stderr,"PRT_SRVR on %s: %s config_port, ioctl TCSETS\n",
      PortServerComputeHost,printer_name);
   perror("PRT_SRVR ioctl");
  }
 }
}
 
/***************************  INIT_PORT() *******************************/

init_port(Port,baud)
char *Port;
char baud;
/* 
set protocol parameters and disable post-processing on serial port
*/
{
 PortFD=fileno(stream);
 
 if (printer_type==FLIP)
  return;

 if (!Parallel) {
/* 
set parity, baud rate 
*/
  struct termios	tty;
  speed_t	spd;

  tcgetattr(PortFD, &tty);
  tty.c_cflag &= ~PARENB;
//  tty.c_cflag |= CSTOPB;
  cfsetispeed(&tty, (speed_t)baud);
  cfsetospeed(&tty, (speed_t)baud);
  tcsetattr(PortFD, TCSANOW, &tty);

/****  Gary's old code.

  if (ioctl(PortFD, TIOCGETP, &parity)==ERROR) {
   (void)fprintf(stderr,"PRT_SRVR on %s: %s init_port: ioctl TIOCGETP\n",PortServerComputeHost,Port);
   perror("PRT_SRVR ioctl");
  }
  parity.sg_flags &= (~ O_EVENP );
  parity.sg_flags &= (~ O_ODDP );
  parity.sg_ispeed = baud;
  parity.sg_ospeed = baud;
  if (ioctl(PortFD, TIOCSETP, &parity)==ERROR) {
   (void)fprintf(stderr,"PRT_SRVR on %s: %s init_port: ioctl TIOCSETP\n",PortServerComputeHost,Port);
   perror("PRT_SRVR ioctl");
  }
****/
 }

 if (OutputToPrinter==TRUE) {
  send_reset();
 }

}

/************************  SEND_RESET()  ************************************/

send_reset()
/*
remind printer to get ready
*/
{
/***
 if (!Parallel) {
***/
  if ((write(PortFD,"\033E",strlen("\033E")))==ERROR) {
   (void)fprintf(stderr,"PRT_SRVR on %s: %s send_reset: write; PortFD: %d\n",
    PortServerComputeHost,port,PortFD);
   perror("PRT_SRVR write");
  }
/***
 } else {
  if (stream)
   (void)fputs("\033E",stream);
 }
***/
 if (stream && (OutputToPrinter==TRUE && printer_type!=EPSON))
/* 
12 char. per inch ==> 200 cps
*/
  (void)fputs("\033\&k4S",stream);
 if (printer_type==HP1200C || printer_type==PHP1200C) {
  num1200images=0;
  AtTopOfPage=TRUE;
 }
}

/*************************  INIT_COLORS()  *******************************/

init_colors(size,bytes)
int size,bytes;
/*
allocate and initialize PaintJet lookup color table
*/
{
char string[32];			/* buffer for commands to printer */
/* 
allocate paintjet colormap lookup table.  
*/
 colortable=(int *)malloc((unsigned)size*sizeof(int));
/* 
create graphics escape sequences for multiple color planes 
*/
 sprintf((char *)&top_planes[0],"\033\*b%dV",bytes);
 sprintf((char *)&last_plane[0],"\033\*b%dW",bytes);
/* 
designate PLANES color planes 
*/
 sprintf(string,"\033\*r%dU",PLANES);
 fputs(string,stream);
/* 
set picture width 
*/
 sprintf(string,"\033\*r%dS",bytes*BYTE_LEN);
 fputs(string,stream);
 if (printer_type==HP1200C || printer_type==PHP1200C) {
/*
set to fast paper mode
*/
  fputs("\033\*o-1Q",stream);
 } else {
/* 
move horizontal position 
*/
  sprintf(&string[0],"\033\&a%dH",(1440-(bytes*BYTE_LEN))/2);
  fputs(string,stream);
/* 
set left margin 
*/
  fputs("\033\*r1A",stream);
 }
}

/*************************  INIT_BW()  *******************************/

init_bw(bytes)
int bytes;
/*
initialize PaintJet for black-and-white printing
*/
{
 char string[32];			/* buffer for commands to printer */

/* create graphics escape sequences
*/
 sprintf((char *)&raster_print[0], "\033\*b%dW",bytes);

/* designate 1 color plane 
*/
 fputs("\033\*r1U",stream);

/* set picture width 
*/
 sprintf(string,"\033\*r%dS", bytes*BYTE_LEN);
 fputs(string, stream);
 if (printer_type==HP1200C || printer_type==PHP1200C) {

/* set to fast paper mode
*/
  fputs("\033\*o-1Q", stream);
 } else {
/* move horizontal position 
*/
  sprintf(string,"\033\&a%dH", (1440-(bytes*BYTE_LEN))/2);
  fputs(string, stream);

/* set left margin 
*/
  fputs("\033\*r1A",stream);
 }
}
