/*  This looks like C, but it's really C++!!
 *      $Id: Exa.h,v 1.1.1.1 1999/11/15 22:35:35 chris Exp $
 *
 *      Module:          Exa.hh
 *      Original Author: Richard E. Neitzel
 *      Copyright by the National Center for Atmospheric Research
 *      Date:            $Date: 1999/11/15 22:35:35 $
 *
 * revision history
 * ----------------
 * $Log: Exa.h,v $
 * Revision 1.1.1.1  1999/11/15 22:35:35  chris
 * Import source
 *
 *
 *
 * description:
 *
 */
#ifndef INCExa_hh
#define INCExa_hh
 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mtio.h>
#include <sys/scsi/impl/uscsi.h>
 
static const u_char EXA_DEFAULT_LUN = 0x4;
static const size_t EXA_MAX_BUFFER = 0xffff;
#ifndef ERROR
#define ERROR -1
#endif
 
class Exa {
  public:
    enum status
    {
        ok       = 0x0,
        check    = 0x2,
        busy     = 0x8,
        reserved = 18
    };
 
    enum page
    {
        non_page             = 0x0,
        read_write_page      = 0x1,
        connect_page         = 0x2,
        control_page         = 0xa,
        compression_page     = 0xf,
        configuration_page   = 0x10,
        partition_page       = 0x11,
        vendor_page1         = 0x20,
        vendor_page2         = 0x21,
        compress_status_page = 0x22,
        all_pages            = 0x3f,
        header_only	     = 0x40
    };
 
    enum density {
        exb8500c_nochk = 0x0,
        exb8200        = 0x14,
        exb8500        = 0x15,
        exb_no_change  = 0x7f,
        exb8500c       = 0x8c,
        exb8200c       = 0x90
    };
 
    Exa();
    void RwdOnDest() { rewOnDest = 1; }
    void NoRwdOnDest() { rewOnDest = 0; }
    void CloseOnDest() { closeOnDest = 1; }
    void NocloseOnDest() { closeOnDest = 0; }
    int Density(Exa::density density = Exa::exb8500);
    int Open (char *file, int mode, u_char l);		// open the drive
    void Close ();					// close the drive
    int GetDensity();
    int Motion(int threshold);
    int GetMotion();
    int Reconnect(int threshold);
    int GetReconnect();
    int FwdSpace(int count = 1);
    int BackSpace(int count = 1);
    int FwdSpaceFile(int count = 1);
    int BackSpaceFile(int count = 1);
    int Eot();
    int Locate(int block);
    int Rewind();
    int Read(void *buffer, size_t size);
    int ReadSense(void *buffer = NULL, size_t size = 29);
    int Write(void *buffer, size_t size);
    int Filemark(int count = 1, int size = 0);
    int ModeSense(void *buffer, int page);
    int ModeSelect(void *buffer, int page);
    int Buffered();
    int Unbuffered();
    int Erase();
    int Eject();
    int Lock();
    int Unlock();
    int Position();
    int Release();
    int Reserve();
    int Reset();
    int FreeSpace();
    ~Exa() { if (rewOnDest) Rewind(); if (closeOnDest) close(fd); }
 
  protected:
    int fd;                     // File descriptor.
    int closeOnDest;            // 1 = close on destruction.
    int rewOnDest;              // 1 = rewind on destruction.
    u_char lun;                 // Logical unit number.
 
    struct uscsi_cmd scsi;      // Actual command block.
    u_char cmd[10];             // Issued command.
    u_char sense[29];           // Buffer for local ReadSense().
 
    int SendCmd();              // Issue command & check for errors.
    int BigRead(void *buffer, size_t size);
    int BigWrite(void *buffer, size_t size);
 
  private:
    int currDensity;
    int currMotion;
    int currReconnect;
 
    void Clr() { memset(cmd,0,sizeof(cmd)); memset(&scsi,0,sizeof(scsi)); }
 
    enum ExaCmds
    {
        eject          = 0x1b,
        erase          = 0x19,
        locate         = 0x2b,
        lock           = 0x1e,
        mode_select    = 0x15,
        mode_sense     = 0x1a,
        position       = 0x34,
        read           = 0x08,
        release        = 0x17,
        request_sense  = 0x03,
        reserve        = 0x16,
        reset          = 0x0c,
        rewind         = 0x01,
        space          = 0x11,
        write          = 0x0a,
        write_filemark = 0x10,
    };
 
    enum ExaSense
    {
        no_sense = 0x0,
        ili      = 0x20,
    };
 
};

#endif // INCExa_hh
 
