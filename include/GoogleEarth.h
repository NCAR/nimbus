/*
-------------------------------------------------------------------------
OBJECT NAME:    GoogleEarthKML.h

DESCRIPTION:    Header File for class to generate GoogleEarth KML file.
                functions.
-------------------------------------------------------------------------
*/

#ifndef _GoogleEarthKML_h_
#define _GoogleEarthKML_h_

#include <string>
#include <iostream>
#include <fstream>

class GoogleEarthKML
{
public:
  GoogleEarthKML(std::string nc_file);
  ~GoogleEarthKML();

  void SetLatestLAT(float lat)	{ _latestLAT = lat; }
  void SetLatestLON(float lon)	{ _latestLON = lon; }
  void SetLatestALT(float alt)	{ _latestALT = alt; }
  void SetLatestTAS(float tas)	{ _latestTAS = tas; }

  void update();

private:
  void createHeader();
  void createFooter();

  std::string startBubbleCDATA(), endBubbleCDATA();

  std::ofstream _outFile;

  std::string _startTime, _endTime;

  float _latestLAT, _latestLON, _latestALT, _latestTAS;

  bool _headerWritten;

  // We don't want to put out every second of data.  Makes for large file.
  // Output every TimeStep.
  size_t _timeCounter;
  static const size_t TimeStep;
};

#endif
