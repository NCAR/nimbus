Attribute VB_Name = "MTPio"
Option Explicit

' modSleep - Use API which allows other processes to continue
  Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

  Public RAOBcount%           'Number of RAOBs used to calculate RC set
  Public LR1!                 'LR extension from top to zLRb
  Public LR2!                 'LR extension from zLRb to 50 km
  Public Tcp!                 'Cold point temperature
  Public Zcp!                 'Cold point pressure altitude
  Public zLRb!                'Altitude (km) of first LR break
  Public RAOBbias!            'Bias added to RAOB before calculating RCs
  Public RecordStep%          'Step between sampled RAOBs (normally 1)
  Public MinimumRAOBz!        'Minimum acceptable RAOB burst altitude (km)
  Public ExcessTamplitude!    'Maximum T increment to ground emission
  Public SURC$                'SU used to calculate RCs (determines IF bandpass)
  Public RCtemplate$(0 To 64) 'RC RAOB Template (eg BGDH__2003010912.RAOB2)
  Public RAp!(1 To 150)       'Pressure Altitude array passed to plot routine
  Public RTp!(1 To 150)       'Temperature array passed to plot routine
  Public RTp2!(1 To 150)      'Second Temperature array passed to plot routine
  Public RTp3!(1 To 150)      'Third Temperature array passed to plot routine
  Public RealTime As Boolean  'TRUE for real time data analysis
  Public ReadREF As Boolean   '
  Public Ceiling!             'Aircraft service ceiling (used to flag bad Zp)
  Public UTsecMTPlast&        '
  Public UTsecNAVlast&        '
  Public hhmmssNavLast$       '
  Public hhmmssTO$            '
  Public ALTkmft!             '
  Public ALTkmLast!           'mjmmjm
  Public TTOavg!              'Average Time Tag Offset
  Public SerialIO As Boolean  'TRUE if RS-232 data being read from Serial Port
  Public Gnav!(1 To 3)        '
  Public Gmms!(1 To 3)        '
' Flags indicating existence of lines in RAW file scan
  Public Aline%, Bline%, Cline%, Dline%, Eline%, Fline%
  Public M01line%, M02line%, Ptline%, IWGline%
  Public Region$              'Region in RC calculation
  Public pALTlast!            'Needed if no DADS data on ground
  Public FoundSmatrix As Boolean  'TRUE if RC cfg file has Smatrix
  Public LineCurr$(1 To 7)    'Current A, B, C, d, D, E lines
  Public LineLast$(1 To 7)    'Last A, B, C, d, D, E lines
  Public LineDiff$(1 To 7)    'Counts Differences
  
' CAL File Parameters
  Public Tgref!, Tndref!, g1nom!, g2nom!, g3nom!, NDO1nom!, NDO2nom!, NDO3nom!
  Public g1slope!, g2slope!, g3slope!, nd1slope!, nd2slope!, nd3slope!
  Public Gnom!(1 To 3), Gslope!(1 To 3), NDnom!(1 To 3), NDslope!(1 To 3)
  
  Public BP!(0 To 64, 1 To 15, 1 To 33)   'Retrieval pressure levels

  Public StatusBit%(0 To 7)
  Public LT1%                             'Level of RAOB tropopause #1
  Public Lt2%                             'Level of RAOB tropopause #2
  Public Record%                          'Current Record
  Public yymmddMTP$                       'MTP clock yymmdd
  Public hhmmssMTP$                       'MTP clock hhmmss
  Public hhmmssNAV$                       'NAV clock hhmmss
  Public RequestATPupdate As Boolean      'If TRUE, update ATP form

' Aircraft Altitudes
  Public pALTft!                          'Pressure Altitude (ft)
  Public gALTft!                          'Geometric Altitude (ft)
  Public rALTft!                          'Radar Altitude (ft)
  Public mALTft!                          'MMS Pressure Altitude (ft)
'
  Public RawCycle%
  Public GoodCycle%
  Public BadCycle%
  Public Cyclestats%(1 To 5)         ' 1=Filerecords    2=Used records
                                     ' 3=GoodScans      4=GoodTrops
                                     ' 5=GoodScans and GoodTrops
' Next scan parameters
  Public NpALT!
  Public NpALTft!
  Public nOATn!
  Public nOATnav!
  Public NALTkm!
  Public NALTft!
  Public NALTkmu!
  Public NALTcor!
  Public nOAT!
  
  Public asterisk!
  Public Rdate As String * 8
  Public FUTdate As String * 8
  
' Mux Parameters
  Public Ttgt!, Tnd!, Tlo1!, Tlo2!, Tifa!, Ttgtlo!, Ttgthi!, ACCp!, Tmtr!, Tmxr!
  Public Twin!, Vref!, Tdc1!, Tdc2!, Vps5!, Vps12!, ACCm!, Tlo!, Thi!, dACC!

' NGV Mux Parameters
  Public Vm15!, Vp05!, Vp15!, Vsyn!, Vmtr!, Vp08!, Vvid!, Vm08!
  Public Tdat!, Tair!, Tsmp!, Tpsp!, Tnc!, Tsyn                   'Tmtr!, ACCp!
  Public R350!, Ttg1!, Ttg2!, Tmix!, Tamp!, R600!                 'Twin!, Tnd!

' Mapping between old mux parameters and new NGV parameters
' New    Old
'        Vps12
'        ACCm
' 00 Ttgt = Ttgt
' 01 Vm15 =      = Analog circuits on controller board and in receiver
' 02 Vp05 = Vps5 = +5V logic (there is a second +5V to T control boards)
' 03 Vp15 =      = Controller board (IF and LO amplifiers)
' 04 Vsyn =      = 24V for LO Synthesizer
' 05 Vmtr =      = 24V for Stepper Motor
' 06 Vp08 =      = Regulated +15 V for MUX
' 07 Vvid =      = Video signal
' 08 Vm08 =      = Regulated -15 V for MUX
' 09 Tdat =      = T data controller board
' 10 Tair =      = T fairing air
' 11 Tsmp =      = T scan motor plate
' 12 Tpsp = Tdc1 = T power supplies
' 13 Tnc  = Tdc2 = Tcpu
' 14 Tsyn = Tlo2
' 15 Tmtr = Tmtr
' 16 ACCp = ACCp = for now. Later will add firmware to calculate max and min
' 17 R350 =
' 18 Ttg1 = Ttgtlo
' 19 Ttg2 = Ttgthi
' 20 Twin = Twin
' 21 Tmix = Tifa
' 22 Tamp = Tlo1
' 23 Tnd  = Tnd
' 24 R600 =
  Public RT!(1 To 33)            'Retrieved temperature profile (K)
  Public RA!(1 To 33)            'Retrieval altitudes (km)
  Public CB%(1 To 3)             'Base (Target) Counts
  Public CBL%(1 To 3)            'Base Counts Last Record
  Public CB2%(1 To 3)            'Base (Target) Counts for second target
  Public CB2L%(1 To 3)           'Base Counts Last Record for second target
  Public CS%(1 To 3, 1 To 10)    'Sky Counts
  Public CSL%(1 To 3, 1 To 10)   'Sky Counts Last Record
  Public dND%(1 To 3)            'Noise Diode Deflection
  Public dNDL%(1 To 3)           'Noise Diode Deflection Last
  Public dND2%(1 To 3)           'Noise Diode Deflection for second target
  Public dND2L%(1 To 3)          'Noise Diode Deflection
  Public CN%(1 To 3)             'Noise Diode Counts
  Public CNL%(1 To 3)            'Noise Diode Counts
  Public CN2%(1 To 3)            'Noise Diode Counts for second target
  Public CN2L%(1 To 3)           'Noise Diode Counts
  Public CSP%(1 To 3)            'Not Used
  Public CSM%(1 To 3)            'Not Used
  Public CSMA%(1 To 3, 1 To 10)  'CS Moving Average
  Public CSMAL%(1 To 3, 1 To 10) 'CS Moving Average Last Record
  Public TD1#(1 To 200)          'First derivative of T profile
  Public TD2#(1 To 200)          'Second derivative of T profile
  Public RTavg!(33)              'Average temperature profile
  Public RAavg!(33)              'Average T profile altitudes
  
' Parameters used in REFrecord (Definitions are in the data structure)
  Public GoodScan As Boolean, GoodTrop As Boolean, Cycle%, Nlev%, Nlev1%, Nlev2%, TTO1!, TTO2!, TTO3!
  Public TTO!, EditWord%, MakeWord%, UTsecLast&, UTsecMTP&, UTsecNav&, HHMMSS$

' Outside Air Temperatures
  Public OATn!                            'A-line OAT (Celcius), OATnav (Kelvin)
  Public OATbest!                         'Best estimate of OAT
  Public OAT!                             'cTo + OATn + OATnavCOR
' OATmtp, OATnav, and OATmms are defined below with REF data structure parameters
  
  Public UTsec&, Pitch!, Roll!, Latitude!, Longitude!, Heading!, TAS!, Wspd!, Wdir!, rALT!, mALT!, pALT!, gALT!, hALT!
' Optional new parameters provided in default IWG packet (see UDP module)
' Public GroundSpeed!, TrackAngle!, AttackAngle!, SideSlipAngle!, wALT!
' Public DriftAngle!, IAS!, Zgps!, Mach!, Vzac!, Tdew!, Ttotal!, Mach2!
' Public Pstatic!, Pdynamic!, Pcabin!, VWS!, SZA!, SEAC!, SAG!, SAAC!
  Public Zt1!, Zt2!, TT1!, TT2!, Th1!, Th2!, SEp1!, SEp2!, SEm1!, SEm2!
  Public OATmtp!, OATnav!, OATmms!, LRac!, Zice!, dTice!, Elcor!, MRI!, MRI2!, ZtIPV!, IPVt!
  Public RCformatUsed%, ElCorUsed!, LRac2!, LRac3!, RFImask%(1 To 5), StatusBits%
  Public Tmin5!, Zmin5!, RCindex1%, RCindex2%, TwoAAprofiles As Boolean
  Public g!(1 To 3), Geqn!(1 To 3), Gnd!(1 To 3), Goat!(1 To 3), TA!(1 To 3, 1 To 10)
  Public zzz%(1 To 33), TTT!(1 To 33), TSE%(1 To 33), Zgeo%(1 To 33), ND%(1 To 33), TTTT%(1 To 33)
  Public Muxs!(0 To 32), C%(1 To 3, 1 To 14), Mux%(1 To 32)
  Public TTMA!, TMMA!, TWMA!, TOMA!, CMA%(1 To 3, 1 To 14)
  Public RCfmt%(1 To 5)       '
  Public sZZZ!(1 To 33), sTTT!(1 To 33), sTSE!(1 To 33), sZg!(1 To 33), sND!(1 To 33)
  Public TSEi!(0 To 64, 1 To 33)
  
  Public Tptz!(1 To 100000)  'Array of 1 Hz in situ P, T, Z data
  Public Pptz!(1 To 100000)
  Public Zptz!(1 To 100000)
  Public iPTZ&              'Number of elements
  Public UTptz&             'UT Tptz(1) - 1 second
  Public Te!(1 To 2000, 0 To 3)

' Additional Parameters used in MPRecord
  Public OK As Boolean, ALTkm!, Tac!, SE1!, Theta1!
  
' Additional Parameters used in REFlimitsRecord
  Public UTmin&, UTmax&, Mins!(1 To 40), Maxs!(1 To 40), NV%, VSCAL!(0 To 4)
  Public UTtakeoff&, UTlanding&, Tzgm!, Ztgm!, Thgm!, UTgm As Long
                
' Minimum and Maximum Parameter Values for a flight
  Public UTksmin!, UTksmax!, LATmin!, LATmax!, LONmin!, LONmax!
  Public Pitchmin!, PitchMax!, Rollmin!, RollMax!, pALTmin!, pALTmax!
  Public Tacmin!, Tacmax!, LRmin!, LRmax!, Tmin!, Tmax!, Zmin!, Zmax!
  Public Zt1min!, Zt1max!, PT1min!, PT1max!, T1min!, T1max!, TSEmin!, TSEmax!
  Public Zt2min!, Zt2max!, PT2min!, PT2max!, T2min!, T2max!
  Public zTmin!, ZtMax!, PTmin!, PTmax!, gALTmin!, gALTmax!
  Public GoodRecords%, Badrecords%, NretMin%, NretMax%
  Public zgeomax!, zgeomin!, NDmax!, NDmin!, MRImin!, MRImax!
  
' Retrieval Coefficients
  Public sOBrms!(1 To 30)           'Apriori 1 sigma errors
  Public sOBav!(1 To 30)            'AA observables
  Public sBP!(1 To 33)              'pressure levels
  Public sRTav!(1 To 33)            'AA T profile
  Public sRMSa!(1 To 33)            'Variance on AA T Profile
  Public sRMSe!(1 To 33)            'Formal Retrieval errors
  Public Src!(1 To 33, 1 To 30)     '33 retrieval levels, 30 observables
  Public CHnLSBloss!(1 To 3)        'Chn LSB RF loss percentage
  Public CH1LSBloss!                'Replaced by above line
'-------------------------------------------------------------------------
' Define Data Structures

' Random Everything File (REF) data type
  Type REFrecord
    GoodScan As Boolean                 'Good Scan based on editting criteria
    GoodTrop As Boolean                 'Good tropopause
    Cycle As Integer                    'Record number in REF/ERF file
    Nlev As Integer                     'Total number of retrieved levels: 1 < Nlev< 33
    Nlev1 As Integer                    'Lowest level retrieved
    Nlev2 As Integer                    'Highest level retrieved
    TTO As Single                       'Time Tag Offset
'   MAKEword Bitmap
'   NavQual          1                  'Nav data quality
'   TAqual           2                  'Antenna temperature quality
'   CtsQual          4                  'Raw counts quality
'   CycleQual        8                  'Cycle quality (logical AND of bits 1-3)
'   TtgtFix         16                  'Set if bad Target Temperature due to RFI
'   RAWbad          32                  'Trop outside of allowed limits
    MakeWord As Integer                 'Word containing REFmake quality bits
'   EDITword Bitmap
'   RetAlt           1                  'Data outside retrieved altitude limits
'   Temperature      2                  'Data outside allowed temperature limits
'   OATtrop          4                  'OAT tropopause substituted for MTP tropopause
'   ZtOff            8                  'MTP trop too far from aircraft
'   TropAlt         16                  'MTP trop outside allowed trop altitude range
'   Pitch           32                  'Aircraft pitch outside allowed limits
'   Roll            64                  'Aircraft roll outside allowed limits
'   Nlev < 8       128                  'Number of retrieved levels, Nlev < 8
'   Rate           256                  'pALT rate of change > RateMin
    EditWord As Integer                 'Word containing REFedit quality bits
  
    UTsec As Long                       'UT sec used in analysis (normally UTsecNAV)
    UTsecMTP As Long                    'UT sec kept by MTP Data Unit
    UTsecNav As Long                    'UT sec from NAV
    pALT As Single                      'Pressure altitude [km]
    gALT As Single                      'Geometric altitude [km]
    rALT As Single                      'Radar altitude [km]
    Pitch As Single                     'A/C pitch [deg]
    Roll As Single                      'A/C roll [deg]
    Latitude As Single                  'Latitude [deg]
    Longitude As Single                 'Longitude [deg]
    Heading As Single                   'Heading [deg]
    TAS As Single                       'True Air Speed [m/s]
    Wspd As Single                      'Wind Speed [kts]
    Wdir As Single                      'Wind Direction [deg]
    Zt1 As Single                       'Tropopause 1 altitude [km]
    TT1 As Single                       'Tropopause 1 temperature [K]
    Th1 As Single                       'Tropopause 1 potential temperature [K]
    SEp1 As Single                      'Tropopause 1 Std Error + [km]
    SEm1 As Single                      'Tropopause 1 Std Error - [km]
    Zt2 As Single                       'Tropopause 2 altitude [km]
    TT2 As Single                       'Tropopause 2 temperature [K]
    Th2 As Single                       'Tropopause 2 potential temperature [K]
    SEp2 As Single                      'Tropopause 2 Std Error + [km]
    SEm2 As Single                      'Tropopause 2 Std Error - [km]
    OATmtp As Single                    'Outside air temperature from MTP [K]
    OATnav As Single                    'Outside air temperature from NAV [K]
    OATmms As Single                    'Outside air temperature from MMS/PTW [K]
    LRac As Single                      'Lapse rate at A/C [K/km]
    Zice As Single                      'Altitude for ice formation [km]
    dTice As Single                     'Temperature change to ice formation altitude [K]
    Elcor As Single                     'Sensor Unit elevation scan correction
    MRI As Single                       'Meridional Region Index
    ZtIPV As Single                     'Altitude [km] of IPV tropopause
    IPVt As Single                      'IPV threshold in PVU [=10^(-6) m2/kg/s2]

'   Eventually generalize RCformat to include all details of RCs
    RCformatUsed As Integer             'Retrieval coefficient format
                                        '0= Old DC8 up/down format
                                        '1= ER2 (original)
                                        '2= Natural order, except for horizon
                                        '3= 3 horizon observables
    NFL As Integer                      'Number of Flight Levels
    Nobs As Integer                     'Number of observables
    Nret As Integer                     'Number of retrievables
    Nif As Integer                      'Number of IF channels
  
    mALT As Single                      'MMS altitude (km)
    ElCorUsed As Single
    LRac2 As Single
    LRac3 As Single
    RFImask(5) As Integer
    StatusBits As Integer
    Tmin5 As Single                     'Minimum temperature for flight above 5 km
    Zmin5 As Single                     'Altitude of minimum temperature
    RCindex1 As Integer                 'Index of Reg() used in retrieval
    RCindex2 As Integer                 'Index of second Reg() if used (negative if none)
    TwoAAprofiles As Boolean            'TRUE if there are two AA TB profiles
    
    Spair(1 To 10) As Integer           'Place Holders for expansion
    
    g1 As Single                        'Gains actually used to process data
    g2 As Single
    g3 As Single
  
    G1eqn As Single                     'Gain equation gains
    G2eqn As Single
    G3eqn As Single
  
    G1nd As Single                      'Noise Diode gains
    G2nd As Single
    G3nd As Single
  
    G1oat As Single                     'OAT-derived gains (MTP, NAV or MMS)
    G2oat As Single
    G3oat As Single
  
    TA(1 To 3, 1 To 10) As Single       'Measured antenna temperatures
    zzz(1 To 33) As Integer             'Retrieved pressure altitudes [10 m]
    TTTT(1 To 33) As Integer             'Retrieved temperatures at zzz values [100 mK]
    TSE(1 To 33) As Integer             'Standard errors on TTT values [100 mK]
    Zgeo(1 To 33) As Integer            'Geometric heights corresponding to zzz [km]
    ND(1 To 33) As Integer              'Number density profile [#/m3*E-21]
    Counts(1 To 3, 1 To 12) As Integer  'Raw Counts Data
    Muxs(0 To 16) As Single             'MUX values as real numbers
    Mux(1 To 16) As Integer             'Raw Mux counts
  
    More(1 To 4) As Single              'Place Holders for expansion
    TTMA As Single                      'Ttgt MA
    TMMA As Single                      'Tmxr MA
    TWMA As Single                      'Twin MA
  
    CMA(1 To 3, 1 To 12) As Integer     'Counts Moving Averages
  End Type
 
' MP File Header Record
  Type MPRecord
    OK As Integer
    UTsec As Long
    Nlev As Integer
    pALT As Single
    Pitch As Single
    Roll As Single
    Tac As Single
    Zt1 As Single
    Zt2 As Single
    SE1 As Single
    Theta1 As Single
    Latitude As Single
    Longitude As Single
    LRac As Single
    MRI As Single
    IPV As Single
    Unused(1 To 5) As Integer
  
    a1(1 To 33) As Integer
    a2(1 To 33) As Integer
    a3(1 To 33) As Integer
    a4(1 To 33) As Integer
    A5(1 To 33) As Integer
  End Type

' Limits Record Data Structure
  Type REFlimitsRecord
    UTmin As Long                       'Minimum UT [sec]
    UTmax As Long                       'Maximum UT [sec]
    Mins(1 To 40) As Single             'Array of Min values
    Maxs(1 To 40) As Single             'Array of Max values
    NV As Integer                       'Number of retrieved variables
    VSCAL(0 To 4) As Single             'Scale factors for retrieved variables
    Unused As String * 52               'Place Holder for expansion
    TempArray(1 To 54) As Integer       'Place Holder for expansion
    UTtakeoff As Long                   'Takeoff UT
    UTlanding As Long                   'Landing UT
    Ztgm As Single                      'Altitude of global minimum T
    Tzgm As Single                      'Global minimum T for flight above 5 km
    Thgm As Single                      'Theta at global minimym T
    UTgm As Long                        'UT at time of global minimum T
  End Type

' Declare User-Defined Data Types
  Public A As MPRecord
  Public REF As REFrecord
  Public RLR As REFlimitsRecord
  

' Random Everything File (REF) data type
  Type REF2record
    GoodScan As Boolean                 'Good Scan based on editting criteria
    GoodTrop As Boolean                 'Good tropopause
    Cycle As Integer                    'Record number in REF/ERF file
    Nlev As Integer                     'Total number of retrieved levels: 1 < Nlev< 33
    Nlev1 As Integer                    'Lowest level retrieved
    Nlev2 As Integer                    'Highest level retrieved
    TTO As Single                       'Average Time Tag Offset
    TTO1 As Single                      'Channel 1 TTO
    TTO2 As Single                      'Channel 2 TTO
    TTO3 As Single                      'Channel 3 TTO

'   MAKEword Bitmap
'   NavQual          1                  'Nav data quality
'   TAqual           2                  'Antenna temperature quality
'   CtsQual          4                  'Raw counts quality
'   CycleQual        8                  'Cycle quality (logical AND of bits 1-3)
'   TtgtFix         16                  'Set if bad Target Temperature due to RFI
'   RAWbad          32                  'Trop outside of allowed limits
    MakeWord As Integer                 'Word containing REFmake quality bits
'   EDITword Bitmap
'   RetAlt           1                  'Data outside retrieved altitude limits
'   Temperature      2                  'Data outside allowed temperature limits
'   OATtrop          4                  'OAT tropopause substituted for MTP tropopause
'   ZtOff            8                  'MTP trop too far from aircraft
'   TropAlt         16                  'MTP trop outside allowed trop altitude range
'   Pitch           32                  'Aircraft pitch outside allowed limits
'   Roll            64                  'Aircraft roll outside allowed limits
'   Nlev < 8       128                  'Number of retrieved levels, Nlev < 8
'   Rate           256                  'pALT rate of change > RateMin
    EditWord As Integer                 'Word containing REFedit quality bits
  
    UTsec As Long                       'UT sec used in analysis (normally UTsecNAV)
    UTsecMTP As Long                    'UT sec kept by MTP Data Unit
    UTsecNav As Long                    'UT sec from NAV
    
    pALT As Single                      'Pressure altitude [km]
    gALT As Single                      'Geometric altitude [km]
    rALT As Single                      'Radar altitude [km]
    mALT As Single                      'MMS altitude [km]
    
    Pitch As Single                     'A/C pitch [deg]
    Roll As Single                      'A/C roll [deg]
    Latitude As Single                  'Latitude [deg]
    Longitude As Single                 'Longitude [deg]
    Heading As Single                   'Heading [deg]
    TAS As Single                       'True Air Speed [m/s]
    Wspd As Single                      'Wind Speed [kts]
    Wdir As Single                      'Wind Direction [deg]
    
    Zt1 As Single                       'Tropopause 1 altitude [km]
    TT1 As Single                       'Tropopause 1 temperature [K]
    Th1 As Single                       'Tropopause 1 potential temperature [K]
    SEp1 As Single                      'Tropopause 1 Std Error + [km]
    SEm1 As Single                      'Tropopause 1 Std Error - [km]
    Zt2 As Single                       'Tropopause 2 altitude [km]
    TT2 As Single                       'Tropopause 2 temperature [K]
    Th2 As Single                       'Tropopause 2 potential temperature [K]
    SEp2 As Single                      'Tropopause 2 Std Error + [km]
    SEm2 As Single                      'Tropopause 2 Std Error - [km]
    OATmtp As Single                    'Outside air temperature from MTP [K]
    OATnav As Single                    'Outside air temperature from NAV [K]
    OATmms As Single                    'Outside air temperature from MMS/PTW [K]
    LRac As Single                      'Lapse rate at A/C [K/km]
    LRac2 As Single
    LRac3 As Single
    Zice As Single                      'Altitude for ice formation [km]
    dTice As Single                     'Temperature change to ice formation altitude [K]
    Elcor As Single                     'Sensor Unit elevation scan correction
    ElCorUsed As Single
    MRI As Single                       'Meridional Region Index
    ZtIPV As Single                     'Altitude [km] of IPV tropopause
    IPVt As Single                      'IPV threshold in PVU [=10^(-6) m2/kg/s2]
    Tmin5 As Single                     'Minimum temperature for flight above 5 km
    Zmin5 As Single                     'Altitude of minimum temperature

    g1 As Single                        'Gains actually used to process data
    g2 As Single
    g3 As Single
  
    G1eqn As Single                     'Gain equation gains
    G2eqn As Single
    G3eqn As Single
  
    G1nd As Single                      'Noise Diode gains
    G2nd As Single
    G3nd As Single
  
    G1oat As Single                     'OAT-derived gains (MTP, NAV or MMS)
    G2oat As Single
    G3oat As Single

'   Eventually generalize RCformat to include all details of RCs
    RCformatUsed As Integer             'Retrieval coefficient format
                                        '0= Old DC8 up/down format
                                        '1= ER2 (original)
    
    NFL As Integer                      'Number of Flight Levels
    Nobs As Integer                     'Number of observables
    Nret As Integer                     'Number of retrievables
    Nif As Integer                      'Number of IF channels
  
    RFImask(5) As Integer
    StatusBits As Integer
    RCindex1 As Integer                 'Index of Reg() used in retrieval
    RCindex2 As Integer                 'Index of second Reg() if used (negative if none)
    TwoAAprofiles As Boolean            'TRUE if there are two AA TB profiles
    Algorithm As Integer                'Algorithm used on each retrieval
    Stats(1 To 3, 1 To 2) As Single     'AA Avg, RMS, and Total Error
    
    Spair(1 To 79) As Integer           'Place Holders for expansion
  
    TA(1 To 3, 1 To 10) As Single       'Measured antenna temperatures
    sZZZ(1 To 33) As Single             'Retrieved pressure altitudes [m]
    sTTT(1 To 33) As Single             'Retrieved temperatures at zzz values [K]
    sTSE(1 To 33) As Single             'Standard errors on TTT values [mK]
    sZg(1 To 33) As Single              'Geometric heights corresponding to zzz [m]
    sND(1 To 33) As Single              'Number density profile [#/m3*E-21]
    Counts(1 To 3, 1 To 14) As Integer  'Raw Counts Data
    Muxs(0 To 32) As Single             'MUX values as real numbers
    Mux(0 To 32) As Integer             'Raw Mux counts
    
    CMA(1 To 3, 1 To 14) As Integer     'Counts Moving Averages
    TTMA As Single                      'Ttgt MA
    TMMA As Single                      'Tmxr MA
    TWMA As Single                      'Twin MA
    Tcp As Single                       'Tcold point
    Zcp As Single                       'Zcold point
    More(1 To 95) As Single             'Place Holders for expansion
  
  End Type
  Public REF2 As REF2record

' RC set for one flight level
  Type RC_Set_1FL
    sBP As Single              'Flight level pressure altitude (hPa)
    sOBrms!(1 To 30)           '1-sigma apriori observable errors
    sOBav!(1 To 30)            'Archive Average observables
    sBPrl!(1 To 33)            'Pressure at retrieval levels
    sRTav!(1 To 33)            'Average T at retrieval levels
    sRMSa!(1 To 33)            'Variance in T at retrieval levels
    sRMSe!(1 To 33)            'Formal error in T at retrieval levels
    Src!(1 To 33, 1 To 30)     '33 retrieval levels, 30 observables
    Spare!(1 To 67)
  End Type
  Public RCFL As RC_Set_1FL
  
' RC Configuration Information
  Type RCconfiguration
    RCformat As Integer
    CreationDateTime As Date
    RAOBfilename As String * 80
    RCfilename As String * 80
    RAOBcount As Integer
    LR1 As Single              'LR above top of RAOB
    zLRb As Single             'LR break altitude
    LR2 As Single              'LR above break altitude
    RecordStep As Single       'Record Step through available RAOBs
    RAOBmin As Single          'Minimum acceptable RAOB altitude
    ExcessTamplitude As Single 'Random Excess Noise Level on Ground
    Nobs As Integer            'Number of observables
    Nret As Integer            'Number of retrieval levels
    dZ!(1 To 33)               'Retrieval offset levels wrt flight level
    NFL As Integer             'Number of flight levels
    Zr!(1 To 20)               'Flight levels (km)
    Nlo As Integer             'Number of LO channels
    LO!(1 To 3)                'LO frequencies (GHz)
    Nel As Integer             'Number of elevation angles
    El!(1 To 10)               'Scan mirror elevation angles
    Nif As Integer             'Number of IF frequencies
    IFoff!(1 To 3, 1 To 16)    'IF frequency offsets (GHz)
    IFwt!(1 To 3, 1 To 16)     'Weights assigned to each IF frequency
    Spare!(1 To 130)
    SURC As String * 4         'SU IFB used to calculate RCs (added 20050128)
    CHnLSBloss!(1 To 3)        'CHn LSB RF loss
    RAOBbias As Single         'Bias added to RAOB before calculating RCs
    CH1LSBloss As Single       'CH1 LSB linear RF loss gradient
'   Sensitivity matrix: iRC, NFL, Nlo, Nel
    SmatrixN1!(1 To 15, 1 To 3, 1 To 10)  'Linear term
    SmatrixN2!(1 To 15, 1 To 3, 1 To 10)  'Quadratic term
  End Type
  Public RCFG As RCconfiguration

  Type RCconfiguration2
    RCformat As Integer
    CreationDateTime As Date
    RAOBfilename As String * 80
    RCfilename As String * 80
    RAOBcount As Integer
    LR1 As Single              'LR above top of RAOB
    zLRb As Single             'LR break altitude
    LR2 As Single              'LR above break altitude
    RecordStep As Single       'Record Step through available RAOBs
    RAOBmin As Single          'Minimum acceptable RAOB altitude
    ExcessTamplitude As Single 'Random Excess Noise Level on Ground
    Nobs As Integer            'Number of observables
    Nret As Integer            'Number of retrieval levels
    dZ!(1 To 33)               'Retrieval offset levels wrt flight level
    NFL As Integer             'Number of flight levels
    Zr!(1 To 20)               'Flight levels (km)
    Nlo As Integer             'Number of LO channels
    LO!(1 To 3)                'LO frequencies (GHz)
    Nel As Integer             'Number of elevation angles
    El!(1 To 10)               'Scan mirror elevation angles
    Nif As Integer             'Number of IF frequencies
    IFoff2!(1 To 12)           'IF frequency offsets (GHz)
    IFwt2!(1 To 12)            'Weights assigned to each IF frequency
    SURC As String * 4         'SU - to make sure correct IF is used
    Spare!(1 To 207)
'   Sensitivity matrix: iRC, NFL, Nlo, Nel, bias and slope
    SmatrixN1!(1 To 15, 1 To 3, 1 To 10)
    SmatrixN2!(1 To 15, 1 To 3, 1 To 10)
  End Type
  Public RCFG2 As RCconfiguration2

Sub DecodeGVline(ByVal Line$, Reset As Boolean)
'Instrument on 23:21:18 12-05-2007
'A 23:21:40 12-05-2007
'IWG1,20010920T151645,14.642,-96.4235,4229.12,,4255.74,4235.87,137.172,134.938,209.011,0.162247,0.0558355,185.039,189.068,4.23184,2.98645,0.90837,0.488137,2.95111,5.49085,-7.03531,14.0805,614.777,72.7016,860.533,8.57255,78.2577,0.0569099,0.821413,0.749384,,
'B 009653 013837 014684 009729 013910 014784 010241 014390 015307 010302 014467 015363 010306 014442 015380 010310 014441 015373 010310 014453 015369 010299 014442 015366 010292 014442 015369 009755 013976 014663
'M01: 2943 2109 2884 3097 3086 2954 2479 2936
'M02: 2101 1247 1429 4095 1531 1191 4095 1060
'Pt: 2436 14145 14143 16383 14187 14163 14028 14768
'E 011467 015347 016344 009836 013969 014870

Dim s$(40), n%, i%, j%, T!, Count&, RR!, RT!, R7M0!, MTPtime$, MTPdate$, x$
Dim Ct!(8), r!(8), Tt(8), Voltage!, Vscale!(1 To 8), Tscale!(1 To 8)
Dim AA As Double, Bb As Double, cC As Double, DD As Double, WriteSMdata As Boolean, DeltaC%
Static SMlu%

  WriteSMdata = False  'Set TRUE to write out Scan Motor micro-step data in A-line
  Select Case Left$(Line$, 1)
  Case "A"
  ' A 20071205 23:21:40 002.30 00.05 001.11 00.10 01.30 0.01 267.00 0.20 +35.678 +0.045 -120.987 +0.998 75555 75550
  ' A YYYYMMDD HH:MM:SS Avg    Rms   Avg    Rms   Avg   Rms  Avg    Rms  Avg     Rms    Avg      Rms
  '                     Pitch  Pitch Roll   Roll  Zp    Zp   OAT    OAT  LatAvg  LatRms LonAvg   LonRms SMcmd SMenc
  ' Units               deg    deg   deg    deg   km    km    K     K    deg     deg    deg      deg    --    --
  '          1         2         3         4         5         6         7         8         9        10         11
  ' 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901
  ' Line$ = "A 20071205 23:21:40 002.30 00.05 001.11 00.10 01.30 0.01 267.00 0.20"
    MTPdate$ = Mid$(Line$, 3, 8)
'   yyyymmdd$ = Left$(MTPdate$, 4) + Mid$(MTPdate$, 6, 2) + Right$(MTPdate$, 2)
    MTPtime$ = Mid$(Line$, 12, 8)
'    UTsec = fTstringToSec(MTPtime$, True)
    Line$ = Right$(Line$, Len(Line$) - 21)
    Call ParseString2(Line$, " ", s$(), n%)
    For i = 1 To n
      T = Val(s$(i))                         'Counts
      Select Case i
      Case 1: PitchAvg = T
      Case 2: PitchRms = T
      Case 3: RollAvg = T
      Case 4: RollRms = T
      Case 5: ZpAvg = T
      Case 6: ZpRms = T
      Case 7: OatAvg = T
      Case 8: OatRms = T
      Case 9: LatAvg = T
      Case 10: LatRms = T
      Case 11: LonAvg = T
      Case 12: LonRms = T
      Case 13: SMcmd = T
      Case 14: SMenc = T
      Case Else
        MsgBox "A-line parameter " + Str(i) + "not defined!", vbOKOnly
      End Select
    Next i
    Aline = 1
    
  Case "B"
  ' B 009653 013837 014684 009729 013910 014784 010241 014390 015307 010302 014467 015363 010306 014442 015380 010310 014441 015373 010310 014453 015369 010299 014442 015366 010292 014442 015369 009755 013976 014663
    Call ParseString2(Line$, " ", s$(), n%)
    For j = 1 To Nel
      For i = 1 To Channels
         C(i, j) = Val(s$(i + Channels * (j - 1)))
      Next i
    Next j
    Bline = 1
    
  Case "M"
    Select Case Left$(Line$, 3)
    Case "M01"
'   M01: 2943 2109 2884 3097 3086 2954 2479 2936
      Call ParseString2(Line$, " ", s$(), n%)
      Vscale(8) = -5.1               '-15V PS    Vm15
      Vscale(7) = 2#                 'VCC  PS    Vp05
      Vscale(6) = 5.1                '+15V PS    Vp15
      Vscale(5) = 7.79               '+24V Syn   Vsyn
      Vscale(4) = 7.79               '+24V Step  Vmtr
      Vscale(3) = 2.78               '+8V  PS    Vp08
      Vscale(2) = 1#                 'Video V.   Vvid
      Vscale(1) = -2.73              '-8V  PS    Vm08
      For i = 1 To n
        Mux(i) = Val(s$(i))                         'Counts
        Voltage = Vscale(i) * (Mux(i) / 1000#)      'Voltage
        Muxs(i) = Voltage
        Select Case i
        Case 1: Vm08 = Voltage
        Case 2: Vvid = Voltage
        Case 3: Vp08 = Voltage
        Case 4: Vmtr = Voltage
        Case 5: Vsyn = Voltage
        Case 6: Vp15 = Voltage
        Case 7: Vp05 = Voltage
        Case 8: Vm15 = Voltage
        End Select
      Next i
      M01line = 1
      
    Case "M02"
'   M02: 2101 1247 1429 4095 1531 1191 4095 1060
      AA = 0.0009376: Bb = 0.0002208: cC = 0.0000001276
      Call ParseString2(Line$, " ", s$(), n%)
      ' "Acceler   "    'Acc+
      ' "T Data    "    'Tdat
      ' "T Motor   "    'Tmtr
      ' "T Pod Air "    'Tair
      ' "T Scan    "    'Tsmp
      ' "T Pwr Sup "    'Tpsp
      ' "T N/C     "    'Tnc
      ' "T Synth   "    'Tsyn
      For i = 1 To n
        Select Case i
        Case Is = 1
          Mux(i + 8) = Val(s$(i))
          ACCp = -((Mux(i + 8) * 0.001) - 2.5) / 0.4   ' MMA1250D accellerometer 2.5V +/- .25V @ 0G
          Muxs(i + 8) = ACCp
        Case Else
          Count = Val(s$(i))
          Mux(i + 8) = Count
          If (Count = 4095) Or (Count = 0) Then
            '
          Else
            Count = 4096# - Count
            RR = (1 / (Count / 4096#)) - 1
            RT = 34800# * RR
            Muxs(i + 8) = (1# / (AA + Bb * Log(RT) + cC * Log(RT) ^ 3) - 273.15)
            T = Muxs(i + 8)
            Select Case i + 8
            Case 10: Tdat = T
            Case 11: Tmtr = T
            Case 12: Tair = T
            Case 13: Tsmp = T
            Case 14: Tpsp = T
            Case 15: Tnc = T
            Case 16: Tsyn = T
            End Select
          End If
        End Select
      Next i
    End Select
    M02line = 1
    
  Case "P"
    ' "Rref 350 "   'R350
    ' "Target 1 "   'Ttlo
    ' "Target 2 "   'Tthi
    ' "Window   "   'Twin
    ' "Mixer    "   'Tmix
    ' "Dblr Amp "   'Tamp
    ' "Noise D. "   'Tnd
    ' "Rref 600 "   'R600
    ' R(0) = 350    'rref low
    ' R(7) = 600    'rref hi
    R7M0 = 250#     'R(7)-R(0)
    AA = -244.3364635: Bb = 0.462418: cC = 0.0000588: DD = -0.000000013
'   Pt: 2436 14145 14143 16383 14187 14163 14028 14768
    Call ParseString2(Line$, " " + vbCrLf, s$(), n%)

    For i = 1 To n
      Mux(i + 16) = Val(s$(i))
      Ct(i - 1) = Mux(i + 16)
    Next i
    DeltaC = Ct(7) - Ct(0)
    If DeltaC = 0 Then DeltaC = 12275
    For i = 0 To 7
      r(i) = 350# + R7M0 * (Ct(i) - Ct(0)) / DeltaC
      T = AA + Bb * r(i) + cC * r(i) ^ 2 + DD * r(i) ^ 3
      Muxs(i + 17) = T
      Select Case i
      Case 0: R350 = T
      Case 1: Ttg1 = T
      Case 2: Ttg2 = T
      Case 3: Twin = T
      Case 4: Tmix = T
      Case 5: Tamp = T
      Case 6: Tnd = T
      Case 7: R600 = T
      End Select
    Next i
    Ptline = 1
    Ttgt = (Ttg1 + Ttg2) / 2#
    Muxs(0) = Ttgt
    If WriteSMdata Then
      If SMlu = 0 Then
        SMlu = FreeFile
        Open "c:\mtp\data\ngv\start08\20080515\sm.txt" For Output As SMlu
        x$ = "UTks" + vbTab + "Zp" + vbTab + "Pitch" + vbTab + "Roll" + vbTab + "SMdif" + vbTab + "Tdat" + vbTab + "Tsynth" + vbTab + "Tmix"
        Print #SMlu, x$
      End If
      Print #SMlu, UTsec / 1000; vbTab; ZpAvg; vbTab; PitchAvg; vbTab; RollAvg; vbTab; SMcmd - SMenc; vbTab; Tdat; vbTab; Tsyn; vbTab; Tmix
    End If
    
  Case "E"
'   E 011467 015347 016344 009836 013969 014870
    Call ParseString2(Line$, " " + vbCrLf, s$(), n%)
    For j = Nel + 1 To Ncts
      For i = 1 To Channels
         C(i, j) = Val(s$(i + Channels * (j - Nel - 1)))
      Next i
    Next j
    Eline = 1
    
  Case "I"
    Select Case Left$(Line$, 4)
    Case "IWG1"
'   0    1               2      3        4    5    6      7       8       9       10      11       12        13      14      15      16      17     18        19      20      21       22      23      24      25      26      27      28        29       30      31  32
'   Pref Date     Time   Lat    Lon      gALT wAlt pALT   rALT    GrndSpd TrueAS  IndAS   Mach No  Vert Vel  TrueHdg Track   Drift   Pitch   Roll   SideSlip  AofA    Amb_T   Dewpoint Total T StaticP DynamiP Cabin P Wind Sp WindDir VWS       SZA      SEac    Sz
'        yyyymmdd$ UTsec Lat    Lon      Zg     Zw Zp     Zr      GroundS TAS     IAS     Mach     Vzac      Heading TrackAn DriftAn Pitch   Roll   SideSlipA AttackA OATn    Tdew     Ttotal  Pstatic Pdynamic Pcabin Wspd    Wdir    VWS       SZA      SEAC    SAG SAAC
'   IWG1,20010920T151645,14.642,-96.4235,4229.12,,4255.74,4235.87,137.172,134.938,209.011,0.162247,0.0558355,185.039,189.068,4.23184,2.98645,0.90837,0.488137,2.95111,5.49085,-7.03531,14.0805,614.777,72.7016,860.533,8.57255,78.2577,0.0569099,0.821413,0.749384,,
      Decode_IWG Line$, Reset
      UTsec = UTsecI
'  If frmMTPbin.chkShowChannels(12).Value = 1 Then yyyymmddI$ = Trim(frmFLTINFO.cboFltDates.Text)  '"20100930"
      
      MapGVtoOther               'set x = xI, where x=variable (eg pALT) and I is its value in IWG line from UDP module
      IWGline = 1
    Case Else
      'Instrument on line
      Beep
    End Select
    
  End Select

End Sub
Function fGetRIFstartTime(RIFfile$) As Long
Dim IWGlu%

  IWGlu = FreeFile
  Open RIFfile$ For Random Access Read Write As IWGlu Len = Len(IWG)
  Call IWGread(IWGlu%, 1)
  Close IWGlu
  Call DateTtoStrings(DandT, YYYYMMDD, HHMMSS, UTsec&, False)
  fGetRIFstartTime = UTsec

End Function

Function fGetRAWstartTime(RAWfile$) As Long
Dim A$, lu%, GoToNext As Boolean

  lu = FreeFile
  Open RAWfile$ For Input As lu
  Do
    Line Input #lu, A$
  Loop Until Left$(A$, 1) = "A"
  Close lu
  Call DecodeAline(A$, GoToNext)
  fGetRAWstartTime = UTsec
End Function

Function fGetMMSstartTime(MMfile$) As Long
' MMSfile$  Nav file to read.

Dim i%, j%, A$, HeaderCount%
Dim MMlu%, Tcorrection%, UT&

    MMlu = FreeFile
    Open MMfile$ For Input As MMlu
    Input #MMlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #MMlu, A$: Next i
    Select Case MMfile$
    Case "c:\MTP\Data\ER2\CRYSTAL\MM\MM20020726.ER2": Tcorrection = 1550.84
    Case "c:\MTP\Data\ER2\CRYSTAL\MM\MM20020728.ER2": Tcorrection = 1941.93
    Case Else: Tcorrection = 0
    End Select
    
    Line Input #MMlu, A$
    UT = Val(Left$(A$, 8)) + 0.5
    Close MMlu
    fGetMMSstartTime = UT

End Function

Sub GetECMWFvalues(UTsec&, UT&, P!, T!, PTWfile$)
Dim i%, A$, HeaderCount%, Tf!, Ts!
Static PTWlu%

  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open PTWfile$ For Input As PTWlu
    Input #PTWlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #PTWlu, A$: Next i
  Else
    If EOF(PTWlu) Then
      Close PTWlu
      Exit Sub
    End If
  End If
   
  If UTsec < 0 Then
    Line Input #PTWlu, A$
    UT = Val(Left$(A$, 10))
  Else
    Do  'Catch up
      Line Input #PTWlu, A$
      UT = Val(Left$(A$, 10))
    Loop Until (UT >= UTsec Or EOF(PTWlu))
  End If
  
  If EOF(PTWlu) Then UT = -1: Exit Sub
'     UT(s)   T(C)
'  22020.00   -70.0765    11.1022    -5.1630    -8.9322    -0.0000    43.7356    80.6196    -0.1199     0.0000     0.1755    80.2876   380.8031    86.3063   373.2655    82.4692   377.4995    80.6904   380.0693
'  22030.00   -70.2081    10.9010    -5.1943    -8.9611    -0.0004    43.6481    80.5197    -0.1191     0.0000     0.1657    80.2412   380.9119    86.3128   373.2581    82.4692   377.4954    80.6904   380.0667
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'    Line Input #PTWlu, a$
    
    P = -99999#
    T = Val(Mid$(A$, 13, 9)) + cTo
'Debug.Print UT; P; fPtoZ(P)
End Sub

Sub GetRIFvalues(IWGlu%, IWGrecord&, UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit As Boolean)
' Prefix$ . Nav file 2 letter prefix
' UTsec ... current time, if <0 then just get next record, if >= 0 go to that time
' UT    ... time read from Nav file corresponding to UTsec
' P, T, Th, U, V, W ... pressure, temperature, theta, and winds at UT
' PTWfile$  Nav file to read.
' Quit .... TRUE if EOF

Dim i%, mTh!, mT!, mT2!, mT3!, mT4!, mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!, Wu!, Wv!, Ww!
Dim x!, mMach2!
'    param(1) = P             hPa
'    param(2) = T             K
'    param(3) = mPitch        deg
'    param(4) = mRoll         deg
'    param(5) = mLatitude     deg
'    param(6) = mLongitude    deg
'    param(7) = mHeading      deg
'    param(8) = mZg           km
'    param(9) = mZp           km
'    param(10) = mWspd        kts
'    param(11) = mWdir        deg
'    param(12) = mU           m/s
'    param(13) = mV           m/s
'    param(14) = mW           m/s
'    param(15) = mTheta       K    '                    HIPPO-1    HIPPO-3
'    param(16) = OATn0        K    '
'    param(17) = OATn1        K    'Research T for NGV, ATFR       NA
'    param(18) = OATn2        K    'Research T for NGV, ATHR1      ATHL1
'    param(19) = OATn3        K    'Research T for NGV, ATHR2      ATHL2
'    param(20) = OATn4        K    'Research T for NGV             ATHR1
'    param(21) = OATn5        K    'Research T for NGV             ATHR2
'    param(22) = Mach         -
'    param(23) = Mach2        -
'    param(24) = wALT         km
'    param(25) = rALT         km
'    param(26) = hALT         km
'    param(27) = iALT         km   'IRS altitude
'    param(28) = Ptotal       hPa
'    param(29) = Pdynamic     hPa
'    param(30) = GroundSpeed  m/s
'    param(31) = PSFC         hPa  'Corrected Static Pressure, Fuselage
'    param(32) = Vzac         m/s

'Mach2 was (20) now OATn5
'    param(20)= Mach2         -    'Mach No Squared                XMACH2

  For i = 1 To 50
    param(i) = 99999#
  Next i
  
  Do
    Call IWGread(IWGlu, IWGrecord)
  Loop Until UT >= UTsec
End Sub

Sub GetNGline(NGVlu, UTsec&, P!, T!, mT!, mT2!, mT3!, mT4!, mLatitude!, mLongitude!, mZg!, mZp!, mPitch!, mRoll!, mMach2!)
Dim i%, j%, A$, b$, HeaderCount%, x!, Utime&, TAS!
Static LastT!, LastP!, LastWspd!, LastWdir!, LastZp!, LastZg!, LastUTsec!
    
'Date , UTC, ACINS, ALT, AT_A, GGALT, GGLAT, GGLON, GGSPD, GSF, MACH_A, PALT_A, Pitch, PS_A, PT_A, QC_A, Roll, TAS_A, THDG, TTFR, TTHR1, TTHR2, TT_A, VSPD, ATFR, ATHR1, ATHR2, ATTACK, pALT, PSFC, SSLIP, TASF, WDC, WSC, XMACH2
'2008-04-30,13:29:41.269,-0.0029799212,1820.7245,24.727966,1724.4001,39.91276,-105.118164,0.0,0.0,0.03119573,1821.0283,-1.0026349,812.765,813.31976,0.55300844,-0.8809662,0.0,217.52313,20.431808,17.903831,18.24286,24.785934,0.018811557,20.431707,17.90373,18.24276,0.0,1819.125,812.8905,0.0,0.45533758,0.0,0.0,1.7573701E-6
'1/8/2009,16:32:44,0.374,-999.99,-999.99,12852.19629,42.126431,-105.492157,215.226807,215.09935,-999.99,12652.89063,3.66073,-999.99,-999.99,-999.99,-0.129523,-999.99,323.379913,-28.615019,-28.713249,-29.837944,-25.016117,1.890335,-56.131058,-55.97263,-56.358284,-999.99,12663.59277,173.964905,-0.220948,235.266891,276.022919,31.262674,0.63714
'         1         2         3         4         5
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
' First format is START-08, Second format is HIPPO
    Line Input #NGVlu, A$
    'UTsec , Latitude, Longitude, ALTkm, T1, T2
    'b$ = Left$(A$, 10)
    'Debug.Print A$
    i = InStr(1, A$, ",")
    b$ = Left$(A$, i - 1)
    j = InStr(i + 1, A$, ",")
    HHMMSS$ = Mid$(A$, i + 1, j - i - 1)
    UTsec = fTstringToSec(HHMMSS$, True)
'    If UTsec < LastUTsec Then UTsec = UTsec + 86400#
    i = InStr(j + 1, A$, ",")
    x = Val(Mid$(A$, j + 1, i - j - 1)) 'ACINS
    j = InStr(i + 1, A$, ",")
    x = Val(Mid$(A$, i + 1, j - i)) 'ALT
    i = InStr(j + 1, A$, ",")
    T = Val(Mid$(A$, j + 1, i - j)) + cTo  'AT_A
    j = InStr(i + 1, A$, ",")
    mZg = Val(Mid$(A$, i + 1, j - i)) / 1000# 'GGALT
    i = InStr(j + 1, A$, ",")
    mLatitude = Val(Mid$(A$, j + 1, i - j))  'GGLAT
    j = InStr(i + 1, A$, ",")
    mLongitude = Val(Mid$(A$, i + 1, j - i))   'GGLON
    i = InStr(j + 1, A$, ",")
    x = Val(Mid$(A$, j + 1, i - j))  'GGSPD
    j = InStr(i + 1, A$, ",")
    x = Val(Mid$(A$, i + 1, j - i))  'GSF_A
    i = InStr(j + 1, A$, ",")
    x = Val(Mid$(A$, j + 1, i - j))  'MACH_A
    j = InStr(i + 1, A$, ",")
    mZp = Val(Mid$(A$, i + 1, j - i)) / 1000# 'PALT_A
    i = InStr(j + 1, A$, ",")
    mPitch = Val(Mid$(A$, j + 1, Len(A$) - j))  'Pitch
    j = InStr(i + 1, A$, ",")
    x = Val(Mid$(A$, i + 1, j - i))  'PS_A
    i = InStr(j + 1, A$, ",")
    x = Val(Mid$(A$, j + 1, Len(A$) - j))  'PT_A
    j = InStr(i + 1, A$, ",")
    x = Val(Mid$(A$, i + 1, j - i))  'QC_A
    i = InStr(j + 1, A$, ",")
    mRoll = Val(Mid$(A$, j + 1, Len(A$) - j))  'Roll
    j = InStr(i + 1, A$, ",")
    x = Val(Mid$(A$, i + 1, j - i))  'TAS_A
    i = InStr(j + 1, A$, ",")
    x = Val(Mid$(A$, j + 1, Len(A$) - j))  'THDG
    j = InStr(i + 1, A$, ",")                           'HIPPO-1 HIPPO-3
    x = Val(Mid$(A$, i + 1, j - i))                     'TTFR    TTHL1
    i = InStr(j + 1, A$, ",")
    x = Val(Mid$(A$, j + 1, Len(A$) - j))               'TTHR1   TTHL2
    j = InStr(i + 1, A$, ",")
    x = Val(Mid$(A$, i + 1, j - i))                     'TTHR2   TTHR1
    i = InStr(j + 1, A$, ",")
    x = Val(Mid$(A$, j + 1, Len(A$) - j))               'TT_A    TTHR2
    j = InStr(i + 1, A$, ",")
    x = Val(Mid$(A$, i + 1, j - i))                     'VSPD    TT_A
    i = InStr(j + 1, A$, ",")
    Select Case Mission$
    Case "HIPPO"
      mT = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo      'ATFR
      j = InStr(i + 1, A$, ",")
      mT2 = Val(Mid$(A$, i + 1, j - i)) + cTo           'ATHR1
      i = InStr(j + 1, A$, ",")
      mT3 = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo     'ATHR2
      If T < 0 Then T = mT3
      j = InStr(i + 1, A$, ",")
      x = Val(Mid$(A$, i + 1, j - i))                   'ATTACK
      i = InStr(j + 1, A$, ",")
      mZp = Val(Mid$(A$, j + 1, Len(A$) - j))           'pALT
      j = InStr(i + 1, A$, ",")
      x = Val(Mid$(A$, i + 1, j - i))                   'PSFC
      If x = -999.99 Or x = -32767 Then
        mZp = x / 1000
        P = fZtoP(mZp)
      Else
        mZp = fPtoZ(x)
      End If
      i = InStr(j + 1, A$, ",")
      x = Val(Mid$(A$, j + 1, Len(A$) - j))             'SSLIP
      j = InStr(i + 1, A$, ",")
      x = Val(Mid$(A$, i + 1, j - i))                   'TASF
      i = InStr(j + 1, A$, ",")
      x = Val(Mid$(A$, j + 1, Len(A$) - j))             'WDC
      j = InStr(i + 1, A$, ",")
      x = Val(Mid$(A$, i + 1, j - i))                   'WSC
      i = InStr(j + 1, A$, ",")
      x = Val(Mid$(A$, j + 1, Len(A$) - j))             'XMACH2
    Case Else    '"HIPPO-2", "HIPPO-3", "PREDICT"
      x = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo       'VSPD
      j = InStr(i + 1, A$, ",")
      mT = Val(Mid$(A$, i + 1, j - i)) + cTo            'ATLH1
      i = InStr(j + 1, A$, ",")
      mT2 = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo     'ATLH2
      j = InStr(i + 1, A$, ",")
      mT3 = Val(Mid$(A$, i + 1, j - i)) + cTo           'ATRH1
      i = InStr(j + 1, A$, ",")
      mT4 = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo     'ATRH2
      j = InStr(i + 1, A$, ",")
      x = Val(Mid$(A$, i + 1, j - i))                   'PALT
      i = InStr(j + 1, A$, ",")
      x = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo       'PSFC
'      If X = -999.99 Or X = -32767 Then
'        mZp = x / 1000
'        P = fZtoP(mZp)
'      Else
'        mZp = fPtoZ(X)
'      End If
      j = InStr(i + 1, A$, ",")
      x = Val(Mid$(A$, i + 1, j - i))                   'SSLIP
      i = InStr(j + 1, A$, ",")
      x = Val(Mid$(A$, j + 1, Len(A$) - j))             'TASF
      j = InStr(i + 1, A$, ",")
      x = Val(Mid$(A$, i + 1, j - i))                   'WDC
      i = InStr(j + 1, A$, ",")
      x = Val(Mid$(A$, j + 1, Len(A$) - j))             'WSC
      j = InStr(i + 1, A$, ",")
      If j = 0 Then
        x = Val(Right$(A$, Len(A$) - i))                  'XMACH2
        If x = -32767 Then
          
        End If
      End If
    End Select
    Mach2 = x
    mMach2 = x
End Sub

Sub GetMMSline(NGVlu, UTsec&, P!, T!, mT!, mT2!, mT3!, mT4!, mLatitude!, mLongitude!, mZg!, mZp!, mPitch!, mRoll!, mMach2!)
Dim i%, j%, A$, b$, HeaderCount%, x!, Utime&, TAS!, UT&, Th!, U!, V!, W!
Static LastT!, LastP!, LastWspd!, LastWdir!, LastZp!, LastZg!, LastUTsec!
' UTsec ... current time, if <0 then just read next record
' UT    ... time read from MMS file
' P, T, Th, U, V, W ... pressure, temperature, theta, and winds at UT
' MMSfile$  Nav file to read.

Static MMlu%, Tcorrection%
   
  If UTsec < 0 Then
    Line Input #MMlu, A$
    UT = Int(Val(Left$(A$, 8)) + 0.5) + Tcorrection
  Else
    Do  'Catch up                    'ER2 MMS 1940 sec slow on 20020728 and 1550 sec slow on 20020726
      Line Input #MMlu, A$
      UT = Int(Val(Left$(A$, 8)) + 0.5) + Tcorrection
    Loop Until (UT >= UTsec And Mid$(A$, 18, 4) <> "9999") Or EOF(MMlu)
  End If
  
'  If EOF(MMlu) Then
'    Close MMlu
'    MMlu = 0
'    Quit = True
'    Exit Sub
'  End If

Select Case Val(YYYYMMDD$)
Case Is < 20010801
'GMT       Psta Tsta Thta   U    V    W
' 24518.8  9237 2559 2618   11   -8  -14
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
  P = (Val(Mid$(A$, 9, 6)) / 10#)
  T = Val(Mid$(A$, 15, 5) / 10#) 'Kelvin
  OATmms = T + cTo
  Th = Val(Mid$(A$, 20, 5) / 10#)
  U = Val(Mid$(A$, 25, 5) / 10#)
  V = Val(Mid$(A$, 30, 5) / 10#)
  W = Val(Mid$(A$, 35, 5) / 10#)

Case Else
'CAMEX-4, CRYSTAL
'  GMT     Psta  Tsta  Thta   U    V    W
'  GMT       Psta Tsta Thta   U    V    W
' 65314.726 10158 3053 3039  -77    7    5
' 71931.1   566 21124 47970 -100   21   -1   'Final CRYSTAL data
'  GMT     Psta  Tsta  Thta    U     V     W
' 50226.9  8978 27915 28788 99999 99999 99999 'Preliminary GH data
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'  GMT     Psta  Tsta  Thta    U     V     W
' 49055.5  9163 28429 29148  -821   138   208
' 51115.6  2332 21848 33117  3368  -283    -3  'GloPac
  P = (Val(Mid$(A$, 10, 5)) / 10#)
  T = Val(Mid$(A$, 16, 5) / 100#) 'Kelvin
  OATmms = T '+ cTo
  Th = Val(Mid$(A$, 22, 5) / 100#)
  U = Val(Mid$(A$, 28, 5) / 10#)
  V = Val(Mid$(A$, 34, 5) / 10#)
  W = Val(Mid$(A$, 40, 5) / 10#)

End Select
End Sub
Sub GetNGline_Trex(NGVlu, UTsec&, P!, T!, mT!, mLatitude!, mLongitude!, mZg!, mZp!, mPitch!, mRoll!, mMach2!)
Dim i%, j%, A$, HeaderCount%, x!, Utime&, TAS!
Static PTWlu%, LastT!, LastP!, LastWspd!, LastWdir!, LastZp!, LastZg!
   
' UTC      ATRL  AT_A  GGALT  GGLAT  GGLON  PALT  PITCH  PSFC  ROLL  TAS_A  Time
'22:24:24 -41.8909 -43.3967 9302.98 36.6453 -118.323 9150.33 2.21617 300.439 -0.0850106 241.245 13954
'19:00:04 13.1145 12.9608 1707.68 39.9024 -105.101 1615.38 -1.77342 833.586 0.0328811 0 1694
    Line Input #NGVlu, A$
    'UTsec , Latitude, Longitude, ALTkm, T1, T2
    HHMMSS$ = Left$(A$, 8)
    UTsec = fTstringToSec(HHMMSS$, True)
    i = InStr(10, A$, " ")
    mT = Val(Mid$(A$, 10, i - 10)) + cTo
    j = InStr(i + 1, A$, " ")
    T = Val(Mid$(A$, i + 1, j - i)) + cTo
    i = InStr(j + 1, A$, " ")
    mZg = Val(Mid$(A$, j + 1, i - j)) / 1000#
    j = InStr(i + 1, A$, " ")
    mLatitude = Val(Mid$(A$, i + 1, j - i))
    i = InStr(j + 1, A$, " ")
    mLongitude = Val(Mid$(A$, j + 1, i - j))
    j = InStr(i + 1, A$, " ")
    mZp = Val(Mid$(A$, i + 1, j - i)) / 1000#
    i = InStr(j + 1, A$, " ")
    mPitch = Val(Mid$(A$, j + 1, i - j))
    j = InStr(i + 1, A$, " ")
    P = Val(Mid$(A$, i + 1, j - i))
    i = InStr(j + 1, A$, " ")
    mRoll = Val(Mid$(A$, j + 1, i - j))
    j = InStr(i + 1, A$, " ")
    TAS = Val(Mid$(A$, i + 1, j - i))
    i = InStr(j + 1, A$, " ")
    Utime = (Mid$(A$, j + 1, Len(A$) - j))
End Sub

Sub IWGreadAVG(IWGlu%, Irecord&, UTsec&, P!, Tav, Tre, Tre2, Tre3, Tre4!, Latitude!, Longitude!, mZg!, pALT, Pitch, Roll, Mach2)
  
  Call IWGread(IWGlu%, Irecord&)
  
  DandT = IWG_DandT
  Call DateTtoStrings(DandT, YYYYMMDD, HHMMSS, UTsec&, False)
  Latitude = IWG_Parameter(2)
  Longitude = IWG_Parameter(3)
  mZg = IWG_Parameter(4)
  pALT = IWG_Parameter(6) / cft_km
  Mach2 = IWG_Parameter(11) ^ 2
  Pitch = IWG_Parameter(16)
  Roll = IWG_Parameter(17)
  Tav = IWG_Parameter(20)
  Tre = Tav
  Tre2 = Tav
  Tre3 = Tav
  Tre4 = Tav
  P = IWG_Parameter(23)

End Sub

Sub MapGVtoOther()
'Public yyyymmddI$, UTsecI&, pALTI!, gALTI!, rALTI!, pALTftI!, gALTfeetI!, OATnI!, OATnavI!, WspdI!, WdirI!
'Public UTsecNavI&, HeadingI!, PitchI!, RollI!, LatitudeI!, LongitudeI!, TASI!

  If Val(YYYYMMDDI$) > Val(YYYYMMDD$) Then
    UTsec = UTsecI + 86400
  Else
    UTsec = UTsecI
  End If
  pALT = pALTI
  gALT = gALTI
  rALT = rALTI
  mALT = pALT
  pALTft = pALTftI
'  galtfeet = gALTfeetI
  OATn = OATnI
  OATnav = OATnavI
  OATmms = OATnav       'Needed in WriteArray() for OATV(2,r)
  Wspd = WspdI
  Wdir = WdirI
  UTsecNav = UTsecNavI
  Heading = HeadingI
  Pitch = PitchI
  Roll = RollI
  Latitude = LatitudeI
  Longitude = LongitudeI
  TAS = TASI
End Sub

Sub MTPwrite(REFlu%, GoodScansOnly As Boolean, WriteBadHeaders As Boolean, f As Form)
Dim Ext$, MPheader$, V!(1 To 20), LocalOpen As Boolean
Dim Filename$, MPlu%, HDRfile$, HDRfileOld$, L%, x!, A$, dZp!, dZg!, Discard As Boolean, dALT!, MRIthreshold!
Dim UTsecLast&, DiscardMRI As Boolean, Rev$, RevComment$

  If REFlu% = 0 Then
    Filename$ = Path$ + AC$ + YYYYMMDD$ + ".ERF"
    REFopen Filename$
    LocalOpen = True
  End If
  
  MPheader$ = " #####0, #0, #####0, #####0, ##0.000, ##0.0, ##0.0, ##0.0, ##0.0, ##0.0, ##0.0, ##0.0, ##0.000, ###0.000, ##0.0, #0.00, #00.00, #0.00"
' Rexample$ = "  52900, 33,  52915,  52906,   0.064,  -0.8,   0.2, 298.3,  11.2,  99.9, 303.4, 325.8,  29.612,  -95.166, 999.9,  2.00, 200.05, 15.67"
     
  If f.chkDiscard.Value = 1 Then Discard = True Else Discard = False
  dALT = Val(f.txtDiscard.Text)
  If f.chkMRIedit.Value = 1 Then DiscardMRI = True Else DiscardMRI = False
  MRIthreshold = Val(f.txtMPmri.Text)
     
' Will read & modify header after first assigning MP output file
  MPlu% = FreeFile
  Open MTP_MPfile$ For Output As #MPlu%
  
  HDRfile$ = fSetupFileName("ICT")
  Rev = f.cboICTrev.Text
  RevComment = f.cboRevComment.Text
  Call UpdateMTPheader(HDRfile$, MPlu%, Rev$, RevComment$)
  
' Write Limits Record
  Record% = 0
  Do
    Record% = Record% + 1
    If Record > f.ProgressBar1.Max Then Exit Do
    f.txtRecord.Text = Str(Record)
    f.ProgressBar1.Value = Record
    Call REFread(REFlu%, Record%)
    If Not EOF(REFlu) Then
      If Not GoodScansOnly Or GoodScan Or WriteBadHeaders Then GoSub WriteCycle
    End If
    DoEvents
  Loop Until EOF(REFlu%)
  
  Close (MPlu%)
  If LocalOpen Then Close (REFlu%)
  
  Exit Sub

WriteCycle:
' NB only come here on GoodScan = True  (get rid of checks for goodscan)
  If EditNlev And (Nlev < NlevMin Or Nlev > NlevMax) Then
    EditWord = EditWord Or 128: GoodScan = False ' Determine total # of acceptable levels; throw out if <7 levels
    NlevPC = NlevPC + 1
  End If
  If Nlev1 = 0 Then Nlev1 = 1
  If pALT < zzz(Nlev1) / 100 Then EditWord = EditWord Or 1: RetAltPC = RetAltPC + 1: GoodScan = False
  If UTsec > 63000 And GoodScan Then
  Nlev = Nlev
  End If
' Update the number of valid levels
  V(1) = UTsec: V(4) = UTsec + ScanTime * LocHor / 16
  If Not EOF(REFlu) Then
    Call REFread(REFlu%, Record% + 1)
    V(3) = UTsec
    If EOF(REFlu) Then V(3) = V(1) + ScanTime
  End If
  V(2) = Nlev: V(5) = pALT: V(6) = Pitch: V(7) = Roll: V(8) = OATmtp
  V(9) = Zt1: V(10) = Zt2: V(11) = Th1: V(12) = Th2: V(13) = Latitude: V(14) = Longitude
  If LRac > 20 Then V(15) = -9999 Else V(15) = LRac
  V(16) = MRI: V(17) = Tcp: V(18) = Zcp
  If WriteBadHeaders And Not GoodScan Then V(2) = 0
  If Discard And pALT < dALT Then V(2) = 0
  If DiscardMRI And MRI > MRIthreshold Then V(2) = 0
  
  If (MakeWord And 32) Or Not GoodTrop Then  'RAWbad cycles formatted for MP files
    V(9) = -9999: V(10) = -9999: V(11) = -9999: V(12) = -9999
  End If
  
  If GoodScan Or WriteBadHeaders Or (Discard And pALT < dALT) Or (DiscardMRI And MRI > MRIthreshold) Then
  ' Check to make sure time series is monotonic
    If V(1) > UTsecLast And Not EOF(REFlu) Then
      Print #MPlu%, fUsing$(MPheader$, V!()) + ","
    Else
'      MsgBox "UTsec<UTsecLast: " + Format(UTsec, "000000") + " < " + Format(UTsecLast, "000000"), vbOKOnly
      Return
    End If
    'Debug.Print fUsing$(MPheader$, V!())
    If Not (Discard And pALT < dALT) And Not (DiscardMRI And MRI > MRIthreshold) Then
      If Not GoodScansOnly Or GoodScan Then
        dZp = 1000 * (pALT - sZZZ(16))
        For L = Nlev2 To Nlev1 Step -1                   'top down
          If sTSE(L) > 9.9 Then sTSE(L) = 9.9            'Avoid formats running together
          V(1) = sZZZ(L) * 1000#                         'meters, pressure altitude
          A$ = fUsingN("####0", V(1), 5) + ", "
          V(2) = sTTT(L)                                 'K, temperature
          A$ = A$ + fUsingN("000.0", V(2), 5) + ", "
          V(3) = sTSE(L)                                 'K, temperature error
          A$ = A$ + fUsingN("0.0", V(3), 3) + ", "
          V(4) = sZg(L) * 1000#                          'meters, geometric altitude
          A$ = A$ + fUsingN("####0", V(4), 5) + ", "
          V(5) = sND(L)
          If L = Nlev1 Then
            A$ = A$ + fUsingN("####0", V(5), 5)
          Else
            A$ = A$ + fUsingN("####0", V(5), 5) + ","
          End If
          Print #MPlu, A$
        Next L
      Else
'        Print #MPlu, " -9999, -9999, -9999, -9999, -9999"
      End If
    End If
  End If
  UTsecLast = V(1)
Return
End Sub

Sub UpdateMTPheader(HDRfile$, MPlu%, Rev$, RevComment$)
Dim lu%, i%, j%, L%, n%, A$, b$, Nhdr%, yr$, dy$, mo$, fln&, fltnr&, FltDate&
Dim x$, lux%
Dim FlightNo&, Objective$, FlightDate$, NUMfile$

  lu% = FreeFile
  Open HDRfile$ For Input As lu%
  Input #lu%, Nhdr   'Get number of header lines
  Close (lu%)

  lu% = FreeFile
  Open HDRfile$ For Input As lu%   'Start over
  
  For j = 1 To Nhdr                'Update dates in MP file header
    Line Input #lu%, A$
    Select Case j
    Case 7
      A$ = Mid$(YYYYMMDD$, 1, 4) + ", "      'set YR
      A$ = A$ + Mid$(YYYYMMDD$, 5, 2) + ", " 'set MO
      A$ = A$ + Mid$(YYYYMMDD$, 7, 2) + ", " 'set DY
      b$ = Date$: L = Len(b$)
      yr$ = Right$(b$, 4): dy$ = Mid$(b$, L - 6, 2): mo$ = Mid$(b$, L - 9, 2)
      A$ = A$ + yr$ + ", " + mo$ + ", " + dy$
    Case 81
      A$ = "REVISION: R" + Rev$
    Case 82
      A$ = "R" + Rev$ + ": " + RevComment$
    Case Else
    End Select

    Print #MPlu%, A$
  Next j
  Close (lu%)

End Sub


Sub Read_HPA()
Dim lu%, Filename$, A$, i%

  Filename$ = MNpath$ + "Setup\" + Mission$ + "_HPA.txt"
' Check if there is a pressure altitude correction file
  If Dir$(Filename$) = "" Then
    Pcorr = 99.99
  Else
    lu = FreeFile
    Open Filename$ For Input As #lu
    Do
      Line Input #lu, A$
    Loop Until Left$(A$, 8) = YYYYMMDD$ Or EOF(lu)
    If EOF(lu) Then
      Pcorr = 99.99
    Else
      Pcorr = Val(Right$(A$, Len(A$) - 9))
    End If
    Close lu
  End If
End Sub

Sub ReadMissionRCSdo(ByVal lu%, ByRef n%, ByRef V!())
Dim i%, j%, k%, A$
' Read l number per line, or several separated by a SINGLE space
' Terminate when a blank line is encountered.
    i = 1
    Input #lu, A$
    k = 1
    j = InStr(A$, " ")
    If j > 0 Then
      Do
        V(i) = Mid(A$, k, j - k)
        k = j + 1
        j = InStr(k, A$, " ")
        i = i + 1
        If j = 0 Then
          V(i) = Mid(A$, k, Len(A$) - k + 1)
          Exit Do
        End If
      Loop
    Else
      V(i) = Val(A$)
    End If
    
    Do
      Input #lu, A$
      If A$ = "" Then Exit Do
      k = 1
      j = InStr(A$, " ")
      If j > 0 Then
        i = i + 1
        Do
          V(i) = Mid(A$, k, j - k)
          k = j + 1
          j = InStr(k, A$, " ")
          i = i + 1
          If j = 0 Then
            V(i) = Mid(A$, k, Len(A$) - k + 1)
            Exit Do
          End If
        Loop
      Else
        i = i + 1
        V(i) = Val(A$)
      End If
    Loop
    n = i

End Sub

Sub ReadMissionRCSprocess(lu%, Category$, EOFflag As Boolean)
Dim i0%, i1%, i2%, cMd$, V$, i%, j%, response As Variant
Dim A$, rFLA!(1 To 20), n%

  Select Case Category$
  Case "GENERAL"
    Do
      If EOF(lu%) Then
        EOFflag = True
        Exit Sub
      End If
      FIsize% = FIsize% + 1
      Input #lu%, cMd$                       'Read a line
     'Debug.Print cmd$
      If Len(cMd$) = 0 Then Exit Sub
      i1% = InStr(1, cMd$, "=")
      If i1% > 0 Then                     'Look for a command line
        i2% = InStr(i1%, cMd$, " ")       'and end of its value
        If i2% = 0 Then i2% = Len(cMd$) + 1
        V$ = Mid$(cMd$, i1% + 1, i2% - i1% - 1)
        Select Case Left$(cMd$, i1% - 1)
        Case "GenDate": Gendate$ = V$
        Case "Ceiling": Ceiling = Val(V$)
        Case "CycleTime": CycleTime = Val(V$)
        Case "SU": SU$ = V$
        Case "Nif": Nif = Val(V$)
        Case "Nel": Nel = Val(V$)
        Case "Nlo": Nlo = Val(V$)
        Case "Nobs": Nobs = Val(V$)
        Case "Nret": Nret = Val(V$)
        Case "CH1LSBloss":  CHnLSBloss(1) = Val(V$): CH1LSBloss = Val(V$)
        Case "CH2LSBloss":  CHnLSBloss(2) = Val(V$)
        Case "CH3LSBloss":  CHnLSBloss(3) = Val(V$)
        Case Else
        End Select
      End If
    Loop
      
  Case "RC_ALTITUDES"
    ReadMissionRCSdo lu, NFL, rFLA()
    For i = 1 To NFL: FLA(i) = Int(rFLA(i) * 100#): Next i
            
  Case "ELEVATION_ANGLES"
    ReadMissionRCSdo lu, Nel, El()
    
  Case "LO_FREQUENCIES"
    ReadMissionRCSdo lu, Nlo, LO()
 
  Case "OBSERVABLE_ERRORS"
    ReadMissionRCSdo lu, Nobs, sOBrms()
    
  Case "IF_BANDPASS"
    For i = 1 To Nlo
      For j = 1 To Nif
        Input #lu, n, IFoff(i, j), IFwt(i, j)
        IFoff(i, j) = IFoff(i, j) / 1000#
        'Debug.Print n; IFoff(i, j), IFwt(i, j)
      Next j
    Next i

  Case "ZP_OFFSETS"
    For i = 1 To Nret
      Input #lu, dZ(i)
    Next i
 
  Case "EOF"
    EOFflag = True
    Exit Sub
    
  Case Else
  End Select

End Sub

Sub RCwrite2(iRC%, FQFN$)
Dim A As RCconfiguration2, lu%, i%, j%, k%, n%
Dim b As RC_Set_1FL, CreationDateTime As Date
' RC file - This is original RCwrite routine, which is no longer used.
' First record contains configuration information
' It is followed by one record for each flight level with RCs and related info

  lu = FreeFile
  Open FQFN For Random Access Read Write As lu Len = Len(A)
  
  A.RCformat = RCformat(iRC)
  CreationDateTime = Date + Time
  A.CreationDateTime = CreationDateTime
  A.RAOBfilename = RAOBfilename
  A.RCfilename = RCfilename
  A.RAOBcount = RAOBcount
  A.LR1 = LR1                 'LR above top of RAOB
  A.zLRb = zLRb               'LR break altitude
  A.LR2 = LR2                 'LR above break altitude
  A.RecordStep = RecordStep   'Record Step through available RAOBs
  A.RAOBmin = MinimumRAOBz    'Minimum acceptable RAOB altitude
  A.ExcessTamplitude = ExcessTamplitude 'Random Excess Noise Level on Ground
  A.SURC = SURC$
  
  A.Nobs = Nobs               'Number of observables
' Retrieval offset levels wrt flight level
  A.Nret = Nret               'Number of retrieval levels
  For i = 1 To Nret: A.dZ!(i) = dZ(i): Next i
' Flight levels (km)
  A.NFL = NFL                 'Number of flight levels
  For i = 1 To NFL: A.Zr!(i) = Zr(i): Next i
' LO frequencies (GHz)
  A.Nlo = Nlo                 'Number of LO channels
  For i = 1 To Nlo: A.LO!(i) = LO(i): Next i
' Scan mirror elevation angles
  A.Nel = Nel                 'Number of elevation angles
  For i = 1 To Nel: A.El!(i) = El(i): Next i
' IF frequency offsets (GHz) and weights
  A.Nif = Nif                 'Number of IF frequencies
  For j = 1 To Nif
      A.IFoff2!(j) = IFoff2(j)    'IFoff2 is original single IF bandpass
      A.IFwt2!(j) = IFwt2(j)
  Next j
'  A.RAOBbias = RAOBbias
'  A.CH1LSBloss = CH1LSBloss
'  For i = 1 To Channels
'    A.CHnLSBloss(i) = CHnLSBloss(i)
'  Next i
' Sensitivity matrix: iRC, NFL, Nlo, Nel, bias and slope
  For i = 1 To NFL
    For j = 1 To Nlo
      For k = 1 To Nel
        A.SmatrixN1!(i, j, k) = Smatrix(iRC, i, j, k, 1)
        A.SmatrixN2!(i, j, k) = Smatrix(iRC, i, j, k, 2)
      Next k
    Next j
  Next i
  
  Put #lu%, 1, A
  
  For n = 2 To NFL + 1
    b.sBP = BP(iRC, n - 1, 16)               'Flight level pressure altitude (hPa)
    For i = 1 To Nobs
      b.sOBrms!(i) = OBrms(n - 1, i)         '1-sigma apriori observable errors
      b.sOBav!(i) = OBav(iRC, n - 1, i)      'Archive Average observables
    Next i
    'Debug.Print n - 1; OBrms(n - 1, 1); OBrms(n - 1, 30); b.sOBrms(30)
    For i = 1 To Nret
      b.sBPrl!(i) = BP(iRC, n - 1, i)        'Pressure at retrieval levels
      b.sRTav!(i) = RTav(iRC, n - 1, i)      'Average T at retrieval levels
      b.sRMSa!(i) = RMSa(iRC, n - 1, i)      'Variance in T at retrieval levels
      b.sRMSe!(i) = RMSe(iRC, n - 1, i)      'Formal error in T at retrieval levels
      For j = 1 To Nobs
        b.Src!(i, j) = rc(iRC, n - 1, i, j)  '33 retrieval levels, 30 observables
      Next j
    Next i
    Put #lu%, n, b
    DoEvents
  Next n
  Close lu
End Sub

Sub ReadMissionRCS(Mission$, Category$, All As Boolean)
' Read Mission initialization parameters
' If ALL is TRUE, read all parameters
' If ALL is FALSE, read only the requested CATEGORY
Dim i0%, i1%, i2%, lu%, cMd$, V$, FixPath As Boolean, i%, j%, response As Variant
Dim A$, Cat$, EOFflag As Boolean

' Read MISSION_RCS.txt file
  lu = FreeFile
  Open SUpath$ + Mission$ + "_RCS.txt" For Input As lu

  FIsize% = 0
  FixPath = False
  If All Then
    Do
NextCategory:
      If EOF(lu%) Then GoTo Exit_Sub

      FIsize% = FIsize% + 1
      Input #lu%, cMd$                       'Read a line
      If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
        i0% = InStr(2, cMd$, "]")
        If i0% = 0 Then
          MsgBox "Right Bracket not found. Fix line number " + Str(FIsize%), vbOK
          Exit Sub
        End If
    
        Cat$ = Mid$(cMd$, 2, i0% - 2)  'Remove square brackets
        Call ReadMissionRCSprocess(lu, Cat$, EOFflag)
        If EOFflag Then GoTo Exit_Sub Else GoTo NextCategory
      End If

      Loop

  Else
  
NextCategory2:
    If EOF(lu%) Then GoTo Exit_Sub

    FIsize% = FIsize% + 1
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Right Bracket not found. Fix line number " + Str(FIsize%), vbOK
        Exit Sub
      End If
    
      Cat$ = Mid$(cMd$, 2, i0% - 2)  'Remove square brackets
      If Cat$ = Category$ Then
        Call ReadMissionRCSprocess(lu, Cat$, EOFflag)
        'For i = 1 To Nobs: OBrms(i) = sOBrms(i): Next i
        GoTo Exit_Sub
      End If
      GoTo NextCategory2
    Else
      GoTo NextCategory2
    End If
  End If

Exit_Sub:
  Close (lu%)

Check_Category:

End Sub

Sub ReadMTPsetupINI(Category$, AddItem$())
' Doesn't seem to be used anywhere mjmxxx 20110802
  Dim i0%, i1%, i2%, lu%, cMd$, V$, FixPath As Boolean

  If Len(Dir("C:\MTP\Setup\MTPsetup.INI")) = 0 Then
    MsgBox "Unable to open C:\MTP\Setup\MTPsetup.INI", vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open "C:\MTP\Setup\MTPsetup.INI" For Input As lu%
  FIsize% = 0
  FixPath = False
  Do
NextCategory:
    If EOF(lu%) Then GoTo Exit_Sub

    FIsize% = FIsize% + 1
    Input #lu%, cMd$                       'Read a line
  
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Right Bracket not found. Fix line number " + Str(FIsize%), vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Category$ Then    'Read setup info for only the requested program
        Select Case Mid$(cMd$, 2, i0% - 2)
        Case "SYSTEM"
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
              Case "ProgramDrive$":  ProgramDrive$ = V$
              Case "DataDrive$":  DataDrive$ = V$
              Case "UID$": UID$ = V$
              Case "DefaultRTmode"
                DefaultRTmode = Val(V$)
                If DefaultRTmode = 0 Then RealTime = False Else RealTime = True
              Case "DataSourceMode"
                DataSourceMode = Val(V$)
              Case "DataDestinationMode"
                DataDestinationMode = Val(V$)
              Case "AlternateRoot"
                If V$ = """""" Then Rdir1$ = "" Else Rdir1$ = Val(V$)
              Case "UseDefaultRCs"
                If Val(V$) = 1 Then UseDefaultRCs = True Else UseDefaultRCs = False
              Case Else
              End Select
            End If
          Loop
        Case "cboDestination"
        
        Case Else
        End Select
      End If
    End If
  Loop
  
Exit_Sub:
  Close (lu%)

End Sub

Sub ReadProgramSetup(Program$, Category$)
  Dim i0%, i1%, i2%, lu%, cMd$, V$, FixPath As Boolean

  If Len(Dir("C:\MTP\Setup\ProgramSetup.INI")) = 0 Then
    MsgBox "Unable to open C:\MTP\Setup\ProgramSetup.INI", vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open "C:\MTP\Setup\ProgramSetup.INI" For Input As lu%
  FIsize% = 0
  FixPath = False
  Do
NextCategory:
    If EOF(lu%) Then GoTo Exit_Sub

    FIsize% = FIsize% + 1
    Input #lu%, cMd$                       'Read a line
  
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Right Bracket not found. Fix line number " + Str(FIsize%), vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Program$ Then    'Read setup info for only the requested program
        Select Case Mid$(cMd$, 2, i0% - 2)
        Case "CTC"
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
              Case "LastY1":  'LastY1 = Val(V$)
              Case "LastY2":  'LastY2 = Val(V$)
              Case "LastZ1":  'LastZ1 = Val(V$)
              Case "LastZ2":  'LastZ1 = Val(V$)
              Case Else
              End Select
            End If
          Loop
        Case Else
        End Select
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)

End Sub

Sub ReadPlatform(Platform$)
Dim i0%, i1%, i2%, lu%, cMd$, V$, FixPath As Boolean, i%, j%, response As Variant
Dim Filename$
' Read new mjm .INI Platform Format
'Exit Sub
  On Error GoTo CopyCal
  lu% = FreeFile
Carryon:
  Open Filename$ For Input As lu%
  CalFileFormat = CInt(fReadFirstNumber(lu))
'MsgBox "ReadCAL Entry!", vbOKOnly

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
    Case "GENERAL"
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
          Case "GenDate": Gendate$ = V$
          Case "FltDate": FltDate$ = V$
          Case "UTstart"
          Case "UTend"
          Case "Channels": Channels = Val(V$)
          Case "Nel": Nel = Val(V$)
          Case "Emissivity": Emissivity = Val(V$)
          Case "Reflectivity": Reflectivity = Val(V$)
          Case "DeltaTmin": DeltaTmin = Val(V$)
          Case Else
          End Select
        End If
      Loop

    Case "FIT_INFO"
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
          Case "Nfit": Nfit = Val(V$)
          Case "Offset": NP$(1) = V$: GOF(1) = 0#
          Case "FitVar1": NP$(2) = V$
          Case "FitVar2": NP$(3) = V$
          Case "FitVar3": NP$(4) = V$
          Case "FitVar4": NP$(5) = V$
          Case "GEC11": GEC(1, 1) = Val(V$)
          Case "GEC12": GEC(1, 2) = Val(V$)
          Case "GEC13": GEC(1, 3) = Val(V$)
          Case "GEC14": GEC(1, 4) = Val(V$)
          Case "GEC15": GEC(1, 5) = Val(V$)
          Case "GEC21": GEC(2, 1) = Val(V$)
          Case "GEC22": GEC(2, 2) = Val(V$)
          Case "GEC23": GEC(2, 3) = Val(V$)
          Case "GEC24": GEC(2, 4) = Val(V$)
          Case "GEC25": GEC(2, 5) = Val(V$)
          Case "GEC31": GEC(3, 1) = Val(V$)
          Case "GEC32": GEC(3, 2) = Val(V$)
          Case "GEC33": GEC(3, 3) = Val(V$)
          Case "GEC34": GEC(3, 4) = Val(V$)
          Case "GEC35": GEC(3, 5) = Val(V$)
          Case "GOF1": GOF(2) = Val(V$)
          Case "GOF2": GOF(3) = Val(V$)
          Case "GOF3": GOF(4) = Val(V$)
          Case "GOF4": GOF(5) = Val(V$)
          Case "TARGET":
            TGToffset = Val(V$)
'            MsgBox "ReadCAL:" + Str(TGToffset), vbOKOnly
          Case "MIXER": MXRoffset = Val(V$)
          Case Else
          End Select
        End If
      Loop
      
    Case "WINDOW_CORRECTIONS"
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
          Select Case Left$(cMd$, 3)
          Case "WCT"
            i = Val(Mid$(cMd$, 4, 1))
            j = Val(Mid$(cMd$, 5, 2))
            WINcor(i, j) = Val(V$)
          Case Else
          End Select
        End If
      Loop
      
    Case "EOF"
      Exit Sub
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
  Exit Sub

CopyCal:
  response = MsgBox("Calfile has not been copied to flight directory!" + vbCrLf + " Do you wish to copy the default Calfile?", vbYesNo)
  If response = vbYes Then
    'frmMTPbin.cmdCopyCal_Click
    GoTo Carryon
  Else
    Stop
    Exit Sub
  End If
End Sub

Sub ReadPTZtoArray(ZpMin!)
Dim Prefix$, PTWfile$, Pr!, Tr!, i&, Quit As Boolean, UT&, param!(1 To 75)
' Read 1 Hz Nav data into P, T and Z arrays
' iMax

  Prefix$ = Right$(RAWextension$, 2)
  PTWfile$ = MNpath$ + Prefix$ + "\" + Prefix$ + YYYYMMDD$ + fACext(AC$)
  If AC$ = "NG" Then PTWfile$ = MNpath$ + Prefix$ + "\" + Prefix$ + YYYYMMDD$ + ".asc"
  UTsec = -1
  Quit = True       'Set TRUE to force STATIC logical unit to be set to 0
  Do
    Call GetNAVvalues(Prefix$, UTsec&, UT&, Pr!, Tr!, param!(), PTWfile$, Quit)
  Loop Until Tr < 999# And fPtoZ(Pr) > ZpMin
  Tptz!(1) = Tr
  Pptz!(1) = Pr
  Zptz!(1) = fPtoZ(Pr)
  UTptz = UT - 1    'So UT = UTptz + i
  i = 1
  Do        'Read nav PT values into memory
    i = i + 1
    Call GetNAVvalues(Prefix$, UTsec&, UT&, Pr!, Tr!, param!(), PTWfile$, Quit)
    If Tr > 999 Then Exit Do
    Tptz!(i) = Tr
    Pptz!(i) = Pr
    Zptz!(i) = fPtoZ(Pr)
  Loop Until Quit
  iPTZ = i

End Sub

Sub BINread(lu%, Record%)
Dim A As REFrecord, i%, j%, x!
' Record 1 is FI record
' Record 2 is Limits record
' Record 3, 4 and 5 are spares

  Get #lu%, Record% + HiddenRecords, A
  
  GoodScan = A.GoodScan
  MakeWord = A.MakeWord
  Cycle = A.Cycle
  UTsec = A.UTsec
  UTsecMTP = A.UTsecMTP
  UTsecNav = A.UTsecNav
  TTO = A.TTO
  
  pALT = A.pALT
  gALT = A.gALT
  rALT = A.rALT
  mALT = A.mALT
  
  Pitch = A.Pitch
  Roll = A.Roll
  Latitude = A.Latitude
  Longitude = A.Longitude
  Heading = A.Heading
  TAS = A.TAS
  Wspd = A.Wspd
  Wdir = A.Wdir
  Elcor = A.Elcor
  ElCorUsed = A.ElCorUsed
  
  OATnav = A.OATnav
  OATmms = A.OATmms
  OATmtp = A.OATmtp
  
  TTMA = A.TTMA
  TMMA = A.TMMA
  TWMA = A.TWMA
  
  For i% = 1 To Channels
    RFImask(i) = A.RFImask(i)
    For j% = 1 To 12
      C(i%, j%) = A.Counts(i%, j%)
      CMA(i, j) = A.CMA(i, j)
    Next j%
  Next i%
  
  Muxs(0) = A.Muxs(0)
  For i% = 1 To 16
    Muxs(i%) = A.Muxs(i%)
    Mux(i%) = A.Mux(i%)
  Next i%
  ' Muxs(i) Parameter
  ' 00 Ttgt
  ' 01 Tnd
  ' 02 Tlo1
  ' 03 Tlo2
  ' 04 Tifa
  ' 05 Tlo
  ' 06 Thi
  ' 07 ACC+
  ' 08 Twin
  ' 09 Tmtr
  ' 10
  ' 11 Vref
  ' 12 Tdc1
  ' 13 Tdc2
  ' 14 PS5
  ' 15 PS12
  ' 16 ACC-
  Ttgt = A.Muxs(0)            'Either tgt lo or tgt hi
  Tnd = A.Muxs(1)
  Tlo1 = A.Muxs(2)
  Tlo2 = A.Muxs(3)
  Tifa = A.Muxs(4)
  Ttgtlo = A.Muxs(5)
  Ttgthi = A.Muxs(6)
  ACCp = A.Muxs(7)
  Twin = A.Muxs(8)
  Tmtr = A.Muxs(9)
  'asterisk = a.Muxs(10)
  Vref = A.Muxs(11)
  Tdc1 = A.Muxs(12)  '11 on DC8
  Tdc2 = A.Muxs(13)  '12 on DC8
  Vps5 = A.Muxs(14)
  Vps12 = A.Muxs(15)
  ACCm = A.Muxs(16)

  dACC = ACCp - ACCm             'Peak positive acceleration minus peak negative acceleration
  For i = 1 To Channels
    dND(i) = C(i, 11) - C(i, 12)
    CN(i) = dND(i)
    CB(i) = C(i, 12)                   'Channel i base counts
    CS(i, LocHor) = C(i, LocHor)
  Next i
  
  dACC = ACCp - ACCm             'Peak positive acceleration minus peak negative acceleration
  
  Tac = OATmtp
End Sub

Sub ERFopen(ERFfile$)
' Open the default ERF file
  ERFlu = FreeFile
  FileFormatIn = fREFreadFileFormat(ERFfile$)

  Select Case FileFormatIn
  Case 32
    Open ERFfile$ For Random Access Read Write As ERFlu Len = Len(REF)
  Case 33, 0
    Open ERFfile$ For Random Access Read Write As ERFlu Len = Len(REF2)
  End Select

End Sub



Sub GetPTWvalues_All(UTsec&, UT&, Ps!, pt!, Ts!, PTWfile$)
' UTsec is requested UT
' UT    is returned UT

Dim i%, A$, HeaderCount%, Tport!, Tstar!, Tf!
Static PTWlu%

  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open PTWfile$ For Input As PTWlu
    Input #PTWlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #PTWlu, A$: Next i
  Else
    If EOF(PTWlu) Then
      Close PTWlu
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Line Input #PTWlu, A$
    If 1 = 0 Then UT = Val(Left$(A$, 7)) Else UT = Val(Left$(A$, 6))
  Loop Until (UT >= UTsec And Mid$(A$, 7, 6) <> "999.99") Or EOF(PTWlu)
'  Loop Until UT >= UTsec Or EOF(PTWlu)
  If EOF(PTWlu) Then UT = -1: Exit Sub
'Changing PTW formats!
'19990409
'UtSec BoxTemp RamPres VertDifPres HorzDifPres AirTemp AnaPxdTemp TAS StatPres DigPxdTemp
'  47385   23.02    0.77   -0.19   -1.40  296.55   35.73   11.41 1009.78   36.00
'
'19990912
'UtSec BoxTemp RamPres VertDifPres HorzDifPres AirTemp AnaPxdTemp System28 TAS StatPres DigPxdTemp
'  48801   36.20    1.08    0.45   -0.43  303.48   33.61   26.10   13.60 1011.89   28.00
'
'20020601
'CRYSTAL-FACE Format (NB Fast Air Temp <1 sec, but sampled at 1 sec, Slow Air Temp = 4 sec)
' UT  FastTmp SlowTmp StatP  PitotP    RamP    TAS
'60992 302.92 303.13 1017.47 1017.65   0.18   5.54
'60993 302.64 303.08 1017.45 1017.67   0.22   6.13
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'    Line Input #PTWlu, a$
    
    Select Case Val(YYYYMMDD$)
    Case Is < 19990912             'Format changed (see above)
      Ps = Val(Mid$(A$, 64, 8))
      Ts = Val(Mid$(A$, 40, 8)) + cTo
    Case Is < 20020601
      Ps = Val(Mid$(A$, 72, 8))
      Ts = Val(Mid$(A$, 40, 8)) + cTo
    Case Is < 20050800
'HAVE2 - Starboard and Port T probes
'UtSec BoxTemp StarAirTemp PortAirTemp PlateTemp System28 TAS PitotPres PPTemp StatPres SPTemp RamPres STatd PTatd
'  65378    46.18   304.23   304.38    35.18    26.66    67.31  1039.41    34.00  1012.84    34.00    26.57  2290.71  2286.16
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
      If 1 = 0 Then  'Raw files from Tom are different from archived files!!
        Ps = Val(Mid$(A$, 80, 9))
        Tport = Val(Mid$(A$, 17, 9))     'Kelvin
        Tstar = Val(Mid$(A$, 26, 9))     'Kelvin
        Ts = Tport
      Else  'Same as C-F, pre-AVE
' UT  PortTmp StarTmp StatP  PitotP    RamP    TAS
'67620 304.71 304.91 1012.83 1022.85  10.02  41.50
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
        Ps = Val(Mid$(A$, 21, 7))
        pt = Val(Mid$(A$, 29, 7))
        Tport = Val(Mid$(A$, 7, 6))    'Kelvin
        Tstar = Val(Mid$(A$, 14, 6))    'Kelvin
        Ts = (Tport + Tstar) / 2#           'Average
      End If
    Case Else
      Ps = Val(Mid$(A$, 21, 7))
      Tport = Val(Mid$(A$, 7, 6))    'Kelvin
      Tstar = Val(Mid$(A$, 14, 6))    'Kelvin
      Ts = Tport
    End Select
    If Ps < 0 Then Ps = 0#
'Debug.Print UT; P; fPtoZ(P)



End Sub

Sub RCread2(iRC%, FQFN$)
Dim A As RCconfiguration2, lu%, i%, j%, k%, n%
Dim b As RC_Set_1FL, CreationDateTime As Date
' RC file -- This is original RCread routine which is no longer used. Kept so
'            old RC files can be read or converted to new data structure
'            Separate IF bandpasses are now used for each frequency. Originally only 1 was used
' First record contains configuration information
' It is followed by one record for each flight level with RCs and related info

  lu = FreeFile
  Open FQFN For Random Access Read Write As lu Len = Len(A)
  Get #lu%, 1, A
  
  RCformat(iRC) = A.RCformat
  CreationDateTime = A.CreationDateTime
  RAOBfilename = Trim(A.RAOBfilename)
  RCfilename = Trim(A.RCfilename)
  RCtemplate$(iRC) = RAOBfilename   'RC RAOB Template (eg BGDH__2003010912.RAOB2)
  If Mid$(fGetFilename(RCtemplate$(iRC)), 6, 1) = "_" Then RAOBreanalysis = True
  RAOBcount = A.RAOBcount
  LR1 = A.LR1                 'LR above top of RAOB
  zLRb = A.zLRb               'LR break altitude
  LR2 = A.LR2                 'LR above break altitude
  RecordStep = A.RecordStep   'Record Step through available RAOBs
  MinimumRAOBz = A.RAOBmin    'Minimum acceptable RAOB altitude
  ExcessTamplitude = A.ExcessTamplitude 'Random Excess Noise Level on Ground
  SURC$ = A.SURC
  
  Nobs = A.Nobs               'Number of observables
' Retrieval offset levels wrt flight level
  Nret = A.Nret               'Number of retrieval levels
  For i = 1 To Nret: dZ!(i) = A.dZ(i): Next i
' Flight levels (km)
  NFL = A.NFL                 'Number of flight levels
  For i = 1 To NFL: Zr!(i) = A.Zr(i): Next i
' LO frequencies (GHz)
  Nlo = A.Nlo                 'Number of LO channels
  For i = 1 To Nlo: LO!(i) = A.LO(i): Next i
' Scan mirror elevation angles
  Nel = A.Nel                 'Number of elevation angles
  For i = 1 To Nel: El!(i) = A.El(i): Next i
' IF frequency offsets (GHz) and weights
  Nif = A.Nif                 'Number of IF frequencies
  For j = 1 To Nif
    IFoff!(1, j) = A.IFoff2(j)
    IFwt!(1, j) = A.IFwt2(j)
  Next j
  For i = 2 To Nlo
    For j = 1 To Nif
      IFoff!(i, j) = IFoff(1, j)
      IFwt!(i, j) = IFwt(1, j)
    Next j
  Next i
' Sensitivity matrix: iRC, NFL, Nlo, Nel, bias and slope
  For i = 1 To NFL
    For j = 1 To Nlo
      For k = 1 To Nel
        Smatrix(iRC, i, j, k, 1) = A.SmatrixN1!(i, j, k)
        Smatrix(iRC, i, j, k, 2) = A.SmatrixN2!(i, j, k)
      Next k
    Next j
  Next i
  FoundSmatrix = True
  
  For n = 2 To NFL + 1
    Get #lu%, n, b
    BP(iRC, n - 1, 16) = b.sBP               'Flight level pressure altitude (hPa)
    For i = 1 To Nobs
      OBrms!(n - 1, i) = b.sOBrms(i)         '1-sigma apriori observable errors
      OBav!(iRC, n - 1, i) = b.sOBav(i)      'Archive Average observables
    Next i
    'Debug.Print n - 1; OBrms(n - 1, 1); OBrms(n - 1, 30)
    For i = 1 To Nret
      BP!(iRC, n - 1, i) = b.sBPrl(i)        'Pressure at retrieval levels
      RAav!(iRC, n - 1, i) = fPtoZ(b.sBPrl(i))
      RTav!(iRC, n - 1, i) = b.sRTav(i)      'Average T at retrieval levels
      RMSa!(iRC, n - 1, i) = b.sRMSa(i)      'Variance in T at retrieval levels
      RMSe!(iRC, n - 1, i) = b.sRMSe(i)      'Formal error in T at retrieval levels
      For j = 1 To Nobs
        rc!(iRC, n - 1, i, j) = b.Src(i, j)  '33 retrieval levels, 30 observables
      Next j
    Next i
    DoEvents
  Next n
  Close lu

End Sub

Sub RCwrite(iRC%, FQFN$)
Dim A As RCconfiguration, lu%, i%, j%, k%, n%
Dim b As RC_Set_1FL, CreationDateTime As Date
' RC file
' First record contains configuration information
' It is followed by one record for each flight level with RCs and related info

  lu = FreeFile
  Open FQFN For Random Access Read Write As lu Len = Len(A)
  
  A.RCformat = RCformat(iRC)
  CreationDateTime = Date + Time
  A.CreationDateTime = CreationDateTime
  A.RAOBfilename = RAOBfilename
  A.RCfilename = RCfilename
  A.RAOBcount = RAOBcount
  A.LR1 = LR1                 'LR above top of RAOB
  A.zLRb = zLRb               'LR break altitude
  A.LR2 = LR2                 'LR above break altitude
  A.RecordStep = RecordStep   'Record Step through available RAOBs
  A.RAOBmin = MinimumRAOBz    'Minimum acceptable RAOB altitude
  A.ExcessTamplitude = ExcessTamplitude 'Random Excess Noise Level on Ground
  A.SURC = SURC               'SU (IFB) used to calculate RC set
  
  A.Nobs = Nobs               'Number of observables
' Retrieval offset levels wrt flight level
  A.Nret = Nret               'Number of retrieval levels
  For i = 1 To Nret: A.dZ!(i) = dZ(i): Next i
' Flight levels (km)
  A.NFL = NFL                 'Number of flight levels
  For i = 1 To NFL: A.Zr!(i) = Zr(i): Next i
' LO frequencies (GHz)
  A.Nlo = Nlo                 'Number of LO channels
  For i = 1 To Nlo: A.LO!(i) = LO(i): Next i
' Scan mirror elevation angles
  A.Nel = Nel                 'Number of elevation angles
  For i = 1 To Nel: A.El!(i) = El(i): Next i
' IF frequency offsets (GHz) and weights
  A.Nif = Nif                 'Number of IF frequencies
  For i = 1 To Nlo
    For j = 1 To Nif
      A.IFoff!(i, j) = IFoff(i, j)
      A.IFwt!(i, j) = IFwt(i, j)
    Next j
  Next i
  A.RAOBbias = RAOBbias
  A.CH1LSBloss = CH1LSBloss
  For i = 1 To Channels
    A.CHnLSBloss(i) = CHnLSBloss(i)
  Next i
' Sensitivity matrix: iRC, NFL, Nlo, Nel, bias and slope
  For i = 1 To NFL
    For j = 1 To Nlo
      For k = 1 To Nel
        A.SmatrixN1!(i, j, k) = Smatrix(iRC, i, j, k, 1)
        A.SmatrixN2!(i, j, k) = Smatrix(iRC, i, j, k, 2)
      Next k
    Next j
  Next i
  
  Put #lu%, 1, A
  
  For n = 2 To NFL + 1
    b.sBP = BP(iRC, n - 1, 16)               'Flight level pressure altitude (hPa)
    For i = 1 To Nobs
      b.sOBrms!(i) = OBrms(n - 1, i)         '1-sigma apriori observable errors
      b.sOBav!(i) = OBav(iRC, n - 1, i)      'Archive Average observables
    Next i
    'Debug.Print n - 1; OBrms(n - 1, 1); OBrms(n - 1, 30); b.sOBrms(30)
    For i = 1 To Nret
      b.sBPrl!(i) = BP(iRC, n - 1, i)        'Pressure at retrieval levels
      b.sRTav!(i) = RTav(iRC, n - 1, i)      'Average T at retrieval levels
      b.sRMSa!(i) = RMSa(iRC, n - 1, i)      'Variance in T at retrieval levels
      b.sRMSe!(i) = RMSe(iRC, n - 1, i)      'Formal error in T at retrieval levels
      For j = 1 To Nobs
        b.Src!(i, j) = rc(iRC, n - 1, i, j)  '33 retrieval levels, 30 observables
      Next j
    Next i
    Put #lu%, n, b
    DoEvents
  Next n
  Close lu
  
End Sub

Sub RCread(iRC%, FQFN$)
Dim A As RCconfiguration, lu%, i%, j%, k%, n%
Dim b As RC_Set_1FL, CreationDateTime As Date, File$
' RC file
' First record contains configuration information
' It is followed by one record for each flight level with RCs and related info

  lu = FreeFile
  Open FQFN For Random Access Read Write As lu Len = Len(A)
  Get #lu%, 1, A
  
  RCformat(iRC) = A.RCformat
  CreationDateTime = A.CreationDateTime
  RAOBfilename = Trim(A.RAOBfilename)
  RCfilename = Trim(A.RCfilename)
  If Dir(RAOBfilename) = "" Then
    File$ = RSpath$ + fGetFilename(RAOBfilename)
    If Dir(File$) <> "" Then RAOBfilename = File$
  End If
' Template based RAOB2 files used to be stored in for example
' C:\MTP\RAOB\Missions\TC4\ path
' They were moved in Dec 2007 to for example
' C:\MTP\Data\ER2\TC4\RAOB\ path
' This was done to keep all mission related RAOB information in the MTP\Data\ path
' This check is necessary because the RAOB2 file contains the path that the RCs were
' originally calculated in, which includes all RCs before Dec 2007
'  If Mid$(RAOBfilename, 2, 20) = ":\MTP\RAOB\Missions\" Then
'    RAOBfilename = MNpath$ + "RAOB\" + fGetFilename(RAOBfilename)
'  End If
  RCtemplate$(iRC) = RAOBfilename   'RC RAOB Template (eg BGDH__2003010912.RAOB2)
  If Mid$(fGetFilename(RCtemplate$(iRC)), 6, 1) = "_" Then RAOBreanalysis = True
  RAOBcount = A.RAOBcount
  LR1 = A.LR1                 'LR above top of RAOB
  zLRb = A.zLRb               'LR break altitude
  LR2 = A.LR2                 'LR above break altitude
  RecordStep = A.RecordStep   'Record Step through available RAOBs
  MinimumRAOBz = A.RAOBmin    'Minimum acceptable RAOB altitude
  ExcessTamplitude = A.ExcessTamplitude 'Random Excess Noise Level on Ground
  SURC = A.SURC
  Select Case Trim(SURC)      'SURC only written after 20060128
  Case "DC8", "ER2S", "ER2T"
  Case Else
  SURC = ""                   'Set to null string to avoid crap
  End Select
  
  Nobs = A.Nobs               'Number of observables
' Retrieval offset levels wrt flight level
  Nret = A.Nret               'Number of retrieval levels
  For i = 1 To Nret: dZ!(i) = A.dZ(i): Next i
' Flight levels (km)
  NFL = A.NFL                 'Number of flight levels
  For i = 1 To NFL: Zr!(i) = A.Zr(i): Next i
' LO frequencies (GHz)
  Nlo = A.Nlo                 'Number of LO channels
  For i = 1 To Nlo: LO!(i) = A.LO(i): Next i
' Scan mirror elevation angles
  Nel = A.Nel                 'Number of elevation angles
  For i = 1 To Nel: El!(i) = A.El(i): Next i
' IF frequency offsets (GHz) and weights
  Nif = A.Nif                 'Number of IF frequencies
'  For j = 1 To Nif
'    IFoff!(i, j) = A.IFoff(i, j)
'    IFwt!(i, j) = A.IFwt(i, j)
'  Next j
  For i = 1 To Nlo
    For j = 1 To Nif
      IFoff!(i, j) = A.IFoff(i, j)
      IFwt!(i, j) = A.IFwt(i, j)
    Next j
  Next i
  RAOBbias = A.RAOBbias
  CH1LSBloss = A.CH1LSBloss
  For i = 1 To Channels
    CHnLSBloss(i) = CHnLSBloss(i)
  Next i
' Sensitivity matrix: iRC, NFL, Nlo, Nel, bias and slope
  For i = 1 To NFL
    For j = 1 To Nlo
      For k = 1 To Nel
        Smatrix(iRC, i, j, k, 1) = A.SmatrixN1!(i, j, k)
        Smatrix(iRC, i, j, k, 2) = A.SmatrixN2!(i, j, k)
      Next k
    Next j
  Next i
  FoundSmatrix = True
  
  For n = 2 To NFL + 1
    Get #lu%, n, b
    BP(iRC, n - 1, 16) = b.sBP               'Flight level pressure altitude (hPa)
    For i = 1 To Nobs
      OBrms!(n - 1, i) = b.sOBrms(i)         '1-sigma apriori observable errors
      OBav!(iRC, n - 1, i) = b.sOBav(i)      'Archive Average observables
    Next i
    'Debug.Print n - 1; OBrms(n - 1, 1); OBrms(n - 1, 30)
    For i = 1 To Nret
      BP!(iRC, n - 1, i) = b.sBPrl(i)        'Pressure at retrieval levels
      RAav!(iRC, n - 1, i) = fPtoZ(b.sBPrl(i))
      RTav!(iRC, n - 1, i) = b.sRTav(i)      'Average T at retrieval levels
      RMSa!(iRC, n - 1, i) = b.sRMSa(i)      'Variance in T at retrieval levels
      RMSe!(iRC, n - 1, i) = b.sRMSe(i)      'Formal error in T at retrieval levels
      For j = 1 To Nobs
        rc!(iRC, n - 1, i, j) = b.Src(i, j)  '33 retrieval levels, 30 observables
      Next j
    Next i
    DoEvents
  Next n
  Close lu

  
End Sub

Sub REF2readFLTINFO(FQFN$)
Dim i%, lu%, iNRC%

  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF2)
  Get #lu%, 1, FIR2
  Close lu
  
  Mission$ = Trim(FIR2.Mission)
  FileFormatIn = FIR2.Fileformat
  FltNumber$ = Trim(FIR2.FltNumber)
  Pi$ = FIR2.Pi
  Yeer = FIR2.Yeer
  Doy = FIR2.Doy
  YYYYMMDD = FIR2.YYYYMMDD
  FlightDate = fDate(YYYYMMDD$)
  TotalCycles = FIR2.TotalCycles
  Channels = FIR2.Channels
  
  Drive$ = UCase$(FIR2.Drive)                  'eg C:
  Rdir$ = UCase$(Trim(FIR2.Rdir))              'eg \DC8\
  Rdir2$ = Rdir1$ + "\MTP\Data" + Rdir$        'eg \MTP\Data\DC8\
  Platform$ = Mid$(Rdir$, 2, Len(Rdir$) - 2)   'eg DC8
  AC$ = Mid$(Rdir$, 2, 2)                      'eg DC
  Root$ = Drive$ + Rdir2$                      'eg C:\MTP\Data\DC8\
  MNpath$ = Root$ + Mission$ + "\"             'eg C:\MTP\Data\DC8\SOLVE\
  Call DirCheck(Root$, Mission$)
  RCpath$ = MNpath$ + "RC\"                    'eg C:\MTP\Data\DC8\SOLVE\RC\
  Call DirCheck(MNpath, "RC")
  RSpath$ = MNpath$ + "RAOB\"                  'eg C:\MTP\Data\DC8\SOLVE\RAOB\
  Call DirCheck(MNpath, "RAOB")
  RTpath$ = RSpath$ + "Templates\"             'eg C:\MTP\Data\DC8\SOLVE\RAOB\
  Call DirCheck(RSpath, "Templates")
  MPpath$ = MNpath$ + "MP\"                    'eg C:\MTP\Data\DC8\SOLVE\MP\
  Call DirCheck(MNpath, "MP")
  SUpath$ = MNpath$ + "Setup\"                 'eg C:\MTP\Data\DC8\SOLVE\Setup\
  Call DirCheck(MNpath, "Setup")
  Path$ = MNpath$ + YYYYMMDD$ + "\"            'eg C:\MTP\Data\DC8\SOLVE\20000120\
  Call DirCheck(MNpath, YYYYMMDD$)
  PathDot$ = Path$ + AC$ + YYYYMMDD$ + "."     'eg C:\MTP\Data\DC8\SOLVE\20000120\DC20000120.
  yymmdd$ = Right$(YYYYMMDD$, 6)
  RCs$ = Left$(AC$, 1) + "RC"
' Default Fully-Qualified File Names
  BINfile$ = PathDot$ + "BIN"
  CALfile$ = PathDot$ + "CAL"
  ERFfile$ = PathDot$ + "ERF"
  RAFfile$ = PathDot$ + "RAF"
  INPfile$ = PathDot$ + "INP"
  LOGfile$ = PathDot$ + "LOG"
  MMSfile$ = MNpath$ + "MMS\MM" + YYYYMMDD$ + "." + Platform$
  RAOBfile$ = RSpath$ + Mission$ + ".RAOB2"
  RAWfile$ = PathDot$ + "RAW"
  REFfile$ = PathDot$ + "REF"
  RTSfile$ = PathDot$ + "RTS"
  OUTfile$ = PathDot$ + "OUT"
  MPfile$ = MPpath$ + "MP" + YYYYMMDD$ + "." + Platform$
'  USEfile$ = RCpath$ + USE5$ + ".USE"
  Pgm$ = Trim(FIR2.Pgm)
  USE5$ = FIR2.USE5
  SU$ = RTrim(FIR2.SU)
  NFL = FIR2.NFL
  For i = 1 To NFL: FLA(i) = FIR2.FLA(i): Next i
  PgmDrive$ = FIR2.PgmDrive
  
  Algorithm = FIR2.Algorithm
  MRIavg = FIR2.MRIavg
  MRIrms = FIR2.MRIrms
  
  utMTPcor = FIR2.utMTPcor
  DTavg = FIR2.DTavg
  DTrms = FIR2.DTrms
  
  ALTfujCONST = FIR2.ALTfujCONST
  ALTfujSLOPE = FIR2.ALTfujSLOPE
  OATnavCOR = FIR2.OATnavCOR
  CalSource = Trim(FIR2.CalSource)
  
  GainScale = FIR2.GainScale
  REFsource = FIR2.REFsource
  OATsource = FIR2.OATsource
' OATsource: 1=MTP, 2=A/C, 3=MMS
' GainScale: 1=EQN, 2=ND,  3=OAT
' REFsource: 1=TGT, 2=OAT
  
  If GainScale = 0 Then  'Undefined, generate new definition
    Select Case CalSource$
    Case "DADStgt": GainScale = 3: REFsource = 2: OATsource = 2
    Case "GAIN_EQN": GainScale = 1: REFsource = 1: OATsource = 2
    Case "OATnav": GainScale = 3: REFsource = 1: OATsource = 2
    Case "MMS": GainScale = 3: REFsource = 1: OATsource = 3
    Case "DADS": GainScale = 3: REFsource = 1: OATsource = 2
    Case "ND": GainScale = 2: REFsource = 1: OATsource = 2
    End Select
  End If
  UseMMSpALT = FIR2.UseMMSpALT
  
  LAT1 = FIR2.LAT1
  LAT2 = FIR2.LAT2
  LAT3 = FIR2.LAT3
  LAT4 = FIR2.LAT4
  UserLATs = FIR2.UserLATs
  
  EnableCalfile = FIR2.EnableCalfile
'  DoAll = FIR2.DoAll
  DoAllMask = FIR2.DoAllMask

  EditTropAlt = FIR2.EditTropAlt
  TropAltMin = FIR2.TropAltMin
  TropAltMax = FIR2.TropAltMax
  TropAltPC = FIR2.TropAltPC
  
  EditRetAlt = FIR2.EditRetAlt
  RetAltMin = FIR2.RetAltMin
  RetAltMax = FIR2.RetAltMax
  RetAltPC = FIR2.RetAltPC
  
  EditZtOff = FIR2.EditZtOff
  ZtOffA = FIR2.ZtOffA
  ZtOffB = FIR2.ZtOffB
  ZtOffPC = FIR2.ZtOffPC
  
  EditTemperature = FIR2.EditTemperature
  TemperatureMin = FIR2.TemperatureMin
  TemperatureMax = FIR2.TemperatureMax
  TemperaturePC = FIR2.TemperaturePC
  
  EditPitch = FIR2.EditPitch
  ePitchMin = FIR2.ePitchMin
  ePitchMax = FIR2.ePitchMax
  PitchPC = FIR2.PitchPC
  
  EditRoll = FIR2.EditRoll
  eRollMin = FIR2.eRollMin
  eRollMax = FIR2.eRollMax
  RollPC = FIR2.RollPC
  
  EditNav = FIR2.EditNav
  NavMin = FIR2.NavMin
  NavMax = FIR2.NavMax
  NavQualPC = FIR2.NavQualPC
  
  EditTA = FIR2.EditTA
  TAmin = FIR2.TAmin
  TAmax = FIR2.TAmax
  TAqualPC = FIR2.TAqualPC
  
  EditCts = FIR2.EditCts
  CtsMin = FIR2.CtsMin
  CtsMax = FIR2.CtsMax
  CtsQualPC = FIR2.CtsQualPC
  
  EditCycle = FIR2.EditCycle
  CycleQualPC = FIR2.CycleQualPC
  
  EditTtgt = FIR2.EditTtgt
  TtgtMin = FIR2.TtgtMin
  TtgtMax = FIR2.TtgtMax
  TtgtPC = FIR2.TtgtPC
  
  EditNlev = FIR2.EditNlev
  NlevMin = FIR2.NlevMin
  NlevMax = FIR2.NlevMax
  NlevPC = FIR2.NlevPC
  
  EditRAWbad = FIR2.EditRAWbad
  RAWbadmin = FIR2.RAWbadmin
  RAWbadmax = FIR2.RAWbadmax
  RAWbadPC = FIR2.RAWbadPC
  
  EditRate = FIR2.EditRate
  RateMin = FIR2.RateMin
  RateMax = FIR2.RateMax
  RatePC = FIR2.RatePC
  
  EditOATtrop = FIR2.EditOATtrop
  OATtropPC = FIR2.OATtropPC
  
  OATzt10 = FIR2.OATzt10
  Tzt10 = FIR2.Tzt10
  OATzt20 = FIR2.OATzt20
  Tzt20 = FIR2.Tzt20
  OATks10 = FIR2.OATks10
  OATks20 = FIR2.OATks20
  
  OATzt11 = FIR2.OATzt11
  Tzt11 = FIR2.Tzt11
  OATzt21 = FIR2.OATzt21
  Tzt21 = FIR2.Tzt21
  OATks11 = FIR2.OATks11
  OATks21 = FIR2.OATks21
  
  OATzt12 = FIR2.OATzt12
  Tzt12 = FIR2.Tzt12
  OATzt22 = FIR2.OATzt22
  Tzt22 = FIR2.Tzt22
  OATks12 = FIR2.OATks12
  OATks22 = FIR2.OATks22
  
  OATzt13 = FIR2.OATzt13
  Tzt13 = FIR2.Tzt13
  OATzt23 = FIR2.OATzt23
  Tzt23 = FIR2.Tzt23
  OATks13 = FIR2.OATks13
  OATks23 = FIR2.OATks23
'
  OATzt14 = FIR2.OATzt14
  Tzt14 = FIR2.Tzt14
  OATzt24 = FIR2.OATzt24
  Tzt24 = FIR2.Tzt24
  
' HISTORY information
  CALversion = FIR2.CALversion
  MAKEversion = FIR2.MAKEversion
  EDITversion = FIR2.EDITversion
  FLTINFOversion = FIR2.FLTINFOversion
  
  Tstamp = FIR2.Tstamp
  RAWstamp = FIR2.RAWstamp
  MMSstamp = FIR2.MMSstamp
  REFstamp = FIR2.REFstamp
  ERFstamp = REFstamp
  ERFstamp = FIR2.ERFstamp
  CTCstamp = FIR2.CTCstamp
  
  Ncts = FIR2.Ncts
  If Ncts = 0 Then Ncts = 12
  
  ATPrange = FIR2.ATPrange
  If ATPrange = 0 Then ATPrange = 20
  
End Sub

Function fREFreadFileFormat(FQFN$)
Dim lu%

  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF2)
  Get #lu%, 1, FIR
  Close lu
  
  fREFreadFileFormat = FIR.Fileformat

End Function

Sub REF2writeCALFILE(FQFN$)
Dim i%, j%, lu%, iNRC%

  CFR2.Gendate = Gendate$
  For i = 1 To 10
    CFR2.WCTdates(i) = WCTdates$(i)
  Next i
  CFR2.UTstart = UTstart
  CFR2.UTend = UTend
  CFR2.Channels = Channels
  CFR2.Nel = Nel
  CFR2.Emissivity = Emissivity
  CFR2.Reflectivity = Reflectivity
  CFR2.DeltaTmin = DeltaTmin
  CFR2.RHS = RHS
  CFR2.LocHor = LocHor
  CFR2.Targets = Targets
  
' "FIT_INFO"
  CFR2.Nfit = Nfit
  For i = 1 To Nfit
    CFR2.NP(i) = NP$(i)
  Next i
  
  For i = 1 To Channels
    CFR2.GOF(i) = GOF(i)
    For j = 1 To Nfit
      CFR2.GEC(i, j) = GEC(i, j)
    Next j
  Next i
  
  aTGToffset(0) = TGToffset
  aMXRoffset(0) = MXRoffset
  aNAVoffset(0) = NAVoffset
  aNDoffset(0) = NDoffset
  
  CFR2.aTGToffset(0) = aTGToffset(0)
  CFR2.aMXRoffset(0) = aMXRoffset(0)
  CFR2.aNAVoffset(0) = aNAVoffset(0)
  CFR2.aNDoffset(0) = aNDoffset(0)
  CFR2.aTGToffset(1) = aTGToffset(1)
  CFR2.aMXRoffset(1) = aMXRoffset(1)
  CFR2.aNAVoffset(1) = aNAVoffset(1)
  CFR2.aNDoffset(1) = aNDoffset(1)
  CFR2.aTGToffset(2) = aTGToffset(2)
  CFR2.aMXRoffset(2) = aMXRoffset(2)
  CFR2.aNAVoffset(2) = aNAVoffset(2)
  CFR2.aNDoffset(2) = aNDoffset(2)
  CFR2.aTGToffset(3) = aTGToffset(3)
  CFR2.aMXRoffset(3) = aMXRoffset(3)
  CFR2.aNAVoffset(3) = aNAVoffset(3)
  CFR2.aNDoffset(3) = aNDoffset(3)
  
' "WINDOW_CORRECTIONS"
  CFR2.EnableWCT = EnableWCT
  For i = 1 To Channels
    For j = 1 To Nel
      CFR2.WINcor(i, j) = WINcor(i, j)
    Next j
  Next i
' RAW counts editting criteria
  CFR2.CMAcycles = CMAcycles
  CFR2.CMAcycles2 = CMAcycles2
  CFR2.RFIthreshold = RFIthreshold
  CFR2.RFIiterations = RFIiterations
  CFR2.RFIiterations2 = RFIiterations2
  CFR2.MUXthreshold = MUXthreshold
  CFR2.BadCycles = BadCycles
  CFR2.Badcycles2 = Badcycles2
  CFR2.UseMAforCB = UseMAforCB
  CFR2.UseMAforCS = UseMAforCS
  CFR2.UseMAforCSgain = UseMAforCSgain
  CFR2.UseMAforCN = UseMAforCN
  CFR2.UseMAforTtgt = UseMAforTtgt
  CFR2.UseMAforTifa = UseMAforTifa
  CFR2.RAWextension = RAWextension
  CFR2.ScanTime = ScanTime
  
' Gain Limits
  For i = 1 To Channels
    CFR2.GeqnMin(i) = GeqnMin(i)
    CFR2.GeqnMax(i) = GeqnMax(i)
    CFR2.GnavMin(i) = GnavMin(i)
    CFR2.GnavMax(i) = GnavMax(i)
    CFR2.GndMin(i) = GndMin(i)
    CFR2.GndMax(i) = GndMax(i)
  Next i
  
' Channel Weights
  For i = 1 To 3: CFR2.ChInfo(i) = ChInfo(i):  Next i

' Fit Region
  CFR2.TBfitX1 = TBfitX1
  CFR2.TBfitX2 = TBfitX2
  CFR2.TBfitY1 = TBfitY1
  CFR2.TBfitY2 = TBfitY2

' Noise Diode Noise Temperatures
  For i = 1 To Channels
    CFR2.Cnd0(i) = Cnd0(i)
    CFR2.Cnd1(i) = Cnd1(i)
    CFR2.Cnd2(i) = Cnd2(i)
  Next i
  CFR2.TrefND = TrefND
  
  CFR2.MTPyaw = MTPyaw
  CFR2.MTPpitch = MTPpitch
  CFR2.MTProll = MTProll
  CFR2.MTPfiduciary = MTPfiduciary
  CFR2.fEcCount = fEcCount
  For i = 1 To 10
    CFR2.ElSUI(i) = ElSUI(i)
  Next i
  
  CFR2.NRC = NRC
  For i = 0 To NRC - 1
    CFR2.Reg(i) = Reg(i)
    CFR2.RCformat(i) = RCformat(i)
    CFR2.RCuse(i) = RCuse(i)
  Next i
  
  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF2)
  Put #lu%, 3, CFR2
  Close lu
  DoEvents

End Sub

Sub REF2writeFLTINFO(FQFN$)
Dim i%, lu%, iNRC%
  ' Only allow DoAll flag to be set in default FLTINFO file!!!
  'If FQFN$ <> "C:\MTP\Setup\FLTINFO.REF" Then DoAll = False
  ReadSETUP ("SYSTEM")
  Drive$ = DataDrive$
  PgmDrive$ = ProgramDrive$

  FIR2.Tstamp = Date + Time
  FIR2.RAWstamp = RAWstamp
  FIR2.REFstamp = REFstamp
  FIR2.ERFstamp = ERFstamp
  FIR2.CTCstamp = CTCstamp
  
  FIR2.Mission = Mission$
  FIR2.Fileformat = FileFormatOut
  FIR2.FltNumber = FltNumber$
  FIR2.Pi = Pi$
  FIR2.Yeer = Yeer
  FIR2.Doy = Doy
  FIR2.YYYYMMDD = YYYYMMDD
  FIR2.TotalCycles = TotalCycles
  FIR2.Channels = Channels
  
  FIR2.Drive = Drive$
  FIR2.Rdir = Rdir$
  FIR2.Path = Path$
  FIR2.Pgm = Pgm$
  FIR2.USE5 = USE5$
  FIR2.SU = SU$
  FIR2.NFL = NFL
  For i = 1 To NFL: FIR2.FLA(i) = FLA(i): Next i
  FIR2.PgmDrive = PgmDrive$
  
  FIR2.Algorithm = 0    'Algorithm
  FIR2.MRIavg = MRIavg
  FIR2.MRIrms = MRIrms
  
  FIR2.utMTPcor = utMTPcor
  FIR2.DTavg = DTavg
  FIR2.DTrms = DTrms
  FIR2.ALTfujCONST = ALTfujCONST
  FIR2.ALTfujSLOPE = ALTfujSLOPE
  FIR2.OATnavCOR = OATnavCOR
  FIR2.CalSource = CalSource
  FIR2.GainScale = GainScale
  FIR2.REFsource = REFsource
  FIR2.OATsource = OATsource
  
  FIR2.UseMMSpALT = UseMMSpALT
  
  FIR2.LAT1 = LAT1
  FIR2.LAT2 = LAT2
  FIR2.LAT3 = LAT3
  FIR2.LAT4 = LAT4
  FIR2.UserLATs = UserLATs
  
  FIR2.EnableCalfile = EnableCalfile
  FIR2.DoAll = DoAll
  FIR2.DoAllMask = DoAllMask
  
  FIR2.EditTropAlt = EditTropAlt
  FIR2.TropAltMin = TropAltMin
  FIR2.TropAltMax = TropAltMax
  FIR2.TropAltPC = TropAltPC
  
  FIR2.EditRetAlt = EditRetAlt
  FIR2.RetAltMin = RetAltMin
  FIR2.RetAltMax = RetAltMax
  FIR2.RetAltPC = RetAltPC
  
  FIR2.EditZtOff = EditZtOff
  FIR2.ZtOffA = ZtOffA
  FIR2.ZtOffB = ZtOffB
  FIR2.ZtOffPC = ZtOffPC
  
  FIR2.EditTemperature = EditTemperature
  FIR2.TemperatureMin = TemperatureMin
  FIR2.TemperatureMax = TemperatureMax
  FIR2.TemperaturePC = TemperaturePC
  
  FIR2.EditPitch = EditPitch
  FIR2.ePitchMin = ePitchMin
  FIR2.ePitchMax = ePitchMax
  FIR2.PitchPC = PitchPC
  
  FIR2.EditRoll = EditRoll
  FIR2.eRollMin = eRollMin
  FIR2.eRollMax = eRollMax
  FIR2.RollPC = RollPC
  
  FIR2.EditNav = EditNav
  FIR2.NavMin = NavMin
  FIR2.NavMax = NavMax
  FIR2.NavQualPC = NavQualPC
  
  FIR2.EditTA = EditTA
  FIR2.TAmin = TAmin
  FIR2.TAmax = TAmax
  FIR2.TAqualPC = TAqualPC
  
  FIR2.EditCts = EditCts
  FIR2.CtsMin = CtsMin
  FIR2.CtsMax = CtsMax
  FIR2.CtsQualPC = CtsQualPC
  
  FIR2.EditCycle = EditCycle
  FIR2.CycleQualPC = CycleQualPC
  
  FIR2.EditTtgt = EditTtgt
  FIR2.TtgtMin = TtgtMin
  FIR2.TtgtMax = TtgtMax
  FIR2.TtgtPC = TtgtPC
  
  FIR2.EditNlev = EditNlev
  FIR2.NlevMin = NlevMin
  FIR2.NlevMax = NlevMax
  FIR2.NlevPC = NlevPC
  
  FIR2.EditRAWbad = EditRAWbad
  FIR2.RAWbadmin = RAWbadmin
  FIR2.RAWbadmax = RAWbadmax
  FIR2.RAWbadPC = RAWbadPC
  
  FIR2.EditRate = EditRate
  FIR2.RateMin = RateMin
  FIR2.RateMax = RateMax
  FIR2.RatePC = RatePC

  
  FIR2.EditOATtrop = EditOATtrop
  FIR2.OATtropPC = OATtropPC
  
  FIR2.OATzt10 = OATzt10
  FIR2.Tzt10 = Tzt10
  FIR2.OATzt20 = OATzt20
  FIR2.Tzt20 = Tzt20
  FIR2.OATks10 = OATks10
  FIR2.OATks20 = OATks20
  
  FIR2.OATzt11 = OATzt11
  FIR2.Tzt11 = Tzt11
  FIR2.OATzt21 = OATzt21
  FIR2.Tzt21 = Tzt21
  FIR2.OATks11 = OATks11
  FIR2.OATks21 = OATks21
  
  FIR2.OATzt12 = OATzt12
  FIR2.Tzt12 = Tzt12
  FIR2.OATzt22 = OATzt22
  FIR2.Tzt22 = Tzt22
  FIR2.OATks12 = OATks12
  FIR2.OATks22 = OATks22
  
  FIR2.OATzt13 = OATzt13
  FIR2.Tzt13 = Tzt13
  FIR2.OATzt23 = OATzt23
  FIR2.Tzt23 = Tzt23
  FIR2.OATks13 = OATks13
  FIR2.OATks23 = OATks23
'
  FIR2.OATzt14 = OATzt14
  FIR2.Tzt14 = Tzt14
  FIR2.OATzt24 = OATzt24
  FIR2.Tzt24 = Tzt24
  
' HISTORY information
  FIR2.CALversion = CALversion
  FIR2.MAKEversion = MAKEversion
  FIR2.EDITversion = EDITversion
  FLTINFOversion = FileDateTime(ProgramDrive$ + "\MTP\VB6\BAS\FLTINFO.bas") 'FLTINFO version
  FIR2.FLTINFOversion = FLTINFOversion
    
  FIR2.Tstamp = Tstamp
  FIR2.RAWstamp = RAWstamp
  FIR2.MMSstamp = MMSstamp
  FIR2.REFstamp = REFstamp
  FIR2.ERFstamp = ERFstamp
  FIR2.CTCstamp = CTCstamp
  
  FIR2.Ncts = Ncts
  FIR2.ATPrange = ATPrange
  
  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF2)
  Put #lu%, 1, FIR2
  Close lu
  
End Sub

Sub REFopen(REFfile$)
    
  FileFormatIn = fREFreadFileFormat(REFfile$)
  If FileFormatIn = 0 Then FileFormatIn = 33
  REFlu = FreeFile
  Select Case FileFormatIn
  Case 32
    Open REFfile$ For Random Access Read Write As REFlu Len = Len(REF)
  Case 33
    Open REFfile$ For Random Access Read Write As REFlu Len = Len(REF2)
  End Select
  
End Sub

Sub BINopen(BINfile$)
    
  BINlu = FreeFile
  FileFormatIn = fREFreadFileFormat(BINfile$)

  Select Case FileFormatIn
  Case 32
    Open BINfile$ For Random Access Read Write As BINlu Len = Len(REF)
  Case 33
    Open BINfile$ For Random Access Read Write As BINlu Len = Len(REF2)
  End Select
  
End Sub
Sub REFread(lu%, Record%)
  
  Select Case FileFormatIn
  Case 32: Call REFread32(lu%, Record%)
  Case 33: Call REFread33(lu%, Record%)
  End Select
  
End Sub

Sub CheckRCconfig(f As Form)

'  Print #CFGlu, Format(IFwt(Nif), "#0.0000")
End Sub

Function fGetLRextension(ByRef RMSfilename$, LR1!, LR2!, zLRb!) As Boolean
Dim lu%, nRAOBtemplates%, iY, iM, iD, UT0, LR11, LR12, zLRb1, LR21, LR22, zLRb2, A$, i%, ii%
Dim RAOBfilename$, RAOBfilename1$, RAOBfilename2$, RAOB1$, RAOB2$, RAOB1x$, RAOB2x$
Dim RAOBrangeUsedFile$, iNum%

    RAOBrangeUsedFile$ = MNpath$ + "RAOB\" + Mission$ + "_RAOBrangeUnique.txt"
    If Dir$(RAOBrangeUsedFile$, vbNormal) = "" Then
      MsgBox "Warning: " + RAOBrangeUsedFile$ + " does not exist!", vbOKOnly
      fGetLRextension = False
      Exit Function
    End If
  ' Open MissionRAOBrange.txt file and set LR extension parameters
    lu = FreeFile
    Open RAOBrangeUsedFile$ For Input As lu 'eg SOLVE2_RAOBrangeUsed.txt
    Input #lu, A$                           'skip header line
    nRAOBtemplates = 0
    Do      'Read list of RAOBs within 200 km of Flight Track
      Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, zLRb1, LR21, LR22, zLRb2, A$
      i = InStr(1, A$, vbTab)
      RAOB1$ = Left$(A$, i - 1)
      ' Generate default filename for RAOB template file of soundings in case RMS command is used
      Select Case Len(RAOB1$)
      Case 5: RAOBfilename$ = RAOB1$ + "_"
      Case 4: RAOBfilename$ = RAOB1$ + "__"
      Case 3: RAOBfilename$ = RAOB1$ + "___"
      End Select
      RAOBfilename1$ = RAOBfilename$ + Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
      If UT0 > 42.3 Then  ' Allow for day change if after 1200UT
        RAOBfilename2$ = RAOBfilename$ + Format(iY, "0000") + Format(iM, "00") + Format(iD + 1, "00")
      Else
        RAOBfilename2$ = ""
      End If
      ii = InStr(i + 1, A$, vbTab)
      RAOB2x$ = Mid$(A$, i + 1, ii - i - 1)
      'Debug.Print RMSfilename$; " "; RAOBfilename1; " "; RAOBfilename2
    Loop Until Left$(RMSfilename, 14) = RAOBfilename1 Or Left$(RMSfilename, 14) = RAOBfilename2 Or EOF(lu)
    If EOF(lu) Then
      fGetLRextension = False
    Else
      fGetLRextension = True
      LR1 = LR11
      LR2 = LR12
      zLRb = zLRb1
    End If
    Close lu

End Function

Function fRMSfilename(WMO4l$, WMOnumber&, iY, iM%, iD%, iH%) As String
Dim RMSfilename$, Status As Boolean, Record&

  ' Generate default filename for RAOB template file of soundings in case RMS command is used
  If WMO4l = "XXXX" Then  'First make sure ICAO name does not exist as MTP default is ICAO
    Status = fGetUAsite(Str(WMOnumber), Record)
    If Status And Left$(GPicao, 4) <> "9999" Then WMO4l = Left$(GPicao, 4)
  End If
  
  If WMO4l = "XXXX" Or WMO4l = "9999" Then
    RMSfilename$ = Format(WMOnumber, "00000") + "_"
  Else
    If InStr(WMO4l, " ") = 0 Then
      RMSfilename$ = WMO4l + "__"
    Else
      RMSfilename$ = Left$(WMO4l, InStr(WMO4l, " ") - 1) + "___"
    End If
  End If
  fRMSfilename = RMSfilename$ + Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00") + Format(Ihour, "00")

End Function

Public Sub GoSleep(rSeconds As Single)      'pass seconds as single to allow decimal
    Const clMillPerSec As Long = 1000       'milliseconds per second
    Dim lSeconds As Long
    lSeconds = rSeconds * clMillPerSec      'convert to long
    Sleep lSeconds                          'call sleep
End Sub


Sub Read_Gaines_Hipskind_Format(lu%, Record%, Prefix$)
Dim Org$, Sname$, Mname$, Ivol%, Nvol%, dX(1 To 2), ReduxDate$, n%
Dim Xname$(1 To 2), NSCOML%, NNCOML%
Static VSCAL!(1 To 100), Vmiss!(1 To 100), Vname$(1 To 100)
Static ASCAL!(1 To 100), Amiss!(1 To 100), Aname$(1 To 100)
' If Record=0, read header, otherwise read data
Dim A$, i%, x2!, x1!(1 To 100), NXm1%, V!(1 To 100, 1 To 100), AuxV!(1 To 100), num!(1 To 100)
Dim PV!(1 To 100)
Static nHeader%, FFI%, NV%, NauxV%, NX%(1 To 100), NXDEF%(1 To 100), ZZp!(1 To 100)

If Record = 0 Then
  Input #lu, nHeader%, FFI            '1  Number of header records and File Format
  Select Case FFI
  Case 1001                           ' XS File Format
    Line Input #lu, Pi$               '2  PI
    Line Input #lu, Org$              '3  Organization
    Line Input #lu, Sname$            '4  Instrument
    Line Input #lu, Mname$            '5  Mission
    Mission$ = Mname$
    Call Read_N_Numbers(lu, 2, num())
    Ivol = num(1)
    Nvol = num(2)                     '6
    Line Input #lu, A$                '7  Flight date and Reduction Date
'   1995 12 11  1996 06 26  {FLT DATE & REDUCTION DATE}
'   12345678901234567890123
    YYYYMMDD$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
    ReduxDate$ = Mid$(A$, 13, 4) + Mid$(A$, 18, 2) + Mid$(A$, 21, 2)
    Call Read_N_Numbers(lu, 1, num())
    dX(1) = num(1)
    Line Input #lu, Xname$(1)                '7  Flight date and Reduction Date
    
    Call Read_N_Numbers(lu, 1, num())
    NV = num(1)                       '11 Number of Primary variables
    Call Read_N_Numbers(lu, NV, num())
    For n = 1 To NV: VSCAL(n) = num(n): Next n
    Call Read_N_Numbers(lu, NV, num())
    For n = 1 To NV: Vmiss(n) = num(n): Next n
    For n = 1 To NV: Line Input #lu, Vname(n):  Next n
    Call Read_N_Numbers(lu, 1, num())
    NSCOML = num(1)                   ' Number of special comment lines
    For n = 1 To NSCOML: Line Input #lu, A$:  Next n
    Call Read_N_Numbers(lu, 1, num())
    NNCOML = num(1)                   ' Number of normal comment lines
    For n = 1 To NNCOML: Line Input #lu, A$:  Next n
    Record = 0
  Case 2010                           ' XS File Format
    Line Input #lu, Pi$               '2  PI
    Line Input #lu, Org$              '3  Organization
    Line Input #lu, Sname$            '4  Instrument
    Line Input #lu, Mname$            '5  Mission
    Mission$ = Mname$
    If Mission$ = "TOTE/VOTE" Then Mission$ = "TOTE_VOTE"
    Call Read_N_Numbers(lu, 2, num())
    Ivol = num(1)
    Nvol = num(2)                     '6
    Line Input #lu, A$                '7  Flight date and Reduction Date
'   1995 12 11  1996 06 26  {FLT DATE & REDUCTION DATE}
'   12345678901234567890123
    YYYYMMDD$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
    ReduxDate$ = Mid$(A$, 13, 4) + Mid$(A$, 18, 2) + Mid$(A$, 21, 2)
    Call Read_N_Numbers(lu, 2, num())
    dX(1) = num(1)
    dX(2) = num(2)                    '8  Interval between primary variables, 0.0 is non-uniform
    Call Read_N_Numbers(lu, 1, num())
    NX(1) = num(1)
    Call Read_N_Numbers(lu, 1, num())
    NXDEF(1) = num(1)
    For i = 1 To NXDEF(1): Input #lu, x1(i): Next i
    For i = 1 To NXDEF(1): ZZp(i) = fPtoZ(x1(i)): Next i  'convert p to Zp
    Line Input #lu, Xname$(1)         '9  Mission
    Line Input #lu, Xname$(2)         '10 Mission
    Call Read_N_Numbers(lu, 1, num())
    NV = num(1)                       '11 Number of Primary variables
    Call Read_N_Numbers(lu, NV, num())
    For n = 1 To NV: VSCAL(n) = num(n): Next n
    Call Read_N_Numbers(lu, NV, num())
    For n = 1 To NV: Vmiss(n) = num(n): Next n
    For n = 1 To NV: Line Input #lu, Vname(n):  Next n
    Call Read_N_Numbers(lu, 1, num())
    NauxV = num(1)                    ' Number of Auxiliary Variables
    Call Read_N_Numbers(lu, NauxV, num())
    For n = 1 To NauxV: ASCAL(n) = num(n): Next n
    Call Read_N_Numbers(lu, NauxV, num())
    For n = 1 To NauxV: Amiss(n) = num(n): Next n
    For n = 1 To NauxV: Line Input #lu, Aname(n):  Next n
    Call Read_N_Numbers(lu, 1, num())
    NSCOML = num(1)                   ' Number of special comment lines
    For n = 1 To NSCOML: Line Input #lu, A$:  Next n
    Call Read_N_Numbers(lu, 1, num())
    NNCOML = num(1)                   ' Number of normal comment lines
    For n = 1 To NNCOML: Line Input #lu, A$:  Next n
    Record = 0
  Case 2110
    Line Input #lu, Pi$               '2  PI
    Line Input #lu, Org$              '3  Organization
    Line Input #lu, Sname$            '4  Instrument
    Line Input #lu, Mname$            '5  Mission
    Mission$ = Mname$
    If Mission$ = "TOTE/VOTE" Then Mission$ = "TOTE_VOTE"
    Call Read_N_Numbers(lu, 2, num())
    Ivol = num(1)
    Nvol = num(2)                     '6
    Line Input #lu, A$                '7  Flight date and Reduction Date
'   1995 12 11  1996 06 26  {FLT DATE & REDUCTION DATE}
'   12345678901234567890123
    YYYYMMDD$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
    ReduxDate$ = Mid$(A$, 13, 4) + Mid$(A$, 18, 2) + Mid$(A$, 21, 2)
    Call Read_N_Numbers(lu, 2, num())
    dX(1) = num(1)
    dX(2) = num(2)                    '8  Interval between primary variables, 0.0 is non-uniform
    Line Input #lu, Xname$(1)         '9  Mission
    Line Input #lu, Xname$(2)         '10 Mission
    Call Read_N_Numbers(lu, 1, num())
    NV = num(1)                       '11 Number of Primary variables
    Call Read_N_Numbers(lu, NV, num())
    For n = 1 To NV: VSCAL(n) = num(n): Next n
    Call Read_N_Numbers(lu, NV, num())
    For n = 1 To NV: Vmiss(n) = num(n): Next n
    For n = 1 To NV: Line Input #lu, Vname(n):  Next n
    Call Read_N_Numbers(lu, 1, num())
    NauxV = num(1)                    ' Number of Auxiliary Variables
    Call Read_N_Numbers(lu, NauxV, num())
    For n = 1 To NauxV: ASCAL(n) = num(n): Next n
    Call Read_N_Numbers(lu, NauxV, num())
    For n = 1 To NauxV: Amiss(n) = num(n): Next n
    For n = 1 To NauxV: Line Input #lu, Aname(n):  Next n
    Call Read_N_Numbers(lu, 1, num())
    NSCOML = num(1)                   ' Number of special comment lines
    For n = 1 To NSCOML
      Line Input #lu, A$
      Select Case n
      Case 1
      Case 2
        i = InStr(A$, ":")
        Longitude = Val(Right$(A$, Len(A$) - i - 1))
      Case 3
        i = InStr(A$, ":")
        Latitude = Val(Right$(A$, Len(A$) - i - 1))
      Case 4
      End Select
      
    Next n
    Call Read_N_Numbers(lu, 1, num())
    NNCOML = num(1)                   ' Number of normal comment lines
    For n = 1 To NNCOML: Line Input #lu, A$:  Next n
    Record = 0
  End Select
Else
  Select Case FFI
  Case 1001
  
  Case 2010
    Input #lu, x2                                  ' Read Second Primary Variable
    For n = 1 To NauxV: Input #lu, AuxV(n): Next n 'Read Auxiliary Variables
    For n = 1 To NV
      For i = 1 To NX(1): Input #lu, V(i, n): Next i  'Read Primary Variables
    Next n                                'Read Secondary Variables
    Select Case Prefix$
    Case "XS"                             ' GMAO Assimilation Data
      UTsec = x2
      Latitude = AuxV(2) * ASCAL(2)
      Longitude = AuxV(1) * ASCAL(1)
      For n = 1 To NV
        For i = 1 To NX(1): PV(i) = V(i, 1) * VSCAL(1): Next i
        For i = 1 To NX(1): TTT(i) = V(i, 2) * VSCAL(2): Next i
        For i = 1 To NX(1): Zgeo(i) = V(i, 3) * VSCAL(3) / 1000#: Next i
        For i = 1 To NX(1): zzz(i) = ZZp(i): Next i
      ' Ignore winds for now
      Next n                                'Read Secondary Variables
      For i = 1 To NX(1)
        sZZZ(i) = zzz(i) / 100#
        sTTT(i) = TTT(i) / 10#
        sZg(i) = Zgeo(i) / 100#
      Next i

    End Select
    
  Case 2110
    Input #lu, x2, NXm1                   ' Read Second Primary Variable and number of First Primary Variables
    For n = 2 To NauxV: Input #lu, AuxV(n): Next n 'Read Auxiliary Variables
    For i = 1 To NXm1
      Input #lu, x1(i)                    ' Read First Primary Variable
      For n = 1 To NV: Input #lu, V(i, n): Next n  'Read Secondary Variables
    Next i
    Select Case Prefix$
    Case "MP"                             ' MTP Data
      UTsec = x2
      AuxV(1) = NXm1
      Nlev = AuxV(1) * ASCAL(1)
      Nlev1 = 33 - Nlev + 1
      Nlev2 = 33
      pALT = AuxV(2) * ASCAL(2)
      Pitch = AuxV(3) * ASCAL(3)
      Roll = AuxV(4) * ASCAL(4)
      OATmtp = AuxV(5) * ASCAL(5)
      Zt1 = AuxV(6) * ASCAL(6)
      Zt2 = AuxV(7) * ASCAL(7)
      Th1 = AuxV(8) * ASCAL(8)
      Th2 = AuxV(9) * ASCAL(9)
      Latitude = AuxV(10) * ASCAL(10)
      Longitude = AuxV(11) * ASCAL(11)
      LRac = AuxV(12) * ASCAL(12)
      MRI = AuxV(13) * ASCAL(13)
      GoodScan = True
      GoodTrop = True
      For i = 1 To Nlev
        zzz(33 - i + 1) = V(i, 1) * VSCAL(1)
        TTT(33 - i + 1) = V(i, 2) * VSCAL(2)
      Next i
      For i = 1 To Nlev
        sZZZ(i) = zzz(i) / 100#
        sTTT(i) = TTT(i) / 10#
      Next i
      
    End Select
  End Select
End If

End Sub

Sub Read_N_Numbers(lu%, n%, V!())
Dim A$, i%, j%, iStart%

  Line Input #lu, A$
  i = 0
  For j = 1 To n
    Do
      iStart = i + 1
      i = InStr(iStart, A$, " ", vbTextCompare)
    Loop Until i > iStart Or i = 0
    If i = 0 Then
      V(j) = Val(Mid$(A$, iStart, Len(A$) - iStart + 1))
    Else
      V(j) = Val(Mid$(A$, iStart, i - iStart))
    End If
  Next j
  
End Sub


Function fReadFile_XXX(Ext$, SUname$) As Boolean
Dim i%, j%, lu%, XXXfile$, IC%, ir%, n%

  If Val(YYYYMMDD$) > 20050601 And Ext$ = "IFB" Then
    XXXfile$ = MNpath$ + "Setup\" + Mission$ + "_" + SU$ + "_" + Ext$ + ".txt"
  Else
    XXXfile$ = MNpath$ + "Setup\" + Mission$ + "_" + Ext$ + ".txt"
  End If
  If Len(Dir$(XXXfile$)) = 0 Then
    MsgBox "The file: " & XXXfile$ & "does not exist!" & vbCrLf & "Create it before proceeding!", vbOKOnly
    fReadFile_XXX = False
  End If
  lu = FreeFile
  Open XXXfile$ For Input As lu
  
  Select Case Ext$
  Case "IFB"
    Input #lu, Nlo             'Number of channels
    Input #lu, Nif             'Number of response segments in IF
    For i = 1 To Nlo
      Input #lu, CHnLSBloss(i) 'CHn LSB RF loss percentage
    Next i
    CH1LSBloss = CHnLSBloss(1)
    For i = 1 To Nlo
      For j = 1 To Nif
        Input #lu, n, IFoff(i, j), IFwt(i, j)
        IFoff(i, j) = IFoff(i, j) / 1000#
        'Debug.Print n; IFoff(i, j), IFwt(i, j)
      Next j
    Next i
    Close lu
    fReadFile_XXX = True

  End Select
  
End Function

Sub ReadInRC(RCpathToDot$, iRC%)
Dim A$, FI$, lu%, i%, j%, k%, L%, V!, Filename$, bpv!, Gendate$
' Routine to read in retrieval coefficients
' Check first if .RCF file exists, and if not check for older .CFG file
' .RCF files are binary, have 30 observables and include .cfg info and all flight levels
' .CFG files are ascii, have 28 observables, and have separate files for each FL, and .CFG
Static MissingRCflag As Boolean

  If Dir(RCpathToDot$ + ".RCF") = "" Then
    If Dir(RCpathToDot$ + ".CFG") = "" Then
      If Not MissingRCflag Then MsgBox "The RC files: " + RCpathToDot$ + ".cfg or .rcf" + vbCrLf + "Could not be found!", vbOKOnly
      MissingRCflag = True
      Exit Sub
    Else
      Call ReadRCconfig(iRC, RCpathToDot$ + ".cfg")
      For i = 1 To NFL
        If Dir(RCpathToDot$ + "." + Format$(FLA(i), "0000")) = "" Then
          If Not MissingRCflag Then MsgBox "The RC file: " + RCpathToDot$ + "." + Format$(FLA(i), "0000") + vbCrLf + "Could not be found!", vbOKOnly
          MissingRCflag = True
          Exit Sub
        End If
        lu% = FreeFile
        Open RCpathToDot$ + "." + Format$(FLA(i), "0000") For Input As #lu%
'123456789012345678901234567890123456789012345678901234567890
'DRCNA012.0850  330.99  089 Generated: 07-23-2003 19:33:19
        Input #lu%, A$
        Filename = Left$(A$, 13)       'RC Filename
        bpv = Mid$(A$, 14, 9)          'FL Barometric pressure
        RAOBcount% = Mid$(A$, 22, 5)     'Number of soundings used to calculate RC set
        Gendate$ = Mid$(A$, 28, 30)    'Date RCs were calculate
      ' Blank line
        Input #lu%, A$
      ' A Priori Observable Errors
        For j = 1 To Nobs: Input #lu%, OBrms(i, j): Next j
      ' Average observable values for soundings used
        For j = 1 To Nobs: Input #lu%, OBav(iRC, i, j): Next j
      ' For each retrieval set (iRC), and
      ' For each flight altitude (i = 1 to NFL), and
      ' For each retrieval level (k = 1 to Nret), read:
      ' bp(iRC, i, k)     Pressure at retrieval level, use to calculate pressure altitude (RAav(iRC, i, k)
      ' RTav(iRC, i, k)   Average temperature at retrieval level
      ' RMSa(iRC, i, k)   Temperature variance at retrieval level
      ' RMSe(iRC, i, k)   Expected retrieval accuracy at retrieval level
      ' rc(iRC, i, k, L)  RC values for this retrieval level (L= 1 to Nobs)
        For k = 1 To Nret
          Input #lu%, BP(iRC, i, k), RTav(iRC, i, k), RMSa(iRC, i, k), RMSe(iRC, i, k)
          RAav(iRC, i, k) = fPtoZ(BP(iRC, i, k))
          For L = 1 To Nobs: Input #lu%, rc(iRC, i, k, L): Next L
        Next k
        Close #lu%
      Next i
    End If
  
  Else
    RCread iRC, RCpathToDot$ + ".RCF"
    
'    RCwrite iRC, RCpathToDot$ + ".RCF2"
  End If
    
End Sub

Sub ReadMissionWCT()
Dim Filename$, A$, lu%, i%, j%, Elev!

  Filename$ = MNpath$ + "Setup\" + Mission$ + "_WCT.TXT"
  lu = FreeFile
  Open Filename$ For Input As lu
  
  Line Input #lu, A$
  For j = 1 To Nel
    Select Case Channels
    Case 2
      Input #lu, Elev, WINcor(1, j), WINcor(2, j)
    Case 3
      Input #lu, Elev, WINcor(1, j), WINcor(2, j), WINcor(3, j)
    End Select
  Next j
  Close lu
  
End Sub

Sub TSwrite(REFlu%, GoodScansOnly As Boolean, WriteBadHeaders As Boolean, f As Form)
Dim Ext$, MPheader$, V!(1 To 4), LocalOpen As Boolean
Dim Filename$, MPfile$, TSfile$, MPlu%, TSlu%, HDRfile$, L%, x!, A$, dZp!, dZg!, Discard As Boolean, dALT!

  If CallingForm.chkUTseconds.Value = 0 Then
    MPheader$ = " ##0.000 ##0.0 ##0.0 #0.00"
  Else
    MPheader$ = " ##0000 ##0.0"
  End If
  If f.chkDiscard.Value = 1 Then Discard = True Else Discard = False
  dALT = Val(f.txtDiscard.Text)
     
  Ext$ = fACext(AC$)
  TSfile$ = "TS" + YYYYMMDD$ + Ext$
  MPfile$ = "MP" + YYYYMMDD$ + Ext$
' Will read & modify header after first assigning MP output file
  TSlu% = FreeFile
  Open Drive$ + Rdir2$ + Mission$ + "\TS\" + TSfile$ For Output As #TSlu%
  
  HDRfile$ = Drive$ + Rdir2$ + Mission$ + "\" + Mission$ + "_TS.HDR"
  Call UpdateTSheader(HDRfile$, TSlu%)
  
  MPlu% = FreeFile
  Open MPpath$ + MPfile$ For Input As #MPlu%
  
' Write Limits Record
  Record% = 0
  'Call MPread(MPlu, 0) 'skip header record
  Do
    Record% = Record% + 1
    If Record > f.ProgressBar1.Max Then Exit Do
    f.txtRecord.Text = Str(Record)
    f.ProgressBar1.Value = Record
    Call MPread(MPlu%, Record%)
    If Not EOF(MPlu) Then
      If Nlev > 0 Then GoSub WriteCycle
    End If
    DoEvents
  Loop Until EOF(MPlu%)
  
  Close TSlu%, MPlu
'  If LocalOpen Then Close (REFlu%)
  
  Exit Sub

WriteCycle:
' NB only come here on GoodScan = True  (get rid of checks for goodscan)
'  If EditNlev And (Nlev < NlevMin Or Nlev > NlevMax) Then
'    EditWord = EditWord Or 128: GoodScan = False ' Determine total # of acceptable levels; throw out if <7 levels
'    NlevPC = NlevPC + 1
'  End If
'  If Nlev1 = 0 Then Nlev1 = 1
'  If pALT < zzz(Nlev1) / 100 Then EditWord = EditWord Or 1: RetAltPC = RetAltPC + 1: GoodScan = False
  
' Update the number of valid levels
  If CallingForm.chkUTseconds.Value = 1 Then
    V(1) = UTsec: V(2) = OATmtp: V(3) = 0#
  Else
    V(1) = UTsec / 1000#: V(2) = OATmtp: V(3) = OATnav: V(4) = OATmtp - OATnav
  End If
'  If GoodScan Or WriteBadHeaders Or (Discard And pALT < dALT) Then
  Print #TSlu%, fUsing$(MPheader$, V!())
  
Return
End Sub


Sub UpdateTSheader(HDRfile$, MPlu%)
Dim lu%, i%, j%, L%, n%, A$, b$, Nhdr%, yr$, dy$, mo$, fln&, fltnr&, FltDate&
Dim x$, lux%
Dim FlightNo&, Objective$, FlightDate$

  lu% = FreeFile
  Open HDRfile$ For Input As lu%
  Input #lu%, Nhdr   'Get number of header lines
  Close (lu%)

  lu% = FreeFile
  Open HDRfile$ For Input As lu%   'Start over
  
  For j = 1 To Nhdr                'Update dates in MP file header
    Line Input #lu%, A$
    Select Case j
    Case 7
      Mid$(A$, 1, 4) = Mid$(YYYYMMDD$, 1, 4)  'set YR
      Mid$(A$, 6, 2) = Mid$(YYYYMMDD$, 5, 2)  'set MO
      Mid$(A$, 9, 2) = Mid$(YYYYMMDD$, 7, 2)  'set DY
      b$ = Date$: L = Len(b$)
      yr$ = Right$(b$, 4): dy$ = Mid$(b$, L - 6, 2): mo$ = Mid$(b$, L - 9, 2)
      Mid$(A$, 13) = yr$ + " " + mo$ + " " + dy$
      If Mission$ = "SONEX" Then
        lux% = FreeFile
        Open "\DC8\SONEX\FLTNR.PRN" For Input As lux%
        Do
          Input #lux%, fln, FltDate
          If FltDate = Val(Right$(YYYYMMDD$, 6)) Then FlightNo = fln
        Loop Until EOF(lux%)
      Else
        lux% = FreeFile
        Open Drive$ + Rdir2$ + Mission$ + "\" + Mission$ + ".NUM" For Input As lux%
        i = -1
        Do: Line Input #lux, b$  'FlightNo, FlightDate, Objective$
          b$ = LTrim(b$)
          n = InStr(1, b$, " ")
          FlightNo = Val(Left$(b$, n - 1))
          b$ = Trim(Right$(b$, Len(b$) - n + 1))
          n = InStr(1, b$, " ")
          If n = 0 Then
            FlightDate = Val(b$)
            Objective$ = "Unknown"
          Else
            FlightDate = Val(Left$(b$, n - 1))
            Objective$ = Trim(Mid$(b$, n, Len(A$) - n + 1))
          End If
          i = i + 1
          If YYYYMMDD$ = Trim(Str(FlightDate)) Then Exit Do
        Loop Until EOF(lux)
      End If
      Close #lux%
      Mid$(A$, 25) = Format$(FlightNo, "00000000")
    Case Else
    End Select

    Print #MPlu%, A$
  Next j
  Close (lu%)


End Sub

Sub WriteCAL(Filename$)
Dim i0%, i1%, i2%, lu%, i%, j%, x$, MyDate

lu% = FreeFile
Open Filename$ For Output As lu%
FIsize% = 0
'AC$ = Mid$(Rdir$, 2, 2)
'Path$ = Drive$ + Rdir2$ + Mission$ + "\" + yyyymmdd$ + "\"

Print #lu, "1                             ' Cal File Format"
Call PrintStr(lu%, "' " + Filename$ + " was last written on " + Date$ + " at " + Time$, "", "")
Call PrintStr(lu%, "' This CAL-file follows a few simple but mandatory format requirements.", "", "")
Call PrintStr(lu%, "' Settings are entered into categories which occupy a single line and", "", "")
Call PrintStr(lu%, "' must be bracketed by square brackets (e.g. [PATH]). Variables for which", "", "")
Call PrintStr(lu%, "' settings are assigned must have exactly the same name in the program in", "", "")
Call PrintStr(lu%, "' which they are used and be followed by an equal sign (=). Spaces are", "", "")
Call PrintStr(lu%, "' not allowed. Anything beyond the first space in a line is ignored.", "", "")
Call PrintStr(lu%, "' Blank lines are also ignored, but are required betweeen categories.", "", "")
Call PrintStr(lu%, "", "", "")


Gendate$ = fTstamp$  'Right$(Date$, 4) + Left$(Date$, 2) + Mid$(Date$, 4, 2)
Call PrintStr(lu%, "[GENERAL]", "", "")
Call PrintStr(lu%, "GenDate", Gendate$, "Analysis Date")
Call PrintStr(lu%, "yyyymmdd$", YYYYMMDD$, "Flight Date")
Call PrintVal(lu%, "UTstart", UTstart, "Start UT")
Call PrintVal(lu%, "UTend", UTend, "End UT")
Call PrintVal(lu%, "Channels", Channels, "Number of frequency channels")
Call PrintVal(lu%, "Nel", Nel, "Number of elevation angles")
Call PrintVal(lu%, "LocHor", LocHor, "Scan Position when viewing the Horizon")
Call PrintBol(lu%, "RHS", RHS, "True if MTP is on Right Hand Side of Aircraft")
Call PrintVal(lu%, "Emissivity", Emissivity, "Emissivity of window")
Call PrintVal(lu%, "Reflectivity", Reflectivity, "Reflectivity of window")
Call PrintVal(lu%, "DeltaTmin", DeltaTmin, "Minimum OAT-TGT Temperature Difference")
Call PrintVal(lu%, "Pitchmin", ePitchMin, "Minimum allowable Pitch")
Call PrintVal(lu%, "Pitchmax", ePitchMax, "Maximum allowable Pitch")
Call PrintVal(lu%, "RollLimit", eRollMin, "Maximum allowable Roll (+ or -)")
Call PrintVal(lu%, "Nsamples", TotalCycles, "Total Number of Samples")
Call PrintStr(lu%, "", "", "")

Call PrintStr(lu%, "[FIT_INFO]", "", "")
Call PrintVal(lu%, "Nfit", Nfit, "Number of Fit coefficients")
Call PrintVal(lu%, "Offset", GOF(1), "Fit Offset")
For i = 1 To Nfit
  Call PrintStr(lu%, "FitVar" + Format(i, "0"), NP$(i), "Fit Variable" + Format(i, "0"))
  For j = 1 To Channels
    x$ = Format(j, "0") + Format(i, "0")
    Call PrintVal(lu%, "GEC" + x$, GEC(j, i), "Fit coefficient" + x$)
  Next j
  If i < Nfit Then Call PrintVal(lu%, "GOF" + Format(i, "0"), GOF(i + 1), "Fit Parameter Offset")
Next i
Call PrintVal(lu%, "Target", TGToffset, "Target temperature offset in cycles")
Call PrintVal(lu%, "Mixer", MXRoffset, "Mixer temperature offset in cycles")
Call PrintVal(lu%, "Nav", NAVoffset, "Nav temperature offset in cycles")
Call PrintVal(lu%, "Noise", NDoffset, "Noise Diode temperature offset in cycles")
Call PrintStr(lu%, "", "", "")

Call PrintStr(lu%, "[ND_INFO]", "", "")
Call PrintVal(lu%, "TrefND", TrefND, "Noise Diode reference temperature")
For i = 1 To Channels
  Call PrintVal(lu%, "Cnd0" + Format(i, "0"), Cnd0(i), "Noise Diode fit parameter" + Format(i, "0") + "0")
  Call PrintVal(lu%, "Cnd1" + Format(i, "0"), Cnd1(i), "Noise Diode fit parameter" + Format(i, "0") + "1")
  Call PrintVal(lu%, "Cnd2" + Format(i, "0"), Cnd2(i), "Noise Diode fit parameter" + Format(i, "0") + "2")
Next i
Call PrintStr(lu%, "", "", "")

Call PrintStr(lu%, "[GAIN_INFO]", "", "")
Call PrintVal(lu%, "GainScale", GainScale, "Gain scale used (1=GE, 2=ND, 3=OAT)")
Call PrintVal(lu%, "OATsource", OATsource, "OAT source used (1=MTP, 2=A/C, 3=MMS)")
Call PrintVal(lu%, "REFsource", REFsource, "Reference Temperature (1=Target, 2=OAT)")
Call PrintVal(lu%, "OATnavCOR", OATnavCOR, "OAT temperature correction (added)")
Call PrintStr(lu%, "RAWextension", RAWextension$, "Extension of RAW data file")
Call PrintStr(lu%, "", "", "")

Call PrintStr(lu%, "[MA_INFO]", "", "")  'Moving Average Info
Call PrintBol(lu%, "UseMAforCB", UseMAforCB, "Use Moving Average for Base Counts")
Call PrintBol(lu%, "UseMAforCS", UseMAforCS, "Use Moving Average for Sky Counts")
Call PrintBol(lu%, "UseMAforCN", UseMAforCN, "Use Moving Average for Noise Diode Counts")
Call PrintBol(lu%, "UseMAforTtgt", UseMAforTtgt, "Use Moving Average for Target Temperature")
Call PrintBol(lu%, "UseMAforTifa", UseMAforTifa, "Use Moving Average for IF Amp Temperature")
Call PrintVal(lu%, "CMAcycles", CMAcycles, "Slow Cycle Moving Average count")
Call PrintVal(lu%, "CMAcycles2", CMAcycles2, "Fast Cycle Moving Average count")
Call PrintStr(lu%, "", "", "")


Call PrintStr(lu%, "[CHANNEL_INFO]", "", "")
For i = 1 To Channels
  Call PrintVal(lu%, "ChInfo" + Format(i, "0"), ChInfo(i), "Channel " + Format(i, "0") + " Information Content")
Next i
Call PrintStr(lu%, "", "", "")

Call PrintStr(lu%, "[WINDOW_CORRECTIONS]", "", "")
For i = 1 To Channels
  For j = 1 To Nel
    x$ = "WCT" + Format(i, "0") + Format(j, "00")
    Call PrintVal(lu%, x$, WINcor(i, j), "Window Correction Table Entry, " + x$)
  Next j
Next i
Call PrintStr(lu%, " ", "", "")

Close (lu%)
End Sub










Sub BINwrite32(lu%, Record%)
Dim A As REFrecord, i%, j%, x%
  
  A.Cycle = Record
  A.GoodScan = GoodScan
  A.MakeWord = MakeWord
  A.UTsec = UTsec
  A.UTsecMTP = UTsecMTP
  A.UTsecNav = UTsecNav
  A.TTO = TTO
  A.pALT = pALT
  A.gALT = gALT           'gALT
  A.rALT = 99.9           'rALT
  A.mALT = mALT
  
  A.Pitch = Pitch
  A.Roll = Roll
  A.Latitude = Latitude
  A.Longitude = Longitude
  A.Heading = Heading
  A.TAS = 999.9        'TAS
  A.Wspd = Wspd
  A.Wdir = Wdir
  A.Elcor = Elcor
  A.ElCorUsed = ElCorUsed
  
  A.OATnav = OATnav
  A.OATmms = OATmms
  A.OATmtp = OATmtp
  
  A.TTMA = TTMA
  A.TMMA = TMMA
  A.TWMA = TWMA
  For i% = 1 To Channels
    A.RFImask(i) = RFImask(i)
'    For j% = 1 To 10: a.TA(i%, j%) = TA(i%, j%):  Next j%
    For j% = 1 To Ncts
      A.Counts(i%, j%) = C(i%, j%)
      A.CMA(i, j) = CMA(i, j)
    Next j%
  Next i%
  
  A.Muxs(0) = Muxs(0)
  For i% = 1 To 16
    A.Muxs(i%) = Muxs(i%)
    A.Mux(i%) = Mux(i%)
  Next i%
  
  Put #lu%, Record% + HiddenRecords, A

End Sub
Sub BINwrite33(ByVal lu%, ByVal Record%)
Dim A As REF2record, i%, j%, x%
  
  A.Cycle = Record
  A.GoodScan = GoodScan
  A.MakeWord = MakeWord
  A.UTsec = UTsec
  A.UTsecMTP = UTsecMTP
  A.UTsecNav = UTsecNav
  A.TTO = TTO
  A.pALT = pALT
  A.gALT = gALT           'gALT
  A.rALT = 99.9           'rALT
  A.mALT = mALT
  
  A.Pitch = Pitch
  A.Roll = Roll
  A.Latitude = Latitude
  A.Longitude = Longitude
  A.Heading = Heading
  A.TAS = 999.9        'TAS
  A.Wspd = Wspd
  A.Wdir = Wdir
  A.Elcor = Elcor
  A.ElCorUsed = ElCorUsed
  
  A.OATnav = OATnav
  A.OATmms = OATmms
  A.OATmtp = OATmtp
  
  A.TTMA = TTMA
  A.TMMA = TMMA
  A.TWMA = TWMA
  
  For i% = 1 To Channels
    A.RFImask(i) = RFImask(i)
'    For j% = 1 To 10: a.TA(i%, j%) = TA(i%, j%):  Next j%
    For j% = 1 To Ncts
      A.Counts(i%, j%) = C(i%, j%)
      A.CMA(i, j) = CMA(i, j)
    Next j%
  Next i%
  A.Muxs(0) = Muxs(0)
  
  For i% = 1 To 32
    A.Muxs(i%) = Muxs(i%)
    A.Mux(i%) = Mux(i%)
  Next i%
  
  Put #2, Record% + HiddenRecords, A

End Sub
Sub BINwrite(lu%, Record%)
  
  Select Case FileFormatOut
  Case 32: Call BINwrite32(lu%, Record%)
  Case 33: Call BINwrite33(lu%, Record%)
  End Select

End Sub



Function fRAWscans(RAWfile$)
Dim lu%, RecordBytes&, ByteCount&, LOFraw%, A$, Count%
' Return number of RAW file cycles. Should be
' greater than actual number by one or two because
' of 99:99 cycles and incomplete cycles

  lu = FreeFile
  Open RAWfile$ For Input As lu

  Count = 0
  Do
    Do  'Go to next A-line
      Line Input #lu, A$
    Loop Until Left$(A$, 1) = "A" Or EOF(lu)
    Count = Count + 1
  Loop Until EOF(lu)
  Close lu
  
  fRAWscans = Count - 1
  
End Function

Sub GetABLEvalues(UTsec&, UT&, mZg!, mZp!, PTWfile$, Quit)
Dim i%, A$, HeaderCount%, Tf!, Ts!
Static PTWlu%

  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open PTWfile$ For Input As PTWlu
    Input #PTWlu, A$  'HeaderCount, a$  'Skip Header Line
    'For i = 2 To HeaderCount: Line Input #PTWlu, a$: Next i
  Else
    If EOF(PTWlu) Then
      Close PTWlu
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Line Input #PTWlu, A$
    UT = Val(Left$(A$, 5))
  Loop Until (UT >= UTsec) Or EOF(PTWlu)

  If EOF(PTWlu) Then UT = -1: Exit Sub
'time     gps     baro
'22949    442      583
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'    Line Input #PTWlu, a$
    
      mZg = Val(Mid$(A$, 7, 6))
      mZp = Val(Mid$(A$, 16, 6))
'Debug.Print UT; P; fPtoZ(P)
End Sub

Sub GetICATSvalues(UTsec&, UT&, P!, T!, GA!, MMfile$, Quit As Boolean)
Dim i%, A$, HeaderCount%
Static MMlu%

  If MMlu = 0 Then
    MMlu = FreeFile
    Open MMfile$ For Input As MMlu
    Input #MMlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #MMlu, A$: Next i
  Else
    If EOF(MMlu) Then
      Close MMlu
      MMfile$ = ""
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Do
      Line Input #MMlu, A$
    Loop Until Left$(A$, 1) = "C"
    UT = fTstringToSec(Mid$(A$, 7, 8), True)
  Loop Until UT >= UTsec Or EOF(MMlu)
    
  If EOF(MMlu) Then
    Close MMlu
    MMlu = 0
    Quit = True
    Exit Sub
  End If

'12345678901234567890
'C 227 15:29:59.000 +34 55.6 -117 53.2  -1.4    0.1   0
'D   0   0    0  57.1   0.0  2209     0  2000.0  3000.0
'E  31.2  31.2  32.6  33.4   0    -16    19.7    0.0
'F    47  934.9 0.000     0.7  208.4   16.6 225.0 4.126
'G  6.2 10.0 13.4 46.03 62.10  28.2  29.4  91.3   34.5
'H  58.9  44.9    -0    -0     0    -0    -0    32
'I  31.2  33.4  312.5  2299 -142.4  28.1  29.3  91.3
'C 227 15:30:00.000 +34 55.6 -117 53.2  -1.4    0.1   0
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
  Line Input #MMlu, A$ 'D
  P = fZtoP(Val(Mid$(A$, 27, 6)) / (1000 * kft_km))  'P (mb)
  Line Input #MMlu, A$ 'E
  T = Mid$(A$, 2, 6)                                 'T(C)
  Line Input #MMlu, A$ 'F
  Line Input #MMlu, A$ 'G
  Line Input #MMlu, A$ 'H
  Line Input #MMlu, A$ 'I
  GA = Mid$(A$, 21, 6)                               'gALT

  OATmms = T + cTo
End Sub

Sub GetNCvalues(UTsec&, UT&, P!, T!, NCfile$, Quit As Boolean)
Dim i%, A$, HeaderCount%
Static MMlu%

  If MMlu = 0 Then
    MMlu = FreeFile
    Open NCfile$ For Input As MMlu
    For i = 1 To 3: Line Input #MMlu, A$: Next i
  Else
    If EOF(MMlu) Then
      Close MMlu
      NCfile$ = ""
      Exit Sub
    End If
  End If
'timewave , ambient_temperature, sTatic_pressure
'date_time,deg C,deg C
'8/11/00 18:19:02,14.18,676.68
'8/11/00 18:19:03,14.23,676.67
'12345678901234567890123456789
  Do  'Catch up
    Line Input #MMlu, A$
    UT = fTstringToSec&(Mid$(A$, 9, 8), True)
  Loop Until UT >= UTsec Or EOF(MMlu)
  
  If EOF(MMlu) Then
    Close MMlu
    MMlu = 0
    Quit = True
    Exit Sub
  End If

  i = InStr(19, A$, ",")
  

  P = Val(Mid$(A$, i + 1, Len(A$) - i))
  T = Val(Mid$(A$, 18, i - 18 + 1)) + cTo
  OATmms = T + cTo
End Sub

Sub GetNextXS(Mode%, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$, Quit As Boolean)
Dim i%, A$, HeaderCount%, V!, Txs!, nMax%
Static PTWlu%, ZZp!(1 To 50), x$, ii%, jj%, j%, nVar%, Vscale!(1 To 10), PV1&
Static PVs!, Ts!, zgs!, Zps!, Wzs!, Wms!
'32 2010
'Atlas , Lamich, Strahan, Lucchesi, Steenrod, Ledvina, Conaty, Pawson, Newman, Lait
'NASA Goddard Space Flight Center Codes 910.3 and 916
'GSFC Assimilation GG1X1 grid data interpolated to the DC8 flight curtain
'SOLVE
'1 1
'2000 03 08 2000 07 26
'0.00000 0.00000
'17
'17   'line10
'1000 850 700 500 400 300 250 200 150 100 70 50 30 10 5 2 1
'Pressure Levels(mb)
'seconds from 0 GMT on flight date
'5
'1.00000E-07 0.100000 1.00000 0.0100000 0.100000  line15
'999999 9999 99999 9999 999                       line16
'Ertel 's Potential Vorticity (K m^2/kg s)
'Temperature (k)
'Geopotential Height(M)
'Zonal wind(M / s)
'Meridional wind(M / s)
'2
'0.0100000 0.100000
'9999 999
'Longitude (degrees East)
'Latitude (degrees North)
'0
'4
'Aircraft position is taken from the DFexchange file
'Assimilation analyses are interpolated to the flight curtain
'linearly in latitude and longitude.
'Data are valid for 12 UTC on 8 March, 2000.


' Mode =1 Get a single profile and close file
' Mode =0 Get next profile
nMax = 50  'Max levels that program can handle
  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open XSfile$ For Input As PTWlu
    Input #PTWlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount
      Line Input #PTWlu, A$
      Select Case i
      Case 10
        nMax = Val(A$)
      Case 11
        jj = 1
        For j = 1 To nMax
          ii = InStr(jj, A$, " ")
          If ii = 0 Then ii = Len(A$) + 1
          ZZp(j) = fPtoZ(Val(Mid$(A$, jj, ii - jj)))
          jj = ii + 1
        Next j
      Case 14
        nVar = Val(A$)
      Case 15
        jj = 1
        For j = 1 To nVar
          ii = InStr(jj, A$, " ")
          If ii = 0 Then ii = Len(A$) + 1
          Vscale(j) = Val(Mid$(A$, jj, ii - jj))
          jj = ii + 1
        Next j
      
      Case 16
        ii = InStr(1, A$, " ")
        PV1 = Val(Left$(A$, ii - 1))
      Case Else
      End Select
    Next i
'    ZZp(1) = fPtoZ(1000): ZZp(2) = fPtoZ(850): ZZp(3) = fPtoZ(700): ZZp(4) = fPtoZ(500)
'    ZZp(5) = fPtoZ(400): ZZp(6) = fPtoZ(300): ZZp(7) = fPtoZ(250): ZZp(8) = fPtoZ(200)
'    ZZp(9) = fPtoZ(150): ZZp(10) = fPtoZ(100): ZZp(11) = fPtoZ(70): ZZp(12) = fPtoZ(50)
'    ZZp(13) = fPtoZ(30): ZZp(14) = fPtoZ(10): ZZp(15) = fPtoZ(5): ZZp(16) = fPtoZ(2)
'    ZZp!(17) = fPtoZ(1):
    x$ = " "
  Else
    If EOF(PTWlu) Then GoTo CloseCode
  End If

    If EOF(PTWlu) Then GoTo CloseCode
    Line Input #PTWlu, A$
    i = InStr(1, A$, " ")
    UT = Val(Left$(A$, i - 1))
'UTsec Lon Lat
'55110 603 758
' -4 38 19 39 42 125 375 681 912 1703
'2949 4920 12817 50960 103319 291838 968698
' 2681 2604 2488 2333 2232 2099 2055 2058 2048 2022
'2002 2002 2045 2211 2380 2504 2488
' 133 1386 2836 5208 6699 8520 9623 10963 12695 15107
'17202 19171 22174 28986 33630 40181 45343
' -45 -37 -36 -40 -17 16 27 33 25 -4
'-19 -25 -35 -13 70 17 141
' -24 -53 -63 27 66 94 61 -18 -76 -120
'-158 -204 -267 -263 -172 -119 34
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'    Line Input #PTWlu, a$
    A$ = Right$(A$, Len(A$) - 6)
    
    Longitude = Val(Left$(A$, InStr(1, A$, " ", 1) - 1)) / 100#
    Latitude = Val(Right$(A$, Len(A$) - InStr(1, A$, " ", 1) + 1)) / 10#
    For i = 1 To nMax: Input #PTWlu, PV(i): Next i
    For i = 1 To nMax: Input #PTWlu, T(i): Next i
    For i = 1 To nMax: Input #PTWlu, Zg(i): Next i
    For i = 1 To nMax: Input #PTWlu, Wz(i): Next i
    For i = 1 To nMax: Input #PTWlu, Wm(i): Next i
    For i = 1 To nMax: Zp(i) = ZZp(i): Next i
    
  If T(1) = 9999 Then
    nMax = nMax - 1
    For i = 1 To nMax
      PV(i) = PV(i + 1) * Vscale(1)
      T(i) = T(i + 1) * Vscale(2)
      Zg(i) = Zg(i + 1) * Vscale(3)
      Wz(i) = Wz(i + 1) * Vscale(4)
      Wm(i) = Wm(i + 1) * Vscale(5)
      Zp(i) = Zp(i + 1)
'Debug.Print i; PV(i); T(i); Zg(i); Zp(i); Wz(i); Wm(i)
    Next i
  
  Else
    For i = 1 To nMax
      PV(i) = PV(i) * Vscale(1)
      T(i) = T(i) * Vscale(2)
      Zg(i) = Zg(i) * Vscale(3)
      Wz(i) = Wz(i) * Vscale(4)
      Wm(i) = Wm(i) * Vscale(5)
'Debug.Print i; PV(i); T(i); Zg(i); Zp(i); Wz(i); Wm(i)
    Next i
  End If
    Levels = nMax

If Mode = 1 Then Close PTWlu: PTWlu = 0
Exit Sub

CloseCode:
      Close PTWlu
      PTWlu = 0
      Quit = True
End Sub

Sub GetNextXS1(Mode%, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$, Quit As Boolean)
Static PTWlu%, ZZp!(1 To 50), x$, ii%, jj%, j%, nVar%, Vscale!(1 To 10), PV1&
Static PVs!, Ts!, zgs!, Zps!, Wzs!, Wms!
Dim Org$, Sname$, Mname$, Ivol%, Nvol%, dX(1 To 2), ReduxDate$, n%
Dim Xname$(1 To 2), NSCOML%, NNCOML%
Static VSCAL!(1 To 100), Vmiss!(1 To 100), Vname$(1 To 100)
Static ASCAL!(1 To 100), Amiss!(1 To 100), Aname$(1 To 100)
' If Record=0, read header, otherwise read data
' Mode = 0, don't close file, Mode = 1, close file, Mod = 3, Close file
Dim A$, i%, x2!, x1!(1 To 100), NXm1%, V!(1 To 100, 1 To 100), AuxV!(1 To 100), num!(1 To 100)
Static nHeader%, FFI%, NV%, NauxV%, NX%(1 To 100), NXDEF%(1 To 100)

  If Mode = 3 And PTWlu > 0 Then Close (PTWlu): PTWlu = 0: Exit Sub

  If PTWlu = 0 And Not (Mode = 3) Then
    PTWlu = FreeFile
    Open XSfile$ For Input As PTWlu
    Input #PTWlu, nHeader%, FFI          '1  Number of header records and File Format
    Line Input #PTWlu, Pi$               '2  PI
    Line Input #PTWlu, Org$              '3  Organization
    Line Input #PTWlu, Sname$            '4  Instrument
    Line Input #PTWlu, Mname$            '5  Mission
    'Mission$ = Mname$
    If Mission$ = "TOTE/VOTE" Then Mission$ = "TOTE_VOTE"
    Call Read_N_Numbers(PTWlu, 2, num())
    Ivol = num(1)
    Nvol = num(2)                     '6
    Line Input #PTWlu, A$                '7  Flight date and Reduction Date
'   1995 12 11  1996 06 26  {FLT DATE & REDUCTION DATE}
'   12345678901234567890123
    YYYYMMDD$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
    ReduxDate$ = Mid$(A$, 13, 4) + Mid$(A$, 18, 2) + Mid$(A$, 21, 2)
    Call Read_N_Numbers(PTWlu, 2, num())
    dX(1) = num(1)
    dX(2) = num(2)                    '8  Interval between primary variables, 0.0 is non-uniform
    Call Read_N_Numbers(PTWlu, 1, num())
    NX(1) = num(1)
    Call Read_N_Numbers(PTWlu, 1, num())
    NXDEF(1) = num(1)
    For i = 1 To NXDEF(1): Input #PTWlu, x1(i): Next i
    For i = 1 To NXDEF(1): ZZp(i) = fPtoZ(x1(i)): Next i  'convert p to Zp
    Line Input #PTWlu, Xname$(1)         '9  Mission
    Line Input #PTWlu, Xname$(2)         '10 Mission
    Call Read_N_Numbers(PTWlu, 1, num())
    NV = num(1)                       '11 Number of Primary variables
    Call Read_N_Numbers(PTWlu, NV, num())
    For n = 1 To NV: VSCAL(n) = num(n): Next n
    Call Read_N_Numbers(PTWlu, NV, num())
    For n = 1 To NV: Vmiss(n) = num(n): Next n
    For n = 1 To NV: Line Input #PTWlu, Vname(n):  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NauxV = num(1)                    ' Number of Auxiliary Variables
    Call Read_N_Numbers(PTWlu, NauxV, num())
    For n = 1 To NauxV: ASCAL(n) = num(n): Next n
    Call Read_N_Numbers(PTWlu, NauxV, num())
    For n = 1 To NauxV: Amiss(n) = num(n): Next n
    For n = 1 To NauxV: Line Input #PTWlu, Aname(n):  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NSCOML = num(1)                   ' Number of special comment lines
    For n = 1 To NSCOML: Line Input #PTWlu, A$:  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NNCOML = num(1)                   ' Number of normal comment lines
    For n = 1 To NNCOML: Line Input #PTWlu, A$:  Next n
    Record = 0
  Else
    If Mode = 3 Then Exit Sub
  End If
    If EOF(PTWlu) Then GoTo CloseCode
    Input #PTWlu, x2                                  ' Read Second Primary Variable
    For n = 1 To NauxV: Input #PTWlu, AuxV(n): Next n 'Read Auxiliary Variables
    For n = 1 To NV
      For i = 1 To NX(1): Input #PTWlu, V(i, n): Next i  'Read Primary Variables
    Next n                                'Read Secondary Variables
      UT = x2
      Latitude = AuxV(2) * ASCAL(2)
      Longitude = AuxV(1) * ASCAL(1)
      For n = 1 To NV
      For i = 1 To NX(1): PV(i) = V(i, 1) * VSCAL(1): Next i
      For i = 1 To NX(1): T(i) = V(i, 2) * VSCAL(2): Next i
      For i = 1 To NX(1): Zg(i) = V(i, 3) * VSCAL(3) / 1000#: Next i
      For i = 1 To NX(1): Zp(i) = ZZp(i): Next i
      ' Ignore winds for now
      Next n                                'Read Secondary Variables

  Levels = NX(1)
  For n = 1 To NX(1)
    If T(1) > 900 Then
      Levels = Levels - 1
      For i = 1 To Levels
        PV(i) = PV(i + 1)
        T(i) = T(i + 1)
        Zg(i) = Zg(i + 1)
        Wz(i) = Wz(i + 1)
        Wm(i) = Wm(i + 1)
        Zp(i) = Zp(i + 1)
'Debug.Print i; PV(i); T(i); Zg(i); Zp(i); Wz(i); Wm(i)
      Next i
    Else
      Exit For
    End If
  Next n
  
  If Mode = 1 Then Close PTWlu: PTWlu = 0
  Exit Sub

CloseCode:
      Close PTWlu
      PTWlu = 0
      Quit = True
End Sub

Sub GetNextXS2(Mode%, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$, Quit As Boolean)
Static PTWlu%, ZZp!(1 To 50), x$, ii%, jj%, j%, nVar%, Vscale!(1 To 10), PV1&
Static PVs!, Ts!, zgs!, Zps!, Wzs!, Wms!
Dim Org$, Sname$, Mname$, Ivol%, Nvol%, dX(1 To 2), ReduxDate$, n%
Dim Xname$(1 To 2), NSCOML%, NNCOML%
Static VSCAL!(1 To 100), Vmiss!(1 To 100), Vname$(1 To 100)
Static ASCAL!(1 To 100), Amiss!(1 To 100), Aname$(1 To 100)
' If Record=0, read header, otherwise read data
' Mode = 0, don't close file, Mode = 1, close file, Mod = 3, Close file
Dim A$, i%, x2!, x1!(1 To 100), NXm1%, V!(1 To 100, 1 To 100), AuxV!(1 To 100), num!(1 To 100)
Static nHeader%, FFI%, NV%, NauxV%, NX%(1 To 100), NXDEF%(1 To 100)
  
  If Mode = 3 And PTWlu > 0 Then Close (PTWlu): PTWlu = 0: Exit Sub

  If PTWlu = 0 And Not (Mode = 3) Then
    PTWlu = FreeFile
    Open XSfile$ For Input As PTWlu
    Input #PTWlu, nHeader%, FFI          '1  Number of header records and File Format
    Line Input #PTWlu, Pi$               '2  PI
    Line Input #PTWlu, Org$              '3  Organization
    Line Input #PTWlu, Sname$            '4  Instrument
    Line Input #PTWlu, Mname$            '5  Mission
    'Mission$ = Mname$
    If Mission$ = "TOTE/VOTE" Then Mission$ = "TOTE_VOTE"
    Call Read_N_Numbers(PTWlu, 2, num())
    Ivol = num(1)
    Nvol = num(2)                     '6
    Line Input #PTWlu, A$                '7  Flight date and Reduction Date
'   1995 12 11  1996 06 26  {FLT DATE & REDUCTION DATE}
'   12345678901234567890123
    YYYYMMDD$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
    ReduxDate$ = Mid$(A$, 13, 4) + Mid$(A$, 18, 2) + Mid$(A$, 21, 2)
    Call Read_N_Numbers(PTWlu, 2, num())
    dX(1) = num(1)
    dX(2) = num(2)                    '8  Interval between primary variables, 0.0 is non-uniform
    Call Read_N_Numbers(PTWlu, 1, num())
    NX(1) = num(1)
    Call Read_N_Numbers(PTWlu, 1, num())
    NXDEF(1) = num(1)
    For i = 1 To NXDEF(1): Input #PTWlu, x1(i): Next i
    For i = 1 To NXDEF(1): ZZp(i) = fPtoZ(x1(i)): Next i  'convert p to Zp
    Line Input #PTWlu, Xname$(1)         '9  Mission
    Line Input #PTWlu, Xname$(2)         '10 Mission
    Call Read_N_Numbers(PTWlu, 1, num())
    NV = num(1)                       '11 Number of Primary variables
    Call Read_N_Numbers(PTWlu, NV, num())
    For n = 1 To NV: VSCAL(n) = num(n): Next n
    Call Read_N_Numbers(PTWlu, NV, num())
    For n = 1 To NV: Vmiss(n) = num(n): Next n
    For n = 1 To NV: Line Input #PTWlu, Vname(n):  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NauxV = num(1)                    ' Number of Auxiliary Variables
    Call Read_N_Numbers(PTWlu, NauxV, num())
    For n = 1 To NauxV: ASCAL(n) = num(n): Next n
    Call Read_N_Numbers(PTWlu, NauxV, num())
    For n = 1 To NauxV: Amiss(n) = num(n): Next n
    For n = 1 To NauxV: Line Input #PTWlu, Aname(n):  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NSCOML = num(1)                   ' Number of special comment lines
    For n = 1 To NSCOML: Line Input #PTWlu, A$:  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NNCOML = num(1)                   ' Number of normal comment lines
    For n = 1 To NNCOML: Line Input #PTWlu, A$:  Next n
    Record = 0
  Else
    If Mode = 3 Then Exit Sub
  End If
    If EOF(PTWlu) Then GoTo CloseCode
    Input #PTWlu, x2                                  ' Read Second Primary Variable
    For n = 1 To NauxV: Input #PTWlu, AuxV(n): Next n 'Read Auxiliary Variables
    For n = 1 To NV
      For i = 1 To NX(1): Input #PTWlu, V(i, n): Next i  'Read Primary Variables
    Next n                                'Read Secondary Variables
      UT = x2
      Latitude = AuxV(2) * ASCAL(2)
      Longitude = AuxV(1) * ASCAL(1)
      For n = 1 To NV
      For i = 1 To NX(1): PV(i) = V(i, 1) * VSCAL(1): Next i
      For i = 1 To NX(1): T(i) = V(i, 2) * VSCAL(2): Next i
      For i = 1 To NX(1): Zg(i) = V(i, 3) * VSCAL(3) / 1000#: Next i
      For i = 1 To NX(1): Zp(i) = ZZp(i): Next i
      ' Ignore winds for now
      Next n                                'Read Secondary Variables

  Levels = NX(1)
  For n = 1 To NX(1)
    If T(1) > 900 Then
      Levels = Levels - 1
      For i = 1 To Levels
        PV(i) = PV(i + 1)
        T(i) = T(i + 1)
        Zg(i) = Zg(i + 1)
        Wz(i) = Wz(i + 1)
        Wm(i) = Wm(i + 1)
        Zp(i) = Zp(i + 1)
'       Debug.Print i; PV(i); T(i); Zg(i); Zp(i); Wz(i); Wm(i)
      Next i
    Else
      Exit For
    End If
  Next n
  
  If Mode = 1 Then Close PTWlu: PTWlu = 0
  Exit Sub

CloseCode:
      Close PTWlu
      PTWlu = 0
      Quit = True
End Sub
Sub GetNextXS3(Mode%, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$, Quit As Boolean)
Static PTWlu%, ZZp!(1 To 50), x$, ii%, jj%, j%, nVar%, Vscale!(1 To 10), PV1&
Static PVs!, Ts!, zgs!, Zps!, Wzs!, Wms!
Dim Org$, Sname$, Mname$, Ivol%, Nvol%, dX(1 To 2), ReduxDate$, n%
Dim Xname$(1 To 2), NSCOML%, NNCOML%
Static VSCAL!(1 To 100), Vmiss!(1 To 100), Vname$(1 To 100)
Static ASCAL!(1 To 100), Amiss!(1 To 100), Aname$(1 To 100)
' If Record=0, read header, otherwise read data
' Mode = 0, don't close file, Mode = 1, close file, Mod = 3, Close file
Dim A$, i%, x2!, x1!(1 To 100), NXm1%, V!(1 To 100, 1 To 100), AuxV!(1 To 100), num!(1 To 100)
Static nHeader%, FFI%, NV%, NauxV%, NX%(1 To 100), NXDEF%(1 To 100)
  
  If Mode = 3 And PTWlu > 0 Then Close (PTWlu): PTWlu = 0: Exit Sub

  If PTWlu = 0 And Not (Mode = 3) Then
    PTWlu = FreeFile
    Open XSfile$ For Input As PTWlu
    Input #PTWlu, nHeader%, FFI          '1  Number of header records and File Format
    Line Input #PTWlu, Pi$               '2  PI
    Line Input #PTWlu, Org$              '3  Organization
    Line Input #PTWlu, Sname$            '4  Instrument
    Line Input #PTWlu, Mname$            '5  Mission
    'Mission$ = Mname$
    If Mission$ = "TOTE/VOTE" Then Mission$ = "TOTE_VOTE"
    Call Read_N_Numbers(PTWlu, 2, num())
    Ivol = num(1)
    Nvol = num(2)                     '6
    Line Input #PTWlu, A$                '7  Flight date and Reduction Date
'   1995 12 11  1996 06 26  {FLT DATE & REDUCTION DATE}
'   12345678901234567890123
    YYYYMMDD$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
    ReduxDate$ = Mid$(A$, 13, 4) + Mid$(A$, 18, 2) + Mid$(A$, 21, 2)
    Call Read_N_Numbers(PTWlu, 2, num())
    dX(1) = num(1)
    dX(2) = num(2)                    '8  Interval between primary variables, 0.0 is non-uniform
    Call Read_N_Numbers(PTWlu, 1, num())
    NX(1) = num(1)
    Call Read_N_Numbers(PTWlu, 1, num())
    NXDEF(1) = num(1)
    For i = 1 To NXDEF(1): Input #PTWlu, x1(i): Next i
    For i = 1 To NXDEF(1): ZZp(i) = fPtoZ(x1(i)): Next i  'convert p to Zp
    Line Input #PTWlu, Xname$(1)         '9  Mission
    Line Input #PTWlu, Xname$(2)         '10 Mission
    Call Read_N_Numbers(PTWlu, 1, num())
    NV = num(1)                       '11 Number of Primary variables
    Call Read_N_Numbers(PTWlu, NV, num())
    For n = 1 To NV: VSCAL(n) = num(n): Next n
    Call Read_N_Numbers(PTWlu, NV, num())
    For n = 1 To NV: Vmiss(n) = num(n): Next n
    For n = 1 To NV: Line Input #PTWlu, Vname(n):  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NauxV = num(1)                    ' Number of Auxiliary Variables
    Call Read_N_Numbers(PTWlu, NauxV, num())
    For n = 1 To NauxV: ASCAL(n) = num(n): Next n
    Call Read_N_Numbers(PTWlu, NauxV, num())
    For n = 1 To NauxV: Amiss(n) = num(n): Next n
    For n = 1 To NauxV: Line Input #PTWlu, Aname(n):  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NSCOML = num(1)                   ' Number of special comment lines
    For n = 1 To NSCOML: Line Input #PTWlu, A$:  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NNCOML = num(1)                   ' Number of normal comment lines
    For n = 1 To NNCOML: Line Input #PTWlu, A$:  Next n
    Record = 0
  Else
    If Mode = 3 Then Exit Sub
  End If
    If EOF(PTWlu) Then GoTo CloseCode
    Input #PTWlu, x2                                  ' Read Second Primary Variable
    For n = 1 To NauxV: Input #PTWlu, AuxV(n): Next n 'Read Auxiliary Variables
    For n = 1 To NV
      For i = 1 To NX(1): Input #PTWlu, V(i, n): Next i  'Read Primary Variables
    Next n                                'Read Secondary Variables
      UT = x2
      Latitude = AuxV(2) * ASCAL(2)
      Longitude = AuxV(1) * ASCAL(1)
      For n = 1 To NV
      For i = 1 To NX(1): PV(i) = V(i, 1) * VSCAL(1): Next i
      For i = 1 To NX(1): T(i) = V(i, 2) * VSCAL(2): Next i
      For i = 1 To NX(1): Zg(i) = V(i, 3) * VSCAL(3) / 1000#: Next i
      For i = 1 To NX(1): Zp(i) = ZZp(i): Next i
      ' Ignore winds for now
      Next n                                'Read Secondary Variables

  Levels = NX(1)
  For n = 1 To NX(1)
    If T(1) > 900 Then
      Levels = Levels - 1
      For i = 1 To Levels
        PV(i) = PV(i + 1)
        T(i) = T(i + 1)
        Zg(i) = Zg(i + 1)
        Wz(i) = Wz(i + 1)
        Wm(i) = Wm(i + 1)
        Zp(i) = Zp(i + 1)
'       Debug.Print i; PV(i); T(i); Zg(i); Zp(i); Wz(i); Wm(i)
      Next i
    Else
      Exit For
    End If
  Next n
  
  If Mode = 1 Then Close PTWlu: PTWlu = 0
  Exit Sub

CloseCode:
      Close PTWlu
      PTWlu = 0
      Quit = True
End Sub
Sub GetNextXS4(Mode%, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$, Quit As Boolean)
Static PTWlu%, ZZp!(1 To 50), x$, ii%, jj%, j%, nVar%, Vscale!(1 To 10), PV1&
Static PVs!, Ts!, zgs!, Zps!, Wzs!, Wms!
Dim Org$, Sname$, Mname$, Ivol%, Nvol%, dX(1 To 2), ReduxDate$, n%
Dim Xname$(1 To 2), NSCOML%, NNCOML%
Static VSCAL!(1 To 100), Vmiss!(1 To 100), Vname$(1 To 100)
Static ASCAL!(1 To 100), Amiss!(1 To 100), Aname$(1 To 100)
' If Record=0, read header, otherwise read data
' Mode = 0, don't close file, Mode = 1, close file, Mod = 3, Close file
Dim A$, i%, x2!, x1!(1 To 100), NXm1%, V!(1 To 100, 1 To 100), AuxV!(1 To 100), num!(1 To 100)
Static nHeader%, FFI%, NV%, NauxV%, NX%(1 To 100), NXDEF%(1 To 100)
  
  If Mode = 3 And PTWlu > 0 Then Close (PTWlu): PTWlu = 0: Exit Sub

  If PTWlu = 0 And Not (Mode = 3) Then
    PTWlu = FreeFile
    Open XSfile$ For Input As PTWlu
    Input #PTWlu, nHeader%, FFI          '1  Number of header records and File Format
    Line Input #PTWlu, Pi$               '2  PI
    Line Input #PTWlu, Org$              '3  Organization
    Line Input #PTWlu, Sname$            '4  Instrument
    Line Input #PTWlu, Mname$            '5  Mission
    'Mission$ = Mname$
    If Mission$ = "TOTE/VOTE" Then Mission$ = "TOTE_VOTE"
    Call Read_N_Numbers(PTWlu, 2, num())
    Ivol = num(1)
    Nvol = num(2)                     '6
    Line Input #PTWlu, A$                '7  Flight date and Reduction Date
'   1995 12 11  1996 06 26  {FLT DATE & REDUCTION DATE}
'   12345678901234567890123
    YYYYMMDD$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
    ReduxDate$ = Mid$(A$, 13, 4) + Mid$(A$, 18, 2) + Mid$(A$, 21, 2)
    Call Read_N_Numbers(PTWlu, 2, num())
    dX(1) = num(1)
    dX(2) = num(2)                    '8  Interval between primary variables, 0.0 is non-uniform
    Call Read_N_Numbers(PTWlu, 1, num())
    NX(1) = num(1)
    Call Read_N_Numbers(PTWlu, 1, num())
    NXDEF(1) = num(1)
    For i = 1 To NXDEF(1): Input #PTWlu, x1(i): Next i
    For i = 1 To NXDEF(1): ZZp(i) = fPtoZ(x1(i)): Next i  'convert p to Zp
    Line Input #PTWlu, Xname$(1)         '9  Mission
    Line Input #PTWlu, Xname$(2)         '10 Mission
    Call Read_N_Numbers(PTWlu, 1, num())
    NV = num(1)                       '11 Number of Primary variables
    Call Read_N_Numbers(PTWlu, NV, num())
    For n = 1 To NV: VSCAL(n) = num(n): Next n
    Call Read_N_Numbers(PTWlu, NV, num())
    For n = 1 To NV: Vmiss(n) = num(n): Next n
    For n = 1 To NV: Line Input #PTWlu, Vname(n):  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NauxV = num(1)                    ' Number of Auxiliary Variables
    Call Read_N_Numbers(PTWlu, NauxV, num())
    For n = 1 To NauxV: ASCAL(n) = num(n): Next n
    Call Read_N_Numbers(PTWlu, NauxV, num())
    For n = 1 To NauxV: Amiss(n) = num(n): Next n
    For n = 1 To NauxV: Line Input #PTWlu, Aname(n):  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NSCOML = num(1)                   ' Number of special comment lines
    For n = 1 To NSCOML: Line Input #PTWlu, A$:  Next n
    Call Read_N_Numbers(PTWlu, 1, num())
    NNCOML = num(1)                   ' Number of normal comment lines
    For n = 1 To NNCOML: Line Input #PTWlu, A$:  Next n
    Record = 0
  Else
    If Mode = 3 Then Exit Sub
  End If
    If EOF(PTWlu) Then GoTo CloseCode
    Input #PTWlu, x2                                  ' Read Second Primary Variable
    For n = 1 To NauxV: Input #PTWlu, AuxV(n): Next n 'Read Auxiliary Variables
    For n = 1 To NV
      For i = 1 To NX(1): Input #PTWlu, V(i, n): Next i  'Read Primary Variables
    Next n                                'Read Secondary Variables
      UT = x2
      Latitude = AuxV(2) * ASCAL(2)
      Longitude = AuxV(1) * ASCAL(1)
      For n = 1 To NV
      For i = 1 To NX(1): PV(i) = V(i, 1) * VSCAL(1): Next i
      For i = 1 To NX(1): T(i) = V(i, 2) * VSCAL(2): Next i
      For i = 1 To NX(1): Zg(i) = V(i, 3) * VSCAL(3) / 1000#: Next i
      For i = 1 To NX(1): Zp(i) = ZZp(i): Next i
      ' Ignore winds for now
      Next n                                'Read Secondary Variables

  Levels = NX(1)
  For n = 1 To NX(1)
    If T(1) > 900 Then
      Levels = Levels - 1
      For i = 1 To Levels
        PV(i) = PV(i + 1)
        T(i) = T(i + 1)
        Zg(i) = Zg(i + 1)
        Wz(i) = Wz(i + 1)
        Wm(i) = Wm(i + 1)
        Zp(i) = Zp(i + 1)
'       Debug.Print i; PV(i); T(i); Zg(i); Zp(i); Wz(i); Wm(i)
      Next i
    Else
      Exit For
    End If
  Next n
  
  If Mode = 1 Then Close PTWlu: PTWlu = 0
  Exit Sub

CloseCode:
      Close PTWlu
      PTWlu = 0
      Quit = True
End Sub

Sub GetTDCvalues(UTsec&, UT&, P!, T!, Th!, U!, V!, W!, MMfile$, Quit As Boolean)
Dim i%, i2%, A$, HeaderCount%
Static MMlu%, Tcorrection%

If 1 = 1 Then 'EUPLEX and SCOUT-O3 final data
  If Quit Then
    If MMlu <> 0 Then Close MMlu
    MMlu = 0
    Quit = False
  End If

  If MMlu = 0 Then
    MMlu = FreeFile
    Open MMfile$ For Input As MMlu
    Input #MMlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount + 1: Line Input #MMlu, A$: Next i
  Else
    If EOF(MMlu) Then
      Close MMlu
      MMfile$ = ""
      Exit Sub
    End If
  End If
   
  Do  'Catch up                    'ER2 MMS 1940 sec slow on 20020728 and 1550 sec slow on 20020726
    Line Input #MMlu, A$
    UT = Int(Val(Left$(A$, 8)) + 0.5)
  Loop Until (UT >= UTsec) Or EOF(MMlu)
  
  If EOF(MMlu) Then
    Close MMlu
    MMlu = 0
    Quit = True
    Exit Sub
  End If
'   Time    Pst*      H*      Tst*    Vtrue*  U*      Ud*
'   22821   945.7     578   -3.06    13.1     3.9      30
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'   13450   986.1     228   245.2   29.50     9.3     281   303.7
  
  P = (Val(Mid$(A$, 10, 7)))
  T = Val(Mid$(A$, 33, 8)) + cTo            'Kelvin
  OATmms = T
'  Th = Val(Mid$(a$, 20, 5) / 10#)
'  U = Val(Mid$(a$, 25, 5) / 10#)
'  v = Val(Mid$(a$, 30, 5) / 10#)
'  W = Val(Mid$(a$, 35, 5) / 10#)
Else   'SCOUT-O3  tab delimited and no header, prelim data
  If MMlu = 0 Then
    MMlu = FreeFile
    Open MMfile$ For Input As MMlu
    Input #MMlu, HeaderCount, A$  'Skip Header Info
  Else
    If EOF(MMlu) Then
      Close MMlu
      MMfile$ = ""
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Line Input #MMlu, A$
    i = InStr(1, A$, vbTab)
    UT = Int(Val(Left$(A$, i - 1)) + 0.5)
  Loop Until (UT >= UTsec) Or EOF(MMlu)
  
  If EOF(MMlu) Then
    Close MMlu
    MMlu = 0
    Quit = True
    Exit Sub
  End If
'Time  Pst*  H*  Vtrue*  Tst*  Uwind*  W_dir*  Q*
'18000 1003.2  84  22.8  33.75 6.3 27  306.5
'18001 1003.2  84  22.9  33.75 6.4 27  306.5
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
  i2 = InStr(i + 1, A$, vbTab)
  P = (Val(Mid$(A$, i + 1, i2 - i - 1)))
  i = InStr(i2 + 1, A$, vbTab)
  i2 = InStr(i + 1, A$, vbTab)
  i = InStr(i2 + 1, A$, vbTab)
  T = Val(Mid$(A$, i2 + 1, i - i2 - 1)) + cTo   'Kelvin
  OATmms = T

End If

End Sub

Sub GetUCSEvalues(UTsec&, UT&, P!, T!, mPitch!, mRoll!, mLatitude!, mLongitude!, mHeading!, mZg!, mZp!, mWspd!, mWdir!, UCSEfile$, Quit As Boolean)
Dim i%, A$, HeaderCount%, Char As Byte
Static UCSElu%
  If UCSElu = 0 Then
    UCSElu = FreeFile
    Open UCSEfile$ For Input As UCSElu
      Input #UCSElu, HeaderCount, A$  'Skip Header Info
      For i = 2 To HeaderCount: Line Input #UCSElu, A$: Next i
'    Char = Input(1, UCSElu)
'    Debug.Print Hex$(Asc(Char))
  Else
    If EOF(UCSElu) Then
      Close UCSElu
      UCSEfile$ = ""
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Line Input #UCSElu, A$
    UT = Int(Val(Left$(A$, 6)) + 0.5)
  Loop Until (UT >= UTsec And Mid$(A$, 18, 4) <> "9999") Or EOF(UCSElu)
  
  If EOF(UCSElu) Then
    Close UCSElu
    UCSElu = 0
    Quit = True
    Exit Sub
  End If
'TIME    Tout Pstat  Vtrue  Halt  Dr_ang Head    Pitch Attac Sl_ang Uwind  W_dir   Roll    Lat       Long      Gr_sp Tr_ang Alt   SS1       SS2              SRK   SYS_TIME
'18660 34.98  1001.5 68.01  81    -0.88  288.74  3.08  6.07  15.05  6.05   288.75  0.59    -12.4188  130.8933  0.0   213    34    111111111 1110111111101111 11110 51632526
'18661 34.94  1001.5 65.13  81    -0.88  288.74  3.08  6.10  15.02  5.81   288.75  0.60    -12.4188  130.8933  0.0   192    34    111111111 1110111111101111 11110 51632527

' TIME    Tout Pstat  Vtrue  Halt  Dr_ang Head    Pitch Attac Sl_ang Uwind  W_dir   Roll    Lat       Long      Gr_sp Tr_ang Alt   SS1              SS2              SRK  SYS_TIME
' 28207 -12.75 947.37 49.37  563   0      57.94   3.05  0     0      13.71  58.33   0.45    67.8215   20.3273   0     177    441   1111111111111111 1011111111111111 110  1
' 36807 -63.16 74.83  699.22 18006 7.16   3.55    4.84  5.93  0.58   27.93  -52.89  1.03    75.3677   24.4817   659.4 12     16978 1111111111111111 111111111111111  110  8601
' 34085 -0.3   962.3  56.5   433   0      58.15   2.75  0     0      999    9999    0.65    67.82     20.33     999   999999 99999 9999999999999999 9999999999999999 9999 5
' 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'          1         2         3         4         5         6         7         8         9         10        11        12        13        14        15        16        17
  T = Val(Mid$(A$, 7, 6)) + cTo 'Kelvin
  P = Val(Mid$(A$, 14, 6))
  'Th = fTheta(T, P)
  mHeading = Val(Mid$(A$, 41, 7))
  mZp = Val(Mid$(A$, 28, 5))
  mZg = Val(Mid$(A$, 124, 5))
  mPitch = Val(Mid$(A$, 49, 5))
  mRoll = Val(Mid$(A$, 83, 7))
  mLatitude = Val(Mid$(A$, 91, 9))
  mLongitude = Val(Mid$(A$, 101, 9))
  mWspd = Val(Mid$(A$, 68, 6))
  If Val(Mid$(A$, 75, 6)) = 9999 Then mWdir = 999 Else mWdir = Val(Mid$(A$, 75, 6))
  

End Sub

Sub GetNAVvalues(Prefix$, UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit As Boolean)
' Prefix$ . Nav file 2 letter prefix
' UTsec ... current time, if <0 then just get next record, if >= 0 go to that time
' UT    ... time read from Nav file corresponding to UTsec
' P, T, Th, U, V, W ... pressure, temperature, theta, and winds at UT
' PTWfile$  Nav file to read.
' Quit .... TRUE if EOF

Dim i%, mTh!, mT!, mT2!, mT3!, mT4!, mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!, Wu!, Wv!, Ww!
Dim x!, mMach2!
'    param(1) = P             hPa
'    param(2) = T             K
'    param(3) = mPitch        deg
'    param(4) = mRoll         deg
'    param(5) = mLatitude     deg
'    param(6) = mLongitude    deg
'    param(7) = mHeading      deg
'    param(8) = mZg           km
'    param(9) = mZp           km
'    param(10) = mWspd        kts
'    param(11) = mWdir        deg
'    param(12) = mU           m/s
'    param(13) = mV           m/s
'    param(14) = mW           m/s
'    param(15) = mTheta       K    '                    HIPPO-1    HIPPO-3
'    param(16) = OATn0        K    '
'    param(17) = OATn1        K    'Research T for NGV, ATFR       NA
'    param(18) = OATn2        K    'Research T for NGV, ATHR1      ATHL1
'    param(19) = OATn3        K    'Research T for NGV, ATHR2      ATHL2
'    param(20) = OATn4        K    'Research T for NGV             ATHR1
'    param(21) = OATn5        K    'Research T for NGV             ATHR2
'    param(22) = Mach         -
'    param(23) = Mach2        -
'    param(24) = wALT         km
'    param(25) = rALT         km
'    param(26) = hALT         km
'    param(27) = iALT         km   'IRS altitude
'    param(28) = Ptotal       hPa
'    param(29) = Pdynamic     hPa
'    param(30) = GroundSpeed  m/s
'    param(31) = PSFC         hPa  'Corrected Static Pressure, Fuselage
'    param(32) = Vzac         m/s

'Mach2 was (20) now OATn5
'    param(20)= Mach2         -    'Mach No Squared                XMACH2

  For i = 1 To 50
    param(i) = 99999#
  Next i
  
  Select Case Prefix$
  Case "NG"
    Call GetNGvalues(UTsec&, UT, P!, T!, mT!, mT2!, mT3!, mT4!, mLatitude!, mLongitude!, mZg!, mZp!, mPitch!, mRoll!, mMach2!, PTWfile$, Quit)
    If Quit Then Exit Sub
    param(3) = mPitch
    param(4) = mRoll
    param(5) = mLatitude
    param(6) = mLongitude
    param(8) = mZg
    param(9) = mZp
    param(16) = T
    param(17) = mT
    param(18) = mT2
    param(19) = mT3
    param(20) = mT4
    If Mach2 > 0 Then param(22) = Sqr(mMach2)
    param(23) = mMach2
  Case "MM"
    Call GetMMSvalues(UTsec, UT, P, T, mTh, Wu, Wv, Ww, PTWfile$, Quit)
    param(12) = Wu
    param(13) = Wv
    param(14) = Ww
    param(22) = Mach
    param(23) = Mach2
  Case "PT": Call GetPTWvalues(UTsec&, UT, P!, T!, PTWfile$)
  Case "FT": Call GetFTvalues(UTsec&, UT, P!, T!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, PTWfile$)
    param(5) = mLatitude
    param(6) = mLongitude
    param(8) = mZg
    param(9) = mZp
    param(10) = mWspd
    param(11) = mWdir
  Case "NC": Call GetNCvalues(UTsec&, UT, P!, T!, PTWfile$, Quit)
  Case "IC"
    Call GetICATSvalues(UTsec&, UT&, P!, T!, mZg, PTWfile$, Quit)
    param(8) = mZg
  Case "UC":
    'UTsec = fTstringToSec(Right$(AA$, 6), False) + 27  'MTP is 27 seconds slow
    Call GetUCSEvalues(UTsec&, UT&, P!, T!, mPitch!, mRoll!, mLatitude!, mLongitude!, mHeading!, mZg!, mZp!, mWspd!, mWdir!, PTWfile$, Quit)
    param(3) = mPitch
    param(4) = mRoll
    param(5) = mLatitude
    param(6) = mLongitude
    param(7) = mHeading
    param(8) = mZg
    param(9) = mZp
    param(10) = mWspd
    param(11) = mWdir
  Case "TD"
    'Call GetABLEvalues(UTsec&, UT&, mZg!, mZp!, PTWfile$, Quit)
    
    Call GetTDCvalues(UTsec&, UT&, P!, T!, mTh!, Wu!, Wv!, Ww!, PTWfile$, Quit)
  End Select
  
  param(1) = P                   'hPa
  param(2) = T                   'K
  param(15) = fTheta(T, P)       'K
  'NewRetrieval = False
End Sub

Sub GetXSvalues(Mode%, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$, Quit As Boolean)
Dim i%, A$, HeaderCount%, V!, Txs!
Static PTWlu%, ZZp!(1 To 17), x$, ii%, jj%, j%, nMax%, nVar%, Vscale!(1 To 10), PV1&
Static PVs!, Ts!, zgs!, Zps!, Wzs!, Wms!
'32 2010
'Atlas , Lamich, Strahan, Lucchesi, Steenrod, Ledvina, Conaty, Pawson, Newman, Lait
'NASA Goddard Space Flight Center Codes 910.3 and 916
'GSFC Assimilation GG1X1 grid data interpolated to the DC8 flight curtain
'SOLVE
'1 1
'2000 03 08 2000 07 26
'0.00000 0.00000
'17
'17   'line10
'1000 850 700 500 400 300 250 200 150 100 70 50 30 10 5 2 1
'Pressure Levels(mb)
'seconds from 0 GMT on flight date
'5
'1.00000E-07 0.100000 1.00000 0.0100000 0.100000  line15
'999999 9999 99999 9999 999                       line16
'Ertel 's Potential Vorticity (K m^2/kg s)
'Temperature (k)
'Geopotential Height(M)
'Zonal wind(M / s)
'Meridional wind(M / s)
'2
'0.0100000 0.100000
'9999 999
'Longitude (degrees East)
'Latitude (degrees North)
'0
'4
'Aircraft position is taken from the DFexchange file
'Assimilation analyses are interpolated to the flight curtain
'linearly in latitude and longitude.
'Data are valid for 12 UTC on 8 March, 2000.


' Mode =1 Get a single profile and close file
' Mode =0 Get next profile
  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open XSfile$ For Input As PTWlu
    Input #PTWlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount
      Line Input #PTWlu, A$
      Select Case i
      Case 10
        nMax = Val(A$)
      Case 11
        jj = 1
        For j = 1 To nMax
          ii = InStr(jj, A$, " ")
          If ii = 0 Then ii = Len(A$) + 1
          ZZp(j) = fPtoZ(Val(Mid$(A$, jj, ii - jj)))
          jj = ii + 1
        Next j
      Case 14
        nVar = Val(A$)
      Case 15
        jj = 1
        For j = 1 To nVar
          ii = InStr(jj, A$, " ")
          If ii = 0 Then ii = Len(A$) + 1
          Vscale(j) = Val(Mid$(A$, jj, ii - jj))
          jj = ii + 1
        Next j
      
      Case 16
        ii = InStr(1, A$, " ")
        PV1 = Val(Left$(A$, ii - 1))
      Case Else
      End Select
    Next i
'    ZZp(1) = fPtoZ(1000): ZZp(2) = fPtoZ(850): ZZp(3) = fPtoZ(700): ZZp(4) = fPtoZ(500)
'    ZZp(5) = fPtoZ(400): ZZp(6) = fPtoZ(300): ZZp(7) = fPtoZ(250): ZZp(8) = fPtoZ(200)
'    ZZp(9) = fPtoZ(150): ZZp(10) = fPtoZ(100): ZZp(11) = fPtoZ(70): ZZp(12) = fPtoZ(50)
'    ZZp(13) = fPtoZ(30): ZZp(14) = fPtoZ(10): ZZp(15) = fPtoZ(5): ZZp(16) = fPtoZ(2)
'    ZZp!(17) = fPtoZ(1):
    x$ = " "
  Else
    If EOF(PTWlu) Then GoTo CloseCode
  End If
   
  Do  'Catch up
    If EOF(PTWlu) Then GoTo CloseCode
    Line Input #PTWlu, A$
    i = InStr(1, A$, " ")
    UT = Val(Left$(A$, i - 1))
    If UT < UTsec Then
      For i = 1 To 10
        If EOF(PTWlu) Then GoTo CloseCode
        Line Input #PTWlu, A$
      Next i 'skip data
    End If
  Loop Until UT >= UTsec
'UTsec Lon Lat
'55110 603 758
' -4 38 19 39 42 125 375 681 912 1703
'2949 4920 12817 50960 103319 291838 968698
' 2681 2604 2488 2333 2232 2099 2055 2058 2048 2022
'2002 2002 2045 2211 2380 2504 2488
' 133 1386 2836 5208 6699 8520 9623 10963 12695 15107
'17202 19171 22174 28986 33630 40181 45343
' -45 -37 -36 -40 -17 16 27 33 25 -4
'-19 -25 -35 -13 70 17 141
' -24 -53 -63 27 66 94 61 -18 -76 -120
'-158 -204 -267 -263 -172 -119 34
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'    Line Input #PTWlu, a$
    A$ = Right$(A$, Len(A$) - 6)
    
    Longitude = Val(Left$(A$, InStr(1, A$, " ", 1) - 1)) / 100#
    Latitude = Val(Right$(A$, Len(A$) - InStr(1, A$, " ", 1) + 1)) / 10#
    For i = 1 To nMax: Input #PTWlu, PV(i): Next i
    For i = 1 To nMax: Input #PTWlu, T(i): Next i
    For i = 1 To nMax: Input #PTWlu, Zg(i): Next i
    For i = 1 To nMax: Input #PTWlu, Wz(i): Next i
    For i = 1 To nMax: Input #PTWlu, Wm(i): Next i
    For i = 1 To nMax: Zp(i) = ZZp(i): Next i
    
'    Do
'      If PV(1) = PV1 Then  'Substitute for missing data
'        PV(1) = PVs
'        T(1) = Ts
'        Zg(1) = zgs
'        Wz(1) = Wzs
'        Wm(1) = Wms
'        Zp(1) = Zps
'      Else                  'Save last good cycle
'        PVs = PV(1)
'        Ts = T(1)
'        zgs = Zg(1)
'        Wzs = Wz(1)
'        Wms = Wm(1)
'        Zps = Zp(1)
'      End If
'    Loop Until PV(1) <> PV1
    
  If T(1) = 9999 Then
    nMax = nMax - 1
    For i = 1 To nMax
      PV(i) = PV(i + 1) * Vscale(1)
      T(i) = T(i + 1) * Vscale(2)
      Zg(i) = Zg(i + 1) * Vscale(3)
      Wz(i) = Wz(i + 1) * Vscale(4)
      Wm(i) = Wm(i + 1) * Vscale(5)
      Zp(i) = Zp(i + 1)
'     Debug.Print i; PV(i); T(i); Zg(i); Zp(i); Wz(i); Wm(i)
    Next i
  
  Else
    For i = 1 To nMax
      PV(i) = PV(i) * Vscale(1)
      T(i) = T(i) * Vscale(2)
      Zg(i) = Zg(i) * Vscale(3)
      Wz(i) = Wz(i) * Vscale(4)
      Wm(i) = Wm(i) * Vscale(5)
'     Debug.Print i; PV(i); T(i); Zg(i); Zp(i); Wz(i); Wm(i)
    Next i
  End If
    Levels = nMax
If Mode = 1 Then Close PTWlu: PTWlu = 0
Exit Sub

CloseCode:
      Close PTWlu
      PTWlu = 0
      Quit = True
End Sub

Sub ReadRCconfig(iRC%, RCname$)
Dim i%, j%, k%, L%, i0%, i1%, i2%, lu%, cMd$, V$, z!, TA1!(1 To 3, 1 To 10), TA2!(1 To 3, 1 To 10)
Dim OB1!(1 To 30), OB2!(1 To 30), r$(1 To 8)
    
  
  If Len(Dir(RCname$)) = 0 Then
    MsgBox "Could not find RC configuration file " + RCname$, vbOKOnly
    Exit Sub
  End If
  
  FoundSmatrix = False
  lu% = FreeFile
  Open RCname$ For Input As lu%

  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      Select Case Mid$(cMd$, 2, i0% - 2)
      Case "RCformat"
        Input #lu, RCformat(iRC)
      Case "RAOB"
        Input #lu, RCtemplate$(iRC)     'RC RAOB Template (eg BGDH__2003010912.RAOB2)
        If Mid$(fGetFilename(RCtemplate$(iRC)), 6, 1) = "_" Then RAOBreanalysis = True
        Input #lu, RAOBcount              'Number of available RAOBs
      ' Need to take into account that older RC files only had 6 [RAOB] entries
        i = 0
        Do
          i = i + 1
          Line Input #lu, r$(i)
        Loop Until Len(r$(i)) = 0
        If i = 7 Then
          LR1 = Val(r$(1))         'LR above top of RAOB
          zLRb = Val(r$(2))            'LR break altitude
          LR2 = Val(r$(3))        'LR above break altitude
          RecordStep = Val(r$(4))          'Record Step through available RAOBs
          MinimumRAOBz = Val(r$(5))        'Minimum acceptable RAOB altitude
          ExcessTamplitude = Val(r$(6))    'Random Excess Noise Level on Ground
        Else
          LR1 = Val(r$(1))         'LR above top of RAOB
          RecordStep = Val(r$(2))          'Record Step through available RAOBs
          MinimumRAOBz = Val(r$(3))        'Minimum acceptable RAOB altitude
          ExcessTamplitude = Val(r$(4))    'Random Excess Noise Level on Ground
        End If
        
      Case "Nret"
        Input #lu, Nret
        For i = 1 To Nret  'Read pressure altitude offsets
          Input #lu, dZ(i)
        Next i
      
      Case "Nobs"
        Input #lu, Nobs
        For i = 1 To Nobs  'Read observable errors
          Input #lu, OBrms(1, i) 'These should not be used as there is only one cfg file, but NFL OBrms values
        Next i
      
      Case "NFL"
        Input #lu, NFL     'Read Flight levels
        For i = 1 To NFL
          Input #lu, z
          FLA(i) = Int(z * 100 + 0.5)
        Next i
      Case "Nlo"
        Input #lu, Nlo     'Read Flight levels
        For i = 1 To Nlo
          Input #lu, LO(i)
        Next i
      Case "Nel"
        Input #lu, Nel     'Read Flight levels
        For i = 1 To Nel
          Input #lu, El(i)
        Next i
      Case "Smatrix"
        FoundSmatrix = True
        For i = 1 To NFL
          For j = 1 To Nlo
            For k = 1 To Nel
              For L = 1 To 2  'linear and quadratic term, there is no bias term
                Input #lu, Smatrix(iRC, i, j, k, L)
              Next L
              TA1(j, k) = Smatrix(iRC, i, j, k, 1)
              TA2(j, k) = Smatrix(iRC, i, j, k, 2)
            Next k
          Next j
          Call MapTAtoOB(TA1(), OB1(), Channels, Nel, LocHor, ChInfo())
          Call MapTAtoOB(TA2(), OB2(), Channels, Nel, LocHor, ChInfo())
          For j = 1 To Nobs
            SmatrixOB(iRC, i, j, 1) = OB1(j)
            SmatrixOB(iRC, i, j, 2) = OB2(j)
          Next j
        Next i
        
        
      Case Else
      
      End Select
    End If
  Loop

Exit_Sub:
  Close (lu%)
  
End Sub

Sub ReadMTPH(MTPHname$)
Dim i%, j%, k%, L%, i0%, i1%, i2%, lu%, cMd$, V$, z!, TA1!(1 To 3, 1 To 10), TA2!(1 To 3, 1 To 10)
Dim IntegrationTime!, x$, Nel%, Nlo%
    
  
  If Len(Dir(MTPHname$)) = 0 Then
    MsgBox "Could not find MTPH configuration file " + MTPHname$, vbOKOnly
    Exit Sub
  End If
  
  lu% = FreeFile
  Open MTPHname$ For Input As lu%

  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      Select Case Mid$(cMd$, 2, i0% - 2)
      Case "Aircraft"
        Input #lu, x$
      
      Case "NominalPitch"
        Input #lu, x$
      
      Case "Offsets"
        Line Input #lu, x$
        MTPyaw = Val(Left$(x$, 7))
        Line Input #lu, x$
        MTPpitch = Val(Left$(x$, 7))
        Line Input #lu, x$
        MTProll = Val(Left$(x$, 7))
      
      Case "Integ. Time"
        Input #lu, x$
      
      Case "Frequencies"  'Read Frequencies
        Input #lu, Nlo
        For i = 1 To Nlo
          Line Input #lu, x$
          LOSUI(i) = Val(x$)
        Next i
      
      Case "El. Angles"  'Read Elevation Angles
        Line Input #lu, x$
        Nel = Left$(x$, 3)
        Line Input #lu, x$    'Fiduciary
        For i = 1 To Nel
          Line Input #lu, x$
          j = InStr(1, x$, "'")
          ElSUI(i) = Val(Left$(x$, j))
          If Abs(ElSUI(i)) > EmaxDegrees Then EmaxDegrees = ElSUI(i)
        Next i
      End Select
      
    End If
  Loop

Exit_Sub:
  Close (lu%)
  
End Sub
Sub ReadWCT(Filename$)
Dim i0%, i1%, i2%, lu%, cMd$, V$, FixPath As Boolean, i%, j%, response As Variant

  On Error GoTo CopyCal
  lu% = FreeFile
Carryon:
  Open Filename$ For Input As lu%
  CalFileFormat = CInt(fReadFirstNumber(lu))
  FIsize% = 0
  FixPath = False

  Do
NextCategory:
    If EOF(lu%) Then GoTo Exit_Sub

    FIsize% = FIsize% + 1
    Input #lu%, cMd$                       'Read a line
  ' Extract PI$
    If FIsize% = 2 Then Pi$ = Right$(cMd$, Len(cMd$) - 7)
    
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
      Stop
    End If

    Select Case Mid$(cMd$, 2, i0% - 2)
    Case "WINDOW_CORRECTIONS"
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
          Select Case Left$(cMd$, 3)
          Case "WCT"
            i = Val(Mid$(cMd$, 4, 1))
            j = Val(Mid$(cMd$, 5, 2))
            WINcor(i, j) = Val(V$)
          Case Else
          End Select
        End If
      Loop
      
    Case "EOF"
      Exit Sub
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
  Exit Sub

CopyCal:
  response = MsgBox("Calfile has not been copied to flight directory!" + vbCrLf + " Do you wish to copy the default Calfile?", vbYesNo)
  If response = vbYes Then
    'frmMTPbin.cmdCopyCal_Click
    GoTo Carryon
  Else
    Stop
    Exit Sub
  End If
End Sub

Sub WriteRCconfig(f As Form)
Dim CFGlu%, i%, j%, k%, L%, CFGfile$, mm$, MyDate As Variant

' Open I/O files
  CFGlu = FreeFile
  CFGfile$ = f.txtRAOBpath.Text + "RC\" + fReplaceExtension(f.txtRCfile.Text, "CFG")
  Open CFGfile$ For Output As CFGlu
  
  Print #CFGlu, "' RC Configuration file: Created- " + Date$ + " " + Time$
  Print #CFGlu, ""
  
  Print #CFGlu, "[RCformat]"
  Print #CFGlu, Str(Val(f.txtRCformat.Text))
  Print #CFGlu, ""
  
  Print #CFGlu, "[RAOB]"
  Print #CFGlu, Trim(f.cboRAOBfiles.Text)
  Print #CFGlu, Str(RAOBcount)                    'Number of available RAOBs
  Print #CFGlu, Str(Val(f.txtLR1.Text))           'LR above top of RAOB
  Print #CFGlu, Str(Val(f.txtZb.Text))            'LR break altitude
  Print #CFGlu, Str(Val(f.txtLR2.Text))           'LR above break altitude
  Print #CFGlu, Str(Val(f.txtRecordStep.Text))    'Record Step through available RAOBs
  Print #CFGlu, Str(Val(f.txtRAOBmin.Text))       'Minimum acceptable RAOB altitude
  If f.chkExcessGndT.Value = True Then
    Print #CFGlu, Str(Val(f.txtExcessTamplitude)) 'Random Excess Noise Level on Ground
  Else
    Print #CFGlu, "0"                             'Random Excess Noise Level on Ground
  End If
  Print #CFGlu, SURC$                             'SU IF bandpass
  Print #CFGlu, ""

  Print #CFGlu, "[Nobs]"
  Print #CFGlu, Str(Nobs)
  For i = 1 To Nobs - 1                           'Read observable errors
    Print #CFGlu, Format(OBrms(1, i), "0.00") + " "; 'Assume all RCs have same FLs
  Next i
  Print #CFGlu, Format(OBrms(1, Nobs), "0.00")
  Print #CFGlu, ""
  
  Print #CFGlu, "[Nret]"
  Print #CFGlu, Str(Nret)
  For i = 1 To Nret - 1 'Write retrieval offset levels
    Print #CFGlu, Format(dZ(i), "##0.0") + " ";
  Next i
  Print #CFGlu, Format(dZ(Nret), "##0.0")
  Print #CFGlu, ""
  
  Print #CFGlu, "[NFL]"
  Print #CFGlu, Str(NFL)
  For i = 1 To NFL - 1 'Write flight levels
    Print #CFGlu, Format(Zr(i), "#0.00") + " ";
  Next i
  Print #CFGlu, Format(Zr(NFL), "#0.00")
  Print #CFGlu, ""
  
  Print #CFGlu, "[Nlo]"
  Print #CFGlu, Str(Nlo)
  For i = 1 To Nlo - 1     'Write LO Frequencies
    Print #CFGlu, Format(LO(i), "#00.000") + " ";
  Next i
  Print #CFGlu, Format(LO(Nlo), "#00.000")
  Print #CFGlu, ""
  
  Print #CFGlu, "[Nel]"
  Print #CFGlu, Str(Nel)
  For i = 1 To Nel - 1     'Write LO Frequencies
    Print #CFGlu, Format(El(i), "#00.0") + " ";
  Next i
  Print #CFGlu, Format(El(Nel), "#00.0")
  Print #CFGlu, ""
  
  Print #CFGlu, "[Nif]"
  Print #CFGlu, Str(Nif)
  For i = 1 To Nif      'Write IF offset frequencies (GHz)
    Print #CFGlu, formats(IFoff(1, i), "#0.0000") + " ";
  Next i
  
  Print #CFGlu, formats(IFwt(1, Nif), "#0.0000")
  For i = 1 To Nif      'Write IF weights
    Print #CFGlu, formats(IFwt(1, i), "#0.0000") + " ";
  Next i
  Print #CFGlu, ""
  Print #CFGlu, ""
'  Print #CFGlu, Format(IFwt(Nif), "#0.0000")
' Write sensitivity matrix
  Print #CFGlu, "[Smatrix]"
  For i = 1 To NFL
    For j = 1 To Nlo
      For k = 1 To Nel   'linear and quadratic term, there is no bias term
        Print #CFGlu, formats(Smatrix(0, i, j, k, 1), "#0.000") + " "; formats(Smatrix(0, i, j, k, 2), "#0.00000") + " ";
      Next k
    Next j
    Print #CFGlu, ""
  Next i
  Print #CFGlu, ""
  Close CFGlu
End Sub

Sub ReadCAL(Filename$)
Dim i0%, i1%, i2%, lu%, cMd$, V$, FixPath As Boolean, i%, j%, response As Variant
' Read new mjm .INI Calfile Format
'Exit Sub
  On Error GoTo CopyCal
  lu% = FreeFile
Carryon:
  Open Filename$ For Input As lu%
  CalFileFormat = CInt(fReadFirstNumber(lu))
'MsgBox "ReadCAL Entry!", vbOKOnly

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
    Case "GENERAL"
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
          Case "GenDate": Gendate$ = V$
          Case "FltDate": FltDate$ = V$
          Case "UTstart"
          Case "UTend"
          Case "Channels": Channels = Val(V$)
          Case "Nel": Nel = Val(V$)
          Case "Emissivity": Emissivity = Val(V$)
          Case "Reflectivity": Reflectivity = Val(V$)
          Case "DeltaTmin": DeltaTmin = Val(V$)
          Case Else
          End Select
        End If
      Loop

    Case "FIT_INFO"
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
          Case "Nfit": Nfit = Val(V$)
          Case "Offset": NP$(1) = V$: GOF(1) = 0#
          Case "FitVar1": NP$(2) = V$
          Case "FitVar2": NP$(3) = V$
          Case "FitVar3": NP$(4) = V$
          Case "FitVar4": NP$(5) = V$
          Case "GEC11": GEC(1, 1) = Val(V$)
          Case "GEC12": GEC(1, 2) = Val(V$)
          Case "GEC13": GEC(1, 3) = Val(V$)
          Case "GEC14": GEC(1, 4) = Val(V$)
          Case "GEC15": GEC(1, 5) = Val(V$)
          Case "GEC21": GEC(2, 1) = Val(V$)
          Case "GEC22": GEC(2, 2) = Val(V$)
          Case "GEC23": GEC(2, 3) = Val(V$)
          Case "GEC24": GEC(2, 4) = Val(V$)
          Case "GEC25": GEC(2, 5) = Val(V$)
          Case "GEC31": GEC(3, 1) = Val(V$)
          Case "GEC32": GEC(3, 2) = Val(V$)
          Case "GEC33": GEC(3, 3) = Val(V$)
          Case "GEC34": GEC(3, 4) = Val(V$)
          Case "GEC35": GEC(3, 5) = Val(V$)
          Case "GOF1": GOF(2) = Val(V$)
          Case "GOF2": GOF(3) = Val(V$)
          Case "GOF3": GOF(4) = Val(V$)
          Case "GOF4": GOF(5) = Val(V$)
          Case "TARGET":
            TGToffset = Val(V$)
'            MsgBox "ReadCAL:" + Str(TGToffset), vbOKOnly
          Case "MIXER": MXRoffset = Val(V$)
          Case Else
          End Select
        End If
      Loop
      
    Case "WINDOW_CORRECTIONS"
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
          Select Case Left$(cMd$, 3)
          Case "WCT"
            i = Val(Mid$(cMd$, 4, 1))
            j = Val(Mid$(cMd$, 5, 2))
            WINcor(i, j) = Val(V$)
          Case Else
          End Select
        End If
      Loop
      
    Case "EOF"
      Exit Sub
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
  Exit Sub

CopyCal:
  response = MsgBox("Calfile has not been copied to flight directory!" + vbCrLf + " Do you wish to copy the default Calfile?", vbYesNo)
  If response = vbYes Then
    'frmMTPbin.cmdCopyCal_Click
    GoTo Carryon
  Else
    Stop
    Exit Sub
  End If
End Sub

Sub DecodeAline(A$, GoToNext As Boolean)
' This sub decodes the raw A-line data from the MTP Data Unit
' Parameters include:
' AlineFormat        Single character identifying the A-line format
' DOY                Day of Year
' HH:MM:SS           UT time
' Latitude           Latitude (degrees)
' Longitude          Longitude (degrees)
' Heading            Heading (degrees)
' Pitch              Aircraft Pitch (degrees)
' Roll               Aircraft Roll (degrees)
' gALT               Geometric Altitude (km)
' pALT               Pressure Altitude (km)
' OATn               Aircraft Outside Air Temperature (Celcius)
' Wind Speed         Aircraft Wind Speed (knots)
' Wind Direction     Aircraft Wind Direction (degrees)
' ElCorr             Sensor Unit Elevation Angle Correction (degrees)
' ElCorrUsed         0/1 depending on whether Elcorr was used
' MTP Clock yymmdd   Ditto
' MTP Cloci hhmmss   Ditto
' Status             Receiver Hardware Information
'   StatusBit(7)     128   Acceleration >60 counts
'   StatusBit(6)     64    DU: 0=#1 (old) 1=#2 (new)
'   StatusBit(5)     32    SU: 0=#1 (old) 1=#2 (new)
'   StatusBit(4)     16    Spare
'   StatusBit(3)     8     Spare
'   StatusBit(2)     4     Spare
'   StatusBit(0)     1     Timeout on Phase Lock 0=OK, 1=Fail
'
' Other parameters calculated here are:
' pALTft             Pressure Altitude (ft)
' gALTft             Geometric Altitude (ft)
' OATnav             Aircraft Outside Air Temperature (Kelvin)

Dim AlineFormat$, x$, ALTkmu!, ALTcor!, dALT!
Static dtMTP!
Static LatitudeLast!, LongitudeLast!, gALTlast!, gALTftLast!, pALTftLast!
  
  GoToNext = False
  AlineFormat = Mid$(A$, 2, 1)
  If AlineFormat = "W" And Val(YYYYMMDD$) < 19990000 Then AlineFormat = "Y" 'WAM has different format
  Select Case AlineFormat
  Case " ", "G", "N", "X", "W", "!"     'WB57 after WAM (ie ACCENT) is "W", for WAM is "Y"
' ER2 Format
'          1         2         3         4         5         6         7         8         9         10
' 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456
'   doy hh mm ss snn.lat snnn.lon   hdg ppp.p rr.rr .zgeo  pALT   OAT  Wspd  Wdir Scndispc yymmdd hhmmss
'   doy hh mm ss snn.lat snnn.lon   hdg ppp.p rr.rr .zgeo  pALT   OAT  Wspd  Wdir Scndispc Sta yymmdd hhmmss
' AG266:22:02:32  -3.051 -159.368  64.2    .6   3.7 20479 20178 -65.9    .9   3.2 -188.9 1 255 970923 220222
' Geophysica
' AG...12:46:23  67.821   20.327  36.4   3.0    .6   451   552 -10.3  15.9  36.6 -191.8 1   0 030123
' Put in colon (:) 20030125
' AG...:00:15:16  67.821   20.327 149.5   3.5   -.4 99999   494  18.1  15.4 149.9 -192.4 1   0 030125 143357
' AG173:19:36:50 +35.184 -98.3247 346.0    .2   -.2 13980 13170 -62.8  12.6 170.5 -192.5 1   0 050622 193607
    
    Latitude = Val(Mid$(A$, 15, 8))
    Longitude = Val(Mid$(A$, 23, 9))
    Heading = Val(Mid$(A$, 32, 6))
    Pitch = Val(Mid$(A$, 38, 6))
    Roll = Val(Mid$(A$, 44, 6))
    gALT = Val(Mid$(A$, 50, 6)) / 1000#   'gALT in km
    If gALT = 99.999 Then gALT = gALTlast 'GPS/geometric alt [km]
    gALTft = gALT * cft_km
    pALT = Val(Mid$(A$, 56, 6)) / 1000#   'pALT in km
    pALTft = pALT * cft_km
    If pALT > 23# Then pALT = 99.9
    OATn = Val(Mid$(A$, 62, 6))           'Default OAT is OATnav
    OATnav = OATn + cTo                   'Convert to K
    Wspd = Val(Mid$(A$, 68, 6))
    Wdir = Val(Mid$(A$, 74, 6))
    If AlineFormat = "W" Then Wdir = 0: Wspd = 0 'xxx mjm 990422
    Elcor = Val(Mid$(A$, 80, 7))
    ElCorUsed = Val(Mid$(A$, 87, 2))
    If ElCorUsed Then StatusBit(1) = 0 Else StatusBit(1) = 2  '0 if ElCorUsed, 2 if not used
    If Len(A$) > 102 Then
      StatusBits = Val(Mid$(A$, 89, 4))
      StatusBit(7) = StatusBits And 128   ' Acceleration >60 counts
      StatusBit(6) = StatusBits And 64    ' DU: 0=#1 (old) 1=#2 (new)
      StatusBit(5) = StatusBits And 32    ' SU: 0=#1 (old) 1=#2 (new)
      StatusBit(4) = StatusBits And 16    ' Spare
      StatusBit(3) = StatusBits And 8     ' Spare
      StatusBit(2) = StatusBits And 4     ' Spare
      StatusBit(0) = StatusBits And 1     ' Timeout on Phase Lock 0=OK, 1=Fail
    Else
      StatusBits = 0
      StatusBit(7) = 0
      StatusBit(6) = 0
      StatusBit(5) = 0
      StatusBit(4) = 0
      StatusBit(3) = 0
      StatusBit(2) = 0
      StatusBit(0) = 0
    End If
    
  Case "Y", "!"
' WB57 Format for WAM
'          1         2         3         4         5         6         7         8         9"        10         11
' 12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123"
'   DOY:HH:MM:SS Latitude Longitud  Heading  Pitch   Roll   Z_GEO    pALT Temper ..Wspd ..Wdir sddd.d n yymmdd hhmmss"
'   ###:##:##:## S###.### S###.### S###.### S###.# S###.# S###### S###### S###.# S###.# S###.# "
'Final version:
' AW054:14:00:20 +027.681 +065.230 -128.200 +035.4 -023.8 +045323 +045323 +028.6 +158.6 +042.1
'Original version:
' AW054:17:42:53 +043.432 +122.321 +004.2 -038.2 +035.4 05941 05941 -008.6 128.4 085.5
    Latitude = Val(Mid$(A$, 16, 8))
    Longitude = Val(Mid$(A$, 25, 8))
    Heading = Val(Mid$(A$, 34, 8))
    Pitch = Val(Mid$(A$, 43, 6))
    Roll = Val(Mid$(A$, 50, 6))
    gALTft = Val(Mid$(A$, 57, 7))
    gALT = gALTft / cft_km                   'gALT km
    pALTft = Val(Mid$(A$, 65, 7))
    pALT = pALTft / cft_km                   'pALT km
    OATn = Val(Mid$(A$, 73, 6))
    OATnav = OATn + cTo
    Wspd = Val(Mid$(A$, 80, 5))
    Wdir = Val(Mid$(A$, 87, 5))
    Wdir = Wdir + 180
    Elcor = Val(Mid$(A$, 80, 7))
    ElCorUsed = Val(Mid$(A$, 87, 2))
    If ElCorUsed Then StatusBit(1) = 0 Else StatusBit(1) = 2
    If Len(A$) > 102 Then
      StatusBits = Val(Mid$(A$, 89, 4))
      StatusBit(7) = StatusBits And 128   ' Acceleration >60 counts
      StatusBit(6) = StatusBits And 64    ' DU: 0=#1 (old) 1=#2 (new)
      StatusBit(5) = StatusBits And 32    ' SU: 0=#1 (old) 1=#2 (new)
      StatusBit(4) = StatusBits And 16    ' Spare
      StatusBit(3) = StatusBits And 8     ' Spare
      StatusBit(2) = StatusBits And 4     ' Spare
      StatusBit(0) = StatusBits And 1     ' Timeout on Phase Lock 0=OK, 1=Fail
    Else
      StatusBits = 0
      StatusBit(7) = 0
      StatusBit(6) = 0
      StatusBit(5) = 0
      StatusBit(4) = 0
      StatusBit(3) = 0
      StatusBit(2) = 0
      StatusBit(0) = 0
    End If
    
  Case "D", "E", "!"
' DC8 Format
'          1         2         3         4         5         6         7         8         9"
' 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
' AD999 99999999 9999999 99999999 9999 99999 99999 99999 99999 99999 9999   9999 999999 1 999999 999999
' AD250 16:12:31  30.230 -081.668  074 -01.1  -0.6 00034 -0079  27.8 0000   0000   13.0 1 010907 161115
' AD250 16:51:37  26.343 -083.030  194   1.0  10.5 29688 27998 -26.3 0002   0041   11.4 1 010907 165022
'   doy hh:mm:ss snn.lat snnn.lon  hdg ppp.p  rr.r radar pALTft.OATn Wspd   Wdir ScnDis P yymmdd hhmmss'
' TexAQS
' AE224 17:38:05  39.913 -105.117 x999 - 0.2 - 0.7 95960 05436  35.6 0007   0204   12.3 1 000811 173802
'                                 ^fix
' SOLVE
' AD 75 05:40:40  67.821   20.335  207    .3    .6 01589 02055 -13.4 0000   0000   11.9 1 000315 053809
' CAMEX-4
' AD250 16:12:31  30.230 -081.668  074 -01.1  -0.6 00034 -0079  27.8 0000   0000   13.0 1 010907 161115
' SOLVE2
' AD309:15:41:12 +34.480 -136.900 013 +00.0 +00.0 34000 34000 -10.10 1 021106 154112
' PAVE
' AD999:99:99:99 +99.999 -999.999  999 +99.9 +99.9 99999 99999 -99.9 9999   9999 +999.9 0 000 041209 193917
' AD107:19:39:49 +34.480 -136.900  013 +00.0 +00.0 01100 01100 +17.1 0110   0270 +180.0 1 000 041209 193949
'          1         2         3         4         5         6         7         8         9"
' 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"

    Latitude = Val(Mid$(A$, 15, 8))
    Longitude = Val(Mid$(A$, 23, 9))
    Heading = Val(Mid$(A$, 32, 5))
    Pitch = Val(Mid$(A$, 37, 6))
    Roll = Val(Mid$(A$, 43, 6))
    gALTft = Val(Mid$(A$, 49, 6))
    If gALTft <> 99999 Then gALT = gALTft / cft_km Else gALT = gALTlast         'GPS/geometric alt [km]
    pALTft = Val(Mid$(A$, 55, 6))      'pressure altitude
    If pALTft <> 99999 Then pALT = pALTft / cft_km
    OATn = Val(Mid$(A$, 61, 6))
    OATnav = OATn + cTo
    Wspd = Val(Mid$(A$, 68, 4))
    Wdir = Val(Mid$(A$, 74, 5))
    Elcor = Val(Mid$(A$, 79, 7))
    ElCorUsed = Val(Mid$(A$, 86, 2))
 
  Case Else
    Call MsgBox("You are not using a valid A-line format!", vbOK)
    Exit Sub
  
  End Select

' DU date and time should always be present at end of A-line, decode from right side
  x$ = Right$(A$, 13)
  yymmddMTP$ = Left$(x$, 6)
  hhmmssMTP$ = Right$(x$, 6)  'use Right$ in case formatting error
   
' Check to make sure all data are valid and set flags if not
  If Abs(Latitude) > 90 Then Latitude = 99.999
  If Abs(Longitude) > 180 Then Longitude = 999.999
  If Heading < 0 Then Heading = Heading + 360
  If Heading > 360 Then Heading = 999
  If Abs(Pitch) > 90 Then Pitch = 999.9
  If Abs(Roll) > 90 Then Roll = 999.9
  If Wspd > 999 Then Wspd = 999
  If Wdir > 999 Then Wdir = 999
  
  If gALT > Ceiling Then
    gALT = 99.9
    'Call MsgBox("Geometric Altitude (gALT) exceeded aircraft Ceiling= " & Str(Ceiling) & "!", vbOKOnly)
  End If
  
  If gALT = 0 Then gALT = gALTlast Else gALTlast = gALT  'ER2 drops gALT occassionally
  
  If pALT > Ceiling Then
    pALT = 99.9
    'Call MsgBox("Pressure Altitude (pALT) exceeded aircraft Ceiling= " & Str(Ceiling) & "!", vbOKOnly)
  End If
  If pALT < 0# Then pALT = 0#: pALTft = 0#

' Fix pALT if necessary
  If pALT = 99.9 Then
    pALTft = pALTlast * cft_km
    ALTkm = pALTlast
  Else  'code below needs to be cleaned up
    pALTlast = pALT
    If pALT < 0 Then pALT = 0
'    ALTkmu = ALTkm
'    'ALTcor = .046 - .000437 * ALTkmu ^ 2 + 3.315E-05 * (ALTkmu ^ 3)
'    ALTkm = ALTkmu + ALTfujCONST * ALTcor
  End If
' The following line not used, but might want to set up a idiot light or beep
  If Abs(Roll) > RollThreshold Then RollThresholdFlag = True Else RollThresholdFlag = False
  
  UTsecMTPlast = UTsecMTP
  UTsecMTP = fTstringToSec(hhmmssMTP$, False)

  If Mid$(A$, 7, 8) <> "99:99:99" Then                  'Use NAV UT if it's present
    Doy = Val(Mid$(A$, 3, 3))
    If Doy = 0 Or Doy = 999 Then Doy = fYYYYMMDDtoDOY(YYYYMMDD$)
    UTsecNav = fTstringToSec(Mid$(A$, 7, 8), True)      'Nav has colons 'xxx mjm was 7,8!!! 03/01/22
    UTsec = UTsecNav
    dtMTP = UTsecMTP - UTsec
  Else
    Doy = 999
    UTsec = UTsecMTPlast - dtMTP + (UTsecMTP - UTsecMTPlast)  'UTsecMTP - (Diff of MTP-NAV) + Scan Length
    UTsecNav = UTsec
    If Latitude > 99 Then
      Latitude = LatitudeLast
      Longitude = LongitudeLast
      gALT = gALTlast
      gALTft = gALTftLast
    End If
  End If
  UTsecNAVlast = UTsec
  LatitudeLast = Latitude
  LongitudeLast = Longitude
  gALTlast = gALT
  gALTftLast = gALTft
  
'  If UTtakeoff > 0 And AC$ = "ER" And Mission$ = "CAMEX4" Then
'    ALTkm = fZnavCorr(ALTkm, (UTsec - UTtakeoff) / 1000)
'  End If
'  pALT = ALTkm         'Have some sort of valid altitude at this point
'  pALTft = pALT * cft_km
  If pALTftLast = 0 Then pALTftLast = pALTft
  dALT = pALTft - pALTftLast
      
  hhmmssNAV$ = fSecToTstring(UTsecNav, False)         'Get rid of colons
End Sub


Sub DecodeBCDElines(A$, SU$, Lt$, GoToNext As Boolean)
Dim i%, j%, Jindex%, x!, iChannels%, Target1 As Boolean
' Allow a channel to be dropped in retrieval
  iChannels = Channels        'Default is to use all channels
'  Select Case Mission$
'  Case "EUPLEX"
'    Select Case yyyymmdd$
'    Case "20030115", "20030119": iChannels = 3  'Read 3 Channels, but retrieve only 2
'    End Select
'  End Select
  
  Lt$ = Left$(A$, 1)
  Select Case Lt$

  Case "B"                           '  C(i,j)
    For j = 1 To 5
      If RHS Then Jindex = j Else Jindex = 11 - j
      For i = 1 To iChannels
        x = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * iChannels * (j - 1), 6))
        If x > 30000 Or x < 0 Then C(i, Jindex) = 30000 Else C(i, Jindex) = x
      Next i
    Next j
    Bline% = True
    

  Case "C"                           '  C(i,j)
    For j = 6 To 10
      If RHS Then Jindex = j Else Jindex = 11 - j
      For i = 1 To iChannels
        x = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * iChannels * (j - 6), 6))
        If x > 30000 Or x < 0 Then C(i, Jindex) = 30000 Else C(i, Jindex) = x
      Next i
    Next j
    Cline% = True

  Case "D"                           '  MUX()
    
    Select Case SU$
    Case "DC8"
      If Mission$ = "SOLVE2" Or (Mission$ = "PAVE" And YYYYMMDD$ = "20050109") Then
' Before 20041120 was 'If yyyymmdd$ > 20021100 Then'
' D line now contains raw counts again, and
' d line contains converted numbers which are ignored as they are converted
'
' 0        1         2         3         4         5         6         7         8         9         10        11
' 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'     Tnd    Tamp   Tsyn   Tmix   Tt1    Tt2    ACC+   Twin   Tmot   XXX    Vref   Tdat1  Tdat2  VCC/2  15V/n  Trad  ACCm"
' D +39.30 +44.49 -89.57 +39.55 +03.40 +04.10 +01.05 +02.84 +35.01 +01.98 +02.05 -104.42 -104.42 +02.49 +01.91 +42.00  ACCm not added until after SOLVE2
' Channel Numbers are:
' 1 tND, 2 tLO1, 3 tLO2, 4 tIFA,5 TGTl,6 TGTh,7 ACCp 8 tWIN,9 tMTR,10 xxx,11 ref,12 tDC1,13 tDC2,14 tPS5,15 tPS12,16 Trad, 17 ACCm
        For i = 1 To 16: Muxs(i) = Mid$(A$, 2 + (i - 1) * 7, 7): Next i
      Else
        For j = 1 To 17: Mux(j) = fHexToDec(Mid$(A$, 3 + 4 * (j - 1), 3)): Next j
      ' First 4 channels (different assignments in Old vs New format)
        For j = 1 To 17: Muxs(j) = ThermistorDC(j, Mux(j)): Next j
      End If
        
      Muxs(13) = Muxs(12)  'DC2 Make DC8 DC1 and DC2 same as ER2
      Muxs(12) = Muxs(11)  'DC1
      Tifa = Muxs(4)
      Tlo = Muxs(5)
      Thi = Muxs(6)
      If YYYYMMDD$ > 20021100 Then
        Ttgt = Tlo
      Else
        If Thi < -37.5 Then Ttgt = Tlo Else Ttgt = Thi   'DC8 used -33C xxx
      End If
    Case "ER2G"
      For j = 1 To 16
        Mux(j) = fHexToDec(Mid$(A$, 3 + 4 * (j - 1), 3))
      Next j
      For j = 1 To 16: Muxs(j) = Thermistor(j, Mux(j)): Next j
      
    Case "ER2S"
      For j = 1 To 16
        Mux(j) = fHexToDec(Mid$(A$, 3 + 4 * (j - 1), 3))
      Next j
      For j = 1 To 16: Muxs(j) = ThermistorER_1(j, Mux(j)): Next j
      Tifa = Muxs(4)
      Tlo = Muxs(5)
      Thi = Muxs(6)
      If YYYYMMDD$ > "20051100" Then
        If Tlo < -26 Then Ttgt = Tlo Else Ttgt = Thi   'New RTD circuit has new overlap region
      Else
        If Thi < -37.5 Then Ttgt = Tlo Else Ttgt = Thi   'DC8 used -33C xxx
      End If
      Twin = ThermistorER_1(8, Mux(8))
     'Debug.Print UTks; Tifa; Tlo; Thi; Ttgt
    Case "ER2T"
      For j = 1 To 16
        Mux(j) = fHexToDec(Mid$(A$, 3 + 4 * (j - 1), 3))
      Next j
      For j = 1 To 16: Muxs(j) = ThermistorER_2(j, Mux(j)): Next j
      Tifa = Muxs(4)
      Tlo = Muxs(5)
      Thi = Muxs(6)
      If YYYYMMDD$ > "20070701" Then
        If Thi < 0# Then Ttgt = Tlo Else Ttgt = Thi    'range changed to +/-80 C for hot target
      Else
        If Thi < -37.5 Then Ttgt = Tlo Else Ttgt = Thi
      End If
      Twin = ThermistorER_2(8, Mux(8))
      If RealTime And OATn = 0# Then
        OATn = Twin - 17         'Default OAT is OATnav
        OATnav = OATn + cTo                   'Convert to K
        OAT = OATnav
      End If
      
    Case Else
      Stop
    End Select
    Muxs(0) = Ttgt
  If Muxs(9) < -99 Then Muxs(9) = -99.9
  If Muxs(12) < -99 Then Muxs(12) = -99.9
  If Muxs(13) < -99 Then Muxs(13) = -99.9
  ' Muxs(i) Parameter
  ' 00 Ttgt  ' 01 Tnd  ' 02 Tlo1  ' 03 Tlo2
  ' 04 Tifa  ' 05 Tlo  ' 06 Thi   ' 07 ACC+
  ' 08 Twin  ' 09 Tmtr ' 10       ' 11 Vref
  ' 12 Tdc1  ' 13 Tdc2 ' 14 PS5   ' 15 PS12  ' 16 ACC-
  Tnd = Muxs(1)
  Tlo1 = Muxs(2)   'VCO=LO equivalences
  Tlo2 = Muxs(3)
  Tmxr = Muxs(4)  'IFA=MXR
  Tifa = Muxs(4)
  Tlo = Muxs(5) 'target low
  Thi = Muxs(6) 'target hi
  'If SU$ <> "DC8" Then
  '  If Thi < -37.5 Then Ttgt = Tlo Else Ttgt = Thi 'DC8 used -33C xxx
  'End If
  Muxs(0) = Ttgt
  
  Twin = Muxs(8) 'Window
  If Twin < -80 Or Twin > 45 Then Muxs(8) = Twin   '99
  Tmtr = Muxs(9)
  Vref = Muxs(11)    'Voltage reference for a/d to correct thermistors (=2.51 V)
  Tdc1 = Muxs(12)
  Tdc2 = Muxs(13)
  Vps5 = Muxs(14)
  Vps12 = Muxs(15)
'  dACC = (Muxs(7) - Muxs(16)) / 5.2  'dACC is in units of 0.01 g
  dACC = (Muxs(7) - Muxs(16)) / 52  'dACC is in units of 0.1 g
'  Muxs(17) = OATn
'  Muxs(18) = ALTkm * 3    Dline = True
  Dline = True
    
  Case "E"
' 1234567890123456789012345678901234567890
' E 20021 17719 15062 12060 10726 10930
    If Mid$(A$, 2, 1) = " " Then Target1 = True Else Target1 = False
    For j = 11 To 12
      For i = 1 To iChannels
        x = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * iChannels * (j - 11), 6))
        If x > 30000 Or x < 0 Then x = 30000
        ' Check to see if there are two targets
        If Target1 Then C(i, j) = x Else C(i, j + 2) = x
      Next i
    Next j
  
    
  ' Define Base, Sky and Noise Diode counts (Used in a number of places)
  
    For i = 1 To iChannels
      CS(i, LocHor) = C(i, LocHor)
      If Target1 Then
        CB(i) = C(i, 12): dND(i) = C(i, 11) - CB(i)
      Else
        CB2(i) = C(i, 14): dND2(i) = C(i, 13) - CB2(i)
      End If
    Next i
    ' dND(i) = c(i, 11) - CB(i) was dND(i) = c(1, 11) - CB(i) until 20030924!
    Eline% = True
  
  Case "F": 'Print "Instrument Failure!" + Space$(61);
  
  Case "I": 'Beep: Print "Instrument on!" + Space$(66);
  
  Case Else
  
  End Select

End Sub

Function fZnavCorr(z!, dUT!)
' z is NAV pressure altitude (km)
' dUT are UT ks since takeoff
' Return with corrected NAV pressure altitude (km)

Select Case AC$
Case "ER"
  Select Case Mission$
  Case "CAMEX4"
    'ZPnavCOR [meters] = 0 + 85 * (utsec - utsec_takeoff)/14400 + 162 * (ZPnav/20 km)2 + 105 * (ZPnav/20 km)10
    fZnavCorr = z + (85 * dUT / 14.4 + 162 * (z / 20) ^ 2 + 105 * (z / 20) ^ 10) / 1000
  Case Else
    fZnavCorr = z
  End Select

Case Else
  fZnavCorr = z
End Select

End Function


Sub GetINUvalues(UTsec&, UT&, Pinu!, PA1!, Pim!, PA2!, INUfile$)
Dim i%, A$, HeaderCount%
Static INUlu%

  If INUlu = 0 Then
    INUlu = FreeFile
    Open INUfile$ For Input As INUlu
    For i = 2 To 6: Line Input #INUlu, A$: Next i
  Else
    If EOF(INUlu) Then
      Close INUlu
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Line Input #INUlu, A$
    UT = 3600# * Val(Mid$(A$, 5, 2)) + 60# * Val(Mid$(A$, 8, 2)) + Val(Mid$(A$, 11, 2))
  Loop Until UT >= UTsec
'NASA 809 Flight #00-036  01/11/00/011 (GMT)       RECORDER 4     HARD DISK 11
'                                                                                                     INS    INS    . . . . . based on MMS pressure sensors . . . . .
'DAY TIME     T     INU       INU    INU   PRESS    SAT   PITCH   ROLL   TRUE  AIR  N/S   E/W   GND   Wind  Wind     mach                  static    SAT   wind  wind
'DDD HH:MM:SS S  LATITUDE LONGITUDE  ALT                                 HEAD  SPD  VEL   VEL   SPD   Dir   Speed   number   TAS  altitude  press   temp    dir  speed    TIME
'                   deg       deg     ft    mbar    degC   deg    deg    deg  knts  knts  knts knts   deg   knots           knots   feet      mb   deg C    deg  knots  HH:MM:SS
'148 13:13:13    N42.203  W 72.525    602  991.4     0.0  + 4.2  - 0.1  215.5       N  0  E  0    0  213.1   20.1              0      354 1000.34    0.0  180.0    0.0  13:13:13
'148 13:13:14    N42.203  W 72.525    602  991.4     0.0  + 4.2  - 0.1  215.5       N  0  E  0    0  213.1   20.1
'1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345
'         1         2         3         4         5         6         7         8         9         10        11        12        13        14        15        16        17

'NASA 809 Flight #01-136  09/09/01/252 (GMT)       RECORDER 5     HARD DISK 5
'                                                                                                     INS    INS    . . . . . based on MMS pressure sensors . . . . .
'DAY TIME     T     INU       INU    INU   PRESS    SAT   PITCH   ROLL   TRUE  AIR  N/S   E/W   GND   Wind  Wind     mach                  static    SAT   wind  wind
'DDD HH:MM:SS S  LATITUDE LONGITUDE  ALT                                 HEAD  SPD  VEL   VEL   SPD   Dir   Speed   number   TAS  altitude  press   temp    dir  speed    TIME
'                   deg       deg     ft    mbar    degC   deg    deg    deg  knts  knts  knts knts   deg   knots           knots   feet      mb   deg C    deg  knots  HH:MM:SS
'252 16:00:00    N30.233  W 81.768   9537  709.4    -3.8  +17.7  - 5.3  255.6       S 37  W165  169  183.0    6.4   0.264    169     9456  711.65   -3.8  166.7    5.4  16:00:00
'252 16:00:01    N30.233  W 81.769   9619  707.2    -3.8  +17.0  - 5.0  255.2       S 39  W165  170  178.9    5.1   0.264    169     9491  710.68   -3.8  154.0    5.1  16:00:01

  Pinu = Val(Mid$(A$, 34, 7)) / 3280.8
  If Pinu < 0 Then Pinu = 0#
  Pim = Val(Mid$(A$, 129, 8) / 3280.8)
  PA1 = fPtoZ(Val(Mid$(A$, 41, 7)) / 3280.8)
  PA2 = fPtoZ(Val(Mid$(A$, 137, 8)) / 3280.8)
End Sub

Sub GetFTvalues(UTsec&, UT&, P!, T!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, FTfile$)
Dim i%, A$, HeaderCount%, x!
Static PTWlu%, LastT!, LastP!, LastWspd!, LastWdir!, LastZp!, LastZg!

  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open FTfile$ For Input As PTWlu
    Input #PTWlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #PTWlu, A$: Next i
  Else
    If EOF(PTWlu) Then
      Close PTWlu
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    If P <> 9999.99 Then LastP = P
    If T <> 999.99 Then LastT = T
    Line Input #PTWlu, A$
    UT = Val(Left$(A$, 5))
    P = Val(Mid$(A$, 21, 8))
    T = Val(Mid$(A$, 29, 7))
  Loop Until UT >= UTsec Or EOF(PTWlu)
  If EOF(PTWlu) Then UT = -1: Exit Sub
'  UT    Lat    Lon    Pres    Temp   Theta   TAS     GPS_Alt  WindSpd WindDir
'66978  34.64 -118.08  930.66 306.40 312.76   0.00   724.00    2.40  359.80
'60681  32.16 -107.58   76.56 212.71 443.41 208.01 18087.20  999.99  999.99
'60682  32.16 -107.58   76.53 999.99 999.99 999.99 18088.60  999.99  999.99
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'    Line Input #PTWlu, a$
  If UTsec > 40508 Then
  x = x
  End If
    If P = 9999.99 Then P = LastP
    If P < 0 Then P = 0#
    If T = 999.99 Then T = LastT
    mLatitude = Mid$(A$, 6, 7)
    mLongitude = Mid$(A$, 13, 8)
    If mZg < 0 Then mZg = 0
    mZg = Val(Mid$(A$, 50, 9)) / 1000#
    mZp = fPtoZ(Val(Mid$(A$, 21, 8)))
    mWspd = Mid$(A$, 59, 8)
    If mWspd = "9999.99" Then mWspd = 0
    mWdir = Mid$(A$, 67, 8)
    If mWdir = "9999.99" Then mWdir = 0
    If mZg <> 9999.99 Then LastZg = mZg
    If mWspd <> 9999.99 Then LastWspd = mWspd
    If mWdir <> 9999.99 Then LastWdir = mWdir

'Debug.Print UT; P; fPtoZ(P)
End Sub

Sub GetNGvalues(UTsec&, UT&, P!, T!, mT!, mT2!, mT3!, mT4!, mLatitude!, mLongitude!, mZg!, mZp!, mPitch!, mRoll!, mMach2!, FTfile$, Quit As Boolean)
Dim i%, j%, A$, HeaderCount%, x!, Utime&, TAS!, b$, PALT_A!
Static PTWlu%, LastT!, LastP!, LastWspd!, LastWdir!, LastZp!, LastZg!, LastUTsec!
  
  If UT = 0 And PTWlu > 0 Then
    Close PTWlu
    PTWlu = 0
'  Else
'    Quit = False
'    PTWlu = 0
  End If
  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open FTfile$ For Input As PTWlu
    Line Input #PTWlu, A$   'Skip Header Info
    LastUTsec = 0
  Else
    If EOF(PTWlu) Then
      Close PTWlu
      PTWlu = 0
      Quit = True
      Exit Sub
    End If
  End If
   
' UTC      ATRL  AT_A  GGALT  GGLAT  GGLON  PALT  PITCH  PSFC  ROLL  TAS_A  Time
'22:24:24 -41.8909 -43.3967 9302.98 36.6453 -118.323 9150.33 2.21617 300.439 -0.0850106 241.245 13954
'19:00:04 13.1145 12.9608 1707.68 39.9024 -105.101 1615.38 -1.77342 833.586 0.0328811 0 1694
'Date , UTC, ACINS, ALT, AT_A, GGALT, GGLAT, GGLON, GGSPD, GSF, MACH_A, PALT_A, Pitch, PS_A, PT_A, QC_A, Roll, TAS_A, THDG, TTFR, TTHR1, TTHR2, TT_A, VSPD, ATFR, ATHR1, ATHR2, ATTACK, pALT, PSFC, SSLIP, TASF, WDC, WSC, XMACH2
'2008-04-30,13:29:41.269,-0.0029799212,1820.7245,24.727966,1724.4001,39.91276,-105.118164,0.0,0.0,0.03119573,1821.0283,-1.0026349,812.765,813.31976,0.55300844,-0.8809662,0.0,217.52313,20.431808,17.903831,18.24286,24.785934,0.018811557,20.431707,17.90373,18.24276,0.0,1819.125,812.8905,0.0,0.45533758,0.0,0.0,1.7573701E-6
'         1         2         3         4         5
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
SkipBlankLine:
    Line Input #PTWlu, A$
    If A$ = "" Then GoTo SkipBlankLine
    'UTsec , Latitude, Longitude, ALTkm, T1, T2
    'b$ = Left$(A$, 10)
    i = InStr(1, A$, ",")
    'Debug.Print A$
    b$ = Left$(A$, i - 1)
    j = InStr(i + 1, A$, ",")
    HHMMSS$ = Mid$(A$, i + 1, j - i - 1)
    If Len(HHMMSS$) = 7 Then
      HHMMSS$ = "0" + HHMMSS$
    End If
    UT = fTstringToSec(HHMMSS$, True)
    If UT < LastUTsec Then UT = UT + 86400#
    i = InStr(j + 1, A$, ",")
    ACINS = Val(Mid$(A$, j + 1, i - j))                 'ACINS
    j = InStr(i + 1, A$, ",")
    pALT = Val(Mid$(A$, i + 1, j - i))                   'ALT
    i = InStr(j + 1, A$, ",")
    T = Val(Mid$(A$, j + 1, i - j)) + cTo               'AT_A
    If x = -999.99 Or x = -32767 Then
       T = T
    End If
    j = InStr(i + 1, A$, ",")
    mZg = Val(Mid$(A$, i + 1, j - i)) / 1000#           'GGALT
    i = InStr(j + 1, A$, ",")
    mLatitude = Val(Mid$(A$, j + 1, i - j))             'GGLAT
    j = InStr(i + 1, A$, ",")
    mLongitude = Val(Mid$(A$, i + 1, j - i))            'GGLON
    i = InStr(j + 1, A$, ",")
    GroundSpeed = Val(Mid$(A$, j + 1, i - j))           'GGSPD
    j = InStr(i + 1, A$, ",")
    GSF_A = Val(Mid$(A$, i + 1, j - i))                 'GSF_A
    i = InStr(j + 1, A$, ",")
    Mach = Val(Mid$(A$, j + 1, i - j))                  'MACH_A
    j = InStr(i + 1, A$, ",")
    mZp = Val(Mid$(A$, i + 1, j - i)) / 1000#           'PALT_A
    P = fZtoP(mZp)
    i = InStr(j + 1, A$, ",")
    mPitch = Val(Mid$(A$, j + 1, Len(A$) - j))          'Pitch
    j = InStr(i + 1, A$, ",")
    Pstatic = Val(Mid$(A$, i + 1, j - i))               'PS_A
    i = InStr(j + 1, A$, ",")
    Ptotal = Val(Mid$(A$, j + 1, Len(A$) - j))          'PT_A
    Pdynamic = Ptotal - Pstatic
    j = InStr(i + 1, A$, ",")
    QC_A = Val(Mid$(A$, i + 1, j - i))                  'QC_A
    i = InStr(j + 1, A$, ",")
    mRoll = Val(Mid$(A$, j + 1, Len(A$) - j))           'Roll
    j = InStr(i + 1, A$, ",")
    TAS = Val(Mid$(A$, i + 1, j - i))                   'TAS_A
    i = InStr(j + 1, A$, ",")
    Heading = Val(Mid$(A$, j + 1, Len(A$) - j))         'THDG
    j = InStr(i + 1, A$, ",")                           'HIPPO-1 HIPPO-3
    TOATn1 = Val(Mid$(A$, i + 1, j - i))                'TTFR    TTHL1
    i = InStr(j + 1, A$, ",")
    TOATn2 = Val(Mid$(A$, j + 1, Len(A$) - j))          'TTHR1   TTHL2
    j = InStr(i + 1, A$, ",")
    TOATn3 = Val(Mid$(A$, i + 1, j - i))                'TTHR2   TTHR1
    i = InStr(j + 1, A$, ",")
    TOATn4 = Val(Mid$(A$, j + 1, Len(A$) - j))          'TT_A    TTHR2
    j = InStr(i + 1, A$, ",")
    TOATn0 = Val(Mid$(A$, i + 1, j - i))                 'VSPD    TT_A
    i = InStr(j + 1, A$, ",")
    Select Case Mission$
    Case "HIPPO"
      mT = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo      'ATFR
      j = InStr(i + 1, A$, ",")
      mT2 = Val(Mid$(A$, i + 1, j - i)) + cTo           'ATHR1
      i = InStr(j + 1, A$, ",")
      mT3 = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo     'ATHR2
      If T < 0 Then T = mT3
      j = InStr(i + 1, A$, ",")
      AttackAngle = Val(Mid$(A$, i + 1, j - i))         'ATTACK
      i = InStr(j + 1, A$, ",")
      mZp = Val(Mid$(A$, j + 1, Len(A$) - j))           'pALT
      j = InStr(i + 1, A$, ",")
      PSFC = Val(Mid$(A$, i + 1, j - i))                'PSFC
      If x = -999.99 Or x = -32767 Then
        mZp = mZp / 1000
        P = fZtoP(mZp)
      Else
        mZp = fPtoZ(x)
      End If
      i = InStr(j + 1, A$, ",")
      SideSlipAngle = Val(Mid$(A$, j + 1, Len(A$) - j)) 'SSLIP
      j = InStr(i + 1, A$, ",")
      TAS = Val(Mid$(A$, i + 1, j - i))                 'TASF
      i = InStr(j + 1, A$, ",")
      Wdir = Val(Mid$(A$, j + 1, Len(A$) - j))          'WDC
      j = InStr(i + 1, A$, ",")
      Wspd = Val(Mid$(A$, i + 1, j - i))                'WSC
      i = InStr(j + 1, A$, ",")
      Mach2 = Val(Mid$(A$, j + 1, Len(A$) - j))         'XMACH2
    Case "HIPPO-3", "PREDICT", "HIPPO-4", "HIPPO-5", "TORERO", "MPEX"
      Vzac = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo    'VSPD
      j = InStr(i + 1, A$, ",")
      mT = Val(Mid$(A$, i + 1, j - i)) + cTo            'ATLH1
      i = InStr(j + 1, A$, ",")
      mT2 = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo     'ATLH2
      j = InStr(i + 1, A$, ",")
      mT3 = Val(Mid$(A$, i + 1, j - i))                 'ATRH1
      i = InStr(j + 1, A$, ",")
      mT4 = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo     'ATLH2
      j = InStr(i + 1, A$, ",")
      pALT = Val(Mid$(A$, i + 1, j - i)) / 1000#        'PALT
      i = InStr(j + 1, A$, ",")
      PSFC = Val(Mid$(A$, j + 1, Len(A$) - j)) + cTo    'PSFC
      If PSFC = -999.99 Or PSFC = -32767 Then
        PSFC = mZp / 1000
        P = fZtoP(PSFC)
      End If
      j = InStr(i + 1, A$, ",")
      SideSlipAngle = Val(Mid$(A$, i + 1, j - i))       'SSLIP
      i = InStr(j + 1, A$, ",")
      TAS = Val(Mid$(A$, j + 1, Len(A$) - j))           'TASF
      j = InStr(i + 1, A$, ",")
      Wdir = Val(Mid$(A$, i + 1, j - i))                'WDC
      i = InStr(j + 1, A$, ",")
      Wspd = Val(Mid$(A$, j + 1, Len(A$) - j))          'WSC
      j = InStr(i + 1, A$, ",")
      mMach2 = Val(Right$(A$, Len(A$) - i))             'XMACH2
      Mach2 = mMach2
      If Mach2 < 0 Then
      x = x
      End If
      If mMach2 = -32767 And mZp > 0 And mZp < 25 Then
        mMach2 = 0.02089 + 0.06555 * mZp - 0.00122 * mZp ^ 2
      End If
      
    Case Else
      MsgBox vbOKOnly, "GetNGvalues does not recognize this mission!"
    End Select
    LastUTsec = UT
End Sub
Sub GetNGvalues_Trex(UTsec&, UT&, P!, T!, mT!, mLatitude!, mLongitude!, mZg!, mZp!, mPitch!, mRoll!, FTfile$, Quit As Boolean)
Dim i%, j%, A$, HeaderCount%, x!, Utime&, TAS!
Static PTWlu%, LastT!, LastP!, LastWspd!, LastWdir!, LastZp!, LastZg!, LastUTsec!

  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open FTfile$ For Input As PTWlu
    Input #PTWlu, A$   'Skip Header Info
    'For i = 2 To HeaderCount: Line Input #PTWlu, a$: Next i
    LastUTsec = 0
  Else
    If EOF(PTWlu) Then
      Close PTWlu
      PTWlu = 0
      Quit = True
      Exit Sub
    End If
  End If
   
' UTC      ATRL  AT_A  GGALT  GGLAT  GGLON  PALT  PITCH  PSFC  ROLL  TAS_A  Time
'22:24:24 -41.8909 -43.3967 9302.98 36.6453 -118.323 9150.33 2.21617 300.439 -0.0850106 241.245 13954
'19:00:04 13.1145 12.9608 1707.68 39.9024 -105.101 1615.38 -1.77342 833.586 0.0328811 0 1694
    Line Input #PTWlu, A$
    'UTsec , Latitude, Longitude, ALTkm, T1, T2
    HHMMSS$ = Left$(A$, 8)
    UT = fTstringToSec(HHMMSS$, True)
    If UT < LastUTsec Then UT = UT + 86400#
    i = InStr(10, A$, " ")
    mT = Val(Mid$(A$, 10, i - 10)) + cTo
    j = InStr(i + 1, A$, " ")
    T = Val(Mid$(A$, i + 1, j - i)) + cTo
    i = InStr(j + 1, A$, " ")
    mZg = Val(Mid$(A$, j + 1, i - j)) / 1000#
    j = InStr(i + 1, A$, " ")
    mLatitude = Val(Mid$(A$, i + 1, j - i))
    i = InStr(j + 1, A$, " ")
    mLongitude = Val(Mid$(A$, j + 1, i - j))
    j = InStr(i + 1, A$, " ")
    mZp = Val(Mid$(A$, i + 1, j - i)) / 1000#
    i = InStr(j + 1, A$, " ")
    mPitch = Val(Mid$(A$, j + 1, i - j))
    j = InStr(i + 1, A$, " ")
    P = Val(Mid$(A$, i + 1, j - i))
    i = InStr(j + 1, A$, " ")
    mRoll = Val(Mid$(A$, j + 1, i - j))
    j = InStr(i + 1, A$, " ")
    TAS = Val(Mid$(A$, i + 1, j - i))
    i = InStr(j + 1, A$, " ")
    Utime = (Mid$(A$, j + 1, Len(A$) - j))
    LastUTsec = UT
End Sub
Sub GetPTWvalues(UTsec&, UT&, P!, T!, PTWfile$)
Dim i%, A$, HeaderCount%, Tf!, Ts!
Static PTWlu%

  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open PTWfile$ For Input As PTWlu
    Input #PTWlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #PTWlu, A$: Next i
  Else
    If EOF(PTWlu) Then
      Close PTWlu
      Exit Sub
    End If
  End If
   
  If UTsec < 0 Then
    Line Input #PTWlu, A$
    If 1 = 0 Then UT = Val(Left$(A$, 7)) Else UT = Val(Left$(A$, 6))
  Else
    Do  'Catch up
      Line Input #PTWlu, A$
      If 1 = 0 Then UT = Val(Left$(A$, 7)) Else UT = Val(Left$(A$, 6))
    Loop Until (UT >= UTsec And Mid$(A$, 7, 6) <> "999.99") Or EOF(PTWlu)
  End If
  If UT <> UTsec Then
  i = i
  End If
'  Loop Until UT >= UTsec Or EOF(PTWlu)
  If EOF(PTWlu) Then UT = -1: Exit Sub
'Changing PTW formats!
'19990409
'UtSec BoxTemp RamPres VertDifPres HorzDifPres AirTemp AnaPxdTemp TAS StatPres DigPxdTemp
'  47385   23.02    0.77   -0.19   -1.40  296.55   35.73   11.41 1009.78   36.00
'
'19990912
'UtSec BoxTemp RamPres VertDifPres HorzDifPres AirTemp AnaPxdTemp System28 TAS StatPres DigPxdTemp
'  48801   36.20    1.08    0.45   -0.43  303.48   33.61   26.10   13.60 1011.89   28.00
'
'20020601
'CRYSTAL-FACE Format (NB Fast Air Temp <1 sec, but sampled at 1 sec, Slow Air Temp = 4 sec)
' UT  FastTmp SlowTmp StatP  PitotP    RamP    TAS
'60992 302.92 303.13 1017.47 1017.65   0.18   5.54
'60993 302.64 303.08 1017.45 1017.67   0.22   6.13
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'    Line Input #PTWlu, a$
    
    Select Case Val(YYYYMMDD$)
    Case Is < 19990912             'Format changed (see above)
      P = Val(Mid$(A$, 64, 8))
      T = Val(Mid$(A$, 40, 8)) + cTo
    Case Is < 20020601
      P = Val(Mid$(A$, 72, 8))
      T = Val(Mid$(A$, 40, 8)) + cTo
    Case Is < 20050800
'HAVE2 - Starboard and Port T probes
'UtSec BoxTemp StarAirTemp PortAirTemp PlateTemp System28 TAS PitotPres PPTemp StatPres SPTemp RamPres STatd PTatd
'  65378    46.18   304.23   304.38    35.18    26.66    67.31  1039.41    34.00  1012.84    34.00    26.57  2290.71  2286.16
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
      If 1 = 0 Then  'Raw files from Tom are different from archived files!!
        P = Val(Mid$(A$, 80, 9))
        Tf = Val(Mid$(A$, 17, 9))     'Kelvin
        Ts = Val(Mid$(A$, 26, 9))     'Kelvin
        T = Tf
      Else  'Same as C-F, pre-AVE
' UT  FastTmp SlowTmp StatP  PitotP    RamP    TAS
'65378 304.00 304.14 1009.92 1039.41  29.49  70.96
        P = Val(Mid$(A$, 21, 7))
        Tf = Val(Mid$(A$, 7, 6))    'Kelvin
        Ts = Val(Mid$(A$, 14, 6))    'Kelvin
        T = Tf                 'Tfast=Tstarboard
      End If
    Case Else
      P = Val(Mid$(A$, 21, 7))
      Tf = Val(Mid$(A$, 7, 6))    'Kelvin
      Ts = Val(Mid$(A$, 14, 6))    'Kelvin
      T = Tf
    End Select
    If P < 0 Then P = 0#
'Debug.Print UT; P; fPtoZ(P)

End Sub


Sub GetMGvalues(UTsec&, UT&, dALT!, dLAT!, dLON!, q%, Turb!, re!, MGfile$)
Dim i%, A$, HeaderCount%
Static MGlu%

  If MGlu = 0 Then
    MGlu = FreeFile
    Open MGfile$ For Input As MGlu
    Input #MGlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #MGlu, A$: Next i
  Else
    If EOF(MGlu) Then
      Close MGlu
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Line Input #MGlu, A$
    UT = Val(Left$(A$, 8)) + 0.5
  Loop Until UT >= UTsec
' UT      DALT    DLAT     DLONG   Q TEDR  REYN
' 47640.6   210  4219573  -7253231 1 99999 9999
'123456789012345678901234567890123456789012345678901234567890
'         1         2         3         4         5         6         7         8
  dALT = Val(Mid$(A$, 9, 6) / 1000#)   'Celcius
  dLAT = Val(Mid$(A$, 15, 9) / 100000#)
  dLON = Val(Mid$(A$, 24, 10) / 100000#)
  q = Val(Mid$(A$, 25, 1))
  Turb = Val(Mid$(A$, 36, 6) / 100#)
  re = Val(Mid$(A$, 42, 5) / 100#)

End Sub

Sub GetMMSvalues(UTsec&, UT&, P!, T!, Th!, U!, V!, W!, MMfile$, Quit As Boolean)
' UTsec ... current time, if <0 then just read next record
' UT    ... time read from MMS file
' P, T, Th, U, V, W ... pressure, temperature, theta, and winds at UT
' MMSfile$  Nav file to read.

Dim i%, j%, A$, HeaderCount%
Static MMlu%, FPlu%, Tcorrection%, FPfile$
' Set Quit TRUE on entry if multiple passes are to be made so that previous file is closed
  If Quit Then
    If MMlu <> 0 Then Close MMlu
    MMlu = 0
    Quit = False
  End If
  
  If MMlu = 0 Then
    MMlu = FreeFile
    Open MMfile$ For Input As MMlu
    Input #MMlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #MMlu, A$: Next i
    Select Case MMfile$
    Case "c:\ER2\CRYSTAL\MM\MM20020726.ER2": Tcorrection = 1550.84
    Case "c:\ER2\CRYSTAL\MM\MM20020728.ER2": Tcorrection = 1941.93
    Case Else: Tcorrection = 0
    End Select
  Else
    If EOF(MMlu) Then
      Close MMlu, FPlu%
      MMfile$ = ""
      Exit Sub
    End If
  End If
     
  If UTsec < 0 Then  'If UTsec negative then just read next record
    Line Input #MMlu, A$
    UT = Int(Val(Left$(A$, 8)) + 0.5) + Tcorrection
  Else
    Do  'Catch up                    'ER2 MMS 1940 sec slow on 20020728 and 1550 sec slow on 20020726
      Line Input #MMlu, A$
      UT = Int(Val(Left$(A$, 8)) + 0.5) + Tcorrection
    Loop Until (UT >= UTsec And Mid$(A$, 18, 4) <> "9999") Or EOF(MMlu)
  End If
  
  If EOF(MMlu) Then
    Close MMlu, FPlu%
    MMlu = 0
    FPlu = 0
    Quit = True
    Exit Sub
  End If

Select Case Val(YYYYMMDD$)
Case Is < 20010801
'GMT       Psta Tsta Thta   U    V    W
' 24518.8  9237 2559 2618   11   -8  -14
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
  P = (Val(Mid$(A$, 9, 6)) / 10#)
  T = Val(Mid$(A$, 15, 5) / 10#) 'Kelvin
  OATmms = T + cTo
  Th = Val(Mid$(A$, 20, 5) / 10#)
  U = Val(Mid$(A$, 25, 5) / 10#)
  V = Val(Mid$(A$, 30, 5) / 10#)
  W = Val(Mid$(A$, 35, 5) / 10#)

Case Else
'CAMEX-4, CRYSTAL
'  GMT     Psta  Tsta  Thta   U    V    W
'  GMT       Psta Tsta Thta   U    V    W
' 65314.726 10158 3053 3039  -77    7    5
' 71931.1   566 21124 47970 -100   21   -1   'Final CRYSTAL data
'  GMT     Psta  Tsta  Thta    U     V     W
' 50226.9  8978 27915 28788 99999 99999 99999 'Preliminary GH data
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'  GMT     Psta  Tsta  Thta    U     V     W
' 49055.5  9163 28429 29148  -821   138   208
' 51115.6  2332 21848 33117  3368  -283    -3  'GloPac
' 63397.3  8860 99999 99999 9999 9999 9999

  P = (Val(Mid$(A$, 10, 5)) / 10#)
  T = Val(Mid$(A$, 16, 5) / 100#) 'Kelvin
  OATmms = T '+ cTo
  If T = 999.99 Then Exit Sub
  Th = Val(Mid$(A$, 22, 5) / 100#)
'  U = Val(Mid$(A$, 27, 5) / 10#)
'  V = Val(Mid$(A$, 32, 5) / 10#)
'  W = Val(Mid$(A$, 37, 5) / 10#)
  U = Val(Mid$(A$, 28, 5) / 10#)
  V = Val(Mid$(A$, 34, 5) / 10#)
  W = Val(Mid$(A$, 40, 5) / 10#)

End Select

End Sub

Sub GetMMSvaluesM(UTsec&, UT&, P!, T!, Th!, U!, V!, W!, MMfile$, FPfile$, Quit As Boolean)
' UTsec ... current time, if <0 then just read next record
' UT    ... time read from MMS file
' P, T, Th, U, V, W ... pressure, temperature, theta, and winds at UT
' MMSfile$  Nav file to read.

Dim i%, j%, A$, HeaderCount%
Static MMlu%, FPlu%, Tcorrection%
' Set Quit TRUE on entry if multiple passes are to be made so that previous file is closed
  If Quit Then
    If MMlu <> 0 Then Close MMlu
    MMlu = 0
    If FPlu <> 0 Then Close FPlu
    FPlu = 0
    Quit = False
  End If
  
  If MMlu = 0 Then
    MMlu = FreeFile
    Open MMfile$ For Input As MMlu
    Input #MMlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #MMlu, A$: Next i
    Select Case MMfile$
    Case "c:\ER2\CRYSTAL\MM\MM20020726.ER2": Tcorrection = 1550.84
    Case "c:\ER2\CRYSTAL\MM\MM20020728.ER2": Tcorrection = 1941.93
    Case Else: Tcorrection = 0
    End Select
  Else
    If EOF(MMlu) Then
      Close MMlu, FPlu%
      MMfile$ = ""
      Exit Sub
    End If
  End If
  
  If LCase$(Right$(MMfile$, 4)) <> ".ict" Then
    i = InStrRev(MMfile$, "\")
    j = Len(MMfile$)
    FPfile$ = Left$(MMfile$, i - 3) + "FP\FP" + Right$(MMfile$, j - i - 2)
  End If
  
  If FPlu = 0 And Dir$(FPfile$) <> "" Then
    FPlu = FreeFile
    Open FPfile$ For Input As FPlu
    Input #FPlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #FPlu, A$: Next i
  End If
   
  If UTsec < 0 Then  'If UTsec negative then just read next record
    Line Input #MMlu, A$
    UT = Int(Val(Left$(A$, 8)) + 0.5) + Tcorrection
  Else
    Do  'Catch up                    'ER2 MMS 1940 sec slow on 20020728 and 1550 sec slow on 20020726
      Line Input #MMlu, A$
      UT = Int(Val(Left$(A$, 8)) + 0.5) + Tcorrection
    Loop Until (UT >= UTsec And Mid$(A$, 18, 4) <> "9999") Or EOF(MMlu)
  End If
  
  If EOF(MMlu) Then
    Close MMlu, FPlu%
    MMlu = 0
    FPlu = 0
    Quit = True
    Exit Sub
  End If

Select Case Val(YYYYMMDD$)
Case Is < 20010801
'GMT       Psta Tsta Thta   U    V    W
' 24518.8  9237 2559 2618   11   -8  -14
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
  P = (Val(Mid$(A$, 9, 6)) / 10#)
  T = Val(Mid$(A$, 15, 5) / 10#) 'Kelvin
  OATmms = T + cTo
  Th = Val(Mid$(A$, 20, 5) / 10#)
  U = Val(Mid$(A$, 25, 5) / 10#)
  V = Val(Mid$(A$, 30, 5) / 10#)
  W = Val(Mid$(A$, 35, 5) / 10#)

Case Else
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'  GMT     Psta  Tsta  Thta    U     V     W
' 49055.5  9163 28429 29148  -821   138   208
' 51115.6  2332 21848 33117  3368  -283    -3  'GloPac
' 63593.3, 9172,29349,30083,-9999,-9999,-9999  'ATTREX

  P = (Val(Mid$(A$, 10, 5)) / 10#)
  T = Val(Mid$(A$, 16, 5) / 100#) 'Kelvin
  OATmms = T
  Th = Val(Mid$(A$, 22, 5) / 100#)
  U = Val(Mid$(A$, 28, 5) / 10#)
  V = Val(Mid$(A$, 34, 5) / 10#)
  W = Val(Mid$(A$, 40, 5) / 10#)

  If FPlu > 0 Then
'1234567890123456789012345678901234
'   UT     P_ALT LAT    LONG    TAS
' 50226.9   979  34918 -117876  825
' 50227.9  1002  34918 -117875  824
' 50228.9  1026  34918 -117874  831
'138518.3,-9999,-99999,-999999,-999
  
    If UTsec < 0 Then
      Line Input #FPlu, A$
      UT = Int(Val(Left$(A$, 8)) + 0.5) + Tcorrection
    Else
      Do  'Catch up
        Line Input #FPlu, A$
        UT = Int(Val(Left$(A$, 8)) + 0.5) + Tcorrection
      Loop Until (UT >= UTsec) Or EOF(FPlu)
    End If
    TAS = Mid$(A$, 31, 4) / 10#
    If TAS >= 0 Then
      Mach = TAS / Sqr(cCp / cCv * cRd * T)
      Mach2 = Mach ^ 2
    Else
      TAS = 0#
      Mach = 0#
      Mach2 = 0#
    End If
  End If
End Select

End Sub

Sub GetMMSvalues_n(UTsec&, UT&, P!, T!, Th!, U!, V!, W!, MMfile$, Quit As Boolean)
Dim i%, A$, HeaderCount%
Static MMlu%

  If MMlu = 0 Then
    MMlu = FreeFile
    Open MMfile$ For Input As MMlu
    Input #MMlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #MMlu, A$: Next i
  Else
    If EOF(MMlu) Then
      Close MMlu
      MMfile$ = ""
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Line Input #MMlu, A$
    UT = Int(Val(Left$(A$, 8)) + 0.5)
  Loop Until UT >= UTsec Or EOF(MMlu)
  
  If EOF(MMlu) Then
    Close MMlu
    MMlu = 0
    Quit = True
    Exit Sub
  End If

'GMT       Psta Tsta Thta   U    V    W
' 24518.8  9237 2559 2618   11   -8  -14
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
If Mission$ = "CAMEX4" Then
  P = (Val(Mid$(A$, 11, 6)) / 10#)
  T = Val(Mid$(A$, 17, 5) / 10#) - cTo  'Celcius
  OATmms = T + cTo
  Th = Val(Mid$(A$, 22, 5) / 10#)
  U = Val(Mid$(A$, 27, 5) / 10#)
  V = Val(Mid$(A$, 32, 5) / 10#)
  W = Val(Mid$(A$, 37, 5) / 10#)

Else
  P = (Val(Mid$(A$, 9, 6)) / 10#)
  T = Val(Mid$(A$, 15, 5) / 10#) - cTo  'Celcius
  OATmms = T + cTo
  Th = Val(Mid$(A$, 20, 5) / 10#)
  U = Val(Mid$(A$, 25, 5) / 10#)
  V = Val(Mid$(A$, 30, 5) / 10#)
  W = Val(Mid$(A$, 35, 5) / 10#)
End If
P = fPtoZ(P)
End Sub
Sub GetPTWvalues_n(UTsec&, UT&, P!, T!, PTWfile$)
Dim i%, A$, HeaderCount%, Tf!, Ts!
Static PTWlu%

  If PTWlu = 0 Then
    PTWlu = FreeFile
    Open PTWfile$ For Input As PTWlu
    Input #PTWlu, HeaderCount, A$  'Skip Header Info
    For i = 2 To HeaderCount: Line Input #PTWlu, A$: Next i
  Else
    If EOF(PTWlu) Then
      Close PTWlu
      Exit Sub
    End If
  End If
   
  Do  'Catch up
    Line Input #PTWlu, A$
    UT = Val(Left$(A$, 7))
  Loop Until UT >= UTsec Or EOF(PTWlu)
  If EOF(PTWlu) Then UT = -1: Exit Sub
'Changing PTW formats!
'19990409
'UtSec BoxTemp RamPres VertDifPres HorzDifPres AirTemp AnaPxdTemp TAS StatPres DigPxdTemp
'  47385   23.02    0.77   -0.19   -1.40  296.55   35.73   11.41 1009.78   36.00
'
'19990912
'UtSec BoxTemp RamPres VertDifPres HorzDifPres AirTemp AnaPxdTemp System28 TAS StatPres DigPxdTemp
'  48801   36.20    1.08    0.45   -0.43  303.48   33.61   26.10   13.60 1011.89   28.00
'
'20020601
'CRYSTAL-FACE Format (NB Fast Air Temp <1 sec, but sampled at 1 sec, Slow Air Temp = 4 sec)
'UtSec  BoxTemp   FastAirT SlowAirT   PlateT System28      TAS StatPres SPTemp  PitotPres   PPTemp  RamPres    FTatd    STatd
'  52368    29.96   300.44   301.02    27.54    26.58     3.45  1012.13    26.00  1012.20    26.00     0.07  2377.34  2366.47
'  62372    27.97  9999.00  9999.00    50.21    26.90  9999.00   981.43    50.00  1002.72    50.00    21.29  2070.64  2063.57
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
    Line Input #PTWlu, A$
    
    Select Case Val(YYYYMMDD$)
    Case Is < 19990912             'Format changed (see above)
      P = Val(Mid$(A$, 64, 8))     'Celcius
      T = Val(Mid$(A$, 40, 8))
    Case Is < 20020601
      P = Val(Mid$(A$, 72, 8))     'Celcius
      T = Val(Mid$(A$, 40, 8))
    Case Else
      P = Val(Mid$(A$, 81, 8))
      Tf = Val(Mid$(A$, 18, 8))    'Kelvin
      Ts = Val(Mid$(A$, 27, 8))    'Kelvin
      T = Tf
    End Select
    If P < 0 Then P = 0#
'Debug.Print UT; P; fPtoZ(P)
End Sub


Sub ExportTXTfile(INPfile$, OUTfile$)
Dim i%, j%, V(0 To 30) As String, H(0 To 30) As String, Free!, out$, Gain1!, Gain2!, Gain3!
Dim INPlu%, OUTlu%, Hcount%
' INPfile$ is a REF file
' OUTfile$ is a TXT file
'
INPlu = FreeFile
Open INPfile$ For Random Access Read Write As INPlu Len = Len(REF)
'FileRecords = LOF(INPlu) / Len(REF) - HiddenRecords

OUTlu% = FreeFile
Open OUTfile$ For Output As OUTlu

Free = 0#
' Print Formatting information
  Print #OUTlu, "Flight" + vbTab + YYYYMMDD$
  Print #OUTlu, "UT Range" + vbTab + Format$(UTmin / 1000#, "#00.000") + vbTab + Format$(UTmax / 1000#, "#00.000")
  Print #OUTlu, "LAT Range" + vbTab + Format$(LATmin, "#00.000") + vbTab + Format$(LATmax, "#00.000")
  Print #OUTlu, "LON Range" + vbTab + Format$(LONmin, "#000.000") + vbTab + Format$(LONmax, "#000.000")
  Print #OUTlu, "ALT Range" + vbTab + Format$(pALTmin, "#00.000") + vbTab + Format$(pALTmax, "#00.000")
  Print #OUTlu, "T Range" + vbTab + Format$(Tacmin, "#00.000") + vbTab + Format$(Tacmax, "#00.000")
 
  
  H$(0) = "UTks": H$(1) = "pALT": H$(2) = "OATmtp": H$(3) = "Lat"
  H$(4) = "Long": H$(5) = "Pitch": H$(6) = "Roll": H$(7) = "Hdg"
  H$(8) = "LRac": H$(9) = "Zt1": H$(10) = "Zt2": H$(11) = "OATnav"
  H$(12) = "Gain1": H$(13) = "Gain2": H$(14) = "Gain3": H$(15) = "CB1"
  H$(16) = "CB2": H$(17) = "CB3": H$(18) = "dND1": H$(19) = "dND2"
  H$(20) = "dND3": H$(21) = "C15": H$(22) = "C25": H$(23) = "C35"
  H$(24) = "C16": H$(25) = "C26": H$(26) = "C36"
'  H$(27) = "C101" + vbTab + "C102" + vbTab + "C103" + vbTab + "C104" + vbTab + "C105" + vbTab + "C106" + vbTab + "C107" + vbTab + "C108" + vbTab + "C109" + vbTab + "C110" + vbTab + "C111" + vbTab + "C112"
'  H$(28) = "C201" + vbTab + "C202" + vbTab + "C203" + vbTab + "C204" + vbTab + "C205" + vbTab + "C206" + vbTab + "C207" + vbTab + "C208" + vbTab + "C209" + vbTab + "C210" + vbTab + "C211" + vbTab + "C212"
'  H$(29) = "C301" + vbTab + "C302" + vbTab + "C303" + vbTab + "C304" + vbTab + "C305" + vbTab + "C306" + vbTab + "C307" + vbTab + "C308" + vbTab + "C309" + vbTab + "C310" + vbTab + "C311" + vbTab + "C312"
  H$(27) = "Ttgt" + vbTab + "Tnd" + vbTab + "Tlo1" + vbTab + "Tlo2" + vbTab + "Tifa" + vbTab + "Tlo" + vbTab + "Thi" + vbTab + "Acc+" + vbTab + "Twin" + vbTab
  H$(27) = H$(27) + "Tmtr" + vbTab + "Spare" + vbTab + "Vref" + vbTab + "DC1" + vbTab + "DC2" + vbTab + "PS5" + vbTab + "PS12" + vbTab + "Acc-"

Hcount = 27
out$ = H$(0)
For j = 1 To Hcount
     out$ = out$ + vbTab + H$(j)
Next j
Print #OUTlu%, out$  'Write header string

For i = 1 To TotalCycles
  Call REFread(INPlu, i)
' Map parameters to an array to facilitate formatting
  V(0) = Format$(UTsec / 1000#, "##0.000"): V(1) = Format$(pALT, "#0.000")
  V(2) = Format$(OATmtp, "##0.0"): V(3) = Format$(Latitude, "##0.000")
  V(4) = Format$(Longitude, "###0.000"): V(5) = Format$(Pitch, "##0.0")
  V(6) = Format$(Roll, "#00.0"): V(7) = Format$(Heading, "#000.0")
  V(8) = Format$(LRac, "##0.0"): V(9) = Format$(Zt1, "#0.00")
  V(10) = Format$(Zt2, "#0.00"): V(11) = Format$(OATnav, "#00.00")
'  Gain1 = G1         '(c(1, 6) - CB1) / (90 - Ttgt)  'Lab Hot Target Gains
'  Gain2 = G2         '(c(2, 6) - CB2) / (90 - Ttgt)
'  Gain3 = G3         '(c(3, 6) - CB3) / (90 - Ttgt)
  V(12) = Format$(g(1), "##0.00"): V(13) = Format$(g(2), "##0.00")
  V(14) = Format$(g(3), "##0.00"): V(15) = Format$(CB(1), "##0.00")
  V(16) = Format$(CB(2), "##0.00"): V(17) = Format$(CB(3), "##0.00")
  V(18) = Format$(dND(1), "##0.00"): V(19) = Format$(dND(2), "##0.00")
  V(20) = Format$(dND(3), "##0.00"): V(21) = Format$(C(1, 5), "##0.00")
  V(22) = Format$(C(2, 5), "##0.00"): V(23) = Format$(C(3, 5), "##0.00")
  V(24) = Format$(C(1, 6), "##0.00"): V(25) = Format$(C(2, 6), "##0.00")
  V(26) = Format$(C(3, 6), "##0.00")
  
'  v(27) = Format$(c(1, 1), "####0")
'  For j = 2 To 12
'    v(27) = v(27) + vbTab + Format$(c(1, j), "###0")
'  Next j
'
'  v(28) = Format$(c(2, 1), "####0")
'  For j = 2 To 12
'    v(28) = v(28) + vbTab + Format$(c(2, j), "###0")
'  Next j
'
'  v(29) = Format$(c(3, 1), "####0")
'  For j = 2 To 12
'    v(29) = v(29) + vbTab + Format$(c(3, j), "###0")
'  Next j

  
  V(Hcount) = Format$(Muxs(0), "##0.00")
  For j = 1 To 16
    V(Hcount) = V(Hcount) + vbTab + Format$(Muxs(j), "##0.00")
  Next j
'
  out$ = V(0)
  For j = 1 To Hcount
    out$ = out$ + vbTab + V(j)
  Next j
  If Latitude < 90 Then Print #OUTlu, out$
  DoEvents
Next i

Close INPlu, OUTlu

End Sub


Sub MPreadOld(lu%, Record%)
Dim A$, i%
Static nHeader%

If Record = 0 Then
  Input #lu, nHeader%, A$
  Record = 1
  Do           'Read all header lines
    Record = Record + 1
    Line Input #lu, A$
    Select Case Record%
    Case 2
      Pi$ = "MJ Mahoney"
    Case 5
      Mission$ = A$
      Select Case Mission$
      Case "TOTE/VOTE": Mission$ = "TOTE_VOTE"
      Case "CRYSTAL-FACE": Mission$ = "CRYSTAL"
      End Select
    Case 7
      YYYYMMDD$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
    Case Is < nHeader
    End Select
  Loop Until Record = nHeader
  Record = 0
Else

'         1         2         3         4         5         6         7         8
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123
'  34710 16   0.327  12.6   1.4 252.6  99.9  99.9 999.9 999.9  64.666 -147.101 999.9
  Line Input #lu, A$
  UTsec = Left$(A$, 7)
  Nlev = Mid$(A$, 8, 3)
  Nlev1 = 33 - Nlev + 1
  Nlev2 = 33
  pALT = Mid$(A$, 11, 8)  'km
  Pitch = Mid$(A$, 19, 6)
  Roll = Mid$(A$, 25, 6)
  OATmtp = Mid$(A$, 31, 6)
  Zt1 = Mid$(A$, 37, 6)
  Zt2 = Mid$(A$, 43, 6)
  Th1 = Mid$(A$, 49, 6)
  Th2 = Mid$(A$, 55, 6)
  Latitude = Mid$(A$, 61, 8)
  Longitude = Mid$(A$, 69, 9)
  LRac = Mid$(A$, 78, 6)
  If Nlev > 0 Then GoodScan = True Else GoodScan = False
  GoodTrop = True
  For i = 1 To Nlev
'1234567890
'  723 2277
    Line Input #lu, A$
    zzz(33 - i + 1) = Left$(A$, 6)
    TTT(33 - i + 1) = Mid$(A$, 7, 6)
    'Debug.Print zzz(Nlev - i + 1); TTT(Nlev - 1 + 1)
  Next i
  For i = 1 To Nlev
    sZZZ(i) = zzz(i) / 100#
    sTTT(i) = TTT(i) / 10#
  Next i
'  60268 31  16.983   1.5  -0.7 189.0  16.9  99.9 377.6 999.9   6.726  -83.813  -1.5  0.49
' 28386 225.7 2.1 28282   479
  
End If
'57  2110
'GARY, Bruce (bgary@jpl.nasa.gov) and MAHONEY, MJ
'M/S 246-101; Jet Propulsion Laboratory; Pasadena, CA 91109
'DC-8 Microwave Temperature Profiler (MTP/DC8)
'TOTE/VOTE
'1 1
'1995 12 11  1996 06 26  {FLT DATE & REDUCTION DATE}
'0.0  0.0
'Remote sensing altitude (units of 10 meters)
'Elapsed UT seconds from 0 hours on day given by DATE
'1              {NV = number of primary variables temp/mtp}
'0.1            {scale factors for primary variables temp/mtp}
'9999           {missing values for primary variables temp/mtp}
'Retrieved air temperature (K)
'12             {number of auxiliary variables}
'1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
'99 99.999 99.9 99.9 999.9 999.9 999.9 999.9 999.9 99.999 999.999 999.9
'NX(1) is the number of altitudes in subsequent data records
'Pressure altitude of DC-8 (km)
'aircraft Pitch(Deg)
'aircraft Roll(Deg)
'Horizon brightness temperature (ie, OAT, similar to SAT); avg ch1, ch2 & ch3 (C)
'Tropopause #1 (km).
'Tropopause #2 (km).
'Potential temperature of tropopause #1 (K).
'Potential temperature of tropopause #2 (K).
'Latitude (Deg)
'Longitude (Deg)
'dT/dz (K/km) for 1.0 km layer centered on aircraft flight altitude.
'3              {number of special comment lines}
'The following data appear to be normal.
'This data has undergone FINAL CALIBRATION.  RMS performance given below is
'  based on 30 intercomparisons with radiosondes.
'23             {number of normally included comment lines}
'All altitudes are pressure altitudes.  PALT = A/C 's pressure altitude.
'Here 's a brief free-form tutorial on how to decipher the MTP data: Data groups
'   consist of the following group of lines per 17-second observing cycle.
'First line is:  UTSEC, # alt's in following table, Pressure Altitude, Pitch,
'   Roll, Outside air temp (K), tropopause altitude #1 (km), tropopause altitude
'   #2 (km) [if present], potential temperatures of tropopause #1 and #2
'   (4-cycle average), latitude, longitude, & lapse rate near flight level.
'   The 1-liners (for each cycle) can be stripped & imported into a spreadsheet
'   for convenient plotting of trop altitude, lapse rate, etc. The tropopause
'   altitudes are calculated by cubic spline interpolation of the retrieved
'   altitudes using the WMO definition (that is, trop #1 is lowest altitude
'   where average lapse rate > -2 K/km from initial -2 K/km point to any point
'   within 2 km; trop #2 occurs above first trop after lapse rate is < -3K/km
'   for >1 km, and then first trop definition applies, possibly from within
'   the 1 km region.)
'Remaining set of lines for each cycle consist of 2 columns: col#1 is pressure
'   altitude (units of 10 m), & col#2 is temperature from MTP (units of 0.1 K).
'Note: Temperature accuracy varies with altitude distance from flight level,
'   latitude and season.  Within 2 km of aircraft RMS < 1.0 K, within 5 km of
'   aircraft RMS < 2.0 K, within 10 km of aircraft RMS < 3.0 K.  These
'   values are for aircraft flight altitude of 9.5 to 12.5 km.  For additional
'   clarification ask bgary@jpl.nasa.gov.
'
'  34710 16   0.327  12.6   1.4 252.6  99.9  99.9 999.9 999.9  64.666 -147.101 999.9
'  723 2277
'  553 2379
'  433 2447
'  353 2485
'  283 2522
'  233 2551
'  193 2571
'  163 2586
'  133 2598
'  103 2617
'   83 2631
'   63 2634
'   48 2600
'   33 2518
'   18 2476
'    3 2458
'  34727 16   0.490  12.8  -1.2 260.0  99.9  99.9 999.9 999.9  64.655 -147.093 999.9
'  739 2261
'  569 2369
'


End Sub
Sub MPreadNew(lu%, Record%)
Dim A$, i%

'         1         2         3         4         5         6         7         8
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
'  34710 16   0.327  12.6   1.4 252.6  99.9  99.9 999.9 999.9  64.666 -147.101 999.9
'  61811 31  17.248   0.0   0.0 186.3  16.6  99.9 373.6 999.9   4.082  -83.474   0.2  1.04
  
  Line Input #lu, A$
  UTsec = Left$(A$, 7)
  Nlev = Mid$(A$, 8, 3)
  Nlev1 = 33 - Nlev + 1
  Nlev2 = 33
  pALT = Mid$(A$, 11, 8)  'km
  ALTkm = pALT
  Pitch = Mid$(A$, 19, 6)
  Roll = Mid$(A$, 25, 6)
  OATmtp = Mid$(A$, 31, 6)
  Zt1 = Mid$(A$, 37, 6)
  Zt2 = Mid$(A$, 43, 6)
  Th1 = Mid$(A$, 49, 6)
  Th2 = Mid$(A$, 55, 6)
  Latitude = Mid$(A$, 61, 8)
  Longitude = Mid$(A$, 69, 9)
  LRac = Mid$(A$, 78, 6)
  MRI = Mid$(A$, 84, 6)
  If Nlev > 0 Then GoodScan = True Else GoodScan = False
  GoodTrop = True
  For i = 1 To Nlev
'1234567890
'  723 2277
    Line Input #lu, A$
    sZZZ(33 - i + 1) = Left$(A$, 6) / 1000#
    sTTT(33 - i + 1) = Mid$(A$, 7, 6)
    'Debug.Print zzz(Nlev - i + 1); TTT(Nlev - 1 + 1)
  Next i
'         1         2         3         4         5         6         7         8
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
'  60268 31  16.983   1.5  -0.7 189.0  16.9  99.9 377.6 999.9   6.726  -83.813  -1.5  0.49
' 28386 225.7 2.1 28282   479
  

'62  2110
'MJ MAHONEY (Michael.J.Mahoney@jpl.nasa.gov)
'M/S 246-102; Jet Propulsion Laboratory; Pasadena, CA 91109-8099
'WB-57 Microwave Temperature Profiler (MTP/WB-57)
'cR -AVE
'1 1
'2006 02 01  2006 08 26  20060003 {FLT DATE, REDUCTION DATE & FLIGHT NUMBER}
'0.0  0.0
'Remote sensing altitude (meters)
'Elapsed UT seconds from 0 hours on day given by DATE
'4                         {NV = number of primary variables: temp, SEtemp, Zg, #density}
'1.0   1.0    1.0 1E+21    {scale factors for primary variables: temp, SEtemp, Zg, #density}
'99999 9999 99999 99999    {missing values for primary variables: temp, SEtemp, Zg, #density}
'Retrieved air temperature (K)
'Standard error of retrieved air temperture (K)
'Geometric altitude (meters).
'Molecular air density (number per cubic meter)
'13             {number of auxiliary variables}
'1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
'99 99.999 99.9 99.9 999.9 99.9 99.9 999.9 999.9 99.999 999.999 999.9 9.99
'NX(1) is the number of altitudes in subsequent data records
'Pressure altitude of WB-57 (km)
'Aircraft Pitch(Deg)
'Aircraft Roll(Deg)
'Horizon brightness temperature (ie, OAT, similar to SAT); avg ch1 & ch2 & ch3(K)
'Tropopause #1 (km).
'Tropopause #2 (km).
'Potential temperature of tropopause #1 (K).
'Potential temperature of tropopause #2 (K).
'Latitude (Deg)
'Longitude (Deg)
'dT/dz (K/km) for 1.0 km layer centered on aircraft flight altitude.
'MRI ... a retrieval quality metric ranges 0-2, <1 is excellent
'8              {number of special comment lines}
'*** Final Data   Final Data   Final Data   Final Data   Final Data   Final Data
'*** Version 1
'***
'*** We strongly encourage anyone using the MTP data to contact us regarding it's use
'***
'*** For more information go to: http://mtp.jpl.nasa.gov/missions/have2/have2.html
'***
'*** Final Data   Final Data   Final Data   Final Data   Final Data   Final Data
'19             {number of normally included comment lines}
'Here 's a brief free-form tutorial on how to decipher the MTP data: Data groups
'   consist of the following group of lines per 15-second observing cycle.
'First line is:  UTSEC, number of retrieval levels in following table, Pressure
'   Altitude, Pitch, Roll, Outside air temp (K), tropopause altitude #1 (km),
'   tropopause altitude #2 (km) [if present], potential temperatures of tropopause
'   #1 and #2, latitude, longitude, & lapse rate near flight level.
'   The 1-liners (for each cycle) can be stripped & imported into a spreadsheet
'   for convenient plotting of trop altitude, lapse rate, etc. The tropopause
'   altitudes are calculated by cubic spline interpolation of the retrieved
'   altitudes using the WMO definition (that is, trop #1 is lowest altitude
'   where average lapse rate > -2 K/km from initial -2 K/km point to any point
'   within 2 km; trop #2 occurs above first trop after lapse rate is < -3K/km
'   for >1 km, and then first trop definition applies, possibly from within
'   the 1 km region.)
'Remaining set of lines for each cycle consist of 5 columns: col#1 is pressure
'   altitude (meters), col#2 is temperature from MTP (Kelvin), col#3 is temperature
'   error estimate (K), col#4 is geometric altitude (meters), based on GPS
'   altitude (meters), and col#5 is molecular air density [1E+21/m3].
'
'  57921  0   1.018  10.5   1.8 269.3  16.2  99.9 376.6 999.9   9.997  -84.201  -4.0  2.00

End Sub
Sub MPread(lu%, Record%)
Dim A$, i%
Static nHeader%

If Record = 0 Then
  Line Input #lu, A$
  nHeader = Left$(A$, 2)
'  Record = 1
  Do           'Read all header lines
    Record = Record + 1
    Line Input #lu, A$
    If Len(A$) < 2 Then Exit Do
'    Select Case Record%
'    Case 2
'      Pi$ = "MJ Mahoney"
'    Case 5
'      Mission$ = A$
'      Select Case Mission$
'      Case "TOTE/VOTE": Mission$ = "TOTE_VOTE"
'      Case "CRYSTAL-FACE": Mission$ = "CRYSTAL"
'      End Select
'    Case 7
'      yyyymmdd$ = Left$(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)
'    Case Is < nHeader
'    End Select
'Debug.Print Record; Len(A$)
  Loop Until Record = nHeader
  Record = 0
End If
End Sub

Sub MPwrite(REFlu%, GoodScansOnly As Boolean, WriteBadHeaders As Boolean, f As Form)
Dim Ext$, MPheader$, MPdata3$, MPdata5$, V!(1 To 20), LocalOpen As Boolean
Dim Filename$, MPlu%, HDRfile$, HDRfileOld$, L%, x!, A$, dZp!, dZg!, Discard As Boolean, dALT!, MRIthreshold!
Dim UTsecLast&, DiscardMRI As Boolean

  If REFlu% = 0 Then
    Filename$ = Path$ + AC$ + YYYYMMDD$ + ".ERF"
    REFopen Filename$
    LocalOpen = True
  End If
  
  MPheader$ = " #####0 #0 ##0.000 ##0.0 ##0.0 ##0.0 ##0.0 ##0.0 ##0.0 ##0.0 ##0.000 ###0.000 ##0.0 #0.00 #00.00 #0.00"
' Rexample$ = "  52900 33   0.064  -0.8   0.2 298.3  11.2  99.9 303.4 325.8  29.612  -95.166 999.9  2.00 200.05 15.67"
  MPdata3$ = " ###0 ###0 #0"
  MPdata5$ = " ###0 ###0 #0 ####0 ####0"
  If Mission$ = "TexAQS" Then MPdata5$ = " ####0 ###0.0 000.0 0.0 ####0 000.0"
     
  If f.chkDiscard.Value = 1 Then Discard = True Else Discard = False
  dALT = Val(f.txtDiscard.Text)
  If f.chkMRIedit.Value = 1 Then DiscardMRI = True Else DiscardMRI = False
  MRIthreshold = Val(f.txtMPmri.Text)
     
' Will read & modify header after first assigning MP output file
  MPlu% = FreeFile
  If Mission$ = "TCSP" Or Mission$ = "CAMEX4" Then
    MPfile$ = MPpath$ + Mission$ + "_MTP_" + YYYYMMDD$ + "_MP.txt"
  End If
  
  Open MPfile$ For Output As #MPlu%
  
  HDRfile$ = fSetupFileName("HDR")
  Call UpdateMPheader(HDRfile$, MPlu%)
  
' Write Limits Record
  Record% = 0
  Do
    Record% = Record% + 1
    If Record > f.ProgressBar1.Max Then Exit Do
    f.txtRecord.Text = Str(Record)
    f.ProgressBar1.Value = Record
    Call REFread(REFlu%, Record%)
    If Not EOF(REFlu) Then
      If Not GoodScansOnly Or GoodScan Or WriteBadHeaders Then GoSub WriteCycle
    End If
    DoEvents
  Loop Until EOF(REFlu%)
  
  Close (MPlu%)
  If LocalOpen Then Close (REFlu%)
  
  Exit Sub

WriteCycle:
' NB only come here on GoodScan = True  (get rid of checks for goodscan)
  If EditNlev And (Nlev < NlevMin Or Nlev > NlevMax) Then
    EditWord = EditWord Or 128: GoodScan = False ' Determine total # of acceptable levels; throw out if <7 levels
    NlevPC = NlevPC + 1
  End If
  If Nlev1 = 0 Then Nlev1 = 1
  If pALT < zzz(Nlev1) / 100 Then EditWord = EditWord Or 1: RetAltPC = RetAltPC + 1: GoodScan = False
  If UTsec > 63000 And GoodScan Then
  Nlev = Nlev
  End If
' Update the number of valid levels
  V(1) = UTsec: V(2) = Nlev: V(3) = pALT: V(4) = Pitch: V(5) = Roll: V(6) = OATmtp
  V(7) = Zt1: V(8) = Zt2: V(9) = Th1: V(10) = Th2: V(11) = Latitude: V(12) = Longitude
  V(13) = LRac: V(14) = MRI: V(15) = Tcp: V(16) = Zcp
  If WriteBadHeaders And Not GoodScan Then V(2) = 0
  If Discard And pALT < dALT Then V(2) = 0
  If DiscardMRI And MRI > MRIthreshold Then V(2) = 0
  
  If (MakeWord And 32) Or Not GoodTrop Then  'RAWbad cycles formatted for MP files
    V(7) = 99.9: V(8) = 99.9: V(9) = 999.9: V(10) = 999.9
  End If
  
  If GoodScan Or WriteBadHeaders Or (Discard And pALT < dALT) Or (DiscardMRI And MRI > MRIthreshold) Then
  ' Check to make sure time series is monotonic
    If UTsec > UTsecLast Then
      Print #MPlu%, fUsing$(MPheader$, V!())
    Else
'      MsgBox "UTsec<UTsecLast: " + Format(UTsec, "000000") + " < " + Format(UTsecLast, "000000"), vbOKOnly
      Return
    End If
    'Debug.Print fUsing$(MPheader$, V!())
    If Not (Discard And pALT < dALT) And Not (DiscardMRI And MRI > MRIthreshold) Then
      If Not GoodScansOnly Or GoodScan Then
      dZp = 1000 * (pALT - sZZZ(16))
      'dZg = 1000 * gALT - Zgeo(16) * 10
        For L = Nlev2 To Nlev1 Step -1  'top down
          Select Case Mission$
          Case "WAM", "CIREX", "ACCENT", "ACCENT2"
            V(1) = sZZZ(L): V(2) = sTTT(L): V(3) = sTSE(L)
            Print #MPlu%, fUsing$(MPdata3$, V())
          Case Else
'           x = sZZZ(L) / 100    ' NrDen = NrDensity(L) / 1E+21
'           If ND(L) = 9999 Then Zgeo(L) = 999.99: NrDen = 99999
'           If X > 0.1 And dZg(L) <> 9999 Then
        
            If sTSE(L) > 9.9 Then sTSE(L) = 9.9  'Avoid formats running together
            If Mission$ = "TexAQS" Then
              V(1) = sZZZ(L)             'meters, pressure altitude
              A$ = fUsingN("####0", V(1), 6)
              V(2) = fZtoP(V(1))         'hPa, pressure altitude
              A$ = A$ + fUsingN("###0.0", V(2), 7)
              V(3) = sTTT(L)             'K, temperature
              A$ = A$ + fUsingN("000.0", V(3), 6)
              V(4) = sTSE(L)             'K, temperature error
              A$ = A$ + fUsingN("0.0", V(4), 4)
              V(5) = sZg(L)              'meters, geometric altitude
              A$ = A$ + fUsingN("####0", V(5), 6)
              V(6) = fTheta(sTTT(L), fZtoP(sZZZ(L)))  'K, Theta
              A$ = A$ + fUsingN("000.0", V(6), 6)
              Print #MPlu, A$
            Else
'             v(1) = zzz(L): v(2) = TTT(L): v(3) = TSE(L): v(4) = Zgeo(L): v(5) = ND(L)
'             Print #MPlu%, fUsing$(MPdata5$, v())
          
              V(1) = sZZZ(L) * 1000#       'meters, pressure altitude
              A$ = fUsingN("####0", V(1), 6)
              V(2) = sTTT(L)               'K, temperature
              'If L = 16 And sZZZ(L) > 15 Then
              'V(2) = OATmtp
              'End If
              A$ = A$ + fUsingN("000.0", V(2), 6)
              V(3) = sTSE(L)               'K, temperature error
              A$ = A$ + fUsingN("0.0", V(3), 4)
              V(4) = sZg(L) * 1000#         'meters, geometric altitude
              A$ = A$ + fUsingN("####0", V(4), 6)
              V(5) = sND(L)
              A$ = A$ + fUsingN("####0", V(5), 6)
              Print #MPlu, A$
            End If
          'End If
          End Select
        'End If
        Next L
      End If
    End If
  End If
  UTsecLast = UTsec
Return

End Sub

Sub CheckLimits(Record%)
Dim i%, x!, y!
' Collect statistics for LIMITS record
  If Nlev < 99 And Nlev > NretMax Then NretMax = Nlev 'Added for GTE
  If Nlev < 99 And Nlev < NretMin Then NretMin = Nlev
  If OK Then GoodRecords = GoodRecords + 1 Else Badrecords = Badrecords + 1
  If UTsec < UTmin Then UTmin = UTsec
  If UTsec > UTmax Then UTmax = UTsec
  If pALT < 99.9 And pALT > pALTmax Then pALTmax = pALT
  If pALT < 99.9 And pALT < pALTmin Then pALTmin = pALT
  If gALT < 99.9 And gALT > gALTmax Then gALTmax = gALT
  If gALT < 99.9 And gALT < gALTmin Then gALTmin = gALT
  If Pitch < 99.9 And Pitch < Pitchmin Then Pitchmin = Pitch
  If Pitch < 99.9 And Pitch > PitchMax Then PitchMax = Pitch
  If Roll < 99.9 And Roll < Rollmin Then Rollmin = Roll
  If Roll < 99.9 And Roll > RollMax Then RollMax = Roll
  If Tac < 999.9 And Tac < Tacmin Then Tacmin = Tac
  If Tac < 999.9 And Tac > Tacmax Then Tacmax = Tac
  If Zt1 < 99.9 And Zt1 < Zt1min Then Zt1min = Zt1
  If Zt1 < 99.9 And Zt1 > Zt1max Then Zt1max = Zt1
  If Zt2 < 99.9 And Zt2 < Zt2min Then Zt2min = Zt2
  If Zt2 < 99.9 And Zt2 > Zt2max Then Zt2max = Zt2
  If Th1 < 999.9 And Th1 > PT1max Then PT1max = Th1
  If Th1 < 999.9 And Th1 < PT1min Then PT1min = Th1
  If Th2 < 999.9 And Th2 > PT2max Then PT2max = Th2
  If Th2 < 999.9 And Th2 < PT2min Then PT2min = Th2
  If Th1 < 900 Then TT1 = fKelvin(Th1, fZtoP(Zt1)) Else TT1 = 999.9
  If TT1 < 900 And TT1 < T1min Then T1min = TT1
  If TT1 < 900 And TT1 > T1max Then T1max = TT1
  If Th2 < 900 Then TT2 = fKelvin(Th2, fZtoP(Zt2)) Else TT2 = 999.9
  If TT2 < 900 And TT2 < T2min Then T2min = TT2
  If TT2 < 900 And TT2 > T2max Then T2max = TT2
  If Latitude < 999.9 And Latitude < LATmin Then LATmin = Latitude
  If Latitude < 999.9 And Latitude > LATmax Then LATmax = Latitude
  If Longitude < 999.9 And Longitude < LONmin Then LONmin = Longitude
  If Longitude < 999.9 And Longitude > LONmax Then LONmax = Longitude
  If LRac < 99.9 And LRac < LRmin Then LRmin = LRac
  If LRac < 99.9 And LRac > LRmax Then LRmax = LRac
  If MRI < 99.9 And MRI < MRImin Then MRImin = MRI
  If MRI < 99.9 And MRI > MRImax Then MRImax = MRI
    
'  If Record% Mod 10 = 0 Then
  If Nlev1 > 0 Then
    For i = Nlev1 To Nlev2
      x = sZZZ(i) '* VSCAL0
      If x < Zmin Then Zmin = x
      If x > Zmax Then Zmax = x
      y = sTTT(i) '* VSCAL1
      If y < Tmin Then Tmin = y: zTmin = x
      If y > Tmax Then Tmax = y: ZtMax = x
      x = sTSE(i) '* VSCAL2
      If x < 9.9 And x < TSEmin Then TSEmin = x
      If x < 9.9 And x > TSEmax Then TSEmax = x
      x = sZg(i) '* VSCAL3
      If x < 99.9 And x < zgeomin Then zgeomin = x
      If x < 99.9 And x > zgeomax Then zgeomax = x
      x = sND(i) '* VSCAL4
      If x < 99999 And x < NDmin Then NDmin = x
      If x > -99999 And x > NDmax Then NDmax = x
    Next i
'Debug.Print Tmin; Tmax; Zmin; Zmax; Record%
    PTmin = fTheta(Tmin, fZtoP(zTmin))
    PTmax = fTheta(Tmax, fZtoP(ZtMax))
End If

End Sub

Sub CheckLimitsInit()
' Initialize LIMITS variables
  NV = 4
  VSCAL(0) = VSCAL0: VSCAL(1) = VSCAL1: VSCAL(2) = VSCAL2
  VSCAL(3) = VSCAL3: VSCAL(4) = VSCAL4
  
  NretMin = 99: NretMax = 0
  GoodRecords = 0: Badrecords = 0
  UTmin = 100000: UTmax = 0
  pALTmin = 999.9: pALTmax = -10
  Pitchmin = 90: PitchMax = -90
  Rollmin = 90: RollMax = -90
  Tacmin = 400: Tacmax = 0
  Zt1min = 99.9: Zt1max = 0
  Zt2min = 99.9: Zt2max = 0
  PT1min = 600: PT1max = 0
  PT2min = 600: PT2max = 0
  T1min = 400: T1max = 0
  T2min = 400: T2max = 0
  LATmin = 90: LATmax = -90
  LONmin = 180: LONmax = -180
  LRmin = 99: LRmax = -99
  Zmin = 99: Zmax = 0
  Tmin = 999: Tmax = 0
  zTmin = 99: ZtMax = 0
  TSEmin = 99: TSEmax = 0
  zgeomin = 9999: zgeomax = 0
  NDmin = 99999: NDmax = 0
  MRImin = 99: MRImax = 0
  gALTmin = 999.9: gALTmax = -10
  
End Sub

Function fReadFirstNumber(lu%) As Long
Dim A$, i%

Line Input #lu, A$
i = InStr(1, A$, " ", vbTextCompare)
If i = 0 Then
  fReadFirstNumber = Val(A$)
Else
  fReadFirstNumber = Val(Left$(A$, i - 1))
End If
End Function

Sub REFread32(lu%, Record%)
Dim A As REFrecord, i%, j%, x!
' Record 1 is FI record
' Record 2 is Limits record
' Record 3, 4 and 5 are spares

  Get #lu%, Record% + HiddenRecords, A
  
  GoodScan = A.GoodScan
  GoodTrop = A.GoodTrop
  Cycle = A.Cycle
  'Debug.Print Cycle; GoodScan; GoodTrop
  Nlev = A.Nlev
  Nlev1 = A.Nlev1
  Nlev2 = A.Nlev2
  MakeWord = A.MakeWord
  EditWord = A.EditWord
  UTsec = A.UTsec
  UTsecMTP = A.UTsecMTP
  UTsecNav = A.UTsecNav
  pALT = A.pALT
  gALT = A.gALT
  rALT = A.rALT
  Pitch = A.Pitch
  Roll = A.Roll
  Latitude = A.Latitude
  Longitude = A.Longitude
  Heading = A.Heading
  TAS = A.TAS
  Wspd = A.Wspd
  Wdir = A.Wdir
  Zt1 = A.Zt1
  TT1 = A.TT1
  Th1 = A.Th1
  SEp1 = A.SEp1
  SEm1 = A.SEm1
  Zt2 = A.Zt2
  TT2 = A.TT2
  Th2 = A.Th2
  SEp2 = A.SEp2
  SEm2 = A.SEm2
  OATmtp = A.OATmtp
  OATnav = A.OATnav
  OATmms = A.OATmms
  LRac = A.LRac
  LRac2 = A.LRac2
  LRac3 = A.LRac3
  Zice = A.Zice
  dTice = A.dTice
  Elcor = A.Elcor
  ElCorUsed = A.ElCorUsed
  MRI = A.MRI
  ZtIPV = A.ZtIPV
  IPVt = A.IPVt
  StatusBits = A.StatusBits
  Tmin5 = A.Tmin5
  Zmin5 = A.Zmin5
  RCindex1 = A.RCindex1
  RCindex2 = A.RCindex2
  
  g(1) = A.g1
  g(2) = A.g2
  g(3) = A.g3
  Geqn(1) = A.G1eqn
  Geqn(2) = A.G2eqn
  Geqn(3) = A.G3eqn
  Gnd(1) = A.G1nd
  Gnd(2) = A.G2nd
  Gnd(3) = A.G3nd
  Goat(1) = A.G1oat
  Goat(2) = A.G2oat
  Goat(3) = A.G3oat
  'Debug.Print Record; Goat(1); Goat(2); Goat(3)
  For i% = 1 To Channels
    RFImask(i) = A.RFImask(i)
    For j% = 1 To 10: TA(i%, j%) = A.TA(i%, j%):  Next j%
  Next i%
  For i% = 1 To 33
    zzz(i%) = A.zzz(i%)
    TTTT(i%) = A.TTTT(i%)
    '2003.09.10 mjm increased accuracy of T to 0.01 K
    'Replace TTT by TTTT in data structure so TTT could be made real so that
    'TTT could be /10 here if necessary and also /10 everywhere that it is used
    If TTTT(i) > 3200 Then TTT(i) = TTTT(i) / 10# Else TTT(i) = TTTT(i)
    'Debug.Print i%; zzz(i%); TTT(i%)
    TSE(i%) = A.TSE(i%)
    Zgeo(i%) = A.Zgeo(i%)
    ND(i%) = A.ND(i%)
    sZZZ(i%) = zzz(i%) / 100# 'new real numbers
    sTTT(i%) = TTT(i%) / 10#
    sZg(i%) = Zgeo(i) / 100#
    sTSE(i%) = TSE(i) / 10#
    sND(i%) = ND(i)
  Next i%
  
  For i% = 1 To Channels
    For j% = 1 To 12
      C(i%, j%) = A.Counts(i%, j%)
      CMA(i, j) = A.CMA(i, j)
    Next j%
  Next i%
  
  Muxs(0) = A.Muxs(0)
  For i% = 1 To 16
    Muxs(i%) = A.Muxs(i%)
    Mux(i%) = A.Mux(i%)
  Next i%
  ' Muxs(i) Parameter
  ' 00 Ttgt
  ' 01 Tnd
  ' 02 Tlo1
  ' 03 Tlo2
  ' 04 Tifa
  ' 05 Tlo
  ' 06 Thi
  ' 07 ACC+
  ' 08 Twin
  ' 09 Tmtr
  ' 10
  ' 11 Vref
  ' 12 Tdc1
  ' 13 Tdc2
  ' 14 PS5
  ' 15 PS12
  ' 16 ACC-
  Ttgt = A.Muxs(0)            'Either tgt lo or tgt hi
  Tnd = A.Muxs(1)
  Tlo1 = A.Muxs(2)
  Tlo2 = A.Muxs(3)
  Tifa = A.Muxs(4)
  Ttgtlo = A.Muxs(5)
  Ttgthi = A.Muxs(6)
  ACCp = A.Muxs(7)
  Twin = A.Muxs(8)
  Tmtr = A.Muxs(9)
  'asterisk = a.Muxs(10)
  Vref = A.Muxs(11)
  Tdc1 = A.Muxs(12)  '11 on DC8
  Tdc2 = A.Muxs(13)  '12 on DC8
  Vps5 = A.Muxs(14)
  Vps12 = A.Muxs(15)
  ACCm = A.Muxs(16)

  dACC = ACCp - ACCm             'Peak positive acceleration minus peak negative acceleration
  For i = 1 To Channels
    dND(i) = C(i, 11) - C(i, 12)
    CN(i) = dND(i)
    CB(i) = C(i, 12)                   'Channel i base counts
    CS(i, LocHor) = C(i, LocHor)
  Next i
  
  dACC = ACCp - ACCm             'Peak positive acceleration minus peak negative acceleration
  
  Tac = OATmtp
  ALTkm = pALT
End Sub

Sub REFread33(lu%, Record%)
Dim A As REF2record, i%, j%, x!
' Record 1 is FI record
' Record 2 is Limits record
' Record 3, 4 and 5 are spares

  Get #lu%, Record% + HiddenRecords, A
  
  GoodScan = A.GoodScan
  GoodTrop = A.GoodTrop
  Cycle = A.Cycle
  'Debug.Print Cycle; GoodScan; GoodTrop
  Nlev = A.Nlev
  Nlev1 = A.Nlev1
  Nlev2 = A.Nlev2
  TTO = A.TTO
  TTO2 = A.TTO2
  TTO3 = A.TTO3
  MakeWord = A.MakeWord
  EditWord = A.EditWord
  UTsec = A.UTsec
  UTsecMTP = A.UTsecMTP
  UTsecNav = A.UTsecNav
  pALT = A.pALT
  gALT = A.gALT
  rALT = A.rALT
  mALT = A.mALT
  Pitch = A.Pitch
  Roll = A.Roll
  Latitude = A.Latitude
  Longitude = A.Longitude
  Heading = A.Heading
  TAS = A.TAS
  Wspd = A.Wspd
  Wdir = A.Wdir
  Zt1 = A.Zt1
  TT1 = A.TT1
  Th1 = A.Th1
  SEp1 = A.SEp1
  SEm1 = A.SEm1
  Zt2 = A.Zt2
  TT2 = A.TT2
  Th2 = A.Th2
  SEp2 = A.SEp2
  SEm2 = A.SEm2
  Tcp = A.Tcp
  Zcp = A.Zcp
  OATmtp = A.OATmtp
  OATnav = A.OATnav
  OATmms = A.OATmms
  LRac = A.LRac
  LRac2 = A.LRac2
  LRac3 = A.LRac3
  Zice = A.Zice
  dTice = A.dTice
  Elcor = A.Elcor
  ElCorUsed = A.ElCorUsed
  MRI = A.MRI
  ZtIPV = A.ZtIPV
  IPVt = A.IPVt
  StatusBits = A.StatusBits
  Tmin5 = A.Tmin5
  Zmin5 = A.Zmin5
  RCindex1 = A.RCindex1
  RCindex2 = A.RCindex2
  TwoAAprofiles = A.TwoAAprofiles
  If A.Algorithm > 0 Then Algorithm = A.Algorithm
  Stats(1, 1) = A.Stats(1, 1)
  Stats(2, 1) = A.Stats(2, 1)
  Stats(3, 1) = A.Stats(3, 1)
  Stats(1, 2) = A.Stats(1, 2)
  Stats(2, 2) = A.Stats(2, 2)
  Stats(3, 2) = A.Stats(3, 2)
  
  g(1) = A.g1
  g(2) = A.g2
  g(3) = A.g3
  Geqn(1) = A.G1eqn
  Geqn(2) = A.G2eqn
  Geqn(3) = A.G3eqn
  Gnd(1) = A.G1nd
  Gnd(2) = A.G2nd
  Gnd(3) = A.G3nd
  Goat(1) = A.G1oat
  Goat(2) = A.G2oat
  Goat(3) = A.G3oat
  'Debug.Print Record; Goat(1); Goat(2); Goat(3)
  For i% = 1 To Channels
    RFImask(i) = A.RFImask(i)
    For j% = 1 To 10: TA(i%, j%) = A.TA(i%, j%):  Next j%
  Next i%
  For i% = 1 To 33
    sZZZ(i%) = A.sZZZ(i%)
    sTTT(i%) = A.sTTT(i%)
    sTSE(i%) = A.sTSE(i%)
    sZg(i%) = A.sZg(i%)
    sND(i%) = A.sND(i%)
    'Debug.Print i; sZZZ(i); sTTT(i)
  Next i%
  
  For i% = 1 To Channels
    For j% = 1 To Ncts
      C(i%, j%) = A.Counts(i%, j%)
      CMA(i, j) = A.CMA(i, j)
    Next j%
  Next i%
  
  Muxs(0) = A.Muxs(0)
  For i% = 1 To 32
    Muxs(i%) = A.Muxs(i%)
    Mux(i%) = A.Mux(i%)
  Next i%
' Mapping between old mux parameters and new NGV parameters
' New    Old
'        Vps12
'        ACCm
' 00 Ttgt = Ttgt
' 01 Vm15 =      = Analog circuits on controller board and in receiver
' 02 Vp05 = Vps5 = +5V logic (there is a second +5V to T control boards)
' 03 Vp15 =      = Controller board (IF and LO amplifiers)
' 04 Vsyn =      = 24V for LO Synthesizer
' 05 Vmtr =      = 24V for Stepper Motor
' 06 Vp08 =      = Regulated +15 V for MUX
' 07 Vvid =      = Video signal
' 08 Vm08 =      = Regulated -15 V for MUX
' 09 Tdat =      = T data controller board
' 10 Tair =      = T fairing air
' 11 Tsmp =      = T scan motor plate
' 12 Tpsp = Tdc1 = T power supplies
' 13 Tnc  = Tdc2 = Tcpu
' 14 Tsyn = Tlo2
' 15 Tmtr = Tmtr
' 16 ACCp = ACCp = for now. Later will add firmware to calculate max and min
' 17 R350 =
' 18 Ttg1 = Ttgtlo
' 19 Ttg2 = Ttgthi
' 20 Twin = Twin
' 21 Tmix = Tifa
' 22 Tamp = Tlo1
' 23 Tnd  = Tnd
' 24 R600 =
' Muxs(i) Parameter
  ' 00 Ttgt
  ' 01 Tnd
  ' 02 Tlo1
  ' 03 Tlo2
  ' 04 Tifa
  ' 05 Tlo
  ' 06 Thi
  ' 07 ACC+
  ' 08 Twin
  ' 09 Tmtr
  ' 10
  ' 11 Vref
  ' 12 Tdc1 Power Supplies
  ' 13 Tdc2 CPU
  ' 14 PS5
  ' 15 PS12
  ' 16 ACC-
  If Left$(SU$, 3) <> "CAN" Then
    Ttgt = A.Muxs(0)            'Either tgt lo or tgt hi
    Tnd = A.Muxs(1)
    Tlo1 = A.Muxs(2)
    Tlo2 = A.Muxs(3)
    Tifa = A.Muxs(4)
    Ttgtlo = A.Muxs(5)
    Ttgthi = A.Muxs(6)
    ACCp = A.Muxs(7)
    Twin = A.Muxs(8)
    Tmtr = A.Muxs(9)
    'asterisk = a.Muxs(10)
    Vref = A.Muxs(11)
    Tdc1 = A.Muxs(12)  '11 on DC8
    Tdc2 = A.Muxs(13)  '12 on DC8
    Vps5 = A.Muxs(14)
    Vps12 = A.Muxs(15)
    ACCm = A.Muxs(16)
    dACC = ACCp - ACCm             'Peak positive acceleration minus peak negative acceleration
Else
    Ttgt = A.Muxs(0)            'Either tgt lo or tgt hi
    Vm08 = A.Muxs(1)
    Vvid = A.Muxs(2)
    Vp08 = A.Muxs(3)
    Vmtr = A.Muxs(4)
    Vsyn = A.Muxs(5)
    Vp15 = A.Muxs(6)
    Vp05 = A.Muxs(7)
    Vm15 = A.Muxs(8)
    ACCp = A.Muxs(9)
    Tdat = A.Muxs(10)
    Tmtr = A.Muxs(11)
    Tair = A.Muxs(12)
    Tsmp = A.Muxs(13)
    Tpsp = A.Muxs(14)
    Tnc = A.Muxs(15)
    R350 = A.Muxs(16)
    Tsyn = A.Muxs(17)
    Ttg1 = A.Muxs(18)
    Ttg2 = A.Muxs(19)
    Twin = A.Muxs(20)
    Tmix = A.Muxs(21)
    Tamp = A.Muxs(22)
    Tnd = A.Muxs(23)
    R600 = A.Muxs(24)
    dACC = ACCp
  End If
  
  For i = 1 To Channels
    dND(i) = C(i, 11) - C(i, 12)
    CN(i) = dND(i)
    CB(i) = C(i, 12)                   'Channel i base counts
    CS(i, LocHor) = C(i, LocHor)
  Next i
  
  dACC = ACCp - ACCm             'Peak positive acceleration minus peak negative acceleration
  
  Tac = OATmtp
  ALTkm = pALT
End Sub

Sub REFreadLIMITS(FQFN$)
Dim i%, lu%
  
  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF)
  Get #lu%, 2, RLR
  Close lu
  
  NV% = RLR.NV
  For i% = 0 To NV%: VSCAL(i%) = RLR.VSCAL(i%): Next i%
  
  UTmin = RLR.UTmin:   UTmax = RLR.UTmax
  UTtakeoff = RLR.UTtakeoff:  UTlanding = RLR.UTlanding
  GoodRecords = RLR.Mins(1):  Badrecords = RLR.Maxs(1)
  NretMin = RLR.Mins(2): NretMax = RLR.Maxs(2)
  pALTmin = RLR.Mins(3): pALTmax = RLR.Maxs(3)
  Pitchmin = RLR.Mins(4): PitchMax = RLR.Maxs(4)
  Rollmin = RLR.Mins(5): RollMax = RLR.Maxs(5)
  Tacmin = RLR.Mins(6): Tacmax = RLR.Maxs(6)
  Zt1min = RLR.Mins(7): Zt1max = RLR.Maxs(7)
  Zt2min = RLR.Mins(8): Zt2max = RLR.Maxs(8)
  T1min = RLR.Mins(9): T1max = RLR.Maxs(9)
  T2min = RLR.Mins(10): T2max = RLR.Maxs(10)
  PT1min = RLR.Mins(11): PT1max = RLR.Maxs(11)
  PT2min = RLR.Mins(12): PT2max = RLR.Maxs(12)
  LATmin = RLR.Mins(13): LATmax = RLR.Maxs(13)
  LONmin = RLR.Mins(14): LONmax = RLR.Maxs(14)
  LRmin = RLR.Mins(15): LRmax = RLR.Maxs(15)
  Zmin = RLR.Mins(16): Zmax = RLR.Maxs(16)
  Tmin = RLR.Mins(17): Tmax = RLR.Maxs(17)
  zTmin = RLR.Mins(18): ZtMax = RLR.Maxs(18)
  PTmin = RLR.Mins(19): PTmax = RLR.Maxs(19)
  TSEmin = RLR.Mins(20): TSEmax = RLR.Maxs(20)
  zgeomin = RLR.Mins(21): zgeomax = RLR.Maxs(21)
  NDmin = RLR.Mins(22): NDmax = RLR.Maxs(22)
  MRImin = RLR.Mins(23): MRImax = RLR.Maxs(23)
  gALTmin = RLR.Mins(24): gALTmax = RLR.Maxs(24)
  UTksmin = RLR.Mins(25): UTksmax = RLR.Maxs(25)
  Ztgm = RLR.Ztgm                    'Altitude of global minimum T
  Tzgm = RLR.Tzgm                    'Global minimum T for flight above 5 km
  Thgm = RLR.Thgm                    'Theta at global minimym T
  UTgm = RLR.UTgm                    'UT at time of global minimum T
'
'  Summary of Limits Information in Record 3 of every REF file
'
'  UTmin and UTmax                'UT range [sec]
'  GoodRecords and  Badrecords    'Number of good and bad records
'  NretMin and NretMax            'Range of retrieved levels
'  pALTmin and pALTmax            'Range of aircraft pressure altitudes [km]
'  Pitchmin and Pitchmax          'Range of aircraft pitch [deg]
'  Rollmin and Rollmax            'Range of aircraft roll [deg]
'  Tacmin and Tacmax              'Range of aircraft outside air temperatures [K]
'  Zt1min and Zt1max              'Range of tropopause 1 altitudes [km]
'  Zt2min and Zt2max              'Range of tropopause 2 altitudes [km]
'  T1min and T1max                'Range of tropopause 1 temperatures [K]
'  T2min and T2max                'Range of tropopause 1 temperatures [K]
'  PT1min and PT1max              'Range of tropopause 1 potential temperatures [K]
'  PT2min and PT2max              'Range of tropopause 2 potential temperatures [K]
'  LATmin and LATmax              'Latitude range covered during flight
'  LONmin and LONmax              'Longitude range covered during flight
'  LRmin and LRmax                'Range of lapse rates measured at aircraft during flight [K/km]
'  Zmin and Zmax                  'Range of retrieved altitudes during flight [km]
'  Tmin and Tmax                  'Range of retrieved temperatures during flight [km]
'  zTmin and zTmax                'Altitudes at Tmin and Tmax [km]
'  PTmin and PTmax                'Potential temperatures at Tmin and Tmax [K]
'  TSEmin and TSEmax              'Range of errors on tropopause altitudes [km]
'  zgeomin and zgeomax            'Range of geometric altitudes corresponding to Zmin and Zmax [km]
'  NDmin and NDmax                'Range of number density
'  MRImin and MRImax
'  gALTmin and gALTmax
'  UTksmin and UTksmax              'UT Used min and max (ie no ascent/descent)
End Sub

Sub REF2readLIMITS(FQFN$)
Dim i%, lu%
  
  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF2)
  Get #lu%, 2, RLR
  Close lu
  
  NV% = RLR.NV
  For i% = 0 To NV%: VSCAL(i%) = RLR.VSCAL(i%): Next i%
  
  UTmin = RLR.UTmin:   UTmax = RLR.UTmax
  UTtakeoff = RLR.UTtakeoff:  UTlanding = RLR.UTlanding
  GoodRecords = RLR.Mins(1):  Badrecords = RLR.Maxs(1)
  NretMin = RLR.Mins(2): NretMax = RLR.Maxs(2)
  pALTmin = RLR.Mins(3): pALTmax = RLR.Maxs(3)
  Pitchmin = RLR.Mins(4): PitchMax = RLR.Maxs(4)
  Rollmin = RLR.Mins(5): RollMax = RLR.Maxs(5)
  Tacmin = RLR.Mins(6): Tacmax = RLR.Maxs(6)
  Zt1min = RLR.Mins(7): Zt1max = RLR.Maxs(7)
  Zt2min = RLR.Mins(8): Zt2max = RLR.Maxs(8)
  T1min = RLR.Mins(9): T1max = RLR.Maxs(9)
  T2min = RLR.Mins(10): T2max = RLR.Maxs(10)
  PT1min = RLR.Mins(11): PT1max = RLR.Maxs(11)
  PT2min = RLR.Mins(12): PT2max = RLR.Maxs(12)
  LATmin = RLR.Mins(13): LATmax = RLR.Maxs(13)
  LONmin = RLR.Mins(14): LONmax = RLR.Maxs(14)
  LRmin = RLR.Mins(15): LRmax = RLR.Maxs(15)
  Zmin = RLR.Mins(16): Zmax = RLR.Maxs(16)
  Tmin = RLR.Mins(17): Tmax = RLR.Maxs(17)
  zTmin = RLR.Mins(18): ZtMax = RLR.Maxs(18)
  PTmin = RLR.Mins(19): PTmax = RLR.Maxs(19)
  TSEmin = RLR.Mins(20): TSEmax = RLR.Maxs(20)
  zgeomin = RLR.Mins(21): zgeomax = RLR.Maxs(21)
  NDmin = RLR.Mins(22): NDmax = RLR.Maxs(22)
  MRImin = RLR.Mins(23): MRImax = RLR.Maxs(23)
  gALTmin = RLR.Mins(24): gALTmax = RLR.Maxs(24)
  UTksmin = RLR.Mins(25): UTksmax = RLR.Maxs(25)
  Ztgm = RLR.Ztgm                    'Altitude of global minimum T
  Tzgm = RLR.Tzgm                    'Global minimum T for flight above 5 km
  Thgm = RLR.Thgm                    'Theta at global minimym T
  UTgm = RLR.UTgm                    'UT at time of global minimum T
'
'  Summary of Limits Information in Record 3 of every REF file
'
'  UTmin and UTmax                'UT range [sec]
'  GoodRecords and  Badrecords    'Number of good and bad records
'  NretMin and NretMax            'Range of retrieved levels
'  pALTmin and pALTmax            'Range of aircraft pressure altitudes [km]
'  Pitchmin and Pitchmax          'Range of aircraft pitch [deg]
'  Rollmin and Rollmax            'Range of aircraft roll [deg]
'  Tacmin and Tacmax              'Range of aircraft outside air temperatures [K]
'  Zt1min and Zt1max              'Range of tropopause 1 altitudes [km]
'  Zt2min and Zt2max              'Range of tropopause 2 altitudes [km]
'  T1min and T1max                'Range of tropopause 1 temperatures [K]
'  T2min and T2max                'Range of tropopause 1 temperatures [K]
'  PT1min and PT1max              'Range of tropopause 1 potential temperatures [K]
'  PT2min and PT2max              'Range of tropopause 2 potential temperatures [K]
'  LATmin and LATmax              'Latitude range covered during flight
'  LONmin and LONmax              'Longitude range covered during flight
'  LRmin and LRmax                'Range of lapse rates measured at aircraft during flight [K/km]
'  Zmin and Zmax                  'Range of retrieved altitudes during flight [km]
'  Tmin and Tmax                  'Range of retrieved temperatures during flight [km]
'  zTmin and zTmax                'Altitudes at Tmin and Tmax [km]
'  PTmin and PTmax                'Potential temperatures at Tmin and Tmax [K]
'  TSEmin and TSEmax              'Range of errors on tropopause altitudes [km]
'  zgeomin and zgeomax            'Range of geometric altitudes corresponding to Zmin and Zmax [km]
'  NDmin and NDmax                'Range of number density
'  MRImin and MRImax
'  gALTmin and gALTmax
'  UTksmin and UTksmax              'UT Used min and max (ie no ascent/descent)
End Sub

Sub REF2readCALFILE(FQFN$)
Dim i%, j%, lu%

  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF2)
  Get #lu%, 3, CFR2
  Close lu
  
  Gendate$ = Trim(CFR2.Gendate)
  For i = 1 To 10
    WCTdates$(i) = Trim(CFR2.WCTdates(i))
  Next i
  UTstart = CFR2.UTstart
  UTend = CFR2.UTend
  Channels = CFR2.Channels
  Nel = CFR2.Nel
  Emissivity = CFR2.Emissivity
  Reflectivity = CFR2.Reflectivity
  DeltaTmin = CFR2.DeltaTmin
  
' "FIT_INFO"
  Nfit = CFR2.Nfit
  For i = 1 To Nfit
    NP$(i) = Trim(CFR2.NP(i))
  Next i
  
  For i = 1 To Channels
    GOF(i) = CFR2.GOF(i)
    For j = 1 To Nfit
      GEC(i, j) = CFR2.GEC(i, j)
    Next j
  Next i
  aTGToffset(0) = CFR2.aTGToffset(0)
  aMXRoffset(0) = CFR2.aMXRoffset(0)
  aNAVoffset(0) = CFR2.aNAVoffset(0)
  aNDoffset(0) = CFR2.aNDoffset(0)
  aTGToffset(1) = CFR2.aTGToffset(1)
  aMXRoffset(1) = CFR2.aMXRoffset(1)
  aNAVoffset(1) = CFR2.aNAVoffset(1)
  aNDoffset(1) = CFR2.aNDoffset(1)
  aTGToffset(2) = CFR2.aTGToffset(2)
  aMXRoffset(2) = CFR2.aMXRoffset(2)
  aNAVoffset(2) = CFR2.aNAVoffset(2)
  aNDoffset(2) = CFR2.aNDoffset(2)
  aTGToffset(3) = CFR2.aTGToffset(3)
  aMXRoffset(3) = CFR2.aMXRoffset(3)
  aNAVoffset(3) = CFR2.aNAVoffset(3)
  aNDoffset(3) = CFR2.aNDoffset(3)
  
  TGToffset = aTGToffset(0)
  MXRoffset = aMXRoffset(0)
  NAVoffset = aNAVoffset(0)
  NDoffset = aNDoffset(0)

  UseMAforCB = CFR2.UseMAforCB
  UseMAforCS = CFR2.UseMAforCS
  UseMAforCSgain = CFR2.UseMAforCSgain
  UseMAforCN = CFR2.UseMAforCN
  UseMAforTtgt = CFR2.UseMAforTtgt
  UseMAforTifa = CFR2.UseMAforTifa
  RAWextension = CFR2.RAWextension
  ScanTime = CFR2.ScanTime

' "WINDOW_CORRECTIONS"
  EnableWCT = CFR2.EnableWCT
  For i = 1 To Channels
    For j = 1 To Nel
      WINcor(i, j) = CFR2.WINcor(i, j)
    Next j
  Next i
  
' RAW counts editting criteria
  CMAcycles = CFR2.CMAcycles              'Slow -- Base, Ttgt, etc.
  CMAcycles2 = CFR2.CMAcycles2            'Fast -- Sky counts
  
  RFIthreshold = CFR2.RFIthreshold
  RFIiterations = CFR2.RFIiterations
  RFIiterations2 = CFR2.RFIiterations2
  MUXthreshold = CFR2.MUXthreshold
  BadCycles = CFR2.BadCycles
  Badcycles2 = CFR2.Badcycles2
  
' Gain Limits
  For i = 1 To Channels
    GeqnMin(i) = CFR2.GeqnMin(i)
    GeqnMax(i) = CFR2.GeqnMax(i)
    GnavMin(i) = CFR2.GnavMin(i)
    GnavMax(i) = CFR2.GnavMax(i)
    GndMin(i) = CFR2.GndMin(i)
    GndMax(i) = CFR2.GndMax(i)
  Next i
  
' Channel Weights
  For i = 1 To Channels: ChInfo(i) = CFR2.ChInfo(i):  Next i

' Fit Region
  TBfitX1 = CFR2.TBfitX1
  TBfitX2 = CFR2.TBfitX2
  TBfitY1 = CFR2.TBfitY1
  TBfitY2 = CFR2.TBfitY2

' Noise Diode Noise Temperatures
  For i = 1 To Channels
    Cnd0(i) = CFR2.Cnd0(i)
    Cnd1(i) = CFR2.Cnd1(i)
    Cnd2(i) = CFR2.Cnd2(i)
  Next i
  TrefND = CFR2.TrefND
  
  MTPyaw = CFR2.MTPyaw
  MTPpitch = CFR2.MTPpitch
  MTProll = CFR2.MTProll
  MTPfiduciary = CFR2.MTPfiduciary
  fEcCount = CFR2.fEcCount
  For i = 1 To 10
    ElSUI(i) = CFR2.fEcCount
  Next i
  
  RHS = CFR2.RHS
  LocHor = CFR2.LocHor
  Targets = CFR2.Targets
  
  NRC = CFR2.NRC
  For i = 0 To NRC - 1
    Reg(i) = CFR2.Reg(i)
    RCformat(i) = CFR2.RCformat(i)
    RCuse(i) = CFR2.RCuse(i)
  Next i
  DoEvents
  
End Sub


Sub REFwriteHEADER(FQFN$)
' Write header using the current file format

  Select Case FileFormatOut
  Case 32
    REFwriteFLTINFO FQFN$   'Record 1
    REFwriteLIMITS FQFN$    'Record 2
    REFwriteCALFILE FQFN$   'REcord 3
  
  Case 33
    REF2writeFLTINFO FQFN$  'Record 1
    REF2writeLIMITS FQFN$   'Record 2
    REF2writeCALFILE FQFN$  'REcord 3
  
  Case Else
    Call MsgBox("The FileFormat: " & Str(FileFormatOut) & " is not supported!", vbOKOnly)
  
  End Select

End Sub

Sub REFreadHEADER(FQFN$)
  
  FileFormatIn = fREFreadFileFormat(FQFN$)
  
  Select Case FileFormatIn
  Case 32
    REFreadFLTINFO FQFN$   'Record 1
    REFreadLIMITS FQFN$    'Record 2
    If EnableCalfile Then REFreadCALFILE FQFN$  'REcord 3
  
  Case 33
    REF2readFLTINFO FQFN$  'Record 1
    REF2readLIMITS FQFN$   'Record 2
    REF2readCALFILE FQFN$  'REcord 3
  
  Case Else
    Call MsgBox("The requested FileFormat:" & Str(FileFormatIn) & " is not supported!", vbOKOnly)
  
  End Select
  
End Sub
Sub REFreadFLTINFO(FQFN$)
Dim i%, lu%, iNRC%

  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF)
  Get #lu%, 1, FIR
  Close lu
  
  Mission$ = Trim(FIR.Mission)
  FileFormatIn = FIR.Fileformat
  FltNumber$ = Trim(FIR.FltNumber)
  Pi$ = FIR.Pi
  Yeer = FIR.Yeer
  Doy = FIR.Doy
  YYYYMMDD = FIR.YYYYMMDD
  FlightDate = fDate(YYYYMMDD$)
  TotalCycles = FIR.TotalCycles
  Channels = FIR.Channels
  
  Drive$ = FIR.Drive                          'eg C:
  Drive$ = "C:"  'xxxmjm
  Rdir$ = Trim(FIR.Rdir)                      'eg \DC8\
  Rdir2$ = Rdir1$ + "\MTP\Data" + Rdir$       'eg \MTP\Data\DC8\
  Platform$ = Mid$(Rdir$, 2, Len(Rdir$) - 2)  'eg DC8
  AC$ = Mid$(Rdir$, 2, 2)                     'eg DC
  Root$ = Drive$ + Rdir2$                     'eg C:\MTP\Data\DC8\
  MNpath$ = Root$ + Mission$ + "\"            'eg C:\MTP\Data\DC8\SOLVE\
  Call DirCheck(Root$, Mission$)
  RCpath$ = MNpath$ + "RC\"                   'eg C:\MTP\Data\DC8\SOLVE\RC\
  Call DirCheck(MNpath, "RC")
  MPpath$ = MNpath$ + "MP\"
  Call DirCheck(MNpath, "MP")
  Path$ = MNpath$ + YYYYMMDD$ + "\"           'eg C:\MTP\Data\DC8\SOLVE\20000120\
  Call DirCheck(MNpath, YYYYMMDD$)
  SUpath$ = MNpath$ + "Setup\"
  Call DirCheck(MNpath, "Setup")
  PathDot$ = Path$ + AC$ + YYYYMMDD$ + "."    'eg C:\MTP\Data\DC8\SOLVE\20000120\DC20000120.
  yymmdd$ = Right$(YYYYMMDD$, 6)
' Default Fully-Qualified File Names
  BINfile$ = PathDot$ + "BIN"
  CALfile$ = PathDot$ + "CAL"
  ERFfile$ = PathDot$ + "ERF"
  INPfile$ = PathDot$ + "INP"
  LOGfile$ = PathDot$ + "LOG"
  MMSfile$ = MNpath$ + "MMS\MM" + YYYYMMDD$ + "." + Platform$
  RAOBfile$ = RSpath$ + Mission$ + ".RAOB2"
  RAWfile$ = PathDot$ + "RAW"
  REFfile$ = PathDot$ + "REF"
  RTSfile$ = PathDot$ + "RTS"
  OUTfile$ = PathDot$ + "OUT"
  MPfile$ = MPpath$ + "MP" + YYYYMMDD$ + "." + Platform$
'  USEfile$ = RCpath$ + USE5$ + ".USE"
  RCs$ = Left$(AC$, 1) + "RC"
  Pgm$ = Trim(FIR.Pgm)
  USE5$ = FIR.USE5
  SU$ = Trim(FIR.SU)
  NFL = FIR.NFL
  For i = 1 To NFL: FLA(i) = FIR.FLA(i): Next i
  PgmDrive$ = FIR.PgmDrive
  
  Algorithm = FIR.Algorithm
  MRIavg = FIR.MRIavg
  MRIrms = FIR.MRIrms
  
  utMTPcor = FIR.utMTPcor
  DTavg = FIR.DTavg
  DTrms = FIR.DTrms
  
  ALTfujCONST = FIR.ALTfujCONST
  ALTfujSLOPE = FIR.ALTfujSLOPE
  OATnavCOR = FIR.OATnavCOR
  CalSource = Trim(FIR.CalSource)
  
  GainScale = FIR.GainScale
  REFsource = FIR.REFsource
  OATsource = FIR.OATsource
' OATsource: 1=MTP, 2=A/C, 3=MMS
' GainScale: 1=EQN, 2=ND,  3=OAT
' REFsource: 1=TGT, 2=OAT
  
  If GainScale = 0 Then  'Undefined, generate new definition
    Select Case CalSource$
    Case "DADStgt": GainScale = 3: REFsource = 2: OATsource = 2
    Case "GAIN_EQN": GainScale = 1: REFsource = 1: OATsource = 2
    Case "OATnav": GainScale = 3: REFsource = 1: OATsource = 2
    Case "MMS": GainScale = 3: REFsource = 1: OATsource = 3
    Case "DADS": GainScale = 3: REFsource = 1: OATsource = 2
    Case "ND": GainScale = 2: REFsource = 1: OATsource = 2
    End Select
  End If
  UseMMSpALT = FIR.UseMMSpALT
  
  LAT1 = FIR.LAT1
  LAT2 = FIR.LAT2
  LAT3 = FIR.LAT3
  LAT4 = FIR.LAT4
  UserLATs = FIR.UserLATs
  
  EnableCalfile = FIR.EnableCalfile
  DoAll = FIR.DoAll
  DoAllMask = FIR.DoAllMask

  MakeEditWord = FIR.MakeEditWord
  
  EditTropAlt = FIR.EditTropAlt
  TropAltMin = FIR.TropAltMin
  TropAltMax = FIR.TropAltMax
  TropAltPC = FIR.TropAltPC
  
  EditRetAlt = FIR.EditRetAlt
  RetAltMin = FIR.RetAltMin
  RetAltMax = FIR.RetAltMax
  RetAltPC = FIR.RetAltPC
  
  EditZtOff = FIR.EditZtOff
  ZtOffA = FIR.ZtOffA
  ZtOffB = FIR.ZtOffB
  ZtOffPC = FIR.ZtOffPC
  
  EditTemperature = FIR.EditTemperature
  TemperatureMin = FIR.TemperatureMin
  TemperatureMax = FIR.TemperatureMax
  TemperaturePC = FIR.TemperaturePC
  
  EditPitch = FIR.EditPitch
  ePitchMin = FIR.ePitchMin
  ePitchMax = FIR.ePitchMax
  PitchPC = FIR.PitchPC
  
  EditRoll = FIR.EditRoll
  eRollMin = FIR.eRollMin
  eRollMax = FIR.eRollMax
  RollPC = FIR.RollPC
  
  EditNav = FIR.EditNav
  NavMin = FIR.NavMin
  NavMax = FIR.NavMax
  NavQualPC = FIR.NavQualPC
  
  EditTA = FIR.EditTA
  TAmin = FIR.TAmin
  TAmax = FIR.TAmax
  TAqualPC = FIR.TAqualPC
  
  EditCts = FIR.EditCts
  CtsMin = FIR.CtsMin
  CtsMax = FIR.CtsMax
  CtsQualPC = FIR.CtsQualPC
  
  EditCycle = FIR.EditCycle
  CycleQualPC = FIR.CycleQualPC
  
  EditTtgt = FIR.EditTtgt
  TtgtMin = FIR.TtgtMin
  TtgtMax = FIR.TtgtMax
  TtgtPC = FIR.TtgtPC
  
  EditNlev = FIR.EditNlev
  NlevMin = FIR.NlevMin
  NlevMax = FIR.NlevMax
  NlevPC = FIR.NlevPC
  
  EditRAWbad = FIR.EditRAWbad
  RAWbadmin = FIR.RAWbadmin
  RAWbadmax = FIR.RAWbadmax
  RAWbadPC = FIR.RAWbadPC
  
  EditRate = FIR.EditRate
  RateMin = FIR.RateMin
  RateMax = FIR.RateMax
  RatePC = FIR.RatePC
  
  EditOATtrop = FIR.EditOATtrop
  OATtropPC = FIR.OATtropPC
  
  OATzt10 = FIR.OATzt10
  Tzt10 = FIR.Tzt10
  OATzt20 = FIR.OATzt20
  Tzt20 = FIR.Tzt20
  OATks10 = FIR.OATks10
  OATks20 = FIR.OATks20
  
  OATzt11 = FIR.OATzt11
  Tzt11 = FIR.Tzt11
  OATzt21 = FIR.OATzt21
  Tzt21 = FIR.Tzt21
  OATks11 = FIR.OATks11
  OATks21 = FIR.OATks21
  
  OATzt12 = FIR.OATzt12
  Tzt12 = FIR.Tzt12
  OATzt22 = FIR.OATzt22
  Tzt22 = FIR.Tzt22
  OATks12 = FIR.OATks12
  OATks22 = FIR.OATks22
  
  OATzt13 = FIR.OATzt13
  Tzt13 = FIR.Tzt13
  OATzt23 = FIR.OATzt23
  Tzt23 = FIR.Tzt23
  OATks13 = FIR.OATks13
  OATks23 = FIR.OATks23
'
  OATzt14 = FIR.OATzt14
  Tzt14 = FIR.Tzt14
  OATzt24 = FIR.OATzt24
  Tzt24 = FIR.Tzt24
  
' HISTORY information
  CALversion = FIR.CALversion
  MAKEversion = FIR.MAKEversion
  EDITversion = FIR.EDITversion
  FLTINFOversion = FIR.FLTINFOversion
  
  Tstamp = FIR.Tstamp
  RAWstamp = FIR.RAWstamp
  MMSstamp = FIR.MMSstamp
  REFstamp = FIR.REFstamp
  ERFstamp = REFstamp
  ERFstamp = FIR.ERFstamp
  CTCstamp = FIR.CTCstamp
  
  NRC = FIR.NRC
  If NRC = 0 Then NRC = 1
  If NRC > NRCmax Then NRC = NRCmax
  Reg$(0) = Reg0$
  Reg$(1) = Reg1$
  Reg$(2) = Reg2$
  If NRC > 16 Then iNRC = 16 Else iNRC = NRC
  For i = 0 To iNRC - 1
    If i < 16 Then
      If FIR.Reg(i) <> "" Then Reg(i) = FIR.Reg(i) Else Reg(i) = ""
    Else
'      If FIR.Reg(i) <> "" Then Reg(i) = FIR.RegExt(i - 15) Else Reg(i) = ""
    End If
    If i <= 10 Then
      RCformat(i) = FIR.RCformat(i)
    Else
      RCformat(i) = FIR.RCfmt(i - 10)
    End If
    If i < 16 Then
      If Abs(FIR.RCuse(i)) > 5000 Then
      RCuse(i) = 1000
      Else
      RCuse(i) = FIR.RCuse(i)
      End If
    Else
'      RCuse(i) = FIR.RCuseExt(i - 15)
    End If
  Next i
  Reg0$ = FIR.Reg(0)
  Reg1$ = FIR.Reg(1)
  Reg2$ = FIR.Reg(2)
  
  ATPrange = 20
  
End Sub

Sub REFwriteFLTINFO(FQFN$)
Dim i%, lu%, iNRC%
  ' Only allow DoAll flag to be set in default FLTINFO file!!!
  'If FQFN$ <> "C:\MTP\Setup\FLTINFO.REF" Then DoAll = False
  ReadSETUP ("SYSTEM")
  Drive$ = DataDrive$
  PgmDrive$ = ProgramDrive$

  FIR.Tstamp = Date + Time
  FIR.RAWstamp = RAWstamp
  FIR.REFstamp = REFstamp
  FIR.ERFstamp = ERFstamp
  FIR.CTCstamp = CTCstamp
  
  FIR.Mission = Mission$
  FIR.Fileformat = FileFormatOut
  FIR.FltNumber = FltNumber$
  FIR.Pi = Pi$
  FIR.Yeer = Yeer
  FIR.Doy = Doy
  FIR.YYYYMMDD = YYYYMMDD
  FIR.TotalCycles = TotalCycles
  FIR.Channels = Channels
  
  FIR.Drive = Drive$
  FIR.Rdir = Rdir$
  FIR.Path = Path$
  FIR.Pgm = Pgm$
  FIR.Reg0 = Reg$(0)
  FIR.Reg1 = Reg$(1)
  FIR.Reg2 = Reg$(2)
  FIR.USE5 = USE5$
  FIR.SU = SU$
  FIR.NFL = NFL
  For i = 1 To NFL: FIR.FLA(i) = FLA(i): Next i
  FIR.PgmDrive = PgmDrive$
  
  FIR.Algorithm = Algorithm
  FIR.MRIavg = MRIavg
  FIR.MRIrms = MRIrms
  
  FIR.utMTPcor = utMTPcor
  FIR.DTavg = DTavg
  FIR.DTrms = DTrms
  FIR.ALTfujCONST = ALTfujCONST
  FIR.ALTfujSLOPE = ALTfujSLOPE
  FIR.OATnavCOR = OATnavCOR
  FIR.CalSource = CalSource
  FIR.GainScale = GainScale
  FIR.REFsource = REFsource
  FIR.OATsource = OATsource
  
  FIR.UseMMSpALT = UseMMSpALT
  
  FIR.LAT1 = LAT1
  FIR.LAT2 = LAT2
  FIR.LAT3 = LAT3
  FIR.LAT4 = LAT4
  FIR.UserLATs = UserLATs
  
  FIR.EnableCalfile = EnableCalfile
  FIR.DoAll = DoAll
  FIR.DoAllMask = DoAllMask
  
  FIR.MakeEditWord = MakeEditWord
  
  FIR.EditTropAlt = EditTropAlt
  FIR.TropAltMin = TropAltMin
  FIR.TropAltMax = TropAltMax
  FIR.TropAltPC = TropAltPC
  
  FIR.EditRetAlt = EditRetAlt
  FIR.RetAltMin = RetAltMin
  FIR.RetAltMax = RetAltMax
  FIR.RetAltPC = RetAltPC
  
  FIR.EditZtOff = EditZtOff
  FIR.ZtOffA = ZtOffA
  FIR.ZtOffB = ZtOffB
  FIR.ZtOffPC = ZtOffPC
  
  FIR.EditTemperature = EditTemperature
  FIR.TemperatureMin = TemperatureMin
  FIR.TemperatureMax = TemperatureMax
  FIR.TemperaturePC = TemperaturePC
  
  FIR.EditPitch = EditPitch
  FIR.ePitchMin = ePitchMin
  FIR.ePitchMax = ePitchMax
  FIR.PitchPC = PitchPC
  
  FIR.EditRoll = EditRoll
  FIR.eRollMin = eRollMin
  FIR.eRollMax = eRollMax
  FIR.RollPC = RollPC
  
  FIR.EditNav = EditNav
  FIR.NavMin = NavMin
  FIR.NavMax = NavMax
  FIR.NavQualPC = NavQualPC
  
  FIR.EditTA = EditTA
  FIR.TAmin = TAmin
  FIR.TAmax = TAmax
  FIR.TAqualPC = TAqualPC
  
  FIR.EditCts = EditCts
  FIR.CtsMin = CtsMin
  FIR.CtsMax = CtsMax
  FIR.CtsQualPC = CtsQualPC
  
  FIR.EditCycle = EditCycle
  FIR.CycleQualPC = CycleQualPC
  
  FIR.EditTtgt = EditTtgt
  FIR.TtgtMin = TtgtMin
  FIR.TtgtMax = TtgtMax
  FIR.TtgtPC = TtgtPC
  
  FIR.EditNlev = EditNlev
  FIR.NlevMin = NlevMin
  FIR.NlevMax = NlevMax
  FIR.NlevPC = NlevPC
  
  FIR.EditRAWbad = EditRAWbad
  FIR.RAWbadmin = RAWbadmin
  FIR.RAWbadmax = RAWbadmax
  FIR.RAWbadPC = RAWbadPC
  
  FIR.EditRate = EditRate
  FIR.RateMin = RateMin
  FIR.RateMax = RateMax
  FIR.RatePC = RatePC

  
  FIR.EditOATtrop = EditOATtrop
  FIR.OATtropPC = OATtropPC
  
  FIR.OATzt10 = OATzt10
  FIR.Tzt10 = Tzt10
  FIR.OATzt20 = OATzt20
  FIR.Tzt20 = Tzt20
  FIR.OATks10 = OATks10
  FIR.OATks20 = OATks20
  
  FIR.OATzt11 = OATzt11
  FIR.Tzt11 = Tzt11
  FIR.OATzt21 = OATzt21
  FIR.Tzt21 = Tzt21
  FIR.OATks11 = OATks11
  FIR.OATks21 = OATks21
  
  FIR.OATzt12 = OATzt12
  FIR.Tzt12 = Tzt12
  FIR.OATzt22 = OATzt22
  FIR.Tzt22 = Tzt22
  FIR.OATks12 = OATks12
  FIR.OATks22 = OATks22
  
  FIR.OATzt13 = OATzt13
  FIR.Tzt13 = Tzt13
  FIR.OATzt23 = OATzt23
  FIR.Tzt23 = Tzt23
  FIR.OATks13 = OATks13
  FIR.OATks23 = OATks23
'
  FIR.OATzt14 = OATzt14
  FIR.Tzt14 = Tzt14
  FIR.OATzt24 = OATzt24
  FIR.Tzt24 = Tzt24
  
' HISTORY information
  FIR.CALversion = CALversion
  FIR.MAKEversion = MAKEversion
  FIR.EDITversion = EDITversion
  FLTINFOversion = FileDateTime(ProgramDrive$ + "\MTP\VB6\BAS\FLTINFO.bas") 'FLTINFO version
  FIR.FLTINFOversion = FLTINFOversion
    
  FIR.Tstamp = Tstamp
  FIR.RAWstamp = RAWstamp
  FIR.MMSstamp = MMSstamp
  FIR.REFstamp = REFstamp
  FIR.ERFstamp = ERFstamp
  FIR.CTCstamp = CTCstamp
  
  FIR.NRC = NRC
  If NRC > 16 Then iNRC = 16 Else iNRC = NRC
  For i = 0 To iNRC - 1
    FIR.Reg(i) = Reg(i)
    If i <= 10 Then
      FIR.RCformat(i) = RCformat(i)
    Else
      FIR.RCfmt(i - 10) = RCformat(i)
    End If
    FIR.RCuse(i) = RCuse(i)
  Next i
  
  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF)
  Put #lu%, 1, FIR
  Close lu
  
End Sub

Sub ReadSETUP(Program$)
  Dim i0%, i1%, i2%, lu%, cMd$, V$, FixPath As Boolean

  If Len(Dir("C:\MTP\Setup\MTPsetup.INI")) = 0 Then
    MsgBox "Unable to open C:\MTP\Setup\MTPsetup.INI", vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open "C:\MTP\Setup\MTPsetup.INI" For Input As lu%
  FIsize% = 0
  FixPath = False
  Do
NextCategory:
    If EOF(lu%) Then GoTo Exit_Sub

    FIsize% = FIsize% + 1
    Input #lu%, cMd$                       'Read a line
  
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Right Bracket not found. Fix line number " + Str(FIsize%), vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Program$ Then    'Read setup info for only the requested program
        Select Case Mid$(cMd$, 2, i0% - 2)
        Case "SYSTEM"
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
              Case "ProgramDrive$":  ProgramDrive$ = V$
              Case "DataDrive$":  DataDrive$ = V$
              Case "UID$": UID$ = V$
              Case "DefaultRTmode"
                DefaultRTmode = Val(V$)
                If DefaultRTmode = 0 Then RealTime = False Else RealTime = True
              Case "DataSourceMode"
                DataSourceMode = Val(V$)
              Case "DataDestinationMode"
                DataDestinationMode = Val(V$)
              Case "DataDestinationMode2"
                DataDestinationMode2 = Val(V$)
              Case "AlternateRoot"
                If V$ = """""" Then Rdir1$ = "" Else Rdir1$ = Val(V$)
              Case "UseDefaultRCs"
                If Val(V$) = 1 Then UseDefaultRCs = True Else UseDefaultRCs = False
              Case Else
              End Select
            End If
          Loop
        Case "cboDataSource"
          cboN0 = -1
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, cMd$                       'Read a line
            'PRINT cmd$
            If Len(cMd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, cMd$, " ")           'Look for first space
            If i1% > 0 Then                     'Look for a command line
              cboN0 = cboN0 + 1
              cDataSource$(cboN0) = Left$(cMd$, i1 - 1)
              i1% = InStr(i1%, cMd$, "=")       'USERNAME
              If i1% > 0 Then
                i2% = InStr(i1 + 1, cMd$, " ")
                cUsername0(cboN0) = Mid$(cMd$, i1 + 1, i2 - i1 - 1)
                i1% = InStr(i2% + 1, cMd$, "=")     'PASSWORD
                i2% = Len(cMd$)
                cPassword0(cboN0) = Mid$(cMd$, i1 + 1, i2 - i1)
              End If
            End If
          Loop
          
        Case "cboDestination"
          cboN = -1
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, cMd$                       'Read a line
            'PRINT cmd$
            If Len(cMd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, cMd$, " ")           'Look for first space
            If i1% > 0 Then                     'Look for a command line
              cboN = cboN + 1
              cDestination$(cboN) = Left$(cMd$, i1 - 1)
              i1% = InStr(i1%, cMd$, "=")       'USERNAME
              If i1% > 0 Then
                i2% = InStr(i1 + 1, cMd$, " ")
                cUsername(cboN) = Mid$(cMd$, i1 + 1, i2 - i1 - 1)
                i1% = InStr(i2% + 1, cMd$, "=")     'PASSWORD
                i2% = Len(cMd$)
                cPassword(cboN) = Mid$(cMd$, i1 + 1, i2 - i1)
              End If
            End If
          Loop
        Case "cboDestination2"
          cboN2 = -1
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, cMd$                       'Read a line
            'PRINT cmd$
            If Len(cMd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, cMd$, " ")           'Look for first space
            If i1% > 0 Then                     'Look for a command line
              cboN2 = cboN2 + 1
              cDestination2$(cboN2) = Left$(cMd$, i1 - 1)
              i1% = InStr(i1%, cMd$, "=")       'USERNAME
              If i1% > 0 Then
                i2% = InStr(i1 + 1, cMd$, " ")
                cUsername2(cboN2) = Mid$(cMd$, i1 + 1, i2 - i1 - 1)
                i1% = InStr(i2% + 1, cMd$, "=")     'PASSWORD
                i2% = Len(cMd$)
                cPassword2(cboN2) = Mid$(cMd$, i1 + 1, i2 - i1)
              End If
            End If
          Loop
        
        Case Else
        End Select
      End If
    End If
  Loop
  
Exit_Sub:
  Close (lu%)
End Sub

Sub REFwriteCALFILE(FQFN$)
Dim i%, j%, lu%, iNRC%

  CFR.Gendate = Gendate$
  For i = 1 To 10
    CFR.WCTdates(i) = WCTdates$(i)
  Next i
  CFR.UTstart = UTstart
  CFR.UTend = UTend
  CFR.Channels = Channels
  CFR.Nel = Nel
  CFR.Emissivity = Emissivity
  CFR.Reflectivity = Reflectivity
  CFR.DeltaTmin = DeltaTmin
  CFR.RHS = RHS
  CFR.LocHor = LocHor
  
' "FIT_INFO"
  CFR.Nfit = Nfit
  For i = 1 To Nfit
    CFR.NP(i) = NP$(i)
  Next i
  
  For i = 1 To Channels
    CFR.GOF(i) = GOF(i)
    For j = 1 To Nfit
      CFR.GEC(i, j) = GEC(i, j)
    Next j
  Next i
  CFR.TGToffset = TGToffset
  CFR.MXRoffset = MXRoffset
  CFR.NAVoffset = NAVoffset
  CFR.NDoffset = NDoffset
  

' "WINDOW_CORRECTIONS"
  CFR.EnableWCT = EnableWCT
  For i = 1 To Channels
    For j = 1 To Nel
      CFR.WINcor(i, j) = WINcor(i, j)
    Next j
  Next i
' RAW counts editting criteria
  CFR.CMAcycles = CMAcycles
  CFR.CMAcycles2 = CMAcycles2
  CFR.RFIthreshold = RFIthreshold
  CFR.RFIiterations = RFIiterations
  CFR.RFIiterations2 = RFIiterations2
  CFR.MUXthreshold = MUXthreshold
  CFR.BadCycles = BadCycles
  CFR.Badcycles2 = Badcycles2
  CFR.UseMAforCB = UseMAforCB
  CFR.UseMAforCS = UseMAforCS
'  CFR.UseMAforCSgain = UseMAforCSgain
  CFR.UseMAforCN = UseMAforCN
  CFR.UseMAforTtgt = UseMAforTtgt
  CFR.UseMAforTifa = UseMAforTifa
  CFR.RAWextension = RAWextension
  
' Gain Limits
  For i = 1 To Channels
    CFR.GeqnMin(i) = GeqnMin(i)
    CFR.GeqnMax(i) = GeqnMax(i)
    CFR.GnavMin(i) = GnavMin(i)
    CFR.GnavMax(i) = GnavMax(i)
    CFR.GndMin(i) = GndMin(i)
    CFR.GndMax(i) = GndMax(i)
  Next i
  
' Channel Weights
  For i = 1 To 3: CFR.ChInfo(i) = ChInfo(i):  Next i

' Fit Region
  CFR.TBfitX1 = TBfitX1
  CFR.TBfitX2 = TBfitX2
  CFR.TBfitY1 = TBfitY1
  CFR.TBfitY2 = TBfitY2

' Noise Diode Noise Temperatures
  For i = 1 To Channels
    CFR.Cnd0(i) = Cnd0(i)
    CFR.Cnd1(i) = Cnd1(i)
    CFR.Cnd2(i) = Cnd2(i)
  Next i
  CFR.TrefND = TrefND
  CFR.NRC = NRC
  
  If NRC > 16 Then
    For i = 16 To NRC - 1
      CFR.RegExt(i - 15) = Reg(i)
      CFR.RCfmt(i - 10) = RCformat(i)
      CFR.RCuseExt(i - 15) = RCuse(i)
    Next i
  End If
  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF)
  Put #lu%, 3, CFR
  Close lu

End Sub

Sub REFreadCALFILE(FQFN$)
Dim i%, j%, lu%

  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF)
  Get #lu%, 3, CFR
  Close lu
  
  Gendate$ = Trim(CFR.Gendate)
  For i = 1 To 10
    WCTdates$(i) = Trim(CFR.WCTdates(i))
  Next i
  UTstart = CFR.UTstart
  UTend = CFR.UTend
  Channels = CFR.Channels
  Nel = CFR.Nel
  Emissivity = CFR.Emissivity
  Reflectivity = CFR.Reflectivity
  DeltaTmin = CFR.DeltaTmin
  
' "FIT_INFO"
  Nfit = CFR.Nfit
  For i = 1 To Nfit
    NP$(i) = Trim(CFR.NP(i))
  Next i
  
  For i = 1 To Channels
    GOF(i) = CFR.GOF(i)
    For j = 1 To Nfit
      GEC(i, j) = CFR.GEC(i, j)
    Next j
  Next i
  TGToffset = CFR.TGToffset
  MXRoffset = CFR.MXRoffset
  NAVoffset = CFR.NAVoffset
  NDoffset = CFR.NDoffset

  UseMAforCB = CFR.UseMAforCB
  UseMAforCS = CFR.UseMAforCS
  'UseMAforCSgain = CFR.UseMAforCSgain
  UseMAforCN = CFR.UseMAforCN
  UseMAforTtgt = CFR.UseMAforTtgt
  UseMAforTifa = CFR.UseMAforTifa
  RAWextension = CFR.RAWextension

' "WINDOW_CORRECTIONS"
  EnableWCT = CFR.EnableWCT
  For i = 1 To Channels
    For j = 1 To Nel
      WINcor(i, j) = CFR.WINcor(i, j)
    Next j
  Next i
  
' RAW counts editting criteria
  CMAcycles = CFR.CMAcycles              'Slow -- Base, Ttgt, etc.
  CMAcycles2 = CFR.CMAcycles2            'Fast -- Sky counts
  
  RFIthreshold = CFR.RFIthreshold
  RFIiterations = CFR.RFIiterations
  RFIiterations2 = CFR.RFIiterations2
  MUXthreshold = CFR.MUXthreshold
  BadCycles = CFR.BadCycles
  Badcycles2 = CFR.Badcycles2
  
' Gain Limits
  For i = 1 To Channels
    GeqnMin(i) = CFR.GeqnMin(i)
    GeqnMax(i) = CFR.GeqnMax(i)
    GnavMin(i) = CFR.GnavMin(i)
    GnavMax(i) = CFR.GnavMax(i)
    GndMin(i) = CFR.GndMin(i)
    GndMax(i) = CFR.GndMax(i)
  Next i
  
' Channel Weights
  For i = 1 To Channels: ChInfo(i) = CFR.ChInfo(i):  Next i

' Fit Region
  TBfitX1 = CFR.TBfitX1
  TBfitX2 = CFR.TBfitX2
  TBfitY1 = CFR.TBfitY1
  TBfitY2 = CFR.TBfitY2

' Noise Diode Noise Temperatures
  For i = 1 To Channels
    Cnd0(i) = CFR.Cnd0(i)
    Cnd1(i) = CFR.Cnd1(i)
    Cnd2(i) = CFR.Cnd2(i)
  Next i
  TrefND = CFR.TrefND
  RHS = CFR.RHS
  LocHor = CFR.LocHor
'  NRC = CFR.NRC    'already read in FIrecord with first part of Reg, RCformat, RCuse
  If NRC > 16 Then
    For i = 16 To NRC - 1
      Reg(i) = CFR.RegExt(i - 15)
      RCformat(i) = CFR.RCfmt(i - 10)
      RCuse(i) = CFR.RCuseExt(i - 15)
    Next i
  End If

End Sub

Sub REFwrite32(lu%, Record%)
Dim A As REFrecord, i%, j%, x%
' Debug.Print Cycle; GoodScan; GoodTrop

  A.GoodScan = GoodScan
  A.GoodTrop = GoodTrop
  A.Cycle = Cycle
  A.Nlev = Nlev
  A.Nlev1 = Nlev1
  A.Nlev2 = Nlev2
  A.MakeWord = MakeWord
  A.EditWord = EditWord
  A.UTsec = UTsec
  A.UTsecMTP = UTsecMTP
  A.UTsecNav = UTsecNav
  A.pALT = pALT
  A.gALT = gALT           'gALT
  A.rALT = 99.9           'rALT
  A.Pitch = Pitch
  A.Roll = Roll
  A.Latitude = Latitude
  A.Longitude = Longitude
  A.Heading = Heading
  A.TAS = 999.9        'TAS
  A.Wspd = Wspd
  A.Wdir = Wdir
  A.Zt1 = Zt1
  A.TT1 = TT1
  A.Th1 = Th1
  A.SEp1 = SEp1
  A.SEm1 = SEm1
  A.Zt2 = Zt2
  A.TT2 = TT2
  A.Th2 = Th2
  A.SEp2 = SEp2
  A.SEm2 = SEm2
  A.OATmtp = OATmtp
  A.OATnav = OATnav
  A.OATmms = OATmms
  A.LRac = LRac
  A.LRac2 = LRac2
  A.LRac3 = LRac3
  A.Zice = Zice
  A.dTice = dTice
  A.Elcor = Elcor
  A.ElCorUsed = ElCorUsed
  A.MRI = MRI
  A.ZtIPV = ZtIPV
  A.IPVt = IPVt
  A.StatusBits = StatusBits
  A.Tmin5 = Tmin5
  A.Zmin5 = Zmin5
  A.RCindex1 = RCindex1
  A.RCindex2 = RCindex2

  A.g1 = g(1)
  A.g2 = g(2)
  A.g3 = g(3)
  A.G1eqn = Geqn(1)
  A.G2eqn = Geqn(2)
  A.G3eqn = Geqn(3)
  A.G1nd = Gnd(1)
  A.G2nd = Gnd(2)
  A.G3nd = Gnd(3)
  A.G1oat = Goat(1)
  A.G2oat = Goat(2)
  A.G3oat = Goat(3)
'Debug.Print Record; Goat(1); Goat(2); Goat(3)
  For i% = 1 To Channels
    A.RFImask(i) = RFImask(i)
    For j% = 1 To 10: A.TA(i%, j%) = TA(i%, j%):  Next j%
  Next i%
  
  For i% = 1 To 33
    A.zzz(i%) = zzz(i%)
    If TTT(i) > 32767 Then TTT(i) = 32767
    A.TTTT(i%) = Int(TTT(i%))
    A.TSE(i%) = TSE(i%)
    A.Zgeo(i%) = Zgeo(i%)
    A.ND(i%) = ND(i%)
  Next i%
  
  For i% = 1 To Channels
    For j% = 1 To 12
      A.Counts(i%, j%) = C(i%, j%)
      A.CMA(i, j) = CMA(i, j)
    Next j%
  Next i%
  
  A.Muxs(0) = Muxs(0)
  For i% = 1 To 16
    A.Muxs(i%) = Muxs(i%)
    A.Mux(i%) = Mux(i%)
  Next i%
  
  Put #lu%, Record% + HiddenRecords, A

End Sub

Sub REFwrite33(lu%, Record%)
Dim A As REF2record, i%, j%, x%
' Debug.Print Cycle; GoodScan; GoodTrop

  A.GoodScan = GoodScan
  A.GoodTrop = GoodTrop
  A.Cycle = Cycle
  A.Nlev = Nlev
  A.Nlev1 = Nlev1
  A.Nlev2 = Nlev2
  A.MakeWord = MakeWord
  A.EditWord = EditWord
  A.UTsec = UTsec
  A.UTsecMTP = UTsecMTP
  A.UTsecNav = UTsecNav
  A.pALT = pALT
  A.gALT = gALT           'gALT
  A.rALT = 99.9           'rALT
  A.Pitch = Pitch
  A.Roll = Roll
  A.Latitude = Latitude
  A.Longitude = Longitude
  A.Heading = Heading
  A.TAS = 999.9        'TAS
  A.Wspd = Wspd
  A.Wdir = Wdir
  A.Zt1 = Zt1
  A.TT1 = TT1
  A.Th1 = Th1
  A.SEp1 = SEp1
  A.SEm1 = SEm1
  A.Zt2 = Zt2
  A.TT2 = TT2
  A.Th2 = Th2
  A.SEp2 = SEp2
  A.SEm2 = SEm2
  A.Tcp = Tcp
  A.Zcp = Zcp
  A.OATmtp = OATmtp
  A.OATnav = OATnav
  A.OATmms = OATmms
  A.LRac = LRac
  A.LRac2 = LRac2
  A.LRac3 = LRac3
  A.Zice = Zice
  A.dTice = dTice
  A.Elcor = Elcor
  A.ElCorUsed = ElCorUsed
  A.MRI = MRI
  A.ZtIPV = ZtIPV
  A.IPVt = IPVt
  A.StatusBits = StatusBits
  A.Tmin5 = Tmin5
  A.Zmin5 = Zmin5
  A.RCindex1 = RCindex1
  A.RCindex2 = RCindex2
  A.TwoAAprofiles = TwoAAprofiles
  A.Algorithm = Algorithm
  A.Stats(1, 1) = Stats(1, 1)
  A.Stats(2, 1) = Stats(2, 1)
  A.Stats(3, 1) = Stats(3, 1)
  A.Stats(1, 2) = Stats(1, 2)
  A.Stats(2, 2) = Stats(2, 2)
  A.Stats(3, 2) = Stats(3, 2)

  A.g1 = g(1)
  A.g2 = g(2)
  A.g3 = g(3)
  A.G1eqn = Geqn(1)
  A.G2eqn = Geqn(2)
  A.G3eqn = Geqn(3)
  A.G1nd = Gnd(1)
  A.G2nd = Gnd(2)
  A.G3nd = Gnd(3)
  A.G1oat = Goat(1)
  A.G2oat = Goat(2)
  A.G3oat = Goat(3)
'Debug.Print Record; Goat(1); Goat(2); Goat(3)
  For i% = 1 To Channels
    A.RFImask(i) = RFImask(i)
    For j% = 1 To 10: A.TA(i%, j%) = TA(i%, j%):  Next j%
  Next i%
  
  For i% = 1 To 33
    A.sZZZ(i%) = sZZZ(i%)
    A.sTTT(i%) = sTTT(i%)
    A.sTSE(i%) = sTSE(i%)
    A.sZg(i%) = sZg(i%)
    A.sND(i%) = sND(i%)
'    Debug.Print i; sZZZ(i); sTTT(i)
  Next i%
  
  For i% = 1 To Channels
    For j% = 1 To Ncts
      A.Counts(i%, j%) = C(i%, j%)
      A.CMA(i, j) = CMA(i, j)
    Next j%
  Next i%
  
  A.Muxs(0) = Muxs(0)
  For i% = 1 To 32
    A.Muxs(i%) = Muxs(i%)
    A.Mux(i%) = Mux(i%)
    'Debug.Print i; Muxs(i); Mux(i)
  Next i%
  
  Put #lu%, Record% + HiddenRecords, A

End Sub

Sub IWGwrite(IWGlu%, Irecord&)
Dim A As IWG_DS, i%
' Write binary IWG data file

  A.DandT = DandT                  ' 001 x Date/Time          yyyy-mm-ddThh:mm:ss   UTsec
  A.Latitude = Latitude            ' 002 x Latitude           deg                   Latitude
  A.Longitude = Longitude          ' 003 x Longitude          deg                   Longitude
  A.gALT = gALT                    ' 004 x GPS_MSL_Alt        m                     gALT, Zg
  A.wALT = wALT                    ' 005   WGS_84_Alt         m                     wALT, Zw
  A.pALT = pALT                    ' 006 x Press_Alt          ft                    pALT, Zp
  A.rALT = rALT                    ' 007   Radar_Alt          ft                    rALT, Zr
  A.GroundSpeed = GroundSpeed      ' 008   Grnd_Spd           m/s                   GroundSpeed
  A.TAS = TAS                      ' 009   True_Airspeed      m/s                   TAS
  A.IAS = IAS                      ' 010   Indicated_Airspeed kts                   IAS
  A.Mach = Mach                    ' 011   Mach_Number        ---                   Mach
  A.Vzac = Vzac                    ' 012   Vert_Velocity      m/s                   Vzac
  A.Heading = Heading              ' 013 x True_Hdg           degt                  Heading
  A.TrackAngle = TrackAngle        ' 014   Track              degt                  TrackAngle
  A.DriftAngle = DriftAngle        ' 015   Drift              deg                   DriftAngle
  A.Pitch = Pitch                  ' 016 x Pitch              deg                   Pitch
  A.Roll = Roll                    ' 017 x Roll               deg                   Roll
  A.SideSlipAngle = SideSlipAngle  ' 018   Side_slip          deg                   SideSlipAngle
  A.AttackAngle = AttackAngle      ' 019   Angle_of_Attack    deg                   AttackAngle
  A.OATn = OATn                    ' 020 x Ambient_Temp       C                     OATn (OATnav = OATn + cTo)
  A.Tdew = Tdew                    ' 021   Dew_Point          C                     Tdew
  A.Ttotal = Ttotal                ' 022   Total_Temp         C                     Ttotal
  A.Pstatic = Pstatic              ' 023   Static_Press       mb                    Pstatic
  A.Pdynamic = Pdynamic            ' 024   Dynamic_Press      mb                    Pdynamic
  A.Pcabin = Pcabin                ' 025   Cabin_Press        mb                    Pcabin
  A.Wspd = Wspd                    ' 026 x Wind_Speed         m/s                   Wspd
  A.Wdir = Wdir                    ' 027 x Wind_Dir           degt                  Wdir
  A.VWS = VWS                      ' 028   Vert_Wind_Spd      m/s                   VWS
  A.SZA = SZA                      ' 029   Solar_Zenith_Angle deg                   SZA
  A.SEAC = SEAC                    ' 030   Sun_Elev_AC        deg                   SEAC
  A.SAG = SAG                      ' 031   Sun_Az_Grd         degt                  SAG
  A.SAAC = SAAC                    ' 032   Sun_Az_AC          degt                  SAAC
  For i = 1 To 17
    A.Spares(i) = 0#
  Next i
  
  Put #IWGlu%, Irecord&, A

End Sub

Sub NAVwrite(NAVlu%, NAVrecord&)
Dim A As NAVrecord, i%
' Write binary NAV data file

  ThetaAC = fTheta(OATn0, Pstatic)
  A.DandT = DandT                  ' 000 x Date/Time              yyyy-mm-ddThh:mm:ss   UTsec
  A.Pstatic = Pstatic              ' 001   Static_Press           mb                    Pstatic
  A.OATnav = OATn0                 ' 002 x Ambient_Temp           K                     OATn (OATnav = OATn + cTo)
  A.Pitch = Pitch                  ' 003 x Pitch                  deg                   Pitch
  A.Roll = Roll                    ' 004 x Roll                   deg                   Roll
  A.Latitude = Latitude            ' 005 x Latitude               deg                   Latitude
  A.Longitude = Longitude          ' 006 x Longitude              deg                   Longitude
  A.Heading = Heading              ' 007 x True_Hdg               degt                  Heading
  A.gALT = gALT                    ' 008 x GPS_MSL_Alt            km                    gALT, Zg
  A.pALT = pALT                    ' 009 x Press_Alt              km                    pALT, Zp
  A.Wspd = Wspd                    ' 010 x Wind_Speed             m/s                   Wspd
  A.Wdir = Wdir                    ' 011 x Wind_Dir               degt                  Wdir
  A.UWS = UWS                      ' 012   East wind component    m/s                   UWS
  A.VWS = VWS                      ' 013   North wind component   m/s
  A.WWS = WWS                      ' 014   Vertical wind          m/s                   WWS
  A.ThetaAC = ThetaAC              ' 015   Potential Temperature  K
  A.OATn0 = OATn0                  ' 016   AT_A                   K
  A.OATn1 = OATn1                  ' 017   ATFR                   K
  A.OATn2 = OATn2                  ' 018   ATHL1                  K
  A.OATn3 = OATn3                  ' 019   ATHL2                  K
  A.OATn4 = OATn4                  ' 020   ATHR1                  K
  A.OATn5 = OATn5                  ' 021   ATHR2                  K
  A.Mach = Mach                    ' 022   Mach_Number            ---                   Mach
  A.Mach2 = Mach2                  ' 023   Mach_Number squared    ---                   Mach2
  A.wALT = wALT                    ' 024   WGS_84_Alt             km                    wALT, Zw
  A.rALT = rALT                    ' 025   Radar_Alt              km                    rALT, Zr
  A.hALT = hALT                    ' 026   Geopotential Ht        km                    hALT, Zw
  A.iALT = iALT                    ' 027   IRS altitude           km                    iALT, Zr
  A.Ptotal = Ptotal                ' 028
  A.Pdynamic = Pdynamic            ' 029   Dynamic_Press          hPa                   Pdynamic
  A.GroundSpeed = GroundSpeed      ' 030   Grnd_Spd               m/s                   GroundSpeed
  A.PSFC = PSFC                    ' 031
  A.Vzac = Vzac                    ' 032   Vert_Velocity          m/s                   Vzac
  
  A.TOATn0 = TOATn0                ' 033   Total_Temp             K                     Ttotal
  A.TOATn1 = TOATn1                ' 034   Total_Temp             K                     Ttotal
  A.TOATn2 = TOATn2                ' 035   Total_Temp             K                     Ttotal
  A.TOATn3 = TOATn3                ' 036   Total_Temp             K                     Ttotal
  A.TOATn4 = TOATn4                ' 037   Total_Temp             K                     Ttotal
  A.TOATn5 = TOATn5                ' 038   Total_Temp             K                     Ttotal
  A.Ttotal = Ttotal                ' 039   Total_Temp             K                     Ttotal
  
  A.ACINS = ACINS                  ' 040   IRS Vert. Acc.         m/s2
  A.QC_A = QC_A                    ' 041   ADC Impact Pressure
  A.TAS = TAS                      ' 042   True_Airspeed          m/s                   TAS
  A.IAS = IAS                      ' 043   Indicated_Airspeed     kts                   IAS
  A.TrackAngle = TrackAngle        ' 044   Track                  degt                  TrackAngle
  A.DriftAngle = DriftAngle        ' 045   Drift                  deg                   DriftAngle
  A.SideSlipAngle = SideSlipAngle  ' 046   Side_slip              deg                   SideSlipAngle
  A.AttackAngle = AttackAngle      ' 047   Angle_of_Attack        deg                   AttackAngle
  
  A.Tdew = Tdew                    ' 048   Dew_Point              K                     Tdew
  A.Pcabin = Pcabin                ' 049   Cabin_Press            hPa                   Pcabin
  A.SZA = SZA                      ' 050   Solar_Zenith_Angle     deg                   SZA
  A.SEAC = SEAC                    ' 051   Sun_Elev_AC            deg                   SEAC
  A.SAG = SAG                      ' 052   Sun_Az_Grd             degt                  SAG
  A.SAAC = SAAC                    ' 053   Sun_Az_AC              degt                  SAAC
  For i = 1 To 20
    A.Npares(i) = 0#
  Next i
  
  Put #NAVlu%, NAVrecord&, A
End Sub
Sub NAVread(NAVlu%, NAVrecord&)
Dim A As NAVrecord, i%
' Read binary NAV data file

  Get #NAVlu%, NAVrecord&, A

  DandT = A.DandT                  ' 000 x Date/Time              yyyy-mm-ddThh:mm:ss   UTsec
  Pstatic = A.Pstatic              ' 001   Static_Press           mb                    Pstatic
  OATn = A.OATnav                  ' 002 x Ambient_Temp           K                     OATn (OATnav = OATn + cTo)
  Pitch = A.Pitch                  ' 003 x Pitch                  deg                   Pitch
  Roll = A.Roll                    ' 004 x Roll                   deg                   Roll
  Latitude = A.Latitude            ' 005 x Latitude               deg                   Latitude
  Longitude = A.Longitude          ' 006 x Longitude              deg                   Longitude
  Heading = A.Heading              ' 007 x True_Hdg               degt                  Heading
  gALT = A.gALT                    ' 008 x GPS_MSL_Alt            km                    gALT, Zg
  pALT = A.pALT                    ' 009 x Press_Alt              km                    pALT, Zp
  Wspd = A.Wspd                    ' 010 x Wind_Speed             m/s                   Wspd
  Wdir = A.Wdir                    ' 011 x Wind_Dir               degt                  Wdir
  UWS = A.UWS                      ' 012   East wind component    m/s                   UWS
  VWS = A.VWS                      ' 013   North wind component   m/s
  WWS = A.WWS                      ' 014   Vertical wind          m/s                   WWS
  ThetaAC = A.ThetaAC              ' 015   Potential Temperature  K
  OATn0 = A.OATn0                  ' 016   AT_A                   K
  OATn1 = A.OATn1                  ' 017   ATFR                   K
  OATn2 = A.OATn2                  ' 018   ATHL1                  K
  OATn3 = A.OATn3                  ' 019   ATHL2                  K
  OATn4 = A.OATn4                  ' 020   ATHR1                  K
  OATn5 = A.OATn5                  ' 021   ATHR2                  K
  Mach = A.Mach                    ' 022   Mach_Number            ---                   Mach
  Mach2 = A.Mach2                  ' 023   Mach_Number squared    ---                   Mach2
  wALT = A.wALT                    ' 024   WGS_84_Alt             km                    wALT, Zw
  rALT = A.rALT                    ' 025   Radar_Alt              km                    rALT, Zr
  hALT = A.hALT                    ' 026   Geopotential Ht        km                    hALT, Zw
  iALT = A.iALT                    ' 027   IRS altitude           km                    iALT, Zr
  Ptotal = A.Ptotal                ' 028
  Pdynamic = A.Pdynamic            ' 029   Dynamic_Press          hPa                   Pdynamic
  GroundSpeed = A.GroundSpeed      ' 030   Grnd_Spd               m/s                   GroundSpeed
  PSFC = A.PSFC                    ' 031
  Vzac = A.Vzac                    ' 032   Vert_Velocity          m/s                   Vzac
  
  TOATn0 = A.TOATn0                ' 033   Total_Temp             K                     Ttotal
  TOATn1 = A.TOATn1                ' 034   Total_Temp             K                     Ttotal
  TOATn2 = A.TOATn2                ' 035   Total_Temp             K                     Ttotal
  TOATn3 = A.TOATn3                ' 036   Total_Temp             K                     Ttotal
  TOATn4 = A.TOATn4                ' 037   Total_Temp             K                     Ttotal
  TOATn5 = A.TOATn5                ' 038   Total_Temp             K                     Ttotal
  Ttotal = A.Ttotal                ' 039   Total_Temp             K                     Ttotal
  
  ACINS = A.ACINS                  ' 040   IRS Vert. Acc.         m/s2
  QC_A = A.QC_A                    ' 041   ADC Impact Pressure
  TAS = A.TAS                      ' 042   True_Airspeed          m/s                   TAS
  IAS = A.IAS                      ' 043   Indicated_Airspeed     kts                   IAS
  TrackAngle = A.TrackAngle        ' 044   Track                  degt                  TrackAngle
  DriftAngle = A.DriftAngle        ' 045   Drift                  deg                   DriftAngle
  SideSlipAngle = A.SideSlipAngle  ' 046   Side_slip              deg                   SideSlipAngle
  AttackAngle = A.AttackAngle      ' 047   Angle_of_Attack        deg                   AttackAngle
  
  Tdew = A.Tdew                    ' 048   Dew_Point              K                     Tdew
  Pcabin = A.Pcabin                ' 049   Cabin_Press            hPa                   Pcabin
  SZA = A.SZA                      ' 050   Solar_Zenith_Angle     deg                   SZA
  SEAC = A.SEAC                    ' 051   Sun_Elev_AC            deg                   SEAC
  SAG = A.SAG                      ' 052   Sun_Az_Grd             degt                  SAG
  SAAC = A.SAAC                    ' 053   Sun_Az_AC              degt                  SAAC
'  For i = 1 To 20
'    Npares(i) = A.Npares(i)
'  Next i
  
    NAV_DandT = DandT
  NAV_Parameter(5) = Latitude
  NAV_Parameter(6) = Longitude
  NAV_Parameter(8) = gALT
  'NAV_Parameter(8) = wALT
  NAV_Parameter(9) = pALT
  NAV_Parameter(25) = rALT
  NAV_Parameter(30) = GroundSpeed
  NAV_Parameter(42) = TAS
  NAV_Parameter(43) = IAS
  NAV_Parameter(22) = Mach
  NAV_Parameter(32) = Vzac
  NAV_Parameter(7) = Heading
  NAV_Parameter(44) = TrackAngle
  NAV_Parameter(45) = DriftAngle
  NAV_Parameter(3) = Pitch
  NAV_Parameter(4) = Roll
  NAV_Parameter(46) = SideSlipAngle
  NAV_Parameter(47) = AttackAngle
  NAV_Parameter(20) = OATn
  NAV_Parameter(48) = Tdew
  NAV_Parameter(39) = Ttotal
  NAV_Parameter(1) = Pstatic
  NAV_Parameter(29) = Pdynamic
  NAV_Parameter(49) = Pcabin
  NAV_Parameter(10) = Wspd
  NAV_Parameter(11) = Wdir
  NAV_Parameter(14) = WWS
  NAV_Parameter(50) = SZA
  NAV_Parameter(51) = SEAC
  NAV_Parameter(52) = SAG
  NAV_Parameter(53) = SAAC
  NAV_Parameter(33) = TOATn0
  NAV_Parameter(34) = TOATn1
  NAV_Parameter(35) = TOATn2
  NAV_Parameter(36) = TOATn3
  NAV_Parameter(37) = TOATn4
  NAV_Parameter(33) = TOATn5
  NAV_Parameter(16) = OATn0
  NAV_Parameter(17) = OATn1
  NAV_Parameter(18) = OATn2
  NAV_Parameter(19) = OATn3
  NAV_Parameter(20) = OATn4
  NAV_Parameter(21) = OATn5
  NAV_Parameter(23) = Mach2
  NAV_Parameter(28) = Ptotal
  NAV_Parameter(23) = Mach2
 

End Sub

Sub IWGread(IWGlu%, Irecord&)
Dim A As IWG_DS, i%
' Read binary IWG data file

  Get #IWGlu%, Irecord&, A

  DandT = A.DandT                  ' 001 x Date/Time          yyyy-mm-ddThh:mm:ss   UTsec
  Latitude = A.Latitude            ' 002 x Latitude           deg                   Latitude
  Longitude = A.Longitude          ' 003 x Longitude          deg                   Longitude
  gALT = A.gALT                    ' 004 x GPS_MSL_Alt        m                     gALT, Zg
  wALT = A.wALT                    ' 005   WGS_84_Alt         m                     wALT, Zw
  pALT = A.pALT                    ' 006 x Press_Alt          ft                    pALT, Zp
  rALT = A.rALT                    ' 007   Radar_Alt          ft                    rALT, Zr
  GroundSpeed = A.GroundSpeed      ' 008   Grnd_Spd           m/s                   GroundSpeed
  TAS = A.TAS                      ' 009   True_Airspeed      m/s                   TAS
  IAS = A.IAS                      ' 010   Indicated_Airspeed kts                   IAS
  Mach = A.Mach                    ' 011   Mach_Number        ---                   Mach
  Vzac = A.Vzac                    ' 012   Vert_Velocity      m/s                   Vzac
  Heading = A.Heading              ' 013 x True_Hdg           degt                  Heading
  TrackAngle = A.TrackAngle        ' 014   Track              degt                  TrackAngle
  DriftAngle = A.DriftAngle        ' 015   Drift              deg                   DriftAngle
  Pitch = A.Pitch                  ' 016 x Pitch              deg                   Pitch
  Roll = A.Roll                    ' 017 x Roll               deg                   Roll
  SideSlipAngle = A.SideSlipAngle  ' 018   Side_slip          deg                   SideSlipAngle
  AttackAngle = A.AttackAngle      ' 019   Angle_of_Attack    deg                   AttackAngle
  OATn = A.OATn                    ' 020 x Ambient_Temp       C                     OATn (OATnav = OATn + cTo)
  Tdew = A.Tdew                    ' 021   Dew_Point          C                     Tdew
  Ttotal = A.Ttotal                ' 022   Total_Temp         C                     Ttotal
  Pstatic = A.Pstatic              ' 023   Static_Press       mb                    Pstatic
  Pdynamic = A.Pdynamic            ' 024   Dynamic_Press      mb                    Pdynamic
  Pcabin = A.Pcabin                ' 025   Cabin_Press        mb                    Pcabin
  Wspd = A.Wspd                    ' 026 x Wind_Speed         m/s                   Wspd
  Wdir = A.Wdir                    ' 027 x Wind_Dir           degt                  Wdir
  VWS = A.VWS                      ' 028   Vert_Wind_Spd      m/s                   VWS
  SZA = A.SZA                      ' 029   Solar_Zenith_Angle deg                   SZA
  SEAC = A.SEAC                    ' 030   Sun_Elev_AC        deg                   SEAC
  SAG = A.SAG                      ' 031   Sun_Az_Grd         degt                  SAG
  SAAC = A.SAAC                    ' 032   Sun_Az_AC          degt                  SAAC
  For i = 1 To 17
    Spares(i) = A.Spares(i)
  Next i
  
  IWG_DandT = DandT
  IWG_Parameter(2) = Latitude
  IWG_Parameter(3) = Longitude
  IWG_Parameter(4) = gALT
  IWG_Parameter(5) = wALT
  IWG_Parameter(6) = pALT
  IWG_Parameter(7) = rALT
  IWG_Parameter(8) = GroundSpeed
  IWG_Parameter(9) = TAS
  IWG_Parameter(10) = IAS
  IWG_Parameter(11) = Mach
  IWG_Parameter(12) = Vzac
  IWG_Parameter(13) = Heading
  IWG_Parameter(14) = TrackAngle
  IWG_Parameter(15) = DriftAngle
  IWG_Parameter(16) = Pitch
  IWG_Parameter(17) = Roll
  IWG_Parameter(18) = SideSlipAngle
  IWG_Parameter(19) = AttackAngle
  IWG_Parameter(20) = OATn
  IWG_Parameter(21) = Tdew
  IWG_Parameter(22) = Ttotal
  IWG_Parameter(23) = Pstatic
  IWG_Parameter(24) = Pdynamic
  IWG_Parameter(25) = Pcabin
  IWG_Parameter(26) = Wspd
  IWG_Parameter(27) = Wdir
  IWG_Parameter(28) = VWS
  IWG_Parameter(29) = SZA
  IWG_Parameter(30) = SEAC
  IWG_Parameter(31) = SAG
  IWG_Parameter(32) = SAAC
  'Zp = pALT
  'Zg = gALT
  'ZW = wALT
  'Zr = rALT
  'OATnav = OATn + cTo

End Sub
Sub REFwrite(lu%, Record%)

  Select Case FileFormatOut
  Case 32: Call REFwrite32(lu%, Record%)
  Case 33: Call REFwrite33(lu%, Record%)
  End Select
  'Call REFread33(lu, Record)
End Sub


Sub REFwriteLIMITS(FQFN$)
Dim i%, lu%
  
  RLR.NV = NV%
  For i% = 0 To NV%: RLR.VSCAL(i%) = VSCAL(i%): Next i%
  
  RLR.UTmin = UTmin: RLR.UTmax = UTmax
  RLR.UTtakeoff = UTtakeoff:  RLR.UTlanding = UTlanding
  RLR.Mins(1) = GoodRecords: RLR.Maxs(1) = Badrecords
  RLR.Mins(2) = NretMin: RLR.Maxs(2) = NretMax
  RLR.Mins(3) = pALTmin: RLR.Maxs(3) = pALTmax
  RLR.Mins(4) = Pitchmin: RLR.Maxs(4) = PitchMax
  RLR.Mins(5) = Rollmin: RLR.Maxs(5) = RollMax
  RLR.Mins(6) = Tacmin: RLR.Maxs(6) = Tacmax
  RLR.Mins(7) = Zt1min: RLR.Maxs(7) = Zt1max
  If Zt2min = 99.9 And Zt2max = 0 Then Zt2min = 99.9: Zt2max = 99.9
  RLR.Mins(8) = Zt2min: RLR.Maxs(8) = Zt2max
  RLR.Mins(9) = T1min: RLR.Maxs(9) = T1max
  RLR.Mins(10) = T2min: RLR.Maxs(10) = T2max
  RLR.Mins(11) = PT1min: RLR.Maxs(11) = PT1max
  If PT2min = 600 And PT2max = 0 Then PT2min = 999.9: PT2max = 999.9
  RLR.Mins(12) = PT2min: RLR.Maxs(12) = PT2max
  RLR.Mins(13) = LATmin: RLR.Maxs(13) = LATmax
  RLR.Mins(14) = LONmin: RLR.Maxs(14) = LONmax
  RLR.Mins(15) = LRmin: RLR.Maxs(15) = LRmax
  RLR.Mins(16) = Zmin: RLR.Maxs(16) = Zmax
  RLR.Mins(17) = Tmin: RLR.Maxs(17) = Tmax
  RLR.Mins(18) = zTmin: RLR.Maxs(18) = ZtMax
  RLR.Mins(19) = PTmin: RLR.Maxs(19) = PTmax
  RLR.Mins(20) = TSEmin: RLR.Maxs(20) = TSEmax
  RLR.Mins(21) = zgeomin: RLR.Maxs(21) = zgeomax
  RLR.Mins(22) = NDmin: RLR.Maxs(22) = NDmax
  RLR.Mins(23) = MRImin: RLR.Maxs(23) = MRImax
  RLR.Mins(24) = gALTmin: RLR.Maxs(24) = gALTmax
  RLR.Mins(25) = UTksmin: RLR.Maxs(25) = UTksmax
  RLR.Ztgm = Ztgm                    'Altitude of global minimum T
  RLR.Tzgm = Tzgm                    'Global minimum T for flight above 5 km
  RLR.Thgm = Thgm                    'Theta at global minimym T
  RLR.UTgm = UTgm                    'UT at time of global minimum T
  
  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF)
  Put #lu%, 2, RLR
  Close lu
  
End Sub

Sub REF2writeLIMITS(FQFN$)
Dim i%, lu%
  
  RLR.NV = NV%
  For i% = 0 To NV%: RLR.VSCAL(i%) = VSCAL(i%): Next i%
  
  RLR.UTmin = UTmin: RLR.UTmax = UTmax
  RLR.UTtakeoff = UTtakeoff:  RLR.UTlanding = UTlanding
  RLR.Mins(1) = GoodRecords: RLR.Maxs(1) = Badrecords
  RLR.Mins(2) = NretMin: RLR.Maxs(2) = NretMax
  RLR.Mins(3) = pALTmin: RLR.Maxs(3) = pALTmax
  RLR.Mins(4) = Pitchmin: RLR.Maxs(4) = PitchMax
  RLR.Mins(5) = Rollmin: RLR.Maxs(5) = RollMax
  RLR.Mins(6) = Tacmin: RLR.Maxs(6) = Tacmax
  RLR.Mins(7) = Zt1min: RLR.Maxs(7) = Zt1max
  If Zt2min = 99.9 And Zt2max = 0 Then Zt2min = 99.9: Zt2max = 99.9
  RLR.Mins(8) = Zt2min: RLR.Maxs(8) = Zt2max
  RLR.Mins(9) = T1min: RLR.Maxs(9) = T1max
  RLR.Mins(10) = T2min: RLR.Maxs(10) = T2max
  RLR.Mins(11) = PT1min: RLR.Maxs(11) = PT1max
  If PT2min = 600 And PT2max = 0 Then PT2min = 999.9: PT2max = 999.9
  RLR.Mins(12) = PT2min: RLR.Maxs(12) = PT2max
  RLR.Mins(13) = LATmin: RLR.Maxs(13) = LATmax
  RLR.Mins(14) = LONmin: RLR.Maxs(14) = LONmax
  RLR.Mins(15) = LRmin: RLR.Maxs(15) = LRmax
  RLR.Mins(16) = Zmin: RLR.Maxs(16) = Zmax
  RLR.Mins(17) = Tmin: RLR.Maxs(17) = Tmax
  RLR.Mins(18) = zTmin: RLR.Maxs(18) = ZtMax
  RLR.Mins(19) = PTmin: RLR.Maxs(19) = PTmax
  RLR.Mins(20) = TSEmin: RLR.Maxs(20) = TSEmax
  RLR.Mins(21) = zgeomin: RLR.Maxs(21) = zgeomax
  RLR.Mins(22) = NDmin: RLR.Maxs(22) = NDmax
  RLR.Mins(23) = MRImin: RLR.Maxs(23) = MRImax
  RLR.Mins(24) = gALTmin: RLR.Maxs(24) = gALTmax
  RLR.Mins(25) = UTksmin: RLR.Maxs(25) = UTksmax
  RLR.Ztgm = Ztgm                    'Altitude of global minimum T
  RLR.Tzgm = Tzgm                    'Global minimum T for flight above 5 km
  RLR.Thgm = Thgm                    'Theta at global minimym T
  RLR.UTgm = UTgm                    'UT at time of global minimum T
  
  lu = FreeFile
  Open FQFN$ For Random Access Read Write As lu Len = Len(REF2)
  Put #lu%, 2, RLR
  Close lu
  
End Sub


Sub UpdateMPheader(HDRfile$, MPlu%)
Dim lu%, i%, j%, L%, n%, A$, b$, Nhdr%, yr$, dy$, mo$, fln&, fltnr&, FltDate&
Dim x$, lux%
Dim FlightNo&, Objective$, FlightDate$, NUMfile$

  lu% = FreeFile
  Open HDRfile$ For Input As lu%
  Input #lu%, Nhdr   'Get number of header lines
  Close (lu%)

  lu% = FreeFile
  Open HDRfile$ For Input As lu%   'Start over
  
  For j = 1 To Nhdr                'Update dates in MP file header
    Line Input #lu%, A$
    Select Case j
    Case 7
      Mid$(A$, 1, 4) = Mid$(YYYYMMDD$, 1, 4)  'set YR
      Mid$(A$, 6, 2) = Mid$(YYYYMMDD$, 5, 2)  'set MO
      Mid$(A$, 9, 2) = Mid$(YYYYMMDD$, 7, 2)  'set DY
      b$ = Date$: L = Len(b$)
      yr$ = Right$(b$, 4): dy$ = Mid$(b$, L - 6, 2): mo$ = Mid$(b$, L - 9, 2)
      Mid$(A$, 13) = yr$ + " " + mo$ + " " + dy$
      If Mission$ = "SONEX" Then
        lux% = FreeFile
        Open "\DC8\SONEX\FLTNR.PRN" For Input As lux%
        Do
          Input #lux%, fln, FltDate
          If FltDate = Val(Right$(YYYYMMDD$, 6)) Then FlightNo = fln
        Loop Until EOF(lux%)
      Else
        NUMfile$ = fSetupFileName("NUM")
        lux% = FreeFile
        Open NUMfile$ For Input As lux%
        i = -1
        Do: Line Input #lux, b$  'FlightNo, FlightDate, Objective$
          b$ = LTrim(b$)
          n = InStr(1, b$, " ")
          FlightNo = Val(Left$(b$, n - 1))
          b$ = Trim(Right$(b$, Len(b$) - n + 1))
          n = InStr(1, b$, " ")
          If n = 0 Then
            FlightDate = Val(b$)
            Objective$ = "Unknown"
          Else
            FlightDate = Val(Left$(b$, n - 1))
            Objective$ = Trim(Mid$(b$, n, Len(A$) - n + 1))
          End If
          i = i + 1
          If YYYYMMDD$ = Trim(Str(FlightDate)) Then Exit Do
        Loop Until EOF(lux)
      End If
      Close #lux%
      Mid$(A$, 25) = Format$(FlightNo, "00000000")
    Case Else
    End Select

    Print #MPlu%, A$
  Next j
  Close (lu%)

End Sub



Function fReadLF(lu%) As String
Dim C$, out$
out$ = ""
Do
  C$ = Input(1, lu)
  Select Case C$
  Case vbLf
  Case vbCr
  Case Else
    out$ = out$ + C$
  End Select
Loop Until C$ = vbLf Or EOF(lu)
fReadLF = out$
End Function

