VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "VB Winsock Chat"
   ClientHeight    =   6810
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7545
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   6810
   ScaleWidth      =   7545
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00FF0000&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   15
      Left            =   6000
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   37
      Top             =   6000
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00800000&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   14
      Left            =   6000
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   36
      Top             =   6360
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00FF00FF&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   13
      Left            =   6360
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   35
      Top             =   6000
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00800080&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   12
      Left            =   6360
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   34
      Top             =   6360
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H0000FFFF&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   11
      Left            =   5280
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   33
      Top             =   6000
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00008080&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   10
      Left            =   5280
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   32
      Top             =   6360
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H0000FF00&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   9
      Left            =   5640
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   31
      Top             =   6000
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00008000&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   8
      Left            =   5640
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   30
      Top             =   6360
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H000000FF&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   5
      Left            =   4560
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   29
      Top             =   6000
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00000080&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   4
      Left            =   4560
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   28
      Top             =   6360
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H000080FF&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   7
      Left            =   4920
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   27
      Top             =   6000
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00004080&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   6
      Left            =   4920
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   26
      Top             =   6360
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00000000&
      Height          =   375
      Index           =   1
      Left            =   3840
      ScaleHeight     =   315
      ScaleWidth      =   315
      TabIndex        =   25
      Top             =   6360
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00FFFFFF&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   0
      Left            =   3840
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   24
      Top             =   6000
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00C0C0C0&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   2
      Left            =   4200
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   23
      Top             =   6000
      Width           =   375
   End
   Begin VB.PictureBox picSelColor 
      BackColor       =   &H00808080&
      BorderStyle     =   0  'None
      Height          =   375
      Index           =   3
      Left            =   4200
      ScaleHeight     =   375
      ScaleWidth      =   375
      TabIndex        =   22
      Top             =   6360
      Width           =   375
   End
   Begin VB.CommandButton cmdPorts 
      Caption         =   "Port Settings"
      Height          =   285
      Left            =   2640
      TabIndex        =   21
      TabStop         =   0   'False
      Top             =   480
      Width           =   1095
   End
   Begin VB.ListBox lstConnections 
      Height          =   1620
      ItemData        =   "frmMain.frx":0442
      Left            =   3840
      List            =   "frmMain.frx":0444
      MultiSelect     =   2  'Extended
      TabIndex        =   7
      Top             =   840
      Width           =   2775
   End
   Begin VB.CommandButton cmdKick 
      Caption         =   "Kick"
      Height          =   495
      Left            =   6720
      TabIndex        =   8
      Top             =   840
      Visible         =   0   'False
      Width           =   735
   End
   Begin VB.CommandButton cmdClearDraw 
      Caption         =   "Clear"
      Height          =   735
      Left            =   6735
      TabIndex        =   9
      Top             =   6000
      Width           =   725
   End
   Begin VB.PictureBox picDraw 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   3015
      Left            =   3840
      ScaleHeight     =   2985
      ScaleWidth      =   3585
      TabIndex        =   12
      TabStop         =   0   'False
      Top             =   2880
      Width           =   3615
   End
   Begin VB.TextBox txtName 
      Height          =   285
      Left            =   960
      TabIndex        =   0
      Top             =   120
      Width           =   2775
   End
   Begin VB.TextBox txtIP 
      Height          =   285
      Left            =   960
      TabIndex        =   1
      Top             =   480
      Width           =   1575
   End
   Begin VB.Timer tmrSendData 
      Interval        =   1
      Left            =   1080
      Top             =   1320
   End
   Begin VB.TextBox txtDialog 
      Height          =   2175
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      TabIndex        =   11
      TabStop         =   0   'False
      Top             =   4560
      Width           =   3615
   End
   Begin VB.TextBox txtMessage 
      Height          =   855
      Left            =   120
      TabIndex        =   6
      Top             =   2880
      Width           =   3615
   End
   Begin MSWinsockLib.Winsock sckConnect 
      Left            =   600
      Top             =   1320
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   327681
   End
   Begin VB.CommandButton cmdSend 
      Caption         =   "Send"
      Height          =   375
      Left            =   120
      TabIndex        =   10
      TabStop         =   0   'False
      Top             =   3840
      Width           =   3615
   End
   Begin VB.CommandButton cmdDisconnect 
      Caption         =   "Disconnect"
      Height          =   375
      Left            =   2520
      TabIndex        =   4
      Top             =   840
      Width           =   1215
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "Connect"
      Height          =   375
      Left            =   1320
      TabIndex        =   3
      Top             =   840
      Width           =   1215
   End
   Begin VB.CommandButton cmdHost 
      Caption         =   "Host"
      Height          =   375
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   1215
   End
   Begin MSWinsockLib.Winsock sckConnection 
      Index           =   0
      Left            =   120
      Top             =   1320
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   327681
   End
   Begin VB.TextBox txtStatus 
      Height          =   1125
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   5
      TabStop         =   0   'False
      Top             =   1320
      Width           =   3615
   End
   Begin VB.Label lblHost 
      AutoSize        =   -1  'True
      Height          =   195
      Left            =   3840
      TabIndex        =   20
      Top             =   360
      Width           =   3615
   End
   Begin VB.Label Label7 
      AutoSize        =   -1  'True
      Caption         =   "Draw:"
      Height          =   195
      Left            =   3840
      TabIndex        =   19
      Top             =   2640
      Width           =   420
   End
   Begin VB.Label Label6 
      AutoSize        =   -1  'True
      Caption         =   "Host:"
      Height          =   195
      Left            =   3840
      TabIndex        =   18
      Top             =   120
      Width           =   375
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      Caption         =   "Members:"
      Height          =   195
      Left            =   3840
      TabIndex        =   17
      Top             =   600
      Width           =   690
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "Message:"
      Height          =   195
      Left            =   120
      TabIndex        =   16
      Top             =   2640
      Width           =   690
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      Caption         =   "Message Dialog:"
      Height          =   195
      Left            =   120
      TabIndex        =   15
      Top             =   4320
      Width           =   1185
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Connect to:"
      Height          =   195
      Left            =   120
      TabIndex        =   14
      Top             =   480
      Width           =   825
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Name:"
      Height          =   195
      Left            =   120
      TabIndex        =   13
      Top             =   120
      Width           =   465
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'VB Winsock Many-Chat 1.01
'(C) Richard C. Yarnell 12/17/1998
'    ryarnell@andrew.cmu.edu
'    http://www.geocities.com/SiliconValley/Campus/6818/

'This project shows how to create a VB5 chat room where
'users can exchange messages and draw pictures for one another.
'One person runs this program and decides to be the server and
'up to 8 people (This is the default and can be modified.)
'can connect to it.  Multiple servers can be running on the same
'computer on different ports.

'All data is sent through the server.  When someone who is connected
'to the server sends a message, it is first sent to the server and then
'sent to all other connections.  When the server sends a message, it is
'just sent to everyone immediately.

'Revisions:
'1.01 (12/17/1998):
'      Fixed bugs involved sending of data.
'      Data was being deleted from the queue before it was sent because
'      the connection was not ready for the transfer at the time.
'      This may be because I was previously testing on the T1 connection
'      at school and just now started testing using a modem for Internet
'      access while at home.
'1.00 (12/9/1998): Initial release.

'Known Bugs:
'*  Setting the number of maximum connections much higher than 10
'   generates an "Out of buffer space" error.  The actual number that
'   causes the error may vary.  I have had it occur with numbers as
'   low as 12 and work with numbers as high as 18.  I think there may
'   be a limit on the number of Winsock controls loaded at once.
'   I don't what to do about this one.  Please email with any advice.

'This forces the declaration of all variables, preventing misspellings of variables and type mismatches.
Option Explicit

'Sometimes parameters are sent along with the commands that are sent between computers.
'All parameters will be formatted to be exactly PARAM_LEN characters long to simplify the parsing of commands by the receiving computer(s).
Const PARAM_LEN = 10

'This is a collection of commands and data to be sent to other computers, either the server (if you have connected to one) or to all connected computers (if you are the server).
Dim SendList As New Collection
'This is a collection of commands and data that specifies where to send the items in SendList.
'Each item in SendList has an associated item in SendTo which says to which computer the information in SendList is to be sent.
Dim SendTo As New Collection

'These are used in tracking where your mouse is when drawing pictures.
Dim iX As Integer, iY As Integer

'When this flag is set and the form is unloaded, the confirm quit message will not appear.
'Read more about why this is used in the Form_Unload event.
Dim bJustQuit As Boolean
Public Sub ProcessData(vsString As String, viConnection As Integer)
'This procedure processes data received from either the server or from connections to the server.
'vsString = the command string being processed
'viConnection = the connection from which the command string was received

Dim i As Integer
Dim sCommand As String
Dim sInstruction As String
Dim sData As String
Dim sFormatStr As String

For i = 1 To PARAM_LEN
    sFormatStr = sFormatStr & "0"
Next i

'Separate commands may be received together so each command is followed by a carriage return.
'So as long as a carriage return is found in the data stream, there must be a command in it so continue processing data.
Do While InStr(1, vsString, vbCrLf)
    
    'Store in sCommand the part of the data stream that contains the first command.
    sCommand = Mid(vsString, 1, InStr(1, vsString, vbCrLf) - 1)
    
    'Each command contains an instruction such as [Message] or [Disconnect].
    'Some commands also contain parameters.
    'Here the instruction part of the command is stored in sInstruction and the rest is stored in sData.
    sInstruction = Mid(sCommand, 1, InStr(1, sCommand, "]"))
    sData = Mid(sCommand, InStr(1, sCommand, "]") + 2, Len(sCommand))
    
    'Branch depending upon the instruction.
    Select Case sInstruction
        Case "[Change Name]"
            'This command is sent by a connecting user when they change their name in their Name text box.  (Only the server will receive such a command.)
            
            'Update their name in the name list.
            lstConnections.List(viConnection) = sData
            'Refresh the name list on all connected computers.
            SendPersonName viConnection
        Case "[Clear Draw]"
            'This command is sent when someone presses the Clear button to clear the picture box.
            
            'Clear the picture box.
            picDraw.Cls
            'For each open connection, send the command to clear the picture box.
            '(This is done only by the server.)
            For i = 0 To giMaxConnections - 1
                If sckConnection(i).State = sckConnected And i <> viConnection Then
                    SendList.Add "[Clear Draw]"
                    SendTo.Add "sckConnection(" & i & ")"
                End If
            Next i
        Case "[Disconnect]"
            'This is a request to be disconnected from the server.  People cannot disconnect themselves from the server - the server must disconnect them.
            
            'Close the connection.
            sckConnection(viConnection).Close
            'Call the procedure that ensures that one winsock control is listening for connections.
            MakeListen
            'Update the status.
            txtStatus.Text = txtStatus.Text & vbCrLf & lstConnections.List(viConnection) & " disconnected."
            'Reset their name in the name list.
            lstConnections.List(viConnection) = "[Open]"
            'Have the server notify all connected computer that this person has disconnected.
            For i = 0 To giMaxConnections - 1
                If sckConnection(i).State = sckConnected Then
                    SendList.Add "[Disconnected] " & Format(viConnection, sFormatStr)
                    SendTo.Add "sckConnection(" & i & ")"
                End If
            Next i
        Case "[Disconnected]"
            'This command is received when the server notifies someone that someone else has disconnected.
            
            'Update the status.
            txtStatus.Text = txtStatus.Text & vbCrLf & lstConnections.List(sParam(sData, 1)) & " disconnected."
            'Reset their name in the name list.
            lstConnections.List(sParam(sData, 1)) = "[Open]"
        Case "[Host]"
            'This command is received from the server when one connects to it.  It notifies connecting people of the host's name.
            
            'Update the name of the host.
            lblHost.Caption = sData
            'Update the status.
            txtStatus.Text = txtStatus.Text & vbCrLf & "You have connected to " & sData & "."
        Case "[Host Change Name]"
            'This command is received from the server when the host changes their name.
            
            'Update the name of the server.
            lblHost.Caption = sData
        Case "[Joined]"
            'This command is sent to the server when someone joins, notifying the server of the name of the person connecting.
            
            'Update the status.
            txtStatus.Text = txtStatus.Text & vbCrLf & sData & " joined."
            'If you are the server then notify all other connections that someone has joined and send the name of the new connection.
            For i = 0 To giMaxConnections - 1
                If sckConnection(i).State = sckConnected And i <> viConnection Then
                    SendList.Add "[Joined] " & sData
                    SendTo.Add "sckConnection(" & i & ")"
                End If
            Next i
            'Also if you are the server...
            If lstConnections.ListCount > 0 And sckConnect.State <> sckConnected Then
                'Update the name in the connections list.
                lstConnections.List(viConnection) = sData
                'Send the name list to the person connecting.
                SendPeopleList viConnection
                'Notify all open connections of the new person who just joined.
                SendPersonName viConnection
            End If
        Case "[Kicked]"
            'This command is sent by the server notifying connections that someone was kicked.
            
            'Update the status.
            txtStatus.Text = txtStatus.Text & vbCrLf & "Kicked " & lstConnections.List(sParam(sData, 1)) & "."
            'Reset their name in the name list.
            lstConnections.List(sParam(sData, 1)) = "[Open]"
        Case "[Line]"
            'This command is sent when someone draws a line.
        
            'Draw the line.
            picDraw.Line (sParam(sData, 1), sParam(sData, 2))-(sParam(sData, 3), sParam(sData, 4)), sParam(sData, 5)
            'If you are the server, send the data on the line to all open connections.
            For i = 0 To giMaxConnections - 1
                If sckConnection(i).State = sckConnected And i <> viConnection Then
                    SendList.Add "[Line] " & sData
                    SendTo.Add "sckConnection(" & i & ")"
                End If
            Next i
        Case "[Message]"
            'This command is sent when someone enters a message.
        
            'Show the message.
            txtDialog.Text = txtDialog.Text & sData & vbCrLf
            'Notify all open connections of the message.
            For i = 0 To giMaxConnections - 1
                If sckConnection(i).State = sckConnected And i <> viConnection Then
                    SendList.Add "[Message] " & sData
                    SendTo.Add "sckConnection(" & i & ")"
                End If
            Next i
        Case "[People]"
            'This is sent by the server to notify open connections of name changes.
            
            'Update the name list.
            lstConnections.List(sParam(sData, 1)) = sLongParam(sData, 2)
    End Select
    
    'Remove the processed command from the data stream.
    vsString = Mid(vsString, InStr(1, vsString, vbCrLf) + 2, Len(vsString))
Loop
End Sub
Private Sub cmdClearDraw_Click()
'Someone clicked the Clear button.  This clears the drawing.

Dim i As Integer

If sckConnect.State = sckConnected Then
    'If you are connected to the server, send the command to the server.
    SendList.Add "[Clear Draw]"
    SendTo.Add "sckConnect"
Else
    'If you are the server, send the command to all open connections.
    For i = 0 To giMaxConnections - 1
        If sckConnection(i).State = sckConnected Then
            SendList.Add "[Clear Draw]"
            SendTo.Add "sckConnection(" & i & ")"
        End If
    Next i
End If
picDraw.Cls
End Sub
Private Sub cmdHost_Click()
'Someone clicked the Host button to host a chat room.

Dim i As Integer

'Don't allow this to happen if connections are already open.
If bConnected Then
    MsgBox "One or more connections are currently open.  Disconnect before attempting to host a chat room.", vbInformation, App.Title
    Exit Sub
End If

'Clear stuff to start a new chat room (name list, dialog, etc.)
ClearStuff

'Close the Winsock control that allows you to connect to the server.
sckConnect.Close

'Listen on all possible connections on the appropiate ports.
For i = 0 To giMaxConnections - 1
    sckConnection(i).Close
    sckConnection(i).LocalPort = glFirstLocalPort + i
    sckConnection(i).RemotePort = glFirstRemotePort + i
    lstConnections.AddItem "[Open]"
Next i

'Call the procedure that ensures that one winsock control is listening for connections.
MakeListen

'Update the status.
txtStatus.Text = txtStatus.Text & vbCrLf & "Hosting."
'Show the host's name.
lblHost.Caption = txtName.Text

'Show the Kick button.  This is only available to the server.
cmdKick.Visible = True
End Sub
Private Sub cmdKick_Click()
'The server decided to kick some people.

Dim i As Integer, j As Integer
Dim sFormatStr As String

For i = 1 To PARAM_LEN
    sFormatStr = sFormatStr & "0"
Next i

'Check who is selected on the name list.
For i = 0 To giMaxConnections - 1
    If lstConnections.Selected(i) Then
        'When a selected name is found, nofity all open connections that this person was kicked.
        'But do not send this information to other people who are being kicked.
        For j = 0 To giMaxConnections - 1
            If sckConnection(j).State = sckConnected And lstConnections.Selected(j) = False Then
                SendList.Add "[Kicked] " & Format(i, sFormatStr)
                SendTo.Add "sckConnection(" & j & ")"
            End If
        Next j
        'Close the connection.
        sckConnection(i).Close
        'Call the procedure that ensures that one winsock control is listening for connections.
        MakeListen
        'Update the status.
        txtStatus.Text = txtStatus.Text & vbCrLf & "Kicked " & lstConnections.List(i) & "."
        'Reset their name in the name list.
        lstConnections.List(i) = "[Open]"
    End If
Next i

'Deselect all names from the name list.
For i = 0 To giMaxConnections - 1
    lstConnections.Selected(i) = False
Next i
End Sub
Private Sub cmdPorts_Click()
'Someone clicked the Port Settings button.

'Do not allow this to happen if connections are open.
If bConnected Then
    MsgBox "One or more connections are currently open.  Disconnect before attempting to change the port settings.", vbInformation, App.Title
    Exit Sub
End If

'Show frmPorts.
frmPorts.Show vbModal
End Sub
Private Sub cmdSend_Click()
'Someone clicked the Send button to send a message.

Dim i As Integer

If sckConnect.State = sckConnected Then
    'If you are connected to the server, send the message to the server.
    SendList.Add "[Message] " & txtName.Text & ": " & txtMessage.Text
    SendTo.Add "sckConnect"
Else
    'If you are the server, send the message to all open connections.
    For i = 0 To giMaxConnections - 1
        If sckConnection(i).State = sckConnected Then
            SendList.Add "[Message] " & txtName.Text & ": " & txtMessage.Text
            SendTo.Add "sckConnection(" & i & ")"
        End If
    Next i
End If

'Update the message dialog.
txtDialog.Text = txtDialog.Text & txtName.Text & ": " & txtMessage.Text & vbCrLf
End Sub
Private Sub cmdConnect_Click()
'Someone clicked the Connect button to connect to someone acting as a server.

Dim i As Integer

'Do not allow this to happen if connections are open.
If bConnected Then
    MsgBox "One or more connections are currently open.  Disconnect before attempting to connect to a chat room.", vbInformation, App.Title
    Exit Sub
End If

'The program will attempt to connect on each possible port, beginning with the first one possible.
'If the connection is not possible, an error will occur so this error handler will increment the port being used and try again.
On Error GoTo Err_cmdConnect_Click

'Clear stuff to start a new chat room (name list, dialog, etc.)
ClearStuff

'Update the status.
txtStatus.Text = txtStatus.Text & vbCrLf & "Connecting..."

'Make sure all connections are closed.
For i = 0 To giMaxConnections - 1
    sckConnection(i).Close
Next i

'Close the port being used to connect.
sckConnect.Close
'Set the ports to the first possible ports.
sckConnect.LocalPort = glFirstRemotePort
sckConnect.RemotePort = glFirstLocalPort
'Attempt to connect.
'If it cannot connect, a run-time error occurs.  See error handler below.
sckConnect.Connect txtIP.Text

'If the program has gotten here, it is connected.

'Show the list of connections (empty for now).
For i = 0 To giMaxConnections - 1
    lstConnections.AddItem "[Open]"
Next i

'Send the user's name to the server.
SendList.Add "[Joined] " & txtName.Text
SendTo.Add "sckConnect"

Exit Sub

Err_cmdConnect_Click:
'An error has occured connecting.
'Increment the port and try again.
sckConnect.Close
sckConnect.LocalPort = sckConnect.LocalPort + 1
sckConnect.RemotePort = sckConnect.RemotePort + 1
If sckConnect.LocalPort = glFirstRemotePort + giMaxConnections Then
    'If it is not possible to connect on any port...
    'Update the status.
    txtStatus.Text = txtStatus.Text & vbCrLf & "Unable to connect."
    'Close the port.
    sckConnect.Close
    Exit Sub
End If
Resume
End Sub
Private Sub cmdDisconnect_Click()
'Someone clicked the Disconnect button to break a connection.

Dim i As Integer

'Connections to the server cannot be broken by the person who connected.
'The connection must be closed by the server.
'If you are connected to the server, send a request to the server to be disconnected.
If sckConnect.State = sckConnected Then
    If SendList.Count = 0 Then
        'If SendList is empty, just add the request.
        SendList.Add "[Disconnect]"
        SendTo.Add "sckConnect"
    Else
        'If SendList is not empty, add the request at the beginning to give it top priority.
        SendList.Add "[Disconnect]", , 1
        SendTo.Add "sckConnect", , 1
    End If
    'Update the status.
    txtStatus.Text = txtStatus.Text & vbCrLf & "Disconnecting..."
    Exit Sub
End If

'Close all connections.
sckConnect.Close
For i = 0 To giMaxConnections - 1
    sckConnection(i).Close
Next i

'Update status.
txtStatus.Text = txtStatus.Text & vbCrLf & "Disconnected."

'Clear stuff to start a new chat room (name list, dialog, etc.)
ClearStuff
End Sub
Private Sub Form_Load()
Dim i As Integer

'Call the procedure that gets the user preferences from an INI file.
GetPreferences

'Load enough Winsock controls to handle all connections.
For i = 1 To giMaxConnections - 1
    Load sckConnection(i)
Next i
End Sub
Private Sub Form_Unload(Cancel As Integer)
Dim i As Integer

'This flag is set after you have been disconnected from the server following an attempt to close the program.
'When this flag is set, the form will just unload - skipping the code below.
If bJustQuit Then
    'Call the procedure that writes the user preferences to an INI file.
    WritePreferences
    Exit Sub
End If

'Confirm desire to quit.
If MsgBox("Are you sure you want to exit?", vbQuestion + vbYesNo, App.Title) = vbNo Then
    Cancel = True
    Exit Sub
End If

'If you are connected to the server, send a request to the server to be disconnected.
'Do not allow the user to quit the program until disconnected from the server.
If sckConnect.State = sckConnected Then
    'Send the request to be disconnected.
    SendList.Add "[Disconnect]"
    SendTo.Add "sckConnect"
    'Update the status.
    txtStatus.Text = txtStatus.Text & vbCrLf & "Disconnecting..."
    'Set the sckConnect.Tag property to "Quit" so when disconnected, the program knows to quit immediately.
    sckConnect.Tag = "Quit"
    'Do not Unload the form.
    Cancel = True
    Exit Sub
End If

'Close all connections.
sckConnect.Close
For i = 0 To giMaxConnections - 1
    sckConnection(i).Close
Next i
    
'Call the procedure that writes the user preferences to an INI file.
WritePreferences
End Sub
Private Sub picDraw_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
'The mouse is over the drawing surface and the button was just pressed.

Dim siX1 As String
Dim siY1 As String
Dim siX2 As String
Dim siY2 As String
Dim sFormatStr As String, sFormatStr2 As String
Dim i As Integer

'The coordinates of the line to be drawn must be exactly PARAM_LEN characters long.
'So if one coordinate is negative, the "-" counts as a character and the number part must be only (PARAM_LEN - 1) characters.
For i = 1 To PARAM_LEN
    sFormatStr = sFormatStr & "0"
Next i

For i = 1 To PARAM_LEN - 1
    sFormatStr2 = sFormatStr2 & "0"
Next i

'Format the coordinates.
If X >= 0 Then
    siX2 = Format(X, sFormatStr)
Else
    siX2 = Format(X, sFormatStr2)
End If
If Y >= 0 Then
    siY2 = Format(Y, sFormatStr)
Else
    siY2 = Format(Y, sFormatStr2)
End If

'Draw the line.
'Since the mouse was just pressed, the line will just be a point where the mouse was clicked.
picDraw.Line (X, Y)-(X, Y), sCurrentColor

'Remember where the mouse is so new lines can be drawn connecting to this point.
iX = X
iY = Y

If sckConnect.State = sckConnected Then
    'If you are connected to the server, send the info on the line to the server.
    SendList.Add "[Line] " & siX2 & siY2 & siX2 & siY2 & sCurrentColor
    SendTo.Add "sckConnect"
Else
    'If you are the server, send the info on the line to all open connections.
    For i = 0 To giMaxConnections - 1
        If sckConnection(i).State = sckConnected Then
            SendList.Add "[Line] " & siX2 & siY2 & siX2 & siY2 & sCurrentColor
            SendTo.Add "sckConnection(" & i & ")"
        End If
    Next i
End If
End Sub
Private Sub picDraw_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim siX1 As String
Dim siY1 As String
Dim siX2 As String
Dim siY2 As String
Dim sFormatStr As String, sFormatStr2 As String
Dim i As Integer

If Button = vbLeftButton Then
    'The mouse button is down and the mouse is moving over the drawing surface.
    
    'The coordinates of the line to be drawn must be exactly PARAM_LEN characters long.
    'So if one coordinate is negative, the "-" counts as a character and the number part must be only (PARAM_LEN - 1) characters.
    For i = 1 To PARAM_LEN
        sFormatStr = sFormatStr & "0"
    Next i
    For i = 1 To PARAM_LEN - 1
        sFormatStr2 = sFormatStr2 & "0"
    Next i
    
    'Format the coordinates.
    'This time the line is being drawn from the previous mouse coordinates to its current coordinates.  (See the MouseDown event.)
    If iX >= 0 Then
        siX1 = Format(iX, sFormatStr)
    Else
        siX1 = Format(iX, sFormatStr2)
    End If
    If iY >= 0 Then
        siY1 = Format(iY, sFormatStr)
    Else
        siY1 = Format(iY, sFormatStr2)
    End If
    If X >= 0 Then
        siX2 = Format(X, sFormatStr)
    Else
        siX2 = Format(X, sFormatStr2)
    End If
    If Y >= 0 Then
        siY2 = Format(Y, sFormatStr)
    Else
        siY2 = Format(Y, sFormatStr2)
    End If
    
    'Draw the line.
    picDraw.Line (iX, iY)-(X, Y), sCurrentColor
    If sckConnect.State = sckConnected Then
        'If you are connected to the server, send the info on the line to the server.
        SendList.Add "[Line] " & siX1 & siY1 & siX2 & siY2 & sCurrentColor
        SendTo.Add "sckConnect"
    Else
        'If you are the server, send the info on the line to all open connections.
        For i = 0 To giMaxConnections - 1
            If sckConnection(i).State = sckConnected Then
                SendList.Add "[Line] " & siX1 & siY1 & siX2 & siY2 & sCurrentColor
                SendTo.Add "sckConnection(" & i & ")"
            End If
        Next i
    End If
    
    'Remember where the mouse is so new lines can be drawn connecting to this point.
    iX = X
    iY = Y
End If
End Sub

Private Sub picSelColor_Click(Index As Integer)
Dim i As Integer

For i = 0 To picSelColor.UBound
    picSelColor(i).BorderStyle = 0
Next i

picSelColor(Index).BorderStyle = 1
End Sub

Private Sub sckConnect_Close()
'This occurs when the connection to the server is broken.

'Update the status.
txtStatus.Text = txtStatus.Text & vbCrLf & "Disconnected."
'Close the connection
sckConnect.Close
'Clear the names list.
lstConnections.Clear
'Clear the label holding the host's name.
lblHost.Caption = ""

'Clear stuff to start a new chat room (name list, dialog, etc.)
ClearStuff

'If the user tried to quit the program, generating the request to be disconnected, set the flag to quit and unload the form.  (See Form_Unload.)
If sckConnect.Tag = "Quit" Then
    bJustQuit = True
    Unload Me
End If
End Sub
Private Sub sckConnect_DataArrival(ByVal bytesTotal As Long)
'Data has arrived at the computer connected to the server.

Dim sString As String

'Get the data.
sckConnect.GetData sString, vbString

'Process the data.  Pass -1 for the computer sending the data because it was from the server.
ProcessData sString, -1
End Sub
Private Sub sckConnection_Close(Index As Integer)
'One of the connections to the server was closed.

'Close the connection.
sckConnection(Index).Close

'If a chat room is up, begin to listen again.
If lstConnections.ListCount > 0 And sckConnect.State <> sckConnected Then
    'Call the procedure that ensures that one winsock control is listening for connections.
    MakeListen
End If
End Sub
Private Sub sckConnection_ConnectionRequest(Index As Integer, ByVal requestID As Long)
'A connection was requested from the server.

'Close the port.
sckConnection(Index).Close
'Accept the connection.
sckConnection(Index).Accept requestID

'Send information on the Host to the connection.
SendList.Add "[Host] " & txtName.Text
SendTo.Add "sckConnection(" & Index & ")"

'Call the procedure that ensures that one winsock control is listening for connections.
MakeListen
End Sub
Private Sub sckConnection_DataArrival(Index As Integer, ByVal bytesTotal As Long)
'Data has arrived at the server from an open connection.
Dim sString As String

'Get the data.
sckConnection(Index).GetData sString, vbString

'Process the data.  Pass the index of the connection from which the data came.
ProcessData sString, Index
End Sub
Private Sub tmrSendData_Timer()
'The is the timer that continuously checks for data to send.

'Remembers whether or not something has been sent.
'Only one piece of data can be sent at a time, otherwise the data runs togeter.
Dim bSent As Boolean

'Index variable to determine which piece of data from the queue will be sent.
Dim iSend As Long

'Remembers where the data will be sent.
Dim iConnection As Integer

'Start the index variable at 1.
iSend = 1

'Loop while nothing has been sent and while the index variable is less than the maximum.
Do While bSent = False And iSend <= SendTo.Count
    If SendTo.Item(iSend) = "sckConnect" And sckConnect.State = sckConnected Then
        'Check to see if it is to be sent to the server and make sure the connection is still open.
        
        'Send the data.
        sckConnect.SendData SendList.Item(iSend) & vbCrLf
    
        'Delete the data from the queue.
        SendTo.Remove iSend
        SendList.Remove iSend
        
        'Something has been sent.
        bSent = True
    ElseIf Mid(SendTo.Item(iSend), 1, 13) = "sckConnection" Then
        'Check to see if it is to be sent to one of the connections to you, the server.
    
        'Parse the string containing the name of the connection to determine which connection to send to.
        iConnection = Mid(SendTo.Item(iSend), 15, Len(SendTo.Item(iSend)) - 15)
        
        'Ensure that the connection is open.
        If sckConnection(iConnection).State = sckConnected Then
            'Send the data.
            sckConnection(iConnection).SendData SendList.Item(iSend) & vbCrLf
        
            'Delete the data from the queue.
            SendTo.Remove iSend
            SendList.Remove iSend
            
            'Something has been sent.
            bSent = True
        End If
    End If
    
    'Increment index variable.
    iSend = iSend + 1
Loop
End Sub
Private Sub txtDialog_Change()
'The text in the message dialog text box has changed.

'Put the selection point at the end of the text box so you are seeing the most recent text.
txtDialog.SelStart = Len(txtDialog.Text)
End Sub
Private Sub txtIP_KeyPress(KeyAscii As Integer)
If KeyAscii = vbKeyReturn Then
    'If enter was pressed in the text box that inputs an IP address to connect to, simulate the pressing of the Connect button.
    cmdConnect_Click
    'Make VB think nothing was pressed on the keyboard.  This prevents it from making an annoying beep.
    KeyAscii = 0
End If
End Sub
Private Sub txtMessage_KeyPress(KeyAscii As Integer)
If KeyAscii = vbKeyReturn Then
    'If enter was pressed in the text box that inputs a message to send, simulate the pressing of the Send button.
    cmdSend_Click
    'Clear the text box.
    txtMessage.Text = ""
    'Make VB think nothing was pressed on the keyboard.  This prevents it from making an annoying beep.
    KeyAscii = 0
End If
End Sub
Private Sub txtName_KeyPress(KeyAscii As Integer)
Dim i As Integer

If KeyAscii = vbKeyReturn Then
    'If enter was pressed in the text box that inputs your name, update your name on your screen and on all other computers.
    
    'If you are connected to the server, send the Change Name command.
    If sckConnect.State = sckConnected Then
        SendList.Add "[Change Name] " & txtName.Text
        SendTo.Add "sckConnect"
    End If
    
    'If you are the server, update your name on your screen.
    If lstConnections.ListCount > 0 And sckConnect.State <> sckConnected Then
        lblHost.Caption = txtName.Text
        
        'Send your name to all open connections.
        For i = 0 To giMaxConnections - 1
            If sckConnection(i).State = sckConnected Then
                SendList.Add "[Host Change Name] " & txtName.Text
                SendTo.Add "sckConnection(" & i & ")"
            End If
        Next i
    End If
        
    'Make VB think nothing was pressed on the keyboard.  This prevents it from making an annoying beep.
    KeyAscii = 0
End If
End Sub
Private Sub txtStatus_Change()
'The text in the status text box has changed.

'Put the selection point at the end of the text box so you are seeing the most recent text.
txtStatus.SelStart = Len(txtStatus.Text)

'If there is a blank carriage return at the beginning, delete it.
If Mid(txtStatus.Text, 1, 2) = vbCrLf Then
    txtStatus.Text = Mid(txtStatus.Text, 3, Len(txtStatus.Text))
End If
End Sub
Public Sub SendPeopleList(viConnection As Integer)
'This is a procedure to send the connection list to a particular connection so that user knows who is connected.

Dim i As Integer, j As Integer
Dim sFormatStr As String

For i = 1 To PARAM_LEN
    sFormatStr = sFormatStr & "0"
Next i

'Make sure that user is connected.
If sckConnection(viConnection).State = sckConnected Then

    'Send the name for each connection.
    For i = 0 To giMaxConnections - 1
        SendList.Add "[People] " & Format(i, sFormatStr) & lstConnections.List(i)
        SendTo.Add "sckConnection(" & viConnection & ")"
    Next i
    
    'Send the host's name as well.
    SendList.Add "[Host Refresh] " & txtName.Text
    SendTo.Add "sckConnection(" & viConnection & ")"
End If
End Sub
Public Sub SendPersonName(viConnection As Integer)
'This is a procedure to send a specific user's name to all open connections.

Dim i As Integer, j As Integer
Dim sFormatStr As String

For i = 1 To PARAM_LEN
    sFormatStr = sFormatStr & "0"
Next i

'Check to see which connections are open and send that name to them.
For i = 0 To giMaxConnections - 1
    If sckConnection(i).State = sckConnected Then
        SendList.Add "[People] " & Format(viConnection, sFormatStr) & lstConnections.List(viConnection)
        SendTo.Add "sckConnection(" & i & ")"
    End If
Next i
End Sub
Public Sub ClearStuff()
'This procedure clears stuff out that is used during a chat room.
'It is used to reset stuff after a chat room is closed.

Dim i As Integer

'Clear the data queue.
For i = 1 To SendList.Count
    SendList.Remove 1
Next i
For i = 1 To SendTo.Count
    SendTo.Remove 1
Next i

'Hide the Kick button.
cmdKick.Visible = False

'Clear the host's name.
lblHost.Caption = ""

'Clear the connection list.
lstConnections.Clear

'Clear the dialog.
txtDialog.Text = ""

'Clear the drawing.
picDraw.Cls
End Sub
Public Function sParam(vsData As String, viNum As Integer) As String
'This function pulls the (viNum)th parameter from datastream vsData, which is being processed in the ProcessData procedure.
'This parameter is exactly PARAM_LEN characters long.

sParam = Mid(vsData, PARAM_LEN * (viNum - 1) + 1, PARAM_LEN)
End Function
Public Function sLongParam(vsData As String, viNum As Integer) As String
'This function pulls the (viNum)th parameter from datastream vsData, which is being processed in the ProcessData procedure.
'This parameter can be any length and is usually at the end of a command.
'This type of parameter usually contains a name and is therefore not a fixed length.

sLongParam = Mid(vsData, PARAM_LEN * (viNum - 1) + 1, Len(vsData))
End Function
Public Function sCurrentColor() As String
'This function checks to see which color button is selected for drawing.
'The color is returned formatted as a parameter so it can be transmitted.

Dim i As Integer
Dim sFormatStr As String

For i = 1 To PARAM_LEN
    sFormatStr = sFormatStr & "0"
Next i

'Check all color buttons for the selected one.
For i = 0 To picSelColor.UBound
    If picSelColor(i).BorderStyle = 1 Then
        sCurrentColor = Format(picSelColor(i).BackColor, sFormatStr)
        Exit Function
    End If
Next i
End Function
Public Function bConnected() As Boolean
'This function returns True if any connections are open.
'This is used to see if you are allowed to change port settings, host a chat room, or connect to a chat room.

Dim i As Integer

For i = 0 To giMaxConnections - 1
    If sckConnection(i).State <> sckClosed Then
        bConnected = True
        Exit Function
    End If
Next i

If sckConnect.State <> sckClosed Then
    bConnected = True
End If
End Function

Public Sub MakeListen()
'This procedure ensures that exactly one winsock control is listening for connections.

Dim i As Integer

For i = 0 To giMaxConnections - 1
    If sckConnection(i).State = sckClosed Then
         sckConnection(i).Listen
         Exit Sub
    End If
Next i
End Sub

Public Sub GetPreferences()
'This procedure reads the user preferences from an INI file.
'This program is not designed to teach how to use INI files,
'therefore this section is not documented.

Dim lLen As Long
Dim sTemp As String * 5

lLen = GetPrivateProfileString("Many Chat", "First Local Port", "600", sTemp, 5, "manychat.ini")
glFirstLocalPort = Mid(sTemp, 1, lLen)

lLen = GetPrivateProfileString("Many Chat", "First Remote Port", "700", sTemp, 5, "manychat.ini")
glFirstRemotePort = Mid(sTemp, 1, lLen)

lLen = GetPrivateProfileString("Many Chat", "Maximum Connections", "8", sTemp, 5, "manychat.ini")
giMaxConnections = Mid(sTemp, 1, lLen)
End Sub

Public Sub WritePreferences()
'This procedure reads the user preferences from an INI file.
'This program is not designed to teach how to use INI files,
'therefore this section is not documented.

Dim lRetVal As Long
Dim sTemp As String

sTemp = glFirstLocalPort
lRetVal = WritePrivateProfileString("Many Chat", "First Local Port", sTemp, "manychat.ini")

sTemp = glFirstRemotePort
lRetVal = WritePrivateProfileString("Many Chat", "First Remote Port", sTemp, "manychat.ini")

sTemp = giMaxConnections
lRetVal = WritePrivateProfileString("Many Chat", "Maximum Connections", sTemp, "manychat.ini")
End Sub
