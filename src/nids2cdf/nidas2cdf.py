#!/usr/bin/python

import os
import re
import sys

import psycopg2

# if using something from RAF tree.
#sys.path.append("/home/local/raf/python")
#import raf.ac_config

from netCDF4 import Dataset



def CreateNetCDFHeader(f):
  # Dimensions.
  f.createDimension('MTP_Time', None)
  f.createDimension('MTP_NumberTemplates', 30)
  f.createDimension('MTP_ALT', 33)
  f.createDimension('MTP_TemplateALT', 33)
  f.createDimension('MTP_TemplateIDX', 43)
  f.createDimension('MTP_NumberChannels', 3)
  f.createDimension('MTP_BTScanAngle', 10)
  f.createDimension('MTP_CntScanAngle', 12)

  # Variables
  mtp_time = f.createVariable('MTP_Time', 'i4', ('MTP_Time'))
  mtp_time.long_name = 'time of MTP measurement'
  mtp_time.standard_name = 'time'
  mtp_time.strptime_format = 'seconds since %F %T %z'
  mtp_time.Category = 'MTP'

  cnts_angle = f.createVariable('CntScanAngle_MTP', 'f4', ('MTP_CntScanAngle'))
  cnts_angle.long_name = 'MTP Pointing Angle (incl Target)'
  cnts_angle.units = 'degree'
  cnts_angle.Category = 'MTP'

  cnts = f.createVariable('CNT_MTP', 'f4', ('MTP_Time', 'MTP_NumberChannels', 'MTP_CntScanAngle'))
  cnts.long_name = 'MTP Channel Scan Counts (incl Target)'
  cnts.units = 'count'
  cnts.Category = 'MTP'

  bt_angle = f.createVariable('BTScanAngle_MTP', 'f4', ('MTP_BTScanAngle'))
  bt_angle.long_name = 'MTP Pointing Angle'
  bt_angle.units = 'degree'
  bt_angle.Category = 'MTP'

  bt = f.createVariable('BT_MTP', 'f4', ('MTP_Time', 'MTP_NumberChannels', 'MTP_BTScanAngle'))
  bt.long_name = 'MTP Brightness Temperatures'
  bt.units = 'deg_K'
  bt.Category = 'MTP'

  template_idx = f.createVariable('TPLIDX_MTP', 'i4', ('MTP_Time'))
  template_idx2 = f.createVariable('TPLIDX2_MTP', 'i4', ('MTP_Time'))

  template_alt = f.createVariable('TPLALT_MTP', 'f4', ('MTP_Time', 'MTP_TemplateALT'))
  template_alt.long_name = 'MTP Template Altitude'
  template_alt.units = 'km'
  template_alt.Category = 'MTP'

  template_bt = f.createVariable('TPLBT_MTP', 'f4', ('MTP_TemplateIDX', 'MTP_TemplateALT', 'MTP_NumberChannels', 'MTP_BTScanAngle'))
  template_bt.long_name = 'MTP Template Brightness Temperature'
  template_bt.units = 'deg_K'
  template_bt.Category = 'MTP'

  template_alt = f.createVariable('TPLID_MTP', 'S1', ('MTP_TemplateIDX'))
  template_obs_t = f.createVariable('TPLOBSTIME_MTP', 'S1', ('MTP_NumberTemplates'))

  template_lat = f.createVariable('TPLLAT_MTP', 'f4', ('MTP_NumberTemplates'))
  template_lat.long_name = 'Latitude of Rawindsonde used for template'
  template_lat.units = 'degrees_N'
  template_lat.Category = 'MTP'

  template_lon = f.createVariable('TPLLON_MTP', 'f4', ('MTP_NumberTemplates'))
  template_lon.long_name = 'Longitude of Rawindsonde used for template'
  template_lon.units = 'degrees_E'
  template_lon.Category = 'MTP'

  alt = f.createVariable('ALT_MTP', 'f4', ('MTP_Time', 'MTP_ALT'))
  alt.long_name = 'MTP Profile Altitude Component'
  alt.units = 'km'
  alt.Category = 'MTP'

  temp = f.createVariable('TEMP_MTP', 'f4', ('MTP_Time', 'MTP_ALT'))
  temp.long_name = 'MTP Profile Temperature Component'
  temp.units = 'deg_K'
  temp.Category = 'MTP'

  mri = f.createVariable('MRI_MTP', 'f4', ('MTP_Time'))
  mri.long_name = 'MTP Quality Indicator'
  mri.units = 'count'
  mri.Category = 'MTP'

  mri = f.createVariable('TROP_MTP', 'f4', ('MTP_Time'))
  mri.long_name = 'MTP Calculated Tropopause Altitude for this Profile'
  mri.units = 'km'
  mri.Category = 'MTP'

  freq = f.createVariable('FREQ_MTP', 'f4', ('MTP_NumberChannels'))
  freq.long_name = 'MTP Channel Frequency'
  freq.units = 'GHz'
  freq.Category = 'MTP'



  accp = f.createVariable('ACCPCNTE_MTP', 'f4', ('MTP_Time'))
  accp.long_name = 'MTP Engineering Multiplexer Acceler Counts'
  accp.units = 'count'
  accp.Category = 'MTP'

  smcmd = f.createVariable('SMCMD_MTP', 'f4', ('MTP_Time'))
  smcmd.long_name = 'MTP Scan Motor Commanded Position'
  smcmd.units = 'count'
  smcmd.Category = 'MTP'

  smenc = f.createVariable('SMENC_MTP', 'f4', ('MTP_Time'))
  smenc.long_name = 'MTP Scan Motor Encoded Position'
  smenc.units = 'count'
  smenc.Category = 'MTP'

  tair = f.createVariable('TAIRCNTE_MTP', 'f4', ('MTP_Time'))
  tair.long_name = 'MTP Engineering Multiplexer T Pod Air Counts'
  tair.units = 'count'
  tair.Category = 'MTP'

  tamp = f.createVariable('TAMPCNTP_MTP', 'f4', ('MTP_Time'))
  tamp.long_name = 'MTP Platinum Multiplexer Amplifier Temp Counts'
  tamp.units = 'count'
  tamp.Category = 'MTP'

  tdat = f.createVariable('TDATCNTE_MTP', 'f4', ('MTP_Time'))
  tdat.long_name = 'MTP Engineering Multiplexer T Data Counts'
  tdat.units = 'count'
  tdat.Category = 'MTP'

  tmix = f.createVariable('TMIXCNTP_MTP', 'f4', ('MTP_Time'))
  tmix.long_name = 'MTP Platinum Multiplexer Mixer Temperature Counts'
  tmix.units = 'count'
  tmix.Category = 'MTP'

  tmtr = f.createVariable('TMTRCNTE_MTP', 'f4', ('MTP_Time'))
  tmtr.long_name = 'MTP Engineering Multiplexer T Motor Counts'
  tmtr.units = 'count'
  tmtr.Category = 'MTP'

  tnc = f.createVariable('TNCCNTE_MTP', 'f4', ('MTP_Time'))
  tnc.long_name = 'MTP Engineering Multiplexer T N/C Counts'
  tnc.units = 'count'
  tnc.Category = 'MTP'

  tnd = f.createVariable('TNDCNTP_MTP', 'f4', ('MTP_Time'))
  tnd.long_name = 'MTP Platinum Multiplexer Noise Diode Temp Counts'
  tnd.units = 'count'
  tnd.Category = 'MTP'

  tpsp = f.createVariable('TPSPCNTE_MTP', 'f4', ('MTP_Time'))
  tpsp.long_name = 'MTP Engineering Multiplexer T Power Supply Counts'
  tpsp.units = 'count'
  tpsp.Category = 'MTP'

  tr350 = f.createVariable('TR350CNTP_MTP', 'f4', ('MTP_Time'))
  tr350.long_name = 'MTP Platinum Multiplexer R350 Counts'
  tr350.units = 'count'
  tr350.Category = 'MTP'

  tr600 = f.createVariable('TR600CNTP_MTP', 'f4', ('MTP_Time'))
  tr600.long_name = 'MTP Platinum Multiplexer R600 Counts'
  tr600.units = 'count'
  tr600.Category = 'MTP'

  tsmp = f.createVariable('TSMPCNTE_MTP', 'f4', ('MTP_Time'))
  tsmp.long_name = 'MTP Engineering Multiplexer T Scan Counts'
  tsmp.units = 'count'
  tsmp.Category = 'MTP'

  tsync = f.createVariable('TSYNCNTE_MTP', 'f4', ('MTP_Time'))
  tsync.long_name = 'MTP Engineering Multiplexer T Synth Counts'
  tsync.units = 'count'
  tsync.Category = 'MTP'

  ttcnt = f.createVariable('TTCNTRCNTP_MTP', 'f4', ('MTP_Time'))
  ttcnt.long_name = 'MTP Platinum Multiplexer Target Center Temp Counts'
  ttcnt.units = 'count'
  ttcnt.Category = 'MTP'

  ttedg = f.createVariable('TTEDGCNTP_MTP', 'f4', ('MTP_Time'))
  ttedg.long_name = 'MTP Platinum Multiplexer Target Edge Temp Counts'
  ttedg.units = 'count'
  ttedg.Category = 'MTP'

  twin = f.createVariable('TWINCNTP_MTP', 'f4', ('MTP_Time'))
  twin.long_name = 'MTP Platinum Multiplexer Polyethelene Window Temp Counts'
  twin.units = 'count'
  twin.Category = 'MTP'


  vm08 = f.createVariable('VM08CNTE_MTP', 'f4', ('MTP_Time'))
  vm08.long_name = 'MTP Engineering Multiplexer Vm08 Counts'
  vm08.units = 'count'
  vm08.Category = 'MTP'

  vm15 = f.createVariable('VM15CNTE_MTP', 'f4', ('MTP_Time'))
  vm15.long_name = 'MTP Engineering Multiplexer Vm15 Counts'
  vm15.units = 'count'
  vm15.Category = 'MTP'

  vmtr = f.createVariable('VMTRCNTE_MTP', 'f4', ('MTP_Time'))
  vmtr.long_name = 'MTP Engineering Multiplexer Vmtr Counts'
  vmtr.units = 'count'
  vmtr.Category = 'MTP'

  vp05 = f.createVariable('VP05CNTE_MTP', 'f4', ('MTP_Time'))
  vp05.long_name = 'MTP Engineering Multiplexer Vp05 Counts'
  vp05.units = 'count'
  vp05.Category = 'MTP'

  vp08 = f.createVariable('VP08CNTE_MTP', 'f4', ('MTP_Time'))
  vp08.long_name = 'MTP Engineering Multiplexer Vp08 Counts'
  vp08.units = 'count'
  vp08.Category = 'MTP'

  vp15 = f.createVariable('VP15CNTE_MTP', 'f4', ('MTP_Time'))
  vp15.long_name = 'MTP Engineering Multiplexer Vp15 Counts'
  vp15.units = 'count'
  vp15.Category = 'MTP'

  vsync = f.createVariable('VSYNCNTE_MTP', 'f4', ('MTP_Time'))
  vsync.long_name = 'MTP Engineering Multiplexer Vsyn Counts'
  vsync.units = 'count'
  vsync.Category = 'MTP'

  vvid = f.createVariable('VVIDCNTE_MTP', 'f4', ('MTP_Time'))
  vvid.long_name = 'MTP Engineering Multiplexer Vvid Counts'
  vvid.units = 'count'
  vvid.Category = 'MTP'


def parseMTPATP(line):
    parts = line.split(',')


def parseMTP(line):
    parts = line.split(',')



# Open/Create netCDF.
f = Dataset('simple.nc', 'w', format='NETCDF4')
CreateNetCDFHeader(f)


# Scan data from stdin (data_dump).
for line in sys.stdin:
    if 'MTP' not in line: continue

    parts = line.split()
    if line.startswith('MTPP'):
        parseMTPATP(parts[8])
    else:
        parseMTP(parts[8])



f.close()

