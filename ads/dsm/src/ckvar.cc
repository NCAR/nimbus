/* ckvar.cc
   Test program to check variable calculations.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// System include files.
#include <vxWorks.h>
#include <mv162.h>
#include <bootLib.h>
#include <stdio.h>
#include <taskLib.h>

// Program include files.

// Class include files.
#include <DsmConfig.h>
#include <TapeHeader.h>
#include <SyncVar.h>
#include <Palt.h>
#include <Tasr.h>

// Program functions. 

// Class declarations.
DsmConfig dsm_config;                   // net configuration class
TapeHeader dsm_hdr;
SyncVar psw (dsm_hdr, "PSW");
SyncVar qcr (dsm_hdr, "QCR");
SyncVar ttb (dsm_hdr, "TTB");
Palt palt (psw);
Tasr tasr (qcr, psw, ttb);

// Global variables.
extern BOOT_PARAMS sysBootParams;

ckvar ()
{
// Initialize the network configuration class.
  dsm_config.DsmConfig ();
  dsm_config.selectByHost (sysBootParams.targetName);
  printf ("header file = %s\n", dsm_config.dsmHeaderName());
 
// Initialize the tape header class.
  dsm_hdr.TapeHeader();
  dsm_hdr.readHdr (dsm_config.dsmHeaderName());

// Initialize the variable classes.
  psw.SyncVar (dsm_hdr, "PSW");
  qcr.SyncVar (dsm_hdr, "QCR");
  ttb.SyncVar (dsm_hdr, "TTB");
  palt.Palt (psw);
  tasr.Tasr (qcr, psw, ttb);

// Compute values.
  psw.computeFromSample ((short)2000);
  qcr.computeFromSample ((short)2000);
  ttb.computeFromSample ((short)2000);

  palt.computePalt();
  tasr.computeTasr();

// Print values.
  printf ("psw.volts = %f, value = %f\n", psw.volts(), psw.value());
  printf ("qcr.volts = %f, value = %f\n", qcr.volts(), qcr.value());
  printf ("ttb.volts = %f, value = %f\n", ttb.volts(), ttb.value());
  printf ("palt = %f, tasr = %f\n", palt.palt(), tasr.tasr());
}
/*****************************************************************************/
