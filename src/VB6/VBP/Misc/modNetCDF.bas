Attribute VB_Name = "modNetCDF"
Public c$
Public net(1 To 16384) As Byte

'.h                         .bas                                   Comment
'----------------------------------------------------------------------------------------
'int mode                   ByVal mode As Long
'int *ncidp                 ByRef ncidp As Long
'size_t var                 ByVal var As Long
'size_t *var                ByRef var As Long
'const size_t *var          ByRef var As Long
'nc_type xtype              ByVal xtype As Long
'nc_type *xtype             ByRef xtype As Long
'const char *path           ByVal path As String
'char *name                 ByVal name As String    Pre-allocate string as string(NC_MAX_NAME, chr(0))
'short op                   ByVal op as Integer
'short *op                  ByRef op As Integer
'float op                   ByVal op As Single
'float *op                  ByRef op As Single
'double op                  ByVal op As Double
'double *op                 ByRef op As Double
'void op                    ByVal op As Any
'void *op                   ByRer op As Any
'char op                    ByVal op As Integer     Chars are passed as integers unless you know they are UNICODE
'char *op                   ByRef op as Integer
        
        ' The netcdf external data types
    Enum nc_type
        NC_BYTE = 1 '/* signed 1 byte integer */
        NC_CHAR = 2 '/* ISO/ASCII character */
        NC_SHORT = 3 '/* signed 2 byte integer */
        NC_INT = 4 '/* signed 4 byte integer */
        NC_FLOAT = 5 '/* single precision floating point number */
        NC_DOUBLE = 6 '/* double precision floating point number */
    End Enum

    Enum cmode
        NC_NOWRITE = 0
        NC_WRITE = &H1      '/* read & write */
        NC_CLOBBER = 0
        NC_NOCLOBBER = &H4  '/* Don't destroy existing file on create */
        NC_FILL = 0         '/* argument to ncsetfill to clear NC_NOFILL */
        NC_NOFILL = &H100   '/* Don't fill data section an records */
        NC_LOCK = &H400     '/* Use locking if available */
        NC_SHARE = &H800   '/* Share updates, limit cacheing */
    End Enum

    '/*
    ' *   Default fill values, used unless _FillValue attribute is set.
    ' * These values are stuffed into newly allocated space as appropriate.
    ' * The hope is that one might use these to notice that a particular datum
    ' * has not been set.
    ' */
    Public Const NC_FILL_BYTE As Byte = 255
    Public Const NC_FILL_CHAR As Byte = 0
    Public Const NC_FILL_SHORT As Integer = -32767
    Public Const NC_FILL_INT As Long = -2147483647
    Public Const NC_FILL_FLOAT As Single = 9.96921E+36     '/* near 15 * 2^119 */
    Public Const NC_FILL_DOUBLE As Double = 9.96920996838687E+36

    '* 'size' argument to ncdimdef for an unlimited dimension
    Public Const NC_UNLIMITED As Long = 0

    '* attribute id to put/get a global attribute
    Public Const NC_GLOBAL As Long = -1

    '* These maximums are enforced by the interface, to facilitate writing
    '* applications and utilities.  However, nothing is statically allocated to
    '* these sizes internally.
    Enum netCDF_limits
        NC_MAX_DIMS = 10  '/* max dimensions per file */
        NC_MAX_ATTRS = 2000 '/* max global or per variable attributes */
        NC_MAX_VARS = 2000 '/* max variables per file */
        nc_max_name = 128 '/* max length of a name */
        NC_MAX_VAR_DIMS = 10 '/* max per variable dimensions */
    End Enum


' The returned values are pointers to strings, so you need two API functions to get the string from the pointer,

Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal length As Long)

Public Declare Function IsBadStringPtr Lib "kernel32" Alias "IsBadStringPtrA" (ByVal lpsz As Long, ByVal ucchMax As Long) As Long

' Remember to handle strings ByVal and not ByRef

Private Declare Function nc_put_var_float Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, ByRef varvalue As Single)
'Note:
'For a netcdf variable without dimension write

'Example:
'Dim MyValue As Single
'Call nc_put_var_float(ByVal File_ID, ByVal Variable_ID, MyValue)

'For an netcdf variable with at least one dimensions and, let's say, 500 elements write

'Example:
'Dim MyValue(1 To 500) As Single
'Call nc_put_var_float(ByVal File_ID, ByValVariable_ID, MyValue(1))

'Just handle the first element of the VB6 array. This example set all 500 values into the netcdf variable

    ' LIBRARY-WIDE AND FILE OPERATIONS
    '
    ' const char *nc_inq_libvers(void);
    Declare Function nc_inq_libvers Lib "netcdf.dll" () As String
    Declare Function nc_inq_format Lib "netcdf.dll" (ByVal ncid As Long, ByVal pointer As Long) As Integer
    ' const char *nc_strerror(int ncerr1);
    Declare Function nc_strerror Lib "netcdf.dll" (ByVal ncerr1 As Long) As String
    ' int nc_create(const char *path, int cmode, int *ncidp);
    Declare Function nc_create Lib "netcdf.dll" (ByVal Path As String, ByVal cmode As Long, _
        ByRef ncidp As Long) As Long
    ' int nc_open(const char *path, int mode, int *ncidp);
    Declare Function nc_open Lib "netcdf.dll" (ByVal Path As String, ByVal cmode As Long, _
        ByRef ncidp As Long) As Long
    ' int nc_set_fill(int ncid, int fillmode, int *old_modep);
    Declare Function nc_set_fill Lib "netcdf.dll" (ByVal ncid As Long, ByVal fillmode As Long, _
        ByRef old_modep As Long) As Long
    ' int nc_redef(int ncid);
    Declare Function nc_redef Lib "netcdf.dll" (ByVal ncid As Long) As Long
    ' int nc_enddef(int ncid);
    Declare Function nc_enddef Lib "netcdf.dll" (ByVal ncid As Long) As Long
    ' int nc_sync(int ncid);
    Declare Function nc_sync Lib "netcdf.dll" (ByVal ncid As Long) As Long
    ' int nc_abort(int ncid);
    Declare Function nc_abort Lib "netcdf.dll" (ByVal ncid As Long) As Long
    ' int nc_close(int ncid);
    Declare Function nc_close Lib "netcdf.dll" (ByVal ncid As Long) As Long

    ' INQ FUNCTIONS
    '
    ' int nc_inq(int ncid, int *ndimsp, int *nvarsp, int *nattsp, int *unlimdimidp);
    Declare Function nc_inq Lib "netcdf.dll" (ByVal ncid As Long, ByRef ndimsp As Long, _
        ByRef nvarsp As Long, ByRef nattsp As Long, ByRef unlimdimidp As Long) As Long
    ' int  nc_inq_ndims(int ncid, int *ndimsp);
    Declare Function nc_inq_ndims Lib "netcdf.dll" (ByVal ncid As Long, ByRef ndimsp As Long) As Long
    ' int  nc_inq_nvars(int ncid, int *nvarsp);
    Declare Function nc_inq_nvars Lib "netcdf.dll" (ByVal ncid As Long, ByRef nvarsp As Long) As Long
    ' int  nc_inq_natts(int ncid, int *nattsp);
    Declare Function nc_inq_natts Lib "netcdf.dll" (ByVal ncid As Long, ByRef nattsp As Long) As Long
    ' int  nc_inq_unlimdim(int ncid, int *unlimdimidp);
    Declare Function nc_inq_unlimdim Lib "netcdf.dll" (ByVal ncid As Long, ByRef unlimdimidp As Long) As Long
    ' int nc_def_dim(int ncid, const char *name, size_t len, int *idp);
    Declare Function nc_def_dim Lib "netcdf.dll" (ByVal ncid As Long, ByVal name As String, ByVal leng As Long, ByRef idp As Long) As Long
    ' int nc_inq_dimid(int ncid, const char *name, int *idp);
    Declare Function nc_inq_dimid Lib "netcdf.dll" (ByVal ncid As Long, ByVal name As String, _
        ByRef idp As Long) As Long
    ' int nc_inq_dim(int ncid, int dimid, char *name, size_t *lenp);
    Declare Function nc_inq_dim Lib "netcdf.dll" (ByVal ncid As Long, ByVal dimid As Long, _
        ByVal name As String, ByRef lenp As Long) As Long
    ' int  nc_inq_dimname(int ncid, int dimid, char *name);
    Declare Function nc_inq_dimname Lib "netcdf.dll" (ByVal ncid As Long, ByVal dimid As Long, _
        ByVal name As String) As Long
    ' int  nc_inq_dimlen(int ncid, int dimid, size_t *lenp);
    Declare Function nc_inq_dimlen Lib "netcdf.dll" (ByVal ncid As Long, ByVal dimid As Long, _
        ByRef lenp As Long) As Long
    ' int nc_rename_dim(int ncid, int dimid, const char *name);
    Declare Function nc_rename_dim Lib "netcdf.dll" (ByVal ncid As Long, ByVal dimid As Long, _
        ByVal name As String) As Long
    ' int nc_inq_att(int ncid, int varid, const char *name, nc_type *xtypep, size_t *lenp);
    Declare Function nc_inq_att Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
       ByVal name As String, ByRef xtypep As NetCDF.nc_type, ByRef lenp As Long) As Long
    ' int  nc_inq_attid(int ncid, int varid, const char *name, int *idp);
    Declare Function nc_inq_attid Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
       ByVal name As String, ByRef xtypep As NetCDF.nc_type, ByRef lenp As Long) As Long
    ' int  nc_inq_atttype(int ncid, int varid, const char *name, nc_type *xtypep);
    Declare Function nc_inq_atttype Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
       ByVal name As String, ByRef xtypep As NetCDF.nc_type, ByRef lenp As Long) As Long
    ' int  nc_inq_attlen(int ncid, int varid, const char *name, size_t *lenp);
    Declare Function nc_inq_attlen Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
       ByVal name As String, ByRef lenp As Long) As Long
    ' int nc_inq_attname(int ncid, int varid, int attnum, char *name);
    Declare Function nc_inq_attname Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
       ByVal attnum As Long, ByVal attname As String) As Long

    ' ATTRIBUTE READING AND WRITING

    ' int nc_copy_att(int ncid_in, int varid_in, const char *name, int ncid_out, int varid_out);
    Declare Function nc_copy_att Lib "netcdf.dll" (ByVal ncid_in As Long, ByVal varid_in As Long, _
       ByVal name As String, ByVal ncid_out As Long, ByVal varid_out As Long) As Long
    ' int nc_rename_att(int ncid, int varid, const char *name, const char *newname);
    Declare Function nc_rename_att Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
       ByVal name As String, ByRef newname As String) As Long
    ' int nc_del_att(int ncid, int varid, const char *name);
    Declare Function nc_del_att Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
       ByVal name As String) As Long
    ' int nc_put_att_text(int ncid, int varid, const char *name,
    ' size_t len, const char *op);
    Declare Function nc_put_att_text Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
      ByVal name As String, ByVal leng As Long, ByVal op As String) As Long
    ' int nc_get_att_text(int ncid, int varid, const char *name, char *ip);
    Declare Function nc_get_att_text Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, _
      ByVal name As String, ByVal op As String) As Long
    
    Declare Function NC_INQ_VAR Lib "netcdf.dll" (ByVal ncid As Long, ByVal varid As Long, ByVal name$, ByVal xtype As Long, ByVal ndims As Long, ByVal dimids As Long, ByVal natts As Long)

'Passing strings: when passing in a string to a function, use a string,
'when passing in a pointer to a string so that the function can fill it
'(for example when requesting an attribute name, use a
'System.Text.StringBuilder.
'
'Since VB doesn't have an unsigned byte, I've left those functions
'out of the wrapper class. If you need to read unsigned bytes, read them as
'shorts, and netcdf will automatically convert them for you.
'
Type nCDF
  net(1 To 16384) As Byte
End Type

Public Function LongToUnsigned(ByVal Value As Long) As Double
    If Value < 0 Then
        LongToUnsigned = Value + 4294967296#
    Else
        LongToUnsigned = Value
    End If
End Function

Public Function StringFromPointer(StringPointer As Long, MaxLength As Long) As String
Dim sRet As String
Dim lret As Long

  If StringPointer = 0 Then
    StringFromPointer = ""
    Exit Function
  End If
  
  If IsBadStringPtr(StringPointer, MaxLength) Then
    StringFromPointer = ""
    Exit Function
  End If

  sRet = Space$(MaxLength)
  CopyMemory ByVal sRet, ByVal StringPointer, ByVal Len(sRet)
  If err.LastDllError = 0 Then
    If InStr(sRet, Chr$(0)) > 0 Then sRet = Left$(sRet, InStr(sRet, Chr$(0)) - 1)
  Else
    sRet = ""
  End If
  StringFromPointer = sRet

End Function
