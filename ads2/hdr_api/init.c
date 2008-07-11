/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize a tape/file

ENTRY POINTS:	InitFlightHeader()
		ReleaseFlightHeader()

TYPE:		User API

DESCRIPTION:	

REFERENCES:	none outside this module.

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <malloc.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

#include <fcntl.h>
#include <unistd.h>

#include "hdr_api.h"


struct Fl *HAPI_header = 0;	/* Pointer where data is/will be stored	*/
struct var_list	HAPI_var[MAX_VARS]; /* Pointers into *header		*/

char	*HAPI_var_name[MAX_VARS]; /* List of variable names on tape	*/
bool	HAPI_HeaderInitialized = FALSE;
int	taperr;			/* Tape Error, analogous to errno	*/
int	HAPI_name_ptr = 0;	/* Index to current variable		*/
float	HAPI_version;

static	int setup_var_list(), get_itemtype(struct Blk *p);
static	int set_var_name(char *p, int i, int type);


/* -------------------------------------------------------------------- */
int InitFlightHeader(const char name[], int status)
{
  int	fd, rc, nbytes;
  bool	tape = FALSE;

  HAPI_HeaderInitialized = FALSE;

  if (strncmp(name, "/dev", 4) == 0)
    tape = TRUE;


  /* Open File/Tape
   */
  if ((fd = open(name, O_RDONLY)) == ERR)
    {
    fprintf(stderr, "InitFlightHeader: can't open %s\n", name);
    taperr = errno;
    return(ERR);
    }

  if ((HAPI_header= (struct Fl *)malloc((unsigned)TAPE_BLK_SIZE)) == NULL)
    {
    fprintf(stderr, "InitFlightHeader: malloc error\n");
    taperr = errno;
    close(fd);
    return(ERR);
    }

  /* If it's a tape then skip past the first record
   */
  if (tape)
    {
    read(fd, (char *)HAPI_header, TAPE_BLK_SIZE);

    if (strncmp(FIRST_REC_STRING, (char *)HAPI_header,
                strlen(FIRST_REC_STRING)) != 0)
      {
      fprintf(stderr, "InitFlightHeader: !(ADS DATA TAPE)\n");
      free((char *)HAPI_header);
      taperr = BADHDR;
      close(fd);
      return(ERR);
      }

    nbytes = read(fd, (char *)HAPI_header, TAPE_BLK_SIZE);
    }
  else
    {
    read(fd, (char *)HAPI_header, 20);

    if (strncmp((char *)HAPI_header, FIRST_REC_STRING,
                strlen(FIRST_REC_STRING)) != 0)
      lseek(fd, 0L, 0);

    nbytes = read(fd, (char *)HAPI_header, sizeof(struct Fl));
    nbytes += read(fd, &((char *)HAPI_header)[sizeof(struct Fl)],
                   ntohl(HAPI_header->thdrlen));
    }



  /* Realloc to a the real size we need
   */
  HAPI_header = (struct Fl *)realloc((char *)HAPI_header, (unsigned)nbytes);


  /* Check if valid file
   */
  if (strcmp(TAPEHDR_STR, HAPI_header->thdr) != 0)
    {
    fprintf(stderr, "InitFlightHeader: Bad header\n");
    free((char *)HAPI_header);
    taperr = BADHDR;
    close(fd);
    return(ERR);
    }

  /* Setup the var pointers and init the var name list
   */
  if ((rc = setup_var_list()) == OK)
    {
    HAPI_HeaderInitialized = TRUE;
    HAPI_version = atof(HAPI_header->version);
    }

  if (status == LEAVE_OPEN && rc != ERR)
    rc = fd;
  else
    close(fd);

  return(rc);

}	/* END INITFLIGHTHEADER */

/* -------------------------------------------------------------------- */
void ReleaseFlightHeader()
{
  if (HAPI_HeaderInitialized)
    free(HAPI_header);

  HAPI_HeaderInitialized = FALSE;
}

/* -------------------------------------------------------------------- */
static int setup_var_list()
{
  int	i, type, rc = OK;
  char	*p;
  int	nItems, cnt;

  /* loop
   *	- Get Struct Type
   *	- Add var name to list of var names
   *	- Set Pointer into heap
   */
  p = (char *)HAPI_header + ntohl(HAPI_header->item_len);
  nItems = ntohl(HAPI_header->n_items);
  cnt = 0;

  for (i = 0; i < nItems; ++i)
    {
    if ((type = get_itemtype((struct Blk *)p)) != ERR)
      {
      if (set_var_name(p, cnt, type) == ERR)
        { rc = ERR; break; }

      /* Set Pointer & Type, increment to next structure
       */
      HAPI_var[cnt].ptr = (void *)p;
      HAPI_var[cnt].type = type;
      ++cnt;
      }

    p += ntohl(((struct Blk *)p)->item_len);
    }

  HAPI_var[i].ptr = NULL;
  HAPI_var_name[i] = NULL;

  return(rc);

}	/* END SETUP_VAR_LIST */

/* -------------------------------------------------------------------- */
static int get_itemtype(struct Blk *p)
{
  char	*type;

  type = p->item_type;

  /* Determine Struct Type
   */
  switch (type[0])
    {
    case 'C':		// CLIMET_STR * CMidgets
    case 'G':		// Covers GPSMAG, GPSTRIM, GPSTANS2&3
    case 'J':		// Covers TDL's
    case 'L':		// LORAN C
    case 'N':		// UVW Neph
    case 'O':		// Ophir III
    case 'R':		// RDMA
      return(BLK);

    case 'A':
      if (strcmp(type, ASYNC_STR) == 0)
        return(ASYNC);
      else
        return(BLK);

    case 'D':
      if (strcmp(type, DPRES_STR) == 0)
        return(BLK);
      else
      if (strcmp(type, DME_STR) == 0)
        return(DME);
      else
      if (strcmp(type, DIGOUT_STR) == 0)
        return(SH);
      break;

    case 'E':
      return(EVENT);

    case 'H':
      if (strcmp(type, HDR_STR) == 0 || strcmp(type, HW_GPS_STR) == 0 ||
	  strcmp(type, H2OTDL_STR) == 0)
        return(BLK);
      else
      if(strcmp(type, HSKP_STR) == 0)
        return(SH);
      break;

    case 'I':
      if (strcmp(type, IRS_STR) == 0)
        return(IRS);
      else
      if(strcmp(type, INS_STR) == 0)
        return(INS);
      break;

    case 'M':		// MASP
      if (strcmp(type, MASSPEC_STR) == 0)
        return(BLK);
      else
      if (strcmp(type, MCA_STR) == 0)
        return(BLK);
      else
        return(MASP1);

    case 'P':
      if (strcmp(type, PMS1V3_STR) == 0)
        return(PMS1V3);

      if (strcmp(type, PMS1V2_STR) == 0)
        return(PMS1V2);

      if (strcmp(type, PMS2D_STR) == 0)
        return(PMS2D);

      if (strcmp(type, PMS2DH_STR) == 0)
        return(PMS2DH);

      if (strcmp(type, PMS1D_STR) == 0)
        return(PMS1D);

      return(BLK);	// PPS GPS

    case 'S':
      if (strcmp(type, SER_STR) == 0)
        return(SERIAL);

      if (strcmp(type, SDI_STR) == 0)
        return(SH);
      break;

    case 'T':
      return(THDR);

    case 'U':
      return(UVHYG);
    }

  fprintf(stderr, "InitFlightHeader: Unknown struct type, [%s], encountered\n", type);
  taperr = BADTYPE;
  return(ERR);

}	/* END GETITEMTYPE */

/* -------------------------------------------------------------------- */
static int set_var_name(char *p, int i, int type)
{
  switch (type)
    {
    case SH:
      HAPI_var_name[i] = ((struct Sh *)p)->name;

      {	/* Clean up bug in xbuild that allowed spaces to be stored as part
      * of the variable name.
      */
      char   *r, *t;

      for (r = t = ((struct Sh *)p)->name; *r; ++r)
        if (*r != ' ')
          *t++ = *r;

      *t = *r;
      }

      break;

    case INS: case DME: case BLK: case UVHYG:
      HAPI_var_name[i] = ((struct Blk *)p)->item_type;
      break;

    case EVENT:
      if ((HAPI_var_name[i] = malloc((unsigned)LEN_12)) == NULL)
        {
        taperr = errno;
        return(ERR);
        }

/*      strcpy(HAPI_var_name[i], ((struct Evt *)p)->item_type); */
      strcpy(HAPI_var_name[i], "EVT");
      strcat(HAPI_var_name[i], ((struct Evt *)p)->locn);
      break;

    case MASP1:
      if ((HAPI_var_name[i] = malloc((unsigned)LEN_12)) == NULL)
        {
        taperr = errno;
        return(ERR);
        }

      strcpy(HAPI_var_name[i], ((struct Masp *)p)->item_type);
      strcat(HAPI_var_name[i], ((struct Masp *)p)->locn);
      break;

    case PMS1D:
      if ((HAPI_var_name[i] = malloc((unsigned)LEN_12)) == NULL)
        {
        taperr = errno;
        return(ERR);
        }

      strcpy(HAPI_var_name[i], ((struct Pms1 *)p)->name);
      strcat(HAPI_var_name[i], ((struct Pms1 *)p)->locn);
      break;

    case PMS1V2:
      if ((HAPI_var_name[i] = malloc((unsigned)LEN_12)) == NULL)
        {
        taperr = errno;
        return(ERR);
        }

      strcpy(HAPI_var_name[i], ((struct Pms1v2 *)p)->name);
      strcat(HAPI_var_name[i], ((struct Pms1v2 *)p)->locn);
      break;
  
    case PMS1V3:
      if ((HAPI_var_name[i] = malloc((unsigned)LEN_12)) == NULL)
        {
        taperr = errno;
        return(ERR);
        }

      strcpy(HAPI_var_name[i], ((struct Pms1v3 *)p)->name);
      strcat(HAPI_var_name[i], ((struct Pms1v3 *)p)->locn);
      break;
  
    case IRS:
      if ((HAPI_var_name[i] = malloc((unsigned)LEN_12)) == NULL)
        {
        taperr = errno;
        return(ERR);
        }

      strcpy(HAPI_var_name[i], ((struct Irs *)p)->item_type);
      strcat(HAPI_var_name[i], ((struct Irs *)p)->locn);
      break;

    case PMS2D:
      HAPI_var_name[i] = ((struct Pms2 *)p)->name;
      break;

    case PMS2DH:
      HAPI_var_name[i] = ((struct Pms2h *)p)->name;
      break;

    case ASYNC:
      HAPI_var_name[i] = ((struct Asy *)p)->name;
      break;

    case SERIAL:
      HAPI_var_name[i] = ((struct Serial *)p)->name;
      break;

    default:
      taperr = BADTYPE;
      fprintf(stderr, "InitFlightHeader: Unknown struct type encountered\n");
      return(ERR);
    }

  return(OK);

}	/* END SETVARNAME */

/* END INIT.C */
