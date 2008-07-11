/*
VideoOps.c - offer functions that operate on and return information
about the video image
*/
#include    <X11/Xlib.h>
#include <xview/xview.h>
#include "Video.h"
#include "proto.h"
#include "windsvideo.h"
#include "constants.h"

/****************************** GetVideoImage() *****************************/

static XImage *image;

XImage *
GetVideoImage(videosource) 
int videosource;
{
VideoIn video;

 video=GetUserVideo(videosource);
 SetVideoLive(videosource,FALSE);
 image=(XImage *)XGetAttr(video,VIDEO_XIMAGE);
 SetVideoLive(videosource,TRUE);
 return image;
}

/****************************** DestroyVideoImage() *****************************/

DestroyVideoImage()
{
 (void)XDestroyImage(image);
}

/****************************** GetVideoLive() *****************************/

int
GetVideoLive(videosource)
/*
return 1 if video is live, 0 if still
*/
{
 if (GetUserVideo(videosource))
  return XGetAttr((int)GetUserVideo(videosource),VIDEO_LIVE);
 else
  return FALSE;
}

/****************************** SetVideoLive() *****************************/

void
SetVideoLive(videosource,live)
int videosource,live;
/*
set video to live if argument live is non-zero, else set video to still
*/
{
 if (GetUserVideo(videosource))
  XSetAttr((int)GetUserVideo(videosource),VIDEO_LIVE,live,NULL);
}

/****************************** GetVideoChannel() *****************************/

int
GetVideoChannel(videosource)
int videosource;
/*
return the value of the current input channel, i.e. either VIDEO_IN_ONE or
VIDEO_IN_TWO
*/
{
 if (GetUserVideo(videosource))
  return XGetAttr((int)GetUserVideo(videosource),VIDEO_CHANNEL);
 else
  return ERROR;
}

/****************************** SetVideoChannel() *****************************/

void
SetVideoChannel(videosource,newchannel)
int videosource,newchannel;
/*
if the requested input channel newchannel is different from the current 
input channel, attempt to honor request to change to
new input channel. First set video from live to still if not already
stilled; if a successful switch of channels is made, set video to
original live/still status. If switch is unsuccessful (requested channel
may not have a signal present), set channel back to original and restore
value of panel button that made the request to begin with.
*/
{
int video,livevideo[MAXLIVEWINDOWS];

 if (!GetUserVideo(videosource)) {
  if (!CreateVideo(videosource,newchannel))
   return;
 }
/*
save current status
*/
 if (newchannel == GetVideoChannel(videosource))
  return;
 for (video=0; video<MAXLIVEWINDOWS; video++) {
  livevideo[video] = GetVideoLive(video);
  if (livevideo[video])
/*
live video must be frozen for this to work. Moreover, we're freezing 
BOTH video windows. This precludes the ugly glitch introduced when both
windows are on the same channel and one attempts to switch to the other
channel which has no signal; in the subsequent reset of that failed 
channel switch to original channel, the other window glitches for some
reason. With both windows frozen on ANY attempt to switch channels on
EITHER window, that glitch is precluded BUT there is still a noticeable
flash on both windows as the freeze/unfreeze occurs. This is judged to
be the lesser of two evils for now... 
*/
   SetVideoLive(video,FALSE);
 }
 if (XSetAttr((int)GetUserVideo(videosource),VIDEO_CHANNEL,newchannel,NULL)==XV_ERROR) {
/*
check with error status of video segment
*/
  CheckVideoStatus();
 }
/*
reset video live values
*/
 for (video=0; video<MAXLIVEWINDOWS; video++) {
  if (livevideo[video])
   SetVideoLive(video,TRUE);
 }
}

/*************************** GetFirstChannelValue() ***************************/

int
GetFirstChannelValue() 
/*
translation routine to facilitate information transfer to modules that
don't know about XVideo #defines
*/
{
 return (int)VIDEO_IN_ONE;
}

/************************** GetSecondChannelValue() ****************************/

int
GetSecondChannelValue()
/*
translation routine to facilitate information transfer to modules that
don't know about XVideo #defines
*/
{
 return (int)VIDEO_IN_TWO;
}

/************************** GetVideoHeight() ****************************/

GetVideoHeight() 
{
 return XGetAttr(GetUserVideo(0),VIDEO_HEIGHT);
}

/************************** GetVideoWidth() ****************************/

GetVideoWidth() 
{
 return XGetAttr(GetUserVideo(0),VIDEO_WIDTH);
}

/************************** GetVideoHardware() ****************************/

GetVideoHardware() 
{
 switch (XGetAttr(GetUserVideo(0),VIDEO_HARDWARE)) {
  case VIDEO_VIPER:
   (void)fprintf(stderr,"VIDEO_VIPER installed\n");
   return OK;
  case VIDEO_VIDEOVIEW:
   (void)fprintf(stderr,"VIDEO_VIDEOVIEW installed\n");
   return OK;
  case VIDEO_XVIDEO_24:
   (void)fprintf(stderr,"VIDEO_XVIDEO_24 installed\n");
   return OK;
  default:
   return ERROR;
 }
}

