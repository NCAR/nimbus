/* menus.c -- handle menu stuff

   SCCS ID: 
@(#)menus.c	4.2 11/8/89

*/

#include <xview/xview.h>
#include <xview/panel.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/****************************  WINDS INCLUDES  ******************************/

#include <lengths.h>
#include <display.h>
#include <constants.h>
#include <help.h>
#include <xy_plt.h>
#include <ops.h>
#include <files.h>
#include <plotattrs.h> 
#include <network.h> 
#include <proto.h> 

/***************************  GLOBAL WITHIN FILE  ****************************/

struct menulist {
 char name[NAMLEN];
 char category[CATEGORY_LENGTH];
 char title[MAX_TITLE_LENGTH];
 int memindex;
 int mode;
 int attrindex;
};
static struct menulist *MenuList=0;
static int NumMenuEntries=0;
static int SortDone=FALSE;

static MenuListCmp(struct menulist *a, struct menulist *b);

/*************************************************************************/

ClearMenuList()
{
 (void)free((struct menulist *)MenuList);
 NumMenuEntries=0;
}

/*************************************************************************/

AddToMenuList(totalnames,namelist,mode)
int totalnames,mode;
char *namelist;
{
int index,entry,name,omitted;

 MenuList=(struct menulist *)realloc((void *)
  MenuList,(size_t)((NumMenuEntries+totalnames)*sizeof(struct menulist)));
 for (name=0,omitted=0,entry=NumMenuEntries; name<totalnames; name++) {
  index=(mode==DERIVED?name+GetNumRawNames():name);
  if (GetTotalRange(index)!=0.0) {
   (void)sprintf(MenuList[entry].name,"%s",&namelist[name*NAMLEN]); 
   (void)sprintf(MenuList[entry].title,"%s",
    (char *)VarDB_GetTitle(MenuList[entry].name));
   MenuList[entry].memindex=name;
   MenuList[entry].mode=mode;
   MenuList[entry].attrindex=index;
   entry++;
  } else {
   if (!SortDone)
    (void)fprintf(stderr,"WARNING: variable %s omitted due to zero range\n",
     &namelist[NAMLEN*name]);
   omitted++;
  }
 }
 NumMenuEntries+=totalnames-omitted;
}

/*************************************************************************/

GetNumMenuEntries()
{
 return NumMenuEntries;
}

/*************************************************************************/

GetMenuListAttrIndex(entry)
int entry;
{
 return MenuList[entry].attrindex;
}

/*************************************************************************/

GetMenuListIndex(entry)
int entry;
{
 return MenuList[entry].memindex;
}

/*************************************************************************/

GetMenuListMode(entry)
int entry;
{
 return MenuList[entry].mode;
}

/*************************************************************************/

char *
GetMenuListTitle(entry)
int entry;
{
 return MenuList[entry].title;
}

/*************************************************************************/

char *
GetMenuListName(entry)
int entry;
{
 return MenuList[entry].name;
}

/****************************************************************************/

char *
GetMenuListCategory(entry)
int entry;
{
 return MenuList[entry].category;
}

/****************************************************************************/

SetMenuListCategory(varname,category)
char *varname;
char *category;
{
 (void)sprintf(MenuList[GetMenuListPosn(varname)].category,"%s",category);
}

/****************************************************************************/

GetMenuListPosn(varname)
char *varname;
/*
search menu list character array for a match with varname. If found,
return its position in that array (OR'd with a value indicating RAW or
DERIVED if OrWithMode is TRUE); else put out a message and return ERROR
*/
{
 int indx;            /* utility indexing   */

 indx = 0;
 while (indx < GetNumMenuEntries()) {
  if ( (strcasecmp((char *)GetMenuListName(indx),varname) )==0)   {
   return(indx);
  }
  indx++;
 }
 return(ERROR);
}

/*************************************************************************/

InitMenuList() 
{
 ClearMenuList();
 AddToMenuList(GetNumRawNames(),GetRawNameList(),RAW);
 AddToMenuList(GetNumDerivedNames(),GetDerivedNameList(),DERIVED);
 SortMenuList();
}

/************************  MAKE_WINDS_MENUS()  ***************************/

make_winds_menus()
/* make all menus -- don't change the order listed here
*/
{
  
 get_bulletin_9();
 SetVarCategories();
 ResizeScrollPanel(GetBaseFrame());
 make_alt_menu();
}

/***************************************************************************/

static MenuListCmp(a,b)
struct menulist *a,*b;
{
 return (strcmp((char *)a->name,(char *)b->name));
}

/***************************************************************************/

SortMenuList()
{
 qsort((char *)MenuList,NumMenuEntries,sizeof(struct menulist),MenuListCmp);
 SortDone=TRUE;
}

/***************************************************************************/

TraceIsDeletable(quadrant)
int quadrant;
{
 return
 (num_of_traces[quadrant] > 1 && plot_type[quadrant]!=SKEW_T);
}

/************************  SetVarCategories()  ***************************/

SetVarCategories()
{
struct parameter *var_list;
static struct cat *category;
int var,found;
char *LastTitle;
 category=first_category();
 while (category) {
  LastTitle=(char *)strdup(category->name);
  category=category->next;
 } 
 for (var=0; var<NumMenuEntries; var++) {
  var_list=top_of_bulletin9();
  found=FALSE;
  while (!found && var_list) {
   if (!(strcasecmp(MenuList[var].name,var_list->name))) { /* found */
    found=TRUE;
   }
   var_list=var_list->next;
  }
  if (!(found)) { /* variable not in bulletin 9 */
   SetMenuListCategory(MenuList[var].name,LastTitle);
  }
 }
}
