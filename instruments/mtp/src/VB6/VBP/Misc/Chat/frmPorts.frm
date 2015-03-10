VERSION 5.00
Begin VB.Form frmPorts 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Ports"
   ClientHeight    =   1935
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3975
   Icon            =   "frmPorts.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1935
   ScaleWidth      =   3975
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtMaxConnections 
      Height          =   285
      Left            =   2640
      TabIndex        =   4
      Top             =   840
      Width           =   1215
   End
   Begin VB.TextBox txtFirstRemotePort 
      Height          =   285
      Left            =   2640
      TabIndex        =   3
      Top             =   480
      Width           =   1215
   End
   Begin VB.TextBox txtFirstLocalPort 
      Height          =   285
      Left            =   2640
      TabIndex        =   2
      Top             =   120
      Width           =   1215
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   495
      Left            =   2040
      TabIndex        =   1
      Top             =   1320
      Width           =   1215
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   495
      Left            =   720
      TabIndex        =   0
      Top             =   1320
      Width           =   1215
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      Caption         =   "First Remote Port:"
      Height          =   195
      Left            =   120
      TabIndex        =   7
      Top             =   480
      Width           =   1260
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Maximum Number of Connections:"
      Height          =   195
      Left            =   120
      TabIndex        =   6
      Top             =   840
      Width           =   2415
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "First Local Port:"
      Height          =   195
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   1095
   End
End
Attribute VB_Name = "frmPorts"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdCancel_Click()
Unload Me
End Sub
Private Sub cmdOK_Click()
'OK was clicked.

Dim i As Integer

'Redefine the settings.
glFirstLocalPort = txtFirstLocalPort.Text
glFirstRemotePort = txtFirstRemotePort.Text
giMaxConnections = txtMaxConnections.Text

'Make sure enough Winsock controls are loaded to handle all connections.
For i = frmMain.sckConnection.UBound + 1 To giMaxConnections - 1
    Load frmMain.sckConnection(i)
Next i

Unload Me
End Sub
Private Sub Form_Load()
'Show the current settings in the text boxes.
txtFirstLocalPort.Text = glFirstLocalPort
txtFirstRemotePort.Text = glFirstRemotePort
txtMaxConnections.Text = giMaxConnections
End Sub
