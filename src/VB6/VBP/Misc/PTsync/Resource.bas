Attribute VB_Name = "Resource"
Option Explicit
' Copyright © 2003-2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 2.22 6/2/2004

Private Const LANG_NEUTRAL = &H0
Private Const SUBLANG_NEUTRAL = &H0
Private Const SUBLANG_DEFAULT = &H1
Private Const SUBLANG_SYS_DEFAULT = &H2

Private Const E_INVALIDARG = &H80070057
Private Const E_OUTOFMEMORY = &H8007000E
Private Const S_OK = &H0

Private Const SM_CXSMICON = 49
Private Const SM_CYSMICON = 50
Private Const SM_CXICON = 11
Private Const SM_CYICON = 12

Private Const MAX_PATH = 260
Private Const SHGFI_SHELLICONSIZE = &H4
Private Const SHGFI_SMALLICON = &H1
Private Const SHGFI_SYSICONINDEX = &H4000
Private Const SHGFI_LARGEICON = &H0
Private Const SHGFI_ICON = &H100

Public Enum ICON_SIZE
    ICON_DEFAULT = 0
    ICON_SMALL = -1
    ICON_LARGE = -2
    ICON_HUGE = -4
    ICON_SHELL_DEFAULT = 8
    ICON_SHELL_SMALL = -9
    ICON_SHELL_LARGE = -10
    ICON_ACTUAL = -99
End Enum

' Predefined Resource Types
Private Enum RES_TYPE
    RT_CURSOR = 1
    RT_BITMAP = 2
    RT_ICON = 3
    RT_MENU = 4
    RT_DIALOG = 5
    RT_STRING = 6
    RT_FONTDIR = 7
    RT_FONT = 8
    RT_ACCELERATOR = 9
    RT_RCDATA = 10
    RT_MESSAGETABLE = 11

    RT_GROUP_CURSOR = RT_CURSOR + 11
    RT_GROUP_ICON = RT_ICON + 11
    RT_VERSION = 16
    RT_DLGINCLUDE = 17

    RT_PLUGPLAY = 19
    RT_VXD = 20
    RT_ANICURSOR = 21
    RT_ANIICON = 22

    RT_HTML = 23

    RT_MANIFEST = 24
    MINIMUM_RESERVED_MANIFEST_RESOURCE_ID = 1  ' inclusive
    CREATEPROCESS_MANIFEST_RESOURCE_ID = 1
    ISOLATIONAWARE_MANIFEST_RESOURCE_ID = 2
    ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID = 3
    MAXIMUM_RESERVED_MANIFEST_RESOURCE_ID = 16 ' inclusive
End Enum

Private Enum IMAGE_TYPE
    IMAGE_BITMAP = 0
    IMAGE_ICON = 1
    IMAGE_CURSOR = 2
    IMAGE_ENHMETAFILE = 3
End Enum

' OEM Resource Ordinal Numbers
Private Enum OEM_BITMAP_ID
    OBM_CLOSE = 32754
    OBM_UPARROW = 32753
    OBM_DNARROW = 32752
    OBM_RGARROW = 32751
    OBM_LFARROW = 32750
    OBM_REDUCE = 32749
    OBM_ZOOM = 32748
    OBM_RESTORE = 32747
    OBM_REDUCED = 32746
    OBM_ZOOMD = 32745
    OBM_RESTORED = 32744
    OBM_UPARROWD = 32743
    OBM_DNARROWD = 32742
    OBM_RGARROWD = 32741
    OBM_LFARROWD = 32740
    OBM_MNARROW = 32739
    OBM_COMBO = 32738
    OBM_UPARROWI = 32737
    OBM_DNARROWI = 32736
    OBM_RGARROWI = 32735
    OBM_LFARROWI = 32734

    OBM_OLD_CLOSE = 32767
    OBM_SIZE = 32766
    OBM_OLD_UPARROW = 32765
    OBM_OLD_DNARROW = 32764
    OBM_OLD_RGARROW = 32763
    OBM_OLD_LFARROW = 32762
    OBM_BTSIZE = 32761
    OBM_CHECK = 32760
    OBM_CHECKBOXES = 32759
    OBM_BTNCORNERS = 32758
    OBM_OLD_REDUCE = 32757
    OBM_OLD_ZOOM = 32756
    OBM_OLD_RESTORE = 32755
End Enum

Private Enum OEM_CURSOR_ID
    OCR_NORMAL = 32512
    OCR_IBEAM = 32513
    OCR_WAIT = 32514
    OCR_CROSS = 32515
    OCR_UP = 32516
    OCR_SIZE = 32640             ' OBSOLETE: use OCR_SIZEALL */
    OCR_ICON = 32641            ' OBSOLETE: use OCR_NORMAL */
    OCR_SIZENWSE = 32642
    OCR_SIZENESW = 32643
    OCR_SIZEWE = 32644
    OCR_SIZENS = 32645
    OCR_SIZEALL = 32646
    OCR_ICOCUR = 32647           ' OBSOLETE: use OIC_WINLOGO */
    OCR_NO = 32648

    OCR_HAND = 32649

    OCR_APPSTARTING = 32650
End Enum

Private Enum OEM_ICON_ID
    OIC_SAMPLE = 32512
    OIC_HAND = 32513
    OIC_QUES = 32514
    OIC_BANG = 32515
    OIC_NOTE = 32516

    OIC_WINLOGO = 32517
    OIC_WARNING = OIC_BANG
    OIC_ERROR = OIC_HAND
    OIC_INFORMATION = OIC_NOTE
End Enum

Private Const ORD_LANGDRIVER = 1       ' The ordinal number for the entry point of
                                ' language drivers.

' Standard Icon IDs
' = "#" & cstr(ID)
Private Enum ICON_ID
    IDI_APPLICATION = 32512
    IDI_HAND = 32513
    IDI_QUESTION = 32514
    IDI_EXCLAMATION = 32515
    IDI_ASTERISK = 32516
    IDI_WINLOGO = 32517
    IDI_WARNING = IDI_EXCLAMATION
    IDI_ERROR = IDI_HAND
    IDI_INFORMATION = IDI_ASTERISK
End Enum

Private Enum LOAD_RESOURCE_FLAG
    LR_DEFAULTCOLOR = &H0&
    LR_MONOCHROME = &H1&
    LR_COLOR = &H2&
    LR_COPYRETURNORG = &H4&
    LR_COPYDELETEORG = &H8&
    LR_LOADFROMFILE = &H10&
    LR_LOADTRANSPARENT = &H20&
    LR_DEFAULTSIZE = &H40&
    LR_VGACOLOR = &H80&
    LR_LOADMAP3DCOLORS = &H1000&
    LR_CREATEDIBSECTION = &H2000&
    LR_COPYFROMRESOURCE = &H4000&
    LR_SHARED = &H8000&
End Enum

Private Enum PIC_TYPE
    PICTYPE_UNINITIALIZED = (-1)
    PICTYPE_NONE = 0
    PICTYPE_BITMAP = 1
    PICTYPE_METAFILE = 2
    PICTYPE_ICON = 3
    PICTYPE_ENHMETAFILE = 4
End Enum

Private Type PICTDESC
    cbSizeofstruct As Long
    PicType As PIC_TYPE
    hImage As Long
    xExt As Integer
    yExt As Integer
End Type

Private Type PICTDESC_ICON
    cbSizeofstruct As Long
    PicType As Long
    hIcon As Long
End Type

Private Type PICTDESC_BMP
    cbSizeofstruct As Long
    PicType As Long
    hbitmap As Long
    hPal As Long
End Type

Private Type PICTDESC_WMF
    cbSizeofstruct As Long
    PicType As Long
    hMeta As Long
    xExt As Long
    yExt As Long
End Type

Private Type PICTDESC_EMF
    cbSizeofstruct As Long
    PicType As Long
    hemf As Long
End Type

Private Type GUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(7) As Byte
End Type

Private Declare Function GetSystemMetrics Lib "user32.dll" ( _
    ByVal nIndex As Long) As Long

Private Declare Function IIDFromString Lib "ole32.dll" ( _
    ByVal lpsz As String, _
    ByRef lpiid As GUID) As Long

Private Declare Sub StringFromIID Lib "ole32.dll" ( _
    ByRef rclsid As GUID, _
    ByVal lplpsz As String)

Private Declare Function OleCreatePictureIndirect Lib "oleaut32.dll" ( _
    ByRef lpPictDesc As PICTDESC, _
    ByRef riid As GUID, _
    ByVal fOwn As Long, _
    ByRef lplpvObj As IPicture) As Long

Private Type BITMAP
    bmType As Long
    bmWidth As Long
    bmHeight As Long
    bmWidthBytes As Long
    bmPlanes As Integer
    bmBitsPixel As Integer
    bmBits As Long
End Type

Private Type SHFILEINFO
    hIcon As Long ' : icon
    iIcon As Long ' : icondex
    dwAttributes As Long ' : SFGAO_ flags
    szDisplayName As String * MAX_PATH ' : display name (or path)
    szTypeName As String * 80 ' : type name
End Type

Private Declare Function SHGetFileInfoA Lib "shell32.dll" ( _
    ByVal pszPath As String, _
    ByVal dwFileAttributes As Long, _
    ByRef psfi As SHFILEINFO, _
    ByVal cbFileInfo As Long, _
    ByVal uFlags As Long) As Long

Private Declare Function ImageList_GetIconSize Lib "comctl32.dll" ( _
     ByVal himl As Long, _
     ByRef cx As Long, _
     ByRef cy As Long) As Long

Private Declare Function GetObjectBitmap Lib "GDI32" Alias "GetObjectA" ( _
    ByVal hbitmap As Long, ByVal cbBuffer As Long, _
    lpBitmap As BITMAP) As Long

Private Declare Function LoadStringA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal wID As Long, _
    ByVal lpBuffer As String, _
    ByVal nBufferMax As Long) As Long

Private Declare Function LoadCursorA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal lpCursorName As String) As Long

Private Declare Function LoadIconA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal lpIconName As String) As Long

Private Declare Function LoadBitmapA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal lpBitmapName As String) As Long

Private Declare Function LoadImageA Lib "user32" ( _
    ByVal hInst As Long, _
    ByVal lpszName As String, _
    ByVal uType As IMAGE_TYPE, _
    ByVal cxDesired As Long, _
    ByVal cyDesired As Long, _
    ByVal fuLoad As LOAD_RESOURCE_FLAG) As Long

Private Declare Function LoadLibraryA Lib "kernel32" ( _
    ByVal lpLibFileName As String) As Long

Private Declare Function FindResourceA Lib "kernel32" ( _
    ByVal hInstance As Long, _
    ByVal lpName As String, _
    ByVal lpType As String) As Long

Private Declare Function LockResource Lib "kernel32" ( _
    ByVal hResData As Long) As Long

Private Declare Function LoadResource Lib "kernel32" ( _
    ByVal hInstance As Long, _
    ByVal hResInfo As Long) As Long

Private Declare Function BeginUpdateResourceA Lib "kernel32" ( _
    ByVal pFileName As String, _
    ByVal bDeleteExistingResources As Long) As Long

Private Declare Function SizeofResource Lib "kernel32" ( _
    ByVal hInstance As Long, _
    ByVal hResInfo As Long) As Long

Private Declare Function UpdateResourceA Lib "kernel32" ( _
    ByVal hUpdate As Long, _
    ByVal lpType As String, _
    ByVal lpName As String, _
    ByVal wLanguage As Long, _
    ByRef lpData As Any, _
    ByVal cbData As Long) As Long

Private Declare Function EndUpdateResourceA Lib "kernel32" ( _
    ByVal hUpdate As Long, _
    ByVal fDiscard As Long) As Long

Private Declare Function FreeLibrary Lib "kernel32" ( _
    ByVal hLibModule As Long) As Long

Private InitDone As Boolean
Private Data() As Byte
Private DataLoc As Long
Private Sub ResInit()
    DataLoc = -1
    InitDone = True
End Sub
Public Function ResDataText(ResName As String, Fid As String, Optional ResType As String = "DATA") As Boolean
    Dim fn As Long
    Dim s As String

    ResDataText = False
    If Not ResDataOpen(ResName, ResType) Then Exit Function

    fn = FreeFile()
    On Error Resume Next
    Open Fid For Output Access Write As fn
    If Err.Number <> 0 Then
        ResDataClose
        Exit Function
    End If

    While Not ResDataEOF()
        s = ResDataReadLine()
        Print #fn, s
    Wend

    Close #fn
    ResDataClose
    ResDataText = True
End Function
Public Function ResDataOpen(Name As String, Optional ResType As String = "DATA") As Boolean
    If Not InitDone Then ResInit

    On Error Resume Next
    Data = LoadResData(Name, ResType)
    If Err.Number = 0 Then
        ResDataOpen = True
        DataLoc = 0
    Else
        ResDataOpen = False
        DataLoc = -1
    End If
End Function
Public Function ResDataReadLine() As String
    If Not InitDone Then ResInit

    Dim s As String
    Dim i As Long
    Dim eos As Boolean

    s = ""
    If DataLoc < 0 Then
        ResDataReadLine = s
        Exit Function
    End If

    If DataLoc > UBound(Data) Then
        ResDataReadLine = s
        Exit Function
    End If

    eos = False
    For i = DataLoc To UBound(Data)
        Select Case Data(i)
            Case Asc(vbCr), Asc(vbLf):
                eos = True
            Case Else
                If eos Then
                    DataLoc = i
                    Exit For
                End If
                s = s & Chr(Data(i))
        End Select
    Next i
    If i > UBound(Data) Then DataLoc = -1
    ResDataReadLine = s
End Function
Public Function ResDataEOF() As Boolean
    If Not InitDone Then ResInit

    If DataLoc < 0 Then
        ResDataEOF = True
    Else
        ResDataEOF = False
    End If
End Function
Public Function ResDataClose() As Boolean
    If Not InitDone Then ResInit

    Erase Data
    DataLoc = -1
    ResDataClose = True
End Function
Public Function GetIconSize(IconSize As ICON_SIZE) As Long
    Dim shfi As SHFILEINFO
    Dim hImgLst As Long
    Dim cx As Long
    Dim cy As Long

    GetIconSize = -1
    Select Case IconSize
        Case ICON_DEFAULT: ' Load a System icon image
            hImgLst = SHGetFileInfoA(".", 0, shfi, Len(shfi), _
                SHGFI_SYSICONINDEX Or SHGFI_ICON)
        Case ICON_SMALL: ' Load a System Small icon image
            hImgLst = SHGetFileInfoA(".", 0, shfi, Len(shfi), _
                SHGFI_SYSICONINDEX Or SHGFI_ICON Or SHGFI_SMALLICON)
        Case ICON_LARGE: ' Load a System Large icon image
            hImgLst = SHGetFileInfoA(".", 0, shfi, Len(shfi), _
                SHGFI_SYSICONINDEX Or SHGFI_ICON Or SHGFI_LARGEICON)
        Case ICON_HUGE:
                GetIconSize = 48
                Exit Function
        Case ICON_SHELL_DEFAULT: ' Load a Shell icon image
            hImgLst = SHGetFileInfoA(".", 0, shfi, Len(shfi), _
                SHGFI_SYSICONINDEX Or SHGFI_ICON Or SHGFI_SHELLICONSIZE)
        Case ICON_SHELL_LARGE:  ' Load a Shell Large icon image
            hImgLst = SHGetFileInfoA(".", 0, shfi, Len(shfi), _
                SHGFI_SYSICONINDEX Or SHGFI_ICON Or SHGFI_SHELLICONSIZE Or SHGFI_LARGEICON)
        Case ICON_SHELL_SMALL:  ' Load a Shell Small icon image
            hImgLst = SHGetFileInfoA(".", 0, shfi, Len(shfi), _
                SHGFI_SYSICONINDEX Or SHGFI_ICON Or SHGFI_SHELLICONSIZE Or SHGFI_SMALLICON)
    End Select

    If hImgLst <> 0 Then
        If ImageList_GetIconSize(hImgLst, cx, cy) Then
            GetIconSize = cx
        End If
    End If
End Function
Public Function ResIconLoad(Optional ID As String, _
    Optional IconSize As ICON_SIZE = ICON_DEFAULT) As Picture

    Dim hIcon As Long
    Dim xy As Long
    Dim LoadFlag As Long
    Dim iPic As IPicture
    Dim pd As PICTDESC
    Dim iidIPicture As GUID
    Dim AppPath As String
    Dim hExe As Long

    Dim hImlst As Long
    Dim fi As SHFILEINFO
    Dim cx As Long
    Dim cy As Long
    Dim Reg As Registry
    Dim result As Long

    Set ResIconLoad = Nothing
    If Len(ID) <= 0 Then Exit Function

    LoadFlag = LR_DEFAULTCOLOR Or LR_COPYFROMRESOURCE
    Select Case IconSize
        Case ICON_DEFAULT, ICON_SMALL, ICON_LARGE, ICON_HUGE:
            xy = GetIconSize(IconSize)

        Case ICON_SHELL_DEFAULT, ICON_SHELL_SMALL, ICON_SHELL_LARGE:
            xy = GetIconSize(IconSize)

        Case ICON_ACTUAL:
            xy = 0
            LoadFlag = 0

        Case Is > 0  ' custom size
            xy = IconSize

        Case Else:
            xy = 0
            LoadFlag = LoadFlag Or LR_DEFAULTSIZE

    End Select

    If ApiUnderIde Then
        Set Reg = New Registry
        AppPath = Reg.GetHome
        hExe = LoadLibraryA(AppPath)
        If hExe = 0 Then
            AppPath = Replace(AppPath & ".exe", App.EXEName & ".exe", "PT" & App.EXEName, , , vbTextCompare)
            hExe = LoadLibraryA(AppPath)
        End If
        If hExe = 0 Then Exit Function
        hIcon = LoadImageA(hExe, ID, IMAGE_ICON, xy, xy, LoadFlag Or LR_DEFAULTCOLOR Or LR_COPYFROMRESOURCE)
    Else
        hIcon = LoadImageA(App.hInstance, ID, IMAGE_ICON, xy, xy, LoadFlag)
    End If

    If hIcon <> 0 Then
        pd.cbSizeofstruct = Len(pd)
        pd.PicType = PICTYPE_ICON
        pd.hImage = hIcon

        result = IIDFromString(StrConv("{7BF80980-BF32-101A-8BBB-00AA00300CAB}", vbUnicode), iidIPicture)
        result = OleCreatePictureIndirect(pd, iidIPicture, True, iPic)
        Set ResIconLoad = iPic
    Else
        Set ResIconLoad = Nothing
    End If

    If hExe <> 0 Then FreeLibrary hExe
End Function
Function CursorToPicture(ByVal hIcon As Long) As IPicture
    Dim iPic As IPicture
    Dim pd As PICTDESC
    Dim iidIPicture As GUID
    
    pd.cbSizeofstruct = Len(pd)
    pd.PicType = vbPicTypeIcon
    pd.hImage = hIcon

    IIDFromString "{7BF80980-BF32-101A-8BBB-00AA00300CAB}", iidIPicture
    OleCreatePictureIndirect pd, iidIPicture, True, iPic
    Set CursorToPicture = iPic
End Function
Function BitmapToPicture(ByVal hBmp As Long, _
                         Optional ByVal hPal As Long = 0) _
                         As IPicture

    Dim iPic As IPicture
    Dim pd As PICTDESC
    Dim iidIPicture As GUID

    pd.cbSizeofstruct = Len(pd)
    pd.PicType = vbPicTypeBitmap
    pd.hImage = hBmp
'    pd.hPalOrXYExt = hpal
    
    IIDFromString "{7BF80980-BF32-101A-8BBB-00AA00300CAB}", iidIPicture
    
    OleCreatePictureIndirect pd, iidIPicture, True, iPic
    Set BitmapToPicture = iPic
End Function
Function MetafileToPicture(ByVal hMeta As Long, _
                           ByVal xExt As Integer, _
                           ByVal yExt As Integer, _
                           Optional fOld As Boolean) As IPicture

    If hMeta = 0 Then Exit Function
    Dim iPic As IPicture
    Dim pd As PICTDESC
    Dim iidIPicture As GUID

    ' Fill picture description (assume enhanced)
    pd.cbSizeofstruct = Len(pd)
    If fOld Then
        pd.PicType = vbPicTypeMetafile
    Else
        pd.PicType = vbPicTypeEMetafile
    End If
    pd.hImage = hMeta
    pd.xExt = xExt
    pd.yExt = yExt

    IIDFromString "{7BF80980-BF32-101A-8BBB-00AA00300CAB}", iidIPicture
    OleCreatePictureIndirect pd, iidIPicture, True, iPic
    Set MetafileToPicture = iPic
End Function
Public Sub ResCopy(SrcExe As String, DestExe As String)
    Dim hResLoad As Long
    Dim hExe As Long
    Dim hRes As Long
    Dim hUpdateRes As Long
    Dim lpResLock As Long
    Dim result As Boolean

    ' Load the .EXE file that contains the dialog box you want to copy.
    hExe = LoadLibraryA("hand.exe")
    If (hExe = 0) Then
'    ErrorHandler("Could not load exe.");
        Exit Sub
    End If

    ' Locate the dialog box resource in the .EXE file.
    hRes = FindResourceA(hExe, "AboutBox", RT_DIALOG)
    If (hRes = 0) Then
'    ErrorHandler("Could not locate dialog box.");
        FreeLibrary hExe
        Exit Sub
    End If

    ' Load the dialog box into global memory.
    hResLoad = LoadResource(hExe, hRes)
    If (hResLoad = 0) Then
'    ErrorHandler("Could not load dialog box.");
        FreeLibrary hExe
        Exit Sub
    End If

    ' Lock the dialog box into global memory.
    lpResLock = LockResource(hResLoad)
    If (lpResLock = 0) Then
'    ErrorHandler("Could not lock dialog box.");
        FreeLibrary hExe
        Exit Sub
    End If

    ' Open the file to which you want to add the dialog box resource.
    hUpdateRes = BeginUpdateResourceA("foot.exe", False)
    If (hUpdateRes = 0) Then
'    ErrorHandler("Could not open file for writing.");
        FreeLibrary hExe
        Exit Sub
    End If

    ' Add the dialog box resource to the update list.
    result = UpdateResourceA(hUpdateRes, _
        RT_DIALOG, _
        "AboutBox", _
        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), _
        lpResLock, _
        SizeofResource(hExe, hRes))

    If (result = 0) Then
'    ErrorHandler("Could not add resource.");
        FreeLibrary hExe
        Exit Sub
    End If

    ' Write changes to FOOT.EXE and then close it.
    If EndUpdateResourceA(hUpdateRes, False) <> 0 Then
'    ErrorHandler("Could not write changes to file.");
    End If

    ' Clean up.
    If FreeLibrary(hExe) <> 0 Then
'    ErrorHandler("Could not free executable.");
        Exit Sub
    End If
End Sub
Private Function MAKEINTRESOURCE(ResID As Integer) As String
    MAKEINTRESOURCE = "##" & CStr(ResID)
End Function
'  A language ID is a 16 bit value which is the combination of a
'  primary language ID and a secondary language ID.  The bits are
'  allocated as follows:
'
'       +-----------------------+-------------------------+
'       |     Sublanguage ID    |   Primary Language ID   |
'       +-----------------------+-------------------------+
'        15                   10 9                       0   bit
'
'
'  Language ID creation/extraction macros:
'
'    MAKELANGID    - construct language id from a primary language id and
'                    a sublanguage id.
'    PRIMARYLANGID - extract primary language id from a language id.
'    SUBLANGID     - extract sublanguage id from a language id.
Private Function MAKELANGID(PriLangID As Integer, SubLangID As Integer) As Integer
    Dim p As Variant
    Dim s As Variant

    p = ApiConvUInt16Dec(PriLangID)
    s = ApiConvUInt16Dec(SubLangID)

    MAKELANGID = ApiConvDecUInt16((s * 1024) + p)   ' ((s << 10) | p)
End Function
Private Function PRIMARYLANGID(LangID As Integer) As Integer
    PRIMARYLANGID = LangID & &H3FF   '   (lgid) & 0x3ff
End Function
Private Function SubLangID(LangID As Integer) As Integer
    SubLangID = LangID \ 1024   ' lgid >> 10
End Function

'The following example copies a dialog box resource from one executable file, Hand.exe, to another, Foot.exe, by following these steps:
'
'Use the LoadLibrary function to load the executable file Hand.exe.
'Use the FindResource and LoadResource functions to locate and load the dialog box resource.
'Use the LockResource function to retrieve a pointer to the dialog box resource data.
'Use the BeginUpdateResource function to open an update handle to Foot.exe.
'Use the UpdateResource function to copy the dialog box resource from Hand.exe to Foot.exe.
'Use the EndUpdateResource function to complete the update.
'The following code implements these steps.
'
'HRSRC hResLoad;     // handle to loaded resource
'HANDLE hExe;        // handle to existing .EXE file
'HRSRC hRes;         // handle/ptr. to res. info. in hExe
'HANDLE hUpdateRes;  // update resource handle
'char *lpResLock;    // pointer to resource data
'BOOL result;
'// Load the .EXE file that contains the dialog box you want to copy.
'hExe = LoadLibrary("hand.exe");
'if (hExe == NULL)
'{
'    ErrorHandler("Could not load exe.");
'}
'
'// Locate the dialog box resource in the .EXE file.
'hRes = FindResource(hExe, "AboutBox", RT_DIALOG);
'if (hRes == NULL)
'{
'    ErrorHandler("Could not locate dialog box.");
'}
'
'// Load the dialog box into global memory.
'hResLoad = LoadResource(hExe, hRes);
'if (hResLoad == NULL)
'{
'    ErrorHandler("Could not load dialog box.");
'}
'
'// Lock the dialog box into global memory.
'lpResLock = LockResource(hResLoad);
'if (lpResLock == NULL)
'{
'    ErrorHandler("Could not lock dialog box.");
'}
'
'// Open the file to which you want to add the dialog box resource.
'hUpdateRes = BeginUpdateResource("foot.exe", FALSE);
'if (hUpdateRes == NULL)
'{
'    ErrorHandler("Could not open file for writing.");
'}
'
'// Add the dialog box resource to the update list.
'result = UpdateResource(hUpdateRes,       // update resource handle
'     RT_DIALOG,                   // change dialog box resource
'     "AboutBox",                  // dialog box name
'     MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),  // neutral language
'     lpResLock,                   // ptr to resource info
'     SizeofResource(hExe, hRes)); // size of resource info.
'if (result == FALSE)
'{
'    ErrorHandler("Could not add resource.");
'}
'
'// Write changes to FOOT.EXE and then close it.
'if (!EndUpdateResource(hUpdateRes, FALSE))
'{
'    ErrorHandler("Could not write changes to file.");
'}
'
'// Clean up.
'if (!FreeLibrary(hExe))
'{
'    ErrorHandler("Could not free executable.");
'}

