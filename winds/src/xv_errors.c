#include <stdio.h>
#include <xview/xview.h>
#include <xview/xv_error.h>

#include "proto.h"


xview_error_proc(Xv_object object, Attr_avlist avlist)
{
  time_to_log();
  (void)fprintf(stderr,"%s\n",xv_error_format(object,avlist));
  return XV_ERROR;
}

/*ARGSUSED*/
xlib_error_proc(Display *dpy, XErrorEvent *event)
{
  time_to_log();
  (void)fprintf(stderr,
 "xlib error proc called for event type %d error code %d major code %d minor code %d \n", event->type,event->error_code,event->request_code,event->minor_code);
  return XV_ERROR;
}
