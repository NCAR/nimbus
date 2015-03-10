VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form frmEdit 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Edit Time Servers"
   ClientHeight    =   6000
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   8640
   Icon            =   "Edit.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6000
   ScaleWidth      =   8640
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer timOutClient 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   1560
      Top             =   5280
   End
   Begin VB.CommandButton btnSave 
      Caption         =   "&Save"
      Height          =   375
      Left            =   7080
      TabIndex        =   40
      Top             =   4560
      Width           =   1095
   End
   Begin VB.CommandButton btnCancel 
      Caption         =   "&Cancel"
      Height          =   375
      Left            =   5880
      TabIndex        =   39
      Top             =   4560
      Width           =   1095
   End
   Begin VB.CommandButton btnHelp 
      Caption         =   "&Help"
      Height          =   375
      Left            =   240
      TabIndex        =   41
      Top             =   5280
      Visible         =   0   'False
      Width           =   1095
   End
   Begin TabDlg.SSTab tabEdit 
      Height          =   2655
      Left            =   240
      TabIndex        =   6
      Top             =   2400
      Width           =   8175
      _ExtentX        =   14420
      _ExtentY        =   4683
      _Version        =   393216
      Tab             =   1
      TabsPerRow      =   4
      TabHeight       =   520
      TabCaption(0)   =   " Main Information "
      TabPicture(0)   =   "Edit.frx":212A
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "lblHostName"
      Tab(0).Control(1)=   "lblState"
      Tab(0).Control(2)=   "lblCountry"
      Tab(0).Control(3)=   "lblServiceArea"
      Tab(0).Control(4)=   "lblPolicy"
      Tab(0).Control(5)=   "lblContact"
      Tab(0).Control(6)=   "cboCountry"
      Tab(0).Control(7)=   "txtHostName"
      Tab(0).Control(8)=   "cboState"
      Tab(0).Control(9)=   "cboServiceArea"
      Tab(0).Control(10)=   "cboPolicy"
      Tab(0).Control(11)=   "cboContact"
      Tab(0).Control(12)=   "chkEnabled"
      Tab(0).ControlCount=   13
      TabCaption(1)   =   " Protocols Supported "
      TabPicture(1)   =   "Edit.frx":2146
      Tab(1).ControlEnabled=   -1  'True
      Tab(1).Control(0)=   "lblDaytimeFormat"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "chkDaytime"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "cboDaytimeFormat"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "chkSNTP"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "chkTime"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).ControlCount=   5
      TabCaption(2)   =   " Additional Information "
      TabPicture(2)   =   "Edit.frx":2162
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "lblSyncProto"
      Tab(2).Control(1)=   "lblStratum"
      Tab(2).Control(2)=   "lblServerHardware"
      Tab(2).Control(3)=   "lblCoords"
      Tab(2).Control(4)=   "lblLocation"
      Tab(2).Control(5)=   "lblServerOpSys"
      Tab(2).Control(6)=   "lblSyncHardware"
      Tab(2).Control(7)=   "cboStratum"
      Tab(2).Control(8)=   "cboSyncProto"
      Tab(2).Control(9)=   "cboServerHardware"
      Tab(2).Control(10)=   "cboLocation"
      Tab(2).Control(11)=   "cboCoords"
      Tab(2).Control(12)=   "cboServerOpSys"
      Tab(2).Control(13)=   "cboSyncHardware"
      Tab(2).ControlCount=   14
      Begin VB.CheckBox chkEnabled 
         Caption         =   "Enabled"
         Height          =   255
         Left            =   -69240
         TabIndex        =   9
         Top             =   480
         Width           =   1935
      End
      Begin VB.ComboBox cboSyncHardware 
         Height          =   315
         Left            =   -69720
         Sorted          =   -1  'True
         TabIndex        =   32
         Top             =   840
         Width           =   2655
      End
      Begin VB.ComboBox cboServerOpSys 
         Height          =   315
         Left            =   -73560
         Sorted          =   -1  'True
         TabIndex        =   26
         Top             =   480
         Width           =   2655
      End
      Begin VB.ComboBox cboContact 
         Height          =   315
         Left            =   -73560
         Sorted          =   -1  'True
         TabIndex        =   19
         Top             =   2040
         Width           =   3855
      End
      Begin VB.ComboBox cboPolicy 
         Height          =   315
         Left            =   -73560
         Sorted          =   -1  'True
         TabIndex        =   17
         Top             =   1680
         Width           =   6495
      End
      Begin VB.ComboBox cboServiceArea 
         Height          =   315
         Left            =   -73560
         Sorted          =   -1  'True
         TabIndex        =   15
         Top             =   1200
         Width           =   6495
      End
      Begin VB.ComboBox cboCoords 
         Height          =   315
         Left            =   -73560
         Sorted          =   -1  'True
         TabIndex        =   38
         Top             =   2040
         Width           =   3855
      End
      Begin VB.ComboBox cboLocation 
         Height          =   315
         Left            =   -73560
         Sorted          =   -1  'True
         TabIndex        =   36
         Top             =   1680
         Width           =   6495
      End
      Begin VB.ComboBox cboServerHardware 
         Height          =   315
         Left            =   -69720
         Sorted          =   -1  'True
         TabIndex        =   28
         Top             =   480
         Width           =   2655
      End
      Begin VB.ComboBox cboSyncProto 
         Height          =   315
         Left            =   -73560
         Sorted          =   -1  'True
         TabIndex        =   30
         Top             =   840
         Width           =   2655
      End
      Begin VB.CheckBox chkTime 
         Caption         =   "Time Protocol"
         Height          =   255
         Left            =   360
         TabIndex        =   21
         Top             =   960
         Width           =   1575
      End
      Begin VB.CheckBox chkSNTP 
         Caption         =   "SNTP (Simple Network Time Protocol)"
         Height          =   255
         Left            =   360
         TabIndex        =   20
         Top             =   480
         Width           =   4095
      End
      Begin VB.ComboBox cboDaytimeFormat 
         Enabled         =   0   'False
         Height          =   315
         Left            =   600
         Style           =   2  'Dropdown List
         TabIndex        =   24
         Top             =   2040
         Width           =   2775
      End
      Begin VB.CheckBox chkDaytime 
         Caption         =   "Daytime Protocol"
         Height          =   255
         Left            =   360
         TabIndex        =   22
         Top             =   1440
         Width           =   2295
      End
      Begin VB.ComboBox cboStratum 
         Height          =   315
         Left            =   -73560
         Style           =   2  'Dropdown List
         TabIndex        =   34
         Top             =   1200
         Width           =   1215
      End
      Begin VB.ComboBox cboState 
         Height          =   315
         Left            =   -69240
         Sorted          =   -1  'True
         TabIndex        =   13
         Top             =   840
         Width           =   2175
      End
      Begin VB.TextBox txtHostName 
         Height          =   285
         Left            =   -73560
         TabIndex        =   8
         Top             =   480
         Width           =   3015
      End
      Begin VB.ComboBox cboCountry 
         Height          =   315
         Left            =   -73560
         Sorted          =   -1  'True
         TabIndex        =   11
         Top             =   840
         Width           =   2175
      End
      Begin VB.Label lblSyncHardware 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Hardware:"
         Height          =   195
         Left            =   -70560
         TabIndex        =   31
         Top             =   840
         Width           =   735
      End
      Begin VB.Label lblServerOpSys 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Server OS:"
         Height          =   195
         Left            =   -74445
         TabIndex        =   25
         Top             =   480
         Width           =   780
      End
      Begin VB.Label lblLocation 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Location:"
         Height          =   195
         Left            =   -74325
         TabIndex        =   35
         Top             =   1680
         Width           =   660
      End
      Begin VB.Label lblCoords 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Coordinates:"
         Height          =   195
         Left            =   -74550
         TabIndex        =   37
         Top             =   2040
         Width           =   885
      End
      Begin VB.Label lblContact 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Contact:"
         Height          =   195
         Left            =   -74265
         TabIndex        =   18
         Top             =   2040
         Width           =   600
      End
      Begin VB.Label lblPolicy 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Access Policy:"
         Height          =   195
         Left            =   -74700
         TabIndex        =   16
         Top             =   1680
         Width           =   1035
      End
      Begin VB.Label lblDaytimeFormat 
         BackStyle       =   0  'Transparent
         Caption         =   "Daytime Server Sends this Format:"
         Enabled         =   0   'False
         Height          =   255
         Left            =   600
         TabIndex        =   23
         Top             =   1800
         Width           =   3495
      End
      Begin VB.Label lblServiceArea 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Service Area:"
         Height          =   195
         Left            =   -74625
         TabIndex        =   14
         Top             =   1200
         Width           =   960
      End
      Begin VB.Label lblServerHardware 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "&Hardware:"
         Height          =   195
         Left            =   -70560
         TabIndex        =   27
         Top             =   480
         Width           =   735
      End
      Begin VB.Label lblStratum 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Stratum:"
         Height          =   195
         Left            =   -74250
         TabIndex        =   33
         Top             =   1200
         Width           =   585
      End
      Begin VB.Label lblSyncProto 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Sync Method:"
         Height          =   195
         Left            =   -74655
         TabIndex        =   29
         Top             =   840
         Width           =   990
      End
      Begin VB.Label lblCountry 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Country:"
         Height          =   195
         Left            =   -74250
         TabIndex        =   10
         Top             =   840
         Width           =   585
      End
      Begin VB.Label lblState 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "State/Province/Region:"
         Height          =   195
         Left            =   -71055
         TabIndex        =   12
         Top             =   840
         Width           =   1710
      End
      Begin VB.Label lblHostName 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Server Name:"
         Height          =   195
         Left            =   -74640
         TabIndex        =   7
         Top             =   480
         Width           =   975
      End
   End
   Begin VB.CommandButton btnClose 
      Caption         =   "&Save Changes and Close"
      Default         =   -1  'True
      Height          =   375
      Left            =   5520
      TabIndex        =   43
      Top             =   5280
      Width           =   2895
   End
   Begin VB.PictureBox picAccess 
      Appearance      =   0  'Flat
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      HasDC           =   0   'False
      Height          =   240
      Left            =   8355
      Picture         =   "Edit.frx":217E
      ScaleHeight     =   240
      ScaleWidth      =   240
      TabIndex        =   45
      Tag             =   "ACCESS"
      Top             =   0
      Width           =   240
   End
   Begin VB.CommandButton btnDiscard 
      Cancel          =   -1  'True
      Caption         =   "&Discard Changes and Close"
      Height          =   375
      Left            =   2520
      TabIndex        =   42
      Top             =   5280
      Width           =   2895
   End
   Begin MSComctlLib.StatusBar sbEdit 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   44
      Top             =   5745
      Width           =   8640
      _ExtentX        =   15240
      _ExtentY        =   450
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   15187
         EndProperty
      EndProperty
   End
   Begin VB.CommandButton btnTest 
      Caption         =   "&Test Server"
      Enabled         =   0   'False
      Height          =   375
      Left            =   7080
      TabIndex        =   5
      Top             =   1800
      Width           =   1335
   End
   Begin VB.CommandButton btnDel 
      Caption         =   "&Delete"
      Enabled         =   0   'False
      Height          =   375
      Left            =   7080
      TabIndex        =   4
      Top             =   1320
      Width           =   1335
   End
   Begin VB.CommandButton btnChg 
      Caption         =   "&Edit"
      Enabled         =   0   'False
      Height          =   375
      Left            =   7080
      TabIndex        =   3
      Top             =   840
      Width           =   1335
   End
   Begin VB.CommandButton btnNew 
      Caption         =   "&New"
      Height          =   375
      Left            =   7080
      TabIndex        =   2
      Top             =   360
      Width           =   1335
   End
   Begin MSComctlLib.TreeView tvEdit 
      Height          =   1815
      Left            =   240
      TabIndex        =   1
      Top             =   360
      Width           =   6615
      _ExtentX        =   11668
      _ExtentY        =   3201
      _Version        =   393217
      HideSelection   =   0   'False
      Indentation     =   529
      LabelEdit       =   1
      LineStyle       =   1
      Sorted          =   -1  'True
      Style           =   6
      Appearance      =   1
      MousePointer    =   1
   End
   Begin MSWinsockLib.Winsock sckTest 
      Left            =   2160
      Top             =   5280
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
   End
   Begin VB.Label lblAccess 
      BackStyle       =   0  'Transparent
      Caption         =   "&Z"
      ForeColor       =   &H8000000F&
      Height          =   255
      Left            =   8040
      TabIndex        =   46
      ToolTipText     =   "12"
      Top             =   0
      Width           =   255
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Tme &Servers:"
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   2895
   End
End
Attribute VB_Name = "frmEdit"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/

Private WithEvents PTMe As PTForm
Attribute PTMe.VB_VarHelpID = -1
Private Status As Panel

'Private TimeOutCnt As Long
Private BegTrip As Variant
Private RoundTrip As Variant
Private SntpIn As SNTP_MESSAGE

Private ServerCnt As Long
Private Servers() As TimeServer
Private NewServer As TimeServer
Private EditServer As TimeServer
Private TestServer As TimeServer
Private UnloadOK As Boolean
Private Sub btnCancel_Click()
    Set EditServer = Nothing
    Set NewServer = Nothing
    EditInProgress False
End Sub
Private Sub btnChg_Click()
    Set NewServer = Nothing
    If EditServer Is Nothing Then
        btnChg.Enabled = False
    Else
        DispServer EditServer
        EditInProgress True
    End If
End Sub
Private Sub btnClose_Click()
    SaveChanges
    EditCancelled = False
    UnloadOK = True
    Unload Me
End Sub
Private Sub SaveChanges()
    Dim i As Long

    Erase Tss
    TsCnt = 0
    For i = 0 To ServerCnt - 1
        If Not Servers(i).Deleted Then
            ReDim Preserve Tss(TsCnt)
            Set Tss(TsCnt) = Servers(i)
            TsCnt = TsCnt + 1
        End If
    Next i

    SaveServers
End Sub
Private Sub btnDel_Click()
    Dim yorn As VbMsgBoxResult

    EditInProgress True
    yorn = MsgBox("Do you want to delete Time Server """ & EditServer.HostName & """?", vbYesNo Or vbQuestion, App.Title & " - Confirm Deletion")
    If yorn = vbYes Then
        EditServer.Deleted = True
        ServerRemove EditServer
        Set EditServer = Nothing
    End If

    EditInProgress False
End Sub
Private Sub btnDiscard_Click()
    EditCancelled = True
    UnloadOK = True
    Unload Me
End Sub
Private Sub btnNew_Click()
    Set NewServer = New TimeServer
    DispServer NewServer
    tabEdit.Tab = 0
    EditInProgress True
End Sub
Private Sub EditInProgress(b As Boolean)
    tvEdit.Enabled = Not b
    btnNew.Enabled = Not b

    If EditServer Is Nothing Then
        btnChg.Enabled = False
        btnDel.Enabled = False
    Else
        btnChg.Enabled = Not b
        btnDel.Enabled = Not b
    End If

    lblHostName.Enabled = b
    txtHostName.Enabled = b
    chkEnabled.Enabled = b

    lblCountry.Enabled = b
    cboCountry.Enabled = b
    lblState.Enabled = b
    cboState.Enabled = b
    lblServiceArea.Enabled = b
    cboServiceArea.Enabled = b
    lblLocation.Enabled = b
    cboLocation.Enabled = b
    lblCoords.Enabled = b
    cboCoords.Enabled = b

    chkSntp.Enabled = b
    chkTime.Enabled = b
    chkDaytime.Enabled = b
    If chkDaytime.Value = vbChecked Then
        lblDaytimeFormat.Enabled = b
        cboDaytimeFormat.Enabled = b
    Else
        lblDaytimeFormat.Enabled = False
        cboDaytimeFormat.Enabled = False
    End If
    lblServerHardware.Enabled = b
    cboServerHardware.Enabled = b
    lblServerOpSys.Enabled = b
    cboServerOpSys.Enabled = b
    lblSyncProto.Enabled = b
    cboSyncProto.Enabled = b
    lblSyncHardware.Enabled = b
    cboSyncHardware.Enabled = b

    lblStratum.Enabled = b
    cboStratum.Enabled = b
    lblPolicy.Enabled = b
    cboPolicy.Enabled = b
    lblContact.Enabled = b
    cboContact.Enabled = b

    btnSave.Enabled = b
    btnCancel.Enabled = b

    btnDiscard.Enabled = Not b
    btnClose.Enabled = Not b
    
    If tvEdit.Enabled Then
        tvEdit.SetFocus
    Else
    
    End If
End Sub
Private Sub btnSave_Click()
    Dim i As Long
    Dim s As String
    Dim yorn As VbMsgBoxResult

    txtHostName.Text = Trim(txtHostName.Text)
    If Len(txtHostName.Text) <= 0 Then
        tabEdit.Tab = 0
        Status.Text = "You must enter the Server's name"
        ApiBeep
        Exit Sub
    End If

    cboCountry.Text = Trim(cboCountry.Text)
    cboState.Text = Trim(cboState.Text)

    If Len(cboState.Text) > 0 Then
        If Len(cboCountry.Text) <= 0 Then
            tabEdit.Tab = 0
            Status.Text = "You must specify a Country if you specify as State/Province/Region"
            ApiBeep
            Exit Sub
        End If
    End If

    If chkDaytime.Value = vbChecked Then
        If cboDaytimeFormat.ListIndex < 0 Then
            tabEdit.Tab = 1
            Status.Text = "If Server supports the Daytime Protocol, you must select a Daytime Format"
            ApiBeep
            Exit Sub
        End If
    End If

    If Not (NewServer Is Nothing) Then ' must be new server
        For i = 0 To ServerCnt - 1
            If StrComp(Servers(i).HostName, txtHostName.Text, vbTextCompare) = 0 Then
                ApiBeep
                s = Servers(i).Country
                If Len(Servers(i).State) > 0 Then
                    s = Servers(i).State & ", " & s
                End If
                
                yorn = MsgBox("You already have a server named '" & txtHostName.Text & "'" & vbCrLf & "(" & s & ")" & vbCrLf & vbCrLf & "Would you like to replace that server's information with this information?", vbYesNo Or vbQuestion, App.Title & " - Duplicate Server Name")
                If yorn <> vbYes Then Exit Sub
                ScrapServer Servers(i)
                Set NewServer = Nothing
                EditInProgress False
                Exit Sub
            End If
        Next i
        ScrapServer NewServer
        NewServer.Tag = CStr(ServerCnt)
        ReDim Preserve Servers(ServerCnt)
        Set Servers(ServerCnt) = NewServer
        ServerCnt = ServerCnt + 1
        ServerAdd NewServer
        Set EditServer = NewServer
        Set NewServer = Nothing
        EditInProgress False
        Exit Sub
    End If

    ' editing existing server
    If StrComp(EditServer.HostName, txtHostName.Text, vbTextCompare) <> 0 Then   ' new name
        For i = 0 To ServerCnt - 1
            If StrComp(Servers(i).HostName, txtHostName.Text, vbTextCompare) = 0 Then
                ApiBeep
                yorn = MsgBox("You already have a server named '" & txtHostName.Text & "'. Would you like to replace that server's information with this information?", vbYesNo Or vbQuestion, App.Title & " - Duplicate Server Name")
                If yorn <> vbYes Then Exit Sub
                ServerRemove Servers(i)
                ScrapServer Servers(i)
                ServerAdd Servers(i)
                EditInProgress False
                Exit Sub
            End If
        Next i
    End If

    ServerRemove EditServer
    ScrapServer EditServer
    ServerAdd EditServer
    EditInProgress False
End Sub
Private Sub ScrapServer(ts As TimeServer)
    ts.HostName = Trim(txtHostName.Text)
    If chkEnabled.Value = vbChecked Then
        ts.Enabled = True
    Else
        ts.Enabled = False
    End If

    ItemAdd cboCountry.Text, cboCountry
    ts.Country = cboCountry.Text

    ItemAdd cboState.Text, cboState
    ts.State = cboState.Text

    ItemAdd cboServerHardware.Text, cboServerHardware
    ts.ServerHardware = cboServerHardware.Text

    ItemAdd cboServerOpSys.Text, cboServerOpSys
    ts.ServerOpSys = cboServerOpSys.Text

    ItemAdd cboContact.Text, cboContact
    ts.Contact = cboContact.Text

    ItemAdd cboCoords.Text, cboCoords
    ts.Coords = cboCoords.Text

    If chkDaytime.Value = vbChecked Then
        ts.DaytimeSupported = True
        ts.DaytimeFormat = cboDaytimeFormat.ItemData(cboDaytimeFormat.ListIndex)
    Else
        ts.DaytimeSupported = False
    End If
    If chkTime.Value = vbChecked Then
        ts.TimeSupported = True
    Else
        ts.TimeSupported = False
    End If
    If chkSntp.Value = vbChecked Then
        ts.SntpSupported = True
    Else
        ts.SntpSupported = False
    End If

    ItemAdd cboLocation.Text, cboLocation
    ts.Location = cboLocation.Text

    ItemAdd cboPolicy.Text, cboPolicy
    ts.Policy = cboPolicy.Text

    ItemAdd cboServiceArea.Text, cboServiceArea
    ts.ServiceArea = cboServiceArea.Text

    ts.Stratum = cboStratum.ItemData(cboStratum.ListIndex)

    ItemAdd cboSyncProto.Text, cboSyncProto
    ts.SyncProto = cboSyncProto.Text

    ItemAdd cboSyncHardware.Text, cboSyncHardware
    ts.SyncHardware = cboSyncHardware.Text
End Sub
Private Sub btnTest_Click()
    Dim HostName As String

    If tvEdit.Enabled Then ' not editing
        If EditServer Is Nothing Then
            btnTest.Enabled = False
            Exit Sub
        Else
            Set TestServer = EditServer.Clone
        End If
    Else ' editing
        If NewServer Is Nothing Then
            Set TestServer = EditServer.Clone
        Else
            Set TestServer = NewServer.Clone
        End If
        TestServer.HostName = Trim(txtHostName.Text)
    End If

    If Not (TestServer Is Nothing) Then
        InitClientSntp TestServer, ACTIVITY_EDIT_TEST
    Else
        btnTest.Enabled = False
    End If
End Sub
Private Sub chkDaytime_Click()
    If chkDaytime.Value = vbChecked Then
        lblDaytimeFormat.Enabled = chkDaytime.Enabled
        cboDaytimeFormat.Enabled = chkDaytime.Enabled
    Else
        lblDaytimeFormat.Enabled = False
        cboDaytimeFormat.Enabled = False
    End If
End Sub
Private Sub ServerAdd(ts As TimeServer)
    Dim Root As MSComctlLib.Node
    Dim cn As MSComctlLib.Node
    Dim sn As MSComctlLib.Node
    Dim Parent As MSComctlLib.Node
    Dim n As MSComctlLib.Node
    Dim s As String

    On Error Resume Next

    Set Root = tvEdit.Nodes(KEY_ROOT)
    If Root Is Nothing Then
        Set Root = tvEdit.Nodes.Add(, tvwFirst, KEY_ROOT, ROOT_DESC)
        Root.Sorted = True
    End If
    Set Parent = Root

    s = UCase(Trim(ts.Country))
    If Len(s) > 0 Then
        Set cn = Nothing
        Set cn = tvEdit.Nodes(KEY_COUNTRY & s)
        If cn Is Nothing Then
            Set cn = tvEdit.Nodes.Add(Root, tvwChild, KEY_COUNTRY & s, ts.Country)
            cn.Sorted = True
        End If
        Set Parent = cn

        s = UCase(Trim(ts.State))
        If Len(s) > 0 Then
            Set sn = Nothing
            Set sn = tvEdit.Nodes(KEY_STATE & s)
            If sn Is Nothing Then
                Set sn = tvEdit.Nodes.Add(cn, tvwChild, KEY_STATE & s, ts.State)
                sn.Sorted = True
            End If
            Set Parent = sn
        End If
    End If

    s = Trim(UCase(ts.HostName))
    Set n = tvEdit.Nodes.Add(Parent, tvwChild, KEY_HOST & s, ts.HostName)
    n.Sorted = True
    n.Tag = ts.Tag
    If tvEdit.Visible Then
        n.EnsureVisible
        Set tvEdit.SelectedItem = n
    End If
    Err.Clear

    ItemAdd ts.Country, cboCountry
    ItemAdd ts.State, cboState
    ItemAdd ts.SyncProto, cboSyncProto
    ItemAdd ts.SyncHardware, cboSyncHardware
    ItemAdd ts.Contact, cboContact
    ItemAdd ts.ServerOpSys, cboServerOpSys
    ItemAdd ts.ServerHardware, cboServerHardware
    ItemAdd ts.Location, cboLocation
    ItemAdd ts.Policy, cboPolicy
    ItemAdd ts.Coords, cboCoords
    ItemAdd ts.ServiceArea, cboServiceArea
End Sub
Private Sub ServerRemove(ts As TimeServer)
    Dim Root As MSComctlLib.Node
    Dim cn As MSComctlLib.Node
    Dim sn As MSComctlLib.Node
    Dim p As MSComctlLib.Node
    Dim n As MSComctlLib.Node
    Dim s As String

    On Error Resume Next
    s = Trim(UCase(ts.HostName))
    Set n = tvEdit.Nodes(KEY_HOST & s)
    If n Is Nothing Then
        Err.Clear
        Exit Sub
    End If

    Set Root = tvEdit.Nodes(KEY_ROOT)
    Do While (Not (n Is Root)) And (n.Children = 0)
        Set p = n.Parent
        tvEdit.Nodes.Remove n.Index
        Set n = p
    Loop
End Sub
Private Function InitClientSntp(ts As TimeServer, ActivityCode As ACTIVITY_CODE) As Winsock
    Dim DQ As String
    Dim SntpOut(47) As Byte
    Dim inx As Long

    DQ = SockHostDQ(ts.HostName)
    If Len(DQ) <= 0 Then
        Status.Text = "Server Test Failed: Could Not Resolve Host Name - " & ts.HostName
        btnTest.Enabled = True
        Exit Function
    End If

    SntpOut(0) = &H1B

    With sckTest
        .Close
        .Tag = JoinSckTag(TIMEOUT_VAL, ActivityCode, ts.Index)
        .Protocol = sckUDPProtocol
        .RemotePort = PORT_SNTP  'S/NTP service port
        .RemoteHost = DQ ' HostName
        .Connect
        .SendData SntpOut
    End With
    BegTrip = ApiTickStart()

    Status.Text = "Connecting To " & ts.HostName & " (" & DQ & ") ..."
    Set InitClientSntp = sckTest
    timOutClient.Enabled = True
End Function
Private Sub FiniClientSntp(ActivityCode As ACTIVITY_CODE, SntpIn As SNTP_MESSAGE)
    Dim s As String
    Dim i As Long
    Dim CurSecs As Variant
    Dim TotalSecs As Variant
    Dim WholeSecs As Variant
    Dim FracSecs As Variant
    Dim ost As SYSTEMTIME
    Dim nst As SYSTEMTIME
    Dim ts As SNTP_TIMESTAMP
    Dim CurDate As Date
    Dim NewDate As Date
    Dim lv As ListView
    Dim result As String

    result = "Test of Server """ & TestServer.HostName & """ SUCCEEDED!" & vbCrLf
    result = result & vbCrLf
    GetSystemTime ost
    result = result & "Response Received in: " & FormatNumber(RoundTrip, 0) & " msecs (" & FormatNumber(RoundTrip / 1000, 3) & " secs)" & vbCrLf
    result = result & vbCrLf

    ts = SntpIn.TransmitTimeStamp
    TotalSecs = Round(TimeStampToSecs(ts), 3)

    If TotalSecs = 0 Then
        Status.Text = "Test of Server """ & TestServer.HostName & """ Failed"
        result = Status.Text & vbCrLf
        result = result & "Server Returned Invalid Time (0)"
        MsgBox result, vbOKOnly, App.Title & " - Time Server Test"
        btnTest.Enabled = True
        Exit Sub
    End If

    TotalSecs = TotalSecs + (RoundTrip / 2000)
    TotalSecs = Round(TotalSecs, 3)
    WholeSecs = Int(TotalSecs)
    FracSecs = TotalSecs - WholeSecs

    ' reduce TotalSecs to prevent overflow
    TotalSecs = TotalSecs - DateDiff("s", #1/1/1900#, #1/1/1970#)
    NewDate = DateAdd("s", TotalSecs, #1/1/1970#)
    nst.wYear = Year(NewDate)
    nst.wMonth = Month(NewDate)
    nst.wDay = Day(NewDate)
    nst.wHour = Hour(NewDate)
    nst.wMinute = Minute(NewDate)
    nst.wSecond = Second(NewDate)
    nst.wMilliseconds = Round(FracSecs * 1000, 0)
    If (ActivityCode And ACTIVITY_FUNCTION_MASK) = ACTIVITY_SYNC Then
        SetSystemTime nst
    End If

    result = result & "Our Current Date/Time: " & DispSystemTime(ost) & vbCrLf

    CurSecs = DateDiff("s", #1/1/1970#, DateSerial(ost.wYear, ost.wMonth, ost.wDay) + TimeSerial(ost.wHour, ost.wMinute, ost.wSecond))
    CurSecs = CurSecs + (ost.wMilliseconds / 1000)
    s = FormatNumber(TotalSecs - CurSecs, 3, vbFalse) & " secs"
    If Left(s, 1) <> "-" Then s = "+" & s

    result = result & "Server's Current Date/Time: " & DispSystemTime(nst) & vbCrLf
    result = result & vbCrLf
    result = result & "Difference/Adjustment to Our Clock: " & s & vbCrLf
    If (ActivityCode And ACTIVITY_FUNCTION_MASK) <> ACTIVITY_SYNC Then
        result = result & "(Note: No actual adjustment was made -- this was just a test of the server)" & vbCrLf
    End If

    TestServer.SntpSupported = True
    If TestServer.RoundTrip <= 0 Then
        TestServer.RoundTrip = RoundTrip
    End If
    TestServer.Enabled = True

    Status.Text = "Test of Server """ & TestServer.HostName & """ Succeeded!"
    MsgBox result, vbOKOnly, App.Title & " - Time Server Test"
    btnTest.Enabled = True
End Sub
Private Sub Form_Load()
    Dim i As Long
    Dim Root As MSComctlLib.Node
    Dim ts As TimeServer

    Set PTMe = New PTForm
    PTMe.ConstructorEx Me, "Edit Time Servers"

    Set Status = sbEdit.Panels(1)

    cboDaytimeFormat.Clear
    Set ts = New TimeServer
    For i = DAYTIME_LBOUND To DAYTIME_UBOUND
        cboDaytimeFormat.AddItem ts.DispDaytimeFormat(i)
        cboDaytimeFormat.ItemData(cboDaytimeFormat.NewIndex) = i
    Next i
    Set ts = Nothing

    cboStratum.Clear
    cboStratum.AddItem "Unknown"
    cboStratum.ItemData(cboStratum.NewIndex) = 0
    For i = 1 To 6
        cboStratum.AddItem CStr(i)
        cboStratum.ItemData(cboStratum.NewIndex) = i
    Next i

    ReDim Servers(TsCnt)
    For ServerCnt = 0 To TsCnt - 1
        Set Servers(ServerCnt) = Tss(ServerCnt).Clone
    Next ServerCnt
    ServerCnt = TsCnt

    tvEdit.Nodes.Clear
    On Error Resume Next
    For i = 0 To ServerCnt - 1
        Servers(i).Tag = CStr(i)
        ServerAdd Servers(i)
    Next i

    cboCountry.Text = ""
    cboState.Text = ""

    Set Root = tvEdit.Nodes(KEY_ROOT)
    Root.EnsureVisible
    Root.Expanded = True
    tvEdit.MousePointer = ccArrow

    LabelAlign lblCountry, cboCountry
    LabelAlign lblState, cboCountry
    LabelAlign lblServiceArea, cboServiceArea
    LabelAlign lblPolicy, cboPolicy
    LabelAlign lblContact, cboContact

    LabelAlign lblServerOpSys, cboServerOpSys
    LabelAlign lblServerHardware, cboServerHardware
    LabelAlign lblSyncProto, cboSyncProto
    LabelAlign lblSyncHardware, cboSyncHardware
    LabelAlign lblStratum, cboStratum
    LabelAlign lblLocation, cboLocation
    LabelAlign lblCoords, cboCoords
    PTMe.ReInit
    
    EditInProgress False
End Sub
Private Sub Form_Unload(Cancel As Integer)
    Dim yorn As VbMsgBoxResult

    If Not UnloadOK Then
        yorn = MsgBox("Do you want to save the changes you've made?", vbQuestion Or vbYesNoCancel, App.Title & " - Close Edit Window?")
        Select Case yorn
            Case vbYes
                SaveChanges

            Case vbNo

            Case Else
                Cancel = True
                Exit Sub
        End Select
    End If

    PTMe.Unload
End Sub
Private Sub ItemAdd(ByRef Text As String, cbo As ComboBox)
    Dim i As Long

    Text = Trim(Text)
    For i = 0 To cbo.ListCount - 1
        If StrComp(Text, cbo.List(i), vbTextCompare) = 0 Then
            If cbo.Visible Then
                cbo.ListIndex = i
            End If
            Exit Sub
        End If
    Next i

    cbo.AddItem Text
    If cbo.Visible Then
        cbo.ListIndex = cbo.NewIndex
    End If
End Sub
Private Sub sckTest_DataArrival(ByVal bytesTotal As Long)
    Dim buf() As Byte
    Dim DataSntp As SNTP_MESSAGE
    Dim DataTime() As Byte
    Dim DataDaytime As String
    Dim ActivityCode As ACTIVITY_CODE
    Dim TimeOut As Long
    Dim ServerInx As Long

    timOutClient.Enabled = False
    RoundTrip = ApiTickElapsed(BegTrip)
    SplitSckTag sckTest.Tag, TimeOut, ActivityCode, ServerInx

    On Error Resume Next
    Select Case ActivityCode And ACTIVITY_PROTO_MASK

        Case ACTIVITY_SNTP:
            If bytesTotal <> LenB(DataSntp) Then
                Status.Text = "Server Test Failed: Invalid Reply - " & CStr(bytesTotal)
                ApiBeep
            Else
                ReDim buf(bytesTotal - 1)
                sckTest.GetData buf, vbByte And vbArray
                ApiMemoryCopy VarPtr(DataSntp), VarPtr(buf(0)), bytesTotal
                If Err.Number <> 0 Then
                    Status.Text = "Server Test Failed: Error - " & Err.Description & " (" & Err.Number & ")"
                    ApiBeep
                    Err.Clear
                Else
                    FiniClientSntp ActivityCode, DataSntp
                End If
            End If
'
'        Case ACTIVITY_TIME:
'            If bytesTotal <> 4 Then
''                ActivityLog ActivityCode, "Invalid Reply - " & CStr(bytesTotal)
'            Else
'                ReDim buf(bytesTotal - 1)
'                sckTest.GetData buf, vbByte And vbArray
'                If Err.Number <> 0 Then
''                    ActivityLog ActivityCode, "Error:", Err.Description & " (" & Err.Number & ")"
'                    Err.Clear
'                Else
'                    FiniClientTime ActivityCode, buf
'                End If
'            End If
'
'        Case ACTIVITY_DAYTIME:
'            sckTest.GetData DataDaytime, vbString
'            If Err.Number <> 0 Then
''                ActivityLog ActivityCode, "Error:", Err.Description & " (" & Err.Number & ")"
'                Err.Clear
'            Else
'                FiniClientDaytime ActivityCode, DataDaytime
'            End If
'
'        Case Else:
''            ActivityLog ActivityCode, "Unknown Activity in sckClient_DataArrival: " & DispProtocol(ActivityCode)
    
    End Select

    sckTest.Close
    btnTest.Enabled = True
End Sub
Private Sub tabEdit_Click(PreviousTab As Integer)
    Select Case tabEdit.Tab
        Case 0
            cboCountry.SelStart = 1
            cboCountry.SelLength = 0
            cboState.SelStart = 1
            cboState.SelLength = 0
            cboServiceArea.SelStart = 1
            cboServiceArea.SelLength = 0
            cboPolicy.SelStart = 1
            cboPolicy.SelLength = 0
            cboContact.SelStart = 1
            cboContact.SelLength = 0
        Case 1
        
        Case 2
            cboServerOpSys.SelStart = 1
            cboServerOpSys.SelLength = 0
            cboServerHardware.SelStart = 1
            cboServerHardware.SelLength = 0
            cboSyncProto.SelStart = 1
            cboSyncProto.SelLength = 0
            cboSyncHardware.SelStart = 1
            cboSyncHardware.SelLength = 0
            cboLocation.SelStart = 1
            cboLocation.SelLength = 0
            cboCoords.SelStart = 1
            cboCoords.SelLength = 0
    End Select
End Sub
Private Sub timOutClient_Timer()
    Dim sck As Winsock
    Dim inx As Long
    Dim ActivityCode As ACTIVITY_CODE
    Dim TimeOut As Long
    Dim ServerInx As Long

    SplitSckTag sckTest.Tag, TimeOut, ActivityCode, ServerInx
    TimeOut = TimeOut - 1
    sckTest.Tag = JoinSckTag(TimeOut, ActivityCode, ServerInx)
    If TimeOut > 0 Then Exit Sub

    timOutClient.Enabled = False
    sckTest.Close
    btnTest.Enabled = True
    Status.Text = "Server Test Failed: Connection Has Timed Out"
    ApiBeep
'    ActivityAdjust
End Sub
Private Sub DispSyncError(sck As Winsock, ActivityCode As ACTIVITY_CODE, msg As String)
    Dim s As String

'    ActivityLog ActivityCode, "Error: " & Msg
'    ActivityLog ActivityCode, "This may indicate a temporary or permanent problem with the time server.", " "
'    ActivityLog ActivityCode, "A different time server may succeed.", " "
'    ActivityLog ActivityCode, " ", " "
'    ActivityLog ActivityCode, "Also, if you are using a firewall or other time of Internet filter, make sure", " "
'    ActivityLog ActivityCode, "the program " & App.Path & App.EXEName, " "
'    s = "can access "
'    If sck.Protocol = sckUDPProtocol Then
'        s = s & "UDP"
'    Else
'        s = s & "TCP"
'    End If
'    s = s & " port " & CStr(sck.RemotePort)
'    ActivityLog ActivityCode, s, " "
End Sub
Private Sub tvEdit_DblClick()
    If btnChg.Enabled Then btnChg_Click
End Sub
Private Sub tvEdit_NodeClick(ByVal Node As MSComctlLib.Node)
    Dim inx As Long
    Dim LstItm As ListItem

    If Not NodeIsHost(Node) Then
        btnChg.Enabled = False
        btnDel.Enabled = False
        btnTest.Enabled = False
        DispServer Nothing
        Exit Sub
    End If

    inx = CLng(Node.Tag)
    Set EditServer = Servers(inx)
    DispServer Servers(inx)

    btnChg.Enabled = True
    btnDel.Enabled = True
    btnTest.Enabled = True
End Sub
Private Sub DispServer(ts As TimeServer)
    If ts Is Nothing Then
        txtHostName.Text = ""
        lblHostName.FontBold = True
        btnTest.Enabled = False

        chkEnabled.Value = vbUnchecked

        cboCountry.ListIndex = -1
        cboState.ListIndex = -1

        cboLocation.ListIndex = -1
        cboPolicy.ListIndex = -1
        cboServerHardware.ListIndex = -1
        cboServerOpSys.ListIndex = -1
        cboContact.ListIndex = -1
        cboCoords.ListIndex = -1

        chkSntp.Value = vbUnchecked

        chkDaytime.Value = vbUnchecked
        lblDaytimeFormat.Enabled = False
        cboDaytimeFormat.Enabled = False
        cboDaytimeFormat.ListIndex = -1

        chkTime.Value = vbUnchecked

        cboServiceArea.ListIndex = -1
        cboStratum.ListIndex = -1
        cboSyncProto.ListIndex = -1
        cboSyncHardware.ListIndex = -1
    Else
        txtHostName.Text = Trim(ts.HostName)
        If Len(Trim(ts.HostName)) > 0 Then
            lblHostName.FontBold = False
            btnTest.Enabled = True
        Else
            lblHostName.FontBold = True
            btnTest.Enabled = False
        End If
    
        If ts.Enabled Then
            chkEnabled.Value = vbChecked
        Else
            chkEnabled.Value = vbUnchecked
        End If
    
        ItemAdd ts.Country, cboCountry
        ItemAdd ts.State, cboState
    
        ItemAdd ts.Location, cboLocation
        ItemAdd ts.Policy, cboPolicy
        ItemAdd ts.ServerHardware, cboServerHardware
        ItemAdd ts.ServerOpSys, cboServerOpSys
        ItemAdd ts.Contact, cboContact
        ItemAdd ts.Coords, cboCoords
    
        If ts.SntpSupported Then
            chkSntp.Value = vbChecked
        Else
            chkSntp.Value = vbUnchecked
        End If
    
        If ts.DaytimeSupported Then
            chkDaytime.Value = vbChecked
            lblDaytimeFormat.Enabled = chkDaytime.Enabled
            cboDaytimeFormat.Enabled = chkDaytime.Enabled
            cboDaytimeFormat.ListIndex = ts.DaytimeFormat
        Else
            chkDaytime.Value = vbUnchecked
            lblDaytimeFormat.Enabled = False
            cboDaytimeFormat.Enabled = False
            cboDaytimeFormat.ListIndex = 0
        End If
    
        If ts.TimeSupported Then
            chkTime.Value = vbChecked
        Else
            chkTime.Value = vbUnchecked
        End If
    
        ItemAdd ts.ServiceArea, cboServiceArea
        cboStratum.ListIndex = ts.Stratum
        ItemAdd ts.SyncProto, cboSyncProto
        ItemAdd ts.SyncHardware, cboSyncHardware
    End If
End Sub
Private Sub txtHostName_Change()
    If Len(Trim(txtHostName.Text)) > 0 Then
        lblHostName.FontBold = False
        btnTest.Enabled = True
    Else
        lblHostName.FontBold = True
        btnTest.Enabled = False
    End If
End Sub
