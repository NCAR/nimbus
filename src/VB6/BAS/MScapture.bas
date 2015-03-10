Attribute VB_Name = "MScapture"
'--------------------------------------------------------------------
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' From http://support.microsoft.com/kb/161299/EN-US/
' MJM January 23, 2008
' Visual Basic 4.0 16/32 Capture Routines
'
' This module contains several routines for capturing windows into a picture.
' All the routines work on both 16 and 32 bit Windows platforms.
' The routines also have palette support.
'
' CreateBitmapPicture -   Creates a picture object from a bitmap and palette.
' CaptureWindow -         Captures any window given a window handle.
' CaptureActiveWindow -   Captures the active window on the desktop.
' CaptureForm -           Captures the entire form.
' CaptureClient -         Captures the client area of a form.
' CaptureScreen -         Captures the entire screen.
' PrintPictureToFitPage - Prints any picture as big as possible on the page.
'
' NOTES
'    - No error trapping is included in these routines.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
  Option Explicit
  Option Base 0

  Private Type PALETTEENTRY
     peRed As Byte
     peGreen As Byte
     peBlue As Byte
     peFlags As Byte
  End Type

  Private Type LOGPALETTE
     palVersion As Integer
     palNumEntries As Integer
     palPalEntry(255) As PALETTEENTRY  ' Enough for 256 colors.
  End Type

  Private Type GUID
     Data1 As Long
     Data2 As Integer
     Data3 As Integer
     Data4(7) As Byte
  End Type

  Private Const RASTERCAPS As Long = 38
  Private Const RC_PALETTE As Long = &H100
  Private Const SIZEPALETTE As Long = 104

  Private Type RECT
   Left As Long
   Top As Long
   Right As Long
   Bottom As Long
  End Type

  Private Declare Function CreateCompatibleDC Lib "GDI32" (ByVal hDC As Long) As Long
  Private Declare Function CreateCompatibleBitmap Lib "GDI32" ( _
            ByVal hDC As Long, ByVal nWidth As Long, _
            ByVal nHeight As Long) As Long
  Private Declare Function GetDeviceCaps Lib "GDI32" ( _
            ByVal hDC As Long, ByVal iCapabilitiy As Long) As Long
  Private Declare Function GetSystemPaletteEntries Lib "GDI32" ( _
            ByVal hDC As Long, ByVal wStartIndex As Long, _
            ByVal wNumEntries As Long, lpPaletteEntries As PALETTEENTRY) _
            As Long
  Private Declare Function CreatePalette Lib "GDI32" ( _
            lpLogPalette As LOGPALETTE) As Long
  Private Declare Function SelectObject Lib "GDI32" ( _
            ByVal hDC As Long, ByVal hObject As Long) As Long
  Private Declare Function BitBlt Lib "GDI32" ( _
            ByVal hDCDest As Long, ByVal XDest As Long, _
            ByVal YDest As Long, ByVal nWidth As Long, _
            ByVal nHeight As Long, ByVal hDCSrc As Long, _
            ByVal XSrc As Long, ByVal YSrc As Long, ByVal dwRop As Long) _
            As Long
  Private Declare Function DeleteDC Lib "GDI32" ( _
            ByVal hDC As Long) As Long
  Private Declare Function GetForegroundWindow Lib "user32" () _
            As Long
  Private Declare Function SelectPalette Lib "GDI32" ( _
            ByVal hDC As Long, ByVal hPalette As Long, _
            ByVal bForceBackground As Long) As Long
  Private Declare Function RealizePalette Lib "GDI32" ( _
            ByVal hDC As Long) As Long
  Private Declare Function GetWindowDC Lib "user32" ( _
            ByVal hwnd As Long) As Long
  Private Declare Function GetDC Lib "user32" ( _
            ByVal hwnd As Long) As Long
  Private Declare Function GetWindowRect Lib "user32" ( _
            ByVal hwnd As Long, lpRect As RECT) As Long
  Private Declare Function ReleaseDC Lib "user32" ( _
            ByVal hwnd As Long, ByVal hDC As Long) As Long
  Private Declare Function GetDesktopWindow Lib "user32" () As Long

  Private Type PicBmp
    Size As Long
    Type As Long
    hBmp As Long
    hPal As Long
    Reserved As Long
  End Type

  Private Declare Function OleCreatePictureIndirect _
            Lib "olepro32.dll" (PicDesc As PicBmp, RefIID As GUID, _
            ByVal fPictureOwnsHandle As Long, IPic As IPicture) As Long

Private Declare Function GetTempPathA Lib "kernel32" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
Private Declare Function GetTempFileNameA Lib "kernel32" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long

'Private Declare Function GetSaveFileNameA Lib "comdlg32.dll" (pOpenfilename As OPENFILENAME) As Long
'Private Declare Function GetTempPathA Lib "kernel32" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
'Private Declare Function GetTempFileNameA Lib "kernel32" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
'Private Declare Function DeleteFile Lib "kernel32" Alias "DeleteFileA" (ByVal lpFileName As String) As Long
'Private Declare Function RemoveDirectory Lib "kernel32" Alias "RemoveDirectoryA" (ByVal lpPathName As String) As Long
'Private GetTempPath$

      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      ' CreateBitmapPicture
      '    - Creates a bitmap type Picture object from a bitmap and
      '      palette.
      '
      ' hBmp
      '    - Handle to a bitmap.
      '
      ' hPal
      '    - Handle to a Palette.
      '    - Can be null if the bitmap doesn't use a palette.
      '
      ' Returns
      '    - Returns a Picture object containing the bitmap.
      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
         Public Function CreateBitmapPicture(ByVal hBmp As Long, _
            ByVal hPal As Long) As Picture

            Dim r As Long
         Dim pic As PicBmp
         ' IPicture requires a reference to "Standard OLE Types."
         Dim IPic As IPicture
         Dim IID_IDispatch As GUID

         ' Fill in with IDispatch Interface ID.
         With IID_IDispatch
            .Data1 = &H20400
            .Data4(0) = &HC0
            .Data4(7) = &H46
         End With

         ' Fill Pic with necessary parts.
         With pic
            .Size = Len(pic)          ' Length of structure.
            .Type = vbPicTypeBitmap   ' Type of Picture (bitmap).
            .hBmp = hBmp              ' Handle to bitmap.
            .hPal = hPal              ' Handle to palette (may be null).
         End With

         ' Create Picture object.
         r = OleCreatePictureIndirect(pic, IID_IDispatch, 1, IPic)

         ' Return the new Picture object.
         Set CreateBitmapPicture = IPic
      End Function

      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      ' CaptureWindow
      '    - Captures any portion of a window.
      '
      ' hWndSrc
      '    - Handle to the window to be captured.
      '
      ' Client
      '    - If True CaptureWindow captures from the client area of the
      '      window.
      '    - If False CaptureWindow captures from the entire window.
      '
      ' LeftSrc, TopSrc, WidthSrc, HeightSrc
      '    - Specify the portion of the window to capture.
      '    - Dimensions need to be specified in pixels.
      '
      ' Returns
      '    - Returns a Picture object containing a bitmap of the specified
      '      portion of the window that was captured.
      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      ''''''
      '
         Public Function CaptureWindow(ByVal hWndSrc As Long, _
            ByVal Client As Boolean, ByVal LeftSrc As Long, _
            ByVal TopSrc As Long, ByVal WidthSrc As Long, _
            ByVal HeightSrc As Long) As Picture

            Dim hDCMemory As Long
            Dim hBmp As Long
            Dim hBmpPrev As Long
            Dim r As Long
            Dim hDCSrc As Long
            Dim hPal As Long
            Dim hPalPrev As Long
            Dim RasterCapsScrn As Long
            Dim HasPaletteScrn As Long
            Dim PaletteSizeScrn As Long
         Dim LogPal As LOGPALETTE

         ' Depending on the value of Client get the proper device context.
         If Client Then
            hDCSrc = GetDC(hWndSrc) ' Get device context for client area.
         Else
            hDCSrc = GetWindowDC(hWndSrc) ' Get device context for entire
                                          ' window.
         End If

         ' Create a memory device context for the copy process.
         hDCMemory = CreateCompatibleDC(hDCSrc)
         ' Create a bitmap and place it in the memory DC.
         hBmp = CreateCompatibleBitmap(hDCSrc, WidthSrc, HeightSrc)
         hBmpPrev = SelectObject(hDCMemory, hBmp)

         ' Get screen properties.
         RasterCapsScrn = GetDeviceCaps(hDCSrc, RASTERCAPS) ' Raster
                                                            ' capabilities.
         HasPaletteScrn = RasterCapsScrn And RC_PALETTE       ' Palette
                                                              ' support.
         PaletteSizeScrn = GetDeviceCaps(hDCSrc, SIZEPALETTE) ' Size of
                                                              ' palette.

         ' If the screen has a palette make a copy and realize it.
         If HasPaletteScrn And (PaletteSizeScrn = 256) Then
            ' Create a copy of the system palette.
            LogPal.palVersion = &H300
            LogPal.palNumEntries = 256
            r = GetSystemPaletteEntries(hDCSrc, 0, 256, _
                LogPal.palPalEntry(0))
            hPal = CreatePalette(LogPal)
            ' Select the new palette into the memory DC and realize it.
            hPalPrev = SelectPalette(hDCMemory, hPal, 0)
            r = RealizePalette(hDCMemory)
         End If

         ' Copy the on-screen image into the memory DC.
         r = BitBlt(hDCMemory, 0, 0, WidthSrc, HeightSrc, hDCSrc, _
            LeftSrc, TopSrc, vbSrcCopy)

      ' Remove the new copy of the  on-screen image.
         hBmp = SelectObject(hDCMemory, hBmpPrev)

         ' If the screen has a palette get back the palette that was
         ' selected in previously.
         If HasPaletteScrn And (PaletteSizeScrn = 256) Then
            hPal = SelectPalette(hDCMemory, hPalPrev, 0)
         End If

         ' Release the device context resources back to the system.
         r = DeleteDC(hDCMemory)
         r = ReleaseDC(hWndSrc, hDCSrc)

         ' Call CreateBitmapPicture to create a picture object from the
         ' bitmap and palette handles. Then return the resulting picture
         ' object.
         Set CaptureWindow = CreateBitmapPicture(hBmp, hPal)
      End Function

Sub CaptureFormMJ(f As Form, PNGfilename$, Mode%, OK As Boolean)
' General form capture routine
' f .............. form
' PNGfilename$ ... fqfn of PNG file saved
'                  if ="" then a filename f.NameNNN.PNG will be created
' Mode ........... 0 ... capture full screen
'                  1 ... capture full form
'                  2 ... capture only client area of form
'                  3 ... capture active window
' OK ............. if TRUE as MsgBox will verify that capture was made
' This sub uses a form named frmCaptureForm with a picturebox control named picImage
' The image is save to a temporary BMP image which is  then converted
' using JG.convertEx routine in the janGraphics.dll library in C:/Windows/system32/
' Also, use Project|References to make sure the janGraphics Library is selected
Dim Filename$, Status As Boolean
Dim i&, DefaultFolder$, TempFolder$, TempFile$
Dim JG As New janGraphics.Compendium, ErrDesc$, iErrNumber As Long
Dim File$
  
  With frmCaptureForm
  .AutoRedraw = True
    With .picImage
    .Left = 0
    .Top = 0
    .Width = f.Width
    .Height = f.Height
    Select Case Mode
    Case 0
    .Picture = CaptureScreen()          'Capture full screen
    Case 1
    .Picture = CaptureForm(f)           'Capture full form
    Case 2
    .Picture = CaptureClient(f)         'Capture client area of form
    Case 3
    .Picture = CaptureActiveWindow()    'Capture active window
    End Select
    End With
  End With
  DoEvents
  
' Construct default filename if one is not provided
  If PNGfilename$ = "" Then
    i = 0
    Do
      i = i + 1
      Filename$ = "C:\MTP\PNG\" & f.Name & Format(i, "000") & ".png"
    Loop Until Len(Dir(Filename$)) = 0
  Else
    Filename$ = PNGfilename$
  End If
    
  On Error GoTo Err_
  TempFolder$ = GetTempFolderName
  TempFile$ = TempFolder$ & "TempPic.bmp"
  SavePicture frmCaptureForm!picImage.Picture, TempFile$
  
  JG.convertEx TempFile$, Filename$, 0, 0, False, 80
  
  i = InStrRev(Filename$, "\")
  If i = 0 Then
    MsgBox "PNG file of " & f.Name & " was not saved!", vbOKOnly
    Exit Sub
  Else
    DefaultFolder$ = Left$(Filename$, i)
    
Err_:
    If Err.Number <> 0 Then iErrNumber = Err.Number: ErrDesc$ = Err.Description
    Set JG = Nothing
    If Len(TempFile$) > 0 Then Kill TempFile$: TempFile$ = ""
    If Len(TempFolder$) > 0 Then RmDir TempFolder$: TempFolder$ = ""
    If iErrNumber <> 0 Then MsgBox ErrDesc$, vbCritical, "Error No " & iErrNumber

    ' Save image of form that was put in picture box
    If OK Then
      If PNGfilename$ = "" Then
        MsgBox "PNG file of " & f.Name & " was saved in: " & vbCrLf & Filename$, vbOKOnly
      Else
        MsgBox "PNG file was saved in: " & vbCrLf & Filename$, vbOKOnly
      End If
    End If
  End If

End Sub

Public Function GetTempFolderName() As String
    Dim s As String, i As Integer
    s = Space(260)
    i = GetTempFileNameA(GetTempPath, "~si", 0, s)
    s = Left$(s, InStr(1, s, Chr$(0)) - 1)
    Kill s
    MkDir s
    GetTempFolderName = s & "\"
End Function
Public Function GetTempPath() As String
    Dim s As String, i As Integer
    s = Space(100)
    i = GetTempPathA(100, s)
    GetTempPath = Left$(s, i)
End Function

      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      ' CaptureScreen
      '    - Captures the entire screen.
      '
      ' Returns
      '    - Returns a Picture object containing a bitmap of the screen.
      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      Public Function CaptureScreen() As Picture
            Dim hWndScreen As Long

         ' Get a handle to the desktop window.
         hWndScreen = GetDesktopWindow()

         ' Call CaptureWindow to capture the entire desktop give the handle
         ' and return the resulting Picture object.

         Set CaptureScreen = CaptureWindow(hWndScreen, False, 0, 0, _
            Screen.Width \ Screen.TwipsPerPixelX, _
            Screen.Height \ Screen.TwipsPerPixelY)
      End Function

      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      ' CaptureForm
      '    - Captures an entire form including title bar and border.
      '
      ' frmSrc
      '    - The Form object to capture.
      '
      ' Returns
      '    - Returns a Picture object containing a bitmap of the entire
      '      form.
      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      Public Function CaptureForm(frmSrc As Form) As Picture
         ' Call CaptureWindow to capture the entire form given its window
         ' handle and then return the resulting Picture object.
         Set CaptureForm = CaptureWindow(frmSrc.hwnd, False, 0, 0, _
            frmSrc.ScaleX(frmSrc.Width, vbTwips, vbPixels), _
            frmSrc.ScaleY(frmSrc.Height, vbTwips, vbPixels))
      End Function

      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      ' CaptureClient
      '    - Captures the client area of a form.
      '
      ' frmSrc
      '    - The Form object to capture.
      '
      ' Returns
      '    - Returns a Picture object containing a bitmap of the form's
      '      client area.
      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      Public Function CaptureClient(frmSrc As Form) As Picture
         ' Call CaptureWindow to capture the client area of the form given
         ' its window handle and return the resulting Picture object.
         Set CaptureClient = CaptureWindow(frmSrc.hwnd, True, 0, 0, _
            frmSrc.ScaleX(frmSrc.ScaleWidth, frmSrc.ScaleMode, vbPixels), _
            frmSrc.ScaleY(frmSrc.ScaleHeight, frmSrc.ScaleMode, vbPixels))
      End Function

      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      ' CaptureActiveWindow
      '    - Captures the currently active window on the screen.
      '
      ' Returns
      '    - Returns a Picture object containing a bitmap of the active
      '      window.
      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      Public Function CaptureActiveWindow() As Picture
         Dim hWndActive As Long
         Dim r As Long
         Dim RectActive As RECT

         ' Get a handle to the active/foreground window.
         hWndActive = GetForegroundWindow()

         ' Get the dimensions of the window.
         r = GetWindowRect(hWndActive, RectActive)

         ' Call CaptureWindow to capture the active window given its
         ' handle and return the Resulting Picture object.
      Set CaptureActiveWindow = CaptureWindow(hWndActive, False, 0, 0, _
            RectActive.Right - RectActive.Left, _
            RectActive.Bottom - RectActive.Top)
      End Function

      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      ' PrintPictureToFitPage
      '    - Prints a Picture object as big as possible.
      '
      ' Prn
      '    - Destination Printer object.
      '
      ' Pic
      '    - Source Picture object.
      '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
      '
      Public Sub PrintPictureToFitPage(Prn As Printer, pic As Picture)
         Const vbHiMetric As Integer = 8
         Dim PicRatio As Double
         Dim PrnWidth As Double
         Dim PrnHeight As Double
         Dim PrnRatio As Double
         Dim PrnPicWidth As Double
         Dim PrnPicHeight As Double

         ' Determine if picture should be printed in landscape or portrait
         ' and set the orientation.
         If pic.Height >= pic.Width Then
            Prn.Orientation = vbPRORPortrait   ' Taller than wide.
         Else
            Prn.Orientation = vbPRORLandscape  ' Wider than tall.
         End If

         ' Calculate device independent Width-to-Height ratio for picture.
         PicRatio = pic.Width / pic.Height

         ' Calculate the dimentions of the printable area in HiMetric.
         PrnWidth = Prn.ScaleX(Prn.ScaleWidth, Prn.ScaleMode, vbHiMetric)
         PrnHeight = Prn.ScaleY(Prn.ScaleHeight, Prn.ScaleMode, vbHiMetric)
         ' Calculate device independent Width to Height ratio for printer.
         PrnRatio = PrnWidth / PrnHeight

         ' Scale the output to the printable area.
         If PicRatio >= PrnRatio Then
            ' Scale picture to fit full width of printable area.
            PrnPicWidth = Prn.ScaleX(PrnWidth, vbHiMetric, Prn.ScaleMode)
            PrnPicHeight = Prn.ScaleY(PrnWidth / PicRatio, vbHiMetric, _
               Prn.ScaleMode)
         Else
            ' Scale picture to fit full height of printable area.
            PrnPicHeight = Prn.ScaleY(PrnHeight, vbHiMetric, Prn.ScaleMode)
            PrnPicWidth = Prn.ScaleX(PrnHeight * PicRatio, vbHiMetric, _
               Prn.ScaleMode)
         End If

         ' Print the picture using the PaintPicture method.
         Prn.PaintPicture pic, 0, 0, PrnPicWidth, PrnPicHeight
      End Sub
      '--------------------------------------------------------------------

