/* Exb162.cc 1.18 1/21/94 NOAO */
 
/*

* Originally developed as a C function library by the National Optical 
* Astronomy Observatories(*)

Developed 1991, 1992, 1993 by the National Optical Astronomy Observatories(*)
 
(*) Operated by the Association of Universities for Research in
    Astronomy, Inc. (AURA) under cooperative agreement with the
    National Science Foundation.
*/

/*
modification history
--------------------

02g,06mar95,jvp	 Integrated pExbPhysDev as a private variable, rather than as
                 function arguements.  Debugged to work as a class. Eliminated
                 exb prefix from all function names. Named class Exb162.
02f,26aug94,rjh  Converted exbLib.c to Exb162.cc C++.
02e,14jan94,jmv  Removed reference to SCCS_ID in exbSenseDump(). 
02d,07jan94,jmv  Fixed bug in calculation of immed in exbWriteFilemarks.
02c,11dec93,jmv  Modified header to fit new style.
02b,10aug93,bdg  fixes to exbWriteFilemarks() to handle immed option.
02a,10jun93,bdg  release 2.  EXB-8505 & compression support.  exbCommand()
		  rewrite.  modified exbReadPosition() call.  page support.
01m,02jun93,bdg  fixed exbReadPosition().  added exbLoad().
01l,24nov92,bdg  fixed bug in exbSpace() preventing space by filemarks.
01k,16jul92,bdg  repaired fixed block mode bug.  upgraded exbRead() and
                  exbWrite().
01j,13apr92,bdg  full ANSI C compatibility.
01i,12nov91,bdg  added EXB_OPT_XFER_RATE_SLOW and FAST to the open modes.
01h,12oct91,bdg  added variable exbBlockSize.  remove mode select commands
                 from exbRead (), exbWrite (), and exbRecoverBuffer ().
                 added errnoSet () to exbCommand ().
01g,04oct91,bdg  added open options to exbOpen ().  created exbOptions[SG]et ().
                 changed all routines to handle both 8200 and 8500s.  added
                 numerous error checks.  added fixed block mode.
01f,13jul91,bdg  extensive mod to exbWrite () to handle errors.  added function
                 exbReadPosition ().
01e,09jul91,bdg  upgraded to EXB-8500 series.  modified exbModeSelect ().
01d,02jun91,bdg  finished exbReadBlockLimits (); fixed exbRequestSense ();
                 modified exbOpen () to return NULL on any error.
01c,02may91,bdg  fixed several ANSI forward references.
01b,27apr91,bdg  Release 1.1, general cleanup and comments.
01a,02mar91,bdg  alpha test version.
*/

/*
DESCRIPTION
 
These routines support the Exabyte 8200, 8500, and 8505 tape drives under
Wind River VxWorks 5.0.2b and associated SCSI driver.  The routines function
on a Motorola MV147SA-1, but should be portable to any other VME board
supported by the VxWorks 5.0 SCSI driver.  Acknowledgement is given to
P. R. Bade and The Johns Hopkins University/Applied Physics Laboratory
for much of the philosophy behind these routines.
 
The Wind River SCSI library is built around SCSI disk drives and do not
support tape drives well.  In particular, the routine scsiModeSelect()
fails for removable media.  These routines use only the scsiPhysDevCreate(),
scsiPhysDevDelete(), scsiPhysDevIdGet(), and scsiIoctl() calls.  All
Exabyte firmware commands are supported through the scsiIoctl() call.
 
Five known problems exist with the WRS drivers: the lack of an errnoSet() on
scsiIoctl() write commands that return MEDIUM ERRORs, the inability to
clear the SCSI bus without doing an entire bus reset, the lack of appropriate
error numbers for tape media (i.e., end-of-tape, filemark, illegal length),
the inability of the scsiLib to disconnect the initiator from the SCSI bus,
and the inability to read a variable block size.  The last two are by far the
most serious problems.
 
SEE ALSO
scsiLib (1)
 
AUTHOR
.CS
Bret D. Goodrich, Global Oscillation Network Group,
                  National Solar Observatory,
                  National Optical Astronomy Observatories
                  Tucson, Arizona, USA
                  goodrich@noao.edu
.CE
*/
#include <Exb162.h>

Exb162::Exb162()
{
  int modepages1[EXB_MODE_PAGES_SIZE] =

    {
       EXB_MODE_SENSE_VENDOR_SIZE,
       9, 12,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0, 16, 15,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,256,
    };

    memcpy (modepages, modepages1, sizeof(modepages));
}

/*******************************************************************************
* busReset - reset the SCSI bus.
*
* This routine is not really an Exabyte function, but it does remove the
* problem of resetting the bus after a SCSI catastrophe.  The Western
* Digital 33c93 SBIC controller does not release the bus when the RST line
* of the SCSI is asserted, in commands like sysScsiBusReset ().  We need to
* reinitialize the entire SBIC structure with sysScsiInit ().  The
* busReset () command just executes sysScsiInit ().
*
* RETURNS:
*   OK or ERROR if SCSI initialization failed.
*/
 
STATUS Exb162::busReset ()
 
{
//    IMPORT STATUS sysScsiInit ();	/* definition of the function */
	return(0);
//    return sysScsiInit ();
}


/*******************************************************************************
* open - open an Exabyte tape device
*
* This routine opens an Exabyte tape device.  If the device already exists
* then use its current physical device address.  Otherwise, create the
* physical device address and select the mode.  If an error occurs, delete
* the physical device address and return ERROR.
*
* RETURN
*   OK if successful, ERROR otherwise.
*/

STATUS Exb162::open (
    int scsiBusId,			/* SCSI bus target number */
    int mode,				/* device open mode */
    int blocksize)			/* size of a data block */
{
    char inquiry_blk[EXB_INQUIRY_SIZE];
    char modeSense[EXB_MODE_SENSE_SIZE];
    int ready;
    /* if a tape physical address exists, delete it and make a new one */
    if ((pExbPhysDev = scsiPhysDevIdGet (pSysScsiCtrl, scsiBusId, EXB_LUN)) !=
	NULL)
	scsiPhysDevDelete (pExbPhysDev);

    /*
     * LUN 0, timeout default (250 ms), removable media, large # of
     * blocks, 1024 bytes per block.
     */
    if ((pExbPhysDev = scsiPhysDevCreate (pSysScsiCtrl, scsiBusId,
	EXB_LUN, SCSI_DEF_SELECT_TIMEOUT, SCSI_DEV_SEQ_ACCESS,
	EXB_REMOVABLE, EXB_BLOCKS, EXB_BLOCK_SIZE)) == NULL)

	return ERROR;			/* errno is S_scsiLib_SELECT_TIMEOUT */

    /*
     * The useIdentify variable in the SCSI driver needs to be set to FALSE
     * or the device permanently hangs when doing a write state.  So we
     * don't send identify message and tape drive won't disconnect.  Also
     * we need to set the extended (request) sense flag and change the
     * sense length to the maximum allowable.
     */
    pExbPhysDev->useIdentify = FALSE;
    pExbPhysDev->extendedSense = TRUE;
    pExbPhysDev->reqSenseDataLength = EXB_REQUEST_SENSE_SIZE;

    /*
     * If the open mode requests it, test the unit for readiness.  This
     * option will trap out the Unit Attention errors when the device
     * first comes online.  We also rewind the unit if we tested its
     * readiness.  This allows us to send new MODE_SELECT parameters later.
     */
    if (mode & EXB_OPT_UNIT_READY_ON_OPEN || mode & EXB_OPT_REWIND_ON_OPEN)
    {
	ready = EXB_OPEN_WAIT;
	while (testUnitReady () != OK)
	{
	    if (--ready < 0)
	    {
		/* device never did become ready.  return an error. */
		(VOID) close ();
		return ERROR;
	    }
	    taskDelay (sysClkRateGet ());
	}

	if (mode & EXB_OPT_REWIND_ON_OPEN)
	    if (rewind () == ERROR)
	    {
		(VOID) close ();
		return ERROR;
	    }
    }

    /*
     * Do an inquiry to get information about the drive.  This information
     * will be used to determine the drive type for later calls.
     */
    if (inquiry (inquiry_blk, EXB_INQUIRY_SIZE) == ERROR)
    {
        (VOID) close ();
        return ERROR;
    }
    bcopy (&inquiry_blk[8], pExbPhysDev->devVendorID, VENDOR_ID_LENGTH);
    pExbPhysDev->devVendorID[VENDOR_ID_LENGTH] = NULL;
    bcopy (&inquiry_blk[16], pExbPhysDev->devProductID, PRODUCT_ID_LENGTH);
    pExbPhysDev->devProductID[PRODUCT_ID_LENGTH] = NULL;
    bcopy (&inquiry_blk[32], pExbPhysDev->devRevLevel, REV_LEVEL_LENGTH);
    pExbPhysDev->devRevLevel[REV_LEVEL_LENGTH] = NULL;
 
    /* This should be a supported Exabyte product */
    if (!DEVICE_IS_EXABYTE (pExbPhysDev->devVendorID))
    {
        errnoSet (S_scsiLib_DEV_UNSUPPORTED);
        (VOID) close ();
        return ERROR;
    }

    /*
     * Do a mode select to set our user configured information into
     * the device.
     */

    /* generic mode information */
    exbOpt[scsiBusId] = mode;
    exbBlocksize[scsiBusId] = blocksize;
    if (exbBlocksize[scsiBusId] <= 0)
    {
	exbOpt[scsiBusId] |= EXB_OPT_VARIABLE_BLOCK_LENGTH;
	exbBlocksize[scsiBusId] = 0;
    }
    else
	exbOpt[scsiBusId] &= ~EXB_OPT_VARIABLE_BLOCK_LENGTH;

    /*
     * The parameter list options are:
     *     buffered/unbuffered, block descriptor length
     */
    bzero ((char *) modeSense, EXB_MODE_SENSE_SIZE);
    modeSense[2] = ((exbOpt[scsiBusId] & EXB_OPT_UNBUFFERED) ?
	MODE_UNBUFFERED : MODE_BUFFERED);
    modeSense[3] = EXB_MODE_SENSE_BLOCK_SIZE;

    /*
     * The block descriptor list options are:
     *     density select, block length
     */
    modeSense[4] = EXB_DENSITY_NO_CHANGE;
    I4toC3 (exbBlocksize[scsiBusId], &modeSense[9]);

    /*
     * The vendor unique list options are:
     *     cartridge type (not used), no disconnect, even byte disconnect,
     *     parity enable, no auto load (not used), 112m cartridge,
     *     motion, reconnect, and gap thresholds.
     */
    modeSense[12] = ((exbOpt[scsiBusId] & EXB_OPT_NO_DISCONNECT) ? 0x20 : 0) |
	((exbOpt[scsiBusId] & EXB_OPT_EVEN_BYTE_DISCONNECT) ? 0x04 : 0) |
	((exbOpt[scsiBusId] & EXB_OPT_PARITY_CHECK) ? 0x02 : 0);
    modeSense[13] = ((exbOpt[scsiBusId] & EXB_OPT_CARTRIDGE_112M) ? 0x01 : 0);

    if (exbOpt[scsiBusId] & EXB_OPT_XFER_RATE_SLOW)
    {
	modeSense[14] = MODE_MOTION_SLOW;
	modeSense[15] = MODE_RECONNECT_SLOW;
    }
    else if (exbOpt[scsiBusId] & EXB_OPT_XFER_RATE_FAST)
    {
	modeSense[14] = MODE_MOTION_FAST;
	modeSense[15] = MODE_RECONNECT_FAST;
    }
    else
    {
	modeSense[14] = MODE_MOTION_NORM;
	modeSense[15] = MODE_RECONNECT_NORM;
    }
    modeSense[16] = MODE_GAP_MAX;

    /*
     * EXB-8200 specific mode
     */
    if (DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	exbOpt[scsiBusId] |= EXB_OPT_8200;
	exbOpt[scsiBusId] |= EXB_OPT_NO_COMPRESS;
	modeSense[4] = EXB_DENSITY_DEFAULT;
    }

    /*
     * EXB-8500 specific mode
     */
    else if (DEVICE_IS_8500 (pExbPhysDev->devProductID))
    {
	exbOpt[scsiBusId] |= EXB_OPT_NO_COMPRESS;
	modeSense[4] = ((exbOpt[scsiBusId] & EXB_OPT_8200) ?
	    EXB_DENSITY_8200 : EXB_DENSITY_8500);
    }

    /*
     * EXB-8505 specific mode
     */
    else if (DEVICE_IS_8505 (pExbPhysDev->devProductID))
    {
	if (exbOpt[scsiBusId] & EXB_OPT_NO_COMPRESS)
	    modeSense[4] = ((exbOpt[scsiBusId] & EXB_OPT_8200) ?
		EXB_DENSITY_8200 : EXB_DENSITY_8500);
	else
	    modeSense[4] = ((exbOpt[scsiBusId] & EXB_OPT_8200) ?
		EXB_DENSITY_8200C : EXB_DENSITY_8500C);
    }

    /*
     * Invalid Exabyte device
     */
    else
    {
	errnoSet (S_scsiLib_DEV_UNSUPPORTED);
	(VOID) close ();
	return ERROR;
    }

    /* Write the mode */
    if (modeSelect (modeSense, EXB_MODE_SENSE_SIZE, 0) == ERROR)
    {
	(VOID) close ();
	return ERROR;
    }

    return OK;
}


/*******************************************************************************
* close - close an Exabyte tape device
*
* This routine closes the Exabyte tape device by deleting the physical
* device pointer.
*
* RETURN
*   STATUS value of scsiPhysDevDelete ().
*/

STATUS Exb162::close()
{
    return scsiPhysDevDelete (pExbPhysDev);
}


/*******************************************************************************
* testUnitReady - test for the unit ready
*
* This routine tests the Exabyte for readiness to accept a SCSI command.  If
* the drive is reset by a Bus Reset or a tape is loaded, the drive returns
* a CHECK_CONDITION with sense UNIT_ATTENTION.  Any subsequent SCSI call
* (except REQUEST_SENSE or INQUIRY) will return a GOOD status.
*
* RETURN
*   OK or ERROR if the drive is not ready.
*/

STATUS Exb162::testUnitReady()
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_TEST_UNIT_READY;

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* rewind - rewind the tape
*
* The REWIND command causes the drive to rewind the tape to the logical
* beginning of tape (LBOT).
*
* RETURN
*    OK or ERROR if the device could not execute the command.
*/

STATUS Exb162::rewind()
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_REWIND;
    scsiCmdBlock[1] = ((exbOpt[pExbPhysDev->scsiDevBusId] & EXB_OPT_IMMEDIATE) ?
	1 : 0);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* requestSense - request the sense of the tape device.
*
* The REQUEST_SENSE command requests that the device transfer sense data to
* the initiator.  The EXB-8200 returns at total of 14 bytes of sense data to
* the initiator; the EXB-8500 and EXB-8505 return 29 bytes.
*
* The sense data is valid for the CHECK_CONDITION status just presented to
* the initiator.  This sense data is preserved in the device for the initiator
* receiving the CHECK_CONDITION status.  Sense data is cleared when any
* subsequent command that is not a REQUEST_SENSE on an INQUIRY command is
* received from the initiator receiving the CHECK_CONDITION status.
*
* A blocksize greater than the REQUEST_SENSE default buffer size will have the
* effect of setting the CLRCNT bit.  Setting this bit enables the initiator to
* reset the Read/Write Data Error counter, the Tracking Retry counter, the
* Read/Write Retry counter, and the Underrun/Overrun counter.
*
* RETURN
*   OK or ERROR if the device is unable to process the command.
*/

STATUS Exb162::requestSense (
    char *block,			/* data block */
    int blocksize)			/* size of data block */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
    BOOL clrcnt;

    /* Determine the clear counter flag */
    if (blocksize > EXB_REQUEST_SENSE_SIZE ||
	(DEVICE_IS_8200 (pExbPhysDev->devProductID) &&
	blocksize > EXB_REQUEST_SENSE_SIZE_8200))

	clrcnt = TRUE;

    else if (blocksize > EXB_REQUEST_SENSE_SIZE)
	clrcnt = TRUE;

    else
	clrcnt = FALSE;

    if (blocksize > 0xff)
	blocksize = 0xff;

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_REQUEST_SENSE;
    scsiCmdBlock[4] = (UINT8) blocksize;
    scsiCmdBlock[5] = (clrcnt ? 0x80 : 0x00);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) block, blocksize);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* readBlockLimits - read the block length limits of the tape device.
*
* The READ_BLOCK_LIMITS command requests that the device return data
* identifying the maximum and minumum logical block lengths supported.  The
* data returned by the READ_BLOCK_LIMITS command applies to both the variable
* and fixed block lengths for the READ and WRITE commands.
*
* The value returned for the maxBlockLength field is 0x03c000 (240 KBytes).
* If the ND (No Disconnect During Data Transfer) bit is set to 1, the value
* returned is 0x28000 (160 KBytes).  The value retuned for the minBlockLength
* field is 0x00001 (1 byte).
*
* RETURN
*   OK or ERROR if the device is unable to process the command.
*/

STATUS Exb162::readBlockLimits (
    int *maxBlockLength,		/* pointer to max block length */
    int *minBlockLength)		/* pointer to min block length */
{
    SCSI_COMMAND scsiCmdBlock;
    UINT8 block[EXB_READ_BLOCK_LIMITS_SIZE];
    STATUS status;
 
    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_READ_BLOCK_LIMITS;

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	block, EXB_READ_BLOCK_LIMITS_SIZE);
    if (status == SCSI_STATUS_GOOD)
    {
	C3toI4 (&block[1], *maxBlockLength);
	C2toI4 (&block[4], *minBlockLength);
    }

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* read - read data from the tape
*
* This routine reads data from the tape.  The data buffer address and size
* are passed as arguments.  The number of bytes actually read are returned.
* If there are no errors the number of read bytes equals the number of
* requested bytes.
*
* RETURN:
*   The number of bytes read or ERROR if read failed.
*/

int Exb162::read (
    char *block,			/* data block */
    int blocksize)			/* size of data block */
{
    SCSI_COMMAND scsiCmdBlock;
    int xferLength;
    int nbytes;
    STATUS status;

    /* check for a valid block size multiple */
    xferLength = blocksize;
    if (exbBlocksize[pExbPhysDev->scsiDevBusId])
    {
	if (blocksize % exbBlocksize[pExbPhysDev->scsiDevBusId])
	{
	    errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	    return ERROR;
	}
	else
	    xferLength = blocksize / exbBlocksize[pExbPhysDev->scsiDevBusId];
    }

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_READ;
    scsiCmdBlock[1] = (((exbOpt[pExbPhysDev->scsiDevBusId] &
	EXB_OPT_SUPPRESS_ILI) ? 2 : 0) |
	((exbOpt[pExbPhysDev->scsiDevBusId] &
	EXB_OPT_VARIABLE_BLOCK_LENGTH) ? 0 : 1));
    I4toC3 (xferLength, &scsiCmdBlock[2]);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) block, blocksize);
    switch (status)
    {
	case (STATUS) SCSI_STATUS_GOOD:
	    nbytes = blocksize;
	    break;

	/*
	 * If a CHECK_CONDITION occurs, we need to find the number of bytes
	 * actually read.  In addition, if the transfer length was incorrect
	 * we need to change the errno to report this.  Otherwise the errno
	 * should be set from command().
	 */
	case (STATUS) SCSI_STATUS_CHECK_CONDITION:
	    nbytes = ERROR;

	    /* check for valid data not read, return bytes or blocks */
	    if (SENSE_VALID (pExbPhysDev->pReqSenseData))
	    {
		C4toI4 (&pExbPhysDev->pReqSenseData[3], xferLength);

		/* convert from blocks to bytes */
		if (exbBlocksize[pExbPhysDev->scsiDevBusId])
		    nbytes = blocksize -
			xferLength * exbBlocksize[pExbPhysDev->scsiDevBusId];
		else
		    nbytes = blocksize - xferLength;
	    }

	    /* check for transfer length incorrect */
	    if (SENSE_KEY (pExbPhysDev->pReqSenseData) ==
		SCSI_SENSE_KEY_NO_SENSE &&
		SENSE_ILI (pExbPhysDev->pReqSenseData))

		errnoSet (S_exbLib_ILLEGAL_LENGTH);

	    /* check for filemark detected */
	    else if (SENSE_KEY (pExbPhysDev->pReqSenseData) ==
		SCSI_SENSE_KEY_NO_SENSE &&
		SENSE_FMK (pExbPhysDev->pReqSenseData))

		errnoSet (S_exbLib_FILEMARK);

	    /* check for physical end of tape */
	    else if (SENSE_KEY (pExbPhysDev->pReqSenseData) ==
		SCSI_SENSE_KEY_MEDIUM_ERROR &&
		SENSE_EOM (pExbPhysDev->pReqSenseData))
	    {
		if (DEVICE_IS_8200 (pExbPhysDev->devProductID) ||
		    SENSE_PEOT (pExbPhysDev->pReqSenseData))

		    errnoSet (S_exbLib_PHYSICAL_END_OF_TAPE);
	    }
	    break;

	default:
	    nbytes = ERROR;
	    break;
    }

    return nbytes;
}


/*******************************************************************************
* write - write a buffer to the tape
*
* This command writes a buffer to the tape.  If the write is successful,
* return with the number of bytes written.  If any error condition results
* which sets the CHECK_CONDITION status, the number of bytes actually written
* is returned.  If, under this error condition, all bytes are successfully
* written (as with variable block size data or LEOT reached on the last
* block of fixed block size data) an ERROR is returned.  The errno is
* set to the appropriate SCSI device error.  If LEOT is reached and not all
* bytes are transferred, the bytes transferred will be returned.
*
* RETURN
*   The number of bytes written, or ERROR if a device error has occurred.
*/

int Exb162::write (
    char *block,			/* data block */
    int blocksize)			/* size of data block */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
    int nbytes;
    int xferLength;

    /* check for a valid block size multiple */
    xferLength = blocksize;
    if (exbBlocksize[pExbPhysDev->scsiDevBusId])
    {
	if (blocksize % exbBlocksize[pExbPhysDev->scsiDevBusId])
	{
	    errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	    return ERROR;
	}
	else
	    xferLength /= exbBlocksize[pExbPhysDev->scsiDevBusId];
    }

    /*
     * Create the CDB for the command.  The fixed bit should be determined.
     * The transfer length is either in blocks or bytes.
     */
    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_WRITE;
    scsiCmdBlock[1] = ((exbOpt[pExbPhysDev->scsiDevBusId] &
	EXB_OPT_VARIABLE_BLOCK_LENGTH) ? 0 : 1);
    I4toC3 (xferLength, &scsiCmdBlock[2]);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) block, blocksize);
    switch (status)
    {
	case (STATUS) SCSI_STATUS_GOOD:
	    nbytes = blocksize;
	    break;

	/*
	 * If a CHECK CONDITION occurred, determine the bytes actually
	 * transferred, or if the error was EOM, nbytes == -1 and the
	 * error "end_of_medium" is returned.  if the latter occurs, all
	 * requested bytes were written, even though -1 was returned.
	 */
	case (STATUS) SCSI_STATUS_CHECK_CONDITION:
	    nbytes = ERROR;

	    /* check for valid data not written, return bytes or blocks */
	    if (SENSE_VALID (pExbPhysDev->pReqSenseData))
	    {
		C4toI4 (&pExbPhysDev->pReqSenseData[3], xferLength);

		/* convert from blocks to bytes */
		if (exbBlocksize[pExbPhysDev->scsiDevBusId])
		    nbytes = blocksize -
			xferLength * exbBlocksize[pExbPhysDev->scsiDevBusId];
		else
		    nbytes = blocksize - xferLength;
	    }

	    /* Check for logical end-of-tape */
	    if (SENSE_KEY (pExbPhysDev->pReqSenseData) ==
		SCSI_SENSE_KEY_NO_SENSE &&
		SENSE_EOM (pExbPhysDev->pReqSenseData))

		if (DEVICE_IS_8200 (pExbPhysDev->devProductID) ||
		    !SENSE_LBOT (pExbPhysDev->pReqSenseData))

		    errnoSet (S_exbLib_END_OF_MEDIUM);
	    break;

	default:
	    nbytes = ERROR;
	    break;
    }

    return nbytes;
}


/*******************************************************************************
* writeFilemarks - write filemarks to the tape
*
* This routine writes filemarks to the tape.  The number of EOFs to write
* passed as the second argument.  If the number is negative setmarks are
* written rather than filemarks.  This option is available only in EXB-8505
* mode.
*
* RETURN
*   OK or ERROR if unable to process command.
*/

STATUS Exb162::writeFilemarks (
    int num)				/* number of filemarks */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
    BOOL setmark;			/* filemarks or setmarks flag */
    BOOL immed;				/* immediate return (8500 only) */
 
    /*
     * Set/clear the immediate return flag.  The 8200 does not support
     * immediate mode, so it's always cleared.
     *
     * If the number argument is negative, we want to write setmarks
     * instead of filemarks.  set the setmark flag and negate the number
     */
    if (DEVICE_IS_8200 (pExbPhysDev->devProductID))
	setmark = immed = FALSE;

    else
    {
        immed = (exbOpt[pExbPhysDev->scsiDevBusId] & EXB_OPT_IMMEDIATE) &&
            !(exbOpt[pExbPhysDev->scsiDevBusId] & EXB_OPT_UNBUFFERED);
	setmark = (num < 0);
	num = ((num < 0) ? -num : num);
    }

    /*
     * Create the CDB for the command.  Set/clear the immediate bit.
     * Set the number of filemarks to write.
     */
    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_WRITE_FILEMARKS;
    scsiCmdBlock[1] = (immed ? 1 : 0) | (setmark ? 2 : 0);
    I4toC3 (num, &scsiCmdBlock[2]);
    scsiCmdBlock[5] = ((exbOpt[pExbPhysDev->scsiDevBusId] &
	EXB_OPT_SHORT_FILEMARK) ? 0x80 : 0);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* space - move the tape forward or backward
*
* This routine moves the tape forward or backward a requested number of
* filemarks or logical blocks.  A negative number of filemarks is a
* backwards tape motion.  If the endOfData flag is positive, skip to the
* end of valid data written on the tape.  Early EXB-8200 devices do not
* support skip to end-of-data.
*
* RETURN
*   OK or ERROR if unable to process command.
*/

STATUS Exb162::space (
    int type,				/* space type (0=record,1=EOF,3=EOD) */
    int num)				/* number of blocks or files */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
 
    if (type != EXB_SPACE_RECORD && type != EXB_SPACE_FILEMARK &&
	type != EXB_SPACE_END_OF_DATA)
    {
	errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	return ERROR;
    }

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_SPACE;
    scsiCmdBlock[1] = type & 0x07;
    I4toC3 (num, &scsiCmdBlock[2]);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);
    if (status == SCSI_STATUS_CHECK_CONDITION)

	/* trap out blank checks */
	if (SENSE_KEY (pExbPhysDev->pReqSenseData) ==
	    SCSI_SENSE_KEY_BLANK_CHECK && type == EXB_SPACE_END_OF_DATA)
	{
	    status = SCSI_STATUS_GOOD;
	}
	else if (SENSE_FMK (pExbPhysDev->pReqSenseData))	/* FMK */
	    errnoSet (S_exbLib_FILEMARK);
	else if (SENSE_EOM (pExbPhysDev->pReqSenseData))	/* EOM */
	    errnoSet (S_exbLib_END_OF_MEDIUM);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* inquiry - inquire about the tape device parameters
*
* This routine uses the vxWorks version, scsiInquiry (), but stores the
* results in the physical device address structure *pExbPhysDev.  If
* the variable "outputBlock" is not NULL, a copy of the inquiry block
* will be placed in it.
*
* If the blocksize is 6, return the Supported Vital Product Data Page.  If the
* blocksize is 14, return the Unit Serial Number Page.
*
* RETURN
*   OK or ERROR if output block not large enough or unable to process command.
*/

STATUS Exb162::inquiry (
    char *block,			/* data block */
    int blocksize)			/* size of data block */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
 
    if (blocksize > 0xff)
	blocksize = 0xff;

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_INQUIRY;
    scsiCmdBlock[1] = ((blocksize == 6 || blocksize == 14) ? 0x01 : 0x00);
    scsiCmdBlock[2] = ((blocksize == 14) ? 0x80 : 0);
    scsiCmdBlock[4] = (UINT8) blocksize;

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) block, blocksize);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}
 
 
/*******************************************************************************
* recoverBufferedData - recover data from tape buffer.
*
* This routine reads data that has been transferred to the tape buffer but
* has not been written on the tape cartridge.  This command is normally used
* to recover from error or exception conditions that make it impossible to
* write the buffered data to the tape.
*
* This routine is supported on on EXB-8200 devices.  An ILLEGAL REQUEST error
* results on EXB-8500 devices.
*
* RETURNS:
*   Number of bytes recovered or ERROR if unable to process command.
*/
 
int Exb162::recoverBufferedData (
    char *block,			/* data block */
    int blocksize)			/* size of data block */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
    int xferLength;
    int nbytes;				/* bytes successfully read */
 
    if (!DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }
 
    /* check for a valid block size multiple */
    xferLength = blocksize;
    if (exbBlocksize[pExbPhysDev->scsiDevBusId])
    {
	if (blocksize % exbBlocksize[pExbPhysDev->scsiDevBusId])
	{
	    errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	    return ERROR;
	}
        else
	    xferLength = blocksize / exbBlocksize[pExbPhysDev->scsiDevBusId];
    }

    /*
     * Read the requested number of bytes.  If this fails, determine the
     * number of bytes actually read.
     */
    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_RECOVER_BUF_DATA;
    scsiCmdBlock[1] = (exbBlocksize[pExbPhysDev->scsiDevBusId] ? 0x01: 0x00);
    I4toC3 (xferLength, &scsiCmdBlock[2]);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) block, blocksize);
    switch (status)
    {
	/* All requested bytes were read */
	case (STATUS) SCSI_STATUS_GOOD:
	    nbytes = blocksize;
	    break;
 
	/*
	 * Check Condition error.  Look for unread bytes in Information Bytes.
	 * Possible error conditions are: "Modes do not match", sense key =
	 * illegal_request & check_condition status; "Attempt to recover more
	 * data than contained in the CTS buffer", valid & EOM bits set, sense
	 * key = 0, & check_condition status.
	 */
	case (STATUS) SCSI_STATUS_CHECK_CONDITION:
	    nbytes = ERROR;

	    /* check for valid data not read, return bytes or blocks */
	    if (SENSE_VALID (pExbPhysDev->pReqSenseData))
	    {
		C4toI4 (&pExbPhysDev->pReqSenseData[3], xferLength);
 
		/* convert from blocks to bytes */
		if (exbBlocksize[pExbPhysDev->scsiDevBusId])
		    nbytes = blocksize -
			xferLength * exbBlocksize[pExbPhysDev->scsiDevBusId];
		else
		    nbytes = blocksize - xferLength;
	    }
 
	    /*
	     * If the error is "hard" return ERROR.  Test all of the
	     * second sense byte, except the Filemark and EOM bits.  This
	     * includes the illegal length bits and the 4 bit
	     * sense key.
	     */
	    if (SENSE_KEY (pExbPhysDev->pReqSenseData) ==
		SCSI_SENSE_KEY_NO_SENSE)
	    {
		if (SENSE_FMK (pExbPhysDev->pReqSenseData))      /* FMK */
		    errnoSet (S_exbLib_FILEMARK);
 
		else if (SENSE_ILI (pExbPhysDev->pReqSenseData)) /* ILI */
		    errnoSet (S_exbLib_ILLEGAL_LENGTH);
 
		nbytes = ERROR;
	    }
	    break;
 
	/*
	 * Other SCSI status errors return ERROR
	 */
	default:
	    nbytes = ERROR;
	    break;
    }

    return nbytes;
}


/*******************************************************************************
* verify - verify tape blocks
*
* THIS COMMAND IS NOT FULLY TESTED.
*/

STATUS Exb162::verify (
    int blocksize)			/* size of data block */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
 
    if (DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }

    /* check for a valid block size multiple */
    if (exbBlocksize[pExbPhysDev->scsiDevBusId])
    {
	if (blocksize % exbBlocksize[pExbPhysDev->scsiDevBusId])
	{
	    errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	    return ERROR;
	}
	else
	    blocksize /= exbBlocksize[pExbPhysDev->scsiDevBusId];
    }

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_VERIFY_SEQ;
    scsiCmdBlock[1] = ((exbOpt[pExbPhysDev->scsiDevBusId] &
	EXB_OPT_VARIABLE_BLOCK_LENGTH) ? 0 : 1) |
	((exbOpt[pExbPhysDev->scsiDevBusId] & EXB_OPT_IMMEDIATE) ? 4 : 0);
    I4toC3 (blocksize, &scsiCmdBlock[2]);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* modeSelect - select the operation modes
*
* This routine selects the Exabyte operational modes.  The input block is
* transferred to the Exabyte into the selected page.
*
* RETURN
*   OK or ERROR if the command failed.
*/

STATUS Exb162::modeSelect (
    char *block,			/* block to write */
    int blocksize,			/* size of block */
    int page)				/* page number to write */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
 
    if (page != 0 && DEVICE_IS_8200 (pExbPhysDev->devProductID))
	page = 0;

    /*
     * Determine the largest allowable blocksize.  For EXB-8200s or non-page
     * format requests, valid sizes are 0, 4, 5-9, 12, 13-17 bytes.  We will
     * curtail this a bit and force one of three possibilities: parameter,
     * block and vendor lists, parameter and block lists, or parameter list.
     */
    if (blocksize < EXB_MODE_SENSE_PARAM_SIZE)
	blocksize = page = 0;

    else if (blocksize < EXB_MODE_SENSE_PARAM_SIZE + EXB_MODE_SENSE_BLOCK_SIZE)
    {
	blocksize = EXB_MODE_SENSE_PARAM_SIZE;
	page = 0;
    }

    else if (blocksize <
	EXB_MODE_SENSE_PARAM_SIZE + EXB_MODE_SENSE_BLOCK_SIZE + modepages[page])
    {
	block[3] = EXB_MODE_SENSE_BLOCK_SIZE;
	blocksize = EXB_MODE_SENSE_PARAM_SIZE + EXB_MODE_SENSE_BLOCK_SIZE;
	page = 0;
    }

    else
	blocksize = EXB_MODE_SENSE_PARAM_SIZE + EXB_MODE_SENSE_BLOCK_SIZE +
	    modepages[page];

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_MODE_SELECT;
    scsiCmdBlock[1] = (page ? 0x10 : 0x00);
    scsiCmdBlock[4] = (UINT8) blocksize;

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) block, blocksize);

    return (status == (STATUS) SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* reserveUnit - reserve the tape unit for exclusive use
*
* This command reserves the tape unit for an initiator's exclusive use, or, if
* third party reservations are in effect, for another SCSI device's use.  The
* reservation remains in effect until an exbReleaseUnit () command is received
* or a RESET.
*
* Third party reservations are not supported by EXB-8200 drives and return
* an ILLEGAL REQUEST error.
*
* RETURN
*   OK or ERROR if unable to reserve the device.
*/

STATUS Exb162::reserveUnit (
    BOOL thirdParty,			/* enable/disable third party */
    int thirdPartyId)			/* 3rd party SCSI id number */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    if (thirdParty && DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	return ERROR;
    }

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_RESERVE;
    scsiCmdBlock[1] = (thirdParty ? (0x10 | (thirdPartyId & 0x0e)) : 0);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* releaseUnit - release the tape unit
*
* This command releases the tape unit from an initiator's exclusive use, or, if
* third party reservations are in effect, from another SCSI device's use.
*
* Third party reservations are not supported by EXB-8200 drives and return
* an ILLEGAL REQUEST error.
*
* RETURN
*   OK or ERROR if unable to release the device.
*/

STATUS Exb162::releaseUnit (
    BOOL thirdParty,			/* enable/disable 3rd part use */
    int thirdPartyId)			/* third pary SCSI id number */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    if (thirdParty && DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	return ERROR;
    }

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_RELEASE;
    scsiCmdBlock[1] = (thirdParty ? (0x10 | (thirdPartyId & 0x0e)) : 0);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* erase - erase the tape media
*
* This routine erases the tape media from the current position to the
* end-of-tape.
*
* RETURN
*   OK or ERROR if unable to process command.
*/

STATUS Exb162::erase()
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_ERASE;
    scsiCmdBlock[1] = 0x01 | ((exbOpt[pExbPhysDev->scsiDevBusId] &
	EXB_OPT_IMMEDIATE) ? 0x02 : 0x00);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* modeSense - read the current mode of the tape unit
*
* This command is complementary to the MODE_SELECT command.  It returns the
* current mode of the tape unit.  
*
* RETURN
*   OK or ERROR if unable to process command.
*/

STATUS Exb162::modeSense (
    char *block,			/* data block */
    int blocksize,			/* size of data block */
    int page)				/* page number to return (0x3f = all) */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    if (page != 0 && DEVICE_IS_8200 (pExbPhysDev->devProductID))
	page = 0;

    if (blocksize < EXB_MODE_SENSE_PARAM_SIZE)
	blocksize = page = 0;

    else if (blocksize < EXB_MODE_SENSE_PARAM_SIZE + EXB_MODE_SENSE_BLOCK_SIZE)
    {
	blocksize = EXB_MODE_SENSE_PARAM_SIZE;
	page = 0;
    }
    else if (blocksize < 
	EXB_MODE_SENSE_PARAM_SIZE + EXB_MODE_SENSE_BLOCK_SIZE + modepages[page])
    {
	blocksize = EXB_MODE_SENSE_PARAM_SIZE + EXB_MODE_SENSE_BLOCK_SIZE;
	page = 0;
    }
    else if (blocksize > 0xff)
	blocksize = 0xff;

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_MODE_SENSE;
    scsiCmdBlock[2] = page & 0x3f;
    scsiCmdBlock[4] = (UINT8) blocksize;

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) block, blocksize);

    return (status == (STATUS) SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* load - load/unload the tape medium
*
* This routine loads or unloads the tape medium depending upon the state
* of the "load" variable.
*
* RETURN
*   OK or ERROR if unable to load or unload tape.
*/

STATUS Exb162::load (
    BOOL load)				/* TRUE=load, FALSE=unload */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;
    printf("Exb-load: status = %x\n",status);
    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    printf("Exb-load: 1\n");
    scsiCmdBlock[0] = SCSI_OPCODE_LOAD_UNLOAD;
    printf("Exb-load: 2\n");
    taskDelay(500);
    scsiCmdBlock[1] = ((exbOpt[pExbPhysDev->scsiDevBusId] &
	EXB_OPT_IMMEDIATE) ? 0x01 : 0x00);
    scsiCmdBlock[4] = (load ? 0x01 : 0x00);

    printf("Exb-load: 3\n");
    taskDelay(500);
    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    printf("Exb-load: 4\n");
    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* receiveDiagnosticResults - get results of internal diagnostics test
*
* THIS COMMAND DOES NOT FUNCTION WITHOUT DISCONNECT CAPABILITIES.
*/

STATUS Exb162::receiveDiagnosticResults ()

{
    errnoSet (S_scsiLib_ILLEGAL_REQUEST);
    return ERROR;
}


/*******************************************************************************
* sendDiagnostics - start internal diagnostics test
*
* THIS COMMAND DOES NOT FUNCTION WITHOUT DISCONNECT CAPABILITIES.
*/

STATUS Exb162::sendDiagnostics ()

{
    errnoSet (S_scsiLib_ILLEGAL_REQUEST);
    return ERROR;
}


/*******************************************************************************
* lock - prevent or allow medium removal
*
* This routine prevents the ejection of the tape cartridge.
*
* RETURN
*   OK or ERROR if unable to process command.
*/

STATUS Exb162::lock (
    BOOL lock)				/* TRUE = lockout eject button */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_CTRL_MEDIUM_REMOVAL;
    scsiCmdBlock[4] = (lock ? 0x01 : 0x00);

    status = command (scsiCmdBlock, SCSI_GROUP_0_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* locate - position the tape at the specified block address
*
* This routine positions the tape to the requested 1024 Kbyte block.
*
* This command is not available on EXB-8200 devices.  An Illegal Request
* error is returned.
*
* RETURN
*   OK, or ERROR if buffer is not empty and could not be written before
*   executing the LOCATE command.
*/

STATUS Exb162::locate (
    int location)			/* block to position tape to */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    if (DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_1_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_SEEK_EXT;
    scsiCmdBlock[1] = ((exbOpt[pExbPhysDev->scsiDevBusId] &
	EXB_OPT_IMMEDIATE) ? 0x01 : 0x00);
    I4toC4 (location, &scsiCmdBlock[3]);

    status = command (scsiCmdBlock, SCSI_GROUP_1_CMD_LENGTH,
	(UINT8 *) NULL, 0);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* readPosition - read the current position of the tape unit
*
* The READ POSITION command reports the current position of the tape unit.
* The number of 1024-byte blocks currently written is returned in the
* *position argument.
*
* This command is not available on EXB-8200 devices.  An Illegal Request
* error is returned.
*
* RETURN
*   OK or ERROR if unable to process command.
*/

STATUS Exb162::readPosition (
    int *position)			/* pointer to current block number */
{
    SCSI_COMMAND scsiCmdBlock;
    UINT8 block[EXB_READ_POSITION_SIZE];
    STATUS status;

    if (DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_1_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_READ_POSITION;

    status = command (scsiCmdBlock, SCSI_GROUP_1_CMD_LENGTH,
	block, EXB_READ_POSITION_SIZE);

    /*
     * Read the position from the returned block.  If the block position
     * is unknown the BPU bit will be set.  Do not fill the position integer
     * and return with an error.
     */
    if (status == (STATUS) SCSI_STATUS_GOOD)
    {
	/* is block position unknown? */
	if (block[0] & 0x04)
	{
	    errnoSet (S_exbLib_POSITION_UNKNOWN);
	    status = ERROR;
	}
	else
	    C4toI4 (&block[4], *position);
    }

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* writeBuffer - write new EEPROM microcode
*
* THIS COMMAND DOES NOT FUNCTION.
*/

int Exb162::writeBuffer (
    char *block,			/* EEPROM block to write */
    int blocksize)			/* size of block */
{
    if (DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }

    errnoSet (S_scsiLib_ILLEGAL_REQUEST);
    return ERROR;
}


/*******************************************************************************
* readBuffer - read the tape drive microcode
*
* This routine reads the microcode image of the tape drive.  For EXB-8500
* drives the block size must be 175656 bytes, for the EXB-8505 it must be
* 347688 bytes.  This command does not function on the EXB-8200.
*
* RETURN
*   OK or ERROR if the command failed.
*/

int Exb162::readBuffer (
    char *block,			/* block to read EEPROM into */
    int blocksize)			/* size of block */
{
    SCSI_COMMAND scsiCmdBlock;
    STATUS status;

    if (DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }

    else if (DEVICE_IS_8500 (pExbPhysDev->devProductID))
	if (blocksize < EXB_BUFFER_SIZE_8500)
	{
	    errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	    return ERROR;
	}
	else
	   blocksize = EXB_BUFFER_SIZE_8500;

    else if (DEVICE_IS_8505 (pExbPhysDev->devProductID))
	if (blocksize < EXB_BUFFER_SIZE_8505)
	{
	    errnoSet (S_scsiLib_ILLEGAL_PARAMETER);
	    return ERROR;
	}
	else
	   blocksize = EXB_BUFFER_SIZE_8505;

    bzero ((char *) scsiCmdBlock, SCSI_GROUP_1_CMD_LENGTH);
    scsiCmdBlock[0] = SCSI_OPCODE_READ_BUFFER;
    scsiCmdBlock[1] = 0x01;
    I4toC3 (blocksize, &scsiCmdBlock[6]);

    status = command (scsiCmdBlock, SCSI_GROUP_1_CMD_LENGTH,
	(UINT8 *) block, blocksize);

    return (status == SCSI_STATUS_GOOD) ? OK : ERROR;
}


/*******************************************************************************
* logSelect - select log entry
*
* THIS COMMAND DOES NOT FUNCTION.
*/

STATUS Exb162::logSelect ()

{
    if (DEVICE_IS_8200 (pExbPhysDev->devProductID) ||
	DEVICE_IS_8500 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }

    errnoSet (S_scsiLib_ILLEGAL_REQUEST);
    return ERROR;
}


/*******************************************************************************
* logSense - get log entry
*
* THIS COMMAND DOES NOT FUNCTION.
*/

STATUS Exb162::logSense ()

{
    if (DEVICE_IS_8200 (pExbPhysDev->devProductID) ||
	DEVICE_IS_8500 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }

    errnoSet (S_scsiLib_ILLEGAL_REQUEST);
    return ERROR;
}


/*******************************************************************************
* command - process a SCSI command
*
* RETURN
*   Status byte from the scsiIoctl () call or ERROR if the
* opcode is unknown or not functional.  The scsiIoctl() call should set
* errno if an error occurs.
*/

STATUS Exb162::command (
    SCSI_COMMAND scsiCmdBlock,		/* CDB block */
    int cmdblocksize,			/* 6 or 10 byte */
    UINT8 *block,			/* additional data */
    int blocksize)			/* length of additional data */
{
    SCSI_TRANSACTION scsiXaction;

    scsiXaction.cmdAddress = (UINT8 *) scsiCmdBlock;
    scsiXaction.cmdLength = cmdblocksize;
    scsiXaction.dataAddress = block;
						/* passive READ direction */
    scsiXaction.dataDirection = EXB_COMMAND_READ;
    scsiXaction.dataLength = blocksize;
    scsiXaction.addLengthByte = -1;		/* No additional information */

    /* select any opcode-specific changes */
    switch (scsiCmdBlock[0])
    {
	case SCSI_OPCODE_REQUEST_SENSE:
	    scsiXaction.addLengthByte = REQ_SENSE_ADD_LENGTH_BYTE;
	    break;

	case SCSI_OPCODE_WRITE:
	    scsiXaction.addLengthByte = 0;
	    scsiXaction.dataDirection = EXB_COMMAND_WRITE;
	    break;

	case SCSI_OPCODE_WRITE_FILEMARKS:
	case SCSI_OPCODE_SEND_DIAGNOSTIC:
	case SCSI_OPCODE_WRITE_BUFFER:
	    scsiXaction.dataDirection = EXB_COMMAND_WRITE;
	    break;

	case SCSI_OPCODE_INQUIRY:
	    if (scsiXaction.dataLength == 6 || scsiXaction.dataLength == 14)
		scsiXaction.addLengthByte = INQUIRY_ADD_LENGTH_BYTE - 1;
	    else
		scsiXaction.addLengthByte = INQUIRY_ADD_LENGTH_BYTE;
	    break;

	case SCSI_OPCODE_MODE_SENSE:
	    scsiXaction.addLengthByte = MODE_SENSE_ADD_LENGTH_BYTE;
	    break;
    }

    /*
     * Send the request to the tape drive.  The IOCTL does a REQUEST_SENSE
     * command after it executes the command if that command generated a
     * CHECK CONDITION error.  If there was a CHECK_CONDITION we'll need
     * only the SENSE KEY to set the errno.
     *
     * scsiLib PROBLEM: If a variable-block read is too small, CHECK CONDITION
     * is returned but the scsiIoctl() return is OK!  If a variable-block read
     * is too big, scsiIoctl() crashes.
     */

    if (scsiIoctl (pExbPhysDev, FIOSCSICOMMAND, (int) &scsiXaction) != OK ||
	scsiXaction.statusByte != SCSI_STATUS_GOOD)
    {
        switch ((STATUS) scsiXaction.statusByte)
        {
	    case SCSI_STATUS_GOOD:
	        break;

	    case SCSI_STATUS_CHECK_CONDITION:
		switch (pExbPhysDev->pReqSenseData[2] & 0x0f)
		{
		    case SCSI_SENSE_KEY_NOT_READY:
			errnoSet (S_scsiLib_DEV_NOT_READY);
			break;

		    case SCSI_SENSE_KEY_MEDIUM_ERROR:
			errnoSet (S_scsiLib_MEDIUM_ERROR);
			break;
 
		    case SCSI_SENSE_KEY_HARDWARE_ERROR:
			errnoSet (S_scsiLib_HARDWARE_ERROR);
			break;
 
		    case SCSI_SENSE_KEY_ILLEGAL_REQUEST:
			errnoSet (S_scsiLib_ILLEGAL_REQUEST);
			break;
 
		    case SCSI_SENSE_KEY_UNIT_ATTENTION:
			errnoSet (S_scsiLib_UNIT_ATTENTION);
			break;
 
		    case SCSI_SENSE_KEY_DATA_PROTECT:
			errnoSet (S_scsiLib_WRITE_PROTECTED);
			break;
 
		    case SCSI_SENSE_KEY_BLANK_CHECK:
			errnoSet (S_scsiLib_BLANK_CHECK);
			break;
 
		    case SCSI_SENSE_KEY_ABORTED_COMMAND:
			errnoSet (S_scsiLib_ABORTED_COMMAND);
			break;
 
		    case SCSI_SENSE_KEY_VOLUME_OVERFLOW:
			errnoSet (S_scsiLib_VOLUME_OVERFLOW);
			break;
 
		    default:
			errnoSet (S_scsiLib_DEV_UNSUPPORTED);
			break;
		}

		break;

	    case SCSI_STATUS_BUSY:
		errnoSet (S_exbLib_TARGET_IS_BUSY);
		break;

	    case SCSI_STATUS_RESERV_CONFLICT:
		errnoSet (S_exbLib_TARGET_IS_RESERVED);
		break;

	    default:
		errnoSet (S_scsiLib_DEV_UNSUPPORTED);
		break;
	}

	if (exbOpt[pExbPhysDev->scsiDevBusId] & EXB_OPT_SENSE_DUMP)
	{
	    printf ("command: ERROR--target %d, opcode %d, error 0x%02x\n",
		pExbPhysDev->scsiDevBusId, (int) scsiCmdBlock[0],
		(UINT) scsiXaction.statusByte);
	    senseDump ((char *) pExbPhysDev->pReqSenseData);
	}
    }

    return (STATUS) scsiXaction.statusByte;
}


/*
 * Strings to match sense bits
 */
LOCAL char *senseKey[] = 
{
    "No Sense", "Recovered Error", "Not Ready", "Medium Error",
    "Hardware Error", "Illegal Request", "Unit Attention", "Data Protect",
    "Blank Check", "Vendor Key", "Copy Aborted", "Aborted Command",
    "Equal", "Volume Overflow", "Miscompare", "Reserved",
};

LOCAL char *unitSense1[] =
{
    "Logical Beginning of Tape", "Tape Not Present", "Tape Motion Error",
    "Error Counter Overflow", "Media Error", "Formatted Buffer Parity Error",
    "SCSI Bus Parity Error", "Power Fail"
};
 
LOCAL char *unitSense2[] =
{
    "Formatter Error", "Servo System Error", "Write Error 1",
    "Under Run Error", "Filemark Error", "Write Protect",
    "Tape Mark Detect Error", "Transfer Abort Error (8200 only)"
};
 
LOCAL char *unitSense3[] =
{
    "Write Splice Error O", "Write Splice Error B", "Physical End of Tape",
    "Reserved", "Reserved", "Reverse Retries Required", "Cleaned", "Clean",
};


/*******************************************************************************
* senseDump - print the REQUEST_SENSE buffer
*
* This routine prints the contents of the REQUEST_SENSE buffer pointed to by
* "sense" in a human-readable format.  If the input argument "sense" is NULL
* an exbRequestSense() command is first performed.  If "sense" is -1 the
* sense status returned by the last CHECK_CONDITION error from scsiIoctl()
* will be printed.
*
* RETURN
*   The REQUEST_SENSE sense key.
*/

STATUS Exb162::senseDump (
    char *sense)			/* sense block (must be > 29 bytes) */
{
    int i;
    int bits, rwdec;
    char block[EXB_REQUEST_SENSE_SIZE];
    UINT8 *usense;

    /*
     * If there is no input sense block, do a REQUEST_SENSE to get one.  If
     * the sense address is -1, use the last sense from the SCSI_PHYS_DEV.
     */
    if (sense == NULL)
    {
	if (requestSense (block, EXB_REQUEST_SENSE_SIZE)
	    == ERROR)
	    return ERROR;
	usense = (UINT8 *) block;
    }
    else if (sense == (char *) -1)
	usense = pExbPhysDev->pReqSenseData;
    else
	usense = (UINT8 *) sense;

    /* print out the sense block */
    printf ("exbSenseDump: \n");
    printf ("    vendor: %s product: %-8.8s revision: %s\n",
	pExbPhysDev->devVendorID, pExbPhysDev->devProductID,
	pExbPhysDev->devRevLevel);
    printf ("    sense:  %s %s%s%s\n",
	senseKey[SENSE_KEY(usense)],
	SENSE_FMK(usense) ? "Filemark " : "",
	SENSE_EOM(usense) ? "End-of-medium " : "",
	SENSE_ILI(usense) ? "Illegal length indicator " : "");
					/* valid bit--info bytes have value */
    if (SENSE_VALID (usense))
    {
	C4toI4 (&usense[3], i);
	printf ("    unprocessed %s:              %d\n",
	    exbBlocksize[pExbPhysDev->scsiDevBusId] ? "blocks" : "bytes ", i);
    }

    /*
     * It's an 8200 sense block.  Set the byte positions of the status bits
     * and the Read/Write Data Error counter.  Nothing else to be done.
     */

    if (usense[REQ_SENSE_ADD_LENGTH_BYTE] == 6)
    {
	bits = 8;
	rwdec = 11;
    }

    /*
     * It's an 850[05] sense block.  Set the byte positions of the status bits
     * and the Read/Write Data Error counter.  Print out all the other status
     * stuff the 8200 doesn't have.
     */
    else if (usense[REQ_SENSE_ADD_LENGTH_BYTE] == 21)
    {
	bits = 19;
	rwdec = 16;
	printf ("    log parameter page:              0x%02x\n", usense[8]);
	printf ("    log parameter page code:         0x%02x\n", usense[9]);
	printf ("    under/overrun counter:           0x%02x\n", usense[11]);
	printf ("    additional sense code:           0x%02x\n", usense[12]);
	printf ("    additional sense code qualifier: 0x%02x\n", usense[13]);

	C3toI4 (&usense[23], i);
	printf ("    remaining 1 KByte tape blocks:   0x%08x\n", i);
	printf ("    tracking retry counter:          0x%02x\n", usense[26]);
	printf ("    read/write retry counter:        0x%02x\n", usense[27]);
	printf ("    fault symptom code:              0x%02x\n", usense[28]);
    }

    else
	bits = rwdec = 0;

    /*
     * Print the stuff all Exabytes have, although at different locations.
     */
    if (bits != 0)
    {
	C3toI4 (&usense[rwdec], i);
	printf ("    read/write data error counter:   %d\n", i);

	printf ("    unit sense bits 1: ");
	for (i=0; i<8; i++)
	    if (usense[bits] & (1>>i))
		printf ("    %s ", unitSense1[i]);
	printf ("\n");

	printf ("    unit sense bits 2: ");
	for (i=0; i<8; i++)
	    if (usense[bits+1] & (1>>i))
		printf ("    %s ", unitSense2[i]);
	printf ("\n");

	printf ("    unit sense bits 3: ");
	for (i=0; i<8; i++)
	    if (usense[bits+2] & (1>>i))
		printf ("    %s ", unitSense3[i]);
	printf ("\n");
    }

    return (STATUS) usense[2];
}


/*******************************************************************************
* remainingTape - return remaining 1024-byte block until LEOT reached.
*
* This routine returns the number of remaining 1024 Kbytes blocks until
* LEOT is reached.  It does not include any data written to the tape buffer
* but not written to tape.
*
* RETURN
*   OK or ERROR if the command could not be performed.
*/

STATUS Exb162::remainingTape (
    int *remain)			/* remaining blocks */
{
    char block[EXB_REQUEST_SENSE_SIZE];
    UINT8 *usense;

    if (DEVICE_IS_8200 (pExbPhysDev->devProductID))
    {
	errnoSet (S_scsiLib_ILLEGAL_REQUEST);
	return ERROR;
    }
 
    if (requestSense (block, EXB_REQUEST_SENSE_SIZE) == ERROR)
	return ERROR;

    usense = (UINT8 *) block;
    C3toI4 (&usense[23], *remain);

    return OK;
}


/*******************************************************************************
* isFilemark - return filemark flag
*
* This routine returns the state of the sense flag for the end-of-file mark.
*
* RETURN
*   OK Or ERROR if the command could not be performed.
*/

STATUS Exb162::isFilemark (
    BOOL *fmk)				/* TRUE if tape is at EOF */
{
    char block[EXB_REQUEST_SENSE_SIZE];

    if (requestSense (block, EXB_REQUEST_SENSE_SIZE) == ERROR)
	return ERROR;

    *fmk = block[2] & 0x80;

    return OK;
}
/******************************************************************************/
