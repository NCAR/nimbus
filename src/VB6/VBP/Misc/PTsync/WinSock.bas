Attribute VB_Name = "SockHelper"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 1.1 5/13/2004

Private Const WSADESCRIPTION_LEN = 256 + 1
Private Const WSASYS_STATUS_LEN = 128 + 1
Private Const SOMAXCONN = &H7FFFFFFF

Private Const WSA_E_CANCELLED = 10111&
Private Const WSA_E_NO_MORE = 10110&
Private Const WSABASEERR = 10000
Private Const WSAEACCES = 10013&
Private Const WSAEADDRINUSE = 10048&
Private Const WSAEADDRNOTAVAIL = 10049&
Private Const WSAEAFNOSUPPORT = 10047&
Private Const WSAEALREADY = 10037&
Private Const WSAEBADF = 10009&
Private Const WSAECANCELLED = 10103&
Private Const WSAECONNABORTED = 10053&
Private Const WSAECONNREFUSED = 10061&
Private Const WSAECONNRESET = 10054&
Private Const WSAEDESTADDRREQ = 10039&
Private Const WSAEDISCON = 10101&
Private Const WSAEDQUOT = 10069&
Private Const WSAEFAULT = 10014&
Private Const WSAEHOSTDOWN = 10064&
Private Const WSAEHOSTUNREACH = 10065&
Private Const WSAEINPROGRESS = 10036&
Private Const WSAEINTR = 10004&
Private Const WSAEINVAL = 10022&
Private Const WSAEINVALIDPROCTABLE = 10104&
Private Const WSAEINVALIDPROVIDER = 10105&
Private Const WSAELOOP = 10062&
Private Const WSAEMFILE = 10024&
Private Const WSAEMSGPARTIAL = (WSABASEERR + 100)
Private Const WSAEMSGSIZE = 10040&
Private Const WSAENAMETOOLONG = 10063&
Private Const WSAENETDOWN = 10050&
Private Const WSAENETRESET = 10052&
Private Const WSAENETUNREACH = 10051&
Private Const WSAENOBUFS = 10055&
Private Const WSAENOMORE = 10102&
Private Const WSAENOPROTOOPT = 10042&
Private Const WSAENOTCONN = 10057&
Private Const WSAENOTEMPTY = 10066&
Private Const WSAENOTSOCK = 10038&
Private Const WSAEOPNOTSUPP = 10045&
Private Const WSAEPFNOSUPPORT = 10046&
Private Const WSAEPROCLIM = 10067&
Private Const WSAEPROTONOSUPPORT = 10043&
Private Const WSAEPROTOTYPE = 10041&
Private Const WSAEPROVIDERFAILEDINIT = (WSABASEERR + 106)
Private Const WSAEREFUSED = 10112&
Private Const WSAEREMOTE = 10071&
Private Const WSAESHUTDOWN = 10058&
Private Const WSAESOCKTNOSUPPORT = 10044&
Private Const WSAESTALE = 10070&
Private Const WSAETIMEDOUT = 10060&
Private Const WSAETOOMANYREFS = 10059&
Private Const WSAEUSERS = 10068&
Private Const WSAEWOULDBLOCK = 10035&

Private Const WSAHOST_NOT_FOUND = 11001&
Private Const WSANO_DATA = 11004&
Private Const WSANO_ADDRESS = WSANO_DATA
Private Const WSANO_RECOVERY = 11003&
Private Const WSANOTINITIALISED = 10093&
Private Const WSASERVICE_NOT_FOUND = 10108&
Private Const WSASYSCALLFAILURE = 10107&
Private Const WSASYSNOTREADY = 10091&
Private Const WSATRY_AGAIN = 11002&
Private Const WSATYPE_NOT_FOUND = 10109&

' socket types
Private Const SOCK_DGRAM = 2
Private Const SOCK_RAW = 3
Private Const SOCK_RDM = 4
Private Const SOCK_SEQPACKET = 5
Private Const SOCK_STREAM = 1

' Protocols
Private Const IPPROTO_IP = 0                           ' dummy for IP '
Private Const IPPROTO_HOPOPTS = 0                      ' IPv6 hop-by-hop options '
Private Const IPPROTO_ICMP = 1                         ' control message protocol '
Private Const IPPROTO_IGMP = 2                         ' internet group management protocol '
Private Const IPPROTO_GGP = 3                          ' gateway^2 (deprecated) '
Private Const IPPROTO_IPV4 = 4                         ' IPv4 '
Private Const IPPROTO_TCP = 6                          ' tcp '
Private Const IPPROTO_PUP = 12                         ' pup '
Private Const IPPROTO_UDP = 17                         ' user datagram protocol '
Private Const IPPROTO_IDP = 22                         ' xns idp '
Private Const IPPROTO_IPV6 = 41                        ' IPv6 '
Private Const IPPROTO_ROUTING = 43                     ' IPv6 routing header '
Private Const IPPROTO_FRAGMENT = 44                    ' IPv6 fragmentation header '
Private Const IPPROTO_ESP = 50                         ' IPsec ESP header '
Private Const IPPROTO_AH = 51                          ' IPsec AH '
Private Const IPPROTO_ICMPV6 = 58                      ' ICMPv6 '
Private Const IPPROTO_NONE = 59                        ' IPv6 no next header '
Private Const IPPROTO_DSTOPTS = 60                     ' IPv6 destination options '
Private Const IPPROTO_ND = 77                          ' UNOFFICIAL net disk proto '
Private Const IPPROTO_ICLFXBM = 78

Private Const IPPROTO_RAW = 255                        ' raw IP packet '
Private Const IPPROTO_MAX = 256

' Port/socket numbers: network standard functions
Private Const IPPORT_ECHO = 7
Private Const IPPORT_DISCARD = 9
Private Const IPPORT_SYSTAT = 11
Private Const IPPORT_DAYTIME = 13
Private Const IPPORT_NETSTAT = 15
Private Const IPPORT_FTP = 21
Private Const IPPORT_TELNET = 23
Private Const IPPORT_SMTP = 25
Private Const IPPORT_TIMESERVER = 37
Private Const IPPORT_SNTP = 123
Private Const IPPORT_NAMESERVER = 42
Private Const IPPORT_WHOIS = 43
Private Const IPPORT_MTP = 57

' Port/socket numbers: host specific functions
Private Const IPPORT_TFTP = 69
Private Const IPPORT_RJE = 77
Private Const IPPORT_FINGER = 79
Private Const IPPORT_TTYLINK = 87
Private Const IPPORT_SUPDUP = 95

' UNIX TCP sockets
Private Const IPPORT_EXECSERVER = 512
Private Const IPPORT_LOGINSERVER = 513
Private Const IPPORT_CMDSERVER = 514
Private Const IPPORT_EFSSERVER = 520

' UNIX UDP sockets
Private Const IPPORT_BIFFUDP = 512
Private Const IPPORT_WHOSERVER = 513
Private Const IPPORT_ROUTESERVER = 520

' recv flags
Private Const MSG_PEEK = &H2
Private Const MSG_OOB = &H1

' address families
Private Const AF_INET = 2
Private Const AF_INET6 = 23
Private Const AF_IRDA = 26

Private Type HOSTENT
    hName As Long
    hAliases As Long
    hAddrType As Integer
    hLength As Integer
    hAddrList As Long
End Type

Private Type WSADATA
    wVersion As Integer
    wHighVersion As Integer
    szDescription As String * WSADESCRIPTION_LEN
    szSystemStatus As String * WSASYS_STATUS_LEN
    iMaxSockets As Integer
    iMaxUdpDg As Integer
    lpVendorInfo As Long
End Type

Private Type ADDR_INET
    Addr As Long
'    B1 As Byte ' alternate layout
'    B2 As Byte
'    B3 As Byte
'    B4 As Byte
End Type

Private Type ADDR_INET6
    b(15) As Byte
'    W(7) As Integer ' alternate layout
End Type

' generic SOCKADDR definition
Private Type SOCKADDR
    Family As Integer
    Data(13) As Byte
End Type

Private Type SOCKADDR_INET
    Family As Integer ' AF_INET
    Port As Integer
    Addr As Long ' ADDR_INET
    Zero(7) As Byte
End Type

Private Type SOCKADDR_INET6
    Family As Integer ' AF_INET6
    Port As Integer
    Flowinfo As Long
    Addr As ADDR_INET6
    Scope_id As Long
End Type

Private Type SOCKADDR_IRDA
    Family As Integer ' AF_IRDA
    DeviceID(3) As Byte
    ServiceName(24) As Byte
End Type

Private Declare Function WSAStartup Lib "ws2_32.dll" ( _
     ByVal wVersionRequired As Integer, _
     ByRef lpWSAData As WSADATA) As Long

Private Declare Function WSACleanup Lib "ws2_32.dll" () As Long

Private Declare Function gethostbyname Lib "ws2_32.dll" ( _
     ByVal Name As String) As Long

Private Declare Function inet_ntoa Lib "ws2_32.dll" ( _
    ByVal inaddr As Long) As Long

Private Declare Function inet_addr Lib "ws2_32.dll" _
    (ByVal DQ As String) As Long

Private Declare Function htonl Lib "ws2_32.dll" ( _
     ByVal hostlong As Long) As Long

Private Declare Function htons Lib "ws2_32.dll" ( _
     ByVal hostshort As Integer) As Integer

Private Declare Function ntohl Lib "ws2_32.dll" ( _
     ByVal netlong As Long) As Long

Private Declare Function ntohs Lib "ws2_32.dll" ( _
     ByVal netshort As Integer) As Integer

Private Declare Function Socket Lib "ws2_32.dll" Alias "socket" ( _
     ByVal af As Long, _
     ByVal lType As Long, _
     ByVal Protocol As Long) As Long

Private Declare Function Listen Lib "ws2_32.dll" Alias "listen" ( _
     ByVal s As Long, _
     ByVal backlog As Long) As Long

Private Declare Function Accept Lib "ws2_32.dll" Alias "accept" ( _
     ByVal Socket As Long, _
     ByRef Addr As SOCKADDR_INET, _
     ByRef addrlen As Long) As Long

Private Declare Function Connect Lib "ws2_32.dll" Alias "connect" ( _
     ByVal s As Long, _
     ByRef Name As SOCKADDR, _
     ByVal namelen As Long) As Long

Private Declare Function Send Lib "ws2_32.dll" Alias "send" ( _
     ByVal s As Long, _
     ByVal buf As String, _
     ByVal lLen As Long, _
     ByVal Flags As Long) As Long

Private Declare Function Recv Lib "ws2_32.dll" Alias "recv" ( _
     ByVal s As Long, _
     ByVal buf As String, _
     ByVal lLen As Long, _
     ByVal Flags As Long) As Long

Private Declare Function closesocket Lib "ws2_32.dll" ( _
     ByVal s As Long) As Long

Private InitDone As Boolean
Public Function SockInit() As Boolean
    Dim result As Long
    Dim WsaInfo As WSADATA

    SockInit = False
    result = WSAStartup(&H101, WsaInfo)
    If result <> 0 Then ' error
        MsgBox "Windows' Network Socket Services Are Not Available", vbCritical Or vbOKOnly, App.FileDescription
        Exit Function
    End If

    SockInit = True
End Function
Public Sub SockFini()
    WSACleanup
End Sub
Public Function SockHostDQ(HostName As String) As String
    Dim IpAddr As Long

    If Not InitDone Then SockInit

    IpAddr = SockHostIpAddr(HostName)
    SockHostDQ = SockIpAddrDQ(IpAddr)
End Function
Public Function SockHostIpAddr(HostName As String) As Long
    Dim IpAddr As Long
    Dim HostEntry As HOSTENT
    Dim Ptr As Long

    If Not InitDone Then SockInit

    Ptr = gethostbyname(HostName)
    If Ptr = 0 Then ' failure
        SockHostIpAddr = 0
        Exit Function
    End If

    ApiMemoryCopy VarPtr(HostEntry), Ptr, LenB(HostEntry)
    ApiMemoryCopy VarPtr(Ptr), HostEntry.hAddrList, 4
    ApiMemoryCopy VarPtr(IpAddr), Ptr, HostEntry.hLength

    SockHostIpAddr = IpAddr
End Function
Public Function SockIpAddrDQ(IpAddr As Long) As String
    Dim s As String
    Dim Ptr As Long

    If Not InitDone Then SockInit

    Ptr = inet_ntoa(IpAddr)
    s = ApiTextCopy(Ptr, PT_TEXT_ASCII)
    SockIpAddrDQ = s
End Function
Public Function SockDqIpAddr(DQ As String) As Long

    If Not InitDone Then SockInit
    
    SockDqIpAddr = inet_addr(DQ)
End Function
Public Function SockAccept(Socket As Long) As Long
    Dim sa As SOCKADDR_INET

    If Not InitDone Then SockInit

    SockAccept = Accept(Socket, sa, LenB(sa))
End Function
Public Function SockListen(Socket As Long) As Long

    If Not InitDone Then SockInit
    
    SockListen = Listen(Socket, SOMAXCONN)
End Function
Private Sub Sample()
    Dim ListenSocket As Long
    Dim saServer As SOCKADDR_INET
    Dim LocalHost As String
    Dim LocalIP As String

    ' Create a listening socket
    ListenSocket = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)

    ' Get the local host information
    LocalHost = gethostbyname("")
'    localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

    ' Set up the sockaddr structure
    saServer.Family = AF_INET
    saServer.Addr = inet_addr(LocalIP)
    saServer.Port = htons(5150)

'    ' Bind the listening socket using the
'    ' information in the sockaddr structure
'    bind( ListenSocket,(SOCKADDR*) &saServer, sizeof(saServer) );
End Sub

