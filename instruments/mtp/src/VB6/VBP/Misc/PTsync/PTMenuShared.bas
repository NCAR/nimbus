Attribute VB_Name = "MenuShared"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 1.0 5/26/2004

Private Const ERROR_SOURCE_MENU = "PTMenu"
Private Const ERROR_SOURCE_ITEM = "PTMenuItem"

Public Enum PTMENU_ERROR
    PTMENU_ERROR_BASE = vbObjectError + 100

    PTMENU_MENU_LO
    PTMENU_MENU_UNCONSTRUCTED = PTMENU_MENU_LO
    PTMENU_MENU_ALREADY_CONSTRUCTED
    PTMENU_NO_WINDOW
    PTMENU_MENU_HI = PTMENU_MENU_ALREADY_CONSTRUCTED

    PTMENU_ITEM_LO
    PTMENU_ITEM_UNCONSTRUCTED = PTMENU_ITEM_LO
    PTMENU_ITEM_ALREADY_CONSTRUCTED
    PTMENU_ITEM_HI = PTMENU_ITEM_ALREADY_CONSTRUCTED
End Enum

Public MenuNextID As Long
Public Sub MenuError(ErrorNum As PTMENU_ERROR)
    Dim Desc As String
    Dim Src As String

    Select Case ErrorNum
        Case PTMENU_MENU_UNCONSTRUCTED: Desc = "Menu has not yet been constructed"
        Case PTMENU_MENU_ALREADY_CONSTRUCTED: Desc = "Menu has already been constructed"
        Case PTMENU_NO_WINDOW:  Desc = "Can't Find Window Handle"
        Case PTMENU_ITEM_UNCONSTRUCTED: Desc = "Menu Item has not yet been constructed"
        Case PTMENU_ITEM_ALREADY_CONSTRUCTED: Desc = "Menu Item has already been constructed"
        Case Else:  Desc = "Unknown Error (" & CStr(ErrorNum) & ")"
    End Select

    Select Case ErrorNum
        Case PTMENU_MENU_LO To PTMENU_MENU_HI: Src = ERROR_SOURCE_MENU
        Case PTMENU_ITEM_LO To PTMENU_ITEM_HI: Src = ERROR_SOURCE_ITEM
        Case Else: Src = ERROR_SOURCE_MENU
    End Select

    Err.Raise ErrorNum, Src, Desc
End Sub
