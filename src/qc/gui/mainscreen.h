#ifndef _mainscreen_h_
#define _mainscreen_h_

//  mainscreen.h:  A. Bokinsky  QC project Spring 1995

//  The MainScreen class creates the MainScreen of the gui.
//  The MainScreen inherits from the Motif_Window class, which provides the
//  actual widow to be displayed.

#include "warningbox.h"
#include "window.h"
#include "buttonbar.h"
#include "text.h"
#include "error.h"
#include "message.h"

class MainScreen : public Motif_Window {
public:
   MainScreen( int argc, char *argv[] );
  ~MainScreen();
  void       activate();            // sets up the mainscreen.
  int        pause();               // returns the pause data member.
  void       pause( int p );        // sets the pause data member.
  void       clear();               // clears the warningboxes from the screen.
  void       reset();               // clears the text boxes on the mainscreen.
  void       snooze();              // pauses the generation of warningboxes.  
  void       quit();                // quits the system.
  void       store( WarningBox *w );// store a warningbox.
  WarningBox *lookup( char *key );  // lookup a warningbox.
  void       get_next_message();    // get the next error message. 

// display the new message in the Text boxes
  void       show_message( Message *msg );

private:
// The BinarySearchTree class is private to the MainScreen class.
// It is used to store WarningBoxes.
  class BinarySearchTree {
  public:
     BinarySearchTree();
    ~BinarySearchTree();
    void       insert( WarningBox *w );  // Insert a WarningBox into the tree.
    WarningBox *lookup( char *key );     // Get a WarningBox from the tree.
    void       clear();                  // Undisplay all WarningBoxes.
  private:
    WarningBox       *w_;
    BinarySearchTree *right_tree_;
    BinarySearchTree *left_tree_;
  };
  BinarySearchTree *binary_tree_;

  Text **text_;
  int number_of_texts_;
  int pause_;
  Error *error_;
};

#endif
