//  buttonbar.cc:  A. Bokinsky  QC project Spring 1995
//  This file contains the functions provided by the ButtonBar class.

#include <iostream.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>

#include "buttonbar.h"

// This function creates a row of buttons.  Each button is
// specified in the button structure array passed to the function.
// To add a new button to button bar, simply add a new Button structure to
// the Button structure array.  The last Button structure must have the null
// string as the name field, and a null function pointer as the function field.
ButtonBar::ButtonBar( Widget parent, Button b[], Motif_Window *w ) {
  int	i;

  if ( !strcmp( b[ 0 ].button_name, "" ) )
    return;
  
  int number_of_buttons = 0;
  for (i = 0; strcmp( b[ i ].button_name, "" ); i++ )
    number_of_buttons++;
  
  form_  = XtVaCreateManagedWidget( "form2", xmFormWidgetClass, parent,
				    XmNfractionBase, number_of_buttons,
				    NULL );

// This loop creates the buttons as it passes them to the XtAddCallback
//  function, the buttons are not stored, because that is not needed.
  for ( i = 0; i < number_of_buttons; i++ )
    XtAddCallback( XtVaCreateManagedWidget
		   ( "button", xmPushButtonGadgetClass, form_,
		     XmNleftAttachment,  XmATTACH_POSITION,
		     XmNleftPosition,    i,
		     XmNrightAttachment, XmATTACH_POSITION,
		     XmNrightPosition,   i + 1,
		     XmNlabelString,    XmStringCreateSimple(b[i].button_name),
		     NULL ),
		   XmNactivateCallback, b[ i ].procedure, w );
}

// The ButtonBar destructor:
ButtonBar::~ButtonBar() {}
