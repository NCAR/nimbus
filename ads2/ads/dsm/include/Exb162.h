/* MODULE:  Exb162.h
*  PURPOSE: Exabyte class definition 
*  DATE:    8/26/94

* Originally developed as a C function library by the National Optical 
* Astronomy Observatories(*)
Developed 1991, 1992, 1993 by the National Optical Astronomy Observatories(*)
 
(*) Operated by the Association of Universities for Research in
    Astronomy, Inc. (AURA) under cooperative agreement with the
    National Science Foundation.
*
*/
 
#ifndef EXB162_H
#define EXB162_H
 
#include "mv162.h" /* mvme162 includes */
#include "vxWorks.h"
#include "scsiLib.h"
#include "ioLib.h"
#include "errnoLib.h"
#include "logLib.h"
#include "stdioLib.h"
#include "taskLib.h"
#include "memLib.h"
#include "sysLib.h"
#include "string.h"

#include "exbDefs.h"
 
/* transfer block sizes for commands that return integers */
 
#define EXB_READ_BLOCK_LIMITS_SIZE      6
#define EXB_READ_POSITION_SIZE          20
 
/* strings to look for in the INQUIRY block */
 
#define EXB_VENDOR              "EXABYTE"
#define EXB_PRODUCT_8200        "EXB-8200"
#define EXB_PRODUCT_8500        "EXB-8500"
#define EXB_PRODUCT_8505        "EXB-8505"
 
/* description of Exabyte for scsiPhysDevCreate() */
 
#define EXB_LUN                 0
#define EXB_REMOVABLE           TRUE
#define EXB_BLOCKS              0x7fffff
#define EXB_BLOCK_SIZE          0x400
 
#define EXB_COMMAND_READ        0       /* directions for exbCommand() */
#define EXB_COMMAND_WRITE       1
 
/* bit fields for MODE_SELECT & MODE_SENSE */
 
#define MODE_UNBUFFERED         0x00
#define MODE_BUFFERED           0x10
#define MODE_ND                 0x20    /* no disconnect bit */
#define MODE_EBD                0x04    /* even byte disconnect bit */
#define MODE_PE                 0x02    /* parity enable bit */
#define MODE_MOTION_NORM        0x80    /* motion threshold */
#define MODE_MOTION_SLOW        0xc0
#define MODE_MOTION_FAST        0x40
#define MODE_RECONNECT_NORM     0x80    /* reconnect threshold */
#define MODE_RECONNECT_SLOW     0x40
#define MODE_RECONNECT_FAST     0xc0
#define MODE_GAP_MAX            0x07    /* gap threshold */
 
/* SCSI commands not in scsiLib.h */
 
#define SCSI_OPCODE_READ_POSITION       ((UINT8) 0x34)
 
/* comparisons for device INQUIRY */
 
#define DEVICE_IS_EXABYTE(a)    (bcmp ((a), EXB_VENDOR, \
                                    sizeof (EXB_VENDOR)-1) == 0)
#define DEVICE_IS_8200(a)       (bcmp ((a), EXB_PRODUCT_8200, \
                                    sizeof (EXB_PRODUCT_8200)-1) == 0)
#define DEVICE_IS_8500(a)       (bcmp ((a), EXB_PRODUCT_8500, \
                                    sizeof (EXB_PRODUCT_8500)-1) == 0)
#define DEVICE_IS_8505(a)       (bcmp ((a), EXB_PRODUCT_8505, \
                                    sizeof (EXB_PRODUCT_8505)-1) == 0)
 
/* conversion of 8-bit integers to 4 byte integers & visa versa */
  
#define C4toI4(b,a)     (a) = (((char *) (b))[0] << 24) | \
                              (((UINT8 *) (b))[1] << 16) | \
                              (((UINT8 *) (b))[2] <<  8) | \
                               ((UINT8 *) (b))[3];
#define C3toI4(b,a)     (a) = (((char *) (b))[0] << 16) | \
                              (((UINT8 *) (b))[1] <<  8) | \
                               ((UINT8 *) (b))[2];
#define C2toI4(b,a)     (a) = (((UINT8 *) (b))[0] <<  8) | \
                               ((UINT8 *) (b))[1];
#define I4toC3(a,b)     {*(((UINT8 *) (b))+0) = \
                            (UINT8) (((a) & 0x00ff0000) >> 16); \
                         *(((UINT8 *) (b))+1) = \
                            (UINT8) (((a) & 0x0000ff00) >>  8); \
                         *(((UINT8 *) (b))+2) = \
                            (UINT8)  ((a) & 0x000000ff); }
#define I4toC4(a,b)     {*(((UINT8 *) (b))+0) = \
                            (UINT8) (((a) & 0xff000000) >> 24); \
                         *(((UINT8 *) (b))+1) = \
                            (UINT8) (((a) & 0x00ff0000) >> 16); \
                         *(((UINT8 *) (b))+2) = \
                            (UINT8) (((a) & 0x0000ff00) >>  8); \
                         *(((UINT8 *) (b))+3) = \
                            (UINT8)  ((a) & 0x000000ff); }
 
/* bit fields for REQUEST_SENSE */
 
#define SENSE_ERROR_CODE(a)     ((a)[0] & 0x7f)
#define SENSE_VALID(a)          ((a)[0] & 0x80)
#define SENSE_KEY(a)            ((a)[2] & 0x0f)
#define SENSE_ILI(a)            ((a)[2] & 0x20)
#define SENSE_EOM(a)            ((a)[2] & 0x40)
#define SENSE_FMK(a)            ((a)[2] & 0x80)
#define SENSE_LBOT(a)           ((a)[19] & 0x01)
#define SENSE_PEOT(a)           ((a)[21] & 0x04)
 
IMPORT SCSI_CTRL        *pSysScsiCtrl;  /* usrConfig created SCSI controller */

class Exb162
{
  public:
    Exb162();
    STATUS busReset();
    STATUS open(int scsiBusId, int mode, int blocksize);
    STATUS close();
    STATUS testUnitReady();
    STATUS rewind();
    STATUS requestSense(char *block,int blocksize);
    STATUS readBlockLimits(int *maxBlockLength, int *minBlockLength);
    int read(char *block, int blocksize);
    int write(char *block, int blocksize);
    STATUS writeFilemarks(int num);
    STATUS space(int type, int num);
    STATUS inquiry(char *block, int blocksize);
    STATUS recoverBufferedData(char *block, int blocksize);
    STATUS verify(int blocksize);
    STATUS modeSelect(char *block, int blocksize, int page);
    STATUS reserveUnit(BOOL thirdParty, int thirdPartyId);
    STATUS releaseUnit(BOOL thirdParty, int thirdPartyId);
    STATUS erase();
    STATUS modeSense(char *block,int blocksize, int page);
    STATUS load(BOOL load);
    STATUS receiveDiagnosticResults();
    STATUS sendDiagnostics();
    STATUS lock(BOOL lock);
    STATUS locate(int location);
    STATUS readPosition(int *position);
    int writeBuffer(char *block, int blocksize);
    int readBuffer(char *block, int blocksize);
    STATUS logSelect();
    STATUS logSense();
    STATUS senseDump(char *sense);
    STATUS remainingTape(int *remain);
    STATUS isFilemark(BOOL *fmk);

  private:
    STATUS command(SCSI_COMMAND scsiCmdBlock,
		   int cmdblocksize,UINT8 *block, int blocksize);
    SCSI_PHYS_DEV *pExbPhysDev;			// SCSI physical device pointer
    UINT      exbOpt[SCSI_MAX_BUS_ID + 1];
    UINT      exbBlocksize[SCSI_MAX_BUS_ID + 1];
    int modepages [EXB_MODE_PAGES_SIZE];
};
#endif
