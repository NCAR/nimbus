/*
filemgr.c

Provide services for creation and updating of ascii text files, and restoring
values from them. These text files are assumed to consist of "entries", which
consist of ascii strings in keyword-value format.

The keyword is assumed to be one word, while the string value(s) that follow it
may be any number of words. Exactly one space is to be between the keyword-value
pairs. These constraints are managed properly if the files in question are 
written by this module only, i.e. no manual editing is to be done. The user
need only ensure that the total length of the keyword-value string is no more 
than MAXENTRYLENGTH, and that there are no more than MAXENTRIES entries total
for each entity being dealt with. These violations will not cause a crash here,
but can result in loss of the complete intended string.

It is provided that the file being read and written may contain entries from
other user modules than just the one calling these routines. This is why the
user must supply functions with info about current state of only the relevant
set of keywords, which may not be all the keywords in the file. Only those
keyword entries are changed. For instance, a user initialization file may
contain entries from various user-config menus, e.g. printer setup and color
choice; when the printer setup is changed and saved, the color choices should
be left undisturbed. Thus the call is made with only the user routine that
updates the printer parameters, and only those entries are updated in the file.

When the files are opened, lockf()'s are applied to prevent simultaneous
access. A save file is created on open for writing in case of crash before
the file is written back out; this file is optionally used instead of the
normal file on subsequent access.

User's should access only the following functions:

SaveEntriesToFile()
RestoreEntriesFromFile()
ReadFileIntoEntryArray()

...consult the documentation at the start of those procedures for details on
their use.
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>

#define ERROR -1
#define OK 1
#define FALSE 0
#define TRUE 1
#define MAXENTRYLENGTH 512
#define MAXENTRIES 300
#define SAVESUFFIX ".filemgrsave"
#define MAXERRMSGLENGTH 128
#define MAXFLNMLENGTH 128
/*
error conditions
*/
#define USERECOVERYFILE 0
#define FILENOTOPENED 1

/*
buffer for file read
*/
static char Entry[MAXENTRIES][MAXENTRYLENGTH];
/*
number of entries in the file
*/
static int NumFileEntries;
/*
FILE stuff
*/
static FILE *FileHandle;
/*
save file stuff
*/
static int RecoveryMode=FALSE;
/*
declarations
*/
int (*ErrorReportFunc)();

/***************************  GetEntryString() ********************************/

char *
GetEntryString(int which)
/*
if the index is within bounds, return the which'th Entry, else return NULL
*/
{
 if (which <0 || which>=MAXENTRIES) {
  (void)fprintf(stderr,
   "WARNING: value %d out of bounds as index in GetEntryString()\n",which);
  return NULL;
 }
 return Entry[which]; 
}

/***************************  PrintEntries() ********************************/

PrintEntries()
/*
debugging aid: print out the buffer contents version of current file 
*/
{
int entry;
 for (entry=0; entry<GetNumFileEntries(); entry++) {
  (void)fprintf(stderr,"%s\n",(char *)GetEntryString(entry));
 }
}

/**************************  SetEntryString() ********************************/

SetEntryString(which,EntryKeyword,EntryValue)
int which;
char *EntryKeyword,*EntryValue;
/*
if the index which is within bounds, update the value of the which'th entry
with keyword-value pair EntryKeyword-EntryValue
*/
{
 if (which < 0 || which>=MAXENTRIES) {
  (void)fprintf(stderr,
   "WARNING: index %d out of bounds in SetEntryString()\n",which);
  return;
 }
 if (EntryKeyword) {
  if (strlen(EntryKeyword)+strlen(EntryValue) >=MAXENTRYLENGTH) {
   (void)fprintf(stderr,
    "WARNING: exceeded MAXENTRYLENGTH in SetEntryString()\n");
   return;
  }
  (void)sprintf(&Entry[which][0],"%s %s",EntryKeyword,EntryValue);
 } else {
  (void)sprintf(&Entry[which][0],"%s",EntryValue);
 }
}

/***********************  RestoreEntriesFromFile() **************************/

RestoreEntriesFromFile(filename,NumUserEntries,
 GetUserEntryKeyword,SetUserEntryValue,ErrorFunc)
char *filename;
int NumUserEntries;
char * (*GetUserEntryKeyword)();
int (*SetUserEntryValue)();
int (*ErrorFunc)();
/*
Read contents of file named filename into Entry buffer; if it indeed has any
entries, search it with each of NumUserEntries user entries, using user-supplied
function GetUserEntryKeyword() to find a match in the buffer. These "user 
entries" are the current state in memory of the user entity using this module. 
If a keyword match is found, call the user-supplied function SetUserEntryValue 
with the entry number and entire entry string, to let the user do whatever is 
needed with the associated value. 

Error conditions are reported back to user supplied function ErrorFunc().
*/
{
int entry, position;

 ErrorReportFunc=ErrorFunc;
 if (ReadFileIntoEntryArray(filename,ErrorFunc)==ERROR)
  return;
 for (entry=0; entry<NumUserEntries; entry++) {
  if ( (position=FindEntryWithKeyword((char *)(*GetUserEntryKeyword)(entry)))!=ERROR) {
   (*SetUserEntryValue)(entry,(char *)GetEntryString(position));
  }
 }
}

/**************************  SaveEntriesToFile() *****************************/

SaveEntriesToFile(filename,NumUserEntries,GetUserEntryKeyword,GetUserEntryValue,
ErrorFunc)
char *filename;
int NumUserEntries;
char * (*GetUserEntryKeyword)();
char * (*GetUserEntryValue)();
int (*ErrorFunc)();
/*
Read contents of file named filename into Entry buffer to establish the
current saved state of ALL keywords in that file. Examine each keyword in
buffer and search for a match in set of keywords supplied by user function
GetUserEntryKeyword(). If match is found, update that buffer entry with the 
value obtained from user function GetUserEntryValue(). The user MUST supply
a non-NULL value with this function.

Save user's state by writing Entry buffer back out to file.

Error conditions are reported back to user supplied function ErrorFunc().
*/
{
int entry,position;

 ErrorReportFunc=ErrorFunc;
 if (ReadFileIntoEntryArray(filename,ErrorFunc)==ERROR) {
  (void)AlertUser("WARNING: SaveEntriesToFile couldn't read in file");
 }
 for (entry=0; entry<NumUserEntries; entry++) {
  if ((position=FindEntryWithKeyword((char *)(*GetUserEntryKeyword)(entry)))==ERROR) {
/*
keyword not found in file; append it as a new one
*/
   AppendEntry((char *)(*GetUserEntryKeyword)(entry),
               (char *)(*GetUserEntryValue)(entry));
  } else
   SetEntryString(position,
    (char *)(*GetUserEntryKeyword)(entry),(char *)(*GetUserEntryValue)(entry));
 }
 if (WriteEntriesToFile(filename)==ERROR)
  (void)AlertUser("WARNING: SaveEntriesToFile couldn't write out file");
}

/***********************  AppendEntry() *****************************/

AppendEntry(keyword,value)
char *keyword,*value;
{
 SetEntryString(GetNumFileEntries(),keyword,value);
 SetNumFileEntries(GetNumFileEntries()+1);
}

/***********************  FindEntryWithString() *****************************/

FindEntryWithString(String)
char *String;
/*
search the Entry buffer for a match with String, which can be anywhere
in the string. If found, return its position in the file, else return ERROR.
*/
{
int entry;
 
 for (entry=0; entry<GetNumFileEntries(); entry++) {
  if (strstr((char *)GetEntryString(entry),String)!=NULL) {
   return entry;
  }
 }
 return ERROR;
}

/***********************  FindEntryWithKeyword() *****************************/

FindEntryWithKeyword(EntryKeyword)
char *EntryKeyword;
/*
search the Entry buffer for a match with EntryKeyword, assumed to be the first
word in the string. If found, return its position in the file, else return ERROR.
*/
{
int entry;
char CandidateKeyword[MAXENTRYLENGTH];
 
 for (entry=0; entry<GetNumFileEntries(); entry++) {
  (void)sscanf((char *)GetEntryString(entry),"%s",CandidateKeyword);
  if (!(strcmp(CandidateKeyword,EntryKeyword))) {
   return entry;
  }
 }
 return ERROR;
}

/**************************  SetNumFileEntries() *****************************/

SetNumFileEntries(value) 
int value;
/*
Set the number of File Entries to value.
*/
{
 NumFileEntries=value;
}

/**************************  GetNumFileEntries() *****************************/

GetNumFileEntries() 
/*
return the number of file entries.
*/
{
 return NumFileEntries;
}

/****************************************************************************/

ReadFileIntoArray(filename,ErrorFunc)
char *filename;
int (*ErrorFunc)();
/*
Read contents of file named filename into Entry buffer, updating value of
NumFileEntries.

Error conditions are reported back to user supplied function ErrorFunc().
*/
{
char entryvalue[MAXENTRYLENGTH];

 ErrorReportFunc=ErrorFunc;
 SetNumFileEntries(0);
 if (OpenFile(filename,"r")==OK) {
  while (fgets(entryvalue,MAXENTRYLENGTH,FileHandle)!=NULL) {
   SetEntryString(GetNumFileEntries(),(char *)NULL,entryvalue);
   SetNumFileEntries(GetNumFileEntries()+1);
  }
  CloseFile(filename,"r");
  return OK;
 } else {
  return ERROR;
 } 
}

/***********************  ReadFileIntoEntryArray() **************************/

ReadFileIntoEntryArray(filename,ErrorFunc)
char *filename;
int (*ErrorFunc)();
/*
Read contents of file named filename into Entry buffer, updating value of
NumFileEntries.

Error conditions are reported back to user supplied function ErrorFunc().
*/
{
char keyword[MAXENTRYLENGTH];
char entryvalue[MAXENTRYLENGTH];

 ErrorReportFunc=ErrorFunc;
 SetNumFileEntries(0);
 if (OpenFile(filename,"r")==OK) {
/*
assuming one and only one space between keyword and entry value string(s)
*/
  while (fscanf(FileHandle,"%s ",keyword)!=EOF &&
         fgets(entryvalue,MAXENTRYLENGTH,FileHandle)!=NULL) {
/*
lose the trailing line feed inherited from fgets()
*/
   (void)sprintf(&entryvalue[strlen(entryvalue)-1],"\0");
   SetEntryString(GetNumFileEntries(),keyword,entryvalue);
   SetNumFileEntries(GetNumFileEntries()+1);
  }
  CloseFile(filename,"r");
  return OK;
 } else {
  return ERROR;
 } 
}

/**************************  WriteEntriesToFile() *****************************/

WriteEntriesToFile(filename)
char *filename;
/*
Write the contents of Entry buffer back out to file named filename.
*/
{
int entry;

 if (OpenFile(filename,"w")==OK) {
  for (entry=0; entry<GetNumFileEntries(); entry++) {
   (void)fprintf(FileHandle,"%s\n",(char *)GetEntryString(entry));
  }
  CloseFile(filename,"w");
  return OK;
 } else {
  return ERROR;
 }
}

/*****************************  OpenFile() ********************************/

OpenFile(filename,mode)
char *filename;
char *mode;
/*
Open file named filename with mode mode; check for saved file, if it exists
enter Recovery Mode. Lock the file if mode is for writing. If not in Recovery
Mode, make a copy of the file in case of abnormal termination before completion,
if mode is for writing.
*/
{
char *Filename;
char ErrorMsg[MAXERRMSGLENGTH];
char savefilename[MAXFLNMLENGTH];

 (void)sprintf(savefilename,"%s%s",filename,SAVESUFFIX);
 if (SaveFileExists(savefilename)) {
  if (AlertUser("Recovery File exists; use it instead?")) {
   SetRecoveryMode(TRUE);
  } else {
   SetRecoveryMode(FALSE);
  }
 }
 errno=0;
 (void)sprintf(ErrorMsg,"\0");
 Filename=GetRecoveryMode()?savefilename:filename;
 if ( (FileHandle=(FILE *)WINDSfopen(Filename,mode))==NULL) {
  switch (errno) {
/*
error codes as per intro(2)
*/
   case ENOENT:
    if (!strcmp(mode,"w"))
     (void)sprintf(ErrorMsg,"Cannot write file %s in OpenFile",Filename);
    else
     (void)sprintf(ErrorMsg,"File %s does not exist in OpenFile",Filename);
    break;
   case EACCES:
    (void)sprintf(ErrorMsg,"Permission problem in OpenFile on %s",Filename);
    break;
   default:
    (void)sprintf(ErrorMsg,
     "Error # %d in OpenFile opening file %s; see stderr for details",
     errno,Filename);
    perror("OpenFile");
    break;
  }
  (void)AlertUser(ErrorMsg);
  SetRecoveryMode(FALSE);
  return ERROR;
 } else {
  if (!(strcmp(mode,"w"))) {
   (void)LockFile(0,FileHandle);
   if (!GetRecoveryMode())
    SaveFile(savefilename,FileHandle);
  }
  return OK;
 }
}

/*****************************  CloseFile() ********************************/

CloseFile(filename,mode)
char *filename;
char *mode;
/*
Close file named filename; if write mode, unlock it. Remove the save-recovery
file.
*/
{
 if (!(strcmp(mode,"w"))) {
  UnlockFile(0,FileHandle);
 }
 if (GetRecoveryMode()) {
  SetRecoveryMode(FALSE);
 }
 if ( (WINDSfclose(FileHandle)==EOF))
  (void)AlertUser("WARNING: problem closing file");
 RemoveSaveFile(filename);
}

/*****************************  LockFile() ********************************/

LockFile(fd,fp)
int fd;
FILE *fp;
/*
Lock file with either file descriptor fd or filehandle fp.
*/
{
 int filedes = fd?fd:fileno(fp);

 if (lockf(filedes,F_LOCK,0)==ERROR) {
  switch (errno) {
   case ECOMM:
    (void)AlertUser("WARNING: file lock failed: link to remote machine gone");
    break;
   case EAGAIN:
   case EACCES:
    (void)AlertUser("WARNING: file lock failed: file already locked");
    break;
   case EBADF:
    (void)AlertUser("WARNING: file lock failed: bad fd or no write permission");
    break;
   case EDEADLK:
    (void)AlertUser("WARNING: file lock failed: deadlock would occur");
    break;
   case EINTR:
    (void)AlertUser("WARNING: file lock failed: signal interrupt");
    break;
   case ENOLCK:
    (void)AlertUser("WARNING: file lock failed: no more file locks available");
    break;
  }
  return ERROR;
 }
 return OK;
}

/*****************************  UnlockFile() ********************************/

UnlockFile(fd,fp)
int fd;
FILE *fp;
/*
Unlock file with either file descriptor fd or filehandle fp.
*/
{
 int filedes = fd?fd:fileno(fp);

 if (lockf(filedes,F_ULOCK,0)==ERROR) {
  switch (errno) {
   case EBADF:
    (void)AlertUser("WARNING: file unlock failed: bad fd");    
    break;
   case ENOLCK:  
    (void)AlertUser("WARNING: file unlock failed: no more file locks available");
    break;
  }
  return ERROR;
 }
 return OK;
}

/*****************************  SaveFile() ********************************/

/*ARGSUSED*/
SaveFile(filename,fp)
char *filename;
FILE *fp;
/*
Create a recovery file version of the file
*/
{
}

/**************************  RemoveSaveFile() ********************************/

RemoveSaveFile(filename)
char *filename;
/*
Remove the recovery version of the file
*/
{
char savefilename[MAXFLNMLENGTH];

 (void)sprintf(savefilename,"%s%s",filename,SAVESUFFIX);
}

/**************************  SetRecoveryMode() *****************************/


SetRecoveryMode(value)
int value;
/*
set Recovery Mode to value
*/
{
 RecoveryMode=value;
}

/**************************  GetRecoveryMode() *****************************/

GetRecoveryMode() 
/*
return the value of Recovery Mode
*/
{
 return RecoveryMode;
}

/**************************  SaveFileExists() *****************************/

/*ARGSUSED*/
SaveFileExists(filename) 
char *filename;
/*
if the file named filename exists, return TRUE, else return FALSE
*/
{
/***
int result;
FILE *fp;

 result = ((fp=(FILE *)WINDSfopen(filename,"r"))!=NULL);
 (void)WINDSfclose (fp);
 return result;
***/
 return FALSE;
}

/*****************************  AlertUser() ********************************/

AlertUser(message) 
char *message;
/*
notify the user with the string in message
*/
{
 if (ErrorReportFunc) {
  if (message && strlen(message)) {
   (*ErrorReportFunc)(message);
  }
 } else {
  if (message && strlen(message)) {
   (void)fprintf(stderr,"%s\n",message);
  }
 }
 return FALSE;
}
