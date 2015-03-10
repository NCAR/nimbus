VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "mswinsck.ocx"
Begin VB.Form UTsync 
   Caption         =   "Synchronize UTC"
   ClientHeight    =   2490
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5490
   LinkTopic       =   "Form1"
   ScaleHeight     =   2490
   ScaleWidth      =   5490
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame2 
      Caption         =   "Protocol"
      Height          =   1185
      Left            =   4320
      TabIndex        =   14
      Top             =   45
      Width           =   1065
      Begin VB.OptionButton optProtocol 
         Caption         =   "SNTP"
         Height          =   165
         Index           =   3
         Left            =   120
         TabIndex        =   18
         Top             =   915
         Width           =   825
      End
      Begin VB.OptionButton optProtocol 
         Caption         =   "NTP"
         Height          =   165
         Index           =   2
         Left            =   120
         TabIndex        =   17
         Top             =   690
         Width           =   675
      End
      Begin VB.OptionButton optProtocol 
         Caption         =   "Daytime"
         Height          =   225
         Index           =   1
         Left            =   120
         TabIndex        =   16
         Top             =   450
         Width           =   885
      End
      Begin VB.OptionButton optProtocol 
         Caption         =   "Time"
         Height          =   225
         Index           =   0
         Left            =   120
         TabIndex        =   15
         Top             =   210
         Value           =   -1  'True
         Width           =   675
      End
   End
   Begin VB.CheckBox chkServerSearch 
      Caption         =   "Search for Server"
      Height          =   315
      Left            =   1530
      TabIndex        =   13
      Top             =   2130
      Width           =   1635
   End
   Begin VB.Timer tmrTimeOut 
      Left            =   3825
      Top             =   2085
   End
   Begin VB.CommandButton cmdUTsync 
      Caption         =   "E&xit"
      Height          =   465
      Index           =   1
      Left            =   4470
      TabIndex        =   12
      Top             =   1980
      Width           =   975
   End
   Begin VB.Frame Frame1 
      Caption         =   "NTP Server"
      Height          =   1185
      Left            =   60
      TabIndex        =   7
      Top             =   45
      Width           =   4185
      Begin VB.ComboBox cboLocation 
         Height          =   315
         Left            =   990
         TabIndex        =   8
         Top             =   210
         Width           =   3075
      End
      Begin VB.Label lblServerName 
         Caption         =   "Hostname: "
         Height          =   255
         Left            =   120
         TabIndex        =   11
         Top             =   870
         Width           =   3945
      End
      Begin VB.Label lblIPaddress 
         Caption         =   "IP Address: "
         Height          =   255
         Left            =   120
         TabIndex        =   10
         Top             =   615
         Width           =   3975
      End
      Begin VB.Label Label1 
         Caption         =   "Location:"
         Height          =   195
         Left            =   120
         TabIndex        =   9
         Top             =   240
         Width           =   705
      End
   End
   Begin VB.TextBox txtDateTime 
      Height          =   315
      Left            =   990
      MultiLine       =   -1  'True
      TabIndex        =   6
      Top             =   1710
      Width           =   3150
   End
   Begin VB.TextBox txtActivity 
      Height          =   315
      Left            =   990
      MultiLine       =   -1  'True
      TabIndex        =   5
      Top             =   1350
      Width           =   3150
   End
   Begin VB.TextBox txtDelay 
      Alignment       =   1  'Right Justify
      Height          =   315
      Left            =   990
      MultiLine       =   -1  'True
      TabIndex        =   4
      Top             =   2100
      Width           =   495
   End
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   3270
      Top             =   2070
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
      RemotePort      =   123
      LocalPort       =   123
   End
   Begin VB.CommandButton cmdUTsync 
      Caption         =   "&Sync UT"
      Height          =   495
      Index           =   0
      Left            =   4485
      TabIndex        =   0
      Top             =   1440
      Width           =   975
   End
   Begin VB.Label Label6 
      Caption         =   "Delay (ms)"
      Height          =   285
      Left            =   90
      TabIndex        =   3
      Top             =   2100
      Width           =   825
   End
   Begin VB.Label Label5 
      Caption         =   "Date/Time"
      Height          =   285
      Left            =   90
      TabIndex        =   2
      Top             =   1740
      Width           =   825
   End
   Begin VB.Label Label4 
      Caption         =   "Activity"
      Height          =   255
      Left            =   90
      TabIndex        =   1
      Top             =   1380
      Width           =   705
   End
End
Attribute VB_Name = "UTsync"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Domain$(), IPaddress$(), Location$(), nDefault%, nServers%, nServer%, nProtocol%

'Author: MJ Mahoney
'Email:  Michael.J.Mahoney@jpl.nasa.gov
'WWW:    http://mtp.jpl.nasa.gov

Option Explicit
   

Dim TimeDelay As Single, Data As String, NTPtime As Double, TimeStart As Single
Dim LngTimeFrom1990 As Long, st As SYSTEMTIME, UTCdate As Date, ServerFound As Boolean
Dim sNTP As SNTP_MESSAGE
Private Sub cboLocation_Click()
  
  nDefault = cboLocation.ListIndex
  cboLocation.Text = cboLocation.List(nDefault)
  lblIPaddress.Caption = "IP Address: " + IPaddress(nDefault)
  lblServerName.Caption = "Hostname:   " + Domain(nDefault)

End Sub


Private Sub cmdUTsync_Click(Index As Integer)
Dim Port%, i%
Dim sNTPout(0 To 47) As Byte

For i = 0 To 3
  If optProtocol(i).Value = True Then
    nProtocol = i
    If i = 0 Then Port = 37    'Time Protocol RFC-868
    If i = 1 Then Port = 13    'Daytime Protocol RFC-867
    If i > 1 Then Port = 123   'NTP RFC-1305 and SNTP RFC-1769
    Exit For
  End If
Next i

Select Case Index
Case 0                      'Sync UT
  txtActivity.Text = "Connecting ..."
  txtDateTime.Text = "Getting time ..."
  cmdUTsync(0).Enabled = False    'Sync UT
  cmdUTsync(1).Enabled = True     'Exit
  cboLocation.Enabled = False
  Winsock1.Close
  sNTPout(0) = &H1B
  TimeStart = Timer
  
  Select Case Port
  Case 13, 37
    With Winsock1
      .Protocol = sckTCPProtocol
      .RemoteHost = Trim$(IPaddress(nDefault))   ' Server IP Address
      .RemotePort = Port                         ' Port
      .LocalPort = Port
      .Connect
    End With
    Me.Refresh
    tmrTimeOut.Enabled = True
    tmrTimeOut.Interval = 2000   'Time Out after 2 seconds
    ServerFound = False
    nServer = -1
  
  Case 123
    With Winsock1
      .Protocol = sckUDPProtocol
      .RemoteHost = Trim$(IPaddress(nDefault))   ' Server IP Address
      .RemotePort = Port                         ' Port
      .LocalPort = 124
      .Connect
      .SendData sNTPout
    End With
    Me.Refresh
    tmrTimeOut.Enabled = True
    tmrTimeOut.Interval = 2000   'Time Out after 2 seconds
    ServerFound = False
    nServer = -1
  End Select
Case 1      'Exit
  Unload Me
  
Case 2      'Search for server code to avoid resetting nServer count
  txtActivity.Text = "Connecting ..."
  txtDateTime.Text = "Getting time ..."
  cmdUTsync(0).Enabled = False    'Sync UT
  cmdUTsync(1).Enabled = True     'Exit
  cboLocation.Enabled = False
  Winsock1.Close
  'sNTP = Empty
  With Winsock1
    .RemoteHost = Trim$(IPaddress(nDefault))   ' Server IP Address
    .RemotePort = Port                         ' Port
    .Connect
  End With
  Me.Refresh
  
End Select

End Sub

Private Sub Form_Load()
Dim i%

  nServers = 24
  ReDim Domain$(0 To nServers), IPaddress$(0 To nServers), Location$(0 To nServers)
  Domain(0) = "time-a.nist.gov": IPaddress(0) = "129.6.15.28": Location(0) = "NIST, Gaithersburg, Maryland"
  Domain(1) = "time-a.nist.gov": IPaddress(1) = "129.6.15.28": Location(1) = "NIST, Gaithersburg, Maryland"
  Domain(2) = "time-b.nist.gov": IPaddress(2) = "129.6.15.29": Location(2) = "NIST, Gaithersburg, Maryland"
  Domain(3) = "time-a.timefreq.bldrdoc.gov": IPaddress(3) = "132.163.4.101": Location(3) = "NIST, Boulder, Colorado"
  Domain(4) = "time-b.timefreq.bldrdoc.gov": IPaddress(4) = "132.163.4.102": Location(4) = "NIST, Boulder, Colorado"
  Domain(5) = "time-c.timefreq.bldrdoc.gov": IPaddress(5) = "132.163.4.103": Location(5) = "NIST, Boulder, Colorado"
  Domain(6) = "utcnist.colorado.edu": IPaddress(6) = "128.138.140.44": Location(6) = "University of Colorado, Boulder"
  Domain(7) = "time.nist.gov": IPaddress(7) = "192.43.244.18": Location(7) = "NCAR, Boulder, Colorado"
  Domain(8) = "time-nw.nist.gov": IPaddress(8) = "131.107.1.10": Location(8) = "Microsoft, Redmond, Washington"
  Domain(9) = "nist1.datum.com": IPaddress(9) = "66.243.43.21": Location(9) = "Datum, San Jose, California"
  Domain(10) = "nist1-dc.glassey.com": IPaddress(10) = "216.200.93.8": Location(10) = "Abovenet, Virginia"
  Domain(11) = "nist1-ny.glassey.com": IPaddress(11) = "208.184.49.9": Location(11) = "Abovenet, New York City"
  Domain(12) = "nist1-sj.glassey.com": IPaddress(12) = "207.126.98.204": Location(12) = "Abovenet, San Jose, California"
  Domain(13) = "nist1.aol-ca.truetime.com": IPaddress(13) = "207.200.81.113": Location(13) = "TrueTime, AOL facility, Sunnyvale, California"
  Domain(14) = "nist1.aol-va.truetime.com": IPaddress(14) = "205.188.185.33": Location(14) = "TrueTime, AOL facility, Virginia"
  Domain(15) = "tock.usno.navy.mil": IPaddress(15) = "192.5.41.41": Location(15) = "US Naval Observatory, Anapolis, MD"
  Domain(16) = "tick.usno.navy.mil": IPaddress(16) = "192.5.41.41": Location(16) = "US Naval Observatory, Anapolis, MD"
  Domain(17) = "ntp1.fau.de": IPaddress(17) = "131.188.3.221": Location(17) = "Univerisity of Erlangen, Germany"
  Domain(18) = "ntps1-0.cs.tu-berlin.de": IPaddress(18) = "130.149.17.21": Location(18) = "Berlin, Germany"
  Domain(19) = "time.ien.it": IPaddress(19) = "193.204.114.233": Location(19) = "IEN, Italy"
  Domain(20) = "swisstime.ethz.ch": IPaddress(20) = "129.132.2.21": Location(20) = "ETH Zurich, Switzerland"
  Domain(21) = "ntp.cs.mu.oz.au": IPaddress(21) = "128.250.37.2": Location(21) = "Austria"
  Domain(22) = "ntp-cup.external.hp.com": IPaddress(22) = "192.6.38.127": Location(22) = "Hewlett-Packard, USA"
  Domain(23) = "montpelier.caltech.edu": IPaddress(23) = "192.12.19.20": Location(23) = "Caltech, Pasadena, CA, USA"
  nDefault = 0
  For i = 0 To nServers - 1
    cboLocation.AddItem Trim$(Location(i))
  Next i
  cboLocation.ListIndex = nDefault
  lblIPaddress.Caption = "IP Address: " + IPaddress(nDefault)
  lblServerName.Caption = "Hostname:   " + Domain(nDefault)
  
  
End Sub



Private Sub tmrTimeOut_Timer()
  
  If ServerFound Then
    tmrTimeOut.Enabled = False
    nServer = -1
    
  Else
    Winsock1.Close
    cmdUTsync(0).Enabled = True
    cmdUTsync(1).Enabled = True
    cboLocation.Enabled = True
    txtActivity.Text = "Ready ..."
    txtDateTime.Text = "Unable to reach " + Location(nDefault)
    If chkServerSearch.Value = 1 Then  'Search for a server
      If nServer = -1 Then nServer = 1 Else nServer = nServer + 1
      If nServer > nServers Then
        tmrTimeOut.Enabled = False
        txtDateTime.Text = "Could not find any server in program list!"
      Else
        nDefault = nServer
        cboLocation.ListIndex = nDefault - 1
        lblIPaddress.Caption = "IP Address: " + IPaddress(nDefault)
        lblServerName.Caption = "Hostname:   " + Domain(nDefault)
        cmdUTsync_Click (2)
      End If
    Else
      tmrTimeOut.Enabled = False
      nServer = -1
    End If
  End If
End Sub

Private Sub WinSock1_Connect()
    txtActivity.Text = "Connected ..."
    TimeStart = Timer
End Sub

Private Sub WinSock1_DataArrival(ByVal bytesTotal As Long)
    
    ServerFound = True
    txtActivity.Text = "Getting time ..."
    Winsock1.GetData Data, vbString
    Debug.Print Data
    'sNTP = sNTP & Data
    WinSock1_Close
    
End Sub

Private Sub WinSock1_Close()
    On Error Resume Next
    
    Do Until Winsock1.State = sckClosed
      Winsock1.Close
      DoEvents
    Loop
    
    TimeDelay = ((Timer - TimeStart) / 2)  'One way delay
    txtDelay.Text = Format(TimeDelay * 1000#, "###0.0")
    Call SyncClock(Data)
End Sub

Private Sub SyncClock(tStr As String)
Dim x!, MM$, DS As Date, HH%, Min%, SS%, YY%, Mon%, DD%, DandT As Date
Dim B1 As SNTP_TIMESTAMP, B2 As SNTP_TIMESTAMP, B3 As SNTP_TIMESTAMP
Dim ts1 As SYSTEMTIME, DisplayTimeMs$
Dim ts2 As SYSTEMTIME, ts3 As SYSTEMTIME
Dim Totalsecs1 As Variant, Totalsecs2 As Variant, Totalsecs3 As Variant

    txtActivity.Text = "Converting time ..."
    tStr = Trim(tStr)
    Select Case nProtocol
    Case 0   'Time
      If Len(tStr) <> 4 Then
        MsgBox "Unable to connect to server!", vbCritical, "GetTime!"
        GoTo CarryOn
      End If
      NTPtime = Asc(Left$(tStr, 1)) * 256 ^ 3 + Asc(Mid$(tStr, 2, 1)) * 256 ^ 2 + _
                Asc(Mid$(tStr, 3, 1)) * 256 ^ 1 + Asc(Right$(tStr, 1))
      LngTimeFrom1990 = NTPtime - 2840140800#
      UTCdate = DateAdd("s", CDbl(LngTimeFrom1990 + CLng(TimeDelay)), #1/1/1990#)
    Case 1   'Daytime
' JJJJJ YY-MM-DD HH:MM:SS TT L UT1 msADV UTC(NIST) <OTM>
' 53103 04-04-08 00:42:41 50 0 0 521.4 UTC(NIST) *
'1234567890123456789012345678901234567890123456789
      If nDefault = 23 Then
'1234567890123456789012345678901234567890123456789
'Fri Apr  9 20:53:51 2004
        YY = Mid$(tStr, 21, 4): Mon = fMonthNumber(Mid$(tStr, 5, 3)): DD = Mid$(tStr, 9, 2)
        HH = Mid$(tStr, 12, 2): Min = Mid$(tStr, 15, 2): SS = Mid$(tStr, 18, 2)
      Else
        YY = 2000 + Mid$(tStr, 8, 2): Mon = Mid$(tStr, 11, 2): DD = Mid$(tStr, 14, 2)
        HH = Mid$(tStr, 17, 2): Min = Mid$(tStr, 20, 2): SS = Mid$(tStr, 23, 2)
      'UTCdate = DateDiff("s", #1/1/1990#, DandT)
      End If
      UTCdate = DateSerial(YY, Mon, DD) + TimeSerial(HH, Min, SS)
    Case 2, 3
      B1 = sNTP.OriginateTimeStamp
      B2 = sNTP.ReceiveTimeStamp
      B3 = sNTP.TransmitTimeStamp
      
      Totalsecs1 = Round(TimeStampToSecs(B1), 3)
      Totalsecs2 = Round(TimeStampToSecs(B2), 3)
      Totalsecs3 = Round(TimeStampToSecs(B3), 3)
      GetSystemTime ts1
      DisplayTimeMs = DispSystemTime(ts1)
    End Select
                      
    Select Case Month(UTCdate)
        Case 1: MM = "January"
        Case 2: MM = "February"
        Case 3: MM = "March"
        Case 4: MM = "April"
        Case 5: MM = "May"
        Case 6: MM = "June"
        Case 7: MM = "July"
        Case 8: MM = "August"
        Case 9: MM = "September"
        Case 10: MM = "October"
        Case 11: MM = "November"
        Case 12: MM = "December"
    End Select
    
    txtDateTime.Text = Year(UTCdate) & " " & MM & " " & Day(UTCdate) & " " & _
                     Format(Hour(UTCdate), "00") & ":" & _
                     Format(Minute(UTCdate), "00") & ":" & _
                     Format(Second(UTCdate), "00")
CarryOn:
    ServerFound = True
    cmdUTsync(0).Enabled = True
    cmdUTsync(1).Enabled = True
    cboLocation.Enabled = True
    txtActivity.Text = "Ready ..."
End Sub
'---------- Ende Formular "Form1" alias Form1.frm  ----------
'-------------- Ende Projektdatei Project1.vbp --------------

Function fMonthNumber(Month$)
  Select Case Month$
  Case "Jan": fMonthNumber = 1
  Case "Feb": fMonthNumber = 2
  Case "Mar": fMonthNumber = 3
  Case "Apr": fMonthNumber = 4
  Case "May": fMonthNumber = 5
  Case "Jun": fMonthNumber = 6
  Case "Jul": fMonthNumber = 7
  Case "Aug": fMonthNumber = 8
  Case "Sep": fMonthNumber = 9
  Case "Oct": fMonthNumber = 10
  Case "Nov": fMonthNumber = 11
  Case "Dec": fMonthNumber = 12
  End Select
End Function

