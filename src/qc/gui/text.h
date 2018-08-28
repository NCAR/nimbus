#ifndef _text_h_
#define _text_h_

//  text.h:  A. Bokinsky  QC project Spring 1995

//  The Text class creates a Motif Scrolled Text Widget, used to display
//   the error messages in the MainScreen and the WarningBoxes.
//  The Text class provides two methods for inserting into the text box:
//  1).  will insert the contents of a Message structure after the key
//  2).  will insert the string insert, after the string search.
//  The clear method clears the entire text box.

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include "error.h"
#include "message.h"

class Text {
public:  
   Text( Widget parent, char *name );
  ~Text();
  void   insert( char *key, Message *msg );      // insert msg after key.
  void   insert( char *search, char *insert );   // insert insert after search.
  void   clear();                                // clear the Text widget.
private:
  Widget form_;
  Widget text_;
  char *name_;
};

#endif
