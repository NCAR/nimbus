/* SyncRecord.h
   Class for building the 1 second synchronous logical records.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef SYNCRECORD_H
#define SYNCRECORD_H

#include <header.h>

#include <Bc635Vme.h>
#include <Counter.h>
#include <DigitalIn.h>
#include <Dpres.h>
#include <DsmConfig.h>
#include <Dsp56002.h>
#include <GpsTans2.h>
#include <GpsTans3.h>
#include <GreyVmeHouse.h>
#include <HwIrs.h>
#include <HwGps.h>
#include <JplTdl.h>
#include <LhTdl.h>
#include <Mcr.h>
#include <Climet.h>
#include <Neph.h>
#include <Ophir3.h>
#include <Ozone.h>
#include <Pms1Vme.h>
#include <Pms2dHouse.h>
#include <PpsGps.h>
#include <RandomBuf.h>
#include <Rdma.h>
#include <SampleTable.h>
#include <Spp.h>
#include <Synchro.h>
#include <TapeHeader.h>
#include <UvHyg.h>
#include <Vm3118.h>

class SyncRecord {
public:
  SyncRecord (TapeHeader& hdr, DsmConfig& dcfg, SampleTable& st,
              Bc635Vme& tp,
              Counter *ctr,
              DigitalIn *di,
              Dpres *dpres[],
              Dsp56002 *dp,
              HwIrs *hwirs[],
              HwGps *hwgps[],
              Mcr *mc,
              Ophir3 *oph3[],
              Pms1Vme *p1v[],
              Pms2dHouse *tdh[],
              GpsTans2 *tan2[],
              GpsTans3 *tan3[],
              PpsGps *gpspps[],
              Spp *serspp[],
              UvHyg *uv[],
              Vm3118 *vm3,
              Synchro *synchro,
              GreyVmeHouse *grayh[],
              JplTdl *jpltdl[],
              LhTdl *lhtdl,
              Rdma *rdma,
              Neph *neph[],
              Climet *climet
              );

  void buildRecord ();				// build the 1 second record
  RandomBuf *buf;				// logical record buffer
 
private:
  void buildHdr();				// build the HDR block
  void buildAnalog();				// build the Analog block
  void buildAnaAux();				// build the Analog aux block
  void buildCounter();				// build the Counters block
  void buildDigitalIn();			// build the Digital inputs blk
  void buildDigitalOut();			// build the Digital outputs blk
  void buildDpres();				// build the Digital pressure 
  void buildGpsTans();				// build the Trimble tans blks
  void buildGreyVmeHouse();			// build the Grey house blocks
  void buildHwIrs();				// build the Honeywell irs blks
  void buildHwGps();				// build the Honeywell gps blks
  void buildJplTdl();				// build the JPlTdl blks
  void buildLhTdl();				// build the JPlTdl blks
  void buildClimet();				// build the Climet block
  void buildNeph();				// build the Neph block
  void buildNoNoy();				// build the NO_NOY blks
  void buildPpsGps();				// build the Honeywell gps blks
  void buildOphir3();				// build the Ophir3 blks
  void buildOzone();				// build the NOAA ozone blks
  void buildPms1Vme();				// build the vme pms 1d blocks
  void buildPms2dHouse();			// build the 2d house blocks
  void buildRdma();				// build the Rdma block
  void buildSynchro();				// build synchro block
  void buildSpp();				// build spp block
  void buildUvHyg();				// build the UV hygrometer blks

  SampleTable &samp_table;			// sample table
  Bc635Vme &tfp;				// time-freq processor intfc
  Counter *counter;				// counters interface
  DigitalIn *digital_in;			// digital inputs interfaces
  Dpres *d_pres[MAX_UVHYG_INTFC];		// digital pressure interface
  Dsp56002 *dsp;				// M56002 dsp board interface
  GpsTans2 *tans2[MAX_TANS_INTFC];		// Trimble Tans I & II intfcs 
  GpsTans3 *tans3[MAX_TANS_INTFC];		// Trimble Tans III intfcs 
  GreyVmeHouse *greyh[MAX_GREYVME_INTFC];  	// pms Grey interfaces
  HwIrs *hw_irs[MAX_HWIRS_INTFC];		// Honeywell irs interfaces 
  HwGps *hw_gps[MAX_HWGPS_INTFC];		// Honeywell irs interfaces 
  PpsGps *pps_gps[MAX_PPS_INTFC];		// Collins PPS GPS interfaces
  Mcr *mcr;					// Mcr interface
  Ophir3 *ophir3[MAX_OPHIR3_INTFC];		// Ophir3 interfaces 
  Pms1Vme *pms1v[MAX_PMS1VME_INTFC];		// pms 1d interfaces
  Pms2dHouse *pms2dh[MAX_PMSVME2D_INTFC];  	// pms 2d interfaces
  Synchro *synchro;				// Synchro interface
  Spp *spp[MAX_SPP_INTFC];			// Spp interfaces
  UvHyg *uvh[MAX_UVHYG_INTFC];			// UV hygrometer interfaces
  Vm3118 *vm3118;				// analog auxiliary input intfc
  JplTdl *jpl_tdl[MAX_UVHYG_INTFC];		// Laser Hygrometer
  LhTdl *lh_tdl;				// Small laser hygrometer
  Climet *climet_cn;				// Climet
  Neph *nephelometer[MAX_UVHYG_INTFC];				// Neph
  Rdma *radial_dma;				// Rdma

  Hdr_blk hdr_blk;				// HDR block struct
  int lr_len;					// logical record length
};

#endif

