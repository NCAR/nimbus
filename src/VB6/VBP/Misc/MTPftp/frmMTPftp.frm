VERSION 5.00
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "Mscomct2.ocx"
Begin VB.Form frmMTPftp 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "MTP Realtime FTP"
   ClientHeight    =   4995
   ClientLeft      =   5670
   ClientTop       =   3795
   ClientWidth     =   4980
   Icon            =   "frmMTPftp.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4995
   ScaleWidth      =   4980
   StartUpPosition =   2  'CenterScreen
   Begin InetCtlsObjects.Inet ctlInternet 
      Left            =   2985
      Top             =   4410
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
   End
   Begin VB.TextBox txtCurrent 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2265
      MultiLine       =   -1  'True
      TabIndex        =   27
      Top             =   2280
      Width           =   480
   End
   Begin VB.TextBox txtFilesRead 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1185
      MultiLine       =   -1  'True
      TabIndex        =   25
      Top             =   2280
      Width           =   480
   End
   Begin VB.CheckBox chkUseLatest 
      Caption         =   "Use Latest File"
      Height          =   225
      Left            =   2850
      TabIndex        =   22
      Top             =   4140
      Value           =   1  'Checked
      Width           =   1350
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "&Start"
      Height          =   375
      Left            =   4245
      TabIndex        =   21
      Top             =   4185
      Width           =   690
   End
   Begin VB.Timer Timer1 
      Left            =   3765
      Top             =   4575
   End
   Begin VB.TextBox txtFilenameDate 
      Height          =   285
      Left            =   1185
      TabIndex        =   18
      Text            =   "MTPcurrent_20080512"
      Top             =   1680
      Width           =   3795
   End
   Begin VB.TextBox txtLocalFolder 
      Height          =   285
      Left            =   1185
      TabIndex        =   16
      Text            =   "C:/MTP/RTA/RAW/"
      Top             =   1365
      Width           =   3795
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   345
      Left            =   4245
      TabIndex        =   14
      Top             =   4590
      Width           =   690
   End
   Begin VB.TextBox txtPassword 
      Height          =   285
      Left            =   1185
      TabIndex        =   13
      Text            =   "Michael.J.Mahoney@jpl.nasa.gov"
      Top             =   1050
      Width           =   3780
   End
   Begin VB.TextBox txtUsername 
      Height          =   300
      Left            =   1185
      TabIndex        =   11
      Text            =   "anonymous"
      Top             =   720
      Width           =   3780
   End
   Begin VB.TextBox txtRemoteFolder 
      Height          =   285
      Left            =   1185
      TabIndex        =   9
      Text            =   "/pub/temp/users/haggerty"
      Top             =   405
      Width           =   3780
   End
   Begin VB.TextBox txtFTPsite 
      Height          =   285
      Left            =   1185
      TabIndex        =   6
      Text            =   "ftp.eol.ucar.edu"
      Top             =   90
      Width           =   3780
   End
   Begin MSComCtl2.MonthView MonthView1 
      Height          =   2370
      Left            =   45
      TabIndex        =   5
      Top             =   2565
      Width           =   2700
      _ExtentX        =   4763
      _ExtentY        =   4180
      _Version        =   393216
      ForeColor       =   -2147483630
      BackColor       =   -2147483633
      Appearance      =   1
      StartOfWeek     =   76218369
      CurrentDate     =   39581
   End
   Begin VB.Frame Frame1 
      Caption         =   "FTP Operation "
      Height          =   810
      Left            =   2850
      TabIndex        =   3
      Top             =   3285
      Width           =   2100
      Begin VB.OptionButton optFTPdirection 
         Caption         =   "Send Files"
         Height          =   195
         Index           =   1
         Left            =   105
         TabIndex        =   20
         Top             =   525
         Width           =   1650
      End
      Begin VB.OptionButton optFTPdirection 
         Caption         =   "Receive Files"
         Height          =   195
         Index           =   0
         Left            =   105
         TabIndex        =   19
         Top             =   270
         Value           =   -1  'True
         Width           =   1650
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Files to FTP "
      Height          =   780
      Left            =   2850
      TabIndex        =   2
      Top             =   2490
      Width           =   2100
      Begin VB.OptionButton optXferFiles 
         Caption         =   "All .txt Files"
         Height          =   195
         Index           =   0
         Left            =   105
         TabIndex        =   0
         ToolTipText     =   "Perform operations on All text files"
         Top             =   240
         Width           =   1170
      End
      Begin VB.OptionButton optXferFiles 
         Caption         =   "MTPcurrent.RAW"
         Height          =   195
         Index           =   1
         Left            =   105
         TabIndex        =   1
         ToolTipText     =   "Perfom operations on only this file"
         Top             =   480
         Width           =   1590
      End
   End
   Begin VB.Label Label 
      Caption         =   "Current"
      Height          =   210
      Index           =   8
      Left            =   1695
      TabIndex        =   26
      Top             =   2310
      Width           =   525
   End
   Begin VB.Label Label 
      Caption         =   "Files Read"
      Height          =   210
      Index           =   7
      Left            =   45
      TabIndex        =   24
      Top             =   2310
      Width           =   750
   End
   Begin VB.Label Label 
      Caption         =   "Status"
      Height          =   210
      Index           =   6
      Left            =   45
      TabIndex        =   23
      Top             =   1995
      Width           =   750
   End
   Begin VB.Label Label 
      Caption         =   "Filename_Date"
      Height          =   210
      Index           =   5
      Left            =   45
      TabIndex        =   17
      Top             =   1680
      Width           =   1125
   End
   Begin VB.Label Label 
      Caption         =   "Local Folder"
      Height          =   210
      Index           =   4
      Left            =   45
      TabIndex        =   15
      Top             =   1365
      Width           =   975
   End
   Begin VB.Label Label 
      Caption         =   "FTP Site"
      Height          =   225
      Index           =   3
      Left            =   45
      TabIndex        =   12
      Top             =   120
      Width           =   975
   End
   Begin VB.Label Label 
      Caption         =   "Username"
      Height          =   195
      Index           =   2
      Left            =   45
      TabIndex        =   10
      Top             =   750
      Width           =   975
   End
   Begin VB.Label Label 
      Caption         =   "Remote Folder"
      Height          =   195
      Index           =   1
      Left            =   45
      TabIndex        =   8
      Top             =   450
      Width           =   1110
   End
   Begin VB.Label Label 
      Caption         =   "Password"
      Height          =   195
      Index           =   0
      Left            =   45
      TabIndex        =   7
      Top             =   1065
      Width           =   1035
   End
   Begin VB.Label lblStatus 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      BorderStyle     =   1  'Fixed Single
      Caption         =   "#"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1185
      TabIndex        =   4
      Top             =   1995
      Width           =   3795
   End
End
Attribute VB_Name = "frmMTPftp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Const cAppTitle = "FTP Using Internet Transfer Control"

Dim gsServer   As String
Dim gsUser     As String
Dim gsPswd     As String
Dim gsLocalDir As String
Dim gsFTPDir   As String
Dim FormLoaded As Boolean
Dim DIRresult   As String
Sub FTPreceive(DIRresult As String)
Dim bSingleFile As Boolean
Dim sText$, sError$, sFileType$, sMsg$, sSql$
Dim oFTP As New FTP
Static DirectoryLengthLast As Long

  On Error GoTo cmdReceiveError

' See what file(s) to transfer.
  lblStatus = ""
  If optXferFiles(1) Then
    bSingleFile = True
    sText = "MTPcurrent.RAW"
    sFileType = "MTPcurrent.RAW"
  Else
    bSingleFile = False
    sText = "All .txt files"
    sFileType = ".txt"
  End If

' Confirm the transfer.
' If MsgBox("Receive " & sText & "?", vbQuestion + vbYesNo, cAppTitle) = vbNo Then Exit Sub

' Inform the user.
  Screen.MousePointer = vbHourglass
  lblStatus = "Receiving " & sText

' Do the FTP operation.
  sMsg = "File transfer successful."
  gsLocalDir = txtLocalFolder.Text
  With oFTP
    ' Initialize the FTP connection.
    If Not .FTPInit(gsServer, gsUser, gsPswd, 0, 0, "", sError) Then GoTo cmdReceiveError
    ' Transfer the file(s)
    If bSingleFile Then
        Call .FTPGet(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then GoTo cmdReceiveError
    Else
        Call .FTPGetAll(gsLocalDir, Left$(txtFilenameDate.Text, 19) + sFileType, gsFTPDir, sError, FileList(), nFiles, DirectoryLengthLast)
        If sError <> "" Then
            sMsg = sError
            GoTo cmdReceiveError
        End If
    End If
    ' Close the connection.
    Call .FTPClose
  End With
  lblStatus = sMsg

' House cleaning.
  Set oFTP = Nothing
  Screen.MousePointer = vbDefault
Exit Sub

cmdReceiveError:
  If sError = "" Then sError = "Error receiving data."
  Call pDisplayError(sError)
  lblStatus = ""
  Set oFTP = Nothing

End Sub

Sub FTPsend()
Dim bSingleFile As Boolean
Dim sText       As String
Dim sError      As String
Dim sFileType   As String
Dim sMsg        As String
Dim oFTP        As New FTP


On Error GoTo cmdSendError
'
' See what file(s) to transfer.
'
lblStatus = ""
If optXferFiles(1) Then
    bSingleFile = True
    sText = "MTPcurrent.RAW"
    sFileType = "MTPcurrent.RAW"
Else
    bSingleFile = False
    sText = "all .txt files"
    sFileType = ".txt"
End If

'
' Confirm the transfer.
'
If MsgBox("Send " & sText & "?", vbQuestion + vbYesNo, _
   cAppTitle) = vbNo Then Exit Sub
   
'
' Inform the user.
'
Screen.MousePointer = vbHourglass
lblStatus = "Sending " & sText

'
' Do the FTP operation.
'
sMsg = "File transfer successful."
With oFTP
    '
    ' Initialize the FTP connection.
    '
    If Not .FTPInit(gsServer, gsUser, gsPswd, 0, 0, "", sError) Then
        GoTo cmdSendError
    End If

    '
    ' Transfer the file(s)
    '
    If bSingleFile Then
        Call .FTPPut(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then GoTo cmdSendError
    Else
        Call .FTPPutAll(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then
            sMsg = sError
            GoTo cmdSendError
        End If
    End If
    '
    ' Close the connection.
    '
    Call .FTPClose
End With
lblStatus = sMsg
'
' House cleaning.
'
Set oFTP = Nothing
Screen.MousePointer = vbDefault
Exit Sub

cmdSendError:
    If sError = "" Then sError = "Error sending data."
    Call pDisplayError(sError)
    lblStatus = ""
    Set oFTP = Nothing

End Sub


Private Sub cmdExit_Click()

  Unload Me
  End
  
End Sub

Private Sub cmdReceive_Click()
Dim bSingleFile As Boolean
Dim sText$, sError$, sFileType$, sMsg$, sSql$
Dim oFTP As New FTP
Static DirectoryLengthLast As Long

  On Error GoTo cmdReceiveError

' See what file(s) to transfer.
  lblStatus = ""
  If optXferFiles(1) Then
    bSingleFile = True
    sText = "MTPcurrent.RAW"
    sFileType = "MTPcurrent.RAW"
  Else
    bSingleFile = False
    sText = "All .txt files"
    sFileType = ".txt"
  End If

' Confirm the transfer.
' If MsgBox("Receive " & sText & "?", vbQuestion + vbYesNo, cAppTitle) = vbNo Then Exit Sub

' Inform the user.
  Screen.MousePointer = vbHourglass
  lblStatus = "Receiving " & sText

' Do the FTP operation.
  sMsg = "File transfer successful."
  gsLocalDir = txtLocalFolder.Text
  With oFTP
    ' Initialize the FTP connection.
    If Not .FTPInit(gsServer, gsUser, gsPswd, 0, 0, "", sError) Then GoTo cmdReceiveError
    ' Transfer the file(s)
    If bSingleFile Then
        Call .FTPGet(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then GoTo cmdReceiveError
    Else
        Call .FTPGetAll(gsLocalDir, sFileType, gsFTPDir, sError, FileList(), nFiles, DirectoryLengthLast)
        If sError <> "" Then
            sMsg = sError
            GoTo cmdReceiveError
        End If
    End If
    ' Close the connection.
    Call .FTPClose
  End With
  lblStatus = sMsg

' House cleaning.
  Set oFTP = Nothing
  Screen.MousePointer = vbDefault
Exit Sub

cmdReceiveError:
  If sError = "" Then sError = "Error receiving data."
  Call pDisplayError(sError)
  lblStatus = ""
  Set oFTP = Nothing

End Sub

Private Sub pDisplayError(ByVal sError As String)
Dim sMsg As String

Screen.MousePointer = vbDefault
If Trim$(sError) = "" Then
    sMsg = Err.Description
Else
    sMsg = sError & vbCrLf & vbCrLf & Err.Description
End If
If Err.Number = 0 Then
    MsgBox sMsg, vbCritical, cAppTitle
Else
    MsgBox sMsg & " (" & CStr(Err.Number) & ")", vbCritical, cAppTitle
End If
End Sub

Private Sub cmdSend_Click()
Dim bSingleFile As Boolean
Dim sText       As String
Dim sError      As String
Dim sFileType   As String
Dim sMsg        As String
Dim oFTP        As New FTP


On Error GoTo cmdSendError
'
' See what file(s) to transfer.
'
lblStatus = ""
If optXferFiles(1) Then
    bSingleFile = True
    sText = "MTPcurrent.RAW"
    sFileType = "MTPcurrent.RAW"
Else
    bSingleFile = False
    sText = "all .txt files"
    sFileType = ".txt"
End If

'
' Confirm the transfer.
'
If MsgBox("Send " & sText & "?", vbQuestion + vbYesNo, _
   cAppTitle) = vbNo Then Exit Sub
   
'
' Inform the user.
'
Screen.MousePointer = vbHourglass
lblStatus = "Sending " & sText

'
' Do the FTP operation.
'
sMsg = "File transfer successful."
With oFTP
    '
    ' Initialize the FTP connection.
    '
    If Not .FTPInit(gsServer, gsUser, gsPswd, 0, 0, "", sError) Then
        GoTo cmdSendError
    End If

    '
    ' Transfer the file(s)
    '
    If bSingleFile Then
        Call .FTPPut(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then GoTo cmdSendError
    Else
        Call .FTPPutAll(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then
            sMsg = sError
            GoTo cmdSendError
        End If
    End If
    '
    ' Close the connection.
    '
    Call .FTPClose
End With
lblStatus = sMsg
'
' House cleaning.
'
Set oFTP = Nothing
Screen.MousePointer = vbDefault
Exit Sub

cmdSendError:
    If sError = "" Then sError = "Error sending data."
    Call pDisplayError(sError)
    lblStatus = ""
    Set oFTP = Nothing
End Sub

Private Sub File1_Click()

End Sub


Private Sub cmdStart_Click()

  If optFTPdirection(0).Value = True Then
    Timer1.Interval = 1000
    Timer1.Enabled = True
    FileIndex = 0
    nFiles = 0
    'Call FTPreceive
  Else
    Timer1.Enabled = True
    FileIndex = 0
    nFiles = 0
    Call FTPsend
  End If
End Sub

Private Sub Form_Load()
'
' Initialize Logon/FTP parameters.
' User should be prompted for logon info
'
  If FormLoaded Then Exit Sub
  
  gsUser = txtUsername.Text         '"anonymous"    'User ID to logon to FTP server.
  gsPswd = txtPassword.Text         '"Michael.J.Mahoney@jpl.nasa.gov"   'Password to logon to FTP server.

  gsServer = txtFTPsite.Text        '"ftp.eol.ucar.edu"       'FTP server name.
  gsLocalDir = txtLocalFolder.Text  '"C:\Temp"  'Folder on local client holding files to FTP.
  gsFTPDir = txtRemoteFolder.Text   '"/pub/temp/users/haggerty"     'Folder on FTP server to xfer files to/from

  Timer1.Interval = 60000           'check once a minute
  Timer1.Enabled = False
  MonthView1.Value = Date
  MonthView1_DateClick Date
  FormLoaded = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
  
  Set frmMTPftp = Nothing

End Sub


Private Sub MonthView1_DateClick(ByVal DateClicked As Date)
Dim yyyymmdd$
Dim fso As New FileSystemObject

  yyyymmdd$ = Format(Year(DateClicked), "0000") + Format(Month(DateClicked), "00") + Format(Day(DateClicked), "00")
' Debug.Print DateClicked; "  "; yyyymmdd$
  txtFilenameDate.Text = "MTPcurrent_" & yyyymmdd$
  If Not fso.FolderExists(txtLocalFolder.Text + yyyymmdd$ + "/") Then
    fso.CreateFolder (txtLocalFolder.Text + yyyymmdd$ + "/")
  End If
  txtLocalFolder.Text = txtLocalFolder.Text + yyyymmdd$ + "/"
End Sub


Private Sub Timer1_Timer()
Dim INPlu%, OUTlu%, Line$
Static nFilesLast%

  Timer1.Enabled = False
  If nFiles > 0 Then
  ' Set appropriate FileIndex
    If FileIndex = 0 Then
      If chkUseLatest.Value = 1 Then
        FileIndex = nFiles
      Else
        FileIndex = 1
        Timer1.Interval = 4000
      End If
    End If
    If FileIndex = nFiles Then Timer1.Interval = 60000
  End If
  
' Copy next RAW file to MTPcurrent.RAW
  If nFiles > FileIndex Then
    INPlu = FreeFile
    Open txtLocalFolder.Text + FileList(FileIndex) For Input As #INPlu
    OUTlu = FreeFile
    Open "C:\MTP\RTA\RAW\MTPcurrent.RAW" For Output As #OUTlu
    Do
      Line Input #INPlu, Line$
      Print #OUTlu, Line$
    Loop Until EOF(INPlu)
    Close INPlu, OUTlu
    txtFilenameDate.Text = FileList(FileIndex)
    FileIndex = FileIndex + 1
    DoEvents
  End If
  
' Check for more files
  Call FTPreceive(DIRresult)
  txtFilesRead.Text = Format(nFiles, "0")
  txtCurrent.Text = Format(FileIndex, "0")
  'Debug.Print "FileIndex= "; FileIndex; " "; Date; " "; Time; FileDateTime("C:\MTP\RTA\RAW\MTPcurrent.RAW")
  Timer1.Enabled = True

End Sub


