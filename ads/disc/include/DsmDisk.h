/* DsmDisk.h
   Manages the 9 GB removable disk drives..

   Author: Janet S. Anstett
   Copyright by the National Center for Atmospheric Research
*/

#ifndef DSMDISK_H
#define DSMDISK_H

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#ifdef SVR4
#include <sys/mnttab.h>
#else
#include <mntent.h>
#endif

#include <DsmMessage.h>
#include <TapeHeader.h>
#include <DsmConfig.h>
#include <DsmTape.h>
#include <UnixTask.h>

class DsmDisk
{
public:
  DsmDisk(TapeHeader &th, DsmMessage &win_m, DsmConfig &dsm_c);
  void checkFiles();				// checks the disk status
  void openFiles();                             // opens disk files
  void closeFiles();                            // closes disk files
  void writeFiles(char *, int);                 // writes data to files
  void writeHeader();                           // writes header records
  ~DsmDisk();
  
private:
  TapeHeader &tape_header;
  DsmMessage &win_msg;
  DsmConfig &dsm_config;

  UnixTask	*nimbus;

  int filesopened;                              // indicates file status
  int diskfull;                                 // indicates disk full
  int rmounted[2];                              // Which removables are mounted
  int numberwrites;                             // how many writes to different files
  char rtdatadir[2][80];                        // directories for real time data
  int outads[2], out2d[2], outmcr[2], outavaps[2], outgrey[2];
  int outmh[2], outmd[2], outdh[2];             // handles to disk files
  FILE *ptrads[2], *ptr2d[2], *ptrmcr[2], *ptravaps[2], *ptrgrey[2];
  FILE *ptrmh[2], *ptrmd[2], *ptrdh[2];         // ptrs to disk files
  int cntmcr;                                    // count of MCR records in file
  char destfile[2][80], dest2d[80], destmcr[2][80], destavaps[80], destgrey[80];
  char destmh[80], destmd[80], destdh[80];
  char extfile[10], ext2d[10], extmcr[10], extavaps[10], extgrey[10];
  char extmh[10], extmd[10], extdh[10];         // file name extensions
  int pms2d, mcr, avaps, masp, grey;            // indicator of existing data
  char msg_str[DSM_MSG_STRING_SIZE];          	// message string data
};

#endif
