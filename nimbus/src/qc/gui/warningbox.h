#ifndef _warningbox_h_
#define _warningbox_h_

//  warningbox.h:  A. Bokinsky  QC project Spring 1995

//  The WarningBox class creates the warning box used in the QC project.
//  The WarningBox inherits from the Motif_Window class, which provides the
//  actual widow to be displayed.

#include "window.h"
#include "text.h"
#include "buttonbar.h"

class WarningBox : public Motif_Window {
public:
   WarningBox( const char *name, Widget p );
  ~WarningBox();

// updates the WarningBox with the new message.
  void       update( Message *message );
  void       display();          // display the WarningBox to the screen.
  void       disable();          // disable the WarningBox.
  int        realize();          // draw the WarningBox to the screen.
  int        unrealize();        // remove the WarningBox from the screen.
  void       clear();            // clear the text in the WarningBox.
  void       snooze( int l );    // snooze the WarningBox for lenght l. 
  const char *name();            // returns the name of the variable. 
private:
  int  enabled();
  char *name_;
  int  disabled_;
  int  snooze_length_;
  int  snoozed_;
  Text *text_;
};

#endif
