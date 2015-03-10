Attribute VB_Name = "ListVue"
Option Explicit
' Copyright © 2003-2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 2.1 4/30/2004

Public Const LV_SORT_TAG = "SORT"
Public Const LV_HIDE_TAG = "HIDE"

Private Const LVM_FIRST As Long = &H1000
Private Const LVM_SETCOLUMNWIDTH As Long = (LVM_FIRST + 30)
Private Const LVM_GETTOOLTIPS = (LVM_FIRST + 78)
Private Const LVM_SETTOOLTIPS = (LVM_FIRST + 74)
Private Const LVM_SETINFOTIP = (LVM_FIRST + 173)
Private Const LVM_UPDATE = (LVM_FIRST + 42)

Private Const LVS_ALIGNLEFT = &H800
Private Const LVS_ALIGNTOP = &H0
Private Const LVS_AUTOARRANGE = &H100

Private Const LVSCW_AUTOSIZE As Long = -1
Private Const LVSCW_AUTOSIZE_USEHEADER As Long = -2

Private Const EM_SETTABSTOPS = &HCB

Private Declare Function SendMessageA Lib "user32" ( _
    ByVal hwnd As Long, _
    ByVal wMsg As Long, _
    ByVal wParam As Long, _
    ByVal lParam As Any) As Long

Private InitDone As Boolean
Private RawData() As Variant  ' attribute, drive
Public lvSwapEm As Boolean
Public Sub lvEnsureVisible(lv As ListView, LstItm As ListItem)
    Dim inx As Long

    inx = LstItm.Index
    If inx < lv.ListItems.Count Then lv.ListItems(inx + 1).EnsureVisible
    If inx > 1 Then lv.ListItems(inx - 1).EnsureVisible
    LstItm.EnsureVisible
End Sub
Public Sub lvMoveUp(lv As ListView, btnUp As Control, btnDown As Control)
    Dim inx As Long

    lv.SetFocus
    If lv.SelectedItem Is Nothing Then
        lvCheckUpDown lv, btnUp, btnDown
        Exit Sub
    End If

    inx = lv.SelectedItem.Index
    If inx = 1 Then
        lvCheckUpDown lv, btnUp, btnDown
        Exit Sub
    End If

    lvSwap lv, inx, inx - 1
    Set lv.SelectedItem = lv.ListItems(inx - 1)
    lvCheckUpDown lv, btnUp, btnDown
End Sub
Public Sub lvMoveDown(lv As ListView, btnUp As Control, btnDown As Control)
    Dim inx As Long

    lv.SetFocus
    If lv.SelectedItem Is Nothing Then
        lvCheckUpDown lv, btnUp, btnDown
        Exit Sub
    End If

    inx = lv.SelectedItem.Index
    If inx = lv.ListItems.Count Then
        lvCheckUpDown lv, btnUp, btnDown
        Exit Sub
    End If

    lvSwap lv, inx, inx + 1
    Set lv.SelectedItem = lv.ListItems(inx + 1)
    lvCheckUpDown lv, btnUp, btnDown
End Sub
Public Sub lvSwap(lv As ListView, inx1 As Long, inx2 As Long)
    Dim SaveItem As ListItem
    Dim SaveKey1 As String
    Dim SaveKey2 As String

    Set SaveItem = lv.ListItems.Add(, "SWAP")

    lvCopy lv.ListItems(inx1), SaveItem, lv.ColumnHeaders.Count
    lvCopy lv.ListItems(inx2), lv.ListItems(inx1), lv.ColumnHeaders.Count
    lvCopy SaveItem, lv.ListItems(inx2), lv.ColumnHeaders.Count

    lv.ListItems.Remove SaveItem.Index

    ' swap keys
    SaveKey1 = lv.ListItems(inx1).Key
    SaveKey2 = lv.ListItems(inx2).Key
    lv.ListItems(inx1).Key = "TEMP1"
    lv.ListItems(inx2).Key = "TEMP2"
    lv.ListItems(inx1).Key = SaveKey2
    lv.ListItems(inx2).Key = SaveKey1
End Sub
Public Sub lvCopy(liSrc As ListItem, liDest As ListItem, SubItems As Long)
    Dim i As Long

    liDest.Bold = liSrc.Bold
    liDest.Checked = liSrc.Checked
    liDest.ForeColor = liSrc.ForeColor
    liDest.Ghosted = liSrc.Ghosted
    liDest.Icon = liSrc.Icon
    liDest.SmallIcon = liSrc.SmallIcon
    liDest.Tag = liSrc.Tag
    liDest.Text = liSrc.Text
    liDest.ToolTipText = liSrc.ToolTipText

    For i = 1 To SubItems - 1
        liDest.SubItems(i) = liSrc.SubItems(i)
    Next i
End Sub
Public Function lvCheckedCount(lv As ListView) As Long
    Dim i As Long
    Dim chkcnt As Long

    chkcnt = 0
    For i = 1 To lv.ListItems.Count
        If lv.ListItems(i).Checked Then chkcnt = chkcnt + 1
    Next i

    lvCheckedCount = chkcnt
End Function
Public Sub lvCheckUpDown(lv As ListView, btnUp As Control, btnDown As Control)
    
    If lv.SelectedItem Is Nothing Then
        btnUp.Enabled = False
        btnDown.Enabled = False
    ElseIf lv.SelectedItem.Index = 1 Then
        btnUp.Enabled = False
        btnDown.Enabled = True
    Else
        btnUp.Enabled = True
        If lv.SelectedItem.Index = lv.ListItems.Count Then
            btnDown.Enabled = False
        Else
            btnUp.Enabled = True
            btnDown.Enabled = True
        End If
    End If
End Sub
Public Sub LvAdjust(lv As ListView)
    Dim i As Long
    Dim adj As Boolean

    For i = 1 To lv.ColumnHeaders.Count
        adj = False
        If InStr(1, lv.ColumnHeaders(i).Tag, "HIDDEN", vbTextCompare) > 0 Then
            lv.ColumnHeaders(i).Width = 0
        End If
        If InStr(1, lv.ColumnHeaders(i).Tag, LV_HIDE_TAG, vbTextCompare) > 0 Then
            lv.ColumnHeaders(i).Width = 0
        End If
        If InStr(1, lv.ColumnHeaders(i).Tag, LV_SORT_TAG, vbTextCompare) > 0 Then
            lv.ColumnHeaders(i).Width = 0
        End If
        If lv.ColumnHeaders(i).Width > 0 Then
            adj = True
            If (i = 1) And lv.Checkboxes Then
                If lv.ListItems.Count > 0 Then
                    If Len(Trim(lv.ListItems(1).Text)) <= 0 Then
                        lv.ColumnHeaders(1).Width = 329
                        adj = False
                    End If
                End If
            End If
        End If

        If adj Then
            SendMessageA lv.hwnd, LVM_SETCOLUMNWIDTH, i - 1, _
                LVSCW_AUTOSIZE_USEHEADER
        End If
    Next
End Sub
Public Sub LvInit(lv As ListView)
    Dim ROWS As Long
    Dim Cols As Long

    If lvSwapEm Then
        Cols = lv.ColumnHeaders.Count
        ROWS = lv.ListItems.Count
    Else
        Cols = lv.ListItems.Count
        ROWS = lv.ColumnHeaders.Count
    End If

    If Cols > 0 Then Cols = Cols - 1
    If ROWS > 0 Then ROWS = ROWS - 1

    ReDim Preserve RawData(100, 100)
End Sub
Public Sub LvWriteCell(ByRef lv As ListView, ByVal Row As Long, ByVal Col As Long, Text As String, Optional Tag As Variant)
    ' Row and Col are zero-relative.
    ' Row 0 = column headers
    ' Col 0 = ListItem.Text
    Dim swap As Long
    Dim LstItm As ListItem
    Dim i As Long

    If Not InitDone Then LvInit lv

    If lvSwapEm Then
        swap = Row
        Row = Col
        Col = swap
    End If

    If Col > UBound(RawData, 2) Then
        ReDim Preserve RawData(UBound(RawData, 1), Col)
    End If
'    If Tag <> "" Then
'        RawData(Row, Col) = Tag
'    Else
'        RawData(Row, Col) = Text
'    End If

    While Row > lv.ListItems.Count
        Set LstItm = lv.ListItems.Add()
        LstItm.Text = " "
        For i = 1 To lv.ColumnHeaders.Count - 1
            LstItm.SubItems(i) = " "
        Next i
    Wend

    While Col >= lv.ColumnHeaders.Count
        lv.ColumnHeaders.Add
    Wend

    If Col = 0 Then ' Text property?
        If Row = 0 Then
            lv.ColumnHeaders(Col + 1).Text = Text
            If Not IsMissing(Tag) Then lv.ColumnHeaders(Col + 1).Tag = CStr(Tag)
        Else
            lv.ListItems(Row).Text = Text
            If Not IsMissing(Tag) Then lv.ListItems(Row).Tag = CStr(Tag)
        End If
    Else
        If Row = 0 Then
            lv.ColumnHeaders(Col + 1).Text = Text
            If Not IsMissing(Tag) Then lv.ColumnHeaders(Col + 1).Tag = CStr(Tag)
        Else
            lv.ListItems(Row).SubItems(Col) = Text
            If Not IsMissing(Tag) Then lv.ListItems(Row).Tag = CStr(Tag)
        End If
    End If
End Sub
Public Function LvReadCell(ByRef lv As ListView, ByVal Row As Long, ByVal Col As Long) As String
    ' Row and Col are zero-relative.
    ' Row 0 = column headers
    ' Col 0 = ListItem.Text

    If Not InitDone Then LvInit lv

    If Col = 0 Then ' Text property?
        If Row = 0 Then
            LvReadCell = lv.ColumnHeaders(Col + 1).Text
        Else
            LvReadCell = lv.ListItems(Row).Text
        End If
    Else
        If Row = 0 Then
            LvReadCell = lv.ColumnHeaders(Col + 1).Text
        Else
            LvReadCell = lv.ListItems(Row).SubItems(Col)
        End If
    End If
End Function

