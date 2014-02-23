%-------------------------------------------------------------------------------
% Load PICARRO Private data (.h5) and aircraft .nc file and write them out as 
% .mat files for import into next step.
% Also do some simple plotting so we can determine flight takeoff and landing 
% times.
%-------------------------------------------------------------------------------

%-------------------------------------------------------------------------------
% When working in interactive mode, you want to clear everything before you 
% rerun. Makes debugging alot easier if declared vars aren't hanging around 
% with their previous values.
%-------------------------------------------------------------------------------
tic
clc
clear all
close all

%%------------------------------------------------------------------------------
% read flight-specific constants from input file
%-------------------------------------------------------------------------------
flightConstants

%%------------------------------------------------------------------------------
% Set some constants.
%
% This section assumes that the following structure is under the PICARRO dir:
%  RF##
%        <project>_RF##.nc	 % Aircraft .nc file for flight
%        Private
%                *.h5		 % PICARRO .h5 files for flight
%-------------------------------------------------------------------------------

doy_start=datenum(flightYear,flightMonth,flightDay);
slash=['/'];
rafPath = [picPath,'RF',flightNum];
ncFile = [slash, project,'_rf', flightNum, '.nc'];
dirname = [rafPath, slash, 'Private', slash];

cd(dirname);


%%------------------------------------------------------------------------------
% Read in PICARRO data
%-------------------------------------------------------------------------------
% UPGRADE: - NOT TESTED YET
% if usejava('Desktop')
% If starting in gui mode
% filename=uigetfile('*.h5','Select files','MultiSelect','on');
% file=[dirname,char(filename(1))];
% else
% if in -nodisplay or -nodesktop mode
filename=dir('*h5');
file=[dirname,char(filename(1).name)];
% end



data = h5read(file, [slash, 'results']);

all_fieldnames=fieldnames(data);
% 
for i=1:length(fieldnames(data))
fieldname=char(all_fieldnames(i));
DATA.(fieldname)=[];

end


for kk=1:length(filename)
  clear data

  % UPGRADE: - NOT TESTED YET
  % if usejava('Desktop')
  % If starting in gui mode
  % file=[dirname,char(filename(kk))];
  % else
  % if in -nodisplay or -nodesktop mode
    file=[dirname,char(filename(kk).name)];
  % end

  data = h5read(file, [slash, 'results']);

    for i=1:length(fieldnames(data))
       fieldname=char(all_fieldnames(i));
       data_i=data.(fieldname);
       DATA.(fieldname)=[DATA.(fieldname);data_i];
    end

end

%%------------------------------------------------------------------------------
% Write a .mat file with the PICARRO data in it to be used in the next step of 
% the processing
%-------------------------------------------------------------------------------
cd(rafPath);

PICARRO=DATA;
PICARRO.DOY=DATA.JULIAN_DAYS;
picOutfilename=['RF', flightNum, '_PICARRO_data.mat'];

save(picOutfilename,'PICARRO')

%%------------------------------------------------------------------------------
% Plot the PICARRO data so can determine start and end times of flight. 
% Also visually inspect to be sure all values look resonable:
%
% CO2_dry < 400 (Values 400-600 indicate a cabin air leak that needs to be
%                investigated before the next flight)
% CH4_dry ~= 1.8
% H2O < 0.1     (at altitude, Will be much higher on the ground and takes a
%                while to dry out after takeoff/dips - exponential decay. If stays
%                above 0.1 at altitude, need to dry out dessicant before next 
%                flight.)
% Confirm cals were run as expected.
%
% This is as far as you have to go to be confident PICARRO is working for next 
% flight.
% If the RAF PM hasn't processed the flight data yet, you won't be able to 
% go past this point - code will crash in next section.
%-------------------------------------------------------------------------------
fig=figure()
plot(PICARRO.JULIAN_DAYS,PICARRO.CO2_dry)
title('CO2 dry')
saveas(fig,'CO2_dry','jpg')
saveas(fig,'CO2_dry','fig')
toc
fig=figure()
plot(PICARRO.JULIAN_DAYS,PICARRO.CH4_dry)
title('CH4 dry')
saveas(fig,'CH4_dry','jpg')
saveas(fig,'CH4_dry','fig')
fig=figure()
plot(PICARRO.JULIAN_DAYS,PICARRO.H2O)
title('H2O')
saveas(fig,'H2O_dry','jpg')
saveas(fig,'H2O_dry','fig')

%%------------------------------------------------------------------------------
% Read in data from RAF netCDF file
%-------------------------------------------------------------------------------

iflt = 1;
 ncid = netcdf.open([rafPath,ncFile],'NC_NOWRITE');
% "header" info ?
 [ndims,nvars,ngatts,unlimdimid] = netcdf.inq(ncid);
 if nvars>0
  for ivar = 0:0  % look only at first variable to get time information for 
                  % start date and start time
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

% close netcdf file
  netcdf.close(ncid)
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


%%------------------------------------------------------------------------------
% Write a .mat file with the CO data in it to be used in the next step of 
% the processing
%-------------------------------------------------------------------------------
TIME=double(TIME);
DOY=doy_start+datenum(0,0,0,0,0,TIME)-datenum(flightYear-1,12,31);

filename=['RF', flightNum, '_CO_data.mat'];

AC.CORAW_AL=CORAW_AL;
AC.GGLAT=GGLON;
AC.PALTF=PALTF;
AC.PSXC=PSXC;
AC.TIME=TIME;
AC.DOY=DOY;
AC.JD=DOY+offset;

AC.INLETP_AL=INLETP_AL;
AC.FO3 = FO3;
AC.THETA = THETA;
AC.PLWCC=PLWCC;
AC.PLWCD_RWOO=PLWCD_RWOO;

save(filename,'AC')

%%------------------------------------------------------------------------------
% Plot the aircraft altitude from the .nf file  so can determine start and end 
% times of flight. 
%-------------------------------------------------------------------------------
fig=figure()
plot(DOY,PALTF)
title('PALTF')
saveas(fig,'PALTF','jpg')
saveas(fig,'PALTF','fig')
toc

%-------------------------------------------------------------------------------
% Now combine picarro and co data onto common time base
% by running 'Contrast_combine_CO_PICARRO_DATA'
%-------------------------------------------------------------------------------
