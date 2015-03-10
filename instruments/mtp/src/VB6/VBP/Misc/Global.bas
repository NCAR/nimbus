Attribute VB_Name = "Global"
Option Explicit

Public Const APPKEY = "Sync"
Public Const TS_FID = "\TimeServers.txt"
Public Const GMT = "GMT"

Public Const ROOT_DESC = "Time Servers"

Public Const KEY_ROOT = "ROOT"
Public Const KEY_STATE = "XS"
Public Const KEY_COUNTRY = "XC"
Public Const KEY_HOST = "XH"
Public Const KEY_DUMMY = "XD"

Public Const ACTIVITY_LIM = 100
Public Const TIMEOUT_VAL = 5

Public Const AF_INET = 2

Public Const PORT_SNTP = 123
Public Const PORT_TIME = 37
Public Const PORT_DAYTIME = 13

Public Const SNTP_LI_MASK = &HC0
Public Const SNTP_VN_MASK = &H38
Public Const SNTP_MODE_MASK = 7

Private Const JAN_1970 = 2208988800#  ' 1970 - 1900 in seconds */
Private Const NTP_SCALE = 4294967296#  ' 2^32, of course! */

Private Const NTP_MODE_CLIENT = 3 ' NTP client mode */
Private Const NTP_MODE_SERVER = 4 ' NTP server mode */
Private Const NTP_VERSION = 3 ' The current version */
Private Const NTP_VERSION_MIN = 1 ' The minum valid version */
Private Const NTP_VERSION_MAX = 4 ' The maximum valid version */
Private Const NTP_STRATUM_MIN = 1 ' The minum valid stratum */
Private Const NTP_STRATUM_MAX = 15 ' The maximum valid stratum */

Public Enum SYNC_PROTO
    SYNC_SNTP = 1
    SYNC_DAYTIME
    SYNC_TIME
End Enum

Public Enum ACTIVITY_CODE
    ACTIVITY_AUTO = &H10000
    ACTIVITY_CLIENT = &H20000
    ACTIVITY_SERVER = &H40000
    ACTIVITY_EDIT = &H80000
    ACTIVITY_ROLE_MASK = &HFF0000

    ACTIVITY_TEST = &H100
    ACTIVITY_SYNC = &H200
    ACTIVITY_DETAILS = &H800
    ACTIVITY_FUNCTION_MASK = &HFF00&

    ACTIVITY_SNTP = &H1
    ACTIVITY_TIME = &H2
    ACTIVITY_DAYTIME = &H4
    ACTIVITY_PROTO_MASK = &HFF&

    ACTIVITY_LOG_DETAIL = &H1000000
    ACTIVITY_DEBUG = &H80000000

    ACTIVITY_AUTO_SYNC = ACTIVITY_AUTO Or ACTIVITY_SNTP Or ACTIVITY_SYNC
    ACTIVITY_AUTO_TEST = ACTIVITY_AUTO Or ACTIVITY_SNTP Or ACTIVITY_TEST
    ACTIVITY_AUTO_DETAILS = ACTIVITY_AUTO Or ACTIVITY_DETAILS

    ACTIVITY_CLIENT_SNTP = ACTIVITY_CLIENT Or ACTIVITY_SNTP
    ACTIVITY_CLIENT_SNTP_SYNC = ACTIVITY_CLIENT Or ACTIVITY_SNTP Or ACTIVITY_SYNC Or ACTIVITY_LOG_DETAIL
    ACTIVITY_CLIENT_SNTP_TEST = ACTIVITY_CLIENT Or ACTIVITY_SNTP Or ACTIVITY_TEST Or ACTIVITY_LOG_DETAIL
    
    ACTIVITY_CLIENT_DAYTIME = ACTIVITY_CLIENT Or ACTIVITY_DAYTIME
    ACTIVITY_CLIENT_DAYTIME_SYNC = ACTIVITY_CLIENT Or ACTIVITY_DAYTIME Or ACTIVITY_SYNC Or ACTIVITY_LOG_DETAIL
    ACTIVITY_CLIENT_DAYTIME_TEST = ACTIVITY_CLIENT Or ACTIVITY_DAYTIME Or ACTIVITY_TEST Or ACTIVITY_LOG_DETAIL

    ACTIVITY_CLIENT_TIME = ACTIVITY_CLIENT Or ACTIVITY_TIME
    ACTIVITY_CLIENT_TIME_SYNC = ACTIVITY_CLIENT Or ACTIVITY_TIME Or ACTIVITY_SYNC Or ACTIVITY_LOG_DETAIL
    ACTIVITY_CLIENT_TIME_TEST = ACTIVITY_CLIENT Or ACTIVITY_TIME Or ACTIVITY_TEST Or ACTIVITY_LOG_DETAIL
    
    ACTIVITY_CLIENT_DETAILS = ACTIVITY_CLIENT Or ACTIVITY_DETAILS

    ACTIVITY_SERVER_SNTP = ACTIVITY_SERVER Or ACTIVITY_SNTP Or ACTIVITY_SYNC
    ACTIVITY_SERVER_DAYTIME = ACTIVITY_SERVER Or ACTIVITY_DAYTIME Or ACTIVITY_SYNC
    ACTIVITY_SERVER_TIME = ACTIVITY_SERVER Or ACTIVITY_TIME Or ACTIVITY_SYNC
    ACTIVITY_SERVER_DETAILS = ACTIVITY_SERVER Or ACTIVITY_DETAILS

    ACTIVITY_EDIT_SYNC = ACTIVITY_EDIT Or ACTIVITY_SNTP Or ACTIVITY_SYNC
    ACTIVITY_EDIT_TEST = ACTIVITY_EDIT Or ACTIVITY_SNTP Or ACTIVITY_TEST
End Enum

Public Enum SERVER_COLS
    COL_ENABLED = 0
    COL_COUNTRYCODE = 0
    COL_COUNTRY
    COL_STATE
    COL_HOSTNAME
    COL_COORDS
    COL_SERVICEAREA
    COL_POLICY
    COL_SYNC
    COL_CONFIG
    COL_CONTACT

    COL_STRATUM

    COL_SNTP
    COL_TIME
    COL_DAYTIME
    COL_DAYTIME_FORMAT

    COL_ROUNDTRIP_SORT
    COL_ROUNDTRIP
    COL_LOCATION
    COL_MAX = COL_LOCATION
End Enum

'Public Type SNTP_SERVER
'    Enabled As Boolean
'    CountryCode As String
'    Country As String
'    State As String
'    HostName As String
'    Location As String
'    Coords As String
'    ServiceArea As String
'    Policy As String
'    Sync As String
'    Config As String
'    Contact As String
'    SNTP As Boolean
'    Stratum As Long
'    Time As Boolean
'    Daytime As Boolean
'    DaytimeFormat As DAYTIME_FORMAT
'    RoundTrip As Variant
'End Type

Public Type SNTP_SECS32
    Whole As Integer
    Frac As Integer
End Type

Public Type SNTP_TIMESTAMP
    Data(7) As Byte
End Type

Public Type SNTP_MESSAGE
    LiVnMode As Byte
    Stratum As Byte
    Poll As Byte
    Precision As Byte
    RootDelay As SNTP_SECS32
    RootDispersion As SNTP_SECS32
    ReferenceID(3) As Byte
    ReferenceTimeStamp As SNTP_TIMESTAMP
    OriginateTimeStamp As SNTP_TIMESTAMP
    ReceiveTimeStamp As SNTP_TIMESTAMP
    TransmitTimeStamp As SNTP_TIMESTAMP
'    KeyID As Long
'    MessageDigest(127) As Byte
End Type

Public Type SYSTEMTIME
    wYear As Integer
    wMonth As Integer
    wDayOfWeek As Integer
    wDay As Integer
    wHour As Integer
    wMinute As Integer
    wSecond As Integer
    wMilliseconds As Integer
End Type

Public Declare Sub GetSystemTime Lib "kernel32" (lpSystemTime As SYSTEMTIME)

'Public Declare Function SetSystemTime Lib "kernel32" ( _
    lpSystemTime As SYSTEMTIME) As Long

'Public Reg As Registry
'Public ActiveTz As New TimeZone
'Public AutoServer As TimeServer
'Public ClientServer As TimeServer
'Public TsCnt As Long
'Public Tss() As TimeServer
'Public EditCancelled As Boolean

Public Function TimeStampToSecs(ts As SNTP_TIMESTAMP) As Variant
    Dim i As Long
    Dim lim As Long
    Dim d As Variant
'            For i = 0 To 3
'                intPart = Int(256 * intPart + SNTPData(offset + i))
'            Next
    d = CDec(0)
    lim = UBound(ts.Data)
    For i = 0 To lim
        d = Int((256 * d) + ts.Data(i))
'        for (i = 0, d = 0.0; i < 8; ++i)
'        d = 256.0*d+packet[NTP_REFERENCE+i];
    Next i
    TimeStampToSecs = d / NTP_SCALE
End Function

Public Function DispSystemTime(st As SYSTEMTIME) As String
    Dim Disp As String
    Dim i As Long
    Dim secs As String
    Dim msecs As String

    Disp = FormatDateTime(DateSerial(st.wYear, st.wMonth, st.wDay) + TimeSerial(st.wHour, st.wMinute, st.wSecond), vbGeneralDate)
    secs = Format(st.wSecond, "00")
    i = InStrRev(Disp, secs) - 1
    If i <= 0 Then
        DispSystemTime = Disp
        Exit Function
    End If
    msecs = Format(st.wMilliseconds, "000")
    DispSystemTime = Left(Disp, i) & secs & "." & msecs & Mid(Disp, i + 3)
End Function

