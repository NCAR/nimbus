/*  This looks like C, but it's really C++!!
 *      $Id: Exa.cc,v 1.1.1.1 1999/11/15 22:35:36 chris Exp $
 *
 *      Module:          Exa.cc
 *      Original Author: Richard E. Neitzel
 *      Copyright by the National Center for Atmospheric Research
 *      Date:            $Date: 1999/11/15 22:35:36 $
 *
 * revision history
 * ----------------
 * $Log: Exa.cc,v $
 * Revision 1.1.1.1  1999/11/15 22:35:36  chris
 * Import source
 *
 *
 *
 * description:
 *
 */

#include <Exa.h>

/******************************************************************************
** Public Functions.
******************************************************************************/
 
/* -------------------------------------------------------------------- */
Exa::Exa()
{
  fd = -1;
  lun = 0;
  closeOnDest = 0;
  rewOnDest = 0;
  currDensity = -1;
  currMotion = -1;
  currReconnect = -1;
}
 
/* -------------------------------------------------------------------- */
int Exa::Open (char *file, int mode, u_char l)

// Attempt to open the drive.
{
  if (fd >= 0) 				// drive is already open
    return fd;

  if ((fd = open(file, mode)) != ERROR) {
    lun = l << 5;
    closeOnDest = 1;
    rewOnDest = 1;
    currDensity = -1;
    currMotion = -1;
    currReconnect = -1;
  }
  return fd;
}
 
/* -------------------------------------------------------------------- */
void Exa::Close ()
 
// Close the drive.
{
  close (fd);

  fd = -1;
  lun = 0;
  closeOnDest = 0;
  rewOnDest = 0;
  currDensity = -1;
  currMotion = -1;
  currReconnect = -1;
}

/* -------------------------------------------------------------------- */
int Exa::FwdSpace(int count)
{
    Clr();
 
    cmd[0] = Exa::space;
    cmd[1] = lun;
 
    union {
        int i;
        char c[4];
    } t;
 
    t.i = count;
 
    cmd[2] = t.c[1];
    cmd[3] = t.c[2];
    cmd[4] = t.c[3];
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::BackSpace(int count)
{
    return(FwdSpace(~count));
}
 
/* -------------------------------------------------------------------- */
int Exa::FwdSpaceFile(int count)
{
    Clr();
 
    cmd[0] = Exa::space;
    cmd[1] = lun | 1;
 
    union {
        int i;
        char c[4];
    } t;
 
    t.i = count;
 
    cmd[2] = t.c[1];
    cmd[3] = t.c[2];
    cmd[4] = t.c[3];
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}

/* -------------------------------------------------------------------- */
int Exa::BackSpaceFile(int count)
{
    return(FwdSpaceFile(~count));
}
 
/* -------------------------------------------------------------------- */
int Exa::Eot()
{
    Clr();
 
    cmd[0] = Exa::space;
    cmd[1] = lun | 3;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Locate(int block)
{
    Clr();
 
    cmd[0] = Exa::locate;
    cmd[1] = lun;
 
    memcpy(&cmd[3],&block,4);
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 10;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Rewind()
{
    Clr();
 
    cmd[0] = Exa::rewind;
    cmd[1] = lun;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Erase()
{
    Clr();
 
    cmd[0] = Exa::erase;
    cmd[1] = lun | 1;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Eject()
{
    Clr();
 
    cmd[0] = Exa::eject;
    cmd[1] = lun;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Lock()
{
    Clr();
 
    cmd[0] = Exa::lock;
    cmd[1] = lun;
    cmd[4] = 1;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}

/* -------------------------------------------------------------------- */
int Exa::Unlock()
{
    Clr();
 
    cmd[0] = Exa::lock;
    cmd[1] = lun;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Release()
{
    Clr();
 
    cmd[0] = Exa::release;
    cmd[1] = lun | 0x18;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Reserve()
{
    Clr();
 
    cmd[0] = Exa::reserve;
    cmd[1] = lun | 0x18;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Reset()
{
    Clr();
 
    cmd[0] = Exa::reset;
 
    scsi.uscsi_flags = USCSI_ISOLATE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Read(void *buffer, size_t size)
{
    Clr();
 
    cmd[0] = Exa::read;
    cmd[1] = lun;
 
    cmd[2] = (u_char)((size & 0xff0000) >> 16);
    cmd[3] = (u_char)((size & 0xff00) >> 8);
    cmd[4] = (u_char)(size & 0xff);
 
    if (size > EXA_MAX_BUFFER)
      return(BigRead(buffer,size));
 
    scsi.uscsi_flags = USCSI_ISOLATE | USCSI_READ;
    scsi.uscsi_cdblen = 6;
    scsi.uscsi_bufaddr = (caddr_t)buffer;
    scsi.uscsi_buflen = size;
 
    int i = SendCmd();
 
    if (i == Exa::ok)
      return(size);
 
    if (i == Exa::busy || i == Exa::reserved)
      return(0);
 
    ReadSense(NULL);
 
    if (!(sense[0] & 0x80))
      return(0);                // Assume it was a fatal error.
 
    if (!(sense[2] & (Exa::no_sense | Exa::ili)))
      return(0);
 
    int moved;
 
    memcpy(&moved,&sense[3],4);
 
    if (moved > 0)
      size -= moved;
 
    return(size);
}
 
/* -------------------------------------------------------------------- */
int Exa::Write(void *buffer, size_t size)
{
    Clr();
 
    cmd[0] = Exa::write;
    cmd[1] = lun;
 
    cmd[2] = (u_char)((size & 0xff0000) >> 16);
    cmd[3] = (u_char)((size & 0xff00) >> 8);
    cmd[4] = (u_char)(size & 0xff);
 
    if (size > EXA_MAX_BUFFER) {
//    printf ("calling BigWrite\n");
      return(BigWrite(buffer,size));
    }
 
    scsi.uscsi_flags = USCSI_ISOLATE | USCSI_WRITE;
    scsi.uscsi_cdblen = 6;
    scsi.uscsi_bufaddr = (caddr_t)buffer;
    scsi.uscsi_buflen = size;
 
    int i = SendCmd();
 
    if (i == Exa::ok)
      return(size);
 
    if (i == Exa::busy || i == Exa::reserved)
      return(0);
 
    ReadSense(NULL);
 
    if (!(sense[0] & 0x80))
      return(0);                // Assume it was a fatal error.
 
    if (!(sense[2] & (Exa::no_sense | Exa::ili)))
      return(0);
 
 
    int moved;
 
    memcpy(&moved,&sense[3],4);
 
    if (moved > 0)
      size -= moved;
 
    return(size);
}
 
/* -------------------------------------------------------------------- */
int Exa::ReadSense(void *buffer, size_t size)
{
    Clr();
 
    cmd[0] = Exa::request_sense;
    cmd[1] = lun;
 
    cmd[4] = (u_char)(size & 0xff);
 
    scsi.uscsi_flags = USCSI_ISOLATE | USCSI_READ;
    scsi.uscsi_cdblen = 6;
 
    if (buffer != NULL)
      scsi.uscsi_bufaddr = (caddr_t)buffer;
    else
      {
          memset(sense,0,sizeof(sense));
          scsi.uscsi_bufaddr = (caddr_t)sense;
          if (size > 29)
            size = 29;
      }
 
    scsi.uscsi_buflen = size;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::Position()
{
    Clr();
 
    cmd[0] = Exa::position;
    cmd[1] = lun;
 
    scsi.uscsi_flags = USCSI_ISOLATE | USCSI_READ;
    scsi.uscsi_cdblen = 10;
 
    int pos[5];
 
    scsi.uscsi_bufaddr = (caddr_t)pos;
    scsi.uscsi_buflen = sizeof(pos);
 
    int i = SendCmd();
 
 
    if (i != Exa::ok)
      return(-1);
 
    i = pos[0];
 
    if (i & 4)
      return(0);
 
    return(pos[1]);
}
 
/* -------------------------------------------------------------------- */
int Exa::Filemark(int count, int size)
{
    Clr();
 
    cmd[0] = Exa::write_filemark;
    cmd[1] = lun;
 
    cmd[2] = (u_char)((count & 0xff0000) >> 16);
    cmd[3] = (u_char)((count & 0xff00) >> 8);
    cmd[4] = (u_char)(count & 0xff);
 
    if (size)
      cmd[5] = 0x80;
 
    scsi.uscsi_flags = USCSI_ISOLATE | USCSI_WRITE;
    scsi.uscsi_cdblen = 6;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::SendCmd()
{
    scsi.uscsi_cdb = (caddr_t)&cmd;
 
    int i = ioctl(fd,USCSICMD,&scsi);
 
    if (i < 0)
      {
          perror("");
          return(i);
      }
 
    return((int)scsi.uscsi_status & 0xffff);
}
 
/* -------------------------------------------------------------------- */
int Exa::BigRead(void *buffer, size_t size)
{
    int bytes = Read(buffer,EXA_MAX_BUFFER);
 
    if (bytes <= 0)
      return(bytes);
 
    buffer += bytes;
    size -= bytes;
 
    do
      {
          size_t tsize = size;
 
          if (tsize > EXA_MAX_BUFFER)
            tsize = EXA_MAX_BUFFER;
 
          int i = Read(buffer,tsize);
 
          if (i <= 0)
            return(bytes);
 
          size -= i;
          buffer += i;
 
          bytes += i;
 
      } while (size);
 
    return(bytes);
}
 
/* -------------------------------------------------------------------- */
int Exa::BigWrite(void *buffer, size_t size)
{
    int bytes = Write(buffer,EXA_MAX_BUFFER);
 
    if (bytes <= 0)
      return(bytes);
 
    buffer += bytes;
    size -= bytes;
 
    do
      {
          size_t tsize = size;
 
          if (tsize > EXA_MAX_BUFFER)
            tsize = EXA_MAX_BUFFER;
 
          int i = Write(buffer,tsize);
 
          if (i <= 0)
            return(bytes);
 
          size -= i;
          buffer += i;
 
          bytes += i;
 
      } while (size);
 
    return(bytes);
 
}
 
/* -------------------------------------------------------------------- */
int Exa::ModeSense(void *buffer, int page)
{
    Clr();
 
    cmd[0] = mode_sense;
    cmd[1] = lun;
    cmd[2] = (char)page;
 
    char size;
 
    switch(page)
      {
        case Exa::non_page:
          size = 0x11;
          break;
 
        case Exa::read_write_page:
          size = 0x15;
          break;
 
        case Exa::connect_page:
          size = 0x18;
          break;
 
        case Exa::control_page:
          size = 0x14;
          break;
 
        case Exa::compression_page:
          size = 0x1c;
          break;
 
        case Exa::configuration_page:
          size = 0x1b;
          break;
 
        case Exa::partition_page:
          size = 0x16;
          break;
        case Exa::vendor_page1:
          size = 0x12;
          break;
 
        case Exa::vendor_page2:
          size = 0x12;
          break;
 
        case Exa::compress_status_page:
          size = 0x19;
          break;
 
        case Exa::all_pages:
          size = 0xff;
          break;
 
        default:
          return(-1);
          break;
      }
 
    cmd[4] = size;
 
    scsi.uscsi_flags = USCSI_ISOLATE | USCSI_READ;
    scsi.uscsi_cdblen = 6;
 
    scsi.uscsi_bufaddr = (caddr_t)buffer;
    scsi.uscsi_buflen = (int)size & 0xff;
 
    return(SendCmd());
}
 
/* -------------------------------------------------------------------- */
int Exa::ModeSelect(void *buffer, int page)
{
    Clr();
 
    cmd[0] = mode_select;
    cmd[1] = lun;
    cmd[2] = (char)page;
 
    char size;
 
    switch(page)
      {
        case Exa::non_page:
          size = 0x11;
          break;
 
        case Exa::read_write_page:
          size = 0x15;
          break;
 
        case Exa::connect_page:
          size = 0x18;
          break;
 
        case Exa::control_page:
          size = 0x14;
          break;
 
        case Exa::compression_page:
          size = 0x1c;
          break;
 
        case Exa::configuration_page:
          size = 0x1b;
          break;
 
        case Exa::partition_page:
          size = 0x16;
          break;
        case Exa::vendor_page1:
          size = 0x12;
          break;
 
        case Exa::vendor_page2:
          size = 0x12;
          break;
 
        case Exa::compress_status_page:
          size = 0x19;
          break;
 
        case Exa::all_pages:
          size = 0xff;
          break;
 
        case Exa::header_only:		// transfer only the parameter list hdr
          cmd[2] = 0;
          size = 0x04;
          break;
 
        default:
          return(-1);
          break;
      }
 
    cmd[4] = size;
 
    scsi.uscsi_flags = USCSI_ISOLATE | USCSI_WRITE;
    scsi.uscsi_cdblen = 6;
 
    scsi.uscsi_bufaddr = (caddr_t)buffer;
    scsi.uscsi_buflen = (int)size & 0xff;
 
    return(SendCmd());
}

/* -------------------------------------------------------------------- */
int Exa::Buffered()

// Put the drive into the buffered mode.
{
  u_char buffer[4] =  {0, 0, 0x10, 0};

  return(ModeSelect(buffer,Exa::header_only));
}
 
/* -------------------------------------------------------------------- */
int Exa::Unbuffered()
 
// Put the drive into the unbuffered mode.
{
  u_char buffer[4] =  {0, 0, 0, 0};

  return(ModeSelect(buffer,Exa::header_only));
}
 
/* -------------------------------------------------------------------- */
int Exa::FreeSpace()
{
    int i = ReadSense();
 
    if (i != Exa::ok)
      return(i);
 
    union {
        int i;
        char c[4];
    } size;
 
    size.i = 0;
 
    size.c[1] = sense[23];
    size.c[2] = sense[24];
    size.c[3] = sense[25];
 
    return(size.i);
}
 
/* -------------------------------------------------------------------- */
int Exa::GetDensity()
{
    if (currDensity == -1)
      {
          u_char buffer[17];
 
          int i = ModeSense(buffer,Exa::non_page);
 
          if (i != Exa::ok)
            return(i);
 
          currDensity = (int)buffer[4] & 0xff;
      }
    return(currDensity);
}
 
/* -------------------------------------------------------------------- */
int Exa::Density(Exa::density density)
{
    u_char buffer[17];
 
    int i = ModeSense(buffer,Exa::non_page);
 
    if (i != Exa::ok)
      return(i);
 
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] &= 0x7f;
    buffer[4] = density;
 
    currDensity = density & 0xff;
 
    return(ModeSelect(buffer,Exa::non_page));
}
 
/* -------------------------------------------------------------------- */
int Exa::GetMotion()
{
    if (currMotion == -1)
      {
          u_char buffer[17];
 
          int i = ModeSense(buffer,Exa::non_page);
 
          if (i != Exa::ok)
            return(i);
 
          currMotion = (int)buffer[15] & 0xff;
      }
    return(currMotion);
}
 
 
/* -------------------------------------------------------------------- */
int Exa::Motion(int threshold)
{
    u_char buffer[17];
 
    int i = ModeSense(buffer,Exa::non_page);
 
    if (i != Exa::ok)
      return(i);
 
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] &= 0x7f;
    buffer[15] = threshold;
 
    currMotion = threshold;
 
    return(ModeSelect(buffer,Exa::non_page));
}
 
/* -------------------------------------------------------------------- */
int Exa::GetReconnect()
{
    if (currReconnect == -1)
      {
          u_char buffer[17];
 
          int i = ModeSense(buffer,Exa::non_page);
 
          if (i != Exa::ok)
            return(i);
 
          currReconnect = (int)buffer[16] & 0xff;
      }
    return(currReconnect);
}
 
/* -------------------------------------------------------------------- */
int Exa::Reconnect(int threshold)
{
    u_char buffer[17];
 
    int i = ModeSense(buffer,Exa::non_page);
 
    if (i != Exa::ok)
      return(i);
 
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] &= 0x7f;
    buffer[16] = threshold;
 
    currReconnect = threshold;
 
    return(ModeSelect(buffer,Exa::non_page));
}
 
