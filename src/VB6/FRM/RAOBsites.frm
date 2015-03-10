VERSION 5.00
Begin VB.Form frmRAOBsites 
   Caption         =   "RAOB Sites"
   ClientHeight    =   2475
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5070
   Icon            =   "RAOBsites.frx":0000
   LinkTopic       =   "Form2"
   ScaleHeight     =   2475
   ScaleWidth      =   5070
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdDetails 
      Caption         =   "Gen BIN"
      Height          =   540
      Index           =   11
      Left            =   45
      TabIndex        =   30
      ToolTipText     =   "Generate MTP_UA.BIN file from intl2000.wmo"
      Top             =   1905
      Width           =   975
   End
   Begin VB.Frame Frame1 
      Caption         =   "Find"
      Height          =   855
      Left            =   0
      TabIndex        =   17
      Top             =   0
      Width           =   5055
      Begin VB.CommandButton cmdDetails 
         Caption         =   "First"
         Height          =   255
         Index           =   9
         Left            =   4320
         TabIndex        =   29
         ToolTipText     =   "Save BIN file as C:\MTP\RAOB\MTP_UA.txt"
         Top             =   480
         Width           =   615
      End
      Begin VB.CommandButton cmdDetails 
         Caption         =   "Last"
         Height          =   255
         Index           =   8
         Left            =   4320
         TabIndex        =   28
         ToolTipText     =   "Save BIN file as C:\MTP\RAOB\MTP_UA.txt"
         Top             =   240
         Width           =   615
      End
      Begin VB.CommandButton cmdDetails 
         Caption         =   "N+1"
         Height          =   255
         Index           =   6
         Left            =   3600
         TabIndex        =   27
         ToolTipText     =   "Save BIN file as C:\MTP\RAOB\MTP_UA.txt"
         Top             =   240
         Width           =   615
      End
      Begin VB.CommandButton cmdDetails 
         Caption         =   "N-1"
         Height          =   255
         Index           =   7
         Left            =   3600
         TabIndex        =   26
         ToolTipText     =   "Save BIN file as C:\MTP\RAOB\MTP_UA.txt"
         Top             =   480
         Width           =   615
      End
      Begin VB.TextBox txtShow 
         Height          =   285
         Index           =   2
         Left            =   1800
         TabIndex        =   25
         Text            =   "72393"
         Top             =   480
         Width           =   855
      End
      Begin VB.OptionButton optShow 
         Caption         =   "Record"
         Height          =   195
         Index           =   2
         Left            =   1800
         TabIndex        =   23
         Top             =   240
         Width           =   855
      End
      Begin VB.OptionButton optShow 
         Caption         =   "WMO"
         Height          =   195
         Index           =   1
         Left            =   960
         TabIndex        =   22
         Top             =   240
         Width           =   735
      End
      Begin VB.OptionButton optShow 
         Caption         =   "ICAO"
         Height          =   195
         Index           =   0
         Left            =   120
         TabIndex        =   21
         Top             =   240
         Width           =   735
      End
      Begin VB.CommandButton cmdDetails 
         Caption         =   "Go"
         Height          =   255
         Index           =   0
         Left            =   2880
         TabIndex        =   20
         ToolTipText     =   "Show ICAO or WMO site entered"
         Top             =   480
         Width           =   615
      End
      Begin VB.TextBox txtShow 
         Height          =   285
         Index           =   0
         Left            =   120
         TabIndex        =   19
         Text            =   "VBG"
         Top             =   480
         Width           =   735
      End
      Begin VB.TextBox txtShow 
         Height          =   285
         Index           =   1
         Left            =   960
         TabIndex        =   18
         Text            =   "72393"
         Top             =   480
         Width           =   735
      End
      Begin VB.Label lblRecords 
         Caption         =   "of 1000"
         Height          =   255
         Left            =   2880
         TabIndex        =   24
         Top             =   240
         Width           =   615
      End
   End
   Begin VB.CommandButton cmdDetails 
      Cancel          =   -1  'True
      Caption         =   "Close"
      Height          =   525
      Index           =   10
      Left            =   4080
      TabIndex        =   16
      ToolTipText     =   "Save BIN file as C:\MTP\RAOB\MTP_UA.txt"
      Top             =   1920
      Width           =   975
   End
   Begin VB.CommandButton cmdDetails 
      Caption         =   "TXT to BIN"
      Height          =   540
      Index           =   1
      Left            =   2100
      TabIndex        =   15
      ToolTipText     =   "Save MTP_UA.TXT file as C:\MTP\RAOB\BIN\MTP_UA.txt"
      Top             =   1905
      Width           =   990
   End
   Begin VB.TextBox txtLocation 
      Alignment       =   1  'Right Justify
      Height          =   285
      Index           =   6
      Left            =   4440
      MultiLine       =   -1  'True
      TabIndex        =   7
      Top             =   1560
      Width           =   615
   End
   Begin VB.TextBox txtLocation 
      Alignment       =   1  'Right Justify
      Height          =   285
      Index           =   5
      Left            =   2640
      MultiLine       =   -1  'True
      TabIndex        =   6
      Top             =   1560
      Width           =   735
   End
   Begin VB.TextBox txtLocation 
      Alignment       =   1  'Right Justify
      Height          =   285
      Index           =   4
      Left            =   960
      MultiLine       =   -1  'True
      TabIndex        =   5
      Top             =   1560
      Width           =   615
   End
   Begin VB.TextBox txtLocation 
      Height          =   285
      Index           =   0
      Left            =   4440
      TabIndex        =   4
      Top             =   1200
      Width           =   615
   End
   Begin VB.TextBox txtLocation 
      Height          =   285
      Index           =   1
      Left            =   3960
      TabIndex        =   3
      Top             =   1200
      Width           =   375
   End
   Begin VB.TextBox txtLocation 
      Height          =   285
      Index           =   3
      Left            =   120
      TabIndex        =   2
      Text            =   "12345678901234567890123456789012"
      Top             =   1200
      Width           =   3255
   End
   Begin VB.TextBox txtLocation 
      Height          =   285
      Index           =   2
      Left            =   3480
      TabIndex        =   1
      Top             =   1200
      Width           =   375
   End
   Begin VB.CommandButton cmdDetails 
      Caption         =   "BIN to TXT"
      Height          =   540
      Index           =   4
      Left            =   1065
      TabIndex        =   0
      ToolTipText     =   "Save MTP_UA.BIN file as C:\MTP\RAOB\BIN\MTP_UA.txt"
      Top             =   1905
      Width           =   990
   End
   Begin VB.Label Label 
      Caption         =   "Location"
      Height          =   255
      Index           =   20
      Left            =   120
      TabIndex        =   14
      Top             =   960
      Width           =   855
   End
   Begin VB.Label Label12 
      Caption         =   "GP Ht [m]"
      Height          =   255
      Left            =   3480
      TabIndex        =   13
      Top             =   1560
      Width           =   855
   End
   Begin VB.Label Label11 
      Caption         =   "Longitude"
      Height          =   255
      Left            =   1680
      TabIndex        =   12
      Top             =   1560
      Width           =   735
   End
   Begin VB.Label Label10 
      Caption         =   "Latitude"
      Height          =   255
      Left            =   120
      TabIndex        =   11
      Top             =   1560
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "Regiom"
      Height          =   255
      Index           =   11
      Left            =   4440
      TabIndex        =   10
      Top             =   960
      Width           =   495
   End
   Begin VB.Label Label 
      Caption         =   "Cntry"
      Height          =   255
      Index           =   13
      Left            =   3960
      TabIndex        =   9
      Top             =   960
      Width           =   375
   End
   Begin VB.Label Label 
      Caption         =   "State"
      Height          =   255
      Index           =   14
      Left            =   3480
      TabIndex        =   8
      Top             =   960
      Width           =   375
   End
End
Attribute VB_Name = "frmRAOBsites"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Public Sub ShowDetails()
  cmdDetails_Click (0)
End Sub

Sub UpdateRAOBsiteInfo()
  txtLocation(0).Text = GPregion
  txtLocation(1).Text = GPcountry
  txtLocation(2).Text = GPstate
  txtLocation(3).Text = GPstation
  txtLocation(4).Text = Format(GPlatitude, "##0.00")
  txtLocation(5).Text = Format(GPlongitude, "###0.00")
  txtLocation(6).Text = GPaltitude
  txtShow(1).Text = WMOnumber
  txtShow(0).Text = ICAOsite
End Sub



Private Sub cmdDetails_Click(Index As Integer)
Dim lu%, Status As Boolean, Rec&, A$
Dim BINlu%, OUTlu%, INPlu%, OLDlu%, FileRecords&, V!(1 To 4), X$, i%, UArecord&
Static Record&
'
' SOME NOTES ON RADIOSONDE SITE FILES
'
' Over the years radiosonde site information has been downloaded from various
' locations. Initially we used the WMO flatfile, which also includes surface sites
' This file is quite large and unweildly so we are currently starting with two files
' with International and North American radiosonde lists available at:
' http://raob.fsl.noaa.gov/General_Information.html
' The files here are called "History Files" as they provide a complete history of
' when site parameters (e.g., latitude, longitude, elevation, etc.) changed.
' The file names seem to change; they are currently called:
' http://raob.fsl.noaa.gov/intl/international_sites.txt
' http://raob.fsl.noaa.gov/intl/stat2000.txt
' Note that FSL is now called GDS (Global Data Systems) under ESRL
' Since we are only interested in the current RAOB site information, the two History
' Files are editted to reflect this and stored at: C:\MTP\RAOB\BIN\MTP_UA.TXT
' This is done using the Gen BIN button on the frmRAOBsites form.
' Next the "TXT to BIN" button is used to generate: C:\MTP\RAOB\BIN\MTP_UA.BIN
' Rather than starting from scratch every time new sites are encountered, we
' generally edit the MTP_UA.TXT file manually, adding the new site to the end.
' Then, the "TXT to BIN" button is used to generate: C:\MTP\RAOB\BIN\MTP_UA.BIN
'
'123456789012345678901234567890123456789012345678901234567890
'WMOstn Name      Latit  Longit  Altm R CY ST Location
'042700 BGBW      61.18  -45.42    26 6 GL -- NARSSARSSUAQ                     00


Select Case Index
Case 0
  For i = 0 To 6
    txtLocation(i).Text = ""              'Clear current entries
  Next i
  i = -1
  Do: i = i + 1
  Loop Until optShow(i).Value = True
  If i = 2 Then  'Goto Record
    lu = FreeFile
    Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #lu Len = Len(GP)
    FileRecords = LOF(lu) \ Len(GP)
    Record = Val(txtShow(2).Text)     'Goto
    If Record < 1 Or Record > FileRecords Then Exit Sub
    GPread lu, Record
    Close lu
    UpdateRAOBsiteInfo
  Else
    Status = fGetUAsite(UCase$(txtShow(i).Text), Record)
    txtShow(2).Text = Str(Record)
    If Status Then UpdateRAOBsiteInfo
  End If

Case 1  'Save MTP_UA.TXT as MTP_UA.BIN
  BINlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #BINlu Len = Len(GP)
  INPlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.TXT" For Input As #INPlu
  Record = 0
  Do
'461 72768 GGW       48.22   -106.62   700 4  US   MT  GLASGOW INTL ARPT                00
'462 72476 GJT       39.12   -108.53  1475 4  US   CO  GRAND JUNCTION                   00
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'         1         2         3         4         5         6         7         8         9
    Line Input #INPlu, X$
    Record = Record + 1
    GPcards = Val(Mid$(X$, 4, 6)) * 10
    GPicao = Trim(Mid$(X$, 11, 5))
    GPlatitude = Val(Mid$(X$, 20, 6))
    GPlongitude = Val(Mid$(X$, 29, 7))
    GPaltitude = Val(Mid$(X$, 37, 5))
    GPregion = Val(Mid$(X$, 43, 1))
    GPcountry = Trim(Mid$(X$, 45, 3))
    GPstate = Trim(Mid$(X$, 49, 4))
    GPstation = Trim(Mid$(X$, 54, 30))
    GPwrite BINlu, Record
    txtShow(2) = Record
  Loop Until EOF(INPlu)
  Close BINlu, INPlu
  
Case 2  'Add a new site
  BINlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #BINlu Len = Len(GP)
  FileRecords = LOF(BINlu) \ Len(GP)
  GPcards = Val(txtShow(1).Text) * 10
  GPicao = txtShow(0).Text
  GPregion = Val(txtLocation(0).Text)
  GPcountry = txtLocation(1).Text
  GPstate = txtLocation(2).Text
  GPstation = txtLocation(3).Text
  GPlatitude = Val(txtLocation(4).Text)
  GPlongitude = Val(txtLocation(5).Text)
  GPaltitude = Val(txtLocation(6).Text)
  GPwrite BINlu, FileRecords + 1
  txtShow(2) = FileRecords + 1
  Close BINlu
  
Case 3  'Remove a site
  OLDlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #OLDlu Len = Len(GP)
  BINlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BINnew" For Random Access Read Write As #BINlu Len = Len(GP)
  FileRecords = LOF(OLDlu) \ Len(GP)
'  Status = fGetUAsite(txtStation(AddIndex).Text, UArecord)
  Record = Val(txtShow(2).Text)
  ' Copy first RECORD-1 records
  For i = 1 To Record - 1
    Rec = i
    GPread OLDlu, Rec
    GPwrite BINlu, Rec
    txtShow(2).Text = Val(i)
    DoEvents
  Next i
  UpdateRAOBsiteInfo  'Display site before REMOVED site
  If Record < FileRecords Then
    For i = Record To FileRecords - 1
      Rec = i + 1
      GPread OLDlu, Rec
      GPwrite BINlu, Rec - 1
      txtShow(2).Text = Val(Rec - 1)
      DoEvents
    Next i
  End If
  Close BINlu
  Close OLDlu
  Kill "C:\MTP\RAOB\BIN\MTP_UA.BIN"
  Name "C:\MTP\RAOB\BIN\MTP_UA.BINnew" As "C:\MTP\RAOB\BIN\MTP_UA.BIN"
  
Case 4  'Save sites as ASCII file
  BINlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #BINlu Len = Len(GP)
  OUTlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.txt" For Output As #OUTlu
  Record = 0
  FileRecords = LOF(BINlu) \ Len(GP)
  Do
    Record = Record + 1
    GPread BINlu, Record
    txtShow(2) = Record
    X$ = Format(Record, "000 ") + Format(GPcards / 10, "00000") + Format(GPicao, " @@@@@@@@")
    V(1) = GPlatitude:   X$ = X$ + " " + fUsing("##0.00", V())
    V(1) = GPlongitude:  X$ = X$ + " " + fUsing("#####0.00", V())
    V(1) = GPaltitude:   X$ = X$ + " " + fUsing("####0", V())
    X$ = X$ + Format(GPregion, " 0") + Format(GPcountry, "  @@ ") + Format(GPstate, "  @@  ")
    X$ = X$ + GPstation + Space(32 - Len(GPstation)) + " 00"
    
'    If GPcountry = "US" Then
      Debug.Print X$
      Print #OUTlu, X$
'    End If
  Loop Until EOF(BINlu) Or Record = FileRecords
  Close BINlu, OUTlu

Case 5  'Save edit information
  BINlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #BINlu Len = Len(GP)
  FileRecords = LOF(BINlu) \ Len(GP)
'  i = -1
'  Do: i = i + 1
'  Loop Until optShow(i).value = True
'  Status = fGetUAsite(txtShow(i).Text, UArecord)
'  If Status Then
    GPcards = Val(txtShow(1).Text) * 10
    GPicao = txtShow(0).Text
    GPregion = Val(txtLocation(0).Text)
    GPcountry = txtLocation(1).Text
    GPstate = txtLocation(2).Text
    GPstation = txtLocation(3).Text
    GPlatitude = Val(txtLocation(4).Text)
    GPlongitude = Val(txtLocation(5).Text)
    GPaltitude = Val(txtLocation(6).Text)
    GPwrite BINlu, Record
'  End If
  Close BINlu
  ReInit = True   'Force fGetUAsite to re-read BIN file for updated information
  
Case 6, 7, 8, 9 'Next
  lu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #lu Len = Len(GP)
  FileRecords = LOF(lu) \ Len(GP)
  Select Case Index
  Case 6
    Record = Val(txtShow(2).Text) + 1 'Next
    If Record > FileRecords Then Exit Sub
  Case 7
    Record = Val(txtShow(2).Text) - 1 'Previous
    If Record < 1 Then Exit Sub
  Case 9
    Record = 1                         'First
  Case 8                               'Last
    Record = FileRecords
  End Select
  GPread lu, Record
  Close lu
  txtShow(2).Text = Str(Record)
  UpdateRAOBsiteInfo
  
Case 10
  Unload Me

Case 11  'Generate MTP_UA.BIN file from intl2000.wmo ascii text file
'         1         2         3         4         5         6         7         8         9        10        11
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
' 1902
'ENJA  99999  1001  70.93   -8.67     9 00000000 99999999  JAN MAYEN(NOR-NAVY)           99 NO
'ENAS  99999  1004  78.92   11.93     8 00000000 99999999  NY-ALESUND II                 99 NO
'ICAO  WBAN  WMO   Lat    Lon     Altit yyyymmdd yyyymmdd  City                          ST Country
'Download International and North American radiosonde list files from:
'http://raob.fsl.noaa.gov/General_Information.html
'The file names seem to change; they are currently called:
'http://raob.fsl.noaa.gov/intl/international_sites.txt
'http://raob.fsl.noaa.gov/intl/stat2000.txt
'Note that FSL is now called GDS (Global Data Systems) under ESRL

  INPlu = FreeFile
  Open "C:\MTP\RAOB\BIN\intl2000.wmo" For Input As INPlu
  Line Input #INPlu, X$
  BINlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #BINlu Len = Len(GP)
  Record = 0
  Do
    Line Input #INPlu, X$
    If Mid$(X$, 49, 8) = "99999999" Then 'ignore history lines
      Record = Record + 1
      GPcards = Val(Mid$(X$, 13, 5)) * 10   'WMO number*10
      GPicao = Trim(Left$(X$, 4))       '
      GPlatitude = Val(Mid$(X$, 19, 6))
      GPlongitude = Val(Mid$(X$, 26, 7))
      GPaltitude = Val(Mid$(X$, 34, 5))
      GPregion = 9                         'Region unknown
      GPcountry = Trim(Mid$(X$, 92, 2))
      GPstate = Trim(Mid$(X$, 89, 2))
      GPstation = Trim(Mid$(X$, 59, 29))
      GPwrite BINlu, Record
      txtShow(2) = Record
    End If
  Loop Until EOF(INPlu)


End Select
FileRecords = FileLen("C:\MTP\RAOB\BIN\MTP_UA.BIN") \ Len(GP)
lblRecords.Caption = "of " + Str(FileRecords)

End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()

optShow(0).Value = True
cmdDetails_Click 0

End Sub

