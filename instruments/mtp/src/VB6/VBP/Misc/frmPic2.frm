VERSION 5.00
Begin VB.Form frmPic2 
   BackColor       =   &H00FFFFFF&
   Caption         =   "Form1"
   ClientHeight    =   6510
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   8475
   LinkTopic       =   "Form1"
   ScaleHeight     =   6510
   ScaleWidth      =   8475
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picTrack2 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      ForeColor       =   &H0000FFFF&
      Height          =   6030
      Left            =   480
      Picture         =   "frmPic2.frx":0000
      ScaleHeight     =   6030
      ScaleWidth      =   7815
      TabIndex        =   0
      Top             =   120
      Width           =   7815
   End
End
Attribute VB_Name = "frmPic2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Public FirstMouseLon!, FirstMouseLat!

Dim UV&()                   'UTsec, UTsecMTP, UTsecNav
'Dim UVindex%                'Current record
'Dim OATV!()                 'Tnav, Tmms, Tmtp
Dim NAVV!()                 'Pitch, Roll, Latitude, Longitude, Heading, TAS, Wspd, Wdir, Elcor, ElcorUsed

Dim ALTV!()                 'pALT, gALT, rALT, mALT

Private Sub Form_DblClick()
' capture this forms image to image1
'
'    Set frmPic1.Image1.Picture = CaptureForm(Me)
'
' Of course, we could have done it using :
'
'    set image1.Picture = capturewindow(me.hWnd .... etc)
    
'
' You could of course now save the picture using :
'
' SavePicture frmPic1.Image1.Picture, MAPget2.txtGIFfile.Text
' CaptureForm Me, MAPget2.txtGIFfile.Text
 CaptureFormMJ Me, MAPget2.txtGIFfile.Text, 3, True
End Sub

Private Sub Form_Load()
  
  With frmPic2
  .picTrack2.ScaleMode = vbPixels
  .picTrack2.AutoRedraw = True
  End With

End Sub


Private Sub picTrack2_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
FirstMouseLon = Int(x)
FirstMouseLat = Int(y)
End Sub


Private Sub picTrack2_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%, j%, k%, k1%, k2%, iUT&, jLoc%, iX%, iY%, Xthreshold!, Ythreshold!, iRec%, Track$
Dim Second As Boolean, GotFirst As Boolean, Rsave%

  If TotalCycles <= 20 Then Exit Sub
  If Not ArrayRead Then Exit Sub
  iX = Int(x): iY = Int(y): iRec = 0: iUT = 0
  Xthreshold = 10 * (Val(MAPget2.txtLON2.Text) - Val(MAPget2.txtLON1.Text)) / picTrack2.Width
  Ythreshold = 10 * (Val(MAPget2.txtLAT2.Text) - Val(MAPget2.txtLAT1.Text)) / picTrack2.Height
  If MAPget2.chkSecond.Value = 1 Then Second = True Else Second = False
  GotFirst = False
  Rsave = 0
  For r = 1 To TotalCycles
    If Abs(NAVV(3, r) - y) < Ythreshold And Abs(NAVV(4, r) - x) < Xthreshold Then
      If Not GotFirst Then
        GotFirst = True
        If Not Second Then
          iUT = UV(1, r): iRec = r:  Exit For
        Else
          If Rsave = 0 Then Rsave = r
        End If
      End If
      If GotFirst And r > Rsave + 20 Then
        iUT = UV(1, r): iRec = r: Exit For
      End If
    End If
  Next r

  If iUT <> 0 Or iRec <> 0 Then
    
    Select Case MAPget2.cboTrack.Text
    Case "Rec No.": Track$ = "  Scan=" + Str(iRec)
    Case "seconds": Track$ = "  UT(s)=" + Format(iUT, "#####0")
    Case "ks": Track$ = "  UT(ks)=" + Format(iUT / 1000, "##0.000")
    Case "hhmmss": Track$ = "  UT=" + Format(iUT / 1000, "##0.000")
    Case "hh:mm:ss": Track$ = "  UT=" + fSecToTstring$(iUT, True)
    End Select
    
    Select Case MAPget2.cboPalt.Text
    Case "km": Track$ = Track$ + " Z(km)=" + Format(ALTV(1, r), "#0.00")
    Case "kft": Track$ = Track$ + " Z(kft)=" + Format(ALTV(1, r) * kft_km, "##0.0")
    End Select
    picTrack2.ToolTipText = "Lon=" + Format(x, "##0.00") + "  " + "Lat=" + Format(y, "##0.00") + Track$
  
  Else
    
    picTrack2.ToolTipText = "Lon=" + Format(x, "##0.00") + "  " + "Lat=" + Format(y, "##0.00")
  End If
End Sub


