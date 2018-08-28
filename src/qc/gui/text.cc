//  text.cc:  A. Bokinsky  QC project Spring 1995
//  This file contains the functions provided by the Text class.

#include <iostream>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/ScrolledW.h>
#include <cstring>

#include "text.h"
static const int screen_width = 100;         // the constant width of the text.

//  The Text class constructor, it creates a ScrolledText Widget.
Text::Text( Widget parent, char *name ) {
  name_ = new char[ strlen( name ) + 1 ];
  strcpy( name_, name );
  form_ = XtVaCreateManagedWidget( "form", xmFormWidgetClass, parent,
				   XmNfractionBase, 1,
				   NULL );

  XtVaCreateManagedWidget( "label", xmLabelWidgetClass, form_,
			   XmNlabelString,    XmStringCreateSimple( name ),
			   NULL );

  Widget frame =
    XtVaCreateManagedWidget( "frame", xmFrameWidgetClass, form_,
			     XmNtopAttachment,    XmATTACH_POSITION,
			     XmNtopPosition,      0,
			     XmNleftAttachment,   XmATTACH_POSITION,
			     XmNleftPosition,     0,
			     XmNbottomAttachment, XmATTACH_POSITION,
			     XmNbottomPosition,   1,
			     XmNrightAttachment,  XmATTACH_POSITION,
			     XmNrightPosition,    1,
			     NULL );

  text_ = XmCreateScrolledText( frame, "text", NULL, 0 );
  XtManageChild( text_ );
}

//  Text class destructor.
Text::~Text() {
  delete name_;
}

//  Inserts the contents of the Message structure into the text box,
//   after the string key.
void Text::insert( char *key, Message *msg ) {
  if ( strcmp( msg->type, name_ ) )
    if ( strcmp( msg->vname, name_ ) )
      return;

  char message[ screen_width ];
  sprintf( message, "%6.2f      %4d      %4d    %12s\n",
  	   msg->val,
	   msg->Count_Consec,
	   msg->Count_last2min,
	   msg->Timestamp );
  insert( key, message );
}

//  This function inserts a string into the text box,
//   after the string search.  
void Text::insert( char *search, char *insert ) {
  unsigned int	i;
  char *string, *p;
  XmTextPosition pos1;

  if ( !insert )
    return;

// The blanks array separates the entry name from the rest of the message:
  char *blanks = new char[ SOCK_NAMELEN - strlen( search ) ];
  for (i = 0; i < SOCK_NAMELEN - strlen( search ); i++ )
    blanks[ i ] = ' ';
  blanks[ i ] = '\0';

// If the text widget is empty, the string is inserted first:
  if ( !( string = XmTextGetString( text_ ) ) ||
       !*string ||
       !strcmp( search, "" ) ) {
    char *output_string = new char[ strlen( search ) +
				    strlen( blanks ) + 
				    strlen( insert ) + 2 ];
    strcpy( output_string, search );
    strcat( output_string, blanks );
    strcat( output_string, insert );
    strcat( output_string, " " );
    XmTextInsert( text_, XmTextGetLastPosition( text_ ), output_string );
    return;
  }

// The string should be inserted into the middle of the text Widget:
  for ( p = strstr( string, search );
       ( p != NULL ) && ( *( p - 1 ) != ' ' ) ;
       p = strstr( p + strlen( search ), search ) );

// The string should replace a previous message:
  if ( p ) {
    pos1 = ( XmTextPosition )( p + strlen( search ) - string );
    XmTextReplace( text_, pos1,
		  pos1 + SOCK_NAMELEN - strlen( search ) , blanks );
    pos1 += SOCK_NAMELEN - strlen( search );
    for ( i = 0; i < strlen( insert ) - 1; i++, p++ )
      if ( *p != insert[ i ] ) {
	XmTextReplace( text_, pos1 + i, pos1 + strlen( insert + i ),
		      insert + i );
	break;
      }
    XtFree( string );
  }

// The string is to be inserted at the end of the existing text:
  else {
    char *output_string = new char[ strlen( search ) +
				    strlen( blanks ) + 
				    strlen( insert ) + 2 ];
    strcpy( output_string, search );
    strcat( output_string, blanks );
    strcat( output_string, insert );
    strcat( output_string, " " );
    XmTextInsert( text_, XmTextGetLastPosition( text_ ), output_string );
  }
}

//  Clears the textbox.
void Text::clear() {
  XmTextSetString( text_, NULL );
}
