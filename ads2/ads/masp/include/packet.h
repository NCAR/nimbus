/* packet.h -- This file contains all output data packet structures

   Original Author:  Keith S. Barr
   Copyright 1994
   National Center for Atmospheric Research
   Boulder, Colorado

   Written in support of ASHOE/MAESA

   Revision History:
      07/20/1994 ksb Created
      09/27/1995 ksb Removed FCAS code in prep for DLR
*/

#ifndef PACKET_H
#define PACKET_H

#define MASP_PKT_SZ	1020

struct DAS_HK_PACKET {
  unsigned short sync_flag; 	/* sync word				*/
  unsigned short length;	/* the packet length			*/
  unsigned short type;		/* the packet type (DH)			*/
  unsigned short packets;	/* no. of packets since last hk		*/
  unsigned long  time;		/* the seconds of the year		*/
  unsigned short ch_calc;	/* no. of checksum errors calculated	*/
  unsigned short corrupt;	/* no. of corrupt packets since hk	*/
  unsigned short ch_read;	/* masp: checksum errors read		*/
  unsigned short b_calb;	/* masp: blocks trying to send calb	*/
  unsigned short b_time;	/* masp: blocks trying to send time	*/
  unsigned short spare1;	
  unsigned short spare2;
  unsigned short write_time;	/* no. of ticks to write last hk pkt.	*/
  unsigned short xons_sent;	/* no. of XONs sent to MASP		*/
  unsigned short tas;		/* True airspeed sent to probes		*/
  unsigned short masp_trig;	/* masp trigger threshold		*/
  unsigned short masp_xsit;	/* masp minimum transit time		*/
  unsigned short masp_max;	/* max number of masp data packets	*/
  unsigned short masp_maGain;	/* DOF Rejection Gain, mask		*/
  unsigned short masp_tsGain;	/* DOF Rejection Gain, total		*/
  unsigned short spare3;
  unsigned short spare4;
  unsigned short adc[16];	/* uncalibrated A to D channels		*/
  unsigned short cksum;		/* checksum				*/
};

struct FALCON_NAV_PACKET {
  unsigned char flight[5];	/* 1234_ flight number			*/
  unsigned char date[9];	/* DD.MM.YY_ date			*/
  unsigned char time[13];	/* HH:MM:SS.SEC_ time			*/
  unsigned char lat[14];	/* +2.456789e+23_ GPS Latitude		*/
  unsigned char lon[14];	/* +2.456789e+23_ GPS Longitude		*/
  unsigned char pStatic[14];	/* +2.456789e+23_ Static Pressure	*/
  unsigned char pAlt[14];	/* +2.456789e+23_ Pressure Altitude	*/
  unsigned char temp[14];	/* +2.456789e+23_ Temperature		*/
  unsigned char tas[14];	/* +2.456789e+23_ True Airspeed		*/
  unsigned char hdg[14];	/* +2.456789e+23_ Heading		*/
  unsigned char wVel[14];	/* +2.456789e+23_ Wind Velocity		*/
  unsigned char wDir[14];	/* +2.456789e+23_ Wind Direction	*/
  unsigned char pitch[14];	/* +2.456789e+23_ Pitch			*/
  unsigned char roll[14];	/* +2.456789e+23_ Roll			*/
  unsigned char cksum[3];	/* 123 Checksum				*/

  unsigned long our_time;	/* MASP time in seconds of the year	*/
};

struct C130_NAV_PACKET {
  unsigned char time[9];	/* hh:mm:ss_ time			*/
  unsigned char lat[14];	/* +2.456789e+12_ GPS Latitude		*/
  unsigned char lon[14];	/* +2.456789e+12_ GPS Longitude		*/
  unsigned char pStatic[14];	/* +2.456789e+12_ Static Pressure	*/
  unsigned char pCabin[14];	/* +2.456789e+12_ Cabin Pressure	*/
  unsigned char pAlt[14];	/* +2.456789e+12_ Pressure Altitude	*/
  unsigned char temp[14];	/* +2.456789e+12_ Temperature		*/
  unsigned char dewP[14];	/* +2.456789e+12_ Dew Point		*/
  unsigned char wVel[14];	/* +2.456789e+12_ Wind Velocity		*/
  unsigned char wDir[14];	/* +2.456789e+12_ Wind Direction	*/
  unsigned char wVert[14];	/* +2.456789e+12_ Vertical Wind		*/
  unsigned char mix[14];	/* +2.456789e+12_ Mixing Ratio		*/
  unsigned char hdg[14];	/* +2.456789e+12_ Heading		*/
  unsigned char tas[14];	/* +2.456789e+12_ True Airspeed		*/
  unsigned char tSurf[14];	/* +2.456789e+12_ Surface Temperature	*/
  unsigned char pitch[14];	/* +2.456789e+12_ Pitch			*/
  unsigned char roll[14];	/* +2.456789e+12_ Roll			*/
  unsigned char lwc[14];	/* +2.456789e+12 Liquid Water Content	*/
  unsigned char spare[1];	/* _ place holder to fill out word	*/
	
  unsigned long  our_time;	/* our time in seconds of the year	*/
};


struct DC8_NAV_PACKET {
  unsigned char typeC[2];    /* C_ */
  unsigned char day[4];      /* aaa_ Julian Day */
  unsigned char time[13];    /* bb:bb:bb.bbb_ hh:mm::ss.sss UT */
  unsigned char lat[9];      /* +cc cc.c_ deg min */
  unsigned char lon[10];     /* +ddd dd.d_ deg min */
  unsigned char pitch[6];    /* eee.e_ deg */
  unsigned char roll[7];     /* ffff.f_ deg */
  unsigned char windV[3];    /* ggg knots */
  unsigned char tailC[2];    /* cr/lf */

  unsigned char typeD[2];    /* D_ */
  unsigned char windD[4];    /* hhh_ deg */
  unsigned char tas[4];      /* iii_ knots */
  unsigned char gs[5];       /* jjjj_ knots */
  unsigned char hdg[6];      /* kkk.k_ deg */
  unsigned char drift[6];    /* lll.l_ deg */
  unsigned char palt[6];     /* mmmmm_ feet */
  unsigned char ralt[6];     /* nnnnn_ feet */
  unsigned char dptGE[8];    /* ooooo.o_ C */
  unsigned char dptEGG[7];   /* ppppp.p C */
  unsigned char tailD[2];    /* cr/lf */

  unsigned char typeE[2];    /* E_ */
  unsigned char temp[6];     /* qqq.q_ C */
  unsigned char tat[6];      /* rrr.r_ C */
  unsigned char irt[6];      /* sss.s_ C */
  unsigned char tcalc[6];    /* ttt.t_ C */
  unsigned char cas[4];      /* uuu_ knots */
  unsigned char vs[7];       /* vvvvvv_ ft/min */
  unsigned char dtg[8];      /* wwwww.w_ nmi */
  unsigned char ttg[7];      /* xxxx.x_ min */
  unsigned char align[2];    /* yy */
  unsigned char tailE[2];    /* cr/lf */

  unsigned char typeF[2];    /* F_ */
  unsigned char cabin[6];    /* zzzzz_ feet */
  unsigned char press[7];    /* JJJJ.J_ mbar */
  unsigned char mach[6];     /* K.KKK_ mach number */
  unsigned char ctd[8];      /* LLLLL.L_ nmi */
  unsigned char track[7];    /* MMMM.M_ deg */
  unsigned char tkerr[7];    /* NNNN.N_ deg */
  unsigned char tkang[6];    /* OOO.O_ deg */
  unsigned char sphum[5];    /* P.PPP gH2O/kg air */
  unsigned char tailF[2];    /* cr/lf */

  unsigned char typeG[2];    /* G_ */
  unsigned char pph2o[5];    /* QQ.Q_ mbar */
  unsigned char rhice[5];    /* RR.R_ % */
  unsigned char rhh2o[5];    /* SS.S_ % */
  unsigned char svph2o[6];   /* TT.TT_ mbar */
  unsigned char svpice[6];   /* UU.UU_ mbar */
  unsigned char segrnd[6];   /* VVV.V_ deg */
  unsigned char seacft[6];   /* WWW.W_ deg */
  unsigned char sagrnd[6];   /* XXX.X_ deg */
  unsigned char saacft[7];   /* YYYY.Y_ deg */
  unsigned char tailG[2];    /* cr/lf */

  unsigned char typeH[2];    /* H_ */
  unsigned char gpsUT[13];   /* aa:aa:aa.aaa_ hh:mm:ss.sss UT */
  unsigned char gpslat[9];   /* +bb bb.b_ deg/min */
  unsigned char gpslon[10];  /* +ccc cc.c_ deg/min */
  unsigned char gpsalt[6];   /* ddddd_ feet */
  unsigned char theta[6];    /* eee.e_ */
  unsigned char spareH[8];   /* spaces */
  unsigned char tailH[2];    /* cr/lf */

  unsigned char typeI[2];    /* I_ */
  unsigned char megrnd[6];   /* eee.e_ deg */
  unsigned char meacft[6];   /* fff.f_ deg */
  unsigned char magrnd[6];   /* ggg.g_ deg */
  unsigned char maacft[7];   /* hhhh.h_ deg */
  unsigned char tfFlag[2];   /* i_ 1=from, 2=to */
  unsigned char wayLat[9];   /* +jj jj.j_ deg/min */
  unsigned char wayLon[9];   /* +kkk kk.k deg/min */
  unsigned char spareI[7];   /* spaces */
  unsigned char tailI[2];    /* cr/lf */

  unsigned long  our_time;   /* our time in seconds of the year */
};


	
struct ER2_NAV_PACKET {
  unsigned char ourSeconds[9];	// 31622400_ max seconds
  unsigned char navDat[511];	// spaces following good data
  unsigned char tail[2]; 	// cr/lf
};


struct MASP_HK_PKT {
  unsigned short syncFlag;	// packet sync flag, 0x4450
  unsigned short length;	// packet length, 73 words, 0x0049
  unsigned short type;		// packet type, 0x1
  unsigned short missed;	// missed particles, throttling
  unsigned long time;		// seconds of the year
  unsigned short probeMode;	// bitmap of probe mode
  unsigned short dspHeat;	// bitmap of heater status
  short dac0Spare;		// DAC 0-Unused
  short dac1Trigger;		// DAC 1-Trigger Threshold
  short dac2VCAGain;		// DAC 2-VCA Gain
  short dac3CalAmp;		// DAC 3-Calibration Pulse Amplitude
  short dac4TSTec;		// DAC 4-Total Scatter TEC Voltage
  short dac5MATec;		// DAC 5-Masked Aperature TEC Voltage
  short dac6FSTec;		// DAC 6-Forward Scatter TEC Voltage
  short dac7BSTec;		// DAC 7-Backward Scatter TEC Voltage
  short mlTemp;			// Main Laser Temperature
  short tsTempA;		// Total Scatter Temperature
  short tsDCVA;			// Total Scatter DC detector voltage
  short spare20;		// Spare Analog Input
  short q1uAmp;			// Quad Detector 1 uA
  short q2uAmp;			// Quad Detector 2 uA
  short q3uAmp;			// Quad Detector 3 uA
  short q4uAmp;			// Quad Detector 4 uA
  short pAbs;			// Absolute Pressure
  short pDiff;			// Differential Pressure
  short mlPower;		// Main Laser Power
  short spare28;		// Spare Analog input
  short psP28;			// +28 Volt Power Supply Voltage
  short psP5;			// +5 Volt Power Supply Voltage
  short psP12;			// +12 Volt Power Supply Voltage
  short psM12;			// -12 Volt Power Supply Voltage
  short clTemp;			// Cal Laser Temperature
  short maTempA;		// Masked Aperature Temperature
  short maDCVA;			// Masked Aperature DC detector Voltage
  short fobTemp;		// Forward Optical Bench Temperature
  short fsTempA;		// Forward Scatter Temperature
  short fsDCVA;			// Forward Scatter DC detector Voltage
  short aobTemp;		// Aft Optical Bench Temperature
  short bsTempA;		// Back Scatter Temperature
  short bsDCVA;			// Back Scatter DC detector Voltage
  short spare42;		// Spare temperature input
  short tsTempB;		// Total Scatter Temperature
  short apsM5;			// -5 Volt Analog Supply Voltage
  short qTemp;			// Quad Detector Temperature
  short maTempB;		// Masked Aperature Temperature
  short apsP5;			// +5 Volt Analog Supply Voltage
  short absTemp;		// Absolute Pressure Sensor Temperature
  short fsTempB;		// Forward Scatter Temperature
  short spare50;		// Spare Analog Input Voltage
  short diffTemp;		// Diff. Pressure Sensor Temperature
  short bsTempB;		// Back Scatter Temperature
  short acanTemp;		// Aft Can Air Temperature
  short tsTempC;		// Total Scatter Temperature
  short tsDCVB;			// Total Scatter DC detector Voltage
  short dspcTemp;		// DSP Card Temperature
  short maTempC;		// Masked Aperature Temperature
  short maDCVB;			// Masked Aperature DC detector Voltage
  short pccTemp;		// Pulse Converter Card Temperature
  short fsTempC;		// Forward Scatter Temperature
  short fsDCVB;			// Forward Scatter DC detector Voltage
  short sicTemp;		// Sampling Integrator Card Temperature
  short bsTempC;		// Back Scatter Temperature
  short bsDCVB;			// Back Scatter DC detector Voltage
  short scTemp;			// Spare Card Temperature
  short tsTempD;		// Total Scatter Temperature
  short hkcTemp;		// Housekeeping Card Temperature
  short maTempD;		// Masked Aperature Temperature
  short fcanTemp;		// Forward Can Air Temperature
  short fsTempD;		// Forward Scatter Temperature
  short psTemp;			// Power Supply Temperature
  short bsTempD;		// Back Scatter Temperature
  unsigned short checksum;	// packet checksum
};


struct MASP_PART {
  unsigned short ttimer;	/* # of 20nsec periods +1 in beam	*/
  unsigned short itimer;	/* # of 20.48 usec since particle	*/
  unsigned short total;		/* total scatter...3 to 1 compression	*/
  unsigned short masked;	/* masked aperature...3 to 1 comp	*/
  unsigned short forward;	/* forward scatter...2 to 1 comp	*/
  unsigned short backward;	/* backward scatter...2 to 1 comp	*/
  unsigned short bfratio;	/* BS/FS low gain if hi 2 to 1		*/
};

struct MASP_PART_PKT {
  unsigned short sync_flag;	/* sync flag 0x4450			*/
  unsigned short length;	/* packet length...1020			*/
  unsigned short type;		/* type now = 0x2			*/
  unsigned short fract;		/* # of 15.2588 micro sec of first	*/
  unsigned long  time;		/* seconds of the year			*/
  unsigned short gain;		/* gain of the pulse integrators	*/
  unsigned short status;	/* probe status 1=calibration		*/
  unsigned short adc;		/* ADC offset				*/
  unsigned short rejected;	/* # of rejected particles		*/
  unsigned short dofReject;	/* # of rejected particles outside of DOF */
  struct MASP_PART part[144];	/* particle data			*/
  unsigned short cksum;		/* first 1019 checksum			*/
};

struct MASP_XON_PKT {
  unsigned short sync_flag;
  unsigned short length;
  unsigned short xon;
  unsigned short checksum;
};

#endif
