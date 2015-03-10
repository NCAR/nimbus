VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{27395F88-0C0C-101B-A3C9-08002B2F49FB}#1.1#0"; "PICCLP32.OCX"
Begin VB.Form frmCapture 
   AutoRedraw      =   -1  'True
   BackColor       =   &H00E0E0E0&
   Caption         =   "MJ's Capture"
   ClientHeight    =   4215
   ClientLeft      =   165
   ClientTop       =   450
   ClientWidth     =   4095
   DrawMode        =   6  'Mask Pen Not
   FillColor       =   &H8000000F&
   ForeColor       =   &H8000000F&
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   NegotiateMenus  =   0   'False
   ScaleHeight     =   4215
   ScaleWidth      =   4095
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer tmrSave 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3540
      Top             =   2970
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   3435
      Left            =   30
      TabIndex        =   7
      Top             =   60
      Width           =   4035
      _ExtentX        =   7117
      _ExtentY        =   6059
      _Version        =   393216
      TabHeight       =   520
      BackColor       =   14737632
      TabCaption(0)   =   "&Use"
      TabPicture(0)   =   "frmCapture.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "picFr"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).ControlCount=   1
      TabCaption(1)   =   "&Configure"
      TabPicture(1)   =   "frmCapture.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Label(0)"
      Tab(1).Control(1)=   "Label(2)"
      Tab(1).Control(2)=   "cboQualityJPG"
      Tab(1).Control(3)=   "chkAutoName"
      Tab(1).Control(4)=   "txtAutoNameFilename"
      Tab(1).Control(5)=   "chkTransparency"
      Tab(1).Control(6)=   "chkAutoExit"
      Tab(1).Control(7)=   "Command1"
      Tab(1).ControlCount=   8
      TabCaption(2)   =   "&Help"
      TabPicture(2)   =   "frmCapture.frx":0038
      Tab(2).ControlEnabled=   0   'False
      Tab(2).ControlCount=   0
      Begin VB.CommandButton Command1 
         Caption         =   "Test"
         Height          =   495
         Left            =   -72570
         TabIndex        =   17
         Top             =   1605
         Width           =   1005
      End
      Begin VB.CheckBox chkAutoExit 
         Caption         =   "Exit after Capture"
         Height          =   285
         Left            =   -74880
         TabIndex        =   16
         Top             =   1170
         Width           =   1605
      End
      Begin VB.CheckBox chkTransparency 
         Caption         =   "Make Transparent"
         Height          =   255
         Left            =   -74880
         TabIndex        =   15
         Top             =   840
         Width           =   1785
      End
      Begin VB.TextBox txtAutoNameFilename 
         Height          =   285
         Left            =   -73320
         TabIndex        =   13
         Text            =   "Capture"
         Top             =   3030
         Width           =   1215
      End
      Begin VB.CheckBox chkAutoName 
         Caption         =   "Automatically Name Captured Files"
         Height          =   255
         Left            =   -74880
         TabIndex        =   12
         Top             =   2760
         Width           =   2925
      End
      Begin VB.ComboBox cboQualityJPG 
         Height          =   315
         Left            =   -73770
         TabIndex        =   10
         Text            =   "80"
         Top             =   390
         Width           =   675
      End
      Begin VB.PictureBox picFr 
         BackColor       =   &H8000000E&
         FillColor       =   &H8000000F&
         FillStyle       =   7  'Diagonal Cross
         FontTransparent =   0   'False
         Height          =   3045
         Left            =   60
         ScaleHeight     =   2985
         ScaleWidth      =   3825
         TabIndex        =   8
         Top             =   330
         Width           =   3885
         Begin VB.PictureBox picSelImage 
            AutoRedraw      =   -1  'True
            AutoSize        =   -1  'True
            BackColor       =   &H00808080&
            BorderStyle     =   0  'None
            FillStyle       =   0  'Solid
            Height          =   2000
            Left            =   -30
            MousePointer    =   99  'Custom
            Negotiate       =   -1  'True
            ScaleHeight     =   1995
            ScaleWidth      =   1995
            TabIndex        =   9
            Top             =   -30
            Width           =   2000
            Begin VB.Shape Sh 
               BorderColor     =   &H000000FF&
               Height          =   345
               Left            =   390
               Top             =   630
               Visible         =   0   'False
               Width           =   705
            End
         End
      End
      Begin VB.Label Label 
         Caption         =   "Auto Name Filename"
         Height          =   345
         Index           =   2
         Left            =   -74880
         TabIndex        =   14
         Top             =   3060
         Width           =   1485
      End
      Begin VB.Label Label 
         Caption         =   "JPG Quality"
         Height          =   195
         Index           =   0
         Left            =   -74940
         TabIndex        =   11
         Top             =   450
         Width           =   855
      End
   End
   Begin VB.ComboBox cboExtension 
      Height          =   315
      Left            =   3360
      TabIndex        =   6
      Text            =   "PNG"
      Top             =   3510
      Width           =   735
   End
   Begin VB.TextBox txtFilename 
      Height          =   315
      Left            =   2250
      TabIndex        =   5
      Text            =   "Filename"
      Top             =   3510
      Width           =   1065
   End
   Begin VB.ComboBox cboPath 
      Height          =   315
      Left            =   30
      TabIndex        =   4
      Text            =   "C:\MTP\GIF\"
      Top             =   3510
      Width           =   2205
   End
   Begin VB.CommandButton cmdCapture 
      Caption         =   "E&xit"
      Height          =   345
      Index           =   3
      Left            =   3060
      TabIndex        =   3
      Top             =   3870
      Width           =   1005
   End
   Begin VB.CommandButton cmdCapture 
      Caption         =   "Clip&board"
      Height          =   345
      Index           =   2
      Left            =   2040
      TabIndex        =   2
      Top             =   3870
      Width           =   1005
   End
   Begin VB.CommandButton cmdCapture 
      Caption         =   "&Save"
      Height          =   345
      Index           =   1
      Left            =   1020
      TabIndex        =   1
      Top             =   3870
      Width           =   1005
   End
   Begin VB.CommandButton cmdCapture 
      Caption         =   "&Capture"
      Height          =   345
      Index           =   0
      Left            =   0
      TabIndex        =   0
      Top             =   3870
      Width           =   1005
   End
   Begin PicClip.PictureClip PicClip 
      Left            =   3330
      Top             =   6840
      _ExtentX        =   4313
      _ExtentY        =   1244
      _Version        =   393216
   End
End
Attribute VB_Name = "frmCapture"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim m_bHScrV As Boolean, m_bVScrV As Boolean, m_iBordW As Long, m_iX As Single, m_iY As Single
Dim m_bDraw As Boolean
Dim m_XHi As Single, m_XLo As Single, m_YHi As Single, m_YLo As Single
Dim m_sTempFolder As String, m_sTempFile As String, m_sTitle As String

Dim WithEvents m_cScroll As cScrollBars
Attribute m_cScroll.VB_VarHelpID = -1
Dim WithEvents oleP As VB.OLE
Attribute oleP.VB_VarHelpID = -1
Dim m_sFile As String
Dim m_iBordH As Long
Private Sub SaveAs()
  Dim sFile As String, i As Integer, sExt As String
  
  sFile = FileSaveDialog(hWnd, "Portable Network Graphics (*.png)|*.png|JPEG Image (*.jpg)|*.jpg|BMP Image (*.bmp)|*.bmp|GIF Image (*.gif)|*.gif", "Save Image as", g_sDefaultFolder, txtFilename.Text)
  If Len(sFile) = 0 Then Exit Sub
    
  Select Case LCase$(Right(sFile, 4))
    Case ".png", ".jpg", ".bmp", ".gif"
    Case Else
      MsgBox "Images can be saved only in PNG, BMP, GIF, or  JPEG format.", vbOKOnly
      Exit Sub
  End Select
    
  On Error Resume Next
  GetAttr sFile
  If Err.Number = 0 Then
    Kill sFile
    If Err.Number <> 0 Then
      MsgBox Err.Description, vbCritical
      Exit Sub
    End If
  End If

' Everything is ok
  txtFilename.Text = Left$(fGetFilename(sFile), Len(fGetFilename(sFile)) - 4)
  SaveSetting App.Title, "Setting", "DefaultFileName", txtFilename.Text
  m_sFile = sFile
  tmrSave.Enabled = True

End Sub

Function fGetFilename(File$) As String
' Get Filename from last "/" or "\"
Dim i%, j%
fGetFilename = ""
j = Len(File$)
i = j
Do
  i = i - 1
  If Mid$(File$, i, 1) = "\" Or Mid$(File$, i, 1) = "/" Then
    fGetFilename = Mid$(File$, i + 1, Len(File$))
    Exit Function
  End If
Loop Until i = 1
End Function

Private Sub Capture()
    Sh.Visible = False
'    Tb.Buttons("Gut").Enabled = False
    m_bDraw = False
    
    MousePointer = vbHourglass
    Visible = False
    DoEvents
    g_iiW = 0
    Set m_frmDesktop = New frmDesktop
    m_frmDesktop.Show vbModal, Me

    m_frmCapture.Left = (Screen.Width - m_frmCapture.Width) / 2
    m_frmCapture.Top = (Screen.Height - m_frmCapture.Height) / 2

'    Visible = True
    MousePointer = vbDefault
End Sub

Private Sub CopyToClipboard()
    Clipboard.Clear
    Clipboard.SetData picSelImage
End Sub

Private Sub cboExtension_Click()

  g_sDefaultExtension = cboExtension.List(cboExtension.ListIndex)
  SaveSetting App.Title, "Setting", "DefaultExtension", g_sDefaultExtension

End Sub


Private Sub cboPath_Click()
  g_sDefaultFolder = cboPath.List(cboPath.ListIndex)
  SaveSetting App.Title, "Setting", "DefaultFolder", g_sDefaultFolder
End Sub


Private Sub cboQualityJPG_Click()
        
  g_iQualityJPG = cboQualityJPG.List(cboQualityJPG.ListIndex)
  SaveSetting App.Title, "Setting", "QualityJPG", g_iQualityJPG

End Sub


Private Sub chkAutoExit_Click()
  If chkAutoExit.Value = 1 Then
    g_bAutoExit = True
  Else
    g_bAutoExit = False
  End If
  SaveSetting App.Title, "Setting", "AutoExit", g_bAutoExit

End Sub

Private Sub chkAutoName_Click()
  If chkAutoName.Value = 1 Then
    g_bAutoName = True
    cmdCapture(1).Enabled = False
  Else
    g_bAutoName = False
    cmdCapture(1).Enabled = True
  End If
  SaveSetting App.Title, "Setting", "AutoName", g_bAutoName
  
End Sub

Private Sub chkTransparency_Click()

  If chkTransparency.Value = 1 Then
    g_bTransparency = True
  Else
    g_bTransparency = False
  End If
  SaveSetting App.Title, "Setting", "Transparency", g_bTransparency

End Sub

Private Sub cmdCapture_Click(Index As Integer)
Dim i%, sFile As String

Select Case Index
Case 0  'Capture
  If m_frmCapture Is Nothing Then Set m_frmCapture = New frmCapture
  With m_frmCapture
    .AutoRedraw = True
    With .picSelImage
      .Width = Form1.Width
      .Height = Form1.Height
      .Picture = CaptureForm(Form1)
      DoEvents
    End With
'    With .picFr
'      .Move .Left, .Top, IIf(m_iScaleWidth > ScaleWidth, m_iScaleWidth, ScaleWidth) - 2 * .Left, IIf(m_iScaleHeight > ScaleHeight, m_iScaleHeight, ScaleHeight) - Screen.TwipsPerPixelY * 76
'    End With
  End With
'  Unload Me
  m_frmCapture.Visible = True '  Capture
'  If g_bAutoName Then tmrSave.Enabled = True


Case 1  'Manual Save
  SaveAs
  
  
Case 2  'Clipboard
  CopyToClipboard
  
Case 3  'Exit
  SaveSetting App.Title, "Setting", "QualityJPG", g_iQualityJPG
  SaveSetting App.Title, "Setting", "Transparency", g_bTransparency
  SaveSetting App.Title, "Setting", "AutoExit", g_bAutoExit
  SaveSetting App.Title, "Setting", "DefaultFolder", g_sDefaultFolder
  SaveSetting App.Title, "Setting", "DefaultExtension", g_sDefaultExtension
  SaveSetting App.Title, "Setting", "DefaultFileName", txtFilename.Text
  SaveSetting App.Title, "Setting", "AutoName", g_bAutoName
  Unload Me
  End

  
End Select
'        Case "Options"
'            Dim fOptions As frmOptions
'            Set fOptions = New frmOptions
'            fOptions.Show vbModal, Me
'            Set fOptions = Nothing
'        Case "About"
'            Dim fAbout As frmAbout
'            Set fAbout = New frmAbout
'            fAbout.Show vbModal, Me
'            Set fAbout = Nothing
'    End Select
End Sub

Private Sub Command1_Click()
Dim hWnd As Long
'  hWnd = Form1.hWnd
'  sFile = FileSaveDialog(hWnd, "Portable Network Graphics (*.png)|*.png|JPEG Image (*.jpg)|*.jpg|BMP Image (*.bmp)|*.bmp|GIF Image (*.gif)|*.gif", "Save Image as", g_sDefaultFolder, txtFilename.Text)

End Sub

Private Sub Form_Activate()
    Set m_frmDesktop = Nothing
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = vbKeyEscape Then Unload Me
End Sub

Private Sub Form_Load()
Dim s$, i%, j%
Static Initialize As Boolean

  If Not Initialize Then
    ' Get JPG Quality
    Select Case g_iQualityJPG
    Case Is < 20: g_iQualityJPG = 20
    Case Is > 100: g_iQualityJPG = 100
    End Select
    With cboQualityJPG
      j = 0
      For i = 20 To 100 Step 5
        .AddItem i
        If i = g_iQualityJPG Then j = .NewIndex
      Next i
    End With
    If j < cboQualityJPG.ListCount Then cboQualityJPG.ListIndex = j
    
    ' Get default folder
    With cboPath
      .AddItem "C:\MTP\PNG\"
      .AddItem "C:\WB57\AVE\"
      .AddItem "C:\DC8\SOLVE2\"
    End With
    For i = 0 To cboPath.ListCount - 1
      If g_sDefaultFolder = cboPath.List(i) Then Exit For
    Next i
    If i < cboPath.ListCount Then cboPath.ListIndex = i
    
    ' Get default extension
    With cboExtension
      .AddItem "PNG"
      .AddItem "JPG"
      .AddItem "GIF"
      .AddItem "BMP"
    End With
    For i = 0 To cboExtension.ListCount - 1
      If g_sDefaultExtension = cboExtension.List(i) Then Exit For
    Next i
    If i < cboExtension.ListCount Then cboExtension.ListIndex = i
    
    txtFilename.Text = g_sDefaultFilename
    txtAutoNameFilename.Text = g_sAutoNameFilename
    If g_bAutoName Then chkAutoName.Value = 1 Else chkAutoName.Value = 0
    If g_bAutoExit Then chkAutoExit.Value = 1 Else chkAutoExit.Value = 0
    If g_bTransparency Then chkTransparency.Value = 1 Else chkTransparency.Value = 0
    
    
    Left = (Screen.Width - Width) / 2
    Top = (Screen.Height - Height) / 2
    Initialize = True
  End If
    
    
    
  m_iBordW = (picFr.Width - picFr.ScaleWidth) / 2
  m_iBordH = (picFr.Height - picFr.ScaleHeight) / 2

'  m_sTitle = Caption
  
  Set m_cScroll = New cScrollBars
  m_cScroll.Create picFr.hWnd
  picSelImage.Move 0, 0

  m_iScaleHeight = picFr.Top + 2 * m_iBordH '- 2000
  m_iScaleWidth = picFr.Left + 2 * m_iBordW + 2000
    
  If g_bAutoName Then tmrSave.Enabled = True
  Form1.Show
End Sub

Private Sub Form_Unload(Cancel As Integer)
'    EditPaint "Close"
End Sub

Private Sub picFr_Resize()
    Dim lHeight As Long
    Dim lWidth As Long
    Dim lProportion As Long

    lHeight = 1# * (picSelImage.Height - picFr.ScaleHeight) \ Screen.TwipsPerPixelY
    If (lHeight > 0) Then
        lProportion = lHeight \ (picFr.ScaleHeight \ Screen.TwipsPerPixelY) + 1
        m_cScroll.LargeChange(efsVertical) = lHeight \ lProportion
        m_cScroll.Max(efsVertical) = lHeight
        m_cScroll.Visible(efsVertical) = True
    Else
        m_cScroll.Visible(efsVertical) = False
        picSelImage.Top = 0
    End If
    
    lWidth = (picSelImage.Width - picFr.ScaleWidth) \ Screen.TwipsPerPixelX
    If (lWidth > 0) Then
        lProportion = lWidth \ (picFr.ScaleWidth \ Screen.TwipsPerPixelX) + 1
        m_cScroll.LargeChange(efsHorizontal) = lWidth \ lProportion
        m_cScroll.Max(efsHorizontal) = lWidth
        m_cScroll.Visible(efsHorizontal) = True
    Else
        m_cScroll.Visible(efsHorizontal) = False
        picSelImage.Left = 0
    End If
    picFr.Line (0, 0)-(Screen.Width, Screen.Height), , B
End Sub

Private Sub picSelImage_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button <> 1 Then Exit Sub
    If Not m_bDraw Then
        Sh.Visible = False
'        Tb.Buttons("Gut").Enabled = False
        m_XLo = X
        m_YLo = Y
        m_bDraw = True
    End If
End Sub

Private Sub picSelImage_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button <> 1 Then Exit Sub

    Dim iWidth As Single, iHeight As Single, iLeft As Single, iTop As Single
    If Not m_bDraw Then Exit Sub
    If Not Sh.Visible Then
        Sh.Visible = True
'        Tb.Buttons("Gut").Enabled = True
    End If
    m_XHi = X
    m_YHi = Y
    Sh.Width = Abs(m_XHi - m_XLo)
    Sh.Height = Abs(m_YHi - m_YLo)
    Sh.Visible = True
    If m_XHi > m_XLo And m_YHi > m_YLo Then
        Sh.Top = m_YLo
        Sh.Left = m_XLo
    ElseIf m_XHi > m_XLo And m_YHi < m_YLo Then
        Sh.Top = m_YHi
        Sh.Left = m_XLo
    ElseIf m_XHi < m_XLo And m_YHi < m_YLo Then
        Sh.Top = m_YHi
        Sh.Left = m_XHi
    ElseIf m_XHi < m_XLo And m_YHi > m_YLo Then
        Sh.Top = m_YLo
        Sh.Left = m_XHi
    End If
    DoEvents
End Sub

Private Sub picSelImage_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button <> 1 Then Exit Sub
    m_bDraw = False
End Sub

Private Sub GutPicture()
    Dim iScaleMode As Integer
    Dim iWidth As Single, iHeight As Single, iLeft As Single, iTop As Single

    iWidth = picSelImage.ScaleWidth / Screen.TwipsPerPixelX
    iHeight = picSelImage.ScaleHeight / Screen.TwipsPerPixelY
    Sh.Visible = False
    iScaleMode = picSelImage.ScaleMode
    picSelImage.ScaleMode = 3
    PicClip.Picture = picSelImage.Picture
    iLeft = IIf(Sh.Left < 0, 0, Sh.Left)
    PicClip.ClipX = iLeft
    iTop = IIf(Sh.Top < 0, 0, Sh.Top)
    PicClip.ClipY = iTop
    PicClip.ClipWidth = IIf(iLeft + Sh.Width > iWidth, iWidth - iLeft, Sh.Width)
    PicClip.ClipHeight = IIf(iTop + Sh.Height > iHeight, iHeight - iTop, Sh.Height)
    picSelImage.Picture = PicClip.Clip
    picSelImage.ScaleMode = iScaleMode
    picSelImage.Refresh
    Form_Resize
    
'    Tb.Buttons("Gut").Enabled = False
End Sub

Private Sub picSelImage_DblClick()
    EditPaint "Open"
End Sub

Private Sub EditPaint(sEdit As String)
    Dim iErrNumber As Long, sErrDesc As String
    Select Case sEdit
        Case "Open"
            If oleP Is Nothing Then Set oleP = Controls.Add("VB.OLE", "ctlOLE")
            With oleP    ' Me.PaintBr
                On Error GoTo Err_
                m_sTempFolder = GetTempFolderName
                m_sTempFile = m_sTempFolder & "Snap-It.bmp"
                SavePicture picSelImage.Picture, m_sTempFile
                .CreateLink m_sTempFile
                .Action = 7
            End With
            Caption = m_sTitle & "_E"
            Visible = False
            Exit Sub
        Case "Save"
            picSelImage.Picture = LoadPicture(m_sTempFile)
            Exit Sub
    End Select

Err_:
    Caption = m_sTitle
    Visible = True
    SetFocus
    If Err.Number <> 0 Then iErrNumber = Err.Number: sErrDesc = Err.Description
    If Len(m_sTempFile) > 0 Then DeleteFile m_sTempFile: m_sTempFile = ""
    If Len(m_sTempFolder) > 0 Then RemoveDirectory m_sTempFolder: m_sTempFolder = ""
    If iErrNumber <> 0 Then MsgBox sErrDesc, vbCritical, "Error No " & iErrNumber
End Sub

Private Sub oleP_Updated(code As Integer)
    Select Case code
        Case 1      ' Saved
            EditPaint "Save"
        Case 2      ' Closed
            EditPaint "Close"
    End Select
End Sub

Private Sub Form_Resize()
    If Me.WindowState = vbMinimized Then Exit Sub
    
    With picFr
        .Move .Left, .Top, IIf(m_iScaleWidth > ScaleWidth, m_iScaleWidth, ScaleWidth) - 2 * .Left, IIf(m_iScaleHeight > ScaleHeight, m_iScaleHeight, ScaleHeight) - Screen.TwipsPerPixelY * 76
    End With
End Sub

Private Sub m_cScroll_Change(eBar As EFSScrollBarConstants)
   m_cScroll_Scroll eBar
End Sub

Private Sub m_cScroll_Scroll(eBar As EFSScrollBarConstants)
   If (eBar = efsHorizontal) Then
      picSelImage.Left = -Screen.TwipsPerPixelX * m_cScroll.Value(eBar)
   Else
      picSelImage.Top = -Screen.TwipsPerPixelY * m_cScroll.Value(eBar)
   End If
End Sub



Private Sub tmrSave_Timer()
Dim i As Long, sDefaultFolder As String, sTempFolder As String, sTempFile As String
Dim JG As New janGraphics.Compendium, sErrDesc As String, iErrNumber As Long
Dim sFile As String

    tmrSave.Enabled = False
    If g_bAutoName Then   'Get m_sFile if SaveAs not used
      sFile = cboPath.Text + txtAutoNameFilename.Text
      i = 0
      Do                'Search for first unused filename for auto capture
        i = i + 1
        m_sFile = sFile + Format(i, "000") + "." + cboExtension.Text
      Loop Until Dir(m_sFile) = ""
    End If
    
    On Error GoTo Err_
    sTempFolder = GetTempFolderName
    sTempFile = sTempFolder & "TempPic.bmp"
    SavePicture m_frmCapture!picSelImage.Picture, sTempFile
    
    JG.convertEx sTempFile, m_sFile, 0, 0, False, g_iQualityJPG
    
    i = InStrRev(m_sFile, "\")
    If i = 0 Then Exit Sub
    sDefaultFolder = Left$(m_sFile, i)
    If g_sDefaultFolder <> sDefaultFolder Then
        SaveSetting App.Title, "Setting", "DefaultFolder", sDefaultFolder
        g_sDefaultFolder = sDefaultFolder
    End If
    
Err_:
    If Err.Number <> 0 Then iErrNumber = Err.Number: sErrDesc = Err.Description
    Set JG = Nothing
    If Len(sTempFile) > 0 Then DeleteFile sTempFile: sTempFile = ""
    If Len(sTempFolder) > 0 Then RemoveDirectory sTempFolder: sTempFolder = ""
    If iErrNumber <> 0 Then MsgBox sErrDesc, vbCritical, "Error No " & iErrNumber
    
    If g_bAutoExit Then cmdCapture_Click (3)

'    Unload Me
' Variant to hold 2-dimensional array returned by GetAllSettings
' Integer to hold counter.
'Dim MySettings As Variant, intSettings As Integer
' Place some settings in the registry.
'SaveSetting appname:="MyApp", section:="Startup", _
'Key:="Top", setting:=75
'SaveSetting "MyApp", "Startup", "Left", 50
'' Retrieve the settings.
'MySettings = GetAllSettings(appname:="MyApp", section:="Startup")
'   For intSettings = LBound(MySettings, 1) To UBound(MySettings, 1)
'      Debug.Print MySettings(intSettings, 0), MySettings(intSettings, 1)
'   Next intSettings
'DeleteSetting "MyApp", "Startup"


End Sub


