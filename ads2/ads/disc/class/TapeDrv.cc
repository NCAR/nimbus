#include <TapeDrv.h>


/* -------------------------------------------------------------------- */
TapeDrv::TapeDrv(char *deviceName)
{
  Opened = FALSE;

  if ((fd = open(deviceName, O_RDONLY)) != ERR)
    {
    Opened = TRUE;
    Eject();
    Close();
    }

}

/* -------------------------------------------------------------------- */
int TapeDrv::Open(char *deviceName)
{
  struct mtget tstat;
  struct mtop op;

  if ((fd = open(deviceName, O_WRONLY)) == ERR)
    {
#ifndef SVR4
    fprintf(stderr, "TapeDrv::Open: can't open %s\n", deviceName);
#endif
    return(ERR);
    }

#ifndef SVR4
  ioctl(fd, MTIOCGET, &tstat);

  if (GMT_ONLINE(tstat.mt_gstat))
    {
    Opened = TRUE;

    op.mt_op = MTSETBLK;
    op.mt_count = 0;
    ioctl(fd, MTIOCTOP, &op);

    op.mt_op = MTSETDENSITY;
    op.mt_count = 0x15;
    ioctl(fd, MTIOCTOP, &op);

    return(OK);
    }

  close(fd);
  return(ERR);

#else
  return(OK);
#endif

}

/* -------------------------------------------------------------------- */
int TapeDrv::Write(char *buffer, int len)
{
  int rc;

  if (!Opened)
    return(ERR);

  if ((rc = write(fd, buffer, len)) != len)
    {
    fprintf(stderr, "TapeDrv::Write() bad return value of %d, errno = %d\n",
      rc, errno);
    }

  return(rc);

}

/* -------------------------------------------------------------------- */
int TapeDrv::Filemark(int n, int val)
{
  int		rc = 0;
  struct mtop	mt_command;

  if (Opened)
    {
    mt_command.mt_op = MTWEOF;
    mt_command.mt_count = 1;

    if ((rc = ioctl(fd, MTIOCTOP, &mt_command)) == ERR)
      fprintf(stderr, "TapeDrv::FileMark() write EOF failed, errno = %d.\n", errno);
    }

  return(rc);

}

/* -------------------------------------------------------------------- */
void TapeDrv::Close()
{
  if (Opened)
    {
    close(fd);
    Opened = FALSE;
    }

}

/* -------------------------------------------------------------------- */
int TapeDrv::Eject()
{
  int		rc = 0;
  struct mtop	mt_command;

  if (Opened)
    {
    mt_command.mt_op = MTOFFL;
    mt_command.mt_count = 1;

    if ((rc = ioctl(fd, MTIOCTOP, &mt_command)) == ERR)
      fprintf(stderr, "TapeDrv::Eject() write EOF failed, errno = %d.\n", errno);
    }

  return(rc);

}

/* -------------------------------------------------------------------- */
TapeDrv::~TapeDrv()
{
  Eject();

}

/* END TAPEDRV.CC */
