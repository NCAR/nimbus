#include <stdio.h>
#include <ctape.h>
#include <constants.h>
#include <proto.h>


/****************************  INIT_HEADER()  ********************************/

InitHeader(hdr_file)
char *hdr_file;
{

 if ( InitFlightHeader(hdr_file, CLOSE) == ERR) {
  (void)fprintf(stderr,"Problem w/InitFlightHeader(): %d\n",taperr);
  return ERROR;
 }
return OK;
}

/****************************  FIND_VAR()  ************************************/

find_var(varname)
char *varname;
{
char *result;
 if (GetName(varname,&result)==OK)
  return TRUE;
 return FALSE;
}

/****************************  HDR_FLTNO()  **********************************/

char *
hdr_fltno()
/* return pointer to flight number field in header
*/
{
static char *fltno;

 GetFlightNumber(&fltno);
 return ( (char *)fltno);
}

/****************************  HDR_DATE()  ************************************/

char *
hdr_date()
/* return pointer to date field in header
*/
{
static char *date;
 
 GetHeaderDate(&date);
 return( (char *)date);
}

/***********************  GETHEADER()  ****************************************/

struct Fl*
GetHeader()
{
 return get_header();
}

/***********************  READ_HDR()  ****************************************/

int
read_hdr(fd,hdr_ptr)
int fd;
struct Fl *hdr_ptr;

{
  if (!read (fd, (char *)hdr_ptr, MX_PHYS)) {
    (void)fprintf(stderr,"FATAL: couldn't read header file\n");
    return ERROR;
  }
  if (strcmp (hdr_ptr->thdr, "THDR")) {
    (void)fprintf(stderr,"FATAL: Invalid tape header i.d. string read.\n");
    return ERROR;
  }
  return 0;
}

/**************************   IS_DIGITAL_TYPE()  ******************************/ 
int
is_digital_type(type)
char *type;
{
 if (!type) {
  return ERROR;
 }
 return (!(strcmp(type,"D")));
}

