/* panel_menu.h -- objects associated with panel type menus
*/

/* page up/down operation identifiers
*/
#define PAGE_UP   	0
#define PAGE_DOWN 	1

#define PANELMENUSIZEFACTOR	.78
#define FIELD_MARGIN		8 /* pixels between each field per page */

#define MENU_FIELD_X 10		/* x-coord for each left-most field */
#define TITLE_X 140		/* x-coord for each title field */
#define PANEL_MAX_FIELDS 32

/* 
WARNING: increase value of NUM_PANEL_MENUS in ../include/display.h as you add
new panel menus here!!!
*/
#define STATSMENU	0
#define ATTRSMENU	1
#define PRINTINGMENU   	2
#define PRINTCONFIGMENU 3
#define PLOTSPEXMENU 	4
#define FOLDERSMENU 	5

#define TOGGLE_ON_ALL	1
#define TOGGLE_ON_THIS_PAGE	2

#define FIXED_RANGE	0
#define FLOATING_RANGE	1

/* 
types of menu fields
*/
#define INTERNAL	0
#define USERLEVEL	1
/*
maximum length of message labels
*/
#define MAXMSGLABELLENGTH	64
