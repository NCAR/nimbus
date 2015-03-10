VERSION 5.00
Begin VB.Form frmSummary 
   Caption         =   "REF File Limits Summary"
   ClientHeight    =   7875
   ClientLeft      =   7950
   ClientTop       =   1050
   ClientWidth     =   4140
   Icon            =   "Limits.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   7875
   ScaleWidth      =   4140
   Begin VB.TextBox Text48 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   81
      Top             =   2160
      Width           =   975
   End
   Begin VB.TextBox Text47 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   80
      Top             =   2160
      Width           =   975
   End
   Begin VB.TextBox Text45 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   78
      Top             =   2400
      Width           =   975
   End
   Begin VB.TextBox Text46 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   77
      Top             =   2400
      Width           =   975
   End
   Begin VB.TextBox Text39 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   75
      Top             =   1920
      Width           =   975
   End
   Begin VB.TextBox Text40 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   74
      Top             =   1920
      Width           =   975
   End
   Begin VB.TextBox txtElapseds 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   73
      Top             =   1560
      Width           =   975
   End
   Begin VB.TextBox txtElapsedHHMMSS 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   72
      Top             =   1560
      Width           =   975
   End
   Begin VB.TextBox Text44 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   70
      Top             =   3000
      Width           =   975
   End
   Begin VB.TextBox Text43 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   69
      Top             =   3000
      Width           =   975
   End
   Begin VB.TextBox Text42 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   66
      Top             =   1320
      Width           =   975
   End
   Begin VB.TextBox Text41 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   65
      Top             =   1320
      Width           =   975
   End
   Begin VB.TextBox Text50 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1080
      MultiLine       =   -1  'True
      TabIndex        =   63
      Top             =   720
      Width           =   495
   End
   Begin VB.TextBox Text38 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   62
      Top             =   7560
      Width           =   975
   End
   Begin VB.TextBox Text37 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   61
      Top             =   7560
      Width           =   975
   End
   Begin VB.TextBox Text35 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   58
      Top             =   7200
      Width           =   975
   End
   Begin VB.TextBox Text36 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   57
      Top             =   7200
      Width           =   975
   End
   Begin VB.TextBox Text33 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   55
      Top             =   6960
      Width           =   975
   End
   Begin VB.TextBox Text34 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   54
      Top             =   6960
      Width           =   975
   End
   Begin VB.TextBox Text32 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   53
      Top             =   6720
      Width           =   975
   End
   Begin VB.TextBox Text31 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   52
      Top             =   6720
      Width           =   975
   End
   Begin VB.TextBox Text30 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   50
      Top             =   6480
      Width           =   975
   End
   Begin VB.TextBox Text29 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   49
      Top             =   6480
      Width           =   975
   End
   Begin VB.TextBox Text28 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   47
      Top             =   6240
      Width           =   975
   End
   Begin VB.TextBox Text27 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   46
      Top             =   6240
      Width           =   975
   End
   Begin VB.TextBox Text26 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   44
      Top             =   5880
      Width           =   975
   End
   Begin VB.TextBox Text25 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   43
      Top             =   5880
      Width           =   975
   End
   Begin VB.TextBox Text24 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   41
      Top             =   5640
      Width           =   975
   End
   Begin VB.TextBox Text23 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   40
      Top             =   5640
      Width           =   975
   End
   Begin VB.TextBox Text22 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   38
      Top             =   5400
      Width           =   975
   End
   Begin VB.TextBox Text21 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   37
      Top             =   5400
      Width           =   975
   End
   Begin VB.TextBox Text20 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   35
      Top             =   5040
      Width           =   975
   End
   Begin VB.TextBox Text19 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   34
      Top             =   5040
      Width           =   975
   End
   Begin VB.TextBox Text18 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   32
      Top             =   4800
      Width           =   975
   End
   Begin VB.TextBox Text17 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   31
      Top             =   4800
      Width           =   975
   End
   Begin VB.TextBox Text16 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   29
      Top             =   4560
      Width           =   975
   End
   Begin VB.TextBox Text15 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   28
      Top             =   4560
      Width           =   975
   End
   Begin VB.TextBox Text14 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   26
      Top             =   3240
      Width           =   975
   End
   Begin VB.TextBox Text13 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   25
      Top             =   3240
      Width           =   975
   End
   Begin VB.TextBox Text12 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   23
      Top             =   2760
      Width           =   975
   End
   Begin VB.TextBox Text11 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   22
      Top             =   2760
      Width           =   975
   End
   Begin VB.TextBox Text10 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   20
      Top             =   3720
      Width           =   975
   End
   Begin VB.TextBox Text9 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   19
      Top             =   3720
      Width           =   975
   End
   Begin VB.TextBox Text8 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   17
      Top             =   3480
      Width           =   975
   End
   Begin VB.TextBox Text7 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   16
      Top             =   3480
      Width           =   975
   End
   Begin VB.TextBox Text6 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   14
      Top             =   4200
      Width           =   975
   End
   Begin VB.TextBox Text5 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   13
      Top             =   4200
      Width           =   975
   End
   Begin VB.TextBox Text4 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   11
      Top             =   3960
      Width           =   975
   End
   Begin VB.TextBox Text3 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   10
      Top             =   3960
      Width           =   975
   End
   Begin VB.TextBox Text2 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   8
      Top             =   1080
      Width           =   975
   End
   Begin VB.TextBox Text1 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2040
      MultiLine       =   -1  'True
      TabIndex        =   7
      Top             =   1080
      Width           =   975
   End
   Begin VB.TextBox txtREF 
      Height          =   285
      Left            =   480
      TabIndex        =   1
      Top             =   360
      Width           =   3615
   End
   Begin VB.TextBox txtMP 
      Height          =   285
      Left            =   480
      TabIndex        =   0
      Top             =   0
      Width           =   3615
   End
   Begin VB.Label Label30 
      Caption         =   "[hh:mm:ss]"
      Height          =   255
      Left            =   1200
      TabIndex        =   82
      Top             =   2160
      Width           =   855
   End
   Begin VB.Label Label29 
      Caption         =   "Flt Duration    [hh:mm:ss,s]"
      Height          =   255
      Left            =   120
      TabIndex        =   79
      Top             =   2400
      Width           =   1935
   End
   Begin VB.Label Label28 
      Caption         =   "Takeoff/Landing           [s]"
      Height          =   255
      Left            =   120
      TabIndex        =   76
      Top             =   1920
      Width           =   1935
   End
   Begin VB.Label Label3 
      Caption         =   "MTP On Time [hh:mm:ss,s]"
      Height          =   255
      Left            =   120
      TabIndex        =   71
      Top             =   1560
      Width           =   1935
   End
   Begin VB.Label Label27 
      Caption         =   "A/C GPS Altitude [km]"
      Height          =   255
      Left            =   120
      TabIndex        =   68
      Top             =   3000
      Width           =   1935
   End
   Begin VB.Label Label26 
      Caption         =   " [hh:mm:ss]"
      Height          =   255
      Left            =   1200
      TabIndex        =   67
      Top             =   1320
      Width           =   855
   End
   Begin VB.Label Label25 
      Caption         =   "Progress [%]"
      Height          =   255
      Left            =   120
      TabIndex        =   64
      Top             =   720
      Width           =   975
   End
   Begin VB.Label Label24 
      Caption         =   "Meridional Region Index"
      Height          =   255
      Left            =   120
      TabIndex        =   60
      Top             =   7560
      Width           =   1815
   End
   Begin VB.Label Label23 
      Caption         =   "PT @ Tmin/max [K]"
      Height          =   255
      Left            =   120
      TabIndex        =   59
      Top             =   7200
      Width           =   1695
   End
   Begin VB.Label Label22 
      Caption         =   "Altitude @Tmin/max [km]"
      Height          =   255
      Left            =   120
      TabIndex        =   56
      Top             =   6960
      Width           =   1815
   End
   Begin VB.Label Label21 
      Caption         =   "ATP Temperature [K]"
      Height          =   255
      Left            =   120
      TabIndex        =   51
      Top             =   6720
      Width           =   1695
   End
   Begin VB.Label Label20 
      Caption         =   "ATP Altitude [km]"
      Height          =   255
      Left            =   120
      TabIndex        =   48
      Top             =   6480
      Width           =   1455
   End
   Begin VB.Label Label19 
      Caption         =   "Lapse Rate [K/km]"
      Height          =   255
      Left            =   120
      TabIndex        =   45
      Top             =   6240
      Width           =   1455
   End
   Begin VB.Label Label18 
      Caption         =   "Trop2 Temperature [K]"
      Height          =   255
      Left            =   120
      TabIndex        =   42
      Top             =   5880
      Width           =   1695
   End
   Begin VB.Label Label17 
      Caption         =   "Trop2 Theta [K]"
      Height          =   255
      Left            =   120
      TabIndex        =   39
      Top             =   5640
      Width           =   1455
   End
   Begin VB.Label Label16 
      Caption         =   "Trop2 Altitude [km]"
      Height          =   255
      Left            =   120
      TabIndex        =   36
      Top             =   5400
      Width           =   1455
   End
   Begin VB.Label Label15 
      Caption         =   "Trop1 Temperature [K]"
      Height          =   255
      Left            =   120
      TabIndex        =   33
      Top             =   5040
      Width           =   1695
   End
   Begin VB.Label Label14 
      Caption         =   "Trop1 Theta [K]"
      Height          =   255
      Left            =   120
      TabIndex        =   30
      Top             =   4800
      Width           =   1455
   End
   Begin VB.Label Label13 
      Caption         =   "Trop1 Altitude [km]"
      Height          =   255
      Left            =   120
      TabIndex        =   27
      Top             =   4560
      Width           =   1455
   End
   Begin VB.Label Label12 
      Caption         =   "A/C OAT [K]"
      Height          =   255
      Left            =   120
      TabIndex        =   24
      Top             =   3240
      Width           =   1455
   End
   Begin VB.Label Label11 
      Caption         =   "A/C Pressure Altitude [km]"
      Height          =   255
      Left            =   120
      TabIndex        =   21
      Top             =   2760
      Width           =   1935
   End
   Begin VB.Label Label10 
      Caption         =   "Roll [deg]"
      Height          =   255
      Left            =   120
      TabIndex        =   18
      Top             =   3720
      Width           =   1455
   End
   Begin VB.Label Label9 
      Caption         =   "Pitch [deg]"
      Height          =   255
      Left            =   120
      TabIndex        =   15
      Top             =   3480
      Width           =   1455
   End
   Begin VB.Label Label8 
      Caption         =   "Longitude [deg]"
      Height          =   255
      Left            =   120
      TabIndex        =   12
      Top             =   4200
      Width           =   1455
   End
   Begin VB.Label Label7 
      Caption         =   "Latitude [deg]"
      Height          =   255
      Left            =   120
      TabIndex        =   9
      Top             =   3960
      Width           =   1455
   End
   Begin VB.Label Label6 
      Caption         =   "UT                                 [s]"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   1080
      Width           =   2055
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      Caption         =   "Maximum"
      Height          =   255
      Left            =   3120
      TabIndex        =   5
      Top             =   840
      Width           =   975
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      Caption         =   "Minimum"
      Height          =   255
      Left            =   2040
      TabIndex        =   4
      Top             =   840
      Width           =   975
   End
   Begin VB.Label Label2 
      Caption         =   "REF"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   0
      TabIndex        =   3
      Top             =   360
      Width           =   495
   End
   Begin VB.Label Label1 
      Caption         =   "MP"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   0
      TabIndex        =   2
      Top             =   0
      Width           =   375
   End
End
Attribute VB_Name = "frmSummary"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

