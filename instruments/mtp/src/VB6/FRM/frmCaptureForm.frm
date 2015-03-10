VERSION 5.00
Begin VB.Form frmCaptureForm 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Capture Form"
   ClientHeight    =   2085
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   1875
   Icon            =   "frmCaptureForm.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2085
   ScaleWidth      =   1875
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picImage 
      Height          =   1980
      Left            =   15
      ScaleHeight     =   1920
      ScaleWidth      =   1755
      TabIndex        =   0
      Top             =   60
      Width           =   1815
      Begin VB.PictureBox Picture1 
         Height          =   15
         Left            =   525
         ScaleHeight     =   15
         ScaleWidth      =   30
         TabIndex        =   1
         Top             =   735
         Width           =   30
      End
   End
End
Attribute VB_Name = "frmCaptureForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

