/*
-------------------------------------------------------------------------
OBJECT NAME:	DataFile.h

FULL NAME:	Data File Class

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#ifndef DATAFILE_H
#define DATAFILE_H

#ifdef PNG
#include <zlib.h>
#endif

#include <define.h>

#include "Probe.h"
#include <raf/hdrAPI.h>

#include <vector>

/* ADS image record types */
#define ADS_WORD	0x4144
#define HDR_WORD	0x5448
#define SDI_WORD	0x8681
#define AVAPS_WORD	0x4156

#define PMS2DC1		0x4331
#define PMS2DC2		0x4332
// 64 bit Fast 2DC, 25um.
#define PMS2DC4		0x4334
// 64 bit Fast 2DC, 25um.
#define PMS2DC6		0x4336
#define PMS2DG1		0x4731
#define PMS2DG2		0x4732
#define PMS2DH1		0x4831
#define PMS2DH2		0x4832
#define PMS2DP1		0x5031
#define PMS2DP2		0x5032
 
#define PMS2_SIZE	4116
#define PMS2_RECSIZE	(0x8000 / PMS2_SIZE) * PMS2_SIZE

typedef std::vector<Probe *> ProbeList;


/* -------------------------------------------------------------------- */
class ADS_DataFile {

public:
  ADS_DataFile(char fName[]);
  ~ADS_DataFile();

  const std::string &
  FileName() const	{ return _fileName; }

  const char *
  HeaderVersion() const	{ return(_hdr ? _hdr->Version() : (char *)"5"); }

  const std::string &
  ProjectNumber() const	{ return _projectName; }

  const std::string &
  FlightNumber() const	{ return _flightNumber; }

  const std::string &
  FlightDate() const	{ return _flightDate; }

  void	SetPosition(int position);

  int
  GetPosition() const { return(nIndices == 0 ? 0 : 100 * currPhys / nIndices); }

//  int	NextSyncRecord(char buff[]);
  bool	LocatePMS2dRecord(P2d_rec *buff, int h, int m, int s);
  bool	FirstPMS2dRecord(P2d_rec *buff);
  bool	NextPMS2dRecord(P2d_rec *buff);
  bool	PrevPMS2dRecord(P2d_rec *buff);

  int	NextPhysicalRecord(char buff[]);

  void	ToggleSyntheticData();

  bool	isValidProbe(char *pr);

  const ProbeList&
  Probes() const { return _probeList; }

protected:
  enum HeaderType { NoHeader, ADS2, PMS2D };

  typedef struct { long index; short time[3]; } Index;

  void		initADS2();
  void		initADS3();

  long long	ntohll(long long * p) const;

  void		buildIndices(), sort_the_table(int, int), SortIndices(int);
  void		SwapPMS2D(P2d_rec *);
  void		check_rico_half_buff(P2d_rec *buff, size_t start, size_t end);

  std::string	_fileName;

  std::string	_projectName;
  std::string	_flightNumber;
  std::string	_flightDate;

  Header	* _hdr;
#ifdef PNG
  gzFile	gz_fd;
#else
  int		gz_fd;
#endif
  FILE		*fp;
  size_t	savePos;

  ProbeList	_probeList;

  bool		gzipped, useTestRecord;
  HeaderType	_fileHeaderType;

  Index		*indices;
  size_t	currPhys;
  size_t	currLR;
  size_t	nIndices;

  P2d_rec	physRecord[P2DLRPR], *testRecP;

};	/* END DATAFILE.H */

#endif
