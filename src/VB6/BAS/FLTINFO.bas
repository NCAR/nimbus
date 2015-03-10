Attribute VB_Name = "FLTINFO"
Option Explicit
' FLTINFO
'
  Public TBfitRMS!(0 To 7)
' Indices used in various programs
' Public NFL%                        'Number of retrieval flight levels (normally 9 or 10), was Nra
  Public Nret%                       'Number of retrieval levels (normally 33)
  Public Nobs%                       'Number of observables (Now Nlo*Nel Was Nlo*(Nel-1)+1)
  Public Nlo%                        'Number of LO frequencies (normally 2 or 3)
  Public Nif%                        'Number of IF channels (9 to 12)
  Public Nnu%                        'Number of USB+LSB IF channels (18 to 24)
' Public Nel%                        'Number of elevation angles (normally 5 up, horizon and 4 down)
  Public NH%                         'Number of numerical integration altitudes
  
'  Public NRC%                       'Actual Number of RC sets used
  Public Const NRCmax = 65           'Maximum number of RC sets (could be 88 if needed)
  Public Const NRCbin = 15           'Number of RC sets on MTPbin TBs tab
                                     'Number of "Counts" channels. Has been 12,
  Public Const NctsMax = 14          'But 14 allowed for in case of second target
  Public aRC(0 To 64)                'Mapping between Used RCs and all those available
  
' Data Processing Flags
  Public ArrayRead As Boolean        'TRUE if RAW File has been read into memory
  Public BatchBusy As Boolean        'Batch Process Busy Flag
  Public BatchStart As Boolean       'Batch processing has been started
  Public RAOBreanalysis  As Boolean  'Flag to use nearby RAOBs to determine RCs
  Public RCsiteUsed As Boolean       'TRUE if near a RC site
  Public SubTrop As Boolean          'Substitute RAOB tropopause for MTP tropopause (normally on ascent/descent)
  Public UseMMSpALT As Boolean       'TRUE if MMS pressure altitude is being used
  Public UserLATs As Boolean         'If FALSE, use default values for transition regions
  Public TAqual As Boolean           '
  Public NavQual As Boolean
  Public CtsQual As Boolean
  Public CycleQual As Boolean
  Public NavData As Boolean          'Nav Data is being received if TRUE
  
  ' TB Sensitivity Matrices in natural order (Smatrix) and mapped to observable space (SmatrixOB)
  Public Smatrix!(0 To 64, 1 To 15, 1 To 3, 1 To 10, 1 To 2)
  Public SmatrixOB!(0 To 64, 1 To 15, 1 To 30, 1 To 2)
  'ReDim SmatrixOB(0 To NRC - 1, 1 To NFL, 1 To Nobs, 1 To 2)
  'ReDim Smatrix(0 To NRC - 1, 1 To NFL, 1 To Channels, 1 To Nel, 1 To 2)
  
' Batch Processing Parameters
  Public BatchFQFN$                  'Fully Qualified Filename
  Public ProcessList%(0 To 10)       'Batch Process List
  Public ProcessCount%               'Items in Batch Process List
  Public CurrentProcess%             'Current Process Number
  
  Public ScanTime!                   'Average Scan Length for flight (needed by TTO)
  Public nRAOBtemplates%             'Number of RAOBs that were near flight track
  Public RCset%(1 To 35)             'TRUE if RC set based on RAOB within 1 day of FlightDate
  Public rLatitude!(1 To 35)         'RAOB site latitude for RC determination
  Public rLongitude!(1 To 35)        'RAOB site longitude for RC determination
   
  Public Cifa!(1 To 3)
  Public Clo1!(1 To 3)
  Public Clo2!(1 To 3)
  Public Coff!(1 To 3)
  Public Cnav!(1 To 3)
  
' These are the FLTINFO.INI shared variables!
  Public CurrentFormNumber%          '
  Public FIsize%                     'Number of records in FLTINFO .INI file
  Public Source$                     'Source of RAW data ("F"-file, "D"-Serial Port, "T"-Test Mode)
    
' Retrieval Coefficient Related Parameters
  Public OATnavCOR!                  'Correction ADDED to Outside Air Temperature (based on RAOB comparisons)
  Public OATmtpCOR!                  'Correction ADDED to ATP (based on difference between Tmtp and RT(16))
  Public RegNr%                      'RC set used when only 3 (Reg0$, etc.) No longer used
  Public UserLATn$, CalSource$
  Public MRIrec%, MRI1sum!, MRI2sum!
  Public ATPrange!                   'Distance from aircraft to use in Wavg, Wrms calculation

' Version strings for various analysis programs
  Public vCF$, vD1$, vD2$, vMP$, vFR$, vFW$, vDT$
  Public CurrentStamp As Date        'Time of current real time record
  Public LastStamp As Date           'Time of last real time record
  Public CommandStamp As Date        'Time of last Command.txt file
  Public LastCommandStamp As Date
  Public FltDate$
  
' Parameters used in CAL File Data Structure
  Public Gendate$, WCTdates(1 To 10) As String
  'UTstart and UTend never set!!!, NDoffset, FIversion
  Public UTstart&                    'UT seconds of first record
  Public UTend&                      'UT seconds of last record
  Public Channels%, Nel%, Emissivity!, Reflectivity!, DeltaTmin!
  Public Nfit%, NP$(1 To 5), GEC!(1 To 3, 1 To 5), GOF!(1 To 5)                'Gain Equation Coefficients
  Public MXRoffset%, TGToffset%, NAVoffset!, NDoffset%, WINcor!(1 To 3, 1 To 10)
  Public aTGToffset%(0 To 3), aMXRoffset%(0 To 3), aNDoffset%(0 To 3), aNAVoffset!(0 To 3)
  Public EnableWCT As Boolean, CMAcycles%, CMAcycles2%, RFIiterations%, RFIiterations2%
  Public RFIthreshold%, MUXthreshold!, BadCycles%, Badcycles2%
  Public GeqnMax!(1 To 3), GeqnMin!(1 To 3), GnavMax!(1 To 3), GnavMin!(1 To 3)
  Public GndMax!(1 To 3), GndMin!(1 To 3), ChInfo!(1 To 3), ChInfoSum!
  Public TBfitX1%, TBfitX2%, TBfitY1%, TBfitY2%
  Public UseMAforCB As Boolean, UseMAforCN As Boolean, UseMAforCS As Boolean, UseMAforCSgain As Boolean
  Public Cnd0!(1 To 3), Cnd1!(1 To 3), Cnd2!(1 To 3), TrefND!, RHS As Boolean
  Public LocHor%, UseMAforTtgt As Boolean, UseMAforTifa As Boolean, Targets%
  Public RAWextension As String * 3, NRC%, RCformat%(0 To 75), Reg$(0 To 75), RCuse%(0 To 75)
  Public RCformats%, NRCavailable%
' Parameters used in FIrecord Data Structure
  Public TotalCycles%, Reg0$, Reg1$, Reg2$, USE5$, Algorithm%, MRIavg!, MRIrms!
  Public utMTPcor!, DTavg!, DTrms!, ALTfujCONST!, ALTfujSLOPE!, LAT1!, LAT2!, LAT3!, LAT4!
  Public DoAll As Boolean, DoAllMask%, NFL%, FLA%(0 To 25), SU$
  Public OATsource%, GainScale%, REFsource%, EnableCalfile As Boolean, MakeEditWord&
  Public EditTropAlt As Boolean, TropAltMin!, TropAltMax!, TropAltPC%
  Public EditRetAlt As Boolean, RetAltMin!, RetAltMax!, RetAltPC%
  Public EditRate As Boolean, RateMin!, RateMax!, RatePC%
  Public EditZtOff As Boolean, ZtOffA!, ZtOffB!, ZtOffPC%
  Public EditTemperature As Boolean, TemperatureMin!, TemperatureMax!, TemperaturePC%
  Public EditPitch As Boolean, ePitchMax!, ePitchMin!, PitchPC%
  Public EditRoll As Boolean, eRollMax!, eRollMin!, RollPC%
  Public EditRAWbad, RAWbadmin!, RAWbadmax!, RAWbadPC%
  Public EditNlev As Boolean, NlevMin!, NlevMax!, NlevPC%
  Public EditTtgt As Boolean, TtgtMin!, TtgtMax!, TtgtPC%
  Public EditNav As Boolean, NavMin!, NavMax!, NavQualPC%
  Public EditCts As Boolean, CtsMin!, CtsMax!, CtsQualPC%
  Public EditCycle As Boolean, CycleQualPC%
  Public EditTA As Boolean, TAmin!, TAmax!, TAqualPC%
  Public EditOATtrop As Boolean, OATtropPC%, OATtrop As Boolean
  Public OATzt10!, OATzt11!, OATzt12!, OATzt13!, OATzt14!, OATzt20!, OATzt21!, OATzt22!, OATzt23!, OATzt24!
  Public OATks10!, OATks11!, OATks12!, OATks13!, OATks20!, OATks21!, OATks22!, OATks23!
  Public Tzt10!, Tzt11!, Tzt12!, Tzt13!, Tzt14!, Tzt20!, Tzt21!, Tzt22!, Tzt23!, Tzt24!
  Public CALversion As Date, MAKEversion As Date, EDITversion As Date, FLTINFOversion As Date, EXEversion As Date
  Public Tstamp As Date, RAWstamp As Date, REFstamp As Date
  Public ERFstamp As Date, MMSstamp As Date, CTCstamp As Date
  Public Ncts%, MTPyaw!, MTPpitch!, MTProll!, MTPfiduciary!, EmaxDegrees!, fEcCount%
'
  Public IFoff2!(1 To 12)
  Public IFwt2!(1 To 12)
  Public IFoff!(1 To 3, 1 To 16)  'IF frequency offsets from bandpass model
  Public IFwt!(1 To 3, 1 To 16)   'Weights given to each section of the IF bandpass model
  Public LO!(1 To 3)              'LO frequencies (GHz)
  Public LOSUI!(1 To 3)
  Public El!(1 To 10)             'Elevation Angles (degrees) from RC file
  Public ElSUI!(1 To 10)          'Elevation Angles (degrees) from SUI file
  Public dZ!(1 To 33)             'Altitude Offsets wrt flight level for 33 retrieved levels (km)
  Public CalFileFormat%           'CAL file format (0 = BLG, 1= INI, 2=BINARY)
  Public Zr!(1 To 25)
  Public OBrms!(1 To 16, 1 To 30)
  Public OBrms1s!(1 To 30)          '1-sigma apriori observable errors
  Public sOBav!(1 To 30)
  Public OBav!(0 To 64, 1 To 16, 1 To 30)
  Public rc!(0 To 64, 1 To 15, 1 To 33, 1 To 30)
  Public RTav!(0 To 64, 1 To 16, 1 To 33)
  Public RAav!(0 To 64, 1 To 16, 1 To 33)
' RMSai=Archive RMS, RMSei=Expected RMS performance
  Public RMSa!(0 To 64, 1 To 16, 1 To 33), RMSe!(0 To 64, 1 To 16, 1 To 33)
  Public UserRCindex%              'User Selected RC index
  Public RCwt!
  Public Pcorr!                    'pressure altitude correction
  Public Spares!(1 To 17)
  Public IWGrecord&
  Public IWG_DandT As Date
  Public IWG_Parameter!(2 To 49)
  Public NAV_DandT As Date
  Public NAV_Parameter!(1 To 73)
  Public IWGforward As Boolean

' ----------------------------------------------------------------------------

' Define Calfile and FLTinfo data structures
'
' CAL File Data Structure
Type CFrecord
' "GENERAL"
  Gendate As String * 8           'Analysis Date
  WCTdates(1 To 10) As String * 8  'Flight used to generate CAL file
  UTstart As Long
  UTend As Long
  Channels As Integer           'Number of frequency channels
  Nel As Integer                'Number of elevation angles
  Emissivity As Single          'Emissivity of window
  Reflectivity As Single        'Reflectivity of window
  DeltaTmin As Single           'Minimum temperature difference between target and sky

' "FIT_INFO"
  Nfit As Integer               'Number of fit parameters (including offset)
  NP(1 To 5) As String * 6      'Alphanumeric fit parameter (eg Tifa, Ttgt)
  GEC(1 To 5, 1 To 5) As Single 'Gain Equation Coefficients (channel, parameter)
  GOF(1 To 5) As Single         'Gain Equation Offsets (Channel)
  TGToffset As Integer          'Target Temperature Lead/Lag (+ is lead)
  MXRoffset As Integer          'Mixer Temperature Lead/Lag

' "WINDOW_CORRECTIONS"
  WINcor(1 To 3, 1 To 10) As Single  'Window corrections (Channel, El Angle)
  EnableWCT As Boolean
  
' RAW counts editting criteria
  CMAcycles As Integer               'Counts smoothing cycles
  RFIthreshold As Integer            'Maximum counts offset from moving average
  RFIiterations As Integer           'No of iterations needed to converge
  MUXthreshold As Single             'Maximum dT in mux temperatures
  BadCycles As Integer               'No of cycles not satisfying RFI threshold
  
' Gain Thresholds
  GeqnMin(1 To 3) As Single           'Gain Equation Min
  GeqnMax(1 To 3) As Single           'Gain Equation Max
  GnavMin(1 To 3) As Single
  GnavMax(1 To 3) As Single
  GndMin(1 To 3) As Single
  GndMax(1 To 3) As Single

' Channel Weights
  ChInfo(1 To 3) As Single

' Fit Region
  TBfitX1 As Integer
  TBfitX2 As Integer
  TBfitY1 As Integer
  TBfitY2 As Integer
  Badcycles2 As Integer
  RFIiterations2 As Integer
  
  SpInt As Integer
  UseMAforCB As Boolean
  UseMAforCS As Boolean
  NDoffset As Integer
  
  Cnd0(1 To 3) As Single
  Cnd1(1 To 3) As Single
  Cnd2(1 To 3) As Single
  TrefND As Single
  NAVoffset As Single
  RHS As Boolean
  LocHor As Integer
  UseMAforCN As Boolean
  CMAcycles2 As Integer   'For Sky Counts only
  UseMAforTtgt As Boolean
  UseMAforTifa As Boolean
  RAWextension As String * 3
' FI expansion begins here  THESE COMMENTED LINES ARE IN FIrecord below
'  NRC As Integer                      'Number of RC sets
'  RCformat(0 To 10) As Integer        'Number integating RC format (normally = 2 for all sets)
'  Reg(0 To 15) As String * 5          '5 character string for each RC set used
'  RCuse(0 To 15) As Single            'Number of times a give RC has been used in retrieval for a flight
'  RCfmt(1 To 5) As Integer            'Rest of RCformat array
  NRC As Integer
  RCfmt(1 To 24) As Integer           'Rest of RCformat array
  RegExt(1 To 19) As String * 5
  RCuseExt(1 To 19) As Single
End Type

' FLTinfo Data Structure
Type FIrecord
  Mission As String * 15              'Mission name
  Fileformat As Byte                  'File format
  FltNumber As String * 8             'Flight number
  Pi As String * 80                   'Principal investigator
  Yeer As Integer                     'Year
  Doy As Integer                      'Day of Year
  YYYYMMDD As String * 8              'Year/Month/Day string
  TotalCycles As Integer              'Number of records in file
  Channels As Integer                 'Number of MTP receiver channels
  Drive As String * 2                 'Data drive
  Rdir As String * 8                  'Platform directory (DC8, ER2 or WB57)
  Path As String * 40                 'Flight Directory
  Pgm As String * 40                  'Program Directory
  Reg0 As String * 5                  'Retrieval Coefficient Name for Tropics
  Reg1 As String * 5                  'Retrieval Coefficient Name for Mid-Latitudes
  Reg2 As String * 5                  'Retrieval Coefficient Name for Polar Region
  USE5 As String * 5                  'USE file name for standard errors on trop
  Algorithm As Integer                'Processing Algorithm (e.g., MRI, LAT)
  MRIavg As Single                    'Average MRI value for flight
  MRIrms As Single                    'Standard Deviation on MRI value for flight
  
  utMTPcor As Single                  'UT Correction to ADD to MTP clock [sec]
  DTavg As Single                     'Average value of UTnav-UTmtp for flight
  DTrms As Single                     'Standard deviation of UTnav-UTmtp for flight
  
  ALTfujCONST As Single               'Altitude offset to be applied to NAV altitude
  ALTfujSLOPE As Single               'Slope of NAV altitude correction
  OATnavCOR As Single                 'OAT correction to ADD to NAV temperature
  CalSource As String * 8             'Obsolete
  UseMMSpALT As Boolean               'If TRUE, use MMS pressure altitude in analysis
  
  LAT1 As Single                      'South side of Tropical-to-Mid-latitude transition region [deg]
  LAT2 As Single                      'North side of Tropical-to-Mid-latitude transition region [deg]
  LAT3 As Single                      'South side of Mid-latitude-to-Polar transition region [deg]
  LAT4 As Single                      'North side of Mid-latitude-to-Polar transition region [deg]
  UserLATs As Boolean                 'If FALSE, use default values for transition regions
  
  DoAll As Boolean                    'If TRUE, perform end-to-end data analysis
  DoAllMask As Integer                'Mask specifying analysis steps
  NFL As Integer                      'Number of levels for which retrieval coefficients are available
  FLA(1 To 15) As Integer             'Array of retrieval levels [10 m]
  SU  As String * 4                   'String specifying Sensor Unit
  
  OATsource As Integer                'Outside Air Temperature source (e.g., MTP, NAV, MMS)
  GainScale As Integer                'Gain Scale (e.g., Gain equation, OAT, Noise Diode)
  REFsource As Integer                'Reference Source (either OAT or Target)
  
  PgmDrive As String * 2              'Program Drive (obsolete, handled in MTPsetup.ini now)
  EnableCalfile As Boolean            'True if Calfile is in header record
  
  MakeEditWord As Long                'Not used
  
  EditTropAlt As Single               'If TRUE, edit allowed range of trop altitudes
  TropAltMin As Single                'Minimum allowed trop altitude [km]
  TropAltMax As Single                'Maximum allowed trop altitude [km]
  TropAltPC As Integer                'Number of scans flagged as bad for this reason
  
  EditRetAlt As Single                'If TRUE, edit pressure altitude range of retrievals
  RetAltMin As Single                 'Minimum allowed retrieval altitude [km]
  RetAltMax As Single                 'Maximum allowed retrieval altitude [km]
  RetAltPC As Integer                 'Number of scans flagged as bad for this reason
  
  EditZtOff As Single                 'If TRUE, flag trops that are too far from aircraft
  ZtOffA As Single                    'Allowed offset of trop ABOVE aircraft
  ZtOffB As Single                    'Allowed offset of trop BELOW aircraft
  ZtOffPC As Integer                  'Number of scans flagged as bad for this reason
  
  EditTemperature As Single           'If TRUE, flag scans which violate temperature limits
  TemperatureMin As Single            'Minimum allowed retrieved temperature [K]
  TemperatureMax As Single            'Maximum allowed retrieved temperature [K]
  TemperaturePC As Integer            'Number of scans flagged as bad for this reason
  
  EditPitch As Single                 'If TRUE, flag scans that violate pitch limits
  ePitchMin As Single                 'Minumum allowed pitch [deg]
  ePitchMax As Single                 'Maximum allowed pitch [deg]
  PitchPC As Integer                  'Number of scans flagged as bad for this reason
  
  EditRoll As Single                  'If TRUE, flag scans that violate roll limits
  eRollMin As Single                  'Minumum allowed roll [deg]
  eRollMax As Single                  'Maximum allowed roll [deg]
  RollPC As Integer                   'Number of scans flagged as bad for this reason
  
' The following limits apply to raw data processing and are hard wired in MTPret
  EditNav As Boolean                  'If TRUE, flag scans with bad NAV data
  NavMin As Single                    'Not used
  NavMax As Single                    'Not used
  NavQualPC As Integer                'Number of scans flagged as bad for this reason
  
  EditTA As Boolean                   'If TRUE, flag scans that violate antenna temperature limits
  TAmin As Single                     'Minimum allowed antenna temperature [K]
  TAmax As Single                     'Maximum allowed antenna temperature [K]
  TAqualPC As Integer                 'Number of scans flagged as bad for this reason
' The following is no longer used. Instead we now use the CQV flag in the MA routines
' to set the CtsQual flag, which in turn determines the increments CtsQualPC when
' retrievals are done (see code in Main)
  EditCts As Boolean                  'If TRUE, flag scans with bad Raw Data Counts
  CtsMin As Single                    'Minumum allowed raw counts
  CtsMax As Single                    'Maximum allowed raw counts
  CtsQualPC As Integer                'Number of scans flagged as bad for this reason
  
  EditCycle As Boolean                'If TRUE, raw editting enabled
  CycleQualPC As Integer              'Number of scans flagged as bad for this reason
   
  EditTtgt As Boolean                 'If TRUE, flag scans with bad Target temperatures
  TtgtMin As Single                   'Minumum allowed target temperature [K]
  TtgtMax As Single                   'Maximum allowed target temperature [K]
  TtgtPC As Integer                   'Number of scans flagged as bad for this reason
  
  EditRAWbad As Boolean               'Always TRUE, flag bad Raw tropopause
  RAWbadmin As Single                    '6 km
  RAWbadmax As Single                    '23 km
  RAWbadPC As Integer                 'Number of scans flagged as bad for this reason
  
  EditNlev As Boolean                 'If TRUE, flag scans with too few retrieved levels
  NlevMin As Single                   '8
  NlevMax As Single                   '33
  NlevPC As Integer                   'Number of scans flagged as bad for this reason
  
' OAT Tropopause Substitution Information
  EditOATtrop As Boolean              'If TRUE, use OAT-derived or RAOB tropopause instead of MTP
  OATtropPC As Integer                'Number of scans which have substituted trops
  
  OATzt10 As Single                   'ASCENT trop 1 altitude [km]
  Tzt10 As Single                     'ASCENT trop 1 temperature [K]
  OATzt20 As Single                   'ASCENT trop 2 altitude [km]
  Tzt20 As Single                     'ASCENT trop 2 temperature [K]
  OATks10 As Single                   'UT [ks] to begin substituting trops
  OATks20 As Single                   'UT [ks] to savesubstituting trops
  
  OATzt11 As Single                   'DESCENT trop 1 altitude [km]
  Tzt11 As Single                     'DESCENT trop 1 temperature [K]
  OATzt21 As Single                   'DESCENT trop 2 altitude [km]
  Tzt21 As Single                     'DESCENT trop 2 temperature [K]
  OATks11 As Single                   'UT [ks] to begin substituting trops
  OATks21 As Single                   'UT [ks] to stop substituting trops
  
  OATzt12 As Single                   'DIP 1 trop 1 altitude [km]
  Tzt12 As Single                     'DIP 1 trop 1 temperature [K]
  OATzt22 As Single                   'DIP 1 trop 2 altitude [km]
  Tzt22 As Single                     'DIP 1 trop 2 temperature [K]
  OATks12 As Single                   'UT [ks] to begin substituting trops
  OATks22 As Single                   'UT [ks] to stop substituting trops
  
  OATzt13 As Single                   'DIP 2 trop 1 altitude [km]
  Tzt13 As Single                     'DIP 2 trop 1 temperature [K]
  OATzt23 As Single                   'DIP 2 trop 2 altitude [km]
  Tzt23 As Single                     'DIP 2 trop 2 temperature [K]
  OATks13 As Single                   'UT [ks] to begin substituting trops
  OATks23 As Single                   'UT [ks] to stop substituting trops
  
' HISTORY information
  CALversion As Date                  'Date CAL file was last modified
  MAKEversion As Date                 'Date MTPret.exe file was last modified
  EDITversion As Date                 'Date MTPedit.exe file was last modified
  FLTINFOversion As Date              'Date MTPinfo.exe file was last modified
  SP1(1 To 2) As Date                 'Place Holders for expansion
  FIversion As Integer                'FLTinfo version
  spa As Integer                      'Spare
'
  EditRate As Boolean
  RateMin As Single
  RateMax As Single
  RatePC As Integer
'
  OATzt14 As Single                   'Sub trop 1 altitude [km]
  Tzt14 As Single                     'Sub 2 trop 1 temperature [K]
  OATzt24 As Single                   'Sub 2 trop 2 altitude [km]
  Tzt24 As Single                     'Sub 2 trop 2 temperature [K]
  
  Tstamp As Date                      'Timestamp of last analysis
  RAWstamp As Date                    'Timestamp of RAW data file
  MMSstamp As Date                    'Timestamp of MMS file
  REFstamp As Date                    'Timestamp of REF file from MTPret
  ERFstamp As Date                    'Timestamp of ERF file from MTPedit
  CTCstamp As Date                    'Timestamp of BMP file from MTPctc
  SP2(1 To 5) As Date                 'Place Holders for expansion
  
' FI expansion begins here
  NRC As Integer                      'Number of RC sets
  RCformat(0 To 10) As Integer        'Number integating RC format (normally = 2 for all sets)
  Reg(0 To 15) As String * 5          '5 character string for each RC set used
  RCuse(0 To 15) As Single            'Number of times a give RC has been used in retrieval for a flight
  RCfmt(1 To 5) As Integer            'Rest of RCformat array
End Type
 
Type CFrecord2
' "GENERAL"
  Gendate As String * 8           'Analysis Date
  WCTdates(1 To 10) As String * 8  'Flight used to generate CAL file
  UTstart As Long
  UTend As Long
  Channels As Integer           'Number of frequency channels
  Nel As Integer                'Number of elevation angles
  Emissivity As Single          'Emissivity of window
  Reflectivity As Single        'Reflectivity of window
  DeltaTmin As Single           'Minimum temperature difference between target and sky

' "FIT_INFO"
  Nfit As Integer               'Number of fit parameters (including offset)
  NP(1 To 5) As String * 6      'Alphanumeric fit parameter (eg Tifa, Ttgt)
  GEC(1 To 5, 1 To 5) As Single 'Gain Equation Coefficients (channel, parameter)
  GOF(1 To 5) As Single         'Gain Equation Offsets (Channel)

' "WINDOW_CORRECTIONS"
  WINcor(1 To 3, 1 To 10) As Single  'Window corrections (Channel, El Angle)
  EnableWCT As Boolean
  
' RAW counts editting criteria
  CMAcycles As Integer               'Counts smoothing cycles (slow)
  CMAcycles2 As Integer              'For Sky Counts only (fast), <= CMAcycles
  RFIiterations As Integer           'No of iterations needed to converge (slow)
  RFIiterations2 As Integer          'No of iterations needed to converge (fast)
  BadCycles As Integer               'No of cycles not satisfying RFI threshold (slow)
  Badcycles2 As Integer              'No of cycles not satisfying RFI threshold (fast)
  
  RFIthreshold As Integer            'Maximum counts offset from moving average
  MUXthreshold As Single             'Maximum dT in mux temperatures
  UseMAforCB As Boolean              'Use Moving Average for Base Counts
  UseMAforCS As Boolean              'Use Moving Average for Sky Counts
  UseMAforCN As Boolean              'Use Moving Average for Noise Diode Deflection Counts
  UseMAforTtgt As Boolean            'Use Moving Average for Target Temperature
  UseMAforTifa As Boolean            'Use Moving Average for IF Amp Temperature
  
' Cycle offsets
  aTGToffset(0 To 3) As Integer      'Target Temperature Lead/Lag (+ is lead)
  aMXRoffset(0 To 3) As Integer      'Mixer Temperature Lead/Lag
  aNDoffset(0 To 3) As Integer       'Noise Diode cycle shift
  aNAVoffset(0 To 3) As Single       'OAT cycle shift
  aSpare1(0 To 3) As Single
  aSpare2(0 To 3) As Single

' Gain Thresholds
  GeqnMin(1 To 3) As Single           'Gain Equation Min
  GeqnMax(1 To 3) As Single           'Gain Equation Max
  GnavMin(1 To 3) As Single
  GnavMax(1 To 3) As Single
  GndMin(1 To 3) As Single
  GndMax(1 To 3) As Single

' Channel Weights
  ChInfo(1 To 3) As Single

' Fit Region
  TBfitX1 As Integer
  TBfitX2 As Integer
  TBfitY1 As Integer
  TBfitY2 As Integer
  
  RHS As Boolean                       'TRUE if MTP is on Right Hand Side of a/c
  LocHor As Integer                    'Scan step of horizon (either 5 or 6)
  RAWextension As String * 3           'Extension of file with substituted P/T
  Targets As Integer                   'Number of reference targets
  
' Noise Diode Temperature Fit
  Cnd0(1 To 3) As Single               'Offset
  Cnd1(1 To 3) As Single               'First order coefficient
  Cnd2(1 To 3) As Single               'Second order coefficient
  TrefND As Single                     'Reference temperature (Celcius)
' Instrument Attitude
  MTPyaw As Single
  MTPpitch As Single
  MTProll As Single
  MTPfiduciary As Single               'Scan Mirror Fiduciary Angle
  ElSUI(1 To 10) As Single             'Elevation Angles from SUI file
  fEcCount As Integer                  'Number of scans with Elevation > fEmax
  UseMAforCSgain As Boolean            'Use Moving Average for Sky Counts Based gain
  ScanTime As Single                   'Average length of a scan from fScanLength(m1, m2)
 
  Expand(1 To 84) As Single            'Room for expansion
  
' FI expansion begins here
  NRC As Integer                      'Number of RC sets
  RCformat(0 To 64) As Integer        'Number indicating RC format (normally = 2 for all sets)
                                      'RCformat = 3, for 10*Nlo observables (ie separate horizon T for each freq)
  Reg(0 To 64) As String * 5          '5 character string for each RC set used
  RCuse(0 To 64) As Single            'Number of times a give RC has been used in retrieval for a flight
' Len(CFR2)=1744
' Len(REF2)=2000
' Could add 256/11 or 23 more sets of RCs
'  Placeholder(0 To 63) As Single      'This makes len(CFR2)=2000
  RCformax(65 To 87) As Integer        'Number indicating RC format (normally = 2 for all sets)
                                       'RCformat = 3, for 10*Nlo observables (ie separate horizon T for each freq)
  Rex(65 To 87) As String * 5          '5 character string for each RC set used
  RCusx(65 To 87) As Single            'Number of times a give RC has been used in retrieval for a flight
  Filler As String * 3
End Type

' FLTinfo Data Structure
Type FIrecord2
  Mission As String * 15              'Mission name
  Fileformat As Byte                  'File format (number >=32)
  FltNumber As String * 8             'Flight number
  Pi As String * 80                   'Principal investigator
  Yeer As Integer                     'Year
  Doy As Integer                      'Day of Year
  YYYYMMDD As String * 8              'Year/Month/Day string
  TotalCycles As Integer              'Number of records in file
  Channels As Integer                 'Number of MTP receiver channels
  Drive As String * 2                 'Data drive
  Rdir As String * 8                  'Platform directory (DC8, ER2 or WB57)
  Path As String * 80                 'Flight Directory
  Pgm As String * 80                  'Program Directory
  USE5 As String * 5                  'USE file name for standard errors on trop
  Algorithm As Integer                'Processing Algorithm (e.g., MRI, LAT)
  MRIavg As Single                    'Average MRI value for flight
  MRIrms As Single                    'Standard Deviation on MRI value for flight
  
  utMTPcor As Single                  'UT Correction to ADD to MTP clock [sec]
  DTavg As Single                     'Average value of UTnav-UTmtp for flight
  DTrms As Single                     'Standard deviation of UTnav-UTmtp for flight
  
  ALTfujCONST As Single               'Altitude offset to be applied to NAV altitude
  ALTfujSLOPE As Single               'Slope of NAV altitude correction
  OATnavCOR As Single                 'OAT correction to ADD to NAV temperature
  CalSource As String * 8             'Obsolete
  UseMMSpALT As Boolean               'If TRUE, use MMS pressure altitude in analysis
  
  LAT1 As Single                      'South side of Tropical-to-Mid-latitude transition region [deg]
  LAT2 As Single                      'North side of Tropical-to-Mid-latitude transition region [deg]
  LAT3 As Single                      'South side of Mid-latitude-to-Polar transition region [deg]
  LAT4 As Single                      'North side of Mid-latitude-to-Polar transition region [deg]
  UserLATs As Boolean                 'If FALSE, use default values for transition regions
  
  DoAll As Boolean                    'If TRUE, perform end-to-end data analysis
  DoAllMask As Integer                'Mask specifying analysis steps
  NFL As Integer                      'Number of levels for which retrieval coefficients are available
  FLA(1 To 25) As Integer             'Array of retrieval levels [10 m]
  SU  As String * 4                   'String specifying Sensor Unit
  
  OATsource As Integer                'Outside Air Temperature source (e.g., MTP, NAV, MMS)
  GainScale As Integer                'Gain Scale (e.g., Gain equation, OAT, Noise Diode)
  REFsource As Integer                'Reference Source (either OAT or Target)
  
  PgmDrive As String * 2              'Program Drive (obsolete, handled in MTPsetup.ini now)
  EnableCalfile As Boolean            'True if Calfile is in header record
  
  EditTropAlt As Single               'If TRUE, edit allowed range of trop altitudes
  TropAltMin As Single                'Minimum allowed trop altitude [km]
  TropAltMax As Single                'Maximum allowed trop altitude [km]
  TropAltPC As Integer                'Number of scans flagged as bad for this reason
  
  EditRetAlt As Single                'If TRUE, edit pressure altitude range of retrievals
  RetAltMin As Single                 'Minimum allowed retrieval altitude [km]
  RetAltMax As Single                 'Maximum allowed retrieval altitude [km]
  RetAltPC As Integer                 'Number of scans flagged as bad for this reason
  
  EditZtOff As Single                 'If TRUE, flag trops that are too far from aircraft
  ZtOffA As Single                    'Allowed offset of trop ABOVE aircraft
  ZtOffB As Single                    'Allowed offset of trop BELOW aircraft
  ZtOffPC As Integer                  'Number of scans flagged as bad for this reason
  
  EditTemperature As Single           'If TRUE, flag scans which violate temperature limits
  TemperatureMin As Single            'Minimum allowed retrieved temperature [K]
  TemperatureMax As Single            'Maximum allowed retrieved temperature [K]
  TemperaturePC As Integer            'Number of scans flagged as bad for this reason
  
  EditPitch As Single                 'If TRUE, flag scans that violate pitch limits
  ePitchMin As Single                 'Minumum allowed pitch [deg]
  ePitchMax As Single                 'Maximum allowed pitch [deg]
  PitchPC As Integer                  'Number of scans flagged as bad for this reason
  
  EditRoll As Single                  'If TRUE, flag scans that violate roll limits
  eRollMin As Single                  'Minumum allowed roll [deg]
  eRollMax As Single                  'Maximum allowed roll [deg]
  RollPC As Integer                   'Number of scans flagged as bad for this reason
  
' The following limits apply to raw data processing and are hard wired in MTPret
  EditNav As Boolean                  'If TRUE, flag scans with bad NAV data
  NavMin As Single                    'Not used
  NavMax As Single                    'Not used
  NavQualPC As Integer                'Number of scans flagged as bad for this reason
  
  EditTA As Boolean                   'If TRUE, flag scans that violate antenna temperature limits
  TAmin As Single                     'Minimum allowed antenna temperature [K]
  TAmax As Single                     'Maximum allowed antenna temperature [K]
  TAqualPC As Integer                 'Number of scans flagged as bad for this reason
' The following is no longer used. Instead we now use the CQV flag in the MA routines
' to set the CtsQual flag, which in turn determines the increments CtsQualPC when
' retrievals are done (see code in Main)
  EditCts As Boolean                  'If TRUE, flag scans with bad Raw Data Counts
  CtsMin As Single                    'Minumum allowed raw counts
  CtsMax As Single                    'Maximum allowed raw counts
  CtsQualPC As Integer                'Number of scans flagged as bad for this reason
  
  EditCycle As Boolean                'If TRUE, raw editting enabled
  CycleQualPC As Integer              'Number of scans flagged as bad for this reason
   
  EditTtgt As Boolean                 'If TRUE, flag scans with bad Target temperatures
  TtgtMin As Single                   'Minumum allowed target temperature [K]
  TtgtMax As Single                   'Maximum allowed target temperature [K]
  TtgtPC As Integer                   'Number of scans flagged as bad for this reason
  
  EditRAWbad As Boolean               'Always TRUE, flag bad Raw tropopause
  RAWbadmin As Single                    '6 km
  RAWbadmax As Single                    '23 km
  RAWbadPC As Integer                 'Number of scans flagged as bad for this reason
  
  EditNlev As Boolean                 'If TRUE, flag scans with too few retrieved levels
  NlevMin As Single                   '8
  NlevMax As Single                   '33
  NlevPC As Integer                   'Number of scans flagged as bad for this reason
  
' OAT Tropopause Substitution Information
  EditOATtrop As Boolean              'If TRUE, use OAT-derived or RAOB tropopause instead of MTP
  OATtropPC As Integer                'Number of scans which have substituted trops
  
  OATzt10 As Single                   'ASCENT trop 1 altitude [km]
  Tzt10 As Single                     'ASCENT trop 1 temperature [K]
  OATzt20 As Single                   'ASCENT trop 2 altitude [km]
  Tzt20 As Single                     'ASCENT trop 2 temperature [K]
  OATks10 As Single                   'UT [ks] to begin substituting trops
  OATks20 As Single                   'UT [ks] to stop substituting trops
  
  OATzt11 As Single                   'DESCENT trop 1 altitude [km]
  Tzt11 As Single                     'DESCENT trop 1 temperature [K]
  OATzt21 As Single                   'DESCENT trop 2 altitude [km]
  Tzt21 As Single                     'DESCENT trop 2 temperature [K]
  OATks11 As Single                   'UT [ks] to begin substituting trops
  OATks21 As Single                   'UT [ks] to stop substituting trops
  
  OATzt12 As Single                   'DIP 1 trop 1 altitude [km]
  Tzt12 As Single                     'DIP 1 trop 1 temperature [K]
  OATzt22 As Single                   'DIP 1 trop 2 altitude [km]
  Tzt22 As Single                     'DIP 1 trop 2 temperature [K]
  OATks12 As Single                   'UT [ks] to begin substituting trops
  OATks22 As Single                   'UT [ks] to stop substituting trops
  
  OATzt13 As Single                   'DIP 2 trop 1 altitude [km]
  Tzt13 As Single                     'DIP 2 trop 1 temperature [K]
  OATzt23 As Single                   'DIP 2 trop 2 altitude [km]
  Tzt23 As Single                     'DIP 2 trop 2 temperature [K]
  OATks13 As Single                   'UT [ks] to begin substituting trops
  OATks23 As Single                   'UT [ks] to stop substituting trops
  
' HISTORY information
  CALversion As Date                  'Date CAL file was last modified
  MAKEversion As Date                 'Date MTPret.exe file was last modified
  EDITversion As Date                 'Date MTPedit.exe file was last modified
  FLTINFOversion As Date              'Date MTPinfo.exe file was last modified
  SP1(1 To 2) As Date                 'Place Holders for expansion
  FIversion As Integer                'FLTinfo version
  spa As Integer                      'Spare
'
  EditRate As Boolean
  RateMin As Single
  RateMax As Single
  RatePC As Integer
'
  OATzt14 As Single                   'Sub trop 1 altitude [km]
  Tzt14 As Single                     'Sub 2 trop 1 temperature [K]
  OATzt24 As Single                   'Sub 2 trop 2 altitude [km]
  Tzt24 As Single                     'Sub 2 trop 2 temperature [K]
  
  Tstamp As Date                      'Timestamp of last analysis
  RAWstamp As Date                    'Timestamp of RAW data file
  MMSstamp As Date                    'Timestamp of MMS file
  REFstamp As Date                    'Timestamp of REF file from MTPret
  ERFstamp As Date                    'Timestamp of ERF file from MTPedit
  CTCstamp As Date                    'Timestamp of BMP file from MTPctc
  SP2(1 To 5) As Date                 'Place Holders for expansion
  
  Ncts As Integer                     'Number of "Counts" channels
  ATPrange As Single

End Type

Type Test
  Mission As String * 15              'Mission name
  Fileformat As Byte                  'File format
  FltNumber As String * 8             'Flight number
  Pi As String * 80                   'Principal investigator
  Yeer As Integer                     'Year
  Doy As Integer                      'Day of Year
End Type

Type IWG_DS
' IWG1 packet is comma delimited
'  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
'
'                            Num x Parameter          Unit                  Variable
'                            000   Prefix             IWG1
  DandT As Date            ' 001 x Date/Time          yyyy-mm-ddThh:mm:ss   UTsec
  Latitude As Single       ' 002 x Latitude           deg                   Latitude
  Longitude As Single      ' 003 x Longitude          deg                   Longitude
  gALT As Single           ' 004 x GPS_MSL_Alt        m                     gALT, Zg
  wALT As Single           ' 005   WGS_84_Alt         m                     wALT, Zw
  pALT As Single           ' 006 x Press_Alt          ft                    pALT, Zp
  rALT As Single           ' 007   Radar_Alt          ft                    rALT, Zr
  GroundSpeed As Single    ' 008   Grnd_Spd           m/s                   GroundSpeed
  TAS As Single            ' 009   True_Airspeed      m/s                   TAS
  IAS As Single            ' 010   Indicated_Airspeed kts                   IAS
  Mach As Single           ' 011   Mach_Number        ---                   Mach
  Vzac As Single           ' 012   Vert_Velocity      m/s                   Vzac
  Heading As Single        ' 013 x True_Hdg           degt                  Heading
  TrackAngle As Single     ' 014   Track              degt                  TrackAngle
  DriftAngle As Single     ' 015   Drift              deg                   DriftAngle
  Pitch As Single          ' 016 x Pitch              deg                   Pitch
  Roll As Single           ' 017 x Roll               deg                   Roll
  SideSlipAngle As Single  ' 018   Side_slip          deg                   SideSlipAngle
  AttackAngle As Single    ' 019   Angle_of_Attack    deg                   AttackAngle
  OATn As Single           ' 020 x Ambient_Temp       C                     OATn (OATnav = OATn + cTo)
  Tdew As Single           ' 021   Dew_Point          C                     Tdew
  Ttotal As Single         ' 022   Total_Temp         C                     Ttotal
  Pstatic As Single        ' 023   Static_Press       mb                    Pstatic
  Pdynamic As Single       ' 024   Dynamic_Press      mb                    Pdynamic
  Pcabin As Single         ' 025   Cabin_Press        mb                    Pcabin
  Wspd As Single           ' 026 x Wind_Speed         m/s                   Wspd
  Wdir As Single           ' 027 x Wind_Dir           degt                  Wdir
  VWS As Single            ' 028   Vert_Wind_Spd      m/s                   VWS
  SZA As Single            ' 029   Solar_Zenith_Angle deg                   SZA
  SEAC As Single           ' 030   Sun_Elev_AC        deg                   SEAC
  SAG As Single            ' 031   Sun_Az_Grd         degt                  SAG
  SAAC As Single           ' 032   Sun_Az_AC          degt                  SAAC
  Spares(1 To 17) As Single
End Type

Type NAVrecord
'HIPPO-1
'Date , UTC, ACINS, ALT, AT_A, GGALT, GGLAT, GGLON, GGSPD, GSF, MACH_A, PALT_A, Pitch, PS_A, PT_A, QC_A, Roll, TAS_A, THDG, TTFR, TTHR1, TTHR2, TT_A, VSPD,ATFR, ATHR1, ATHR2, ATTACK, pALT, PSFC, SSLIP, TASF, WDC, WSC, XMACH2
'2008-04-30,13:29:41.269,-0.0029799212,1820.7245,24.727966,1724.4001,39.91276,-105.118164,0.0,0.0,0.03119573,1821.0283,-1.0026349,812.765,813.31976,0.55300844,-0.8809662,0.0,217.52313,20.431808,17.903831,18.24286,24.785934,0.018811557,20.431707,17.90373,18.24276,0.0,1819.125,812.8905,0.0,0.45533758,0.0,0.0,1.7573701E-6
'HIPPO-3
'Date , UTC, ACINS, ALT, AT_A, GGALT, GGLAT, GGLON, GGSPD, GSF, MACH_A, PALT_A, Pitch, PS_A, PT_A, QC_A, Roll, TAS_A,THDG, TTHL1, TTHL2, TTHR1, TTHR2, TT_A, VSPD, ATHL1, ATHL2, ATHR1, ATHR2, pALT, PSFC, SSLIP, TASF, WDC, WSC, XMACH2
'2010-04-10,21:06:00,-0.183823,12.481941,29.042074,11.304000,-14.334365,-170.714294,66.888550,67.043289,0.198335,14.878050,-1.095315,1011.450806,1041.396484,27.924776,0.020956,69.288658,60.793900,32.503986,32.266582,32.783104,32.502163,31.704926,-0.692419,29.697296,30.210632,30.289169,30.010517,17.919741,1011.094238,0.790658,71.200035,79.968185,4.607385,0.041720

'                            Num x Parameter          Unit                  Variable      Also NCAR Parameter
'                            000   Prefix             IWG1
  DandT As Date            ' 000 x Date/Time          yyyy-mm-ddThh:mm:ss   UTsec
  Pstatic As Single        ' 001   Static_Press       mb                    Pstatic       PS_A=ADC Static Pressure
  OATnav As Single         ' 002 x Ambient_Temp       K                     OATnav        OATnav = OATn + cTo, AT_A=ADC Ambient Air Temperature
  Pitch As Single          ' 003 x Pitch              deg                   Pitch         Pitch=IRS Aircraft Pitch Angle
  Roll As Single           ' 004 x Roll               deg                   Roll          Roll=IRS Aircraft Roll Angle
  Latitude As Single       ' 005 x Latitude           deg                   Latitude      GGLAT=Reference GPS Latitude
  Longitude As Single      ' 006 x Longitude          deg                   Longitude     GGLON=Reference GPS Longitude
  Heading As Single        ' 007 x True_Hdg           degt                  Heading       THDG=IRS Aircraft True Heading Angle
  gALT As Single           ' 008 x Geometric Ht       m                     gALT, Zg
  pALT As Single           ' 009 x Press_Alt          ft                    pALT, Zp      PALT=NACA Pressure Altitude
  Wspd As Single           ' 010 x Wind_Speed         m/s                   Wspd          WDC=GPS-Corrected Horizontal Wind Direction
  Wdir As Single           ' 011 x Wind_Dir           degt                  Wdir          WSC=GPS-Corrected Horizontal Wind Speed
  UWS As Single            ' 012                      m/s
  VWS As Single            ' 013                      m/s
  WWS As Single            ' 014   Vert_Wind_Spd      m/s                   WWS
  ThetaAC As Single        ' 015
  OATn0 As Single          ' 016 x Ambient_Temp       C                     OATn          OATnav = OATn + cTo, AT_A=ADC Ambient Air Temperature
  OATn1 As Single          ' 017                      C                     OATn1         ATFR=Ambient Temperature, Fast Response (Radome)
  OATn2 As Single          ' 018                      C                     OATn2         ATLH1=Ambient Temperature, Deiced Left
  OATn3 As Single          ' 019                      C                     OATn3         ATLH2=Ambient Temperature, Deiced Left
  OATn4 As Single          ' 020                      C                     OATn4         ATRH1=Ambient Temperature, Deiced Right
  OATn5 As Single          ' 021                      C                     OATn5         ATRH2=Ambient Temperature, Deiced Right
  Mach As Single           ' 022   Mach_Number        ---                   Mach          MACH_A=ADC Mach Number
  Mach2 As Single          ' 023                                                          XMACH2=Aircraft Mach Number Squared
  
  wALT As Single           ' 024   WGS_84_Alt         m                     wALT, Zw      GGALT=Reference GPS Altitude (MSL)
  rALT As Single           ' 025   Radar_Alt          ft                    rALT, Zr
  hALT As Single           ' 026   Geopotential Ht    m                     hALT
  iALT As Single           ' 027                      m                     iALT          ALT=IRS-Computed Aircraft Altitude
  Ptotal As Single         ' 028                      mb                    Ptotal        PT_A=ADC Total Pressure
  Pdynamic As Single       ' 029   Dynamic_Press      mb                    Pdynamic      PT_A-PS_A=ADC Total Pressure - ADC Static Pressure
  GroundSpeed As Single    ' 030   Grnd_Spd           m/s                   GroundSpeed   GGSPD=Reference GPS Ground Speed
  PSFC As Single           ' 031                      mb                                  PSFC=Corrected Static Pressure, Fuselage
  Vzac As Single           ' 032   Vert_Velocity      m/s                   Vzac          VSPD=IRS Vertical Speed
  
  TOATn0 As Single         ' 033                      C                     TOATn         TT_A=ADC Total Air Temperature
  TOATn1 As Single         ' 034                      C                     TOATn1        TTFR=Total Temperature, Fast Response (Radome)
  TOATn2 As Single         ' 035                      C                     TOATn2        TTLH1=Total Temperature, Deiced Right
  TOATn3 As Single         ' 036                      C                     TOATn3        TTLH2=Total Temperature, Deiced Right
  TOATn4 As Single         ' 037                      C                     TOATn4        TTRH1=Total Temperature, Deiced Right
  TOATn5 As Single         ' 038                      C                     TOATn5        TTRH2=Total Temperature, Deiced Right
  Ttotal As Single         ' 039   Total_Temp         C                     Ttotal
  
  ACINS As Single          ' 040                                            ACINS         ACINS=IRS Vertical Acceleration
' GSF As Single            '                                                GSF           GSF=IRS Aircraft Ground Speed
  QC_A As Single           ' 041                                            QC            ADC Impact Pressure
  TAS As Single            ' 042   True_Airspeed      m/s                   TAS           TAS_A=ADC True Air Speed
' TASF As Single           '                                                              TASF=Aircraft True Airspeed, Fuselage
  IAS As Single            ' 043   Indicated_Airspeed kts                   IAS
  TrackAngle As Single     ' 044   Track              degt                  TrackAngle
  DriftAngle As Single     ' 045   Drift              deg                   DriftAngle
  SideSlipAngle As Single  ' 046   Side_slip          deg                   SideSlipAngle SSLIP=Sideslip Angle, Reference
  AttackAngle As Single    ' 047   Angle_of_Attack    deg                   AttackAngle   ATTACK=Attack Angle, Reference
  
  Tdew As Single           ' 048   Dew_Point          C                     Tdew
  Pcabin As Single         ' 049   Cabin_Press        mb                    Pcabin
  SZA As Single            ' 050   Solar_Zenith_Angle deg                   SZA
  SEAC As Single           ' 051   Sun_Elev_AC        deg                   SEAC
  SAG As Single            ' 052   Sun_Az_Grd         degt                  SAG
  SAAC As Single           ' 053   Sun_Az_AC          degt                  SAAC
  Npares(1 To 20) As Single
End Type

Public FIR As FIrecord, CFR As CFrecord
Public FIR2 As FIrecord2, CFR2 As CFrecord2
Public TST As Test, IWG As IWG_DS, NAV As NAVrecord
Function fGetSelectedOption(Opt As Object) As Integer
' Determine which element of an option button control array was selected
' Returns -1 if no control has been selected

' Use : OptSelected = GetSelectedOption(Option1)

  Dim i As Integer

  fGetSelectedOption = -1

  For i = Opt.LBound To Opt.UBound
    If Opt(i).Value Then
      fGetSelectedOption = i
      Exit For
    End If
  Next
End Function
Sub InitializeGainLimits()
    
  Select Case AC$
  Case "ER", "WB"
    Select Case Mission$
    Case "ACCENT2", "ACCENT"
    ' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
      GeqnMax(1) = 45: GeqnMax(2) = 45: GeqnMax(3) = 45   'xxx mjm was 25 for SOLVE
      GeqnMin(1) = 10: GeqnMin(2) = 10: GeqnMin(3) = 10

    ' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
      GnavMax(1) = 45: GnavMax(2) = 45: GnavMax(3) = 45  'was 25 for SOLVE
      GnavMin(1) = 10: GnavMin(2) = 10: GnavMin(3) = 10

    ' Noise Diode MINs were:     MAXs were:
      GndMax(1) = 26: GndMax(2) = 16: GndMax(3) = 26
      GndMin(1) = 5: GndMin(2) = 7: GndMin(3) = 7
    ' Default for gain picture box
    '  txtGain1.Text = 30: txtGain2.Text = 40
    Case Else
    ' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
      GeqnMax(1) = 40: GeqnMax(2) = 40: GeqnMax(3) = 40   'xxx mjm was 25 for SOLVE
      GeqnMin(1) = 10: GeqnMin(2) = 10: GeqnMin(3) = 10

    ' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
      GnavMax(1) = 40: GnavMax(2) = 40: GnavMax(3) = 40  'was 25 for SOLVE
      GnavMin(1) = 10: GnavMin(2) = 10: GnavMin(3) = 10

    ' Noise Diode MINs were:     MAXs were:
      GndMax(1) = 26: GndMax(2) = 26: GndMax(3) = 26
      GndMin(1) = 5: GndMin(2) = 7: GndMin(3) = 7
    ' Default for gain picture box
'      txtGain1.Text = 10: txtGain2.Text = 20
    End Select
    
  Case "DC"
    Select Case Mission$
    Case "SOLVE", "TexAQS", "CAMEX4"
    ' Default scale for Gain picture window
'      txtGain1.Text = 12
'      txtGain2.Text = 17
  
    ' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
      GeqnMax(1) = 14: GeqnMax(2) = 16: GeqnMax(3) = 17
      GeqnMin(1) = 9: GeqnMin(2) = 11: GeqnMin(3) = 12#

    ' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
      GnavMax(1) = 14: GnavMax(2) = 16: GnavMax(3) = 17
      GnavMin(1) = 9: GnavMin(2) = 11: GnavMin(3) = 12

    ' Noise Diode MINs were:     MAXs were:
      GndMax(1) = 24: GndMax(2) = 26: GndMax(3) = 27
      GndMin(1) = 9: GndMin(2) = 11: GndMin(3) = 12

    Case "PEMTB"
'      If chkCh2Only.value = 1 Then   mjmmjm
      ' Default scale for Gain picture window
'        txtGain1.Text = 5
'        txtGain2.Text = 15
  
      ' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
'        GeqnMax(1) = 7.5: GeqnMax(2) = 11: GeqnMax(3) = 11
'        GeqnMin(1) = 5: GeqnMin(2) = 6: GeqnMin(3) = 6

      ' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
'        GnavMax(1) = 7.5: GnavMax(2) = 11: GnavMax(3) = 11
'        GnavMin(1) = 5: GnavMin(2) = 6: GnavMin(3) = 6

      ' Noise Diode MINs were:     MAXs were:
'        GndMax(1) = 26: GndMax(2) = 26: GndMax(3) = 26
'        GndMin(1) = 5: GndMin(2) = 7: GndMin(3) = 9
'      Else
      ' Default scale for Gain picture window
        'txtGain1.Text = 5
        'txtGain2.Text = 9
  
      ' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
        GeqnMax(1) = 7.5: GeqnMax(2) = 8.5: GeqnMax(3) = 8.5
        GeqnMin(1) = 5: GeqnMin(2) = 6: GeqnMin(3) = 6

      ' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
        GnavMax(1) = 7.5: GnavMax(2) = 8.5: GnavMax(3) = 8.5
        GnavMin(1) = 5: GnavMin(2) = 6: GnavMin(3) = 6

      ' Noise Diode MINs were:     MAXs were:
        GndMax(1) = 26: GndMax(2) = 26: GndMax(3) = 26
        GndMin(1) = 5: GndMin(2) = 7: GndMin(3) = 9
'      End If
      
    Case "TOTE_VOTE"
    ' Default scale for Gain picture window
      'txtGain1.Text = 5
      'txtGain2.Text = 11
  
    ' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
      GeqnMax(1) = 9: GeqnMax(2) = 10: GeqnMax(3) = 11
      GeqnMin(1) = 5: GeqnMin(2) = 8: GeqnMin(3) = 9

    ' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
      GnavMax(1) = 9: GnavMax(2) = 10: GnavMax(3) = 11
      GnavMin(1) = 5: GnavMin(2) = 8: GnavMin(3) = 9

    ' Noise Diode MINs were:     MAXs were:
      GndMax(1) = 26: GndMax(2) = 26: GndMax(3) = 26
      GndMin(1) = 5: GndMin(2) = 7: GndMin(3) = 9
  
    Case Else
    ' Default scale for Gain picture window
'      txtGain1.Text = 12
'      txtGain2.Text = 17
  
    ' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
      GeqnMax(1) = 16: GeqnMax(2) = 18: GeqnMax(3) = 20
      GeqnMin(1) = 9: GeqnMin(2) = 11: GeqnMin(3) = 12#

    ' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
      GnavMax(1) = 16: GnavMax(2) = 18: GnavMax(3) = 20
      GnavMin(1) = 9: GnavMin(2) = 11: GnavMin(3) = 12

    ' Noise Diode MINs were:     MAXs were:
      GndMax(1) = 24: GndMax(2) = 26: GndMax(3) = 27
      GndMin(1) = 9: GndMin(2) = 11: GndMin(3) = 12

    End Select
  End Select
'Gain limits
''For i = 1 To Channels
''  GeqnMin(i) = Val(frmFLTINFO.txtGE1(i - 1).Text): GeqnMax(i) = Val(frmFLTINFO.txtGE2(i - 1).Text)
''  GnavMin(i) = Val(frmFLTINFO.txtNV1(i - 1).Text): GnavMax(i) = Val(frmFLTINFO.txtNV2(i - 1).Text)
''  GndMin(i) = Val(frmFLTINFO.txtND1(i - 1).Text): GndMax(i) = Val(frmFLTINFO.txtND2(i - 1).Text)
''Next i

End Sub


Function fCurrentFormNumber(Form$)
Dim i%

For i = 0 To Forms.Count - 1
  If Forms(i).Name = Form$ Then fCurrentFormNumber = i
Next i

End Function

Sub LoadListBox(C As Control, Name$, DecimalPts%, UnitScale!)
Dim i0%, i1%, i2%, lu%, cMd$, V$, INIfile$, fmt$
  
  Select Case Name$
  Case "DC8", "ER2", "WB57", "M55", "NGV": INIfile$ = "C:\MTP\Setup\Missions.INI"
  Case Else: INIfile$ = "C:\MTP\Setup\Hardware.INI"
  End Select
    
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  
  Select Case DecimalPts
  Case 0:: fmt$ = "#0"
  Case 1: fmt$ = "#0.0"
  Case 2: fmt$ = "#0.00"
  Case 3: fmt$ = "#0.000"
  End Select
  
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0

  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, cMd$                       'Read a line
          If cMd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem Format$(Val(cMd$) * UnitScale, fmt$)
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
  
End Sub

Sub LoadListBoxH(C As Control, Name$, DecimalPts%, UnitScale!)
Dim i0%, i1%, i2%, lu%, cMd$, V$, INIfile$, fmt$
  
  INIfile$ = "C:\MTP\Setup\Hardware.INI"
    
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  
  Select Case DecimalPts
  Case 0:: fmt$ = "#0"
  Case 1: fmt$ = "#0.0"
  Case 2: fmt$ = "#0.00"
  Case 3: fmt$ = "#0.000"
  End Select
  
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0

  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, cMd$                       'Read a line
          If cMd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem Format$(Val(cMd$) * UnitScale, fmt$)
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
End Sub


Sub LoadListBoxM(C As Control, Name$, DecimalPts%, UnitScale!)
Dim i0%, i1%, i2%, lu%, cMd$, V$, INIfile$, fmt$
 
  INIfile$ = "C:\MTP\Setup\Missions.INI"
    
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  
  Select Case DecimalPts
  Case 0:: fmt$ = "#0"
  Case 1: fmt$ = "#0.0"
  Case 2: fmt$ = "#0.00"
  Case 3: fmt$ = "#0.000"
  End Select
  
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0

  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, cMd$                       'Read a line
          If cMd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem Format$(Val(cMd$) * UnitScale, fmt$)
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
End Sub

Sub MapTArmsToOBrms(TArms!(), OBrms!(), Channels%, Nel%, LocHor%, ChInfo!())
Dim j%

' Prepare Observable Error (OBrms) vector
  Select Case LocHor
  Case 6
  ' This is final ob order: CH1:1-5, 7-10, CH2:1-5, 7-10, CH3:1-5, 7-10, OAT
    For j = 1 To 5: OBrms(j) = TArms(1, j): Next j
    For j = 7 To 10: OBrms(j - 1) = TArms(1, j): Next j
    If Channels > 1 Then
      For j = 11 To 15: OBrms(j - 1) = TArms(2, j - 10): Next j
      For j = 17 To 20: OBrms(j - 2) = TArms(2, j - 10): Next j
    End If
    If Channels > 2 Then
      For j = 21 To 25: OBrms(j - 2) = TArms(3, j - 20): Next j
      For j = 27 To 30: OBrms(j - 3) = TArms(3, j - 20): Next j
    End If
  Case 5
  ' This is final ob order: CH1:1-4, 6-10, CH2:1-4, 6-10, CH3:1-4, 6-10, OAT
    For j = 1 To 4: OBrms(j) = TArms(1, j): Next j
    For j = 6 To 10: OBrms(j - 1) = TArms(1, j): Next j
    If Channels > 1 Then
      For j = 11 To 14: OBrms(j - 1) = TArms(2, j - 10): Next j
      For j = 16 To 20: OBrms(j - 2) = TArms(2, j - 10): Next j
    End If
    If Channels > 2 Then
      For j = 21 To 24: OBrms(j - 2) = TArms(3, j - 20): Next j
      For j = 26 To 30: OBrms(j - 3) = TArms(3, j - 20): Next j
    End If
  Case Else
    MsgBox "Only valid LocHor values are 5 and 6", vbOKOnly
    Stop
  End Select
' Calculate horizon observable (normally =, might want to wt by ChInfo
' Following is not correct if separate horizon TBs!!!!
' But this code is not used anywhere in RCcalc, MTPsim or
  Select Case Channels
  Case 1: OBrms(10) = TArms(1, LocHor)
  Case 2: OBrms(19) = TArms(1, LocHor)
  Case 3: OBrms(28) = TArms(1, LocHor)
  End Select

End Sub

Sub ReadFLAfile(Platform$, Mission$)
' Read Retrieval Levels in units of 10 m from default .FLA file
Dim i%, lu#, A$

  lu = FreeFile
  Open MNpath$ + "Setup\" + Mission$ + "_FLA.txt" For Input As lu
  i = 0
  Do
    Input #lu, A$
    i = i + 1
    FLA(i) = Val(A$) * 100#
  Loop Until EOF(lu)
  Close lu
  NFL = i
End Sub

Sub ReadFLTINFOfromRegistry()

App.Title = "FLTINFO"
Mission$ = GetSetting(App.Title, "PATH", "Mission", "AVE")
YYYYMMDD$ = GetSetting(App.Title, "PATH", "FlightDate", "")
Rdir$ = "/" + GetSetting(App.Title, "PATH", "Platform") + "/"
Drive$ = GetSetting(App.Title, "PATH", "DefaultDataDrive", "C:")
Pgm$ = GetSetting(App.Title, "PATH", "DefaultProgramPath")
Path$ = GetSetting(App.Title, "PATH", "DefaultDataPath")
TotalCycles = GetSetting(App.Title, "PATH", "TotalCycles", 1)

End Sub

Sub ReadFLTINFO(Filename$)
Dim i0%, i1%, i2%, lu%, cMd$, V$, FixPath As Boolean
Dim M%

'Avoid using this sub
  lu% = FreeFile
  Open Filename$ For Input As lu%
  FIsize% = 0
  FixPath = False
Do
NextCategory:
  If EOF(lu%) Then GoTo Exit_Sub

  FIsize% = FIsize% + 1
  Input #lu%, cMd$                       'Read a line
  If FIsize% = 2 Then                    'Extract PI$
    Pi$ = Right$(cMd$, Len(cMd$) - 7)
    'Debug.Print Pi$
  End If
  If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
    i0% = InStr(2, cMd$, "]")
    If i0% = 0 Then
      'Print "Right Bracket not found. Fix line number ", FIsize%
      Stop
    End If

    Select Case Mid$(cMd$, 2, i0% - 2)
    Case "CALIBRATION"
      Do
        If EOF(lu%) Then GoTo Exit_Sub
        FIsize% = FIsize% + 1
        Input #lu%, cMd$                       'Read a line
        'Debug.Print cmd$
        If Len(cMd$) = 0 Then GoTo NextCategory
        i1% = InStr(1, cMd$, "=")
        If i1% > 0 Then                     'Look for a command line
          i2% = InStr(i1%, cMd$, " ")    'and end of its value
          If i2% = 0 Then i2% = Len(cMd$) + 1
          V$ = Mid$(cMd$, i1% + 1, i2% - i1% - 1)
          Select Case Left$(cMd$, i1% - 1)
          Case "RegNr": RegNr = Int(Val(V$))
          Case "Reg0$": Reg0$ = V$
          Case "Reg1$": Reg1$ = V$
          Case "Reg2$": Reg2$ = V$
          Case "Reg3$"  'This corrects old INI files which used 1,2,3 not 0,1,2
            Reg0$ = Reg1$
            Reg1$ = Reg2$
            Reg2$ = V$
          Case "USE5$": USE5$ = V$
          Case "Algorithm": Algorithm = V$
          Case "utMTPcor": utMTPcor = Val(V$)
          Case "DTavg": DTavg = Val(V$)
          Case "DTrms": DTrms = Val(V$)
          Case "MRIavg": MRIavg = Val(V$)
          Case "MRIrms": MRIrms = Val(V$)
          Case "ALTfujCONST": ALTfujCONST = Val(V$)
          Case "ALTfujSLOPE": ALTfujSLOPE = Val(V$)
          Case "OATnavCOR": OATnavCOR = Val(V$)
          Case "CalSource$": CalSource$ = V$
          Case "useMMSpALT": UseMMSpALT = V$
          Case "LAT1": LAT1 = Val(V$)
          Case "LAT2": LAT2 = Val(V$)
          Case "LAT3": LAT3 = Val(V$)
          Case "LAT4": LAT4 = Val(V$)
          Case "UserLATn$": UserLATn$ = V$
          Case "UserLATs": UserLATs = V$
          Case Else
          End Select
        End If
      Loop

    Case "PATH"
      Do
        If EOF(lu%) Then GoTo Exit_Sub
        FIsize% = FIsize% + 1
        Input #lu%, cMd$                       'Read a line
        'PRINT cmd$
        If Len(cMd$) = 0 Then GoTo NextCategory

        i1% = InStr(1, cMd$, "=")
        If i1% > 0 Then                     'Look for a command line
          i2% = InStr(i1%, cMd$, " ")   'and end of its value
          If i2% = 0 Then i2% = Len(cMd$) + 1
          V$ = Mid$(cMd$, i1% + 1, i2% - i1% - 1)
         'Debug.Print cmd$
          Select Case Left$(cMd$, i1% - 1)
          Case "Mission$": Mission$ = V$
          Case "yymmdd$"
            YYYYMMDD$ = "19" + V$  'Fix old format
            FixPath = True
          Case "yyyymmdd$": YYYYMMDD$ = V$
          Case "TotalCycles": TotalCycles = Val(V$)
          Case "Drive$"
            Drive$ = V$
'            If Drive$ <> Left$(filename$, 2) Then Drive$ = Left$(filename$, 2)
          Case "Path$"
            Path$ = V$
            If FixPath Then
              M = InStr(1, Path$, Mission$)
              M = M + 1
              M = InStr(M, Path$, "\")
              If Mid$(Path$, M + 1, 1) = "9" Then
                Path$ = Left$(Path$, M) + YYYYMMDD$ + "\"
                RAWstamp = Tstamp
                MMSstamp = Tstamp
                REFstamp = Tstamp
                ERFstamp = Tstamp
                CTCstamp = Tstamp
              End If
            End If
          Case "Rdir$": Rdir$ = V$
          Case "Pgm$": Pgm$ = V$
          Case Else
          End Select
        End If
      Loop
      
    Case "LIMITS"
      Do
        If EOF(lu%) Then GoTo Exit_Sub
        FIsize% = FIsize% + 1
        Input #lu%, cMd$                       'Read a line
        'PRINT cmd$
        If Len(cMd$) = 0 Then GoTo NextCategory

        i1% = InStr(1, cMd$, "=")
        If i1% > 0 Then                     'Look for a command line
          i2% = InStr(i1%, cMd$, " ")       'and end of its value
          If i2% = 0 Then i2% = Len(cMd$) + 1
          V$ = Mid$(cMd$, i1% + 1, i2% - i1% - 1)
          'Debug.Print cmd$
          Select Case Left$(cMd$, i1% - 1)
          
          Case "EditTropAlt": EditTropAlt = V$
          Case "TropAltMin": TropAltMin = Val(V$)
          Case "TropAltMax": TropAltMax = Val(V$)
          Case "TropAltPC": TropAltPC = Val(V$)
          
          Case "EditRetAlt": EditRetAlt = V$
          Case "RetAltMin": RetAltMin = Val(V$)
          Case "RetAltMax": RetAltMax = Val(V$)
          Case "RetAltPC": RetAltPC = Val(V$)
          
          Case "EditTemperature": EditTemperature = V$
          Case "TemperatureMin": TemperatureMin = Val(V$)
          Case "TemperatureMax": TemperatureMax = Val(V$)
          Case "TemperaturePC": TemperaturePC = Val(V$)
         
          Case "EditZtOff": EditZtOff = V$
          Case "ZtOffA": ZtOffA = Val(V$)
          Case "ZtOffB": ZtOffB = Val(V$)
          Case "ZtOffPC": ZtOffPC = Val(V$)
          
          Case "EditPitch": EditPitch = V$
          Case "ePitchMin": ePitchMin = Val(V$)
          Case "ePitchMax": ePitchMax = Val(V$)
          Case "PitchPC": PitchPC = Val(V$)
          
          Case "EditRoll": EditRoll = V$
          Case "eRollMin": eRollMin = Val(V$)
          Case "eRollMax": eRollMax = Val(V$)
          Case "RollPC": RollPC = Val(V$)
          
          Case "RAWbadPC": RAWbadPC = V$
          Case "NlevPC": NlevPC = V$
          
          Case Else
          End Select
        End If
      Loop
    Case "OATtrop"
      Do
        If EOF(lu%) Then GoTo Exit_Sub
        FIsize% = FIsize% + 1
        Input #lu%, cMd$                       'Read a line
        'Debug.Print cmd$
        If Len(cMd$) = 0 Then GoTo NextCategory

        i1% = InStr(1, cMd$, "=")
        If i1% > 0 Then                     'Look for a command line
          i2% = InStr(i1%, cMd$, " ")       'and end of its value
          If i2% = 0 Then i2% = Len(cMd$) + 1
          V$ = Mid$(cMd$, i1% + 1, i2% - i1% - 1)
          'Debug.Print cmd$
          Select Case Left$(cMd$, i1% - 1)
          Case "EditOATtrop": EditOATtrop = V$
          Case "OATtropPC": OATtropPC = V$
          
          Case "OATzt10": OATzt10 = V$
          Case "Tzt10": Tzt10 = V$
          Case "OATzt11": OATzt11 = V$
          Case "Tzt11": Tzt11 = V$
          Case "OATzt12": OATzt12 = V$
          Case "Tzt12": Tzt12 = V$
          Case "OATzt13": OATzt13 = V$
          Case "Tzt13": Tzt13 = V$
          
          Case "OATzt20": OATzt20 = V$
          Case "Tzt20": Tzt20 = V$
          Case "OATzt21": OATzt21 = V$
          Case "Tzt21": Tzt21 = V$
          Case "OATzt22": OATzt22 = V$
          Case "Tzt22": Tzt22 = V$
          Case "OATzt23": OATzt23 = V$
          Case "Tzt23": Tzt23 = V$
        
          Case "OATks10": OATks10 = V$
          Case "OATks11": OATks11 = V$
          Case "OATks12": OATks12 = V$
          Case "OATks13": OATks13 = V$
          
          Case "OATks20": OATks20 = V$
          Case "OATks21": OATks21 = V$
          Case "OATks22": OATks22 = V$
          Case "OATks23": OATks23 = V$
          
          Case Else
          End Select
        End If
      Loop
    
    Case "VERSION"
      Do
        If EOF(lu%) Then GoTo Exit_Sub
        FIsize% = FIsize% + 1
        Input #lu%, cMd$                       'Read a line
        'PRINT cmd$
        If Len(cMd$) = 0 Then GoTo NextCategory

        i1% = InStr(1, cMd$, "=")
        If i1% > 0 Then                     'Look for a command line
          i2% = InStr(i1%, cMd$, " ")   'and end of its value
          If i2% = 0 Then i2% = Len(cMd$) + 1
          V$ = Mid$(cMd$, i1% + 1, i2% - i1% - 1)
          Select Case Left$(cMd$, i1% - 1)
          Case "vCF$": vCF$ = V$
          Case "vD1$": vD1$ = V$
          Case "vD2$": vD2$ = V$
          Case "vMP$": vMP$ = V$
          Case "vFR$": vFR$ = V$
          Case "vFW$": vFW$ = V$
          Case "vDT$": vDT$ = V$
          Case Else
          End Select
        End If
      Loop
    Case "HISTORY"
      Do
        If EOF(lu%) Then GoTo Exit_Sub
        FIsize% = FIsize% + 1
        Input #lu%, cMd$                       'Read a line
        'PRINT cmd$
        If Len(cMd$) = 0 Then GoTo NextCategory

        i1% = InStr(1, cMd$, "=")
        If i1% > 0 Then                     'Look for a command line
          i2% = InStr(i1%, cMd$, " ")   'and end of its value
          If i2% = 0 Then i2% = Len(cMd$) + 1
          V$ = Mid$(cMd$, i1% + 1, i2% - i1% - 1)
          Select Case Left$(cMd$, i1% - 1)
          Case "Tstamp": Tstamp = V$
          Case "RAWstamp": RAWstamp = V$
          Case "REFstamp": REFstamp = V$
          Case "ERFstamp": ERFstamp = V$
          Case "CTCstamp": CTCstamp = V$
          Case Else
          End Select
        End If
      Loop
      
      Case Else
    End Select

  End If

Loop
Exit_Sub:
  Close (lu%)
  AC$ = Mid$(Rdir$, 2, 2)
  Root$ = Drive$ + Rdir$
  Path$ = Root$ + Mission$ + "\" + YYYYMMDD$ + "\"  'Just in case!
  yymmdd$ = Right$(YYYYMMDD$, 6)
End Sub

Sub LoadComboBox(C As Control, Name$)
Dim i0%, i1%, i2%, lu%, cMd$, V$, INIfile$
  
  Select Case Name$
  Case "DC8", "ER2", "WB57", "M55", "NGV": INIfile$ = "C:\MTP\Setup\Missions.INI"
  Case Else: INIfile$ = "C:\MTP\Setup\Hardware.INI"
  End Select
    
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0
  C.Clear
  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, cMd$                       'Read a line
          If cMd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem cMd$
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
  

End Sub

Sub LoadComboBoxH(C As Control, Name$)
Dim i0%, i1%, i2%, lu%, cMd$, V$, INIfile$
  
  INIfile$ = "C:\MTP\Setup\Hardware.INI"
  
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0
  C.Clear
  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, cMd$                       'Read a line
          If cMd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem cMd$
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
  

End Sub

Sub LoadComboBoxM(C As Control, Name$)
Dim i0%, i1%, i2%, lu%, cMd$, V$, INIfile$
  
  INIfile$ = "C:\MTP\Setup\Missions.INI"
    
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0
  C.Clear
  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, cMd$                       'Read a line
          If cMd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem cMd$
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
  

End Sub

Sub MapOBtoTA(iRC%, TA!(), ob!(), Channels%, Nel%)
Dim i%, j%
  
  If (Nobs Mod 10) <> 0 Then
    For i = 1 To Channels
      For j = 1 To Nel
        Select Case j + 10 * (i - 1)
        Case Is < 6: TA(i, j) = ob(j)
        Case Is = 6: TA(i, j) = ob(28)
        Case Is < 11: TA(i, j) = ob(j - 1)
        Case Is < 16: TA(i, j) = ob(j + 9 * (i - 1))
        Case Is = 16: TA(i, j) = ob(28)
        Case Is < 21: TA(i, j) = ob(j - 1 + 9 * (i - 1))
        Case Is < 26: TA(i, j) = ob(j + 9 * (i - 1))
        Case Is = 26: TA(i, j) = ob(28)
        Case Is < 31: TA(i, j) = ob(j - 1 + 9 * (i - 1))
        End Select
      Next j
    Next i
    
  Else
    For i = 1 To Channels
      For j = 1 To 10
        TA(i, j) = ob(j + (i - 1) * 10)
      Next j
    Next i
  
  End If
  
End Sub

Sub MapTAtoOB(TA!(), ob!(), Channels%, Nel%, LocHor%, ChInfo!())
Dim j%

  If (Nobs Mod 10) <> 0 Then 'Assume all RCs are same format
  ' Prepare Observable Vector:
    Select Case LocHor
    Case 6
  ' This is final ob order: CH1:1-5, 7-10, CH2:1-5, 7-10, CH3:1-5, 7-10, OAT
      For j = 1 To 5: ob(j) = TA(1, j): Next j
      For j = 7 To 10: ob(j - 1) = TA(1, j): Next j
      If Channels > 1 Then
        For j = 11 To 15: ob(j - 1) = TA(2, j - 10): Next j
        For j = 17 To 20: ob(j - 2) = TA(2, j - 10): Next j
      End If
      If Channels > 2 Then
        For j = 21 To 25: ob(j - 2) = TA(3, j - 20): Next j
        For j = 27 To 30: ob(j - 3) = TA(3, j - 20): Next j
      End If
    Case 5
  ' This is final ob order: CH1:1-4, 6-10, CH2:1-4, 6-10, CH3:1-4, 6-10, OAT
      For j = 1 To 4: ob(j) = TA(1, j): Next j
      For j = 6 To 10: ob(j - 1) = TA(1, j): Next j
      If Channels > 1 Then
        For j = 11 To 14: ob(j - 1) = TA(2, j - 10): Next j
        For j = 16 To 20: ob(j - 2) = TA(2, j - 10): Next j
      End If
      If Channels > 2 Then
        For j = 21 To 24: ob(j - 2) = TA(3, j - 20): Next j
        For j = 26 To 30: ob(j - 3) = TA(3, j - 20): Next j
      End If
    Case Else
      MsgBox "Only valid LocHor values are 5 and 6", vbOKOnly
      Stop
    End Select
  ' Calculate horizon observable
    Select Case Channels
    Case 1: ob(10) = TA(1, LocHor)
    Case 2
      ob(19) = (ChInfo(1) * TA(1, LocHor) + ChInfo(2) * TA(2, LocHor)) / (ChInfo(1) + ChInfo(2))
    Case 3
      ob(28) = (ChInfo(1) * TA(1, LocHor) + ChInfo(2) * TA(2, LocHor) + ChInfo(3) * TA(3, LocHor)) / (ChInfo(1) + ChInfo(2) + ChInfo(3))
    End Select

  Else  'Separate horizon observables for each channel
    For j = 1 To 10: ob(j) = TA(1, j): Next j
    For j = 1 To 10: ob(10 + j) = TA(2, j): Next j
    For j = 1 To 10: ob(20 + j) = TA(3, j): Next j

  End If
  
End Sub

Sub UpdateMissionsINI(Platform$, Mission$)
Dim INPlu%, OUTlu%, MissionsINI$, NewMissionsINI$
Dim i0%, cMd$
  
  INPlu% = FreeFile
  MissionsINI$ = "C:\MTP\Setup\Missions.INI"
  Open MissionsINI$ For Input As INPlu%
  OUTlu% = FreeFile
  NewMissionsINI$ = "C:\MTP\Setup\NewMissions.INI"
  Open NewMissionsINI$ For Output As OUTlu%

  Do
    If EOF(INPlu%) Then GoTo Exit_Sub
    Input #INPlu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Platform$ Then   'Found Platform
        Print #OUTlu, cMd$                'Platform
        Input #INPlu, cMd$                'First mission for platform
        If cMd$ <> Mission$ Then Print #OUTlu, Mission$  'Don't add if already there
        Print #OUTlu, cMd$
        Do
          Input #INPlu%, cMd$                      'Copy rest of file
          Print #OUTlu, cMd$
        Loop Until cMd$ = "[EOF]"
      Else
        Print #OUTlu, cMd$
        Do
          Input #INPlu%, cMd$                      'Copy rest of file
          Print #OUTlu, cMd$
        Loop Until cMd$ = ""
      End If
    End If
  Loop Until cMd$ = "[EOF]"
 
Exit_Sub:
  Close OUTlu, INPlu
  
  If Len(Dir("C:\MTP\Setup\Missions.SAV")) > 0 Then Kill "C:\MTP\Setup\Missions.SAV"
  Name MissionsINI$ As "C:\MTP\Setup\Missions.SAV"
  If Len(Dir("C:\MTP\Setup\Missions.INI")) > 0 Then Kill MissionsINI$
  Name NewMissionsINI$ As MissionsINI$
  
End Sub

Sub WriteFLTINFO(Filename$)

Dim i0%, i1%, i2%, lu%, FI$

lu% = FreeFile
Open Filename$ For Output As lu%
FIsize% = 0
AC$ = Mid$(Rdir$, 2, 2)
Path$ = Drive$ + Rdir2$ + Mission$ + "\" + YYYYMMDD$ + "\"
FI$ = Path$ + AC$ + YYYYMMDD$ + ".INI"

Call PrintStr(lu%, "' " + FI$ + " was last written on " + Date$ + " at " + Time$, "", "")

If Asc(Left$(Pi$, 1)) <> 32 Then
  Call PrintStr(lu%, "' PI$: " + Pi$, "", "")
Else
  Call PrintStr(lu%, "' PI$: MJ MAHONEY (m.j.mahoney@jpl.nasa.gov) & Bruce GARY (bgary@jpl.nasa.gov)", "", "")
End If

Call PrintStr(lu%, "' This INI-file follows a few simple but mandatory format requirements.", "", "")
Call PrintStr(lu%, "' Settings are entered into categories which occupy a single line and", "", "")
Call PrintStr(lu%, "' must be bracketed by square brackets (e.g. [PATH]). Variables for which", "", "")
Call PrintStr(lu%, "' settings are assigned must have exactly the same name in the program in", "", "")
Call PrintStr(lu%, "' which they are used and be followed by an equal sign (=). Spaces are", "", "")
Call PrintStr(lu%, "' not allowed. Anything beyond the first space in a line is ignored.", "", "")
Call PrintStr(lu%, "' Blank lines are also ignored, but are required betweeen categories.", "", "")
Call PrintStr(lu%, "", "", "")

Call PrintStr(lu%, "[PATH]", "", "")
Call PrintStr(lu%, "Mission$", Mission$, "Mission Name")
Call PrintStr(lu%, "yyyymmdd$", YYYYMMDD$, "Flight Date")
Call PrintVal(lu%, "TotalCycles", TotalCycles, "Number of scans in REF file")
Call PrintStr(lu%, "Drive$", Drive$, "Default system hard drive")
Call PrintStr(lu%, "Rdir$", Rdir$, "Default path from root directory")
Call PrintStr(lu%, "Path$", Path$, Drive$ + Rdir2$ + Mission$ + "\yyyyddmm\")
Call PrintStr(lu%, "Pgm$", Pgm$, "Program file directory path")
Call PrintStr(lu%, "", "", "")

Call PrintStr(lu%, "[CALIBRATION]", "", "")
Call PrintStr(lu%, "Reg0$", Reg$(0), "Tropical RC")
Call PrintStr(lu%, "Reg1$", Reg$(1), "Mid-Latitude RC")
Call PrintStr(lu%, "Reg2$", Reg$(2), "Polar Rc")
Call PrintVal(lu%, "RegNr", RegNr, "RC coefficient index")
Call PrintStr(lu%, "USE5$", USE5$, "USE file for Standard Errors")
Call PrintVal(lu%, "Algorithm", Algorithm, "Retrieval Algorithm Index")

Call PrintVal(lu%, "MRIavg", MRIavg, "Average Meridional Region Index")
Call PrintVal(lu%, "MRIrms", MRIrms, "RMS Meridional Region Index")
Call PrintVal(lu%, "utMTPcor", utMTPcor, "Correction to be ADDED to MTP clock [sec]")
Call PrintVal(lu%, "DTavg", DTavg, "Average D1.BAS NAV-MTP error [sec]")
Call PrintVal(lu%, "DTrms", DTrms, "Variance of NAV-MPT error [sec]")
Call PrintVal(lu%, "ALTfujCONST", ALTfujCONST, "Offset and")
Call PrintVal(lu%, "ALTfujSLOPE", ALTfujSLOPE, "Slope of fit to Nav altitude")
Call PrintVal(lu%, "OATnavCOR", OATnavCOR, "Correction to be ADDED to Nav OAT if no MMS data [K]")
Call PrintStr(lu%, "CalSource$", CalSource$, "T-Calibration Source")
Call PrintBol(lu%, "useMMSpALT", UseMMSpALT, "IF CalSource=MMS, use MMS pALT is True")
Call PrintVal(lu%, "LAT1", LAT1, "Beginning and")
Call PrintVal(lu%, "LAT2", LAT2, "End of tropical/mid-latitude transition region [deg]")
Call PrintVal(lu%, "LAT3", LAT3, "Beginning and")
Call PrintVal(lu%, "LAT4", LAT4, "End of mid-latitude/polar transition region [deg]")
Call PrintStr(lu%, "UserLATn$", UserLATn$, "N=Default, Y=User transition regions")
Call PrintBol(lu%, "UserLATs", UserLATs, "False=Default, True=User transition regions")
Call PrintStr(lu%, "", "", "")

Call PrintStr(lu%, "[LIMITS]", "", "")
Call PrintBol(lu%, "EditTropAlt", EditTropAlt, "Limit tropopause range [km]?")
Call PrintVal(lu%, "TropAltMin", TropAltMin, "Minimum acceptable tropopause solution [km]")
Call PrintVal(lu%, "TropAltMax", TropAltMax, "Maximum acceptable tropopause solution [km]")
Call PrintVal(lu%, "TropAltPC", TropAltPC, "Percent of scans edited for this reason")

Call PrintBol(lu%, "EditRetAlt", EditRetAlt, "Limit allowed retrieved altitude [km]?")
Call PrintVal(lu%, "RetAltMin", RetAltMin, "Minimum allowed retrieved altitude [km]")
Call PrintVal(lu%, "RetAltMax", RetAltMax, "Maximum allowed retrieved altitude [km]")
Call PrintVal(lu%, "RetAltPC", RetAltPC, "Percent of scans edited for this reason")

Call PrintBol(lu%, "EditZtOff", EditZtOff, "Limit A/C offset from Trop [km]?")
Call PrintVal(lu%, "ZtOffA", ZtOffA, "Minimum (Trop-pALT) a/c is Above trop [km]")
Call PrintVal(lu%, "ZtOffB", ZtOffB, "Maximum (Trop-pALT) if a/c is Below trop [km]")
Call PrintVal(lu%, "ZtOffPC", ZtOffPC, "Percent of scans edited for this reason")

Call PrintBol(lu%, "EditTemperature", EditTemperature, "Limit allowed Temperature range [K]?")
Call PrintVal(lu%, "TemperatureMin", TemperatureMin, "Minumum allowed Temperature [K]")
Call PrintVal(lu%, "TemperatureMax", TemperatureMax, "Maximum allowed Temperature [K]")
Call PrintVal(lu%, "TemperaturePC", TemperaturePC, "Percent of scans edited for this reason")

Call PrintBol(lu%, "EditPitch", EditPitch, "Limit allowed A/C Pitch [K]?")
Call PrintVal(lu%, "ePitchMin", ePitchMin, "Minimum allowed A/C Pitch [K]")
Call PrintVal(lu%, "ePitchMax", ePitchMax, "Maximum allowed A/C Pitch [K]")
Call PrintVal(lu%, "PitchPC", PitchPC, "Percent of scans edited for this reason")

Call PrintBol(lu%, "EditRoll", EditRoll, "Limit allowed A/C Roll [K]?")
Call PrintVal(lu%, "eRollMin", eRollMin, "Minimum allowed A/C Roll [K]")
Call PrintVal(lu%, "eRollMax", eRollMax, "Maximum allowed A/C Roll [K]")
Call PrintVal(lu%, "RollPC", RollPC, "Percent of scans edited for this reason")

Call PrintVal(lu%, "RAWbadPC", RAWbadPC, "Percent of scans edited for this reason")
Call PrintVal(lu%, "NlevPC", NlevPC, "Percent of scans edited for this reason")

Call PrintStr(lu%, " ", "", "")


Call PrintStr(lu%, "[OATtrop]", "", "")
Call PrintBol(lu%, "EditOATtrop", EditOATtrop, "Use OAT tropopauses on ascent/descent/dips?")
Call PrintVal(lu%, "OATtropPC", OATtropPC, "Percent of scans with OAT tropopauses")

Call PrintVal(lu%, "OATzt10", OATzt10, "OAT tropopause 1 on ascent [km]")
Call PrintVal(lu%, "Tzt10", Tzt10, "Temperature at tropopause 1 on ascent [km]")
Call PrintVal(lu%, "OATzt20", OATzt20, "OAT tropopause 2 on ascent [km]")
Call PrintVal(lu%, "Tzt20", Tzt20, "Temperature at tropopause 2 on ascent [km]")
Call PrintVal(lu%, "OATks10", OATks10, "UT at beginning of ascent [ks]")
Call PrintVal(lu%, "OATks20", OATks20, "UT at end of ascent [ks]")

Call PrintVal(lu%, "OATzt11", OATzt11, "OAT tropopause 1 on descent [km]")
Call PrintVal(lu%, "Tzt11", Tzt11, "Temperature at tropopause 1 on descent [km]")
Call PrintVal(lu%, "OATzt21", OATzt21, "OAT tropopause 2 on descent [km]")
Call PrintVal(lu%, "Tzt21", Tzt21, "Temperature at tropopause 2 on descent [km]")
Call PrintVal(lu%, "OATks11", OATks11, "UT as beginning of descent [ks]")
Call PrintVal(lu%, "OATks21", OATks21, "UT as end of descent [ks]")

Call PrintVal(lu%, "OATzt12", OATzt12, "OAT tropopause 1 on dip1 [km]")
Call PrintVal(lu%, "Tzt12", Tzt12, "Temperature at tropopause 1 on dip1 [km]")
Call PrintVal(lu%, "OATzt22", OATzt22, "OAT tropopause 2 on dip1 [km]")
Call PrintVal(lu%, "Tzt22", Tzt22, "Temperature at tropopause 2 on dip1 [km]")
Call PrintVal(lu%, "OATks12", OATks12, "UT as beginning of dip1 [ks]")
Call PrintVal(lu%, "OATks22", OATks22, "UT as end of dip1 [ks]")

Call PrintVal(lu%, "OATzt13", OATzt13, "OAT tropopause 1 on dip2 [km]")
Call PrintVal(lu%, "Tzt13", Tzt13, "Temperature at tropopause 1 on dip2 [km]")
Call PrintVal(lu%, "OATzt23", OATzt23, "OAT tropopause 2 on dip2 [km]")
Call PrintVal(lu%, "Tzt23", Tzt23, "Temperatrue at tropopause 2 on dip2 [km]")
Call PrintVal(lu%, "OATks13", OATks13, "UT as beginning of dip2 [ks]")
Call PrintVal(lu%, "OATks23", OATks23, "UT as end of dip2 [ks]")
Call PrintStr(lu%, " ", "", "")

Call PrintStr(lu%, "[VERSION]", "", "")
Call PrintStr(lu%, "vCF$", vCF$, "CALFILE date")
Call PrintStr(lu%, "vD1$", vD1$, "D1.BAS program version")
Call PrintStr(lu%, "vD2$", vD2$, "D2.BAS program version")
Call PrintStr(lu%, "vMP$", vMP$, "RET_MP.BAS program version")
Call PrintStr(lu%, " ", "", "")

Call PrintStr(lu%, "[HISTORY]", "", "")
Call PrintDate(lu%, "Tstamp", Tstamp, "Time tag for FLTINFO")
Call PrintDate(lu%, "RAWstamp", RAWstamp, "Time tag for writing RAW file")
Call PrintDate(lu%, "REFstamp", REFstamp, "Time tag for retrieval to REF file")
Call PrintDate(lu%, "ERFstamp", ERFstamp, "Time tag for editing REF file to ERF file")
Call PrintDate(lu%, "CTCstamp", CTCstamp, "Time tag for plotting CTC")
Call PrintStr(lu%, " ", "", "")

Close (lu%)

End Sub

Sub PrintDate(lu%, x1$, X As Date, x2$)
Dim fmt$, L%
  
  If X = 0 Then
    If lu% = 0 Then Debug.Print x1$ Else Print #lu%, x1$
  Else
    fmt$ = x1$ + "=" + Format$(X, "yyyy mm dd hh:mm:ss")
    L% = 30 - Len(fmt$)
    If L% < 0 Then L% = 0
    fmt$ = fmt$ + Space$(L%) + "'" + x2$
    If lu% = 0 Then Debug.Print fmt$ Else Print #lu%, fmt$
  End If
End Sub

Sub PrintStr(lu%, x1$, X$, x2$)
' Print to lu%, x1$=x$         'x2$
' eg            PgmDrive$=c:   'Program Drive
Dim fmt$, L%

  If X$ = "" Then
    If lu% = 0 Then Debug.Print x1$ Else Print #lu%, x1$
  Else
    fmt$ = x1$ + "=" + X$
    L% = 30 - Len(fmt$)
    If L% < 0 Then L% = 0
    fmt$ = fmt$ + Space$(L%) + "'" + x2$
    If lu% = 0 Then Debug.Print fmt$ Else Print #lu%, fmt$
  End If

End Sub


Sub PrintVal(lu%, x1$, X, x2$)
Dim fmt$, xx$, L%

  x1$ = x1$ + "="
  X = Int(1000 * X) / 1000
  xx$ = Str$(X)
  Select Case X
  Case Is < -1
  Case Is < 0: xx$ = "-0" + Right$(X, Len(xx$) - 1)
  Case Is = 0: xx$ = "+0"
  Case Is < 1: xx$ = "+0" + Right$(X, Len(xx$) - 1)
  Case Else: xx$ = "+" + Right$(X, Len(xx$) - 1)
  End Select
  fmt$ = x1$ + xx$
  L% = 30 - Len(fmt$)
  fmt$ = fmt$ + Space$(L%) + "'" + x2$
  If lu% = 0 Then Debug.Print fmt$ Else Print #lu%, fmt$
End Sub
Sub PrintBol(lu%, x1$, X As Boolean, x2$)
Dim fmt$, L%
  If X Then
    fmt$ = x1$ + "=" + "True"
  Else
    fmt$ = x1$ + "=" + "False"
  End If
    L% = 30 - Len(fmt$)
    fmt$ = fmt$ + Space$(L%) + "'" + x2$
    If lu% = 0 Then Debug.Print fmt$ Else Print #lu%, fmt$

End Sub
Sub WriteFLTINFOtoRegistry()

App.Title = "FLTINFO"
SaveSetting App.Title, "PATH", "Mission", Mission$
SaveSetting App.Title, "PATH", "FlightDate", YYYYMMDD$
SaveSetting App.Title, "PATH", "Platform", Mid$(Rdir$, 2, Len(Rdir$) - 1)
SaveSetting App.Title, "PATH", "DefaultDataDrive", Drive$
SaveSetting App.Title, "PATH", "DefaultProgramPath", Pgm$
SaveSetting App.Title, "PATH", "DefaultDataPath", Drive$ + Rdir2$ + Mission$ + "\yyyyddmm\"
SaveSetting App.Title, "PATH", "TotalCycles", TotalCycles

End Sub

Sub UpdateFltDates(f As Form)
Dim lu%, A$, FltDateFile$, FltDate&, FlightNo&, i%, LI%, n%, M%, FlightDate$
Dim FltDateFileOld$


With f
  .cboFltDates.Clear
  .cboFltNos.Clear
  .cboObjectives.Clear
  
  FltDateFile$ = fSetupFileName("NUM")
  lu = FreeFile
  Open FltDateFile$ For Input As lu
  i = -1
  Do: Line Input #lu, A$  'FlightNo, FlightDate, Objective$
    A$ = LTrim(A$)
    n = InStr(1, A$, " ")
    FlightNo = Val(Left$(A$, n))
    A$ = Trim(Right$(A$, Len(A$) - n + 1))
    n = InStr(1, A$, " ")
    If n = 0 Then
      FlightDate = Val(A$)
      Objective$ = "Unknown"
    Else
      FlightDate = Val(Left$(A$, n))
      Objective$ = Trim(Mid$(A$, n, Len(A$) - n + 1))
    End If
    .cboFltDates.AddItem Str(FlightDate)
    .cboFltNos.AddItem Format(FlightNo, "000000")
    .cboObjectives.AddItem Objective$
    i = i + 1
    If YYYYMMDD$ = Trim(Str(FlightDate)) Then LI = i
  Loop Until EOF(lu)
  .cboFltDates.ListIndex = LI
  .cboFltNos.ListIndex = LI
  .cboObjectives.ListIndex = LI
End With
Close lu
  
End Sub

Function fSetupFileName(Ext$) As String
' Ext$ is extension without a dot
' If new setup filename doesn't exist, check for the old name
Dim FileOld$, File$

  FileOld$ = DataDrive$ + "\MTP\Data\" + frmFLTINFO.cboPlatforms.Text + "\" + frmFLTINFO.cboMissions.Text _
    + "\" + frmFLTINFO.cboMissions.Text + "." + Ext$
  File$ = DataDrive$ + "\MTP\Data\" + frmFLTINFO.cboPlatforms.Text + "\" + _
    frmFLTINFO.cboMissions.Text + "\Setup\" + frmFLTINFO.cboMissions.Text + "_" + Ext$ + ".txt"
' See if Mission_Ext.txt file exists. If not, check to see if Mission.Ext file exists.
  If Len(Dir(File$)) = 0 Then
    ' Check if original Mission.Ext file exits
    If Len(Dir(FileOld$)) = 0 Then
      MsgBox "Neither" + File$ + " or " + vbCrLf + FileOld$ + " exist!", vbOKOnly
      Exit Function
    Else
      File$ = FileOld$
    End If
  End If
  fSetupFileName = File$

End Function



