/*
-------------------------------------------------------------------------
OBJECT NAME:    spefm.c

FULL NAME:      Sail Plane Electric Field Mill

ENTRY POINTS:   EFinit()

STATIC FNS:  

DESCRIPTION:    Template for AMLIB std & user directories.

REFERENCES:  

REFERENCED BY:  compute.c

COPYRIGHT:  University Corporation for Atmospheric Research, 1995, 1998
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE  *vft0, *vft1, *vft2, *vfb0, *vfb1, *vfb2,
    *vfp0, *vfp1, *vfs0, *vfs1, *vfs2,
    *vnt0, *vnt1, *vnb0, *vnb1, *vnp0, *vnp1, *vns0, *vns1;

/*  Values from /home/local/proj/965/Defaults on 30 April 1998  RLR  */
static NR_TYPE
       VFT0[2] = {1.8678523, 0.7281622},
       VFT1[5] = {1.2758189, 1.6866612, -0.5817351, 0.1557366, -0.0153861},
       VFT2[5] = {-0.3178759, 11.1196213, -21.0850239, 19.4571877, -6.6446362},

       VFB0[2] = {2.3236587, 0.6342919},
       VFB1[5] = {1.6050974, 2.208256, -1.4576938, 0.6427022, -0.1087554},
       VFB2[6] = {0.5232773, 15.1426353, -62.0584412, 137.305481, -147.812561, 61.3088837},

       VFP0[2] = {1.997771, 0.7001574},
       VFP1[5] = {1.5428402, 1.8446898, -1.2446543, 0.6440516, -0.125748},

       VFS0[2] = {2.4673603, 0.6006902},
       VFS1[6] = {1.3857181, 5.7496543, -12.0287008, 14.6949568, -8.7708292, 2.0107875},
       VFS2[4] = {-0.7455, 95.03968, -1282.47888, 5837.75244},

       VNT0[2] = {-1.21, 175.78},
       VNT1[2] = {-2.87, 2.97},

       VNB0[2] = {-1.14, 182.73},
       VNB1[2] = {-2.84, 2.99},

       VNP0[2] = {-1.10, 178.2},
       VNP1[2] = {-2.88, 2.97},
 
       VNS0[2] = {-1.11, 174.4},
       VNS1[2] = {-2.81, 2.96},
 
       RFZ = 0.74,
       RFY = 1.93,
       RNZ = 0.962,
       RNY = 1.040,
 
       CFZ = 0.29,
       CFY = 0.29,
       CNZ = 0.56,
       CNY = 0.56,
 
       RXZ = 1.0,
       RXY = 1.0,
       CXZ = -0.025,
       CXY = -0.025;

/* -------------------------------------------------------------------- */
void EFinit(DERTBL *varp)
{
  NR_TYPE  *tmp;
  if ((tmp = GetDefaultsValue("VFT0", varp->name)) == NULL)
  {
	vft0 = VFT0;
    sprintf(buffer, "Values set to %f, %f in AMLIB function EFinit.\n", vft0[0], vft0[1]);
    LogMessage(buffer);
  }
  else
    vft0 = tmp;
/*  vft0 = GetDefaultsValue("VFT0", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFT1", varp->name)) == NULL)
  {
	vft1 = VFT1;
    sprintf(buffer, "Values set to %f, %f, %f, %f, %f in AMLIB function EFinit.\n", vft1[0], vft1[1], vft1[2], vft1[3], vft1[4]);
    LogMessage(buffer);
  }
  else
    vft1 = tmp;
/*  vft1 = GetDefaultsValue("VFT1", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFT2", varp->name)) == NULL)
  {
	vft2 = VFT2;
    sprintf(buffer, "Values set to %f, %f, %f, %f, %f in AMLIB function EFinit.\n", vft2[0], vft2[1], vft2[2], vft2[3], vft2[4]);
    LogMessage(buffer);
  }
  else
    vft2 = tmp;
/*  vft2 = GetDefaultsValue("VFT2", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFB0", varp->name)) == NULL)
  {
	vfb0 = VFB0;
    sprintf(buffer, "Values set to %f, %f in AMLIB function EFinit.\n", vfb0[0], vfb0[1]);
    LogMessage(buffer);
  }
  else
    vfb0 = tmp;
/*  vfb0 = GetDefaultsValue("VFB0", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFB1", varp->name)) == NULL)
  {
	vfb1 = VFB1;
    sprintf(buffer, "Values set to %f, %f, %f, %f, %f in AMLIB function EFinit.\n", vfb1[0], vfb1[1], vfb1[2], vfb1[3], vfb1[4]);
    LogMessage(buffer);
  }
  else
    vfb1 = tmp;
/*  vfb1 = GetDefaultsValue("VFB1", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFB2", varp->name)) == NULL)
  {
	vfb2 = VFB2;
    sprintf(buffer, "Values set to %f, %f, %f, %f, %f, %f in AMLIB function EFinit.\n", vfb2[0], vfb2[1], vfb2[2], vfb2[3], vfb2[4], vfb2[5]);
    LogMessage(buffer);
  }
  else
    vfb2 = tmp;
/*  vfb2 = GetDefaultsValue("VFB2", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFP0", varp->name)) == NULL)
  {
	vfp0 = VFP0;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vfp0[0], vfp0[1]);
    LogMessage(buffer);
  }
  else
    vfp0 = tmp;
/*  vfp0 = GetDefaultsValue("VFP0", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFP1", varp->name)) == NULL)
  {
	vfp1 = VFP1;
    sprintf(buffer, "Values set to %f, %f, %f, %f, %f in AMLIB function EFinit.\n", vfp1[0], vfp1[1], vfp1[2], vfp1[3], vfp1[4]);
    LogMessage(buffer);
  }
  else
    vfp1 = tmp;
/*  vfp1 = GetDefaultsValue("VFP1", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFS0", varp->name)) == NULL)
  {
	vfs0 = VFS0;
    sprintf(buffer, "Values set to %f, %f in AMLIB function EFinit.\n", vfs0[0], vfs0[1]);
    LogMessage(buffer);
  }
  else
    vfs0 = tmp;
/*  vfs0 = GetDefaultsValue("VFS0", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFS1", varp->name)) == NULL)
  {
	vfs1 = VFS1;
    sprintf(buffer, "Values set to %f, %f, %f, %f, %f, %f, in AMLIB function EFinit.\n", vfs1[0], vfs1[1], vfs1[2], vfs1[3], vfs1[4], vfs1[5]);
    LogMessage(buffer);
  }
  else
    vfs1 = tmp;
/*  vfs1 = GetDefaultsValue("VFS1", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VFS2", varp->name)) == NULL)
  {
	vfs2 = VFS2;
    sprintf(buffer, "Values set to %f, %f, %f, %f, in AMLIB function EFinit.\n", vfs2[0], vfs2[1], vfs2[2], vfs2[3]);
    LogMessage(buffer);
  }
  else
    vfs2 = tmp;
/*  vfs2 = GetDefaultsValue("VFS2", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VNT0", varp->name)) == NULL)
  {
	vnt0 = VNT0;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vnt0[0], vnt0[1]);
    LogMessage(buffer);
  }
  else
    vnt0 = tmp;
/*  vnt0 = GetDefaultsValue("VNT0", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VNT1", varp->name)) == NULL)
  {
    vnt1 = VNT1;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vnt1[0], vnt1[1]);
    LogMessage(buffer);
  }
  else
    vnt1 = tmp;
/*  vnt1 = GetDefaultsValue("VNT1", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VNB0", varp->name)) == NULL)
  {
    vnb0 = VNB0;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vnb0[0], vnb0[1]);
    LogMessage(buffer);
  }
  else
    vnb0 = tmp;
/*  vnb0 = GetDefaultsValue("VNB0", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VNB1", varp->name)) == NULL)
  {
    vnb1 = VNB1;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vnb1[0], vnb1[1]);
    LogMessage(buffer);
  }
  else
    vnb1 = tmp;
/*  vnb1 = GetDefaultsValue("VNB1", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VNP0", varp->name)) == NULL)
  {
    vnp0 = VNP0;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vnp0[0], vnp0[1]);
    LogMessage(buffer);
  }
  else
    vnp0 = tmp;
/*  vnp0 = GetDefaultsValue("VNP0", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("VNP1", varp->name)) == NULL)
  {
    vnp1 = VNP1;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vnp1[0], vnp1[1]);
    LogMessage(buffer);
  }
  else
    vnp1 = tmp;
/*  vnp1 = GetDefaultsValue("VNP1", varp->name)  <-- original code */;

  if ((tmp = GetDefaultsValue("VNS0", varp->name)) == NULL)
  {
    vns0 = VNS0;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vns0[0], vns0[1]);
    LogMessage(buffer);
  }
  else
    vns0 = tmp;
/*  vns0 = GetDefaultsValue("VNS0", varp->name)  <-- original code */;

  if ((tmp = GetDefaultsValue("VNS1", varp->name)) == NULL)
  {
    vns1 = VNS1;
    sprintf(buffer, "Values set to %f, %f, in AMLIB function EFinit.\n", vns1[0], vns1[1]);
    LogMessage(buffer);
  }
  else
    vns1 = tmp;
/*  vns1 = GetDefaultsValue("VNS1", varp->name);  <-- original code */

  if ((tmp = GetDefaultsValue("RFZ", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", RFZ);
    LogMessage(buffer);
  }
  else
    RFZ = tmp[0];
/*  RFZ = (GetDefaultsValue("RFZ", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("RFY", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", RFY);
    LogMessage(buffer);
  }
  else
    RFY = tmp[0];
/*  RFY = (GetDefaultsValue("RFY", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("RNZ", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", RNZ);
    LogMessage(buffer);
  }
  else
    RNZ = tmp[0];
/*  RNZ = (GetDefaultsValue("RNZ", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("RNY", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", RNY);
    LogMessage(buffer);
  }
  else
    RNY = tmp[0];
/*  RNY = (GetDefaultsValue("RNY", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("CFZ", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", CFZ);
    LogMessage(buffer);
  }
  else
    CFZ = tmp[0];
/*  CFZ = (GetDefaultsValue("CFZ", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("CFY", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", CFY);
    LogMessage(buffer);
  }
  else
    CFY = tmp[0];
/*  CFY = (GetDefaultsValue("CFY", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("CNZ", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", CNZ);
    LogMessage(buffer);
  }
  else
    CNZ = tmp[0];
/*  CNZ = (GetDefaultsValue("CNZ", varp->name))[0]  <-- original code */;

  if ((tmp = GetDefaultsValue("CNY", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", CNY);
    LogMessage(buffer);
  }
  else
    CNY = tmp[0];
/*  CNY = (GetDefaultsValue("CNY", varp->name))[0]  <-- original code */;

  if ((tmp = GetDefaultsValue("RXZ", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", RXZ);
    LogMessage(buffer);
  }
  else
    RXZ = tmp[0];
/*  RXZ = (GetDefaultsValue("RXZ", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("RXY", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", RXY);
    LogMessage(buffer);
  }
  else
    RXY = tmp[0];
/*  RXY = (GetDefaultsValue("RXY", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("CXZ", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", CXZ);
    LogMessage(buffer);
  }
  else
    CXZ = tmp[0];
/*  CXZ = (GetDefaultsValue("CXZ", varp->name))[0];  <-- original code */

  if ((tmp = GetDefaultsValue("CXY", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, in AMLIB function EFinit.\n", CXY);
    LogMessage(buffer);
  }
  else
    CXY = tmp[0];
/*  CXY = (GetDefaultsValue("CXY", varp->name))[0];  <-- original code */

}  /* END EFINIT */

/* -------------------------------------------------------------------- */
void sefeft(DERTBL *varp)
{
  NR_TYPE  vft, at, eft;
  int  sign;

  vft  = GetSample(varp, 0);
  sign  = (int)GetSample(varp, 1);

  sign = (sign) ? -1 : 1;

  if (vft > 3.134)
    at = vft0[0] + vft0[1] * vft;
  else
  if (vft > 0.95)
    at = vft1[0] + vft1[1] * vft +
    vft1[2] * pow((double)vft, (double)2.0) +
    vft1[3] * pow((double)vft, (double)3.0) +
    vft1[4] * pow((double)vft, (double)4.0); 
  else
    at = vft2[0] + vft2[1] * vft +
    vft2[2] * pow((double)vft, (double)2.0) +
    vft2[3] * pow((double)vft, (double)3.0) +  
    vft2[4] * pow((double)vft, (double)4.0); 

  eft = pow((double)10.0, (double)at) / 1000.0 * sign;

  PutSample(varp, eft);

}  /* END SEFEFT */

/* -------------------------------------------------------------------- */
void sefefb(DERTBL *varp)
{
  NR_TYPE  vfb, at, efb;
  int  sign;

  vfb  = GetSample(varp, 0);
  sign  = (int)GetSample(varp, 1);

  sign = (sign) ? -1 : 1;

  if (vfb > 1.895)
    at = vfb0[0] + vfb0[1] * vfb;
  else
  if (vfb > 0.352)
    at = vfb1[0] + vfb1[1] * vfb +
    vfb1[2] * pow((double)vfb, (double)2.0) +
    vfb1[3] * pow((double)vfb, (double)3.0) +
    vfb1[4] * pow((double)vfb, (double)4.0); 
  else
    at = vfb2[0] + vfb2[1] * vfb +
    vfb2[2] * pow((double)vfb, (double)2.0) +
    vfb2[3] * pow((double)vfb, (double)3.0) +   
    vfb2[4] * pow((double)vfb, (double)4.0) +
    vfb2[5] * pow((double)vfb, (double)5.0); 
  
  efb = pow((double)10.0, (double)at) / 1000.0 * sign;

  PutSample(varp, efb);

}  /* END SEFEFB */

/* -------------------------------------------------------------------- */
void sefefp(DERTBL *varp)
{
  NR_TYPE  vfp, at, efp;
  int  sign;

  
  vfp  = GetSample(varp, 0);
  sign  = (int)GetSample(varp, 1);

  sign = (sign) ? -1 : 1;

  if (vfp > 1.747)
    at = vfp0[0] + vfp0[1] * vfp;
  else
    at = vfp1[0] + vfp1[1] * vfp +
    vfp1[2] * pow((double)vfp, (double)2.0) +
    vfp1[3] * pow((double)vfp, (double)3.0) +
    vfp1[4] * pow((double)vfp, (double)4.0); 

  efp = pow((double)10.0, (double)at) / 1000.0 * sign;

  PutSample(varp, efp);

}  /* END SEFEFP */

/* -------------------------------------------------------------------- */
void sefefs(DERTBL *varp)
{
  NR_TYPE  vfs, at, efs;
  int  sign;

  vfs  = GetSample(varp, 0);
  sign  = (int)GetSample(varp, 1);

  sign = (sign) ? -1 : 1;

  if (vfs > 1.43)
    at = vfs0[0] + vfs0[1] * vfs;
  else
  if (vfs > 0.093)
    at = vfs1[0] + vfs1[1] * vfs +
    vfs1[2] * pow((double)vfs, (double)2.0) +
    vfs1[3] * pow((double)vfs, (double)3.0) +
    vfs1[4] * pow((double)vfs, (double)4.0) +
    vfs1[5] * pow((double)vfs, (double)5.0);
  else
    at = vfs2[0] + vfs2[1] * vfs +
    vfs2[2] * pow((double)vfs, (double)2.0) +
    vfs2[3] * pow((double)vfs, (double)3.0);

  efs = pow((double)10.0, (double)at) / 1000.0 * sign;

  PutSample(varp, efs);

}  /* END SEFEFS */

/* -------------------------------------------------------------------- */
void sefent(DERTBL *varp)
{
  NR_TYPE  vnt, ent;
  int  Zrange;

  vnt  = GetSample(varp, 0);
  Zrange  = (int)GetSample(varp, 1) & 0x0004;

/*  if (Zrange)  */
  if (Zrange > 0.5)  
    ent = (vnt - vnt0[0]) * vnt0[1];
  else
    ent = (vnt - vnt1[0]) * vnt1[1];

  PutSample(varp, ent);

}  /* END SEFENT */

/* -------------------------------------------------------------------- */
void sefenb(DERTBL *varp)
{
  NR_TYPE  vnb, enb;
  int  Zrange;

  vnb  = GetSample(varp, 0);
  Zrange  = (int)GetSample(varp, 1) & 0x0004;

  /*  if (Zrange)  */ 
  if (Zrange > 0.5) 
    enb = (vnb - vnb0[0]) * vnb0[1];
  else
    enb = (vnb - vnb1[0]) * vnb1[1];

  PutSample(varp, enb);

}  /* END SEFENB */

/* -------------------------------------------------------------------- */
void sefenp(DERTBL *varp)
{
  NR_TYPE  vnp, enp;
  int  Yrange;

  vnp  = GetSample(varp, 0);
  Yrange  = (int)GetSample(varp, 1) & 0x0002;

  /* if (Yrange)   */  
  if (Yrange > 0.5) 
    enp = (vnp - vnp0[0]) * vnp0[1];
  else
    enp = (vnp - vnp1[0]) * vnp1[1];

  PutSample(varp, enp);

}  /* END SEFENP */

/* -------------------------------------------------------------------- */
void sefens(DERTBL *varp)
{
  NR_TYPE  vns, ens;
  int  Yrange;

  vns  = GetSample(varp, 0);
  Yrange  = (int)GetSample(varp, 1) & 0x0002;

  /* if (Yrange)   */  
  if (Yrange > 0.5) 
    ens = (vns - vns0[0]) * vns0[1];
  else
    ens = (vns - vns1[0]) * vns1[1];

  PutSample(varp, ens);

}  /* END SEFENS */

/* -------------------------------------------------------------------- */
void sefefqz(DERTBL *varp)
{
  NR_TYPE  eft, efb;

  eft  = GetSample(varp, 0);
  efb  = GetSample(varp, 1);

  PutSample(varp, 0.5 * (eft + efb));

}  /* END SEFEFQZ */

/* -------------------------------------------------------------------- */
void sefefqy(DERTBL *varp)
{
  NR_TYPE  efp, efs;

  efp  = GetSample(varp, 0);
  efs  = GetSample(varp, 1);

  PutSample(varp, 0.5 * (efp + efs));

}  /* END SEFEFQY */

/* -------------------------------------------------------------------- */
void sefenqz(DERTBL *varp)
{
  NR_TYPE  ent, enb;

  ent  = GetSample(varp, 0);
  enb  = GetSample(varp, 1);

  PutSample(varp, 0.5 * (ent + enb));

}  /* END SEFENQZ */

/* -------------------------------------------------------------------- */
void sefenqy(DERTBL *varp)
{
  NR_TYPE  enp, ens;

  enp  = GetSample(varp, 0);
  ens  = GetSample(varp, 1);

  PutSample(varp, 0.5 * (enp + ens));

}  /* END SEFENQY */

/* -------------------------------------------------------------------- */
void sefexy(DERTBL *varp)
{
  NR_TYPE  efqy, enqy;

  efqy  = GetSample(varp, 0);
  enqy  = GetSample(varp, 1);

  PutSample(varp, CXY * (efqy - RXY * enqy));

}  /* END SEFEXY */

/* -------------------------------------------------------------------- */
void sefexz(DERTBL *varp)
{
  NR_TYPE  efqz, enqz;

  efqz  = GetSample(varp, 0);
  enqz  = GetSample(varp, 1);

  PutSample(varp, CXZ * (efqz - RXZ * enqz));

}  /* END SEFEXZ */

/* -------------------------------------------------------------------- */
static NR_TYPE  ezo, efz, efy, enz, eny;

void sefeyo(DERTBL *varp)
{
  NR_TYPE  roll, eft, efb, efp, efs, ent, enb, enp, ens, fy, fz, ny,
    nz, sbank, cbank, eyo, vyo, vzo;

  roll  = GetSample(varp, 0) * DEG_RAD;
  vyo  = GetSample(varp, 1);
  vzo  = GetSample(varp, 2);
  eft  = GetSample(varp, 3);
  efb  = GetSample(varp, 4);
  efp  = GetSample(varp, 5);
  efs  = GetSample(varp, 6);
  ent  = GetSample(varp, 7);
  enb  = GetSample(varp, 8);
  enp  = GetSample(varp, 9);
  ens  = GetSample(varp, 10);

  fy = 0.5 * (efp - RFY * efs) * CFY;
  fz = 0.5 * (eft - RFZ * efb) * CFZ;
  ny = 0.5 * (enp - RNY * ens) * CNY;
  nz = 0.5 * (ent - RNZ * enb) * CNZ;

  sbank =  sin(roll);    
  cbank =  cos(roll); 

  efy = cbank * fy - sbank * fz;
  efz = cbank * fz + sbank * fy;

  eny = cbank * ny - sbank * nz;
  enz = cbank * nz + sbank * ny;

  eyo = cbank * vyo - sbank * vzo;
  ezo = cbank * vzo + sbank * vyo;

  PutSample(varp, eyo);

}  /* END SEFEYO */

/* -------------------------------------------------------------------- */
void sefezo(DERTBL *varp)
{
  PutSample(varp, ezo);

}  /* END SEFEZO */

/* -------------------------------------------------------------------- */
void sefefz(DERTBL *varp)
{
  PutSample(varp, efz);

}  /* END SEFEFZ */

/* -------------------------------------------------------------------- */
void sefefy(DERTBL *varp)
{
  PutSample(varp, efy);

}  /* END SEFEFY */

/* -------------------------------------------------------------------- */
void sefenz(DERTBL *varp)
{
  PutSample(varp, enz);

}  /* END SEFENZ */

/* -------------------------------------------------------------------- */
void sefeny(DERTBL *varp)
{
  PutSample(varp, eny);

}  /* END SEFENY */

/* END SPEMF.C */
