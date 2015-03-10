VERSION 5.00
Begin VB.Form frmNetCDF 
   Caption         =   "Form1"
   ClientHeight    =   2415
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6840
   LinkTopic       =   "Form1"
   ScaleHeight     =   2415
   ScaleWidth      =   6840
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdTest 
      Caption         =   ".NET"
      Height          =   345
      Index           =   2
      Left            =   1845
      TabIndex        =   4
      Top             =   90
      Width           =   825
   End
   Begin VB.TextBox txtOut 
      Height          =   1395
      Left            =   105
      TabIndex        =   3
      Top             =   945
      Width           =   6690
   End
   Begin VB.CommandButton cmdTest 
      Caption         =   "Test 2"
      Height          =   345
      Index           =   1
      Left            =   975
      TabIndex        =   2
      Top             =   90
      Width           =   825
   End
   Begin VB.CommandButton cmdRead 
      Caption         =   "Read"
      Height          =   345
      Left            =   105
      TabIndex        =   1
      Top             =   510
      Width           =   795
   End
   Begin VB.CommandButton cmdTest 
      Caption         =   "Test 1"
      Height          =   345
      Index           =   0
      Left            =   75
      TabIndex        =   0
      Top             =   90
      Width           =   825
   End
End
Attribute VB_Name = "frmNetCDF"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub ReadTestFile(ByVal name As String)
Dim ncid As Long
Dim dimid As Long
Dim varid As Long
Dim natts As Long, nvars As Long, ndims As Long, unlimdimid As Long
Dim dimlen As Long, attlen As Long
Dim dimids(1) As Integer
Dim xtype As nc_type
Dim res As Long
Dim dn As String
Dim i As Integer, j As Integer
Dim dimname As String 'NewString  'Builder(NetCDF.netCDF_limits.NC_MAX_NAME)

On Error Resume Next

  For i = 1 To nc_max_name
    dimname = dimname + Chr(0)
  Next i
' Open the netcdf file and read it out.
  res = nc_open(name, cmode.NC_NOWRITE, ncid)

  res = nc_inq(ncid, ndims, nvars, natts, unlimdimid)

  Log ("Read " & Str(ndims) & " dims, " & Str(nvars) & " vars, " & _
            Str(natts) & " global atts, and an unlimited dimension of " & Str(unlimdimid) & ".")

' Read dimension metadata.
  For i = 0 To ndims - 1
    res = nc_inq_dim(ncid, i, dimname, dimlen)
    If (res <> 0) Then GoTo err
    Log ("dimid: " & Str(i) & " name: " & dimname & " len: " & Str(dimlen))
  Next

' Read global attributes.
  For i = 0 To natts - 1
    Call ReadAtt(ncid, NetCDF.NC_GLOBAL, i)
  Next

' Read variables, including their attributes.
  For i = 0 To nvars - 1
   ' Call ReadVar(ncid, i)
  Next

' Close the netCDF file.
  res = nc_close(ncid)
  If (res <> 0) Then GoTo err
  Exit Sub

err:
  MsgBox ("Error:" & nc_strerror(res))

End Sub
Private Sub ReadAtt(ByVal ncid As Long, ByVal varid As Long, ByVal attnum As Long)
' Read an attribute, given an ncid for an open file, a varid (which can be
' NetCDF.NC_GLOBAL for file (i.e. global) attributes), and an attnum. This
' function will learn everything about the attribute, and log the results.

Dim attname As String 'New StringBuilder(NetCDF.netCDF_limits.NC_MAX_NAME)
Dim atttext As String 'Builder,
Dim attbyte() As Byte
Dim attint() As Long
Dim xtype As nc_type
Dim attlen As Long
Dim res As Long
Dim i As Integer

        'First find the name, given the attnum.
        res = nc_inq_attname(ncid, varid, attnum, attname)
        If (res <> 0) Then GoTo err

        'Now find the type and length, using the name.
        res = nc_inq_att(ncid, varid, attname, xtype, attlen)
        If (res <> 0) Then GoTo err
        Log ("varid: " & Str(varid) & " attnum: " & Str(attnum) & " name: " & _
            attname & " len: " & Str(attlen) & " type: " & Str(xtype))

        'Now read the value, depending on the type.
        Select Case xtype
            Case nc_type.NC_CHAR
                'There's possibly some size limits I should be checking for here?
                atttext = Space$(attlen)
                res = nc_get_att_text(ncid, varid, attname, atttext)
                If (res <> 0) Then GoTo err
                Log ("text value: " & atttext)

            Case nc_type.NC_BYTE
                ReDim attbyte(attlen - 1)
                res = nc_get_att_uchar(ncid, varid, attname, attbyte)
                If (res <> 0) Then GoTo err
                For i = 0 To attlen - 1
                    Log ("index: " & Str(i) & "value: " & Str(attbyte(i)))
                Next

            Case nc_type.NC_INT
                ReDim attint(attlen - 1)
                res = nc_get_att_int(ncid, varid, attname, attint)
                If (res <> 0) Then GoTo err
                For i = 0 To attlen - 1
                    Log ("index: " & Str(i) & "value: " & Str(attint(i)))
                Next



        End Select

        Exit Sub
err:
        MsgBox ("error:" & NetCDF.nc_strerror(res))

    End Sub
Private Sub Log(ByVal msg As String)
' Append any string passed in to the textbox. Add a CR/LF.
  txtOut.Text = txtOut.Text & msg & Chr(13) & Chr(10)
End Sub
Sub gennet()
'
'    PROGRAM GENNET.FOR
'
'     Test program to read any netCDF file
'                                                     Barry Schwartz
'                                                     October 1993
'                                                     modified Apr 1996
'
'
'     This program will ask the user for the name of a netcdf file
'     to read. It will open that file and using the proper netCDF
'     calls, get info on the variables and their dimensions. It then
'     generates a FORTRAN program that can be used to actually read the
'     netCDF file and fills the variables with data. This program
'     can be used to read ANY netCDF file. The user only has to
'     write fortran statements to print the data or pass to another
'     program. Once you have generated a program, you may use it
'     to read any file of that data type; i.e., the program is general
'     until the powers to be change variable attributes.
'
'     to compile: f77 gennet.for /usr/local/netcdf/lib/libnetcdf.a
'
'
      include 'netcdf.inc' !THIS INCLUDE FILE CONTAINS CONSTANTS,ETC
'
      PARAMETER (MVRLEN=3) !MAX NUMBER OF VALID-RANGE VALUES
      PARAMETER (MTLEN=80) !MAX LENGTH OF TITLE ATTRIBUTES
      INTEGER NCID,RCODE,NDIMS,NVARS,NATTS,RECDIM,DIMID,VARID
      INTEGER VARTYP(100),VDIMS(100),vvdims(100,10),NVATTS(100)
      CHARACTER*31 DIMNAM(100) !ARRAY HOLDING DIMENSION NAMES
      CHARACTER*31 VARNAM(100) !ARRAY HOLDING VARIABLE NAMES
      CHARACTER*31 ATTNAM(100,10) !array holding attribute name
      character*50 long_name(100)
      INTEGER DIMSIZ(100) !ARRAY HOLDING DIMENSION SIZES
      INTEGER nvdims(100)
      INTEGER ATTYPE(100,10),ATTLEN(100,10)
'                       !NOTE MAX NUMBER OF VARIABLES IS 100
'                       !     MAX NUMBER OF ATTRIBUTES PER VAR=10
'      INTEGER START(50,10),COUNT(50,10) !ALLOW 50 VARIABLES WITH 10 DIMS
      CHARACTER*11 AVARTYP(6),AVT(100)
      CHARACTER*72 INPUT_FILE
      DATA AVARTYP /'LOGICAL*1','CHARACTER*1','INTEGER*2',
     +'INTEGER*4','REAL*4','REAL*8'/
'
'
      open(unit=10,file='readnet.for',status='unknown')
'
      WRITE(6,5)
    5 FORMAT(' ENTER NETCDF FILE NAME')
      READ(5,6) INPUT_FILE
6     Format (A72)
'
'     OPEN NETCDF FILE

      ncid = NCOPN(INPUT_FILE, NCNOWRIT, rcode)
'
'     write(6,10) NCID,RCODE
  10  format('ncid= ',i6,1x,' rcode= ',i5)
'_________________________________________________________________
'   ***inquire about the number of dimensions, varaibles, attributes
'          DIMENSION IDS RUN SEQUENTIALLY FROM 1 TO NDIMS
'
      Call NCINQ(ncid, ndims, nvars, natts, RECDIM, rcode)
'
'     WRITE(6,20) NDIMS,NVARS,NATTS,RECDIM,RCODE
'   20 FORMAT('NDIMS= ',I3,' NVARS= ',I3,' NATTS= ',I3,
' 1    'RECDIM= ',I6,' RCODE FROM NCINQ= ',I5)
'
'   ***now store the dimension names and sizes in arrays
'
      DO 100 I=1,NDIMS !NUMBER OF DIMENSIONS RETURNED FROM NCINQ
      dimid = i
      Call NCDINQ(ncid, dimid, DIMNAM(i), DIMSIZ(i), rcode)
'
'     DIMENSION IDS ARE I, I.E, 1,NDIMS
'     DIMNAM ARE DIMENSION NAMES (CHARACTER IDS)
'     DIMSIZ IS THE SIZE OF EACH DIMENSION
'
'     RECDIM IS THE ID OF THE RECORD DIMENSION
'
      IF(RECDIM.NE.-1) NUMRECS=DIMSIZ(RECDIM)
      IF(RCODE.NE.0) THEN
'                    WRITE(6,75) I
   75 FORMAT(' NONE ZERO RETURN CODE: DIMENSION ID= ',I3)
                     STOP 80
                     End If
'     WRITE(6,70) I,DIMNAM(I),DIMSIZ(I)
   70 FORMAT(' DIMENSION ID= ',I3,' NAME= ',A31,' SIZE= ',I3)
100   CONTINUE
'
' Done
'___________________________________________________________________
' VARIABLEs
'
'     VARIABLES LIKE DIMENSIONS..RUN SEQUENTIALLY FROM 1 TO NVARS
      DO 200 I=1,NVARS
      varid = i
'
'     get long_names
'
      call ncagtc(ncid,i,'long_name',long_name(i),mtlen,rcode)
'
'     NCVINQ gets vaiable names, their types and their shapes
'
      CALL NCVINQ(NCID,VARID,VARNAM(I),VARTYP(I),NVDIMS(I),
     +VDIMS,NVATTS(I),RCODE)
'
'     BE CAREFUL...VDIMS IS AN ARRAY (SIZE NVDIMS(I) THUS THE USE OF 2ND ARRAY
      IF(NVDIMS(I).NE.0) THEN
      DO 175 K=1,NVDIMS(I)
      VVDIMS(I,K)=VDIMS(K) !VVDIMS CONTAINS THE DIMENSION ID'S
175   CONTINUE
                          End If
'
'     VARNAM=VARIABLE NAMES
'     VARTYP=VARIABLE TYPES
'     NVDIMS=NUMBER OF DIMENSIONS FOR VARIABLE
'     VVDIMS= NVDIMS DIMENSION IDS FOR THIS VARIABLE
'     NVATTS=NUMBER OF ATTRIBUTES FOR VARIABLE
'
'     WRITE(6,150) VARID,VARNAM(I),VARTYP(I),NVDIMS(I),NVATTS(I),
'    +(vvdims(i,k),k=1,nvdims(i))
  150 FORMAT(/,' VAR ID= ',I2,' VARNAM= ',A10,' VARTYP= ',I1,
     +' NVDIMS= ',I1,'NUM ATTS=',i2,' vdims= ',<nvdims(i)>i3)
200   CONTINUE
'
'
'     DONE WITH VARIABLES....
'_____________________________________________________________________
'                            NOW GET INFO ON THE VARIABLE'S ATTRIBUTES
'
      DO 300 I=1,NVARS !GET ATTRIBUTES FOR ALL VARIABLES
      varid = i
      DO 250 J=1,NVATTS(I)
      CALL NCANAM(NCID,VARID,J,ATTNAM(I,J),RCODE) !get attribute names
'
'     GET ATTRIBUTE TYPES AND LENGTH
      CALL NCAINQ(NCID,VARID,ATTNAM(I,J),ATTYPE(I,J),ATTLEN(I,J),
     +RCODE)
'
'     GET ATTRIBUTE VALUES; BE CAREFUL FIRST MUST KNOW IF CHAR OR NUMBER
'
'      IF(ATTYPE(I,J).EQ.2) THEN
'                           IF(ATTLEN(I,J).GT.MTLEN) THEN
'                           WRITE(6,245)
'  245 FORMAT(' PROBLEM WITH ATTRIBUTE NAME: TOO LONG')
'                           STOP
'                                                    ENDIF
'      CALL NCAGTC(NCID,VARID,ATTNAM(I,J),ATTIT(I,J),MTLEN,RCODE)
'                            ENDIF
'      IF(ATTYPE(I,J).NE.2) THEN
'      CALL NCAGT(NCID,VARID,ATTNAM(I,J),VRVAL(I,J),RCODE)
250   CONTINUE
'
'      WRITE(6,275) VARNAM(I),(J,ATTNAM(I,J),ATTYPE(I,J),ATTLEN(I,J),
'     1J=1,NVATTS(I))
  275 FORMAT(/,' VARIABLE= ',A31,
     +<NVATTS(I)>(/,' ATTRIBUTE #',I2,' IS: ',A31,'TYPE= ',I3,
     +' LEN= ',I3))
'
'     END OF ATTRIBUTE CALLS
'
300   CONTINUE
'
'****************************************************************
'              ****DONE NOW GENERATE FORTRAN PROGRAM INTERFACE*****
'
      DO 500 I=1,NVARS
'
      varid = i
      AVT(I)=AVARTYP(VARTYP(VARID)) !AVT IS THE CHARACTER NAME OF VARTYP
500   CONTINUE
'
'     NOW GENERATE FORTRAN TEMPLATE WITH VARIABLE TYPES, NAMES AND DIMS
'                          ALLOW VARIABLEs TO HAVE 4 DIMENSIONS
'
      write(10,510) INPUT_FILE
  510 FORMAT(''C    FORTRAN TEMPLATE FOR FILE= ',A40)
'     write(10,515)
' 515 format(6x,'INCLUDE',1H','netcdf.inc',1H')
      WRITE(10,550) NVARS
  550 FORMAT(6X,'PARAMETER (NVARS=',I2,')',' !NUMBER OF VARIABLES')
      IF(RECDIM.NE.-1) WRITE(10,554) dimsiz(recdim)
  554 FORMAT(6X,'PARAMETER (NREC=',I6,')   !CHANGE THIS TO GENERALIZE')
      WRITE(10,575) NVARS
  575 FORMAT('C     VARIABLE IDS RUN SEQUENTIALLY FROM 1 TO NVARS=',I3)
      WRITE(10,576)
  576 FORMAT(6X,'INTEGER*4 RCODE')
'
'      WRITE(6,576)
' 576 FORMAT(6X,'CHARACTER*40 INPUT_FILE')
      IF(RECDIM.NE.-1) WRITE(10,577)
  577 FORMAT(6X,'INTEGER*4 RECDIM')
      write(10,578)
  578 format(6x,'CHARACTER*50 long_name(nvars)')
      write(10,584)
  584 format(6x,'CHARACTER*50 name(100)')

      write(10,579)
'
  579 format('C     ****VARIABLES FOR THIS NETCDF FILE****',
     1/,'C')
      DO 600 I=1,NVARS
      varid = i
'
      IF(NVDIMS(VARID).EQ.0) WRITE(10,582) AVT(VARID),VARNAM(VARID)
  582 format(6x,a,1x,a)
'
      IF(NVDIMS(VARID).EQ.1) THEN ! 1single dimension variable
      IF(RECDIM.NE.-1) THEN
                       IF(VVDIMS(VARID,1).EQ.RECDIM)
     1                 WRITE(10,580) AVT(VARID),VARNAM(VARID)
  580                  FORMAT(6x,A,1X,A,'(NREC)')
                       IF(VVDIMS(VARID,1).NE.RECDIM)
     1                 WRITE(10,583) AVT(VARID),VARNAM(VARID),
1                      DIMSIZ (VVDIMS(varid, 1))
  583                  FORMAT(6X,A,1X,A,'(',I5,')')
                       End If
      IF(RECDIM.EQ.-1) WRITE(10,581) AVT(VARID),VARNAM(VARID),
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID))
  581 FORMAT(6X,A,1X,A,'(',I5,')')
                             End If
      IF(NVDIMS(VARID).EQ.2) THEN ! double dimension variable
      IF(RECDIM.NE.-1) WRITE(10,585) AVT(VARID),VARNAM(VARID),
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID)-1)
  585 FORMAT(6X,A,1X,A,'(',I5,',NREC)')
      IF(RECDIM.EQ.-1) WRITE(10,586) AVT(VARID),VARNAM(VARID),
'     +(DIMSIZ(VVDIMS(VARID,J)),J=NVDIMS(VARID),1,-1)
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID))

  586 FORMAT(6X,A,1X,A,'(',I5,',',I5,')')
                             End If
      IF(NVDIMS(VARID).EQ.3) THEN ! triple dimension variable
      IF(RECDIM.NE.-1) WRITE(10,590) AVT(VARID),VARNAM(VARID),
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID)-1)
'     +(DIMSIZ(VVDIMS(VARID,J)),J=NVDIMS(VARID)-1,1,-1)
  590 FORMAT(6X,A,1X,A,'(',I5,',',I5,',NREC)')
      IF(RECDIM.EQ.-1) WRITE(10,591) AVT(VARID),VARNAM(VARID),
'     +(DIMSIZ(VVDIMS(VARID,J)),J=NVDIMS(VARID),1,-1)
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID))
  591 FORMAT(6X,A,1X,A,'(',I5,',',I5,',',I5,')')
                             End If
      IF(NVDIMS(VARID).EQ.4) THEN !variable with 4 dimensions (rare)
      IF(RECDIM.NE.-1) WRITE(10,595) AVT(VARID),VARNAM(VARID),
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID)-1)
'     +(DIMSIZ(VVDIMS(VARID,J)),J=NVDIMS(VARID)-1,1,-1)
  595 FORMAT(6X,A,1X,A,'(',I5,',',I5,',',I5,',NREC)')
      IF(RECDIM.EQ.-1) WRITE(10,596) AVT(VARID),VARNAM(VARID),
'     +(DIMSIZ(VVDIMS(VARID,J)),J=NVDIMS(VARID),1,-1)
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID))
  596 FORMAT(6X,A,1X,A,'(',I5,',',I5,',',I5,',',I5,')')
                             End If
      IF(NVDIMS(VARID).EQ.5) THEN
      IF(RECDIM.NE.-1) WRITE(10,597) AVT(VARID),VARNAM(VARID),
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID)-1)
 597  FORMAT(6X,A,1X,A,'(',I5,',',I5,',',I5,',',I5,',NREC)')
 598  FORMAT(6X,A,1X,A,'(',I5,',',I5,',',I5,',',I5,',',I5,')')
      IF(RECDIM.EQ.-1) WRITE(10,598) AVT(VARID),VARNAM(VARID),
     +(DIMSIZ(VVDIMS(VARID,J)),J=1,NVDIMS(VARID))
                              End If
'
600   CONTINUE
      write(10,602)
  602 format('C*************************************')
      write(10,603)
 603  format(6x,'character*80 input_file')
      write(10,605)
  605 format(6x,'INTEGER*4 START(10)',/,6X,'INTEGER*4 COUNT(10)')
      WRITE(10,620)
  620 FORMAT(6X,'INTEGER VDIMS(10) !ALLOW UP TO 10 DIMENSIONS')
      WRITE(10,621)
  621 FORMAT(6X,'CHARACTER*31 DUMMY')

'     WRITE OUT THE STATEMENTS TO DECLARE START AND COUNT FOR EACH VARIABLE
'      DO 624 I=1,NVARS
'      WRITE(10,622) I,NVDIMS(I)
'  622 FORMAT(6X,'INTEGER*4 START',I2.2,'(',I1,')')
'      WRITE(10,623) I,NVDIMS(I)
'  623 FORMAT(6X,'INTEGER*4 COUNT',I2.2,'(',I1,')')
'
'  624 CONTINUE
'
'     generate data statements with variable ids and types and names
'
'     WRITE(10,750)
' 750 FORMAT(6X,'DATA VARS/')
'      WRITE(10,775) (VARNAM(I),I=1,NVARS) !check this
' 775 FORMAT(<NVARS-1>(5X,'+',1H',A31,2H',/),5X,'+',1H',A31,2H'/)
'      WRITE(10,778)
'  778 FORMATM(6X,'DATA VARTYP/')
'      WRITE(10,779) (VARTYP(I),I=1,NVARS)
'  779 FORMAT(5X,'+',<NVARS-1>(I1,','),I1,'/')
'      WRITE(10,780)
'  780 FORMAT(6X,'DATA NVDIMS/')
'      WRITE(10,790) (NVDIMS(I),I=1,NVARS)
'  790 FORMAT(5X,'+',<NVARS-1>(I1,','),I1,'/')
'      WRITE(10,805)
'  805 FORMAT(6X,'DATA DIMSIZ/')
'      WRITE(10,810) (DIMSIZ(I),I=1,NDIMS)
'  810 FORMAT(5X,'+',<NDIMS-1>(I5,','),I5,'/')
'      GENERATE START AND COUNT DATA ARRAYS
'
'     generate statements to CREATE DATA STATEMENTS FOR START AND COUNT
'
'      DO 875 I=1,NVARS
'      IF(NVDIMS(I).EQ.1) THEN
'      WRITE(10,829) I,START(I,1)
'  829 FORMAT(6X,'DATA START',I2.2,'/',I1,'/')
'      WRITE(10,830) I,COUNT(I,1)
'  830 FORMAT(6X,'DATA COUNT',I2.2,'/',I5,'/')
'                           ELSE
'      WRITE(10,831) I,(START(I,J),J=1,NVDIMS(I))
'  831 FORMAT(6X,'DATA START',I2.2,'/',<NVDIMS(I)-1>(I1,','),
'     +I1,'/')
'      WRITE(10,835) I,(COUNT(I,J),J=1,NVDIMS(I))
'  835 FORMAT(6X,'DATA COUNT',I2.2,'/',<NVDIMS(I)-1>(I5,','),
'     +I5,'/')
'                           ENDIF
'
'  875 CONTINUE
'
'     write an array of long_names
'
      write(10,889)
 889  format('C',/,'C      LONG NAMES FOR EACH VARIABLE',/,'C')
      write(10,890)
 890  format(6x,'data long_name/')
      do 895 i=1,nvars-1
      write(10,894) long_name(i)
 894  format(5x,'*',1h',a50,1h',',')
895   CONTINUE
      write(10,896) long_name(nvars)
 896  format(5x,'*',1h',a50,1h','/',/,'C')
'
'     write statement to open file
'      WRITE(10,1000) INPUT_FILE
' 1000 FORMAT(6X,'NCID=NCOPN(',1H',A40,1H',/,5x,'+',
'     +',NCNOWRIT,RCODE)')
'
      write(10,900)
 900  format(6x,'write(6,1)')
      write(10,910)
 910  format(' 1',4x,'format(',1h',' enter your input file',1h',')')
      write(10,920)
 920  format(6x,'read(5,2) input_file')
      write(10,930)
 930  format(' 2',4x,'format(a80)')
      write(10,940)
 940  format(6x,'ilen=index(input_file,',1h','   ',1h',')')
      write(10,950)
 950  format(6x,'ncid=ncopn(input_file(1:ilen-1),0,rcode)')
'
      
'     get info on the record dimension for this file
      IF(RECDIM.NE.-1) THEN
      WRITE(10,1001)
 1001 FORMAT(6X,'CALL NCINQ(NCID,NDIMS,NVARS,NGATTS,RECDIM,RCODE)')
      WRITE(10,1002)
 1002 FORMAT(6X,'CALL NCDINQ(NCID,RECDIM,DUMMY,NRECS,RCODE)')
      WRITE(10,1003)
 1003 FORMAT('C     !NRECS! NOW CONTAINS NUM RECORDS FOR THIS FILE')
                       End If
'
'*****************************************************
'
'     GET INFO ON THE DIMENSIONS
'
'     recdim will contain the id of the record dimension
'     NOW READY TO GENERATE CALL STATEMENTRS TO FILL VARIABLES WITH
' Values
'
'     in order to make the generated program usable, we need info
'     on the dimensions of the variables. If we do this in the pgm,
'     the only variable not with a constant dimension is the record
'     variable.
'
      DO 1500 I=1,NVARS
      write(10,1010) varnam(i)

 1010 format('C',/,'C    statements to fill ',a31,/,'C')
      LENSTR = 1
      K = 0
'     generate code to get the variable id from its name to be safe
'
      write(10,1012) varnam(i)
 1012 format(6x,'ivarid = ncvid(ncid,',1H',a31,1H',',rcode)')
      if(rcode.ne.0) then
                     write(6,1013) varnam(i)
 1013 format(' something has changed in this data: rerun gennet')
                     Stop
                     End If
'
      WRITE(10,1015)
 1015 FORMAT(6X,'CALL NCVINQ(NCID,ivarid,DUMMY,NTP,NVDIM,',
1     'VDIMS,NVS,RCODE)')
'     here we get number of sdims and their ids nvdim and vdims
      WRITE(10,1018)
 1018 FORMAT(6X,'LENSTR=1')
      II = i * 10
      WRITE(10,1020) II
 1020 FORMAT(6X,'DO ',I3,' J=1,NVDIM')
      WRITE(10,1025)
'     here we get the size of each nvdim dimension in ndsize
 1025 FORMAT(6X,'CALL NCDINQ(NCID,VDIMS(J),DUMMY,NDSIZE,RCODE)')
      WRITE(10,1030)
 1030 FORMAT(6X,'LENSTR=LENSTR*NDSIZE')
      WRITE(10,1035)
 1035 FORMAT(6X,'START(J)=1',/,6X,'COUNT(J)=NDSIZE')
      WRITE(10,1040) II
 1040 FORMAT(1X,I3,'  CONTINUE')
'      DO 1100 J=1,NVDIMS(I)
'      LENSTR= LENSTR*COUNT(I,J) !NEEDED FOR CHARACTER VARIABLES
' Start(i, J) = 1
' Index = VVDIMS(i, J)
' Count(i, J) = DIMSIZ(Index)
'      WRITE(10,1025) J,START(I,J),J,COUNT(I,J)
' 1025 FORMAT(6X,'START(',I1,')=',I5,/,6X,'COUNT(',I1,')=',I5)
' 1100 CONTINUE
      IF(VARTYP(I).EQ.2) THEN !CHARACTER VAIABLES
      WRITE(10,1250) VARNAM(I)
 1250 FORMAT(6X,'CALL NCVGTC(NCID,ivarid,START,COUNT',
1     ',',/,5x,'+',A31,',LENSTR,RCODE)')
                                           Else
      WRITE(10,1350) VARNAM(I)
 1350 FORMAT(6X,'CALL NCVGT(NCID,ivarid,START,COUNT',
1     ',',/,5x,'+',A31,',RCODE)')
                                           End If
1500  CONTINUE
'
'     write code to check the nlong_names against those in the data array
'     and to check to see if number of variables has changed
'
      write(10,1501)
 1501 format(/,'C',/,'C',5x,'following code: checks output code code',
     *' against current input file',/,'C')
'
      write(10,1502)
 1502 format('C',/,6x,
     *'call ncinq(ncid,ndims,nvarsc,ngatts,nrecdim,rcode)')
      n1 = (i + 1) * 10
      write(10,1503) n1
 1503 format(6x,'if(nvarsc.ne.nvars) write(6,',i3,')')
      write(10,1504) n1
 1504 format(2x,i3,1x,'format(',1h','number of variables has changed',
     *1h',')')
'
      n8 = (i + 8) * 10
      write(10,1505) n8
 1505 format('C',/,6x,'do ',i3,' i=1,nvars')
      n4 = (i + 4) * 10
      write(10,1508) n4
 1508 format(6x,'do ',i3,' j=1,nvarsc')
      write(10,1510)
 1510 format(6x,'call ncagtc(ncid,j,',1h','long_name',1h',
     *',name(j),50,rcode)')
      write(10,1511)
 1511 format(6x,'ilen=index(long_name(i),',1h','   ',1h',')')
      write(10,1515) n8
 1515 format(6x,'if(long_name(i)(1:ilen-1).eq.name(j)(1:ilen-1))'
     *' go to ',i3)
      write(10,1520) n4
 1520 format(2x,i3,1x,'continue')
      n5 = (i + 5) * 10
      write(10,1525) n5
 1525 format(6x,'write(6,',i3,')',' name(j)')
      write(10,1530) n5
 1530 format(2x,i3,1x,'format(',1h','unknown variable ',1h',',a50)')
      n6 = (i + 6) * 10
      write(10,1535) n6
 1535 format(6x,'write(6,',i3,')')
      write(10,1540) n6
 1540 format(2x,i3,1x,'format(',1h','rerun gennet',1h',')')
      write(10,1550)
 1550 format(6x,'stop')
      write(10,1555) n8
 1555 format(2x,i3,1x,'continue')
'
      
      write(10,1599)
 1599 format('C',/,6x,'CALL NCCLOS(NCID,RCODE)',/,'C')
'
      WRITE(10,1700)
      WRITE(10,1600)
 1600 FORMAT('C     HERE IS WHERE YOU WRITE STATEMENTS TO USE THE DATA')
      WRITE(10,1700)
 1700 FORMAT('C')
      WRITE(10,1700)
      WRITE(10,1700)
      WRITE(10,1800)
 1800 FORMAT(6X,'STOP')
      WRITE(10,1900)
 1900 FORMAT(6X,'END')
'
'
      WRITE(6,2000)
 2000 FORMAT(' ***GENERATED FORTRAN PGM CALLED readnet.for***')
'
      Stop


End Sub

Sub main()

End Sub

Private Sub cmdRead_Click()
Dim B As nCDF, lu%, i#, n%, j#

lu = FreeFile
Open "c:\MTP\RAOB\COSMIC\wetPrf_C001.2006.301.11.19.G15_0001.0001_nc" For Random Access Read Write As lu Len = Len(B)

Get #lu, 1, B

c$ = ""
For i = 1 To 5000
  c$ = c$ + Chr(B.net(i))
Next i
i = i

j = InStr(1, c$, "fileStamp")

For i = 1 To 5000
n = B.net(i)
Select Case n
Case 95, 46
    Debug.Print Format(Chr(n), "00");
Case Is > 96
  If n < 123 Then
    Debug.Print Format(Chr(n), "00");
  Else
    Debug.Print Format(Chr(B.net(i)), "00");
  End If
  
Case Is > 64
  If n < 91 Then
    Debug.Print Format(Chr(n), "00");
  Else
    Debug.Print Format(Chr(B.net(i)), "00");
  End If
  
Case Else
  Debug.Print Format(Chr(B.net(i)), "00");
End Select
Next i


End Sub


Private Sub cmdTest_Click(Index As Integer)
Dim Version1 As Long, Version2 As String, Path$, i As Long, Filename As String
Dim status As Long, ncid As Long, ndims As Long, nvars As Long, ngatts As Long, unlimdimid As Long
Dim varname(0 To 10) As String, VarNameP(0 To 10) As Long, length As Long, dimipd As Long, Lengthp As Long
Dim varid As Long, xtype As Long, dimids As Long, natts As Long

  On Error Resume Next
  
  Select Case Index
  Case 0
    Version1 = nc_inq_libvers()
    'status = nc_inq_format(ncid, Version1)
    Version2 = StringFromPointer(Version1, 1024)
    Debug.Print Version2

  Case 1  'NC_OPEN and NC_INQ
'         nc_open                   /* open existing netCDF dataset */
'           ...
'         nc_inq                    /* find out what is in it */
'              ...
'            nc_inq_dim             /* get dimension names, lengths */
'              ...
'            nc_inq_var             /* get variable names, types, shapes */
'                 ...
'               nc_inq_attname      /* get attribute names */
'                 ...
'               nc_inq_att          /* get attribute types and lengths */
'                 ...
'               nc_get_att          /* get attribute values */
'                 ...
'            nc_get_var             /* get values of variables */
'              ...
'         nc_close                  /* close netCDF dataset */'


    Path$ = "c:\MTP\RAOB\COSMIC\wetPrf_C001.2006.301.11.19.G15_0001.0001_nc"
    status = nc_open(Path$, 0, ncid)
    status = nc_inq(ncid, ndims, nvars, ngatts, unlimdimid)
    Debug.Print ncid; ndims; nvars; ngatts; unlimdimid
    
    Call ReadAtt(ncid, varid, attnum)
    status = nc_inq_format(ncid, Version1)
    Version2 = StringFromPointer(Version1, 1024)
    Debug.Print Version2
    'status = nc_inq_dim(ncid, 1, VarName, lengthp)
    For i = 0 To nvars - 1
    status = NC_INQ_VAR(ncid, i, varname$(i), xtype, ndims, dimids, natts)
      'status = nc_inq_dim(ncid, i, VarName(i), Lengthp)
      'VarName(i) = StringFromPointer(VarNameP(i), nc_max_name)
    Next i
    'int nc_inq_dimlen  (int ncid, int dimid, size_t *lengthp);

    Debug.Print ncid; varname(0); Lengthp; dimidp
    
    Case 2  '.NET
      Filename$ = "c:\MTP\RAOB\COSMIC\wetPrf_C001.2006.301.11.19.G15_0001.0001_nc"
      Call ReadTestFile(Filename)
  
  End Select
  'ByVal ncid As Long, ByVal dimid As Long, byVal name As String, ByRef lenp As Long)
End Sub

Private Sub cmdTest1_Click()

End Sub


Private Sub Form_Load()
Call main
End Sub


