// MODULE:  SccClass.cc
// PURPOSE: Class for setting up serial port 2 on MV162
// DATE:    3/15/95
// AUTHOR:  Reif Heck

# include <SccClass.h>

// SCC Class constructor

SccClass::SccClass(int port)
{

  if (port == SCC_PORT_A) {
    if ( (port_id = open( "/tyCo/0", O_RDWR, 0666)) < 0) 
      perror("error opening serial port A");
  }
  else {
    if ( (port_id = open( "/tyCo/1", O_RDWR, 0666)) < 0) 
      perror("error opening serial port B");
  }
}


int SccClass::readString (char *str)
{
  int j;
 
  for (j = 0; (readPort ((str + j), sizeof(char)) == sizeof(char)) &&
       (*(str + j) != '\n'); j++);
  return j;
}
 

int SccClass::writeString (char *str)
{
  return writePort (str, strlen (str));
}


// writePort function writes data out port #2 on MV162
int SccClass::writePort (char *str, int len)
{
  if (write (port_id, str,len) == ERROR)
  { 
    perror("error writing to serial port B");
    return(ERROR);
  }
  return len;
}
 

// readPort reads in data from serial port #2 on MV162
// Returns pointer to data holding buffer or ERROR
// if read returns ERROR


int SccClass::readPort(char *str, int len)

{
  int j;

  if (j = read (port_id, str, len) == ERROR)
  {
    perror("error reading from serial port B");
    return ERROR;
  } 
//printf ("j = %d\n", j);	// always returns 0 ??
  return len;
}
