clc
clear all
close all
tic
% %%

%% insert RF number and flight day
%%info: RF4 (2014,01,18); RF5 (2014,01,21);
result=['09'];
doy_start=datenum(2014,02,05);

cd 'C:\Users\kaser\Documents\CONTRAST\infield\GVnc'

%% load cabin data
[FileName,PathName] = uigetfile('*.nc','Select cabin file');




%
% GET RAF VARIABLES FROM NETCDF FILE FOR FLIGHT using MatLab 2010a commands
%   these commands should be compatible with MatLab 2011a
% RAF variables are preliminary and taken from the ads system in flight so 
%   they include cals, blanks and other "bad" data.
%
%  open a file
   % pick a flight file

iflt = 1;
      ncid = netcdf.open([PathName,FileName],'NC_NOWRITE');
% "header" info ?
 [ndims,nvars,ngatts,unlimdimid] = netcdf.inq(ncid);
 if nvars>0
  for ivar = 0:0  % look only at first variable to get time information for start date and start time
  ivar;
  [varname, xtype, dimids, numatts] = netcdf.inqVar(ncid,ivar);
  varid = netcdf.inqVarID(ncid,varname);
   for iatts = 0:numatts-1
    iatts;
    attname = netcdf.inqAttName(ncid,varid,iatts);
    attrvalue = netcdf.getAtt(ncid,varid,attname);
   end
  eval([varname ' = netcdf.getVar(ncid,varid);']);
  end
 end
% 
% 
      ncid = netcdf.open([PathName,FileName],'NC_NOWRITE');

     varid = netcdf.inqVarID(ncid,'Time'); % time
     TIME = netcdf.getVar(ncid,varid); 
      
     varid = netcdf.inqVarID(ncid,'GGLAT'); % altitude in feet
     GGLAT = netcdf.getVar(ncid,varid);%
%       
     varid = netcdf.inqVarID(ncid,'GGLON'); % altitude in feet
     GGLON = netcdf.getVar(ncid,varid);%
     
     
     varid = netcdf.inqVarID(ncid,'CORAW_AL'); % ground speed
     CORAW_AL = netcdf.getVar(ncid,varid);%
     
     varid = netcdf.inqVarID(ncid,'PALTF'); % true air speed
     PALTF = netcdf.getVar(ncid,varid);%
     
      varid = netcdf.inqVarID(ncid,'INLETP_AL'); % true air speed
     INLETP_AL = netcdf.getVar(ncid,varid);%
  
     
      % close netcdf file
      netcdf.close(ncid)
      %


TIME=double(TIME);


DOY=doy_start+datenum(0,0,0,0,0,TIME)-datenum('12-31-2013');




filename=['RF', result, '_CO_data.mat'];

GV.CORAW_AL=CORAW_AL;
GV.GGLAT=GGLAT;
GV.GGLON=GGLON;
GV.PALTF=PALTF;
GV.TIME=TIME;
GV.DOY=DOY;
GV.JD=DOY-1;%% needs to be +1 sometimes if start after UTC 00 (e.g. for RF05)
GV.INLETP_AL=INLETP_AL;



% cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
cd C:\Users\kaser\Documents\CONTRAST\matlab\RF09\resultsRF09
save(filename,'GV')


figure()
plot(DOY,CORAW_AL)
toc
