VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PTForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Attribute VB_Ext_KEY = "Member0" ,"PTCtrls"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 1.6 9/9/2004

Public Enum PTFORM_RESIZE_STRATEGY
    PTFORM_RESIZE_MANUAL
    PTFORM_RESIZE_AUTO
    PTFORM_RESIZE_2STAGE
    PTFORM_RESIZE_DELAYED
End Enum

Private Const REG_SAVE_POS = "SavePos"
Private Const REG_REMEMBER = "Remember"
Private Const REG_MIN_TO_SYSTRAY = "MinToSysTray"
Private Const REG_GUMBY = "Gumby"
Private Const REG_TOP = "Top"
Private Const REG_LEFT = "Left"
Private Const REG_HEIGHT = "Height"
Private Const REG_WIDTH = "Width"
Private Const REG_WINDOW_STATE = "WindowState"
Private Const REG_BORDER_STYLE = "BorderStyle"

Private Const ACCESS_TIP = "Click Here to Adjust Window Options"

Private WinVer As PT_OS_INFO

Private Const LWA_ALPHA = &H2&
Private Const GWL_EXSTYLE As Long = (-20)
Private Const GWL_STYLE As Long = (-16)

' Window Styles
Private Const WS_OVERLAPPED As Long = &H0
Private Const WS_POPUP As Long = &H80000000
Private Const WS_CHILD As Long = &H40000000
Private Const WS_MINIMIZE As Long = &H20000000
Private Const WS_VISIBLE As Long = &H10000000
Private Const WS_DISABLED As Long = &H8000000
Private Const WS_CLIPSIBLINGS As Long = &H4000000
Private Const WS_CLIPCHILDREN As Long = &H2000000
Private Const WS_MAXIMIZE As Long = &H1000000
Private Const WS_CAPTION As Long = &HC00000     ' WS_BORDER or WS_DLGFRAME  */
Private Const WS_BORDER As Long = &H800000
Private Const WS_DLGFRAME As Long = &H400000
Private Const WS_VSCROLL As Long = &H200000
Private Const WS_HSCROLL As Long = &H100000
Private Const WS_SYSMENU As Long = &H80000
Private Const WS_THICKFRAME As Long = &H40000
Private Const WS_GROUP As Long = &H20000
Private Const WS_TABSTOP As Long = &H10000

Private Const WS_MINIMIZEBOX As Long = &H20000
Private Const WS_MAXIMIZEBOX As Long = &H10000

' Common Window Styles
Private Const WS_OVERLAPPEDWINDOW As Long = WS_OVERLAPPED Or WS_CAPTION Or WS_SYSMENU Or WS_THICKFRAME Or WS_MINIMIZEBOX Or WS_MAXIMIZEBOX
Private Const WS_POPUPWINDOW As Long = WS_POPUP Or WS_BORDER Or WS_SYSMENU
Private Const WS_CHILDWINDOW As Long = WS_CHILD

Private Const WS_TILED As Long = WS_OVERLAPPED
Private Const WS_ICONIC As Long = WS_MINIMIZE
Private Const WS_SIZEBOX As Long = WS_THICKFRAME
Private Const WS_TILEDWINDOW As Long = WS_OVERLAPPEDWINDOW
 
' Dialog Styles
Private Const DS_ABSALIGN         As Long = &H1
Private Const DS_SYSMODAL         As Long = &H2
Private Const DS_LOCALEDIT As Long = &H20          ' Edit items get Local storage. */
Private Const DS_SETFONT As Long = &H40            ' User specified font for Dlg controls */
Private Const DS_MODALFRAME As Long = &H80         ' Can be combined with WS_CAPTION  */
Private Const DS_NOIDLEMSG As Long = &H100         ' WM_ENTERIDLE message will not be sent */
Private Const DS_SETFOREGROUND As Long = &H200     ' not in win3.1 */

' if(WINVER >= as long = &h0400)
Private Const DS_3DLOOK As Long = &H4
Private Const DS_FIXEDSYS As Long = &H8&
Private Const DS_NOFAILCREATE As Long = &H10
Private Const DS_CONTROL As Long = &H400
Private Const DS_CENTER As Long = &H800&
Private Const DS_CENTERMOUSE As Long = &H1000
Private Const DS_CONTEXTHELP As Long = &H2000&

Private Const DS_SHELLFONT = (DS_SETFONT Or DS_FIXEDSYS)
' End If ' WINVER >= as long = &h0400 */
' if(_WIN32_WCE >= as long = &h0500)
Private Const DS_USEPIXELS As Long = &H8000&
' End If

' Extended Window Styles
Private Const WS_EX_DLGMODALFRAME As Long = &H1
Private Const WS_EX_NOPARENTNOTIFY As Long = &H4
Private Const WS_EX_TOPMOST As Long = &H8
Private Const WS_EX_ACCEPTFILES As Long = &H10
Private Const WS_EX_TRANSPARENT As Long = &H20

Private Const WS_EX_MDICHILD As Long = &H40
Private Const WS_EX_TOOLWINDOW As Long = &H80&
Private Const WS_EX_WINDOWEDGE As Long = &H100
Private Const WS_EX_CLIENTEDGE As Long = &H200&
Private Const WS_EX_CONTEXTHELP As Long = &H400&

Private Const WS_EX_RIGHT As Long = &H1000&
Private Const WS_EX_LEFT As Long = &H0&
Private Const WS_EX_RTLREADING As Long = &H2000&
Private Const WS_EX_LTRREADING As Long = &H0&
Private Const WS_EX_LEFTSCROLLBAR As Long = &H4000&
Private Const WS_EX_RIGHTSCROLLBAR As Long = &H0&

Private Const WS_EX_CONTROLPARENT As Long = &H10000
Private Const WS_EX_STATICEDGE As Long = &H20000
Private Const WS_EX_APPWINDOW As Long = &H40000

Private Const WS_EX_OVERLAPPEDWINDOW As Long = (WS_EX_WINDOWEDGE Or WS_EX_CLIENTEDGE)
Private Const WS_EX_PALETTEWINDOW As Long = (WS_EX_WINDOWEDGE Or WS_EX_TOOLWINDOW Or WS_EX_TOPMOST)
Private Const WS_EX_LAYERED As Long = &H80000
Private Const WS_EX_NOINHERITLAYOUT As Long = &H100000      ' Disable inheritence of mirroring by children
Private Const WS_EX_LAYOUTRTL As Long = &H400000            ' Right to left mirroring
Private Const WS_EX_COMPOSITED As Long = &H2000000
Private Const WS_EX_NOACTIVATE As Long = &H8000000

Private Const STARTF_USESHOWWINDOW = &H1
Private Const SW_NORMAL = 1
Private Const SW_MAXIMIZE = 3
Private Const SW_MINIMIZE = 6
Private Const SW_SHOWMINIMIZED = 2
Private Const SW_SHOWMAXIMIZED = 3
Private Const SW_SHOWMINNOACTIVE = 7

' SetWindowPos Flags
Private Enum SWP_FLAG
    SWP_NOSIZE = &H1
    SWP_NOMOVE = &H2
    SWP_NOZORDER = &H4
    SWP_NOREDRAW = &H8
    SWP_NOACTIVATE = &H10
    SWP_FRAMECHANGED = &H20       ' The frame changed: send WM_NCCALCSIZE
    SWP_SHOWWINDOW = &H40
    SWP_HIDEWINDOW = &H80
    SWP_NOCOPYBITS = &H100
    SWP_NOOWNERZORDER = &H200     ' Don't do owner Z ordering
    SWP_NOSENDCHANGING = &H400    ' Don't send WM_WINDOWPOSCHANGING

    SWP_DRAWFRAME = SWP_FRAMECHANGED
    SWP_NOREPOSITION = SWP_NOOWNERZORDER

    SWP_DEFERERASE = &H2000
    SWP_ASYNCWINDOWPOS = &H4000

    SWP_FLAGS = SWP_NOMOVE Or SWP_NOSIZE
End Enum

Private Enum STD_WINDOW_HANDLE
    HWND_BOTTOM = 1
    HWND_TOP = 0
    HWND_TOPMOST = -1
    HWND_NOTOPMOST = -2
End Enum

Private Enum RGN_MODE
    RGN_AND = 1
    RGN_OR = 2
    RGN_XOR = 3
    RGN_DIFF = 4
    RGN_COPY = 5
    RGN_MAX = RGN_COPY
    RGN_MIN = RGN_AND
End Enum

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

' Startup infomation used to position new window
Private Type STARTUPINFO
    cb As Long
    lpReserved As Long
    lpDesktop As Long
    lpTitle As Long
    dwX As Long
    dwY As Long
    dwXSize As Long
    dwYSize As Long
    dwXCountChars As Long
    dwYCountChars As Long
    dwFillAttribute As Long
    dwFlags As Long
    wShowWindow As Integer
    cbReserved2 As Integer
    lpReserved2 As Long
    hStdInput As Long
    hStdOutput As Long
    hStdError As Long
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

Private Declare Function SetForegroundWindow Lib "user32" ( _
    ByVal hWnd As Long) As Long

Private Declare Function SetWindowPos Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal hWndInsertAfter As STD_WINDOW_HANDLE, _
    ByVal X As Long, _
    ByVal Y As Long, _
    ByVal cx As Long, _
    ByVal cy As Long, _
    ByVal uFlags As SWP_FLAG) As Long

Private Declare Function GetWindowLongA Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal nIndex As Long) As Long

Private Declare Function SetWindowLongA Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal nIndex As Long, _
    ByVal dwNewLong As Long) As Long

Private Declare Function GetClientRect Lib "user32.dll" ( _
    ByVal hWnd As Long, _
    ByRef lpRect As RECT) As Long

Private Declare Function SetLayeredWindowAttributes Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal crey As Byte, _
    ByVal bAlpha As Byte, _
    ByVal dwFlags As Long) As Long

Private Declare Function CombineRgn Lib "gdi32.dll" ( _
    ByVal hDestRgn As Long, _
    ByVal hSrcRgn1 As Long, _
    ByVal hSrcRgn2 As Long, _
    ByVal nCombineMode As RGN_MODE) As Long

Private Declare Function BeginPath Lib "GDI32" ( _
    ByVal hdc As Long) As Long

Private Declare Function TextOutA Lib "gdi32.dll" ( _
    ByVal hdc As Long, _
    ByVal X As Long, _
    ByVal Y As Long, _
    ByVal lpString As String, _
    ByVal nCount As Long) As Long

Private Declare Function EndPath Lib "GDI32" ( _
    ByVal hdc As Long) As Long

Private Declare Function Ellipse Lib "GDI32" ( _
    ByVal hdc As Long, _
    ByVal X1 As Long, _
    ByVal Y1 As Long, _
    ByVal X2 As Long, _
    ByVal Y2 As Long) As Long

Private Declare Function Rectangle Lib "GDI32" ( _
    ByVal hdc As Long, _
    ByVal X1 As Long, _
    ByVal Y1 As Long, _
    ByVal X2 As Long, _
    ByVal Y2 As Long) As Long

Private Declare Function RoundRect Lib "GDI32" ( _
    ByVal hdc As Long, _
    ByVal X1 As Long, _
    ByVal Y1 As Long, _
    ByVal X2 As Long, _
    ByVal Y2 As Long, _
    ByVal X3 As Long, _
    ByVal Y3 As Long) As Long

Private Declare Function Polygon Lib "GDI32" ( _
    ByVal hdc As Long, _
    ByRef lpPoint As POINTAPI, _
    ByVal nCount As Long) As Long

Private Declare Function Polyline Lib "GDI32" ( _
    ByVal hdc As Long, _
    ByRef lpPoint As POINTAPI, _
    ByVal nCount As Long) As Long

Private Declare Function PathToRegion Lib "GDI32" ( _
    ByVal hdc As Long) As Long

Private Declare Function GetRgnBox Lib "GDI32" ( _
    ByVal hRgn As Long, _
    ByRef lpRect As RECT) As Long

Private Declare Function CreateRectRgn Lib "gdi32.dll" ( _
    ByVal X1 As Long, _
    ByVal Y1 As Long, _
    ByVal X2 As Long, _
    ByVal Y2 As Long) As Long

Private Declare Function CreateRectRgnIndirect Lib "GDI32" ( _
    ByRef lpRect As RECT) As Long

Private Declare Function SetWindowRgn Lib "user32.dll" ( _
    ByVal hWnd As Long, _
    ByVal hRgn As Long, _
    ByVal bRedraw As Boolean) As Long

Private Declare Function InvalidateRect Lib "user32.dll" ( _
    ByVal hWnd As Long, _
    ByRef lpRect As RECT, _
    ByVal bErase As Long) As Long

Private Declare Function InvalidateRgn Lib "user32.dll" ( _
    ByVal hWnd As Long, _
    ByVal hRgn As Long, _
    ByVal bErase As Long) As Long

Private Declare Function UpdateWindow Lib "user32.dll" ( _
    ByVal hWnd As Long) As Long

Private Declare Function DeleteObject Lib "gdi32.dll" ( _
    ByVal hObject As Long) As Long

Private Declare Sub GetStartupInfoA Lib "kernel32" ( _
    ByRef lpStartupInfo As STARTUPINFO)

Private RegMeKey As String
Private mvarIsForm0 As Boolean

Private SplitCnt As Long
Private Splits() As PTSplit
Private StretchCnt As Long
Private Stretches() As PTStretch

Private mvarID As Long
Private mvarIndex As Long
Private mvarName As String

Private WithEvents mvarForm As Form
Attribute mvarForm.VB_VarHelpID = -1
Private mvarIsUnloaded As Boolean
Private mvarCtrls() As PTCtrl
Private mvarCtrlCnt As Long
Private mvarWindowState As FormWindowStateConstants
Private mvarBorderStyle As FormBorderStyleConstants
Private mvarMaxButton As Boolean
Private mvarMinButton As Boolean
Private mvarControlBox As Boolean
Private mvarShowInTaskbar As Boolean
Private mvarClipControls As Boolean
Private mvarWhatsThisButton As Boolean

Private mvarMinToSysTray As Boolean
Private TrayHandle As Long
Private mvarGumby As Boolean

Private mvarTop As Single
Private mvarLeft As Single
Private mvarHeight As Single
Private mvarWidth As Single
Private mvarFontSize As Single

Private mvarResizeStrategy As PTFORM_RESIZE_STRATEGY
Private mvarNoResizeCnt As Long
Private mvarResizeDeferred As Long
Private mvarResizing As Boolean
Private mvarReady As Boolean
Private mvarIsTop As Boolean
Private mvarIsTopMost As Boolean

Private mvarOrigBorderStyle As FormBorderStyleConstants
Private mvarOrigLeft As Single
Private mvarOrigTop As Single
Private mvarOrigHeight As Single
Private mvarOrigWidth As Single
Private mvarOrigFontSize As Single

Private mvarMinWidth As Single
Private mvarMinHeight As Single
Private mvarMaxWidth As Single
Private mvarMaxHeight As Single
Private mvarStretchWidth As Single
Private mvarStretchHeight As Single

Private mvarScaleHorz As Single
Private mvarScaleVert As Single
Private mvarScaleFont As Single

Private mvarRemember As Boolean
Private mvarSaveSize As Boolean
Private mvarSavePos As Boolean

Private OrigImgAccessWidth As Single
Private OrigImgAccessHeight As Single
Private WithEvents imgAccess As Image
Attribute imgAccess.VB_VarHelpID = -1

Private OrigPicAccessWidth As Single
Private OrigPicAccessHeight As Single
Private WithEvents picAccess As PTCtrl
Attribute picAccess.VB_VarHelpID = -1

Private OrigBtnAccessWidth As Single
Private OrigBtnAccessHeight As Single
Private WithEvents btnAccess As CommandButton
Attribute btnAccess.VB_VarHelpID = -1

Private WithEvents sbAccess As StatusBar
Attribute sbAccess.VB_VarHelpID = -1
Private pnlAccess As Panel

Private mnuAccess As PTMenu
Attribute mnuAccess.VB_VarHelpID = -1
Private WithEvents mnuCancel As PTMenuItem
Attribute mnuCancel.VB_VarHelpID = -1
Private WithEvents mnuGumby As PTMenuItem
Attribute mnuGumby.VB_VarHelpID = -1
Private WithEvents mnu1to1 As PTMenuItem
Attribute mnu1to1.VB_VarHelpID = -1
Private WithEvents mnuRemember As PTMenuItem
Attribute mnuRemember.VB_VarHelpID = -1
Private WithEvents mnuMinToSysTray As PTMenuItem
Attribute mnuMinToSysTray.VB_VarHelpID = -1

Private mnuZoom As PTMenu
Private WithEvents mnu85 As PTMenuItem
Attribute mnu85.VB_VarHelpID = -1
Private WithEvents mnu100 As PTMenuItem
Attribute mnu100.VB_VarHelpID = -1
Private WithEvents mnu125 As PTMenuItem
Attribute mnu125.VB_VarHelpID = -1
Private WithEvents mnu150 As PTMenuItem
Attribute mnu150.VB_VarHelpID = -1

Public Event Initialize()
Public Event Load()
Public Event Unload(Cancel As Integer)
Public Event KeyPress(ByRef KeyAscii As Integer)
Public Event KeyDown(ByRef KeyCode As Integer, Shift As Integer)
Public Event KeyUp(ByRef KeyCode As Integer, Shift As Integer)
Public Event QueryUnload(ByRef Cancel As Integer, UnloadMode As Integer)
Public Event BeforeResize(ByRef Cancel As Boolean)
Public Event Resize()
Public Event AfterResize()
Public Event MinToSysTrayChanged(ByRef NewValue As Boolean)
Public Event TrayEvent(TrayID As Long, Action As PT_MOUSE_ACTION)
Public Property Get ResizeDeferred() As Long
    ResizeDeferred = mvarResizeDeferred
End Property
Public Sub Stretch(ByRef DeltaWidth As Single, ByRef DeltaHeight As Single)
    Dim OrigWidth As Single
    Dim OrigHeight As Single

    OrigWidth = mvarForm.Width
    OrigHeight = mvarForm.Height

    mvarNoResizeCnt = mvarNoResizeCnt + 1
    mvarForm.Width = mvarOrigWidth + DeltaWidth
    DeltaWidth = mvarForm.Width - OrigWidth
    mvarStretchWidth = mvarStretchWidth + DeltaWidth

    mvarForm.Height = OrigHeight + DeltaHeight
    DeltaHeight = mvarForm.Height - OrigHeight
    mvarStretchHeight = mvarStretchHeight + DeltaHeight
    mvarNoResizeCnt = mvarNoResizeCnt - 1
End Sub
Public Property Get Remember() As Boolean
    Remember = mvarRemember
End Property
Public Property Let Remember(NewValue As Boolean)
    mvarRemember = NewValue
End Property
Public Sub Save(Optional AppID As String = APPKEY)
    Dim Reg As Registry
    Dim i As Long
    Dim lim As Long
    Dim FormRegKey As String

    Set Reg = New Registry
    If AppID <> APPKEY Then
        FormRegKey = Reg.GetFormSubKey(mvarForm, AppID)
    Else
        FormRegKey = RegMeKey
    End If

    Reg.WriteValue REG_MIN_TO_SYSTRAY, mvarMinToSysTray, FormRegKey
    Reg.WriteValue REG_GUMBY, mvarGumby, FormRegKey

    Reg.WriteValue REG_REMEMBER, mvarRemember, FormRegKey
    If Not mvarRemember Then Exit Sub

    If mvarSaveSize Then
        If SplitCnt > 0 Then
            lim = UBound(Splits)
            For i = 0 To lim
                Splits(i).Save FormRegKey
            Next i
        End If

        If StretchCnt > 0 Then
            lim = UBound(Stretches)
            For i = 0 To lim
                Stretches(i).Save FormRegKey
            Next i
        End If

        Reg.WriteValue REG_WINDOW_STATE, mvarForm.WindowState, FormRegKey
        Reg.WriteValue REG_BORDER_STYLE, mvarBorderStyle, FormRegKey

        If mvarForm.WindowState = vbNormal Then
            Reg.WriteValue REG_HEIGHT, mvarForm.Height - mvarStretchHeight, FormRegKey
            Reg.WriteValue REG_WIDTH, mvarForm.Width - mvarStretchWidth, FormRegKey
        End If
    End If

    If mvarSavePos Then
        If (mvarForm.StartupPosition = vbStartUpManual) Or (mvarForm.StartupPosition = vbStartUpWindowsDefault) Then
            If mvarForm.WindowState = vbNormal Then
                Reg.WriteValue REG_TOP, mvarForm.Top, FormRegKey
                Reg.WriteValue REG_LEFT, mvarForm.Left, FormRegKey
            End If
        End If
    End If
End Sub
Public Function Restore(Optional AppID As String = APPKEY) As FormWindowStateConstants
    Dim Reg As Registry
    Dim i As Long
    Dim lim As Long
    Dim FormRegKey As String
    Dim si As STARTUPINFO

    Set Reg = New Registry
    If AppID <> APPKEY Then
        FormRegKey = Reg.GetFormSubKey(mvarForm, AppID)
    Else
        FormRegKey = RegMeKey
    End If

    mvarMinToSysTray = Reg.ReadValue(REG_MIN_TO_SYSTRAY, False, FormRegKey)

    mvarRemember = Reg.ReadValue(REG_REMEMBER, True, FormRegKey)
    If Not mvarRemember Then
        Restore = mvarWindowState
        Exit Function
    End If

    Me.Gumby = Reg.ReadValue(REG_GUMBY, False, FormRegKey)
    If mvarGumby Or (mvarForm.WindowState = vbSizable) Or (mvarForm.WindowState = vbSizableToolWindow) Then
        mvarWidth = Reg.ReadValue(REG_WIDTH, mvarForm.Width, FormRegKey)
        If mvarWidth < mvarMinWidth Then mvarWidth = mvarMinWidth
        mvarHeight = Reg.ReadValue(REG_HEIGHT, mvarForm.Height, FormRegKey)
        If mvarHeight < mvarMinHeight Then mvarHeight = mvarMinHeight
    End If

    mvarForm.Width = mvarWidth
    mvarWidth = mvarForm.Width
    mvarForm.Height = mvarHeight
    mvarHeight = mvarForm.Height
    mvarSaveSize = True

'    Me.BorderStyle = Reg.ReadValue(REG_BORDER_STYLE, mvarBorderStyle, FormRegKey)

    mvarWindowState = Reg.ReadValue(REG_WINDOW_STATE, vbNormal, FormRegKey)
    If mvarIsForm0 Then ' startup form
        si.cb = Len(si)
        Call GetStartupInfoA(si)
        If si.dwFlags And STARTF_USESHOWWINDOW Then
            If si.wShowWindow = SW_SHOWMAXIMIZED Then
                mvarWindowState = vbMaximized
            ElseIf si.wShowWindow = SW_MAXIMIZE Then
                mvarWindowState = vbMaximized
            ElseIf si.wShowWindow = SW_SHOWMINIMIZED Then
                mvarWindowState = vbMinimized
            ElseIf si.wShowWindow = SW_MINIMIZE Then
                mvarWindowState = vbMinimized
            ElseIf si.wShowWindow = SW_SHOWMINNOACTIVE Then
                mvarWindowState = vbMinimized
            End If
        End If
    End If

    mvarForm.WindowState = mvarWindowState
    Restore = mvarWindowState

    If (mvarForm.StartupPosition = vbStartUpManual) Or (mvarForm.StartupPosition = vbStartUpWindowsDefault) Then
        mvarTop = Reg.ReadValue(REG_TOP, (Screen.Height - mvarForm.Height) / 2, FormRegKey)
        If (mvarTop < 0) Or (mvarTop > (Screen.Height - 300)) Then
            mvarTop = (Screen.Height - mvarHeight) / 2
        End If
        mvarLeft = Reg.ReadValue(REG_LEFT, (Screen.Width - mvarForm.Width) / 2, FormRegKey)
        If (mvarLeft < 0) Or (mvarLeft > (Screen.Width - 300)) Then
            mvarLeft = (Screen.Width - mvarWidth) / 2
        End If

        mvarForm.Top = mvarTop
        mvarTop = mvarForm.Top
        mvarForm.Left = mvarLeft
        mvarLeft = mvarForm.Left
        mvarSavePos = True
    End If

    If SplitCnt > 0 Then
        lim = UBound(Splits)
        For i = 0 To lim
            Splits(i).Restore FormRegKey
        Next i
    End If

    If StretchCnt > 0 Then
        lim = UBound(Stretches)
        For i = 0 To lim
            Stretches(i).Restore FormRegKey
        Next i
    End If
End Function
Public Property Get Form() As Form
    Set Form = mvarForm
End Property
Public Property Get ID() As Long
    ID = mvarID
End Property
Public Property Let ID(ByVal NewValue As Long)
    mvarID = NewValue
End Property
Public Property Get Index() As Long
    Index = mvarIndex
End Property
Public Property Get Name() As String
    Dim s As String

    s = mvarName
    If mvarIndex >= 0 Then
        s = s & "(" & CStr(mvarIndex) & ")"
    End If
    Name = s
End Property
Public Property Get ScaleVert() As Single
    ScaleVert = mvarScaleVert
End Property
Public Property Let ScaleVert(ByVal NewValue As Single)
    mvarScaleVert = NewValue
End Property
Public Property Get ScaleHorz() As Single
    ScaleHorz = mvarScaleHorz
End Property
Public Property Let ScaleHorz(ByVal NewValue As Single)
    mvarScaleHorz = NewValue
End Property
Public Property Get ScaleFont() As Single
    ScaleFont = mvarScaleFont
End Property
Public Property Let ScaleFont(ByVal NewValue As Single)
    mvarScaleFont = NewValue
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
Public Property Get OrigFontSize() As Single
    OrigFontSize = mvarOrigFontSize
End Property
Public Property Get ResizeStrategy() As PTFORM_RESIZE_STRATEGY
    ResizeStrategy = mvarResizeStrategy
End Property
Public Property Let ResizeStrategy(ByVal NewValue As PTFORM_RESIZE_STRATEGY)
    mvarResizeStrategy = NewValue
End Property
Public Function IsTagPresent(Tag As String) As Boolean
    If InStr(1, mvarForm.Tag, vbTab & CStr(Tag) & vbTab, vbTextCompare) > 0 Then
        IsTagPresent = True
    Else
        IsTagPresent = False
    End If
End Function
'Public Property Set ActiveControl(ByVal NewValue As Control)
'    On Error Resume Next
'    NewValue.SetFocus
'    Err.Clear
'End Property
'Public Property Get ActiveControl() As Control
'    Set ActiveControl = mvarForm.ActiveControl
'End Property
'Public Property Get MDIChild() As Boolean
'    MDIChild = mvarForm.MDIChild
'End Property
'Public Property Set Picture(ByVal NewValue As StdPicture)
'    Set mvarForm.Picture = NewValue
'End Property
'Public Property Get Picture() As StdPicture
'    Set Picture = mvarForm.Picture
'End Property
'Public Property Get Image() As Long
'    Image = mvarForm.Image
'End Property
'Public Property Let HelpContextID(ByVal NewValue As Long)
'    mvarForm.HelpContextID = NewValue
'End Property
'Public Property Get HelpContextID() As Long
'    HelpContextID = mvarForm.HelpContextID
'End Property
'Public Property Let FillStyle(ByVal NewValue As FillStyleConstants)
'    mvarForm.FillStyle = NewValue
'End Property
'Public Property Get FillStyle() As FillStyleConstants
'    FillStyle = mvarForm.FillStyle
'End Property
'Public Property Let DrawWidth(ByVal NewValue As Integer)
'    If NewValue >= 0 Then mvarForm.DrawWidth = NewValue
'End Property
'Public Property Get DrawWidth() As Integer
'    DrawWidth = mvarForm.DrawWidth
'End Property
'Public Property Let DrawStyle(ByVal NewValue As DrawStyleConstants)
'    mvarForm.DrawStyle = NewValue
'End Property
'Public Property Get DrawStyle() As DrawStyleConstants
'    DrawStyle = mvarForm.DrawStyle
'End Property
'Public Property Let DrawMode(ByVal NewValue As DrawModeConstants)
'    mvarForm.DrawMode = NewValue
'End Property
'Public Property Get DrawMode() As DrawModeConstants
'    DrawMode = mvarForm.DrawMode
'End Property
'Public Property Get Appearance() As AppearanceConstants
'    Appearance = mvarForm.Appearance
'End Property
'Public Property Let AutoDraw(ByVal NewValue As Boolean)
'    mvarForm.AutoDraw = NewValue
'End Property
'Public Property Get AutoDraw() As Boolean
'    AutoDraw = mvarForm.AutoDraw
'End Property
Public Property Get WhatsThisHelp() As Boolean
    WhatsThisHelp = mvarForm.WhatsThisHelp
End Property
Public Property Let WindowState(ByVal NewValue As FormWindowStateConstants)
    mvarForm.WindowState = NewValue
End Property
Public Property Get WindowState() As FormWindowStateConstants
    WindowState = mvarForm.WindowState
End Property
Public Property Let Visible(ByVal NewValue As Boolean)
    mvarForm.Visible = NewValue
End Property
Public Property Get Visible() As Boolean
    Visible = mvarForm.Visible
End Property
Public Sub TagRemove(Tag As String)
    mvarForm.Tag = Replace(mvarForm.Tag, vbTab & CStr(Tag) & vbTab, "")
End Sub
Public Sub TagAdd(Tag As String)
    mvarForm.Tag = mvarForm.Tag & vbTab & CStr(Tag) & vbTab
End Sub
Public Property Let Tag(ByVal NewValue As String)
    mvarForm.Tag = NewValue
End Property
Public Property Get Tag() As String
    Tag = mvarForm.Tag
End Property
Public Property Get StartupPosition() As StartUpPositionConstants
    StartupPosition = mvarForm.StartupPosition
End Property
Public Property Let ScaleMode(ByVal NewValue As ScaleModeConstants)
    mvarForm.ScaleMode = NewValue
End Property
Public Property Get ScaleMode() As ScaleModeConstants
    ScaleMode = mvarForm.ScaleMode
End Property
Public Property Let ScaleLeft(ByVal NewValue As Single)
    mvarForm.ScaleLeft = NewValue
End Property
Public Property Get ScaleLeft() As Single
    ScaleLeft = mvarForm.ScaleLeft
End Property
Public Property Let ScaleTop(ByVal NewValue As Single)
    mvarForm.ScaleTop = NewValue
End Property
Public Property Get ScaleTop() As Single
    ScaleTop = mvarForm.ScaleTop
End Property
Public Property Let ScaleWidth(ByVal NewValue As Single)
    mvarForm.ScaleWidth = NewValue
End Property
Public Property Get ScaleWidth() As Single
    ScaleWidth = mvarForm.ScaleWidth
End Property
Public Property Let ScaleHeight(ByVal NewValue As Single)
    mvarForm.ScaleHeight = NewValue
End Property
Public Property Get ScaleHeight() As Single
    ScaleHeight = mvarForm.ScaleHeight
End Property
Public Property Set MouseIcon(ByVal NewValue As StdPicture)
    Set mvarForm.MouseIcon = NewValue
End Property
Public Property Get MouseIcon() As StdPicture
    Set MouseIcon = mvarForm.MouseIcon
End Property
Public Property Let MousePointer(ByVal NewValue As MousePointerConstants)
    mvarForm.MousePointer = NewValue
End Property
Public Property Get MousePointer() As MousePointerConstants
    MousePointer = mvarForm.MousePointer
End Property
'Public Property Let KeyPreview(ByVal NewValue As Boolean)
'    mvarForm.KeyPreview = NewValue
'End Property
Public Property Get KeyPreview() As Boolean
    KeyPreview = mvarForm.KeyPreview
End Property
Public Property Get hdc() As Long
    hdc = mvarForm.hdc
End Property
Public Property Get hWnd() As Long
    hWnd = mvarForm.hWnd
End Property
Public Property Get Icon() As IPictureDisp
    Set Icon = mvarForm.Icon
End Property
Public Property Set Icon(NewIcon As IPictureDisp)
    Set mvarForm.Icon = NewIcon
End Property
'Public Property Let FontTransparent(ByVal NewValue As Boolean)
'    mvarForm.FontTransparent = NewValue
'End Property
'Public Property Get FontTransparent() As Boolean
'    FontTransparent = mvarForm.FontTransparent
'End Property
Public Property Let FontSize(ByVal NewValue As Single)
    mvarFontSize = NewValue
    mvarForm.FontSize = mvarFontSize
End Property
Public Property Get FontSize() As Single
    FontSize = mvarForm.FontSize
End Property
Public Property Let FontName(ByVal NewValue As String)
    mvarForm.FontName = NewValue
End Property
Public Property Get FontName() As String
    FontName = mvarForm.FontName
End Property
Public Property Let FontUnderline(ByVal NewValue As Boolean)
    mvarForm.FontUnderline = NewValue
End Property
Public Property Get FontUnderline() As Boolean
    FontUnderline = mvarForm.FontUnderline
End Property
Public Property Let FontStrikethru(ByVal NewValue As Boolean)
    mvarForm.FontStrikethru = NewValue
End Property
Public Property Get FontStrikethru() As Boolean
    FontStrikethru = mvarForm.FontStrikethru
End Property
Public Property Let FontItalic(ByVal NewValue As Boolean)
    mvarForm.FontItalic = NewValue
End Property
Public Property Get FontItalic() As Boolean
    FontItalic = mvarForm.FontItalic
End Property
Public Property Let FontBold(ByVal NewValue As Boolean)
    mvarForm.FontBold = NewValue
End Property
Public Property Get FontBold() As Boolean
    FontBold = mvarForm.FontBold
End Property
Public Property Set Font(ByVal NewValue As StdFont)
    Set mvarForm.Font = NewValue
End Property
Public Property Get Font() As StdFont
    Set Font = mvarForm.Font
End Property
Public Property Let Enabled(ByVal NewValue As Boolean)
    mvarForm.Enabled = NewValue
End Property
Public Property Get Enabled() As Boolean
    Enabled = mvarForm.Enabled
End Property
Public Property Let MinToSysTray(ByVal NewValue As Boolean)
    mvarMinToSysTray = NewValue
End Property
Public Property Get MinToSysTray() As Boolean
    MinToSysTray = mvarMinToSysTray
End Property
Public Property Let CurrentY(ByVal NewValue As Single)
    mvarForm.CurrentY = NewValue
End Property
Public Property Get CurrentY() As Single
    CurrentY = mvarForm.CurrentY
End Property
Public Property Let CurrentX(ByVal NewValue As Single)
    mvarForm.CurrentX = NewValue
End Property
Public Property Get CurrentX() As Single
    CurrentX = mvarForm.CurrentX
End Property
Public Property Let ForeColor(ByVal NewValue As SystemColorConstants)
    mvarForm.ForeColor = NewValue
End Property
Public Property Get ForeColor() As SystemColorConstants
    ForeColor = mvarForm.ForeColor
End Property
Public Property Let BackColor(ByVal NewValue As SystemColorConstants)
    mvarForm.BackColor = NewValue
End Property
Public Property Get BackColor() As SystemColorConstants
    BackColor = mvarForm.BackColor
End Property
Public Property Let Caption(ByVal NewValue As String)
    mvarForm.Caption = NewValue
End Property
Public Property Get Caption() As String
    Caption = mvarForm.Caption
End Property
Public Sub Show(Optional Modal As FormShowConstants = vbModal, Optional OwnerForm As Form = Nothing)
    If OwnerForm Is Nothing Then
        mvarForm.Show Modal
    Else
        mvarForm.Show Modal, OwnerForm
    End If
End Sub
Public Sub ZOrder(Optional Position As Integer = 0)
    mvarForm.ZOrder Position
End Sub
Public Function TextWidth(Textx As String) As Single
    TextWidth = mvarForm.TextWidth(Textx)
End Function
Public Function TextHeight(Textx As String) As Single
    TextHeight = mvarForm.TextHeight(Textx)
End Function
Public Sub SetFocus()
    On Error Resume Next
    mvarForm.SetFocus
    Err.Clear
End Sub
Public Function ScaleX(Width As Single, Optional FromScale As ScaleModeConstants = vbHimetric, Optional ToScale As ScaleModeConstants = -1) As Single
    If ToScale = -1 Then
        ScaleX = mvarForm.ScaleX(Width, FromScale, mvarForm.ScaleMode)
    Else
        ScaleX = mvarForm.ScaleX(Width, FromScale, ToScale)
    End If
End Function
Public Function ScaleY(Width As Single, Optional FromScale As ScaleModeConstants = vbHimetric, Optional ToScale As ScaleModeConstants = -1) As Single
    If ToScale = -1 Then
        ScaleY = mvarForm.ScaleY(Width, FromScale, mvarForm.ScaleMode)
    Else
        ScaleY = mvarForm.ScaleY(Width, FromScale, ToScale)
    End If
End Function
Public Sub Refresh()
    mvarForm.Refresh
End Sub
Public Sub PopupMenu(MenuName As Menu, Optional Flags As MenuControlConstants = 0, Optional X As Single, Optional Y As Single, Optional DefaultMenu As Menu = Nothing)
    mvarForm.PopupMenu MenuName, Flags, X, Y, DefaultMenu
End Sub
Public Sub Hide()
    mvarForm.Hide
End Sub
Public Sub Cls()
    mvarForm.Cls
End Sub
Public Property Let IsTop(ByVal NewValue As Boolean)
    If NewValue Then
        SetWindowPos mvarForm.hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_FLAGS
        DoEvents
        SetWindowPos mvarForm.hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FLAGS
    End If
    mvarIsTop = NewValue
End Property
Public Property Get IsTop() As Boolean
    IsTop = mvarIsTop
End Property
Public Property Let IsTopMost(ByVal NewValue As Boolean)
    If NewValue Then
        SetWindowPos mvarForm.hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_FLAGS
    Else
        SetWindowPos mvarForm.hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FLAGS
    End If
    mvarIsTopMost = NewValue
End Property
Public Property Get IsTopMost() As Boolean
    IsTopMost = mvarIsTopMost
End Property
Public Sub SetFonts(Optional FontName As String = "Arial", Optional MinFontSize As Single = 9)
    Dim ptc As PTCtrl
    Dim inx As Long
    Dim i As Long
    Dim s As String
    Dim lim As Long
    Dim Pnls As Panels
    Dim Pnl As Panel

    mvarForm.FontName = FontName
    If mvarForm.FontSize < MinFontSize Then
        mvarFontSize = MinFontSize
        mvarForm.FontSize = mvarFontSize
    End If

    On Error Resume Next
    If mvarCtrlCnt > 0 Then
        lim = UBound(mvarCtrls)
        For inx = 0 To lim
            Set ptc = mvarCtrls(inx)
            If InStr(1, ptc.Ctrl.Tag, "NONAME", vbTextCompare) <= 0 Then
                ptc.FontName = FontName
            End If
            If InStr(1, ptc.Ctrl.Tag, "NOSIZE", vbTextCompare) <= 0 Then
                If ptc.FontSize < MinFontSize Then ptc.FontSize = MinFontSize
            End If
            ptc.Text = Replace(ptc.Text, "\n", vbCrLf, , , vbTextCompare)
            Err.Clear
        Next inx
    End If

    ReInit
End Sub
Public Sub Constructor(Frm As Form, Optional MinScaleHorz As Single = 0.8, Optional MinScaleVert As Single = 0.8, Optional ID As Long = -1)
    Dim Ctrl As Control
    Dim ptc As PTCtrl
    Dim i As Long
    Dim inx As Long
    Dim ci As PTCtrl
    Dim lim As Long
    Dim Reg As Registry
    Dim Status As StatusBar
    Dim Pnl As Panel
    Dim img As Image
    Dim pic As PictureBox
    Dim Btn As CommandButton
    Dim Cnt As Long

    Set mvarForm = Frm
    If mvarForm Is Forms(0) Then mvarIsForm0 = True
    Set Reg = New Registry
    RegMeKey = Reg.GetFormSubKey(mvarForm)

    mvarID = ID
    mvarName = mvarForm.Name
    If VarType(mvarForm) And vbArray Then
        mvarIndex = mvarForm.Index
    Else
        mvarIndex = -1
    End If

    mvarWindowState = mvarForm.WindowState
    mvarBorderStyle = mvarForm.BorderStyle
    mvarMaxButton = mvarForm.MaxButton
    mvarMinButton = mvarForm.MinButton
    mvarControlBox = mvarForm.ControlBox
    mvarClipControls = mvarForm.ClipControls
    mvarShowInTaskbar = mvarForm.ShowInTaskbar
    mvarWhatsThisButton = mvarForm.WhatsThisButton
    mvarTop = mvarForm.Top
    mvarLeft = mvarForm.Left
    mvarHeight = mvarForm.Height
    mvarWidth = mvarForm.Width

    mvarOrigBorderStyle = mvarBorderStyle
    mvarOrigLeft = mvarLeft
    mvarOrigTop = mvarTop
    mvarOrigHeight = mvarHeight
    mvarOrigWidth = mvarWidth
    mvarOrigFontSize = mvarForm.FontSize

    mvarMinHeight = mvarOrigHeight * MinScaleVert
    mvarMinWidth = mvarOrigWidth * MinScaleHorz
    mvarMaxHeight = Screen.Height
    mvarMaxWidth = Screen.Width

    mvarScaleFont = 1
    mvarScaleHorz = 1
    mvarScaleVert = 1

    mvarCtrlCnt = 0
    Erase mvarCtrls
    If mvarForm.Controls.Count > 0 Then
        On Error Resume Next
        For Each Ctrl In mvarForm.Controls
            Set ptc = New PTCtrl
            ptc.Constructor Me, Ctrl, mvarCtrlCnt
            If ptc.IsAccessButton Then
                Set Btn = Ctrl
                Set btnAccess = Btn
                OrigBtnAccessHeight = Btn.Height
                OrigBtnAccessWidth = Btn.Width
                BuildAccessMenus
            ElseIf ptc.IsAccessPictureBox Then
                Set pic = Ctrl
                Set picAccess = ptc
                If FormAccessUp Is Nothing Then
                    Set FormAccessUp = ResIconLoad("STAR-UP", ICON_ACTUAL)  ' LoadResPicture("STAR-UP", vbResIcon)
                    Set FormAccessDown = ResIconLoad("STAR-DOWN", ICON_ACTUAL)  ' LoadResPicture("STAR-DOWN", vbResIcon)
                End If
                OrigPicAccessHeight = pic.Height
                OrigPicAccessWidth = pic.Width
                BuildAccessMenus
            ElseIf ptc.IsAccessImage Then
                Set img = Ctrl
                Set imgAccess = img
                If FormAccessUp Is Nothing Then
                    Set FormAccessUp = ResIconLoad("STAR-UP", ICON_ACTUAL)  ' LoadResPicture("STAR-UP", vbResIcon)
                    Set FormAccessDown = ResIconLoad("STAR-DOWN", ICON_ACTUAL)  ' LoadResPicture("STAR-DOWN", vbResIcon)
                End If
                OrigImgAccessHeight = img.Height
                OrigImgAccessWidth = img.Width
                BuildAccessMenus
            ElseIf ptc.IsStatusBar Then
                Set Status = Ctrl
                For i = 1 To Status.Panels.Count
                    If StrComp(Status.Panels(i).Key, "Access", vbTextCompare) = 0 Then
                        Set sbAccess = Status
                        Set pnlAccess = Status.Panels(i)
                        pnlAccess.ToolTipText = ACCESS_TIP
                        BuildAccessMenus
                        Exit For
                    End If
                Next i
            End If
            If ptc.Resizable Then
                ReDim Preserve mvarCtrls(mvarCtrlCnt)
                Set mvarCtrls(mvarCtrlCnt) = ptc
'                mvarCtrls(mvarCtrlCnt).Constructor Me, Ctrl
                mvarCtrlCnt = mvarCtrlCnt + 1
            Else
                Set ptc = Nothing
            End If
        Next Ctrl

        If mvarCtrlCnt > 0 Then
            lim = UBound(mvarCtrls)
            For inx = 0 To lim
                Set ci = mvarCtrls(inx)
                ci.Children = 0
                For i = 0 To lim
                    If mvarCtrls(i).Ctrl Is ci.Container Then
                        mvarCtrls(i).Children = mvarCtrls(i).Children + 1
                        Exit For
                    End If
                Next i
            Next inx
        End If
    End If
End Sub
Private Sub BuildAccessMenus()
    If Not (mnuAccess Is Nothing) Then Exit Sub ' only needed once

    Set mnuZoom = New PTMenu
    mnuZoom.Caption = "Select Window &Size"
    Set mnu85 = mnuZoom.AddChoice("&85%")
    Set mnu100 = mnuZoom.AddChoice("&100%")
    Set mnu125 = mnuZoom.AddChoice("1&25%")
    Set mnu150 = mnuZoom.AddChoice("1&50%")

    Set mnuAccess = New PTMenu
    Set mnuGumby = mnuAccess.AddChoice("&Auto-Scale On-Screen Items When Window is Resized")
    mnuGumby.Shortcut = "F2"
    mnuAccess.AddSubMenu mnuZoom
    Set mnu1to1 = mnuAccess.AddChoice("Restore Window Size to &100%")
    mnu1to1.Shortcut = "F3"
    mnuAccess.AddSeparator

    Set mnuMinToSysTray = mnuAccess.AddChoice("Display Icon in System &Tray when Mimimized")
    Set mnuRemember = mnuAccess.AddChoice("&Recall Most Recent Window Settings Next Time Program Starts")

    mnuAccess.AddSeparator

    Set mnuCancel = mnuAccess.AddChoice("&Cancel")

End Sub
Public Sub ConstructorEx(Frm As Form, Optional Title As String = "", Optional MinScaleHorz As Single = 0.85, Optional MinScaleVert As Single = 0.85, Optional ID As Long = -1)
    Dim s As String
    Dim Reg As Registry

    Me.Constructor Frm, MinScaleHorz, MinScaleVert, ID

    If mvarIsForm0 Then
        Set Reg = New Registry
        Reg.Home
    End If

    s = Trim(Title)
    If Len(s) > 0 Then s = " - " & s
    mvarForm.Caption = App.FileDescription & s
    Me.SetFonts

    Me.Restore
    mvarSavePos = True
    mvarSaveSize = True
    mvarReady = True
    If mvarResizeDeferred > 0 Then Me.Resize
End Sub
Public Sub ReInitCtrl(Ctrl As Control)
    Dim inx As Long
    Dim ci As PTCtrl
    Dim i As Long
    Dim lim As Long

    If mvarCtrlCnt > 0 Then
        lim = UBound(mvarCtrls)
        For i = 0 To lim
            If mvarCtrls(i) Is Ctrl Then
                mvarCtrls(i).ReInit mvarScaleHorz, mvarScaleVert, mvarScaleFont
                Exit Sub
            End If
        Next i
    End If
End Sub
Private Sub ResizeCtrls(ByRef Container As Object, Optional ResizeAll As Boolean = False)
    Dim inx As Long
    Dim ci As PTCtrl
    Dim lim As Long
    Dim i As Long
    Dim s As String

    On Error Resume Next
    If mvarCtrlCnt > 0 Then
        lim = UBound(mvarCtrls)
        For inx = 0 To lim
            Set ci = mvarCtrls(inx)
            If ci.Container Is Container Then
                If ci.Visible Or ResizeAll Then
                    ci.Resize mvarScaleHorz, mvarScaleVert, mvarScaleFont
                    If ci.Children > 0 Then
                        ResizeCtrls ci.Ctrl, ResizeAll
                    End If
                End If
            End If
        Next inx
    End If
    DoEvents

End Sub
Public Function Revert(Optional ScaleHorz As Single = 1, Optional ScaleVert As Single = 1) As String
    Dim s As String

    mvarNoResizeCnt = mvarNoResizeCnt + 1
    mvarForm.Width = mvarOrigWidth * ScaleHorz
    mvarForm.Height = mvarOrigHeight * ScaleVert
    mvarNoResizeCnt = mvarNoResizeCnt - 1
    Me.Resize True

    s = FormatPercent(ScaleVert, 1) & "h x " & FormatPercent(ScaleHorz, 1) & "w"
    Revert = s
End Function
Public Sub MakeRound(Optional Radius As Long = 40)
    Dim hRgn1 As Long
    Dim hRgn2 As Long
    Dim rct As RECT

    GetClientRect mvarForm.hWnd, rct

    BeginPath mvarForm.hdc
'    RoundRect mvarForm.hdc, 0, 2, mvarForm.ScaleWidth / Screen.TwipsPerPixelX, (mvarForm.ScaleHeight / Screen.TwipsPerPixelY) - 10, 40, 40
    RoundRect mvarForm.hdc, rct.Left, rct.Top, rct.Right - 1, rct.Bottom - 1, 40, 40
    EndPath mvarForm.hdc

    hRgn1 = PathToRegion(mvarForm.hdc)
    GetRgnBox hRgn1, rct
    hRgn2 = CreateRectRgnIndirect(rct)
    CombineRgn hRgn2, hRgn2, hRgn1, RGN_AND
    DeleteObject hRgn1

    SetWindowRgn mvarForm.hWnd, hRgn2, True
End Sub
Public Sub MakeXParent(Level As Byte)
    Dim lOldStyle As Long

    If WinVer.Major < 5 Then Exit Sub

    lOldStyle = GetWindowLongA(mvarForm.hWnd, GWL_EXSTYLE)
    SetWindowLongA mvarForm.hWnd, GWL_EXSTYLE, lOldStyle Or WS_EX_LAYERED
    'SetWindowLong mvarForm.hwnd, GWL_EXSTYLE, lOldStyle Or WS_EX_LAYERED Or WS_EX_TRANSPARENT
    SetLayeredWindowAttributes mvarForm.hWnd, 0, Level, LWA_ALPHA
End Sub
Public Sub MakeGlass()
    Dim a As Long
    Dim b As Long
    Dim Ctl As Control
    Dim TppX As Single
    Dim TppY As Single

    TppX = Screen.TwipsPerPixelX
    TppY = Screen.TwipsPerPixelY

    a = CreateRectRgn(0, 0, mvarForm.Width / 14, mvarForm.Height / 14)
    b = CreateRectRgn(4, 23, (mvarForm.Width / TppX) - 4, (mvarForm.Height / TppY) - 4)

    CombineRgn a, a, b, RGN_DIFF
    For Each Ctl In mvarForm.Controls
        b = CreateRectRgn((Ctl.Left / TppX) + 3, (Ctl.Top / TppY) + 23, (Ctl.Left + Ctl.Width) / TppX + 3, ((Ctl.Top + Ctl.Height) / TppY) + 23)
        CombineRgn a, a, b, RGN_OR
    Next Ctl
    SetWindowRgn mvarForm.hWnd, a, True
    DeleteObject b
End Sub
Public Property Let ShowInTaskbar(ByVal NewValue As Boolean)
    mvarShowInTaskbar = NewValue
    SetStyle mvarBorderStyle, mvarClipControls, mvarControlBox, mvarMaxButton, mvarMinButton, mvarShowInTaskbar, mvarWhatsThisButton
End Property
Public Property Get ShowInTaskbar() As Boolean
    ShowInTaskbar = mvarShowInTaskbar
End Property
Public Property Let MaxButton(ByVal NewValue As Boolean)
    mvarMaxButton = NewValue
    SetStyle mvarBorderStyle, mvarClipControls, mvarControlBox, mvarMaxButton, mvarMinButton, mvarShowInTaskbar, mvarWhatsThisButton
End Property
Public Property Get MaxButton() As Boolean
    MaxButton = mvarMaxButton
End Property
Public Property Let MinButton(ByVal NewValue As Boolean)
    mvarMinButton = NewValue
    SetStyle mvarBorderStyle, mvarClipControls, mvarControlBox, mvarMaxButton, mvarMinButton, mvarShowInTaskbar, mvarWhatsThisButton
End Property
Public Property Get MinButton() As Boolean
    MinButton = mvarMinButton
End Property
Public Property Let ControlBox(ByVal NewValue As Boolean)
    mvarControlBox = NewValue
    SetStyle mvarBorderStyle, mvarClipControls, mvarControlBox, mvarMaxButton, mvarMinButton, mvarShowInTaskbar, mvarWhatsThisButton
End Property
Public Property Get ControlBox() As Boolean
    ControlBox = mvarControlBox
End Property
Public Property Let WhatsThisButton(ByVal NewValue As Boolean)
    mvarWhatsThisButton = NewValue
    SetStyle mvarBorderStyle, mvarClipControls, mvarControlBox, mvarMaxButton, mvarMinButton, mvarShowInTaskbar, mvarWhatsThisButton
End Property
Public Property Get WhatsThisButton() As Boolean
    WhatsThisButton = mvarWhatsThisButton
End Property
Public Property Let ClipControls(ByVal NewValue As Boolean)
    mvarClipControls = NewValue
    SetStyle mvarBorderStyle, mvarClipControls, mvarControlBox, mvarMaxButton, mvarMinButton, mvarShowInTaskbar, mvarWhatsThisButton
End Property
Public Property Get ClipControls() As Boolean
    ClipControls = mvarClipControls
End Property
Public Property Let BorderStyle(ByVal NewValue As FormBorderStyleConstants)
    mvarBorderStyle = NewValue
    SetStyle mvarBorderStyle, mvarClipControls, mvarControlBox, mvarMaxButton, mvarMinButton, mvarShowInTaskbar, mvarWhatsThisButton
    Me.Resize
End Property
Public Property Get BorderStyle() As FormBorderStyleConstants
    BorderStyle = mvarBorderStyle
End Property
Public Sub ReInit()
    Dim ci As PTCtrl
    Dim i As Long
    Dim Diff As Single
    Dim Orig As Single
    Dim lim As Long

    mvarOrigLeft = mvarForm.Left / mvarScaleHorz
    mvarOrigTop = mvarForm.Top / mvarScaleVert
    mvarOrigHeight = mvarForm.Height / mvarScaleVert
    mvarOrigWidth = mvarForm.Width / mvarScaleHorz
    mvarOrigFontSize = mvarForm.FontSize / mvarScaleFont

    If mvarCtrlCnt > 0 Then
        lim = UBound(mvarCtrls)
        For i = 0 To lim
            Set ci = mvarCtrls(i)
            ci.ReInit mvarScaleHorz, mvarScaleVert, mvarScaleFont
        Next i
    End If
End Sub
Public Sub SetStyle(BorderStyle As FormBorderStyleConstants, ClipControls As Boolean, ControlBox As Boolean, MaxButton As Boolean, MinButton As Boolean, ShowInTaskbar As Boolean, WhatsThisButton As Boolean)
    Dim Style As Long
    Dim StyleEx As Long

    Style = WS_CLIPSIBLINGS
    StyleEx = 0
    Select Case BorderStyle
        Case vbBSNone:
            Style = Style Or WS_CLIPSIBLINGS
            StyleEx = StyleEx Or 0

        Case vbFixedSingle:
            Style = Style Or WS_BORDER Or WS_DLGFRAME Or WS_GROUP Or WS_TABSTOP Or WS_CAPTION
            If ControlBox Then Style = Style Or WS_SYSMENU
            If MaxButton Then Style = Style Or WS_MAXIMIZEBOX
            If MinButton Then Style = Style Or WS_MINIMIZEBOX

            StyleEx = StyleEx Or WS_EX_WINDOWEDGE
            If ShowInTaskbar Then StyleEx = StyleEx Or WS_EX_APPWINDOW
            If WhatsThisButton Then StyleEx = StyleEx Or WS_EX_CONTEXTHELP

        Case vbSizable:
            Style = Style Or WS_BORDER Or WS_DLGFRAME Or WS_THICKFRAME Or WS_GROUP Or WS_TABSTOP Or WS_CAPTION

            If ControlBox Then Style = Style Or WS_SYSMENU
            If MaxButton Then Style = Style Or WS_MAXIMIZEBOX
            If MinButton Then Style = Style Or WS_MINIMIZEBOX

            StyleEx = StyleEx Or WS_EX_WINDOWEDGE
            If ShowInTaskbar Then StyleEx = StyleEx Or WS_EX_APPWINDOW
            If WhatsThisButton Then StyleEx = StyleEx Or WS_EX_CONTEXTHELP

        Case vbFixedDialog:
            Style = Style Or WS_BORDER Or WS_DLGFRAME Or WS_CAPTION Or DS_MODALFRAME
            If ControlBox Then Style = Style Or WS_SYSMENU

            StyleEx = StyleEx Or WS_EX_DLGMODALFRAME Or WS_EX_WINDOWEDGE
            If WhatsThisButton Then StyleEx = StyleEx Or WS_EX_CONTEXTHELP

        Case vbFixedToolWindow:
            Style = Style Or WS_BORDER Or WS_DLGFRAME Or WS_GROUP Or WS_TABSTOP Or WS_CAPTION
            If ControlBox Then Style = Style Or WS_SYSMENU
            If MaxButton Then Style = Style Or WS_MAXIMIZEBOX
            If MinButton Then Style = Style Or WS_MINIMIZEBOX

            StyleEx = StyleEx Or WS_EX_TOOLWINDOW Or WS_EX_WINDOWEDGE
            If mvarForm.WhatsThisButton Then StyleEx = StyleEx Or WS_EX_CONTEXTHELP

        Case vbSizableToolWindow:
            Style = Style Or WS_BORDER Or WS_DLGFRAME Or WS_THICKFRAME Or WS_GROUP Or WS_TABSTOP Or WS_CAPTION
            If ControlBox Then Style = Style Or WS_SYSMENU
            If MaxButton Then Style = Style Or WS_MAXIMIZEBOX
            If MinButton Then Style = Style Or WS_MINIMIZEBOX

            StyleEx = StyleEx Or WS_EX_TOOLWINDOW Or WS_EX_WINDOWEDGE
            If WhatsThisButton Then StyleEx = StyleEx Or WS_EX_CONTEXTHELP

    End Select

    If mvarForm.Visible Then Style = Style Or WS_VISIBLE
    If ClipControls Then Style = Style Or WS_CLIPCHILDREN

    SetWindowLongA mvarForm.hWnd, GWL_STYLE, Style
    SetWindowLongA mvarForm.hWnd, GWL_EXSTYLE, StyleEx

    SetWindowPos mvarForm.hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED Or SWP_NOMOVE Or SWP_NOSIZE Or SWP_NOZORDER
    mvarForm.Refresh
End Sub
Public Function AddSplitter(CtrlA As Control, SplitCtrl As Control, CtrlB As Control, Optional SplitterName As String = "") As PTSplit
    Dim i As Long
    Dim InxA As Long
    Dim InxSplit As Long
    Dim InxB As Long
    Dim lim As Long
    Dim inx As Long

    If mvarCtrlCnt <= 0 Then
        Set AddSplitter = Nothing
        Exit Function
    End If

    InxA = -1
    InxSplit = -1
    InxB = -1
    lim = UBound(mvarCtrls)
    For i = 0 To lim
        If mvarCtrls(i).Ctrl Is CtrlA Then
            InxA = i
        ElseIf mvarCtrls(i).Ctrl Is SplitCtrl Then
            InxSplit = i
        ElseIf mvarCtrls(i).Ctrl Is CtrlB Then
            InxB = i
        End If
        If (InxA >= 0) And (InxSplit >= 0) And (InxB >= 0) Then Exit For
    Next i

    inx = SplitCnt
    SplitCnt = SplitCnt + 1
    ReDim Preserve Splits(inx)
    Set Splits(inx) = New PTSplit
    Splits(inx).Constructor mvarCtrls(InxA), mvarCtrls(InxSplit), mvarCtrls(InxB), SplitCnt
    If Len(SplitterName) > 0 Then
        Splits(inx).Name = SplitterName
    End If
    Set AddSplitter = Splits(inx)

    If mvarSaveSize Then
        Splits(inx).Restore RegMeKey
    End If
End Function
Public Function AddStretcher(StretchCtrl As Control, TargetCtrl As Control, Optional StretcherName As String = "") As PTStretch
    Dim InxTarget As Long
    Dim InxStretch As Long
    Dim i As Long
    Dim lim As Long
    Dim inx As Long

    If mvarCtrlCnt <= 0 Then
        Set AddStretcher = Nothing
        Exit Function
    End If

    InxTarget = -1
    InxStretch = -1
    lim = UBound(mvarCtrls)
    For i = 0 To lim
        If mvarCtrls(i).Ctrl Is StretchCtrl Then
            InxStretch = i
        ElseIf mvarCtrls(i).Ctrl Is TargetCtrl Then
            InxTarget = i
        End If
        If (InxStretch >= 0) And (InxTarget >= 0) Then Exit For
    Next i

    inx = StretchCnt
    StretchCnt = StretchCnt + 1
    ReDim Preserve Stretches(inx)
    Set Stretches(inx) = New PTStretch
    Stretches(inx).Constructor mvarCtrls(InxStretch), mvarCtrls(InxTarget), StretchCnt
    If Len(StretcherName) > 0 Then
        Stretches(inx).Name = StretcherName
    End If
    Set AddStretcher = Stretches(inx)

    If mvarSaveSize Then
        Stretches(inx).Restore RegMeKey
    End If
End Function
Public Property Let Ready(ByVal NewValue As Boolean)
    mvarReady = NewValue
End Property
Public Property Get Ready() As Boolean
    Ready = mvarReady
End Property
Public Property Let Resizing(ByVal NewValue As Boolean)
    mvarResizing = NewValue
End Property
Public Property Get Resizing() As Boolean
    Resizing = mvarResizing
End Property
Public Property Let NoResize(ByVal NewValue As Boolean)
    If NewValue Then
        mvarNoResizeCnt = mvarNoResizeCnt + 1
    Else
        mvarNoResizeCnt = mvarNoResizeCnt - 1
        If mvarNoResizeCnt < 0 Then mvarNoResizeCnt = 0
    End If
End Property
Public Property Get NoResize() As Boolean
    If mvarNoResizeCnt > 0 Then
        NoResize = True
    Else
        NoResize = False
    End If
End Property
Public Property Get MinWidth() As Single
    MinWidth = mvarMinWidth
End Property
Public Property Let MinWidth(ByVal NewValue As Single)
    mvarMinWidth = NewValue
    If mvarWidth > mvarMinWidth Then
        mvarForm.Width = mvarMinWidth
    End If
End Property
Public Property Get MinHeight() As Single
    MinHeight = mvarMinHeight
End Property
Public Property Let MinHeight(ByVal NewValue As Single)
    mvarMinHeight = NewValue
    If mvarHeight > mvarMinHeight Then
        mvarForm.Height = mvarMinHeight
    End If
End Property
Public Property Let Width(ByVal NewValue As Single)
    mvarForm.Width = NewValue
End Property
Public Property Get Width() As Single
    Width = mvarForm.Width
End Property
Public Property Let Height(ByVal NewValue As Single)
    mvarForm.Height = NewValue
End Property
Public Property Get Height() As Single
    Height = mvarForm.Height
End Property
Public Property Let Left(ByVal NewValue As Single)
    mvarForm.Left = NewValue
End Property
Public Property Get Left() As Single
    Left = mvarForm.Left
End Property
Public Property Let Top(ByVal NewValue As Single)
    mvarForm.Top = NewValue
End Property
Public Property Get Top() As Single
    Top = mvarForm.Top
End Property
Public Function Resize(Optional ResizeAll As Boolean = True) As String
    Dim i As Long
    Dim Diff As Single
    Dim s As String
    Dim lim As Long
    Dim Cancel As Boolean
    Dim ci As PTCtrl
    Dim inx As Long

    If Not mvarGumby Then
        RaiseEvent BeforeResize(Cancel)
        If Not Cancel Then
            RaiseEvent Resize
            RaiseEvent AfterResize
        End If
        Exit Function
    End If
        
    s = " " & FormatPercent(mvarScaleVert, 0) & "h x " & FormatPercent(mvarScaleHorz, 0) & "w "
    If (mvarForm.WindowState = vbMinimized) Or mvarResizing Or (Not mvarReady) Or Me.NoResize Then
        If Not (pnlAccess Is Nothing) Then pnlAccess.Text = s
        s = s & " - " & ACCESS_TIP
        If Not (imgAccess Is Nothing) Then imgAccess.ToolTipText = s
        If Not (picAccess Is Nothing) Then picAccess.ToolTipText = s
        If Not (btnAccess Is Nothing) Then btnAccess.ToolTipText = s
        Resize = s
        mvarResizeDeferred = mvarResizeDeferred + 1
        Exit Function
    End If

    RaiseEvent BeforeResize(Cancel)
    If Cancel Then
        mvarResizeDeferred = mvarResizeDeferred + 1
        Exit Function
    End If
    mvarResizing = True

    If mvarForm.Width < mvarMinWidth Then mvarForm.Width = mvarMinWidth
    If mvarForm.Height < mvarMinHeight Then mvarForm.Height = mvarMinHeight

    mvarScaleHorz = (mvarForm.Width - mvarStretchWidth) / mvarOrigWidth
    mvarScaleVert = (mvarForm.Height - mvarStretchHeight) / mvarOrigHeight
    If mvarScaleHorz > mvarScaleVert Then
        mvarScaleFont = mvarScaleVert
    Else
        mvarScaleFont = mvarScaleHorz
    End If

    mvarFontSize = mvarOrigFontSize * mvarScaleFont
    mvarForm.FontSize = mvarFontSize
    
    If mvarCtrlCnt > 0 Then
        ResizeCtrls mvarForm, ResizeAll
        lim = UBound(mvarCtrls) ' for some reason, can't do this in ResizeCtrls (DrawFocusRect leaves droppings)
        For inx = 0 To lim
            Set ci = mvarCtrls(inx)
            If ci.NeedsFocusRect Then
                ci.DrawFocusRect
                Exit For
            End If
        Next inx
    End If

    If SplitCnt > 0 Then ' fix-up splitters
        lim = UBound(Splits)
        For i = 0 To lim
            Splits(i).Resize
        Next i
    End If

    If StretchCnt > 0 Then
        lim = UBound(Stretches)
        For i = 0 To lim
            Stretches(i).Resize
        Next i
    End If

    DoEvents
    mvarResizing = False
    mvarResizeDeferred = 0

'    Resize = FormatPercent(mvarScaleFont, 1)
    RaiseEvent Resize
    s = " " & FormatPercent(mvarScaleVert, 0) & "h x " & FormatPercent(mvarScaleHorz, 0) & "w "
    If Not (pnlAccess Is Nothing) Then pnlAccess.Text = s
    s = s & " - " & ACCESS_TIP
    If Not (imgAccess Is Nothing) Then imgAccess.ToolTipText = s
    If Not (picAccess Is Nothing) Then picAccess.ToolTipText = s
    If Not (btnAccess Is Nothing) Then btnAccess.ToolTipText = s
    Resize = s
    RaiseEvent AfterResize
End Function
Public Sub Unload()
    Dim i As Long
    Dim lim As Long
    Dim DataName As String

    Me.Save

    If SplitCnt > 0 Then
        lim = UBound(Splits)
        For i = 0 To lim
            Set Splits(i) = Nothing
        Next i
        Erase Splits
        SplitCnt = 0
    End If

    If StretchCnt > 0 Then
        lim = UBound(Stretches)
        For i = 0 To lim
            Set Stretches(i) = Nothing
        Next i
        Erase Stretches
        StretchCnt = 0
    End If

    If mvarCtrlCnt > 0 Then
        lim = UBound(mvarCtrls)
        For i = 0 To lim
            Set mvarCtrls(i) = Nothing
        Next i
        mvarCtrlCnt = 0
    End If

    Erase mvarCtrls
    mvarCtrlCnt = 0

    Set mnu85 = Nothing
    Set mnu100 = Nothing
    Set mnu125 = Nothing
    Set mnu150 = Nothing
    Set mnuZoom = Nothing

    Set mnu1to1 = Nothing
    Set mnuGumby = Nothing
    Set mnuRemember = Nothing
    Set mnuCancel = Nothing
    Set mnuAccess = Nothing

    Set pnlAccess = Nothing
    Set sbAccess = Nothing
    Set imgAccess = Nothing
    Set picAccess = Nothing

    Set mvarForm = Nothing

    mvarSaveSize = False
    mvarSavePos = False
    mvarIsUnloaded = True
End Sub
Private Sub btnAccess_Click()
    Dim Choice As Long
    
    Choice = GetAccessChoice
End Sub
Private Sub btnAccess_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If (Button And vbLeftButton) = 0 Then Exit Sub

    Dim Choice As Long
    Dim PicUp As StdPicture

    Set PicUp = btnAccess.Picture
    Set btnAccess.Picture = btnAccess.DownPicture
    Choice = GetAccessChoice
    btnAccess.Picture = PicUp
End Sub
Private Sub Class_Initialize()

    WinVer = ApiWinVersion()

    Erase mvarCtrls
    mvarCtrlCnt = 0
    Erase Splits
    SplitCnt = 0
    Erase Stretches
    StretchCnt = 0

    mvarResizeStrategy = PTFORM_RESIZE_AUTO
    mvarRemember = True
    mvarIsUnloaded = False
End Sub
Private Sub Class_Terminate()
'    Set mvarCtrls = Nothing
    If mvarIsUnloaded Then Me.Unload
End Sub
Private Sub imgAccess_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If (Button And vbLeftButton) = 0 Then Exit Sub

    Dim Choice As Long

    Set imgAccess.Picture = FormAccessDown
    Choice = GetAccessChoice
    Set imgAccess.Picture = FormAccessUp
End Sub
Private Function GetAccessChoice(Optional hCtrl As Long = 0) As Long
    Dim Choice As Long
    Dim ScaleHorz As Single
    Dim ScaleVert As Single

    mnuGumby.Checked = mvarGumby

    mnu85.CheckedRadio = False
    mnu100.CheckedRadio = False
    mnu125.CheckedRadio = False
    mnu150.CheckedRadio = False

    ScaleHorz = Round(mvarScaleHorz, 2)
    ScaleVert = Round(mvarScaleVert, 2)

    If ScaleHorz = ScaleVert Then
        If ScaleHorz = 0.85 Then
            mnu85.CheckedRadio = True
        ElseIf ScaleHorz = 1 Then
            mnu100.CheckedRadio = True
        ElseIf ScaleHorz = 1.25 Then
            mnu125.CheckedRadio = True
        ElseIf ScaleHorz = 1.5 Then
            mnu150.CheckedRadio = True
        End If
    End If

    If mvarGumby Then
        If (ScaleHorz <> 1) Or (ScaleVert <> 1) Then
            mnu1to1.Enabled = True
            mnu1to1.Default = True
        Else
            mnu1to1.Default = False
            mnu1to1.Enabled = False
        End If
        mnu85.Enabled = True
        mnu100.Enabled = True
        mnu125.Enabled = True
        mnu150.Enabled = True
    Else
        mnu1to1.Enabled = False
        mnu85.Enabled = False
        mnu100.Enabled = False
        mnu125.Enabled = False
        mnu150.Enabled = False
    End If

    If mvarRemember Then
        mnuRemember.Checked = True
    Else
        mnuRemember.Checked = False
    End If

    If mvarMinToSysTray Then
        mnuMinToSysTray.Checked = True
    Else
        mnuMinToSysTray.Checked = False
    End If

    Choice = mnuAccess.DispPopup(hCtrl)
    GetAccessChoice = Choice
End Function
Private Sub mnu100_Clicked(ItemID As Long, Caption As String, Checked As Boolean)
    Me.Revert 1, 1
End Sub
Private Sub mnu125_Clicked(ItemID As Long, Caption As String, Checked As Boolean)
    Me.Revert 1.25, 1.25
End Sub
Private Sub mnu150_Clicked(ItemID As Long, Caption As String, Checked As Boolean)
    Me.Revert 1.5, 1.5
End Sub
Private Sub mnu1to1_Clicked(ItemID As Long, Caption As String, Checked As Boolean)
    Me.Revert 1, 1
End Sub
Private Sub mnu85_Clicked(ItemID As Long, Caption As String, Checked As Boolean)
    Me.Revert 0.85, 0.85
End Sub
Private Sub mnuGumby_Clicked(ItemID As Long, Caption As String, Checked As Boolean)
    Me.Gumby = Not Checked
End Sub
Public Property Get Gumby() As Boolean
    Gumby = mvarGumby
'    Select Case mvarBorderStyle
'        Case vbBSNone, vbFixedDialog, vbFixedSingle, vbFixedToolWindow:
'            Gumby = False
'        Case vbSizableToolWindow, vbSizable:
'            Gumby = True
'    End Select
End Property
Public Property Let Gumby(NewValue As Boolean)
    If NewValue Then
        Select Case mvarBorderStyle
            Case vbBSNone, vbFixedDialog, vbFixedSingle, vbSizable:
                Me.BorderStyle = vbSizable
            Case vbSizableToolWindow, vbFixedToolWindow:
                Me.BorderStyle = vbSizableToolWindow
        End Select
    Else
        Select Case mvarOrigBorderStyle
            Case vbFixedSingle, vbFixedDialog, vbBSNone, vbFixedToolWindow
                Me.BorderStyle = mvarOrigBorderStyle
                Me.Revert 1, 1 ' reset form size to 100%

            Case vbSizable:
                Me.BorderStyle = mvarOrigBorderStyle
            Case vbSizableToolWindow:
                Me.BorderStyle = mvarOrigBorderStyle
        End Select
    End If

    mvarGumby = NewValue
End Property
Private Sub mnuMinToSysTray_Clicked(ItemID As Long, Caption As String, Checked As Boolean)
    mvarMinToSysTray = Not Checked
End Sub
Private Sub mnuRemember_Clicked(ItemID As Long, Caption As String, Checked As Boolean)
    mvarRemember = Not Checked
End Sub
Private Sub mvarForm_KeyDown(KeyCode As Integer, Shift As Integer)
    Select Case KeyCode
        Case vbKeyF1:
            If ApiHelpEnabled Then
                mvarForm.btnHelp_Click
            Else
                frmAbout.Show vbModal, mvarForm
            End If

        Case vbKeyF2:
            Me.Gumby = Not mvarGumby

        Case vbKeyF3:
            Me.Revert 1, 1

'        Case vbKeyF24:
'            WakeUpWindow Me
'            Exit Sub

    End Select

    RaiseEvent KeyDown(KeyCode, Shift)
End Sub
Private Sub mvarForm_KeyPress(KeyAscii As Integer)
    RaiseEvent KeyPress(KeyAscii)
End Sub
Private Sub mvarForm_KeyUp(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub
Private Sub mvarForm_Load()
    RaiseEvent Load
End Sub
Private Sub mvarForm_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    RaiseEvent QueryUnload(Cancel, UnloadMode)
End Sub
Private Sub mvarForm_Resize()
    Dim Cancel As Boolean
    Dim DoIt As Boolean

    If mvarForm.WindowState = vbMinimized Then
        If mvarMinToSysTray Then
            If TrayHandle = 0 Then
                TrayHandle = TrayAdd(Me, "Show " & mvarForm.Caption)
            End If
            mvarForm.Hide
            TrayShow TrayHandle
            Exit Sub
        End If
    End If

    If mvarResizing Or Me.NoResize Or (Not mvarReady) Then
        mvarResizeDeferred = mvarResizeDeferred + 1
        Exit Sub
    End If

    DoIt = False
    If mvarHeight <> mvarForm.Height Then
        DoIt = True
    ElseIf mvarWidth <> mvarForm.Width Then
        DoIt = True
'    ElseIf mvarTop <> mvarForm.Top Then
'        DoIt = True
'    ElseIf mvarLeft <> mvarForm.Left Then
'        DoIt = True
    End If
    If Not DoIt Then Exit Sub

    If mvarGumby Then
        Select Case mvarResizeStrategy
            Case PTFORM_RESIZE_AUTO:
                Resize True
            Case PTFORM_RESIZE_2STAGE:
                Resize False
                DoEvents
                Resize True
            Case PTFORM_RESIZE_MANUAL:
    
        End Select
    Else
        RaiseEvent BeforeResize(Cancel)
        If Not Cancel Then
            RaiseEvent Resize
            RaiseEvent AfterResize
        End If
    End If

    mvarTop = mvarForm.Top
    mvarLeft = mvarForm.Left
    mvarHeight = mvarForm.Height
    mvarWidth = mvarForm.Width
End Sub
Public Sub TrayEvent(TrayID As Long, Action As PT_MOUSE_ACTION)
    Select Case Action
'        Case PT_LBUTTONDBLCLK:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_LBUTTONDBLCLK"

        Case PT_LBUTTONDOWN:
            Debug.Print Now & " TrayEvent: " & CStr(TrayID) & "-" & "PT_LBUTTONDOWN"
            mvarMinToSysTray = False
            Me.NoResize = True
            mvarForm.Visible = True
            mvarForm.WindowState = vbNormal
            Me.IsTop = True
            If TrayHandle > 0 Then
                TrayHandle = TrayDelete(TrayHandle)
            End If
            mvarMinToSysTray = True
            Me.NoResize = False

        Case PT_LBUTTONUP:
'            PTMe.Visible = True
'            PTMe.WindowState = vbNormal
'            PTMe.IsTop = True
'            If TrayHandle > 0 Then TrayHandle = TrayDelete(TrayHandle)
            Debug.Print Now & " TrayEvent: " & CStr(TrayID) & "-" & "PT_LBUTTONUP"

'        Case PT_RBUTTONDBLCLK:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_RBUTTONDBLCLK"
'
'        Case PT_RBUTTONDOWN:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_RBUTTONDOWN"
'
'        Case PT_RBUTTONUP:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_RBUTTONUP"
'            PopupMenu mnuTray

    End Select
    RaiseEvent TrayEvent(TrayID, Action)
End Sub
Private Sub mvarForm_Unload(Cancel As Integer)
    RaiseEvent Unload(Cancel)
    If Cancel <> 0 Then Exit Sub
End Sub
Private Sub picAccess_GotFocus()
    picAccess.DrawFocusRect
End Sub
Private Sub picAccess_LostFocus()
    picAccess.EraseFocusRect
End Sub
Private Sub picAccess_KeyPress(KeyAscii As Integer)
    Dim Choice As Long

    Set picAccess.Picture = FormAccessDown
    Choice = GetAccessChoice(picAccess.hWnd)
    Set picAccess.Picture = FormAccessUp
    picAccess.DrawFocusRect
End Sub
Private Sub picAccess_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If (Button And vbLeftButton) = 0 Then Exit Sub

    Dim Choice As Long

    Set picAccess.Picture = FormAccessDown
    Choice = GetAccessChoice(picAccess.hWnd)
    Set picAccess.Picture = FormAccessUp
    picAccess.DrawFocusRect
End Sub
Private Sub sbAccess_PanelClick(ByVal Panel As MSComctlLib.Panel)
    If Panel <> pnlAccess Then Exit Sub

    Dim Choice As Long
    Choice = GetAccessChoice
End Sub
