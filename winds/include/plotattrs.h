#define MAX_ATTR        580        /* max # of entries in plot_attr file  */

#define NUMBULLETIN9FIELDS 5	/* title, units, name, range, range minimum */
struct parameter {                /* bulletin 9 variables  */
 char name[NAMLEN];
 int category_number;
 struct parameter *next;
};

struct cat {                      /*  bulletin 9 categories  */
 char name[CATEGORY_LENGTH];
 int number_of_vars;
 struct cat *next;
};

#ifndef FIXED_RANGE
#include <panel_menus.h>
#endif
#define DEFAULT_RANGE FLOATING_RANGE
#define DEFAULT_FIXED_MIN 0.0
#define DEFAULT_FIXED_MAX 200.0
#define DEFAULT_FLOAT_RNG 200.0
#define DEFAULT_UNITS "unk"
#define DEFAULT_TITLE "No Description Available"

#define ATTRS_FORMAT "%s %s %s %s"
