// window.cc: A. Bokinsky QC project Spring 1995
// This file contains the functions provided by the Motif_Window class.

#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/ShellEP.h>
#include <Xm/PanedW.h>

#include "window.h"

int Motif_Window::x_ = 0;                // static data members to implement
int Motif_Window::y_ = 0;                // the cascading of warningboxes.

static const int increment_x = 20;       // The stepsize for cascading.
static const int increment_y = 20;       

//  This constructor is used for the WarningBox.  It creates a Motif
//  TopLevelShellWidgetClass for the WarningBox window.
Motif_Window::Motif_Window( Widget p, const char *name ) {
  int	tmpX = log10( x_ + 1 );
  char *location  = new char[ tmpX + 4 ]; 
  sprintf( location, "+%d+%d", x_, y_ );
  x_ += increment_x;  y_ += increment_y;

  char *wname = new char[ strlen( name ) + 10  ];
  strcpy( wname, "Warning  ");
  strcat( wname, name );

  parent_ = XtVaAppCreateShell( "Warning",
				"XmQC",
				topLevelShellWidgetClass,
				XtDisplay( p ),
				XmNgeometry, location,
				XmNiconName, wname,
				NULL );
  manager_  = XtVaCreateManagedWidget( "manager", xmPanedWindowWidgetClass,
				      parent_,
				      NULL );
}

//  This constructor is used for the MainScreen.
Motif_Window::Motif_Window() {
  int argc = 0;
  parent_ = XtVaAppInitialize( &app_context_, "XmQC",
                             ( XrmOptionDescList )NULL, 0,
                             &argc,
                             ( String * )NULL, 
                             ( String * )NULL, 
			     XmNiconName, "Quality Control Project",
			     NULL );
  manager_  = XtVaCreateManagedWidget( "manager", xmPanedWindowWidgetClass,
				      parent_,
				      NULL );
}

//  Motif_Window destructor. 
Motif_Window::~Motif_Window() {}

//  Sets the realized_ data member = r.
void Motif_Window::realized( int r ) { realized_ = r; }

//  Returns the realized_ data member.
int Motif_Window::realized() { return realized_; }

//  Returns the parent Widget data member.
Widget Motif_Window::parent() { return parent_; }

//  Returns the Manager Widget data member.
Widget Motif_Window::manager() { return manager_; }

//  Draw the Motif_Window on the screen.
int Motif_Window::realize() {
  if ( realized_ )
    return 0;
  realized_ = 1;
  XtRealizeWidget( parent_ );

// limit the Xinput events so that the window does not get over loaded.
  XSelectInput( XtDisplay( parent_ ) , XtWindow( parent_ ),
	       ExposureMask          |
	       KeyPressMask          |
	       ButtonPressMask    );
  return 1;
}

//  Remove the Motif_Window from the screen.
int Motif_Window::unrealize() {
  if ( !realized_ )
    return 0;
  realized_ = 0;
  XtUnrealizeWidget( parent_ );
  return 1;
}

// This function adds the socket file descriptor to the list of input
// for the application context.  Whenever the socket can perform a read
// the application loop is interrupted, and the XtInputCallbackProc p
// is called.  
void Motif_Window::input( XtInputCallbackProc p, long unsigned int fd ) {
  XtAppAddInput( app_context_, fd, ( void * )XtInputReadMask, p, this );
}

//  Starts the application loop.
void Motif_Window::start_loop() {
  XtAppMainLoop( app_context_ );
}

//  Fuctions to insert the ButtonBar or Text objects into the window.
//  These functions are not really needed, but they make the interface
//  easier to understand for someone using this code.
void Motif_Window::insert( ButtonBar *b ) {}
void Motif_Window::insert( Text *t ) {}

//  This function helps keep the window from getting behind the socket.
void Motif_Window::flush_events() { 
  while ( XtAppPending( app_context_ ) ) {
    XtAppProcessEvent( app_context_, XtIMAll );
  }
}
