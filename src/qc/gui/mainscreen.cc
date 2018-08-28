//  mainscreen.cc:  A. Bokinsky  QC project Spring 1995
//  This file contains the functions provided by the MainScreen class.

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <Xm/MessageB.h>

#include "mainscreen.h"
#include "gui2sig.h"
#include "error.h"
#include "message.h"

static const int snooze_length = 10 * 60;    // snooze variable in seconds
static const int day = 24 * 60 * 60;         // used when quiting.
static const int screen_width = 100;         // the constant width of the text.

//  The callback functions which are activated when the corresponding button
//   is pressed on the MainScreen:
static void pause_callback(    Widget w, XtPointer client_data,
			       XtPointer call_data );
static void snooze_callback(   Widget w, XtPointer client_data,
			       XtPointer call_data );
static void reset_callback(    Widget w, XtPointer client_data,
			       XtPointer call_data );
static void clear_callback(    Widget w, XtPointer client_data,
			       XtPointer call_data );
static void quit_callback(     Widget w, XtPointer client_data,
			       XtPointer call_data );
static void yes_quit_callback( Widget w, XtPointer client_data,
			       XtPointer call_data );

//  The input function is called when the socket can perform a read.
static void input_callback( MainScreen *mainscreen, int *x,
			   long unsigned int *r );


//  The MainScreen constructor, it passes argc and argv to the Motif_Window
//   constructor, and initializes data members.
MainScreen::MainScreen( int argc, char *argv[] ) : Motif_Window() {
  binary_tree_ = new BinarySearchTree;
  pause_ = 1;
  error_ = new Error;
  number_of_texts_ = argc;
  TimerCreate();

// The label printed at the head of each text window:
  char message[ screen_width ];
  sprintf( message, "  %-10s %s  %s    %s\n",
 	   "value ",
	   "Consec ",
	   "last2min ",
	   "Timestamp " );

//  create the text display boxes:
//  The number of text boxes is determined dynamically by the value of argc.
//  To add a new text box, simply add a command line argument
//   to the gui executable.
  text_ = new Text*[ argc ];
  for ( int i = 1; i < argc; i++ ) {
    text_[ i ] = new Text( this->manager(), argv[ i ] );
    text_[ i ]->insert( "variable name:", message );
    this->insert( text_[ i ] );
  }

// The buttonbar displayed on the mainscreen.  To add a new button, simply
// add a new button structure to the array, for example:
//		 {   "New Buton Name",  new_button_callback  },
// Buttons are laid out in the order they are specified in the array.
  Button b[] = { {   "Resume",          pause_callback  },
                 {   "Snooze 10 min.",  snooze_callback },
		 {   "Reset",           reset_callback  },
		 {   "Clear Boxes",     clear_callback  },
		 {   "Quit",            quit_callback   },
		 {   "",                NULL   }  };
  this->insert( new ButtonBar( this->manager(), b, this ) );
}

//  The MainScreen destructor deallocates memory for its data members.
MainScreen::~MainScreen() {
  delete binary_tree_;
  delete[] text_;
  delete error_;
}

//  This function passes the socket file descriptor to the input function.
void MainScreen::activate() {
  this->input( (XtInputCallbackProc)&input_callback, error_->fd() );
}

//  Returns the pause_ variable.
int MainScreen::pause() {
  return pause_;
}

//  Pauses the MainScreen, while paused no WarningBoxes will be generated.
void MainScreen::pause( int p ) {
  pause_ = p;
}

// Get the next error message, and display it.
void MainScreen::get_next_message() {
  WarningBox * wb;

  Message *msg;
  if ( (msg = error_->getNext()) )  {
    if ( !strcmp( msg->type, "quit" ) ) {
      delete error_;
      exit( 0 );
    }
// Display the message on the mainscreen:
    this->show_message( msg );
// If it is a high severity message, and the system is not paused, then
//   display the message in a warningbox:
    if ( !this->pause() && ( msg->sev == SEV_HIGH ) ) {
      if ( ( wb = lookup( msg->vname ) ) )
	wb->update( msg );
      else {
	wb = new WarningBox( msg->vname, this->manager() );
	this->store( wb );
	wb->update( msg );
      }
    }
  }
// Update the display:
  XmUpdateDisplay( this->parent() );
  this->flush_events();
}

//  Updates the textboxes in the MainScreen.
void MainScreen::show_message( Message *msg ) {
  for ( int i = 0; i < number_of_texts_; i++ )
    text_[ i ]->insert( msg->vname, msg ); 
}

//  The function undisplays all the WarningBoxes.  Because the WarningBoxes
//   are stored in the BinarySearchTree, it simply calls the BinarySearchTree
//   clear function.
void MainScreen::clear() {
  binary_tree_->clear();
}

//  This function clears all the textboxes.
void MainScreen::reset() {
  char message[ screen_width ];
  sprintf( message, "  %-10s %s  %s    %s\n",
	   "value ",
	   "Consec ",
	   "last2min ",
	   "Timestamp " );
  for ( int i = 0; i < number_of_texts_; i++ ) {
    text_[ i ]->clear();   text_[ i ]->insert( "variable name:", message );
  }
}

//  The entire window will be snoozed.  Call the TimerSet function with "all"
void MainScreen::snooze() {
  TimerSet( snooze_length, "all" );
}

//  The entire system will quit.  Call the timer code to shut down the socket.
void MainScreen::quit() {
  TimerSet( day, "quit" );
  delete error_;
  exit( 0 );
}

//  Inserts a new WarningBox into the BinarySearchTree.
void MainScreen::store( WarningBox *w ) {
  binary_tree_->insert( w );
}

//  Lookup a WarningBox in the BinarySearchTree by "key".
WarningBox *MainScreen::lookup( char *key ) {
  return binary_tree_->lookup( key );
}

//  The BinarySearchTree constructor.
MainScreen::BinarySearchTree::BinarySearchTree() {
  right_tree_ = NULL;
  left_tree_ = NULL;
  w_ = NULL;
}

//  The BinarySearchTree destructor: recursively deallocates memory of all
//   the nodes in the BinarySearchTree.
MainScreen::BinarySearchTree::~BinarySearchTree() {
  delete right_tree_;
  delete left_tree_;
  delete w_;
};

//  Insert a WarningBox into the BinarySearchTree.
void MainScreen::BinarySearchTree::insert( WarningBox * w ) {
  if ( !w_ || !strcmp( w_->name(), w->name() ) )
    w_ = w;

  else if ( strcmp( w_->name(), w->name() ) > 0) {
    if ( !right_tree_ )
      right_tree_ = new BinarySearchTree;
    right_tree_->insert( w );
  }
  else {
    if ( !left_tree_ )
      left_tree_ = new BinarySearchTree;
    left_tree_->insert( w );
  }
};

//  Lookup, and return a WarningBox in the BinarSearchTree, if it exists.
WarningBox *MainScreen::BinarySearchTree::lookup( char *key ) {
  if ( !w_ || !strcmp( w_->name(), key ) )
    return w_;

  else if ( strcmp( w_->name(), key ) > 0 ) {
    if ( !right_tree_ )
      return NULL;
    return right_tree_->lookup( key );
  }
  else {
    if ( !left_tree_ )
      return NULL;
    return left_tree_->lookup( key );
  }
  return NULL;
}

//  Call the unrealize() function for all the WarningBoxes.
void MainScreen::BinarySearchTree::clear() {
  if ( !w_ )
    return;

  w_->unrealize();
  if ( right_tree_ )
    right_tree_->clear();
  if ( left_tree_ )
    left_tree_->clear();
}

//  The callback function for the pause button.
static void pause_callback( Widget w, XtPointer client_data,
			    XtPointer call_data ) {
  if ( ( ( MainScreen * )client_data )->pause() ) {
    XtVaSetValues( w, XmNlabelString, XmStringCreateSimple( "Pause" ), NULL );
    ( ( MainScreen * )client_data )->pause( 0 );
  }
  else {
    XtVaSetValues( w, XmNlabelString, XmStringCreateSimple( "Resume" ), NULL );
    ( ( MainScreen * )client_data )->pause( 1 );
  }
} 

//  The callback function for the snooze button.
static void snooze_callback( Widget w, XtPointer client_data,
			     XtPointer call_data ) {
  ( ( MainScreen * )client_data )->snooze();
}

//  The callback function for the reset button.
static void reset_callback( Widget w, XtPointer client_data,
			    XtPointer call_data ) {
  ( ( MainScreen * )client_data )->reset();
}

//  The callback function for the clear button.
static void clear_callback( Widget w, XtPointer client_data,
			    XtPointer call_data ) {
  ( ( MainScreen * )client_data )->clear();
}

//  The callback function for the quit button.
static void quit_callback( Widget w, XtPointer client_data,
			   XtPointer call_data ) {
  Widget dialog;
  int nargs = 0;
  Arg args[ 4 ];  // The number of attributes.
  XmString t;
// The question string of the dialog box:
  t = XmStringCreateSimple( "Are you sure you want to quit ?" );
  XtSetArg( args[ nargs ], XmNmessageString, t ); nargs++;

// The title string of the dialog box:
  t = XmStringCreateSimple( "Quit ?" );
  XtSetArg( args[ nargs ], XmNdialogTitle, t ); nargs++;

// The dialog box is modal:
//  XtSetArg( args[ nargs ], XmNdialogStyle, 
//	    XmDIALOG_PRIMARY_APPLICATION_MODAL ); nargs++;
  XtSetArg( args[ nargs ], XmNdialogStyle, 
	    XmDIALOG_FULL_APPLICATION_MODAL ); nargs++;

  dialog = XmCreateQuestionDialog(  XtParent( w ), "quit ?", args, nargs );
  XmStringFree( t );

// The callback function for the OK button on the dialog box:
  XtAddCallback( dialog, XmNokCallback, yes_quit_callback, client_data );

// The help button on the dialog box is removed:
  XtUnmanageChild( XmMessageBoxGetChild( dialog, XmDIALOG_HELP_BUTTON ) );
  XtManageChild( dialog );
  XtPopup( XtParent( dialog ), XtGrabNone );
}

// The callback function for the OK button on the quit dialog.
static void yes_quit_callback( Widget w, XtPointer client_data,
				  XtPointer call_data ) {
  ( ( MainScreen * )client_data )->unrealize();
  ( ( MainScreen * )client_data )->quit();
}

//  The function called when the socket can perform a read.
static void input_callback(MainScreen *mainscreen, int *x,
			     long unsigned int *r) {
  mainscreen->get_next_message();
}
