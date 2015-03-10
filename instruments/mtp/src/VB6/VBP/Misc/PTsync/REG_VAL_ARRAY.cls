VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "REG_VAL_ARRAY"
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
Private mvarDatum() As REG_VAL_ENTRY
Public Function ValueAdd(Optional Name As String = "", Optional DataType As REG_TYPE = REG_DWORD, Optional Data As Variant = 0, Optional Size As Long = 4, Optional Tag As String = "") As Long
    ReDim Preserve mvarDatum(mvarCount)
    Set mvarDatum(mvarCount) = New REG_VAL_ENTRY
    mvarDatum(mvarCount).Name = Name
    mvarDatum(mvarCount).DataType = DataType
    mvarDatum(mvarCount).Size = Size
    mvarDatum(mvarCount).Data = Data
    mvarDatum(mvarCount).Tag = Tag
    ValueAdd = mvarCount
    mvarCount = mvarCount + 1
End Function
Public Property Set Value(inx As Long, ByRef NewValue As REG_VAL_ENTRY)
    Set mvarDatum(inx) = NewValue
End Property
Public Property Get Value(inx As Long) As REG_VAL_ENTRY
    Set Value = mvarDatum(inx)
End Property
Private Sub Class_Initialize()
'    Set mvarKeyEntry = New REG_VAL_ENTRY
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

