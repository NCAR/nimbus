VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "TimeZone"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 2.25 6/2/2004

Private Const REG_NT_TIMEZONES = "SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones"
Private Const REG_9x_TIMEZONES = "SOFTWARE\Microsoft\Windows\CurrentVersion\Time Zones"
Private Const REG_DST_SUBKEY = "SYSTEM\CurrentControlSet\Control\TimeZoneInformation"
Private Const REG_DST_DATANAME = "DisableAutoDaylightTimeSet"

Private Const STANDARD_NT_SUFFIX = " Standard Time"
Private Const STANDARD_9x_SUFFIX = ""
Private Const DST_NT_SUFFIX = " Daylight Saving Time"
Private Const DST_9x_SUFFIX = " Daylight Saving Time"
Private Const TIME_SUFFIX = " Time"
Private Const DST_SUFFIX = " Daylight Time"
Private Const STD_SUFFIX = " Standard Time"
Private Const GMT = "GMT"
Private Const CLT = "Current Local Time"

Public Enum TIME_ZONE_SORT
    TZ_SORT_DESC
    TZ_SORT_GMT
    TZ_SORT_NAME
End Enum

Private Enum TIME_ZONE_ID
    TIME_ZONE_ID_UNKNOWN = 0
    TIME_ZONE_ID_STANDARD = 1
    TIME_ZONE_ID_DAYLIGHT = 2
    TIME_ZONE_ID_INVALID = &HFFFFFFFF
End Enum

Private Enum SER_POS
    POS_SVERSION = 0
    POS_Name
    POS_Desc
    POS_Deleted
    POS_Hidden
    POS_Dirty

    POS_DstEnabled
    POS_DstBiasGMT
    POS_DstBeg
    POS_DstDummy1
    POS_DstDummy2
    POS_DstDummy3
    POS_DstDummy4

    POS_StdBiasGMT
    POS_StdBeg
    POS_StdDummy1
    POS_StdDummy2
    POS_StdDummy3
    POS_StdDummy4
    POS_MapId
    POS_Index

    POS_ReadOnly
    POS_OrigActive
    POS_PrevName

    POS_SNTP_SUPPORTEDServer
    POS_Dummy2
    POS_Dummy3
    POS_Dummy4
    POS_Dummy5
    POS_UBOUND = POS_Dummy5
End Enum

' Code Page Default Values.
Private Enum CODE_PAGE_DEFAULT
    CP_ACP = 0
    CP_OEMCP = 1
    CP_MACCP = 2
    CP_THREAD_ACP = 3
    CP_SYMBOL = 42
    CP_UTF7 = 65000
    CP_UTF8 = 65001
End Enum

'  MBCS and Unicode Translation Flags.
Private Enum MBCS_FLAG
    MB_PRECOMPOSED = &H1                     ' use precomposed chars
    MB_COMPOSITE = &H2                       ' use composite chars
    MB_USEGLYPHCHARS = &H4                   ' use glyph chars, not ctrl chars
    MB_ERR_INVALID_CHARS = &H8               ' error for invalid chars
    
    WC_COMPOSITECHECK = &H200                ' convert composite to precomposed
    WC_DISCARDNS = &H10                      ' discard non-spacing chars
    WC_SEPCHARS = &H20                       ' generate separate chars
    WC_DEFAULTCHAR = &H40                    ' replace w/ default char
    
    WC_NO_BEST_FIT_CHARS = &H400             ' do not use best fit chars
End Enum

Private Type SYSTEMTIME
    wYear As Integer
    wMonth As Integer
    wDayOfWeek As Integer
    wDay As Integer
    wHour As Integer
    wMinute As Integer
    wSecond As Integer
    wMilliseconds As Integer
End Type

Private Type TIME_ZONE_INFORMATION
    Bias As Long
    StandardName(63) As Byte
    StandardDate As SYSTEMTIME
    StandardBias As Long
    DaylightName(63) As Byte
    DaylightDate As SYSTEMTIME
    DaylightBias As Long
End Type

Private Type REG_TIME_ZONE_INFORMATION
    Bias As Long
    StandardBias As Long
    DaylightBias As Long
    StandardDate As SYSTEMTIME
    DaylightDate As SYSTEMTIME
End Type

Private Declare Sub GetSystemTime Lib "kernel32" ( _
    lpSystemTime As SYSTEMTIME)

Private Declare Function GetTimeZoneInformation Lib "kernel32" ( _
    lpTimeZoneInformation As TIME_ZONE_INFORMATION) As Long

Private Declare Function SetTimeZoneInformation Lib "kernel32" ( _
    lpTimeZoneInformation As TIME_ZONE_INFORMATION) As Long

Private Declare Function MultiByteToWideChar Lib "kernel32" ( _
    ByVal CodePage As CODE_PAGE_DEFAULT, _
    ByVal dwFlags As MBCS_FLAG, _
    ByRef lpMultiByteStr As Byte, _
    ByVal cchMultiByte As Long, _
    ByRef lpWideCharStr As Byte, _
    ByVal cchWideChar As Long) As Long

Private Declare Function WideCharToMultiByte Lib "kernel32" ( _
    ByVal CodePage As CODE_PAGE_DEFAULT, _
    ByVal dwFlags As MBCS_FLAG, _
    ByVal lpWideCharStr As String, _
    ByVal cchWideChar As Long, _
    ByVal lpMultiByteStr As String, _
    ByVal cchMultiByte As Long, _
    ByVal lpDefaultChar As String, _
    ByVal lpUsedDefaultChar As Long) As Long

'local variable(s) to hold property value(s)
Private mvarName As String
Private mvarDeleted As Boolean
Private mvarDirty As Boolean
Private mvarHidden As Boolean
Private mvarDesc As String
Private mvarDstBiasGMT As Long
Private mvarStdBiasGMT As Long
Private mvarBiasGMT As Long
Private mvarBiasLocal As Long
Private mvarDstEnabled As Boolean
Private mvarDstBeg As ZoneRule
Private mvarStdBeg As ZoneRule
Private mvarSntpServer As String
Private mvarMapId As String
Private mvarIndex As Long
Private mvarReadOnly As Boolean
Private mvarOrigActive As Boolean
Private mvarPrevName As String

Private mvarTimeZoneRegKey As String

Private StandardSuffix As String
Private DaylightSuffix As String
Public Function Equals(tz As TimeZone) As Boolean
Attribute Equals.VB_Description = "Is this time zone identical to another?"
    Equals = False

    If StrComp(tz.Name, Name, vbTextCompare) <> 0 Then
        If StrComp(tz.Name, mvarPrevName, vbTextCompare) <> 0 Then Exit Function
    End If
    If tz.DstBiasGMT <> mvarDstBiasGMT Then Exit Function
    If tz.StdBiasGMT <> mvarStdBiasGMT Then Exit Function
    If tz.DstEnabled <> mvarDstEnabled Then Exit Function
    If tz.DstBeg.EffectiveDate <> mvarDstBeg.EffectiveDate Then Exit Function
    If tz.StdBeg.EffectiveDate <> mvarStdBeg.EffectiveDate Then Exit Function

    Equals = True
End Function
Public Property Get TimeZoneRegKey() As String
Attribute TimeZoneRegKey.VB_Description = "The Registry key where Windows stores time zone information on this computer."
    TimeZoneRegKey = mvarTimeZoneRegKey
End Property
Public Property Get Active() As Boolean
Attribute Active.VB_Description = "Is this the current time zone for this computer?"
    Dim ltz As TimeZone

    Set ltz = New TimeZone
    Active = Equals(ltz)
End Property
Public Property Let Deleted(ByVal vData As Boolean)
Attribute Deleted.VB_Description = "Is this time zone deleted/disabled?"
    If vData And mvarReadOnly Then
        If mvarDeleted <> False Then
            mvarDeleted = False
            mvarDirty = True
        End If
        If Not mvarHidden = True Then
            mvarHidden = True
            mvarDirty = True
        End If
    Else
        If mvarDeleted <> vData Then
            mvarDeleted = vData
            mvarDirty = True
        End If
    End If
End Property
Public Property Get SntpServer() As String
    SntpServer = mvarSntpServer
End Property
Public Property Let SntpServer(NewServer As String)
    mvarSntpServer = NewServer
End Property
Public Property Get Deleted() As Boolean
    Deleted = mvarDeleted
End Property
Public Property Get Dirty() As Boolean
    Dirty = mvarDirty
End Property
Public Property Let Hidden(ByVal vData As Boolean)
Attribute Hidden.VB_Description = "Should this time zone be hidden from the user?"
    If mvarHidden <> vData Then
        mvarHidden = vData
        mvarDirty = True
    End If
End Property
Public Property Get Hidden() As Boolean
    Hidden = mvarHidden
End Property
Public Property Let MapId(ByVal NewMapId As String)
Attribute MapId.VB_Description = "Windows' MapId for this time zone."
    If mvarMapId <> NewMapId Then
        mvarMapId = NewMapId
        mvarDirty = True
    End If
End Property
Public Property Get MapId() As String
    MapId = mvarMapId
End Property
Public Property Let Index(ByVal NewIndex As Long)
Attribute Index.VB_Description = "Windows' Index value for this time zone."
    If mvarIndex <> NewIndex Then
        mvarIndex = NewIndex
        mvarDirty = True
    End If
End Property
Public Property Get Index() As Long
    Index = mvarIndex
End Property
Public Property Get PrevName() As String
Attribute PrevName.VB_Description = "The name given this time zone the last time it was committed.  If the name changes, information under this old name will be deleted before information is added under the new name."
    PrevName = mvarPrevName
End Property
Public Property Let Name(ByVal vData As String)
Attribute Name.VB_Description = "The unique name for this time zone (should not end in ""Time"", ""Standard Time"" or ""Daylilght Time"")."
    Dim s As String
    Dim Prev As String

    s = Trim(vData) & " "
    s = Replace(s, " Time ", "", , , vbTextCompare)
    s = s & " "
    s = Replace(s, " Standard ", "", , , vbTextCompare)
    s = s & " "
    s = Replace(s, " Daylight ", "", , , vbTextCompare)
    Do While Prev <> s
        Prev = s
        s = Replace(s, "  ", " ")
    Loop

    mvarName = Trim(s)
    If Len(mvarDesc) <= 0 Then mvarDesc = mvarName & TIME_SUFFIX

    mvarDirty = True
End Property
Public Property Get Name() As String
    Name = mvarName & TIME_SUFFIX
End Property
Public Property Get RawName() As String
    RawName = mvarName
End Property
Public Property Get DstName() As String
Attribute DstName.VB_Description = "Name of this time zone when Daylight Saving Time is in effect (usually, Name + "" Daylight Saving Time"")."
    DstName = mvarName & DST_SUFFIX
End Property
Public Property Get StdName() As String
Attribute StdName.VB_Description = "The name of this time zone when Standard Time is in effect."
    StdName = mvarName & STD_SUFFIX
End Property
Private Sub StringByteArray(Text As String, ByRef Bytes() As Byte)
    Dim b() As Byte
    Dim i As Long
    Dim lim As Long

    b = Text
    lim = UBound(Bytes)
    ReDim Preserve b(lim)
    For i = 0 To lim
        Bytes(i) = b(i)
    Next i
End Sub
Public Function DispBias(Bias As Long, Optional Prefix As String = GMT) As String
Attribute DispBias.VB_Description = "Format a bias/offset value."
    Dim mins As Long
    Dim hrs As Long
    Dim Sign As String
    Dim s As String

    If Bias < 0 Then
        Sign = "-"
    Else
        Sign = "+"
    End If

    mins = Abs(Bias Mod 60)
    hrs = Abs(Bias \ 60)

    s = Sign & Format(hrs, "00") & ":" & Format(mins, "00")
    If Len(Prefix) > 0 Then
        s = Prefix & s
    End If

    DispBias = s
End Function
Public Function DispZoneShort(SortOrder As TIME_ZONE_SORT) As String
Attribute DispZoneShort.VB_Description = "Return string that provides short one-line description of this time zone."
    Dim s As String
    Dim Bias As String

    s = ""
    If SortOrder = TZ_SORT_DESC Then
        s = s & mvarDesc '& " - " & Name
    Else
        s = s & Name '& " - " & mvarDesc
    End If

    If DstAvailable Then
        Bias = DispBias(mvarStdBiasGMT) & "/" & DispBias(mvarDstBiasGMT) & ")"
    Else
        Bias = DispBias(mvarStdBiasGMT) & ")"
    End If

    If SortOrder = TZ_SORT_GMT Then
        s = Bias & " " & s
    Else
        s = s & " (" & Bias & ")"
    End If

    If mvarHidden Then
        s = s & " (DISABLED)"
    End If

    DispZoneShort = s
End Function
Public Function DispZoneLong(SortOrder As TIME_ZONE_SORT) As String
Attribute DispZoneLong.VB_Description = "Return string that provides long one-line description of this time zone."
    Dim s As String
    Dim Bias As String

    s = ""
    If SortOrder = TZ_SORT_DESC Then
        s = s & mvarDesc & " - " & Name
    Else
        s = s & Name & " - " & mvarDesc
    End If

    If DstAvailable Then
        Bias = DispBias(mvarStdBiasGMT) & " / " & DispBias(mvarDstBiasGMT)
    Else
        Bias = DispBias(mvarStdBiasGMT)
    End If

    If SortOrder = TZ_SORT_GMT Then
        s = Bias & " " & s
    Else
        s = s & " (" & Bias & ")"
    End If

    If mvarHidden Then
        s = s & " (DISABLED)"
    ElseIf mvarDeleted Then
        s = s & " (DELETED)"
    End If

    DispZoneLong = s
End Function
Public Function Activate() As Boolean
Attribute Activate.VB_Description = "Make this time zone the current time zone for this computer."
    Dim PrevTz As TimeZone
    Dim tzi As TIME_ZONE_INFORMATION
    Dim st As SYSTEMTIME
    Dim result As Long
    Dim i As Long
    Dim s As String
    Dim d As Date
    Dim Reg As Registry

    Set Reg = New Registry
    If mvarDstEnabled Then
        Reg.DeleteValue REG_DST_DATANAME, REG_DST_SUBKEY, HKEY_LOCAL_MACHINE
    Else
        Reg.WriteValue REG_DST_DATANAME, CLng(True), REG_DST_SUBKEY, HKEY_LOCAL_MACHINE
    End If

    StringByteArray StdName, tzi.StandardName
    StringByteArray DstName, tzi.DaylightName

    tzi.Bias = mvarStdBiasGMT * (-1)
    tzi.StandardBias = 0
    tzi.DaylightBias = (mvarDstBiasGMT - mvarStdBiasGMT) * (-1)

    tzi.StandardDate.wYear = 0
    tzi.StandardDate.wMonth = mvarStdBeg.Month
    tzi.StandardDate.wDay = mvarStdBeg.Week
    tzi.StandardDate.wDayOfWeek = mvarStdBeg.DayOfWeek
    tzi.StandardDate.wHour = mvarStdBeg.Hour
    tzi.StandardDate.wMinute = mvarStdBeg.Minute
    tzi.StandardDate.wSecond = mvarStdBeg.Second

    tzi.DaylightDate.wYear = 0
    tzi.DaylightDate.wMonth = mvarDstBeg.Month
    tzi.DaylightDate.wDay = mvarDstBeg.Week
    tzi.DaylightDate.wDayOfWeek = mvarDstBeg.DayOfWeek
    tzi.DaylightDate.wHour = mvarDstBeg.Hour
    tzi.DaylightDate.wMinute = mvarDstBeg.Minute
    tzi.DaylightDate.wSecond = mvarDstBeg.Second

    If Not mvarDstEnabled Then
        tzi.StandardDate.wMonth = 0
        tzi.DaylightDate.wMonth = 0
        tzi.DaylightBias = tzi.StandardBias
    End If

    Set PrevTz = New TimeZone
    result = SetTimeZoneInformation(tzi)
    If result = 0 Then
        Activate = False
    Else    ' success
        Activate = True
        If ApiWinVersion.Platform9x Then ' adjust clock
            d = Now()
            d = DateAdd("n", BiasGMT - PrevTz.BiasGMT, d)
            Time = d
            Date = d
        End If
    End If
End Function
Public Sub Commit()
Attribute Commit.VB_Description = "Write this time zone's information to the Windows Registry."
    Dim SubKey As String
    Dim rtzi As REG_TIME_ZONE_INFORMATION
    Dim st As SYSTEMTIME
    Dim buf() As Byte
    Dim Reg As Registry

    If Not mvarDirty Then Exit Sub

    mvarDirty = False
    If mvarReadOnly Then Exit Sub

    Set Reg = New Registry
    If Len(mvarPrevName) > 0 Then ' must remove old Registry entry
        SubKey = mvarTimeZoneRegKey & "\" & mvarPrevName
        If ApiWinVersion.PlatformNT Then SubKey = SubKey & STD_SUFFIX
        Reg.DeleteKey SubKey, HKEY_LOCAL_MACHINE
    End If

    If mvarDeleted Then Exit Sub

    SubKey = mvarTimeZoneRegKey & "\" & mvarName
    If ApiWinVersion.PlatformNT Then SubKey = SubKey & STD_SUFFIX
    Reg.WriteValue "Std", StdName, SubKey, HKEY_LOCAL_MACHINE
    Reg.WriteValue "Dlt", DstName, SubKey, HKEY_LOCAL_MACHINE
    Reg.WriteValue "Display", "(" & DispBias(mvarStdBiasGMT) & ") " & mvarDesc, SubKey, HKEY_LOCAL_MACHINE

    If Len(mvarMapId) > 0 Then
        Reg.WriteValue "MapID", mvarMapId, SubKey, HKEY_LOCAL_MACHINE
    End If

    If mvarIndex >= 0 Then
        Reg.WriteValue "Index", mvarIndex, SubKey, HKEY_LOCAL_MACHINE
    End If

    rtzi.Bias = mvarStdBiasGMT * (-1)
    rtzi.StandardBias = 0
    rtzi.DaylightBias = (mvarDstBiasGMT - mvarStdBiasGMT) * (-1)

    rtzi.StandardDate.wYear = 0
    rtzi.StandardDate.wMonth = mvarStdBeg.Month
    rtzi.StandardDate.wDay = mvarStdBeg.Week
    rtzi.StandardDate.wDayOfWeek = mvarStdBeg.DayOfWeek
    rtzi.StandardDate.wHour = mvarStdBeg.Hour
    rtzi.StandardDate.wMinute = mvarStdBeg.Minute
    rtzi.StandardDate.wSecond = mvarStdBeg.Second

    rtzi.DaylightDate.wYear = 0
    rtzi.DaylightDate.wMonth = mvarDstBeg.Month
    rtzi.DaylightDate.wDay = mvarDstBeg.Week
    rtzi.DaylightDate.wDayOfWeek = mvarDstBeg.DayOfWeek
    rtzi.DaylightDate.wHour = mvarDstBeg.Hour
    rtzi.DaylightDate.wMinute = mvarDstBeg.Minute
    rtzi.DaylightDate.wSecond = mvarDstBeg.Second

    Reg.WriteValue "TZI", ApiConvUdtByteArray(VarPtr(rtzi), LenB(rtzi)), SubKey, HKEY_LOCAL_MACHINE

    mvarPrevName = mvarName
End Sub
Public Sub LoadFromReg(RegKey As String)
    Dim s As String
    Dim i As Long
    Dim j As Long
    Dim k As Long
    Dim va As REG_VAL_ARRAY
    Dim rtzi As REG_TIME_ZONE_INFORMATION
    Dim st As SYSTEMTIME
    Dim Reg As Registry

    mvarDeleted = False
    Name = RegKey
    mvarDesc = mvarName & TIME_SUFFIX
    Set Reg = New Registry
    Set va = Reg.GetValues(mvarTimeZoneRegKey & "\" & RegKey, HKEY_LOCAL_MACHINE)
    For j = va.LowerBound To va.UpperBound
        Select Case UCase(va.Value(j).Name)
            Case "DISPLAY":
                s = va.Value(j).Data
                k = InStr(1, s, ")")
                s = Mid(s, k + 2)
                mvarDesc = s
'                tz.Desc = va.Value(j).Data
            Case "MAPID":
                mvarMapId = va.Value(j).Data
            Case "INDEX":
                mvarIndex = va.Value(j).Data
            Case "TZI":
                ApiConvByteArrayUdt VarPtr(rtzi), va.Value(j).Data
            Case "STD":
'                tz.ApiTzi.StandardName = va.Value(j).Data
            Case "DLT":
'                tz.ApiTzi.DaylightName = va.Value(j).Data
        End Select
    Next j

    mvarStdBiasGMT = (rtzi.Bias + rtzi.StandardBias) * (-1)
    mvarDstBiasGMT = (rtzi.Bias + rtzi.DaylightBias) * (-1)

    st = rtzi.DaylightDate
    mvarDstBeg.Begin st.wYear, st.wMonth, st.wDay, st.wDayOfWeek, st.wHour, st.wMinute, st.wSecond

    st = rtzi.StandardDate
    mvarStdBeg.Begin st.wYear, st.wMonth, st.wDay, st.wDayOfWeek, st.wHour, st.wMinute, st.wSecond
    If (mvarDstBiasGMT <> 0) And (rtzi.DaylightDate.wMonth <> 0) And (rtzi.StandardDate.wMonth <> 0) Then
        mvarDstEnabled = True
    Else
        mvarDstEnabled = False
    End If
    mvarReadOnly = True
    mvarDirty = False
End Sub
Public Function Clone() As TimeZone
Attribute Clone.VB_Description = "Make a new TimeZone object whose properties are initially exactly like this one."
    Dim tz As TimeZone

    Set tz = New TimeZone
    tz.Serialize(SERIAL_FORMAT_TSV) = Serialize(SERIAL_FORMAT_TSV)
    Set Clone = tz
End Function
Public Function Display(Optional FixAmps As Boolean = True) As String
Attribute Display.VB_Description = "Return multi-line string that describes this time zone."
    Dim s As String
    Dim StdBeg As Date
    Dim DstBeg As Date
    Dim Indent1 As String
    Dim Indent2 As String
    Dim StdDisp As String
    Dim DstDisp As String

    Indent1 = Space(4)
    Indent2 = Space(4)

    s = ""
    s = s & "Time Zone Name: " & Name & vbCrLf
    s = s & "Description: " & mvarDesc & vbCrLf
    s = s & "Standard Time Bias/Offset: " & DispBias(mvarStdBiasGMT) & " (" & DispBias(StdBiasLocal, CLT) & ")" & vbCrLf
    s = s & "Daylight Saving Time Enabled: "
    If mvarDstEnabled Then
        s = s & "Yes"
    Else
        s = s & "No"
    End If
    s = s & vbCrLf
    s = s & "Daylight Time Bias/Offset: " & DispBias(mvarDstBiasGMT) & " (" & DispBias(DstBiasLocal, CLT) & ")" & vbCrLf
    s = s & Indent1 & "Daylight Saving Time in Effect: "
    If DstActive Then
        s = s & "Yes"
    Else
        s = s & "No"
    End If
    s = s & vbCrLf

    StdBeg = mvarStdBeg.EffectiveDate
    DstBeg = mvarDstBeg.EffectiveDate
    StdDisp = mvarStdBeg.Display()
    DstDisp = mvarDstBeg.Display()
    If DstBeg < StdBeg Then ' northern hemisphere
        If DstActive Then
            If Len(DstDisp) > 0 Then s = s & Indent2 & "Daylight Saving Time Began: " & DstDisp & vbCrLf
            If Len(StdDisp) > 0 Then s = s & Indent2 & "Daylight Saving Time Ends: " & StdDisp & vbCrLf
        ElseIf Now() < DstBeg Then
            If Len(DstDisp) > 0 Then s = s & Indent2 & "Daylight Saving Time Begins: " & DstDisp & vbCrLf
            If Len(StdDisp) > 0 Then s = s & Indent2 & "Daylight Saving Time Ends: " & StdDisp & vbCrLf
        Else
            If Len(DstDisp) > 0 Then s = s & Indent2 & "Daylight Saving Time Began: " & DstDisp & vbCrLf
            If Len(StdDisp) > 0 Then s = s & Indent2 & "Daylight Saving Time Ended: " & StdDisp & vbCrLf
        End If
    Else
        If StdActive Then
            If Len(StdDisp) > 0 Then s = s & Indent2 & "Standard Time Began: " & StdDisp & vbCrLf
            If Len(DstDisp) > 0 Then s = s & Indent2 & "Standard Time Ends: " & DstDisp & vbCrLf
        ElseIf Now() < StdBeg Then
            If Len(StdDisp) > 0 Then s = s & Indent2 & "Standard Time Begins: " & StdDisp & vbCrLf
            If Len(DstDisp) > 0 Then s = s & Indent2 & "Standard Time Ends: " & DstDisp & vbCrLf
        Else
            If Len(StdDisp) > 0 Then s = s & Indent2 & "Standard Time Began: " & StdDisp & vbCrLf
            If Len(DstDisp) > 0 Then s = s & Indent2 & "Standard Time Ended: " & DstDisp & vbCrLf
        End If
    End If

    If FixAmps Then s = Replace(s, "&", "&&")
    Display = s
End Function
Private Function SerialDelim(SerialFormat As SERIAL_FORMAT) As String
    Select Case SerialFormat
        Case SERIAL_FORMAT_TSV:
            SerialDelim = vbTab
        Case SERIAL_FORMAT_CSV:
            SerialDelim = ","
        Case SERIAL_FORMAT_XML:
            ' not supported yet
        Case Else:
            SerialDelim = vbTab
    End Select
End Function
Public Property Let Serialize(Optional SerialFormat As SERIAL_FORMAT = SERIAL_FORMAT_TSV, SerialJob As String)
Attribute Serialize.VB_Description = "Return/Accept a text string containing all of this time zone's settings."
    Dim sa() As String
    Dim Rule As ZoneRule

    If Len(SerialJob) <= 0 Then
        Err.Raise SERIAL_EMPTY
        Exit Property
    End If

    sa = Split(SerialJob, SerialDelim(SerialFormat))
    If UBound(sa) < POS_UBOUND Then
        Err.Raise SERIAL_INVALID
        Exit Property
    End If
    If sa(POS_SVERSION) <> CStr(0) Then
        Err.Raise SERIAL_BAD_VERSION
        Exit Property
    End If

    mvarName = sa(POS_Name)
    mvarDeleted = sa(POS_Deleted)
    mvarHidden = sa(POS_Hidden)
    mvarDirty = sa(POS_Dirty)
    mvarDesc = sa(POS_Desc)

    mvarDstEnabled = sa(POS_DstEnabled)
    mvarDstBiasGMT = sa(POS_DstBiasGMT)
    mvarDstBeg.Serialize(SERIAL_FORMAT_SSV) = sa(POS_DstBeg)

    mvarStdBiasGMT = sa(POS_StdBiasGMT)
    mvarStdBeg.Serialize(SERIAL_FORMAT_SSV) = sa(POS_StdBeg)

    mvarSntpServer = sa(POS_SNTP_SUPPORTEDServer)

    mvarIndex = sa(POS_Index)
    mvarMapId = sa(POS_MapId)

    mvarReadOnly = sa(POS_ReadOnly)
    mvarOrigActive = sa(POS_OrigActive)
    mvarPrevName = sa(POS_PrevName)
End Property
Public Property Get Serialize(Optional SerialFormat As SERIAL_FORMAT = SERIAL_FORMAT_TSV) As String
    Dim sa(POS_UBOUND) As String

    sa(POS_SVERSION) = CStr(0)
    sa(POS_Name) = mvarName
    sa(POS_Deleted) = mvarDeleted
    sa(POS_Hidden) = mvarHidden
    sa(POS_Dirty) = mvarDirty
    sa(POS_Desc) = mvarDesc

    sa(POS_DstEnabled) = mvarDstEnabled
    sa(POS_DstBiasGMT) = mvarDstBiasGMT
    sa(POS_DstBeg) = mvarDstBeg.Serialize(SERIAL_FORMAT_SSV)

    sa(POS_StdBiasGMT) = mvarStdBiasGMT
    sa(POS_StdBeg) = mvarStdBeg.Serialize(SERIAL_FORMAT_SSV)

    sa(POS_SNTP_SUPPORTEDServer) = mvarSntpServer

    sa(POS_Index) = mvarIndex
    sa(POS_MapId) = mvarMapId

    sa(POS_ReadOnly) = mvarReadOnly
    sa(POS_OrigActive) = mvarOrigActive
    sa(POS_PrevName) = mvarPrevName

    Serialize = Join(sa, SerialDelim(SerialFormat))
End Property
Public Property Set StdBeg(ByVal NewRule As ZoneRule)
Attribute StdBeg.VB_Description = "The date Standard Time begins in this time zone."
    Set mvarStdBeg = NewRule

    mvarDirty = True
End Property
Public Property Get StdBeg() As ZoneRule
    Set StdBeg = mvarStdBeg
End Property
Public Property Set DstBeg(ByVal NewRule As ZoneRule)
    Set mvarDstBeg = NewRule

    mvarDirty = True
End Property
Public Property Get DstBeg() As ZoneRule
Attribute DstBeg.VB_Description = "Date and Time Daylight Saving Time begins in this time zone."
    Set DstBeg = mvarDstBeg
End Property
Public Property Let DstEnabled(ByVal vData As Boolean)
Attribute DstEnabled.VB_Description = "Is  Daylight Saving Time enabled (will go into effect during appropriate time period) i n this time zone?"
    If mvarDstEnabled <> vData Then
        mvarDstEnabled = vData
        mvarDirty = True
    End If
End Property
Public Property Get DstEnabled() As Boolean
    If (mvarDstBeg.Month = 0) Or (mvarStdBeg.Month = 0) Then
        DstEnabled = False
    Else
        DstEnabled = mvarDstEnabled
    End If
End Property
Public Property Get StdActive(Optional When As Date = 0) As Boolean
Attribute StdActive.VB_Description = "Is Standard Time currently in effect in this time zone?"
    StdActive = (Not DstActive(When))
End Property
Public Property Get DstActive(Optional When As Date = 0) As Boolean
Attribute DstActive.VB_Description = "Is Daylight Saving Time is currently in effect in this time zone?"
    Dim DstDate As Date
    Dim StdDate As Date
    Dim b As Boolean

    DstActive = False
    If mvarStdBeg.Month = 0 Then Exit Property
    If mvarDstBeg.Month = 0 Then Exit Property

    b = True
    DstDate = mvarDstBeg.EffectiveDate
    StdDate = mvarStdBeg.EffectiveDate

    If When = 0 Then When = Now()
    If StdDate > DstDate Then ' northern hemisphere
        If (DstDate < When) And (When < StdDate) Then
            b = True
        Else
            b = False
        End If
    Else ' southern hemisphere
        If (StdDate < When) And (When < DstDate) Then
            b = False
        Else
            b = True
        End If
    End If

    DstActive = b
End Property
Public Property Let StdBiasGMT(ByVal vData As Long)
Attribute StdBiasGMT.VB_Description = "The bias/offset from GMT in effect in this time zone during Standard Time."
    If mvarStdBiasGMT <> vData Then
        mvarStdBiasGMT = vData
        mvarDirty = True
    End If
End Property
Public Property Get StdBiasGMT() As Long
    StdBiasGMT = mvarStdBiasGMT
End Property
Public Property Let DstBiasGMT(ByVal vData As Long)
Attribute DstBiasGMT.VB_Description = "The bias/offset from GMT that applies when Daylight Saving Time is in effect in this time zone."
    If mvarDstBiasGMT <> vData Then
        mvarDstBiasGMT = vData
        mvarDirty = True
    End If
End Property
Public Property Get DstBiasGMT() As Long
    DstBiasGMT = mvarDstBiasGMT
End Property
Public Property Let StdBiasLocal(ByVal vData As Long)
    Dim tz As TimeZone

    Set tz = New TimeZone
    StdBiasGMT = vData - tz.BiasGMT
End Property
Public Property Get StdBiasLocal() As Long
    Dim tz As TimeZone

    Set tz = New TimeZone
    StdBiasLocal = mvarStdBiasGMT - tz.BiasGMT
End Property
Public Property Let DstBiasLocal(ByVal vData As Long)
    Dim tz As TimeZone

    Set tz = New TimeZone
    DstBiasGMT = vData - tz.BiasGMT
End Property
Public Property Get DstBiasLocal() As Long
    Dim tz As TimeZone

    Set tz = New TimeZone
    DstBiasLocal = mvarDstBiasGMT - tz.BiasGMT
End Property
Public Property Get BiasGMT(Optional When As Date = 0) As Long
Attribute BiasGMT.VB_Description = "This time zone's current bias/offset from GMT"
    Dim d As Date

    If When = 0 Then
        d = Now()
    Else
        d = When
    End If

    If DstActive(d) Then
        BiasGMT = mvarDstBiasGMT
    Else
        BiasGMT = mvarStdBiasGMT
    End If
End Property
Public Property Get BiasLocal() As Long
Attribute BiasLocal.VB_Description = "This time zone's current bias/offset from local time (this computer's current time zone)"
    Dim ltz As TimeZone

    Set ltz = New TimeZone
    BiasLocal = ltz.BiasGMT - BiasGMT
End Property
Public Property Get DateTimeGMT() As Date
    Dim st As SYSTEMTIME
    Dim result As Long
    Dim d As Date

    GetSystemTime st
    d = DateSerial(st.wYear, st.wMonth, st.wDay) + TimeSerial(st.wHour, st.wMinute, st.wSecond)

    DateTimeGMT = d
End Property
Public Property Get DateGMT() As Date
    Dim st As SYSTEMTIME
    Dim result As Long
    Dim d As Date

    GetSystemTime st
    d = DateSerial(st.wYear, st.wMonth, st.wDay)

    DateGMT = d
End Property
Public Property Get TimeGMT() As Date
    Dim st As SYSTEMTIME
    Dim result As Long
    Dim d As Date

    GetSystemTime st
    d = TimeSerial(st.wHour, st.wMinute, st.wSecond)

    TimeGMT = d
End Property
Public Property Get DateTimeLocal(Optional DateTimeGMT As Date = 0) As Date
Attribute DateTimeLocal.VB_Description = "Current date and time in this time zone."
    Dim st As SYSTEMTIME
    Dim result As Long
    Dim d As Date

    If DateTimeGMT = 0 Then
        GetSystemTime st
        d = DateSerial(st.wYear, st.wMonth, st.wDay) + TimeSerial(st.wHour, st.wMinute, st.wSecond)
    Else
        d = DateTimeGMT
    End If

    If DstActive Then
        DateTimeLocal = DateAdd("n", mvarDstBiasGMT, d)
    Else
        DateTimeLocal = DateAdd("n", mvarStdBiasGMT, d)
    End If
End Property
Public Property Get TimeLocal(Optional TimeGMT As Date = 0) As Date
Attribute TimeLocal.VB_Description = "The current time in this time zone."
    Dim st As SYSTEMTIME
    Dim result As Long
    Dim d As Date

    If TimeGMT = 0 Then
        GetSystemTime st
        d = TimeSerial(st.wHour, st.wMinute, st.wSecond)
    Else
        d = TimeGMT
    End If

    If DstActive Then
        TimeLocal = DateAdd("n", mvarDstBiasGMT, d)
    Else
        TimeLocal = DateAdd("n", mvarStdBiasGMT, d)
    End If
End Property
Public Property Get DateLocal(Optional DateGMT As Date = 0) As Date
Attribute DateLocal.VB_Description = "Current Date in this time zone."
    Dim st As SYSTEMTIME
    Dim result As Long
    Dim d As Date

    If DateGMT = 0 Then
        d = DateTimeLocal
    Else
        d = DateGMT
    End If

    If DstActive Then
        d = DateAdd("n", mvarDstBiasGMT, d)
    Else
        d = DateAdd("n", mvarStdBiasGMT, d)
    End If

    DateLocal = DateSerial(Year(d), Month(d), Day(d))
End Property
Public Property Let Desc(ByVal vData As String)
Attribute Desc.VB_Description = "Description of this time zone."
    If mvarDesc <> vData Then
        mvarDesc = vData
        mvarDirty = True
    End If
End Property
Public Property Get Desc() As String
    Desc = mvarDesc
End Property
Private Sub Class_Initialize()
    Dim st As SYSTEMTIME
    Dim result As Long
    Dim i As Long
    Dim s As String
    Dim tzi As TIME_ZONE_INFORMATION
    Dim Reg As Registry

    If ApiWinVersion.PlatformNT Then
        mvarTimeZoneRegKey = REG_NT_TIMEZONES
        StandardSuffix = STANDARD_NT_SUFFIX
        DaylightSuffix = DST_NT_SUFFIX
    Else
        mvarTimeZoneRegKey = REG_9x_TIMEZONES
        StandardSuffix = STANDARD_9x_SUFFIX
        DaylightSuffix = DST_9x_SUFFIX
    End If

    Set Reg = New Registry
    i = Reg.ReadValue(REG_DST_DATANAME, 1, REG_DST_SUBKEY, HKEY_LOCAL_MACHINE)
    If i = 0 Then
        mvarDstEnabled = False
    Else
        mvarDstEnabled = True
    End If

    result = GetTimeZoneInformation(tzi)

    Select Case result
        Case TIME_ZONE_ID_DAYLIGHT:
'            mvarDstActive = True

        Case TIME_ZONE_ID_INVALID:
'            mvarDstActive = False
            mvarDeleted = True ' ?

        Case TIME_ZONE_ID_STANDARD:
'            mvarDstActive = False

        Case TIME_ZONE_ID_UNKNOWN: ' no DST in zone
'            mvarDstActive = False

    End Select
    s = ApiTextStrip(tzi.StandardName) ' Trim(ApiTextStrip(StrConv(tzi.StandardName, vbFromUnicode)))
    mvarDesc = s
    Name = s
'    mvarDesc = mvarName
    mvarMapId = ""
    mvarIndex = -1

    mvarStdBiasGMT = (tzi.Bias + tzi.StandardBias) * (-1)
    mvarDstBiasGMT = (tzi.Bias + tzi.DaylightBias) * (-1)

    Set mvarDstBeg = New ZoneRule
    mvarDstBeg.Begin tzi.DaylightDate.wYear, tzi.DaylightDate.wMonth, tzi.DaylightDate.wDay, tzi.DaylightDate.wDayOfWeek, tzi.DaylightDate.wHour, tzi.DaylightDate.wMinute, tzi.DaylightDate.wSecond

    Set mvarStdBeg = New ZoneRule
    mvarStdBeg.Begin tzi.StandardDate.wYear, tzi.StandardDate.wMonth, tzi.StandardDate.wDay, tzi.StandardDate.wDayOfWeek, tzi.StandardDate.wHour, tzi.StandardDate.wMinute, tzi.StandardDate.wSecond

    mvarSntpServer = "time.nist.gov"

    mvarDeleted = False
    mvarReadOnly = False
    mvarDirty = False
End Sub
Private Sub Class_Terminate()
    'the class is being destroyed
End Sub
Public Property Get DstAvailable() As Variant
Attribute DstAvailable.VB_Description = "Is Daylight Saving Time available (are the necessary values set) in this time zone?"
    If (mvarStdBiasGMT = mvarDstBiasGMT) Or (mvarStdBeg.Month = 0) Or (mvarDstBeg.Month = 0) Then
        DstAvailable = False
    Else
        DstAvailable = True
    End If
End Property
Public Property Get OrigActive() As Boolean
Attribute OrigActive.VB_Description = "Was this time zone the active time zone when the object was created?"
    OrigActive = mvarOrigActive
End Property
Public Property Let OrigActive(ByVal vNewValue As Boolean)
    mvarOrigActive = vNewValue
End Property
Public Property Get ReadOnly() As Boolean
Attribute ReadOnly.VB_Description = "Can this time zone's information be changed?"
    ReadOnly = mvarReadOnly
End Property
Public Property Let ReadOnly(ByVal vNewValue As Boolean)
    mvarReadOnly = vNewValue
    If mvarDeleted And mvarReadOnly Then
        mvarDeleted = False
        mvarHidden = True
    End If
End Property
