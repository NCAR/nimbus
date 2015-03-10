Attribute VB_Name = "SysTrayHelper"
Option Explicit
' Copyright 2003-2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 1.3 1/22/2004

' Requires Tray.frm
' Requires project reference to "OLE Automation - stdole2.tlb"

'Public Sub TrayEvent(TrayID As Long, Action As PT_MOUSE_ACTION)
'    Select Case Action
'        Case PT_LBUTTONDBLCLK:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_LBUTTONDBLCLK"
'
'        Case PT_LBUTTONDOWN:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_LBUTTONDOWN"
'            Me.Visible = True
'            Me.WindowState = vbNormal
'            ApiWindowTop Me.hWnd
'            If TrayHandle > 0 Then TrayHandle = TrayDelete(TrayHandle)
'
'        Case PT_LBUTTONUP:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_LBUTTONUP"
'
'        Case PT_RBUTTONDBLCLK:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_RBUTTONDBLCLK"
'
'        Case PT_RBUTTONDOWN:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_RBUTTONDOWN"
'
'        Case PT_RBUTTONUP:
''            Debug.Print "TrayEvent: " & CStr(TrayID) & "-" & "PT_RBUTTONUP"
'            PopupMenu mnuTray
'
'    End Select
'End Sub

'Private Sub Form_Resize()
'    If Me.WindowState = vbMinimized Then
'        If SysTray And (TrayHandle <= 0) Then
'            TrayHandle = TrayAdd(Me, App.FileDescription)
'            Me.Visible = False
'        End If
'        Exit Sub
'    End If
'    If TrayHandle > 0 Then
'        TrayHandle = TrayDelete(TrayHandle)
'        Me.Visible = True
'    End If
'
'    If Me.Width < MinWidth Then
'        Me.Width = MinWidth
'        Exit Sub
'    End If
'    If Me.Height < MinHeight Then
'        Me.Height = MinHeight
'        Exit Sub
'    End If

Public Enum PT_MOUSE_ACTION
    PT_MOUSEMOVE = &H200
    PT_LBUTTONDOWN = &H201      'Button down
    PT_LBUTTONUP = &H202        'Button up
    PT_LBUTTONDBLCLK = &H203    'Double-click
    PT_RBUTTONDOWN = &H204      'Button down
    PT_RBUTTONUP = &H205        'Button up
    PT_RBUTTONDBLCLK = &H206    'Double-click
    PT_MBUTTONDOWN = &H207
    PT_MBUTTONUP = &H208
    PT_MBUTTONDBLCLK = &H209
    PT_XBUTTONDOWN = &H20B
    PT_XBUTTONUP = &H20C
    PT_XBUTTONDBLCLK = &H20D
End Enum

Private Const WM_USER = &H400

Private Const NINF_KEY = &H1    ' WIN32_IE >= 0x0500
Private Const NIN_SELECT = (WM_USER + 0)    ' WIN32_IE >= 0x0500
Private Const NIN_KEYSELECT = (NIN_SELECT Or NINF_KEY)  ' WIN32_IE >= 0x0500
Private Const NIN_BALLOONSHOW = (WM_USER + 2)   ' WIN32_IE >= 0x0501 only
Private Const NIN_BALLOONHIDE = (WM_USER + 3)   ' WIN32_IE >= 0x0501 only
Private Const NIN_BALLOONTIMEOUT = (WM_USER + 4)    ' WIN32_IE >= 0x0501 only
Private Const NIN_BALLOONUSERCLICK = (WM_USER + 5)  ' WIN32_IE >= 0x0501 only

Private Const NIM_ADD = &H0
Private Const NIM_MODIFY = &H1
Private Const NIM_DELETE = &H2
Private Const NIM_SETFOCUS = &H3    ' WIN32_IE >= 0x0500 only
Private Const NIM_SETVERSION = &H4  ' ' WIN32_IE >= 0x0500 only

Private Const NIF_MESSAGE = &H1
Private Const NIF_ICON = &H2
Private Const NIF_TIP = &H4
Private Const NIF_STATE = &H8   ' WIN32_IE >= 0x0500 only
Private Const NIF_INFO = &H10   ' WIN32_IE >= 0x0500 only
Private Const NIF_GUID = &H20   ' WIN32_IE >= 0x600 only

Private Const NIS_HIDDEN = &H1  ' WIN32_IE >= 0x0500 only
Private Const NIS_SHAREDICON = &H2  ' WIN32_IE >= 0x0500 only

' Notify Icon Infotip flags
' icon flags are mutually exclusive
' and take only the lowest 2 bits
Private Const NIIF_NONE = &H0
Private Const NIIF_INFO = &H1
Private Const NIIF_WARNING = &H2
Private Const NIIF_ERROR = &H3
Private Const NIIF_ICON_MASK = &HF
Private Const NIIF_NOSOUND = &H10   ' WIN32_IE >= 0x0501 only

Private Const WM_MOUSEMOVE = &H200
Private Const WM_LBUTTONDOWN = &H201     'Button down
Private Const WM_LBUTTONUP = &H202       'Button up
Private Const WM_LBUTTONDBLCLK = &H203   'Double-click
Private Const WM_RBUTTONDOWN = &H204     'Button down
Private Const WM_RBUTTONUP = &H205       'Button up
Private Const WM_RBUTTONDBLCLK = &H206   'Double-click
Private Const WM_MBUTTONDOWN = &H207
Private Const WM_MBUTTONUP = &H208
Private Const WM_MBUTTONDBLCLK = &H209
Private Const WM_MOUSEWHEEL = &H20A
Private Const WM_XBUTTONDOWN = &H20B
Private Const WM_XBUTTONUP = &H20C
Private Const WM_XBUTTONDBLCLK = &H20D
Private Const WM_MOUSEHOVER = &H2A1
Private Const WM_MOUSELEAVE = &H2A3

Private Type GUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(0 To 7) As Byte
End Type

' Tray notification definitions
Private Type NOTIFY_ICON_DATA_A
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage  As Long
    hIcon  As Long
    szTip As String * 64
End Type

Private Type NOTIFY_ICON_DATA_A_500
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage  As Long
    hIcon  As Long
    szTip As String * 128
End Type

Private Type NOTIFY_ICON_DATA_A_510
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage  As Long
    hIcon  As Long
    szTip As String * 128
    dwState As Long
    dwStateMask As Long
    szInfo As String * 256
    uTimeout As Integer ' UINT
    uVersion As Integer ' UINT
    szInfoTitle As String * 64
    dwInfoFlags As Long
End Type

Private Type NOTIFY_ICON_DATA_A_600
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage  As Long
    hIcon  As Long
    szTip As String * 128
    dwState As Long
    dwStateMask As Long
    szInfo As String * 256
    uTimeout As Integer ' UINT
    uVersion As Integer ' UINT
    szInfoTitle As String * 64
    dwInfoFlags As Long
    guidItem As GUID
End Type

Private Type NOTIFY_ICON_DATA_W
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage  As Long
    hIcon  As Long
    szTip(127) As Byte
End Type

Private Type NOTIFY_ICON_DATA_W_500
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage  As Long
    hIcon  As Long
    szTip(255) As Byte
End Type

Private Type NOTIFY_ICON_DATA_W_501
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage  As Long
    hIcon  As Long
    szTip(255) As Byte
    dwState As Long
    dwStateMask As Long
    szInfo(511) As Byte
    uTimeout As Integer ' UINT
    uVersion As Integer ' UINT
    szInfoTitle(127) As Byte
    dwInfoFlags As Long
End Type

Private Type NOTIFY_ICON_DATA_W_600
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage  As Long
    hIcon  As Long
    szTip(255) As Byte
    dwState As Long
    dwStateMask As Long
    szInfo(511) As Byte
    uTimeout As Integer ' UINT
    uVersion As Integer ' UINT
    szInfoTitle(127) As Byte
    dwInfoFlags As Long
    guidItem As GUID
End Type

Private Type NOTIFYICONDATA
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage As Long
    hIcon As Long
    szTip As String * 64
End Type

Private Type PT_ICON_INFO
    ID As Long
    HomeForm As Object
    TrayForm As frmTray
    hWnd As Long
    Visible As Boolean
    uCallbackMessage  As Long
    hIcon  As Long
    Icon As IPictureDisp
    szTip As String
End Type

Private Declare Function SetForegroundWindow Lib "user32" ( _
    ByVal hWnd As Long) As Long

Private Declare Function Shell_NotifyIconA Lib "shell32" ( _
    ByVal dwMessage As Long, _
    pnid As NOTIFY_ICON_DATA_A) As Boolean

Private niCnt As Long
Private ni() As PT_ICON_INFO
Public Function TrayAdd(HomeForm As Object, Optional Tip As Variant = "", Optional Icon As Variant) As Long
    Dim nid As NOTIFY_ICON_DATA_A
    Dim uFlags As Long
    Dim Frm As frmTray
    Dim hIcon As Long

    TrayAdd = -1
    If HomeForm Is Nothing Then Exit Function

    ReDim Preserve ni(niCnt)

    ni(niCnt).ID = niCnt + 1
    Set ni(niCnt).HomeForm = HomeForm
    Set ni(niCnt).TrayForm = New frmTray
    ni(niCnt).TrayForm.Caption = App.FileDescription & " - " & "System Tray Icon " & CStr(ni(niCnt).ID)
    Set ni(niCnt).TrayForm.HomeForm = ni(niCnt).HomeForm
    ni(niCnt).TrayForm.TrayID = ni(niCnt).ID

    ni(niCnt).uCallbackMessage = WM_MOUSEMOVE

    uFlags = NIF_MESSAGE
    If IsMissing(Tip) Then
        ni(niCnt).szTip = ""
    Else
        ni(niCnt).szTip = Tip
        uFlags = uFlags Or NIF_TIP
    End If
    If IsMissing(Icon) Then
        Set ni(niCnt).Icon = ni(niCnt).HomeForm.Icon
    Else
        Set ni(niCnt).Icon = GetIcon(Icon)
    End If
    If ni(niCnt).Icon Is Nothing Then
        Set ni(niCnt).Icon = ni(niCnt).HomeForm.Icon
    End If
    If Not (ni(niCnt).Icon Is Nothing) Then
        ni(niCnt).hIcon = ni(niCnt).Icon.Handle
        uFlags = uFlags Or NIF_ICON
    Else
        ni(niCnt).hIcon = 0
    End If

    nid.cbSize = LenB(nid)
    nid.uID = ni(niCnt).ID
    nid.hWnd = ni(niCnt).TrayForm.hWnd
    nid.uCallbackMessage = ni(niCnt).uCallbackMessage
    nid.hIcon = ni(niCnt).hIcon
    nid.szTip = ni(niCnt).szTip & vbNullChar
    nid.uFlags = uFlags
    Shell_NotifyIconA NIM_ADD, nid

    ni(niCnt).Visible = True
    niCnt = niCnt + 1
    TrayAdd = niCnt
End Function
Private Function GetIcon(Icon As Variant) As IPictureDisp

    If IsObject(Icon) Then ' handle
        Set GetIcon = Icon
        Exit Function
    End If

    Set GetIcon = LoadResPicture(Icon, vbResIcon)
End Function
Public Sub TrayShow(TrayHandle As Long, Optional hIcon As Long, Optional Tip As Variant = "")
    Dim nid As NOTIFY_ICON_DATA_A
    Dim uFlags As Long
    Dim nix As Long

    If TrayHandle > niCnt Then Exit Sub
    If TrayHandle <= 0 Then Exit Sub
    nix = TrayHandle - 1
    If ni(nix).TrayForm Is Nothing Then Exit Sub ' deleted

    uFlags = NIF_MESSAGE
    If ni(nix).Visible Then
        If IsMissing(Tip) And IsMissing(hIcon) Then Exit Sub
        If Not IsMissing(Tip) Then
            ni(nix).szTip = Tip
            uFlags = uFlags Or NIF_TIP
        End If
        If Not IsMissing(hIcon) Then
            ni(nix).hIcon = hIcon
            uFlags = uFlags Or NIF_ICON
        End If
    End If

    nid.cbSize = LenB(nid)
    nid.uID = ni(nix).ID
    nid.hWnd = ni(nix).TrayForm.hWnd
    nid.uCallbackMessage = ni(nix).uCallbackMessage ' WM_MOUSEMOVE
    nid.hIcon = ni(nix).hIcon
    nid.szTip = ni(nix).szTip & vbNullChar
    nid.uFlags = uFlags
    Shell_NotifyIconA NIM_ADD, nid

    ni(nix).Visible = True
End Sub
Public Sub TrayChangeIcon(TrayHandle As Long, Icon As Variant)
    Dim nid As NOTIFY_ICON_DATA_A
    Dim nix As Long

    If TrayHandle > niCnt Then Exit Sub
    If TrayHandle <= 0 Then Exit Sub
    nix = TrayHandle - 1
    If ni(nix).TrayForm Is Nothing Then Exit Sub ' deleted

    Set ni(nix).Icon = Nothing
    Set ni(nix).Icon = GetIcon(Icon)
    ni(nix).hIcon = ni(nix).Icon.Handle
    If Not ni(nix).Visible Then Exit Sub

    nid.cbSize = LenB(nid)
    nid.uID = ni(nix).ID
    nid.hWnd = ni(nix).TrayForm.hWnd
    nid.hIcon = ni(nix).hIcon
    nid.uFlags = NIF_ICON

    Shell_NotifyIconA NIM_MODIFY, nid
End Sub
Public Sub TrayChangeTip(TrayHandle As Long, Tip As String)
    Dim nid As NOTIFY_ICON_DATA_A
    Dim nix As Long

    If TrayHandle > niCnt Then Exit Sub
    If TrayHandle <= 0 Then Exit Sub
    If ni(nix).TrayForm Is Nothing Then Exit Sub ' deleted

    ni(nix).szTip = Tip
    If Not ni(nix).Visible Then Exit Sub

    nid.cbSize = LenB(nid)
    nid.uID = ni(nix).ID
    nid.hWnd = ni(nix).TrayForm.hWnd
    nid.szTip = ni(nix).szTip & vbNullChar
    nid.uFlags = NIF_TIP

    Shell_NotifyIconA NIM_MODIFY, nid
End Sub
Public Sub TrayHide(TrayHandle As Long)
    Dim nid As NOTIFY_ICON_DATA_A
    Dim nix As Long

    If TrayHandle > niCnt Then Exit Sub
    If TrayHandle <= 0 Then Exit Sub
    nix = TrayHandle - 1
    If ni(nix).TrayForm Is Nothing Then Exit Sub ' deleted
    If Not ni(nix).Visible Then Exit Sub

    nid.cbSize = LenB(nid)
    nid.uID = ni(nix).ID
    nid.hWnd = ni(nix).TrayForm.hWnd

    Shell_NotifyIconA NIM_DELETE, nid
    ni(nix).Visible = False
End Sub
Public Function TrayDelete(TrayHandle As Long, Optional Compact As Boolean = True) As Long
    Dim lim As Long
    Dim nix As Long

    TrayDelete = -1
    If TrayHandle > niCnt Then Exit Function
    If TrayHandle <= 0 Then Exit Function

    TrayDelete = 0
    nix = TrayHandle - 1
    If ni(nix).TrayForm Is Nothing Then Exit Function ' deleted

    TrayHide TrayHandle
    Unload ni(nix).TrayForm
    Set ni(nix).TrayForm = Nothing
    Set ni(nix).HomeForm = Nothing

    If Compact Then
        lim = UBound(ni)
        If nix = lim Then
            If nix > 0 Then    ' can physically remove entry
                lim = lim - 1
                ReDim Preserve ni(lim)
            End If
        End If
        If niCnt <= 0 Then Erase ni
    End If

    niCnt = niCnt - 1
End Function
Public Sub TrayDeleteAll()
    Dim i As Long

    If niCnt > 0 Then
        For i = 0 To UBound(ni)
            TrayDelete ni(i).ID, False
        Next i
    End If

    Erase ni
    niCnt = 0
End Sub
