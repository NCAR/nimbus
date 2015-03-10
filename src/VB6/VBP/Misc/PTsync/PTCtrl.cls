VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PTCtrl"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 1.2 8/8/2004

Private Const SSTAB_HIDE = 75000
Private Const SSTAB_THRESHOLD = -5000

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Type POINTAPI
    X As Long
    Y As Long
End Type

Private Type WINDOWINFO
    cbSize As Long
    rcWindow As RECT
    rcClient As RECT
    dwStyle As Long
    dwExStyle As Long
    dwWindowStatus As Long
    cxWindowBorders As Integer ' UINT
    cyWindowBorders As Integer ' UINT
    atomWindowType As Long ' ATOM
    wCreatorVersion As Integer ' WORD
End Type

Private Declare Function ScreenToClient Lib "user32" ( _
    ByVal hWnd As Long, _
    lpPoint As POINTAPI) As Long

Private Declare Function GetWindowInfo Lib "user32.dll" ( _
    ByVal hWnd As Long, _
    ByRef pwi As WINDOWINFO) As Long

Private Declare Function OSDrawFocusRect Lib "user32" Alias "DrawFocusRect" ( _
    ByVal hdc As Long, _
    ByRef lpRect As RECT) As Long

'local variable(s) to hold property value(s)
Private mvarForm As PTForm
Private mvarID As Long
Private mvarName As String
Private mvarIndex As Long

Private WithEvents picCtrl As PictureBox
Attribute picCtrl.VB_VarHelpID = -1
Private mvarHasFocusRect As Boolean
Private mvarNeedsFocusRect As Boolean
Private mvarCtrl As Control
Private mvarContainer As Object
Private mvarTypeName As String
Private mvarIsStatusBar As Boolean
Private mvarIsImage As Boolean
Private mvarIsPictureBox As Boolean
Private mvarIsCommandButton As Boolean
Private mvarIsAccessCtrl As Boolean
Private mvarHasCaption As Boolean
Private mvarHasText As Boolean
Private mvarHasToolTipText As Boolean
Private mvarHasPicture As Boolean
Private mvarUseFontObject As Boolean
Private mvarChildren As Long
Private mvarDontResize As Boolean

Private mvarIgnoreTop As Boolean
Private mvarIgnoreLeft As Boolean
Private mvarIgnoreHeight As Boolean
Private mvarIgnoreWidth As Boolean
Private mvarIgnoreTabHeight As Boolean
Private mvarIgnoreFontInfo As Boolean

Private mvarOrigTop As Single
Private mvarOrigLeft As Single
Private mvarOrigHeight As Single
Private mvarOrigWidth As Single
Private mvarOrigTabHeight As Single
Private mvarOrigFontSize As Single

Private mvarTop As Single
'Private mvarLeft As Single ' don't shadow left if using SSTab
Private mvarHeight As Single
Private mvarWidth As Single
Private mvarTabHeight As Single
Private mvarFontSize As Single
Private mvarFontName As String

Public Event Click()
Public Event GotFocus()
Public Event LostFocus()
Public Event KeyDown(KeyCode As Integer, Shift As Integer)
Public Event KeyPress(KeyAscii As Integer)
Public Event KeyUp(KeyCode As Integer, Shift As Integer)
Public Event MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
Public Event MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Public Event MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)

Public Sub Save(SubKey As String)

End Sub
Public Sub Restore(SubKey As String)

End Sub
Public Property Get ID() As Long
    ID = mvarID
End Property
Public Property Let ID(ByVal NewValue As Long)
    mvarID = NewValue
End Property
Public Property Let Enabled(ByVal NewValue As Boolean)
    mvarCtrl.Enabled = NewValue
End Property
Public Property Get Enabled() As Boolean
    Enabled = mvarCtrl.Enabled
End Property
Public Property Let Visible(ByVal NewValue As Boolean)
    mvarCtrl.Visible = NewValue
End Property
Public Property Get Visible() As Boolean
    Visible = mvarCtrl.Visible
End Property
Public Property Get IsStatusBar() As Boolean
    IsStatusBar = mvarIsStatusBar
End Property
Public Property Get IsCommandButton() As Boolean
    IsCommandButton = mvarIsCommandButton
End Property
Public Property Get IsImage() As Boolean
    IsImage = mvarIsImage
End Property
Public Property Get IsPictureBox() As Boolean
    IsPictureBox = mvarIsPictureBox
End Property
Public Property Get IsAccessCtrl() As Boolean
    IsAccessCtrl = mvarIsAccessCtrl
End Property
Public Property Get IsAccessButton() As Boolean
    IsAccessButton = mvarIsCommandButton And mvarIsAccessCtrl
End Property
Public Property Get IsAccessImage() As Boolean
    IsAccessImage = mvarIsImage And mvarIsAccessCtrl
End Property
Public Property Get IsAccessPictureBox() As Boolean
    IsAccessPictureBox = mvarIsPictureBox And mvarIsAccessCtrl
End Property
Public Property Get Resizable() As Boolean
    Resizable = Not (mvarIgnoreTop And mvarIgnoreLeft And mvarIgnoreHeight And mvarIgnoreWidth And mvarIgnoreFontInfo And mvarIgnoreTabHeight)
End Property
Public Property Let Height(ByVal NewValue As Single)
    If Not mvarIgnoreHeight Then
        mvarHeight = NewValue
        On Error Resume Next
        mvarCtrl.Height = mvarHeight
        If Err.Number <> 0 Then
            mvarIgnoreHeight = True
            Err.Clear
        End If
    End If
End Property
Public Property Get Height() As Single
    Height = mvarHeight
End Property
Public Property Let Width(ByVal NewValue As Single)
    If Not mvarIgnoreWidth Then
        mvarWidth = NewValue
        On Error Resume Next
        mvarCtrl.Width = mvarWidth
        If Err.Number <> 0 Then
            mvarIgnoreWidth = True
            Err.Clear
        End If
    End If
End Property
Public Property Get Width() As Single
    Width = mvarWidth
End Property
Public Property Let Left(ByVal NewValue As Single)
    If Not mvarIgnoreLeft Then
'        mvarLeft = NewValue
        On Error Resume Next
        mvarCtrl.Left = NewValue
        If Err.Number <> 0 Then
            mvarIgnoreLeft = True
            Err.Clear
        End If
    End If
End Property
Public Property Get Left() As Single
'    Left = mvarLeft
    Left = mvarCtrl.Left
End Property
Public Property Let Top(ByVal NewValue As Single)
    If Not mvarIgnoreTop Then
        mvarTop = NewValue
        On Error Resume Next
        mvarCtrl.Top = mvarTop
        If Err.Number <> 0 Then
            mvarIgnoreTop = True
            Err.Clear
        End If
    End If
End Property
Public Property Get Top() As Single
    Top = mvarTop
End Property
Private Sub FixPanels()
    Dim Pnls As Panels
    Dim Pnl As Panel
    Dim i As Long
    Dim s As String

    If mvarIsStatusBar Then
        ' fix StatusBar Date and Time panels - they don't autosize correctly when font size changed
        Set Pnls = mvarCtrl.Panels
        For i = 1 To Pnls.Count
            Set Pnl = Pnls(i)
            If Pnl.Style = sbrDate Then
                Pnl.Style = sbrText
                Pnl.Style = sbrDate
                Pnl.AutoSize = sbrContents
            ElseIf Ctrl.Panels(i).Style = sbrTime Then
                Pnl.Style = sbrText
                Pnl.Style = sbrTime
                Pnl.AutoSize = sbrContents
            ElseIf (Pnl.Style = sbrText) And (Pnl.AutoSize = sbrContents) Then
                s = Pnl.Text
                Pnl.Text = ""
                Pnl.Text = s
            End If
        Next i
    End If
End Sub
Public Property Let FontSize(ByVal NewValue As Single)
    If Not mvarIgnoreFontInfo Then
        mvarFontSize = NewValue
        If mvarUseFontObject Then
            mvarCtrl.Font.Size = mvarFontSize
        Else
            mvarCtrl.FontSize = mvarFontSize
        End If
        Err.Clear
    End If

    If mvarIsStatusBar Then FixPanels
End Property
Public Property Get FontSize() As Single
    FontSize = mvarFontSize
End Property
Public Property Get FontName() As String
    FontName = mvarFontName
End Property
Public Property Let FontName(ByVal NewValue As String)
    If Not mvarIgnoreFontInfo Then
        mvarFontName = NewValue
        If mvarUseFontObject Then
            mvarCtrl.Font.Name = mvarFontName
        Else
            mvarCtrl.FontName = mvarFontName
        End If
    End If

    If mvarIsStatusBar Then FixPanels
End Property
Public Property Let TabHeight(ByVal NewValue As Single)
    If Not mvarIgnoreTabHeight Then
        mvarTabHeight = NewValue
        mvarCtrl.TabHeight = mvarTabHeight
    End If
End Property
Public Property Get TabHeight() As Single
    TabHeight = mvarTabHeight
End Property
Public Property Get Text() As String
    If mvarHasText Then
        Text = mvarCtrl.Text
    ElseIf mvarHasCaption Then
        Text = mvarCtrl.Caption
    Else
        Text = ""
    End If
End Property
Public Property Let Text(ByVal NewValue As String)
    If mvarHasText Then
        mvarCtrl.Text = NewValue
    ElseIf mvarHasCaption Then
        mvarCtrl.Caption = NewValue
    End If
End Property
Public Property Get ToolTipText() As String
    If mvarHasToolTipText Then
        ToolTipText = mvarCtrl.ToolTipText
    Else
        ToolTipText = ""
    End If
End Property
Public Property Let ToolTipText(ByVal NewValue As String)
    If mvarHasToolTipText Then
        mvarCtrl.ToolTipText = NewValue
    End If
End Property
Public Sub Constructor(Frm As PTForm, Ctrl As Control, Optional ID As Long = -1)
    Dim i As Long
    Dim s As String
    Dim pic As StdPicture

    Set mvarForm = Frm
    Set mvarCtrl = Ctrl

    mvarID = ID
    mvarName = mvarCtrl.Name

    On Error Resume Next
    If mvarCtrl.Container Is mvarForm.Form Then
        Set mvarContainer = mvarForm.Form
    Else
        Set mvarContainer = mvarCtrl.Container
    End If
    If Err.Number <> 0 Then ' no container, i.e. Timer
        Set mvarContainer = mvarForm.Form
        Err.Clear
    End If

    If VarType(mvarCtrl) And vbArray Then
        mvarIndex = mvarCtrl.Index
    Else
        mvarIndex = -1
    End If

    s = mvarCtrl.Text
    If Err.Number = 0 Then
        mvarHasText = True
    Else
        Err.Clear
        s = mvarCtrl.Caption
        If Err.Number = 0 Then
            mvarHasCaption = True
        Else
            Err.Clear
        End If
    End If

    s = mvarCtrl.ToolTipText
    If Err.Number = 0 Then
        mvarHasToolTipText = True
    Else
        Err.Clear
        mvarHasToolTipText = False
    End If

    Set pic = mvarCtrl.Picture
    If Err.Number = 0 Then
        mvarHasPicture = True
    Else
        Err.Clear
        mvarHasPicture = False
    End If

    Init
End Sub
Private Sub Init()

    On Error Resume Next
    mvarOrigTop = mvarCtrl.Top
    If Err.Number <> 0 Then
        mvarIgnoreTop = True
        Err.Clear
    End If

    mvarOrigHeight = mvarCtrl.Height
    If Err.Number <> 0 Then
        mvarIgnoreHeight = True
        Err.Clear
    End If

    mvarOrigWidth = mvarCtrl.Width
    If Err.Number <> 0 Then
        mvarIgnoreWidth = True
        Err.Clear
    End If

    mvarOrigLeft = mvarCtrl.Left
    If Err.Number <> 0 Then
        mvarIgnoreLeft = True
        Err.Clear
    End If
    If Not mvarIgnoreLeft Then
        If mvarOrigLeft < SSTAB_THRESHOLD Then
            mvarOrigLeft = mvarCtrl.Left + SSTAB_HIDE
        End If
    End If

    mvarOrigFontSize = mvarCtrl.Font.Size
    If Err.Number = 0 Then
        mvarUseFontObject = True
    Else
        Err.Clear
        mvarOrigFontSize = mvarCtrl.FontSize
        If Err.Number <> 0 Then
            mvarIgnoreFontInfo = True
            Err.Clear
        End If
    End If

    If Not mvarIgnoreFontInfo Then
        If mvarUseFontObject Then
            mvarFontName = mvarCtrl.Font.Name
        Else
            mvarFontName = mvarCtrl.FontName
        End If
        If Err.Number <> 0 Then
            mvarIgnoreFontInfo = True
            Err.Clear
        End If
    End If

    mvarIgnoreTabHeight = True

    mvarTypeName = VBA.TypeName(mvarCtrl)

    Select Case mvarTypeName
        Case "Label":
            If mvarCtrl.AutoSize Then
                mvarIgnoreHeight = True
                mvarIgnoreWidth = True
            End If

        Case "PictureBox":
            mvarIsPictureBox = True
            If StrComp(mvarCtrl.Tag, "ACCESS", vbTextCompare) = 0 Then
                mvarIsAccessCtrl = True
                mvarIgnoreWidth = True
                mvarIgnoreHeight = True
            End If
            Set picCtrl = mvarCtrl

        Case "Image":
            mvarIsImage = True
            If StrComp(mvarCtrl.Tag, "ACCESS", vbTextCompare) = 0 Then
                mvarIsAccessCtrl = True
            End If

        Case "CommandButton":
            mvarIsCommandButton = True
            If StrComp(mvarCtrl.Tag, "ACCESS", vbTextCompare) = 0 Then
                mvarIsAccessCtrl = True
            End If

        Case "Timer", "WinSock", "ImageList", "CommonDialog", "Menu":
            mvarIgnoreTop = True
            mvarIgnoreLeft = True
            mvarIgnoreHeight = True
            mvarIgnoreWidth = True
            mvarIgnoreFontInfo = True

        Case "StatusBar":
            mvarIsStatusBar = True
            mvarIgnoreTop = True
            mvarIgnoreLeft = True
            Select Case mvarCtrl.Align
                Case vbAlignBottom, vbAlignTop:
                    mvarIgnoreWidth = True
                Case vbAlignLeft, vbAlignLeft:
                    mvarIgnoreHeight = True
            End Select

        Case "SSTab":
            mvarOrigTabHeight = mvarCtrl.TabHeight
            mvarIgnoreTabHeight = False

        Case "ComboBox":
            mvarIgnoreHeight = True

    End Select

    mvarTop = mvarOrigTop
'    mvarLeft = mvarOrigLeft
    mvarHeight = mvarOrigHeight
    mvarWidth = mvarOrigWidth
    mvarFontSize = mvarOrigFontSize
    mvarTabHeight = mvarOrigTabHeight

    mvarChildren = 0
End Sub
Public Sub ReInit(Optional ScaleHorz As Single = -1, Optional ScaleVert As Single = -1, Optional ScaleFont As Single = -1)
    Dim mvarLeft As Single

    If ScaleHorz < 0 Then ScaleHorz = mvarForm.ScaleHorz
    If ScaleVert < 0 Then ScaleVert = mvarForm.ScaleVert
    If ScaleFont < 0 Then ScaleFont = mvarForm.ScaleFont

    If Not mvarIgnoreFontInfo Then
        If mvarUseFontObject Then
            mvarFontSize = mvarCtrl.Font.Size
            mvarFontName = mvarCtrl.Font.Name
        Else
            mvarFontSize = mvarCtrl.FontSize
            mvarFontName = mvarCtrl.FontName
        End If
        If Err.Number = 0 Then
            mvarOrigFontSize = mvarFontSize / ScaleFont
        Else
            mvarIgnoreFontInfo = True
            Err.Clear
        End If
    End If

    If Not mvarIgnoreHeight Then
        mvarHeight = mvarCtrl.Height
        mvarOrigHeight = mvarHeight / ScaleVert
    End If
    If Not mvarIgnoreTop Then
        mvarTop = mvarCtrl.Top
        mvarOrigTop = mvarTop / ScaleVert
    End If
    If Not mvarIgnoreWidth Then
        mvarWidth = mvarCtrl.Width
        mvarOrigWidth = mvarWidth / ScaleHorz
    End If
    If Not mvarIgnoreTabHeight Then
        mvarTabHeight = mvarCtrl.TabHeight
        mvarOrigTabHeight = mvarTabHeight / ScaleVert
    End If

    If Not mvarIgnoreLeft Then
        mvarLeft = mvarCtrl.Left
        If mvarLeft < SSTAB_THRESHOLD Then
            mvarLeft = mvarLeft + SSTAB_HIDE
        End If
        mvarOrigLeft = mvarLeft / ScaleHorz
    End If
End Sub
Public Property Get HasFocusRect() As Boolean
    HasFocusRect = mvarHasFocusRect
End Property
Public Property Get NeedsFocusRect() As Boolean
    NeedsFocusRect = mvarNeedsFocusRect
End Property
Public Sub EraseFocusRect()
    If mvarHasFocusRect Then
        mvarForm.Refresh
        DoEvents
    End If
'    If mvarHasFocusRect Then DrawFocusRect
Debug.Print "Erase: " & mvarHasFocusRect
End Sub
Public Sub DrawFocusRect()
    Dim wi As WINDOWINFO
    Dim rc As RECT
    Dim pt As POINTAPI
    Dim result As Long
    Dim SaveScaleMode As ScaleModeConstants

    Me.EraseFocusRect

    wi.cbSize = Len(wi)
    GetWindowInfo mvarCtrl.hWnd, wi
    pt.X = wi.rcClient.Left
    pt.Y = wi.rcClient.Top
    ScreenToClient mvarForm.hWnd, pt
    rc.Left = pt.X - 1
    rc.Top = pt.Y - 1
    pt.X = wi.rcClient.Right
    pt.Y = wi.rcClient.Bottom
    ScreenToClient mvarForm.hWnd, pt
    rc.Right = pt.X + 1
    rc.Bottom = pt.Y + 1

    SaveScaleMode = mvarForm.ScaleMode
    mvarForm.ScaleMode = 3

    result = OSDrawFocusRect(mvarForm.hdc, rc)
    mvarForm.ScaleMode = SaveScaleMode
    mvarHasFocusRect = True
Debug.Print "Draw: " & mvarHasFocusRect
    DoEvents
End Sub
Public Sub Resize(ScaleHorz As Single, ScaleVert As Single, ScaleFont As Single)
    Dim i As Long
    Dim s As String
    Dim Pnls As Panels
    Dim Pnl As Panel
    Dim mvarLeft As Single

    If mvarHasFocusRect Then
        mvarNeedsFocusRect = True
        DrawFocusRect
    End If

    On Error Resume Next
    If Not mvarIgnoreFontInfo Then
        mvarFontSize = mvarOrigFontSize * ScaleFont
        If mvarUseFontObject Then
            mvarCtrl.Font.Size = mvarFontSize
        Else
            mvarCtrl.FontSize = mvarFontSize
        End If
        If Err.Number <> 0 Then
            mvarIgnoreFontInfo = True
            Err.Clear
        End If
    End If

    If Not mvarIgnoreLeft Then
        mvarLeft = mvarCtrl.Left
        If mvarLeft < SSTAB_THRESHOLD Then
            mvarLeft = (mvarOrigLeft * ScaleHorz) - SSTAB_HIDE
        Else
            mvarLeft = mvarOrigLeft * ScaleHorz
        End If
        If mvarIsAccessCtrl Then
            If mvarIsPictureBox Or mvarIsImage Or mvarIsCommandButton Then
                mvarLeft = mvarForm.ScaleWidth - mvarOrigWidth - 60
            End If
        End If
        mvarCtrl.Left = mvarLeft
        If Err.Number <> 0 Then
            mvarIgnoreLeft = True
            Err.Clear
        End If
    End If

    If Not mvarIgnoreTop Then
        mvarTop = mvarOrigTop * ScaleVert
        If mvarIsAccessCtrl Then
            If mvarIsPictureBox Or mvarIsImage Or mvarIsCommandButton Then mvarTop = 30
        End If
        mvarCtrl.Top = mvarTop
        If Err.Number <> 0 Then
            mvarIgnoreTop = True
            Err.Clear
        End If
'If Abs(mvarTop - mvarCtrl.Top) > (mvarTop * 0.1) Then
'    Debug.Print "Top OOB: " & mvarName & " (" & FormatPercent(mvarTop / mvarCtrl.Top, 0) & ")"
'Else
''    Debug.Print "Top IB: " & mvarName & " (" & FormatPercent(mvarTop / mvarCtrl.Top, 0) & ")"
'End If
    End If

    If Not mvarIgnoreWidth Then
        mvarWidth = mvarOrigWidth * ScaleHorz
        mvarCtrl.Width = mvarWidth
        If Err.Number <> 0 Then
            mvarIgnoreWidth = True
            Err.Clear
        End If
    End If

    If Not mvarIgnoreHeight Then
        mvarHeight = (mvarOrigHeight * ScaleVert)
        mvarCtrl.Height = mvarHeight
        If Err.Number <> 0 Then
            mvarIgnoreHeight = True
            Err.Clear
        End If
'If Abs(mvarHeight - mvarCtrl.Height) > (mvarHeight * 0.1) Then
'    Debug.Print "Height OOB: " & mvarName & " (" & FormatPercent(mvarHeight / mvarCtrl.Height, 0) & ")"
'Else
''    Debug.Print "Height IB: " & mvarName & " (" & FormatPercent(mvarHeight / mvarCtrl.Height, 0) & ")"
'End If
    End If

    If Not mvarIgnoreTabHeight Then
        mvarTabHeight = mvarOrigTabHeight * ScaleVert
        mvarCtrl.TabHeight = mvarTabHeight
        If Err.Number <> 0 Then
            mvarIgnoreTabHeight = True
            Err.Clear
        End If
    End If

    If mvarIsStatusBar Then FixPanels

'    If NeedsFocusRect Then DrawFocusRect
End Sub
Public Property Get OrigTabHeight() As Single
    OrigTabHeight = mvarOrigTabHeight
End Property
Public Property Get OrigFontSize() As Single
    OrigFontSize = mvarOrigFontSize
End Property
Public Property Get OrigWidth() As Single
    OrigWidth = mvarOrigWidth
End Property
Public Property Get OrigHeight() As Single
    OrigHeight = mvarOrigHeight
End Property
Public Property Get OrigTop() As Single
    OrigTop = mvarOrigTop
End Property
Public Property Get OrigLeft() As Single
    OrigLeft = mvarOrigLeft
End Property
Public Property Get TypeName() As String
    TypeName = mvarTypeName
End Property
Public Property Get Index() As Long
    Index = mvarIndex
End Property
Public Property Get hWnd() As Long
    On Error Resume Next
    hWnd = mvarCtrl.hWnd
    If Err.Number <> 0 Then Err.Clear
End Property
Public Property Get ScaleMode() As ScaleModeConstants
    On Error Resume Next
    ScaleMode = mvarCtrl.ScaleMode
    If Err.Number <> 0 Then Err.Clear
End Property
Public Property Let ScaleMode(NewValue As ScaleModeConstants)
    On Error Resume Next
    mvarCtrl.ScaleMode = NewValue
    If Err.Number <> 0 Then Err.Clear
End Property
Public Property Get Picture() As StdPicture
    If mvarHasPicture Then
        Set Picture = mvarCtrl.Picture
    Else
        Set Picture = Nothing
    End If
End Property
Public Property Set Picture(NewValue As StdPicture)
    If mvarHasPicture Then Set mvarCtrl.Picture = NewValue
End Property
Public Property Get Name() As String
    Dim s As String

    s = mvarName
    If mvarIndex >= 0 Then
        s = s & "(" & CStr(mvarIndex) & ")"
    End If
    Name = s
End Property
Public Property Get Ctrl() As Control
    Set Ctrl = mvarCtrl
End Property
Public Property Get Children() As Long
    Children = mvarChildren
End Property
Public Property Let Children(ByVal NewValue As Long)
    mvarChildren = NewValue
End Property
Public Property Get Container() As Object
    Set Container = mvarContainer
End Property
Private Sub Class_Terminate()
    Set mvarForm = Nothing
    Set mvarCtrl = Nothing
    Set mvarContainer = Nothing
End Sub
Private Sub picCtrl_Click()
    RaiseEvent Click
End Sub
Private Sub picCtrl_GotFocus()
    RaiseEvent GotFocus
End Sub
Private Sub picCtrl_KeyDown(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub
Private Sub picCtrl_KeyPress(KeyAscii As Integer)
    RaiseEvent KeyPress(KeyAscii)
End Sub
Private Sub picCtrl_KeyUp(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub
Private Sub picCtrl_LostFocus()
    RaiseEvent LostFocus
End Sub
Private Sub picCtrl_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseDown(Button, Shift, X, Y)
End Sub
Private Sub picCtrl_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub
Private Sub picCtrl_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseUp(Button, Shift, X, Y)
End Sub
