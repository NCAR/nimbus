/*
the meaning of MAXLIVEWINDOWS will change if we move from the 2-window
XVideo board (where it means max per board) to the PowerVideo (where it
will mean max per network, or may for that matter become unnecessary
entirely). Adjust code that uses it accordingly when hardware choices 
become more clear.
*/
#define MAXLIVEWINDOWS	2

#define MAXVIDEOERRORMSGLENGTH 128
