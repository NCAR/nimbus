VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "mswinsck.ocx"
Begin VB.Form Form1 
   Caption         =   "Multicast Test"
   ClientHeight    =   2820
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5355
   LinkTopic       =   "Form1"
   ScaleHeight     =   2820
   ScaleWidth      =   5355
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Left            =   75
      Top             =   1890
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "Connect"
      Height          =   405
      Left            =   2970
      TabIndex        =   0
      Top             =   2400
      Width           =   915
   End
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   60
      Top             =   2325
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public RemoteIP$, RemotePort$, LocalPort$
Function fTstamp() As String
' Generate time stamp in format: yyyymmdd_hhmmss
Dim ymd$, hms$

  ymd$ = Right$(Date$, 4) + Left$(Date$, 2) + Mid$(Date$, 4, 2)
  hms$ = Time$
  hms$ = Left$(hms$, 2) + Mid$(hms$, 4, 2) + Right$(hms$, 2)
  fTstamp = ymd$ + "_" + hms$

End Function

Private Sub Command1_Click()

End Sub

Private Sub cmdConnect_Click()

  If cmdConnect.Caption = "Connect" Then
    Timer1.Enabled = True
    Timer1.Interval = 2000
    RemoteIP = "239.0.0.20"   'Winsock1.LocalIP
    RemotePort = "30001"
    Winsock1_Connect
    cmdConnect.Caption = "Close"
  Else
    Timer1.Enabled = False
    Winsock1_Close
    cmdConnect.Caption = "Connect"
  End If
  
End Sub


Private Sub Timer1_Timer()
Dim UDP_Status$

  Timer1.Enabled = False
  UDP_Status = "<?xml version=""1.0""?><group><name>MTP</name><clock>"
  UDP_Status = UDP_Status & fTstamp() & "</clock></group>"
  Winsock1.SendData UDP_Status
  Timer1.Enabled = True
  
End Sub

Private Sub Winsock1_Close()
  
  Do Until Winsock1.State = sckClosed
    Winsock1.Close
    DoEvents
  Loop

End Sub

Private Sub Winsock1_Connect()
On Error GoTo errhandler

  With Winsock1
' Set the remotehost property
  .RemoteHost = RemoteIP
' Set the remoteport property.
' This should be equal to the localhost property of the remote machine.
  .RemotePort = RemotePort
' The localport property cannot be changed,so check if it has already been set
    If .LocalPort = Empty Then
      .LocalPort = "30001"
      .Bind .LocalPort
    End If
  End With
  
  Exit Sub

errhandler:
  MsgBox "Winsock failed to establish connection with remote server", vbCritical

End Sub


Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
'New_Text is the text that has just arrived
'from across the socket
Dim New_Text As String
'Get the new text
Winsock1.GetData New_Text
'Show the new text
'Text1.SelText = New_Text
'Frame1.Caption = Winsock1.RemoteHostIP
'Show the byte size of this transmission in the statusbar
'StatusBar1.Panels(2).Text = "  Recieved " & bytesTotal & " bytes  "
End Sub


