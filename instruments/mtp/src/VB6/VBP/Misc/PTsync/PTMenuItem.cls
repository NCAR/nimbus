VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PTMenuItem"
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
' Version 1.0 6/1/2004

' ;win40  -- A lot of MF_* flags have been renamed as MFT_* and MFS_* flags */
' Menu flags for Add/Check/EnableMenuItem()
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

Private Enum MENU_ITEM_INFO_MASK
    MIIM_STATE = &H1&
    MIIM_ID = &H2&
    MIIM_SUBMENU = &H4&
    MIIM_CHECKMARKS = &H8&
    MIIM_TYPE = &H10&
    MIIM_DATA = &H20&

    MIIM_STRING = &H40&
    MIIM_BITMAP = &H80&
    MIIM_FTYPE = &H100&
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
Private Enum MENU_FLAG_STYLE
    MFS_GRAYED = &H3&
    MFS_DISABLED = MFS_GRAYED
    MFS_CHECKED = MF_CHECKED
    MFS_HILITE = MF_HILITE
    MFS_ENABLED = MF_ENABLED
    MFS_UNCHECKED = MF_UNCHECKED
    MFS_UNHILITE = MF_UNHILITE
    MFS_DEFAULT = MF_DEFAULT
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

Private Enum GMDI_FLAG
    GMDI_USEDISABLED = &H1&
    GMDI_GOINTOPOPUPS = &H2&
End Enum

Public Enum PTMENU_ITEM_TYPE
    MENU_ITEM_NORMAL
    MENU_ITEM_SEPARATOR
    MENU_ITEM_SUBMENU
End Enum

Private Type MENUITEMINFO
    cbSize As Long
    fMask As MENU_ITEM_INFO_MASK
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

Private Declare Function InsertMenuA Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal nPosition As Long, _
    ByVal wFlags As Long, _
    ByVal wIDNewItem As Long, _
    ByVal lpNewItem As Any) As Long

Private Declare Function InsertMenuItemA Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal uItem As Long, _
    ByVal fByPosition As Boolean, _
    ByRef lpMenuItemInfo As MENUITEMINFO) As Long

Private Declare Function AppendMenuA Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal wFlags As MENU_FLAG, _
    ByVal wIDNewItem As Long, _
    ByVal lpNewItem As String) As Long

Private Declare Function ModifyMenuA Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal nPosition As Long, _
    ByVal wFlags As MENU_FLAG, _
    ByVal wIDNewItem As Long, _
    ByVal lpNewItem As String) As Long

Private Declare Function EnableMenuItem Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal wIDEnableItem As Long, _
    ByVal wEnable As Long) As Long

Private Declare Function CheckMenuItem Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal wIDCheckItem As Long, _
    ByVal wCheck As Long) As Long

Private Declare Function CheckMenuRadioItem Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal idFirst As Long, _
    ByVal idLast As Long, _
    ByVal idCheck As Long, _
    ByVal uFlags As Long) As Long

Private Declare Function SetMenuDefaultItem Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal uItem As Long, _
    ByVal fByPos As Long) As Long

Private Declare Function GetMenuDefaultItem Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal fByPos As Long, _
    ByVal GMDI_FLAG As Long) As Long

Private Declare Function CreateAcceleratorTableA Lib "user32" ( _
    lpaccl As ACCEL, _
    ByVal cEntries As Long) As Long

Private Declare Function DestroyAcceleratorTable Lib "user32" ( _
    ByVal haccel As Long) As Long

Private Type ACCEL
    fVirt As Byte
    Key As Integer
    cmd As Integer
End Type

Private mvarMenuHnd As Long
Private mvarItemType As PTMENU_ITEM_TYPE
Private mvarID As Long
Private mvarCaption As String
Private mvarTag As String
Private mvarItemData As Long
Private mvarShortcut As String
Private mvarVisible As Boolean
Private mvarChecked As Boolean
Private mvarCheckedRadio As Boolean
Private mvarEnabled As Boolean
Private mvarConstructed As Boolean

Public Event Clicked(ItemID As Long, Caption As String, Checked As Boolean)
Public Sub ConstructChoice(MenuHnd As Long, ItemID As Long, Caption As String, Optional BreakBefore As Boolean = False)
    Dim Flags As MENU_FLAG

    If mvarConstructed Then
        MenuError PTMENU_ITEM_ALREADY_CONSTRUCTED
        Exit Sub
    End If

    mvarItemType = MENU_ITEM_NORMAL
    mvarMenuHnd = MenuHnd
    mvarID = ItemID
    mvarCaption = Caption

    Flags = MF_STRING
    If BreakBefore Then Flags = Flags Or MF_MENUBARBREAK
    AppendMenuA mvarMenuHnd, Flags, mvarID, mvarCaption
    mvarConstructed = True
End Sub
Public Sub ConstructSeparator(MenuHnd As Long, ItemID As Long)
    If mvarConstructed Then
        MenuError PTMENU_ITEM_ALREADY_CONSTRUCTED
        Exit Sub
    End If

    mvarItemType = MENU_ITEM_SEPARATOR
    mvarMenuHnd = MenuHnd
    mvarID = ItemID
    mvarCaption = "-"

    AppendMenuA mvarMenuHnd, MF_STRING Or MF_SEPARATOR, mvarID, vbNullString
    mvarConstructed = True
End Sub
Public Sub ConstructSubMenu(MenuHnd As Long, ItemID As Long, SubMenu As PTMenu, Optional BreakBefore As Boolean = False)
    Dim Flags As MENU_FLAG

    If mvarConstructed Then
        MenuError PTMENU_ITEM_ALREADY_CONSTRUCTED
        Exit Sub
    End If

    mvarItemType = MENU_ITEM_SUBMENU
    mvarMenuHnd = MenuHnd
    mvarID = ItemID

    Flags = MF_STRING Or MF_POPUP
    If BreakBefore Then Flags = Flags Or MF_MENUBARBREAK
    AppendMenuA mvarMenuHnd, Flags, SubMenu.hMenu, SubMenu.Caption
    mvarConstructed = True
End Sub
Public Sub Click()
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Sub
    End If

    RaiseEvent Clicked(mvarID, mvarCaption, mvarChecked Or mvarCheckedRadio)
End Sub
Public Property Get MenuHnd() As Long
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    MenuHnd = mvarMenuHnd
End Property
Public Property Get ItemType() As PTMENU_ITEM_TYPE
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    ItemType = mvarItemType
End Property
Public Property Let Caption(ByVal NewValue As String)
    Dim Flags As MENU_FLAG
    Dim s As String

    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    mvarCaption = NewValue

    If Len(mvarShortcut) > 0 Then
        s = mvarCaption & vbTab & mvarShortcut
    Else
        s = mvarCaption
    End If

    Flags = MF_BYCOMMAND Or MF_STRING
    ModifyMenuA mvarMenuHnd, mvarID, Flags, mvarID, s
End Property
Public Property Get Caption() As String
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    Caption = mvarCaption
End Property
Public Property Let Shortcut(ByVal NewValue As String)
    Dim Flags As MENU_FLAG
    Dim s As String

    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    mvarShortcut = NewValue

    If Len(mvarShortcut) > 0 Then
        s = mvarCaption & vbTab & mvarShortcut
    Else
        s = mvarCaption
    End If

    Flags = MF_BYCOMMAND Or MF_STRING
    ModifyMenuA mvarMenuHnd, mvarID, Flags, mvarID, s
End Property
Public Property Get Shortcut() As String
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    Shortcut = mvarShortcut
End Property
Public Property Let Tag(ByVal NewValue As String)
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    mvarTag = NewValue
End Property
Public Property Get Tag() As String
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    Tag = mvarTag
End Property
Public Property Let ItemData(ByVal NewValue As Long)
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    mvarItemData = NewValue
End Property
Public Property Get ItemData() As Long
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    ItemData = mvarItemData
End Property
Public Property Let Enabled(ByVal NewValue As Boolean)
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    mvarEnabled = NewValue
    If mvarEnabled Then
        EnableMenuItem mvarMenuHnd, mvarID, MF_BYCOMMAND Or MF_ENABLED
    Else
        EnableMenuItem mvarMenuHnd, mvarID, MF_BYCOMMAND Or MF_GRAYED
    End If
End Property
Public Property Get Enabled() As Boolean
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    Enabled = mvarEnabled
End Property
Public Property Let Default(ByVal NewValue As Boolean)
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    If NewValue Then
        SetMenuDefaultItem mvarMenuHnd, mvarID, 0
    Else
        SetMenuDefaultItem mvarMenuHnd, -1, 0
    End If
End Property
Public Property Get Default() As Boolean
    Dim DefaultItem As Long

    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    DefaultItem = GetMenuDefaultItem(mvarMenuHnd, MF_BYCOMMAND, GMDI_GOINTOPOPUPS Or GMDI_USEDISABLED)
    If DefaultItem = mvarID Then
        Default = True
    Else
        Default = False
    End If
End Property
Public Property Let Checked(ByVal NewValue As Boolean)
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    mvarChecked = NewValue
    If mvarChecked Then
        CheckMenuItem mvarMenuHnd, mvarID, MF_CHECKED
    Else
        CheckMenuItem mvarMenuHnd, mvarID, MF_UNCHECKED
    End If
End Property
Public Property Get Checked() As Boolean
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    Checked = mvarChecked
End Property
Public Property Let CheckedRadio(ByVal NewValue As Boolean)
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    mvarCheckedRadio = NewValue
    If mvarCheckedRadio Then
        CheckMenuRadioItem mvarMenuHnd, mvarID, mvarID, mvarID, MF_BYCOMMAND
    Else
        CheckMenuRadioItem mvarMenuHnd, mvarID, mvarID, -1, MF_BYCOMMAND
    End If
End Property
Public Property Get CheckedRadio() As Boolean
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    CheckedRadio = mvarCheckedRadio
End Property
Public Property Let Visible(ByVal NewValue As Boolean)
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    mvarVisible = NewValue
End Property
Public Property Get Visible() As Boolean
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    Visible = mvarVisible
End Property
Public Property Let ID(ByVal NewValue As Long)
    Dim Flags As MENU_FLAG
    Dim s As String

    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    If Len(mvarShortcut) > 0 Then
        s = mvarCaption & vbTab & mvarShortcut
    Else
        s = mvarCaption
    End If

    Flags = MF_BYCOMMAND Or MF_STRING
    ModifyMenuA mvarMenuHnd, mvarID, Flags, NewValue, s

    mvarID = NewValue
End Property
Public Property Get ID() As Long
    If Not mvarConstructed Then
        MenuError PTMENU_ITEM_UNCONSTRUCTED
        Exit Property
    End If

    ID = mvarID
End Property
Private Sub Class_Initialize()
    mvarCaption = "New Menu Item"
    mvarConstructed = False
End Sub
