/*
-------------------------------------------------------------------------
OBJECT NAME:	mcrtd.cc

FULL NAME:	MCR real-time display.

ENTRY POINTS:	main()

DESCRIPTION:	Data sent by discComm via socket are processed and
		displayed.

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1998-9
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "header.h"

// Class include files.
#include "adsIO.h"
#include "adsIOrt.h"
#include "Application.h"
#include "Canvas.h"
#include "CanvasWindow.h"
#include "DataBuffer.h"
#include "XColorMap.h"
#include "XFonts.h"
#include "XPen.h"

#include "fbr.h"

/* Program functions */

void	netMessage(int, char*, char*);	// network message handler
void	DisplayTS(Mcr_rec *buf), DisplayData(), DisplayColorBar();
void	RealTimeLoop(), PostProcLoop(char s[]);
static bool NextMCRfile(), NextMCRfileRT();

bool	Frozen = false, RealTimeMode = false;
char	buffer[2048], mcrFileName[1024];
Mcr_rec	mcrRec;

Application     *application;
Canvas          *mainCanvas;
CanvasWindow	*canvasWindow;
DataBuffer	*proc;
XColorMap	*colors;
XFonts		*fonts;
XPen		*pen;

static ADS_DataFile	*file;
static ADS_rtFile	*rtFile;


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	argIdx = 1;

  application = new Application("XmMcrtd", &argc, argv, fallback_resources);

  if (argc < 2)
  {
    std::cerr << "Not enough arguments: mcrtd [-rt proj_num] | [file_name]\n";
    exit(1);
  }
 
  if (strcmp(argv[argIdx], "-rt") == 0)
  {
    printf("Entering real-time mode.\n");
    RealTimeMode = true;
    ++argIdx;
  }

  proc = new DataBuffer(argv[argIdx]);


  /* Get X window going.
   */
  canvasWindow = new CanvasWindow(application->Shell());
  canvasWindow->PopUp();
 
  mainCanvas = new Canvas(canvasWindow->DrawingArea());
  colors = new XColorMap(canvasWindow->DrawingArea());
  pen = new XPen(canvasWindow->DrawingArea());
  fonts = new XFonts(application->Shell());

  DisplayColorBar();


  if (RealTimeMode)
    RealTimeLoop();
  else
    PostProcLoop(argv[argIdx]);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void PostProcLoop(char fileName[])
{
  strcpy(mcrFileName, fileName);
  file = new ADS_DataFile(fileName);

  file->FirstMCRRecord(&mcrRec);

  std::cerr << "Enter start time [return for start of file] :";
  fgets(buffer, 1024, stdin);

  if (strlen(buffer) > 5)
    {
    int	startTime, h, m, s;

    if (strchr(buffer, ':'))
      sscanf(buffer, "%d:%d:%d", &h, &m, &s);
    else
      sscanf(buffer, "%02d%02d%02d",  &h, &m, &s);

    startTime = h * 3600 + m * 60 + s;

    while (HdrBlkTimeToSeconds(&mcrRec) != startTime)
      file->NextMCRRecord(&mcrRec);
    }

  while (1)
    {
    if (mcrRec.id[4] - '0' == proc->CurrentChannel())
      {
      proc->CountsToVolts(&mcrRec);
      proc->RecordData(&mcrRec);

      DisplayTS(&mcrRec);

      if (proc->MakeImages())
        {
        DisplayData();
        proc->RemoveFullScans();
        }

      application->FlushEvents();
//      sleep(1);
      }

    while (Frozen)
      {
      application->FlushEvents();
      sleep(1);
      }

    if (file->NextMCRRecord(&mcrRec) == false)
      if (NextMCRfile() == false)
        break;
    }

}	/* END POSTPROCLOOP */

/* -------------------------------------------------------------------- */
void RealTimeLoop()
{
  int		type, len;

  rtFile = new ADS_rtFile();
  rtFile->LastMCRRecord(&mcrRec);


  /* Main loop...
   */
  while (TRUE)
    {
    while (!rtFile->NextMCRRecord(&mcrRec))
      {
      usleep(500000);
      application->FlushEvents();
      }

    if (mcrRec.id[4] - '0' != proc->CurrentChannel())
      continue;

    proc->CountsToVolts(&mcrRec);
    proc->RecordData(&mcrRec);

    DisplayTS(&mcrRec);

    if (proc->MakeImages())
      {
      DisplayData();
      proc->RemoveFullScans();
      }
    }

}	/* END REALTIMELOOP */
 
/* -------------------------------------------------------------------- */
void DisplayTS(Mcr_rec *buf)
{
  int		i;
  XPoint	pts[1000];
  float		y;
  static int	counter = 0;

  if (Frozen || ++counter % 2)	// Every other second
    return;

  mainCanvas->ClearArea(0, 370, 1050, 200);
  pen->SetColor(1);	// Black

  sprintf(buffer, "Channel %c, %02d:%02d:%02d",
	buf->id[4]+1, ntohs(buf->hour), ntohs(buf->minute), ntohs(buf->second));

if (strlen(buffer) > 2048) std::cout << "buff to much\n";

  pen->SetFont(fonts->Font(XFonts::Point14));
  pen->DrawText(mainCanvas->Surface(), 20, 400, buffer);
  pen->DrawText(mainCanvas->Surface(), 2, 480, "Vdc");

  pen->DrawLine(mainCanvas->Surface(), 50, 410, 1050, 410);
  pen->DrawLine(mainCanvas->Surface(), 50, 510, 1050, 510);
  pen->DrawLine(mainCanvas->Surface(), 50, 560, 1050, 560);

  pen->SetFont(fonts->Font(XFonts::Point12));
  pen->DrawText(mainCanvas->Surface(), 5, 415, " 10.0");
  pen->DrawText(mainCanvas->Surface(), 5, 515, "  0.0");
  pen->DrawText(mainCanvas->Surface(), 5, 565, "-10.0");

  char *title = "  t.bb.t             Filter temps              Dewar temps            v.bb.t     peak Vdc";
  pen->DrawText(mainCanvas->Surface(), 300, 385, title);

  sprintf(buffer,"%8.2f    %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f   %8.2f   %8.2f",
	proc->Housekeeping(0), proc->Housekeeping(1), proc->Housekeeping(2),
	proc->Housekeeping(3), proc->Housekeeping(10), proc->Housekeeping(11),
	proc->Housekeeping(12), proc->Housekeeping(13), proc->PeakVoltage());

if (strlen(buffer) > 2048) std::cout << "buff to much\n";
  pen->DrawText(mainCanvas->Surface(), 300, 400, buffer);

  pen->SetColor(colors->pixel[32]);	// Blue

  for (i = 0; i < 1000; ++i)
    {
    pts[i].x = i + 50;

    y = proc->Voltage(i << 1);

    if (y < 0.0)
      y *= 5;
    else
      y *= 10;

    pts[i].y = 510 - (short)y;
    }

  pen->DrawLines(mainCanvas->Surface(), pts, 1000);
  mainCanvas->ExposeAll();

}	/* END DISPLAYTS */

/* -------------------------------------------------------------------- */
void DisplayData()
{
  int	i, j;
  char	*image;

  static int	displayIdx = 0;

  for (i = 0; i < proc->nImages(); ++i)
    {
    image = proc->ImageData(i);

    for (j = 0; j < nSamplesPerScan; ++j)
      {
      pen->SetColor(colors->pixel[image[j]]);
      pen->DrawPoint(mainCanvas->Surface(), displayIdx, 30 + j);
      }

    if (++displayIdx >= 1050)
      displayIdx = 0;
    }

  pen->SetColor(1);
  pen->DrawRectangleFilled(mainCanvas->Surface(), displayIdx, 30, 3, nSamplesPerScan);

  int	midPoint, tenDeg;


  midPoint = 30 + (nSamplesPerScan / 2);
  pen->DrawLine(mainCanvas->Surface(), 0, midPoint, 15, midPoint);

  tenDeg = 360 / 9;	// 360 samples / 90 degrees.

  for (i = 0; i < 4; ++i)
    {
    pen->DrawLine(mainCanvas->Surface(), 0,midPoint+tenDeg, 10,midPoint+tenDeg);
    pen->DrawLine(mainCanvas->Surface(), 0,midPoint-tenDeg, 10,midPoint-tenDeg);

    tenDeg += 360 / 9;
    }

  mainCanvas->ExposeAll();

}	/* END DISPLAYDATA */

/* -------------------------------------------------------------------- */
void netMessage(int action, char *name, char *msg_str)
{
  std::cerr << "mcrtd: %s", msg_str;

}

/* -------------------------------------------------------------------- */
void DisplayColorBar()
{
  int	i;

  /* Draw color map along top of screen.
   */
  for (i = 0; i < 120; ++i)
    {
    pen->SetColor(colors->pixel[i]);
    pen->DrawRectangleFilled(mainCanvas->Surface(), 230+i*6, 0, 6, 15);
 
    if (i % 10 == 0)
      pen->DrawLine(mainCanvas->Surface(), 230+i*6, 10, 230+i*6, 20);
    }
 
  pen->DrawLine(mainCanvas->Surface(), 230+i*6, 0, 230+i*6, 20);

}	/* END DISPLAYCOLORBAR */

/* -------------------------------------------------------------------- */
static bool NextMCRfile()
{ 
  delete file;
  
  ++(mcrFileName[strlen(mcrFileName)-1]);
  
  file = new ADS_DataFile(mcrFileName);
  
  std::cout << "   \nChanging to MCR file " << mcrFileName << ".\n";
  
  return(file->FirstMCRRecord(&mcrRec));

}       /* END NEXTMCRFILE */

/* END MCRTD.CC */
