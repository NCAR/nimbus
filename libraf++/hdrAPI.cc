/*
-------------------------------------------------------------------------
OBJECT NAME:	hdrAPI.cc

FULL NAME:	Header API, C++ version.

ENTRY POINTS:	

STATIC FNS:	

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "hdrAPI.h"

#include <memory.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

/* -------------------------------------------------------------------- */
Header::Header(const string fileName)
{
  readHeader(fileName.c_str());
}

/* -------------------------------------------------------------------- */
Header::Header(const char fileName[])
{
  readHeader(fileName);
}

/* -------------------------------------------------------------------- */
void Header::readHeader(const char fileName[])
{
  int		fd;
  int		nBytes;
  char		buff[65535];
  bool		tape = false;
  TapeDrive	*drv;

  projName[0] = '\0';

  if (strncmp(fileName, "/dev", 4) == 0)
    tape = true;

  header = (Fl *)buff;

  if (tape)
    {
    drv = new TapeDrive(fileName);
    }
  else
    {
    if ((fd = open(fileName, O_RDONLY)) == (-1))
      {
      cerr << "Header: can't open " << fileName << " fatal, errno = " << errno << ".\n";
      exit(1);
      }
    }


  if (tape)
    {
    int	len = strlen(FIRST_REC_STRING);

    if (drv->Read(buff) < len)
      {
      cerr << "Header: read problem.\n";
      exit(1);
      }

    if (strncmp(FIRST_REC_STRING, buff, len) != 0)
      {
      cerr << "Header: not an ADS tape, fatal.\n";
      exit(1);
      }

    nBytes = drv->Read(buff);
    delete drv;
    }
  else
    {
    read(fd, buff, 20);

    if (strncmp(buff, FIRST_REC_STRING, strlen(FIRST_REC_STRING)) != 0)
      lseek(fd, 0, 0);

    nBytes = read(fd, buff, sizeof(Fl));
    nBytes += read(fd, &buff[sizeof(Fl)], ntohl(header->thdrlen));
    close(fd);
    }


  hdr = new char [nBytes];
  memcpy((void *)hdr, (void *)buff, (unsigned)nBytes);
  header = (Fl *)hdr;

  /* Check if valid file
   */
  if (strcmp(TAPEHDR_STR, header->thdr) != 0)
    {
    cerr << "Header: Bad header, fatal [" << header->thdr << "].\n";
    exit(1);
    }

}	/* END READHEADER */

/* -------------------------------------------------------------------- */
Header::Header(TapeDrive &drv)
{
  char	buff[65535];
  long	nBytes, len;

  len = strlen(FIRST_REC_STRING);

  if ((nBytes = drv.Read(buff)) < len)
    {
    cerr << stderr, "Header: read problem.\n";
    exit(1);
    }

  if (strncmp(FIRST_REC_STRING, buff, len) !=0)
    {
    cerr << "Header: not an ADS tape, fatal.\n";
    exit(1);
    }

  if ((nBytes = drv.Read(buff)) == 0)
    {
    cerr << "Header: empty tape.\n";
    exit(1);
    }

  hdr = new char [nBytes];
  memcpy((void *)hdr, (void *)buff, (unsigned)nBytes);
  header = (Fl *)hdr;

  /* Check if valid file
   */
  if (strcmp(TAPEHDR_STR, header->thdr) != 0)
    {
    cerr << "Header: Bad header, fatal.\n";
    exit(1);
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void *Header::GetFirst()
{
  currentIndx = 0;
  currentPtr = (char *)header + sizeof(Fl);

  return(currentPtr);

}	/* END GETFIRST */

/* -------------------------------------------------------------------- */
void *Header::GetFirst(char blkName[])
{
  currentIndx = 0;
  currentPtr = (char *)header + ntohl(((Fl *)header)->item_len);

  for (currentIndx = 0; currentIndx < ntohl(header->n_items); ++currentIndx)
    {
    if (strcmp(((Blk *)currentPtr)->item_type, blkName) == 0)
      return(currentPtr);

    currentPtr += ntohl(((Blk *)currentPtr)->item_len);
    }

  return(NULL);

}	/* END GETFIRST */

/* -------------------------------------------------------------------- */
void *Header::GetSDI(char varName[])
{
  currentIndx = 0;
  currentPtr = (char *)header + ntohl(((Fl *)header)->item_len);

  for (currentIndx = 0; currentIndx < ntohl(header->n_items); ++currentIndx)
    {
    if (strcmp(((Blk *)currentPtr)->item_type, "SDI") == 0 &&
        strcmp(((Sh *)currentPtr)->name, varName) == 0)
        return(currentPtr);

    currentPtr += ntohl(((Blk *)currentPtr)->item_len);
    }

  return(NULL);

}	/* END GETFIRST */

/* -------------------------------------------------------------------- */
void *Header::GetNext()
{
  if (++currentIndx >= ntohl(header->n_items))
    return(NULL);

  currentPtr += ntohl(((Blk *)currentPtr)->item_len);

  return(currentPtr);

}	/* END GETNEXT */

/* -------------------------------------------------------------------- */
void *Header::GetNext(char blkName[])
{
  if (++currentIndx >= ntohl(header->n_items))
    return(NULL);

  currentPtr += ntohl(((Blk *)currentPtr)->item_len);

  for (; currentIndx < ntohl(header->n_items); ++currentIndx)
    {
    if (strcmp(((Blk *)currentPtr)->item_type, blkName) == 0)
      return(currentPtr);

    currentPtr += ntohl(((Blk *)currentPtr)->item_len);
    }

  return(NULL);

}	/* END GETNEXT */

/* -------------------------------------------------------------------- */
char *Header::ProjectName()
{
  if (strlen(projName) == 0)
    {
    char	*p;

    if ((p = getenv("PROJ_DIR")))
      {
      FILE	*fp;
      char	tmp[256];

      sprintf(tmp, "%s/%s/ProjectName", p, ProjectNumber());

      if ((fp = fopen(tmp, "r")))
        {
        fgets(projName, 32, fp);
        fclose(fp);
        projName[31] = '\0';

        if (projName[strlen(projName)-1] == '\n')
          projName[strlen(projName)-1] = '\0';
        }
      }
    }

  return(projName);

}	/* END PROJECTNAME */

/* -------------------------------------------------------------------- */
Header::~Header()
{
  delete [] hdr;

}	/* END DESTRUCTOR */

/* END HDRAPI.CC */
