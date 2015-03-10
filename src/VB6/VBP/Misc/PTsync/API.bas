Attribute VB_Name = "API"
Option Explicit
Option Compare Text
' Copyright © 2003-2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 2.45 8/9/2004

'============================================
' Public Variables, Consts, Enums and Types
'============================================
Public ApiLastError As Long
Public ApiAbort As Boolean

Public Const vbQuote = """"
Public Const LINKEXT = ".lnk"
Public Const EXE_EXT = ".exe"
Public Const UNKNOWN_LIT = "unknown"

Public Const SECS_PER_DAY As Long = 86400
Public Const SECS_PER_HOUR As Long = 3600

Public Const API_BUFSIZ = 2 ^ 16

Public Const ERROR_ALREADY_EXISTS = 183&
Public Const ERROR_ALLOTTED_SPACE_EXCEEDED = 1344&
Public Const ERROR_NO_MORE_FILES = 18&

Public Const INVALID_FILE_SIZE = &HFFFFFFFF
Public Const INVALID_HANDLE_VALUE = -1

' these are the  vbKeyFx definitions VB omitted
Public Const vbKeyF17 = vbKeyF16 + 1
Public Const vbKeyF18 = vbKeyF16 + 2
Public Const vbKeyF19 = vbKeyF16 + 3
Public Const vbKeyF20 = vbKeyF16 + 4
Public Const vbKeyF21 = vbKeyF16 + 5
Public Const vbKeyF22 = vbKeyF16 + 6
Public Const vbKeyF23 = vbKeyF16 + 7
Public Const vbKeyF24 = vbKeyF16 + 8

Public Enum FORM_TAG
    FORM_READY = 1
    FORM_RESIZING
    FORM_NORESIZE
End Enum

Public Enum SERIAL_FORMAT
    SERIAL_FORMAT_TSV
    SERIAL_FORMAT_CSV
    SERIAL_FORMAT_SSV
    SERIAL_FORMAT_XML
End Enum

Public Enum SERIAL_ERROR
    SERIAL_READONLY = vbObjectError + 1
    SERIAL_EMPTY
    SERIAL_INVALID
    SERIAL_BAD_VERSION
    SERIAL_TYPE_INVALID
End Enum

Public Enum VIRTUAL_KEY
    VK_F1 = &H70
    VK_F2 = &H71
    VK_F3 = &H72
    VK_F4 = &H73
    VK_F5 = &H74
    VK_F6 = &H75
    VK_F7 = &H76
    VK_F8 = &H77
    VK_F9 = &H78
    VK_F10 = &H79
    VK_F16 = &H7F
    VK_F17 = &H80
    VK_F18 = &H81
    VK_F19 = &H82
    VK_F20 = &H83
    VK_F21 = &H84
    VK_F22 = &H85
    VK_F23 = &H86
    VK_F24 = &H87
End Enum

' Commands to pass to WinHelp()
Public Enum HELP_COMMAND
    HELP_CONTEXT = &H1          ' Display topic in ulTopic */
    HELP_QUIT = &H2             ' Terminate help */
    HELP_INDEX = &H3            ' Display index */
    HELP_CONTENTS = &H3
    HELP_HELPONHELP = &H4       ' Display help on using help */
    HELP_SETINDEX = &H5         ' Set current Index for multi index help */
    HELP_SETCONTENTS = &H5
    HELP_CONTEXTPOPUP = &H8
    HELP_FORCEFILE = &H9
    HELP_KEY = &H101            ' Display topic for keyword in offabData */
    HELP_COMMAND = &H102
    HELP_PARTIALKEY = &H105
    HELP_MULTIKEY = &H201
    HELP_SETWINPOS = &H203

    HELP_CONTEXTMENU = &HA
    HELP_FINDER = &HB
    HELP_WM_HELP = &HC
    HELP_SETPOPUP_POS = &HD
End Enum

' SHGetSpecialFolderLocation
'  Caller should use SHGetMalloc to obtain an allocator that can free the pidl
Public Enum CSIDL_CODE
    CSIDL_DESKTOP = &H0&                 ' <desktop>
    CSIDL_INTERNET = &H1&                ' Internet Explorer (icon on desktop)
    CSIDL_PROGRAMS = &H2&                ' Start Menu\Programs
    CSIDL_CONTROLS = &H3&                ' My Computer\Control Panel
    CSIDL_PRINTERS = &H4&                ' My Computer\Printers
    CSIDL_PERSONAL = &H5&                ' My Documents
    CSIDL_FAVORITES = &H6&               ' <user name>\Favorites
    CSIDL_STARTUP = &H7&                 ' Start Menu\Programs\Startup
    CSIDL_RECENT = &H8&                  ' <user name>\Recent
    CSIDL_SENDTO = &H9&                  ' <user name>\SendTo
    CSIDL_BITBUCKET = &HA&               ' <desktop>\Recycle Bin
    CSIDL_STARTMENU = &HB&               ' <user name>\Start Menu
    CSIDL_MYDOCUMENTS = &HC&             ' logical "My Documents" desktop icon
    CSIDL_MYMUSIC = &HD&                 ' "My Music" folder
    CSIDL_MYVIDEO = &HE&                 ' "My Videos" folder
    CSIDL_DESKTOPDIRECTORY = &H10&       ' <user name>\Desktop
    CSIDL_DRIVES = &H11&                 ' My Computer
    CSIDL_NETWORK = &H12&                ' Network Neighborhood (My Network Places)
    CSIDL_NETHOOD = &H13&                ' <user name>\nethood
    CSIDL_FONTS = &H14&                  ' windows\fonts
    CSIDL_TEMPLATES = &H15&
    CSIDL_COMMON_STARTMENU = &H16&       ' All Users\Start Menu
    CSIDL_COMMON_PROGRAMS = &H17&        ' All Users\Start Menu\Programs
    CSIDL_COMMON_STARTUP = &H18&         ' All Users\Startup
    CSIDL_COMMON_DESKTOPDIRECTORY = &H19& ' All Users\Desktop
    CSIDL_APPDATA = &H1A&                ' <user name>\Application Data
    CSIDL_PRINTHOOD = &H1B&              ' <user name>\PrintHood
    CSIDL_LOCAL_APPDATA = &H1C&          ' <user name>\Local Settings\Applicaiton Data (non roaming)
    CSIDL_ALTSTARTUP = &H1D&             ' non localized startup
    CSIDL_COMMON_ALTSTARTUP = &H1E&      ' non localized common startup
    CSIDL_COMMON_FAVORITES = &H1F&
    CSIDL_INTERNET_CACHE = &H20&
    CSIDL_COOKIES = &H21&
    CSIDL_HISTORY = &H22&
    CSIDL_COMMON_APPDATA = &H23&         ' All Users\Application Data
    CSIDL_WINDOWS = &H24&                ' GetWindowsDirectory()
    CSIDL_SYSTEM = &H25&                 ' GetSystemDirectory()
    CSIDL_PROGRAM_FILES = &H26&          ' C:\Program Files
    CSIDL_MYPICTURES = &H27&             ' C:\Program Files\My Pictures
    CSIDL_PROFILE = &H28&                ' USERPROFILE
    CSIDL_SYSTEMX86 = &H29&              ' x86 system directory on RISC
    CSIDL_PROGRAM_FILESX86 = &H2A&       ' x86 C:\Program Files on RISC
    CSIDL_PROGRAM_FILES_COMMON = &H2B&   ' C:\Program Files\Common
    CSIDL_PROGRAM_FILES_COMMONX86 = &H2C& ' x86 Program Files\Common on RISC
    CSIDL_COMMON_TEMPLATES = &H2D&       ' All Users\Templates
    CSIDL_COMMON_DOCUMENTS = &H2E&       ' All Users\Documents
    CSIDL_COMMON_ADMINTOOLS = &H2F&      ' All Users\Start Menu\Programs\Administrative Tools
    CSIDL_ADMINTOOLS = &H30&             ' <user name>\Start Menu\Programs\Administrative Tools
    CSIDL_CONNECTIONS = &H31&            ' Network and Dial-up Connections
    CSIDL_COMMON_MUSIC = &H35&           ' All Users\My Music
    CSIDL_COMMON_PICTURES = &H36&        ' All Users\My Pictures
    CSIDL_COMMON_VIDEO = &H37&           ' All Users\My Video
    CSIDL_RESOURCES = &H38&              ' Resource Direcotry
    CSIDL_RESOURCES_LOCALIZED = &H39&    ' Localized Resource Direcotry
    CSIDL_COMMON_OEM_LINKS = &H3A&       ' Links to All Users OEM specific apps
    CSIDL_CDBURN_AREA = &H3B&            ' USERPROFILE\Local Settings\Application Data\Microsoft\CD Burning
    ' unused = &H3C&
    CSIDL_COMPUTERSNEARME = &H3D&        ' Computers Near Me (computered from Workgroup membership)

    CSIDL_FLAG_CREATE = &H8000&          ' combine with CSIDL_ value to force folder creation in SHGetFolderPath()
    CSIDL_FLAG_DONT_VERIFY = &H4000&     ' combine with CSIDL_ value to return an unverified folder path
    CSIDL_FLAG_NO_ALIAS = &H1000&        ' combine with CSIDL_ value to insure non-alias versions of the pidl
    CSIDL_FLAG_PER_USER_INIT = &H800&    ' combine with CSIDL_ value to indicate per-user init (eg. upgrade)
    CSIDL_FLAG_MASK = &HFF00&            ' mask for all possible flag values
End Enum

Public Enum SHOW_COMMAND
    SW_HIDE = 0
    SW_NORMAL = 1
    SW_SHOWMINIMIZED = 2
    SW_MAXIMIZE = 3
    SW_SHOWMAXIMIZED = 3
    SW_SHOWNOACTIVATE = 4
    SW_SHOW = 5
    SW_MINIMIZE = 6
    SW_SHOWMINNOACTIVE = 7
    SW_RESTORE = 9
    SW_SHOWNA = 8
    SW_SHOWDEFAULT = 10
End Enum

Public Enum PT_BEEP
    MB_ICONQUESTION = &H20&
    MB_OK = &H0&
    MB_ICONHAND = &H10&
    MB_ICONEXCLAMATION = &H30&
    MB_ICONASTERISK = &H40&
End Enum

Public Enum PT_TEXT_ENCODING
    PT_TEXT_ASCII = 1
    PT_TEXT_UNICODE
    PT_TEXT_DEFAULT ' Unicode on NT, else ASCII
End Enum

Public Enum PT_QTY
    PT_QTY_DISK_SIZE = 1
    PT_QTY_FREE_SPACE
    PT_QTY_FREE_QUOTA
    PT_QTY_ALLOC_UNIT
End Enum

Public Enum PT_PROTOCOL
    PT_PROTO_LOCAL = 1
    PT_PROTO_UNC
    PT_PROTO_FTP
    PT_PROTO_UNKNOWN = -1
End Enum

Public Enum PT_DRIVE_TYPE
    DRIVE_UNKNOWN = 0
    DRIVE_NO_ROOT_DIR = 1
    DRIVE_REMOVABLE = 2
    DRIVE_FIXED = 3
    DRIVE_REMOTE = 4
    DRIVE_CDROM = 5
    DRIVE_RAMDISK = 6
End Enum

Public Enum PT_ACCESS
    PT_ACCESS_NONE = 0
    PT_ACCESS_READ = &H1
    PT_ACCESS_WRITE = &H2
    PT_ACCESS_MAX = PT_ACCESS_READ And PT_ACCESS_WRITE
End Enum

Public Enum PT_ATTR
    PT_ATTR_READONLY = &H1
    PT_ATTR_HIDDEN = &H2
    PT_ATTR_SYSTEM = &H4
    PT_ATTR_DIRECTORY = &H10
    PT_ATTR_ARCHIVE = &H20
    PT_ATTR_DEVICE = &H40
    PT_ATTR_NORMAL = &H80
    PT_ATTR_TEMPORARY = &H100
    PT_ATTR_SPARSE_FILE = &H200
    PT_ATTR_REPARSE_POINT = &H400
    PT_ATTR_COMPRESSED = &H800
    PT_ATTR_OFFLINE = &H1000
    PT_ATTR_NOT_CONTENT_INDEXED = &H2000
    PT_ATTR_ENCRYPTED = &H4000

    PT_ATTR_ROOT_FOLDER = PT_ATTR_DIRECTORY Or PT_ATTR_ARCHIVE Or PT_ATTR_NORMAL
    PT_ATTR_DEFAULT_FOLDER = PT_ATTR_DIRECTORY Or PT_ATTR_NORMAL
    PT_ATTR_FILE_MASK = PT_ATTR_ARCHIVE Or PT_ATTR_HIDDEN Or PT_ATTR_READONLY Or PT_ATTR_SYSTEM
    PT_ATTR_PLAIN_FOLDER = PT_ATTR_NORMAL Or PT_ATTR_DIRECTORY
    PT_ATTR_INVALID = &HFFFFFFFF
End Enum

' Return codes from the CopyFileEx callback routine
Public Enum PT_FILE_COPY_PROGRESS
    PROGRESS_CONTINUE = 0
    PROGRESS_CANCEL = 1
    PROGRESS_STOP = 2
    PROGRESS_QUIET = 3
End Enum

Public Enum DISP_BYTES_FORMAT
    DISP_BYTES_LONG
    DISP_BYTES_SHORT
    DISP_BYTES_ALL
End Enum

Public Enum PT_VER_PLATFORM
    VER_PLATFORM_Platform32s = 0
    VER_PLATFORM_WIN32_WINDOWS = 1
    VER_PLATFORM_WIN32_NT = 2
End Enum

Public Enum SHORT_LEVEL
    SHORT_NONE = 0
    SHORT_MIN
    SHORT_NORMAL
    SHORT_MAX
End Enum

Public Type PT_TEXT_SIZE
    WidthPixels As Long
    HeightPixels As Long
    WidthTwips As Long
    HeightTwips As Long
End Type

Public Type PT_SIZE
    WidthPixels As Long
    HeightPixels As Long
End Type

Public Type PT_TEXT_ARRAY
    ParaCnt As Long
    ParaTable() As String
    LineCnt As Long
    LineTable() As String
    WrappedText As String
    Overall As PT_TEXT_SIZE
    LastLine As PT_TEXT_SIZE
    adj As PT_SIZE
    Bug As String
End Type

Public Type PT_OS_INFO
    Desc As String
    WinVer As Long           ' value of SDK header's WINVER (in decimal, not hex)
    WIN32_WINNT As Long      ' value of SDK header's WIN32_WINNT
    WIN32_WINDOWS As Long    ' value of SDK header's WIN32_WINDOWS
    WIN32_IE As Long         ' value of SDK header's WIN32_IE

    Major As Long
    Minor As Long
    Build As Long
    CsdVersion As String

    Platform As PT_VER_PLATFORM
    Platform32s As Boolean
    Platform9x As Boolean
    PlatformNT As Boolean

    ' specific OS versions
    Win31 As Boolean
    Win95 As Boolean    ' any flavor of Win95
    Win95Osr2 As Boolean
    Win98 As Boolean    ' any flavor of Win98
    Win98SE As Boolean
    WinMe As Boolean
    Win9x As Boolean  ' any flavor of Win95 or Win98
    WinNT351 As Boolean
    WinNT4x As Boolean  ' any flavor of WinNT v4.0
    WinNT40 As Boolean
    WinNT40Sp6 As Boolean
    WinNT As Boolean ' any flavor of WinNT (excluding W2K and above)
    Win2K As Boolean
    WinXP As Boolean
    Win2K3 As Boolean

    Unicode As Boolean ' supports Unicode
End Type

Public Type PT_COPY_RESULT
    Fail As Boolean
    Warn As Boolean
    msg As String
    BytesCopied As Variant
End Type

Public Type PT_DRIVE_INFO
    Error As Boolean
    ErrorNum As Long
    ErrorDesc As String
    Label As String
    SerialNum As Long
    SerialString As String
    Flags As Long
    MaxPath As Long
    FileSystem As String

    RootFolder As String
    Network As Boolean
    ShareName As String
    ShareType As Long
    ShareServer As String
    LocalPath As String
    LocalDrive As String
    CurUses As Long
    MaxUses As Long
    Ready As Boolean
    DriveStatus As String
    DriveType As PT_DRIVE_TYPE
    Removable As Boolean

    DriveSize As Variant
    ClusterSize As Long
    FreeSpace As Variant
    DriveCompressionSupported As Boolean
    FileCompressionSupported As Boolean
    EncryptionSupported As Boolean
    UnicodeSupported As Boolean
    LfnSupported As Boolean
    MaxComponentLength As Long
    QuotaSupported As Boolean
    QuotaFree As Variant
    UserName As String
    QuotaDomain As String
    QuotaState As DISKQUOTA_STATE
    QuotaUsed As Variant
    QuotaLimit As Variant
    QuotaWarning As Variant
    QuotaSID As String
    QuotaDisplayName As String
    QuotaAccountStatus As Long
End Type

Public Type PT_SHARE_INFO
    ErrNum As Long
    ErrDesc As String
    NetName As String
    ShareType As Long
    CurUses As Long
    MaxUses As Long
    Server As String
    LocalDrive As String
    LocalPath As String
    Remark As String
End Type

Public Type PT_FOLDER_INFO
    Protocol As PT_PROTOCOL
    RawName As String
    Name As String
    InetHandle As Long
    FtpHandle As Long
    FileHandle As Long
End Type

Public Type PT_FILE_TIME
    Invalid As Boolean
    Time As Date
    Time64 As Variant ' 100-nanosecond intervals since January 1, 1601 UTC
End Type

Public Type PT_FILE_INFO
    Error As Boolean
    ErrorNum As Long
    Continue As Boolean ' used by FindFile
    FindMask As String
    Handle As Long
    Protocol As PT_PROTOCOL
    FullName As String
    Folder As String
    Name As String
    AltName As String
    Extension As String
    Attrs As PT_ATTR
    IsFolder As Boolean
    IsFile As Boolean
    Creation As PT_FILE_TIME
    LastAccess As PT_FILE_TIME
    LastWrite As PT_FILE_TIME
    Size As Variant
    Space As Variant
End Type

Public Type PT_STREAM_INFO
    Fid As String
    Name As String
    ID As String
    Attrs As Long
    AttrsDesc As String
    Size As Variant
End Type

Public Type LARGE_INTEGER
    lowpart As Long
    highpart As Long
End Type

'============================================
' Private Consts, Enums, Types and Declares
'============================================
Private Const API_FORCE_ASCII = False

Private Const KB = 1024#
Private Const MB = (KB * 1024#) ' MegaBytes
Private Const GB = (MB * 1024#) ' GigaBytes
Private Const TB = (GB * 1024#) ' TeraBytes
Private Const pb = (TB * 1024#) ' PetaBytes
Private Const EB = (pb * 1024#) ' ExaBytes
Private Const ZB = (pb * 1024#) ' ZettaBytes
Private Const YB = (pb * 1024#) ' YottaBytes

'Private Const API_FORCE_WINVER = -1
'       WINVER = 400 = Windows 95, Windows98, Windows NT4 (default)
'       WINVER > 400 = Windows NT4 enhancements
'       WINVER = 500 = Windows 2000
'       WINVER = 501 = Windows XP
Private Const CALLBACK_INTERVAL = 5

Private Const MAX_PATH = 260
Private Const MAX_PATHW = 64000
Private Const WM_USER = &H400
Private Const NO_ERROR = 0
Private Const NOERROR = 0
Private Const ERROR_SUCCESS As Long = 0&

Private Const ERROR_ACCESS_DENIED = 5&
Private Const ERROR_BAD_NET_NAME = 67&
Private Const ERROR_BAD_PROVIDER = 1204&
Private Const ERROR_NOT_AUTHENTICATED = 1244&
Private Const ERROR_NO_NETWORK = 1222&
Private Const ERROR_INVALID_ADDRESS = 487&
Private Const ERROR_NO_NET_OR_BAD_PATH = 1203&
Private Const ERROR_NOT_CONNECTED = 2250&
Private Const ERROR_NOT_SUPPORTED = 50&
Private Const ERROR_MORE_DATA = 234
Private Const ERROR_EXTENDED_ERROR = 1208&
Private Const ERROR_CONNECTION_UNAVAIL = 1201&
Private Const ERROR_BAD_DEVICE = 1200&
Private Const ERROR_INVALID_LEVEL As Long = 124&
Private Const ERROR_INVALID_PARAMETER As Long = 87&
Private Const ERROR_NOT_ENOUGH_MEMORY As Long = 8&
Private Const ERROR_INVALID_NAME As Long = 123&

Private Const NERR_BASE                 As Long = 2100&
Private Const NERR_NetNameNotFound      As Long = (NERR_BASE + 210)

Private Const UNICODE_LOCAL_PREFIX = "\\?\"
Private Const UNICODE_UNC_PREFIX = "\\?\UNC\"

Private Const LM20_NNLEN = 12   ' LM 2.0 Net name length
Private Const SHPWLEN = 8              ' Share password length (bytes)

Public Const MAX_UINT32 = 2 ^ 32
Public Const MAX_SINT32 = (2 ^ 31) - 1
Public Const MAX_UINT16 = 2 ^ 16
Public Const MAX_SINT16 = (2 ^ 15) - 1

Private Const EM_SETTABSTOPS = &HCB

Private Const KD_DOWN = 0
Private Const KD_UP = &HC0000000
Private Const KD_EXTENDED = &H1000000

Private Const WM_KEYDOWN = &H100
Private Const WM_KEYUP = &H101

Private Const LWA_ALPHA = &H2&
Private Const GWL_EXSTYLE As Long = (-20)
Private Const GWL_STYLE As Long = (-16)

Private Enum API_MSG_FLAGS
    FORMAT_MESSAGE_ALLOCATE_BUFFER = &H100
    FORMAT_MESSAGE_ARGUMENT_ARRAY = &H2000
    FORMAT_MESSAGE_FROM_HMODULE = &H800
    FORMAT_MESSAGE_FROM_STRING = &H400
    FORMAT_MESSAGE_FROM_SYSTEM = &H1000
    FORMAT_MESSAGE_IGNORE_INSERTS = &H200
    FORMAT_MESSAGE_MAX_WIDTH_MASK = &HFF
End Enum

Private Enum STGM_CODE
    STGM_DIRECT = &H0&
    STGM_TRANSACTED = &H10000
    STGM_SIMPLE = &H8000000
    STGM_READ = &H0&
    STGM_WRITE = &H1&
    STGM_READWRITE = &H2&
    STGM_SHARE_DENY_NONE = &H40&
    STGM_SHARE_DENY_READ = &H30&
    STGM_SHARE_DENY_WRITE = &H20&
    STGM_SHARE_EXCLUSIVE = &H10&
    STGM_PRIORITY = &H40000
    STGM_DELETEONRELEASE = &H4000000
    STGM_CREATE = &H1000&
    STGM_CONVERT = &H20000
    STGM_FAILIFTHERE = &H0&
    STGM_NOSCRATCH = &H100000
End Enum

' SetWindowPos Flags
Private Enum SWP_FLAG
    SWP_NOSIZE = &H1
    SWP_NOMOVE = &H2
    SWP_NOZORDER = &H4
    SWP_NOREDRAW = &H8
    SWP_NOACTIVATE = &H10
    SWP_FRAMECHANGED = &H20       ' The frame changed: send WM_NCCALCSIZE
    SWP_SHOWWINDOW = &H40
    SWP_HIDEWINDOW = &H80
    SWP_NOCOPYBITS = &H100
    SWP_NOOWNERZORDER = &H200     ' Don't do owner Z ordering
    SWP_NOSENDCHANGING = &H400    ' Don't send WM_WINDOWPOSCHANGING

    SWP_DRAWFRAME = SWP_FRAMECHANGED
    SWP_NOREPOSITION = SWP_NOOWNERZORDER

    '   if(WINVER >= = &h0400)
    SWP_DEFERERASE = &H2000
    SWP_ASYNCWINDOWPOS = &H4000
    '   endif ' WINVER >= = &h0400 */

    SWP_FLAGS = SWP_NOMOVE Or SWP_NOSIZE
End Enum

Private Enum STD_WINDOW_HANDLE
    HWND_BOTTOM = 1
    HWND_TOP = 0
    HWND_TOPMOST = -1
    HWND_NOTOPMOST = -2
End Enum

Private Enum IS_TEXT_UNICODE_OPTION
    IS_TEXT_UNICODE_ASCII16 = &H1
    IS_TEXT_UNICODE_CONTROLS = &H4
    IS_TEXT_UNICODE_DBCS_LEADBYTE = &H400
    IS_TEXT_UNICODE_ILLEGAL_CHARS = &H100
    IS_TEXT_UNICODE_NOT_ASCII_MASK = &HF000
    IS_TEXT_UNICODE_NOT_UNICODE_MASK = &HF00
    IS_TEXT_UNICODE_NULL_BYTES = &H1000
    IS_TEXT_UNICODE_ODD_LENGTH = &H200
    IS_TEXT_UNICODE_REVERSE_ASCII16 = &H10
    IS_TEXT_UNICODE_REVERSE_CONTROLS = &H40
    IS_TEXT_UNICODE_REVERSE_MASK = &HF0
    IS_TEXT_UNICODE_REVERSE_SIGNATURE = &H80
    IS_TEXT_UNICODE_REVERSE_STATISTICS = &H20
    IS_TEXT_UNICODE_SIGNATURE = &H8
    IS_TEXT_UNICODE_STATISTICS = &H2
    IS_TEXT_UNICODE_UNICODE_MASK = &HF
End Enum

' CopyFileEx callback routine state change values
Private Enum COPY_FILE_STATE
    CALLBACK_CHUNK_FINISHED = &H0
    CALLBACK_STREAM_SWITCH = &H1
End Enum

' CopyFileEx option flags
Private Enum COPY_FLAG
    COPY_FILE_FAIL_IF_EXISTS = &H1
    COPY_FILE_RESTARTABLE = &H2
    COPY_FILE_OPEN_SOURCE_FOR_WRITE = &H4
    COPY_FILE_ALLOW_DECRYPTED_DESTINATION = &H8
End Enum

Private Enum BACKUP_STREAM_ID
    BACKUP_INVALID = &H0
    BACKUP_DATA = &H1
    BACKUP_EA_DATA = &H2
    BACKUP_SECURITY_DATA = &H3
    BACKUP_ALTERNATE_DATA = &H4
    BACKUP_LINK = &H5
    BACKUP_PROPERTY_DATA = &H6
    BACKUP_OBJECT_ID = &H7
    BACKUP_REPARSE_DATA = &H8
    BACKUP_SPARSE_BLOCK = &H9
End Enum

Private Enum BACKUP_BLOCK_ATTR
    STREAM_NORMAL_ATTRIBUTE = &H0
    STREAM_MODIFIED_WHEN_READ = &H1
    STREAM_CONTAINS_SECURITY = &H2
    STREAM_CONTAINS_PROPERTIES = &H4
    STREAM_SPARSE_ATTRIBUTE = &H8
    STREAM_VALID = STREAM_MODIFIED_WHEN_READ Or STREAM_CONTAINS_SECURITY Or STREAM_CONTAINS_PROPERTIES Or STREAM_SPARSE_ATTRIBUTE
End Enum

Private Enum DESIRED_ACCESS
    Delete = &H10000
    READ_CONTROL = &H20000
    WRITE_DAC = &H40000
    WRITE_OWNER = &H80000
    SYNCHRONIZE = &H100000
    STANDARD_RIGHTS_REQUIRED = &HF0000
    STANDARD_RIGHTS_READ = READ_CONTROL
    STANDARD_RIGHTS_WRITE = READ_CONTROL
    STANDARD_RIGHTS_EXECUTE = READ_CONTROL
    STANDARD_RIGHTS_ALL = &H1F0000
    SPECIFIC_RIGHTS_ALL = &HFFFF
    ACCESS_SYSTEM_SECURITY = &H1000000
    MAXIMUM_ALLOWED = &H2000000
    GENERIC_READ = &H80000000
    GENERIC_WRITE = &H40000000
    GENERIC_EXECUTE = &H20000000
    GENERIC_ALL = &H10000000

    FILE_READ_DATA = &H1    ' file & pipe
    FILE_READ_ACCESS = FILE_READ_DATA   ' file & pipe
    FILE_LIST_DIRECTORY = &H1     ' directory
    FILE_WRITE_DATA = &H2   ' file & pipe
    FILE_WRITE_ACCESS = FILE_WRITE_DATA ' file & pipe
    FILE_ADD_FILE = &H2         ' directory
    FILE_APPEND_DATA = &H4      ' file
    FILE_ADD_SUBDIRECTORY = &H4     '  directory
    FILE_CREATE_PIPE_INSTANCE = &H4 ' named pipe
    FILE_READ_EA = &H8    ' file & directory
    FILE_READ_PROPERTIES = FILE_READ_EA
    FILE_WRITE_EA = &H10  ' file & directory
    FILE_WRITE_PROPERTIES = FILE_WRITE_EA
    FILE_EXECUTE = &H20   ' file
    FILE_TRAVERSE = &H20         ' directory
    FILE_DELETE_CHILD = &H40    ' directory
    FILE_READ_ATTRIBUTES = &H80 ' all
    FILE_WRITE_ATTRIBUTES = &H100 ' all
    FILE_ALL_ACCESS = (STANDARD_RIGHTS_REQUIRED Or SYNCHRONIZE Or &H1FF)
    FILE_GENERIC_READ = (STANDARD_RIGHTS_READ Or FILE_READ_DATA Or FILE_READ_ATTRIBUTES Or FILE_READ_EA Or SYNCHRONIZE)
    FILE_GENERIC_WRITE = (STANDARD_RIGHTS_WRITE Or FILE_WRITE_DATA Or FILE_WRITE_ATTRIBUTES Or FILE_WRITE_EA Or FILE_APPEND_DATA Or SYNCHRONIZE)
    FILE_GENERIC_EXECUTE = (STANDARD_RIGHTS_EXECUTE Or FILE_READ_ATTRIBUTES Or FILE_EXECUTE Or SYNCHRONIZE)
End Enum

Private Enum FILE_CREATION
    CREATE_NEW = 1
    CREATE_ALWAYS = 2
    OPEN_EXISTING = 3
    OPEN_ALWAYS = 4
    TRUNCATE_EXISTING = 5
End Enum

Private Enum FILE_FLAG_AND_ATTR
    FILE_FLAG_FIRST_PIPE_INSTANCE = &H80000
    FILE_FLAG_OPEN_NO_RECALL = &H100000
    FILE_FLAG_OPEN_REPARSE_POINT = &H200000
    FILE_FLAG_POSIX_SEMANTICS = &H1000000
    FILE_FLAG_BACKUP_SEMANTICS = &H2000000
    FILE_FLAG_DELETE_ON_CLOSE = &H4000000
    FILE_FLAG_SEQUENTIAL_SCAN = &H8000000
    FILE_FLAG_RANDOM_ACCESS = &H10000000
    FILE_FLAG_NO_BUFFERING = &H20000000
    FILE_FLAG_OVERLAPPED = &H40000000
    FILE_FLAG_WRITE_THROUGH = &H80000000
    
    FILE_ATTRIBUTE_READONLY = &H1
    FILE_ATTRIBUTE_HIDDEN = &H2
    FILE_ATTRIBUTE_SYSTEM = &H4
    FILE_ATTRIBUTE_DIRECTORY = &H10
    FILE_ATTRIBUTE_ARCHIVE = &H20
    FILE_ATTRIBUTE_DEVICE = &H40
    FILE_ATTRIBUTE_NORMAL = &H80
    FILE_ATTRIBUTE_TEMPORARY = &H100
    FILE_ATTRIBUTE_SPARSE_FILE = &H200
    FILE_ATTRIBUTE_REPARSE_POINT = &H400
    FILE_ATTRIBUTE_COMPRESSED = &H800
    FILE_ATTRIBUTE_OFFLINE = &H1000
    FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = &H2000
    FILE_ATTRIBUTE_ENCRYPTED = &H4000
    FILE_ATTRIBUTE_SETTABLE = FILE_ATTRIBUTE_READONLY Or FILE_ATTRIBUTE_HIDDEN Or FILE_ATTRIBUTE_SYSTEM Or FILE_ATTRIBUTE_ARCHIVE Or FILE_ATTRIBUTE_NORMAL Or FILE_ATTRIBUTE_TEMPORARY Or FILE_ATTRIBUTE_NOT_CONTENT_INDEXED Or FILE_ATTRIBUTE_ENCRYPTED
End Enum

Private Enum FILE_SHARE
    FILE_SHARE_READ = &H1
    FILE_SHARE_WRITE = &H2
    FILE_SHARE_DELETE = &H4
End Enum

Private Enum DISKQUOTA_STATE
    DISKQUOTA_STATE_DISABLED = &H0
    DISKQUOTA_STATE_TRACK = &H1
    DISKQUOTA_STATE_ENFORCE = &H2
    DISKQUOTA_STATE_MASK = &H3
    DISKQUOTA_FILESTATE_INCOMPLETE = &H100
    DISKQUOTA_FILESTATE_REBUILDING = &H200
    DISKQUOTA_FILESTATE_MASK = &H300
    DISKQUOTA_STATE_UNKNOWN = -1
    DISKQUOTA_STATE_NOT_ROOT = -2
End Enum

Private Enum SHARE_TYPE
    STYPE_DISKTREE = 0
    STYPE_PRINTQ = 1
    STYPE_DEVICE = 2
    STYPE_IPC = 3
    STYPE_DFS = 100
    STYPE_SPECIAL = &H80000000
End Enum

Private Enum DISKQUOTA_USER_ACCOUNT
    DISKQUOTA_USER_ACCOUNT_RESOLVED = 0
    DISKQUOTA_USER_ACCOUNT_UNAVAILABLE = 1
    DISKQUOTA_USER_ACCOUNT_DELETED = 2
    DISKQUOTA_USER_ACCOUNT_INVALID = 3
    DISKQUOTA_USER_ACCOUNT_UNKNOWN = 4
    DISKQUOTA_USER_ACCOUNT_UNRESOLVED = 5
End Enum

Private Enum FILE_SYSTEM_FLAG
    FILE_CASE_SENSITIVE_SEARCH = &H1
    FILE_CASE_PRESERVED_NAMES = &H2
    FILE_UNICODE_ON_DISK = &H4
    FILE_PERSISTENT_ACLS = &H8
    FILE_FILE_COMPRESSION = &H10
    FILE_VOLUME_QUOTAS = &H20
    FILE_SUPPORTS_SPARSE_FILES = &H40
    FILE_SUPPORTS_REPARSE_POINTS = &H80
    FILE_SUPPORTS_REMOTE_STORAGE = &H100
    FILE_VOLUME_IS_COMPRESSED = &H8000
    FILE_SUPPORTS_OBJECT_IDS = &H10000
    FILE_SUPPORTS_ENCRYPTION = &H20000
    FILE_NAMED_STREAMS = &H40000
    FILE_READ_ONLY_VOLUME = &H80000

    FS_CASE_SENSITIVE = FILE_CASE_SENSITIVE_SEARCH
    FS_CASE_IS_PRESERVED = FILE_CASE_PRESERVED_NAMES
    FS_UNICODE_STORED_ON_DISK = FILE_UNICODE_ON_DISK
    FS_PERSISTENT_ACLS = FILE_PERSISTENT_ACLS
    FS_FILE_COMPRESSION = FILE_FILE_COMPRESSION
    FS_VOL_IS_COMPRESSED = FILE_VOLUME_IS_COMPRESSED
    FS_VOLUME_QUOTAS = FILE_VOLUME_QUOTAS
    FS_FILE_ENCRYPTION = FILE_SUPPORTS_ENCRYPTION
End Enum

Private Enum ENCRYPTION_STATUS
    FILE_ENCRYPTABLE = 0
    FILE_IS_ENCRYPTED = 1
    FILE_SYSTEM_ATTR = 2
    FILE_ROOT_DIR = 3
    FILE_SYSTEM_DIR = 4
    FILE_UNKNOWN = 5
    FILE_SYSTEM_NOT_SUPPORT = 6
    FILE_USER_DISALLOWED = 7
    FILE_READ_ONLY = 8
    FILE_DIR_DISALLOWED = 9
End Enum

Private Enum SECURITY_IMPERSONATION_LEVEL
    SecurityAnonymous = 1
    SecurityIdentification = 2
    SecurityImpersonation
    SecurityDelegation

    SECURITY_MIN_IMPERSONATION_LEVEL = SecurityAnonymous
    SECURITY_MAX_IMPERSONATION_LEVEL = SecurityDelegation
    DEFAULT_IMPERSONATION_LEVEL = SecurityImpersonation
End Enum

Private Enum FILE_SECURITY_QOS
    SECURITY_ANONYMOUS = (SecurityAnonymous * 2 ^ 16)
    SECURITY_CONTEXT_TRACKING = &H40000
    SECURITY_DELEGATION = (SecurityDelegation * 2 ^ 16)
    SECURITY_EFFECTIVE_ONLY = &H80000
    SECURITY_IMPERSONATION = (SecurityImpersonation * 2 ^ 16)
    SECURITY_IDENTIFICATION = (SecurityIdentification * 2 ^ 16)
    SECURITY_SQOS_PRESENT = &H100000
    SECURITY_VALID_SQOS_FLAGS = &H1F0000
End Enum

' Browsing for directory.
Private Enum BROWSE_FOLDER_FLAG
    BIF_RETURNONLYFSDIRS = &H1        ' For finding a folder to start document searching
    BIF_DONTGOBELOWDOMAIN = &H2       ' For starting the Find Computer
    BIF_STATUSTEXT = &H4
    BIF_RETURNFSANCESTORS = &H8
    BIF_EDITBOX = &H10
    BIF_VALIDATE = &H20                ' insist on valid result (or CANCEL)
    BIF_USENEWUI = &H40

    BIF_BROWSEFORCOMPUTER = &H1000    ' Browsing for Computers.
    BIF_BROWSEFORPRINTER = &H2000     ' Browsing for Printers
    BIF_BROWSEINCLUDEFILES = &H4000   ' Browsing for Everything
End Enum

Private Enum BROWSE_FOLDER_MSG
    ' message from browser
    BFFM_INITIALIZED = 1
    BFFM_SELCHANGED = 2
    BFFM_VALIDATEFAILEDA = 3     ' lParam:szPath ret:1(cont),0(EndDialog)
    BFFM_VALIDATEFAILEDW = 4      ' lParam:wzPath ret:1(cont),0(EndDialog)

    ' messages to browser
    BFFM_ENABLEOK = (WM_USER + 101)
    BFFM_SETSTATUSTEXTA = (WM_USER + 100)
    BFFM_SETSELECTIONA = (WM_USER + 102)
    BFFM_SETSTATUSTEXTW = (WM_USER + 104)
    BFFM_SETSELECTIONW = (WM_USER + 103)
    BFFM_SETOKTEXT = (WM_USER + 105)
    BFFM_SETEXPANDED = (WM_USER + 106)
End Enum

Private Enum SHFILEOP_FUNC
    FO_COPY = &H2
    FO_DELETE = &H3
    FO_MOVE = &H1
    FO_RENAME = &H4
End Enum

Private Enum SHFILEOP_FLAG
    FOF_MULTIDESTFILES = &H1
    FOF_CONFIRMMOUSE = &H2
    FOF_SILENT = &H4
    FOF_RENAMEONCOLLISION = &H8
    FOF_NOCONFIRMATION = &H10
    FOF_WANTMAPPINGHANDLE = &H20
    FOF_ALLOWUNDO = &H40
    FOF_FILESONLY = &H80
    FOF_SIMPLEPROGRESS = &H100
    FOF_NOCONFIRMMKDIR = &H200
    FOF_NOERRORUI = &H400
    FOF_NOCOPYSECURITYATTRIBS = &H800
    FOF_NORECURSION = &H1000
    FOF_NO_CONNECTED_ELEMENTS = &H2000
    FOF_WANTNUKEWARNING = &H4000
End Enum

Private Type WIN32_STREAM_ID
    dwStreamID As Long
    dwStreamAttributes As Long
    dwStreamSizeLow As Long
    dwStreamSizeHigh As Long
    dwStreamNameSize As Long
    cStreamName As Byte
End Type

Private Type FILETIME
    dwLowDateTime As Long
    dwHighDateTime As Long
End Type

Private Type SYSTEMTIME
    wYear As Integer
    wMonth As Integer
    wDayOfWeek As Integer
    wDay As Integer
    wHour As Integer
    wMinute As Integer
    wSecond As Integer
    wMilliseconds As Integer
End Type

Private Type OSVERSIONINFOA
    dwOSVersionInfoSize As Long
    dwMajorVersion As Long
    dwMinorVersion As Long
    dwBuildNumber As Long
    dwPlatformId As Long
    szCSDVersion As String * 128
End Type

Private Type OSVERSIONINFOW
    dwOSVersionInfoSize As Long
    dwMajorVersion As Long
    dwMinorVersion As Long
    dwBuildNumber As Long
    dwPlatformId As Long
    szCSDVersion((128 * 2) - 1) As Byte
End Type

Private Type SHARE_INFO_502
    lpNetName As Long   ' LPWSTR NetName;
    dwShareType As Long   ' DWORD dwShareType;
    lpRemark As Long   ' LPWSTR lpRemark;
    dwPerms As Long   ' DWORD dwPerms;
    dwMaxUses As Long   ' DWORD dwMaxUses;
    dwCurUses As Long   ' DWORD dwCurUses;
    lpPath As Long   ' LPWSTR lpPath;
    lpPasswd As Long   ' LPWSTR lpPasswd;
    reserved As Long   ' DWORD Reserved;
    lpSecurityDescriptor As Long ' PSECURITY_DESCRIPTOR lpSecurityDescriptor;
End Type

Private Type SHARE_INFO_50
    NetName As String * LM20_NNLEN ' 12
    dwShareType As Long
    dwFlags  As Long
    lpRemark As Long
    lpPath As Long
    RwPassword As String * SHPWLEN ' 8
    RoPassword As String * SHPWLEN ' 8
    Pad As String * 2048 ' room for returned strings
End Type

#If API_SELECT_FOLDER Then
Private Type BROWSEINFOA
    hwndOwner As Long ' HWND
    pidlRoot As Long ' LPCITEMIDLIST
    pszDisplayName As Long ' LPSTR Return display name of item selected.
    lpszTitle As Long ' LPCSTR text to go in the banner over the tree.
    ulFlags As Long ' UINT Flags that control the return stuff
    lpfn As Long ' BFFCALLBACK
    lParam As Long ' LPARAM extra info that's passed back in callbacks
    iImage As Long ' int output var: where to return the Image index.
End Type

Private Type BROWSEINFOW
    hwndOwner As Long ' HWND
    pidlRoot As Long ' LPCITEMIDLIST
    pszDisplayName As Long ' LPWSTR Return display name of item selected.
    lpszTitle As Long ' LPCWSTR text to go in the banner over the tree.
    ulFlags As Long ' UINT Flags that control the return stuff
    lpfn As Long ' BFFCALLBACK
    lParam As Long ' LPARAM extra info that's passed back in callbacks
    iImage As Long ' int  output var: where to return the Image index.
End Type
#End If ' API_SELECT_FOLDER

Private Type SHFILEOPSTRUCTA
    hWnd As Long
    wFunc As SHFILEOP_FUNC
    pFrom As String
    pTo As String
    fFlags As Integer
    fAnyOperationsAborted As Long
    hNameMappings As Long
    lpszProgressTitle As String '  only used if FOF_SIMPLEPROGRESS
End Type

Private Type SHFILEOPSTRUCTW
    hWnd As Long
    wFunc As SHFILEOP_FUNC
    pFrom As Long
    pTo As Long
    fFlags As Integer
    fAnyOperationsAborted As Long
    hNameMappings As Long
    lpszProgressTitle As Long '  only used if FOF_SIMPLEPROGRESS
End Type

Private Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Long
End Type

Private Type FIND_DATAA
    dwFileAttributes As Long
    ftCreationTime As FILETIME
    ftLastAccessTime As FILETIME
    ftLastWriteTime As FILETIME
    nFileSizeHigh As Long
    nFileSizeLow As Long
    dwReserved0 As Long
    dwReserved1 As Long
    cFileName As String * MAX_PATH
    cAlternate As String * 14
End Type

Private Type FIND_DATAW
    dwFileAttributes As Long
    ftCreationTime As FILETIME
    ftLastAccessTime As FILETIME
    ftLastWriteTime As FILETIME
    nFileSizeHigh As Long
    nFileSizeLow As Long
    dwReserved0 As Long
    dwReserved1 As Long
    cFileName((MAX_PATH * 2) - 1) As Byte ' 519
    cAlternateFileName(27) As Byte
End Type

Private Type FILE_ATTRIBUTE_DATA
    dwFileAttributes As Long
    ftCreationTime As FILETIME
    ftLastAccessTime As FILETIME
    ftLastWriteTime As FILETIME
    nFileSizeHigh As Long
    nFileSizeLow As Long
End Type

Private Enum GET_FILEEX_INFO_LEVEL
    GetFileExInfoStandard
    GetFileExMaxInfoLevel
End Enum

Private Type BY_HANDLE_FILE_INFORMATION
    dwFileAttributes As Long
    ftCreationTime As FILETIME
    ftLastAccessTime As FILETIME
    ftLastWriteTime As FILETIME
    dwVolumeSerialNumber As Long
    nFileSizeHigh As Long
    nFileSizeLow As Long
    nNumberOfLinks As Long
    nFileIndexHigh As Long
    nFileIndexLow As Long
End Type

Private Type POINTAPI
    X As Long
    Y As Long
End Type

Private Type HELPINFO
    cbSize As Long
    iContextType As Long
    iCtrlId As Long
    hItemHandle As Long
    dwContextId As Long
    MousePos As POINTAPI
End Type

Private Type HELPWININFO
    wStructSize As Long ' including rgchMember
    ' Windows Help divides the display into 1024 units in both
    ' the X and Y directions. To create a secondary window that
    ' fills the upper-left quadrant of the display, for example,
    ' an application would specify zero for the x and y members
    ' and 512 for the dx and dy members.
    X As Long ' X-coordinate of the upper-left corner of the window, in screen coordinates.
    Y As Long ' Y-coordinate of the upper-left corner of the window, in screen coordinates.
    dx As Long ' Width of the window, in pixels.
    dy As Long ' Height of the window, in pixels.
    wMax As Long ' SW_HIDE or SW_MINIMIZE or SW_SHOW, etc.
    rgchMember As String * 2 ' The name of the window. - may be longer than 2 chars
End Type

Private Declare Function GetSystemDirectoryA Lib "kernel32" ( _
    ByVal lpBuffer As String, _
    ByVal nSize As Long) As Long

Private Declare Function GetSystemDirectoryW Lib "kernel32" ( _
    ByRef lpBuffer As Byte, _
    ByVal nSize As Long) As Long

Private Declare Function LoadLibraryA Lib "kernel32" ( _
    ByVal lpLibFileName As String) As Long

Private Declare Function LoadLibraryW Lib "kernel32" ( _
    ByRef lpLibFileName As Byte) As Long

Private Declare Function GetProcAddress Lib "kernel32" ( _
    ByVal hModule As Long, _
    ByVal lpProcName As String) As Long

Private Declare Function FreeLibrary Lib "kernel32" ( _
    ByVal hLibModule As Long) As Long

Private Declare Function GetLastError Lib "kernel32" () As Long

Private Declare Function FormatMessageA Lib "kernel32" ( _
    ByVal dwFlags As API_MSG_FLAGS, _
    ByRef lpSource As Any, _
    ByVal dwMessageId As Long, _
    ByVal dwLanguageId As Long, _
    ByVal lpBuffer As String, _
    ByVal nSize As Long, _
    ByRef Arguments As Long) As Long

Private Declare Function FormatMessageW Lib "kernel32" ( _
    ByVal dwFlags As API_MSG_FLAGS, _
    lpSource As Any, _
    ByVal dwMessageId As Long, _
    ByVal dwLanguageId As Long, _
    ByRef lpBuffer As Byte, _
    ByVal nSize As Long, _
    ByRef Arguments As Long) As Long

Private Declare Function MessageBeep Lib "user32" ( _
    ByVal wType As PT_BEEP) As Long

Private Declare Sub Sleep Lib "kernel32" ( _
    ByVal dwMilliseconds As Long)

Private Declare Sub CopyMemory _
    Lib "kernel32" Alias "RtlMoveMemory" ( _
    ByVal Destination As Long, _
    ByVal Source As Long, _
    ByVal Length As Long)

Private Declare Sub ZeroMemory _
    Lib "kernel32.dll" Alias "RtlZeroMemory" ( _
    ByVal Destination As Long, _
    ByVal Length As Long)

Private Declare Function IsBadReadPtr Lib "kernel32" ( _
    ByVal lp As Long, _
    ByVal ucb As Long) As Long

Private Declare Function IsBadWritePtr Lib "kernel32" ( _
    ByVal lp As Long, _
    ByVal ucb As Long) As Long

Private Declare Function IsBadStringPtrA Lib "kernel32" ( _
    ByVal lpsz As String, _
    ByVal ucchMax As Long) As Long

Private Declare Function IsBadCodePtr Lib "kernel32" ( _
    ByVal lpfn As Long) As Long

Private Declare Function IsBadHugeReadPtr Lib "kernel32" ( _
    ByVal lp As Long, _
    ByVal ucb As Long) As Long

Private Declare Function IsBadHugeWritePtr Lib "kernel32" ( _
    ByVal lp As Long, _
    ByVal ucb As Long) As Long

Private Declare Function IsCharAlphaA Lib "user32" ( _
    ByVal cChar As Byte) As Long

Private Declare Function IsCharAlphaNumericA Lib "user32" ( _
    ByVal cChar As Byte) As Long

Private Declare Function IsCharLowerA Lib "user32" ( _
    ByVal cChar As Byte) As Long

Private Declare Function IsCharUpperA Lib "user32" ( _
    ByVal cChar As Byte) As Long

Private Declare Function IsTextUnicode Lib "advapi32" ( _
    ByRef lpBuffer As Byte, _
    ByVal cb As Long, _
    ByRef lpOptions As IS_TEXT_UNICODE_OPTION) As Long

Private Declare Function LoadStringA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal wID As Long, _
    ByVal lpBuffer As String, _
    ByVal nBufferMax As Long) As Long

Private Declare Function LoadStringW Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal wID As Long, _
    ByRef lpBuffer As Byte, _
    ByVal nBufferMax As Long) As Long

Private Declare Function LoadCursorA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal lpCursorName As String) As Long

Private Declare Function LoadCursorW Lib "user32" ( _
    ByVal hInstance As Long, _
    ByRef lpCursorName As Byte) As Long

Private Declare Function LoadIconA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal lpIconName As String) As Long

Private Declare Function LoadIconW Lib "user32" ( _
    ByVal hInstance As Long, _
    ByRef lpIconName As Byte) As Long

Private Declare Function LoadImageA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal lpszName As String, _
    ByVal uType As Long, _
    ByVal dwDesiredWidth As Long, _
    ByVal dwDesiredHeight As Long, _
    ByVal fuLoadOptions As Long) As Long

Private Declare Function LoadImageW Lib "user32" ( _
    ByVal hInstance As Long, _
    ByRef lpszName As Byte, _
    ByVal uType As Long, _
    ByVal dwDesiredWidth As Long, _
    ByVal dwDesiredHeight As Long, _
    ByVal fuLoadOptions As Long) As Long

Private Declare Function LoadBitmapA Lib "user32" ( _
    ByVal hInstance As Long, _
    ByVal lpBitmapName As String) As Long

Private Declare Function LoadBitmapW Lib "user32" ( _
    ByVal hInstance As Long, _
    ByRef lpBitmapName As Byte) As Long

Private Declare Function GetVersion Lib "kernel32" () As Long

Private Declare Function GetVersionExA Lib "kernel32" ( _
    lpVersionInformation As OSVERSIONINFOA) As Long

Private Declare Function GetVersionExW Lib "kernel32" ( _
    lpVersionInformation As OSVERSIONINFOW) As Long

Private Declare Function GetComputerNameA Lib "kernel32" ( _
    ByVal lpBuffer As String, _
    nSize As Long) As Long

Private Declare Function GetComputerNameW Lib "kernel32" ( _
    ByRef lpBuffer As Byte, _
    ByRef nSize As Long) As Long

Private Declare Function GetUserNameA Lib "advapi32.dll" ( _
    ByVal lpBuffer As String, _
    nSize As Long) As Long

Private Declare Function GetUserNameW Lib "advapi32.dll" ( _
    ByRef lpBuffer As Byte, _
    ByRef nSize As Long) As Long

Private Declare Function GetLogicalDrives Lib "kernel32" () As Long

Private Declare Function GetLogicalDriveStringsA Lib "kernel32" ( _
    ByVal nBufferLength As Long, _
    ByVal lpBuffer As String) As Long

Private Declare Function GetDiskFreeSpaceA Lib "kernel32" ( _
    ByVal lpRootPathName As String, _
    ByRef lpSectorsPerCluster As Long, _
    ByRef lpBytesPerSector As Long, _
    ByRef lpNumberOfFreeClusters As Long, _
    ByRef lpTotalNumberOfClusters As Long) As Long

Private Declare Function GetDiskFreeSpaceW Lib "kernel32" ( _
    ByRef lpRootPathName As Byte, _
    ByRef lpSectorsPerCluster As Long, _
    ByRef lpBytesPerSector As Long, _
    ByRef lpNumberOfFreeClusters As Long, _
    ByRef lpTotalNumberOfClusters As Long) As Long

Private Declare Function GetDiskFreeSpaceExA Lib "kernel32" ( _
    ByVal lpRootPathName As String, _
    ByRef lpFreeBytesAvailableToCaller As LARGE_INTEGER, _
    ByRef lpTotalNumberOfBytes As LARGE_INTEGER, _
    ByRef lpTotalNumberOfFreeBytes As LARGE_INTEGER) As Long

Private Declare Function GetDiskFreeSpaceExW Lib "kernel32" ( _
    ByRef lpRootPathName As Byte, _
    ByRef lpFreeBytesAvailableToCaller As LARGE_INTEGER, _
    ByRef lpTotalNumberOfBytes As LARGE_INTEGER, _
    ByRef lpTotalNumberOfFreeBytes As LARGE_INTEGER) As Long

Public Declare Function GetVolumeInformationA Lib "kernel32" ( _
    ByVal lpRootPathName As String, _
    ByVal lpVolumeNameBuffer As String, _
    ByVal nVolumeNameSize As Long, _
    ByRef lpVolumeSerialNumber As Long, _
    ByRef lpMaximumComponentLength As Long, _
    ByRef lpFileSystemFlags As Long, _
    ByVal lpFileSystemNameBuffer As String, _
    ByVal nFileSystemNameSize As Long) As Long

Private Declare Function GetDriveTypeA Lib "kernel32" ( _
    ByVal nDrive As String) As Long

Private Declare Function NetShareGetInfoW Lib "NETAPI32.dll" _
    Alias "NetShareGetInfo" ( _
    ByRef ServerName As Byte, _
    ByRef NetName As Byte, _
    ByVal nLevel As Long, _
    ByRef pBuffer As Long) As Long

Private Declare Function NetShareGetInfoA Lib "Svrapi.dll" _
    Alias "NetShareGetInfo" ( _
    ByVal ServerName As String, _
    ByVal NetName As String, _
    ByVal nLevel As Integer, _
    ByRef pBuffer As SHARE_INFO_50, _
    ByVal cbBuffer As Integer, _
    ByRef pcbTotalAvail As Integer) As Long

Private Declare Function NetServerGetInfoA Lib "Svrapi.dll" _
    Alias "NetServerGetInfo" ( _
    ByRef ServerName As String, _
    ByVal nLevel As Integer, _
    ByRef pBuffer As Long, _
    ByVal cbuffer As Integer, _
    ByRef pcbTotalAvail As Integer) As Long

Private Declare Function NetApiBufferFree Lib "NETAPI32.dll" ( _
    ByVal lpBuffer As Long) As Long

Private Declare Function WNetGetConnectionA Lib "mpr.dll" ( _
    ByVal lpszLocalName As String, _
    ByVal lpszRemoteName As String, _
    ByRef cbRemoteName As Long) As Long

Private Declare Function WNetGetConnectionW Lib "mpr.dll" ( _
    ByRef lpszLocalName As Byte, _
    ByRef lpszRemoteName As Byte, _
    ByRef cbRemoteName As Long) As Long

Private Declare Function GetCurrentDirectoryA Lib "kernel32" ( _
    ByVal nBufferLength As Long, _
    ByVal lpBuffer As String) As Long

Private Declare Function GetCurrentDirectoryW Lib "kernel32" ( _
    ByVal nBufferLength As Long, _
    ByRef lpBuffer As Byte) As Long

Private Declare Function CreateDirectoryA Lib "kernel32" ( _
    ByVal lpPathName As String, _
    ByRef lpSecurityAttributes As SECURITY_ATTRIBUTES) As Long

Private Declare Function CreateDirectoryW Lib "kernel32" ( _
    ByRef lpPathName As Byte, _
    ByRef lpSecurityAttributes As SECURITY_ATTRIBUTES) As Long

#If API_SELECT_FOLDER Then
Private Declare Function SHBrowseForFolderA Lib "shell32" ( _
    ByRef lpbi As BROWSEINFOA) As Long

Private Declare Function SHBrowseForFolderW Lib "shell32" ( _
    ByRef lpbi As BROWSEINFOW) As Long
#End If 'API_SELECT_FOLDER

Private Declare Function SHGetPathFromIDListA Lib "shell32" ( _
    ByVal pidList As Long, _
    ByVal lpBuffer As String) As Long

Private Declare Function SHGetPathFromIDListW Lib "shell32" ( _
    ByVal pidList As Long, _
    ByRef lpBuffer As Byte) As Long

Private Declare Function SHFileOperationA Lib "shell32.dll" ( _
    ByRef lpFileOp As SHFILEOPSTRUCTA) As Long

Private Declare Function SHFileOperationW Lib "shell32.dll" ( _
    ByRef lpFileOp As SHFILEOPSTRUCTW) As Long

Private Declare Function SHGetPathFromIDList Lib "shell32" ( _
    ByVal pidList As Long, _
    ByVal lpBuffer As String) As Long

Private Declare Function SHGetSpecialFolderLocation Lib "shell32" ( _
    ByVal hwndOwner As Long, _
    ByVal nFolder As Integer, _
    ByRef ppidl As Long) As Long

Private Declare Function PathCompactPathA Lib "shlwapi.dll" ( _
    ByVal hdc As Long, _
    ByVal pszPath As String, _
    ByVal dx As Long) As Long

Private Declare Function PathCompactPathW Lib "shlwapi.dll" ( _
    ByVal hdc As Long, _
    ByRef pszPath As Byte, _
    ByVal dx As Long) As Long

Private Declare Function PathCompactPathExA Lib "shlwapi.dll" ( _
    ByVal pszOut As String, _
    ByVal pszSrc As String, _
    ByVal cchMax As Long, _
    ByVal dwFlags As Long) As Long

Private Declare Function PathCompactPathExW Lib "shlwapi.dll" ( _
    ByRef pszOut As Byte, _
    ByRef pszSrc As Byte, _
    ByVal cchMax As Long, _
    ByVal dwFlags As Long) As Long

Private Declare Function GetTextExtentExPointA Lib "gdi32.dll" ( _
    ByVal hdc As Long, _
    ByVal lpszStr As String, _
    ByVal cchString As Long, _
    ByVal nMaxExtent As Long, _
    ByRef lpnFit As Long, _
    ByVal alpDx As Long, _
    ByRef lpSize As PT_SIZE) As Long

Private Declare Function GetTextExtentExPointI Lib "gdi32.dll" ( _
    ByVal hdc As Long, _
    ByRef pgiIn As Integer, _
    ByVal cgi As Long, _
    ByVal nMaxExtent As Long, _
    ByRef lpnFit As Long, _
    ByRef alpDx As Long, _
    ByRef lpSize As PT_SIZE) As Long

Private Declare Function GetTextExtentPointA Lib "gdi32.dll" ( _
    ByVal hdc As Long, _
    ByVal lpszString As String, _
    ByVal cbString As Long, _
    ByRef lpSize As PT_SIZE) As Long

Private Declare Function GetTextExtentPoint32A Lib "gdi32.dll" ( _
    ByVal hdc As Long, _
    ByVal lpsz As String, _
    ByVal cbString As Long, _
    ByRef lpSize As PT_SIZE) As Long

Private Declare Function GetTextExtentPointI Lib "gdi32.dll" ( _
    ByVal hdc As Long, _
    ByRef pgiIn As Integer, _
    ByVal cgi As Long, _
    ByRef lpSize As PT_SIZE) As Long

Private Declare Function SendMessageA Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal wMsg As Long, _
    ByVal wParam As Long, _
    ByRef lParam As Any) As Long

Private Declare Function SendMessageW Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal wMsg As Long, _
    ByVal wParam As Long, _
    ByRef lParam As Any) As Long

Private Declare Function SendMessageSetTabs Lib _
    "user32" Alias "SendMessageA" ( _
    ByVal hWnd As Long, _
    ByVal wMsg As Long, _
    ByVal wParam As Long, _
    ByRef lParam As Any) As Long

Private Declare Function FindFirstFileA Lib "kernel32" ( _
    ByVal lpFileName As String, _
    ByRef lpFindFileData As FIND_DATAA) As Long

Private Declare Function FindFirstFileW Lib "kernel32" ( _
    ByRef lpFileName As Byte, _
    ByRef lpFindFileData As FIND_DATAW) As Long

Private Declare Function FindNextFileA Lib "kernel32" ( _
    ByVal hFindFile As Long, _
    ByRef lpFindFileData As FIND_DATAA) As Long

Private Declare Function FindNextFileW Lib "kernel32" ( _
    ByVal hFindFile As Long, _
    ByRef lpFindFileData As FIND_DATAW) As Long

Private Declare Function FindClose Lib "kernel32" ( _
    ByVal hFindFile As Long) As Long

Private Declare Function GetFileTime Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpCreationTime As FILETIME, _
    ByRef lpLastAccessTime As FILETIME, _
    ByRef lpLastWriteTime As FILETIME) As Long

Private Declare Function SetFileTime Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpCreationTime As FILETIME, _
    ByRef lpLastAccessTime As FILETIME, _
    ByRef lpLastWriteTime As FILETIME) As Long

Private Declare Function GetFileAttributesA Lib "kernel32" ( _
    ByVal lpFileName As String) As Long

Private Declare Function GetFileAttributesW Lib "kernel32" ( _
    ByRef lpFileName As Byte) As Long

Private Declare Function SetFileAttributesA Lib "kernel32" ( _
    ByVal lpFileName As String, _
    ByVal dwFileAttributes As Long) As Long

Private Declare Function SetFileAttributesW Lib "kernel32" ( _
    ByRef lpFileName As Byte, _
    ByVal dwFileAttributes As Long) As Long

Private Declare Function GetFileAttributesExA Lib "kernel32.dll" ( _
    ByVal lpFileName As String, _
    ByVal fInfoLevelId As Long, _
    ByRef lpFileInformation As FILE_ATTRIBUTE_DATA) As Long

Private Declare Function GetFileAttributesExW Lib "kernel32.dll" ( _
    ByRef lpFileName As Byte, _
    ByVal fInfoLevelId As Long, _
    ByRef lpFileInformation As FILE_ATTRIBUTE_DATA) As Long

Private Declare Function GetFileInformationByHandle Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpFileInformation As BY_HANDLE_FILE_INFORMATION) As Long

Private Declare Function GetFileSize Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpFileSizeHigh As Long) As Long

Private Declare Function GetFileSizeEx Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpFileSize As LARGE_INTEGER) As Long

Private Declare Function GetCompressedFileSizeA Lib "kernel32" ( _
    ByVal lpFileName As String, _
    ByRef lpFileSizeHigh As Long) As Long

Private Declare Function GetCompressedFileSizeW Lib "kernel32" ( _
    ByRef lpFileName As Byte, _
    ByRef lpFileSizeHigh As Long) As Long

Private Declare Function GetShortPathNameA Lib "kernel32" ( _
    ByVal lpszLongPath As String, _
    ByVal lpszShortPath As String, _
    ByVal cchBuffer As Long) As Long

Private Declare Function GetShortPathNameW Lib "kernel32" ( _
    ByRef lpszLongPath As Byte, _
    ByRef lpszShortPath As Byte, _
    ByVal cchBuffer As Long) As Long

Private Declare Function GetLongPathNameA Lib "kernel32.dll" ( _
    ByVal lpszShortPath As String, _
    ByVal lpszLongPath As String, _
    ByVal cchBuffer As Long) As Long

Private Declare Function GetLongPathNameW Lib "kernel32.dll" ( _
    ByRef lpszShortPath As Byte, _
    ByRef lpszLongPath As Byte, _
    ByVal cchBuffer As Long) As Long

Private Declare Function GetBinaryTypeA Lib "kernel32.dll" ( _
    ByVal lpApplicationName As String, _
    ByRef lpBinaryType As Long) As Long

Private Declare Function GetBinaryTypeW Lib "kernel32.dll" ( _
    ByRef lpApplicationName As Byte, _
    ByRef lpBinaryType As Long) As Long

Private Declare Function GetFullPathNameA Lib "kernel32.dll" ( _
    ByVal lpFileName As String, _
    ByVal nBufferLength As Long, _
    ByVal lpBuffer As String, _
    ByVal lpFilePart As String) As Long

Private Declare Function GetFullPathNameW Lib "kernel32.dll" ( _
    ByRef lpFileName As Byte, _
    ByVal nBufferLength As Long, _
    ByVal lpBuffer As String, _
    ByRef lpFilePart As Byte) As Long

Private Declare Function DeleteFileA Lib "kernel32" ( _
    ByVal lpFileName As String) As Long

Private Declare Function DeleteFileW Lib "kernel32" ( _
    ByRef lpFileName As Byte) As Long

Private Declare Function CreateFileA Lib "kernel32" ( _
    ByVal lpFileName As String, _
    ByVal dwDesiredAccess As DESIRED_ACCESS, _
    ByVal dwShareMode As FILE_SHARE, _
    ByRef lpSecurityAttributes As SECURITY_ATTRIBUTES, _
    ByVal dwCreationDisposition As FILE_CREATION, _
    ByVal dwFlagsAndAttributes As FILE_FLAG_AND_ATTR, _
    ByVal hTemplateFile As Long) As Long

Private Declare Function CreateFileW Lib "kernel32" ( _
    ByRef lpFileName As Byte, _
    ByVal dwDesiredAccess As DESIRED_ACCESS, _
    ByVal dwShareMode As FILE_SHARE, _
    ByRef lpSecurityAttributes As SECURITY_ATTRIBUTES, _
    ByVal dwCreationDisposition As FILE_CREATION, _
    ByVal dwFlagsAndAttributes As FILE_FLAG_AND_ATTR, _
    ByVal hTemplateFile As Long) As Long

Private Declare Function CloseHandle Lib "kernel32" ( _
    ByVal hObject As Long) As Long

Private Declare Function ReadFile Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpBuffer As Byte, _
    ByVal nNumberOfBytesToRead As Long, _
    ByRef lpNumberOfBytesRead As Long, _
    ByVal lpOverlapped As Long) As Long

Private Declare Function WriteFile Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpBuffer As Byte, _
    ByVal nNumberOfBytesToWrite As Long, _
    ByRef lpNumberOfBytesWritten As Long, _
    ByVal lpOverlapped As Long) As Long

#If API_BACKUP Or API_FILE_INFO Then
Private Declare Function BackupRead Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpBuffer As Byte, _
    ByVal nNumberOfBytesToRead As Long, _
    ByRef lpNumberOfBytesRead As Long, _
    ByVal bAbort As Long, _
    ByVal bProcessSecurity As Long, _
    ByRef lpContext As Long) As Long

Private Declare Function BackupSeek Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByVal dwLowBytesToSeek As Long, _
    ByVal dwHighBytesToSeek As Long, _
    ByRef lpdwLowByteSeeked As Long, _
    ByRef lpdwHighByteSeeked As Long, _
    ByRef lpContext As Long) As Long

Private Declare Function BackupWrite Lib "kernel32" ( _
    ByVal hFile As Long, _
    ByRef lpBuffer As Byte, _
    ByVal nNumberOfBytesToWrite As Long, _
    ByRef lpNumberOfBytesWritten As Long, _
    ByVal bAbort As Long, _
    ByVal bProcessSecurity As Long, _
    ByRef lpContext As Long) As Long

Private Declare Function CopyFileExA Lib "kernel32.dll" ( _
    ByVal lpExistingFileName As String, _
    ByVal lpNewFileName As String, _
    ByVal lpProgressRoutine As Long, _
    ByRef lpCallbackData As Long, _
    ByRef pbCancel As Long, _
    ByVal dwCopyFlags As Long) As Long

Private Declare Function CopyFileExW Lib "kernel32.dll" ( _
    ByRef lpExistingFileName As Byte, _
    ByRef lpNewFileName As Byte, _
    ByVal lpProgressRoutine As Long, _
    ByRef lpCallbackData As Long, _
    ByRef pbCancel As Boolean, _
    ByVal dwCopyFlags As Long) As Long
#End If ' API_BACKUP

Private Declare Function FileTimeToSystemTime Lib "kernel32" ( _
    ByRef lpFileTime As FILETIME, _
    ByRef lpSystemTime As SYSTEMTIME) As Long

Private Declare Function FileTimeToLocalFileTime Lib "kernel32" ( _
    ByRef lpFileTime As FILETIME, _
    ByRef lpLocalFileTime As FILETIME) As Long

Private Declare Function GetTickCount Lib "kernel32" () As Long

Private Declare Function MapVirtualKeyA Lib "user32" ( _
    ByVal wCode As Long, _
    ByVal wMapType As Long) As Long

Private Declare Function MapVirtualKeyExA Lib "user32" ( _
    ByVal uCode As Long, _
    ByVal uMapType As Long, _
    ByVal dwhkl As Long) As Long

Private Declare Sub CoInitialize Lib "ole32.dll" ( _
    ByVal pvReserved As Long)

Private Declare Sub CoTaskMemFree Lib "ole32.dll" ( _
    ByVal pv As Long)

Private Declare Sub CoUninitialize Lib "ole32.dll" ()

Private Declare Function InitCommonControls _
    Lib "comctl32.dll" () As Long

Private Declare Function WinHelpA Lib "user32.dll" ( _
    ByVal hWnd As Long, _
    ByVal lpHelpFile As String, _
    ByVal wCommand As Long, _
    ByVal dwData As Long) As Long

Private WinVer As PT_OS_INFO
Private HelpFile As String
Private HelpEnabled As Boolean

Private UNICODE_OK As Boolean
Private ICC_OK As Boolean
Private GDFSEX_OK As Boolean
Private GCNW_OK As Boolean
Private CFW_OK As Boolean
Private GFAW_OK As Boolean
Private SFAW_OK As Boolean
Private CDW_OK As Boolean
Private GCDW_OK As Boolean
Private GFSX_OK As Boolean
Private DFW_OK As Boolean
Private FFFW_OK As Boolean
Private FNFW_OK As Boolean
Private GFAXW_OK As Boolean
Private GFAXA_OK As Boolean
Private GCFSW_OK As Boolean
Private GCFSA_OK As Boolean

Private SHBFF_OK As Boolean
Private SHGPFIL_OK As Boolean
Private SHFO_OK As Boolean
Private SHPCP_OK As Boolean
Private SHPCPX_OK As Boolean

Private SHBFFW_OK As Boolean
Private SHGPFILW_OK As Boolean
Private SHFOW_OK As Boolean
Private SHPCPW_OK As Boolean
Private SHPCPXW_OK As Boolean

Private GUNW_OK As Boolean
Private GSDW_OK As Boolean
Private FMW_OK As Boolean
Private BACKUP_OK As Boolean
Private CFX_OK As Boolean
Private CFXA_OK As Boolean
Private CFXW_OK As Boolean
Private GVXW_OK As Boolean
Private GVXA_OK As Boolean
Private GLPNA_OK As Boolean
Private GLPNW_OK As Boolean
Private GSPNA_OK As Boolean
Private GSPNW_OK As Boolean

Private WNGCW_OK As Boolean
'Private XParent_OK As Boolean

Private gCurrentDir As String
Private gFileCopyCallback As Form
Private InitDone As Boolean
Private InitShell32Done As Boolean
Private InitShlwApiDone As Boolean
Private InitNetDone As Boolean
Private Sub ApiInitNet()
    Dim hnd As Long

    If Not InitDone Then ApiInit
    InitNetDone = True

    hnd = LoadLibraryA("mpr.dll")
    If hnd = 0 Then Exit Sub

    If WinVer.Unicode Then
        If GetProcAddress(hnd, "WNetGetConnectionW") <> 0 Then WNGCW_OK = True
    End If

    FreeLibrary hnd
End Sub
Private Sub ApiInitShlwApi()
    Dim hnd As Long

    If Not InitDone Then ApiInit
    InitShlwApiDone = True

    hnd = LoadLibraryA("shlwapi.dll")
    If hnd = 0 Then Exit Sub

    If GetProcAddress(hnd, "PathCompactPathA") <> 0 Then SHPCP_OK = True
    If GetProcAddress(hnd, "PathCompactPathExA") <> 0 Then SHPCPX_OK = True

    If WinVer.Unicode Then
        If GetProcAddress(hnd, "PathCompactPathW") <> 0 Then SHPCPW_OK = True
        If GetProcAddress(hnd, "PathCompactPathExW") <> 0 Then SHPCPXW_OK = True
    End If

    FreeLibrary hnd
End Sub
Private Sub ApiInitShell32()
    Dim hnd As Long

    If Not InitDone Then ApiInit
    InitShell32Done = True

    hnd = LoadLibraryA("shell32")
    If hnd = 0 Then Exit Sub

    If GetProcAddress(hnd, "SHBrowseForFolderA") <> 0 Then SHBFF_OK = True
    If GetProcAddress(hnd, "SHGetPathFromIDListA") <> 0 Then SHGPFIL_OK = True
    If GetProcAddress(hnd, "SHFileOperationA") <> 0 Then SHFO_OK = True

    If WinVer.Unicode Then
        If GetProcAddress(hnd, "SHBrowseForFolderW") <> 0 Then SHBFFW_OK = True
        If GetProcAddress(hnd, "SHGetPathFromIDListW") <> 0 Then SHGPFILW_OK = True
        If GetProcAddress(hnd, "SHFileOperationW") <> 0 Then SHFOW_OK = True
    End If

    FreeLibrary hnd
End Sub
Public Sub ApiInit()
    Dim hnd As Long

    InitDone = True

    hnd = LoadLibraryA("kernel32")
    If hnd = 0 Then Exit Sub

    If GetProcAddress(hnd, "GetVersionExA") <> 0 Then GVXA_OK = True

    If Not API_FORCE_ASCII Then
        If GetProcAddress(hnd, "GetVersionExW") <> 0 Then GVXW_OK = True
    End If
    WinVer = WinVersion()
    If GetProcAddress(hnd, "GetFileSizeEx") <> 0 Then GFSX_OK = True
    If GetProcAddress(hnd, "GetDiskFreeSpaceExA") <> 0 Then GDFSEX_OK = True
    If GetProcAddress(hnd, "GetCompressedFileSizeA") <> 0 Then GCFSA_OK = True
    If GetProcAddress(hnd, "GetFileAttributesExA") <> 0 Then GFAXA_OK = True
    If GetProcAddress(hnd, "CopyFileExA") <> 0 Then CFXA_OK = True
    If GetProcAddress(hnd, "GetLongPathNameA") <> 0 Then GLPNA_OK = True
    If GetProcAddress(hnd, "GetShortPathNameA") <> 0 Then GSPNA_OK = True

    FreeLibrary hnd

    If WinVer.Unicode Then ' GetVersionW worked, so check the rest
        GCFSW_OK = True ' APIExists("kernel32", "GetCompressedFileSizeW")
        SFAW_OK = True ' APIExists("kernel32", "SetFileAttributesW")
        CFXW_OK = True ' APIExists("kernel32", "CopyFileExW")

        GCNW_OK = True ' APIExists("kernel32", "GetComputerNameW")
        GUNW_OK = True ' APIExists("advapi32.dll", "GetUserNameW")
        GSDW_OK = True ' APIExists("kernel32", "GetSystemDirectoryW")
        FMW_OK = True ' APIExists("kernel32", "FormatMessageW")

        CFW_OK = True ' APIExists("kernel32", "CreateFileW")
        DFW_OK = True ' APIExists("kernel32", "DeleteFileW")
        GFAW_OK = True ' APIExists("kernel32", "GetFileAttributesW")
        CDW_OK = True ' APIExists("kernel32", "CreateDirectoryW")
        GCDW_OK = True ' APIExists("kernel32", "GetCurrentDirectoryW")
        FFFW_OK = True ' APIExists("kernel32", "FindFirstFileW")
        FNFW_OK = True ' APIExists("kernel32", "FindNextFileW")
        GFAXW_OK = True ' APIExists("kernel32", "GetFileAttributesExW")
        GLPNW_OK = True ' APIExists("kernel32.dll", "GetLongPathNameW")
        GSPNW_OK = True ' APIExists("kernel32.dll", "GetShortPathNameW")
    End If

'    If WinVer.Major >= 5 Then XParent_OK = True

    CFX_OK = CFXW_OK Or CFXA_OK
    ICC_OK = APIExists("comctl32.dll", "InitCommonControls")

    If WinVer.PlatformNT Then
        BACKUP_OK = True
    End If

    HelpFile = App.Path & "\" & App.Title & ".hlp"
    If ApiFileExists(HelpFile) Then
'        App.HelpFile = helpfile
        HelpEnabled = True
    End If
End Sub
Public Function ApiHelpEnabled() As Boolean
    If Not InitDone Then ApiInit

    ApiHelpEnabled = HelpEnabled
End Function
Public Function ApiHelpTopic(TopicID As Long) As Boolean
    If Not InitDone Then ApiInit
    If Not HelpEnabled Then Exit Function

    WinHelpA 0, HelpFile, HELP_CONTEXT, TopicID
    ApiHelpTopic = True
End Function
Public Function ApiHelpIndex() As Boolean
    If Not InitDone Then ApiInit
    If Not HelpEnabled Then Exit Function

    WinHelpA 0, HelpFile, HELP_INDEX, 0
    ApiHelpIndex = True
End Function
Public Function ApiHelpContents() As Boolean
    If Not InitDone Then ApiInit
    If Not HelpEnabled Then Exit Function

    WinHelpA 0, HelpFile, HELP_CONTENTS, 0
    ApiHelpContents = True
End Function
Public Sub APiHelpQuit()
    WinHelpA 0, HelpFile, HELP_QUIT, 0
End Sub
Private Function WinVersion() As PT_OS_INFO
    Dim result As Long
    Dim osva As OSVERSIONINFOA
    Dim osvw As OSVERSIONINFOW
    Dim uba() As Byte
    Dim s As String
    Dim ptoi As PT_OS_INFO
    Dim Desc As String

    If GVXW_OK Then
        osvw.dwOSVersionInfoSize = LenB(osvw)
        result = GetVersionExW(osvw)
        If result = 0 Then ' failure
            SetApiError
            GVXW_OK = False
            WinVersion = WinVersion()
            Exit Function
        End If
'        ptoi.Unicode = True
        ptoi.Major = osvw.dwMajorVersion
        ptoi.Minor = osvw.dwMinorVersion
        ptoi.Build = osvw.dwBuildNumber
        ptoi.Platform = osvw.dwPlatformId
        uba = osvw.szCSDVersion
        s = uba
        ptoi.CsdVersion = Trim(ApiTextStrip(s))
    ElseIf GVXA_OK Then
        osva.dwOSVersionInfoSize = Len(osva)
        result = GetVersionExA(osva)
        If result = 0 Then ' failure
            SetApiError
            GVXA_OK = False
            WinVersion = WinVersion()
            Exit Function
        End If
'        ptoi.Unicode = False
        ptoi.Major = osva.dwMajorVersion
        ptoi.Minor = osva.dwMinorVersion
        ptoi.Build = osva.dwBuildNumber
        ptoi.Platform = osva.dwPlatformId
        ptoi.CsdVersion = Trim(ApiTextStrip(osva.szCSDVersion))
    Else ' GetVersionEx not supported?
        result = GetVersion()
'        ptoi.Unicode = False
        ptoi.Major = result And &HFF
        ptoi.Minor = (result And &HFF00&) / (2 ^ 8)
        If result And &H80000000 Then
            ptoi.Platform = VER_PLATFORM_WIN32_WINDOWS
            ptoi.Build = 0
        Else
            ptoi.Platform = VER_PLATFORM_WIN32_NT
            ptoi.Build = (result And &HFFFF0000) / (2 ^ 16)
        End If
        ptoi.CsdVersion = ""
    End If

    Select Case ptoi.Platform
        Case VER_PLATFORM_WIN32_NT:
            ptoi.Platform9x = False
            ptoi.PlatformNT = True
            ptoi.Unicode = True
        Case VER_PLATFORM_WIN32_WINDOWS:
            ptoi.Platform9x = True
            ptoi.PlatformNT = False
            ptoi.Unicode = False
        Case Else ' VER_PLATFORM_Platform32s
            ptoi.Platform9x = False
            ptoi.PlatformNT = False
            ptoi.Unicode = False
    End Select

    If API_FORCE_ASCII Then ptoi.Unicode = False

    ' flag individual versions
    Select Case ptoi.Major
        Case 3:
            If ptoi.PlatformNT Then
                ptoi.WinNT351 = True
                Desc = "Windows NT"
            Else
                ptoi.Platform32s = True
                Desc = "Windows Win32s"
            End If

        Case 4:
            If ptoi.PlatformNT Then
                ptoi.WinNT = True
                Desc = "Windows NT"
            End If
            Select Case ptoi.Minor
                Case 0:
                    If ptoi.PlatformNT Then
                        ptoi.WinNT40 = True
                        Desc = "Windows NT 4.0"
                        If ptoi.Build >= 1381 Then
                            ptoi.WinNT40Sp6 = True
                        End If
                    Else
                        ptoi.Win95 = True
                        If ptoi.CsdVersion = "C" Then
                            ptoi.Win95Osr2 = True
                            Desc = "Windows 95 OSR2"
                        End If
                    End If
                Case 10:
                    ptoi.Win98 = True
                    If ptoi.CsdVersion = "uba" Then
                        ptoi.Win98SE = True
                        Desc = "Windows 98 SE"
                    End If
                Case 90
                    ptoi.WinMe = True
                    Desc = "Windows Me"
            End Select

        Case 5:
            Select Case ptoi.Minor
                Case 0:
                    ptoi.Win2K = True
                    Desc = "Windows 2000"
                Case 1:
                    ptoi.WinXP = True
                    Desc = "Windows XP"
                Case 2:
                    ptoi.Win2K3 = True
                    Desc = "Windows 2003"
            End Select
    End Select
    If ptoi.Platform9x And (Not ptoi.WinMe) Then ptoi.Win9x = True

    Desc = Desc & " v" & ptoi.Major & "." & ptoi.Minor
    ptoi.Desc = Desc

'    set WINVER, used by SDK headers
'    WINVER < 0x400 = Windows NT 3.5, Windows NT 3.51
'    WINVER = 0x400 = Windows 95, Windows98, Windows NT4 (default)
'    WINVER > 0x400 = Windows NT4 enhancements
'    WINVER = 0x500 = Windows 2000
'    WINVER = 0x501 = Windows XP
'    WINVER = 0x502 = Windows 2003
    ptoi.WinVer = (ptoi.Major * 100) + ptoi.Minor
    If ptoi.Platform9x Then ptoi.WinVer = 400
    If ptoi.WinNT351 Then ptoi.WinVer = 400
    If ptoi.WinNT40Sp6 Then
        ptoi.WinVer = 401
        Desc = Desc & " SP6"
    End If

'   set WIN32_WINNT
    If ptoi.Win2K3 Then
        ptoi.WIN32_WINNT = 502
    ElseIf ptoi.WinXP Then
        ptoi.WIN32_WINNT = 501
    ElseIf ptoi.Win2K Then
        ptoi.WIN32_WINNT = 500
    ElseIf ptoi.WinNT40 Then
        ptoi.WIN32_WINNT = 400
    End If

'   set WIN32_WINDOWS
    If ptoi.WinMe Then
        ptoi.WIN32_WINDOWS = 490
    ElseIf ptoi.Win98 Then
        ptoi.WIN32_WINDOWS = 410
    ElseIf ptoi.Win95 Then
        ptoi.WIN32_WINDOWS = 400
    End If

'Internet Explorer 6.0 _WIN32_IE>=0x0600
'Internet Explorer 5.01, 5.5 _WIN32_IE>=0x0501
'Internet Explorer 5.0, 5.0a, 5.0b _WIN32_IE>=0x0500
'Internet Explorer 4.01 _WIN32_IE>=0x0401
'Internet Explorer 4.0 _WIN32_IE>=0x0400
'Internet Explorer 3.0, 3.01, 3.02 _WIN32_IE>=0x0300

    WinVersion = ptoi
    ApiLastError = 0
End Function
Public Function ApiSendKey(Frm As Form, Key As VIRTUAL_KEY)
    Dim ScanCode As Long

    ScanCode = MapVirtualKeyA(Key, 0) * 65536
    SendMessageA Frm.hWnd, WM_KEYDOWN, Key, 1 Or ScanCode Or KD_EXTENDED Or KD_DOWN
    SendMessageA Frm.hWnd, WM_KEYUP, Key, 1 Or ScanCode Or KD_EXTENDED Or KD_UP
    DoEvents
End Function
Public Function ApiTickStart() As Variant
    Dim Ticks As Long

    Ticks = GetTickCount()
    ApiTickStart = ApiConvUInt32Dec(Ticks)
End Function
Public Function ApiTickElapsed(TickStart As Variant) As Variant
    Dim Ticks As Variant
    Dim Elapsed As Variant

    Ticks = ApiConvUInt32Dec(GetTickCount())
    If Ticks < TickStart Then Ticks = Ticks + CDec(MAX_UINT32)
    Elapsed = CDec(CDec(Ticks) - CDec(TickStart))
    ApiTickElapsed = Elapsed
End Function
Private Function UnicodeFid(Fid As String) As Byte()
    Dim uba() As Byte

    'For example, "\\?\D:\<path>".
    'For example, "\\?\UNC\<server>\<share>".
    If Left(Fid, 2) = "\\" Then ' UNC
        uba = UNICODE_UNC_PREFIX & Mid(Fid, 3) & Chr(0)
    Else
        uba = UNICODE_LOCAL_PREFIX & Fid & Chr(0)
    End If

    UnicodeFid = uba
End Function
Public Function ApiUnderIde() As Boolean
    Dim b As Boolean

    b = False
    ' the following statement executes only under IDE
    Debug.Assert UnderIde(b)
    ApiUnderIde = b
End Function
Private Function UnderIde(ByRef b As Boolean) As Boolean
   b = True
   UnderIde = True
End Function
Public Function ApiWinVersion() As PT_OS_INFO
    If Not InitDone Then ApiInit

    ApiWinVersion = WinVer
End Function
#If API_SHORTCUT Then
Public Function ApiShortcutMax() As Long
    ApiShortcutMax = MAX_PATH
End Function
Public Function ApiShortcutCreate(LnkFile As String, ExeFile As String, _
    WorkDir As String, ExeArgs As String, Desc As String, _
    IconFile As String, IconInx As Long, ShowCmd As SHOW_COMMAND) As Boolean

    Dim result As Long
    Dim pidl As Long
    Dim cShellLink As PTShellLink
    Dim cPersistFile As PTPersistFile

    ApiShortcutCreate = False

    If Len(LnkFile) <= 0 Then Exit Function
    If Len(ExeFile) <= 0 Then Exit Function

    On Error GoTo ErrHandler
    Set cShellLink = New PTShellLink
    Set cPersistFile = cShellLink

    cShellLink.SetPath ExeFile
    If (WorkDir <> "") Then cShellLink.SetWorkingDirectory WorkDir
    If (ExeArgs <> "") Then cShellLink.SetArguments ExeArgs
    If (Desc <> "") Then cShellLink.SetDescription Desc & vbNullChar
    If (IconFile <> "") Then cShellLink.SetIconLocation IconFile, IconInx ' Set shortcut icon location & index

'   .SetHotkey wHotKey
'   .SetIDList pidl
'   .SetRelativePath pszPathRel, 0

    cShellLink.SetShowCmd ShowCmd

    cShellLink.Resolve 0, SLR_UPDATE
    cPersistFile.Save StrConv(LnkFile, vbUnicode), 0    ' Unicode conversion hack... This must be done!
    'cShellLink.Save StrConv(LnkFile, vbUnicode), 0    ' Unicode conversion hack... This must be done!
    ApiShortcutCreate = True

ErrHandler:
    Set cPersistFile = Nothing
    Set cShellLink = Nothing
End Function
Public Function ApiShortcutInfo(ByRef LnkFile As String, ByRef Desc As String, _
    ByRef ExeFile As String, ByRef WorkDir As String, ByRef Args As String, _
    ByRef ShowCmd As Long) As Boolean

    Dim pidl As Long
    Dim wHotKey As Long
    Dim IconFile As String
    Dim IconIdx As Long
    Dim fd As SL_FIND_DATAA
    Dim cShellLink As PTShellLink
    Dim cPersistFile As PTPersistFile

    ApiShortcutInfo = False

    If Len(LnkFile) <= 0 Then Exit Function

    Set cShellLink = New PTShellLink
    Set cPersistFile = cShellLink

    ' Load Shortcut file...(must do this UNICODE hack!)
    On Error GoTo ErrHandler
    cPersistFile.Load StrConv(LnkFile, vbUnicode), STGM_DIRECT
    'cShellLink.Load StrConv(LnkFile, vbUnicode), STGM_DIRECT

    ExeFile = Space(MAX_PATH)
    cShellLink.GetPath ExeFile, Len(ExeFile), fd, SLGP_UNCPRIORITY
    ExeFile = ApiTextStrip(ExeFile)

    WorkDir = Space(MAX_PATH)
    cShellLink.GetWorkingDirectory WorkDir, Len(WorkDir)
    WorkDir = ApiTextStrip(WorkDir)

    Args = Space(MAX_PATH)
    cShellLink.GetArguments Args, Len(Args)
    Args = ApiTextStrip(Args)

    Desc = Space(MAX_PATH)
    cShellLink.GetDescription Desc, Len(Desc)
    Desc = ApiTextStrip(Desc)

    cShellLink.GetHotkey wHotKey

    IconFile = Space(MAX_PATH)
    IconIdx = 0
    cShellLink.GetIconLocation IconFile, Len(IconFile), IconIdx
    IconFile = ApiTextStrip(IconFile)

    cShellLink.GetIDList pidl
    cShellLink.GetShowCmd ShowCmd

    ApiShortcutInfo = True

ErrHandler:
    Set cPersistFile = Nothing
    Set cShellLink = Nothing
End Function
#End If ' API_SHORTCUT
Public Function ApiSpecialFolder(ByVal csidl As CSIDL_CODE, Optional Frm As Form = Nothing) As String
    Dim s As String
    Dim result As Long
    Dim ppidl As Long

    s = Space(MAX_PATH)

    If Frm Is Nothing Then
        result = SHGetSpecialFolderLocation(0, csidl, ppidl)
    Else
        result = SHGetSpecialFolderLocation(Frm.hWnd, csidl, ppidl)
    End If
    If result = NOERROR Then
        result = SHGetPathFromIDList(ppidl, s)
        If result Then
            s = ApiTextStrip(s)
            s = Trim(s)
            If Right(s, 1) <> "\" Then s = s & "\"
            ApiSpecialFolder = s
        End If
        CoTaskMemFree ppidl
    Else
        ApiSpecialFolder = ""
    End If
End Function
Public Function ApiAttrsGet(Fid As String) As PT_ATTR
    Dim uba() As Byte
    Dim Attrs As Long

    If Not InitDone Then ApiInit

    If GFAW_OK Then
        uba = UnicodeFid(Fid)
        Attrs = GetFileAttributesW(uba(0))
    Else
        Attrs = GetFileAttributesA(Fid)
    End If

    If Attrs = PT_ATTR_INVALID Then
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiAttrsGet = Attrs
End Function
Public Function ApiAttrsSet(Fid As String, Attrs As PT_ATTR) As Long
    Dim uba() As Byte
    Dim result As Long

    If Not InitDone Then ApiInit

    If SFAW_OK Then
        uba = UnicodeFid(Fid)
        result = SetFileAttributesW(uba(0), Attrs)
    Else
        result = SetFileAttributesA(Fid, Attrs)
    End If

    If result = 0 Then ' error
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiAttrsSet = ApiLastError
End Function
Public Function ApiFileRecycle(Filename As String) As Boolean
    Dim SHFOSA As SHFILEOPSTRUCTA
    Dim SHFOSW As SHFILEOPSTRUCTW
    Dim uba() As Byte

    If Not InitDone Then ApiInit
    If Not InitShell32Done Then ApiInitShell32

    If Right(Filename, 1) = "\" Then
        ApiFileRecycle = ApiFolderRecycle(Filename)
        Exit Function
    End If

    If SHFOW_OK Then ' unicode
        SHFOSW.wFunc = FO_DELETE
        uba = UnicodeFid(Filename & vbNullChar)
        SHFOSW.pFrom = VarPtr(uba(0))
        SHFOSW.pTo = 0
        SHFOSW.fFlags = FOF_RENAMEONCOLLISION Or FOF_FILESONLY Or FOF_ALLOWUNDO Or FOF_NOERRORUI Or FOF_NORECURSION Or FOF_SILENT Or FOF_NOCONFIRMATION Or FOF_NO_CONNECTED_ELEMENTS
        ApiLastError = SHFileOperationW(SHFOSW)
        If ApiLastError = 0 Then
            ApiFileRecycle = True ' successful
        Else
            ApiFileRecycle = False
        End If
    Else
        SHFOSA.wFunc = FO_DELETE
        SHFOSA.pFrom = Filename & vbNullChar & vbNullChar
        SHFOSA.pTo = vbNull
        SHFOSA.fFlags = FOF_FILESONLY Or FOF_ALLOWUNDO Or FOF_NOERRORUI Or FOF_NORECURSION Or FOF_SILENT Or FOF_NOCONFIRMATION Or FOF_NO_CONNECTED_ELEMENTS
        ApiLastError = SHFileOperationA(SHFOSA)
        If ApiLastError = 0 Then
            ApiFileRecycle = True
        Else
            ApiFileRecycle = False
        End If
    End If
End Function
Public Function ApiFolderRecycle(FolderName As String) As Boolean
    Dim SHFOSA As SHFILEOPSTRUCTA
    Dim SHFOSW As SHFILEOPSTRUCTW
    Dim uba() As Byte
    Dim s As String
    Dim result As Long

    ' Note: Won't delete non-empty folders. Windows doesn't give failure result when you try.

    If Not InitDone Then ApiInit
    If Not InitShell32Done Then ApiInitShell32

    If Len(FolderName) <= 0 Then Exit Function

    If ApiFolderIsRoot(FolderName) Then ' don't delete root folders
        ApiLastError = -1
        ApiFolderRecycle = ApiLastError
        Exit Function
    End If

    result = ApiAttrsSet(FolderName, PT_ATTR_NORMAL)
    If Right(FolderName, 1) = "\" Then
        s = Left(FolderName, Len(FolderName) - 1)
    Else
        s = FolderName
    End If

    If SHFOW_OK Then
        SHFOSW.wFunc = FO_DELETE
        uba = UnicodeFid(s & vbNullChar)
        SHFOSW.pFrom = VarPtr(uba(0))
        SHFOSW.pTo = 0
        SHFOSW.fFlags = FOF_RENAMEONCOLLISION Or FOF_ALLOWUNDO Or FOF_NOERRORUI Or FOF_NORECURSION Or FOF_SILENT Or FOF_NOCONFIRMATION Or FOF_NO_CONNECTED_ELEMENTS
        ApiLastError = SHFileOperationW(SHFOSW)
        If ApiLastError = 0 Then
            ApiFolderRecycle = True
        Else
            ApiFolderRecycle = False
        End If
    Else
        SHFOSA.wFunc = FO_DELETE
        SHFOSA.pFrom = s & vbNullChar & vbNullChar
        SHFOSA.pTo = vbNull
        SHFOSA.fFlags = FOF_FILESONLY Or FOF_ALLOWUNDO Or FOF_NOERRORUI Or FOF_NORECURSION Or FOF_SILENT Or FOF_NOCONFIRMATION Or FOF_NO_CONNECTED_ELEMENTS
        ApiLastError = SHFileOperationA(SHFOSA)
        If ApiLastError = 0 Then
            ApiFolderRecycle = True
        Else
            ApiFolderRecycle = False
        End If
    End If
End Function
Private Sub ParseFID(Fid As String, ByRef fi As PT_FILE_INFO)
    Dim i As Long
    Dim Sep As String

    If StrComp("ftp://", Left(Fid, 6), vbTextCompare) = 0 Then
        fi.Protocol = PT_PROTO_FTP
        Sep = "/"
    ElseIf Left(Fid, 2) = "\\" Then
        fi.Protocol = PT_PROTO_UNC
        Sep = "\"
    Else
        fi.Protocol = PT_PROTO_LOCAL
        Sep = "\"
    End If

    i = InStrRev(Fid, Sep, Len(Fid) - 1)
    If fi.Protocol = PT_PROTO_LOCAL Then
        If i > 0 Then
            fi.Folder = Left(Fid, i)
        Else
            fi.Folder = ""
        End If
    ElseIf fi.Protocol = PT_PROTO_UNC Then
        If i > 2 Then
            fi.Folder = Left(Fid, i)
        Else
            fi.Folder = ""
        End If
    End If
End Sub
Private Sub ParseFileNames(ByRef fi As PT_FILE_INFO)
    Dim s As String
    Dim i As Long
    Dim uba() As Byte
    Dim buf As String
    Dim buflen As Long
    Dim Unicode As String
    Dim result As Long

    If Len(fi.Name) <= 0 Then ' find name
        i = InStrRev(fi.FullName, "\", Len(fi.FullName) - 1)
        fi.Name = Mid(fi.FullName, i + 1)
        If Right(fi.Name, 1) = "\" Then fi.Name = Left(fi.Name, Len(fi.Name) - 1)
    End If

    i = InStrRev(fi.Name, ".")
    If i > 0 Then
        fi.Extension = Mid(fi.Name, i + 1)
    Else
        fi.Extension = ""
    End If

    fi.FullName = fi.Folder & fi.Name
    If Len(fi.AltName) <= 0 Then
        If GSPNW_OK Then
            uba = UnicodeFid(fi.FullName)
            result = GetShortPathNameW(uba(0), uba(0), buflen)
            Unicode = uba
            s = ApiTextStrip(Unicode)
            i = InStrRev(s, "\", Len(s) - 1)
            If i > 0 Then s = Mid(s, i + 1)
            fi.AltName = s
        ElseIf GSPNA_OK Then
            buf = String(Len(fi.FullName) + 1, vbNullChar)
            result = GetShortPathNameA(fi.FullName, buf, buflen)
            fi.AltName = ApiTextStrip(buf)
        Else
            fi.AltName = fi.Name
        End If
    Else
'        If InStr(1, fi.AltName, "?") > 0 Then Stop
    End If
End Sub
Private Sub ParseFileAttrs(ByRef fi As PT_FILE_INFO)
    If (fi.Attrs And PT_ATTR_DIRECTORY) = 0 Then
        fi.IsFolder = False
        fi.IsFile = True
    Else
        fi.IsFolder = True
        fi.IsFile = False
    End If
End Sub
Private Sub ParseFileTime(ByRef ft As FILETIME, ByRef pft As PT_FILE_TIME)
    Dim st As SYSTEMTIME
    Dim li As LARGE_INTEGER

    On Error Resume Next
    FileTimeToSystemTime ft, st
    pft.Time = DateSerial(st.wYear, st.wMonth, st.wDay) + TimeSerial(st.wHour, st.wMinute, st.wSecond)
    If Err.Number <> 0 Then
        pft.Invalid = True
        Err.Clear
    Else
        pft.Invalid = False
    End If
    li.highpart = ft.dwHighDateTime
    li.lowpart = ft.dwLowDateTime
    pft.Time64 = ApiConvLargeIntDec(li)
End Sub
Private Sub ParseFileSizes(nFileSizeLow As Long, nFileSizeHigh As Long, ByRef fi As PT_FILE_INFO)
    Dim li As LARGE_INTEGER

    If (fi.Attrs And PT_ATTR_DIRECTORY) <> 0 Then
        fi.Size = 0
        fi.Space = 0
    Else
        li.highpart = nFileSizeHigh
        li.lowpart = nFileSizeLow
        fi.Size = ApiConvLargeIntDec(li)
        If (fi.Attrs And PT_ATTR_COMPRESSED) <> 0 Then
            fi.Space = ApiFileSpace(fi)
        Else
            fi.Space = fi.Size
        End If
    End If
End Sub
Public Function ApiFindFirstFile(FindMask As String, ByRef fd As PT_FILE_INFO) As Long
    Dim uba() As Byte
    Dim Unicode As String
    Dim fda As FIND_DATAA
    Dim fdw As FIND_DATAW
    Dim li As LARGE_INTEGER
    Dim result As Long
    Dim i As Long

    If Not InitDone Then ApiInit

    fd.FindMask = FindMask

    ParseFID fd.FindMask, fd
    If fd.Protocol = PT_PROTO_FTP Then
        fd.Continue = False
        fd.ErrorNum = 0
        fd.Error = False
        ApiFindFirstFile = 0
        Exit Function
    End If

    If FFFW_OK Then
        uba = UnicodeFid(FindMask)
        result = FindFirstFileW(uba(0), fdw)
        If result <> INVALID_HANDLE_VALUE Then
            Unicode = fdw.cFileName
            fd.Name = ApiTextStrip(Unicode)
            Unicode = fdw.cAlternateFileName
            fd.AltName = ApiTextStrip(Unicode)
            ParseFileNames fd
            fd.Attrs = fdw.dwFileAttributes
            ParseFileAttrs fd
            ParseFileTime fdw.ftCreationTime, fd.Creation
            ParseFileTime fdw.ftLastAccessTime, fd.LastAccess
            ParseFileTime fdw.ftLastWriteTime, fd.LastWrite
            ParseFileSizes fdw.nFileSizeLow, fdw.nFileSizeHigh, fd
        End If
    Else
        result = FindFirstFileA(FindMask, fda)
        If result <> INVALID_HANDLE_VALUE Then
            fd.Name = ApiTextStrip(fda.cFileName)
            fd.AltName = ApiTextStrip(fda.cAlternate)
            ParseFileNames fd
            fd.Attrs = fda.dwFileAttributes
            ParseFileAttrs fd
            ParseFileTime fda.ftCreationTime, fd.Creation
            ParseFileTime fda.ftLastAccessTime, fd.LastAccess
            ParseFileTime fda.ftLastWriteTime, fd.LastWrite
            ParseFileSizes fda.nFileSizeLow, fda.nFileSizeHigh, fd
        End If
    End If

    If result = INVALID_HANDLE_VALUE Then
        SetApiError
        fd.Continue = False
        fd.Handle = 0
        If ApiLastError = ERROR_NO_MORE_FILES Then
            fd.Error = False
            fd.ErrorNum = 0
        Else
            fd.Error = True
            fd.ErrorNum = ApiLastError
        End If
    Else
        ApiLastError = 0
        fd.Continue = True
        fd.Error = False
        fd.ErrorNum = 0
    End If

    fd.Handle = result
    ApiFindFirstFile = result
End Function
Public Function ApiFindNextFile(ByRef fd As PT_FILE_INFO) As Long
    Dim Unicode As String
    Dim fda As FIND_DATAA
    Dim fdw As FIND_DATAW
    Dim st As SYSTEMTIME
    Dim li As LARGE_INTEGER
    Dim result As Long

    If Not InitDone Then ApiInit

    If FNFW_OK Then
        result = FindNextFileW(fd.Handle, fdw)
        If result <> 0 Then
            Unicode = fdw.cFileName
            fd.Name = ApiTextStrip(Unicode)
            Unicode = fdw.cAlternateFileName
            fd.AltName = ApiTextStrip(Unicode)
            ParseFileNames fd
            fd.Attrs = fdw.dwFileAttributes
            ParseFileAttrs fd
            ParseFileTime fdw.ftCreationTime, fd.Creation
            ParseFileTime fdw.ftLastAccessTime, fd.LastAccess
            ParseFileTime fdw.ftLastWriteTime, fd.LastWrite
            ParseFileSizes fdw.nFileSizeLow, fdw.nFileSizeHigh, fd
        End If
    Else
        result = FindNextFileA(fd.Handle, fda)
        If result <> 0 Then
            fd.Name = ApiTextStrip(fda.cFileName)
            fd.AltName = ApiTextStrip(fda.cAlternate)
            ParseFileNames fd
            fd.Attrs = fda.dwFileAttributes
            ParseFileAttrs fd
            ParseFileTime fda.ftCreationTime, fd.Creation
            ParseFileTime fda.ftLastAccessTime, fd.LastAccess
            ParseFileTime fda.ftLastWriteTime, fd.LastWrite
            ParseFileSizes fda.nFileSizeLow, fda.nFileSizeHigh, fd
        End If
    End If

    If result = 0 Then
        SetApiError
        fd.Continue = False
        If ApiLastError = ERROR_NO_MORE_FILES Then
            fd.Error = False
            fd.ErrorNum = 0
        Else
            fd.Error = True
            fd.ErrorNum = ApiLastError
        End If
        ApiFindClose fd
        fd.Handle = 0
    Else
        ApiLastError = 0
        fd.Continue = True
    End If

    'fd.FindHandle = result
    ApiFindNextFile = result
End Function
Public Function ApiFindClose(ByRef fd As PT_FILE_INFO) As Boolean
    Dim result As Long

    If (fd.Handle <> 0) And (fd.Handle <> INVALID_HANDLE_VALUE) Then
        result = FindClose(fd.Handle)
    Else
        result = 1
    End If

    If result = 0 Then
        SetApiError
    Else
        ApiLastError = 0
    End If

    fd.Continue = False
    fd.Handle = INVALID_HANDLE_VALUE
    ApiFindClose = CBool(result)
End Function
Public Function ApiTextMatch(Target As String, Filters As String) As Boolean
    Dim FFs() As String
    Dim ff As Variant

    If Len(Trim(Filters)) <= 0 Then
        ApiTextMatch = True
        Exit Function
    End If

    FFs = Split(Filters, ";")
    For Each ff In FFs
        If ff = "*.*" Then ff = "*" ' treat *.* as a special case
        If Target Like ff Then
            ApiTextMatch = True
            Exit Function
        End If
    Next ff

    ApiTextMatch = False
End Function
Public Function ApiFileExists(Filename As String) As Boolean
    Dim fh As Long
    Dim fd As PT_FILE_INFO
    Dim Attrs As PT_ATTR

    ApiFileExists = False

    Attrs = ApiAttrsGet(Filename)
    If Attrs <> PT_ATTR_INVALID Then
        If (Attrs And PT_ATTR_DIRECTORY) = 0 Then
            ApiFileExists = True
            Exit Function
        End If
    End If

    fh = ApiFindFirstFile(Filename, fd)
    If fh <> INVALID_HANDLE_VALUE Then
        Attrs = fd.Attrs
        If (Attrs And PT_ATTR_DIRECTORY) = 0 Then
            ApiFileExists = True
        End If
        FindClose fh
    End If

    ApiLastError = 0
End Function
Public Function ApiFolderShortName(Folder As String) As String
    ApiFolderShortName = ApiFileShortName(Folder)
End Function
Public Function ApiFileShortName(Fid As String) As String
    Dim s As String
    Dim suba() As Byte
    Dim luba() As Byte
    Dim result As Long

    If Not InitDone Then ApiInit

    If GSPNW_OK Then
        luba = UnicodeFid(Fid)
        ReDim suba(MAX_PATHW)
        result = GetShortPathNameW(luba(0), suba(0), UBound(suba))
        s = suba
        s = ApiTextStrip(s)
    ElseIf GSPNA_OK Then
        s = String(MAX_PATH + 1, vbNullChar)
        result = GetShortPathNameA(Fid, s, Len(s))
        s = ApiTextStrip(s)
    Else ' can't get short file names from long
        s = Fid
        result = Len(Fid)
    End If

    ApiFileShortName = s

    If result = 0 Then
        SetApiError
    Else
        ApiLastError = 0
    End If
End Function
Public Function ApiFolderLongName(Folder As String) As String
    ApiFolderLongName = LongName(Folder)
End Function
Public Function ApiFileLongName(Fid As String) As String
    ApiFileLongName = LongName(Fid)
End Function
Private Function LongName(Fid As String) As String
    Dim s As String
    Dim suba() As Byte
    Dim luba() As Byte
    Dim result As Long
    Dim i As Long
    Dim sa() As String

    If Not InitDone Then ApiInit

    If GLPNW_OK Then
        suba = UnicodeFid(Fid)
        ReDim luba(MAX_PATHW)
        result = GetLongPathNameW(suba(0), luba(0), UBound(luba))
        If result = 0 Then
            s = Fid
        Else
            s = luba
            s = ApiTextStrip(s)
        End If
        If Left(s, Len(UNICODE_LOCAL_PREFIX)) = UNICODE_LOCAL_PREFIX Then
            s = Mid(s, Len(UNICODE_LOCAL_PREFIX) + 1)
        ElseIf Left(s, Len(UNICODE_UNC_PREFIX)) = UNICODE_UNC_PREFIX Then
            s = Mid(s, Len(UNICODE_UNC_PREFIX) + 1)
        End If
    ElseIf GLPNA_OK Then
        s = String(MAX_PATH + 1, vbNullChar)
        result = GetLongPathNameA(Fid, s, Len(s))
        s = ApiTextStrip(s)
    Else ' can't get long file names from short
        If InStr(1, Fid, "\") <= 0 Then
            s = Fid
        Else
            sa = Split(Fid, "\")
            If Left(Fid, 2) = "\\" Then
                i = 4
                If UBound(sa) >= 3 Then
                    s = "\\" & sa(2) & "\" & sa(3)
                Else
                    s = Fid
                End If
            Else
                i = 1
                s = sa(0)
            End If
            For i = i To UBound(sa)
                s = LengthenName(s & "\" & sa(i))
            Next i
        End If
        result = Len(s)
    End If

    LongName = s

    If result = 0 Then
        SetApiError
    Else
        ApiLastError = 0
    End If
End Function
Private Function LengthenName(Fid As String) As String
    Dim fd As PT_FILE_INFO
    Dim i As Long
    Dim s As String

    i = InStrRev(Fid, "\")
    If i <= 0 Then
        LengthenName = Fid
        Exit Function
    End If

    ApiFindFirstFile Fid, fd
'    If fd.Continue Then
    
    If fd.Continue Then
        s = Left(Fid, i) & fd.Name
        LengthenName = s
        ApiFindClose fd
    Else
        LengthenName = Fid
    End If
End Function
Public Function ApiFileSpace(ByRef fi As PT_FILE_INFO) As Variant
    Dim fh As Long
    Dim result As Long
    Dim li As LARGE_INTEGER
    Dim uba() As Byte

    If Not InitDone Then ApiInit

    fi.Space = fi.Size

    If GCFSW_OK Then
        uba = UnicodeFid(fi.FullName)
        li.lowpart = GetCompressedFileSizeW(uba(0), li.highpart)
        If li.lowpart = INVALID_FILE_SIZE Then
            result = GetLastError()
            If result = 120 Then ' unicode layer bug?
                GCFSW_OK = False
                fi.Space = ApiFileSpace(fi)
            End If
        Else
            fi.Space = ApiConvLargeIntDec(li)
        End If
    ElseIf GCFSA_OK Then
        li.lowpart = GetCompressedFileSizeA(fi.FullName, li.highpart)
        If li.lowpart = INVALID_FILE_SIZE Then
            result = GetLastError()
            If result = 0 Then
                fi.Space = ApiConvLargeIntDec(li)
            End If
        Else
            fi.Space = ApiConvLargeIntDec(li)
        End If
    End If

    ApiFileSpace = fi.Space
End Function
#If API_FILE_INFO Then
Public Function ApiFileInfo(Fid As String) As PT_FILE_INFO
    Dim uba() As Byte
    Dim result As Long
    Dim fad As FILE_ATTRIBUTE_DATA
    Dim fi As PT_FILE_INFO
    Dim st As SYSTEMTIME
    Dim li As LARGE_INTEGER
    Dim fh As Long
    Dim bhfi As BY_HANDLE_FILE_INFORMATION
    Dim i As Long

    If Not InitDone Then ApiInit

    fi.FullName = Fid
    fi.FindMask = fi.FullName

    ParseFID Fid, fi

    If fi.Protocol = PT_PROTO_FTP Then
        fi.ErrorNum = 0
        fi.Error = False
        ApiFileInfo = fi
        Exit Function
    End If

    fi.Creation.Time64 = -1
    fi.Creation.Invalid = True
    fi.LastAccess.Time64 = -1
    fi.LastAccess.Invalid = True
    fi.LastWrite.Time64 = -1
    fi.LastWrite.Invalid = True
    fi.Size = INVALID_FILE_SIZE
    fi.Space = ApiFileSpace(fi)
    fi.AltName = ""

    If GFAXW_OK Then
        uba = UnicodeFid(Fid)
        result = GetFileAttributesExW(uba(0), GetFileExInfoStandard, fad)
        If result = 0 Then
            SetApiError
            fi.ErrorNum = ApiLastError
            fi.Error = True
        Else
            fi.ErrorNum = 0
            fi.Error = False

            ParseFileNames fi
            fi.Attrs = fad.dwFileAttributes
            ParseFileAttrs fi
            ParseFileTime fad.ftCreationTime, fi.Creation
            ParseFileTime fad.ftLastAccessTime, fi.LastAccess
            ParseFileTime fad.ftLastWriteTime, fi.LastWrite
            ParseFileSizes fad.nFileSizeLow, fad.nFileSizeHigh, fi
        End If
    ElseIf GFAXA_OK Then
        result = GetFileAttributesExA(Fid, GetFileExInfoStandard, fad)
        If result = 0 Then
            SetApiError
            fi.ErrorNum = ApiLastError
            fi.Error = True
        Else
            fi.ErrorNum = 0
            fi.Error = False

            ParseFileNames fi
            fi.Attrs = fad.dwFileAttributes
            ParseFileAttrs fi
            ParseFileTime fad.ftCreationTime, fi.Creation
            ParseFileTime fad.ftLastAccessTime, fi.LastAccess
            ParseFileTime fad.ftLastWriteTime, fi.LastWrite
            ParseFileSizes fad.nFileSizeLow, fad.nFileSizeHigh, fi
        End If
    Else
        fh = ApiFileOpen(Fid, PT_ACCESS_NONE, PT_ACCESS_MAX, PT_ATTR_NORMAL, False, True, False)
        If fh = INVALID_HANDLE_VALUE Then
            SetApiError
            fi.ErrorNum = ApiLastError
            fi.Error = True
        Else
            result = GetFileInformationByHandle(fh, bhfi)
            If result = 0 Then
                SetApiError
                fi.ErrorNum = ApiLastError
                fi.Error = True
            Else
                fi.ErrorNum = 0
                fi.Error = False
                ApiFileClose fh

                ParseFileNames fi
                fi.Attrs = bhfi.dwFileAttributes
                ParseFileAttrs fi
                ParseFileTime bhfi.ftCreationTime, fi.Creation
                ParseFileTime bhfi.ftLastAccessTime, fi.LastAccess
                ParseFileTime bhfi.ftLastWriteTime, fi.LastWrite
                ParseFileSizes bhfi.nFileSizeLow, bhfi.nFileSizeHigh, fi
            End If
        End If
    End If

    ApiLastError = fi.ErrorNum
    ApiFileInfo = fi
End Function
Public Function ApiFileSize(Fid As String) As Variant
    Dim fh As Long
    Dim fsHigh As Long
    Dim fi As BY_HANDLE_FILE_INFORMATION
    Dim result As Long
    Dim li As LARGE_INTEGER

    If Not InitDone Then ApiInit

    ApiFileSize = INVALID_FILE_SIZE

    On Error Resume Next
    fh = ApiFileOpen(Fid, PT_ACCESS_NONE, PT_ACCESS_MAX, PT_ATTR_NORMAL, False, True, False)
    If fh = INVALID_HANDLE_VALUE Then
        SetApiError
        Exit Function
    End If

    If GFSX_OK Then
        result = GetFileSizeEx(fh, li)
        If result = 0 Then
            SetApiError
            Exit Function
        End If
    Else
        li.lowpart = GetFileSize(fh, fsHigh)
        li.highpart = fsHigh
        If li.lowpart = INVALID_FILE_SIZE Then
            SetApiError
            Exit Function
        End If
    End If

    ApiFileClose fh
    ApiLastError = 0
    ApiFileSize = ApiConvLargeIntDec(li)
End Function
Private Function ApiFileActualSize(Fid As String) As Variant
    Dim fh As Long
    Dim sa As SECURITY_ATTRIBUTES
    Dim sid As WIN32_STREAM_ID
    Dim buf() As Byte
    Dim buflen As Long
    Dim ReadContext As Long
    Dim result As Long
    Dim Seeked As LARGE_INTEGER
    Dim s As String
    Dim ul As LARGE_INTEGER
    Dim TotalSize As Variant
    Dim uba() As Byte

    If Not InitDone Then ApiInit

    If Not BACKUP_OK Then
        ApiFileActualSize = ApiFileSize(Fid)
        Exit Function
    End If

    sa.nLength = 0
    If CFW_OK Then
        uba = UnicodeFid(Fid)
        fh = CreateFileW(uba(0), GENERIC_READ, FILE_SHARE_READ Or FILE_SHARE_WRITE, sa, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS Or FILE_FLAG_SEQUENTIAL_SCAN, 0)
    Else
        fh = CreateFileA(Fid, GENERIC_READ, FILE_SHARE_READ Or FILE_SHARE_WRITE, sa, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS Or FILE_FLAG_SEQUENTIAL_SCAN, 0)
    End If
    If fh = INVALID_HANDLE_VALUE Then
        SetApiError
        ApiFileActualSize = INVALID_FILE_SIZE
        Exit Function
    End If

    TotalSize = CDec(0)
    ReDim buf(API_BUFSIZ)
    ReadContext = 0
    result = BackupRead(fh, buf(0), LenB(sid), buflen, False, False, ReadContext)
    While (result <> 0) And (buflen >= LenB(sid))
        ApiMemoryCopy VarPtr(sid), VarPtr(buf(0)), LenB(sid)
        ul.highpart = sid.dwStreamSizeHigh
        ul.lowpart = sid.dwStreamSizeLow
        TotalSize = TotalSize + ApiConvLargeIntDec(ul)
        result = BackupSeek(fh, ul.lowpart, ul.highpart, Seeked.lowpart, Seeked.highpart, ReadContext)
        result = BackupRead(fh, buf(0), LenB(sid), buflen, False, True, ReadContext)
    Wend
    result = BackupRead(fh, buf(0), LenB(sid), buflen, False, False, ReadContext)
    CloseHandle fh

    ApiFileActualSize = TotalSize
    ApiLastError = 0
End Function
#End If ' API_FILE_INFO
Public Function ApiFolderExists(FolderName As String, Optional ByRef dirTest As DirListBox) As Boolean
    Dim Attrs As PT_ATTR
    Dim fh As Long
    Dim fd As PT_FILE_INFO
    Dim s As String

    ApiLastError = 0
    ApiFolderExists = False

    s = ApiFolderFix(FolderName)
    If Len(s) <= 0 Then Exit Function

    On Error Resume Next
    Attrs = ApiAttrsGet(FolderName)
    If Attrs <> PT_ATTR_INVALID Then
        If (Attrs And PT_ATTR_DIRECTORY) <> 0 Then
            ApiFolderExists = True
            ApiLastError = 0
            Exit Function
        End If
    End If

    fh = ApiFindFirstFile(s & "*.*", fd)
    If fh <> INVALID_HANDLE_VALUE Then
        ApiFolderExists = True
        FindClose fh
        ApiLastError = 0
        Exit Function
    End If

    ' could be root folder of local drive?
    If ApiFileExists(s & "nul") Then
        ApiFolderExists = True
        ApiLastError = 0
        Exit Function
    End If
    ApiLastError = 0

    If dirTest Is Nothing Then Exit Function

    ' might be network share
    DoEvents ' this can take a few seconds if server is off-line
    On Error Resume Next
    dirTest.Path = s
    dirTest.Refresh
    If Err = 0 Then
        ApiFolderExists = True
    Else
        ApiFolderExists = False
    End If
    Err.Clear

    ApiLastError = 0
End Function
Public Function ApiFolderCurrent() As String
    Dim uba() As Byte
    Dim buf As String
    Dim result As Long

    If Not InitDone Then ApiInit

    If GCDW_OK Then
        ReDim uba(MAX_PATH * 2)
        result = GetCurrentDirectoryW(MAX_PATH, uba(0))
        buf = uba
        ApiFolderCurrent = ApiTextStrip(buf)
    Else
        buf = String(MAX_PATH, Chr(0))
        result = GetCurrentDirectoryA(MAX_PATH, buf)
        ApiFolderCurrent = ApiTextStrip(buf)
    End If

    If result = 0 Then
        SetApiError
    Else
        ApiLastError = 0
    End If
End Function
Public Function ApiFolderOpen(Fid As String, Optional Attrs As PT_ATTR = PT_ATTR_NORMAL) As Long
    Dim uba() As Byte
    Dim fh As Long
    Dim sa As SECURITY_ATTRIBUTES
    Dim DesiredAccess As DESIRED_ACCESS
    Dim ShareMode As FILE_SHARE
    Dim FlagsAndAttrs As FILE_FLAG_AND_ATTR
    Dim CreateDisposition As FILE_CREATION

    If Not InitDone Then ApiInit

    DesiredAccess = FILE_GENERIC_READ
    ShareMode = FILE_SHARE_READ Or FILE_SHARE_WRITE
    CreateDisposition = OPEN_EXISTING
    FlagsAndAttrs = FILE_FLAG_BACKUP_SEMANTICS

    If CFW_OK Then
        uba = UnicodeFid(Fid)
        fh = CreateFileW(uba(0), DesiredAccess, ShareMode, sa, CreateDisposition, FlagsAndAttrs, 0)
    Else
        fh = CreateFileA(Fid, DesiredAccess, ShareMode, sa, CreateDisposition, FlagsAndAttrs, 0)
    End If

    If fh = INVALID_HANDLE_VALUE Then
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFolderOpen = fh
End Function
Public Function ApiFolderRead(fh As Long, ByRef buf() As Byte, buflen As Long, ByRef BytesRead As Long) As Long
    Dim result As Long

    result = ReadFile(fh, buf(0), buflen, BytesRead, 0)

    If result = 0 Then ' failure
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFolderRead = ApiLastError
End Function
Public Function ApiFolderClose(fh As Long) As Long
    Dim result As Long

    result = CloseHandle(fh)

    If result = 0 Then ' failure
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFolderClose = ApiLastError
End Function
Public Function ApiFolderCreate(Fid As String, Optional Attrs As PT_ATTR = PT_ATTR_NORMAL) As Boolean
    Dim uba() As Byte
    Dim sa As SECURITY_ATTRIBUTES
    Dim result As Long

    If Not InitDone Then ApiInit

    sa.nLength = 0
    If CFW_OK Then
        uba = UnicodeFid(Fid)
        result = CreateDirectoryW(uba(0), sa)
    Else
        result = CreateDirectoryA(Fid, sa)
    End If

    If result = 0 Then ' error
        SetApiError
        If ApiLastError <> 183 Then ' already exists
            ApiFolderCreate = False
            Exit Function
        End If
    End If

    ApiAttrsSet Fid, Attrs
    ApiLastError = 0
    ApiFolderCreate = True
End Function
Private Function ApiFunctionAddr(ByVal lngFnPtr As Long) As Long
   ApiFunctionAddr = lngFnPtr
End Function
#If API_SELECT_FOLDER Then
Public Function ApiFolderSelect(Title As String, CurrentDir As String) As String

    If Not InitDone Then ApiInit
    If Not InitShell32Done Then ApiInitShell32

    If SHBFFW_OK Then
        ApiFolderSelect = SelectFolderW(Title, CurrentDir)
    Else
        ApiFolderSelect = SelectFolderA(Title, CurrentDir)
    End If
End Function
Private Function SelectFolderA(Title As String, CurrentDir As String) As String
    Dim idl As Long
    Dim buf() As Byte
    Dim bi As BROWSEINFOA
    Dim uba() As Byte
    Dim Unicode As String

    gCurrentDir = Trim(CurrentDir)
    ReDim buf(MAX_PATH + 1)

    bi.hwndOwner = 0
    uba = StrConv(Title & Chr(0), vbFromUnicode)
    bi.lpszTitle = VarPtr(uba(0))
    buf = String(MAX_PATH + 1, Chr(0))
    bi.pszDisplayName = VarPtr(buf(0)) ' lstrcatA("dn", "")
    bi.pidlRoot = 0 ' BIF_USENEWUI Or BIF_EDITBOX Or
    bi.ulFlags = BIF_EDITBOX Or BIF_STATUSTEXT Or BIF_VALIDATE Or BIF_RETURNONLYFSDIRS   'Or BIF_DONTGOBELOWDOMAIN
    bi.lpfn = ApiFunctionAddr(AddressOf SelectFolderCallBackA)
    bi.lParam = 0

    idl = SHBrowseForFolderA(bi)

    SelectFolderA = CurrentDir
    If idl <> 0 Then
        Unicode = String(MAX_PATH + 1, Chr(0))
        SHGetPathFromIDListA idl, Unicode
        Unicode = Left(Unicode, InStr(Unicode, Chr(0)) - 1)
        Unicode = ApiFolderFix(Unicode)
        If Len(Unicode) <= 0 Then ' \\server specified?
            ApiBeep
        Else
            SelectFolderA = Unicode
        End If
        CoTaskMemFree idl
    End If
End Function
Private Function SelectFolderW(Title As String, CurrentDir As String) As String
    Dim idl As Long
    Dim buf() As Byte
    Dim bi As BROWSEINFOW
    Dim uba() As Byte
    Dim Unicode As String
    Dim dn() As Byte
    Dim ucTitle() As Byte

    gCurrentDir = Trim(CurrentDir)
    uba = CurrentDir & Chr(0)

    bi.hwndOwner = 0 ' frm.hwnd
    ucTitle = Title & Chr(0)
    bi.lpszTitle = VarPtr(ucTitle(0)) ' lstrcatW(StrConv(Title, vbUnicode), StrConv(Chr(0), vbUnicode))   ' get pointer to Title
    ReDim dn(MAX_PATHW)
    bi.pszDisplayName = VarPtr(dn(0)) '  lstrcatW(StrConv(CurrentDir, vbUnicode), StrConv(Chr(0), vbUnicode))
    bi.pidlRoot = 0 ' BIF_USENEWUI Or BIF_EDITBOX Or
    bi.ulFlags = BIF_USENEWUI Or BIF_EDITBOX Or BIF_STATUSTEXT Or BIF_VALIDATE Or BIF_RETURNONLYFSDIRS   'Or BIF_DONTGOBELOWDOMAIN
    bi.lpfn = ApiFunctionAddr(AddressOf SelectFolderCallBackW)
    bi.iImage = 0
    bi.lParam = 0

    idl = SHBrowseForFolderW(bi)

    SelectFolderW = CurrentDir
    If idl <> 0 Then
        ReDim buf(MAX_PATHW)
        SHGetPathFromIDListW idl, buf(0)
        Unicode = buf
        Unicode = Left(Unicode, InStr(Unicode, Chr(0)) - 1)
        Unicode = ApiFolderFix(Unicode)
        If Len(Unicode) <= 0 Then ' \\server specified?
            SelectFolderW = ""
        Else
            SelectFolderW = Unicode
        End If
        CoTaskMemFree idl
    End If
End Function
Private Function SelectFolderCallBackA(ByVal hWnd As Long, _
    ByVal uMsg As Long, ByVal lParam As Long, lpData As Long) As Long
    Dim buf As String
    Dim s As String
    Dim i As Long
    Dim uba() As Byte

    SelectFolderCallBackA = 0
    Select Case uMsg
        Case BFFM_INITIALIZED:
            uba = StrConv(Trim(gCurrentDir) & Chr(0), vbFromUnicode)
            SendMessageA hWnd, BFFM_SETSELECTIONA, True, VarPtr(uba(0))
            SendMessageA hWnd, BFFM_SETEXPANDED, True, VarPtr(uba(0))
        Case BFFM_SELCHANGED:
            buf = Space(MAX_PATH)
            SHGetPathFromIDListA lParam, buf
            buf = Trim(ApiTextStrip(buf))
            If Len(buf) = 0 Then
                SendMessageA hWnd, BFFM_ENABLEOK, 0, 0
                uba = Chr(0)
                SendMessageA hWnd, BFFM_SETSTATUSTEXTA, 0, VarPtr(uba(0))
                Exit Function
            End If
            If Not ApiFolderExists(buf) Then
                SendMessageA hWnd, BFFM_ENABLEOK, 0, 0
                uba = StrConv("Folder Not Accessible" & Chr(0), vbFromUnicode)
                SendMessageA hWnd, BFFM_SETSTATUSTEXTA, 0, VarPtr(uba(0))
            Else
                i = InStrRev(buf, "\")
                If i > 0 Then
                    s = Mid(buf, i + 1)
                End If
                If Len(s) <= 0 Then s = buf

                s = LCase(Left(s, 4))
                If (s = "www.") Or (s = "ftp.") Then
                    SendMessageA hWnd, BFFM_ENABLEOK, 0, 0
                    uba = StrConv("Web and FTP sites are not supported by Replicator" & Chr(0), vbFromUnicode)
                    SendMessageA hWnd, BFFM_SETSTATUSTEXTA, 0, VarPtr(uba(0))
                    ApiBeep
                Else
                    SendMessageA hWnd, BFFM_ENABLEOK, 1, 1
                    uba = Chr(0)
                    SendMessageA hWnd, BFFM_SETSTATUSTEXTA, 0, uba(0)
                End If
            End If

        Case BFFM_VALIDATEFAILEDA:
            uba = StrConv("Invalid Folder" & Chr(0), vbFromUnicode)
            SendMessageA hWnd, BFFM_SETSTATUSTEXTA, 0, VarPtr(uba(0))
            SendMessageA hWnd, BFFM_ENABLEOK, 0, 0
            ApiBeep
            SelectFolderCallBackA = 1
    End Select

'    ApiFormTop hWnd
End Function
Private Function SelectFolderCallBackW(ByVal hWnd As Long, _
    ByVal uMsg As Long, ByVal lParam As Long, lpData As Long) As Long
    Dim buf() As Byte
    Dim s As String
    Dim i As Long
    Dim uba() As Byte
    Dim Unicode As String

    SelectFolderCallBackW = 0
    Select Case uMsg
        Case BFFM_INITIALIZED:
            uba = Trim(gCurrentDir) & Chr(0)
            SendMessageW hWnd, BFFM_SETSELECTIONW, True, VarPtr(uba(0))  ' lstrcatW(StrConv(buf, vbUnicode), StrConv("", vbUnicode))
            SendMessageW hWnd, BFFM_SETEXPANDED, True, VarPtr(uba(0))
        Case BFFM_SELCHANGED:
            ReDim buf(MAX_PATHW)
            SHGetPathFromIDListW lParam, buf(0)
            Unicode = buf
            Unicode = Trim(ApiTextStrip(Unicode))
            If Len(Unicode) = 0 Then
                SendMessageW hWnd, BFFM_ENABLEOK, 0, 0
                uba = Chr(0)
                SendMessageW hWnd, BFFM_SETSTATUSTEXTW, 0, VarPtr(uba(0)) ' lstrcatW(StrConv("", vbUnicode), StrConv("", vbUnicode))
                Exit Function
            End If
            If Not ApiFolderExists(Unicode) Then
                SendMessageW hWnd, BFFM_ENABLEOK, 0, 0
                uba = "Folder Not Accessible" & Chr(0)
                SendMessageW hWnd, BFFM_SETSTATUSTEXTW, 0, VarPtr(uba(0)) 'lstrcatW(StrConv("", vbUnicode), StrConv("", vbUnicode))
            Else
                i = InStrRev(Unicode, "\")
                If i > 0 Then
                    s = Mid(Unicode, i + 1)
                End If
                If Len(s) <= 0 Then s = Unicode

                s = LCase(Left(s, 4))
                If (s = "www.") Or (s = "ftp.") Then
                    uba = "Web and FTP sites are not supported by Replicator" & Chr(0)
                    SendMessageW hWnd, BFFM_SETSTATUSTEXTW, 0, VarPtr(uba(0)) ' lstrcatW(StrConv("Warning: Replicator cannot copy From/To Web or FTP sites", vbUnicode), StrConv("", vbUnicode))
                    SendMessageW hWnd, BFFM_ENABLEOK, 0, 0
                    ApiBeep
                Else
                    uba = Chr(0)
                    SendMessageW hWnd, BFFM_SETSTATUSTEXTW, 0, VarPtr(uba(0)) ' lstrcatW(StrConv("", vbUnicode), StrConv("", vbUnicode))
                    SendMessageW hWnd, BFFM_ENABLEOK, 1, 1
                End If
            End If

        Case BFFM_VALIDATEFAILEDW:
            ApiBeep
            uba = Chr(0)
            SendMessageW hWnd, BFFM_SETSTATUSTEXTW, 0, VarPtr(uba(0)) ' lstrcatW(StrConv("Invalid Folder", vbUnicode), StrConv("", vbUnicode))
            SendMessageW hWnd, BFFM_ENABLEOK, 0, 0
            SelectFolderCallBackW = 1
    End Select

'    ApiFormTop hWnd
End Function
#End If ' API_SELECT_FOLDER
Public Function ApiFileOpenInput(Fid As String)
    ApiFileOpenInput = ApiFileOpen(Fid, PT_ACCESS_READ, PT_ACCESS_MAX, PT_ATTR_NORMAL, False, True, False)
End Function
Public Function ApiFileOpenOutput(Fid As String)
    ApiFileOpenOutput = ApiFileOpen(Fid, PT_ACCESS_WRITE, PT_ACCESS_MAX, PT_ATTR_NORMAL, True, True, False)
End Function
Public Function ApiFileAppend(Fid As String, Optional OtherAccess As PT_ACCESS = PT_ACCESS_READ) As Long
    ApiFileAppend = ApiFileOpen(Fid, PT_ACCESS_WRITE, OtherAccess, PT_ATTR_NORMAL, False, True, False)
End Function
Public Function ApiFileCreate(Fid As String, Optional OtherAccess As PT_ACCESS = PT_ACCESS_READ) As Long
    ApiFileCreate = ApiFileOpen(Fid, PT_ACCESS_WRITE, OtherAccess, PT_ATTR_NORMAL, True, True, False)
End Function
Private Function ApiFileOpen(Fid As String, Optional MyAccess As PT_ACCESS = PT_ACCESS_READ, Optional OtherAccess As PT_ACCESS = PT_ACCESS_READ, Optional Attrs As PT_ATTR = PT_ATTR_NORMAL, Optional Overwrite As Boolean = False, Optional Sequential As Boolean = True, Optional Backup As Boolean = False) As Long
    Dim uba() As Byte
    Dim fh As Long
    Dim sa As SECURITY_ATTRIBUTES
    Dim DesiredAccess As DESIRED_ACCESS
    Dim ShareMode As FILE_SHARE
    Dim FlagsAndAttrs As FILE_FLAG_AND_ATTR
    Dim CreateDisposition As FILE_CREATION

    If Not InitDone Then ApiInit
' SHARE_READ AND SHARE_WRITE on source file, if it's in use.
' Otherwise, SHARE_READ only?
' support template in createdirectoryex?  How about FileOpen when creating new file?

    DesiredAccess = 0
    If MyAccess And PT_ACCESS_READ Then
        DesiredAccess = DesiredAccess Or FILE_GENERIC_READ
    End If
    If MyAccess And PT_ACCESS_WRITE Then
        DesiredAccess = DesiredAccess Or FILE_GENERIC_WRITE
    End If

    ShareMode = 0
    If OtherAccess And PT_ACCESS_READ Then
        ShareMode = ShareMode Or FILE_SHARE_READ
    End If
    If OtherAccess And PT_ACCESS_WRITE Then
        ShareMode = ShareMode Or FILE_SHARE_WRITE
    End If

    If Overwrite Then ' delete file if it already exists
        If MyAccess And PT_ACCESS_WRITE Then
            CreateDisposition = CREATE_ALWAYS
        Else
            CreateDisposition = OPEN_EXISTING
        End If
    Else
        If MyAccess And PT_ACCESS_WRITE Then
            CreateDisposition = OPEN_ALWAYS ' create if doesn't exist
        Else
            CreateDisposition = OPEN_EXISTING
        End If
    End If

    FlagsAndAttrs = 0
    FlagsAndAttrs = FlagsAndAttrs Or (Attrs And FILE_ATTRIBUTE_SETTABLE)
    If Sequential Then
        FlagsAndAttrs = FlagsAndAttrs Or FILE_FLAG_SEQUENTIAL_SCAN
    Else
        FlagsAndAttrs = FlagsAndAttrs Or FILE_FLAG_RANDOM_ACCESS
    End If

    If Backup Then
        FlagsAndAttrs = FlagsAndAttrs Or FILE_FLAG_BACKUP_SEMANTICS
        If MyAccess And PT_ACCESS_WRITE Then
            FlagsAndAttrs = FlagsAndAttrs Or WRITE_DAC Or WRITE_OWNER
        End If
    End If

'fhDest = CreateFile(dest, GENERIC_WRITE, _
'    0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)

    If CFW_OK Then
        uba = UnicodeFid(Fid)
        fh = CreateFileW(uba(0), DesiredAccess, ShareMode, sa, CreateDisposition, FlagsAndAttrs, 0)
        If fh = INVALID_HANDLE_VALUE Then
            ApiSleep 3000  ' wait 3 seconds and try again
            fh = CreateFileW(uba(0), DesiredAccess, ShareMode, sa, CreateDisposition, FlagsAndAttrs, 0)
        End If
    Else
        fh = CreateFileA(Fid, DesiredAccess, ShareMode, sa, CreateDisposition, FlagsAndAttrs, 0)
        If fh = INVALID_HANDLE_VALUE Then
            ApiSleep 3000  ' wait 3 seconds and try again
            fh = CreateFileA(Fid, DesiredAccess, ShareMode, sa, CreateDisposition, FlagsAndAttrs, 0)
        End If
    End If

    If fh = INVALID_HANDLE_VALUE Then
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFileOpen = fh
End Function
Public Function ApiFileClose(fh As Long) As Long
    Dim result As Long

    result = CloseHandle(fh)

    If result = 0 Then ' failure
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFileClose = ApiLastError
End Function
Public Function ApiFileRead(fh As Long, ByRef buf() As Byte, buflen As Long, ByRef BytesRead As Long) As Long
    Dim result As Long

    result = ReadFile(fh, buf(0), buflen, BytesRead, 0)

    If result = 0 Then ' failure
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFileRead = ApiLastError
End Function
Public Function ApiFileWrite(fh As Long, ByRef buf() As Byte, buflen As Long, ByRef BytesWritten As Long) As Long
    Dim result As Long

    result = WriteFile(fh, buf(0), buflen, BytesWritten, 0)

    If result = 0 Then ' failure
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFileWrite = ApiLastError
End Function
#If API_BACKUP Then
Private Function ApiStreamInfo(Fid As String) As PT_STREAM_INFO()
    Dim fh As Long
    Dim sa As SECURITY_ATTRIBUTES
    Dim sid As WIN32_STREAM_ID
    Dim buf() As Byte
    Dim buflen As Long
    Dim ReadContext As Long
    Dim result As Long
    Dim StreamCnt As Long
    Dim si() As PT_STREAM_INFO
    Dim Seeked As LARGE_INTEGER
    Dim s As String
    Dim ul As LARGE_INTEGER
    Dim SeekLoc As Variant

    sa.nLength = 0
    fh = CreateFileA(Fid, GENERIC_READ, 0, sa, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS Or FILE_FLAG_SEQUENTIAL_SCAN, 0)
    If fh = INVALID_HANDLE_VALUE Then
        SetApiError
        ApiStreamInfo = si
        Exit Function
    End If

    ReDim buf(API_BUFSIZ)
    ReadContext = 0
    result = BackupRead(fh, buf(0), LenB(sid), buflen, False, True, ReadContext)
    While (result <> 0) And (buflen >= LenB(sid))
        ApiMemoryCopy VarPtr(sid), VarPtr(buf(0)), LenB(sid)
        ReDim Preserve si(StreamCnt)
        si(StreamCnt).Fid = Fid
        If sid.dwStreamNameSize > 0 Then
            result = BackupRead(fh, buf(0), sid.dwStreamNameSize, buflen, False, True, ReadContext)
            buf(buflen) = 0
            s = buf
            s = Left(s, sid.dwStreamNameSize / 2) ' divide by 2 because it's unicode
        Else
            s = ""
        End If
        si(StreamCnt).Name = s
        si(StreamCnt).ID = DispStreamId(sid.dwStreamID)
        si(StreamCnt).Attrs = sid.dwStreamAttributes
        si(StreamCnt).AttrsDesc = DispBackupBlockAttr(sid.dwStreamAttributes)
        ul.highpart = sid.dwStreamSizeHigh
        ul.lowpart = sid.dwStreamSizeLow
        si(StreamCnt).Size = ApiConvLargeIntDec(ul)
        result = BackupSeek(fh, ul.lowpart, ul.highpart, Seeked.lowpart, Seeked.highpart, ReadContext)
        result = BackupRead(fh, buf(0), LenB(sid), buflen, False, True, ReadContext)
        StreamCnt = StreamCnt + 1
    Wend
    result = BackupRead(fh, buf(0), LenB(sid), buflen, False, True, ReadContext)

    If result = 0 Then ' failure
        SetApiError
    Else
        ApiLastError = 0
    End If

    CloseHandle fh

    ApiStreamInfo = si
End Function
Private Function DispStreamId(Code As BACKUP_STREAM_ID) As String
    Dim s As String

    Select Case Code
        Case BACKUP_INVALID:        s = "Invalid"
        Case BACKUP_DATA:           s = "Primary Data"
        Case BACKUP_EA_DATA:        s = "EAs"
        Case BACKUP_SECURITY_DATA:  s = "Security"
        Case BACKUP_ALTERNATE_DATA: s = "Alt Data"
        Case BACKUP_LINK:           s = "Link"
        Case BACKUP_PROPERTY_DATA:  s = "Prop Data"
        Case BACKUP_OBJECT_ID:      s = "Object ID"
        Case BACKUP_REPARSE_DATA:   s = "Reparse Data"
        Case BACKUP_SPARSE_BLOCK:   s = "Sparse Block"
        Case Else:                  s = "Unknown (" & CStr(Code) & ")"
    End Select

    DispStreamId = s
End Function
Private Function DispBackupBlockAttr(Code As BACKUP_BLOCK_ATTR) As String
    Dim s As String

    s = ""
    If Code = STREAM_NORMAL_ATTRIBUTE Then s = s & "Normal"
    If Code And STREAM_MODIFIED_WHEN_READ Then s = s & "Mod When Read"
    If Code And STREAM_CONTAINS_SECURITY Then s = s & "Security"
    If Code And STREAM_CONTAINS_PROPERTIES Then s = s & "Properties"
    If Code And STREAM_SPARSE_ATTRIBUTE Then s = s & "Sparse"
    If (Code And STREAM_VALID) <> Code Then
        s = s & " (" & CStr((Code Or STREAM_VALID) Xor STREAM_VALID) & ")"
    End If

    DispBackupBlockAttr = Trim(s)
End Function
Public Function ApiFileCopy(ByRef sfi As PT_FILE_INFO, ByRef dfi As PT_FILE_INFO, Optional frmCallBack As Form = Nothing) As PT_COPY_RESULT
    If Not InitDone Then ApiInit

    Set gFileCopyCallback = frmCallBack
    ApiAbort = False
    ApiLastError = 0

    If (sfi.Protocol = PT_PROTO_FTP) Or (dfi.Protocol = PT_PROTO_FTP) Then

    Else ' all local or unc
        'If BACKUP_OK Then
        '    ApiFileCopy = FileBackup(sfi, dfi)
        'ElseIf CFX_OK Then
        '    ApiFileCopy = FileCopyEx(sfi, dfi)
        'Else
            ApiFileCopy = FileCopyRW(sfi, dfi)
        'End If
    End If
End Function
Private Function FileCopyRW(ByRef sfi As PT_FILE_INFO, ByRef dfi As PT_FILE_INFO) As PT_COPY_RESULT
    Dim buf() As Byte
    Dim fhSrc As Long
    Dim fhDst As Long
    Dim blocks As Currency
    Dim maxblocks As Currency
    Dim fsPretty As String
    Dim Cnt As Currency
    Dim s As String
    Dim Attrs As Long
    Dim fs As Variant
    Dim fh As Long
    Dim result As Long
    Dim bcnt As Long
    Dim CallBack As Boolean

    FileCopyRW.Fail = True
    FileCopyRW.Warn = False
    FileCopyRW.msg = ""
    FileCopyRW.BytesCopied = 0

    If (gFileCopyCallback Is Nothing) Then
        CallBack = False
    Else
        CallBack = True
    End If

    fs = sfi.Size
    maxblocks = Int((fs / CCur(API_BUFSIZ)))

    If ApiAbort Then
        ApiLastError = -1
        FileCopyRW.msg = "User Abort"
        Exit Function
    End If

    DoEvents
    fhSrc = ApiFileOpenInput(sfi.FullName)
    If fhSrc = INVALID_HANDLE_VALUE Then
        FileCopyRW.msg = "Could Not Open Source File (" & ApiError(ApiLastError) & "): " & sfi.FullName
        Exit Function
    End If

    ' make room for new copy
    'If ApiFileExists(dfi.FullName) Then ApiFileDelete dfi.FullName
    ApiFileDelete dfi.FullName

    fhDst = ApiFileOpenOutput(dfi.FullName)
    If fhDst = INVALID_HANDLE_VALUE Then
        If ApiLastError <> ERROR_ALREADY_EXISTS Then
            FileCopyRW.msg = "Could Not Open Destination File (" & ApiError(ApiLastError) & "): " & dfi.FullName
            CloseHandle fhSrc
            Exit Function
        End If
    End If

    ReDim buf(API_BUFSIZ - 1)
    blocks = 0
    Cnt = 0

    If CallBack Then
        gFileCopyCallback.ApiFileCopyCallback fs, Cnt, 0, fs, Cnt, CDec(API_BUFSIZ)
    Else
        DoEvents
    End If

    Do While blocks < maxblocks
        If ApiAbort Then
            ApiLastError = -1
            FileCopyRW.msg = "User Abort"
            ApiFileClose fhSrc
            ApiFileClose fhDst
            Exit Function
        End If

        result = ReadFile(fhSrc, buf(0), API_BUFSIZ, bcnt, 0)
        If (result = 0) Or (bcnt <> API_BUFSIZ) Then
            FileCopyRW.msg = SetApiError("Could Not Read Source File", sfi.FullName)
            CloseHandle fhSrc
            CloseHandle fhDst
            Exit Function
        End If

        result = WriteFile(fhDst, buf(0), bcnt, bcnt, 0)
        If (result = 0) Or (bcnt <> API_BUFSIZ) Then
            FileCopyRW.msg = SetApiError("Could Not Write Destination File", dfi.FullName)
            CloseHandle fhSrc
            CloseHandle fhDst
            Exit Function
        End If

        blocks = blocks + 1
        Cnt = Cnt + API_BUFSIZ

        If CallBack Then
            gFileCopyCallback.ApiFileCopyCallback fs, Cnt, 0, fs, Cnt, CDec(API_BUFSIZ)
        Else
            If (blocks Mod CALLBACK_INTERVAL) = 0 Then DoEvents
        End If
    Loop

    If Cnt < fs Then ' copy tail
        bcnt = fs - Cnt
        ReDim buf(bcnt - 1)
        result = ReadFile(fhSrc, buf(0), bcnt, bcnt, 0)
        If result = 0 Then
            FileCopyRW.msg = SetApiError("Could Not Read Source File", sfi.FullName)
            CloseHandle fhSrc
            CloseHandle fhDst
            Exit Function
        End If

        result = WriteFile(fhDst, buf(0), bcnt, bcnt, 0)
        If result = 0 Then
            FileCopyRW.msg = SetApiError("Could Not Write Destination File", dfi.FullName)
            CloseHandle fhSrc
            CloseHandle fhDst
            Exit Function
        End If

        If CallBack Then
            gFileCopyCallback.ApiFileCopyCallback fs, Cnt, 0, fs, Cnt, CDec(API_BUFSIZ)
        Else
            DoEvents
        End If
    End If

    CloseHandle fhSrc

    FileCopyRW.Fail = False

    ' set file attrributes
    Attrs = sfi.Attrs
    If Attrs = PT_ATTR_INVALID Then Attrs = PT_ATTR_NORMAL
    Attrs = Attrs And PT_ATTR_FILE_MASK
    If ApiAttrsSet(dfi.FullName, Attrs) <> 0 Then
        FileCopyRW.msg = "Could Not Set Destination File Attributes (" & ApiError(ApiLastError) & "): " & dfi.FullName
        FileCopyRW.Warn = True
    End If

    result = ApiFileTimeSet(fhDst, sfi.Creation.Time64, sfi.LastAccess.Time64, sfi.LastWrite.Time64)
    If result <> 0 Then
        FileCopyRW.msg = "Could Not Set Destination File Times (" & ApiError(ApiLastError) & "): " & dfi.FullName
        FileCopyRW.Warn = True
    End If

    CloseHandle fhDst
    FileCopyRW.Fail = False
    FileCopyRW.BytesCopied = fs
    ApiLastError = 0
End Function
Private Function FileCopyEx(ByRef sfi As PT_FILE_INFO, ByRef dfi As PT_FILE_INFO) As PT_COPY_RESULT
    Dim lpCopyProgRoutine As Long
    Dim CallBack As Boolean
    Dim usa() As Byte
    Dim uda() As Byte
    Dim result As Long

    FileCopyEx.Fail = True
    FileCopyEx.Warn = False
    FileCopyEx.msg = ""
    FileCopyEx.BytesCopied = 0

    If (gFileCopyCallback Is Nothing) Then
        CallBack = False
        lpCopyProgRoutine = 0
    Else
        CallBack = True
        lpCopyProgRoutine = ApiFunctionAddr(AddressOf CopyProgressRoutine)
    End If

    If CFXW_OK Then
        usa = UnicodeFid(sfi.FullName)
        uda = UnicodeFid(dfi.FullName)
        result = CopyFileExW(usa(0), uda(0), lpCopyProgRoutine, 0, False, 0)
    Else
        result = CopyFileExA(sfi.FullName, dfi.FullName, lpCopyProgRoutine, 0, False, 0)
    End If

    If result = 0 Then ' failure
        FileCopyEx.msg = SetApiError("Copy of " & sfi.FullName & " to " & dfi.FullName & " Failed")
    Else
        FileCopyEx.Fail = False
        FileCopyEx.BytesCopied = sfi.Size
        ApiLastError = 0
    End If

End Function
Public Function CopyProgressRoutine( _
    ByVal TotalFileSize As Currency, _
    ByVal FileBytesTransferred As Currency, _
    ByVal TotalStreamSize As Currency, _
    ByVal StreamBytesTransferred As Currency, _
    ByVal StreamNumber As Long, _
    ByVal CallbackReason As Long, _
    ByVal hSourceFile As Long, _
    ByVal hDestinationFile As Long, _
    ByVal lpData As Long) As PT_FILE_COPY_PROGRESS

    Dim liFileSize As LARGE_INTEGER
    Dim liFileCnt As LARGE_INTEGER
    Dim liStreamSize As LARGE_INTEGER
    Dim liStreamCnt As LARGE_INTEGER

    Dim FileSize As Variant
    Dim FileCnt As Variant
    Dim StreamSize As Variant
    Dim StreamCnt As Variant

    CopyProgressRoutine = PROGRESS_CONTINUE
    If gFileCopyCallback Is Nothing Then Exit Function

    ApiMemoryCopy VarPtr(liFileSize), VarPtr(TotalFileSize), LenB(liFileSize)
    FileSize = ApiConvLargeIntDec(liFileSize)
    ApiMemoryCopy VarPtr(liFileCnt), VarPtr(FileBytesTransferred), LenB(liFileCnt)
    FileCnt = ApiConvLargeIntDec(liFileCnt)
    ApiMemoryCopy VarPtr(liStreamSize), VarPtr(TotalStreamSize), LenB(liStreamSize)
    StreamSize = ApiConvLargeIntDec(liStreamSize)
    ApiMemoryCopy VarPtr(liStreamCnt), VarPtr(StreamBytesTransferred), LenB(liStreamCnt)
    StreamCnt = ApiConvLargeIntDec(liStreamCnt)

    Select Case CallbackReason
        Case CALLBACK_STREAM_SWITCH:
            CopyProgressRoutine = PROGRESS_CONTINUE
         
        Case CALLBACK_CHUNK_FINISHED
            CopyProgressRoutine = PROGRESS_CONTINUE
    End Select

    CopyProgressRoutine = gFileCopyCallback.ApiFileCopyCallback(FileSize, FileCnt, StreamNumber, StreamSize, StreamCnt, CDec(API_BUFSIZ))
End Function
Private Function FileBackup(ByRef sfi As PT_FILE_INFO, ByRef dfi As PT_FILE_INFO) As PT_COPY_RESULT
    Dim fhSrc As Long
    Dim fhDst As Long
    Dim sa As SECURITY_ATTRIBUTES
    Dim sid As WIN32_STREAM_ID
    Dim buf() As Byte
    Dim buflen As Long
    Dim ReadContext As Long
    Dim WriteContext As Long
    Dim result As Long
    Dim CallBack As Boolean
    Dim BlockCnt As Long
    Dim FileByteCopyCnt As Variant
    Dim StreamSize As Variant
    Dim StreamByteCopyCnt As Variant
    Dim StreamNum As Long
    Dim PrevStreamId As Long
    Dim li As LARGE_INTEGER
    Dim Attrs As Long

    FileBackup.Fail = True
    FileBackup.Warn = False
    FileBackup.msg = ""
    FileBackup.BytesCopied = 0

    If gFileCopyCallback Is Nothing Then
        CallBack = False
    Else
        CallBack = True
    End If

    sa.nLength = 0
    fhSrc = CreateFileA(sfi.FullName, GENERIC_READ, 0, sa, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS Or FILE_FLAG_SEQUENTIAL_SCAN, 0)
    If fhSrc = INVALID_HANDLE_VALUE Then
        FileBackup.msg = SetApiError("Could Not Open Source File", sfi.FullName)
        Exit Function
    End If

    'If CallBack Then gFileCopyCallback.ApiFileCopyCallback "Opening " & dfi.FullName
    fhDst = CreateFileA(dfi.FullName, GENERIC_WRITE Or WRITE_OWNER Or WRITE_DAC, FILE_SHARE_READ Or FILE_SHARE_WRITE, sa, CREATE_ALWAYS, FILE_FLAG_BACKUP_SEMANTICS Or FILE_FLAG_SEQUENTIAL_SCAN, 0)
    If fhDst = INVALID_HANDLE_VALUE Then
        FileBackup.msg = SetApiError("Could Not Open Destination File", dfi.FullName)
        CloseHandle fhSrc
        Exit Function
    End If

    BlockCnt = 0
    FileByteCopyCnt = CDec(0)
    StreamByteCopyCnt = CDec(0)
    StreamNum = 0
    PrevStreamId = -1

    If CallBack Then gFileCopyCallback.ApiFileCopyCallback sfi.Size, FileByteCopyCnt, StreamNum, StreamSize, StreamByteCopyCnt, CDec(API_BUFSIZ)
    ReDim buf(API_BUFSIZ)
    ReadContext = 0
    WriteContext = 0
    'If CallBack Then gFileCopyCallback.ApiFileCopyCallback "Copying " & sfi.Fid & " to " & dfi.Fid
    result = BackupRead(fhSrc, buf(0), API_BUFSIZ, buflen, False, True, ReadContext)
    If buflen >= LenB(sid) Then
        ApiMemoryCopy VarPtr(sid), VarPtr(buf(0)), LenB(sid)
    End If
    While (result <> 0) And (buflen > 0)
        If ApiAbort Then
            result = BackupRead(fhSrc, buf(0), 0, buflen, True, True, ReadContext)
            result = BackupWrite(fhDst, buf(0), 0, buflen, True, True, WriteContext)
            CloseHandle fhSrc
            CloseHandle fhDst
            ApiLastError = -1
            FileBackup.msg = "User Abort"
            Exit Function
        End If
        result = BackupWrite(fhDst, buf(0), buflen, buflen, False, True, WriteContext)
        If result = 0 Then
            FileBackup.msg = SetApiError("Could Not Write Destination File", dfi.FullName)
            result = BackupRead(fhSrc, buf(0), 0, buflen, True, True, ReadContext)
            result = BackupWrite(fhDst, buf(0), 0, buflen, True, True, WriteContext)
            CloseHandle fhSrc
            CloseHandle fhDst
            Exit Function
        End If
        FileByteCopyCnt = FileByteCopyCnt + CDec(buflen)
        If FileByteCopyCnt > sfi.Size Then sfi.Size = FileByteCopyCnt
'        If sid.dwStreamID <> PrevStreamId Then ' new stream
'            StreamNum = StreamNum + 1
'            li.highpart = sid.dwStreamSizeHigh
'            li.lowpart = sid.dwStreamSizeLow
'            StreamSize = ApiLargeInt2Dec(li)
'            StreamByteCopyCnt = CDec(0)
'            PrevStreamId = sid.dwStreamID
'        Else
'            StreamByteCopyCnt = StreamByteCopyCnt + CDec(BufLen)
'            If StreamByteCopyCnt >= StreamSize Then ' finished stream
'                If BufLen >= LenB(sid) Then
'                    ApiMemoryCopy VarPtr(sid), VarPtr(buf(0)), LenB(sid)
'                End If
'            End If
'        End If

        result = BackupRead(fhSrc, buf(0), API_BUFSIZ, buflen, False, True, ReadContext)
        If result = 0 Then
            FileBackup.msg = SetApiError("Could Not Read Source File", sfi.FullName)
            result = BackupRead(fhSrc, buf(0), 0, buflen, True, True, ReadContext)
            result = BackupWrite(fhDst, buf(0), 0, buflen, True, True, WriteContext)
            CloseHandle fhSrc
            CloseHandle fhDst
            Exit Function
        End If
        BlockCnt = BlockCnt + 1
        If (BlockCnt Mod CALLBACK_INTERVAL) = 0 Then
            If CallBack Then gFileCopyCallback.ApiFileCopyCallback sfi.Size, FileByteCopyCnt, StreamNum, StreamSize, StreamByteCopyCnt, CDec(API_BUFSIZ)
        Else
            DoEvents
        End If
    Wend

    If CallBack Then gFileCopyCallback.ApiFileCopyCallback sfi.Size, FileByteCopyCnt, StreamNum, StreamSize, StreamByteCopyCnt, CDec(API_BUFSIZ)
    result = BackupRead(fhSrc, buf(0), 0, buflen, True, True, ReadContext)
    result = BackupWrite(fhDst, buf(0), 0, buflen, True, True, WriteContext)
    CloseHandle fhSrc

    ' set file attrributes
    Attrs = sfi.Attrs
    If Attrs = PT_ATTR_INVALID Then Attrs = PT_ATTR_NORMAL
    Attrs = Attrs And PT_ATTR_FILE_MASK
    If ApiAttrsSet(dfi.FullName, Attrs) <> 0 Then
        FileBackup.msg = "Could Not Set Destination File Attributes (" & ApiError(ApiLastError) & "): " & dfi.FullName
        FileBackup.Warn = True
    End If

    result = ApiFileTimeSet(fhDst, sfi.Creation.Time64, sfi.LastAccess.Time64, sfi.LastWrite.Time64)
    If result <> 0 Then
        FileBackup.msg = "Could Not Set Destination File Times (" & ApiError(ApiLastError) & "): " & dfi.FullName
        FileBackup.Warn = True
    End If

    CloseHandle fhDst

    FileBackup.Fail = False
    FileBackup.BytesCopied = sfi.Size
    ApiLastError = 0
End Function
#End If ' API_BACKUP
Private Function SetApiError(Optional Msg1 As String = "", Optional Msg2 As String = "") As String
    ApiLastError = Err.LastDllError
    If ApiLastError = 0 Then ApiLastError = GetLastError()
    If ApiLastError = 0 Then ApiLastError = -1
    SetApiError = Msg1 & " (" & ApiError(ApiLastError) & "): " & Msg2
End Function
Public Function ApiFileDelete(Filename As String) As Long
    Dim uba() As Byte
    Dim result As Long
    Dim ok As Long

    If Not InitDone Then ApiInit

    If Right(Filename, 1) = "\" Then
        ApiFileDelete = ApiFolderDelete(Filename)
        Exit Function
    End If

    result = ApiAttrsSet(Filename, PT_ATTR_NORMAL)

    If DFW_OK Then
        uba = UnicodeFid(Filename)
        ok = DeleteFileW(uba(0))
    Else
        ok = DeleteFileA(Filename)
    End If

    If ok = 0 Then ' error
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFileDelete = ApiLastError
End Function
Public Function ApiFolderFix(FolderName As String) As String
    Dim s As String

    s = Trim(FolderName)
    If Len(s) <= 0 Then
        ApiFolderFix = "\"
    ElseIf Right(s, 1) <> "\" Then
        ApiFolderFix = s & "\"
    Else
        ApiFolderFix = s
    End If
End Function
Public Function ApiFolderRoot(FolderName As String) As String
    Dim Level As Long
    Dim s As String
    Dim i As Long
    Dim sa() As String

    ApiFolderRoot = ""

    s = ApiFolderFix(FolderName)
    Level = 0

    If Left(s, 2) = "\\" Then  'unc
        i = InStr(3, s, "\")
    Else
        i = InStr(1, s, ":")
        If i = 2 Then ' canonical local name
            i = InStr(1, s, "\")
        ElseIf i > 2 Then ' protocol such as FTP:\\
        
        Else
            i = InStr(1, s, "\")
        End If
    End If

    ApiFolderRoot = Left(s, i)
End Function
Public Function ApiFolderIsRoot(FolderName As String) As Boolean
    Dim Level As Long
    Dim s As String
    Dim i As Long

    ApiFolderIsRoot = False

    If Len(FolderName) <= 0 Then Exit Function
    s = ApiFolderFix(FolderName)

    Level = 0
    For i = 1 To Len(s)
        If Mid(s, i, 1) = "\" Then Level = Level + 1
    Next i

    If Left(s, 2) = "\\" Then  'unc
        If Level <= 3 Then ' root folder
            ApiFolderIsRoot = True
            Exit Function
        End If
    ElseIf Mid(s, 2, 1) = ":" Then
        If Level <= 1 Then ' root folder
            ApiFolderIsRoot = True
            Exit Function
        End If
    End If
End Function
Public Function ApiFolderDelete(FolderName As String) As Long
    Dim uba() As Byte
    Dim result As Long
    Dim ok As Long
    Dim s As String

    If Not InitDone Then ApiInit

    If Len(FolderName) <= 0 Then Exit Function

    If ApiFolderIsRoot(FolderName) Then ' don't delete root folders
        ApiLastError = -1
        ApiFolderDelete = ApiLastError
        Exit Function
    End If

    result = ApiAttrsSet(FolderName, PT_ATTR_NORMAL)
    If Right(FolderName, 1) = "\" Then
        s = Left(FolderName, Len(FolderName) - 1)
    Else
        s = FolderName
    End If

    If DFW_OK Then
        uba = UnicodeFid(s)
        ok = DeleteFileW(uba(0))
    Else
        ok = DeleteFileA(s)
    End If

    If ok Then
        ApiLastError = 0
    Else
        SetApiError
    End If

    ApiFolderDelete = ApiLastError
End Function
Public Sub ApiInitCommonControls()
    Dim result As Long

    If Not InitDone Then ApiInit

    If ICC_OK Then result = InitCommonControls()
End Sub
Public Sub ApiFormFont(Frm As Form, Optional FontName As String = "Arial", Optional MinFontSize As Single = 9)
    Dim Ctrl As Control
    Dim i As Long
    Dim s As String

    If Not InitDone Then ApiInit

    On Error Resume Next
    Frm.Font.Name = FontName
    If Frm.Font.Size < 9 Then Frm.Font.Size = 9
    For Each Ctrl In Frm.Controls
        If InStr(1, Ctrl.Tag, "NONAME", vbTextCompare) <= 0 Then
            Ctrl.Font.Name = FontName
            If Err.Number <> 0 Then
                Ctrl.FontName = FontName
                Err.Clear
            End If
        End If
        If InStr(1, Ctrl.Tag, "NOSIZE", vbTextCompare) <= 0 Then
            If Ctrl.Font.Size < 9 Then Ctrl.Font.Size = MinFontSize
            If Err.Number <> 0 Then
                Ctrl.FontSize = MinFontSize
                Err.Clear
            End If
        End If
        Ctrl.Caption = Replace(Ctrl.Caption, "\n", vbCrLf, , , vbTextCompare)
        If StrComp(TypeName(Ctrl), "StatusBar", vbTextCompare) = 0 Then
            ' fix StatusBar Date and Time panels - they don't autosize correctly when font size changed
            For i = 1 To Ctrl.Panels.Count
                If Ctrl.Panels(i).Style = sbrDate Then
                    Ctrl.Panels(i).Style = sbrText
                    Ctrl.Panels(i).Style = sbrDate
                    Ctrl.Panels(i).AutoSize = sbrContents
                ElseIf Ctrl.Panels(i).Style = sbrTime Then
                    Ctrl.Panels(i).Style = sbrText
                    Ctrl.Panels(i).Style = sbrTime
                    Ctrl.Panels(i).AutoSize = sbrContents
                ElseIf (Ctrl.Panels(i).Style = sbrText) And (Ctrl.Panels(i).AutoSize = sbrContents) Then
                    s = Ctrl.Panels(i).Text
                    Ctrl.Panels(i).Text = ""
                    Ctrl.Panels(i).Text = s
                End If
            Next i
        End If
        Err.Clear
    Next Ctrl

    On Error GoTo 0
End Sub
Public Sub ApiTextBoxTabset(Box As TextBox, Tabs() As Long)
    Dim result As Long
    Dim TabCnt As Long
    Dim i As Long

    If Not InitDone Then ApiInit

    For i = 0 To UBound(Tabs)
        Tabs(i) = Tabs(i) '* 12  ' convert chars to dialog units
    Next i

    ' clear existing tab stops
    result = SendMessageSetTabs(Box.hWnd, EM_SETTABSTOPS, 0, vbNullString)

    TabCnt = UBound(Tabs) + 1
    result = SendMessageSetTabs(Box.hWnd, EM_SETTABSTOPS, TabCnt, Tabs(0))

    'result = SendMessageA(Box.hwnd, EM_SETTABSTOPS, TabCnt, VarPtr(Tabs(0)))
End Sub
Public Function ApiFileTimeGet(hFile As Long, ByRef CreationTime As Variant, ByRef LastAccessTime As Variant, ByRef LastWriteTime As Variant) As Long
    Dim ftCreationTime As FILETIME
    Dim ftLastAccessTime As FILETIME
    Dim ftLastWriteTime As FILETIME
    Dim li As LARGE_INTEGER
    Dim result As Long

    If Not InitDone Then ApiInit

    result = GetFileTime(hFile, ftCreationTime, ftLastAccessTime, ftLastWriteTime)

    If result = 0 Then
        SetApiError
    Else
        li.highpart = ftCreationTime.dwHighDateTime
        li.lowpart = ftCreationTime.dwLowDateTime
        CreationTime = ApiConvLargeIntDec(li)
        li.highpart = ftLastAccessTime.dwHighDateTime
        li.lowpart = ftLastAccessTime.dwLowDateTime
        LastAccessTime = ApiConvLargeIntDec(li)
        li.highpart = ftLastWriteTime.dwHighDateTime
        li.lowpart = ftLastWriteTime.dwLowDateTime
        LastWriteTime = ApiConvLargeIntDec(li)
        ApiLastError = 0
    End If

    ApiFileTimeGet = ApiLastError
End Function
Public Function ApiFileTimeSet(hFile As Long, CreationTime As Variant, LastAccessTime As Variant, LastWriteTime As Variant) As Long
    Dim ftCreationTime As FILETIME
    Dim ftLastAccessTime As FILETIME
    Dim ftLastWriteTime As FILETIME
    Dim ftCt As FILETIME
    Dim ftLat As FILETIME
    Dim ftLwt As FILETIME
    Dim li As LARGE_INTEGER
    Dim result As Long
    Dim var As Variant
    Dim newvar As Variant

    If Not InitDone Then ApiInit

    ApiLastError = 0

    li = ApiConvDecLargeInt(CreationTime)
    ftCreationTime.dwHighDateTime = li.highpart
    ftCreationTime.dwLowDateTime = li.lowpart
    li = ApiConvDecLargeInt(LastAccessTime)
    ftLastAccessTime.dwHighDateTime = li.highpart
    ftLastAccessTime.dwLowDateTime = li.lowpart
    li = ApiConvDecLargeInt(LastWriteTime)
    ftLastWriteTime.dwHighDateTime = li.highpart
    ftLastWriteTime.dwLowDateTime = li.lowpart

    result = SetFileTime(hFile, ftCreationTime, ftLastAccessTime, ftLastWriteTime)

    If result = 0 Then
        SetApiError
    Else
        ApiLastError = 0
    End If

    ApiFileTimeSet = ApiLastError
End Function
Public Function ApiDriveArray(Optional SkipDisks As String = "") As String()
    Dim s As String
    Dim temp() As String
    Dim i As Long
    Dim drv() As String

    s = String(4000, " ")
    GetLogicalDriveStringsA Len(s), s
    s = Trim(s)
    temp = Split(s, Chr(0))

    For i = 0 To UBound(temp)
        temp(i) = Trim(temp(i))
        If Len(temp(i)) > 0 Then
            If InStr(1, SkipDisks, temp(i), vbTextCompare) <= 0 Then
                ReDim Preserve drv(i)
                drv(i) = temp(i)
            End If
        End If
    Next i

    ApiDriveArray = drv
End Function
Public Function ApiDriveType(Drive As String) As Long
    Dim s As String
    Dim result As Long

    If Not InitDone Then ApiInit

    ApiDriveType = GetDriveTypeA(ApiFolderRoot(Drive))
    
End Function
Private Function ApiDriveIsRemovable(DriveType As PT_DRIVE_TYPE) As Boolean
    If DriveType = DRIVE_REMOVABLE Then
        ApiDriveIsRemovable = True
    Else
        ApiDriveIsRemovable = False
    End If
End Function
Public Function ApiDriveInfo(Drive As String, Optional Force As Boolean = False) As PT_DRIVE_INFO
    Dim s As String
    Dim uba() As Byte
    Dim tuba() As Byte
    Dim fs As String
    Dim VolName As String
    Dim serno As Long
    Dim Flags As Long
    Dim maxfn As Long
    Dim result As Long
    Dim fso As Scripting.FileSystemObject
    Dim objDrv As Scripting.Drive
    Dim liFreeSpace As LARGE_INTEGER
    Dim liTotalSpace As LARGE_INTEGER
    Dim liUserFreeSpace As LARGE_INTEGER
    Dim SectorsPerCluster As Long
    Dim BytesPerSector As Long
    Dim TotClusters As Long
    Dim FreeClusters As Long
    Dim di As PT_DRIVE_INFO
    Dim si As PT_SHARE_INFO
    Dim Share As String
    Dim Cnt As Long
    Dim sid As String
    Dim QuotaControl As Object 'DiskQuotaControl
    Dim QuotaUser As Object ' DIDiskQuotaUser
    Dim Target As String

    If Not InitDone Then ApiInit
    If Not InitNetDone Then ApiInitNet

    di.RootFolder = ApiFolderRoot(Drive)
    di.DriveType = DRIVE_UNKNOWN
    di.Ready = False
    di.Removable = False
    di.SerialNum = 0
    di.DriveStatus = "Not Ready"
    di.DriveType = DRIVE_UNKNOWN
    di.UnicodeSupported = False
    di.UserName = ApiUserName()
    di.Label = ""
    
    di.ClusterSize = CDec(0)
    di.DriveCompressionSupported = False
    di.DriveSize = CDec(0)
    di.EncryptionSupported = False
    di.Error = False
    di.ErrorDesc = ""
    di.FileSystem = ""
    di.FreeSpace = CDec(0)
    di.FileCompressionSupported = False
    di.LfnSupported = False
    di.MaxComponentLength = 0
    di.Network = False

    di.QuotaAccountStatus = 0
    di.QuotaDisplayName = ""
    di.QuotaDomain = ""
    di.QuotaFree = CDec(0)
    di.QuotaLimit = CDec(0)
    di.QuotaSID = ""
    di.QuotaState = DISKQUOTA_STATE_UNKNOWN
    di.QuotaSupported = False
    di.QuotaUsed = CDec(0)
    di.QuotaWarning = CDec(0)

    di.Network = False
    di.ShareName = ""
    di.ShareServer = ""
    di.LocalDrive = ""
    di.LocalPath = ""

    di.DriveType = ApiDriveType(di.RootFolder)
    If di.DriveType = DRIVE_UNKNOWN Then
        ApiDriveInfo = di
        Exit Function
    End If

    di.Removable = ApiDriveIsRemovable(di.DriveType)
    If (Not di.Removable) Or Force Then
        fs = String(17, Chr(0))
        VolName = String(17, Chr(0))
        result = GetVolumeInformationA(di.RootFolder, VolName, Len(VolName), serno, maxfn, Flags, fs, Len(fs))
        di.FileSystem = ApiTextStrip(fs)
        di.MaxComponentLength = maxfn
        di.Flags = Flags
        If Flags And FS_FILE_COMPRESSION Then
            di.FileCompressionSupported = True
        End If
        If Flags And FS_VOL_IS_COMPRESSED Then
            di.DriveCompressionSupported = True
        End If
        If Flags And FS_VOLUME_QUOTAS Then
            di.QuotaSupported = True
        End If
        If Flags And FS_FILE_ENCRYPTION Then
            di.EncryptionSupported = True
        End If
        If Flags And FS_UNICODE_STORED_ON_DISK Then
            di.UnicodeSupported = True
        End If
        If di.MaxComponentLength > 12 Then
            di.LfnSupported = True
        End If
        di.Label = ApiTextStrip(VolName)
        If di.UnicodeSupported And di.LfnSupported Then
            di.MaxPath = 32000
        Else
            di.MaxPath = maxfn
        End If
        di.SerialNum = serno
        s = Hex(serno)
        If Len(s) < 8 Then s = String(8 - Len(s), "0") & s
        di.SerialString = Left(s, 4) & "-" & Right(s, 4)
    End If

    If di.DriveType = DRIVE_REMOTE Then
        s = Left(di.RootFolder, Len(di.RootFolder) - 1)
        If WNGCW_OK Then
            ReDim uba(2048)
            Cnt = (UBound(uba) / 2) - 1
            tuba = s & vbNullChar
            result = WNetGetConnectionW(tuba(0), uba(0), Cnt)
            Share = uba
            Share = ApiTextStrip(Share)
        Else
            Share = String(1024, Chr(0))
            Cnt = Len(Share)
            result = WNetGetConnectionA(s, Share, Cnt)
            Share = ApiTextStrip(Share)
        End If

        Select Case result
            Case NO_ERROR:
                di.ShareName = Share
                di.Network = True
            Case ERROR_BAD_DEVICE
                di.Error = True
                di.ErrorDesc = "Bad Device"
                di.ShareName = di.ErrorDesc
            Case ERROR_NOT_CONNECTED
                di.ShareName = "(local)" ' "Not Connected"
                di.ShareServer = "\\" & ApiComputerName()
                di.LocalPath = di.RootFolder
            Case ERROR_NO_NETWORK
                di.ShareName = "(local)"   ' "No Network"
                di.ShareServer = "\\" & ApiComputerName()
                di.LocalPath = di.RootFolder
            Case ERROR_CONNECTION_UNAVAIL
                di.ShareName = "Connection Unavailable"
            Case ERROR_NO_NET_OR_BAD_PATH
                di.Error = True
                di.ErrorDesc = "No Net or Bad Path"
                di.ShareName = di.ErrorDesc
            Case ERROR_EXTENDED_ERROR
                di.Error = True
                di.ErrorDesc = "Network Error"
                di.ShareName = di.ErrorDesc
            Case Else
                di.Error = True
                di.ErrorDesc = "Network Error"
                di.ShareName = di.ErrorDesc
        End Select

        If di.Network And (Not di.Error) Then
            si = ApiShareInfo(di.ShareName)
            If si.ErrNum = 0 Then
                di.ShareServer = si.Server
                di.LocalDrive = si.LocalDrive
                di.LocalPath = si.LocalPath
                di.ShareType = si.ShareType
            Else
                di.Error = True
                di.ErrorDesc = si.ErrDesc
            End If
        End If
    End If

    If di.QuotaSupported Then ' get more quota info
        On Error Resume Next
        Set QuotaControl = New DiskQuotaControl ' CreateObject("Microsoft.DiskQuota.1")
        If Err.Number <> 0 Then
            di.QuotaState = DISKQUOTA_STATE_UNKNOWN
            di.ErrorDesc = Err.Description
            Err.Clear
        Else
            If di.Network Then
                Target = di.ShareServer & "\" & di.LocalDrive & "$"
            Else
                Target = di.RootFolder
            End If

            QuotaControl.Initialize Target, 0
            If Err.Number <> 0 Then
                If di.Network Then
                    di.QuotaState = DISKQUOTA_STATE_NOT_ROOT
                Else
                    di.QuotaState = DISKQUOTA_STATE_UNKNOWN
                End If
                Err.Clear
            Else
                di.QuotaSID = QuotaControl.TranslateLogonNameToSID(di.UserName)
                Set QuotaUser = QuotaControl.FindUser(di.QuotaSID)
                di.QuotaDomain = QuotaUser.AccountContainerName
                di.QuotaDisplayName = QuotaUser.DisplayName

                di.QuotaState = QuotaControl.QuotaState()
                If di.QuotaState <> DISKQUOTA_STATE_DISABLED Then
                    di.QuotaUsed = QuotaUser.QuotaUsed
                    di.QuotaLimit = QuotaUser.QuotaLimit
                    di.QuotaWarning = QuotaUser.QuotaThreshold
                    di.QuotaAccountStatus = QuotaUser.AccountStatus
                End If
            End If
        End If
    End If

    On Error Resume Next
    Err.Clear
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set objDrv = fso.GetDrive(di.RootFolder)
    If objDrv.IsReady Or (Err.Number <> 0) Then
        di.Ready = True
        di.DriveStatus = "Ready"
        di.DriveSize = ApiDriveQty(di.RootFolder, PT_QTY_DISK_SIZE)
        di.FreeSpace = ApiDriveQty(di.RootFolder, PT_QTY_FREE_SPACE)
        di.QuotaFree = ApiDriveQty(di.RootFolder, PT_QTY_FREE_QUOTA)
        di.ClusterSize = ApiDriveQty(di.RootFolder, PT_QTY_ALLOC_UNIT)
        Err.Clear
    End If

    Err.Clear
    ApiDriveInfo = di
End Function
Private Function ApiShareInfo(ShareName As Variant) As PT_SHARE_INFO
    Dim result As Long
    Dim si As PT_SHARE_INFO
    Dim pNetName() As Byte
    Dim pServer() As Byte
    Dim ptmpBuffer As Long
    Dim NTbuf As SHARE_INFO_502
    Dim W95buf As SHARE_INFO_50
    Dim X As Integer
    Dim TotAvail As Integer

    If Not InitDone Then ApiInit

    si.NetName = ""
    si.ErrNum = 0
    si.ErrDesc = ""
    si.Server = ""
    si.LocalDrive = ""
    si.LocalPath = ""
    si.ShareType = 0
    si.Remark = ""
    si.CurUses = 0
    si.MaxUses = 0

    si.NetName = LTrim(ShareName)

    ' check for leading "\\" in the ShareName
    If Left(si.NetName, 2) = "\\" Then
        ' find the end of the server in the name
        X = InStr(3, si.NetName, "\")
        ' only a server in the name
        If X = 0 Then
            si.ErrNum = ERROR_INVALID_NAME
            si.ErrDesc = "Need share name not server name."
            ApiShareInfo = si
            Exit Function
        Else
            si.Server = Left(si.NetName, X - 1)
            si.NetName = Mid(si.NetName, X + 1)
        End If
    End If

    ' trim trailing "\"
    X = InStr(si.NetName, "\")
    If X > 0 Then
        si.NetName = Left(si.NetName, X - 1)
    End If

    pNetName = si.NetName & vbNullChar
    pServer = si.Server & vbNullChar

    If WinVer.PlatformNT Then
        result = NetShareGetInfoW(pServer(0), pNetName(0), 502, ptmpBuffer)
    Else
        result = NetShareGetInfoA(si.Server, si.NetName, 50, W95buf, LenB(W95buf), TotAvail)
    End If

    If result <> NO_ERROR Then
        Select Case result
            Case ERROR_ACCESS_DENIED
                si.ErrDesc = "Access Denied"
            Case ERROR_INVALID_LEVEL
                si.ErrDesc = "Not Supported"
            Case ERROR_INVALID_PARAMETER
                si.ErrDesc = "Not Supported"
            Case ERROR_MORE_DATA
                si.ErrDesc = "Buffer Error"
            Case ERROR_NOT_ENOUGH_MEMORY
                si.ErrDesc = "Insufficient Memory"
            Case NERR_NetNameNotFound
                si.ErrDesc = "Not Share"
            Case Else
                si.ErrDesc = "Unknown (" & CStr(result) & ")"
        End Select
        si.ErrNum = result
        ApiShareInfo = si
        Exit Function
    End If

    If WinVer.PlatformNT Then
        ApiMemoryCopy VarPtr(NTbuf), ptmpBuffer, LenB(NTbuf)
        si.NetName = ApiTextRead(NTbuf.lpNetName, PT_TEXT_UNICODE)
        si.ShareType = NTbuf.dwShareType
        si.LocalPath = ApiTextRead(NTbuf.lpPath, PT_TEXT_UNICODE)
        si.LocalDrive = Left(si.LocalPath, 1)
        si.CurUses = NTbuf.dwCurUses
        si.MaxUses = NTbuf.dwMaxUses
        si.Remark = ApiTextRead(NTbuf.lpRemark, PT_TEXT_UNICODE)
        si.ShareType = NTbuf.dwShareType
        result = NetApiBufferFree(ptmpBuffer)
            If result <> ERROR_SUCCESS Then
            si.ErrNum = result
            si.ErrDesc = "NetApiBufferFree: Unknown"
        End If
    Else
        si.NetName = W95buf.NetName
        si.ShareType = W95buf.dwShareType
        si.LocalPath = ApiTextRead(W95buf.lpPath, PT_TEXT_ASCII)
        si.LocalDrive = Left(si.LocalPath, 1)
        si.CurUses = 0
        si.MaxUses = 0
        si.Remark = ApiTextRead(W95buf.lpRemark, PT_TEXT_ASCII)
        si.ShareType = W95buf.dwShareType
    End If

    If Len(si.LocalPath) > 0 Then
        If Right(si.LocalPath, 1) <> "\" Then
            si.LocalPath = si.LocalPath & "\"
        End If
    End If

    ApiShareInfo = si
End Function
Public Function ApiDriveQty(Drive As String, Qty As PT_QTY) As Variant
    Dim liSize As LARGE_INTEGER
    Dim liQuota As LARGE_INTEGER
    Dim liFree As LARGE_INTEGER
    Dim SectorsPerCluster As Long
    Dim BytesPerSector As Long
    Dim FreeClusters As Long
    Dim TotClusters As Long
    Dim result As Long
    Dim Root As String
    Dim i As Long

    If Not InitDone Then ApiInit

    Select Case Len(Drive)
        Case 0:
            ApiDriveQty = CDec(INVALID_FILE_SIZE)
            Exit Function

        Case 1:
            Root = Drive & ":\"

        Case 2:
            Root = Drive & "\"

        Case Else:
            Root = Drive
            Root = ApiFolderFix(Root)
            If Left(Root, 2) = "\\" Then ' network share
                i = InStr(3, Root, "\")
                i = InStr(i + 1, Root, "\")
            Else
                i = InStr(1, Root, "\")
            End If
            Root = Left(Root, i - 1)
    End Select

    Select Case Qty
        Case PT_QTY_DISK_SIZE:
            If GDFSEX_OK Then
                result = GetDiskFreeSpaceExA(Root, liQuota, liSize, liFree)
                If result = 0 Then ' failed
                    SetApiError
                    ApiDriveQty = CDec(INVALID_FILE_SIZE)
                    Exit Function
                End If
                ApiDriveQty = ApiConvLargeIntDec(liSize)
            Else
                GetDiskFreeSpaceA Root, SectorsPerCluster, BytesPerSector, FreeClusters, TotClusters
                If result = 0 Then ' failed
                    SetApiError
                    ApiDriveQty = CDec(INVALID_FILE_SIZE)
                    Exit Function
                End If
                ApiDriveQty = CDec(BytesPerSector) * CDec(SectorsPerCluster) * CDec(TotClusters)
            End If

        Case PT_QTY_FREE_SPACE:
            If GDFSEX_OK Then
                result = GetDiskFreeSpaceExA(Root, liQuota, liSize, liFree)
                If result = 0 Then ' failed
                    SetApiError
                    ApiDriveQty = CDec(INVALID_FILE_SIZE)
                    Exit Function
                End If
                ApiDriveQty = ApiConvLargeIntDec(liFree)
            Else
                GetDiskFreeSpaceA Root, SectorsPerCluster, BytesPerSector, FreeClusters, TotClusters
                If result = 0 Then ' failed
                    SetApiError
                    ApiDriveQty = CDec(INVALID_FILE_SIZE)
                    Exit Function
                End If
                ApiDriveQty = CDec(BytesPerSector) * CDec(SectorsPerCluster) * CDec(FreeClusters)
            End If
        
        Case PT_QTY_FREE_QUOTA:
            If GDFSEX_OK Then
                result = GetDiskFreeSpaceExA(Root, liQuota, liSize, liFree)
                If result = 0 Then ' failed
                    SetApiError
                    ApiDriveQty = CDec(INVALID_FILE_SIZE)
                    Exit Function
                End If
                ApiDriveQty = ApiConvLargeIntDec(liQuota)
            Else
                result = GetDiskFreeSpaceA(Root, SectorsPerCluster, BytesPerSector, FreeClusters, TotClusters)
                If result = 0 Then ' failed
                    SetApiError
                    ApiDriveQty = CDec(INVALID_FILE_SIZE)
                    Exit Function
                End If
                ApiDriveQty = CDec(BytesPerSector) * CDec(SectorsPerCluster) * CDec(FreeClusters)
            End If

        Case PT_QTY_ALLOC_UNIT:
            result = GetDiskFreeSpaceA(Root, SectorsPerCluster, BytesPerSector, FreeClusters, TotClusters)
            If result = 0 Then
                SetApiError
                ApiDriveQty = CDec(INVALID_FILE_SIZE)
                Exit Function
            End If
            ApiDriveQty = CDec(BytesPerSector) * CDec(SectorsPerCluster)

        Case Else:
            ApiLastError = -1
            ApiDriveQty = CDec(INVALID_FILE_SIZE)

    End Select

    ApiLastError = 0
End Function
Public Function ApiComputerName() As String
    Dim result As Long
    Dim uba() As Byte
    Dim ComputerName As String

    If Not InitDone Then ApiInit

    If GCNW_OK Then
        ReDim uba(MAX_PATH * 2)
        result = GetComputerNameW(uba(0), MAX_PATH)
        If result = 0 Then
            SetApiError
            ComputerName = UNKNOWN_LIT
            Exit Function
        End If
        ComputerName = uba
    Else
        ComputerName = String(MAX_PATH, Chr(0))
        result = GetComputerNameA(ComputerName, MAX_PATH)
        If result = 0 Then
            SetApiError
            ComputerName = UNKNOWN_LIT
            Exit Function
        End If
    End If

    ApiComputerName = ApiTextStrip(ComputerName)
    ApiLastError = 0
End Function
Public Function ApiUserName() As String
    Dim result As Long
    Dim uba() As Byte
    Dim UserName As String

    If Not InitDone Then ApiInit

    If GUNW_OK Then
        ReDim uba(MAX_PATH * 2)
        result = GetUserNameW(uba(0), MAX_PATH)
        If result = 0 Then
            SetApiError
            UserName = UNKNOWN_LIT
            Exit Function
        End If
        UserName = uba
    Else
        UserName = String(MAX_PATH, Chr(0))
        result = GetUserNameA(UserName, MAX_PATH)
        If result = 0 Then
            SetApiError
            UserName = UNKNOWN_LIT
            Exit Function
        End If
    End If

    ApiUserName = ApiTextStrip(UserName)
    ApiLastError = 0
End Function
Public Function ApiFolderWinSys() As String
    Dim uba() As Byte
    Dim WinSysDir As String
    Dim result As Long

    If Not InitDone Then ApiInit

    If GSDW_OK Then
        ReDim uba(MAX_PATH * 2)
        result = GetSystemDirectoryW(uba(0), MAX_PATH)
        If result = 0 Then
            SetApiError
            WinSysDir = UNKNOWN_LIT
            Exit Function
        End If
        WinSysDir = uba
    Else
        WinSysDir = String(MAX_PATH, Chr(0))
        result = GetSystemDirectoryA(WinSysDir, MAX_PATH)
        If result = 0 Then
            SetApiError
            WinSysDir = UNKNOWN_LIT
            Exit Function
        End If
    End If
    ApiFolderWinSys = ApiTextStrip(WinSysDir)
End Function
Public Function ApiTextSize(hdc As Long, Text As String) As PT_TEXT_SIZE
    Dim result As Long
    Dim xySize As PT_SIZE

    result = GetTextExtentPoint32A(hdc, Text, Len(Text), xySize)
    ApiTextSize.HeightPixels = xySize.HeightPixels
    ApiTextSize.WidthPixels = xySize.WidthPixels
    ApiTextSize.HeightTwips = xySize.HeightPixels * Screen.TwipsPerPixelY
    ApiTextSize.WidthTwips = xySize.WidthPixels * Screen.TwipsPerPixelX
End Function
Public Function ApiTextWrap(hdc As Long, MaxWidthTwips As Single, Text As String) As PT_TEXT_ARRAY
    Dim ta As PT_TEXT_ARRAY
    Dim temp As PT_TEXT_ARRAY
    Dim s As String
    Dim Paras() As String
    Dim i As Long
    Dim j As Long
    Dim MaxWidthPixels As Long

    ta.adj.WidthPixels = Screen.TwipsPerPixelX
    ta.adj.HeightPixels = Screen.TwipsPerPixelY
    MaxWidthPixels = MaxWidthTwips / ta.adj.WidthPixels

    If Len(Text) <= 0 Then
        ApiTextWrap = ta
        Exit Function
    End If

    s = Replace(Text, "\n", vbCrLf, , , vbTextCompare)
    Paras = Split(s, vbCrLf)

    For i = 0 To UBound(Paras)
        ReDim Preserve ta.ParaTable(ta.ParaCnt)
        ta.ParaTable(ta.ParaCnt) = Paras(i)
        ta.ParaCnt = ta.ParaCnt + 1
'        Paras(i) = Trim(Paras(i))
        If Len(Paras(i)) > 0 Then
            temp = WordWrapPara(hdc, MaxWidthPixels, Paras(i), ta)
        Else
            temp.LineCnt = 0
            Erase temp.LineTable
        End If
        If i > 0 Then ' insert inter-para gap
            ReDim Preserve ta.LineTable(ta.LineCnt)
            ta.LineTable(ta.LineCnt) = ""
            ta.LineCnt = ta.LineCnt + 1
        End If
'        j = 0
'        While j < Temp.LineCnt
'            ta.LineTable(ta.LineCnt) = Temp.LineTable(j)
'            ta.LineCnt = ta.LineCnt + 1
'            j = j + 1
'        Wend
    Next i

    For i = 0 To ta.LineCnt - 1
        ta.WrappedText = ta.WrappedText & ta.LineTable(i) & vbCrLf
    Next i

    ta.Overall.HeightTwips = ta.Overall.HeightPixels * ta.adj.HeightPixels
    ta.LastLine.HeightTwips = ta.LastLine.HeightPixels * ta.adj.HeightPixels
    ta.Overall.WidthTwips = ta.Overall.WidthPixels * ta.adj.WidthPixels
    ta.LastLine.WidthTwips = ta.LastLine.WidthPixels * ta.adj.WidthPixels
    ApiTextWrap = ta
End Function
Private Function WordWrapPara(hdc As Long, MaxWidthPixels As Long, Para As String, ByRef ta As PT_TEXT_ARRAY) As PT_TEXT_ARRAY
    Dim result As Long
    Dim sa() As String
    Dim FitCnt As Long
    Dim xySize As PT_SIZE
    Dim i As Long
    Dim BreakFound As Boolean
    Dim NextBeg As Long

    result = GetTextExtentExPointA(hdc, Para, Len(Para), MaxWidthPixels, FitCnt, 0, xySize)

    If FitCnt >= Len(Para) Then  ' it all fits
        ReDim Preserve ta.LineTable(ta.LineCnt)
        ta.LineTable(ta.LineCnt) = Para
        result = GetTextExtentPoint32A(hdc, ta.LineTable(ta.LineCnt), Len(ta.LineTable(ta.LineCnt)), xySize)
        If xySize.WidthPixels > ta.Overall.WidthPixels Then ta.Overall.WidthPixels = xySize.WidthPixels
        ta.Overall.HeightPixels = ta.Overall.HeightPixels + xySize.HeightPixels
        ta.LineCnt = ta.LineCnt + 1
        ta.LastLine.HeightPixels = xySize.HeightPixels
        ta.LastLine.WidthPixels = xySize.WidthPixels
        Exit Function
    End If

    BreakFound = False
    For i = FitCnt To 1 Step -1
        If BreakFound Then
            If Mid(Para, i, 1) > " " Then Exit For
        Else
            If Mid(Para, i, 1) <= " " Then
                BreakFound = True
                NextBeg = i + 1
            End If
        End If
    Next i
    ReDim Preserve ta.LineTable(ta.LineCnt)
    ta.LineTable(ta.LineCnt) = Left(Para, i)

    result = GetTextExtentPoint32A(hdc, ta.LineTable(ta.LineCnt), Len(ta.LineTable(ta.LineCnt)), xySize)
    If xySize.WidthPixels > ta.Overall.WidthPixels Then ta.Overall.WidthPixels = xySize.WidthPixels
    ta.Overall.HeightPixels = ta.Overall.HeightPixels + xySize.HeightPixels
    ta.LastLine.HeightPixels = xySize.HeightPixels
    ta.LastLine.WidthPixels = xySize.WidthPixels

    ta.LineCnt = ta.LineCnt + 1
    WordWrapPara hdc, MaxWidthPixels, Mid(Para, NextBeg), ta
End Function
Public Function ApiTextFit(hdc As Long, Width As Long, Text As String) As String
    Dim result As Long
    Dim ubuf() As Byte
    Dim abuf As String
    Dim pix As Long

    If Not InitDone Then ApiInit
    If Not InitShlwApiDone Then ApiInitShlwApi

    pix = Width / Screen.TwipsPerPixelX

    If SHPCPW_OK Then
        ubuf = Text & vbNullChar
        ReDim Preserve ubuf(MAX_PATHW + 20)
        result = PathCompactPathW(hdc, ubuf(0), pix)
        abuf = ubuf
        ApiTextFit = ApiTextStrip(abuf)
    ElseIf SHPCP_OK Then
        abuf = Text & String(MAX_PATH - Len(Text) + 2, 0)
        result = PathCompactPathA(hdc, abuf, pix)
        ApiTextFit = ApiTextStrip(abuf)
    Else
        ApiTextFit = Text
    End If
End Function
Public Function ApiTextStrip(ByVal s As String) As String
    Dim i As Long

    i = InStr(s, Chr(0)) ' Find first Null byte
    
    If i > 0 Then
        ApiTextStrip = Left(s, i - 1)
    Else
        ApiTextStrip = s
    End If
End Function
Public Function ApiTextCopy(ByVal Addr As Long, Optional Encoding As PT_TEXT_ENCODING = PT_TEXT_ASCII) As String
    ApiTextCopy = ApiTextRead(Addr, Encoding)
End Function
Public Function ApiTextRead(ByVal Addr As Long, Optional Encoding As PT_TEXT_ENCODING = PT_TEXT_ASCII) As String
    Dim Ptr As Long
    Dim b(1) As Byte
    Dim s As String
    Dim su As String

    ApiTextRead = ""
    If Addr = 0 Then Exit Function
    Ptr = Addr
    s = ""
    If Encoding = PT_TEXT_ASCII Then
        Do
            ApiMemoryCopy VarPtr(b(0)), Ptr, 1
            s = s & Chr(b(0))
            Ptr = Ptr + 1
        Loop While b(0) <> 0
    Else
        Do
            ApiMemoryCopy VarPtr(b(0)), Ptr, 1
            ApiMemoryCopy VarPtr(b(1)), Ptr + 1, 1
            su = b
            s = s & su
            Ptr = Ptr + 2
        Loop While (b(0) <> 0) Or (b(1) <> 0)
    End If

    ApiTextRead = ApiTextStrip(s)
End Function
Public Function ApiTextWrite(ByVal Addr As Long, Text As String, Optional Encoding As PT_TEXT_ENCODING = PT_TEXT_ASCII) As Long
    Dim Ptr As Long
    Dim b(1) As Byte
    Dim Unicode() As Byte
    Dim s As String
    Dim i As Long
    Dim su As String
    Dim ByteCnt As Long

    ApiTextWrite = 0
    If Addr = 0 Then Exit Function
    Ptr = Addr
    ByteCnt = 0
    s = Text & vbNullChar
    If Encoding = PT_TEXT_ASCII Then
        For i = 1 To Len(s)
            b(0) = Asc(Mid(s, i, 1))
            ApiMemoryCopy Ptr, VarPtr(b(0)), 1
            Ptr = Ptr + 1
            ByteCnt = ByteCnt + 1
        Next i
    Else
        Unicode = s
        ApiMemoryCopy Ptr, VarPtr(Unicode(0)), UBound(Unicode) + 1
        ByteCnt = UBound(Unicode) + 1
    End If

    ApiTextWrite = ByteCnt
End Function
Public Sub ApiMemoryCopy(ByVal DestAddr As Long, ByVal SrcAddr As Long, ByVal ByteLen As Long)
    If DestAddr = 0 Then Exit Sub
    If SrcAddr = 0 Then Exit Sub
    If ByteLen <= 0 Then Exit Sub

    If IsBadReadPtr(SrcAddr, ByteLen) Then Exit Sub
    If IsBadWritePtr(DestAddr, ByteLen) Then Exit Sub

    CopyMemory DestAddr, SrcAddr, ByteLen
End Sub
Public Sub ApiMemoryZero(ByVal Addr As Long, ByVal ByteLen As Long)
    If Addr = 0 Then Exit Sub
    If ByteLen <= 0 Then Exit Sub

    If IsBadReadPtr(Addr, ByteLen) Then Exit Sub
    If IsBadWritePtr(Addr, ByteLen) Then Exit Sub

    ZeroMemory Addr, ByteLen
End Sub
Public Sub ApiSleep(ms As Long)
    Sleep ms
End Sub
Public Function APIExists(lib As String, func As String) As Boolean
    Dim hnd As Long

    APIExists = False
    hnd = LoadLibraryA(lib)
    If hnd = 0 Then Exit Function

    If GetProcAddress(hnd, func) = 0 Then
        FreeLibrary hnd
        Exit Function
    End If

    FreeLibrary hnd
    APIExists = True
End Function
Public Function ApiError(Optional ByVal ApiErrNum As Long = 0) As String
    Dim buf As String
    Dim result As Long
    Dim uba() As Byte
    Const MaxMsg = 256

    If ApiErrNum = 0 Then
        ApiErrNum = Err.LastDllError
    End If
    If ApiErrNum = 0 Then
        ApiErrNum = GetLastError()
    End If

    If Not InitDone Then ApiInit

    If ApiErrNum = 0 Then
        ApiError = "Error - File Possibly In Use/Locked"
        Exit Function
    End If

    If FMW_OK Then
        ReDim uba(MaxMsg * 2)
        result = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 0, ApiErrNum, 0, uba(0), MaxMsg, 0)
        buf = uba
    Else
        buf = Space(MaxMsg)
        result = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, ApiErrNum, 0, buf, MaxMsg, 0)
    End If

    If (result > 0) Then
        buf = Replace(buf, Chr(0), "")
        buf = Replace(buf, vbCrLf, " ")
        buf = Replace(buf, vbCr, "")
        buf = Replace(buf, vbLf, " ")
        ApiError = "Error " & CStr(ApiErrNum) & " - " & Trim(buf)
    Else
        ApiError = "Error " & CStr(ApiErrNum) & " - " & "unknown error."
    End If
End Function
Public Sub ApiBeep(Optional BeepType As PT_BEEP = MB_ICONASTERISK)
    MessageBeep BeepType
End Sub
Public Function ApiConvUdtByteArray(UdtAddr As Long, UdtLen As Long) As Byte()
    Dim b() As Byte

    ReDim b(UdtLen - 1)
    ApiMemoryCopy VarPtr(b(0)), UdtAddr, UdtLen - 1
    ApiConvUdtByteArray = b
End Function
Public Function ApiConvByteArrayUdt(UdtAddr As Long, DataArray() As Byte)
    Dim b() As Byte

    ApiMemoryCopy UdtAddr, VarPtr(DataArray(0)), UBound(DataArray) - 1
End Function
Public Function ApiConvLargeIntDec(li As LARGE_INTEGER) As Variant
    Dim decLo As Variant
    Dim decHi As Variant

    decLo = CDec(0)
    decHi = CDec(0)

    If li.lowpart < 0 Then
        decLo = CDec(MAX_UINT32) + CDec(li.lowpart)
    Else
        decLo = CDec(li.lowpart)
    End If

    If li.highpart < 0 Then
        decHi = CDec(MAX_UINT32) + CDec(li.highpart)
    Else
        decHi = CDec(li.highpart)
    End If

    ApiConvLargeIntDec = decLo + (decHi * CDec(MAX_UINT32))
End Function
Public Function ApiConvDecLargeInt(Dec As Variant) As LARGE_INTEGER
    Dim li As LARGE_INTEGER
    Dim temp As Variant

    If Dec > CDec(MAX_UINT32) Then
        temp = Int(Dec / CDec(MAX_UINT32))
        li.highpart = ApiConvDecUInt32(temp)
        li.lowpart = ApiConvDecUInt32(Dec - (temp * CDec(MAX_UINT32)))
    Else
        li.highpart = 0
        li.lowpart = ApiConvDecUInt32(Dec)
    End If

    ApiConvDecLargeInt = li
End Function
Public Function ApiConvUInt32Dec(UInt32 As Long) As Variant
    Dim Dec As Variant

    If UInt32 < 0 Then
        Dec = CDec(MAX_UINT32) + CDec(UInt32)
    Else
        Dec = CDec(UInt32)
    End If
    ApiConvUInt32Dec = Dec
End Function
Public Function ApiConvDecUInt32(Dec As Variant) As Long
    Dim UInt32 As Long

    If Dec < 0 Then
        On Error Resume Next
        Err.Raise 534
        Exit Function
    End If
    If Dec >= CDec(MAX_UINT32) Then
        On Error Resume Next
        Err.Raise 534
        Exit Function
    End If

    If Dec > CDec(MAX_SINT32) Then
        UInt32 = (Dec - CDec(MAX_UINT32))
    Else
        UInt32 = CLng(Dec)
    End If
    ApiConvDecUInt32 = UInt32
End Function
Public Function ApiConvUInt16Dec(UInt16 As Integer) As Variant
    Dim Dec As Variant

    If UInt16 < 0 Then
        Dec = CDec(MAX_UINT16) + CDec(UInt16)
    Else
        Dec = CDec(UInt16)
    End If
    ApiConvUInt16Dec = Dec
End Function
Public Function ApiConvDecUInt16(Dec As Variant) As Long
    Dim UInt16 As Long

    If Dec < 0 Then
        On Error Resume Next
        Err.Raise 534
        Exit Function
    End If
    If Dec >= CDec(MAX_UINT16) Then
        On Error Resume Next
        Err.Raise 534
        Exit Function
    End If

    If Dec > CDec(MAX_SINT16) Then
        UInt16 = (Dec - CDec(MAX_UINT16))
    Else
        UInt16 = CInt(Dec)
    End If
    ApiConvDecUInt16 = UInt16
End Function
Public Function ApiConvTextHex(Text As String, Optional SepCnt As Long = 0) As String
    Dim s As String
    Dim c As String
    Dim i As Long

    s = ""
    For i = 1 To Len(Text)
        c = Hex(Asc(Mid(Text, i, 1)))
        c = String(2 - Len(c), "0") & c
        s = s & c
        If SepCnt > 0 Then
            If (i Mod SepCnt) = 0 Then
                s = s & " "
            End If
        End If
    Next i

    ApiConvTextHex = Trim(s)
End Function
Public Function ApiDispBytes(Dec As Variant, Optional DispBytesFormat As DISP_BYTES_FORMAT = DISP_BYTES_ALL) As String
    Dim DispLong As String
    Dim DispShort As String
    Dim s As String

    If DispBytesFormat <> DISP_BYTES_SHORT Then
        DispLong = FormatNumber(Dec, 0) & " bytes"
    Else
        DispLong = ""
    End If

    If DispBytesFormat <> DISP_BYTES_LONG Then
        If Dec > YB Then
            DispShort = FormatNumber(Dec / YB, 2) & " YB"
        ElseIf Dec > ZB Then
            DispShort = FormatNumber(Dec / ZB, 2) & " ZB"
        ElseIf Dec > EB Then
            DispShort = FormatNumber(Dec / EB, 2) & " EB"
        ElseIf Dec > pb Then
            DispShort = FormatNumber(Dec / pb, 2) & " PB"
        ElseIf Dec > TB Then
            DispShort = FormatNumber(Dec / TB, 2) & " TB"
        ElseIf Dec > GB Then
            DispShort = FormatNumber(Dec / GB, 2) & " GB"
        ElseIf Dec > MB Then
            DispShort = FormatNumber(Dec / MB, 2) & " MB"
        ElseIf Dec > KB Then
            DispShort = FormatNumber(Dec / KB, 2) & " KB"
        Else
            DispShort = FormatNumber(Dec, 0) & " bytes"
        End If
    Else
        DispShort = ""
    End If

    Select Case DispBytesFormat
        Case DISP_BYTES_SHORT:
            ApiDispBytes = DispShort

        Case DISP_BYTES_LONG:
            ApiDispBytes = DispLong

        Case Else:
            ApiDispBytes = DispLong & " (" & DispShort & ")"
    End Select
End Function
Public Function BigMod(Dividend As Variant, Divisor As Variant) As Variant
    Dim temp As Variant

    temp = CDec(0)
    Dividend = CDec(Dividend)
    Divisor = CDec(Divisor)

    temp = Dividend / Divisor
    temp = Int(temp)
    temp = temp * Divisor
    temp = Dividend - temp
    BigMod = temp
End Function
Public Function BigIntDiv(Dividend As Variant, Divisor As Variant) As Variant
    Dim temp As Variant

    temp = CDec(0)
    Dividend = CDec(Dividend)
    Divisor = CDec(Divisor)

    temp = Dividend / Divisor
    temp = Int(temp)
    BigIntDiv = temp
End Function
Public Function ApiDispDHMS(ms As Variant, Optional ShortLevel As SHORT_LEVEL = SHORT_NONE) As String
    Dim days As Long
    Dim hrs As Long
    Dim mins As Long
    Dim secs As Long
    Dim msecs As Long
    Dim temp As Variant
    Dim s As String
    Dim Shorten As Boolean

    If Not IsNumeric(ms) Then
        ApiDispDHMS = ""
        Exit Function
    End If

    If ShortLevel <> SHORT_NONE Then Shorten = True

    If ms = 0 Then
        If ShortLevel = SHORT_MAX Then
            ApiDispDHMS = "0s"
        Else
            ApiDispDHMS = "0 secs"
        End If
        Exit Function
    End If

    temp = CDec(ms)
    msecs = BigMod(temp, 1000)
    temp = BigIntDiv(temp, 1000)

    secs = temp Mod 60
    temp = temp \ 60

    mins = temp Mod 60
    temp = temp \ 60

    hrs = temp Mod 24
    temp = temp \ 24

    days = temp

    s = ""
    If (days > 0) Or (Not Shorten And (Len(s) > 0)) Then
        If Len(s) > 0 Then s = s & ", "
        s = s & FormatNumber(days, 0)
        If ShortLevel = SHORT_MAX Then
            s = s & "d"
        Else
            s = s & " day"
            If days <> 1 Then s = s & "s"
        End If
    End If
    If (hrs > 0) Or (Not Shorten And (Len(s) > 0)) Then
        If Len(s) > 0 Then s = s & ", "
        s = s & FormatNumber(hrs, 0)
        If ShortLevel = SHORT_MAX Then
            s = s & "h"
        ElseIf Shorten Then
            s = s & " hr"
            If hrs <> 1 Then s = s & "s"
        Else
            s = s & " hour"
            If hrs <> 1 Then s = s & "s"
        End If
    End If
    If (mins > 0) Or (Not Shorten And (Len(s) > 0)) Then
        If Len(s) > 0 Then s = s & ", "
        s = s & FormatNumber(mins, 0)
        If ShortLevel = SHORT_MAX Then
            s = s & "m"
        Else
            s = s & " min"
            If mins <> 1 Then s = s & "s"
        End If
    End If
    If (secs > 0) Or (Not Shorten And ((msecs > 0) Or (Len(s) > 0))) Then
        If Len(s) > 0 Then s = s & ", "
        If Shorten And (msecs = 0) Then
            s = s & FormatNumber(secs, 0)
            If ShortLevel = SHORT_MAX Then
                s = s & "s"
            Else
                s = s & " sec"
                If secs <> 1 Then s = s & "s"
            End If
        Else
            s = s & FormatNumber(secs + (msecs / 1000), 3)
            If ShortLevel = SHORT_MAX Then
                s = s & "s"
            Else
                s = s & " sec"
                If (secs <> 1) Or (msecs <> 0) Then s = s & "s"
            End If
        End If
    End If

    If Shorten Then s = Replace(s, ",", "")
    ApiDispDHMS = s
End Function
Public Function ApiDispQuotaState(Code As Long) As String
    Dim s As String

    Select Case Code
        Case DISKQUOTA_STATE_DISABLED: s = "Disabled"
        Case DISKQUOTA_STATE_ENFORCE: s = "Enforced"
        Case DISKQUOTA_STATE_TRACK: s = "Tracking"
        Case DISKQUOTA_FILESTATE_INCOMPLETE: s = "Incomplete"
        Case DISKQUOTA_FILESTATE_REBUILDING: s = "Rebuilding"
        Case DISKQUOTA_STATE_NOT_ROOT: s = "Share Not Root"
        Case DISKQUOTA_STATE_UNKNOWN: s = "Unavailable"
        Case Else: s = "Unknown (" & CStr(Code) & ")"
    End Select

    ApiDispQuotaState = s
End Function
Public Function ApiDispDriveType(DriveType As PT_DRIVE_TYPE) As String
    Dim s As String

    Select Case DriveType
        Case DRIVE_NO_ROOT_DIR: s = "Invalid Drive Letter"
        Case DRIVE_REMOVABLE:   s = "Removable"
        Case DRIVE_FIXED:   s = "Fixed"
        Case DRIVE_REMOTE:  s = "Network"
        Case DRIVE_CDROM:   s = "CD/DVD"
        Case DRIVE_RAMDISK: s = "RAM Drive"
        Case Else:  s = "Unknown Drive Type"
    End Select

    ApiDispDriveType = s
End Function
Public Function ApiDispShareType(Code As Long) As String
    Dim s As String

    Select Case Code
        Case STYPE_DEVICE: s = "Device Share"
        Case STYPE_DFS: s = "DFS" ' Distributed File System
        Case STYPE_DISKTREE: s = "Disk Share"
        Case STYPE_IPC: s = "Shared Pipe" ' Inter-Program Communication (Pipe)
        Case STYPE_PRINTQ: s = "Printer Share"
        Case Else:  s = "Unknown Share Type"
    End Select
    If Code And STYPE_SPECIAL Then s = "Hidden " & s

    ApiDispShareType = s
End Function

