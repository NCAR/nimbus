VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PTMenu"
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
' Version 1.2 6/2/2004

Private Enum MENU_FLAG
    MF_INSERT = &H0&
    MF_CHANGE = &H80&
    MF_APPEND = &H100&
    MF_DELETE = &H200&
    MF_REMOVE = &H1000&
    
    MF_BYCOMMAND = &H0&
    MF_BYPOSITION = &H400&
    
    MF_SEPARATOR = &H800&
    
    MF_ENABLED = &H0&
    MF_GRAYED = &H1&
    MF_DISABLED = &H2&
    
    MF_UNCHECKED = &H0&
    MF_CHECKED = &H8&
    MF_USECHECKBITMAPS = &H200&
    
    MF_STRING = &H0&
    MF_BITMAP = &H4&
    MF_OWNERDRAW = &H100&

    MF_POPUP = &H10&
    MF_MENUBARBREAK = &H20&
    MF_MENUBREAK = &H40&
    
    MF_UNHILITE = &H0&
    MF_HILITE = &H80&

    MF_DEFAULT = &H1000&
    MF_SYSMENU = &H2000&
    MF_HELP = &H4000&
    MF_RIGHTJUSTIFY = &H4000&
    MF_MOUSESELECT = &H8000&
    MF_END = &H80&
End Enum

Private Enum MENU_FLAG_TYPE
    MFT_STRING = MF_STRING
    MFT_BITMAP = MF_BITMAP
    MFT_MENUBARBREAK = MF_MENUBARBREAK
    MFT_MENUBREAK = MF_MENUBREAK
    MFT_OWNERDRAW = MF_OWNERDRAW
    MFT_RADIOCHECK = &H200&
    MFT_SEPARATOR = MF_SEPARATOR
    MFT_RIGHTORDER = &H2000&
    MFT_RIGHTJUSTIFY = MF_RIGHTJUSTIFY
End Enum

' Menu flags for Add/Check/EnableMenuItem()
Private Enum MENU_FLAG_STATE
    MFS_GRAYED = &H3&
    MFS_DISABLED = MFS_GRAYED
    MFS_CHECKED = MF_CHECKED
    MFS_HILITE = MF_HILITE
    MFS_ENABLED = MF_ENABLED
    MFS_UNCHECKED = MF_UNCHECKED
    MFS_UNHILITE = MF_UNHILITE
    MFS_DEFAULT = MF_DEFAULT
End Enum

Private Enum MENU_STYLE
    MNS_NOCHECK = &H80000000
    MNS_MODELESS = &H40000000
    MNS_DRAGDROP = &H20000000
    MNS_AUTODISMISS = &H10000000
    MNS_NOTIFYBYPOS = &H8000000
    MNS_CHECKORBMP = &H4000000
End Enum

Private Enum MENU_INFO_MASK
    MIM_MAXHEIGHT = &H1&
    MIM_BACKGROUND = &H2&
    MIM_HELPID = &H4&
    MIM_MENUDATA = &H8&
    MIM_STYLE = &H10&
    MIM_APPLYTOSUBMENUS = &H80000000
End Enum

' return codes for WM_MENUCHAR */
Private Enum MENU_CHAR
    MNC_IGNORE = 0
    MNC_CLOSE = 1
    MNC_EXECUTE = 2
    MNC_SELECT = 3
End Enum

' System Menu Command Values
Private Enum SYSMENU_CMD
    SC_SIZE = &HF000
    SC_MOVE = &HF010
    SC_MINIMIZE = &HF020
    SC_MAXIMIZE = &HF030
    SC_NEXTWINDOW = &HF040
    SC_PREVWINDOW = &HF050
    SC_CLOSE = &HF060
    SC_VSCROLL = &HF070
    SC_HSCROLL = &HF080
    SC_MOUSEMENU = &HF090
    SC_KEYMENU = &HF100
    SC_ARRANGE = &HF110
    SC_RESTORE = &HF120
    SC_TASKLIST = &HF130
    SC_SCREENSAVE = &HF140
    SC_HOTKEY = &HF150
    
    SC_DEFAULT = &HF160
    SC_MONITORPOWER = &HF170
    SC_CONTEXTHELP = &HF180
    SC_SEPARATOR = &HF00F
End Enum

' Flags for TrackPopupMenu
Private Enum TRACK_POPUP_MENU_FLAG
    TPM_LEFTBUTTON = &H0&
    TPM_RIGHTBUTTON = &H2&
    TPM_LEFTALIGN = &H0&
    TPM_CENTERALIGN = &H4&
    TPM_RIGHTALIGN = &H8&
    
    TPM_TOPALIGN = &H0&
    TPM_VCENTERALIGN = &H10&
    TPM_BOTTOMALIGN = &H20&

    TPM_HORIZONTAL = &H0&  ' Horz alignment matters more */
    TPM_VERTICAL = &H40&   ' Vert alignment matters more */
    TPM_NONOTIFY = &H80&   ' Don't send any notification msgs */
    TPM_RETURNCMD = &H100&
    
    TPM_RECURSE = &H1&
    TPM_HORPOSANIMATION = &H400&
    TPM_HORNEGANIMATION = &H800&
    TPM_VERPOSANIMATION = &H1000&
    TPM_VERNEGANIMATION = &H2000&
    
    TPM_NOANIMATION = &H4000&
    
    TPM_LAYOUTRTL = &H8000&
End Enum

Private Const GW_CHILD = 5
Private Const GW_HWNDNEXT = 2
Private Const GW_HWNDFIRST = 0

Private Type POINTAPI
    X As Long
    Y As Long
End Type

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Type TPMPARAMS
    cbSize As Long
    rcExclude As RECT
End Type

Private Type MENUINFO
    cbSize As Long
    fMask As MENU_INFO_MASK
    dwStyle As MENU_STYLE
    cyMax As Long
    hbrBack As Long
    dwContextHelpID As Long
    dwMenuData As Long
End Type

Private Type MENUITEMINFO
    cbSize As Long
    fMask As Long
    fType As Long
    fState As Long
    wID As Long
    hSubMenu As Long
    hbmpChecked As Long
    hbmpUnchecked As Long
    dwItemData As Long
    dwTypeData As String
    cch As Long
End Type

Private Type MENUITEMTEMPLATE
    mtOption As Integer
    mtID As Integer
    mtString As Byte
End Type

Private Type MENUITEMTEMPLATEHEADER
    versionNumber As Integer
    offset As Integer
End Type

Private Declare Function SetMenuInfo Lib "user32.dll" ( _
    ByVal hNenu As Long, _
    ByRef lpMenuInfo As MENUINFO) As Long

Private Declare Function GetMenuInfo Lib "user32.dll" ( _
    ByVal hNenu As Long, _
    ByRef lpMenuInfo As MENUINFO) As Long

Private Declare Function MenuItemFromPoint Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal hMenu As Long, _
    ByVal ptScreen As POINTAPI) As Long

Private Declare Function CreatePopupMenu Lib "user32" () As Long

Private Declare Function DestroyMenu Lib "user32" ( _
    ByVal hMenu As Long) As Long

Private Declare Function InsertMenuA Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal nPosition As Long, _
    ByVal wFlags As Long, _
    ByVal wIDNewItem As Long, _
    ByVal lpNewItem As Any) As Long

Private Declare Function InsertMenuItemA Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal un As Long, _
    ByVal bool As Boolean, _
    ByRef lpcMenuItemInfo As MENUITEMINFO) As Long

Private Declare Function AppendMenuA Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal wFlags As Long, _
    ByVal wIDNewItem As Long, _
    ByVal lpNewItem As String) As Long

Private Declare Function SetMenuDefaultItem Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal uItem As Long, _
    ByVal fByPos As Long) As Long

Private Declare Function EnableMenuItem Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal wIDEnableItem As Long, _
    ByVal wEnable As Long) As Long

Private Declare Function ModifyMenuA Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal nPosition As Long, _
    ByVal wFlags As Long, _
    ByVal wIDNewItem As Long, _
    ByVal lpString As Any) As Long

Private Declare Function GetCursorPos Lib "user32" ( _
    ByRef lpPoint As POINTAPI) As Long

Private Declare Function GetDesktopWindow Lib "user32" () As Long

Private Declare Function TrackPopupMenu Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal wFlags As TRACK_POPUP_MENU_FLAG, _
    ByVal X As Long, _
    ByVal Y As Long, _
    ByVal nReserved As Long, _
    ByVal hWnd As Long, _
    ByRef lprc As RECT) As Long

Private Declare Function TrackPopupMenuEx Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal fuFlags As TRACK_POPUP_MENU_FLAG, _
    ByVal X As Long, _
    ByVal Y As Long, _
    ByVal hWnd As Long, _
    ByRef lpTPMParams As TPMPARAMS) As Long

Private Declare Function GetFocus Lib "user32" () As Long

Private Declare Function GetForegroundWindow Lib "user32" () As Long

Private Declare Function WindowFromPoint Lib "user32" ( _
    ByVal xPoint As Long, _
    ByVal yPoint As Long) As Long

Private Declare Function GetWindow Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal wCmd As Long) As Long

Private Declare Function GetWindowThreadProcessId Lib "user32" ( _
    ByVal hWnd As Long, _
    ByRef lpdwProcessId As Long) As Long

Private Declare Function GetCurrentProcessId Lib "kernel32" () As Long

Private Declare Function GetWindowRect Lib "user32" ( _
    ByVal hWnd As Long, _
    ByRef lpRect As RECT) As Long

Private mvarMenuHnd As Long
Private mvarCaption As String
Private mvarAutoDismiss As Boolean
Private mvarTag As String
Private mvarItemData As Long
Private mvarItems() As PTMenuItem
Private mvarItemCnt As Long
Private mvarSubMenus() As PTMenu
Private mvarSubMenuCnt As Long
Private Sub Class_Initialize()
    Dim mi As MENUINFO

    mvarMenuHnd = CreatePopupMenu()

    mi.cbSize = Len(mi)
    mi.fMask = MIM_STYLE
    GetMenuInfo mvarMenuHnd, mi

    mi.fMask = MIM_STYLE Or MIM_APPLYTOSUBMENUS
    mi.dwStyle = mi.dwStyle Or MNS_AUTODISMISS
    SetMenuInfo mvarMenuHnd, mi
    mvarAutoDismiss = True

    mvarCaption = "New Menu"
    Erase mvarItems
    mvarItemCnt = 0
End Sub
Private Sub Class_Terminate()
    Dim i As Long

    If mvarSubMenuCnt > 0 Then
        For i = 0 To UBound(mvarSubMenus)
            Set mvarSubMenus(i) = Nothing
        Next i
    End If
    Erase mvarSubMenus

    If mvarItemCnt > 0 Then
        For i = 0 To UBound(mvarItems)
            Set mvarItems(i) = Nothing
        Next i
    End If
    Erase mvarItems

    DestroyMenu mvarMenuHnd
End Sub
Friend Property Get hMenu() As Long
    hMenu = mvarMenuHnd
End Property
Public Property Let Caption(ByVal NewValue As String)
    mvarCaption = NewValue
End Property
Public Property Get Caption() As String
    Caption = mvarCaption
End Property
Public Property Let AutoDismiss(ByVal NewValue As Boolean)
    Dim mi As MENUINFO

    mvarAutoDismiss = NewValue

    mi.cbSize = Len(mi)
    mi.fMask = MIM_STYLE
    GetMenuInfo mvarMenuHnd, mi

    mi.fMask = MIM_STYLE Or MIM_APPLYTOSUBMENUS
    If mvarAutoDismiss Then
        mi.dwStyle = mi.dwStyle Or MNS_AUTODISMISS
    Else
        mi.dwStyle = mi.dwStyle And (Not MNS_AUTODISMISS)
    End If

    SetMenuInfo mvarMenuHnd, mi
End Property
Public Property Get AutoDismiss() As Boolean
    AutoDismiss = mvarAutoDismiss
End Property
Public Property Let Tag(ByVal NewValue As String)
    mvarTag = NewValue
End Property
Public Property Get Tag() As String
    Tag = mvarTag
End Property
Public Property Let ItemData(ByVal NewValue As Long)
    mvarItemData = NewValue
End Property
Public Property Get ItemData() As Long
    ItemData = mvarItemData
End Property
Public Property Get ItemCount() As Long
    ItemCount = mvarItemCnt
End Property
Public Function MenuItem(Index As Long) As PTMenuItem
    If mvarItemCnt > 0 Then
        If (Index < LBound(mvarItems)) Or (Index > UBound(mvarItems)) Then
            Set MenuItem = Nothing
        Else
            Set MenuItem = mvarItems(Index)
        End If
    Else
        Set MenuItem = Nothing
    End If
End Function
Public Property Get SubMenuCount() As Long
    SubMenuCount = mvarSubMenuCnt
End Property
Public Function SubMenu(Index As Long) As PTMenu
    If mvarSubMenuCnt > 0 Then
        If (Index < LBound(mvarSubMenus)) Or (Index > UBound(mvarSubMenus)) Then
            Set SubMenu = Nothing
        Else
            Set SubMenu = mvarSubMenus(Index)
        End If
    Else
        Set SubMenu = Nothing
    End If
End Function
Public Function AddChoice(Caption As String, Optional ByVal ItemID As Long = 0, Optional ByVal BreakBefore As Boolean = False) As PTMenuItem
    Dim NewItem As PTMenuItem
    Dim ID As Long

    If ItemID <= 0 Then
        MenuNextID = MenuNextID + 1
        ID = MenuNextID
    Else
        ID = ItemID
    End If

    Set NewItem = New PTMenuItem
    NewItem.ConstructChoice mvarMenuHnd, ID, Caption, BreakBefore
    ReDim Preserve mvarItems(mvarItemCnt)
    Set mvarItems(mvarItemCnt) = NewItem
    mvarItemCnt = mvarItemCnt + 1
    Set AddChoice = NewItem
End Function
Public Sub AddSeparator()
    Dim NewItem As PTMenuItem

    MenuNextID = MenuNextID + 1
    Set NewItem = New PTMenuItem
    NewItem.ConstructSeparator mvarMenuHnd, MenuNextID
End Sub
Public Sub AddSubMenu(SubMenu As PTMenu, Optional ByVal BreakBefore As Boolean = False)
    Dim NewItem As PTMenuItem
    Dim MenuItem As PTMenuItem

    ' add it to this menu
    MenuNextID = MenuNextID + 1
    Set NewItem = New PTMenuItem
    NewItem.ConstructSubMenu mvarMenuHnd, MenuNextID, SubMenu, BreakBefore

    ' save a copy
    ReDim Preserve mvarSubMenus(mvarSubMenuCnt)
    Set mvarSubMenus(mvarSubMenuCnt) = SubMenu
    mvarSubMenuCnt = mvarSubMenuCnt + 1
End Sub
Public Function DispPopup(Optional ByVal hCtrl As Long = 0, Optional ByVal X As Long = -1, Optional ByVal Y As Long = -1) As Long
    Dim ItemID As Long
    Dim i As Long
    Dim lim As Long
    Dim hDesktop As Long
    Dim CurrentPid As Long
    Dim hChild As Long
    Dim ChildPid As Long
    Dim pt As POINTAPI
    Dim hWnd As Long
    Dim tpm As TPMPARAMS
    Dim rcExclude As RECT

    If mvarItemCnt <= 0 Then
        DispPopup = -1
        Exit Function
    End If

    CurrentPid = GetCurrentProcessId()
    hDesktop = GetDesktopWindow()
    hChild = GetWindow(hDesktop, GW_CHILD)
    Do While hChild
        GetWindowThreadProcessId hChild, ChildPid
        If ChildPid = CurrentPid Then Exit Do
        hChild = GetWindow(hChild, GW_HWNDNEXT)
    Loop

'    If hChild = 0 Then MenuError PTMENU_NO_WINDOW
    hWnd = hChild

    GetCursorPos pt
    If X = -1 Then
        X = pt.X
    End If
    If Y = -1 Then
        Y = pt.Y
    End If

    If hCtrl = 0 Then
        GetWindowRect hWnd, rcExclude
        ItemID = TrackPopupMenu(mvarMenuHnd, TPM_NONOTIFY Or TPM_RETURNCMD Or TPM_LEFTALIGN Or TPM_LEFTBUTTON, X, Y, 0, hWnd, rcExclude)
    Else
        GetWindowRect hCtrl, rcExclude
        rcExclude.Left = rcExclude.Left - 4
        If rcExclude.Left < 0 Then rcExclude.Left = 0
        rcExclude.Right = rcExclude.Right + 4
        rcExclude.Top = rcExclude.Top - 4
        If rcExclude.Top < 0 Then rcExclude.Top = 0
        rcExclude.Bottom = rcExclude.Bottom + 4
        tpm.cbSize = Len(tpm)
        tpm.rcExclude = rcExclude
 
        ItemID = TrackPopupMenuEx(mvarMenuHnd, TPM_NONOTIFY Or TPM_RETURNCMD Or TPM_LEFTALIGN Or TPM_LEFTBUTTON, X, Y, hWnd, tpm)
    End If

    If Not Me.Click(ItemID) Then ' must be in sub menu
        If mvarSubMenuCnt > 0 Then
            lim = UBound(mvarSubMenus)
            For i = 0 To lim
                If mvarSubMenus(i).Click(ItemID) Then Exit For
            Next i
        End If
    End If

    DispPopup = ItemID
End Function
Friend Function Click(ItemID As Long) As Boolean
    Dim MenuItem As PTMenuItem
    Dim i As Long
    Dim lim As Long

    Click = False
    If mvarItemCnt <= 0 Then Exit Function

    lim = UBound(mvarItems)
    For i = 0 To lim
        Set MenuItem = mvarItems(i)
        If Not (MenuItem Is Nothing) Then
            If MenuItem.ItemType = MENU_ITEM_NORMAL Then
                If MenuItem.ID = ItemID Then
                    MenuItem.Click
                    Click = True
                    Exit Function
                End If
            End If
        End If
    Next i
End Function
