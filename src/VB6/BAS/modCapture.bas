Attribute VB_Name = "modCapture"
Option Explicit
' Note Escape key exits program
' SpaceBar cancels AutoName feature

Private Type TForms
    hwnd As Long
    Caption As String
End Type

Private Const SW_RESTORE = 9

Private Declare Function GetWindowText Lib "user32" Alias "GetWindowTextA" (ByVal hwnd As Long, ByVal lpString As String, ByVal cch As Long) As Long
Private Declare Function EnumWindows Lib "user32" (ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Private Declare Function ShowWindow Lib "user32" (ByVal hwnd As Long, ByVal nCmdShow As Long) As Long
Private Declare Function SetForegroundWindow Lib "user32" (ByVal hwnd As Long) As Long

Public m_iScaleHeight As Single, m_iScaleWidth As Single
Public m_frmCapture As frmCapture
Public m_frmDesktop As frmDesktop
Public g_sDefaultFolder As String
Public m_Forms() As TForms
Public g_iiW As Long

' Program setting variables
Public g_iQualityJPG As Long
Public g_sDefaultExtension As String
Public g_sDefaultFilename As String
Public g_sAutoNameFilename As String  'Used during autosave
Public g_bAutoName As Boolean         'FALSE don't autocapture, TRUE autocapture
Public g_bTransparency As Boolean     'Use lower LHC pixel to set transparency if true
Public g_bAutoExit As Boolean         'Close program after a single image capture


Private Const OFN_HIDEREADONLY = &H4
Private Const OFN_FILEMUSTEXIST = &H1000
Private Const OFN_OVERWRITEPROMPT = &H2
Private Const OFN_CREATEPROMPT = &H2000

Private Type OPENFILENAME
    lStructSize As Long
    hWndOwner As Long
    hInstance As Long
    lpstrFilter As String
    lpstrCustomFilter As String
    nMaxCustFilter As Long
    nFilterIndex As Long
    lpstrFile As String
    nMaxFile As Long
    lpstrFileTitle As String
    nMaxFileTitle As Long
    lpstrInitialDir As String
    lpstrTitle As String
    flags As Long
    nFileOffset As Integer
    nFileExtension As Integer
    lpstrDefExt As String
    lCustData As Long
    lpfnHook As Long
    lpTemplateName As String
End Type

'Private Declare Function GetSaveFileNameA Lib "comdlg32.dll" (pOpenfilename As OPENFILENAME) As Long
'Private Declare Function GetTempPathA Lib "kernel32" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
'Private Declare Function GetTempFileNameA Lib "kernel32" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
'Public Declare Function DeleteFile Lib "kernel32" Alias "DeleteFileA" (ByVal lpFileName As String) As Long
'Public Declare Function RemoveDirectory Lib "kernel32" Alias "RemoveDirectoryA" (ByVal lpPathName As String) As Long

Private Declare Function GetSaveFileNameA Lib "comdlg32.dll" (pOpenfilename As OPENFILENAME) As Long
'Private Declare Function GetTempPathA Lib "kernel32" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
'Private Declare Function GetTempFileNameA Lib "kernel32" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
Public Declare Function DeleteFile Lib "kernel32" Alias "DeleteFileA" (ByVal lpFileName As String) As Long
Public Declare Function RemoveDirectory Lib "kernel32" Alias "RemoveDirectoryA" (ByVal lpPathName As String) As Long


Public Function FileSaveDialog(ByVal hWndOwner As Long, ByVal Filter As String, ByVal Title As String, ByVal InitDir As String, ByVal strFileDefault As String) As String
    Dim ofn As OPENFILENAME, i As Long, sFile As String, sExt As String

    ofn.lpstrFile = strFileDefault & Space$(254 - Len(strFileDefault))
    ofn.lStructSize = Len(ofn)
    ofn.hWndOwner = hWndOwner
    ofn.hInstance = App.hInstance
    ofn.nMaxFile = 255
    ofn.lpstrFileTitle = Space$(254)
    ofn.nMaxFileTitle = 255
    ofn.lpstrInitialDir = InitDir
    ofn.lpstrTitle = Title
    ofn.flags = OFN_HIDEREADONLY Or OFN_OVERWRITEPROMPT Or OFN_CREATEPROMPT
    ofn.lpstrFilter = FilterForDialog(Filter)
    ofn.nFilterIndex = 1
    i = GetSaveFileNameA(ofn)

    If (i) Then
        i = InStr(ofn.lpstrFile, Chr(0))
        If i > 0 Then
            sFile = Trim$(Left$(ofn.lpstrFile, i - 1))
        Else
            sFile = Trim$(ofn.lpstrFile)
        End If
        Do While Right$(sFile, 1) = "."
            sFile = Left$(sFile, Len(sFile) - 1)
        Loop
        If Len(sFile) > 0 Then
            sExt = LCase$(Right$(sFile, 4))
            Select Case ofn.nFilterIndex
                Case 1  ' PNG
                    If sExt <> ".png" Then sFile = sFile & ".png"
                Case 2  ' JPG
                    If sExt <> ".jpg" Then sFile = sFile & ".jpg"
                Case 3  ' BMP
                    If sExt <> ".bmp" Then sFile = sFile & ".bmp"
                Case 4  ' GIF
                    If sExt <> ".gif" Then sFile = sFile & ".gif"
            End Select
        End If
    End If
    FileSaveDialog = sFile
End Function

Private Function FilterForDialog(ByVal strFilter As String) As String
    Dim i As Integer, j As Integer
    strFilter = Trim$(strFilter)
    If Len(strFilter) = 0 Then Exit Function
    For i = Len(strFilter) To 0 Step -1
        If Mid$(strFilter, i, 1) <> "|" Then Exit For
    Next i
    strFilter = Left$(strFilter, i)
    If Len(strFilter) = 0 Then Exit Function
    i = 0
    Do
        i = InStr(i + 1, strFilter, "|"): If i = 0 Then Exit Do
        If i > 0 Then Mid$(strFilter, i, 1) = Chr(0)
    Loop
    strFilter = strFilter & Chr(0) & Chr(0)
    FilterForDialog = strFilter
End Function


Public Sub Main()

  If PrevInst Then Exit Sub
' Load program settings from Windows Registry
  g_iQualityJPG = GetSetting(App.Title, "Setting", "QualityJPG", 80)
  g_sDefaultFolder = GetSetting(App.Title, "Setting", "DefaultFolder", App.Path)
  g_sDefaultExtension = GetSetting(App.Title, "Setting", "DefaultExtension", "PNG")
  g_sDefaultFilename = GetSetting(App.Title, "Setting", "DefaultFileName", "temp")
  g_bTransparency = GetSetting(App.Title, "Setting", "Transparency", False)
  g_bAutoName = GetSetting(App.Title, "Setting", "AutoName", False)
  g_bAutoExit = GetSetting(App.Title, "Setting", "AutoExit", False)
  g_sAutoNameFilename = GetSetting(App.Title, "Setting", "AutoNameFilename", "Capture")
  
  Set m_frmDesktop = New frmDesktop
  m_frmDesktop.Show

End Sub

Public Function PrevInst() As Boolean
    Dim sTitle As String, hwnd As Long, bPaint As Boolean, sPaint As String
    
    sTitle = "Capture"
    App.Title = "New name"
    sPaint = sTitle & ".bmp - Paint"
    
    ReDim m_Forms(0)
    EnumWindows AddressOf Callback_EnumWindows, 0
    
    hwnd = WindowFind(sPaint): bPaint = hwnd
    If hwnd = 0 Then hwnd = WindowFind(sTitle & "_W")
    If hwnd = 0 Then hwnd = WindowFind(sTitle & " Options")
    If hwnd = 0 Then hwnd = WindowFind(sTitle & "_D")
    If hwnd = 0 Then hwnd = WindowFind(sTitle)
    If hwnd = 0 Then
        App.Title = sTitle
    Else
        If bPaint Then sTitle = sPaint
        If Not WindowFind(sTitle & "_D") Then
            ShowWindow hwnd, SW_RESTORE
            SetForegroundWindow hwnd
        End If
        PrevInst = True
    End If
End Function

Private Function WindowFind(sCaption) As Long
    Dim i As Long
    For i = 1 To UBound(m_Forms)
        If sCaption = m_Forms(i).Caption Then
            WindowFind = m_Forms(i).hwnd
            Exit Function
        End If
    Next i
End Function

Public Function Callback_EnumWindows(ByVal hwnd As Long, ByVal lpData As Long) As Long
    Dim cnt As Long, i As Long, sTitle As String * 256
    
    cnt = GetWindowText(hwnd, sTitle, 255)
    ' Find Next Window
    If cnt > 0 Then
        If Left$(sTitle, 7) = "Snap-It" Then
            i = UBound(m_Forms) + 1
            ReDim Preserve m_Forms(i)
            m_Forms(i).Caption = Left$(sTitle, cnt)
            m_Forms(i).hwnd = hwnd
        End If
    End If
    Callback_EnumWindows = 1
End Function
