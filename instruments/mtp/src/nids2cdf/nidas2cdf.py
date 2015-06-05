#!/usr/bin/python

import os
import re
import sys

import numpy
import psycopg2

# if using something from RAF tree.
#sys.path.append("/home/local/raf/python")
#import raf.ac_config

from netCDF4 import Dataset


class MTPNetCDF:

  def __init__(self, path):
    # Open/Create netCDF.
    self.ncf = Dataset(path, 'w', format='NETCDF4')
    self.ncf.set_fill_on()

    # Dimensions.
    self.ncf.createDimension('MTP_Time', None)
    self.ncf.createDimension('MTP_NumberTemplates', 30)
    self.ncf.createDimension('MTP_ALT', 33)
    self.ncf.createDimension('MTP_TemplateALT', 33)
    self.ncf.createDimension('MTP_TemplateIDX', 43)
    self.ncf.createDimension('MTP_NumberChannels', 3)
    self.ncf.createDimension('MTP_BTScanAngle', 10)
    self.ncf.createDimension('MTP_CntScanAngle', 12)

    # Variables
    self.mtp_time = self.ncf.createVariable('MTP_Time', 'i4', ('MTP_Time'))
    self.mtp_time.long_name = 'time of MTP measurement'
    self.mtp_time.standard_name = 'time'
    self.mtp_time.strptime_format = 'seconds since %F %T %z'
    self.mtp_time.Category = 'MTP'

    self.cnts_angle = self.ncf.createVariable('CntScanAngle_MTP', 'f4', ('MTP_CntScanAngle'))
    self.cnts_angle.long_name = 'MTP Pointing Angle (incl Target)'
    self.cnts_angle.units = 'degree'
    self.cnts_angle.Category = 'MTP'

    self.cnts = self.ncf.createVariable('CNT_MTP', 'f4', ('MTP_Time', 'MTP_NumberChannels', 'MTP_CntScanAngle'))
    self.cnts.long_name = 'MTP Channel Scan Counts (incl Target)'
    self.cnts.units = 'count'
    self.cnts.Category = 'MTP'

    self.bt_angle = self.ncf.createVariable('BTScanAngle_MTP', 'f4', ('MTP_BTScanAngle'))
    self.bt_angle.long_name = 'MTP Pointing Angle'
    self.bt_angle.units = 'degree'
    self.bt_angle.Category = 'MTP'

    self.bt = self.ncf.createVariable('BT_MTP', 'f4', ('MTP_Time', 'MTP_NumberChannels', 'MTP_BTScanAngle'))
    self.bt.long_name = 'MTP Brightness Temperatures'
    self.bt.units = 'deg_K'
    self.bt.Category = 'MTP'

    self.template_idx = self.ncf.createVariable('TPLIDX_MTP', 'i4', ('MTP_Time'))
    self.template_idx2 = self.ncf.createVariable('TPLIDX2_MTP', 'i4', ('MTP_Time'))

    self.template_alt = self.ncf.createVariable('TPLALT_MTP', 'f4', ('MTP_Time', 'MTP_TemplateALT'))
    self.template_alt.long_name = 'MTP Template Altitude'
    self.template_alt.units = 'km'
    self.template_alt.Category = 'MTP'

    self.template_bt = self.ncf.createVariable('TPLBT_MTP', 'f4', ('MTP_TemplateIDX', 'MTP_TemplateALT', 'MTP_NumberChannels', 'MTP_BTScanAngle'))
    self.template_bt.long_name = 'MTP Template Brightness Temperature'
    self.template_bt.units = 'deg_K'
    self.template_bt.Category = 'MTP'

    self.template_alt = self.ncf.createVariable('TPLID_MTP', 'S1', ('MTP_TemplateIDX'))
    self.template_obs_t = self.ncf.createVariable('TPLOBSTIME_MTP', 'S1', ('MTP_NumberTemplates'))

    self.template_lat = self.ncf.createVariable('TPLLAT_MTP', 'f4', ('MTP_NumberTemplates'))
    self.template_lat.long_name = 'Latitude of Rawindsonde used for template'
    self.template_lat.units = 'degrees_N'
    self.template_lat.Category = 'MTP'

    self.template_lon = self.ncf.createVariable('TPLLON_MTP', 'f4', ('MTP_NumberTemplates'))
    self.template_lon.long_name = 'Longitude of Rawindsonde used for template'
    self.template_lon.units = 'degrees_E'
    self.template_lon.Category = 'MTP'

    self.alt = self.ncf.createVariable('ALT_MTP', 'f4', ('MTP_Time', 'MTP_ALT'))
    self.alt.long_name = 'MTP Profile Altitude Component'
    self.alt.units = 'km'
    self.alt.Category = 'MTP'

    self.temp = self.ncf.createVariable('TEMP_MTP', 'f4', ('MTP_Time', 'MTP_ALT'))
    self.temp.long_name = 'MTP Profile Temperature Component'
    self.temp.units = 'deg_K'
    self.temp.Category = 'MTP'

    self.mri = self.ncf.createVariable('MRI_MTP', 'f4', ('MTP_Time'))
    self.mri.long_name = 'MTP Quality Indicator'
    self.mri.units = 'count'
    self.mri.Category = 'MTP'

    self.trop = self.ncf.createVariable('TROP_MTP', 'f4', ('MTP_Time'))
    self.trop.long_name = 'MTP Calculated Tropopause Altitude for this Profile'
    self.trop.units = 'km'
    self.trop.Category = 'MTP'

    self.freq = self.ncf.createVariable('FREQ_MTP', 'f4', ('MTP_NumberChannels'))
    self.freq.long_name = 'MTP Channel Frequency'
    self.freq.units = 'GHz'
    self.freq.Category = 'MTP'


    self.accp = self.ncf.createVariable('ACCPCNTE_MTP', 'f4', ('MTP_Time'))
    self.accp.long_name = 'MTP Engineering Multiplexer Acceler Counts'
    self.accp.units = 'count'
    self.accp.Category = 'MTP'

    self.smcmd = self.ncf.createVariable('SMCMD_MTP', 'f4', ('MTP_Time'))
    self.smcmd.long_name = 'MTP Scan Motor Commanded Position'
    self.smcmd.units = 'count'
    self.smcmd.Category = 'MTP'

    self.smenc = self.ncf.createVariable('SMENC_MTP', 'f4', ('MTP_Time'))
    self.smenc.long_name = 'MTP Scan Motor Encoded Position'
    self.smenc.units = 'count'
    self.smenc.Category = 'MTP'

    self.tair = self.ncf.createVariable('TAIRCNTE_MTP', 'f4', ('MTP_Time'))
    self.tair.long_name = 'MTP Engineering Multiplexer T Pod Air Counts'
    self.tair.units = 'count'
    self.tair.Category = 'MTP'

    self.tamp = self.ncf.createVariable('TAMPCNTP_MTP', 'f4', ('MTP_Time'))
    self.tamp.long_name = 'MTP Platinum Multiplexer Amplifier Temp Counts'
    self.tamp.units = 'count'
    self.tamp.Category = 'MTP'

    self.tdat = self.ncf.createVariable('TDATCNTE_MTP', 'f4', ('MTP_Time'))
    self.tdat.long_name = 'MTP Engineering Multiplexer T Data Counts'
    self.tdat.units = 'count'
    self.tdat.Category = 'MTP'

    self.tmix = self.ncf.createVariable('TMIXCNTP_MTP', 'f4', ('MTP_Time'))
    self.tmix.long_name = 'MTP Platinum Multiplexer Mixer Temperature Counts'
    self.tmix.units = 'count'
    self.tmix.Category = 'MTP'

    self.tmtr = self.ncf.createVariable('TMTRCNTE_MTP', 'f4', ('MTP_Time'))
    self.tmtr.long_name = 'MTP Engineering Multiplexer T Motor Counts'
    self.tmtr.units = 'count'
    self.tmtr.Category = 'MTP'

    self.tnc = self.ncf.createVariable('TNCCNTE_MTP', 'f4', ('MTP_Time'))
    self.tnc.long_name = 'MTP Engineering Multiplexer T N/C Counts'
    self.tnc.units = 'count'
    self.tnc.Category = 'MTP'

    self.tnd = self.ncf.createVariable('TNDCNTP_MTP', 'f4', ('MTP_Time'))
    self.tnd.long_name = 'MTP Platinum Multiplexer Noise Diode Temp Counts'
    self.tnd.units = 'count'
    self.tnd.Category = 'MTP'

    self.tpsp = self.ncf.createVariable('TPSPCNTE_MTP', 'f4', ('MTP_Time'))
    self.tpsp.long_name = 'MTP Engineering Multiplexer T Power Supply Counts'
    self.tpsp.units = 'count'
    self.tpsp.Category = 'MTP'

    self.tr350 = self.ncf.createVariable('TR350CNTP_MTP', 'f4', ('MTP_Time'))
    self.tr350.long_name = 'MTP Platinum Multiplexer R350 Counts'
    self.tr350.units = 'count'
    self.tr350.Category = 'MTP'

    self.tr600 = self.ncf.createVariable('TR600CNTP_MTP', 'f4', ('MTP_Time'))
    self.tr600.long_name = 'MTP Platinum Multiplexer R600 Counts'
    self.tr600.units = 'count'
    self.tr600.Category = 'MTP'

    self.tsmp = self.ncf.createVariable('TSMPCNTE_MTP', 'f4', ('MTP_Time'))
    self.tsmp.long_name = 'MTP Engineering Multiplexer T Scan Counts'
    self.tsmp.units = 'count'
    self.tsmp.Category = 'MTP'

    self.tsync = self.ncf.createVariable('TSYNCNTE_MTP', 'f4', ('MTP_Time'))
    self.tsync.long_name = 'MTP Engineering Multiplexer T Synth Counts'
    self.tsync.units = 'count'
    self.tsync.Category = 'MTP'

    self.ttcnt = self.ncf.createVariable('TTCNTRCNTP_MTP', 'f4', ('MTP_Time'))
    self.ttcnt.long_name = 'MTP Platinum Multiplexer Target Center Temp Counts'
    self.ttcnt.units = 'count'
    self.ttcnt.Category = 'MTP'

    self.ttedg = self.ncf.createVariable('TTEDGCNTP_MTP', 'f4', ('MTP_Time'))
    self.ttedg.long_name = 'MTP Platinum Multiplexer Target Edge Temp Counts'
    self.ttedg.units = 'count'
    self.ttedg.Category = 'MTP'

    self.twin = self.ncf.createVariable('TWINCNTP_MTP', 'f4', ('MTP_Time'))
    self.twin.long_name = 'MTP Platinum Multiplexer Polyethelene Window Temp Counts'
    self.twin.units = 'count'
    self.twin.Category = 'MTP'


    self.vm08 = self.ncf.createVariable('VM08CNTE_MTP', 'f4', ('MTP_Time'))
    self.vm08.long_name = 'MTP Engineering Multiplexer Vm08 Counts'
    self.vm08.units = 'count'
    self.vm08.Category = 'MTP'

    self.vm15 = self.ncf.createVariable('VM15CNTE_MTP', 'f4', ('MTP_Time'))
    self.vm15.long_name = 'MTP Engineering Multiplexer Vm15 Counts'
    self.vm15.units = 'count'
    self.vm15.Category = 'MTP'

    self.vmtr = self.ncf.createVariable('VMTRCNTE_MTP', 'f4', ('MTP_Time'))
    self.vmtr.long_name = 'MTP Engineering Multiplexer Vmtr Counts'
    self.vmtr.units = 'count'
    self.vmtr.Category = 'MTP'

    self.vp05 = self.ncf.createVariable('VP05CNTE_MTP', 'f4', ('MTP_Time'))
    self.vp05.long_name = 'MTP Engineering Multiplexer Vp05 Counts'
    self.vp05.units = 'count'
    self.vp05.Category = 'MTP'

    self.vp08 = self.ncf.createVariable('VP08CNTE_MTP', 'f4', ('MTP_Time'))
    self.vp08.long_name = 'MTP Engineering Multiplexer Vp08 Counts'
    self.vp08.units = 'count'
    self.vp08.Category = 'MTP'

    self.vp15 = self.ncf.createVariable('VP15CNTE_MTP', 'f4', ('MTP_Time'))
    self.vp15.long_name = 'MTP Engineering Multiplexer Vp15 Counts'
    self.vp15.units = 'count'
    self.vp15.Category = 'MTP'

    self.vsync = self.ncf.createVariable('VSYNCNTE_MTP', 'f4', ('MTP_Time'))
    self.vsync.long_name = 'MTP Engineering Multiplexer Vsyn Counts'
    self.vsync.units = 'count'
    self.vsync.Category = 'MTP'

    self.vvid = self.ncf.createVariable('VVIDCNTE_MTP', 'f4', ('MTP_Time'))
    self.vvid.long_name = 'MTP Engineering Multiplexer Vvid Counts'
    self.vvid.units = 'count'
    self.vvid.Category = 'MTP'


  def loadMTP(self, path):
    # Extract date time and turn it into usec (seconds since midnight)
    dt = numpy.genfromtxt(sys.argv[1], dtype=None, delimiter=',', usecols=(1))

    s = dt[0]
    start_date = "seconds since " + s[0:4] + '-' + s[4:6] + '-' + s[6:8] + ' 00:00:00 +0000'
    usec = []
    for s in dt:
      usec.append(int(s[9:11]) * 3600 + int(s[11:13]) * 60 + int(s[13:15]))

    self.mtp_time.units = start_date
    self.mtp_time[:] = usec


  def loadMTPP(self, path):
    # Load temperature profile (MTPP data).
    alt_in  = numpy.loadtxt(sys.argv[2], dtype='f4', delimiter=',', usecols=(tuple(range(2,35))), ndmin=2)
    temp_in = numpy.loadtxt(sys.argv[2], dtype='f4', delimiter=',', usecols=(tuple(range(35,68))), ndmin=2)

    # Write temp/alt profile (skew-t).
    self.temp[:] = temp_in
    self.alt[:] = alt_in



def main(argv):
    mtpcdf = MTPNetCDF('simple.nc')
    mtpcdf.loadMTP(argv[1])
    mtpcdf.loadMTPP(argv[2])
    mtpcdf.ncf.close()

if __name__ == "__main__":
  main(sys.argv)


