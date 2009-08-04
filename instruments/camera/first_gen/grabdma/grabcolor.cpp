// Look for modifications by Stuart Beaton throughout for G5 Flea camera
// Modified 2006-02-22

/**************************************************************************
 *
 * Title: 	grabcolor
 * Copyright: 	(C) 2005 Don Murray  <donm@ptgrey.com>
 * Description:
 *    Get one color image using libdc1394 and store it as portable pixel map
 *    (ppm). Based on 'grab_gray_image' from libdc1394 examples.
 *    
 *-------------------------------------------------------------------------
 *     License: LGPL
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *************************************************************************/


#include <stdio.h>
#include <libraw1394/raw1394.h>
#include <libdc1394/dc1394_control.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#include "conversions.h"

/*===== User specific configuration ====*/

/* You will need to modify the configuration below to match your camera and setup */

/* Define one of these descriptors.
 * More formats and modes are available, but these "most common" configurations
 * examples are provided.
 *
 * (Note: this example was initially written for Point Grey Research cameras.
 * So "most common" means "most common with PGR cameras".
 * Typical settings for Dragonfly, Flea or Scorpion cameras would be 8 bit mode.
 * Bumblebee cameras transmit in 16 bit mode.)
 *
 */

// SB -2006-02-22: Chose CAMERA_1024x768_8BIT definition
#define CAMERA_1024x768_8BIT
// #define CAMERA_1024x768_16BIT
// #define CAMERA_640x480_8BIT
// #define CAMERA_640x480_16BIT

#ifdef CAMERA_1024x768_8BIT
   int format 		= FORMAT_SVGA_NONCOMPRESSED_1;
   int mode		= MODE_1024x768_MONO;
   int frameRate	= FRAMERATE_15;
   int bytesPerPixel	= 1;
#endif

#ifdef CAMERA_1024x768_16BIT
   int format 		= FORMAT_SVGA_NONCOMPRESSED_1;
   int mode		= MODE_1024x768_MONO16;
   int frameRate	= FRAMERATE_15;
   int bytesPerPixel	= 2;
#endif

#ifdef CAMERA_640x480_8BIT
   int format 		= FORMAT_VGA_NONCOMPRESSED;
   int mode		= MODE_640x480_MONO;
   int frameRate	= FRAMERATE_30;
   int bytesPerPixel	= 1;
#endif

#ifdef CAMERA_640x480_16BIT
   int format 		= FORMAT_VGA_NONCOMPRESSED;
   int mode		= MODE_640x480_MONO16;
   int frameRate	= FRAMERATE_30;
   int bytesPerPixel	= 2;
#endif

/* 
 * Define one of the cards below for use.  If your computer has only one 1394 interface
 * card you should be fine with CARD_0.  If not, run testlibraw and see which of your
 * interfaces has your camera on it, then define that one.
 */

#define USE_CARD_0
//#define USE_CARD_1
//#define USE_CARD_2
//#define USE_CARD_3

#ifdef USE_CARD_0
   int nCard		= 0;
   const char* device	= "/dev/video1394/0";
#endif
#ifdef USE_CARD_1
   int nCard		= 1;
   const char* device	= "/dev/video1394/1";
#endif
#ifdef USE_CARD_2
   int nCard		= 2;
   const char* device	= "/dev/video1394/2";
#endif
#ifdef USE_CARD_3
   int nCard		= 3;
   const char* device	= "/dev/video1394/3";
#endif


/*===== Global Variables ====*/

raw1394handle_t 	raw1394Handle;
dc1394_cameracapture 	dc1394Camera;
bool			bRaw1394HandleCreated = false;
bool			bDc1394CameraCreated = false;
const char*		saveFileName = "image.ppm";
int			imageCounter;

//SB -2006-22-23 file name for time-stamped image;
char  savedImageName[256];

unsigned char* rgbBuffer = new unsigned char[3 * 1024*768]; // SB - fixed dimensions

// SB 2006-02-22 The jpeg include has to be after the global variables definition.
#include "jpeg.c"

/*==== cleanup()
 * This is called when the program exits and destroys the existing connections to the
 * 1394 drivers
 *====*/
void 
cleanup( void ) 
{
   if ( bDc1394CameraCreated )
   {
      dc1394_dma_unlisten( raw1394Handle, &dc1394Camera );
      dc1394_dma_release_camera( raw1394Handle, &dc1394Camera );
   }

   if ( bRaw1394HandleCreated )
   {
      raw1394_destroy_handle( raw1394Handle );
   }
}

int main(int argc, char *argv[]) 
{
//   FILE* 	imagefile;

// SB Image acq. (or save) time and file pointer.
   struct timeval currentTime, captureTime;
   struct tm 	tmCaptureTime;
   int 		yymmdd, hhmmss;
   double	capture_ms, current_ms;

   int		saveDisplayFile = FALSE; // Set true to save currentImage.jpg

   /*-----------------------------------------------------------------------
    * Open ohci and assign handle
    * Note: the variable specifies which 1394 card to open.  If you have multiple cards
    * you may want to try with (1) or (2) etc.
    *-----------------------------------------------------------------------*/
   raw1394Handle = dc1394_create_handle( nCard );
   if ( raw1394Handle == NULL )
   {
      fprintf( stderr, "Unable to aquire a raw1394 handle\n\n"
	       "Please check \n"
	       "  - if the kernel modules `ieee1394',`raw1394' and `ohci1394' are loaded\n"
	       "  - if you have read/write access to /dev/raw1394\n" );
      return 1;
   }
   bRaw1394HandleCreated = true;
   
   
   /*-----------------------------------------------------------------------
    *  get the camera nodes and describe them as we find them
    *-----------------------------------------------------------------------*/
   int numNodes;
   numNodes = raw1394_get_nodecount( raw1394Handle );

   nodeid_t * 	cameraNodes;
   int		numCameras;
   /* Note: set 3rd parameter to 0 if you do not want the camera details printed */
   cameraNodes = dc1394_get_camera_nodes( raw1394Handle, &numCameras, 1 );
   fflush( stdout );
   if ( numCameras < 1 )
   {
      /* if you get this message and you have multiple 1394 cards you may want to try
       * modifying the input parameter for dc1394_create_handle() above
       */
      fprintf( stderr, 
	       "No cameras found! (%d nodes on the bus)\n"
	       "  - could be you need to try a different 1394 device (modify code to fix)\n", 
	       numNodes );
      cleanup();
      return 1;
   }
   printf("Working with the first camera on the bus\n");
   
   /*-----------------------------------------------------------------------
    *  to prevent the iso-transfer bug from raw1394 system, check if
    *  camera is highest node. For details see 
    *  http://linux1394.sourceforge.net/faq.html#DCbusmgmt
    *  and
    *  http://sourceforge.net/tracker/index.php?func=detail&aid=435107&group_id=8157&atid=108157
    *-----------------------------------------------------------------------*/
   if( cameraNodes[0] == numNodes-1)
   {
      fprintf( stderr, "\n"
	       "Sorry, your camera is the highest numbered node\n"
	       "of the bus, and has therefore become the root node.\n"
	       "The root node is responsible for maintaining \n"
	       "the timing of isochronous transactions on the IEEE \n"
	       "1394 bus.  However, if the root node is not cycle master \n"
	       "capable (it doesn't have to be), then isochronous \n"
	       "transactions will not work.  The host controller card is \n"
	       "cycle master capable, however, most cameras are not.\n"
	       "\n"
	       "The quick solution is to add the parameter \n"
	       "attempt_root=1 when loading the OHCI driver as a \n"
	       "module.  So please do (as root):\n"
	       "\n"
	       "   rmmod ohci1394\n"
	       "   insmod ohci1394 attempt_root=1\n"
	       "\n"
	       "for more information see the FAQ at \n"
	       "http://linux1394.sourceforge.net/faq.html#DCbusmgmt\n"
	       "\n");
      cleanup();
      return 1;
   }
   
   /*-----------------------------------------------------------------------
    *  setup capture
    *-----------------------------------------------------------------------*/
   unsigned int channel;
   unsigned int speed;
   if ( dc1394_get_iso_channel_and_speed( raw1394Handle, 
					  cameraNodes[0],
					  &channel, 
					  &speed ) !=DC1394_SUCCESS )  
   {
      fprintf( stderr, "Unable to get the iso channel number\n" );
      cleanup();
      return 1;
   }

   // note: format, mode, frameRate and bytesPerPixel are all defined as globals 
   // in the header

   /* Setup the capture mode */
   int e = dc1394_dma_setup_capture( raw1394Handle, 
				     cameraNodes[0],
				     channel,		
				     format,
				     mode,
				     SPEED_200, 
				     frameRate,
				     8,	// number of buffers
				     1,	// drop frames
				     device,
				     &dc1394Camera );
   if ( e != DC1394_SUCCESS )
   {
      fprintf( stderr,"Unable to setup camera-\n"
	       "check code above line %d of %s to make sure\n"
	       "that the video mode,framerate and format are\n"
	       "supported by your camera\n",
	       __LINE__,__FILE__);
      cleanup();
      return 1;
   }
   bDc1394CameraCreated = true;
   
// SB: 2007-10-29 - Set white balance (resets with poweroff)
   dc1394_set_white_balance(raw1394Handle, dc1394Camera.node, 75, 75);

// SB: 2008/-04-15 - Set gamma to non-unity. 0.5=512, 1=1024, 2=2048, etc.
   dc1394_set_gamma(raw1394Handle, dc1394Camera.node, 1843); // Gamma=1.8

   /*-----------------------------------------------------------------------
    *  have the camera start sending us data
    *-----------------------------------------------------------------------*/
   if ( dc1394_start_iso_transmission( raw1394Handle, dc1394Camera.node )
	!=DC1394_SUCCESS ) 
   {
      fprintf( stderr, "Unable to start camera iso transmission\n" );
      cleanup();
      return 1;
   }

// SB: 2006-02-22 - Following (through 'case') taken out of loop.
   /*-----------------------------------------------------------------------
    *  query the camera to determine the Bayer pattern
    *-----------------------------------------------------------------------*/

   quadlet_t qValue;
   GetCameraControlRegister( raw1394Handle, 
			     cameraNodes[0],
			     0x1040,		// Bayer Tile Mapping register
			     &qValue );
   
  bayer_pattern_t pattern = BAYER_PATTERN_BGGR;
  switch( qValue )
  {
     case 0x42474752:	/* BGGR */
	 pattern = BAYER_PATTERN_BGGR;
	 break;
     case 0x47524247:	/* GRBG */
	 pattern = BAYER_PATTERN_GRBG;
	 break;
     case 0x52474742:  /* RGGB */
	 pattern = BAYER_PATTERN_RGGB;
	 break;
     case 0x47425247:	/* GBRG */
	 pattern = BAYER_PATTERN_GBRG;
	 break;
     case 0x59595959:	/* YYYY = BW */
	 fprintf( stderr, "Camera is black and white\n" );
	 cleanup();
	 return 1;
     default:
	 fprintf( stderr, 
		  "Camera BAYER_TILE_MAPPING register has an unexpected value:\n"
		  "\t0x%x\n", qValue );
	 cleanup();
	 return 1;
   }

// SB 2006-02-22 - ADD LOOP
imageCounter = 0;
while (1) {
   /*-----------------------------------------------------------------------
    *  capture one frame to save to file
    *-----------------------------------------------------------------------*/
   if (dc1394_dma_single_capture( &dc1394Camera )!=DC1394_SUCCESS) 
   {
      fprintf( stderr, "Unable to capture a frame\n");
      cleanup();
      return 1;
   }

   gettimeofday(&captureTime, NULL);
   imageCounter++;
   int bufferSize = dc1394Camera.frame_width * dc1394Camera.frame_height;

// SB 2006-02-22 - moved following line out of loop
//   unsigned char* rgbBuffer = new unsigned char[3 * bufferSize];

   unsigned char* src = (unsigned char*) dc1394Camera.capture_buffer;
   if ( bytesPerPixel > 1 )
   {
      src = new unsigned char[ bufferSize ];
      if ( !src )
      {
      fprintf( stderr, 
	       "Could not allocate copy buffer for color conversion (size = %d)\n",
	       bufferSize );
      cleanup();
      return 1;
      }
      unsigned char* captureBuffer = (unsigned char*) dc1394Camera.capture_buffer;
      for ( int i = 0; i < bufferSize; i++ )
      {
	 src[i] = captureBuffer[ i * bytesPerPixel ];
      }
   }

   /*-----------------------------------------------------------------------
    *  convert to color image
    *-----------------------------------------------------------------------*/
   BayerEdgeSense( src, 
		   rgbBuffer, 
		   dc1394Camera.frame_width,
		   dc1394Camera.frame_height,
		   pattern );

   /*-----------------------------------------------------------------------
    *  save image 
    *-----------------------------------------------------------------------*/
// SB - create unique image file name: "yymmdd-hhmmss.ppm"
   tmCaptureTime = *gmtime(&(captureTime.tv_sec));
   yymmdd = (tmCaptureTime.tm_year-100)*10000 + (tmCaptureTime.tm_mon+1)*100 + tmCaptureTime.tm_mday;
   hhmmss = tmCaptureTime.tm_hour * 10000 + tmCaptureTime.tm_min*100 + tmCaptureTime.tm_sec;
   memset(savedImageName, 0x0, 256);
   sprintf(savedImageName, "%06d-%06d.jpg", yymmdd, hhmmss);

/* Skip writing the ppm file, only do the jpeg file.
   imagefile = fopen( saveFileName, "w" );
   if( imagefile == NULL)
   {
      perror("");
      fprintf( stderr, "Can't create '%s'\n", saveFileName );
      cleanup();
      return 1;
   }

   fprintf( imagefile,
	    "P6\n%u %u 255\n", 
	    dc1394Camera.frame_width,
	    dc1394Camera.frame_height );
   fwrite( rgbBuffer, 3,
	   bufferSize, imagefile );
   fclose( imagefile );

   printf( "saved image to '%s'\n", saveFileName );
*/

// Compress and write jpeg file here.
   write_JPEG_file(savedImageName, 80);
   printf( "Saved image %d, '%s'\n", imageCounter, savedImageName);

   if (saveDisplayFile) {
   // Write the jpeg file for realtime display.
   // There may be a tradeoff between compression level and time to write file.
      write_JPEG_file("currentImage.jpg", 40);
   }

   dc1394_dma_done_with_buffer( &dc1394Camera );
// SB 2006-02-22 delay before re-looping, and end loop.
// dc1394_dma_done_with_buffer must remain in loop!   

   capture_ms = (captureTime.tv_sec)*1000.0 + (captureTime.tv_usec)/1000.0;
   gettimeofday( &currentTime, NULL );
   current_ms = (currentTime.tv_sec)*1000.0 + (currentTime.tv_usec)/1000.0;

   while(1000 > current_ms - capture_ms) { // Time since last picture, in ms
      if (8 < captureTime.tv_usec/100000) { // Within 0.1 of seconds rollover
	break;
	}
      usleep (100000); // sleep 0.1  sec
      gettimeofday( &currentTime, NULL );
      current_ms = (currentTime.tv_sec)*1000.0 + (currentTime.tv_usec)/1000.0;
    }
}	// Loop for another picture.
   /*-----------------------------------------------------------------------
   *  Stop data transmission
   *-----------------------------------------------------------------------*/
   if ( dc1394_stop_iso_transmission( raw1394Handle,dc1394Camera.node ) != DC1394_SUCCESS ) 
   {
      printf("Couldn't stop the camera?\n");
   }
   
    /*-----------------------------------------------------------------------
    *  Close camera
    *-----------------------------------------------------------------------*/
   cleanup();
   return 0;
}

/*************************************************************************
 *
 * $RCSfile: grabcolor.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2005/04/07 21:05:14 $
 * $Log: grabcolor.cpp,v $
 * Revision 1.5  2005/04/07 21:05:14  donm
 * [1] fixed a bug in grabdma - it was using FRAMERATE_120 which was wrong
 * [2] added a user-configuration section that includes specifying which
 *     interface card the user is using
 *
 * Revision 1.4  2005/03/15 19:32:41  donm
 * updating the LGPL notice etc
 *
 *
 *************************************************************************/
