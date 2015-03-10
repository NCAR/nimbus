Attribute VB_Name = "Global"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/

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

Public Declare Sub GetSystemTime Lib "kernel32" ( _
    lpSystemTime As SYSTEMTIME)

Public Declare Function SetSystemTime Lib "kernel32" ( _
    lpSystemTime As SYSTEMTIME) As Long

Public Reg As Registry
Public ActiveTz As New TimeZone
Public AutoServer As TimeServer
Public ClientServer As TimeServer
Public TsCnt As Long
Public Tss() As TimeServer
Public EditCancelled As Boolean
Public Function SecsToTimeStamp(secs As Variant) As SNTP_TIMESTAMP
    Dim i As Long
    Dim ts As SNTP_TIMESTAMP
    Dim d As Variant
    Dim k As Long
    Dim lim As Long

    d = secs / NTP_SCALE
    lim = UBound(ts.Data)
    For i = 0 To lim
        d = d * 256
        k = Int(d)
        If k >= 256 Then k = 255
        ts.Data(i) = k
        d = d - k
'        if ((k = (int)(d *= 256.0)) >= 256) k = 255;
'        packet[NTP_ORIGINATE+i] = k;
'        d -= k;
    Next i ' }
    SecsToTimeStamp = ts
End Function
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
Public Sub LabelAlign(lbl As Label, Ctl As Control)
    Dim adj As Single

    If Ctl.Height > lbl.Height Then
        adj = (Ctl.Height - lbl.Height) / 2
        lbl.Top = Ctl.Top + adj
    ElseIf lbl.Height > Ctl.Height Then
        adj = (lbl.Height - Ctl.Height) / 2
        Ctl.Top = lbl.Top + adj
    Else
        lbl.Top = Ctl.Top
    End If
End Sub
Public Function ActivityDest(ActivityCode As ACTIVITY_CODE) As ListView
    Dim lv As ListView

    Set lv = Nothing
    Select Case ActivityCode And ACTIVITY_ROLE_MASK
        Case ACTIVITY_CLIENT:
            Set lv = frmMain.lvClientLog
        Case ACTIVITY_AUTO:
            Set lv = frmMain.lvAutoLog
        Case ACTIVITY_SERVER:
            Set lv = frmMain.lvServerLog
        Case Else:
            Set lv = Nothing
    End Select

    Set ActivityDest = lv
End Function
Public Sub ActivityLog(ActivityCode As ACTIVITY_CODE, msg As String, Optional Desc As String = "")
    Dim lv As ListView
    Dim LstItm As ListItem
    Dim s As String

    Set lv = ActivityDest(ActivityCode)
    If lv Is Nothing Then Exit Sub

    Do While lv.ListItems.Count >= ACTIVITY_LIM
        lv.ListItems.Remove 1
    Loop

    Set LstItm = lv.ListItems.Add()
    LstItm.SubItems(1) = CDate(Now)
    s = Trim(Desc)
    If Len(s) > 0 Then
        LstItm.SubItems(2) = s
    Else
        LstItm.SubItems(2) = DispProtocol(ActivityCode)
    End If
    LstItm.SubItems(3) = msg
    lvEnsureVisible lv, LstItm
    Set lv.SelectedItem = LstItm
End Sub
Public Sub ActivityAdjust(ActivityCode As ACTIVITY_CODE)
    Dim lv As ListView

    Set lv = ActivityDest(ActivityCode)
    If lv Is Nothing Then Exit Sub

    LvAdjust lv
End Sub
Public Sub ActivityClear(ActivityCode As ACTIVITY_CODE)
    Dim lv As ListView

    Set lv = ActivityDest(ActivityCode)
    If lv Is Nothing Then Exit Sub

    lv.ListItems.Clear
End Sub
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
Public Function DispProtocol(ActivityCode As ACTIVITY_CODE) As String
    Dim s As String

    If ActivityCode And ACTIVITY_DEBUG Then
        s = "*"
    Else
        s = ""
    End If
    Select Case ActivityCode And ACTIVITY_PROTO_MASK
        Case ACTIVITY_DAYTIME:
            s = s & "DAYTIME"
        Case ACTIVITY_SNTP:
            s = s & "SNTP"
        Case ACTIVITY_TIME:
            s = s & "TIME"
    End Select

    Select Case ActivityCode And ACTIVITY_FUNCTION_MASK
        Case ACTIVITY_DETAILS:
            s = s & "Server Details"
    End Select

    DispProtocol = s
End Function
Public Function JoinSckTag(TimeOut As Long, ActivityCode As ACTIVITY_CODE, ServerInx As Long) As String
    JoinSckTag = CStr(TimeOut) & vbTab & CStr(ActivityCode) & vbTab & CStr(ServerInx)
End Function
Public Sub SplitSckTag(Tag As String, ByRef TimeOut As Long, ByRef ActivityCode As ACTIVITY_CODE, ByRef ServerInx As Long)
    Dim sa() As String

    sa = Split(Tag, vbTab)
    TimeOut = CLng(sa(0))
    ActivityCode = CLng(sa(1))
    ServerInx = CLng(sa(2))
End Sub
Public Function NodeIsRoot(n As MSComctlLib.Node)
    NodeIsRoot = (n.Key = KEY_ROOT)
End Function
Public Function NodeIsCountry(n As MSComctlLib.Node)
    NodeIsCountry = (Left(n.Key, Len(KEY_COUNTRY)) = KEY_COUNTRY)
End Function
Public Function NodeIsState(n As MSComctlLib.Node)
    NodeIsState = (Left(n.Key, Len(KEY_STATE)) = KEY_STATE)
End Function
Public Function NodeIsHost(n As MSComctlLib.Node)
    NodeIsHost = (Left(n.Key, Len(KEY_HOST)) = KEY_HOST)
End Function
Public Sub LoadServers(AutoHostName, ClientHostName)
    Dim fn As Long
    Dim s As String
    Dim sa() As String
    Dim b As Boolean
    Dim Fid As String
    Dim i As Long
    Dim ts As TimeServer

    Fid = App.Path & TS_FID
    fn = FreeFile()
    On Error Resume Next
    Open Fid For Input Access Read As fn

    If Err.Number <> 0 Then
        Open Fid For Output Access Write As fn
        ResDataOpen "DefServers"
        While Not ResDataEOF()
            s = ResDataReadLine()
            Print #fn, s
        Wend
        ResDataClose
        Close fn
        Err.Clear
        fn = FreeFile()
        Open Fid For Input Access Read As fn
    End If

    If Err.Number <> 0 Then Exit Sub

    Do While Not EOF(fn)
        Line Input #fn, s
        Set ts = New TimeServer
        ts.Serialize = s
        ReDim Preserve Tss(TsCnt)
        ts.Index = TsCnt
        Set Tss(TsCnt) = ts
        TsCnt = TsCnt + 1
        If StrComp(ts.HostName, AutoHostName, vbTextCompare) = 0 Then
            Set AutoServer = ts
        End If
        If StrComp(ts.HostName, ClientHostName, vbTextCompare) = 0 Then
            Set ClientServer = ts
        End If
    Loop
    Close fn
End Sub
Public Sub SaveServers()
    Dim LstItm As ListItem
    Dim fn As Long
    Dim s As String
    Dim i As Long

    fn = FreeFile()
    On Error Resume Next
    Open App.Path & TS_FID For Output Access Write As fn

    For i = 0 To TsCnt - 1
        s = Tss(i).Serialize
        Print #fn, s
    Next i
    Close fn
End Sub

