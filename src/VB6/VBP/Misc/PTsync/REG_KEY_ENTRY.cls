VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "REG_KEY_ENTRY"
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

Public Enum REG_KEY_ENTRY_ERROR
    PROPERTY_READONLY = vbObjectError + 1
    SERIALIZE_EMPTY
    SERIALIZE_INVALID
    SERIALIZE_BAD_VERSION
End Enum

Private mvarName As String
Private mvarTag As String
Private mvarLastWriteTime As Date
Public Property Let Tag(ByVal NewTag As String)
    mvarTag = NewTag
End Property
Public Property Get Tag() As String
    Tag = mvarTag
End Property
Public Property Let Name(ByVal NewName As String)
    mvarName = NewName
End Property
Public Property Get Name() As String
    Name = mvarName
End Property
Public Property Let LastWriteTime(ByVal NewTime As Date)
    mvarLastWriteTime = NewTime
End Property
Public Property Get LastWriteTime() As Date
    LastWriteTime = mvarLastWriteTime
End Property


