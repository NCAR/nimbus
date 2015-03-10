VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PTSplit"
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
' Version 1.4 7/23/2004

Private Const SPLIT_MIN_SIZE = 1000
Private Const REG_SPLITTERS = "\Splitters\"
Private Const REG_DIRECTION = "Direction"
Private Const REG_COORD = "Coord"
Private Const REG_SIZE_A = "SizeA"
Private Const REG_SIZE_B = "SizeB"
Private Const REG_RATIO_A = "RatioA"
Private Const REG_RATIO_B = "RatioB"
Private Const REG_OVERALL_SIZE = "OverallSize"

Private Enum PT_SPLIT_DIR
    SPLIT_DIR_VERT
    SPLIT_DIR_HORZ
End Enum

Private mvarID As Long
Private mvarIndex As Long
Private mvarName As String

Private mvarCtrlA As PTCtrl
Private mvarSplitCtrl As PTCtrl
Private mvarCtrlB As PTCtrl
Private WithEvents Splitter As PictureBox
Attribute Splitter.VB_VarHelpID = -1

Private mvarDirection As PT_SPLIT_DIR
Private mvarSplitting As Boolean
Private mvarSplitterThickness As Single

Private mvarOrigCoord As Single

Private mvarMinSizeA As Single
Private mvarMaxSizeA As Single
Private mvarOrigSizeA As Single

Private mvarMinSizeB As Single
Private mvarMaxSizeB As Single
Private mvarOrigSizeB As Single
Private mvarOrigPosB As Single

Private mvarAccumDelta As Single

Public Event Begun()
Public Event Changed()
Public Event Finished()
Public Property Get AccumDelta() As Single
    AccumDelta = mvarAccumDelta
    mvarAccumDelta = 0
End Property
Public Sub Save(FormSubKey As String)
    Dim rkey As String
    Dim i As Long
    Dim Reg As Registry
    Dim Direction As PT_SPLIT_DIR
    Dim Coord As Single
    Dim CtrlSizeA As Single
    Dim CtrlSizeB As Single
    Dim OverallSize As Single
    Dim RatioA As Single
    Dim RatioB As Single

    rkey = FormSubKey & REG_SPLITTERS & Me.Name

    Set Reg = New Registry
    Reg.WriteValue REG_DIRECTION, mvarDirection, rkey

    If mvarDirection = SPLIT_DIR_VERT Then
        Coord = mvarSplitCtrl.Top
        CtrlSizeA = mvarCtrlA.Height
        CtrlSizeB = mvarCtrlB.Height
    Else
        Coord = mvarSplitCtrl.Left
        CtrlSizeA = mvarCtrlA.Width
        CtrlSizeB = mvarCtrlB.Width
    End If

    OverallSize = CtrlSizeA + CtrlSizeB
    RatioA = CtrlSizeA / OverallSize
    RatioB = 1 - RatioA

    i = CLng(Round(Coord, 3) * 10000)
    Reg.WriteValue REG_COORD, i, rkey

    i = CLng(Round(CtrlSizeA, 3) * 10000)
    Reg.WriteValue REG_SIZE_A, i, rkey

    i = CLng(Round(CtrlSizeB, 3) * 10000)
    Reg.WriteValue REG_SIZE_B, i, rkey

    i = CLng(Round(OverallSize, 3) * 10000)
    Reg.WriteValue REG_OVERALL_SIZE, i, rkey

    i = CLng(Round(RatioA, 3) * 100000)
    Reg.WriteValue REG_RATIO_A, i, rkey

    i = CLng(Round(RatioB, 3) * 100000)
    Reg.WriteValue REG_RATIO_B, i, rkey
End Sub
Public Sub Restore(SubKey As String)
    Dim rkey As String
    Dim i As Long
    Dim Reg As Registry
    Dim Direction As PT_SPLIT_DIR
    Dim CurCoord As Single
    Dim CurSizeA As Single
    Dim CurSizeB As Single
    Dim CurOverallSize As Single
    Dim CurRatioA As Single
    Dim CurRatioB As Single
    Dim NewCoord As Single
    Dim NewSizeA As Single
    Dim NewSizeB As Single
    Dim NewOverallSize As Single
    Dim NewRatioA As Single
    Dim NewRatioB As Single
    Dim Delta As Single

    rkey = SubKey & REG_SPLITTERS & Me.Name

    Set Reg = New Registry
    Direction = Reg.ReadValue(REG_DIRECTION, mvarDirection, rkey)
    If Direction <> mvarDirection Then Exit Sub

    If mvarDirection = SPLIT_DIR_VERT Then
        CurCoord = mvarSplitCtrl.Top
        CurSizeA = mvarCtrlA.Height
        CurSizeB = mvarCtrlB.Height
    ElseIf mvarDirection = SPLIT_DIR_HORZ Then
        CurCoord = mvarSplitCtrl.Left
        CurSizeA = mvarCtrlA.Width
        CurSizeB = mvarCtrlB.Width
    Else
        Exit Sub ' bad data
    End If

    CurOverallSize = CurSizeA + CurSizeB
    CurRatioA = CurSizeA / CurOverallSize
    CurRatioB = 1 - CurRatioA

    i = CLng(Round(CurCoord, 3) * 10000)
    i = Reg.ReadValue(REG_COORD, i, rkey)
    NewCoord = CSng(i) / 10000

    i = CLng(Round(CurSizeA, 3) * 10000)
    i = Reg.ReadValue(REG_SIZE_A, i, rkey)
    NewSizeA = CSng(i) / 10000

    i = CLng(Round(CurSizeB, 3) * 10000)
    i = Reg.ReadValue(REG_SIZE_B, i, rkey)
    NewSizeB = CSng(i) / 10000

    i = CLng(Round(CurOverallSize, 3) * 10000)
    i = Reg.ReadValue(REG_OVERALL_SIZE, i, rkey)
    NewOverallSize = CSng(i) / 10000

    i = CLng(Round(CurRatioA, 3) * 100000)
    i = Reg.ReadValue(REG_RATIO_A, i, rkey)
    NewRatioA = CSng(i) / 100000

    i = CLng(Round(CurRatioB, 3) * 100000)
    i = Reg.ReadValue(REG_RATIO_B, i, rkey)
    NewRatioB = CSng(i) / 100000

    If NewOverallSize <> CurOverallSize Then
        NewSizeA = NewRatioA * NewOverallSize
'        NewSizeA = NewSizeA * (NewOverallSize / CurOverallSize)
        NewSizeB = NewOverallSize - NewSizeA
    End If

    Delta = NewSizeA - CurSizeA
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
    If mvarDirection = SPLIT_DIR_VERT Then
        Coord = Splitter.Top
    Else
        Coord = Splitter.Left
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
Public Property Get MinSizeB() As Single
    MinSizeB = mvarMinSizeB
End Property
Public Property Let MinSizeB(ByVal NewValue As Single)
    mvarMinSizeB = NewValue
End Property
Public Property Get MaxSizeB() As Single
    MaxSizeB = mvarMaxSizeB
End Property
Public Property Let MaxSizeB(ByVal NewValue As Single)
    mvarMaxSizeB = NewValue
End Property
Public Sub MouseDown(X As Single, Y As Single)
    mvarSplitCtrl.Ctrl.ZOrder 1
    If mvarDirection = SPLIT_DIR_VERT Then
        mvarOrigCoord = Y + mvarSplitCtrl.Top
        mvarOrigSizeA = mvarCtrlA.Height
        mvarOrigSizeB = mvarCtrlB.Height
        mvarOrigPosB = mvarCtrlB.Top
    Else
        mvarOrigCoord = X + mvarSplitCtrl.Left
        mvarOrigSizeA = mvarCtrlA.Width
        mvarOrigSizeB = mvarCtrlB.Width
        mvarOrigPosB = mvarCtrlB.Left
    End If

    mvarSplitting = True
    RaiseEvent Begun
End Sub
Public Sub MouseMove(X As Single, Y As Single)
    Dim Delta As Single
    Dim CurSizeA As Single
    Dim SizeA As Single
    Dim SizeB As Single

    If Not mvarSplitting Then Exit Sub

    If mvarDirection = SPLIT_DIR_VERT Then
        Delta = (Y + mvarSplitCtrl.Top) - mvarOrigCoord
        If Delta = 0 Then Exit Sub

        SizeA = mvarOrigSizeA + Delta
        CurSizeA = mvarCtrlA.Height
        If CurSizeA = SizeA Then ' no change
            Exit Sub
        End If

        If SizeA < mvarMinSizeA Then
            SizeA = mvarMinSizeA
            Delta = SizeA - mvarOrigSizeA
            If Delta = 0 Then Exit Sub
        End If

        SizeB = mvarOrigSizeB - Delta
        If SizeB < mvarMinSizeB Then
            SizeB = mvarMinSizeB
            Delta = mvarOrigSizeB - SizeB
            If Delta = 0 Then Exit Sub
            SizeA = mvarOrigSizeA + Delta
        End If

        On Error Resume Next
        mvarCtrlA.Height = SizeA
        mvarCtrlB.Top = mvarOrigPosB + Delta
        mvarSplitCtrl.Top = mvarCtrlA.Top + SizeA + 1
        mvarCtrlB.Height = SizeB
    Else
        Delta = (X + mvarSplitCtrl.Left) - mvarOrigCoord
        If Delta = 0 Then Exit Sub

        SizeA = mvarOrigSizeA + Delta
        CurSizeA = mvarCtrlA.Width
        If CurSizeA = SizeA Then ' no change
            Exit Sub
        End If

        If SizeA < mvarMinSizeA Then
            SizeA = mvarMinSizeA
            Delta = SizeA - mvarOrigSizeA
            If Delta = 0 Then Exit Sub
        End If

        SizeB = mvarOrigSizeB - Delta
        If SizeB < mvarMinSizeB Then
            SizeB = mvarMinSizeB
            Delta = mvarOrigSizeB - SizeB
            If Delta = 0 Then Exit Sub
            SizeA = mvarOrigSizeA + Delta
        End If

        On Error Resume Next
        mvarCtrlA.Width = SizeA
        mvarCtrlB.Left = mvarOrigPosB + Delta
        mvarSplitCtrl.Left = mvarCtrlA.Left + SizeA + 1
        mvarCtrlB.Width = SizeB
    End If

    DoEvents
    mvarAccumDelta = mvarAccumDelta + Delta
    RaiseEvent Changed
End Sub
Public Sub MouseUp(X As Single, Y As Single)
    Dim Delta As Single

    If Not mvarSplitting Then Exit Sub

    mvarSplitting = False
    If mvarDirection = SPLIT_DIR_VERT Then
        If (X < mvarSplitCtrl.Left) Or (X > mvarSplitCtrl.Width) Then
            Delta = mvarOrigSizeA - mvarCtrlA.Height
            mvarCtrlA.Height = mvarOrigSizeA
            mvarSplitCtrl.Top = mvarCtrlA.Top + mvarCtrlA.Height + 1
            mvarCtrlB.Top = mvarOrigPosB
            mvarCtrlB.Height = mvarOrigSizeB
        End If
    Else
        If (Y < 0) Or (Y > mvarSplitCtrl.Height) Then
            Delta = mvarOrigSizeA - mvarCtrlA.Width
            mvarCtrlA.Width = mvarOrigSizeA
            mvarSplitCtrl.Left = mvarCtrlA.Left + mvarCtrlA.Width + 1
            mvarCtrlB.Left = mvarOrigPosB
            mvarCtrlB.Width = mvarOrigSizeB
        End If
    End If

    mvarCtrlA.ReInit
    mvarSplitCtrl.ReInit
    mvarCtrlB.ReInit

    mvarAccumDelta = mvarAccumDelta + Delta
    RaiseEvent Changed
    RaiseEvent Finished
    mvarSplitCtrl.Ctrl.ZOrder
End Sub
Public Sub Constructor(CtrlA As PTCtrl, SplitCtrl As PTCtrl, CtrlB As PTCtrl, Optional ID As Long = -1)
    Dim TopDelta As Single
    Dim LeftDelta As Single

    Set mvarCtrlA = CtrlA
    Set mvarSplitCtrl = SplitCtrl
    Set mvarCtrlB = CtrlB
    Set Splitter = mvarSplitCtrl.Ctrl

    mvarID = ID
    mvarName = Mid(Splitter.Name, 4)
    mvarIndex = -1

    mvarMinSizeA = SPLIT_MIN_SIZE
    mvarMinSizeB = SPLIT_MIN_SIZE

    TopDelta = Abs(mvarCtrlA.Top - mvarCtrlB.Top)
    LeftDelta = Abs(mvarCtrlA.Left - mvarCtrlB.Left)
    If TopDelta < LeftDelta Then  ' controls are side-by-side
        mvarDirection = SPLIT_DIR_HORZ
        Set Splitter.MouseIcon = LoadResPicture("SPLITH", vbResCursor)
        Splitter.MousePointer = vbCustom
        mvarSplitCtrl.Top = mvarCtrlA.Top
        mvarSplitCtrl.Left = (mvarCtrlA.Left + mvarCtrlA.Width) + 1
        mvarSplitterThickness = mvarCtrlB.Left - (mvarCtrlA.Left + mvarCtrlA.Width)
        mvarSplitCtrl.Width = mvarSplitterThickness
        mvarSplitCtrl.Height = mvarCtrlA.Height
        mvarCtrlB.Top = mvarCtrlA.Top
        mvarCtrlB.Height = mvarCtrlA.Height
        mvarMaxSizeA = mvarCtrlA.Width + mvarSplitterThickness + mvarCtrlB.Width - 10
        mvarMaxSizeB = mvarMaxSizeA
    Else                        ' controls are one above the other
        mvarDirection = SPLIT_DIR_VERT
        Set Splitter.MouseIcon = LoadResPicture("SPLITV", vbResCursor)
        Splitter.MousePointer = vbCustom
        mvarSplitCtrl.Left = mvarCtrlA.Left
        mvarSplitCtrl.Top = mvarCtrlA.Top + mvarCtrlA.Height + 1
        mvarSplitterThickness = mvarCtrlB.Top - (mvarCtrlA.Top + mvarCtrlA.Height)
        mvarSplitCtrl.Height = mvarSplitterThickness
        mvarSplitCtrl.Width = mvarCtrlA.Width
        mvarCtrlB.Left = mvarCtrlA.Left
        mvarCtrlB.Width = mvarCtrlA.Width
        mvarMaxSizeA = mvarCtrlA.Height + mvarSplitterThickness + mvarCtrlB.Height - 10
        mvarMaxSizeB = mvarMaxSizeA
    End If

    mvarCtrlA.ReInit
    mvarSplitCtrl.ReInit
    mvarCtrlB.ReInit
    mvarSplitCtrl.Ctrl.ZOrder
End Sub
Public Sub ManualMove(Delta As Single)
    Dim inx As Long
    Dim Dummy As Single

    If Delta = 0 Then Exit Sub

    If mvarDirection = SPLIT_DIR_VERT Then
        Dummy = mvarSplitCtrl.Left
        MouseDown 0, 0
        MouseMove 0, Delta
        MouseUp Dummy, 0
    Else
        Dummy = mvarSplitCtrl.Top
        MouseDown 0, 0
        MouseMove Delta, 0
        MouseUp 0, Dummy
    End If
End Sub
Public Sub Resize()
    Dim Delta As Single

    If mvarDirection = SPLIT_DIR_VERT Then
        mvarSplitCtrl.Top = mvarCtrlA.Top + mvarCtrlA.Height + 1
        Delta = mvarCtrlB.Top - (mvarCtrlA.Top + mvarCtrlA.Height)
        If Delta > mvarSplitterThickness Then
            Delta = (Delta - mvarSplitterThickness) / 2
            mvarCtrlA.Height = mvarCtrlA.Height + Delta
            mvarCtrlB.Top = mvarCtrlB.Top - Delta
            mvarCtrlB.Height = mvarCtrlB.Height + Delta
        ElseIf Delta < mvarSplitterThickness Then
            Delta = (Delta - mvarSplitterThickness) / 2
            mvarCtrlA.Height = mvarCtrlA.Height + Delta
            mvarCtrlB.Top = mvarCtrlB.Top - Delta
            mvarCtrlB.Height = mvarCtrlB.Height + Delta
        End If
    Else
        mvarSplitCtrl.Left = mvarCtrlA.Left + mvarCtrlA.Width + 1
        Delta = mvarCtrlB.Left - (mvarCtrlA.Left + mvarCtrlA.Width)
        If Delta > mvarSplitterThickness Then
            Delta = (Delta - mvarSplitterThickness) / 2
            mvarCtrlA.Width = mvarCtrlA.Width + Delta
            mvarCtrlB.Left = mvarCtrlB.Left - Delta
            mvarCtrlB.Width = mvarCtrlB.Width + Delta
        ElseIf Delta < mvarSplitterThickness Then
            Delta = (Delta - mvarSplitterThickness) / 2
            mvarCtrlA.Width = mvarCtrlA.Width + Delta
            mvarCtrlB.Left = mvarCtrlB.Left - Delta
            mvarCtrlB.Width = mvarCtrlB.Width + Delta
        End If
    End If
End Sub
Private Sub Class_Terminate()
    Set mvarCtrlA = Nothing
    Set mvarSplitCtrl = Nothing
    Set mvarCtrlB = Nothing
    Set Splitter = Nothing
End Sub
Private Sub Splitter_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button And vbLeftButton <> 0 Then
        MouseDown X, Y
    End If
End Sub
Private Sub Splitter_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button And vbLeftButton <> 0 Then
        MouseMove X, Y
    End If
End Sub
Private Sub Splitter_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button And vbLeftButton <> 0 Then MouseUp X, Y
End Sub

