/*
VideoInit.c - create and offer for consumption the frame, canvas, and
video objects used by the application
*/
#include    <X11/Xlib.h>
#include <xview/xview.h>
#include "Video.h"
#include "proto.h"
#include "windsvideo.h"

static VideoIn UserVideo[MAXLIVEWINDOWS];

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <XPlxExt.h>

plx_IO *XPlxQueryConfig();
plx_signal *XPlxQueryVideo();

plx_IO *plxio = NULL;
plx_signal *plxsignal = NULL;

int videowidth = 640;
int videoheight = 512;

GetHardwareInfo()
{
        Window win;
        Display *disp;
        int scr, mask;
        GC gc;
        XSetWindowAttributes attr;
        XVisualInfo vinfo;
        int depth = 32;
 
 
/*      Connect your client program with the X server   */
        disp = XOpenDisplay(getenv("DISPLAY"));
 
/*      Get a default screen                            */
        scr = (int) XDefaultScreen(disp);
 
        if (strncmp(XServerVendor(disp),"X11/NeWS",8) == 0)
            depth = 24;

/*      Find the visual that matches the desired window depth if
        the window depth is different from the root depth */
        XMatchVisualInfo(disp, scr, depth, TrueColor, &vinfo);
 
/*      WARNING!!! Must set up the colormap, the background_pixel and
        the border_pixel to make it work when the window depth is
        different from the root depth */
        mask = CWBackPixel | CWColormap | CWBorderPixel;
        attr.colormap = XCreateColormap(disp, XRootWindow(disp, scr),
                vinfo.visual, AllocNone);
        attr.background_pixel = BlackPixel(disp, DefaultScreen(disp));
        attr.border_pixel = WhitePixel(disp, DefaultScreen(disp));
 
/*      Create the drawable for the XPlx calls          */
        win = XCreateWindow(disp, RootWindow(disp, scr),
                0, 0, videowidth, videoheight, 1, depth,
                InputOutput, vinfo.visual, mask, &attr);

/*      WARNING!!! Must create the gc with the window */
        gc = XCreateGC(disp, win, 0, 0);

XFlush(disp);


/*      Query the hardware for configuration parameters */
        plxio = XPlxQueryConfig(disp, win, gc);

}

/************************* VideoInstalledOnHost() ****************************/

VideoInstalledOnHost() 
/*
this MUST be redone before it can be released on outside distributions.
This is only a stopgap solution until the right approach can be determined.
Hardwire the known hosts that have a Parallax board installed.
*/
{
#define VIDEOHOSTNAMELENGTH 16
#define NUMVIDEOHOSTS 5
static char videohosts[NUMVIDEOHOSTS][VIDEOHOSTNAMELENGTH]={
/* 
want to eventually change this to exploit the NetVUE entry in netconfig file
*/
"scraff","jackpot","solano","salsa","samba"
};
int host;

 for (host=0; host<NUMVIDEOHOSTS; host++) {
  if (!strcmp(videohosts[host],(char *)GetDisplayHost()))
   return TRUE;
 }
 return FALSE;
/*** 
Using "rsh <host> file /dev/fbs/tvtwo0" is not acceptable because it is VERY 
slow and, when frame buffer IS found, requires tty input to continue for 
some unknown reason.
***/
}

/**************************** CreateVideo() ********************************/

CreateVideo(canvas,videosource)
Canvas canvas;
int videosource;
{
 if (!VideoInstalledOnHost()) {
  SetVideoErrorStatus(VIDEO_UNKNOWN);
  return 0;
 }
 if (UserVideo[videosource]) {
  SetVideoErrorStatus(VIDEO_CHANNEL_IN_USE);
  return 0;
 }
 UserVideo[videosource] = xv_create(canvas,VIDEO_IN,
  VIDEO_SAVE_STILL, TRUE,
  XV_DEPTH,24,
 NULL);
 if (UserVideo[videosource]==(VideoIn)NULL)
  DestroyVideo(videosource);
 return UserVideo[videosource];
}

/**************************** DestroyVideo() ********************************/

DestroyVideo(videosource)
int videosource;
{
 xv_destroy_safe( UserVideo[videosource] );
 UserVideo[videosource]=0;
}

/**************************** GetUserVideo() ********************************/

VideoIn
GetUserVideo(videosource)
int videosource;
/*
return the videosource'th Video object created in CreateUserVideo()
*/
{
 return UserVideo[videosource];
}
