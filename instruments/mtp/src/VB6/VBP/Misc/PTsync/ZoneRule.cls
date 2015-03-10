VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ZoneRule"
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
' Version 1.1 6/2/2004

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

Private Enum SER_POS
    POS_SVERSION = 0
    POS_EffectiveDate
    POS_Year
    POS_Month
    POS_Day
    POS_Week
    POS_DayOfWeek
    POS_Hour
    POS_Minute
    POS_Second

    POS_Dummy1
    POS_Dummy2
    POS_Dummy3
    POS_Dummy4
    POS_Dummy5
    POS_UBOUND = POS_Dummy5
End Enum

'local variable(s) to hold property value(s)
'Private mvarUseRule As Boolean 'local copy
Private mvarEffectiveDate As Date 'local copy
Private mvarYear As Long
Private mvarMonth As Long
Private mvarDay As Long
Private mvarWeek As Long 'local copy
Private mvarDayOfWeek As Long 'local copy
Private mvarHour As Long 'local copy
Private mvarMinute As Long 'local copy
Private mvarSecond As Long 'local copy
Public Property Let Month(ByVal vData As Long)
    mvarMonth = vData
End Property
Public Property Get Month() As Long
    Month = mvarMonth
End Property
Public Property Let Day(ByVal vData As Long)
    mvarDay = vData
End Property
Public Property Get Day() As Long
    Day = mvarDay
End Property
Public Property Let Second(ByVal vData As Long)
    mvarSecond = vData
End Property
Public Property Get Second() As Long
    Second = mvarSecond
End Property
Public Property Let Minute(ByVal vData As Long)
    mvarMinute = vData
End Property
Public Property Get Minute() As Long
    Minute = mvarMinute
End Property
Public Property Let Hour(ByVal vData As Long)
    mvarHour = vData
End Property
Public Property Get Hour() As Long
    Hour = mvarHour
End Property
Public Property Let DayOfWeek(ByVal vData As Long)
    mvarDayOfWeek = vData
End Property
Public Property Get DayOfWeek() As Long
    DayOfWeek = mvarDayOfWeek
End Property
Public Property Let Week(ByVal vData As Long)
    mvarWeek = vData
End Property
Public Property Get Week() As Long
    Week = mvarWeek
End Property
Public Function Begin(Year As Integer, Month As Integer, Day As Integer, DayOfWeek As Integer, Hour As Integer, Minute As Integer, Second As Integer) As Date
    mvarYear = Year
    mvarMonth = Month
    mvarDay = Day
    mvarWeek = Day
    mvarDayOfWeek = DayOfWeek
    mvarHour = Hour
    mvarMinute = Minute
    mvarSecond = Second

    Begin = EffectiveDate
End Function
Public Function Clone() As ZoneRule
    Dim Rule As ZoneRule

    Set Rule = New ZoneRule
    Rule.Serialize(SERIAL_FORMAT_TSV) = Serialize(SERIAL_FORMAT_TSV)
    Set Clone = Rule
End Function
Public Property Let EffectiveDate(Optional EffectiveYear As Long, ByVal vData As Date)
    If VBA.Year(vData) <> 0 Then  ' absolute date
'        mvarUseRule = False
        mvarEffectiveDate = vData
        mvarYear = VBA.Year(mvarEffectiveDate)
        mvarMonth = VBA.Month(mvarEffectiveDate)
        mvarDay = VBA.Day(mvarEffectiveDate)
        mvarHour = VBA.Hour(mvarEffectiveDate)
        mvarMinute = VBA.Minute(mvarEffectiveDate)
        mvarSecond = VBA.Second(mvarEffectiveDate)
    Else ' use rule
        mvarYear = 0
'        mvarUseRule = True
        mvarMonth = VBA.Month(mvarEffectiveDate)
        mvarWeek = VBA.Day(mvarEffectiveDate)
        mvarDayOfWeek = VBA.Weekday(mvarEffectiveDate, vbSunday) - 1
        mvarHour = VBA.Hour(mvarEffectiveDate)
        mvarMinute = VBA.Minute(mvarEffectiveDate)
        mvarSecond = VBA.Second(mvarEffectiveDate)
    End If
End Property
Public Property Get EffectiveDate(Optional EffectiveYear As Long) As Date
    Dim d As Date
    Dim Cnt As Long

    If mvarYear <> 0 Then
        If EffectiveYear = 0 Then ' use current year
            mvarEffectiveDate = DateSerial(VBA.Year(Now()), mvarMonth, mvarDay) + TimeSerial(mvarHour, mvarMinute, mvarSecond)
        Else
            mvarEffectiveDate = DateSerial(mvarYear, mvarMonth, mvarDay) + TimeSerial(mvarHour, mvarMinute, mvarSecond)
        End If
        EffectiveDate = mvarEffectiveDate
        Exit Property
    End If

    If EffectiveYear = 0 Then
        d = DateSerial(VBA.Year(Now), mvarMonth, 1)
    Else
        d = DateSerial(EffectiveYear, mvarMonth, 1)
    End If
    Cnt = mvarWeek

    While VBA.Weekday(d) <> (mvarDayOfWeek + 1)
        d = DateAdd("d", 1, d)
    Wend

    If Cnt < 5 Then
        While Cnt > 1
            d = DateAdd("ww", 1, d)
            Cnt = Cnt - 1
        Wend
    Else    ' last dow of month
        While VBA.Month(d) = mvarMonth
            d = DateAdd("ww", 1, d)
        Wend
        d = DateAdd("ww", -1, d)
    End If

    EffectiveDate = d + TimeSerial(mvarHour, mvarMinute, mvarSecond)
End Property
'Public Property Let UseRule(ByVal vData As Boolean)
'    mvarUseRule = vData
'End Property
'Public Property Get UseRule() As Boolean
'    UseRule = mvarUseRule
'End Property
Public Function Display() As String
    Dim s As String

    s = ""
    If mvarYear <> 0 Then
        Display = CStr(EffectiveDate)
        Exit Function
    End If

    If mvarMonth = 0 Then
        Display = ""
        Exit Function
    End If

    Select Case mvarWeek
        Case 1: s = s & "First"
        Case 2: s = s & "Second"
        Case 3: s = s & "Third"
        Case 4: s = s & "Fourth"
        Case 5: s = s & "Last"
        Case Else:
            Display = ""
            Exit Function
    End Select

    On Error Resume Next
    s = s & " " & WeekdayName(mvarDayOfWeek + 1, False, vbSunday)
    s = s & " of " & MonthName(mvarMonth, False)
    s = s & " at " & CStr(TimeSerial(mvarHour, mvarMinute, mvarSecond))

    If Err.Number <> 0 Then
        s = ""
        Err.Clear
    End If

    Display = s
End Function
Private Sub Class_Initialize()
'    mvarUseRule = False
End Sub
Private Function SerialDelim(SerialFormat As SERIAL_FORMAT) As String
    Select Case SerialFormat
        Case SERIAL_FORMAT_TSV:
            SerialDelim = vbTab
        Case SERIAL_FORMAT_CSV:
            SerialDelim = ","
        Case SERIAL_FORMAT_SSV:
            SerialDelim = ";"
        Case SERIAL_FORMAT_XML:
            ' not supported yet
        Case Else:
            SerialDelim = vbTab
    End Select
End Function
Public Property Get Serialize(Optional SerialFormat As SERIAL_FORMAT = SERIAL_FORMAT_TSV) As String
    Dim sa(POS_UBOUND) As String

    sa(POS_SVERSION) = CStr(0)
    sa(POS_EffectiveDate) = CStr(CDbl(mvarEffectiveDate))
    sa(POS_Year) = CStr(mvarYear)
    sa(POS_Month) = CStr(mvarMonth)
    sa(POS_Day) = CStr(mvarDay)
    sa(POS_Week) = CStr(mvarWeek)
    sa(POS_DayOfWeek) = CStr(mvarDayOfWeek)
    sa(POS_Hour) = CStr(mvarHour)
    sa(POS_Minute) = CStr(mvarMinute)
    sa(POS_Second) = CStr(mvarSecond)

    Serialize = Join(sa, SerialDelim(SerialFormat))
End Property
Public Property Let Serialize(Optional SerialFormat As SERIAL_FORMAT = SERIAL_FORMAT_TSV, SerialJob As String)
    Dim sa() As String

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

    mvarEffectiveDate = CDbl(sa(POS_EffectiveDate))
    mvarYear = sa(POS_Year)
    mvarMonth = sa(POS_Month)
    mvarDay = sa(POS_Day)
    mvarWeek = sa(POS_Week)
    mvarDayOfWeek = sa(POS_DayOfWeek)
    mvarHour = sa(POS_Hour)
    mvarMinute = sa(POS_Minute)
    mvarSecond = sa(POS_Second)
End Property
Public Property Get Year() As Long
    Year = mvarYear
End Property
Public Property Let Year(ByVal vNewValue As Long)
    mvarYear = vNewValue
End Property
