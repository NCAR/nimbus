cont1 = input( ' Did you edit read_applanix_vnav2.m line 92 for the appropriate flight day of week? y/n: ' , 's' );
if cont1
cont2 = input('Did you edit this file for the appropriate mission name (lines 4 and 7)? y/n:','s');
if cont2=='y'
    [C]=read_applanix_vnav2('vnav_Missionrf08.out','RF08_ASCII.txt');
    [onehertztest]=applanix_frequncyconvert(C);
    ncid = netcdf.open('DEEPWAVErf08.nc','NC_WRITE');
    time_dim_ID=netcdf.inqDimID(ncid,'Time');
    [dummy, time_dim_len] = netcdf.inqDim(ncid, time_dim_ID);
%     netcdf.reDef(ncid);
%     %Latitude
% %     LAT_APP_ID=netcdf.defVar(ncid,'LAT_APP','float',time_dim_ID);
% %     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
% %     netcdf.putAtt(ncid,LAT_APP_ID,'units','deg');
% %     netcdf.putAtt(ncid,LAT_APP_ID,'long_name','Latitude - Applanix');
% %     netcdf.putAtt(ncid, LAT_APP_ID,'SampledRate',10);
% %     %longitude
% %     LON_APP_ID=netcdf.defVar(ncid,'LON_APP','float',time_dim_ID);
% %     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
% %     netcdf.putAtt(ncid,LON_APP_ID,'units','deg');
% %     netcdf.putAtt(ncid,LON_APP_ID,'long_name','Longitude - Applanix');
% %     netcdf.putAtt(ncid, LON_APP_ID,'SampledRate',10);
% %     %ellipsoid height
%     HT_APP_ID=netcdf.defVar(ncid,'HT_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,HT_APP_ID,'units','meters_WGS84');
%     netcdf.putAtt(ncid,HT_APP_ID,'long_name','Ellipsoid Height - Applanix');
%     netcdf.putAtt(ncid, HT_APP_ID,'SampledRate',10);
%     
%     %x velocity
%     XVEL_APP_ID=netcdf.defVar(ncid,'XVEL_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,XVEL_APP_ID,'units','m/sec');
%     netcdf.putAtt(ncid,XVEL_APP_ID,'long_name','X velocity - Applanix');
%     netcdf.putAtt(ncid, XVEL_APP_ID,'SampledRate',10);
%     
%     %y velocity
%     YVEL_APP_ID=netcdf.defVar(ncid,'YVEL_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,YVEL_APP_ID,'units','m/sec');
%     netcdf.putAtt(ncid,YVEL_APP_ID,'long_name','Y Velocity - Applanix');
%     netcdf.putAtt(ncid, YVEL_APP_ID,'SampledRate',10);
%     %z velocity
%     ZVEL_APP_ID=netcdf.defVar(ncid,'ZVEL_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,ZVEL_APP_ID,'units','m/sec');
%     netcdf.putAtt(ncid,ZVEL_APP_ID,'long_name','Z Velocity - Applanix');
%     netcdf.putAtt(ncid, ZVEL_APP_ID,'SampledRate',10);
%     %roll
%     ROLL_APP_ID=netcdf.defVar(ncid,'ROLL_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,ROLL_APP_ID,'units','deg');
%     netcdf.putAtt(ncid,ROLL_APP_ID,'long_name','Roll - Applanix');
%     netcdf.putAtt(ncid, ROLL_APP_ID,'SampledRate',10);
%     
%     %pitch
%     
%     PITCH_APP_ID=netcdf.defVar(ncid,'PITCH_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,PITCH_APP_ID,'units','deg');
%     netcdf.putAtt(ncid,PITCH_APP_ID,'long_name','Pitch - Applanix');
%     netcdf.putAtt(ncid, PITCH_APP_ID,'SampledRate',10);
%     
%     
%     %platform heading
%     
%     PHDG_APP_ID=netcdf.defVar(ncid,'PHDG_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,PHDG_APP_ID,'units','deg');
%     netcdf.putAtt(ncid,PHDG_APP_ID,'long_name','Platform Heading - Applanix');
%     netcdf.putAtt(ncid, PHDG_APP_ID,'SampledRate',10);
%     
%     
%     %wander angle
%     
%     WNDR_ANG_APP_ID=netcdf.defVar(ncid,'WNDR_ANG_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,WNDR_ANG_APP_ID,'units','deg');
%     netcdf.putAtt(ncid,WNDR_ANG_APP_ID,'long_name','Wander Angle - Applanix');
%     netcdf.putAtt(ncid, WNDR_ANG_APP_ID,'SampledRate',10);
%     
%     
%     %true heading
%     
%     THDG_APP_ID=netcdf.defVar(ncid,'THDG_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,THDG_APP_ID,'units','deg');
%     netcdf.putAtt(ncid,THDG_APP_ID,'long_name','True Heading (uncorrected) - Applanix');
%     netcdf.putAtt(ncid, THDG_APP_ID,'SampledRate',10);
%     
%     
%     %true heading with weird correction
%     % varid_thdg=netcdf.inqVarID(ncid,'THDG');
%     % THDG=netcdf.getVar(ncid,varid_thdg);
%     % netcdf.reDef(ncid);
%     THDG_APP_C_ID=netcdf.defVar(ncid,'THDG_APP_C','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,THDG_APP_C_ID,'units','deg');
%     netcdf.putAtt(ncid,THDG_APP_C_ID,'long_name','True Heading (corrected) - Applanix');
%     netcdf.putAtt(ncid, THDG_APP_C_ID,'SampledRate',10);
%     % netcdf.endDef(ncid);
%     % for i=1800:a
%     %     if abs(A(i,13)-THDG)>=5
%     %         A(i,13)=A(i,13)+360;
%     %     end
%     % end
%     % THDG_APP_C=ZERO-32767*ones(time_dim_len,1); %set variable to NaN, preserving format
%     % THDG_APP_C(i1:i2,1)=ZERO(1:a,:)+A(:,12);%write new variable preserving format
%     % netcdf.putVar(ncid,THDG_APP_C_ID,THDG_ANG_C_APP);
%     
%     %x acceleration
%     XACCEL_APP_ID=netcdf.defVar(ncid,'XACCEL_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,XACCEL_APP_ID,'units','m/sec^2');
%     netcdf.putAtt(ncid,XACCEL_APP_ID,'long_name','X Acceleration - Applanix');
%     netcdf.putAtt(ncid, XACCEL_APP_ID,'SampledRate',10);
%     
%     
%     %y acceleration
%     YACCEL_APP_ID=netcdf.defVar(ncid,'YACCEL_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,YACCEL_APP_ID,'units','m/sec^2');
%     netcdf.putAtt(ncid,YACCEL_APP_ID,'long_name','Y Acceleration - Applanix');
%     netcdf.putAtt(ncid, YACCEL_APP_ID,'SampledRate',10);
%     
%     %Z acceleration
%     ZACCEL_APP_ID=netcdf.defVar(ncid,'ZACCEL_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,ZACCEL_APP_ID,'units','m/sec^2');
%     netcdf.putAtt(ncid,ZACCEL_APP_ID,'long_name','Z Acceleration - Applanix');
%     netcdf.putAtt(ncid, ZACCEL_APP_ID,'SampledRate',10);
%     
%     %X ANGULAR RATE
%     X_ANGRATE_APP_ID=netcdf.defVar(ncid,'X_ANGRATE_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,X_ANGRATE_APP_ID,'units','deg/sec');
%     netcdf.putAtt(ncid,X_ANGRATE_APP_ID,'long_name','X Angular Rate- Applanix');
%     netcdf.putAtt(ncid, X_ANGRATE_APP_ID,'SampledRate',10);
%     
%     %Y ANGULAR RATE
%     Y_ANGRATE_APP_ID=netcdf.defVar(ncid,'Y_ANGRATE_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,Y_ANGRATE_APP_ID,'units','deg/sec');
%     netcdf.putAtt(ncid,Y_ANGRATE_APP_ID,'long_name','Y Angular Rate- Applanix');
%     netcdf.putAtt(ncid, Y_ANGRATE_APP_ID,'SampledRate',10);
%     
%     %Z ANGULAR RATE
%     Z_ANGRATE_APP_ID=netcdf.defVar(ncid,'Z_ANGRATE_APP','float',time_dim_ID);
%     %netcdf.putAtt(ncid,RSTBC_ID,'_FillValue',-32767.0);
%     netcdf.putAtt(ncid,Z_ANGRATE_APP_ID,'units','deg/sec');
%     netcdf.putAtt(ncid,Z_ANGRATE_APP_ID,'long_name','Z Angular Rate- Applanix');
%     netcdf.putAtt(ncid, Z_ANGRATE_APP_ID,'SampledRate',10);
%     
%     netcdf.endDef(ncid);
    netcdf.close(ncid);
else
    fprintf('You must edit read_applanix_vnav2.m before proceeding.\n Without the appropriate edit, this function will not convert into the correct time format, and data merge will be unpredictable./n');
end
else 
    fprintf('You must edit this code before proceeding.\n');
end