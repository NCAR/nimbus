/* 
printers.h  --  parameters describing the serial port-to-PaintJet printer 
network
*/
/*
types of output devices and associated baud rates
*/
#define PJET 0		/* Parallel PaintJet (serial converted to parallel) */
#define PJET_BAUD	B38400
#define HP1200C 1	/* Parallel HP1200C (serial converted to parallel) */
#define HP1200C_BAUD	B38400
#define SPJET 2		/* Serial port PaintJet (serial out) */
#define SPJET_BAUD	B9600
#define EPSON 3		/* Serial port Epson (serial out) */
#define EPSON_BAUD	B19200
#define ASCIIFEED 4		/* Ascii values */
#define ASCII_BAUD	B9600
#define LCD 5		/* Formatted names and values, LCD type display */
#define LCD_BAUD	B2400
#define FLIP 6		/* Formatted names and values, FLIP type display */
#define FLIP_BAUD	B9600
#define PPJET 7		/* Parallel PaintJet (parallel out) */
#define PHP1200C 8	/* Parallel HP1200C (parallel out) */
/*
serial ports
*/
#define NUMSERIALPORTS 2
/*
text or graphics mode 
*/
#define ASCII 0
#define GRAPHICS 1
/*
format for header in image files: print-mode, window, filename, port, address
*/
#define IMAGEHEADERFORMAT "%s %d %s %d %d"
/*
terminate with extreme prejudice
*/
#define PARTYISOVER	"PleaseDieNow"
/*
codes for IPC from printer-server back to winds - must have values other
than 0-4, since formal argument is overloaded in PrintResponseToSender()
call
*/
#define BYEBYE	-1
#define TEXTFILEPRINTED -2
#define PREAMBLE -3
/*
number of rows in header for each page of lists
*/
#define NUM_HEADER_ROWS 6
#define PJ_LIST_ROWS NUM_HEADER_ROWS+60  /* # list rows to print per page */
#define HP1200_LIST_ROWS NUM_HEADER_ROWS+53  /* # list rows to print per page */

#define PJ_MAX_BYTES_PER_ROW 90   /* HP-PaintJet graphics rows */
#define HP9876A_MAX_BYTES_PER_ROW 70  /* according to HP-9876A graphics doc'n */

#define PJ_QUAD_COLOR_TIME  70  /* secs to print quad color image, PaintJet */
#define PJ_QUAD_BW_TIME     25  /* secs to print quad b/w image, PaintJet */
#define PJ_STRIP_COLOR_TIME 60  /* secs to print strip color image, PaintJet */
#define PJ_STRIP_BW_TIME    25  /* secs to print strip b/w image, PaintJet */
#define PJ_STACK_COLOR_TIME 160 /* secs to print stack color image, PaintJet */
#define PJ_STACK_BW_TIME    55  /* secs to print stack b/w image, PaintJet */
#define PJ_FULL_COLOR_TIME  150 /* secs to print full color image, PaintJet */
#define PJ_FULL_BW_TIME     45  /* secs to print full b/w image, PaintJet */
#define STATS_ROW_TIME 0.8     /* approximation of how many seconds to print
                                  one row of statistics */
#define MAX_LEN_ESC 8  /* maximum length of escape sequence  */
/*
statistics
*/
struct statistics {
 int interval,
     printing;
};

#define DEFAULT_STATS_INTERVAL 900   /* # seconds between stats printouts */

/* 
PaintJet colormap indices 
*/
#define PLANES 3
#define NUM_PJ_COLORS 8
#define PJ_WHITE 0
#define PJ_RED 1
#define PJ_GREEN 2
#define PJ_YELLOW 3
#define PJ_PURPLE 4
#define PJ_MAGENTA 5
#define PJ_BLUE 6
#define PJ_BLACK 7
/*
allowed printconfig keywords
*/
/*
types of printconfig file entry values
*/
#define PRINTCONFIGPRINTERNAME	0
#define PRINTCONFIGMODE		1
#define PRINTCONFIGTYPE		2
#define PRINTCONFIGENABLED	3
#define MAXPRINTERNAMELENGTH 	64 /* max lengths of printer name entries in
				      printconfig file */
#define MAXNAMELENGTH	16
/*
enabled or not
*/
#define NUMENABLEDNAMES	2
#define ENABLEDNAME	"enabled"
#define DISABLEDNAME	"disabled"
/*
Printer types available 
*/
#define NUMPRINTERTYPES 3
#define PAINTJET	0
#define PSPRINTER	1
#define TEXTONLY    	2
#define PJETTYPE	"pjet"
#define PSTYPE		"ps"
#define TEXTONLYTYPE	"textonly"
#define DEFAULTTYPE	PSTYPE
/*
printer names known to WINDS but not likely to be found in printcap file
*/
#define PAINTJETNAME    "PaintJet"
#define EPSONNAME    	"Epson"
/* 
name the assumed device suffixes for serial ports on Sun machines 
*/ 
#define PORT1 "ttyS0"
#define PORT2 "ttyS1"
/*
Unix environment variable name for default printer, and conventional names
for PostScript and line printers in printcap if no default printer.
*/
#define PRINTERENV 		"PRINTER"
#define DEFAULTIMAGEPRINTER 	"ps"
#define DEFAULTTEXTPRINTER 	"lp"
/*
print modes
*/
#define NUMIMAGEMODES	3
/* 
this applies only to those printers with interspersed text and images -- 
namely, the PaintJet. Keep STATSPRINT, COLORPRINT and BWPRINT as the first 3 
values, i.e. -1, 0 and 1, so they can be used as arguments in calls to the
SetWaitTimeForImage() routine. 
*/
#define NUM_WAIT_TYPES 3  
#define STATSPRINT	-1
#define BWPRINT		0
#define GREYPRINT	1
#define COLORPRINT	2
#define BWMODE		"bw"
#define GREYMODE	"grey"
#define COLORMODE	"color"
#define DEFAULTMODE   	GREYMODE
/*
color of printed background, which determines the color map used
*/
#define NUMBKGNDVALUES  4
#define ASISBKGND	"as-is"
#define BLUEBKGND	"blue"
#define BLACKBKGND	"black"
#define WHITEBKGND	"white"
#define DEFAULTBKGND	ASISBKGND
/*
orientation
*/
#define NUMORIENTATIONS 3
#define LANDSCAPE	"landscape"
#define PORTRAIT	"portrait"
#define OPTIMAL		"optimal"
#define DEFAULTORIENT	LANDSCAPE
/*
possible values for number of images per page
*/
#define NUMPERPGVALUES	4
#define ONEPERPAGE	"1"
#define TWOPERPAGE	"2"
#define THREEPERPAGE	"3"
#define FOURPERPAGE	"4"
#define DEFAULTNUMPERPAGE ONEPERPAGE
/*
possible values for # of list columns
*/
#define MAXEPSONCHARSPERLINE 136
#define DEFAULTPJETCHARSPERLINE 96
#define MAXPJETCHARSPERLINE 144
#define NUMLISTCOLVALUES 2
#define PAINTJETCOLUMNS  12
#define PAINTJETCOLS	"12"
#define EPSONCOLUMNS	 17
#define EPSONCOLS    	"17"

#define MAXNUMPRINTERS		64

struct printconfig {
 char printername[MAXPRINTERNAMELENGTH];
 char typename[MAXNAMELENGTH];  
 char modename[MAXNAMELENGTH]; 
 char bkgndname[MAXNAMELENGTH]; 
 char orientname[MAXNAMELENGTH];
 int numperpage; 
 int enabled;
 int type;
 int mode;
 int bkgnd;
 int orient;
};
/*
so pick a number that is certain to be big enough for all of the above in 
a single printconfig record
*/
#define MAX_PRINTCONFIG_RECLENGTH 128
/*
keywords for save values file: order of initialization in char array MUST
match ordinal value of associated #defines in panel_menus.h
*/
#define NUMPRINTFIELDS		2
#define MAXKEYWORDLENGTH	32
#define IMAGEMODEKEYWORD 	"ImageMode"
#define IMAGEPRINTERKEYWORD 	"ImagePrinter"
#define TEXTPRINTERKEYWORD 	"TextPrinter"
/*
Printing Menu: #define ordinal values MUST correspond to order of associated
strings declared in panel_menus.c in PrintEntryKeyword[] array (see above
for associated strings).
*/
#define IMAGEPRINTER    0
#define IMAGEMODE       1
#define BACKGROUND      3
#define ORIENTATION     6
#define NUMPERPAGE      7
#define LASTPGFIRST     8
 
#define TEXTPRINTER     2
#define NUMLISTCOLS     4
 
#define PRINTTOFILE     5
/*
PrintConfig Menu
*/
/* 
how many fields to be saved and restored 
*/
#define NUMPRINTCONFIGSAVEFIELDS 3  
/* 
how many fields are displayed in user menu
*/
#define NUMPRINTCONFIGFIELDS 8
#define PRINTERNAME	0
#define PRINTERMODE	1
#define PRINTERTYPE	2
#define ADDPRINTER	3
#define DELPRINTER	4
#define MODPRINTER	5
#define CONFIGPENDING	6
#define CONFIGAPPLY	7
/*
Pending Config Mode Names
*/
#define NUMCONFIGMODES	3
#define MAXCONFIGMODENAMELEN 32
#define EDITPRINTER	0
#define DISABLEPRINTER	1
#define ENABLEPRINTER	2
/*
multiprint operations
*/
#define NUM_MULTIPRINT_TIME_TYPES 3
