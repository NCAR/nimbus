//  main.cc:  A. Bokinsky  QC project Spring 1995
//  The main program for the QC graphical user interface.

#include <iostream.h>
#include <string.h>
#include <new.h>
#include "mainscreen.h"

// new_handler from B. Stroustrup, "The C++ Programming Language", p. 99
// If new fails then this function causes the program to exit, it
// simplifies the code, because the return value of new does not need to
// be checked each time new is called.
static void out_of_store() {
  cerr <<"operator new failed: out of store" <<endl;
  exit( 1 );
}

// This simple main program creates the mainscreen of the GUI, then
//  activates the mainscreen, draws it, and enters the application loop.
int main( int argc, char *argv[] ) {
  set_new_handler( &out_of_store );

  MainScreen *ms = new MainScreen( argc, argv );
  ms->activate();
  ms->realize();
  ms->start_loop();

  return 0;
}
