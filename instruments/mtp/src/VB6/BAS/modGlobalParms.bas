Attribute VB_Name = "modGlobalParms"
' This module (modGlobalParms.bas) contains constants and other parameters that
' might be used by any of the MTP data processing programs
  Public Const mPI = 3.14159265358979
  Public Const kft_km! = 3.2808         'Km per 1000 ft
  Public Const Good = True
  Public Const BAD = False
  Public Const HiddenRecords = 5        'Header records in REF file

  Public Const VSCAL0 = 0.01            'Scale for Z
  Public Const VSCAL1 = 0.1             'Scale for T
  Public Const VSCAL2 = 0.1             'Scale for TSE
  Public Const VSCAL3 = 0.01            'Scale for Zgeo
  Public Const VSCAL4 = 1E+21           'Scale for ND

' EditWord Bitmap
  Public Const mskRetAlt = 1         'Retrieved Altitude
  Public Const mskTemps = 2          'Outside Air Temperature
  Public Const mskOATtrop = 4        'OAT of Tropopause
  Public Const mskZtOff = 8          'Tropopause Offset
  Public Const mskTropAlt = 16       'Tropopause Altitude
  Public Const mskPitch = 32         'Pitch
  Public Const mskRoll = 64          'Roll
  Public Const mskNlev = 128         'Number of Retrieval Levels

' MakeWord Bitmap
  Public Const mskNav = 1            'Navigation data quality
  Public Const mskTA = 2             'Brightness Temperature
  Public Const mskCts = 4            'Counts quality
  Public Const mskCycle = 8          'Cycle
  Public Const mskTtgt = 16          'Target Temperature
  Public Const mskRAWbad = 32        'RAW data is bad

' Define Color Constants
  Public Const Black = 0
  Public Const Blue = 1
  Public Const Green = 2
  Public Const Cyan = 3
  Public Const Red = 4
  Public Const Magenta = 5
  Public Const Brown = 6
  Public Const LGray = 7
  Public Const bGray = 8
  Public Const LBlue = 9
  Public Const LGreen = 10
  Public Const LCyan = 11
  Public Const LRed = 12
  Public Const LMagenta = 13
  Public Const Yellow = 14
  Public Const White = 15

  Public RAsave!(1 To 4, 1 To 33), RTsave!(1 To 4, 1 To 33)

' System SETUP parameters
  Public ProgramDrive$         'Drive letter (eg "D:") where program files are
  Public DataDrive$            'Drive letter where data files are
  Public UID$                  'Initials of user (eg "mjm")
  Public DefaultRTmode%        'Default REALTIME analysis mode (0=FALSE, 1=TRUE)

  Public CallingForm As Form   'Default form used in program (usually the Main one)
  Public CallingPgm$
  
' File Naming Variables
  Public FileFormatIn As Byte  'Format of binary file being read
  Public FileFormatOut As Byte 'Format of binary file being written
' Data structure (FIR) for Flight Information was changed on October 4, 2004,
' so that Mission$ was only a 15 character string instead of 16. This freed a byte
' for the FileFormat. Prior to this time character 16 was always a space, or ascii
' character code 32 (decimal). Files written using revised data structures will
' have FileFormats >32 and will be documented here. Old files will always have
' FileFormat = 32 because no mission names were ever 16 characters long.
  Public yyyymmdd$             '8-number year, month and day
  Public yymmdd$               'Obsolete 6-number year, month and day
  Public Drive$                'Same as DataDrive$
  Public PgmDrive$             'Same as ProgramDrive$
  Public Yeer%                 'Year (so named to avoid conflict with YEAR function)
  Public Doy%                  'Day of Year
  Public FltNumber$            'Flight Number
  Public PI$                   'Principal Investigator (PI) information for MP File
'  Public UserName$             'User Name (to construct path to programs)
  Public Mission$              'Mission designation (eg "SONEX", "AVE")
  Public Platform$             'Flight Platform (eg "DC8", "ER2")
  Public AC$                   'Two letter aircraft designation (eg "ER", "DC")
  Public FlightDate As Date    'Flight Date
  Public Objective$            'Flight Objective
  Public Rdir$                 'Root Directory =  "\" + Platform$ + "\"
  Public Rdir2$                'New Rdir = \MTP\Data\" + Platform$ + "\"
  Public Root$                 'Root$ = Drive$ + Rdir$
  Public MNpath$               'MNpath$ = Root$ + Mission$ + "\", Mission Path
  Public RCpath$               'RCpath$ = MNpath$ + "RC\", Retrieval Coefficient Path
  Public MPpath$               'MPpath$ = MNpath$ + "MP\", MP File Path
  Public Path$                 'Path$ = MNpath$ + "\" + yyyymmdd$ + "\", Flight Path
  Public PathDot$              'PathDot$ = Path$ + AC$ + yyyymmdd$ + "."
  Public Pgm$                  'Program Directory Path (length of <=40 characters)
  Public RCs$                  'RC preamble (eg "ERC", "DRC") RCs$ = Left$(AC$, 1) + "RC"
  
' Default Logical Unit Assignments
  Public BINlu%           'Default BIN File logical unit
  Public CALlu%           'Default CAL File logical unit
  Public CTClu%           'Default CTC File logical unit
  Public ERFlu%           'Default ERF File logical unit
  Public INPlu%           'Default INPUT logical unit
  Public LOGlu%           'Default LOG File logical unit
  Public MMSlu%           'Default MMS File logical unit (or any NAV File)
  Public MPFlu%           'MP File logical unit
  Public RAOBlu%          'RAOB File logical unit
  Public RAWlu%           'Default RAW File logical unit
  Public REFlu%           'Default REF File logical unit
  Public RTSlu%           'Default Real Time Simulation logical unit
  Public TXTlu%           'Default TXT File
  Public OUTlu%           'Default OUTPUT logical unit
  Public USElu%           'USE File logical unit
  
' Default File Names
  Public BINfile$         'BIN file
  Public CALfile$         'CAL file
  Public CTCfile$         'CTC file
  Public ERFfile$         'ERF file
  Public INPfile$         'INP file
  Public LOGfile$         'LOG file
  Public MMSfile$         'MMS file (any source of Nav data)
  Public MPfile$          'MP file
  Public RAOBfile$        'RAOB file
  Public RAWfile$         'RAW file
  Public REFfile$         'REF file
  Public RTSfile$         'RTS file
  Public txtFile$         'TXT file
  Public OUTfile$         'OUT file
  Public USEfile$         'File containing formal retrieval temperature errors

' Parameters used in Message Box calls
  Public Msg$             'Actual message
  Public response         'Variant answer to message box
  Public Style            'Define buttons, eg vbYesNo
  Public Title$           'eg  WARNING!
  Public Help$            'Name of help file
  Public Ctxt             'Message context
