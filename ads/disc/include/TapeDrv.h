
#ifndef TAPEDRV_H
#define TAPEDRV_H

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mtio.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


#define TRUE	1
#define FALSE	0

#define OK	0
#define ERR	(-1)


class TapeDrv {
public:
  TapeDrv(char *);
  int	Open(char *);
  int	Write(char *, int);
  void	Close();
  int	Filemark(int, int);
  int	Eject();
  ~TapeDrv();

private:
  bool	Opened;
  int	fd;

};

#endif
