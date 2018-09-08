%Writes variables created by read_applanix_vnv.m to netcdf files
[A] = read_applanix_vnav2('vnav_Missionrf15.out','RF15_ASCII.txt');
%read in data from matrix created by applanix_frequency convert
%[outmatrix]=applanix_frequncyconvert(infile)dlmread or fread ascii file, then run frequency convert after making it
%onehertz=applanix_frequncyconvert('RF07_ASCII.txt');
%work as a function
%A=onehertztest;
[a,b]=size(A);
stime=7688;%15030; %UTC start time of netcdf file
i1=A(1,1)-stime; %index of applanix start time
i2=i1+a-1; %index of applanix end time

ncid = netcdf.open('/h/eol/schick/RStudio/DEEPWAVErf15_applanix_sr.nc','NC_WRITE')
%extract time dimension from netcdf file
time_dim_ID=netcdf.inqDimID(ncid,'Time');
[dummy, time_dim_len] = netcdf.inqDim(ncid, [10,time_dim_ID])
%Define new variables (lat, lon, height, x_vel,y_vel,z_vel,roll, pitch,
%heading, x_accel, y_accel, z_accel, x_angular, y_angular, z_angular
% Read in ZERO var (used to preserve data format
%varid_zero=netcdf.inqVarID(ncid,'UTEMP_RWO');
zero_dim_ID=netcdf.inqDimID(ncid,'ZERO');
[dummy, zero_dim_len] = netcdf.inqDim(ncid, zero_dim_ID)

ZERO=netcdf.getVar(ncid,varid_zero);
ZERO=0*ZERO-32767;

% if time_dim_len < a
%     a = time_dim_len
%     i2=i1+a;%-1;
% end

%write new variables to file
%Latitude
netcdf.reDef(ncid);
LAT_APP_ID=netcdf.defVar(ncid,'LAT_APP','float',zero_dim_length);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,LAT_APP_ID,'units','deg');
netcdf.putAtt(ncid,LAT_APP_ID,'long_name','Latitude - Applanix');
netcdf.putAtt(ncid, LAT_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
LAT_APP=ZERO; %set variable to NaN, preserving format
% LAT_APP(i1:i2,1)=ZERO(1:a,:)+A(1:a,2);%write new variable preserving format
LAT_APP(1:10,1)=A(1:10,2);
netcdf.putVar(ncid,LAT_APP_ID,LAT_APP);

netcdf.close(ncid);