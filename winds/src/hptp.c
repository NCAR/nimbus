/* hptp.c -- print server for the HP9876A Thermal Printer.  Assumes 
   following dip switch settings at EAZY 232-488 interface:

SW3	10101011
SW2	10011101
SW1	10001101

   ...where 1 is up and 0 is down.
*/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/termios.h>
#include <sys/ttold.h>

#include <colors.h>
#include <constants.h>
#include <socket.h>
#include <lengths.h>
#include <printers.h>
#include <network.h>
#include <files.h>

FILE *file,				/* image file */
     *stream,				/* output serial port */
     *fopen();		
int port_fd; 				/* serial port file descriptor */
int pixel[NUMCOLORS],			/* color index table from XView side */
    height,width,			/* dimensions of image */
    table_size;				/* max. size needed for color indices */
short int mask[BYTE_LEN]= {  		/* or'ing values */
1, 2, 4, 8, 16, 32, 64, 128
};
unsigned char raster_print[MAX_LEN_ESC];
char errmsg[MAX_MSG_LNGTH];             /* error messages to error log server */
char *port;
char *get_port();
char kill_file[MAX_FLNM_LNGTH];         /* suicide script for killing pjet */
char display_host[MAXHOSTNAMELEN],      /* host on which hptp is running */
     *data,				/* read from socket */
     *winds_path,			/* home dir for WINDS */
     *abs_filename;                     /* absolute filename of image file */
int display_number;                     /* as per net_config file */


main()
/* loop infinitely waiting for list/stats input on a socket and
   looking for new image files in assigned directory.  It is assumed that
   hptp services both ascii and raster requests for printing to an 
   HP9876A printer.
*/
{
/* each host display station has one hptp process running if an HP Thermal 
   Printer is attached.  This process services any image files appearing in the
   directory named as function of local host.
*/
 (void)gethostname(display_host,MAXHOSTNAMELEN);
 if (strchr(display_host, '.'))
   *(strchr(display_host, '.')) = '\0';

 winds_path=(char *)getenv("WINDS");

/* determine which port to use.  If none, done
*/
 display_number=get_entry_by_name(display_host);
 if (( port=(char *)get_port("hptp"))==NULL) {
  close_netconfig(BY_NAME);
  write_to_socket(ERROR_LOG,"hptp not entered in netconfig",29);
  exit(ERROR);
 };
 close_netconfig(BY_NAME);


 init_port(port,B19200);
 config_port(ASCII);
 if ( (stream=fopen(port,"w"))==NULL) {
  (void)fclose(stream);
  (void)sprintf(errmsg,"FOPEN of %s for hptp",port);
  write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
  exit(ERROR);
 } 

/* provide a socket for clients
*/
 switch (bind_socket(HP9876SOCKET,HP9876SOCKET,BLOCKING)) {
  case EWRONGPORT:
   (void)sprintf(errmsg,
   "WARNING: port # for HPTP server not same as requested!!\n");
   write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
   exit(ERROR);
  case EBADOPEN:
   (void)sprintf(errmsg,"hptp opening stream socket");
   write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
   exit(ERROR);
  case EBADBIND:
   (void)sprintf(errmsg,"hptp binding stream socket");
   write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
   exit(ERROR);
  case EBADNAME:
   (void)sprintf(errmsg,"hptp getting socket name");
   write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
   exit(ERROR);
  case BIND_OK:
   break;
 }

/* connect to error log socket
*/
switch (make_client_socket(ERROR_LOG,ERROR_LOG+display_number,display_host)) {
  case EBADOPEN:
   perror("opening stream socket");
   exit(ERROR);
  case EBADHOST:
   (void)fprintf(stderr,"HPTP: %s: unknown host\n",display_host);
   exit(ERROR);
  case CLIENT_OK:
   break;
 }

/* create suicide script, used by prt_srvrs script to kill this process when
   prt_srvrs wants to start a new one just like it.
*/
 (void)sprintf(&kill_file[0],KILL_FILE,winds_path,display_host,"hptp");
 if ( (file=fopen(kill_file,"w"))==NULL) {
  (void)sprintf(errmsg,"WARNING: problem on HPTP FOPEN of KILL_FILE file");
  write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
 } else {
  (void)fprintf(file,"#\nkill -9 %d\n",getpid());
  fclose(file);
 }

/* wait for data
*/
 (void)sprintf(errmsg,"HPTP waiting on data on %s",display_host);
 write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
 do {
  int length;
  length=MAX_FLNM_LNGTH;
  data=(char *)read_from_socket(HP9876SOCKET,&length,BLOCKING);
  if (!(strcmp(data,"bw_image"))) {
   length=MAX_FLNM_LNGTH;
   abs_filename=(char *)read_from_socket(HP9876SOCKET,&length,BLOCKING);
   print_image();
  } else 
   (void)fputs(data,stream);
 } while (1);
 close_socket(HP9876SOCKET);
 (void)close(port_fd);
 (void)fclose(stream);
 exit(0);
}

/**************************  PRINT_IMAGE()  **********************************/


print_image()
{
int bit,  	/* bit in current byte to be masked according to pixel value */
    x,y,  	/* coordinates being examined in pixwin */
    index;	/* where in buffer to start writing values */

/* declare buffer unsigned to insure getting the 8th bit for graphics purposes
...this results in lint complaining profusely about "arg. 1 used inconsistently"
in the sprintf()'s, etc., that use buff[] as an argument.  Ignore it.
*/
unsigned char buff[HP9876A_MAX_BYTES_PER_ROW], 
              value;    		/* value of current pixel */
char command[MAX_FLNM_LNGTH], 		/* system call: get directory listing */
     *image;				/* memory buffer for image file */
int bytes_per_row,			/* # of bytes per each row to print */
    color,				/* utility index */
    process,				/* type of process */
    y_coord;				/* memory buffer index */


/* open filename as specified on socket
*/
   if ( (file=fopen(abs_filename,"r"))==NULL) {
    (void)fclose(file);
    (void)sprintf(errmsg,"HPTP FOPEN of IMAGE_FILEDIR file");
    write_to_socket(ERROR_LOG,"FOPEN of IMAGE_FILEDIR file",25);
    return;
   } 

/* read in parameters of image 
*/
   (void)fscanf(file,"%d %d %d ",
           &width,&height,&table_size);
   for (color=0; color<NUMCOLORS; color++)  
    (void)fscanf(file,"%d ",&pixel[color]);
   (void)fscanf(file,"%d ",&process);

/* truncate bits beyond byte boundary via integer arithmetic
*/
   bytes_per_row=MIN((width/BYTE_LEN),HP9876A_MAX_BYTES_PER_ROW);

/* configure serial port, initialize printer  
*/
   config_port(GRAPHICS);
   init_bw(bytes_per_row);

/* read in image 
*/
   image=(char *)malloc( (unsigned int)BYTE_LEN*bytes_per_row*height);
   (void)fread(&image[0],BYTE_LEN*bytes_per_row,height,file);

   for (y=0; y<height; y++) {

/* initialize first byte of buffer to all white space 
*/
    bit=BYTE_LEN;
    index=0;
    buff[index]=0;
    y_coord=y*bytes_per_row*BYTE_LEN;

    for (x=0; x<bytes_per_row*BYTE_LEN; x++) {

/* determine which bit is to be masked next */

     value=(int)image[y_coord+x];

/* Leave buff[] unchanged (from
   all 0's) on value of pixwl[0] to minimize processing, and send
   fewest non-zeros to printer to speed printing 
*/
     if (value!=pixel[0]) {
      buff[index] |= mask[bit-1];
     }
     if ((bit--)==1)  {  /* leftmost bit of next byte */
      bit=BYTE_LEN;
/* initialize time again
*/
      if (++index<bytes_per_row) {
        buff[index]=0;
      }
     }
    }

/* write out each row in graphics mode */

    (void)fputs(raster_print,stream);
    (void)fwrite(buff,1,index,stream);
   }

/* cleanup: 

   reset form perforation advances to default of 13 mm.  
*/
   (void)fputs("\033&l13T",stream);

/* done with raster file
*/
   (void)fclose(file);
   (void)unlink(abs_filename);

/* reset port to normal post-processing
*/
   config_port(ASCII);
   return;
}

/****************************  CONFIG_PORT() ******************************/

config_port(mode)
int mode;
{
struct termios graphics_chars;

 if (ioctl(port_fd, TCGETS, &graphics_chars)==ERROR) {
  (void)sprintf(errmsg,"HPTP config_port, ioctl TCGETS");
  write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
 }
 if (mode==GRAPHICS)
  graphics_chars.c_oflag &= (~OPOST);
 else
  graphics_chars.c_oflag |= (OPOST);
 if (ioctl(port_fd, TCSETS, &graphics_chars)==ERROR) {
  (void)sprintf(errmsg,"HPTP config_port, ioctl TCSETS");
  write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
 }
}

/***************************  INIT_PORT() *******************************/

init_port(port,baud)
char *port;
char baud;
/* disable post-processing on serial port
*/
{
struct sgttyb  parity;

 if ((port_fd=open(port,O_WRONLY))==ERROR) {
  (void)sprintf(errmsg,"problem with HPTP port open");
  write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
 }
 
/* set parity, baud rate */
 
 if (ioctl(port_fd, TIOCGETP, &parity)==ERROR) {
  (void)sprintf(errmsg,"HPTP INIT_PORT: ioctl TIOCGETP");
  write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
 }
 parity.sg_flags &= (~ O_EVENP );
 parity.sg_flags &= (~ O_ODDP );
 parity.sg_ispeed = baud;
 parity.sg_ospeed = baud;
 if (ioctl(port_fd, TIOCSETP, &parity)==ERROR) {
  (void)sprintf(errmsg,"HPTP INIT_PORT: ioctl TIOCSETP");
  write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
 }
 
 if ((write(port_fd,"\033E",strlen("\033E")))==ERROR) {
  (void)sprintf(errmsg,"HPTP INIT_PORT: write");
  write_to_socket(ERROR_LOG,errmsg,strlen(errmsg));
 }
}

/*************************  INIT_BW()  *******************************/

init_bw(bytes)
int bytes;
{
/* create graphics escape sequences 
*/
 (void)sprintf((char *)&raster_print[0],"\033\*b%dW",bytes);

/*  go to top of form and disable form perforation advances 
*/
 fputs("\f\033&l0T",stream);
}
