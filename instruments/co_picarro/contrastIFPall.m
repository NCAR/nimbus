%load PICARRO data
tic
clc
clear all
close all

%% insert RF number and flight day
%%info: RF4 (2014,01,19); RF5 (2014,01,21); RF08 (2014,02,01);
result=['09'];
doy_start=datenum(2014,02,04);


% cd C:\Users\kaser\Documents\CONTRAST\infield\Picarro\RF08\Private
% 
% dirname='C:\Users\kaser\Documents\CONTRAST\infield\Picarro\RF08\Private\';
picPath='/Users/campos/Documents/macBak/campos/chemData/2014contrast/picarro/';
rafPath='/Users/campos/Documents/macBak/campos/chemData/2014contrast/ifpRAF/';
ncFile = ['CONTRAST_rf', result, '.nc'];

dirname = [picPath, 'Research Flights/RF', result, '/Private/'];
cd(dirname);
% dirname='C:\Users\kaser\Documents\CONTRAST\infield\Picarro\RF08\Private\';

filename=uigetfile('*.h5','Select files','MultiSelect','on');

file=[dirname,char(filename(1))];
data = h5read(file,'/results');

all_fieldnames=fieldnames(data);
% 
for i=1:length(fieldnames(data))
fieldname=char(all_fieldnames(i));
DATA.(fieldname)=[];

end


for kk=1:length(filename)
clear data

file=[dirname,char(filename(kk))];


data = h5read(file,'/results');

% 
    for i=1:length(fieldnames(data))
    fieldname=char(all_fieldnames(i));
    data_i=data.(fieldname);
    DATA.(fieldname)=[DATA.(fieldname);data_i];
    
    end


end

cd(rafPath);

PICARRO=DATA;
PICARRO.DOY=DATA.JULIAN_DAYS-1;
picOutfilename=['RF', result, '_PICARRO_data.mat'];

% cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08
save(picOutfilename,'PICARRO')

figure()
plot(PICARRO.JULIAN_DAYS,PICARRO.CO2_dry)
toc

% Read in data from RAF netCDF file

iflt = 1;
      ncid = netcdf.open([rafPath,ncFile],'NC_NOWRITE');
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
      ncid = netcdf.open([rafPath,ncFile],'NC_NOWRITE');

     varid = netcdf.inqVarID(ncid,'Time'); % time
     TIME = netcdf.getVar(ncid,varid); 
      
     varid = netcdf.inqVarID(ncid,'GGLAT'); % latitude 
     GGLAT = netcdf.getVar(ncid,varid);%
%       
     varid = netcdf.inqVarID(ncid,'GGLON'); % longitude
     GGLON = netcdf.getVar(ncid,varid);%
     
     
     varid = netcdf.inqVarID(ncid,'CORAW_AL'); % prelim CO MR
     CORAW_AL = netcdf.getVar(ncid,varid);%
     
     varid = netcdf.inqVarID(ncid,'PALTF'); % altitude in feet
     PALTF = netcdf.getVar(ncid,varid);%
     
     varid = netcdf.inqVarID(ncid,'PSXC'); % static pressure
     PSXC = netcdf.getVar(ncid,varid);%
     
      varid = netcdf.inqVarID(ncid,'INLETP_AL'); % inlet pressure
     INLETP_AL = netcdf.getVar(ncid,varid);%
  
     
     varid = netcdf.inqVarID(ncid,'FO3_ACD'); % time
     FO3 = netcdf.getVar(ncid,varid); 
      
     varid = netcdf.inqVarID(ncid,'THETA'); % time
     THETA = netcdf.getVar(ncid,varid); 
      
     varid = netcdf.inqVarID(ncid,'PLWCC'); % static pressure
     PLWCC = netcdf.getVar(ncid,varid);%
     
     varid = netcdf.inqVarID(ncid,'PLWCD_RWOO'); % static pressure
     PLWCD_RWOO = netcdf.getVar(ncid,varid);%
     
      % close netcdf file
      netcdf.close(ncid)
      %


TIME=double(TIME);


DOY=doy_start+datenum(0,0,0,0,0,TIME)-datenum('12-31-2013');




filename=['RF', result, '_CO_data.mat'];

GV.CORAW_AL=CORAW_AL;
GV.GGLAT=GGLON;
GV.PALTF=PALTF;
GV.PSXC=PSXC;
GV.TIME=TIME;
GV.DOY=DOY;
GV.JD=DOY;%% needs to be +1 sometimes if start after UTC 00 (e.g. for RF05)
GV.INLETP_AL=INLETP_AL;
GV.FO3 = FO3;
GV.THETA = THETA;
GV.PLWCC=PLWCC;
GV.PLWCD_RWOO=PLWCD_RWOO;

% cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08
save(filename,'GV')


% Now combine picarro and co data onto common time base

% Contrast_combine_CO_PICARRO_DATA