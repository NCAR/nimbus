/* DsmDisk.cc
   Manages the 9 GB removable drives.

   Author: Janet S. Anstett 
   Copyright by the University Corporation for Atmospheric Research
*/

#include <DsmDisk.h>

/* -------------------------------------------------------------------- */
DsmDisk::DsmDisk(TapeHeader &th, DsmMessage &win_m, DsmConfig &dsm_c) :
      tape_header (th), win_msg (win_m), dsm_config (dsm_c)
{
   int i, j;
   char savename[256];
   FILE *mntptr;
   int savelen;

#ifdef SVR4
   struct mnttab *mtresult;
#else
   struct mntent *mtresult;
#endif
   
   filesopened = FALSE;
   diskfull = FALSE;
   avaps = FALSE;
   mcr = FALSE;
   pms2d = FALSE;
   masp = FALSE;
   grey = FALSE;
   
   strcpy(extfile,".ads");
   strcpy(extmcr,".rawMCRa");
   strcpy(ext2d,".2d");
   strcpy(extgrey,".Grey");
   strcpy(extavaps,".avaps");
   strcpy(extdh,".maspdh");
   strcpy(extmh,".maspmh");
   strcpy(extmd,".maspmd");
   sprintf(rtdatadir[0],"/jnet/r1/");
   sprintf(rtdatadir[1],"/jnet/r2/");
   savelen = strlen(rtdatadir[0]) - 1;
   for (i = 0; i < 2; i++)
   {
      rmounted[i] = FALSE;
      sprintf(msg_str,"mount %s >&/dev/null",rtdatadir[i]);
      system(msg_str);
   }
#ifdef SVR4
   if ((mntptr = fopen("/etc/mnttab","r")) == NULL)
#else
   if ((mntptr = fopen("/etc/mtab","r")) == NULL)
#endif
   {
      sprintf(msg_str,"DsmDisk: Cannot open mounttab file.\n");
      win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
   }
#ifdef SVR4
   while (getmntent(mntptr,mtresult) != -1)
#else
   while ((mtresult = getmntent(mntptr)) != NULL)
#endif
   {
      for (i = 0; i < 2; i++)
      {
#ifdef SVR4
         if (strncmp(mtresult->mnt_mountp,rtdatadir[i],savelen) == 0)
#else
         if (strncmp(mtresult->mnt_dir,rtdatadir[i],savelen) == 0)
#endif
         {
            rmounted[i] = TRUE;
         }
      }
   }
   fclose(mntptr);
   if (rmounted[0] == TRUE)
   {
      sprintf(msg_str,"DsmDisk: Removable drive r1 mounted.\n");
      win_msg.sendTapeMsg(RECORDING, R1DRIVE, msg_str);
   }
   else
   {
      sprintf(msg_str,"DsmDisk: Removable drive r1 not mounted, record on local drive.\n");
      win_msg.sendTapeMsg(UNLOADED, R1DRIVE, msg_str);
   }
   numberwrites = 1;
   if (rmounted[1] == TRUE)
   {
      sprintf(msg_str,"DsmDisk: Removable drive r2 mounted.\n");
      win_msg.sendTapeMsg(RECORDING, R2DRIVE, msg_str);
      numberwrites = 2;
   }   

// Delete all flight 0 data files.
   for (i = 0; i < numberwrites; i++)
   {
      sprintf(savename,"%s0a%s",rtdatadir[i],extfile);
      unlink(savename);
      sprintf(savename,"%s0a%s",rtdatadir[i],ext2d);
      unlink(savename);
      sprintf(savename,"%s0a%s",rtdatadir[i],extgrey);
      unlink(savename);
      sprintf(savename,"%s0a%s",rtdatadir[i],extavaps);
      unlink(savename);
      sprintf(savename,"%s0a%s",rtdatadir[i],extdh);
      unlink(savename);
      sprintf(savename,"%s0a%s",rtdatadir[i],extmh);
      unlink(savename);
      sprintf(savename,"%s0a%s",rtdatadir[i],extmd);
      unlink(savename);
      sprintf(savename,"%s0a%s",rtdatadir[i],extmcr);
      for (j = 0; j < 10; j++)
      {
         unlink(savename);
         ++savename[strlen(savename)-1];
      }
   }
}

/* -------------------------------------------------------------------- */
void DsmDisk::openFiles()
{
// Opens the files to store the data in.  Renames current flight files so 
// existing files will not be overwritten.
   
   char *p, buffer[256];
   char savename[256];
   FILE *fp;
   char savelet[2][2];
   char saveletter[2];
   int i;
   time_t t;
   struct tm *ts;

   umask(000);
   strcpy(savelet[0],"a");
   strcpy(savelet[1],"a");
   p = getenv("PROJ_DIR");
   dsm_config.selectByLocn(DSM_CTL_STR);
   sprintf(buffer,"%s/hosts/%s/rtdata.filename", p, dsm_config.hostName());

   if ((fp = fopen(buffer,"w")) == NULL)
   {
      fprintf(stderr,"DsmDisk: Open of %s failed.\n",buffer);
      exit(1);
   }
   t = time(NULL);
   ts = gmtime(&t);
   if (strcmp(tape_header.fltnum(),"0") == 0)
      sprintf(buffer,"%02d%02d%02d%02d-%s",ts->tm_mon+1,
         ts->tm_mday,ts->tm_hour,ts->tm_min,tape_header.fltnum());
   else
   {
      sprintf(buffer,"%s%s",tape_header.projectNumber(),tape_header.fltnum());
   }
//
// Check for existence of a.ads file for same project and flight.
// If it exists, change the first "a" to a "b" and check again,
// before opening new files.
// This will prevent data files from being overwritten, when there is a 
// midflight restart.
//
   for (i = 0; i < numberwrites; i++)
   {
      sprintf(savename,"%s%sa%s",rtdatadir[i],buffer,extfile);
      while (access(savename,F_OK) == 0)
      {
         ++savename[strlen(savename)-5];
         ++savelet[i][0];
      }
   }
   if (strcmp(savelet[0],savelet[1]) > 0)
      strcpy(saveletter,savelet[0]);
   else
      strcpy(saveletter,savelet[1]);
   strcat(buffer,saveletter);
   for (i = 0; i < numberwrites; i++)
   {
      sprintf(destfile[i],"%s%s",rtdatadir[i],buffer);
   }
   fprintf(fp,"%s\n",destfile[0]);
   fclose(fp);

   for (i = 0; i < numberwrites; i++)
   {
      strcpy(dest2d,destfile[i]);
      strcpy(destgrey,destfile[i]);
      strcpy(destmcr[i],destfile[i]);
      strcpy(destavaps,destfile[i]);
      strcpy(destmh,destfile[i]);
      strcpy(destmd,destfile[i]);
      strcpy(destdh,destfile[i]);
      strcat(destfile[i],extfile);
      if ((outads[i] = open(destfile[i], O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
      {
         sprintf(msg_str,"DsmDisk: Cannot open ads destination file %s\n",destfile[i]);
         win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
      }
// Associate a FILE stream with an existing file handle.
      if ((ptrads[i] = fdopen(outads[i],"w")) == NULL)
      {
         sprintf(msg_str,"DsmDisk: Cannot open ptr to ads file %s\n",destfile[i]);
         win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
      }
      if ((pms2d = tape_header.firstDesc(PMS2D_STR)) == TRUE)
      {
         strcat(dest2d,ext2d);
         if ((out2d[i] = open(dest2d, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
         {
            sprintf(msg_str,"DsmDisk: Cannot open 2D destination file %s\n",dest2d);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            pms2d = FALSE;
         }
// Associate a FILE stream with an existing file handle.
         if ((ptr2d[i] = fdopen(out2d[i],"w")) == NULL)
         {
            sprintf(msg_str,"DsmDisk: Cannot open ptr to 2d file %s\n",dest2d);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
      if ((grey = tape_header.getPMS2DType(P2D_G_STR)) == TRUE)
      {
         strcat(destgrey,extgrey);
         if ((outgrey[i] = open(destgrey, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
         {
            sprintf(msg_str,"DsmDisk: Cannot open grey scale destination file %s\n",destgrey);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            grey = FALSE;
         }
// Associate a FILE stream with an existing file handle.
         if ((ptrgrey[i] = fdopen(outgrey[i],"w")) == NULL)
         {
            sprintf(msg_str,"DsmDisk: Cannot open ptr to grey scale file %s\n",destgrey);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
      if ((mcr = tape_header.getSDI("MCROUT")) == TRUE)
      {
         strcat(destmcr[i],extmcr);
         if ((outmcr[i] = open(destmcr[i], O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
         {
            sprintf(msg_str,"DsmDisk: Cannot open MCR destination file %s\n",destmcr[i]);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            mcr = FALSE;
         }
// Associate a FILE stream with an existing file handle.
         if ((ptrmcr[i] = fdopen(outmcr[i],"w")) == NULL)
         {
            sprintf(msg_str,"DsmDisk: Cannot open ptr to mcr file %s\n",destmcr[i]);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
      if ((avaps = tape_header.getAsync(AVAPS_STR)) == TRUE)
      {
         strcat(destavaps,extavaps);
         if ((outavaps[i] = open(destavaps, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
         {
            sprintf(msg_str,"DsmDisk: Cannot open avaps destination file %s\n",destavaps);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            avaps = FALSE;
         }
// Associate a FILE stream with an existing file handle.
         if ((ptravaps[i] = fdopen(outavaps[i],"w")) == NULL)
         {
            sprintf(msg_str,"DsmDisk: Cannot open ptr to avaps file %s\n",destavaps);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
      if ((masp = tape_header.firstDesc(MASP_STR)) == TRUE)
      {
         strcat(destmh,extmh);
         if ((outmh[i] = open(destmh, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
         {
            sprintf(msg_str,"DsmDisk: Cannot open masp destination file %s\n",destmh);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
// Associate a FILE stream with an existing file handle.
         if ((ptrmh[i] = fdopen(outmh[i],"w")) == NULL)
         {
            sprintf(msg_str,"DsmDisk: Cannot open ptr to masp file %s\n",destmh);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
         strcat(destmd,extmd);
         if ((outmd[i] = open(destmd, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
         {
            sprintf(msg_str,"DsmDisk: Cannot open masp destination file %s\n",destmd);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
// Associate a FILE stream with an existing file handle.
         if ((ptrmd[i] = fdopen(outmd[i],"w")) == NULL)
         {
            sprintf(msg_str,"DsmDisk: Cannot open ptr to masp file %s\n",destmd);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
         strcat(destdh,extdh);
         if ((outdh[i] = open(destdh, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
         {
            sprintf(msg_str,"DsmDisk: Cannot open masp destination file %s\n",destdh);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
// Associate a FILE stream with an existing file handle.
         if ((ptrdh[i] = fdopen(outdh[i],"w")) == NULL)
         {
            sprintf(msg_str,"DsmDisk: Cannot open ptr to masp file %s\n",destdh);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
   }
   umask(002);
   filesopened = TRUE;
}

/* -------------------------------------------------------------------- */
void DsmDisk::writeHeader()
{
   int i;
   
// Writes the leader and two copies of the header record to the appropriate
// output files.

   for (i = 0; i < numberwrites; i++)
   {
      if (outads[i] != ERROR)
      {
         write(outads[i],FIRST_REC_STRING,FIRST_REC_SIZE);
         write(outads[i],tape_header.getHeader(),tape_header.thdrlen());
         write(outads[i],tape_header.getHeader(),tape_header.thdrlen());
      }
      if (pms2d)
      {
         write(out2d[i],FIRST_REC_STRING,FIRST_REC_SIZE);
         write(out2d[i],tape_header.getHeader(),tape_header.thdrlen());
         write(out2d[i],tape_header.getHeader(),tape_header.thdrlen());
      }
      if (grey)
      {
         write(outgrey[i],FIRST_REC_STRING,FIRST_REC_SIZE);
         write(outgrey[i],tape_header.getHeader(),tape_header.thdrlen());
         write(outgrey[i],tape_header.getHeader(),tape_header.thdrlen());
      }
      if (mcr)
      {
         write(outmcr[i],FIRST_REC_STRING,FIRST_REC_SIZE);
         write(outmcr[i],tape_header.getHeader(),tape_header.thdrlen());
         write(outmcr[i],tape_header.getHeader(),tape_header.thdrlen());
      }
   }
}

/* -------------------------------------------------------------------- */
void DsmDisk::closeFiles()
{
   int i;
   
   nimbus->killTask();
   sleep(2);

   for (i = 0; i < numberwrites; i++)
   {
      if (outads[i] != ERROR)
         close(outads[i]);
      if (pms2d && (out2d[i] != ERROR))
         close(out2d[i]);
      if (mcr && (outmcr[i] != ERROR))
         close(outmcr[i]);
      if (avaps && (outavaps[i] != ERROR))
         close(outavaps[i]);
      if (masp)
      {
         if (outmh[i] != ERROR)
            close(outmh[i]);
         if (outdh[i] != ERROR)
            close(outdh[i]);
         if (outmd[i] != ERROR)
            close(outmd[i]);
      }
   }

   filesopened = FALSE;

   for (i = 0; i < numberwrites; i++)
   {
      if (rmounted[i] == TRUE)
      {
         sprintf(msg_str,"umount %s",rtdatadir[i]);
//         sprintf(msg_str,"umount %s >&/dev/null",rtdatadir[i]);
         system(msg_str);
      }
   }
}
 
/* -------------------------------------------------------------------- */
void DsmDisk::writeFiles(char *buff, int len)
{
   int i, status, rc[2];
   static bool firstSync = true;
   static Hdr_blk startTime, endTime;
   
// If files have not been opened when the flight number has been entered and
// the starting date and time have been received from the dsms, then
// open the files to store the data in.

   if (diskfull)
      return;
   if (! filesopened)
      if (strlen(tape_header.fltnum()) && strlen(tape_header.date()))
      {
         openFiles();
         writeHeader();

         nimbus = new UnixTask("/usr/local/bin/nimbus");
      }
      else
         return;

//printf("record type %x length %d\n",((unsigned short *)buff)[0],len);
   switch (ntohs(*(unsigned short *)buff))
   {
   case 0x8681:
      for (i = 0; i < numberwrites; i++)
      {
         rc[i] = write(outads[i],buff,len);
         status = fflush(ptrads[i]);
         if (status != 0)
         {
            sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",destfile[i]);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
      if (firstSync)
      {
         memcpy((char *)&startTime,buff,sizeof(Hdr_blk));
         firstSync = false;
      }
      else
         memcpy((char *)&endTime,buff,sizeof(Hdr_blk));
      break;
         
   case 0x4331:	// PMS2D types.
   case 0x4332:
   case 0x5031:
   case 0x5032:
   case 0x4831:	// HVPS
   case 0x4832:
      if (!pms2d)
         return;
      for (i = 0; i < numberwrites; i++)
      {
for (int k = 0; k < 7; ++k)
  printf("%x ", ((P2d_rec *)buff)[k].id);
printf("\n");
         rc[i] = write(out2d[i],buff,len);
         status = fflush(ptr2d[i]);
         if (status != 0)
         {
            sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",dest2d);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }

{ P2d_rec *p = (P2d_rec *)buff;
printf("%x %02d:%02d:%02d - %d ftell=%d\n", ntohs(p->id), ntohs(p->hour), ntohs(p->minute), ntohs(p->second), ntohs(len), ftell(ptr2d[0]));
}
      break;

   case 0x4731: // Grey scale
   case 0x4732:
      if (!grey)
         return;
      for (i = 0; i < numberwrites; i++)
      {
         rc[i] = write(outgrey[i],buff,len);
         status = fflush(ptrgrey[i]);
         if (status != 0)
         {
            sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",destgrey);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
      break;

   case 0x4d43:	// MCR
      if (!mcr)
         return;
      for (i = 0; i < numberwrites; i++)
      {
         rc[i] = write(outmcr[i],buff,len);
         status = fflush(ptrmcr[i]);
         if (status != 0)
         {
            sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",destmcr[i]);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
      ++cntmcr;

      if (cntmcr >= 100800)	// close file after 3 hours.
      {
         for (i = 0; i < numberwrites; i++)
         {
            close(outmcr[i]);
            ++destmcr[i][strlen(destmcr[i])-1];	// increment file name.
            if ((outmcr[i] = open(destmcr[i], O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERROR)
            {
               sprintf(msg_str,"DsmDisk: Cannot open MCR file %s.  Continuing.\n",destmcr[i]);
               win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
               mcr = FALSE;
               return;
            }
// Write header to file.
            write(outmcr[i],FIRST_REC_STRING,FIRST_REC_SIZE);
            write(outmcr[i],tape_header.getHeader(),tape_header.thdrlen());
            write(outmcr[i],tape_header.getHeader(),tape_header.thdrlen());
// Associate a FILE stream with an existing file handle.
            if ((ptrmcr[i] = fdopen(outmcr[i],"w")) == NULL)
            {
               sprintf(msg_str,"DsmDisk: Cannot open ptr to mcr file %s\n",destmcr[i]);
               win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            }
            status = fflush(ptrmcr[i]);
            if (status != 0)
            {
               sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",destmcr[i]);
               win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            }
            cntmcr = 0;
         }
      }
      break;

   case 0x4156:	// AVAPS Dropsonde.
      if (!avaps)
         return;
      for (i = 0; i < numberwrites; i++)
      {
         rc[i] = write(outavaps[i],buff,len);
         status = fflush(ptravaps[i]);
         if (status != 0)
         {
            sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",destavaps);
            win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         }
      }
      break;
      
   case 0x4450:       // MASP data
      if (!masp)
         return;
      switch(ntohs(((unsigned short *)buff)[2]))
      {
      case 0x4448:   // MASP data housekeeping packet.
         for (i = 0; i < numberwrites; i++)
         {
            rc[i] = write(outdh[i],buff,len);
            status = fflush(ptrdh[i]);
            if (status != 0)
            {
               sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",destdh);
               win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            }
         }
         break;
      case 0x1:      // MASP housekeeping packet.
         for (i = 0; i < numberwrites; i++)
         {
            rc[i] = write(outmh[i],buff,len);
            status = fflush(ptrmh[i]);
            if (status != 0)
            {
               sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",destmh);
               win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            }
         }
         break;
      case 0x2:      // MASP data packet.
         for (i = 0; i < numberwrites; i++)
         {
            rc[i] = write(outmd[i],buff,len);
            status = fflush(ptrmd[i]);
            if (status != 0)
            {
               sprintf(msg_str,"DsmDisk: ERROR on flush on %s.\n",destmd);
               win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
            }
         }
         break;
      }
      break;

   case 0x4144:	// Header records.
   case 0x5448:
      break;

   default:
     fprintf(stderr, "DsmDisk: Dropping packet, id=0x%0x.\n", ntohs(*(ushort*)buff));
   }


   if ((rc[0] == ERROR) || (rc[1] == ERROR))
   {
      if (errno == ENOSPC)
      {
         sprintf(msg_str,"DsmDisk:>>> DISK FULL, closing disk file(s), continuing.\n");
         win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
         closeFiles();
         diskfull = TRUE;
         return;
      }
      sprintf(msg_str,"DsmDisk: Write failure to dest disk, errno = %d.\n", errno);
      win_msg.sendTapeMsg(FAILED,filesopened,msg_str);
   }
}

/* -------------------------------------------------------------------- */
DsmDisk::~DsmDisk()
{
   closeFiles();
}

