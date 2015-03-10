Attribute VB_Name = "Combo"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 1.0 4/14/2004

Public Enum COMBO_ALIGN
    COMBO_ALIGN_RIGHT
    COMBO_ALIGN_LEFT
End Enum

' Extended Window Styles
Private Const WS_EX_DLGMODALFRAME = &H1&
Private Const WS_EX_NOPARENTNOTIFY = &H4&
Private Const WS_EX_TOPMOST = &H8&
Private Const WS_EX_ACCEPTFILES = &H10&
Private Const WS_EX_TRANSPARENT = &H20&
Private Const WS_EX_MDICHILD = &H40&
Private Const WS_EX_TOOLWINDOW = &H80&
Private Const WS_EX_WINDOWEDGE = &H100&
Private Const WS_EX_CLIENTEDGE = &H200&
Private Const WS_EX_CONTEXTHELP = &H400&
Private Const WS_EX_RIGHT = &H1000&
Private Const WS_EX_LEFT = &H0&
Private Const WS_EX_RTLREADING = &H2000&
Private Const WS_EX_LTRREADING = &H0&
Private Const WS_EX_LEFTSCROLLBAR = &H4000&
Private Const WS_EX_RIGHTSCROLLBAR = &H0&
Private Const WS_EX_CONTROLPARENT = &H10000
Private Const WS_EX_STATICEDGE = &H20000
Private Const WS_EX_APPWINDOW = &H40000
Private Const WS_EX_OVERLAPPEDWINDOW = (WS_EX_WINDOWEDGE Or WS_EX_CLIENTEDGE)
Private Const WS_EX_PALETTEWINDOW = (WS_EX_WINDOWEDGE Or WS_EX_TOOLWINDOW Or WS_EX_TOPMOST)
Private Const WS_EX_LAYERED = &H80000
Private Const WS_EX_NOINHERITLAYOUT = &H100000      ' Disable inheritence of mirroring by children
Private Const WS_EX_LAYOUTRTL = &H400000            ' Right to left mirroring
Private Const WS_EX_COMPOSITED = &H2000000
Private Const WS_EX_NOACTIVATE = &H8000000

' Window field offsets for GetWindowLong()
Private Const GWL_WNDPROC = (-4)
Private Const GWL_HINSTANCE = (-6)
Private Const GWL_HWNDPARENT = (-8)
Private Const GWL_STYLE = (-16)
Private Const GWL_EXSTYLE = (-20)
Private Const GWL_USERDATA = (-21)
Private Const GWL_ID = (-12)

' ComboBox messages
Private Const CB_GETEDITSEL = &H140
Private Const CB_LIMITTEXT = &H141
Private Const CB_SETEDITSEL = &H142
Private Const CB_ADDSTRING = &H143
Private Const CB_DELETESTRING = &H144
Private Const CB_DIR = &H145
Private Const CB_GETCOUNT = &H146
Private Const CB_GETCURSEL = &H147
Private Const CB_GETLBTEXT = &H148
Private Const CB_GETLBTEXTLEN = &H149
Private Const CB_INSERTSTRING = &H14A
Private Const CB_RESETCONTENT = &H14B
Private Const CB_FINDSTRING = &H14C
Private Const CB_SELECTSTRING = &H14D
Private Const CB_SETCURSEL = &H14E
Private Const CB_SHOWDROPDOWN = &H14F
Private Const CB_GETITEMDATA = &H150
Private Const CB_SETITEMDATA = &H151
Private Const CB_GETDROPPEDCONTROLRECT = &H152
Private Const CB_SETITEMHEIGHT = &H153
Private Const CB_GETITEMHEIGHT = &H154
Private Const CB_SETEXTENDEDUI = &H155
Private Const CB_GETEXTENDEDUI = &H156
Private Const CB_GETDROPPEDSTATE = &H157
Private Const CB_FINDSTRINGEXACT = &H158
Private Const CB_SETLOCALE = &H159
Private Const CB_GETLOCALE = &H15A
Private Const CB_GETTOPINDEX = &H15B
Private Const CB_SETTOPINDEX = &H15C
Private Const CB_GETHORIZONTALEXTENT = &H15D
Private Const CB_SETHORIZONTALEXTENT = &H15E
Private Const CB_GETDROPPEDWIDTH = &H15F
Private Const CB_SETDROPPEDWIDTH = &H160
Private Const CB_INITSTORAGE = &H161
Private Const CB_MULTIPLEADDSTRING = &H163
Private Const CB_GETCOMBOBOXINFO = &H164

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Type COMBOBOXINFO
    cbSize As Long
    rcItem As RECT
    rcButton As RECT
    stateButton As Long
    hwndCombo As Long
    hwndItem As Long
    hwndList As Long
End Type

Private Declare Function GetComboBoxInfo Lib "user32.dll" _
    (ByVal hwndCombo As Long, _
     ByRef pcbi As COMBOBOXINFO) As Long

Private Declare Function GetWindowLongA Lib "user32.dll" _
    (ByVal hwnd As Long, _
     ByVal nIndex As Long) As Long

Private Declare Function SetWindowLongA Lib "user32.dll" _
    (ByVal hwnd As Long, _
     ByVal nIndex As Long, _
     ByVal dwNewLong As Long) As Long

Private Declare Function SendMessageA Lib "user32.dll" _
    (ByVal hwnd As Long, _
     ByVal wMsg As Long, _
     ByVal wParam As Long, _
     ByRef lParam As Any) As Long
Public Sub ComboDropList(cbo As ComboBox)
   SendMessageA cbo.hwnd, CB_SHOWDROPDOWN, True, ByVal 0
End Sub
Public Sub ComboAlign(cbo As ComboBox, Optional Align As COMBO_ALIGN = COMBO_ALIGN_RIGHT)
    Dim result As Long
    Dim ExStyle As Long

    ExStyle = GetWindowLongA(cbo.hwnd, GWL_EXSTYLE)

    Select Case Align
        Case COMBO_ALIGN_RIGHT
            ExStyle = ExStyle Or WS_EX_RIGHT
        Case COMBO_ALIGN_LEFT
            ExStyle = ExStyle And Not WS_EX_RIGHT
    End Select

    result = SetWindowLongA(cbo.hwnd, GWL_EXSTYLE, ExStyle)
End Sub
Public Sub ComboAlignEdit(cbo As ComboBox, Optional Align As COMBO_ALIGN = COMBO_ALIGN_RIGHT)
    Dim result As Long
    Dim hnd As Long
    Dim ExStyle As Long

    hnd = ComboHandleEdit(cbo)

    If hnd = 0 Then Exit Sub

    ExStyle = GetWindowLongA(hnd, GWL_EXSTYLE)

    Select Case Align
        Case COMBO_ALIGN_RIGHT
            ExStyle = ExStyle Or WS_EX_RIGHT
        Case COMBO_ALIGN_LEFT
            ExStyle = ExStyle And Not WS_EX_RIGHT
    End Select

    result = SetWindowLongA(hnd, GWL_EXSTYLE, ExStyle)
End Sub
Public Sub ComboAlignList(cbo As ComboBox, Optional Align As COMBO_ALIGN = COMBO_ALIGN_RIGHT)
    Dim result As Long
    Dim hnd As Long
    Dim ExStyle As Long

    hnd = ComboHandleList(cbo)

    If hnd = 0 Then Exit Sub

    ExStyle = GetWindowLongA(hnd, GWL_EXSTYLE)

    Select Case Align
        Case COMBO_ALIGN_RIGHT
            ExStyle = ExStyle Or WS_EX_RIGHT
        Case COMBO_ALIGN_LEFT
            ExStyle = ExStyle And Not WS_EX_RIGHT
    End Select

    result = SetWindowLongA(hnd, GWL_EXSTYLE, ExStyle)
End Sub
Public Sub ComboAlignScroll(cbo As ComboBox, Optional Align As COMBO_ALIGN = COMBO_ALIGN_RIGHT)
    Dim result As Long
    Dim hnd As Long
    Dim ExStyle As Long

    hnd = ComboHandleList(cbo)

    If hnd = 0 Then Exit Sub

    ExStyle = GetWindowLongA(hnd, GWL_EXSTYLE)

    Select Case Align
        Case COMBO_ALIGN_RIGHT
            ExStyle = ExStyle Or WS_EX_LEFTSCROLLBAR

        Case COMBO_ALIGN_LEFT
            ExStyle = ExStyle And Not WS_EX_LEFTSCROLLBAR

    End Select

    result = SetWindowLongA(hnd, GWL_EXSTYLE, ExStyle)
End Sub
Public Sub ComboAlignButton(cbo As ComboBox, Optional Align As COMBO_ALIGN = COMBO_ALIGN_LEFT)
    Dim result As Long
    Dim ExStyle As Long
    Dim OrigAlign As COMBO_ALIGN

    ExStyle = GetWindowLongA(cbo.hwnd, GWL_EXSTYLE)

    If ExStyle And WS_EX_RIGHT Then
        OrigAlign = COMBO_ALIGN_RIGHT
    Else
        OrigAlign = COMBO_ALIGN_LEFT
    End If

    Select Case Align
        Case COMBO_ALIGN_RIGHT
            ExStyle = ExStyle And Not WS_EX_RIGHT
        Case COMBO_ALIGN_LEFT
            ExStyle = ExStyle Or WS_EX_RIGHT
    End Select

    result = SetWindowLongA(cbo.hwnd, GWL_EXSTYLE, ExStyle)

    Select Case OrigAlign
        Case COMBO_ALIGN_RIGHT
            ComboAlignList cbo, COMBO_ALIGN_RIGHT
        Case COMBO_ALIGN_LEFT
            ComboAlignList cbo, COMBO_ALIGN_LEFT
    End Select
End Sub
Private Function ComboHandleList(cbo As ComboBox) As Long
   Dim result As Long
   Dim cboi As COMBOBOXINFO

   cboi.cbSize = Len(cboi)
   result = GetComboBoxInfo(cbo.hwnd, cboi)
   ComboHandleList = cboi.hwndList
End Function
Private Function ComboHandleEdit(cbo As ComboBox) As Long
   Dim result As Long
   Dim cboi As COMBOBOXINFO

   cboi.cbSize = Len(cboi)
   result = GetComboBoxInfo(cbo.hwnd, cboi)
   ComboHandleEdit = cboi.hwndItem
End Function
Private Function ComboHandleBox(cbo As ComboBox) As Long
   Dim result As Long
   Dim cboi As COMBOBOXINFO

   cboi.cbSize = Len(cboi)
   result = GetComboBoxInfo(cbo.hwnd, cboi)
   ComboHandleBox = cboi.hwndCombo
End Function


