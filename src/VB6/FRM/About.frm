VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About"
   ClientHeight    =   3735
   ClientLeft      =   1620
   ClientTop       =   2070
   ClientWidth     =   6645
   ControlBox      =   0   'False
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   -1  'True
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "About.frx":0000
   LinkMode        =   1  'Source
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   3735
   ScaleWidth      =   6645
   Begin VB.CommandButton cmdOK 
      Cancel          =   -1  'True
      Caption         =   "OK"
      Default         =   -1  'True
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2760
      TabIndex        =   4
      Top             =   2400
      Width           =   1095
   End
   Begin VB.Line linSeparator 
      X1              =   480
      X2              =   6120
      Y1              =   1440
      Y2              =   1440
   End
   Begin VB.Label lblHeading 
      Alignment       =   2  'Center
      Caption         =   "Heading"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   240
      TabIndex        =   1
      Top             =   240
      Width           =   6135
   End
   Begin VB.Label lblApplication 
      Alignment       =   2  'Center
      Caption         =   "Application"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   18
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00008000&
      Height          =   492
      Left            =   120
      TabIndex        =   0
      Top             =   720
      Width           =   6372
   End
   Begin VB.Label lblVersion 
      Alignment       =   2  'Center
      Caption         =   "Version: "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   480
      TabIndex        =   2
      Top             =   1680
      Width           =   5655
   End
   Begin VB.Label lblCopyright 
      Alignment       =   2  'Center
      Caption         =   "Copyright © 1996"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   840
      TabIndex        =   3
      Top             =   1920
      Width           =   4935
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Property Let Version(Version As String)
' Define string property for Version
  lblVersion.Caption = Version
End Property

Private Sub cmdOK_Click()
    'Cancel About form
    Unload frmAbout
End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()
    'Center this form
    Left = (1024 * Screen.TwipsPerPixelX - Width) \ 2
    Top = (768 * Screen.TwipsPerPixelY - Height) \ 2
    'Set defaults
    lblApplication.Caption = "_Application_"
    lblHeading.Caption = "_Heading_"
    lblVersion.Caption = "_Version_"
    lblCopyright.Caption = "_Copyright_"
End Sub

Public Sub Display()
' Display self as modal
  Me.Show vbModal
End Sub

Property Let Application(Application As String)
' Define string property for Application
  lblApplication.Caption = "MJ's MTP Data Analysis Program"
End Property

Property Let Heading(Heading As String)
' Define string property for Heading
  lblHeading.Caption = Heading
End Property

Property Let Copyright(Copyright As String)
' Build complete Copyright string property
  lblCopyright.Caption = "Copyright © " & "MJsoft"
End Property

