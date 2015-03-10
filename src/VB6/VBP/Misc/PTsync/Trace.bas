Attribute VB_Name = "Tracer"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 2.1 8/1/2004

#If TRACE_ON Then
Private InitDone As Boolean
Private Const TRACE_FID = "\Trace.txt"
Private Function TraceError(Loc As String) As VbMsgBoxResult
    Dim yorn As VbMsgBoxResult

    yorn = MsgBox("Error in " & Loc & vbCrLf & CStr(Err.Number) & " (" & Err.Description & ")", vbCritical Or vbOKCancel, App.FileDescription)
    TraceError = yorn
End Function
Public Sub TracePurge()
    Dim TraceFN As Long
    Dim yorn As VbMsgBoxResult

    On Error Resume Next
    TraceFN = FreeFile()
    Open App.Path & TRACE_FID For Output Access Write As TraceFN
    If Err <> 0 Then
        yorn = TraceError("TracePurge")
        If yorn <> vbOK Then End
    End If

    Print #TraceFN, "## " & Now() & " ** Started"
    If Err <> 0 Then
        yorn = TraceError("TracePurge Print")
        If yorn <> vbOK Then End
    End If
    Close #TraceFN
    TraceFN = 0
End Sub
Public Sub Trace(msg As String)
    Dim TraceFN As Long
    Dim yorn As VbMsgBoxResult
    Dim WinVer As PT_OS_INFO

    If Not InitDone Then
        InitDone = True
        WinVer = ApiWinVersion()
        Trace "User: " & ApiUserName()
        Trace "Computer: " & ApiComputerName()
        Trace "Windows Version: " & WinVer.Desc
    End If

    On Error Resume Next
    TraceFN = FreeFile()
    Open App.Path & TRACE_FID For Append Access Write As TraceFN
    If Err <> 0 Then
        yorn = TraceError("TraceOpen")
        If yorn <> vbOK Then End
    End If

    Print #TraceFN, "## " & Now() & vbTab;
    Print #TraceFN, msg
    If Err <> 0 Then
        yorn = TraceError("Trace Print")
        If yorn <> vbOK Then End
    End If
    Close #TraceFN
End Sub
#End If ' TRACE_ON
