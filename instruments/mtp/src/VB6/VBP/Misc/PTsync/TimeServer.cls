VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "TimeServer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Attribute VB_Ext_KEY = "Member0" ,"TimeZone"
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/
' Version 1.2 9/07/2004

Public Enum DAYTIME_FORMAT
    DAYTIME_DEFAULT = -1
    DAYTIME_LBOUND = 0
    DAYTIME_UNKNOWN = DAYTIME_LBOUND
    DAYTIME_NIST
    DAYTIME_SMTP
    DAYTIME_DOW_MMM_DD_YYY_TZ
    DAYTIME_LOCAL
    DAYTIME_KAREN
    DAYTIME_UBOUND = DAYTIME_KAREN
End Enum

Private Const SERIALIZE_VERSION = 1

Private Enum V0
    POS_Version = 0
    POS_Enabled
    POS_CountryCode
    POS_Country
    POS_State
    POS_HostName
    POS_Location
    POS_Coords
    POS_ServiceArea
    POS_Policy
    POS_SyncProto
    POS_ServerHardware
    POS_Contact

    POS_Tag
    DUMMY2
    DUMMY3
    DUMMY4
    DUMMY5

    POS_SntpSupported
    POS_Stratum
    SNTP_DUMMY1
    SNTP_DUMMY2
    SNTP_DUMMY3
    SNTP_DUMMY4
    SNTP_DUMMY5

    POS_TimeSupported
    TIME_DUMMY1
    TIME_DUMMY2
    TIME_DUMMY3

    POS_DaytimeSupported
    POS_DaytimeFormat
    DAYTIME_DUMMY1
    DAYTIME_DUMMY2
    DAYTIME_DUMMY3
    DAYTIME_DUMMY4

    POS_RoundTrip
    DUMMY6
    DUMMY7
    DUMMY8
    DUMMY9
    DUMMY10
    POS_Max = DUMMY10
End Enum

Private Enum V1
    POS_Version = 0
    POS_Enabled
    POS_Country
    POS_State
    POS_City
    POS_HostName
    POS_Stratum
    POS_Location
    POS_Coords
    POS_ServiceArea
    POS_Policy
    POS_Contact
    POS_SyncProto
    POS_SyncHardware
    POS_ServerOpSys
    POS_ServerHardware

    POS_Tag
    DUMMY2
    DUMMY3
    DUMMY4
    DUMMY5

    POS_SntpSupported
    SNTP_DUMMY1
    SNTP_DUMMY2
    SNTP_DUMMY3
    SNTP_DUMMY4
    SNTP_DUMMY5

    POS_TimeSupported
    TIME_DUMMY1
    TIME_DUMMY2
    TIME_DUMMY3

    POS_DaytimeSupported
    POS_DaytimeFormat
    DAYTIME_DUMMY1
    DAYTIME_DUMMY2
    DAYTIME_DUMMY3
    DAYTIME_DUMMY4

    POS_RoundTrip
    DUMMY6
    DUMMY7
    DUMMY8
    DUMMY9
    DUMMY10
    POS_Max = DUMMY10
End Enum

Private mvarIndex As Long
Private mvarDeleted As Long
Private mvarEnabled As Boolean
Private mvarCountry As String
Private mvarState As String
Private mvarCity As String
Private mvarTag As String
Private mvarHostName As String
Private mvarCoords As String
Private mvarServiceArea As String
Private mvarPolicy As String
Private mvarSyncProto As String
Private mvarSyncHardware As String
Private mvarServerHardware As String
Private mvarServerOpSys As String
Private mvarContact As String
Private mvarStratum As Long
Private mvarSntpSupported As Boolean
Private mvarTimeSupported As Boolean
Private mvarDaytimeSupported As Boolean
Private mvarDaytimeFormat As DAYTIME_FORMAT
Private mvarRoundTrip As Long
Private mvarLocation As String
Private mvarTimeZone As TimeZone
Public Property Get TimeZone() As TimeZone
    If mvarTimeZone Is Nothing Then
        Set mvarTimeZone = New TimeZone
    End If

    Set TimeZone = mvarTimeZone
End Property
Public Property Set TimeZone(vData As TimeZone)
    Set mvarTimeZone = vData
End Property
Private Sub Class_Initialize()
    mvarIndex = -1
    mvarEnabled = True
    mvarDeleted = False
    mvarSntpSupported = True
End Sub
Private Sub Class_Terminate()
    Set mvarTimeZone = Nothing
End Sub
Public Function Equals(Server As TimeServer) As Boolean
Attribute Equals.VB_Description = "Are these the same server?"
    If StrComp(mvarHostName, Server.HostName, vbTextCompare) > 0 Then
        Equals = True
    Else
        Equals = False
    End If
End Function
Public Property Get Index() As Long
    Index = mvarIndex
End Property
Public Property Let Index(NewValue As Long)
    mvarIndex = NewValue
End Property
Public Property Get IpAddr() As String
Attribute IpAddr.VB_Description = "IP Address of this server (read-only)."
    IpAddr = SockHostDQ(mvarHostName)
End Property
Private Function SerialDelim(SerialFormat As SERIAL_FORMAT) As String
    Select Case SerialFormat
        Case SERIAL_FORMAT_TSV:
            SerialDelim = vbTab
        Case SERIAL_FORMAT_CSV:
            SerialDelim = ","
        Case SERIAL_FORMAT_XML:
            ' not supported yet
        Case Else:
            SerialDelim = vbTab
    End Select
End Function
Public Property Let Serialize(Optional SerialFormat As SERIAL_FORMAT = SERIAL_FORMAT_TSV, SerialJob As String)
Attribute Serialize.VB_Description = "Convert server information to/from a text string."
    Dim sa() As String
    Dim ver As Long

    ' convert string to object
    If Len(SerialJob) <= 0 Then
        Err.Raise SERIAL_EMPTY
        Exit Property
    End If

    sa = Split(SerialJob, SerialDelim(SerialFormat))
    If Not IsNumeric(sa(V0.POS_Version)) Then
        Err.Raise SERIAL_BAD_VERSION
        Exit Property
    End If

    Select Case CLng(sa(V0.POS_Version))
        Case 0
            mvarEnabled = sa(V0.POS_Enabled)
'            mvarCountryCode = sa(V0.POS_CountryCode)
            mvarCountry = sa(V0.POS_Country)
            mvarState = sa(V0.POS_State)
            mvarHostName = sa(V0.POS_HostName)
            mvarLocation = sa(V0.POS_Location)
            mvarCoords = sa(V0.POS_Coords)
            mvarServiceArea = sa(V0.POS_ServiceArea)
            mvarPolicy = sa(V0.POS_Policy)
            mvarSyncProto = sa(V0.POS_SyncProto)
            mvarServerHardware = sa(V0.POS_ServerHardware)
            mvarContact = sa(V0.POS_Contact)
            mvarTag = sa(V0.POS_Tag)
            mvarSntpSupported = CBool(sa(V0.POS_SntpSupported))
            If IsNumeric(sa(V0.POS_Stratum)) Then
                mvarStratum = CLng(sa(V0.POS_Stratum))
            Else
                mvarStratum = -1
            End If
            mvarTimeSupported = CBool(sa(V0.POS_TimeSupported))
            mvarDaytimeSupported = CBool(sa(V0.POS_DaytimeSupported))
            mvarDaytimeFormat = CLng(sa(V0.POS_DaytimeFormat))
            If IsNumeric(sa(V0.POS_RoundTrip)) Then
                mvarRoundTrip = CLng(sa(V0.POS_RoundTrip))
            Else
                mvarRoundTrip = -1
            End If
        
        Case 1
            mvarEnabled = sa(V1.POS_Enabled)
            mvarCountry = sa(V1.POS_Country)
            mvarState = sa(V1.POS_State)
            mvarHostName = sa(V1.POS_HostName)
            mvarLocation = sa(V1.POS_Location)
            mvarCoords = sa(V1.POS_Coords)
            mvarServiceArea = sa(V1.POS_ServiceArea)
            mvarPolicy = sa(V1.POS_Policy)
            mvarSyncProto = sa(V1.POS_SyncProto)
            mvarSyncHardware = sa(V1.POS_SyncHardware)
            mvarServerOpSys = sa(V1.POS_ServerOpSys)
            mvarServerHardware = sa(V1.POS_ServerHardware)
            mvarContact = sa(V1.POS_Contact)
            mvarTag = sa(V1.POS_Tag)
            mvarSntpSupported = CBool(sa(V1.POS_SntpSupported))
            If IsNumeric(sa(V1.POS_Stratum)) Then
                mvarStratum = CLng(sa(V1.POS_Stratum))
            Else
                mvarStratum = -1
            End If
            mvarTimeSupported = CBool(sa(V1.POS_TimeSupported))
            mvarDaytimeSupported = CBool(sa(V1.POS_DaytimeSupported))
            mvarDaytimeFormat = CLng(sa(V1.POS_DaytimeFormat))
            If IsNumeric(sa(V1.POS_RoundTrip)) Then
                mvarRoundTrip = CLng(sa(V1.POS_RoundTrip))
            Else
                mvarRoundTrip = -1
            End If

        Case Else
            Err.Raise SERIAL_BAD_VERSION
            Exit Property
    End Select

End Property
Public Property Get Serialize(Optional SerialFormat As SERIAL_FORMAT = SERIAL_FORMAT_TSV) As String
    Dim sa(V1.POS_Max) As String

    ' convert object to string
    sa(V1.POS_Version) = CStr(SERIALIZE_VERSION)
    sa(V1.POS_Enabled) = CStr(CLng(mvarEnabled))
    sa(V1.POS_Country) = mvarCountry
    sa(V1.POS_State) = mvarState
    sa(V1.POS_City) = mvarCity
    sa(V1.POS_HostName) = mvarHostName
    sa(V1.POS_Location) = mvarLocation
    sa(V1.POS_Coords) = mvarCoords
    sa(V1.POS_ServiceArea) = mvarServiceArea
    sa(V1.POS_Policy) = mvarPolicy
    sa(V1.POS_SyncProto) = mvarSyncProto
    sa(V1.POS_SyncHardware) = mvarSyncHardware
    sa(V1.POS_ServerOpSys) = mvarServerOpSys
    sa(V1.POS_ServerHardware) = mvarServerHardware
    sa(V1.POS_Contact) = mvarContact
    sa(V1.POS_Tag) = mvarTag
    sa(V1.POS_SntpSupported) = CStr(CLng(mvarSntpSupported))
    sa(V1.POS_Stratum) = mvarStratum
    sa(V1.POS_TimeSupported) = CStr(CLng(mvarTimeSupported))
    sa(V1.POS_DaytimeSupported) = CStr(CLng(mvarDaytimeSupported))
    sa(V1.POS_DaytimeFormat) = CStr(mvarDaytimeFormat)
    sa(V1.POS_RoundTrip) = CStr(mvarRoundTrip)

    Serialize = Join(sa, SerialDelim(SerialFormat))
End Property
Public Function Clone() As TimeServer
    Dim s As String
    Dim ts As TimeServer

    s = Me.Serialize(SERIAL_FORMAT_TSV)
    Set ts = New TimeServer
    ts.Serialize(SERIAL_FORMAT_TSV) = s
    Set Clone = ts
End Function
Public Property Let Location(ByVal vData As String)
Attribute Location.VB_Description = "Location (city, state, address, university, etc.) of this server."
    mvarLocation = vData
End Property
Public Property Get Location() As String
    If Len(mvarLocation) > 0 Then
        Location = mvarLocation
    Else
        Location = mvarCountry
    End If
End Property
Public Property Let RoundTrip(ByVal vData As Long)
Attribute RoundTrip.VB_Description = "Most recent round trip time, in milliseconds."
    mvarRoundTrip = vData
End Property
Public Property Get RoundTrip() As Long
    RoundTrip = mvarRoundTrip
End Property
Public Function DispDaytimeFormat(Optional ByVal FormatCode As DAYTIME_FORMAT = DAYTIME_DEFAULT)
    Dim s As String
    Dim fmt As DAYTIME_FORMAT

    If FormatCode = DAYTIME_DEFAULT Then
        fmt = mvarDaytimeFormat
    Else
        fmt = FormatCode
    End If

    Select Case fmt
        Case DAYTIME_UNKNOWN
            s = "Unknown"
        Case DAYTIME_NIST
            s = "NIST"
        Case DAYTIME_SMTP
            s = "SMTP"
        Case DAYTIME_DOW_MMM_DD_YYY_TZ
            s = "DOW Month Day Year TZ"
        Case DAYTIME_LOCAL
            s = "Local Time"
        Case DAYTIME_KAREN
            s = "Karen's NIST Daytime Format"
        Case Else
            s = "Unknown"
    End Select

    DispDaytimeFormat = s
End Function
Public Property Let DaytimeFormat(ByVal vData As DAYTIME_FORMAT)
Attribute DaytimeFormat.VB_Description = "Code indicating format of Daytime string returned by this server."
    mvarDaytimeFormat = vData
End Property
Public Property Get DaytimeFormat() As DAYTIME_FORMAT
    If mvarDaytimeSupported Then
        DaytimeFormat = mvarDaytimeFormat
    Else
        DaytimeFormat = 0
    End If
End Property
Public Property Let DaytimeSupported(ByVal vData As Boolean)
Attribute DaytimeSupported.VB_Description = "Does this server support the Daytime protocol?"
    mvarDaytimeSupported = vData
End Property
Public Property Get DaytimeSupported() As Boolean
    DaytimeSupported = mvarDaytimeSupported
End Property
Public Property Let TimeSupported(ByVal vData As Boolean)
Attribute TimeSupported.VB_Description = "Does this server support the Time protocol?"
    mvarTimeSupported = vData
End Property
Public Property Get TimeSupported() As Boolean
    TimeSupported = mvarTimeSupported
End Property
Public Property Let SntpSupported(ByVal vData As Boolean)
Attribute SntpSupported.VB_Description = "Does this server support the SNTP Protocol?"
    mvarSntpSupported = vData
End Property
Public Property Get SntpSupported() As Boolean
    SntpSupported = mvarSntpSupported
End Property
Public Property Let Stratum(ByVal vData As Long)
Attribute Stratum.VB_Description = "Server's stratum."
    mvarStratum = vData
End Property
Public Property Get Stratum() As Long
    Stratum = mvarStratum
End Property
Public Property Let Contact(ByVal vData As String)
Attribute Contact.VB_Description = "Contact person for this server."
    mvarContact = vData
End Property
Public Property Get Contact() As String
    Contact = mvarContact
End Property
Public Property Let ServerHardware(ByVal vData As String)
Attribute ServerHardware.VB_Description = "Server's hardware and software configuration."
    mvarServerHardware = vData
End Property
Public Property Get ServerHardware() As String
    ServerHardware = mvarServerHardware
End Property
Public Property Let ServerOpSys(ByVal vData As String)
    mvarServerOpSys = vData
End Property
Public Property Get ServerOpSys() As String
    ServerOpSys = mvarServerOpSys
End Property
Public Property Let SyncProto(ByVal vData As String)
Attribute SyncProto.VB_Description = "Server's Synchronization method."
    mvarSyncProto = vData
End Property
Public Property Get SyncProto() As String
    SyncProto = mvarSyncProto
End Property
Public Property Let SyncHardware(ByVal vData As String)
    mvarSyncHardware = vData
End Property
Public Property Get SyncHardware() As String
    SyncHardware = mvarSyncHardware
End Property
Public Property Let Policy(ByVal vData As String)
Attribute Policy.VB_Description = "Server's access policy."
    mvarPolicy = vData
End Property
Public Property Get Policy() As String
    Policy = mvarPolicy
End Property
Public Property Let ServiceArea(ByVal vData As String)
Attribute ServiceArea.VB_Description = "Area served by this time server."
    mvarServiceArea = vData
End Property
Public Property Get ServiceArea() As String
    If Len(mvarServiceArea) > 0 Then
        ServiceArea = mvarServiceArea
    Else
        ServiceArea = Location
    End If
End Property
Public Property Let Coords(ByVal vData As String)
Attribute Coords.VB_Description = "Server's geographical coordinates (longitude and latitude)."
    mvarCoords = vData
End Property
Public Property Get Coords() As String
    Coords = mvarCoords
End Property
Public Property Let HostName(ByVal vData As String)
Attribute HostName.VB_Description = "DNS hostname of this server."
    mvarHostName = vData
End Property
Public Property Get HostName() As String
    HostName = mvarHostName
End Property
Public Property Let Deleted(ByVal vData As Boolean)
    mvarDeleted = vData
End Property
Public Property Get Deleted() As Boolean
    Deleted = mvarDeleted
End Property
Public Property Let Tag(ByVal vData As String)
    mvarTag = vData
End Property
Public Property Get Tag() As String
    Tag = mvarTag
End Property
Public Property Let Country(ByVal vData As String)
Attribute Country.VB_Description = "Name of country where server is located."
    mvarCountry = vData
End Property
Public Property Get Country() As String
    Country = mvarCountry
End Property
Public Property Let State(ByVal vData As String)
Attribute State.VB_Description = "If server is in the United States, the state where the server resides."
    mvarState = vData
End Property
Public Property Get State() As String
    State = mvarState
End Property
Public Property Let City(ByVal vData As String)
    mvarCity = vData
End Property
Public Property Get City() As String
    City = mvarCity
End Property
Public Property Let Enabled(ByVal vData As Boolean)
Attribute Enabled.VB_Description = "Is this server available?"
    mvarEnabled = vData
End Property
Public Property Get Enabled() As Boolean
    Enabled = mvarEnabled
End Property



