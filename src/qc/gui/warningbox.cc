//  warningbox.cc:  A. Bokinsky  QC project Spring 1995
//  This file contains the functions provided by the WarningBox class.

#include <iostream>
#include <Xm/MessageB.h>
#include <Xm/Protocols.h>
#include <Xm/AtomMgr.h>
#include <cstring>
#include <time.h>

#include "warningbox.h"
#include "gui2sig.h"
#include "error.h"
#include "message.h"

static const int snooze_long = 10 * 60;      // snooze variables in seconds
static const int snooze_short = 2 * 60;
static const int day = 24 * 60 * 60;         // used when disabling a variable.
static const int screen_width = 100;         // the constant width of the text.

//  The callback functions which are activated when the corresponding button
//   is pressed on the WarningBox:
static void disable_callback(          Widget w, XtPointer client_data,
				       XtPointer call_data );
static void yes_disable_callback(      Widget w, XtPointer client_data,
				       XtPointer call_data );
static void dismiss_callback(          Widget w, XtPointer client_data,
				       XtPointer call_data );
static void snooze_long_callback(      Widget w, XtPointer client_data,
				       XtPointer call_data );
static void snooze_short_callback(     Widget w, XtPointer client_data,
				       XtPointer call_data );
static void clear_callback(            Widget w, XtPointer client_data,
				       XtPointer call_data );

//  The WarningBox constructor creates a Motif_Window, and the Text object and
//   ButtonBar that it will contain.
WarningBox::WarningBox( const char *name, Widget p ) :Motif_Window( p, name ) {
// The buttonbar displayed on the WarningBox.  To add a new button, simply
// add a new button structure to the array, for example:
//		 {   "New Buton Name",  new_button_callback  },
// Buttons are laid out in the order they are specified in the array.
  Button b[] = { {   "Disable",    disable_callback   },
		   { "Snooze 10 min.",  snooze_long_callback  },
		   { "Snooze 2 min.",   snooze_short_callback   },
		   { "Clear",      clear_callback     },
		   { "Dismiss",    dismiss_callback   },
		   { "",           NULL      }  };

  snooze_length_ = 0;
  snoozed_ = 0;
  disabled_ = 0;
  name_ = new char[ strlen( name ) + 1 ];
  strcpy( name_, name );
  text_ = new Text( this->manager(), name_ );
  this->insert( text_ );
  this->insert( new ButtonBar( this->manager(), b, this) );
  char message[ screen_width ];
  sprintf( message, "  %-10s %s  %4s    %s\n",
	   "value ",
	   "Consec ",
	   "last2min ",
	   "Timestamp " );
  text_->insert( "Error type:", message );

// This removes the close option from the window manager:
  XmAddWMProtocolCallback( this->parent(),
			   XmInternAtom( XtDisplay( this->parent() ),
					 "WM_DELETE_WINDOW", False ),
			   dismiss_callback, this );
  this->realized( 0 );
}

//  The WarningBox destructor.
WarningBox::~WarningBox() {
  delete name_;
}

//  Determines if the WarningBox is enabled.  This is a backup for the timer
//   routines.  It simply checks the sleep, and disabled data members.
int WarningBox::enabled() {
  if ( disabled_ )
    return 0;
  if ( snooze_length_ == snooze_long )
    if ( ( time( NULL ) - snoozed_ ) < ( snooze_long ) )
      return 0;
  if ( snooze_length_ == snooze_short )
    if ( ( time( NULL ) - snoozed_ ) < ( snooze_short ) )
      return 0;
  return 1;
}

//  Display the WarningBox
void WarningBox::display() {
  if ( !this->enabled() )
    return;
  snoozed_ = 0;
  snooze_length_ = 0;
  this->realize();
  XmUpdateDisplay( this->parent() );
}
      
//  Update the WarningBox with a new error message.
void WarningBox::update( Message *message ) {
  if ( !this->enabled() )
    return;
  text_->insert( message->message, message );
  this->display();
}

//  Disable the WarningBox.
void WarningBox::disable() {
  disabled_ = 1;
  TimerSet( day, this->name() );
}

//  Draw the WarningBox on the screen.
int WarningBox::realize() {
  if ( !this->realized() ) {
    this->realized( 1 );
    XtPopup( this->parent(), XtGrabNone );
    return 1;
  }
  return 0;
}

//  Remove the WarningBox from the screen.
int WarningBox::unrealize() {
  if ( !this->realized() )
    return 0;
  this->realized( 0 );
  XtPopdown( this->parent() );
  return 1;
}

//  Snooze the WarningBox for the time = l
void WarningBox::snooze(int l) {
  snooze_length_ = l;
  snoozed_ = time( NULL );
  TimerSet( l, this->name() );
}

//  Return a const pointer to the WarningBox name_ data member, this way
//   it cannot be changed.
const char *WarningBox::name() {
  return name_;
}

//  Clear the contents of the text box.
void WarningBox::clear() {
  text_->clear();
  char message[ screen_width ];
  sprintf( message, "  %-10s %s %4s    %s\n",
	   "value ",
	   "Consec ",
	   "last2min ",
	   "Timestamp " );
  text_->insert( "Error type:", message );
}

//  The callback function for the disable button.
static void disable_callback( Widget w, XtPointer client_data,
			      XtPointer call_data ) {
  Widget dialog;
  int nargs = 0;
  Arg args[ 4 ];  // The number of attributes.
  XmString t;
  char *title = new char[ strlen( ( ( WarningBox * )client_data )->name() )
			 + 12 ];

// The information message of the dialog:
  t = XmStringCreateSimple( "Disabling is permanent!" );
  XtSetArg( args[ nargs ], XmNmessageString, t ); nargs++;

// The title of the dialog:
  strcpy( title, "Disable " );
  strcat( title,   ( ( WarningBox * )client_data )->name() );
  strcat( title,   " ?" );
  t = XmStringCreateSimple( title );
  XtSetArg( args[ nargs ], XmNdialogTitle, t ); nargs++;

// The dialog is modal:
//  XtSetArg( args[ nargs ], XmNdialogStyle, 
//	    XmDIALOG_PRIMARY_APPLICATION_MODAL ); nargs++;
  XtSetArg( args[ nargs ], XmNdialogStyle, 
	    XmDIALOG_FULL_APPLICATION_MODAL ); nargs++;
  dialog = XmCreateInformationDialog( XtParent( w ), "disable", args, nargs );
  XmStringFree( t );

// The callback function for the OK button:
  XtAddCallback( dialog, XmNokCallback, yes_disable_callback, client_data );

// The help button is removed from the dialog:
  XtUnmanageChild( XmMessageBoxGetChild( dialog, XmDIALOG_HELP_BUTTON ) );
  XtManageChild( dialog );
  XtPopup( XtParent( dialog ), XtGrabNone );
}

//  The callback function for the disable popup dialog OK button.
static void yes_disable_callback( Widget w, XtPointer client_data,
				  XtPointer call_data ) {
  ( ( WarningBox * )client_data )->disable();
  ( ( WarningBox * )client_data )->unrealize();
}

//  The callback function for the dismiss button.
static void dismiss_callback( Widget w, XtPointer client_data,
			      XtPointer call_data ) {
  ( ( WarningBox * )client_data )->unrealize();
}

//  The callback function for the snooze long button.
static void snooze_long_callback( Widget w, XtPointer client_data,
				  XtPointer call_data ) {
  ( ( WarningBox * )client_data )->snooze( snooze_long );
  ( ( WarningBox * )client_data )->unrealize();
}

//  The callback function for the snooze short button.
static void snooze_short_callback( Widget w, XtPointer client_data,
				   XtPointer call_data ) {
  ( ( WarningBox * )client_data )->snooze( snooze_short );
  ( ( WarningBox * )client_data )->unrealize();
}

//  The callback function for the clear button.
static void clear_callback( Widget w, XtPointer client_data,
			    XtPointer call_data ) {
  ( ( WarningBox * )client_data )->clear();
}
