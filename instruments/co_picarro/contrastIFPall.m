%load PICARRO data
tic
clc
clear all
close all

%% insert RF number and flight day
%%info: RF4 (2014,01,19); RF5 (2014,01,21); RF08 (2014,02,01);
% result=['11'];
% doy_start=datenum(2014,02,12);
result=['12'];
doy_start=datenum(2014,02,17);

% cd C:\Users\kaser\Documents\CONTRAST\infield\Picarro\RF08\Private
% 
% dirname='C:\Users\kaser\Documents\CONTRAST\infield\Picarro\RF08\Private\';
picPath='/net/work/Projects/CONTRAST/PICARRO/matlab/';
rafPath = [picPath,'RF',result];
ncFile = ['/CONTRAST_rf', result, '.nc'];

dirname = [picPath, 'RF', result, '/raw_data/Private/'];
cd(dirname);
% dirname='C:\Users\kaser\Documents\CONTRAST\infield\Picarro\RF08\Private\';

% Update this to say
% If starting in gui mode
% filename=uigetfile('*.h5','Select files','MultiSelect','on');
% else if in -nodisplay mode
filename=dir('*h5');

file=[dirname,char(filename(1).name)];
data = h5read(file,'/results');

all_fieldnames=fieldnames(data);
% 
for i=1:length(fieldnames(data))
fieldname=char(all_fieldnames(i));
DATA.(fieldname)=[];

end


for kk=1:length(filename)
clear data

file=[dirname,char(filename(kk).name)];


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

fig=figure()
plot(PICARRO.JULIAN_DAYS,PICARRO.CO2_dry)
saveas(fig,'CO2_dry','jpg')
saveas(fig,'CO2_dry','fig')
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
GV.JD=DOY;%% needs to be +1 if start after UTC 00 (e.g. for RF05), zero otherwise

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
