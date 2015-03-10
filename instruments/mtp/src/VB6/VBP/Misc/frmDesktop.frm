VERSION 5.00
Begin VB.Form frmDesktop 
   AutoRedraw      =   -1  'True
   BorderStyle     =   0  'None
   Caption         =   "Snap-It_D"
   ClientHeight    =   3930
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   5985
   DrawStyle       =   2  'Dot
   FillStyle       =   0  'Solid
   Icon            =   "frmDesktop.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form2"
   ScaleHeight     =   3930
   ScaleWidth      =   5985
   StartUpPosition =   3  'Windows Default
   WindowState     =   2  'Maximized
   Begin VB.PictureBox pic 
      AutoRedraw      =   -1  'True
      AutoSize        =   -1  'True
      Height          =   2505
      Left            =   120
      ScaleHeight     =   2445
      ScaleWidth      =   4785
      TabIndex        =   0
      Top             =   150
      Width           =   4845
      Begin VB.Shape Sh 
         BorderColor     =   &H000000FF&
         BorderWidth     =   3
         Height          =   885
         Left            =   660
         Top             =   630
         Visible         =   0   'False
         Width           =   2445
      End
   End
End
Attribute VB_Name = "frmDesktop"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Const SRCCOPY = &HCC0020
Const SRCAND = &H8800C6
Const SRCINVERT = &H660046

Private Type POINTAPI
    X As Long
    Y As Long
End Type

Private Type RECTL
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Declare Function SetForegroundWindow Lib "USER32" (ByVal hWnd As Long) As Long
Private Declare Function GetParent Lib "USER32" (ByVal hWnd As Long) As Long
Private Declare Function GetClientRect Lib "USER32" (ByVal hWnd As Long, lpRect As RECTL) As Long
Private Declare Function GetWindowRect Lib "USER32" (ByVal hWnd As Long, lpRect As RECTL) As Long
Private Declare Function LockWindowUpdate Lib "USER32" (ByVal hWndLock As Long) As Long
Private Declare Function WindowFromPoint Lib "user32.dll" (ByVal xPoint As Long, ByVal yPoint As Long) As Long
Private Declare Function GetCursorPos Lib "USER32" (lpPoint As POINTAPI) As Long
Private Declare Function GetDesktopWindow Lib "USER32" () As Long
Private Declare Function GetDC Lib "USER32" (ByVal hWnd As Long) As Long
Private Declare Function BitBlt Lib "GDI32" (ByVal hDestDC As Long, _
                                             ByVal X As Integer, _
                                             ByVal Y As Integer, _
                                             ByVal nWidth As Integer, _
                                             ByVal nHeight As Integer, _
                                             ByVal hSrcDC As Long, _
                                             ByVal XSrc As Integer, _
                                             ByVal YSrc As Integer, _
                                             ByVal dwRop As Long) As Integer

Dim mXY() As Long, m_iX As Long, m_iY As Long, m_XX As Long, m_iScreenW As Long, m_iScreenH As Long

Private Sub Form_Deactivate()
    Unload Me
End Sub

Private Sub Form_LostFocus()
    Unload Me
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = vbKeyEscape Then Unload Me
    If KeyCode = vbKeySpace Then
      g_bAutoName = False
      
    End If
End Sub

Private Sub Form_Load()
    Dim DeskhWnd As Long, DeskDC As Long, X As Long, Y As Long, iXm As Long, iYm As Long
    Me.WindowState = vbMaximized
    
    Sh.Visible = False
    With Me.pic
      .AutoRedraw = True
      .AutoSize = True
      .Top = 0
      .Left = 0
      .Width = Screen.Width
      .Height = Screen.Height
    End With
'    SetForegroundWindow hWnd
    'Get the hWnd of the desktop
    DeskhWnd& = GetDesktopWindow()
    'BitBlt needs the DC to copy the image. So, we need the GetDC API.
    DeskDC& = GetDC(DeskhWnd&)
    
    BitBlt Me.pic.hDC, 0&, 0&, Screen.Width, Screen.Height, DeskDC&, 0&, 0&, SRCCOPY
    Me.pic.Picture = Me.pic.Image
    
    m_iX = Screen.TwipsPerPixelX: m_iY = Screen.TwipsPerPixelY
    m_iScreenW = Screen.Width / m_iX: m_iScreenH = Screen.Height / m_iY
    
    m_XX = 4
    iXm = m_iX * m_XX: iYm = m_iY * m_XX
    ReDim mXY(Screen.Width / iXm - 1, Screen.Height / iYm - 1)
    For X = 0 To UBound(mXY, 1)
        For Y = 0 To UBound(mXY, 2)
            mXY(X, Y) = WindowFromPoint(X * m_XX, Y * m_XX)
        Next Y
    Next X
End Sub

Private Sub pic_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Static iX As Single, iY As Single, bSt As Boolean
    
    Dim CurPos As POINTAPI, rct As RECTL, iW As Long, iLeft As Long, iTop As Long, iBottom As Long, iRight As Long

    If (X = iX) And (Y = iY) Then Exit Sub
    iX = X: iY = Y

    GetCursorPos CurPos
    iW = mXY(Int(CurPos.X / m_XX), Int(CurPos.Y / m_XX))
    If g_iiW <> iW Then
        g_iiW = iW
        GetWindowRect iW, rct
        iLeft = rct.Left: iRight = rct.Right: iTop = rct.Top: iBottom = rct.Bottom
        iW = GetParent(iW)
        Do Until iW = 0
            GetWindowRect iW, rct
            If iTop < rct.Top And rct.Bottom > iTop _
                    And iBottom > rct.Bottom And rct.Top < iBottom _
                    And iLeft < rct.Left And rct.Right > iLeft _
                    And iRight > rct.Right And rct.Left < iRight Then
                
                If rct.Right - rct.Left >= m_iX And rct.Bottom - rct.Top >= m_iY Then
                    If iLeft < rct.Left Then iLeft = rct.Left
                    If iRight > rct.Right Then iRight = rct.Right
                    If iTop < rct.Top Then iTop = rct.Top
                    If iBottom > rct.Bottom Then iBottom = rct.Bottom
                    Exit Do
                End If
            End If

            iW = GetParent(iW)
        Loop
        If iLeft < 0 Then iLeft = 0
        If iTop < 0 Then iTop = 0
        If iRight > m_iScreenW Then iRight = m_iScreenW
        If iBottom > m_iScreenH Then iBottom = m_iScreenH
        
        Me!Sh.Move iLeft * m_iX, iTop * m_iY, (iRight - iLeft) * m_iX, (iBottom - iTop) * m_iY
    End If
    
    If bSt Then Exit Sub
    Sh.Visible = True: bSt = True

End Sub

Private Sub pic_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
  
  If m_frmCapture Is Nothing Then Set m_frmCapture = New frmCapture
  'Form1.Show
  'Form1.SetFocus
  DoEvents
  With m_frmCapture
    .AutoRedraw = True
    With .picSelImage
'      .Width = Sh.Width
'      .Height = Sh.Height
'      .PaintPicture Me.Pic.Image, 0, 0, Sh.Width, Sh.Height, Sh.Left, Sh.Top, Sh.Width, Sh.Height
      .Width = Form1.Width
      .Height = Form1.Height
      .Picture = CaptureForm(Form1)
'      .PaintPicture Form1.Image, 0, 0, Form1.Width, Form1.Height, Form1.Left, Form1.Top, Form1.Width, Form1.Height
      DoEvents
'      .Picture = m_frmCapture.picSelImage.Image
    End With
'    With .picFr
'      .Move .Left, .Top, IIf(m_iScaleWidth > ScaleWidth, m_iScaleWidth, ScaleWidth) - 2 * .Left, IIf(m_iScaleHeight > ScaleHeight, m_iScaleHeight, ScaleHeight) - Screen.TwipsPerPixelY * 76
'    End With
  End With
'    With m_frmCapture!Tb.Buttons
'        !Save.Enabled = True
'        !Copy.Enabled = True
'    End With
    Unload Me
    m_frmCapture.Visible = True
End Sub


