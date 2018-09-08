%Writes variables created by read_applanix_vnv.m to netcdf files

%read in data from matrix created by applanix_frequency convert
%[outmatrix]=applanix_frequncyconvert(infile)dlmread or fread ascii file, then run frequency convert after making it
%onehertz=applanix_frequncyconvert('RF07_ASCII.txt');
%work as a function
A=onehertztest(1:20221,:);
[a,b]=size(A);
stime=31560;%15030; %UTC start time of netcdf file
i1=abs(A(1,1)-stime)+1; %index of applanix start time
i2=i1+a-1; %index of applanix end time


ncid = netcdf.open('/h/eol/schick/DEEPWAVE/netcdffiles/DEEPWAVErf26.nc','NC_WRITE')
%extract time dimension from netcdf file
time_dim_ID=netcdf.inqDimID(ncid,'Time');
[dummy, time_dim_len] = netcdf.inqDim(ncid, time_dim_ID)
%Define new variables (lat, lon, height, x_vel,y_vel,z_vel,roll, pitch,
%heading, x_accel, y_accel, z_accel, x_angular, y_angular, z_angular
% Read in ZERO var (used to preserve data format
varid_zero=netcdf.inqVarID(ncid,'ZERO');
ZERO=netcdf.getVar(ncid,varid_zero);

if time_dim_len < a
    a = time_dim_len;
    i2=i1+a-1; %occassionally, the 01 is commented out
end

%write new variables to file
%Latitude
netcdf.reDef(ncid);
LAT_APP_ID=netcdf.defVar(ncid,'LAT_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,LAT_APP_ID,'units','deg');
netcdf.putAtt(ncid,LAT_APP_ID,'long_name','Latitude - Applanix');
netcdf.putAtt(ncid, LAT_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
LAT_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
LAT_APP(i1:i2,1)=ZERO(1:a,:)+A(1:20221,2);%write new variable preserving format
netcdf.putVar(ncid,LAT_APP_ID,LAT_APP);

%longitude
netcdf.reDef(ncid);
LON_APP_ID=netcdf.defVar(ncid,'LON_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,LON_APP_ID,'units','deg');
netcdf.putAtt(ncid,LON_APP_ID,'long_name','Longitude - Applanix');
netcdf.putAtt(ncid, LON_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
LON_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
LON_APP(i1:i2,1)=ZERO(1:a,:)+A(:,3);%write new variable preserving format
netcdf.putVar(ncid,LON_APP_ID,LON_APP);

%ellipsoid height
netcdf.reDef(ncid);
HT_APP_ID=netcdf.defVar(ncid,'HT_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,HT_APP_ID,'units','meters_WGS84');
netcdf.putAtt(ncid,HT_APP_ID,'long_name','Ellipsoid Height - Applanix');
netcdf.putAtt(ncid, HT_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
HT_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
HT_APP(i1:i2,1)=ZERO(1:a,:)+A(:,4);%write new variable preserving format
netcdf.putVar(ncid,HT_APP_ID,HT_APP);

%x velocity
netcdf.reDef(ncid);
XVEL_APP_ID=netcdf.defVar(ncid,'XVEL_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,XVEL_APP_ID,'units','m/sec');
netcdf.putAtt(ncid,XVEL_APP_ID,'long_name','X velocity - Applanix');
netcdf.putAtt(ncid, XVEL_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
XVEL_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
XVEL_APP(i1:i2,1)=ZERO(1:a,:)+A(:,5);%write new variable preserving format
netcdf.putVar(ncid,XVEL_APP_ID,XVEL_APP);

%y velocity
netcdf.reDef(ncid);
YVEL_APP_ID=netcdf.defVar(ncid,'YVEL_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,YVEL_APP_ID,'units','m/sec');
netcdf.putAtt(ncid,YVEL_APP_ID,'long_name','Y Velocity - Applanix');
netcdf.putAtt(ncid, YVEL_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
YVEL_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
YVEL_APP(i1:i2,1)=ZERO(1:a,:)+A(:,6);%write new variable preserving format
netcdf.putVar(ncid,YVEL_APP_ID,YVEL_APP);

%z velocity
netcdf.reDef(ncid);
ZVEL_APP_ID=netcdf.defVar(ncid,'ZVEL_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,ZVEL_APP_ID,'units','m/sec');
netcdf.putAtt(ncid,ZVEL_APP_ID,'long_name','Z Velocity - Applanix');
netcdf.putAtt(ncid, ZVEL_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
ZVEL_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
ZVEL_APP(i1:i2,1)=ZERO(1:a,:)+A(:,7);%write new variable preserving format
netcdf.putVar(ncid,ZVEL_APP_ID,ZVEL_APP);

%roll
netcdf.reDef(ncid);
ROLL_APP_ID=netcdf.defVar(ncid,'ROLL_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,ROLL_APP_ID,'units','deg');
netcdf.putAtt(ncid,ROLL_APP_ID,'long_name','Roll - Applanix');
netcdf.putAtt(ncid, ROLL_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
ROLL_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
ROLL_APP(i1:i2,1)=ZERO(1:a,:)+A(:,8);%write new variable preserving format
netcdf.putVar(ncid,ROLL_APP_ID,ROLL_APP);

%pitch
netcdf.reDef(ncid);
PITCH_APP_ID=netcdf.defVar(ncid,'PITCH_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,PITCH_APP_ID,'units','deg');
netcdf.putAtt(ncid,PITCH_APP_ID,'long_name','Pitch - Applanix');
netcdf.putAtt(ncid, PITCH_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
PITCH_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
PITCH_APP(i1:i2,1)=ZERO(1:a,:)+A(:,9);%write new variable preserving format
netcdf.putVar(ncid,PITCH_APP_ID,PITCH_APP);

%platform heading
netcdf.reDef(ncid);
PHDG_APP_ID=netcdf.defVar(ncid,'PHDG_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,PHDG_APP_ID,'units','deg');
netcdf.putAtt(ncid,PHDG_APP_ID,'long_name','Platform Heading - Applanix');
netcdf.putAtt(ncid, PHDG_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
PHDG_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
PHDG_APP(i1:i2,1)=ZERO(1:a,:)+A(:,10);%write new variable preserving format
netcdf.putVar(ncid,PHDG_APP_ID,PHDG_APP);

%wander angle
netcdf.reDef(ncid);
WNDR_ANG_APP_ID=netcdf.defVar(ncid,'WNDR_ANG_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,WNDR_ANG_APP_ID,'units','deg');
netcdf.putAtt(ncid,WNDR_ANG_APP_ID,'long_name','Wander Angle - Applanix');
netcdf.putAtt(ncid, WNDR_ANG_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
WNDR_ANG_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
WNDR_ANG_APP(i1:i2,1)=ZERO(1:a,:)+A(:,11);%write new variable preserving format
netcdf.putVar(ncid,WNDR_ANG_APP_ID,WNDR_ANG_APP);

%true heading
netcdf.reDef(ncid);
THDG_APP_ID=netcdf.defVar(ncid,'THDG_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,THDG_APP_ID,'units','deg');
netcdf.putAtt(ncid,THDG_APP_ID,'long_name','True Heading (uncorrected) - Applanix');
netcdf.putAtt(ncid, THDG_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
THDG_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
THDG_APP(i1:i2,1)=ZERO(1:a,:)+A(:,12);%write new variable preserving format
netcdf.putVar(ncid,THDG_APP_ID,THDG_APP);

%true heading with weird correction
varid_thdg=netcdf.inqVarID(ncid,'THDG');
THDG=netcdf.getVar(ncid,varid_thdg);
netcdf.reDef(ncid);
THDG_APP_C_ID=netcdf.defVar(ncid,'THDG_APP_C','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,THDG_APP_C_ID,'units','deg');
netcdf.putAtt(ncid,THDG_APP_C_ID,'long_name','True Heading (corrected) - Applanix');
netcdf.putAtt(ncid, THDG_APP_C_ID,'SampledRate',10);
netcdf.endDef(ncid);
for i=1800:a
    if abs(A(i,13)-THDG)>=5
        A(i,13)=A(i,13)+360;
    end
end
THDG_APP_C=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
THDG_APP_C(i1:i2,1)=ZERO(1:a,:)+A(:,12);%write new variable preserving format
netcdf.putVar(ncid,THDG_APP_C_ID,THDG_ANG_C_APP);

%x acceleration
netcdf.reDef(ncid);
XACCEL_APP_ID=netcdf.defVar(ncid,'XACCEL_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,XACCEL_APP_ID,'units','m/sec^2');
netcdf.putAtt(ncid,XACCEL_APP_ID,'long_name','X Acceleration - Applanix');
netcdf.putAtt(ncid, XACCEL_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
XACCEL_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
XACCEL_APP(i1:i2,1)=ZERO(1:a,:)+A(:,13);%write new variable preserving format
netcdf.putVar(ncid,XACCEL_APP_ID,XACCEL_APP);

%y acceleration
netcdf.reDef(ncid);
YACCEL_APP_ID=netcdf.defVar(ncid,'YACCEL_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,YACCEL_APP_ID,'units','m/sec^2');
netcdf.putAtt(ncid,YACCEL_APP_ID,'long_name','Y Acceleration - Applanix');
netcdf.putAtt(ncid, YACCEL_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
YACCEL_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
YACCEL_APP(i1:i2,1)=ZERO(1:a,:)+A(:,14);%write new variable preserving format
netcdf.putVar(ncid,YACCEL_APP_ID,YACCEL_APP);

%Z acceleration
netcdf.reDef(ncid);
ZACCEL_APP_ID=netcdf.defVar(ncid,'ZACCEL_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,ZACCEL_APP_ID,'units','m/sec^2');
netcdf.putAtt(ncid,ZACCEL_APP_ID,'long_name','Z Acceleration - Applanix');
netcdf.putAtt(ncid, ZACCEL_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
ZACCEL_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
ZACCEL_APP(i1:i2,1)=ZERO(1:a,:)+A(:,15);%write new variable preserving format
netcdf.putVar(ncid,ZACCEL_APP_ID,ZACCEL_APP);

%X ANGULAR RATE
netcdf.reDef(ncid);
X_ANGRATE_APP_ID=netcdf.defVar(ncid,'X_ANGRATE_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,X_ANGRATE_APP_ID,'units','deg/sec');
netcdf.putAtt(ncid,X_ANGRATE_APP_ID,'long_name','X Angular Rate- Applanix');
netcdf.putAtt(ncid, X_ANGRATE_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
X_ANGRATE_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
X_ANGRATE_APP(i1:i2,1)=ZERO(1:a,:)+A(:,16);%write new variable preserving format
netcdf.putVar(ncid,X_ANGRATE_APP_ID,X_ANGRATE_APP);

%Y ANGULAR RATE
netcdf.reDef(ncid);
Y_ANGRATE_APP_ID=netcdf.defVar(ncid,'Y_ANGRATE_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,Y_ANGRATE_APP_ID,'units','deg/sec');
netcdf.putAtt(ncid,Y_ANGRATE_APP_ID,'long_name','Y Angular Rate- Applanix');
netcdf.putAtt(ncid, Y_ANGRATE_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
Y_ANGRATE_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
Y_ANGRATE_APP(i1:i2,1)=ZERO(1:a,:)+A(:,17);%write new variable preserving format
netcdf.putVar(ncid,Y_ANGRATE_APP_ID,Y_ANGRATE_APP);

%Z ANGULAR RATE
netcdf.reDef(ncid);
Z_ANGRATE_APP_ID=netcdf.defVar(ncid,'Z_ANGRATE_APP','float',time_dim_ID);
%netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
netcdf.putAtt(ncid,Z_ANGRATE_APP_ID,'units','deg/sec');
netcdf.putAtt(ncid,Z_ANGRATE_APP_ID,'long_name','Z Angular Rate- Applanix');
netcdf.putAtt(ncid, Z_ANGRATE_APP_ID,'SampledRate',10);
netcdf.endDef(ncid);
Z_ANGRATE_APP=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
Z_ANGRATE_APP(i1:i2,1)=ZERO(1:a,:)+A(:,18);%write new variable preserving format
netcdf.putVar(ncid,Z_ANGRATE_APP_ID,Z_ANGRATE_APP);



% %Read Time and RSTB
% %varid_time=netcdf.inqVarID(ncid,'Time');
% %time=netcdf.getVar(ncid,varid_time);
% varid_RSTB=netcdf.inqVarID(ncid,'RSTB');
% RSTB=netcdf.getVar(ncid,varid_RSTB);
% 
% 
% %Add correction to RSTB
% RSTBC=zeros(time_dim_len,1);
% for i=1:time_dim_len;
%  RSTBC(i)=-32767;            %assign missing values to entire array
% end;
% %RF17
% stime=49800;            %start time of flight in UT seconds
% i1=54300-stime;         %find index for start of first segment
% i2=55260-stime;         %find index for end of first segment
% for i=i1:i2
%  dt = 0.3926*(RSTB(i)+273.16) - 114.86;
%  RSTBC(i) = RSTB(i) + dt;
% end;
% 
% i3=57600-stime;         %find index for start of second segment
% i4=59520-stime;         %find index for end of second segment
% for i=i3:i4
%  dt = 0.9766*(RSTB(i)+273.16) - 281.45;
%  RSTBC(i) = RSTB(i) + dt;
% end;
% 
% i1=56500-stime;         %find index for start of third segment
% i2=57300-stime;         %find index for end of third segment
% for i=i1:i2
%  dt = 0.2664*(RSTB(i)+273.16) - 78.63;
%  RSTBC(i) = RSTB(i) + dt;
% end;
% 
% i1=55500-stime;         %find index for start of fourth segment
% i2=56400-stime;         %find index for end of fourth segment
% for i=i1:i2
%  dt = 0.;
%  RSTBC(i) = RSTB(i) + dt;
% end;


% %Write new variable to netcdf file
% netcdf.putVar(ncid,LAT_APP_ID,RSTBC);

netcdf.close(ncid);
