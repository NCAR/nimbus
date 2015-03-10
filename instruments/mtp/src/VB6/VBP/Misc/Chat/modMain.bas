Attribute VB_Name = "Module1"
'glFirstLocalPort is the first port to be used by the host computer.
'The host computer will use all ports from glFirstLocalPort to (glFirstLocalPort + giMaxConnections - 1).
Global glFirstLocalPort As Long

'glFirstRemotePort is the first port to be used by connecting computers.
'The connecting computers will use all ports from glFirstRemotePort to (glFirstRemotePort + giMaxConnections - 1).
Global glFirstRemotePort As Long

'giMaxConnections defines the maximum number of connections that can be made to the server.
'Problems occur when using larger numbers.  I would not recommend using more than 16.
Global giMaxConnections As Integer

'API calls used for reading and writing of preferences
Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) As Long

