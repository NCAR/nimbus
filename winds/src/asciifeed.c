/* asciifeed.c -- manage ascii feeds sent out over serial line 
*/

#include <stdio.h>

#include <printers.h>
#include <socket.h>
#include <lengths.h>
#include <shmem.h>
#include <display.h>
#include <constants.h>
#include <proto.h>

static int	AsciiDataEnabled;	/* TRUE only if ascii data is to
                                         * be sent out serial line */
static int	NumAsciiParms = 0;
static char	**AsciiParm;


/**********************  GetAsciiParmsFromFile()  *****************************/
void GetAsciiParmsFromFile(char filename[])
{
 FILE *WINDSfopen(),*FileHandle;
 char buffer[NAMLEN*2];
 int parm;

 if ( (FileHandle=WINDSfopen(filename,"r")) == NULL) {
  (void)WINDSfclose(FileHandle);
  (void)fprintf(stderr,"no ascii data present\n");
  return;
 }

/*
do a first run through to determine how many entries
*/
 while (fscanf(FileHandle,"%s",buffer)!=EOF) {
  NumAsciiParms++;
 }
 (void)WINDSfclose(FileHandle);
/*
do a second run through, using # entries to size buffers
*/
 FileHandle=WINDSfopen(filename,"r");
 AsciiParm=(char **)malloc((unsigned int)(NumAsciiParms*sizeof(char**)));
 for (parm=0; parm<NumAsciiParms; parm++) {
  AsciiParm[parm]=(char *)malloc((unsigned int)(NAMLEN*2));
  (void)fscanf(FileHandle,"%s",AsciiParm[parm]);
  AsciiParm[parm][NAMLEN-1]='\0';
 }
 (void)WINDSfclose(FileHandle);
}

/**************************  GetNumAsciiParms()  *****************************/
int GetNumAsciiParms (void)
{
 return NumAsciiParms;
}

/**************************  SetAsciiFlag()  *******************************/
void SetAsciiFlag(char filename[])
{
 if ((AsciiDataEnabled=AsciiDataNeeded())) {
  GetAsciiParmsFromFile(filename);
 }
}

/*****************************  AsciiData()  *********************************/
int AsciiData (void)
{
 return AsciiDataEnabled;
}

/**************************  SendAsciiData()  *******************************/
void SendAsciiData(int point, int hh, int mm, int ss)
{
  int	value,index,mode;

  static char	*buff = 0;
  static int	last_point = -1;

  if (point == last_point)
    return;

  if (!buff)
    buff = (char *)malloc((unsigned int)((NumAsciiParms+3)*VALUELEN));


  sprintf(buff, "%04d/%02d/%02d %02d:%02d:%02d",
    (int)get_derived_value("YEAR", (point % get_datablock_size()) * *len_derive),
    (int)get_derived_value("MONTH", (point % get_datablock_size()) * *len_derive),
    (int)get_derived_value("DAY", (point % get_datablock_size()) * *len_derive),
    hh, mm, ss);
  index = strlen(buff);

  for (value = 0; value < GetNumAsciiParms(); value++)
  {
    if ((mode = get_mode_by_name(AsciiParm[value])) != ERROR)
    {
      if (mode == DERIVED)
      {
	sprintf(&buff[index]," %+8e", get_derived_value(
	      AsciiParm[value],(point%get_datablock_size())**len_derive));
      } else {
	sprintf(&buff[index]," %+8e", get_raw_value(
	      AsciiParm[value],(point%get_datablock_size())**len_raw));
      }

      index = strlen(buff);
    } else {
      if (last_point == -1)
      {
	sprintf(&buff[index]," %+8e", 0.0);
	fprintf(stderr,
	    "WARNING: unknown variable %s omitted from serial feed\n",AsciiParm[value]);
      }
    }
  }


  sprintf(&buff[index],"\n");
  if (write_to_socket(ASCIIVALUES,&buff[0],strlen(&buff[0])) == EBADWRITE)
  {
    perror("ascii write_to_socket");
    if (serial_feed_debug())
      fprintf(stderr,"WARNING: no ascii data sent -- socket pblm\n");
  } else {
    if (serial_feed_debug())
      fprintf(stderr,"ASCIIFEED: \n%s\n",buff);
  }

  last_point = point;

}
