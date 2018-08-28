#ifndef _window_h_
#define _window_h_

//  window.h:  A. Bokinsky  QC project Spring 1995

//  The Motif_window class creates a Motif window.  This class is used as the
//   base class for the WarningBox and the MainScreen classes.

#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include "buttonbar.h"
#include "text.h"

class Motif_Window {
public:
  Motif_Window( Widget p, const char *name );
  Motif_Window();
  virtual ~Motif_Window();
  void        realized( int r );      // sets the realized data member to r.
  int         realized();             // returns the realized data member.
  Widget      parent();               // returns the parent Widget.
  Widget      manager();              // returns to manager Widget.
  virtual int realize();              // draws the window.
  virtual int unrealize();            // removes the window from the screen.

// Sets the function that is called when the socket contains data. 
  void        input( XtInputCallbackProc p, long unsigned int fd );

  void        start_loop();           // starts the application loop. 
  void        flush_events();         // flushes the XEvents.  

  void        insert( ButtonBar *b ); // inserts a ButtonBar into the window
  void        insert( Text *t );      // inserts a Text object into the window.
private:
  XtAppContext      app_context_;
  Widget            parent_;
  Widget            manager_;
  int               realized_;
  static int x_;                 // static data members to implement
  static int y_;                 // the cascading of warningboxes.
};

#endif
