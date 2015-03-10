VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PTStretch"
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

Private Const STRETCH_MIN_SIZE = 300
Private Const REG_STRETCHERS = "\Stretchers\"
Private Const REG_DIRECTION = "Direction"
Private Const REG_COORD = "Coord"
Private Const REG_TARGET_SIZE = "TargetSize"

Private Enum PT_STRETCH_DIR
    STRETCH_DIR_NS
    STRETCH_DIR_EW
End Enum

Private mvarID As Long
Private mvarIndex As Long
Private mvarName As String

Private mvarCtrlA As PTCtrl
Private mvarStretchCtrl As PTCtrl
Private mvarCtrlB As PTCtrl
Private WithEvents Stretcher As PictureBox
Attribute Stretcher.VB_VarHelpID = -1

Private mvarDirection As PT_STRETCH_DIR
Private mvarStretching As Boolean
Private mvarStretcherWidth As Single
Private mvarDownCoord As Single
Private mvarMoveCoord As Single
Private mvarMinSizeA As Single
Private mvarMaxSizeA As Single
Private mvarDownSizeA As Single

Private mvarAccumDelta As Single

Public Event Begun()
Public Event Changed(NewDelta As Single, TotalDelta As Single)
Public Event Finished(TotalDelta As Single)
Public Property Get AccumDelta() As Single
    AccumDelta = mvarAccumDelta
    mvarAccumDelta = 0
End Property
Public Sub Save(SubKey As String)
    Dim rkey As String
    Dim i As Long
    Dim Reg As Registry
    Dim Coord As Single
    Dim TargetSize As Single

    rkey = SubKey & REG_STRETCHERS & Me.Name

    Set Reg = New Registry
    Reg.WriteValue REG_DIRECTION, mvarDirection, rkey

    If mvarDirection = STRETCH_DIR_NS Then
        Coord = mvarStretchCtrl.Top
        TargetSize = mvarCtrlA.Height
    Else
        Coord = mvarStretchCtrl.Left
        TargetSize = mvarCtrlA.Width
    End If

    i = CLng(Round(Coord, 3) * 1000)
    Reg.WriteValue REG_COORD, i, rkey

    i = CLng(Round(TargetSize, 3) * 1000)
    Reg.WriteValue REG_TARGET_SIZE, i, rkey
End Sub
Public Sub Restore(SubKey As String)
    Dim rkey As String
    Dim i As Long
    Dim Reg As Registry
    Dim Direction As PT_STRETCH_DIR
    Dim Coord As Single
    Dim TargetSize As Single
    Dim NewCoord As Single
    Dim NewTargetSize As Single
    Dim Delta As Single

    rkey = SubKey & REG_STRETCHERS & Me.Name

    Set Reg = New Registry
    Direction = Reg.ReadValue(REG_DIRECTION, mvarDirection, rkey)
    If Direction <> mvarDirection Then Exit Sub

    If mvarDirection = STRETCH_DIR_NS Then
        Coord = mvarStretchCtrl.Top
        TargetSize = mvarCtrlA.Height
    ElseIf mvarDirection = STRETCH_DIR_EW Then
        Coord = mvarStretchCtrl.Left
        TargetSize = mvarCtrlA.Width
    Else
        Exit Sub ' bad data
    End If

    i = CLng(Round(Coord, 3) * 1000)
    i = Reg.ReadValue(REG_COORD, i, rkey)
    NewCoord = CSng(i) / 1000

    i = CLng(Round(TargetSize, 3) * 1000)
    i = Reg.ReadValue(REG_TARGET_SIZE, i, rkey)
    NewTargetSize = CSng(i) / 1000

    Delta = NewCoord - Coord
    If Delta <> 0 Then Me.ManualMove Delta
End Sub
Public Property Get ID() As Long
    ID = mvarID
End Property
Public Property Let ID(ByVal NewValue As Long)
    mvarID = NewValue
End Property
Public Property Get Index() As Long
    Index = mvarIndex
End Property
Public Property Let Index(ByVal NewValue As Long)
    mvarIndex = NewValue
End Property
Public Property Get Name() As String
    Dim s As String

    s = mvarName
    If mvarIndex >= 0 Then
        s = s & "(" & CStr(mvarIndex) & ")"
    End If
    Name = s
End Property
Public Property Let Name(ByVal NewValue As String)
    mvarName = NewValue
End Property
Public Property Get Coord() As Single
    If mvarDirection = STRETCH_DIR_EW Then
        Coord = Stretcher.Left
    Else
        Coord = Stretcher.Top
    End If
End Property
Public Property Get MinSizeA() As Single
    MinSizeA = mvarMinSizeA
End Property
Public Property Let MinSizeA(ByVal NewValue As Single)
    mvarMinSizeA = NewValue
End Property
Public Property Get MaxSizeA() As Single
    MaxSizeA = mvarMaxSizeA
End Property
Public Property Let MaxSizeA(ByVal NewValue As Single)
    mvarMaxSizeA = NewValue
End Property
Public Sub MouseDown(X As Single, Y As Single)
    mvarStretchCtrl.Ctrl.ZOrder 1
    
    If mvarDirection = STRETCH_DIR_NS Then
        mvarDownCoord = Y ' Y + mvarStretchCtrl.Top
        mvarDownSizeA = mvarCtrlA.Height
    Else
        mvarDownCoord = X  ' X + mvarStretchCtrl.Left
        mvarDownSizeA = mvarCtrlA.Width
    End If

    mvarMoveCoord = mvarDownCoord
    mvarStretching = True
    RaiseEvent Begun
End Sub
Public Function MouseMove(X As Single, Y As Single) As Single
    Dim NewCoord As Single
    Dim NewDelta As Single
    Dim TotalDelta As Single
    Dim NewSize As Single

    If Not mvarStretching Then Exit Function

    If mvarDirection = STRETCH_DIR_NS Then
        NewCoord = Y
    Else
        NewCoord = X
    End If

    NewDelta = NewCoord - mvarMoveCoord
    If NewDelta = 0 Then Exit Function
    TotalDelta = NewCoord - mvarDownCoord

    NewSize = mvarDownSizeA + TotalDelta
    If NewSize < mvarMinSizeA Then
        NewSize = mvarMinSizeA
        TotalDelta = NewSize - mvarDownSizeA
        NewCoord = mvarDownCoord + TotalDelta
        NewDelta = TotalDelta - mvarMoveCoord
    End If
    mvarMoveCoord = NewCoord

    mvarAccumDelta = mvarAccumDelta + NewDelta
    RaiseEvent Changed(NewDelta, TotalDelta)
    MouseMove = NewDelta
End Function
Public Function MouseUp(X As Single, Y As Single) As Single
    Dim TotalDelta As Single
    Dim NewDelta As Single
    Dim NewCoord As Single

    MouseUp = 0
    If Not mvarStretching Then Exit Function

    mvarStretching = False
    If mvarDirection = STRETCH_DIR_NS Then
        If (X < mvarStretchCtrl.Left) Or (X > mvarStretchCtrl.Width) Then
            NewCoord = mvarDownCoord
            NewDelta = NewCoord - mvarMoveCoord
        Else
            NewCoord = mvarMoveCoord
            NewDelta = 0
        End If
        TotalDelta = NewCoord - mvarDownCoord
    Else
        If (Y < mvarStretchCtrl.Top) Or (Y > mvarStretchCtrl.Height) Then
            TotalDelta = mvarDownSizeA - mvarCtrlA.Width
            mvarCtrlA.Width = mvarDownSizeA
            mvarStretchCtrl.Left = mvarCtrlA.Left + mvarCtrlA.Height + 1
        Else

        End If
    End If

'    mvarCtrlA.ReInit
'    mvarStretchCtrl.ReInit

    mvarAccumDelta = mvarAccumDelta + NewDelta
    RaiseEvent Changed(NewDelta, TotalDelta)
    RaiseEvent Finished(TotalDelta)
    mvarStretchCtrl.Ctrl.ZOrder
    MouseUp = TotalDelta
End Function
Public Sub Constructor(StretchCtrl As PTCtrl, TargetCtrl As PTCtrl, Optional ID As Long = -1)
    Dim TopDelta As Single
    Dim LeftDelta As Single

    Set mvarStretchCtrl = StretchCtrl
    Set mvarCtrlA = TargetCtrl
    Set Stretcher = mvarStretchCtrl.Ctrl

    mvarID = ID
    mvarName = Mid(Stretcher.Name, 4)
    mvarIndex = -1

    mvarMinSizeA = STRETCH_MIN_SIZE

    TopDelta = Abs(mvarCtrlA.Top - mvarStretchCtrl.Top)
    LeftDelta = Abs(mvarCtrlA.Left - mvarStretchCtrl.Left)
    If TopDelta < LeftDelta Then  ' controls are side-by-side
        mvarDirection = STRETCH_DIR_EW
    Else                        ' controls are one above the other
        mvarDirection = STRETCH_DIR_NS
        Stretcher.MousePointer = vbSizeNS
    End If

    If mvarDirection = STRETCH_DIR_NS Then
        mvarStretchCtrl.Top = mvarCtrlA.Top + mvarCtrlA.Height + 1
        mvarMinSizeA = mvarCtrlA.Height * 0.8
    Else
        mvarStretchCtrl.Left = mvarCtrlA.Left + mvarCtrlA.Width + 1
        mvarMinSizeA = mvarCtrlA.Width * 0.8
    End If

    mvarCtrlA.ReInit
    mvarStretchCtrl.ReInit
    mvarStretchCtrl.Ctrl.ZOrder
End Sub
Public Function ManualMove(Delta As Single) As Single
    Dim Dummy As Single

    ManualMove = 0
    If Delta = 0 Then Exit Function
    If mvarDirection = STRETCH_DIR_NS Then
        Dummy = mvarStretchCtrl.Left
        MouseDown Dummy, 0
        MouseMove Dummy, Delta
        Delta = MouseUp(Dummy, 0)
    Else
        Dummy = mvarStretchCtrl.Top
        MouseDown 0, Dummy
        MouseMove Delta, Dummy
        Delta = MouseUp(0, Dummy)
    End If

    ManualMove = Delta
End Function
Public Sub Resize()
    If mvarDirection = STRETCH_DIR_NS Then
        mvarStretchCtrl.Top = mvarCtrlA.Top + mvarCtrlA.Height + 1
    Else
        mvarStretchCtrl.Left = mvarCtrlA.Left + mvarCtrlA.Width + 1
    End If
End Sub
Private Sub Class_Terminate()
    Set Stretcher = Nothing
    Set mvarStretchCtrl = Nothing
    Set mvarCtrlA = Nothing
End Sub
Private Sub Stretcher_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button And vbLeftButton <> 0 Then
        MouseDown X, Y
    End If
End Sub
Private Sub Stretcher_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button And vbLeftButton <> 0 Then
        MouseMove X, Y
    End If
End Sub
Private Sub Stretcher_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button And vbLeftButton <> 0 Then MouseUp X, Y
End Sub
