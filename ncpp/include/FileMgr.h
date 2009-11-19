/*
-------------------------------------------------------------------------
OBJECT NAME:	FileMgr.h

FULL NAME:	Data File Manager

TYPE:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#ifndef FILEMGR_H
#define FILEMGR_H

#include "define.h"

#include "DataFile.h"
#include "PlotMgr.h"


/* -------------------------------------------------------------------- */
class FileManager
{
public:
	FileManager();

  friend class DataSource;

  void		NewFile(const char fileName[]);
  void		AddFile(const char fileName[]);

  void		SetCurrentFile(int newFile)	{ _currentFile = newFile; }

  int		NumberOfFiles()	const	{ return(_numberFiles); }
  DataFile	*CurrentFile() const
	{ return(_numberFiles == 0 ? (DataFile *)NULL : _dataFile[_currentFile]); }

  DataFile	*FirstFile()
	{ return(_numberFiles == 0 ? (DataFile *)NULL : _dataFile[_nextCnt = 0]); }

  DataFile	*NextFile()
	{ return(++_nextCnt == _numberFiles ? (DataFile *)NULL : _dataFile[_nextCnt]); }

private:
  int	_numberFiles;
  int	_currentFile;
  int	_nextCnt;

  DataFile	*_dataFile[MAX_DATAFILES];

};	// END FILEMGR.H

#endif
