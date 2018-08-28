#ifndef _buttonbar_h_
#define _buttonbar_h_

//  buttonbar.h:  A. Bokinsky  QC project Spring 1995

//  The ButtonBar class provides a method for creating a row of
//  Motif pushbuttons.  The ButtonBar constructor takes three arguments:
//  1: the parent Widget to contain the ButtonBar (usually a form Widget)
//  2: an array of button structures
//  3: the Motif_Window containing the ButtonBar
//  The ButtonBar class provides two functions: a constructor, destructor.

#include <Xm/Xm.h>
#include <Xm/XmP.h>

static const int name_length = 100;   // The maximum length of a button name.

// this structure contains the name and callback procedure for one button.
typedef struct
{
  char button_name[ name_length ];  // the name of the button.
  XtCallbackProc procedure;         // the callback procedure.
} Button;

class Motif_Window;

class ButtonBar {
public:
// ButtonBar constructor:
  ButtonBar( Widget parent, Button buttons[], Motif_Window *w );
// ButtonBar destructor:
  ~ButtonBar();   
private:
  Widget form_;
};

#endif
