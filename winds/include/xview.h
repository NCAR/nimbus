/* 
xview.hanomalies, hardwires, kluges specific to xview conversion 
*/
/* 
dimensions of window manager frame border 
*/
#define WMGR_SIDE_BORDER_WIDTH 5 
/*
dimensions of window manager top and bottom borders
*/
#define WMGR_BOTTOM_BORDER_HEIGHT 5   
#define WMGR_TOP_BORDER_HEIGHT 26
/* 
pixels visible below control panel, for access to root window's menus, etc. 
*/
#define BKGND_SLIVER 5  
/*
set margins to calculate what interior base frame dimensions should be
*/
#define WINDS_WIDTH_MARGIN (WMGR_SIDE_BORDER_WIDTH*2)
#define WINDS_HEIGHT_MARGIN (WMGR_BOTTOM_BORDER_HEIGHT+WMGR_TOP_BORDER_HEIGHT+BKGND_SLIVER)
/* 
specify characters for whom notify proc is called
*/
#define NOTIFY_STRING "\r\n\t"
