VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "REG_KEY_ARRAY"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"No"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 1.5 4/18/2004

'local variable(s) to hold property value(s)
Private mvarCount As Long
Private mvarKey() As REG_KEY_ENTRY
Public Function KeyAdd(Optional Name As String = "", Optional Tag As String = "") As Long
    ReDim Preserve mvarKey(mvarCount)
    Set mvarKey(mvarCount) = New REG_KEY_ENTRY
    mvarKey(mvarCount).Name = Name
    mvarKey(mvarCount).Tag = Tag
    KeyAdd = mvarCount
    mvarCount = mvarCount + 1
End Function
Public Property Set Key(inx As Long, ByRef NewValue As REG_KEY_ENTRY)
    Set mvarKey(inx) = NewValue
End Property
Public Property Get Key(inx As Long) As REG_KEY_ENTRY
    Set Key = mvarKey(inx)
End Property
Private Sub Class_Initialize()
'    Set mvarKeyEntry = New REG_KEY_ENTRY
End Sub
Public Property Get Count() As Long
    Count = mvarCount
End Property
Public Property Get UpperBound() As Long
    UpperBound = mvarCount - 1
End Property
Public Property Get LowerBound() As Long
    LowerBound = 0
End Property



