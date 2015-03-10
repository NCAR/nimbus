VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "FTP Using Internet Transfer Control"
   ClientHeight    =   2220
   ClientLeft      =   5670
   ClientTop       =   3795
   ClientWidth     =   4590
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2220
   ScaleWidth      =   4590
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame Frame1 
      Caption         =   "File Transfer Operation:"
      Height          =   795
      Left            =   105
      TabIndex        =   5
      Top             =   945
      Width           =   4380
      Begin VB.CommandButton cmdSend 
         Caption         =   "Send to FTP Server"
         Height          =   375
         Left            =   210
         Style           =   1  'Graphical
         TabIndex        =   2
         ToolTipText     =   "Begin the file transfer operation."
         Top             =   315
         Width           =   1935
      End
      Begin VB.CommandButton cmdReceive 
         Caption         =   "Receive from FTP Server"
         Height          =   375
         Left            =   2310
         Style           =   1  'Graphical
         TabIndex        =   3
         ToolTipText     =   "Begin the file transfer operation."
         Top             =   315
         Width           =   1935
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Files to FTP:"
      Height          =   735
      Left            =   105
      TabIndex        =   4
      Top             =   105
      Width           =   4395
      Begin VB.OptionButton optXferFiles 
         Caption         =   "All .txt Files"
         Height          =   330
         Index           =   0
         Left            =   420
         TabIndex        =   0
         ToolTipText     =   "Perform operations on All text files"
         Top             =   315
         Width           =   1170
      End
      Begin VB.OptionButton optXferFiles 
         Caption         =   "TheScarms.txt"
         Height          =   330
         Index           =   1
         Left            =   2520
         TabIndex        =   1
         ToolTipText     =   "Perfom operations on only this file"
         Top             =   315
         Width           =   1380
      End
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
      Left            =   105
      TabIndex        =   6
      Top             =   1890
      Width           =   4440
   End
End
Attribute VB_Name = "frmMain"
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

Private Sub cmdReceive_Click()
Dim bSingleFile As Boolean
Dim sText       As String
Dim sError      As String
Dim sFileType   As String
Dim sMsg        As String
Dim sSql        As String
Dim oFTP        As New FTP

On Error GoTo cmdReceiveError
'
' See what file(s) to transfer.
'
lblStatus = ""
If optXferFiles(1) Then
    bSingleFile = True
    sText = "TheScarms.txt"
    sFileType = "TheScarms.TXT"
Else
    bSingleFile = False
    sText = "all .txt files"
    sFileType = ".txt"
End If
'
' Confirm the transfer.
'
If MsgBox("Receive " & sText & "?", vbQuestion + vbYesNo, _
   cAppTitle) = vbNo Then Exit Sub

'
' Inform the user.
'
Screen.MousePointer = vbHourglass
lblStatus = "Receiving " & sText

'
' Do the FTP operation.
'
sMsg = "File transfer successful."
With oFTP
    '
    ' Initialize the FTP connection.
    '
    If Not .FTPInit(gsServer, gsUser, gsPswd, 0, 0, "", sError) Then
        GoTo cmdReceiveError
    End If
    
    '
    ' Transfer the file(s)
    '
    If bSingleFile Then
        Call .FTPGet(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then GoTo cmdReceiveError
    Else
        Call .FTPGetAll(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then
            sMsg = sError
            GoTo cmdReceiveError
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
    sText = "TheScarms.txt"
    sFileType = "TheScarms.TXT"
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


Private Sub Form_Load()
'
' Initialize Logon/FTP parameters.
' User should be prompted for logon info.
'
gsUser = "TheScarms"    'User ID to logon to FTP server.
gsPswd = "MyPassword"   'Password to logon to FTP server.

gsServer = "Host"       'FTP server name.
gsLocalDir = "C:\Temp"  'Folder on local client holding files to FTP.
gsFTPDir = "C:\FTP"     'Folder on FTP server to xfer files to/from

End Sub

Private Sub Form_Unload(Cancel As Integer)
Set frmMain = Nothing
End Sub


