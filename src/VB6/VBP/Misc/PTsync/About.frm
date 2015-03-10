VERSION 5.00
Begin VB.Form frmAbout 
   Appearance      =   0  'Flat
   BorderStyle     =   1  'Fixed Single
   Caption         =   "About Power Tool"
   ClientHeight    =   5370
   ClientLeft      =   1185
   ClientTop       =   1485
   ClientWidth     =   7425
   ForeColor       =   &H80000008&
   Icon            =   "About.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   5370
   ScaleWidth      =   7425
   StartUpPosition =   1  'CenterOwner
   Visible         =   0   'False
   Begin VB.PictureBox picAccess 
      Appearance      =   0  'Flat
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      HasDC           =   0   'False
      Height          =   240
      Left            =   7110
      Picture         =   "About.frx":212A
      ScaleHeight     =   240
      ScaleWidth      =   240
      TabIndex        =   1
      Tag             =   "ACCESS"
      Top             =   0
      Width           =   240
   End
   Begin VB.TextBox txtComments 
      BackColor       =   &H8000000F&
      BorderStyle     =   0  'None
      Height          =   1215
      Left            =   240
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   5
      Top             =   1440
      Width           =   6975
   End
   Begin VB.CommandButton btnOK 
      Appearance      =   0  'Flat
      Cancel          =   -1  'True
      Caption         =   "&Close"
      Default         =   -1  'True
      Height          =   375
      Left            =   6240
      TabIndex        =   0
      Top             =   4800
      Width           =   975
   End
   Begin VB.Label lblAccess 
      BackStyle       =   0  'Transparent
      Caption         =   "&Z"
      ForeColor       =   &H8000000F&
      Height          =   255
      Left            =   6840
      TabIndex        =   13
      ToolTipText     =   "12"
      Top             =   0
      Width           =   255
   End
   Begin VB.Image imgLogo 
      Height          =   960
      Left            =   240
      Picture         =   "About.frx":26B4
      Stretch         =   -1  'True
      Top             =   240
      Width           =   960
   End
   Begin VB.Label lblSep 
      BackColor       =   &H8000000D&
      BorderStyle     =   1  'Fixed Single
      Height          =   75
      Left            =   180
      TabIndex        =   6
      Top             =   2760
      Width           =   7095
   End
   Begin VB.Label lblVersion 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Version X.X"
      ForeColor       =   &H80000008&
      Height          =   195
      Left            =   1440
      TabIndex        =   3
      Top             =   600
      Width           =   825
   End
   Begin VB.Label lblCDLink 
      BackStyle       =   0  'Transparent
      Caption         =   "http://www.karenware.com/cd.asp"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   -1  'True
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000D&
      Height          =   255
      Left            =   240
      MouseIcon       =   "About.frx":2C83
      MousePointer    =   99  'Custom
      TabIndex        =   8
      ToolTipText     =   "Get Karen's Power Tools on CD!"
      Top             =   3600
      Width           =   3255
   End
   Begin VB.Label lblCDInfo 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   $"About.frx":2DD5
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   240
      TabIndex        =   7
      Top             =   3000
      Width           =   6975
   End
   Begin VB.Label lblSubLink 
      BackStyle       =   0  'Transparent
      Caption         =   "http://www.karenware.com/subscribe/"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   -1  'True
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000D&
      Height          =   255
      Left            =   240
      MouseIcon       =   "About.frx":2EA8
      MousePointer    =   99  'Custom
      TabIndex        =   10
      ToolTipText     =   "Subscribe to Karen's FREE newsletter"
      Top             =   4200
      Width           =   5175
   End
   Begin VB.Label lblSubInfo 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Subscribe to Karen's FREE newsletter, and be the first to know of new and upgraded Power Tools: "
      ForeColor       =   &H80000008&
      Height          =   495
      Left            =   240
      TabIndex        =   9
      Top             =   3960
      Width           =   6975
   End
   Begin VB.Label lblHomeLink 
      BackStyle       =   0  'Transparent
      Caption         =   "http://www.karenware.com/powertools/"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   -1  'True
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000D&
      Height          =   255
      Left            =   240
      MouseIcon       =   "About.frx":2FFA
      MousePointer    =   99  'Custom
      TabIndex        =   12
      ToolTipText     =   "Visit Karen's home page"
      Top             =   4920
      Width           =   4215
   End
   Begin VB.Label lblHomeInfo 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "For more information, and the latest version, visit Karen's Power Tools home page: "
      ForeColor       =   &H80000008&
      Height          =   495
      Left            =   240
      TabIndex        =   11
      Top             =   4680
      Width           =   5775
   End
   Begin VB.Label lblCopyright 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Copyright (c) 1993, 1994, 1999 Karen Kenworthy All Rights Reserved"
      ForeColor       =   &H80000008&
      Height          =   195
      Left            =   1440
      TabIndex        =   4
      Top             =   840
      Width           =   4890
   End
   Begin VB.Label lblTitle 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Power Tool Name"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   18
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000D&
      Height          =   435
      Left            =   1440
      TabIndex        =   2
      Top             =   165
      Width           =   3195
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Copyright © 2002-2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 2.9 8/10/2004

Private Const SW_NORMAL = 1
Private Const SW_SHOW = 5
Private Const SHADOW_OFFSET = 30

Private Const ABOUT_APPID = "About"

Private Declare Function FindExecutableA Lib "shell32.dll" ( _
    ByVal lpFile As String, _
    ByVal lpDirectory As String, _
    ByVal lpResult As String) As Long

Private Declare Function ShellExecuteA Lib "shell32.dll" ( _
    ByVal hWnd As Long, _
    ByVal lpOperation As String, _
    ByVal lpFile As String, _
    ByVal lpParameters As String, _
    ByVal lpDirectory As String, _
    ByVal nShowCmd As Long) As Long

Private WithEvents PTMe As PTForm
Attribute PTMe.VB_VarHelpID = -1

Private CDInfo As String
Private cdLink As String
Private SubInfo As String
Private SubLink As String
Private HomeInfo As String
Private HomeLink As String
Private Sub btnOK_Click()
    Unload Me
End Sub
Private Sub Form_Load()
    Dim ver As String
    Dim s As String
    Dim i As Long
'    Dim Reg As Registry

    CDInfo = Trim(lblCDInfo.Caption) & "  "
    cdLink = Trim(lblCDLink.Caption)
    SubInfo = Trim(lblSubInfo.Caption) & "  "
    SubLink = Trim(lblSubLink.Caption)
    HomeInfo = Trim(lblHomeInfo.Caption) & "  "
    HomeLink = Trim(lblHomeLink.Caption)

    lblTitle.Caption = App.FileDescription & " "

    ver = App.Major & "." & App.Minor
    If App.Revision <> 0 Then
        ver = ver & "." & App.Revision
    End If

    lblVersion.Caption = "Version " & ver
    lblVersion.Top = lblTitle.Top + lblTitle.Height

    s = Replace(App.LegalCopyright, "\n", vbCrLf, , , vbTextCompare)
    lblCopyright = Replace(s, "(c)", "©")
    lblCopyright.Top = lblVersion.Top + lblVersion.Height

    txtComments.Text = Replace(App.Comments, "\n", vbCrLf, , , vbTextCompare)

    Me.Caption = "About " & App.FileDescription

    LayoutAbout

    Set PTMe = New PTForm
    PTMe.Constructor Me
    PTMe.SetFonts

    PTMe.Ready = True
'    Set Reg = New Registry
'    Reg.GetFormSize Me, ABOUT_APPID

    LayoutAbout
'    PTMe.NoResize = True
End Sub
Private Sub PTMe_AfterResize()
    LayoutAbout
    DoEvents
    PTMe.ReInit
End Sub
Private Sub LayoutAbout()
    Dim taInfo As PT_TEXT_ARRAY
    Dim xySize As PT_TEXT_SIZE
    Dim DiffVert As Single
    Dim DiffHorz As Single

    Dim NewCdInfoTop As Single
    Dim NewCdInfoHeight As Single
    Dim NewCdInfoWidth As Single
    Dim NewCdInfoCaption As String
    Dim NewCdLinkTop As Single
    Dim NewCdLinkLeft As Single
    Dim NewCdLinkHeight As Single
    Dim NewCdLinkWidth As Single

    Dim NewSubInfoTop As Single
    Dim NewSubInfoHeight As Single
    Dim NewSubInfoWidth As Single
    Dim NewSubInfoCaption As String
    Dim NewSubLinkTop As Single
    Dim NewSubLinkLeft As Single
    Dim NewSubLinkHeight As Single
    Dim NewSubLinkWidth As Single

    Dim NewHomeInfoTop As Single
    Dim NewHomeInfoHeight As Single
    Dim NewHomeInfoWidth As Single
    Dim NewHomeInfoCaption As String
    Dim NewHomeLinkTop As Single
    Dim NewHomeLinkLeft As Single
    Dim NewHomeLinkHeight As Single
    Dim NewHomeLinkWidth As Single

    Dim NewWidth As Single
    Dim BottomMargin As Single
    Dim RightMargin As Single
    Dim LeftMargin As Single
    Dim CurBase As Single

    BottomMargin = Me.Height - (btnOK.Top + btnOK.Height)
    LeftMargin = txtComments.Left
    RightMargin = Me.Width - (btnOK.Left + btnOK.Width)
    NewWidth = Me.Width - (LeftMargin + RightMargin)
    NewWidth = txtComments.Width

    DiffVert = Me.Height - (btnOK.Top + btnOK.Height)
    DiffVert = DiffVert - BottomMargin

    DiffHorz = Me.Width - (btnOK.Left + btnOK.Width)
    DiffHorz = DiffHorz - RightMargin
    DoEvents

'    If (DiffVert = 0) And (DiffHorz = 0) Then Exit Sub
    CurBase = btnOK.Top + btnOK.Height

    NewHomeInfoWidth = NewWidth - (btnOK.Width + RightMargin)
    taInfo = ApiTextWrap(Me.hdc, NewHomeInfoWidth, HomeInfo)
    NewHomeInfoCaption = taInfo.WrappedText
    NewHomeInfoHeight = taInfo.Overall.HeightTwips
    NewHomeInfoTop = CurBase - NewHomeInfoHeight
    xySize = ApiTextSize(Me.hdc, HomeLink)
    NewHomeLinkHeight = xySize.HeightTwips
    NewHomeLinkWidth = xySize.WidthTwips
    If (taInfo.LastLine.WidthTwips + NewHomeLinkWidth) > NewHomeInfoWidth Then
        NewHomeInfoTop = NewHomeInfoTop - NewHomeLinkHeight
        NewHomeLinkTop = CurBase - NewHomeLinkHeight
        NewHomeLinkLeft = LeftMargin
    Else
        NewHomeLinkTop = (NewHomeInfoTop + NewHomeInfoHeight) - taInfo.LastLine.HeightTwips
        NewHomeLinkLeft = LeftMargin + taInfo.LastLine.WidthTwips
    End If
    DoEvents

    CurBase = NewHomeInfoTop - (NewHomeLinkHeight * 1.5)

    NewSubInfoWidth = NewWidth
    taInfo = ApiTextWrap(Me.hdc, NewSubInfoWidth, SubInfo)
    NewSubInfoCaption = taInfo.WrappedText
    NewSubInfoHeight = taInfo.Overall.HeightTwips
    NewSubInfoTop = CurBase - NewSubInfoHeight
    xySize = ApiTextSize(Me.hdc, SubLink)
    NewSubLinkHeight = xySize.HeightTwips
    NewSubLinkWidth = xySize.WidthTwips
    If (taInfo.LastLine.WidthTwips + NewSubLinkWidth) > NewSubInfoWidth Then
        NewSubInfoTop = NewSubInfoTop - NewSubLinkHeight
        NewSubLinkTop = CurBase - NewSubLinkHeight
        NewSubLinkLeft = LeftMargin
    Else
        NewSubLinkTop = (NewSubInfoTop + NewSubInfoHeight) - taInfo.LastLine.HeightTwips
        NewSubLinkLeft = LeftMargin + taInfo.LastLine.WidthTwips
    End If
    DoEvents

    CurBase = NewSubInfoTop - (NewSubLinkHeight * 1.5)

    NewCdInfoWidth = NewWidth
    taInfo = ApiTextWrap(Me.hdc, NewCdInfoWidth, CDInfo)
    NewCdInfoCaption = taInfo.WrappedText
    NewCdInfoHeight = taInfo.Overall.HeightTwips
    NewCdInfoTop = CurBase - NewCdInfoHeight
    xySize = ApiTextSize(Me.hdc, cdLink)
    NewCdLinkHeight = xySize.HeightTwips
    NewCdLinkWidth = xySize.WidthTwips
    If (taInfo.LastLine.WidthTwips + NewCdLinkWidth) > NewCdInfoWidth Then
        NewCdInfoTop = NewCdInfoTop - NewCdLinkHeight
        NewCdLinkTop = CurBase - NewCdLinkHeight
        NewCdLinkLeft = LeftMargin
    Else
        NewCdLinkTop = (NewCdInfoTop + NewCdInfoHeight) - taInfo.LastLine.HeightTwips
        NewCdLinkLeft = LeftMargin + taInfo.LastLine.WidthTwips
    End If
    DoEvents

    CurBase = NewCdInfoTop - (NewCdLinkHeight * 0.5)

    lblSep.Top = CurBase - lblSep.Height
    lblSep.Width = NewWidth

    CurBase = lblSep.Top - (NewCdLinkHeight * 0.5)

    txtComments.Height = CurBase - txtComments.Top
    txtComments.Width = NewWidth

    lblHomeInfo.Top = NewHomeInfoTop
    lblHomeInfo.Height = NewHomeInfoHeight
    lblHomeInfo.Width = NewHomeInfoWidth
    lblHomeInfo.Caption = NewHomeInfoCaption

    lblHomeLink.Top = NewHomeLinkTop
    lblHomeLink.Left = NewHomeLinkLeft
    lblHomeLink.Height = NewHomeLinkHeight
    lblHomeLink.Width = NewHomeLinkWidth
    lblHomeLink.ZOrder

    lblSubInfo.Top = NewSubInfoTop
    lblSubInfo.Height = NewSubInfoHeight
    lblSubInfo.Width = NewSubInfoWidth
    lblSubInfo.Caption = NewSubInfoCaption

    lblSubLink.Top = NewSubLinkTop
    lblSubLink.Left = NewSubLinkLeft
    lblSubLink.Height = NewSubLinkHeight
    lblSubLink.Width = NewSubLinkWidth
    lblSubLink.ZOrder

    lblCDInfo.Top = NewCdInfoTop
    lblCDInfo.Height = NewCdInfoHeight
    lblCDInfo.Width = NewCdInfoWidth
    lblCDInfo.Caption = NewCdInfoCaption

    lblCDLink.Top = NewCdLinkTop
    lblCDLink.Left = NewCdLinkLeft
    lblCDLink.Height = NewCdLinkHeight
    lblCDLink.Width = NewCdLinkWidth
    lblCDLink.ZOrder
End Sub
Private Sub Form_Unload(Cancel As Integer)
    Dim Reg As Registry

    Set Reg = New Registry
'    Reg.SaveFormSize Me, ABOUT_APPID
    PTMe.Unload
    Set PTMe = Nothing
End Sub
Private Sub lblCDLink_Click()
    ShellExecuteA Me.hWnd, "open", lblCDLink.Caption, "", "", SW_SHOW Or SW_NORMAL
End Sub
Private Sub lblHomeLink_Click()
    ShellExecuteA Me.hWnd, "open", lblHomeLink.Caption, "", "", SW_SHOW Or SW_NORMAL
End Sub
Private Sub lblSubLink_Click()
    ShellExecuteA Me.hWnd, "open", lblSubLink.Caption, "", "", SW_SHOW Or SW_NORMAL
End Sub
