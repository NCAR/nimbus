#include "hdrAPI.h"

#ifdef PNG
#include <zlib.h>
#endif

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

/* -------------------------------------------------------------------- */
Header::Header(const std::string fileName)
{
  _valid = false;
  readHeader(fileName.c_str());
}

/* -------------------------------------------------------------------- */
Header::Header(const char fileName[])
{
  _valid = false;
  readHeader(fileName);
}

/* -------------------------------------------------------------------- */
void Header::readHeader(const char fileName[])
{
  int		nBytes;
  char		buff[65535];
  bool		tape = false;
  bool		gzipped = false;

  _projName[0] = '\0';

  if (strncmp(fileName, "/dev", 4) == 0)
    tape = true;

  if (strstr(fileName, ".gz"))
    {
    std::cout << "hdrAPI: We have gzipped file.\n";
#ifdef PNG
    gzipped = true;
#else
    std::cerr << "hdrAPI: gzip support not compiled into libraf++.\n";
    return;
#endif
    }

  _header = (Fl *)buff;

  if (tape)
    {
    TapeDrive *drv = new TapeDrive(fileName);
    int	len = strlen(FIRST_REC_STRING);

    if (drv->Read(buff) < len)
      {
      std::cerr << "hdrAPI: read problem.\n";
      return;
      }

    if (strncmp(FIRST_REC_STRING, buff, len) != 0)
      {
      std::cerr << "hdrAPI: not an ADS tape.\n";
      return;
      }

    nBytes = drv->Read(buff);
    delete drv;
    }
  else
#ifdef PNG
  if (gzipped)
    {
    gzFile fd;

    if ((fd = gzopen(fileName, "rb")) <= 0)
      {
      std::cerr << "hdrAPI: can't open " << fileName << ".\n";
      return;
      }

    gzread(fd, buff, 20);

    if (strncmp(buff, FIRST_REC_STRING, strlen(FIRST_REC_STRING)) != 0)
      gzseek(fd, 0, SEEK_SET);

    nBytes = gzread(fd, buff, sizeof(Fl));
    nBytes += gzread(fd, &buff[sizeof(Fl)], ntohl(_header->thdrlen));
    gzclose(fd);
    }
  else
#endif
    {
    int	fd;

    if ((fd = open(fileName, O_RDONLY)) == (-1))
      {
      std::cerr	<< "hdrAPI: can't open " << fileName
		<< " fatal, errno = " << errno << ".\n";
      return;
      }

    read(fd, buff, 20);

    if (strncmp(buff, FIRST_REC_STRING, strlen(FIRST_REC_STRING)) != 0)
      lseek(fd, 0, 0);

    nBytes = read(fd, buff, sizeof(Fl));
    nBytes += read(fd, &buff[sizeof(Fl)], ntohl(_header->thdrlen));
    close(fd);
    }

  _hdr = new char [nBytes];
  memcpy((void *)_hdr, (void *)buff, (unsigned)nBytes);
  _header = (Fl *)_hdr;

  /* Check if valid file
   */
  if (strcmp(TAPEHDR_STR, _header->thdr) != 0)
    {
    std::cerr << "hdrAPI: Bad header [" << _header->thdr << "].\n";
    return;
    }

  _valid = true;

}	/* END READHEADER */

/* -------------------------------------------------------------------- */
Header::Header(TapeDrive &drv)
{
  char	buff[65535];
  long	nBytes, len;

  len = strlen(FIRST_REC_STRING);

  if ((nBytes = drv.Read(buff)) < len)
    {
    std::cerr << stderr, "Header: read problem.\n";
    return;
    }

  if (strncmp(FIRST_REC_STRING, buff, len) !=0)
    {
    std::cerr << "Header: not an ADS tape, fatal.\n";
    return;
    }

  if ((nBytes = drv.Read(buff)) == 0)
    {
    std::cerr << "Header: empty tape.\n";
    return;
    }

  _hdr = new char [nBytes];
  memcpy((void *)_hdr, (void *)buff, (unsigned)nBytes);
  _header = (Fl *)_hdr;

  /* Check if valid file
   */
  if (strcmp(TAPEHDR_STR, _header->thdr) != 0)
    {
    std::cerr << "Header: Bad header, fatal.\n";
    return;
    }

  _valid = true;

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
const void *Header::GetFirst()
{
  if (!_valid)
    return(0);

  _currentIndx = 0;
  _currentPtr = (char *)_header + sizeof(Fl);

  return(_currentPtr);

}	/* END GETFIRST */

/* -------------------------------------------------------------------- */
const void *Header::GetFirst(const char blkName[])
{
  if (!_valid)
    return(0);

  _currentIndx = 0;
  _currentPtr = (char *)_header + ntohl(((Fl *)_header)->item_len);

  for (_currentIndx = 0; _currentIndx < ntohl(_header->n_items); ++_currentIndx)
    {
    if (strcmp(((Blk *)_currentPtr)->item_type, blkName) == 0)
      return(_currentPtr);

    _currentPtr += ntohl(((Blk *)_currentPtr)->item_len);
    }

  return(0);

}	/* END GETFIRST */

/* -------------------------------------------------------------------- */
const void *Header::GetSDI(const char varName[])
{
  if (!_valid)
    return(0);

  _currentIndx = 0;
  _currentPtr = (char *)_header + ntohl(((Fl *)_header)->item_len);

  for (_currentIndx = 0; _currentIndx < ntohl(_header->n_items); ++_currentIndx)
    {
    if (strcmp(((Blk *)_currentPtr)->item_type, "SDI") == 0 &&
        strcmp(((Sh *)_currentPtr)->name, varName) == 0)
        return(_currentPtr);

    _currentPtr += ntohl(((Blk *)_currentPtr)->item_len);
    }

  return(0);

}	/* END GETFIRST */

/* -------------------------------------------------------------------- */
const void *Header::GetNext()
{
  if (!_valid)
    return(0);

  if (++_currentIndx >= ntohl(_header->n_items))
    return(0);

  _currentPtr += ntohl(((Blk *)_currentPtr)->item_len);

  return(_currentPtr);

}	/* END GETNEXT */

/* -------------------------------------------------------------------- */
const void *Header::GetNext(const char blkName[])
{
  if (!_valid)
    return(0);

  if (++_currentIndx >= ntohl(_header->n_items))
    return(0);

  _currentPtr += ntohl(((Blk *)_currentPtr)->item_len);

  for (; _currentIndx < ntohl(_header->n_items); ++_currentIndx)
    {
    if (strcmp(((Blk *)_currentPtr)->item_type, blkName) == 0)
      return(_currentPtr);

    _currentPtr += ntohl(((Blk *)_currentPtr)->item_len);
    }

  return(0);

}	/* END GETNEXT */

/* -------------------------------------------------------------------- */
const char *Header::ProjectName()
{
  if (!_valid)
    return(0);

  if (strlen(_projName) == 0)
    {
    char	*p;

    if ((p = getenv("PROJ_DIR")))
      {
      FILE	*fp;
      char	tmp[256];

      sprintf(tmp, "%s/%s/ProjectName", p, ProjectNumber());

      if ((fp = fopen(tmp, "r")))
        {
        fgets(_projName, 32, fp);
        fclose(fp);
        _projName[31] = '\0';

        if (_projName[strlen(_projName)-1] == '\n')
          _projName[strlen(_projName)-1] = '\0';
        }
      }
    }

  return(_projName);

}	/* END PROJECTNAME */

/* -------------------------------------------------------------------- */
Header::~Header()
{
  delete [] _hdr;

}	/* END DESTRUCTOR */

/* END HDRAPI.CC */
