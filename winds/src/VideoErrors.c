/*
VideoErrors.c - handle error status and message transfers
*/
#include    <X11/Xlib.h>
#include <xview/xview.h>
#include "Video.h"
#include "proto.h"
#include "windsvideo.h"

char VideoErrorMsg[MAXVIDEOERRORMSGLENGTH];

/****************************** CheckVideoStatus() *****************************/

void
CheckVideoStatus()
/*
call error func with message apropos to value of error status variable
*/
{
 switch (Xv_VideoStatus) {
  case VIDEO_UNKNOWN:
   (void)sprintf(VideoErrorMsg," Video not available on display %s",
    (char *)GetDisplayHost());
   ShowAlert(VideoErrorMsg);
   break;
  case VIDEO_NO_MORE_CHANNELS:
   ShowAlert(" This channel is not supported");
   break;
  case VIDEO_CHANNEL_IN_USE:
   ShowAlert(" This channel is already in use");
   break;
  case VIDEO_NO_SYNC_DETECTED:
   ShowAlert(" No signal detected on requested channel");
   break;
 }
}

/****************************** ShowAlert() *****************************/

void
ShowAlert(msg)
char *msg;
/*
display msg
*/
{
 (void)fprintf(stderr,"%s\n",msg);
 (void)sprintf(VideoErrorMsg,"%s",msg);
}

/*************************  SetVideoErrorStatus()  ****************************/

SetVideoErrorStatus(status)
int status;
{
 Xv_VideoStatus=status;
}

/*************************  GetVideoErrorMsg()  ****************************/

char *
GetVideoErrorMsg()
{
 return VideoErrorMsg;
}

