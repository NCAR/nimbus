/*
-------------------------------------------------------------------------
OBJECT NAME:	pms2d.c

FULL NAME:	

ENTRY POINTS:	update_pms2d_displays()

STATIC FNS:	build_std2d_slice()
		build_grey_slice()
		display_std_2d()
		display_grey_scale()
		display_hvps()

DESCRIPTION:	Perform display for all PMS 2D & HVPS probes.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1991-95
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <values.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/cursor.h>
#include <xview/cms.h>
#include <xview/svrimage.h>
#include <sys/time.h>

#include <header.h>			/* tape header defines */
#include <colors.h>     
#include <constants.h>     
#include <errors.h>     		/* alert error message codes */
#include <eth_shm.h>			/* ethernet shared memory defines */
#include <files.h>     
#include <help.h>     
#include <lengths.h>     
#include <op_panels.h>			
#include <plotspex.h>     
#include <pms2d.h>			/* probe operation defines */
#include <p2d_layout.h>			/* display layout defines */
#include <proto.h>

/* Program functions */

static void build_std2d_slice();	/* builds std 2d dash list	*/
static void build_grey_slice();		/* builds grey scale dash lists	*/
static void display_std_2d();		/* creates the std 2d display	*/
static void display_grey_scale();	/* creates the grey scale display */
static void display_hvps ();		/* creates the hvps display	*/

/* Global data structures and variables */

static Server_image mem_image_2d;		/* memory pixrect */
static Drawable mem_image_xid;

/* Following three declarations are needed by save_setup() routine	*/

int disp_type[NUM_QUADRANTS];		/* display type, 2d or grey scale */
int disp_2d[NUM_QUADRANTS][MX_P2D];	/* which probes to display	*/
int disp_rate[NUM_QUADRANTS] = 		/* max display update rate (seconds) */
 {P2D_DISP_RATE_MIN,P2D_DISP_RATE_MIN,P2D_DISP_RATE_MIN,P2D_DISP_RATE_MIN};

str4 p2d_names[MX_P2D];			/* pms 2d probe names		*/
str4 grey_names[MX_GREY];		/* grey scale probe names	*/
str4 hvps_names[MX_HVPS];		/* hvps probe names		*/
int num_p2d_probes;			/* number of pms 2d probes	*/
int num_grey_probes;			/* number of grey scale probes	*/
int num_hvps_probes;			/* number of hvps probes	*/

/* font ID
 */
#define FONTID(q)	(GetFontID(Get2dDisplayFont(q)))

/* width in pixels of text rectangle
 */
#define FONTWIDTH(q)	(GetFontWidth(Get2dDisplayFont(q)))

/* height in pixels of text rectangle, inclusive of ascent and descent
 */
#define FONTHEIGHT(q)	(GetFontHeight(Get2dDisplayFont(q)))

/* height in pixels of descending portion of text rectangle
 */
#define FONTDESCENT(q)	(GetFontDescent(Get2dDisplayFont(q)))

/* height in pixels of ascending portion of text rectangle
 */
#define FONTASCENT(q)	(GetFontAscent(Get2dDisplayFont(q)))


#define P2D_DISP_RATE_X(q,string) \
(GetWINDSWindowWidth(q)/2-(strlen(string)*FONTWIDTH(q))/2)
#define NUMSTATUSLINES 	2
#define PMS2DROWGAP	5
#define PMS2DLINEGAP	2

/* status area height 
 */
#define P2D_STAT_HEIGHT(q) ((FONTHEIGHT(q)+PMS2DROWGAP)*NUMSTATUSLINES)
#define P2D_DISP_HEIGHT(q) (GetWINDSWindowHeight(q)-P2D_STAT_HEIGHT(q))
#define P2D_STAT_Y(q,line) (GetWINDSWindowHeight(q)-\
				(P2D_STAT_HEIGHT(q)-(line-1)*PMS2DLINEGAP))

/* status info line 1 y coor 
 */
#define P2D_LINE1_Y(q)     (P2D_STAT_Y(q,2)+FONTHEIGHT(q))

/* status info line 2 y coor 
 */
#define P2D_LINE2_Y(q)     (P2D_LINE1_Y(q)+FONTHEIGHT(q))   

/* assumed length of header status line string
 */
#define MAXSTRLEN	48

/* X coor of header status line; do it so as to center the string 
 */
#define P2D_HDR_X(q) (GetWINDSWindowWidth(q)/2-(MAXSTRLEN*FONTWIDTH(q))/2)


/* -------------------------------------------------------------------- */
/* divide total window width by length of second status line string length,
 * i.e. MAXSTRLEN. This helps choose the correct width font for this string.
 */
Get2dFontWidth(quadrant)
int	quadrant;
{
  return(GetWINDSWindowWidth(quadrant)/MAXSTRLEN);
}
 
/* -------------------------------------------------------------------- */
/* pick a nice number to get a reasonable ratio going here
 */
Get2dFontHeight(quadrant)
int	quadrant;
{
  return(GetWINDSWindowHeight(quadrant)/16);
}
 
/* -------------------------------------------------------------------- */
Get2dAreaTooSmall(quadrant)
int	quadrant;
{
  return(FALSE);
}
 
/* -------------------------------------------------------------------- */
init_pms2d_parms(quadrant, type) 
int	quadrant;
int	type;
{
  int probe;

  disp_type[quadrant] = type;
  disp_rate[quadrant] = P2D_DISP_RATE_MIN;

  for (probe = 0; probe < MX_P2D; probe++)
    disp_2d[quadrant][probe] = 0;

  memset(p2d_names, 0, sizeof(p2d_names));
  memset(grey_names, 0, sizeof(grey_names));
  memset(hvps_names, 0, sizeof(hvps_names));

  read_2d_prb_names();
  init_2d_graphics(quadrant);

}	/* END INIT_PMS2D_PARMS */

/* -------------------------------------------------------------------- */
/* Initialize the display graphics. Called whenever the display is created
 * or the display geometry is changed.
 */
init_2d_graphics(quadrant) 
int	quadrant;
{
  static int graphics_init = FALSE;

  init_state_parms(quadrant, DISPTYPE_PMS2D);

  /* Initialize a memory image big enough for a full screen display.
   */
  if (!graphics_init)
    {
    (*InitMemDraw[GetDrawType()])(	&mem_image_2d, P2D_MAX_WIN_WIDTH, 
					P2D_MAX_WIN_HEIGHT, PIX_DEPTH);

    mem_image_xid = (Drawable)xv_get(mem_image_2d,XV_XID);
    graphics_init = TRUE;
    }

  (*FillArea[GetDrawType()])(	pw_xid[quadrant], 0, 0,
				window_width[quadrant],
				window_height[quadrant], 0);

  show_setup(quadrant);		/* show current disp setup	*/
}

/* -------------------------------------------------------------------- */
/* Parses the tape header, and determines which 2d or grey scale probes
 * are being sampled.
 */
void read_2d_prb_names()
{
  int j;
  int c_cnt = 0;			/* number of C probes */
  int p_cnt = 0;			/* number of P probes */
  int g_cnt = 0;			/* number of grey scale probes */
  int h_cnt = 0;			/* number of hvps probes */
  char *desc;				/* --> header descriptor */

/* Parse the header looking for Pms 2d descriptors. */
  for (j = 0, desc = (char*)tp_hdr + ntohl(tp_hdr->item_len);
	j < ntohl(tp_hdr->n_items); 
	j++, desc += ntohl(((struct Blk*)desc)->item_len)) {

    if (!strcmp (desc, PMS2D_STR)) {

/* Determine the probe type, build the name, write it into the name array. 
   C probes and HVPS probes are sampled through the same interface ports, so
   their name numbers are combined.
*/
/* Bullshit. */


      if (!strcmp(((struct Pms2*)desc)->name, P2D_C_STR)) {
        (void)sprintf (p2d_names[c_cnt + p_cnt], "C%1d", c_cnt/*+h_cnt*/+1);	
        c_cnt++;
      }
      else if (!strcmp(((struct Pms2*)desc)->name, P2D_P_STR)) {
        (void)sprintf (p2d_names[c_cnt + p_cnt], "P%1d", p_cnt+1);
        p_cnt++;
      }
      else if (!strcmp(((struct Pms2*)desc)->name, P2D_G_STR)) {
        (void)sprintf (grey_names[g_cnt], "G%1d", g_cnt+1); 
        g_cnt++;
      }
      else if (!strcmp(((struct Pms2*)desc)->name, P2D_H_STR)) {
        (void)sprintf (hvps_names[h_cnt], "H%1d", /*c_cnt+*/h_cnt+1); 
        h_cnt++;
      }
    }
  }

/* Set the global numbr of probes variables. */
  num_p2d_probes = c_cnt + p_cnt;
  num_grey_probes = g_cnt;
  num_hvps_probes = h_cnt;

}
 
/* -------------------------------------------------------------------- */
get_2d_display_type(quadrant)
int	quadrant;
{
  return(disp_type[quadrant]);
}

/* -------------------------------------------------------------------- */
SetNum2dProbes(quadrant,num)
int quadrant,num;
{
  if (get_2d_display_type(quadrant) == PMS2D_STANDARD)
    num_p2d_probes=num;

  if (get_2d_display_type(quadrant) == PMS2D_GREYSCALE)
    num_grey_probes=num;

  if (get_2d_display_type(quadrant) == PMS2D_HVPS)
   num_hvps_probes=num;
}

/* -------------------------------------------------------------------- */
NumberPMS2Dprobes(int subtype)
{
  if (subtype == PMS2D_STANDARD)
    return num_p2d_probes;

  if (subtype == PMS2D_GREYSCALE)
    return num_grey_probes;

  if (subtype == PMS2D_HVPS)
   return num_hvps_probes;
}

/* -------------------------------------------------------------------- */
int num_pms2d_probes(int quadrant)
{
  if (get_2d_display_type(quadrant) == PMS2D_STANDARD)
    return num_p2d_probes;

  if (get_2d_display_type(quadrant) == PMS2D_GREYSCALE)
    return num_grey_probes;

  if (get_2d_display_type(quadrant) == PMS2D_HVPS)
   return num_hvps_probes;
}

/* -------------------------------------------------------------------- */
char* get_2d_probename(int quadrant, int prb_idx)
{
/* Returns the probe name string corresponding to the current display and
 * requested probe index.
 */

  if (get_2d_display_type(quadrant) == PMS2D_STANDARD) 
    return p2d_names[prb_idx];

  if (get_2d_display_type(quadrant) == PMS2D_GREYSCALE)
    return grey_names[prb_idx];

  if (get_2d_display_type(quadrant) == PMS2D_HVPS)
   return hvps_names[prb_idx];
}

/* -------------------------------------------------------------------- */
int get_2d_prb_index(int quadrant, char *prb_name)
{
/* Returns the probe index corresponding to the requested 2d probe name. */

  int j;
  
  for (j = 0; j < num_pms2d_probes(quadrant); j++) 
    if (!strcmp(get_2d_probename(quadrant, j), prb_name))
      return(j);

  return(ERROR);		/* probe not found */

}

/* -------------------------------------------------------------------- */
int get_2d_prb_color(char *prb_name)
{
/* Returns the display color corresponding to the requested 2d probe name. */

  if (!strcmp (prb_name, P1_STR))
    return P1_COLOR;
  if (!strcmp (prb_name, C1_STR))
    return C1_COLOR;
  if (!strcmp (prb_name, P2_STR))
    return P2_COLOR;
  if (!strcmp (prb_name, C2_STR))
    return C2_COLOR;
  if (!strcmp (prb_name, G1_STR))
    return G1_COLOR;
  if (!strcmp (prb_name, G2_STR))
    return G2_COLOR;
  if (!strcmp (prb_name, H1_STR))
    return H1_COLOR;
  if (!strcmp (prb_name, H2_STR))
    return H2_COLOR;

  return ERROR;                         /* probe not found */
}

/* -------------------------------------------------------------------- */
int pms2d_probe_selected(int quadrant, int index)
{
  return(disp_2d[quadrant][index]);
}

/* -------------------------------------------------------------------- */
int set_2d_cursor(int quadrant)
{
  static Xv_Cursor cursor;                   /* cursor */

/* customize cursor to allow visibility over 2d images */
   cursor = (Xv_Cursor)(xv_get(canvas[quadrant],WIN_CURSOR));

/* UNRESOLVED LINT PROBLEM */ 
   cursor_set(cursor,CURSOR_OP, PIX_SRC ^ PIX_DST, NULL);
   (void)xv_set(canvas[quadrant],WIN_CURSOR,cursor,NULL);
}

/* -------------------------------------------------------------------- */
int select_2d_probe(int quadrant, int index)

/* Selects and deselects a probe for display. */
{
  disp_2d[quadrant][index] = !disp_2d[quadrant][index];
}

/* -------------------------------------------------------------------- */
void select_2d_rate(int quadrant)

/* Toggles through display update rate selections */
{
  disp_rate[quadrant] =
		(disp_rate[quadrant] % P2D_DISP_RATE_MAX) + P2D_DISP_RATE_MIN;

  show_setup(quadrant);
}

/* -------------------------------------------------------------------- */
int pms2d_priority(int quadrant, int op, int index)

/* Manages the data block type display priorities.  The priority of a type 
   is the number of display intervals that have elapsed since the data block
   type was last displayed. 
*/
{
  int j;
  static int priority[NUM_QUADRANTS][MX_P2D] = 
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  switch (op)
    {
    case P2D_PRIORITY_GET:
      return priority[quadrant][index];

    case P2D_PRIORITY_INC:
      for (j = 0; j < num_pms2d_probes (quadrant); j++)
        priority[quadrant][j]++;
      return OK;

    case P2D_PRIORITY_RESET:
      priority[quadrant][index] = 0;
      return OK;

    default:
      return ERROR;
    }

}

/* -------------------------------------------------------------------- */
int update_pms2d_display(int quadrant)

/* This routine is called to update the std 2d or grey scale display in the
   requested quadrant. */
{
  int type;

  if (!repaint_set[quadrant])
    return;

  if ((type = get_2d_display_type(quadrant)) == PMS2D_STANDARD)
    display_std_2d(quadrant);
  else
  if (type == PMS2D_GREYSCALE)
    display_grey_scale(quadrant);
  else
  if (type == PMS2D_HVPS) {
//    printf("display hvps\n");
    display_hvps(quadrant);
  }

}

/* -------------------------------------------------------------------- */
static void display_std_2d(int quadrant)

/* Checks if a new standard 2D data block has been received, and if it is the
   correct one for display.  If so, the display is generated.  This routine
   is called twice per second. */
{
  register int sl_x;                    /* slice x position */
  register int sl_y0;                   /* slice top position */
  register int sl_y1;                   /* slice bottom position */
  register int sl_h;                    /* slice height */
  register int sl_w;                    /* slice width */
  register int sl_cnt;                  /* slice counter */
  register unsigned int *slice;         /* data pointer */
  unsigned char dash_list[STD2D_PIX_PER_SLICE];  /* pen up/down pattern */
  int dash_len;                         /* # current entries in dash_list */
  int first_pix;                        /* first set pixel in a slice */
  int new_idx;                          /* new probe index */

  static int disp_draw[NUM_QUADRANTS] = 	/* draw display flag */
               {FALSE,FALSE,FALSE,FALSE,FALSE};
  static int old_seq[NUM_QUADRANTS] = 		/* last shared mem seq # */
               {0,0,0,0,0};
  static int half_sec_cnt[NUM_QUADRANTS] = 	/* half second counter */
               {0,0,0,0,0};
  static int cur_idx[NUM_QUADRANTS] =      	/* current probe index */
               {0,0,0,0,0};
  static P2d_rec p2d[NUM_QUADRANTS];     /* local buffers */


  /* Check for any new 2D data in the shared memory.  Pull it in if it is
   * of higher priority than the current data block.
   */
  if (!frozen[quadrant] && eth_shm->two.rseq != old_seq[quadrant])
    {
    old_seq[quadrant] = eth_shm->two.rseq;

    /* Build the probe name string from the block id, and get the probe index.
     */
    if ((new_idx = get_2d_prb_index(quadrant, 
         get_std2d_id_string((P2d_rec *)eth_shm->twoBuf))) == ERROR)
      return;				/* invalid id */

    /* Check if new data block has a higher display priority than the
     * present data block.
     */
    if (pms2d_probe_selected (quadrant, new_idx) && (!disp_draw[quadrant] || 
        (pms2d_priority(quadrant, P2D_PRIORITY_GET, new_idx) >
         pms2d_priority(quadrant, P2D_PRIORITY_GET, cur_idx[quadrant]))))
      {
      cur_idx[quadrant] = new_idx;
      memcpy((void *)&p2d[quadrant], (void *)eth_shm->twoBuf, sizeof(P2d_rec));
      disp_draw[quadrant] = TRUE;
      }
    }

  /* If it is time to draw a new display, and there has been valid data
   * received, or there is a new display request, then draw the display.
   */
  if ((++half_sec_cnt[quadrant] / 2) >= disp_rate[quadrant] && 
      disp_draw[quadrant])
    {
    (void)pms2d_priority(quadrant, P2D_PRIORITY_INC, 0); /* incr priorities */
    (void)pms2d_priority(quadrant, P2D_PRIORITY_RESET, cur_idx[quadrant]);
    disp_draw[quadrant] = FALSE;
    half_sec_cnt[quadrant] = 0;

    (*FillArea[GetDrawType()])(mem_image_xid, 0, 0, window_width[quadrant],
      window_height[quadrant], 0);

    /* Compute the slice height and width. */
    sl_w = (window_height[quadrant] - P2D_STAT_HEIGHT(quadrant) - 3 *
			P2D_STRIP_SPACE) / (2 * STD2D_PIX_PER_SLICE);
    sl_h = sl_w * STD2D_PIX_PER_SLICE;

    /* Set the graphics context for the color, width, and dashed lines. */
    (*SetContext[GetDrawType()])(mem_image_xid, GXcopy, 
		get_2d_prb_color (get_std2d_id_string (&p2d[quadrant])), 
		sl_w, LineOnOffDash, 0); 
     
    for (sl_y0 = P2D_STRIP_SPACE, sl_y1 = P2D_STRIP_SPACE + sl_h - 1, 
         sl_cnt = 0, slice = ((unsigned int*)(p2d[quadrant].data));
         sl_y1 < P2D_DISP_HEIGHT(quadrant); 
         sl_y0 += sl_h + P2D_STRIP_SPACE, sl_y1 += sl_h + P2D_STRIP_SPACE)
      {

      for (sl_x = P2D_DISP_MARGIN; 
           (sl_x < window_width[quadrant] - P2D_DISP_MARGIN) && 
           (sl_cnt < STD2D_MAX_SLICE); sl_x += sl_w, sl_cnt++, slice++)
        {

        build_std2d_slice(slice, dash_list, &dash_len, &first_pix, sl_w);
        if (dash_len)
          (*DrawDashLine[GetDrawType()])(mem_image_xid, sl_x, sl_y1 - first_pix,
					sl_x, sl_y0, 0, dash_list, dash_len);
        }
      }

    /* Display the header info */
    show_std2d_hdr(quadrant, &p2d[quadrant]);	

    /* Copy the memory pixrect to the display canvas */
    (*RasterOp[GetDrawType()])(mem_image_xid, 0, 0, pw_xid[quadrant], 0, 0,
               window_width[quadrant], P2D_DISP_HEIGHT(quadrant), GXcopy);
    }

}

/* -------------------------------------------------------------------- */
static void build_std2d_slice(slice, dash_list, len, first, sl_w)
unsigned int *slice;			/* 2d data slice */
unsigned char *dash_list;		/* returned with pen up/down pattern */
int *len;				/* returned # of entries in dash_list */
int *first;				/* returned with first set bit # */
int sl_w;				/* slice display width */

/* Parses through and creates the pen up/down pattern for the slice. */
{
  register unsigned int tslice;		/* temporay storage */	
  register int j, k;			/* loop counters */
  register int set;			/* bit state, set or reset */

  tslice = ntohl(*slice);
  if (tslice == 0xFFFFFFFF) {		/* blank line */
    *len = 0;
    return;
  }
  else if (tslice == 0) {		/* solid line */
    dash_list[0] = (unsigned char)(STD2D_PIX_PER_SLICE * sl_w);  
    if (dash_list[0] == 0) {		/* check for char overflow */
      dash_list[0] = 255;		/* max uchar */
      dash_list[1] = 1;
      *len = 2;
    }
    else 
      *len = 1;
    *first = 0;
    return;
  }

/* Determine the first set bit. For 2d data, a set bit = 0. */
  for (j = 0; (j < STD2D_PIX_PER_SLICE) && (tslice & STD2D_PIXEL_MASK); 
       j++, tslice <<= 1);
  *first = j * sl_w;

/* Parse through the slice alternately counting how many bits are set
   or reset.  A set bit == 0. */
  for (set = 1, *len = 0; j < STD2D_PIX_PER_SLICE;) {
    if (set) {					/* count number of set bits */
      for (k = 0; (j < STD2D_PIX_PER_SLICE) && !(tslice & STD2D_PIXEL_MASK); 
           j++, k++, tslice <<= 1);
      set = 0;
    }
    else {					/* count num of reset bits */
      for (k = 0; (j < STD2D_PIX_PER_SLICE) && (tslice & STD2D_PIXEL_MASK);
           j++, k++, tslice <<= 1);
      set = 1;
    }
    dash_list[(*len)++] = k * sl_w;		/* apply the magnification */
  }
}

/* -------------------------------------------------------------------- */
void show_setup(int quadrant)

/* Shows the current display setup */
{
  char string[20];

/* Draw the status area boundary */
  (*ContextDrawLine[GetDrawType()])(pw_xid[quadrant], 0, P2D_STAT_Y(quadrant,1),
    window_width[quadrant], P2D_STAT_Y(quadrant,1), BLACK, 1, LineSolid);

  (*ContextDrawLine[GetDrawType()])(pw_xid[quadrant], 0, P2D_STAT_Y(quadrant,2),
    window_width[quadrant], P2D_STAT_Y(quadrant,2), BLACK, 1, LineSolid);

/* Show current display update rate */
  (void)sprintf (string, "Update(s):%2d", disp_rate[quadrant]);
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,BLACK,0,0,
    FONTID(quadrant));
  (*DrawText[GetDrawType()])(
   P2D_DISP_RATE_X(quadrant,string),
   P2D_LINE1_Y(quadrant),
   string);
}

/* -------------------------------------------------------------------- */
char *get_std2d_id_string(P2d_rec *p2d)

/* Returns a string with the id of the passed in standard 2d block. */
{
  static str4 id_str;

  memcpy(id_str, (char*)&p2d->id, sizeof(p2d->id));
  id_str[sizeof(p2d->id)] = '\0';

  return(id_str);
}

/* -------------------------------------------------------------------- */
void show_std2d_hdr(int quadrant, P2d_rec *p2d)
{
  char string[80];

  /* Show the probe id string.
  */
  (*SetContext[GetDrawType()])(	pw_xid[quadrant], GXcopy,
				get_2d_prb_color(get_std2d_id_string(p2d)),
				0, 0, FONTID(quadrant)); 

  (*DrawText[GetDrawType()])(	P2D_ID_X, 
				P2D_LINE1_Y(quadrant), 
				get_std2d_id_string(p2d)); 

  (void)sprintf(string, 
	"%02d:%02d:%02d.%-3d     TAS(m/s):%3d   overload(ms):%3d",
	ntohs(p2d->hour), ntohs(p2d->minute), ntohs(p2d->second),
	ntohs(p2d->msec), (ntohs(p2d->tas) * 125) / 255, ntohs(p2d->overld));

  /* Update the panel item
  */
  (*SetContext[GetDrawType()])(	pw_xid[quadrant], GXcopy, BLACK, 0, 0,
				FONTID(quadrant));  

  (*DrawText[GetDrawType()])(	P2D_HDR_X(quadrant),
				P2D_LINE2_Y(quadrant), 
				string);
}

/* -------------------------------------------------------------------- */
static void display_grey_scale(int quadrant)
{
/* Checks if a new grey scale data block has been received, and if it is the
 * correct one for display.  If so, the display is generated.
 */
  register int sl_x;			/* slice x position */
  register int sl_y0;                   /* slice top position */
  register int sl_y1;                   /* slice bottom position */
  register int sl_h;                    /* slice height */
  register int sl_w;                    /* slice width */
  register int sl_cnt;                  /* slice counter */
  int new_idx;				/* new probe index */
  int j;                                /* loop counter */
  unsigned char dash_list[GREY_MAX_LEVELS][GREY_PIX_PER_SLICE]; /* pen up/down*/
  int dash_len[GREY_MAX_LEVELS];       		    /* dash_list length */
  struct GreyParticle *grey;   		/* current particle ptr */
  struct GreySlice *slice;         	/* image slice data pointer */

  static int op[GREY_MAX_LEVELS] = 		/* grey level colors */
		{BLU, GRN, RED};
  static int disp_draw[NUM_QUADRANTS] =         /* draw display flag */
               {FALSE,FALSE,FALSE,FALSE,FALSE};
  static int old_seq[NUM_QUADRANTS] =           /* last shared mem seq # */
               {0,0,0,0,0};
  static int half_sec_cnt[NUM_QUADRANTS] =      /* half second counter */
               {0,0,0,0,0};
  static int cur_idx[NUM_QUADRANTS] =      	/* current probe index */
               {0,0,0,0,0};
  static char greybuf[NUM_QUADRANTS][MX_PHYS];	/* current data buffer */

/* Check for any new grey scale data in the shared memory.  Pull it in if it is
   of higher priority than the current data block. */
  if (!frozen[quadrant] && (eth_shm->grey.rseq != old_seq[quadrant])) {
    old_seq[quadrant] = eth_shm->grey.rseq;

/* Get the probe index. */
    if ((new_idx = get_2d_prb_index (quadrant, 
         ((struct GreyParticle*)(eth_shm->greyBuf))->id)) == ERROR)
      return;                           /* invalid id */

/* Check if new data block has a higher display priority than the
   present data block. */
    if (pms2d_probe_selected (quadrant, new_idx) && (!disp_draw[quadrant] ||
        (pms2d_priority (quadrant, P2D_PRIORITY_GET, new_idx) >
         pms2d_priority (quadrant, P2D_PRIORITY_GET, cur_idx[quadrant])))) {
      cur_idx[quadrant] = new_idx;
      memcpy ( greybuf[quadrant], (char*)(eth_shm->greyBuf), MX_PHYS);
      disp_draw[quadrant] = TRUE;
    }
  }  

/* If it is time to draw a new display, and there has been valid data
   received, or there is a new display request, then draw the display. */

  if (((++half_sec_cnt[quadrant] / 2) >= disp_rate[quadrant]) && 
      disp_draw[quadrant]) {
      

    (void)pms2d_priority (quadrant, P2D_PRIORITY_INC, 0);   /* incr priorities */
    (void)pms2d_priority (quadrant, P2D_PRIORITY_RESET, cur_idx[quadrant]);
    disp_draw[quadrant] = FALSE;
    half_sec_cnt[quadrant] = 0;

/* Blank out the memory image.  */
    (*FillArea[GetDrawType()])(mem_image_xid, 0, 0, window_width[quadrant],
       P2D_DISP_HEIGHT(quadrant), 0);

/* Display the first particle's header information. */
    grey = (struct GreyParticle*)greybuf[quadrant];
    show_grey_hdr (quadrant, grey);

/* Compute the slice height and width. */
    sl_w = (window_height[quadrant] - P2D_STAT_HEIGHT(quadrant) - 2 * P2D_STRIP_SPACE) / 
           GREY_PIX_PER_SLICE;
    sl_h = sl_w * GREY_PIX_PER_SLICE;

    for (sl_x = P2D_DISP_MARGIN, sl_y0 = P2D_STRIP_SPACE, 
         sl_y1 = P2D_STRIP_SPACE + sl_h - 1, sl_cnt = 0, 
/*         slice = (struct GreySlice*)(grey + 1); */
         slice = (struct GreySlice *)&grey[1];
         (grey->id[0] == 'G') &&
         (sl_x < window_width[quadrant] - P2D_DISP_MARGIN); sl_x += sl_w) {

/* Build the dash lists for the slice. */
      build_grey_slice (slice, dash_list[GREY_MIN_IDX],
        dash_list[GREY_MID_IDX], dash_list[GREY_MAX_IDX], 
        &dash_len[GREY_MIN_IDX], &dash_len[GREY_MID_IDX],
        &dash_len[GREY_MAX_IDX], sl_w);

/* Write the dashed lines for each grey level into the memory pixrect. */
      for (j = 0; j < GREY_MAX_LEVELS; j++) {
        if (dash_len[j]) {
          (*SetContext[GetDrawType()])(mem_image_xid, GXcopy, op[j], 
          sl_w, LineOnOffDash, 0);
          (*DrawDashLine[GetDrawType()])(mem_image_xid, sl_x, 
             sl_y1 - (int)dash_list[j][0], sl_x, sl_y0, 0, &dash_list[j][1], 
             dash_len[j]);
        }
      }
            
/* If this is the end of a particle, update pointers to the next particle. */
      if (++sl_cnt >= ntohs(grey->slice_cnt)) {
        sl_x += sl_w;
        ((char*)grey) += sizeof (struct GreyParticle) +
          ntohs(grey->slice_cnt) * sizeof (struct GreySlice);
        slice = (struct GreySlice *)&grey[1];
        sl_cnt = 0;
      }
      else
        slice++;
    }  

/* Copy the memory pixrect to the display canvas */
    (*RasterOp[GetDrawType()])(mem_image_xid, 0, 0, pw_xid[quadrant], 0, 0,
               window_width[quadrant], P2D_DISP_HEIGHT(quadrant), GXcopy);
  }
}

/* -------------------------------------------------------------------- */
static void build_grey_slice (slice, min_list, mid_list, max_list, 
                       min_len, mid_len, max_len, sl_w)
struct GreySlice *slice;     		/* data pointer */
unsigned char *min_list;		/* min shading list */
unsigned char *mid_list;		/* mid shading list */
unsigned char *max_list;		/* max shading list */
int *min_len;				/* min_list length */
int *mid_len;				/* mid_list length */
int *max_len;				/* max_list length */
int sl_w;				/* slice display width */

/* Builds the dash lists for a given grey scale image slice. */
{
  register unsigned char tseg;
  register int min_pen_down;		/* min pen down flag */
  register int mid_pen_down;		/* mid pen down flag */
  register int max_pen_down;		/* max pen down flag */
  int j, k;

  memset (min_list, 0, GREY_PIX_PER_SLICE);
  memset (mid_list, 0, GREY_PIX_PER_SLICE);
  memset (max_list, 0, GREY_PIX_PER_SLICE);
  *min_len = 0;
  *mid_len = 0;
  *max_len = 0;
  min_pen_down = FALSE;
  mid_pen_down = FALSE;
  max_pen_down = FALSE;

  for (k = 0, tseg = slice->segment[k]; k < GREY_BYTES_PER_SLICE;
       k++, tseg = slice->segment[k]) {
    for (j = 0; j < GREY_PIX_PER_BYTE; j++, tseg <<= GREY_BITS_PER_PIXEL) {
         
      switch (tseg & GREY_PIXEL_MASK) {

        case GREY_MIN_LEVEL:                      /* min shading */
          if (!min_pen_down) {
            min_pen_down = TRUE;
            (*min_len)++;
          }
          min_list[*min_len] += sl_w;

          if (mid_pen_down) {
            mid_pen_down = FALSE;
            (*mid_len)++;
          }
          mid_list[*mid_len] += sl_w;
         
          if (max_pen_down) {
            max_pen_down = FALSE;
            (*max_len)++;
          }
          max_list[*max_len] += sl_w;
          break;

        case GREY_MID_LEVEL: 			  /* mid shading */
          if (!mid_pen_down) {
            mid_pen_down = TRUE;
            (*mid_len)++;
          }
          mid_list[*mid_len] += sl_w;
 
          if (min_pen_down) {
            min_pen_down = FALSE;
            (*min_len)++;  
          } 
          min_list[*min_len] += sl_w;
         
          if (max_pen_down) {
            max_pen_down = FALSE;
            (*max_len)++;
          }
          max_list[*max_len] += sl_w;
          break;

        case GREY_MAX_LEVEL:                      /* max shading */
          if (!max_pen_down) { 
            max_pen_down = TRUE; 
            (*max_len)++; 
          }
          max_list[*max_len] += sl_w; 
  
          if (min_pen_down) { 
            min_pen_down = FALSE; 
            (*min_len)++;   
          }     
          min_list[*min_len] += sl_w;
          
          if (mid_pen_down) { 
            mid_pen_down = FALSE; 
            (*mid_len)++; 
          } 
          mid_list[*mid_len] += sl_w;
          break;

        default:                                  /* no shading */
          if (min_pen_down) {  
            min_pen_down = FALSE;  
            (*min_len)++;   
          }      
          min_list[*min_len] += sl_w; 
           
          if (mid_pen_down) {  
            mid_pen_down = FALSE;  
            (*mid_len)++; 
          }     
          mid_list[*mid_len] += sl_w;

          if (max_pen_down) { 
            max_pen_down = FALSE; 
            (*max_len)++; 
          } 
          max_list[*max_len] += sl_w;
      }
    }
  }    

/* A dash list must be at least 2 elements long.  If it is only 1, make it
   into 2. */
  if (*min_len == 1) {
    *min_len = 2;
    min_list[1] = GREY_PIX_PER_SLICE * sl_w - 1;
    min_list[2] = 1;
  }
  if (*mid_len == 1) {
    *mid_len = 2;
    mid_list[1] = GREY_PIX_PER_SLICE * sl_w - 1;
    mid_list[2] = 1;
  }
  if (*max_len == 1) {
    *max_len = 2;
    max_list[1] = GREY_PIX_PER_SLICE * sl_w - 1;
    max_list[2] = 1;
  }
}

/* -------------------------------------------------------------------- */
void show_grey_hdr(int quadrant, GreyParticle *grey)

/* Displays the greyscale particle header information. */
{
  char tmp[80];

  /* Show the probe id string. */
  (*SetContext[GetDrawType()])(pw_xid[quadrant], GXcopy,
		get_2d_prb_color(grey->id), 0, 0, FONTID(quadrant));
  (*DrawText[GetDrawType()])(P2D_ID_X, P2D_LINE1_Y(quadrant), grey->id);


  /* Update the panel item */
  sprintf(tmp, "%02d:%02d:%02d.%03d     TAS(m/s):%3d   overload cnt:%4d",
	ntohs(grey->hour), ntohs(grey->minute), ntohs(grey->second),
	ntohs(grey->msec), ntohs(grey->tas), ntohs(grey->ovld_cnt));

  (*SetContext[GetDrawType()])(pw_xid[quadrant], GXcopy, BLACK, 0, 0,
				FONTID(quadrant));
  (*DrawText[GetDrawType()])(P2D_HDR_X(quadrant), P2D_LINE2_Y(quadrant), tmp);
}

/* -------------------------------------------------------------------- */
/* Checks if a new hvps data block has been received, and if it is the
 * correct one for display.  If so, the display is generated.
 */
static void display_hvps(quadrant)
int	quadrant;
{
  int	new_idx;		/* new probe index	*/
  int	sl_x;			/* slice x position	*/
  int	sl_w;			/* slice width		*/
  int	y, i, top_margin;
  unsigned int	shaded, unshaded;
  unsigned short *slice;	/* data pointer		*/

  static int disp_draw[NUM_QUADRANTS] =         /* draw display flag */
               {FALSE,FALSE,FALSE,FALSE,FALSE};
  static int old_seq[NUM_QUADRANTS] =           /* last shared mem seq # */
               {0,0,0,0,0};
  static int half_sec_cnt[NUM_QUADRANTS] =      /* half second counter */
               {0,0,0,0,0};
  static int cur_idx[NUM_QUADRANTS] =           /* current probe index */
               {0,0,0,0,0};
  static P2d_rec hvps[NUM_QUADRANTS];     /* local buffers */

  /* Check for any new hvps data in the shared memory.  Pull it in if it is
   * of higher priority than the current data block.
   */
  if (!frozen[quadrant] && (eth_shm->two.rseq != old_seq[quadrant]))
    {
    old_seq[quadrant] = eth_shm->two.rseq;

    /* Get the probe index. */
    /* Build the probe name string from the block id, and get the probe index.
     */
    if ((new_idx = get_2d_prb_index (quadrant,
         get_std2d_id_string((P2d_rec *)eth_shm->twoBuf))) == ERROR)
      return;                           /* invalid id */

    /* Check if new data block has a higher display priority than the
     * present data block.
     */
//    if (pms2d_probe_selected (quadrant, new_idx) && (!disp_draw[quadrant] ||
//        (pms2d_priority (quadrant, P2D_PRIORITY_GET, new_idx) >
//         pms2d_priority (quadrant, P2D_PRIORITY_GET, cur_idx[quadrant])))) {
      cur_idx[quadrant] = new_idx;
      memcpy((void *)&hvps[quadrant], (void *)eth_shm->twoBuf, sizeof(P2d_rec));
      disp_draw[quadrant] = TRUE;
//      }
    }
  else
    return;


  slice = (unsigned short *)hvps[quadrant].data;
  if (ntohs(*slice) == 0xcaaa)
    {
fprintf(stderr, "HVPS Diagnostic record, %02d:%02d:%02d, 0xcaaa.\n", ntohs(hvps->hour),
	ntohs(hvps->minute), ntohs(hvps->second));
return;
    }

  if (slice[512] == 0x5555 || slice[1024] == 0x5555)
    {
fprintf(stderr, "HVPS Diagnostic record, %02d:%02d:%02d, 0x5555.\n", ntohs(hvps->hour),
	ntohs(hvps->minute), ntohs(hvps->second));
return;
    }

  /* If it is time to draw a new display, and there has been valid data
   * received, or there is a new display request, then draw the display.
   */
  if (((++half_sec_cnt[quadrant] / 2) >= disp_rate[quadrant]) &&
      disp_draw[quadrant]) {

    (void)pms2d_priority (quadrant, P2D_PRIORITY_INC, 0); /* incr priorities */
    (void)pms2d_priority (quadrant, P2D_PRIORITY_RESET, cur_idx[quadrant]);
    disp_draw[quadrant] = FALSE;
    half_sec_cnt[quadrant] = 0;

    /* Blank out the memory image.  */
    (*FillArea[GetDrawType()])(mem_image_xid, 0, 0, window_width[quadrant],
       window_height[quadrant], 0);

    /* Compute the slice height and width. */
    sl_w = (window_height[quadrant] - P2D_STAT_HEIGHT(quadrant) - 2 *
					P2D_STRIP_SPACE) / HVPS_PIX_PER_SLICE;

    if (sl_w == 0)  /* for displays less than 256 pixels, start at -25 */
      {
      sl_w = 1;
      top_margin = -25;
      }
    else
      top_margin = P2D_DISP_MARGIN;

    /* Set the graphics context for the color, width, and dashed lines. */
    (*SetContext[GetDrawType()])(mem_image_xid, GXcopy,
      get_2d_prb_color (get_std2d_id_string (&hvps[quadrant])),
      sl_w, LineSolid, 0);

    /* Return if it's a diagnostic record. */
    if (ntohs(*(unsigned short *)hvps[quadrant].data) == 0xcaaa)
      return;

    for (sl_x = P2D_DISP_MARGIN, slice = (unsigned short *)hvps[quadrant].data, i = 0;
         sl_x < window_width[quadrant] - P2D_DISP_MARGIN && i < 2048;
         sl_x += sl_w)
      {
      while ((ntohs(*slice) & 0xc000) != 0x4000) /* Find start of slice */
        {
        ++slice;
        ++i;
        }

      y = top_margin;

      do
        {
        shaded = (ntohs(*slice) & 0x3f80) >> 7;
        unshaded = ntohs(*slice) & 0x007f;

        if (ntohs(*slice) == 0x4000)
          y += 128 * sl_w;
        else
          y += unshaded * sl_w;

	if (y < 0)  /* for displays less than 256 pixels, start at -25 */
          {
	  shaded += y;
	  y = 0;
	  }

//printf("unshaded = %d\n", y-top_margin);
        if (shaded)
          {
          (*DrawLine[GetDrawType()])(sl_x, y, sl_x, y+(shaded*sl_w));
          y += shaded * sl_w;
          }

        do	/* Skip any timing words.	*/
          {
          ++slice;
          ++i;
          }
        while ((ntohs(*slice) & 0x8000) != 0 && i < 2048);
        }
      while ((ntohs(*slice) & 0x4000) == 0 && i < 2048);
      }

    if (top_margin > 0)
      {
      (*DrawLine[GetDrawType()])(0, P2D_DISP_MARGIN,
			 window_width[quadrant], P2D_DISP_MARGIN);
      (*DrawLine[GetDrawType()])(0, 256*sl_w+P2D_DISP_MARGIN,
			 window_width[quadrant], 256*sl_w+P2D_DISP_MARGIN);
      }

/* Display the header info */
    show_std2d_hdr (quadrant, &hvps[quadrant]);

/* Copy the memory pixrect to the display canvas */
    (*RasterOp[GetDrawType()])(mem_image_xid, 0, 0, pw_xid[quadrant], 0, 0,
               window_width[quadrant], P2D_DISP_HEIGHT(quadrant), GXcopy);
    }

}	/* END DISPLAY_HVPS */

/* END PMS2D.C */
