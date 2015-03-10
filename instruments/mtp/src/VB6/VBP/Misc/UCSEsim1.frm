VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form frmUCSEsim 
   Caption         =   "UCSE Data Display"
   ClientHeight    =   5970
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   10500
   ForeColor       =   &H80000008&
   LinkTopic       =   "Form1"
   ScaleHeight     =   5970
   ScaleWidth      =   10500
   StartUpPosition =   3  'Windows Default
   Begin TabDlg.SSTab SSTab1 
      Height          =   5895
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   10455
      _ExtentX        =   18441
      _ExtentY        =   10398
      _Version        =   393216
      Tabs            =   2
      TabsPerRow      =   2
      TabHeight       =   520
      TabCaption(0)   =   "I/O Control"
      TabPicture(0)   =   "UCSEsim1.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label(20)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label7"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label6"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Label5"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "Label4"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "Label3"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "Label(41)"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "MSComm1"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "Slider2"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "CommonDialog1"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "Slider3"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "chkASCII"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "cmdOpenUCSE"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "cmdGoToFrame"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).Control(14)=   "txtFilename"
      Tab(0).Control(14).Enabled=   0   'False
      Tab(0).Control(15)=   "cbo2Bh"
      Tab(0).Control(15).Enabled=   0   'False
      Tab(0).Control(16)=   "Frame7"
      Tab(0).Control(16).Enabled=   0   'False
      Tab(0).Control(17)=   "txtSliderPosition"
      Tab(0).Control(17).Enabled=   0   'False
      Tab(0).Control(18)=   "txt2B"
      Tab(0).Control(18).Enabled=   0   'False
      Tab(0).Control(19)=   "txt2A"
      Tab(0).Control(19).Enabled=   0   'False
      Tab(0).Control(20)=   "Frame5"
      Tab(0).Control(20).Enabled=   0   'False
      Tab(0).Control(21)=   "txtPacket2Bh"
      Tab(0).Control(21).Enabled=   0   'False
      Tab(0).Control(22)=   "txtPacket2Ah"
      Tab(0).Control(22).Enabled=   0   'False
      Tab(0).Control(23)=   "Timer1"
      Tab(0).Control(23).Enabled=   0   'False
      Tab(0).Control(24)=   "Frame2"
      Tab(0).Control(24).Enabled=   0   'False
      Tab(0).Control(25)=   "Frame1"
      Tab(0).Control(25).Enabled=   0   'False
      Tab(0).Control(26)=   "cmdRefresh"
      Tab(0).Control(26).Enabled=   0   'False
      Tab(0).Control(27)=   "Command1"
      Tab(0).Control(27).Enabled=   0   'False
      Tab(0).Control(28)=   "Frame6"
      Tab(0).Control(28).Enabled=   0   'False
      Tab(0).Control(29)=   "Frame4"
      Tab(0).Control(29).Enabled=   0   'False
      Tab(0).Control(30)=   "chkReverseBits"
      Tab(0).Control(30).Enabled=   0   'False
      Tab(0).Control(31)=   "cmdStart"
      Tab(0).Control(31).Enabled=   0   'False
      Tab(0).Control(32)=   "chkSynthesize"
      Tab(0).Control(32).Enabled=   0   'False
      Tab(0).Control(33)=   "txtBadFrames"
      Tab(0).Control(33).Enabled=   0   'False
      Tab(0).Control(34)=   "chkValid2B"
      Tab(0).Control(34).Enabled=   0   'False
      Tab(0).ControlCount=   35
      TabCaption(1)   =   "UCSE Frame Synthesizer"
      TabPicture(1)   =   "UCSEsim1.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Frame13"
      Tab(1).Control(1)=   "Frame12"
      Tab(1).Control(2)=   "Frame8"
      Tab(1).Control(3)=   "Frame11"
      Tab(1).Control(4)=   "Frame10"
      Tab(1).Control(5)=   "chkRepeat"
      Tab(1).Control(6)=   "txtUCSEfile"
      Tab(1).Control(7)=   "chkWriteToFile"
      Tab(1).Control(8)=   "cmdOpenER2"
      Tab(1).Control(9)=   "txtER2filename"
      Tab(1).Control(10)=   "Frame9"
      Tab(1).ControlCount=   11
      Begin VB.Frame Frame13 
         Caption         =   "Valid CC2 Bits"
         Height          =   4215
         Left            =   -66600
         TabIndex        =   166
         Top             =   360
         Width           =   1935
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "1. Temperature"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   182
            Top             =   240
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "2. Static Pressure"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   181
            Top             =   480
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "3. True Air Speed"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   180
            Top             =   720
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "4. True Altitude"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   179
            Top             =   960
            Width           =   1455
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "5. Drift Angle"
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   178
            Top             =   1200
            Width           =   1455
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "6. Roll Angle"
            Height          =   255
            Index           =   5
            Left            =   120
            TabIndex        =   177
            Top             =   1440
            Width           =   1455
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "7. True Heading"
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   176
            Top             =   1680
            Width           =   1455
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "8. Status 080-1"
            Height          =   255
            Index           =   7
            Left            =   120
            TabIndex        =   175
            Top             =   1920
            Width           =   1455
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "9. Pitch Angle"
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   174
            Top             =   2160
            Width           =   1455
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "10. Attack Angle"
            Height          =   255
            Index           =   9
            Left            =   120
            TabIndex        =   173
            Top             =   2400
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "11. Slip Angle"
            Height          =   255
            Index           =   10
            Left            =   120
            TabIndex        =   172
            Top             =   2640
            Width           =   1455
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "12. ADC Status"
            Height          =   255
            Index           =   11
            Left            =   120
            TabIndex        =   171
            Top             =   2880
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "13. Wind (Spd/Dir)"
            Height          =   255
            Index           =   12
            Left            =   120
            TabIndex        =   170
            Top             =   3120
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "14. LSS"
            Height          =   255
            Index           =   13
            Left            =   120
            TabIndex        =   169
            Top             =   3360
            Width           =   1455
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "16. Status 080-2"
            Height          =   255
            Index           =   14
            Left            =   120
            TabIndex        =   168
            Top             =   3840
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC2 
            Caption         =   "15. DS"
            Height          =   255
            Index           =   15
            Left            =   120
            TabIndex        =   167
            Top             =   3600
            Width           =   1455
         End
      End
      Begin VB.Frame Frame12 
         Caption         =   "Valid CC1 Bits"
         Height          =   4215
         Left            =   -68760
         TabIndex        =   149
         Top             =   360
         Width           =   2055
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "1. Reserve (GPS)"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   165
            Top             =   240
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "2. GMT"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   164
            Top             =   480
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "3. Latitude"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   163
            Top             =   720
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "4. Reserve (GPS)"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   162
            Top             =   960
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "5. Longitude"
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   161
            Top             =   1200
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "6. Ground Speed"
            Height          =   255
            Index           =   5
            Left            =   120
            TabIndex        =   160
            Top             =   1440
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "7. Track Angle"
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   159
            Top             =   1680
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "8. Altitude"
            Height          =   255
            Index           =   7
            Left            =   120
            TabIndex        =   158
            Top             =   1920
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "9. GPS Status"
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   157
            Top             =   2160
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "10. Reserve (GPS)"
            Height          =   255
            Index           =   9
            Left            =   120
            TabIndex        =   156
            Top             =   2400
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "11. Reserve (GPS)"
            Height          =   255
            Index           =   10
            Left            =   120
            TabIndex        =   155
            Top             =   2640
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "12. Reserve (GPS)"
            Height          =   255
            Index           =   11
            Left            =   120
            TabIndex        =   154
            Top             =   2880
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "13. Reserve (GPS)"
            Height          =   255
            Index           =   12
            Left            =   120
            TabIndex        =   153
            Top             =   3120
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "14. Reserve (GPS)"
            Height          =   255
            Index           =   13
            Left            =   120
            TabIndex        =   152
            Top             =   3360
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "15. Reserve (GPS)"
            Height          =   255
            Index           =   14
            Left            =   120
            TabIndex        =   151
            Top             =   3600
            Width           =   1695
         End
         Begin VB.CheckBox chkValidCC1 
            Caption         =   "16. Reserve (GPS)"
            Height          =   255
            Index           =   15
            Left            =   120
            TabIndex        =   150
            Top             =   3840
            Width           =   1695
         End
      End
      Begin VB.Frame Frame8 
         Caption         =   "Additional Data"
         Height          =   2415
         Left            =   -72360
         TabIndex        =   136
         Top             =   360
         Width           =   2535
         Begin VB.TextBox txtDA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   142
            Text            =   "UCSEsim1.frx":0038
            Top             =   2040
            Width           =   855
         End
         Begin VB.TextBox txtAS 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   141
            Text            =   "UCSEsim1.frx":0040
            Top             =   1680
            Width           =   855
         End
         Begin VB.TextBox txtSA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   140
            Text            =   "UCSEsim1.frx":0048
            Top             =   1320
            Width           =   855
         End
         Begin VB.TextBox txtAA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   139
            Text            =   "UCSEsim1.frx":004E
            Top             =   960
            Width           =   855
         End
         Begin VB.TextBox txtTA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   138
            Text            =   "UCSEsim1.frx":0054
            Top             =   600
            Width           =   855
         End
         Begin VB.TextBox txtGS 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   137
            Text            =   "UCSEsim1.frx":005B
            Top             =   240
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Attack Angle"
            Height          =   255
            Index           =   38
            Left            =   120
            TabIndex        =   148
            Top             =   960
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Slip Angle"
            Height          =   255
            Index           =   33
            Left            =   120
            TabIndex        =   147
            Top             =   1320
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "TAS (km/h)"
            Height          =   255
            Index           =   30
            Left            =   120
            TabIndex        =   146
            Top             =   1680
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Drift Angle"
            Height          =   255
            Index           =   29
            Left            =   120
            TabIndex        =   145
            Top             =   2040
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Gnd Speed (km/h)"
            Height          =   255
            Index           =   24
            Left            =   120
            TabIndex        =   144
            Top             =   240
            Width           =   1335
         End
         Begin VB.Label Label 
            Caption         =   "Track Angle (deg)"
            Height          =   255
            Index           =   23
            Left            =   120
            TabIndex        =   143
            Top             =   600
            Width           =   1335
         End
      End
      Begin VB.CheckBox chkValid2B 
         Caption         =   "Valid B2"
         Height          =   255
         Left            =   9240
         TabIndex        =   125
         ToolTipText     =   "Set to set/clear bits in CC1 and CC2"
         Top             =   1200
         Width           =   975
      End
      Begin VB.Frame Frame11 
         Caption         =   "COM Port Setup"
         Height          =   2475
         Left            =   -72360
         TabIndex        =   124
         Top             =   2880
         Width           =   2535
         Begin VB.ComboBox cboStop 
            Height          =   315
            Left            =   1320
            TabIndex        =   184
            Text            =   "1"
            Top             =   1680
            Width           =   1095
         End
         Begin VB.TextBox txtMScomm 
            Height          =   285
            Left            =   1320
            TabIndex        =   134
            Text            =   "1,9600,N,8"
            Top             =   2100
            Width           =   1095
         End
         Begin VB.ComboBox cboBits 
            Height          =   315
            Left            =   1320
            TabIndex        =   129
            Text            =   "8"
            Top             =   1320
            Width           =   1095
         End
         Begin VB.ComboBox cboParity 
            Height          =   315
            Left            =   1320
            TabIndex        =   128
            Text            =   "N"
            Top             =   960
            Width           =   1095
         End
         Begin VB.ComboBox cboBaud 
            Height          =   315
            Left            =   1320
            TabIndex        =   127
            Text            =   "9600"
            Top             =   600
            Width           =   1095
         End
         Begin VB.ComboBox cboCOMport 
            Height          =   315
            Left            =   1320
            TabIndex        =   126
            Text            =   "1"
            Top             =   240
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Stop Bits"
            Height          =   255
            Index           =   47
            Left            =   150
            TabIndex        =   183
            Top             =   1710
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "MScomm$"
            Height          =   255
            Index           =   46
            Left            =   120
            TabIndex        =   135
            Top             =   2100
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Data Bits"
            Height          =   255
            Index           =   45
            Left            =   120
            TabIndex        =   133
            Top             =   1320
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Parity"
            Height          =   255
            Index           =   44
            Left            =   120
            TabIndex        =   132
            Top             =   960
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Baud Rate"
            Height          =   255
            Index           =   43
            Left            =   120
            TabIndex        =   131
            Top             =   600
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Port Number"
            Height          =   255
            Index           =   42
            Left            =   120
            TabIndex        =   130
            Top             =   240
            Width           =   1095
         End
      End
      Begin VB.Frame Frame10 
         Caption         =   "Input Data"
         Height          =   4575
         Left            =   -74880
         TabIndex        =   98
         Top             =   360
         Width           =   2415
         Begin VB.TextBox txtUTstart 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   110
            Text            =   "UCSEsim1.frx":0061
            Top             =   240
            Width           =   855
         End
         Begin VB.TextBox txtLat 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   109
            Text            =   "UCSEsim1.frx":0063
            Top             =   600
            Width           =   855
         End
         Begin VB.TextBox txtLon 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   108
            Text            =   "UCSEsim1.frx":006C
            Top             =   960
            Width           =   855
         End
         Begin VB.TextBox txtZg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   107
            Text            =   "UCSEsim1.frx":0075
            Top             =   1320
            Width           =   855
         End
         Begin VB.TextBox txtHeading2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   106
            Text            =   "UCSEsim1.frx":007D
            Top             =   3480
            Width           =   855
         End
         Begin VB.TextBox txtRoll2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   105
            Text            =   "UCSEsim1.frx":0082
            Top             =   3120
            Width           =   855
         End
         Begin VB.TextBox txtPitch2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   104
            Text            =   "UCSEsim1.frx":0087
            Top             =   2760
            Width           =   855
         End
         Begin VB.TextBox txtOATn 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   103
            Text            =   "UCSEsim1.frx":008C
            Top             =   2400
            Width           =   855
         End
         Begin VB.TextBox txtP 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   102
            Text            =   "UCSEsim1.frx":0091
            Top             =   2040
            Width           =   855
         End
         Begin VB.TextBox txtZp 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   101
            Text            =   "UCSEsim1.frx":0096
            Top             =   1680
            Width           =   855
         End
         Begin VB.TextBox txtWdir 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   100
            Text            =   "UCSEsim1.frx":009C
            Top             =   4200
            Width           =   855
         End
         Begin VB.TextBox txtWspd 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   99
            Text            =   "UCSEsim1.frx":00A1
            Top             =   3840
            Width           =   855
         End
         Begin MSComCtl2.UpDown udLat 
            Height          =   375
            Left            =   2160
            TabIndex        =   123
            Top             =   600
            Width           =   240
            _ExtentX        =   423
            _ExtentY        =   661
            _Version        =   393216
            Max             =   90
            Min             =   -90
            Enabled         =   -1  'True
         End
         Begin VB.Label Label 
            Caption         =   "Latitude (deg)"
            Height          =   255
            Index           =   25
            Left            =   120
            TabIndex        =   122
            Top             =   600
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Longitude (deg)"
            Height          =   255
            Index           =   26
            Left            =   120
            TabIndex        =   121
            Top             =   960
            Width           =   1335
         End
         Begin VB.Label Label 
            Caption         =   "Zgps (m)"
            Height          =   255
            Index           =   27
            Left            =   120
            TabIndex        =   120
            Top             =   1320
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "UT Start Time (s)"
            Height          =   255
            Index           =   28
            Left            =   120
            TabIndex        =   119
            Top             =   240
            Width           =   1335
         End
         Begin VB.Label Label 
            Caption         =   "Pressure (hPa)"
            Height          =   255
            Index           =   32
            Left            =   120
            TabIndex        =   118
            Top             =   2040
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "OAT (C)"
            Height          =   255
            Index           =   34
            Left            =   120
            TabIndex        =   117
            Top             =   2400
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Zp (m)"
            Height          =   255
            Index           =   35
            Left            =   120
            TabIndex        =   116
            Top             =   1680
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Wind Spd (m/s)"
            Height          =   255
            Index           =   39
            Left            =   120
            TabIndex        =   115
            Top             =   3840
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "Wind Dir (deg)"
            Height          =   255
            Index           =   40
            Left            =   120
            TabIndex        =   114
            Top             =   4200
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "Roll (deg)"
            Height          =   255
            Index           =   31
            Left            =   120
            TabIndex        =   113
            Top             =   3120
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Heading (deg)"
            Height          =   255
            Index           =   36
            Left            =   120
            TabIndex        =   112
            Top             =   3480
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Pitch (deg)"
            Height          =   255
            Index           =   37
            Left            =   120
            TabIndex        =   111
            Top             =   2760
            Width           =   855
         End
      End
      Begin VB.CheckBox chkRepeat 
         Caption         =   "Repeat Last Frame"
         Height          =   255
         Left            =   -74880
         TabIndex        =   97
         Top             =   5040
         Width           =   1815
      End
      Begin VB.TextBox txtUCSEfile 
         Height          =   285
         Left            =   -66840
         TabIndex        =   96
         Text            =   "C:\MTP\UCSE.txt"
         Top             =   5400
         Width           =   2175
      End
      Begin VB.CheckBox chkWriteToFile 
         Caption         =   "Write UCSE Data to "
         Height          =   255
         Left            =   -68760
         TabIndex        =   95
         Top             =   5400
         Width           =   1815
      End
      Begin VB.TextBox txtBadFrames 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5460
         MultiLine       =   -1  'True
         TabIndex        =   94
         Top             =   5400
         Width           =   495
      End
      Begin VB.CommandButton cmdOpenER2 
         Caption         =   "Select ER-2 RAW File"
         Height          =   375
         Left            =   -74880
         TabIndex        =   92
         Top             =   5400
         Width           =   1935
      End
      Begin VB.TextBox txtER2filename 
         Height          =   285
         Left            =   -72840
         TabIndex        =   91
         Text            =   "C:\ER2\SOLVE\20000120\ER20000120.RAW"
         Top             =   5400
         Width           =   3855
      End
      Begin VB.Frame Frame9 
         Caption         =   "Synthesize Mode"
         Height          =   615
         Left            =   -68760
         TabIndex        =   87
         Top             =   4680
         Width           =   4095
         Begin VB.OptionButton optSynthMode 
            Caption         =   "Run ER-2 Flight"
            Height          =   255
            Index           =   2
            Left            =   2400
            TabIndex        =   90
            Top             =   240
            Width           =   1455
         End
         Begin VB.OptionButton optSynthMode 
            Caption         =   "Simulate Flight"
            Height          =   255
            Index           =   1
            Left            =   960
            TabIndex        =   89
            Top             =   240
            Width           =   1335
         End
         Begin VB.OptionButton optSynthMode 
            Caption         =   "Static"
            Height          =   255
            Index           =   0
            Left            =   240
            TabIndex        =   88
            Top             =   240
            Width           =   855
         End
      End
      Begin VB.CheckBox chkSynthesize 
         Caption         =   "Synthesize"
         Height          =   255
         Left            =   6840
         TabIndex        =   86
         Top             =   1200
         Width           =   1095
      End
      Begin VB.CommandButton cmdStart 
         Caption         =   "&Start"
         Height          =   495
         Left            =   8520
         TabIndex        =   85
         Top             =   3240
         Width           =   855
      End
      Begin VB.CheckBox chkReverseBits 
         Caption         =   "Reverse Bits"
         Height          =   255
         Left            =   7920
         TabIndex        =   84
         Top             =   1200
         Width           =   1335
      End
      Begin VB.Frame Frame4 
         Caption         =   "Timer Speed (ms)"
         Height          =   615
         Left            =   6840
         TabIndex        =   81
         Top             =   480
         Width           =   3375
         Begin VB.TextBox txtTimerSpeed 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   82
            Top             =   240
            Width           =   735
         End
         Begin MSComctlLib.Slider Slider1 
            Height          =   255
            Left            =   840
            TabIndex        =   83
            Top             =   240
            Width           =   2415
            _ExtentX        =   4260
            _ExtentY        =   450
            _Version        =   393216
            Max             =   10000
            SelStart        =   500
            Value           =   500
         End
      End
      Begin VB.Frame Frame6 
         Caption         =   "Run Mode"
         Height          =   1575
         Left            =   6840
         TabIndex        =   76
         Top             =   1560
         Width           =   3375
         Begin VB.OptionButton optRunMode 
            Caption         =   "Read Continuously from File"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   80
            Top             =   240
            Width           =   3135
         End
         Begin VB.OptionButton optRunMode 
            Caption         =   "Read and Single Step from File"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   79
            Top             =   480
            Width           =   3015
         End
         Begin VB.OptionButton optRunMode 
            Caption         =   "Read Continuously from Serial Port"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   78
            Top             =   720
            Width           =   2895
         End
         Begin VB.OptionButton optRunMode 
            Caption         =   "Write Continuously to Serial Port"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   77
            Top             =   960
            Width           =   3135
         End
      End
      Begin VB.CommandButton Command1 
         Caption         =   "E&xit"
         Height          =   495
         Left            =   9360
         TabIndex        =   75
         Top             =   3240
         Width           =   855
      End
      Begin VB.CommandButton cmdRefresh 
         Caption         =   "&Refresh"
         Height          =   495
         Left            =   7680
         TabIndex        =   74
         Top             =   3240
         Width           =   855
      End
      Begin VB.Frame Frame1 
         Caption         =   "Packet 2Ah Parameters"
         Height          =   3495
         Left            =   120
         TabIndex        =   57
         Top             =   480
         Width           =   2535
         Begin VB.TextBox txtZgps 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   65
            Top             =   1320
            Width           =   975
         End
         Begin VB.TextBox txtLongitude 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   64
            Top             =   960
            Width           =   975
         End
         Begin VB.TextBox txtLatitude 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   63
            Top             =   600
            Width           =   975
         End
         Begin VB.TextBox txtUTks 
            Height          =   285
            Left            =   600
            TabIndex        =   62
            Text            =   "86.400"
            Top             =   240
            Width           =   615
         End
         Begin VB.TextBox txtHHMMSS 
            Height          =   285
            Left            =   1440
            TabIndex        =   61
            Text            =   "HH:MM:SS"
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox txtCC1 
            Height          =   285
            Left            =   840
            TabIndex        =   60
            Text            =   "0000000000000000"
            Top             =   3120
            Width           =   1575
         End
         Begin VB.TextBox txtGroundSpeed 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   59
            Top             =   1680
            Width           =   975
         End
         Begin VB.TextBox txtTrackAngle 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   58
            Top             =   2040
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "ks"
            Height          =   255
            Index           =   6
            Left            =   1200
            TabIndex        =   73
            Top             =   240
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "Zgps (m)"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   72
            Top             =   1320
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Longitude (deg)"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   71
            Top             =   960
            Width           =   1335
         End
         Begin VB.Label Label 
            Caption         =   "Latitude (deg)"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   70
            Top             =   600
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "GMT"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   69
            Top             =   240
            Width           =   495
         End
         Begin VB.Label Label1 
            Caption         =   "Status"
            Height          =   255
            Left            =   120
            TabIndex        =   68
            Top             =   3120
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "Gnd Speed (km/h)"
            Height          =   255
            Index           =   21
            Left            =   120
            TabIndex        =   67
            Top             =   1680
            Width           =   1335
         End
         Begin VB.Label Label 
            Caption         =   "Track Angle (deg)"
            Height          =   255
            Index           =   22
            Left            =   120
            TabIndex        =   66
            Top             =   2040
            Width           =   1335
         End
      End
      Begin VB.Frame Frame2 
         Caption         =   "Packet 2Bh Parameters"
         Height          =   3495
         Left            =   2760
         TabIndex        =   27
         Top             =   480
         Width           =   3975
         Begin VB.TextBox txtAttack 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   44
            Top             =   1320
            Width           =   615
         End
         Begin VB.TextBox txtPitch 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   43
            Top             =   960
            Width           =   615
         End
         Begin VB.TextBox txtHeading 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3120
            MultiLine       =   -1  'True
            TabIndex        =   42
            Top             =   1680
            Width           =   615
         End
         Begin VB.TextBox txtpALT 
            Alignment       =   1  'Right Justify
            BackColor       =   &H00FFFFFF&
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   41
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtOATc 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   40
            Top             =   240
            Width           =   615
         End
         Begin VB.TextBox txtSlip 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3120
            MultiLine       =   -1  'True
            TabIndex        =   39
            Top             =   1320
            Width           =   615
         End
         Begin VB.TextBox txtPressure 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3120
            MultiLine       =   -1  'True
            TabIndex        =   38
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtRoll 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3120
            MultiLine       =   -1  'True
            TabIndex        =   37
            Top             =   960
            Width           =   615
         End
         Begin VB.TextBox txtOATk 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3120
            MultiLine       =   -1  'True
            TabIndex        =   36
            Top             =   240
            Width           =   615
         End
         Begin VB.TextBox txtCC2 
            Height          =   285
            Left            =   720
            TabIndex        =   35
            Text            =   "0000000000000000"
            Top             =   3120
            Width           =   1575
         End
         Begin VB.TextBox txtTAS 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   34
            Top             =   1680
            Width           =   615
         End
         Begin VB.Frame Frame3 
            Caption         =   "Wind"
            Height          =   615
            Left            =   120
            TabIndex        =   29
            Top             =   2280
            Width           =   3735
            Begin VB.TextBox txtWindDir 
               Alignment       =   1  'Right Justify
               Height          =   285
               Left            =   1200
               MultiLine       =   -1  'True
               TabIndex        =   31
               Top             =   240
               Width           =   615
            End
            Begin VB.TextBox txtWindSpeed 
               Alignment       =   1  'Right Justify
               Height          =   285
               Left            =   3000
               MultiLine       =   -1  'True
               TabIndex        =   30
               Top             =   240
               Width           =   615
            End
            Begin VB.Label Label 
               Caption         =   "Direction (deg)"
               Height          =   255
               Index           =   15
               Left            =   120
               TabIndex        =   33
               Top             =   240
               Width           =   1215
            End
            Begin VB.Label Label 
               Caption         =   "Speed (m/s)"
               Height          =   255
               Index           =   10
               Left            =   1920
               TabIndex        =   32
               Top             =   240
               Width           =   975
            End
         End
         Begin VB.TextBox txtDriftAngle 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   28
            Top             =   2040
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "Attack Angle"
            Height          =   255
            Index           =   14
            Left            =   240
            TabIndex        =   56
            Top             =   1320
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Pitch (deg)"
            Height          =   255
            Index           =   13
            Left            =   240
            TabIndex        =   55
            Top             =   960
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Heading (deg)"
            Height          =   255
            Index           =   12
            Left            =   2040
            TabIndex        =   54
            Top             =   1680
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "pALT (m)"
            Height          =   255
            Index           =   11
            Left            =   240
            TabIndex        =   53
            Top             =   600
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "OAT (C)"
            Height          =   255
            Index           =   8
            Left            =   240
            TabIndex        =   52
            Top             =   240
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Slip Angle"
            Height          =   255
            Index           =   5
            Left            =   2040
            TabIndex        =   51
            Top             =   1320
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Pressure (hPa)"
            Height          =   255
            Index           =   9
            Left            =   2040
            TabIndex        =   50
            Top             =   600
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Roll (deg)"
            Height          =   255
            Index           =   16
            Left            =   2040
            TabIndex        =   49
            Top             =   960
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "OAT (K)"
            Height          =   255
            Index           =   17
            Left            =   2040
            TabIndex        =   48
            Top             =   240
            Width           =   855
         End
         Begin VB.Label Label2 
            Caption         =   "Status"
            Height          =   255
            Left            =   120
            TabIndex        =   47
            Top             =   3120
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "TAS (km/h)"
            Height          =   255
            Index           =   18
            Left            =   240
            TabIndex        =   46
            Top             =   1680
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Drift Angle"
            Height          =   255
            Index           =   19
            Left            =   240
            TabIndex        =   45
            Top             =   2040
            Width           =   975
         End
      End
      Begin VB.Timer Timer1 
         Left            =   240
         Top             =   3000
      End
      Begin VB.TextBox txtPacket2Ah 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00000000&
         Height          =   315
         Left            =   120
         TabIndex        =   20
         Top             =   4440
         Width           =   10095
      End
      Begin VB.TextBox txtPacket2Bh 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   120
         TabIndex        =   19
         Top             =   5040
         Width           =   7215
      End
      Begin VB.Frame Frame5 
         Caption         =   "Frame"
         Height          =   615
         Left            =   8400
         TabIndex        =   15
         Top             =   3720
         Width           =   1815
         Begin VB.TextBox txtFrame 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   17
            Top             =   240
            Width           =   615
         End
         Begin VB.TextBox txtTotalFrames 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   16
            Top             =   240
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "of"
            Height          =   255
            Index           =   4
            Left            =   840
            TabIndex        =   18
            Top             =   240
            Width           =   255
         End
      End
      Begin VB.TextBox txt2A 
         Height          =   285
         Left            =   5520
         TabIndex        =   13
         Top             =   4080
         Width           =   375
      End
      Begin VB.TextBox txt2B 
         Height          =   285
         Left            =   6360
         TabIndex        =   12
         Top             =   4080
         Width           =   375
      End
      Begin VB.TextBox txtSliderPosition 
         Height          =   285
         Left            =   4560
         TabIndex        =   11
         Top             =   4080
         Width           =   375
      End
      Begin VB.Frame Frame7 
         Caption         =   "Slider Move"
         Height          =   735
         Left            =   6840
         TabIndex        =   8
         Top             =   3720
         Width           =   1455
         Begin VB.CommandButton cmdSliderMove 
            Caption         =   "Fwd"
            Height          =   375
            Index           =   0
            Left            =   720
            TabIndex        =   10
            Top             =   240
            Width           =   615
         End
         Begin VB.CommandButton cmdSliderMove 
            Caption         =   "Back"
            Height          =   375
            Index           =   1
            Left            =   120
            TabIndex        =   9
            Top             =   240
            Width           =   615
         End
      End
      Begin VB.ComboBox cbo2Bh 
         Height          =   315
         Left            =   9000
         TabIndex        =   7
         Text            =   "1"
         Top             =   5040
         Width           =   495
      End
      Begin VB.TextBox txtFilename 
         Height          =   285
         Left            =   1200
         TabIndex        =   5
         Text            =   "C:\MTP\Data\M55\SCOUT-O3\UC20030126.BIN"
         Top             =   5400
         Width           =   3795
      End
      Begin VB.CommandButton cmdGoToFrame 
         Caption         =   "Go"
         Height          =   375
         Left            =   9840
         TabIndex        =   4
         Top             =   5400
         Width           =   495
      End
      Begin VB.CommandButton cmdOpenUCSE 
         Caption         =   "&Open File"
         Height          =   375
         Left            =   120
         TabIndex        =   3
         Top             =   5400
         Width           =   975
      End
      Begin VB.CheckBox chkASCII 
         Caption         =   "ASCII"
         Height          =   255
         Left            =   9600
         TabIndex        =   2
         Top             =   5040
         Width           =   735
      End
      Begin MSComctlLib.Slider Slider3 
         Height          =   255
         Left            =   6450
         TabIndex        =   6
         Top             =   5400
         Width           =   3405
         _ExtentX        =   6006
         _ExtentY        =   450
         _Version        =   393216
      End
      Begin MSComDlg.CommonDialog CommonDialog1 
         Left            =   1080
         Top             =   3000
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin MSComctlLib.Slider Slider2 
         Height          =   255
         Left            =   120
         TabIndex        =   14
         Top             =   4800
         Width           =   9975
         _ExtentX        =   17595
         _ExtentY        =   450
         _Version        =   393216
         LargeChange     =   0
         Max             =   47
         SelStart        =   1
         Value           =   1
      End
      Begin MSCommLib.MSComm MSComm1 
         Left            =   6960
         Top             =   3240
         _ExtentX        =   1005
         _ExtentY        =   1005
         _Version        =   393216
         DTREnable       =   -1  'True
      End
      Begin VB.Label Label 
         Caption         =   "Bad"
         Height          =   255
         Index           =   41
         Left            =   5040
         TabIndex        =   93
         Top             =   5430
         Width           =   375
      End
      Begin VB.Label Label3 
         Caption         =   "Packet 2Ah (top) and 2Bh (bottom)"
         Height          =   255
         Left            =   120
         TabIndex        =   26
         Top             =   4080
         Width           =   2535
      End
      Begin VB.Label Label4 
         Caption         =   "Values at Slider Position"
         Height          =   255
         Left            =   2760
         TabIndex        =   25
         Top             =   4080
         Width           =   1815
      End
      Begin VB.Label Label5 
         Caption         =   "2Bh"
         Height          =   255
         Left            =   6000
         TabIndex        =   24
         Top             =   4080
         Width           =   375
      End
      Begin VB.Label Label6 
         Caption         =   "2Ah"
         Height          =   255
         Left            =   5040
         TabIndex        =   23
         Top             =   4080
         Width           =   375
      End
      Begin VB.Label Label7 
         Caption         =   "2Bh Sample to View"
         Height          =   255
         Left            =   7440
         TabIndex        =   22
         Top             =   5160
         Width           =   1455
      End
      Begin VB.Label Label 
         Caption         =   "Frame"
         Height          =   255
         Index           =   20
         Left            =   5970
         TabIndex        =   21
         Top             =   5400
         Width           =   495
      End
   End
   Begin VB.Label Label 
      Height          =   15
      Index           =   7
      Left            =   480
      TabIndex        =   0
      Top             =   5280
      Width           =   135
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open File"
      End
      Begin VB.Menu mnuFIleClose 
         Caption         =   "&Close File"
      End
      Begin VB.Menu mnuFileWriteHex 
         Caption         =   "&Write UCSE File as Hex"
      End
      Begin VB.Menu mnuFileWriteText 
         Caption         =   "Write UCSE as &Text"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit Program"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "About"
      End
   End
End
Attribute VB_Name = "frmUCSEsim"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Function fHex2$(Char As String)
Dim hx$

hx$ = Hex$(Asc(Char))
If Len(hx$) = 1 Then hx$ = "0" + hx$
fHex2$ = hx$

End Function

Function fHexString$(Packet$)
Dim X$, i%

  X$ = ""
  For i = 1 To Len(Packet)
    X$ = X$ + fHex2(Mid$(Packet, i, 1))
  Next i
  fHexString = X$
  
End Function

Sub ReadNextFrameFile()
Dim i%, j%, X$
Static UCSElu%

  Call FindFF0010       'Get "FF00" string
' Find 2Ah Packet
  If EOF(INPlu) Then EOFflag = True: Exit Sub 'Skip CPK
' And read it from Serial Port or UCSE Data File
  X$ = ""
  Do
    Packet_2Ah = Packet_2Ah + Input(1, INPlu)
    If WriteHex Then Debug.Print fHex2$(Right$(Packet_2Ah, 1));
    If Len(Packet_2Ah) > 2 Then
      X$ = Right$(Packet_2Ah, 2) 'Hex$(Asc(Mid$(Packet_2Ah, Len(Packet_2Ah) - 1, 1)))
      X$ = fHex2(Left$(X$, 1)) + fHex2(Right$(X$, 1))
    End If
  Loop Until X$ = "1003"
  If WriteHex Then Debug.Print ""
  Packet_2Ah = Left$(Packet_2Ah, Len(Packet_2Ah) - 2)
  If WriteHex Then Print #OUTlu, fHexString$(Packet_2Ah)
  If Len(Packet_2Ah) > 47 Then
  '  MsgBox "Packet_2Ah has >47 bytes (" + Str(Len(Packet_2Ah)) + " bytes), excluding 2A", vbOKOnly
  End If
' Find 2Bh Packet
  EOFflag = False
  For i = 1 To 5
    FindCode ("&H2B")
    X$ = ""
    If EOF(INPlu) Then EOFflag = True: Exit Sub
'    Packet_2Bh(i) = ""
    Do
      Packet_2Bh(i) = Packet_2Bh(i) + Input(1, INPlu)
      If WriteHex Then Debug.Print fHex2(Right$(Packet_2Bh(i), 1));
      If Len(Packet_2Bh(i)) > 2 Then
        X$ = Right$(Packet_2Bh(i), 2) 'Hex$(Asc(Mid$(Packet_2Bh(i), Len(Packet_2Bh(i)) - 1, 1)))
        X$ = fHex2(Left$(X$, 1)) + fHex2(Right$(X$, 1))
      End If
    Loop Until X$ = "1003"
    If WriteHex Then Debug.Print ""
    Packet_2Bh(i) = Left$(Packet_2Bh(i), Len(Packet_2Bh(i)) - 2)  'Get rid of "1003"
    If WriteHex Then Print #OUTlu, fHexString$(Packet_2Bh(i))
    If Len(Packet_2Bh(i)) > 33 Then
'      MsgBox "Packet_2Ah has >32 bytes, excluding 2B", vbOKOnly
'      j = 0
'      Do
'        j = j + 1
'        If Asc(Mid$(Packet_2Bh(i), j, 1)) = 16 Then
'          If Asc(Mid$(Packet_2Bh(i), j + 1, 1)) = 16 Then
'            Mid$(Packet_2Bh(i), j + 1, 1) = ""
'          End If
'        End If
'      Loop Until j = Len(Packet_2Bh(i))
      If InStr(1, Packet_2Bh(i), Chr(16) + Chr(16)) > 0 Then
'        Debug.Print "Packet_2Bh has " + Str(Len(Packet_2Bh(i))) + " bytes! " + fHexString(Packet_2Bh(i))
        Do
          j = InStr(1, Packet_2Bh(i), Chr(16) + Chr(16))
          If j > 0 Then Packet_2Bh(i) = Left$(Packet_2Bh(i), j) + Right$(Packet_2Bh(i), Len(Packet_2Bh(i)) - j - 1)
        Loop Until j = 0
'        Debug.Print "Packet_2Bh has " + Str(Len(Packet_2Bh(i))) + " bytes! " + fHexString(Packet_2Bh(i))
        'If Len(Packet_2Bh(i)) < 33 Then Exit Sub
      Else
        If Hex$(Asc(Mid$(Packet_2Bh(i), Len(Packet_2Bh(i)) - 1, 1))) + Hex$(Asc(Right$(Packet_2Bh(i), 1))) = "DA" Then
        Else
'         Debug.Print "Packet_2Bh has " + Str(Len(Packet_2Bh(i))) + " bytes! " + fHexString(Packet_2Bh(i))
        End If
 '       Exit Sub
      End If
    End If
  Next i

' Frame Number
  Frame = Frame + 1
  txtFrame.Text = Frame
  If Frame < Slider3.Max Then Slider3.Value = Frame
Exit Sub

End Sub

Sub ReadNextFrameSerial()
Dim i%, j%, X$
Static UCSElu%
  
' Find 2Ah Packet
  Call FindFF0010      'Get "FF00" string

' And read it from Serial Port
  X$ = ""
  
  If MSComm1.InBufferCount > 49 Then
'    Packet_2Ah = ""
    Do
      Packet_2Ah = Packet_2Ah + MSComm1.Input
      Debug.Print " " + Hex$(Asc(Right$(Packet_2Ah, 1)));
      If Len(Packet_2Ah) > 2 Then
        X$ = Right$(Packet_2Ah, 2) 'Hex$(Asc(Mid$(Packet_2Ah, Len(Packet_2Ah) - 1, 1)))
        X$ = Hex$(Asc(Left$(X$, 1))) + Hex$(Asc(Right$(X$, 1)))
      End If
    Loop Until X$ = "103"
  Else
    MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
    Exit Sub
  End If
  Debug.Print ""
  Packet_2Ah = Left$(Packet_2Ah, Len(Packet_2Ah) - 2)
  If Len(Packet_2Ah) > 46 Then
    MsgBox "Packet_2Ah has >46 bytes (" + Str(Len(Packet_2Ah)) + " bytes), excluding 2A", vbOKOnly
  End If

' Find 2Bh Packet
  EOFflag = False
  For i = 1 To 5
    FindCode ("&H2B")
    X$ = ""
    If MSComm1.InBufferCount > 32 Then
'      Packet_2Bh(i) = ""
      Do
        Packet_2Bh(i) = Packet_2Bh(i) + MSComm1.Input
        Debug.Print " " + Hex$(Asc(Right$(Packet_2Bh(i), 1)));
        If Len(Packet_2Bh(i)) > 2 Then
          X$ = Right$(Packet_2Bh(i), 2) 'Hex$(Asc(Mid$(Packet_2Bh(i), Len(Packet_2Bh(i)) - 1, 1)))
          X$ = Hex$(Asc(Left$(X$, 1))) + Hex$(Asc(Right$(X$, 1)))
        End If
      Loop Until X$ = "103"
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      Exit Sub
    End If
    Debug.Print ""
    Packet_2Bh(i) = Left$(Packet_2Bh(i), Len(Packet_2Bh(i)) - 2)
    If Len(Packet_2Bh(i)) > 32 Then
'      MsgBox "Packet_2Ah has >32 bytes, excluding 2B", vbOKOnly
      j = 0
      Do
        j = j + 1
        If Asc(Mid$(Packet_2Bh(i), j, 1)) = 16 Then
          If Asc(Mid$(Packet_2Bh(i), j + 1, 1)) = 16 Then
            Mid$(Packet_2Bh(i), j + 1, 1) = ""
          End If
        End If
      Loop Until j = Len(Packet_2Bh(i))
    End If
  Next i

' Frame Number
  Frame = Frame + 1
  txtFrame.Text = Frame
  If Frame < Slider3.Max Then Slider3.Value = Frame

End Sub

Sub scratch()

MSComm1.Settings = "9600,n,8,1"
If chkEnableRepeatTest.Value = 1 Then repeattest
Mid$(Packet_2Ah, 45, 2) = Chr$(&H0) + Chr$(&H0)
Mid$(Packet_2Ah, 45, 2) = Chr$(&HFF) + Chr$(&HFF)
For i = 1 To 5: Mid$(Packet_2Bh(i), 31, 2) = Chr$(&H0) + Chr$(&H0): Next i
For i = 1 To 5: Mid$(Packet_2Bh(i), 31, 2) = Chr$(&HFF) + Chr$(&HFF): Next i

End Sub
Function hx$(X$)
   Dim hxt$
    For hxi = 1 To Len(X$)
        hxt$ = hxt$ + Right$("00" + Hex$(Asc(Mid$(X$, hxi, 1))), 2)
    Next hxi
    hx$ = hxt$
End Function
Sub repeattest()
rt0:
out$ = FF + FF + Zero + Zero + DLE + A + Packet_2Ah + DLE + ETX
  out$ = out$ + DLE + B + Packet_2Bh(1) + DLE + ETX
  out$ = out$ + DLE + B + Packet_2Bh(2) + DLE + ETX
  out$ = out$ + DLE + B + Packet_2Bh(3) + DLE + ETX
  out$ = out$ + DLE + B + Packet_2Bh(4) + DLE + ETX
  out$ = out$ + DLE + B + Packet_2Bh(5) + DLE + ETX
  MSComm1.Output = out$
  'waitfor 1
GoTo rt0
End Sub

Sub Encode_2Ah(Packet_2Ah As String)
Dim char1 As String, char2 As String, A$, X!
Dim Lat$, Lon$, Deg$, Min$, HHMMSS$, HH_MM_SS$, Z$

' Read CPK(2),GMT(9),LAT(10),AA(2),LON(11),GS(5),TA(3),Zgps(2),CC1(2)
'      1-2    3-11   12-21   22-23 24-34   35-39 40-42 43-44   45-46
' CPK
  A$ = Chr(0) + Chr(13)
' Encode GMT
  HHMMSS$ = fSecToTstring(UTsec, False)
  A$ = A$ + HHMMSS$ + ".00"

' Decode Latitude
  If Latitude > 0 Then A$ = A$ + "N" Else A$ = A$ + "S"
  X = Abs(Latitude)
  A$ = A$ + Format(Int(X), "00")
  A$ = A$ + Format(60 * (X - Int(X)), "00.0000")
    
' Reserved word AAh
  A$ = A$ + "00"

' Encode Longitude
  If Longitude > 0 Then A$ = A$ + "E" Else A$ = A$ + "W"
  X = Abs(Longitude)
  A$ = A$ + Format(Int(X), "000")
  A$ = A$ + Format(60 * (X - Int(X)), "00.0000")

' Encode Ground Speed W (5 bytes)
  A$ = A$ + Format(Val(txtGS.Text), "000.0")

' Encode Track Angle (3 bytes)
  A$ = A$ + Format(Val(txtTA.Text), "000")

' Encode GPS Altitude
  Z$ = fPutN(Int(pALT * 1000), 32768, RBflag)
  A$ = A$ + Z$

' Read Status Byte CC1
  Packet_2Ah = A$ + Chr(CC1high) + Chr(CC1low)
End Sub

Sub Encode_2Bh(Packet_2Bh As String)
Dim i%, B$
' All  2Bh parameters are 2 bytes long and scaled
' T,  P,  TAS, pALT, DA,  Roll, TH,   B,    Pitch,AA,   SA,   B,    Wsd,  Wdir, B,    CC2
' 1-2,3-4,5-6, 7-8,  9-10,11-12,13-14,15-16,17-18,19-20,21-22,23-24,25-26,27-28,29-30,31-32

' 1 T, 2 P, 3 TAS, 4 pALT, 5 DA, 6 Roll, 7 TH, 8 080-1
' 9 Pitch, 10 AA, 11 SA, 12 ADC, 13 Wind, 14 LSS, 15 DS, 16 080-1
' Outside air temperature (C)
  
  B$ = fPutN(OATn, 1024#, RBflag)
' Static Pressure (hPa)
  B$ = B$ + fPutN(fZtoP(Val(txtZp.Text) / 1000#), 1092.16, RBflag)
' TAS (km/h)
  B$ = B$ + fPutN(Val(txtAS.Text), 4730, RBflag)
' Pressure Altitude (meters)
  B$ = B$ + fPutN(Val(txtZp.Text), 32768, RBflag)
' Skip Drift Angle
  B$ = B$ + fPutN(Val(txtDA.Text), 180, RBflag)
' True Roll(deg)
  B$ = B$ + fPutN(Roll, 180, RBflag)
' True Heading (deg)
  B$ = B$ + fPutN(Heading, 180, RBflag)
' B Reserved
  B$ = B$ + Chr(0) + Chr(0)
' True Pitch (deg)
  B$ = B$ + fPutN(Pitch, 180, RBflag)
' Attach Angle (deg)
  B$ = B$ + fPutN(Val(txtAA.Text), 180, RBflag)
' Slip Angle (deg)
  B$ = B$ + fPutN(Val(txtSA.Text), 180, RBflag)
' B Reserved
  B$ = B$ + Chr(0) + Chr(0)
' Wind Speed (deg)
  B$ = B$ + fPutN(Wspd, 128, RBflag)
' Wind Direction (deg)
  B$ = B$ + fPutN(Wdir, 180, RBflag)
' B Reserved
  B$ = B$ + Chr(0) + Chr(0)
' Status Word 2 (deg)
  Packet_2Bh = B$ + Chr(CC2high) + Chr(CC2low)
End Sub


Function fGetChar()
  If RunMode = 2 Then
    If MSComm1.InBufferCount > 0 Then
      fGetChar = Asc(MSComm1.Input)
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      NoComPortData = True
      Exit Function
    End If
  Else
    fGetChar = Asc(Input(1, INPlu)) ' Get one character.
  End If

End Function

Sub FindFF0010()
Dim Char$, CharCode%, LastCode%, hx$
' Look for character string "FF00"
  EOFflag = False
'  GoSub GetChar                      'Get a character
  
repeat:
  Do                                 'Look for first "F"
'    LastCode = CharCode
    GoSub GetChar                    'Get next character
    If RunMode <> 2 Then             'If reading from file, EXIT on EOF
      If EOF(INPlu) Then Exit Do
    End If
  Loop Until CharCode = 255          'LastCode = "&H10" And
  
  GoSub GetChar
  If CharCode = 255 Then             'Found 2 consecutive "F"s
    GoSub GetChar                    'Now look for two "0"s
    If CharCode = 0 Then             'Found first "0"
      GoSub GetChar
      If CharCode = 0 Then           'Found second "0"
        GoSub GetChar                'Look for "10"
        If CharCode = 16 Then
'          GoSub GetChar
'          If CharCode = 42 Then      'Look for "2A"
            Exit Sub
'          Else
'            GoTo repeat
'          End If
        Else
          GoTo repeat
        End If
      End If
    Else
      GoTo repeat
    End If
  Else
    GoTo repeat                      'Didn't find two consecutive "F"s
  End If

Exit Sub

GetChar:
  If RunMode = 2 Then                'Read data from serial port
    If MSComm1.InBufferCount > 0 Then
      CharCode = Asc(MSComm1.Input)
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      NoComPortData = True
      Exit Sub
    End If
  Else                               'Read data from file
    CharCode = Asc(Input(1, INPlu))  'Get one character.
  End If
  hx$ = Hex$(CharCode)
  If Len(hx$) = 1 Then hx$ = "0" + hx$
  If WriteHex Then Debug.Print hx$;: Print #OUTlu, hx$;
  Return

End Sub

Sub SynthesizeData()
' Read ER2 data file and synthesize UCSE data
Dim GoToNext As Boolean, Line$, out$, i%

  If chkRepeat.Value = 0 Then
    Do
      Line Input #INPlu, Line$
    Loop Until Left$(Line$, 1) = "A"

    Call DecodeAline(Line$, GoToNext)
    txtUTstart.Text = Str(UTsec)
    txtLat.Text = Format(Latitude, "##0.0000")
    txtLon.Text = Format(Longitude, "###0.0000")
    txtZg.Text = Format(gALT * 1000, "####0")
    txtZp.Text = Format(pALT * 1000, "####0")
    txtP.Text = Format(fZtoP(pALT), "###0.00")
    txtOATn.Text = Format(OATn, "##0.00")
    txtPitch2.Text = Format(Pitch, "##0.0")
    txtRoll2.Text = Format(Roll, "##0.0")
    txtHeading2.Text = Format(Heading, "##0.0")
    txtWspd.Text = Format(Wspd, "##0.0")
    txtWdir.Text = Format(Wdir, "##0.0")
  
    CC1low = 0
    CC2low = 0
    For i = 0 To 7
      If chkValidCC1(i).Value = 1 Then CC1low = CC1low + 2 ^ i
      If chkValidCC2(i).Value = 1 Then CC2low = CC2low + 2 ^ i
    Next i
    
    CC1high = 0
    CC2high = 0
    For i = 0 To 7
      If chkValidCC1(i + 8).Value = 1 Then CC1high = CC1high + 2 ^ i
      If chkValidCC2(i + 8).Value = 1 Then CC2high = CC2high + 2 ^ i
    Next i
    
    DoEvents
  
    Call Encode_2Ah(Packet_2Ah)
    Call Decode_2Ah(Packet_2Ah)
  End If
  
  out$ = FF + FF + Zero + Zero + DLE + A + Packet_2Ah + DLE + ETX
  For i = 1 To 5
    Call Encode_2Bh(Packet_2Bh(i))
    Call Decode_2Bh(Packet_2Bh(i), i)
    out$ = out$ + DLE + B + Packet_2Bh(i) + DLE + ETX
  Next i
  MSComm1.Output = out$
  DoEvents
  cmdRefresh_Click
  
End Sub


Sub OutputSerialData(EOFflag)
Dim out$, i%
  
  If chkRepeat.Value = 0 Then ReadNextFrame
  cmdRefresh_Click
  
  out$ = FF + FF + Zero + Zero + DLE + A + Packet_2Ah + DLE + ETX
  For i = 1 To 5
    out$ = out$ + DLE + B + Packet_2Bh(i) + DLE + ETX
  Next i
  MSComm1.Output = out$
  DoEvents

End Sub

Sub ReadNextFrame()
Dim i%, j%, X$
Static UCSElu%

  Call FindFF0010        'Get "FF00" string
' Find 2Ah Packet
'  Call FindCode("&H2A")
  If RunMode <> 2 Then     'Make sure no EOF on file input
    If EOF(INPlu) Then EOFflag = True: Exit Sub 'Skip CPK
  End If
' And read it from Serial Port or UCSE Data File
  X$ = ""
  If RunMode = 2 Then              ' Read 2Ah Packet from Serial Port
    If MSComm1.InBufferCount > 46 Then
      Packet_2Ah = ""
      Do
'      For j = 1 To 46
        Packet_2Ah = Packet_2Ah + MSComm1.Input
        Debug.Print " " + Hex$(Asc(Right$(Packet_2Ah, 1)));
'      Next j
        If Len(Packet_2Ah) > 2 Then
          X$ = Right$(Packet_2Ah, 2) 'Hex$(Asc(Mid$(Packet_2Ah, Len(Packet_2Ah) - 1, 1)))
          X$ = Hex$(Asc(Left$(X$, 1))) + Hex$(Asc(Right$(X$, 1)))
        End If
      Loop Until X$ = "103"
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      Exit Sub
    End If
  Else                             ' Read 2Ah Packet from file
    Packet_2Ah = Input(46, INPlu)
  End If
  Debug.Print ""
  Packet_2Ah = Left$(Packet_2Ah, Len(Packet_2Ah) - 2)
  If Len(Packet_2Ah) > 46 Then
    MsgBox "Packet_2Ah has >46 bytes (" + Str(Len(Packet_2Ah)) + " bytes), excluding 2A", vbOKOnly
  End If
' Find 2Bh Packet
  EOFflag = False
  For i = 1 To 5
    FindCode ("&H2B")
    X$ = ""
    If RunMode <> 2 Then If EOF(INPlu) Then EOFflag = True: Exit Sub
    If RunMode = 2 Then
      If MSComm1.InBufferCount > 32 Then
        Packet_2Bh(i) = ""
        Do
'        For j = 1 To 32
          Packet_2Bh(i) = Packet_2Bh(i) + MSComm1.Input
          Debug.Print " " + Hex$(Asc(Right$(Packet_2Bh(i), 1)));
          If Len(Packet_2Bh(i)) > 2 Then
            X$ = Right$(Packet_2Bh(i), 2) 'Hex$(Asc(Mid$(Packet_2Bh(i), Len(Packet_2Bh(i)) - 1, 1)))
            X$ = Hex$(Asc(Left$(X$, 1))) + Hex$(Asc(Right$(X$, 1)))
          End If
        
'        Next j
        Loop Until X$ = "103"
      Else
        MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
        Exit Sub
      End If
    Else
      Packet_2Bh(i) = Input(32, INPlu)  ' Read 2Ah Packet
    End If
    Debug.Print ""
    Packet_2Bh(i) = Left$(Packet_2Bh(i), Len(Packet_2Bh(i)) - 2)
    If Len(Packet_2Bh(i)) > 32 Then
'      MsgBox "Packet_2Ah has >32 bytes, excluding 2B", vbOKOnly
      j = 0
      Do
        j = j + 1
        If Asc(Mid$(Packet_2Bh(i), j, 1)) = 16 Then
          If Asc(Mid$(Packet_2Bh(i), j + 1, 1)) = 16 Then
            Mid$(Packet_2Bh(i), j + 1, 1) = ""
          End If
        End If
      Loop Until j = Len(Packet_2Bh(i))
    End If
  Next i

' Frame Number
  Frame = Frame + 1
  txtFrame.Text = Frame
  If Frame < Slider3.Max Then Slider3.Value = Frame

End Sub

Function Test()
  Filename$ = txtFilename.Text
  INPlu = FreeFile
  Open Filename$ For Binary As INPlu
DoAgain:
  i = 0
  Do    ' Loop until end of file.
    i = i + 1
    LastCode = CharCode
    Char$ = Input(1, INPlu)
    CharCode = Asc(Char$)
    Debug.Print i; Char$; "="; Hex(CharCode)
  Loop Until EOF(INPlu) Or (Hex(CharCode) = "2A") 'LastCode = "&H10" And
GoTo DoAgain
Test = 1
End Function

Function BitString(n&)
Dim B$, i%

  For i = 15 To 0 Step -1
    If 2 ^ i And n Then B$ = B$ + "1" Else B$ = B$ + "0"
  Next i
  BitString = B$

End Function

Function fGetN(Packet$, Index%, RBflag As Boolean)
Dim High%, Low%, Word%, NewWord%
  
  High = Asc(Mid$(Packet$, Index, 1))  ' Get High Byte
  Low = Asc(Mid$(Packet$, Index + 1, 1)) ' Get Low Byte
' Check to see if bits should be reversed
  If RBflag Then: High = ReverseBits(High): Low = ReverseBits(Low)
  If (High And 128) Then
    fGetN = (High - 128) * 256 + Low - 32767 'Take 2's complement
  Else
    fGetN = High * 256 + Low
  End If
End Function

Function fPutN(Parameter!, Pscale!, RBflag As Boolean) As String
Dim X!, absX$, High!, Low!

X = Parameter * 32768# / Pscale
  absX = Abs(X)
  High = Int(absX / 256)
  Low = absX - High * 256

If X < 0 Then
  X = 32767 - (High) * 256 - Low 'Take 2's complement
  High = Int(X / 256) + 128
  Low = X - Int(X / 256) * 256
End If
  If RBflag Then ReverseBits (High): ReverseBits (Low)


fPutN = Chr(High) + Chr(Low)

End Function
Sub FindCode(Code%)
Dim Char$, CharCode%, LastCode%, hx$
  
  EOFflag = False
'  GoSub GetChar
  Do    ' Loop until end of file.
'    LastCode = CharCode
    GoSub GetChar
    If RunMode <> 2 Then 'Look for EOF when reading UCSE file
      If EOF(INPlu) Then Exit Do
    End If
  Loop Until (CharCode = Code)  'LastCode = "&H10" And
Exit Sub

GetChar:
  If RunMode = 2 Then                'Read data from serial port
    If MSComm1.InBufferCount > 0 Then
      CharCode = Asc(MSComm1.Input)
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      NoComPortData = True
      Exit Sub
    End If
  Else                               'Read data from file
    CharCode = Asc(Input(1, INPlu))  'Get one character.
  End If
    hx$ = Hex$(CharCode)
    If Len(hx$) = 1 Then hx$ = "0" + hx$
    If WriteHex Then Debug.Print hx$;: Print #OUTlu, hx$;
  Return


End Sub

Sub FindFF00()
Dim Char$, CharCode%, LastCode%, hx$
' Look for character string "FF00"
  EOFflag = False
'  GoSub GetChar                      'Get a character
'  Do
'    GoSub GetChar
'    hx$ = Hex$(CharCode)
'    If Len(hx$) = 1 Then hx$ = "0" + hx$
'    Debug.Print " " + hx$;
'  Loop
repeat:
  Do                                 'Look for first "F"
'    LastCode = CharCode
    GoSub GetChar                    'Get next character
    If RunMode <> 2 Then             'If reading from file, EXIT on EOF
      If EOF(INPlu) Then Exit Do
    End If
  Loop Until CharCode = 255          'LastCode = "&H10" And
  
  GoSub GetChar
  If CharCode = 255 Then             'Found 2 consecutive "F"s
    GoSub GetChar                    'Now look for two "0"s
    If CharCode = 0 Then             'Found first "0"
      GoSub GetChar
      If CharCode = 0 Then Exit Sub  'Found second "0"
    Else
      GoTo repeat
    End If
  Else
    GoTo repeat                      'Didn't find two consecutive "F"s
  End If

Exit Sub

GetChar:
  If RunMode = 2 Then                'Read data from serial port
    If MSComm1.InBufferCount > 0 Then
      CharCode = Asc(MSComm1.Input)
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      NoComPortData = True
      Exit Sub
    End If
  Else                               'Read data from file
    CharCode = Asc(Input(1, INPlu))  'Get one character.
  End If
    hx$ = Hex$(CharCode)
    If Len(hx$) = 1 Then hx$ = "0" + hx$
    Debug.Print " " + hx$;
  Return
  
End Sub


Function ReverseBits(Char%)
Dim OutChar%
For i = 7 To 0 Step -1
  If 2 ^ i And Char Then
    OutChar = OutChar + 2 ^ (7 - i)
  End If
Next i
ReverseBits = OutChar

End Function
Sub Decode_2Ah(Packet_2Ah As String)
Dim char1 As String, char2 As String, High!, Low!
Dim Lat$, Lon$, Deg$, Min$, HHMMSS$, HH_MM_SS$

' Read CpK(2),GMT(9),LAT(10),AA(2),LON(11),GS(5),TA(3),Zgps(2),CC1(2)
'      1-2    3-11   12-21   22-23 24-34   35-39 40-42 43-44   45-46
' Including "2A"
'      2-3    4-12   13-22   23-24 25-35   36-40 41-43 44-45   46-47
' Read Status Byte CC1
  High = Asc(Mid$(Packet_2Ah, 46, 1))  ' Get High Byte
  Low = Asc(Mid$(Packet_2Ah, 47, 1)) ' Get Low Byte
  CC1 = 256 * High + Low
  txtCC1.Text = BitString(CC1)

' Decode GMT
  HHMMSS$ = Mid$(Packet_2Ah, 4, 9)
  HH_MM_SS$ = Mid$(Packet_2Ah, 4, 2) & ":" & Mid$(Packet_2Ah, 6, 2) & ":" & Mid$(Packet_2Ah, 8, 5)
  txtHHMMSS = HH_MM_SS$
  UTks = 3.6 * Val(Mid$(Packet_2Ah, 4, 2)) + 0.06 * Val(Mid$(Packet_2Ah, 6, 2)) + Val(Mid$(Packet_2Ah, 8, 5)) / 1000#
  txtUTks = Str(UTks)
  If CC1 And 2 Then txtHHMMSS.BackColor = White Else txtHHMMSS.BackColor = Gray
  If CC1 And 2 Then txtUTks.BackColor = White Else txtUTks.BackColor = Gray
' Decode Latitude
  Deg$ = Mid$(Packet_2Ah, 14, 2)
  Min$ = Mid$(Packet_2Ah, 16, 7)
  Latitude = Val(Deg$) + Val(Min$) / 60#
  If Mid$(Packet_2Ah, 13, 1) = "S" Then Latitude = -Latitude
  txtLatitude = Format(Latitude, "#00.000")
  If CC1 And 4 Then txtLatitude.BackColor = White Else txtLatitude.BackColor = Gray
  
' Skip reserved word AAh
   
' Read Longitude
  Deg$ = Mid$(Packet_2Ah, 26, 3)
  Min$ = Mid$(Packet_2Ah, 29, 7)
  Longitude = Val(Deg$) + Val(Min$) / 60#
  If Mid$(Packet_2Ah, 25, 1) = "W" Then Longitude = -Longitude
  txtLongitude = Format(Longitude, "#000.0000")
  If CC1 And 32 Then txtLongitude.BackColor = White Else txtLongitude.BackColor = Gray

' Read Ground Speed W (5 bytes)
  GroundSpeed = Val(Mid$(Packet_2Ah, 36, 5))
  txtGroundSpeed.Text = Str(GroundSpeed)
  If CC1 And 64 Then txtGroundSpeed.BackColor = White Else txtGroundSpeed.BackColor = Gray

' Read Track Angle (3 bytes)
  TrackAngle = Val(Mid$(Packet_2Ah, 41, 3))
  txtTrackAngle.Text = Str(TrackAngle)
  If CC1 And 128 Then txtTrackAngle.BackColor = White Else txtTrackAngle.BackColor = Gray

' Read GPS Altitude
  Zgps = fGetN(Packet_2Ah, 44, RBflag)
  txtZgps = Zgps
  If CC1 And 256 Then txtZgps.BackColor = White Else txtZgps.BackColor = Gray
End Sub


Sub Decode_2Bh(Packet_2Bh As String, n%)
Dim i%, High!, Low!
' All  2Bh parameters are 2 bytes long and scaled
' T,  P,  TAS, pALT, DA,  Roll, TH,   B,    Pitch,AA,   SA,   B,    Wsd,  Wdir, B,    CC2
' 1-2,3-4,5-6, 7-8,  9-10,11-12,13-14,15-16,17-18,19-20,21-22,23-24,25-26,27-28,29-30,31-32

' Status Word 2 (deg)
  Select Case Len(Packet_2Bh)
  
  Case Is < 33
'    Debug.Print "Packet_2Bh has " + Str(Len(Packet_2Bh)) + " bytes! " + fHexString(Packet_2Bh)
    Exit Sub
  Case 33
  Case Is > 33
    If InStr(1, Packet_2Bh, Chr(16) + Chr(16)) > 0 Then
'      Debug.Print "Packet_2Bh has " + Str(Len(Packet_2Bh)) + " bytes! " + fHexString(Packet_2Bh)
      Do
        i = InStr(1, Packet_2Bh, Chr(16) + Chr(16))
        If i > 0 Then Packet_2Bh = Left$(Packet_2Bh, i) + Right$(Packet_2Bh, Len(Packet_2Bh) - i - 1)
      Loop Until i = 0
'      Debug.Print "Packet_2Bh has " + Str(Len(Packet_2Bh)) + " bytes! " + fHexString(Packet_2Bh)
      If Len(Packet_2Bh) < 33 Then Exit Sub
    Else
      If Hex$(Asc(Mid$(Packet_2Bh, Len(Packet_2Bh) - 1, 1))) + Hex$(Asc(Right$(Packet_2Bh, 1))) = "DA" Then
      Else
'        Debug.Print "Packet_2Bh has " + Str(Len(Packet_2Bh)) + " bytes! " + fHexString(Packet_2Bh)
      End If
      Exit Sub
    End If
  End Select
  
  High = Asc(Mid$(Packet_2Bh, 32, 1))  ' Get High Byte
  Low = Asc(Mid$(Packet_2Bh, 33, 1)) ' Get Low Byte
  CC2(n) = 256 * High + Low
  txtCC2.Text = BitString(CC2(n))
' 1 T, 2 P, 3 TAS, 4 pALT, 5 DA, 6 Roll, 7 TH, 8 080-1
' 9 Pitch, 10 AA, 11 SA, 12 ADC, 13 Wind, 14 LSS, 15 DS, 16 080-1
' Outside air temperature (C)
  OAT = fGetN(Packet_2Bh, 2, RBflag) * 1024# / 32768#
  txtOATc.Text = Format(OAT, "##0.00")
  txtOATk.Text = Format(OAT + 273.15, "000.00")
  If CC2(n) And 1 Then txtOATc.BackColor = White Else txtOATc.BackColor = Gray
  If CC2(n) And 1 Then txtOATk.BackColor = White Else txtOATk.BackColor = Gray
' Static Pressure (hPa)
  P = fGetN(Packet_2Bh, 4, RBflag)
  txtPressure.Text = Format(P, "###0.00") * 1092.16 / 32768#
  If CC2(n) And 2 Then txtPressure.BackColor = White Else txtPressure.BackColor = Gray
' Decode TAS (km/h)
  TAS = fGetN(Packet_2Bh, 6, RBflag) * 4730# / 32768#
  txtTAS.Text = Format(TAS, "##0.0")
  If CC2(n) And 4 Then txtTAS.BackColor = White Else txtTAS.BackColor = Gray
' Pressure Altitude (meters)
  pALT = fGetN(Packet_2Bh, 8, RBflag)
  txtpALT.Text = Format(pALT, "####0")
  If CC2(n) And 8 Then txtpALT.BackColor = White Else txtpALT.BackColor = Gray
' Skip Drift Angle
  DriftAngle = fGetN(Packet_2Bh, 10, RBflag) * 180# / 32768#
  txtDriftAngle.Text = Format(DriftAngle, "##0.0")
  If CC2(n) And 16 Then txtDriftAngle.BackColor = White Else txtDriftAngle.BackColor = Gray
' True Roll(deg)
  Roll = fGetN(Packet_2Bh, 12, RBflag) * 180# / 32768#
  txtRoll.Text = Format(Roll, "##0.0")
  If CC2(n) And 32 Then txtRoll.BackColor = White Else txtRoll.BackColor = Gray
' True Heading (deg)
  Heading = fGetN(Packet_2Bh, 14, RBflag) * 180# / 32768#
  txtHeading.Text = Format(Heading, "####0.0")
  If CC2(n) And 64 Then txtHeading.BackColor = White Else txtHeading.BackColor = Gray
' True Pitch (deg)
  Pitch = fGetN(Packet_2Bh, 18, RBflag) * 180# / 32768#
  txtPitch.Text = Format(Pitch, "##0.0")
  If CC2(n) And 256 Then txtPitch.BackColor = White Else txtPitch.BackColor = Gray
' Attach Angle (deg)
  AttackAngle = fGetN(Packet_2Bh, 20, RBflag) * 180# / 32768#
  txtAttack.Text = Format(AttackAngle, "##0.0")
  If CC2(n) And 512 Then txtAttack.BackColor = White Else txtAttack.BackColor = Gray
' Slip Angle (deg)
  SlipAngle = fGetN(Packet_2Bh, 22, RBflag) * 180# / 32768#
  txtSlip.Text = Format(SlipAngle, "##0.0")
  If CC2(n) And 1024 Then txtSlip.BackColor = White Else txtSlip.BackColor = Gray
' Wind Speed (deg)
  WindSpeed = fGetN(Packet_2Bh, 26, RBflag) * 128# / 32768#
  txtWindSpeed.Text = Format(WindSpeed, "##0.0")
  If CC2(n) And 4096 Then txtWindSpeed.BackColor = White Else txtWindSpeed.BackColor = Gray
' Wind Direction (deg)
  WindDir = fGetN(Packet_2Bh, 28, RBflag) * 180# / 32768#
  txtWindDir.Text = Format(WindDir, "###0.0")
  If CC2(n) And 4096 Then txtWindDir.BackColor = White Else txtWindDir.BackColor = Gray

End Sub

Sub Main(EOFflag As Boolean)
Dim Code As Byte, Char As String, n%, Bit$, L As Long
Dim i%, X$
Static lu%, Frame&
' Read data from file or serial port

' Clear Packets
  Packet_2Ah = ""
  For i = 1 To 5: Packet_2Bh(i) = Chr(43): Next i

  If RunMode = 2 Then
    Call ReadNextFrameSerial
  Else
    Call ReadNextFrameFile
  End If
  
  If RunMode = 2 And NoComPortData Then   'RunMode=2 is Read Serial Port
    EOFflag = True
    Exit Sub
  End If
  cmdRefresh_Click
  
End Sub


Sub Test2B()
Dim count&, hx$, UCSE$, CharCode As Byte, CharCode2 As Byte, hx2$, lu%, X$
Dim k%, m1%, m2%, m3%, m4%, m5%

Do
  ReadNextFrameFile
Loop Until EOF(INPlu)








count = 0
'   lu = FreeFile
'   Open "C:\M55\NOcrlf.bin" For Output As lu
repeat:
  Do
    CharCode = Asc(Input(1, INPlu))  'Get one character.
    hx$ = Hex$(CharCode)
    If Len(hx$) = 1 Then hx$ = "0" + hx$
    hx2$ = hx2$ + hx$
    j = InStr(1, hx2$, "FFFF000010")
  Loop Until j > 0
  i = j + 1
  
  Do
  '  hx2$ = "00"
    CharCode = Asc(Input(1, INPlu))  'Get one character.
    hx$ = Hex$(CharCode)
    If Len(hx$) = 1 Then hx$ = "0" + hx$
    hx2$ = hx2$ + hx$
    j = InStr(i, hx2$, "FFFF0000102A")
    If j > i Then
      X$ = Mid$(hx2$, i, j + 6)
      m1 = InStr(1, X$, "1003102B")
      Debug.Print Mid$(X$, 1, m1 + 5)
           
      m2 = InStr(m1 + 1, X$, "1003102B")
      Debug.Print Mid$(X$, m1, m2 - m1 + 1)
      m3 = InStr(m1 + 1, X$, "1003102B")
      Debug.Print Mid$(X$, m2, m3 - m2 + 1)
      m4 = InStr(m1 + 1, X$, "1003102B")
      Debug.Print Mid$(X$, m3, m4 - m3 + 1)
      m5 = InStr(m1 + 1, X$, "1003102B")
      Debug.Print Mid$(X$, m4, m5 - m4 + 1)
    
    End If
'    Debug.Print " " + hx$;
'    If hx$ = "0D" Then
'      CharCode2 = Asc(Input(1, INPlu))  'Get one character.
'      hx2$ = Hex$(CharCode2)
'      If hx2$ <> "A" Then
'        Print #lu, Chr(Val("&H" + hx$)); Chr(Val("&H" + hx2$));
'      End If
'    Else
'      Print #lu, Chr(Val("&H" + hx$));
'    End If
    
    count = count + 1
  Loop Until EOF(INPlu)
  Close All
  ' FF FF 00 00 10
  i = 1
  Do
    j = InStr(i, hx2$, "FFFF000010")
    i = j
    Debug.Print Mid$(hx2$, j, 50)
  Loop
  Exit Sub
  
  UCSE$ = "2A"
  For i = 2 To 277
    CharCode = Asc(Input(1, INPlu))  'Get one character.
    count = count + 1
    hx$ = Hex$(CharCode)
    If Len(hx$) = 1 Then hx$ = "0" + hx$
    UCSE$ = UCSE$ + hx$
    'Debug.Print i; count; UCSE$
  Next i
  Debug.Print count; InStr(UCSE$, "2B"); UCSE$
  GoTo repeat
End Sub

Sub UpdateMScomm()

MScomm$ = ""

Select Case cboBaud.ListIndex
Case 0: MScomm$ = MScomm$ + "300,"
Case 1: MScomm$ = MScomm$ + "4800,"
Case 2: MScomm$ = MScomm$ + "9600,"
Case 3: MScomm$ = MScomm$ + "18200,"
End Select

Select Case cboParity.ListIndex
Case 0: MScomm$ = MScomm$ + "N,"
Case 1: MScomm$ = MScomm$ + "O,"
Case 2: MScomm$ = MScomm$ + "E,"
Case 3: MScomm$ = MScomm$ + "M,"
Case 4: MScomm$ = MScomm$ + "S,"
End Select

Select Case cboBits.ListIndex
Case 0: MScomm$ = MScomm$ + "7,"
Case 1: MScomm$ = MScomm$ + "8,"
End Select

Select Case cboStop.ListIndex
Case 0: MScomm$ = MScomm$ + "1"
Case 1: MScomm$ = MScomm$ + "2"
End Select

COMport = cboCOMport.ListIndex + 1

txtMScomm.Text = MScomm$

End Sub

Private Sub cbo2Bh_Click()
If FileOpen Then cmdRefresh_Click
End Sub

Private Sub cboBaud_Click()
UpdateMScomm
End Sub


Private Sub cboBits_Click()
UpdateMScomm
End Sub


Private Sub cboCOMport_Click()
UpdateMScomm
End Sub


Private Sub cboParity_Click()
UpdateMScomm
End Sub


Private Sub chkReverseBits_Click()
If chkReverseBits.Value = 1 Then
  RBflag = True
Else
  RBflag = False
End If
End Sub

Private Sub chkValidCC2_Click(Index As Integer)
If chkValidCC2(0).Value = 1 Or chkValidCC2(3).Value = 1 Or chkValidCC2(8).Value = 1 Then
  chkValidCC2(0).Value = 1
  chkValidCC2(3).Value = 1
  For i = 8 To 15
    chkValidCC2(i).Value = 1
  Next i
Else
  chkValidCC2(0).Value = 0
  chkValidCC2(3).Value = 0
  For i = 8 To 15
    chkValidCC2(i).Value = 0
  Next i
End If

End Sub


Private Sub cmdGoToFrame_Click()
  
  If Slider3.Value < Frame Then 'Close and Reopen
    Close INPlu
    Filename$ = txtFilename.Text
    INPlu = FreeFile
    Open Filename$ For Binary As INPlu
    Frame = 0
  End If
  
  Do
    FindCode ("&H2A")
    Frame = Frame + 1
    txtFrame.Text = Frame
  Loop Until Frame = Slider3.Value - 1
  Timer1.Enabled = True

End Sub

Private Sub cmdOpenER2_Click()
  Filename$ = txtER2filename.Text
  INPlu = FreeFile
  Open Filename$ For Input As INPlu
  FileOpen = True
  FileLength = LOF(INPlu)
  TotalFrames = FileLength / 412
  txtTotalFrames.Text = Str(Int(TotalFrames))
  Slider3.Min = 1
  Slider3.Max = TotalFrames + 2
  Slider3.Value = 1
  Frame = 0
  chkSynthesize.Value = 1
  optRunMode(3).Value = True
  cmdStart_Click
End Sub

Private Sub cmdOpenUCSE_Click()
Dim hx$, UCSE$

  Filename$ = txtFilename.Text
  INPlu = FreeFile
  Open Filename$ For Binary As INPlu
  FileOpen = True
  FileLength = LOF(INPlu)
  TotalFrames = FileLength / 614   'was 412 based on packet size
  txtTotalFrames.Text = Str(Int(TotalFrames))
  Slider3.Min = 1
  Slider3.Max = TotalFrames + 2
  Slider3.Value = 1
  Frame = 0
  chkSynthesize.Value = 0
  Exit Sub
  
  Do
    If RunMode = 2 Then                'Read data from serial port
      If MSComm1.InBufferCount > 0 Then
        CharCode = Asc(MSComm1.Input)
      Else
        MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
        NoComPortData = True
        Exit Sub
      End If
    Else                               'Read data from file
      CharCode = Asc(Input(1, INPlu))  'Get one character.
    End If
    hx$ = Hex$(CharCode)
    Debug.Print hx$;
  Loop Until hx$ = "2A"
  UCSE$ = "2A"
  For i = 2 To 277
    CharCode = Asc(Input(1, INPlu))  'Get one character.
    hx$ = Hex$(CharCode)
    If Len(hx$) = 1 Then hx$ = "0" + hx$
    UCSE$ = UCSE$ + hx$
    Debug.Print i; UCSE$
  Next i

End Sub


Private Sub cmdRefresh_Click()
Dim Code As Byte, Char As String, n%, Bit$, L As Long
Dim i%, j%, X$
Static UCSElu%

'    If UCSElu = 0 Then
'      UCSElu = FreeFile
'      Open txtUCSEfile.Text For Output As UCSElu
'    End If

' Find 2Ah Packet
  If chkASCII.Value = 0 Then
    Char = ""
    For i = 1 To Len(Packet_2Ah)  'was 47
      X$ = Hex(Asc(Mid$(Packet_2Ah, i, 1)))
      If Len(X$) = 1 Then X$ = "0" + X$
      Char = Char + X$
    Next i
  Else
    Char = ""
    For i = 1 To 47
      If Asc(Mid$(Packet_2Ah, i, 1)) > 20 Then X$ = Mid$(Packet_2Ah, i, 1) Else X$ = "."
      Char = Char + X$ + " "
    Next i
  End If
  txtPacket2Ah.Text = Char
  Call Decode_2Ah(Packet_2Ah)
  DoEvents
  
' Find first valid 2Bh Packet
  If chkValid2B.Value = 1 Then
    For j = 1 To 5: Call Decode_2Bh(Packet_2Bh(j), j): Next j 'decode to CC2(j)

    j = 0
    Do
      j = j + 1
'      Debug.Print Frame; j; CC2(j) And 4457
    Loop Until (((CC2(j) And 288) = 288) And Len(Packet_2Bh(j)) = 33) Or j = 5   '    '4457) = 4457) Or j = 5
    Debug.Print UTks; Frame; j
    If ((CC2(j) And 288) = 288) Then
      cbo2Bh.Text = Str(j)
    Else
      Exit Sub             'cbo2Bh.Text = Str(1)
    End If
  Else
    j = Val(cbo2Bh.Text)
  End If
  
  If chkASCII.Value = 0 Then
    Char = ""
    For i = 1 To 33
      X$ = Hex(Asc(Mid$(Packet_2Bh(j), i, 1)))
      If Len(X$) = 1 Then X$ = "0" + X$
      Char = Char + X$
    Next i
  Else
    Char = ""
    For i = 1 To 33
      If Asc(Mid$(Packet_2Bh(j), i, 1)) > 20 Then X$ = Mid$(Packet_2Bh(j), i, 1) Else X$ = "."
      Char = Char + X$ + " "
    Next i
  End If
  txtPacket2Bh.Text = Char
  Call Decode_2Bh(Packet_2Bh(j), j)
  'Debug.Print j; CC1; CC2(1); CC2(2); CC2(3); CC2(4); CC2(5)
' Bad Frame Counter
' CC1 want bits 2,3,5,8 true (1-8 bits)
' CC2 want bits 1,2,4,6,7,9 true

  If Not (((CC1 And 150) = 150) And (((CC2(1) And 363) < 363) Or ((CC2(2) And 363) < 363) Or ((CC2(3) And 363) < 363) Or ((CC2(4) And 363) < 363) Or ((CC2(5) And 363) < 363))) Then
     BadFrameCounter = BadFrameCounter + 1
     txtBadFrames = Str(BadFrameCounter)
     'Debug.Print Frame, CC1 And 150
'     If (CC1 And 150) = 150 Then X = 1 Else X = 0
'     Print #UCSElu, Frame, vbTab, BadFrameCounter, vbTab, X,
'     For i = 1 To 5
'       If (CC2(i) And 363) = 363 Then X = 1 Else X = 0
'       If i = 5 Then Print #UCSElu, vbTab, X Else Print #UCSElu, vbTab, X,
'     Next i
  Else
  ' Only write good frames
  If chkWriteToFile.Value = 1 Then
    If UCSElu = 0 Then
      UCSElu = FreeFile
      Open txtUCSEfile.Text For Output As UCSElu
      X$ = "Frame" + vbTab + "UTks" + vbTab + "pALT" + vbTab + "Zgps" + vbTab
      X$ = X$ + "OAT" + vbTab + "Latitude" + vbTab + "Longitude" + vbTab + "Pitch" + vbTab + "Roll" + vbTab
      X$ = X$ + "Wspd" + vbTab + "Wdir"
      Print #UCSElu, X$
    End If
'    If Frame Mod 10 = 0 Then
      If UTks <> 0 Then
        Print #UCSElu, Frame, vbTab, UTks, vbTab, pALT / 1000, vbTab, Zgps / 1000, vbTab,
        Print #UCSElu, OAT + 273.15, vbTab, Latitude, vbTab, Longitude, vbTab, Pitch, vbTab,
        Print #UCSElu, Roll, vbTab, WindSpeed, vbTab, WindDir
      Else
      'Debug.Print Frame, CC1 And 150
      End If
'    End If
  End If
  End If
End Sub

Private Sub cmdSliderMove_Click(Index As Integer)
Select Case Index
Case 0
  Slider2.Value = Slider2.Value + 1
Case 1
  Slider2.Value = Slider2.Value - 1
End Select
End Sub

Private Sub cmdStart_Click()

If Not FileOpen And RunMode <> 2 Then
  MsgBox "You must open a file before starting!", vbOKOnly
  Exit Sub
End If

UpdateMScomm

Select Case RunMode
Case 0  'Continuous from file
  Timer1.Enabled = True
  
Case 1  'Single Step from file
  cmdStart.Caption = "&Next"
  Timer1.Enabled = True

Case 2  'Read Continuously from Serial Port
' Use COM1.
  MSComm1.CommPort = COMport
' 9600 baud, no parity, 8 data, and 1 stop bit.
  MSComm1.Settings = MScomm$   '"9600,N,8," + Format(COMport, "0")
' Tell the control to read entire buffer when Input is used.
'  If ReadLine Then
'    MSComm1.InputLen = 0       'Read entire input buffer
'  Else
    MSComm1.InputLen = 1       'Read single character
'  End If
' Open the port.
  MSComm1.PortOpen = True
  Timer1.Enabled = True

Case 3  'Write to Serial Port
' Use COM1.
  MSComm1.CommPort = COMport
' 9600 baud, no parity, 8 data, and 1 stop bit.
  MSComm1.Settings = MScomm$   '"9600,N,8," + Format(COMport, "0")
' Open port
  MSComm1.PortOpen = True
  Frame = 0
  Timer1.Enabled = True
  
End Select

End Sub


Private Sub Command1_Click()
Close All
End
End Sub




Private Sub Form_Load()
RBflag = False 'Don't reverse information word bits by default
chkReverseBits.Value = 0
Timer1.Interval = 1000
txtTimerSpeed.Text = Timer1.Interval
Slider1.Value = Timer1.Interval
Timer1.Enabled = False
WriteHex = False

' Set default option controls
optRunMode(1).Value = True
optSynthMode(2).Value = True
RunMode = 1

cbo2Bh.AddItem "1"
cbo2Bh.AddItem "2"
cbo2Bh.AddItem "3"
cbo2Bh.AddItem "4"
cbo2Bh.AddItem "5"
cbo2Bh.ListIndex = 0
StartUP = True

With cboCOMport
.AddItem "1"
.AddItem "2"
.AddItem "3"
.AddItem "4"
.ListIndex = 3
End With

With cboBaud
.AddItem "300"
.AddItem "4800"
.AddItem "9600"
.AddItem "18200"
.ListIndex = 2
End With

With cboParity
.AddItem "None"
.AddItem "Odd"
.AddItem "Even"
.AddItem "Mark"
.AddItem "Space"
.ListIndex = 1
End With

With cboBits
.AddItem "7"
.AddItem "8"
.ListIndex = 1
End With

With cboStop
.AddItem "1"
.AddItem "2"
.ListIndex = 0
End With

White = &HFFFFFF
Gray = &HC0C0C0

DLE = Chr(&H10)
ETX = Chr(&H3)
FF = Chr(&HFF)
Zero = Chr(&H0)
A = Chr(&H2A)
B = Chr(&H2B)
NoComPortData = False
cft_km = 3280.8
Ceiling = 25

For i = 0 To 15
  chkValidCC1(i).Value = 1
  chkValidCC2(i).Value = 1
Next i

UpdateMScomm

End Sub



Private Sub mnuFIleClose_Click()
Close All

End Sub

Private Sub mnuFileExit_Click()
Close All
End
End Sub

Private Sub mnuFileOpen_Click()

If SSTab1.Tab = 0 Then
' Set Initial directory
  CommonDialog1.InitDir = "C:\ER2\Euplex\"
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen
  Filename$ = CommonDialog1.Filename
  If Len(Filename$) = 0 Then Exit Sub
  txtFilename.Text = Filename$
Else
' Set Initial directory
  CommonDialog1.InitDir = "G:\ER2\"
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "RAW Files (*.RAW)|*.RAW"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen
  Filename$ = CommonDialog1.Filename
  If Len(Filename$) = 0 Then Exit Sub
  txtER2filename.Text = Filename$
End If
  
  INPlu = FreeFile
  Open Filename$ For Binary As INPlu
  FileOpen = True
  FileLength = LOF(INPlu)
  TotalFrames = FileLength / 412
  txtTotalFrames.Text = Str(Int(TotalFrames))
  Slider3.Min = 1
  Slider3.Max = TotalFrames + 2
  Slider3.Value = 1
  Frame = 0
End Sub


Private Sub mnuFileWriteHex_Click()
Dim OUTfilename$

  Filename$ = txtFilename.Text
  INPlu = FreeFile
  Open Filename$ For Binary As INPlu
  
  OUTfilename$ = Left$(Filename$, InStr(Filename$, ".")) + "HEX"
  OUTlu = FreeFile
  Open OUTfilename$ For Output As #OUTlu
  
  FileOpen = True
  FileLength = LOF(INPlu)
  TotalFrames = FileLength / 234   'was 412 based on packet size
  txtTotalFrames.Text = Str(Int(TotalFrames))
  Slider3.Min = 1
  Slider3.Max = TotalFrames + 2
  Slider3.Value = 1
  Frame = 0
  chkSynthesize.Value = 0
  WriteHex = True
  
  Do
    Packet_2Ah = ""      'Chr(42)
    For i = 1 To 5: Packet_2Bh(i) = Chr(43): Next i
    ReadNextFrameFile
    DoEvents
  Loop Until EOF(INPlu)
  Close All
  
End Sub

Private Sub mnuFileWriteText_Click()
Dim OUTfilename$, i%, j%

  Filename$ = txtFilename.Text
  INPlu = FreeFile
  Open Filename$ For Binary As INPlu
  
'  OUTfilename$ = Left$(Filename$, InStr(Filename$, ".")) + "TXT"
'  OUTlu = FreeFile
'  Open OUTfilename$ For Output As #OUTlu
  
  FileOpen = True
  FileLength = LOF(INPlu)
  TotalFrames = FileLength / 234   'was 412 based on packet size
  txtTotalFrames.Text = Str(Int(TotalFrames))
  Slider3.Min = 1
  Slider3.Max = TotalFrames + 2
  Slider3.Value = 1
  Frame = 0
  chkSynthesize.Value = 0
'  WriteHex = True
  
  Do
    Packet_2Ah = ""      'Chr(42)
    For i = 1 To 5: Packet_2Bh(i) = Chr(43): Next i
    ReadNextFrameFile
' cmdRefresh_Click CODE BELOW
' Find 2Ah Packet
    txtPacket2Ah.Text = fHexString(Packet_2Ah)
    Call Decode_2Ah(Packet_2Ah)
    DoEvents
  
' Find first valid 2Bh Packet
    For j = 1 To 5: Call Decode_2Bh(Packet_2Bh(j), j): Next j 'decode to CC2(j)
    
    j = 0
    Do: j = j + 1
    Loop Until (((CC2(j) And 288) = 288) And Len(Packet_2Bh(j)) = 33) Or j = 5   '    '4457) = 4457) Or j = 5
'    Debug.Print UTks; Frame; j
    If ((CC2(j) And 288) = 288) Then
      cbo2Bh.Text = Str(j)
    Else
      Exit Sub             'cbo2Bh.Text = Str(1)
    End If
  
  txtPacket2Bh.Text = fHexString(Packet_2Bh(j))
  Call Decode_2Bh(Packet_2Bh(j), j)
' Bad Frame Counter
' CC1 want bits 2,3,5,8 true (1-8 bits)
' CC2 want bits 1,2,4,6,7,9 true

  If Not (((CC1 And 150) = 150) And (((CC2(1) And 288) = 288) Or ((CC2(2) And 288) = 288) Or ((CC2(3) And 288) = 288) Or ((CC2(4) And 288) = 288) Or ((CC2(5) And 288) = 288))) Then
     BadFrameCounter = BadFrameCounter + 1
     txtBadFrames = Str(BadFrameCounter)
     'Debug.Print Frame, CC1 And 150
'     If (CC1 And 150) = 150 Then X = 1 Else X = 0
'     Print #UCSElu, Frame, vbTab, BadFrameCounter, vbTab, X,
'     For i = 1 To 5
'       If (CC2(i) And 363) = 363 Then X = 1 Else X = 0
'       If i = 5 Then Print #UCSElu, vbTab, X Else Print #UCSElu, vbTab, X,
'     Next i
  Else
  ' Only write good frames
    If UCSElu = 0 Then
      UCSElu = FreeFile
      Open txtUCSEfile.Text For Output As UCSElu
      X$ = "Frame" + vbTab + "UTks" + vbTab + "Roll"   '"pALT" + vbTab + "Zgps" + vbTab
      'X$ = X$ + "OAT" + vbTab + "Latitude" + vbTab + "Longitude" + vbTab + "Pitch" + vbTab + "Roll" + vbTab
      'X$ = X$ + "Wspd" + vbTab + "Wdir"
      Print #UCSElu, X$
    End If
    If UTks <> 0 Then
      Print #UCSElu, Frame, vbTab, UTks, vbTab, Roll      'pALT / 1000, vbTab, Zgps / 1000, vbTab,
'        Print #UCSElu, OAT + 273.15, vbTab, Latitude, vbTab, Longitude, vbTab, Pitch, vbTab,
'        Print #UCSElu, Roll, vbTab, WindSpeed, vbTab, WindDir
      Debug.Print Frame; UTks; Roll
    End If
    End If
    DoEvents
  Loop Until EOF(INPlu)
  Close All
End Sub


Private Sub mnuHelpAbout_Click()
  Load frmAbout
  'Set defaults
  With frmAbout
    .lblApplication.Caption = "UCSE Simulator"
    .lblHeading.Caption = "MJ Mahoney"
    .lblVersion.Caption = "Version: 1.0 Beta"
    .lblCopyright.Caption = "Copyright © 2002"
    .Show
  End With

End Sub

Private Sub optBaud_Click(Index As Integer)
COMbaud = Index
End Sub

Private Sub optBits_Click(Index As Integer)
COMbits = parity
End Sub

Private Sub optRunMode_Click(Index As Integer)
RunMode = Index
End Sub

Private Sub optSynthMode_Click(Index As Integer)
SynthMode = Index
End Sub

Private Sub Slider1_Change()
Timer1.Interval = Slider1.Value
txtTimerSpeed.Text = Slider1.Value
End Sub

Private Sub Slider2_Change()
Dim X$
If Slider2.Value = 0 Then Slider2.Value = 1
If Slider2.Value = 47 Then Slider2.Value = 46
txtSliderPosition.Text = Slider2.Value
X$ = Hex(Asc(Mid$(Packet_2Ah, Slider2.Value, 1)))
If Len(X$) = 1 Then txt2A.Text = "0" + X$ Else txt2A.Text = X$
If Slider2.Value < 33 Then
  X$ = Hex(Asc(Mid$(Packet_2Bh(Val(cbo2Bh.Text)), Slider2.Value, 1)))
Else
  X$ = ""
End If
If Len(X$) = 1 Then txt2B.Text = "0" + X$ Else txt2B.Text = X$

End Sub

Private Sub Timer1_Timer()
Dim EOFflag As Boolean
'Run Mode
' 0   Read continuously from UCSE file
' 1   Read single record from UCSE file
' 2   Read continously  from Serial Port
' 3   Write continuously to Serial Port

  Timer1.Enabled = False
  If RunMode = 3 Then               'Write continuously to Serial Port
    If chkSynthesize.Value = 1 Then 'Use synthesized ER2 data
      Call SynthesizeData
    Else                            'Use data from UCSE file
      Call OutputSerialData(EOFflag)
    End If
  Else                              'Read data from from file continuously(0), from file next (1), from serial port (2)
    If Not EOFflag Then
      Call Main(EOFflag)
    End If
  End If
  
  If EOFflag = False And cmdStart.Caption <> "&Next" Then
    Timer1.Enabled = True           'Get next frame
  Else
    If EOFflag = False Then
      Timer1.Enabled = False        'Use next button to get next frame
      Slider2_Change
      Slider3.Value = Frame
    Else
      Close All
      MSComm1.PortOpen = False
      Exit Sub
    End If
  End If

End Sub


Private Sub txtTimerSpeed_Change()
Timer1.Interval = Val(txtTimerSpeed.Text)
End Sub



