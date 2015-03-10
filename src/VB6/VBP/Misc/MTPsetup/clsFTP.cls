VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "FTP"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private fso       As New FileSystemObject
Private gsFTPForm As frmMTPsetup  'Form housing FTP control
Private gobjFTP   As Inet    'FTP control object

Public Function FTPGetAll(ByVal sLocalFolder As String, _
    ByVal sFileNameString As String, ByVal sFTPFolder As String, _
    sError As String, ByRef FileList() As String, ByRef nFiles%, _
    ByRef DirectoryLengthLast As Long) As Boolean
'
' Retrieve all files whose name contains a particular string
' from a folder on the FTP server into the local folder.
'
Dim vChunk       As Variant
Dim bDone        As Boolean
Dim sData        As String
Dim sDirectory   As String
Dim sErrorText   As String
Dim lErrorNumber As Long

  FTPGetAll = False
  On Error GoTo FTPGetAllError
'
' Change folders on the FTP server and get a list of files.
'
  With gobjFTP
    .Execute , "CD " & sFTPFolder
    Call pWaitForResponse
    If gobjFTP.ResponseCode <> 0 Then GoTo FTPGetAllError
    
    .Execute , "Dir"
    Call pWaitForResponse
    
    vChunk = .GetChunk(256)
    Call pWaitForResponse
    
    Do Until bDone
      sDirectory = sDirectory & vChunk
      vChunk = .GetChunk(256)
      If Len(vChunk) = 0 Then bDone = True
    Loop
  End With
'
' If there are files, get them.
'
  If sDirectory = "" Then
    sError = "No files found."
  Else
    If Len(sDirectory) > DirectoryLengthLast Then
      FTPGetAll = fGetFileList(sLocalFolder, UCase$(sFileNameString), sDirectory, sFTPFolder, sError, FileList(), nFiles)
      DirectoryLengthLast = Len(sDirectory)
    End If
  End If

Exit Function
    
FTPGetAllError:
  lErrorNumber = gobjFTP.ResponseCode
  If lErrorNumber <> 0 Then
    sErrorText = gobjFTP.ResponseInfo
  Else
    sErrorText = "Error during FTP Get operation."
  End If
  sError = fFmtErrorTxt(sErrorText, "FTPGetAll", "")

End Function
Public Function FTPPutAll(ByVal sLocalFolder As String, _
    ByVal sFileNameString As String, ByVal sFTPFolder As String, _
    sError As String) As Boolean
'
' Loop through all files in the local folder. FTP each file,
' whose name contains a particular string, to a folder on the FTP
' server.
'
Dim bFiles    As Boolean
Dim sFileName As String
Dim ofile     As File
Dim ofiles    As Files

bFiles = False
FTPPutAll = False
On Error GoTo FTPPutAllError

Set ofiles = fso.GetFolder(sLocalFolder).Files
For Each ofile In ofiles
    sFileName = UCase$(ofile.Name)
    If InStr(1, sFileName, UCase$(sFileNameString)) > 0 Then
        bFiles = True
        If Not FTPPut(sLocalFolder, sFileName, sFTPFolder, sError) Then
            Exit Function
        End If
    End If
Next
Set ofile = Nothing
Set ofiles = Nothing

If bFiles Then
    FTPPutAll = True
Else
    sError = "No files found."
End If
Exit Function
    
FTPPutAllError:
    sError = fFmtErrorTxt("Error during FTP Put operation.", "FTPPutAll", "")
    Set ofile = Nothing
    Set ofiles = Nothing
End Function

Private Sub pWaitForResponse()
'
' This loop waits until an FTP action completes.
'
Do Until Not gobjFTP.StillExecuting
    DoEvents
Loop
End Sub

Private Function fGetFileList(ByVal sLocalFolder As String, _
    ByVal sFileType As String, ByVal sDirectory As String, _
    ByVal sFTPFolder As String, sError As String, ByRef FileList() As String, _
    ByRef nFiles%) As Boolean

Dim i          As Integer
Dim j As Integer
Dim sFTPFile   As String
Dim sFTPFileUC As String
Dim sMatchStr  As String
Dim fso As New FileSystemObject
Dim Status2 As Boolean
'
' Loop through the directory listing and Get each
' file of type sFileType from the FTP Server folder
' to the local folder.
'
  fGetFileList = False
  On Error GoTo fGetFileListError
  j = 0
  sMatchStr = UCase$(sFileType)
  Do Until Len(sDirectory) = 0
    i = InStr(1, sDirectory, vbCr)
    If i > 0 Then
        If Len(sDirectory) <= 2 Then
            sDirectory = ""
            Exit Do
        End If
        sFTPFile = Left$(sDirectory, i - 1)
        sFTPFileUC = UCase$(sFTPFile)
        'If InStr(1, sFTPFileUC, sMatchStr) > 0 And Left$(sFTPFile, 11) = "MTPcurrent_" Then
        'If Left$(sFTPFile, 19) = Left$(frmMTPftp.txtFilenameDate.Text, 19) And Right$(sFTPFile, 3) = "txt" Then
          j = j + 1
          FileList(j) = sFTPFile
        'End If
        sDirectory = Mid$(sDirectory, i + 2)
    End If
  Loop
  If j > 0 Then nFiles = j
  fGetFileList = True

  For i = 1 To nFiles
    If Not fso.FileExists(frmMTPsetup.txtLocalFolder.Text + FileList(i)) Then
      Status2 = FTPGet(sLocalFolder, FileList(i), sFTPFolder, sError, False)
    End If
  Next i
  Exit Function
  
fGetFileListError:
  sError = fFmtErrorTxt("Error while getting multiple files.", "fGetFileList", "")

End Function
Private Sub Class_Initialize()
'
' Create a new instance of the form holding the
' Internet Transfer Control so that each instance
' of this class has its own version preventing
' any contention of resources. Assign the form
' and control to Class Level variables.
'
Set gsFTPForm = New frmMTPsetup
Set gobjFTP = gsFTPForm.ctlInternet
End Sub
Private Sub Class_Terminate()
'
' Clear object variables and unload the form.
'
Unload gsFTPForm
Set gobjFTP = Nothing
Set gsFTPForm = Nothing
End Sub
Public Function FTPPut(ByVal sLocalFolder As String, _
    ByVal sFileName As String, ByVal sFTPFolder As String, _
    sError As String) As Boolean
'
' Copy a single file from the local folder to the FTP server.
'
Dim sErrorText   As String
Dim lErrorNumber As Long

FTPPut = False
On Error GoTo FTPPutError
gobjFTP.Execute , "Put """ & sLocalFolder & "\" & sFileName & _
                  """ " & sFTPFolder & "/" & sFileName
Call pWaitForResponse
'
' Check for errors.
'
If gobjFTP.ResponseCode <> 0 Then GoTo FTPPutError
FTPPut = True
Exit Function

FTPPutError:
    lErrorNumber = gobjFTP.ResponseCode
    If lErrorNumber <> 0 Then
        sErrorText = gobjFTP.ResponseInfo
    Else
        sErrorText = "Error during FTP Put operation."
    End If
    sError = fFmtErrorTxt(sErrorText, "FTPPut", "")
End Function


Public Function FTPGet(ByVal sLocalFolder As String, _
    ByVal sFileName As String, ByVal sFTPFolder As String, _
    sError As String, Optional ByVal bSpecifyFolder As Boolean = True) As Boolean
'
' Get a single file from a folder on the
' FTP server and copy it to the local folder.
'
Dim sLocalFile   As String
Dim sErrorText   As String
Dim lErrorNumber As Long

FTPGet = False
On Error GoTo FTPGetError
'
' If the file exists locally, delete it first.
'
sLocalFile = sLocalFolder & sFileName
'If fso.FileExists(sLocalFile) Then Call fso.DeleteFile(sLocalFile, True)
'
' Get the file.
'
'dss
If bSpecifyFolder Then
    '
    ' Specify the folder.
    ' Both formats below work!
    'gobjFTP.Execute , "Get " & sFTPFolder & "/" & sFileName & " """ & sLocalFile & """"
    gobjFTP.Execute , "Get " & sFTPFolder & "/" & sFileName & " " & sLocalFile
Else
    '
    ' Do not specify the folder since a Change Directory
    ' was already issued by fGetFileList.
    '
    'gobjFTP.Execute , "Get " & sFTPFolder & "/" & sFileName & " " & sLocalFile
    gobjFTP.Execute , "Get " & sFileName & " " & sLocalFile '& """"

End If
Call pWaitForResponse
'
' Check for errors.
'
If gobjFTP.ResponseCode <> 0 Then GoTo FTPGetError
FTPGet = True
Exit Function

FTPGetError:
    lErrorNumber = gobjFTP.ResponseCode
    If lErrorNumber = 0 Then
        sErrorText = "Error during FTP Get operation."
    Else
        sErrorText = gobjFTP.ResponseInfo
    End If
    sError = fFmtErrorTxt(sErrorText, "FTPGet", "")
End Function
Public Sub FTPClose()

'
' Close the FTP connection.
'
On Error Resume Next
gobjFTP.Execute , "CLOSE"
End Sub
Public Function FTPInit(ByVal sURL As String, ByVal sUserName As String, _
           ByVal sPassword As String, ByVal iTimeOut As Integer, _
           ByVal iRemotePort As Integer, ByVal sProxy As String, _
           sError As String) As Boolean

'
' Set the FTP parameters required to establish a connection.
'
' NOTE: This causes form "frmFTP" to load. It is
'       unloaded when this object is destroyed.
'
On Error GoTo FTPInitError
FTPInit = False
With gobjFTP
    .URL = sURL
    .UserName = sUserName
    .Password = sPassword
    .Protocol = icFTP               'Use FTP transfer protocol.
    
    If iTimeOut = 0 Then            'Wait 60 seconds for a response
        .RequestTimeout = 60        ' if no value was passed in.
    Else
        .RequestTimeout = iTimeOut
    End If
    
    If iRemotePort <> 0 Then        'Use the default port (typically
        .RemotePort = iRemotePort   ' 21) if no value was passed in.
    End If
    
    If sProxy = "" Then             'If no proxy was passed in,
        .AccessType = icUseDefault  ' use the default registry
    Else                            ' settings.
        .AccessType = icNamedProxy
        .Proxy = sProxy
    End If
End With
FTPInit = True
Exit Function

FTPInitError:
    sError = fFmtErrorTxt("Error initializing FTP connection.", "FTPInitError", "")
End Function
Private Function fFmtErrorTxt(ByVal sError As String, ByVal sProcedure As String, ByVal sADOErr As String)
Dim sErr As String

fFmtErrorTxt = "Error Msg: " & sError & vbCrLf & _
                "Procedure: " & sProcedure

If sADOErr = "" Then
    If Err.Number <> 0 Then
        sErr = "Error Number: " & CStr(Err.Number)
    End If
    If Err.Source = "" Then
        sErr = sErr & vbCrLf & "Error Source: " & "FTP Class"
    Else
        sErr = sErr & vbCrLf & "Error Source: " & Err.Source
    End If
    If Err.Description <> "" Then
        sErr = sErr & vbCrLf & "Description: " & Err.Description
    End If
    fFmtErrorTxt = fFmtErrorTxt & vbCrLf & sErr
Else
    fFmtErrorTxt = fFmtErrorTxt & vbCrLf & sADOErr
End If
End Function










