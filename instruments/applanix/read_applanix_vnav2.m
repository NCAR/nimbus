function [C] = read_applanix_vnav2(filename,fileout)
%EDITED FOR RF23 Monday July14 for Stuart
% EDIT August 2014 Kelly Schick
% Read a binary applanix real time solution file
% Run POSPAC software to read in recorded applanix file
% It automatically creates a binary file for real time INS solution
% Look for file in project folder, ie if the project is named "current"
%   PosPac MM5\current\Mission\Extract\vnav_Mission.out
% call function with filename = 'vnav_Mission.out'
% call the function assuming filename is in the current working directory
%   or call with full pathname to file
% Format for vnav file and rms error file vrms file is in the
%   POSPac MMS GNSS-Inertial Tools.pdf manual in section 10.
% Output:
%   call function with fileout = the output ascii file name
%
% Output array B has dimension [ndata,17] where the 17 columns are:
% time_(seconds_of_week)
% latitude_(deg)
% longitude_(deg)
% altitude_(meters)
% x_velocity_(meters/second)
% y_velocity_(meters/second)
% z_velocity_(meters/second)
% roll_(deg)
% pitch_(deg)
% platform_heading_(deg)
% wander_angle_(deg)
% x_body_acceleration_(meters/second^2)
% y_body_acceleration_(meters/second^2)
% z_body_acceleration_(meters/second^2)
% x_body_angular_rate_(deg/second)
% y_body_angular_rate_(deg/second)
% z_body_angular_rate_(deg/second)

fid = fopen(filename,'r');

% Read all the values into 1D array A assuming double floating point
A=fread(fid,'double','n');

B = reshape(A,17,[])';
fclose(fid);
[a,b]=size(B);
C=zeros(a,b+1);
C(1:a,1:b)=B; %create a new matrix for column manipulation %%EDIT 8 AUG 2014 KES
% output the ascii data
fid2 = fopen(fileout,'w');
%fprintf(fid2,['time_(seconds_of_week) lat_(deg) long_(deg) ' ...
    %'ht_ellipsoid_(meters_WGS84) x_vel_(m/s) y_vel_(m/s) z_vel_(m/s) ' ...
    %'roll_(deg) pitch_(deg) heading_(deg) wander_angle_(deg) ' ...
    %'x_body_accel_(m/s^2) y_body_accel_(m/s^2) z_body_accel_(m/s^2) ' ...
    %'x_body_angular_rate_(deg/s) y_body_angular_rate_(deg/s) ' ...
    %'z_body_angular_rate_(deg/s)\n']);  %EDIT 7 AUG 2014 KES
fprintf(fid2,['time_(UTC_seconds_of_day) lat_(deg) long_(deg) ' ... %EDIT 7 AUG 2014 KES
    'ht_ellipsoid_(meters_WGS84) x_vel_(m/s) y_vel_(m/s) z_vel_(m/s) ' ...
    'roll_(deg) pitch_(deg) heading_(deg) wander_angle_(deg) true_heading_(deg) ' ... %EDIT 8 AUG 2014 KES
    'x_body_accel_(m/s^2) y_body_accel_(m/s^2) z_body_accel_(m/s^2) ' ...
    'x_body_angular_rate_(deg/s) y_body_angular_rate_(deg/s) ' ...
    'z_body_angular_rate_(deg/s)\n']); 
% Change from radians to degrees everywhere
C(:,2) = B(:,2)*180/pi();
C(:,3) = B(:,3)*180/pi();
%B(:,8) = B(:,4)*180/pi();
C(:,8) = B(:,8)*180/pi(); %EDIT 5 AUG 2014 KES
C(:,9) = B(:,9)*180/pi();
C(:,10) = B(:,10)*180/pi(); %platform heading
C(:,11) = B(:,11)*180/pi(); %wander angle
%add a column to calculate true heading by subtracting wander angle from
%platform heading %%EDIT 8 AUG 2014
C(:,12) = C(:,10)-C(:,11); 
% for j=1:a
%     if 0<= C(j,10) && C(j,10) < 180
%         C(j,12) = (360+C(j,10))-C(j,11);
%     elseif 180 <= C(j,10) && C(j,10) <=360
%         C(j,12)=C(j,10)-C(j,11);
%     end
%     if C(j,12) > 360
%         C(j,12)=C(j,12)-360;
%     elseif C(j,12)<0
%         C(j,12)=C(j,12)+360;
%     end
% end
C(:,13) = B(:,12);
C(:,14) = B(:,13);
C(:,15) = B(:,14);
C(:,16) = B(:,15)*180/pi();
C(:,17) = B(:,16)*180/pi();
C(:,18) = B(:,17)*180/pi();

%convert time to seconds UTC, not seconds of week %%EDIT 7 AUG 2014 KES

days = 2* 86400*ones(a,1); %Sunday = 0, Monday =1, Tuesday = 2, Wednesday =3, Thursday = 4, Friday =5, Saturday =6
C(:,1) = B(:,1) - days -16; %GPS time is 16 seconds ahead of UTC
% group1=C(1:10,:);
% groupcount=floor(b/10);
% for i=1:18
%     
%     avg(i)=mean(group1(:,i));
%     
% end
fprintf(fid2,'%10.4f %14.9f %14.9f %12.5f %10.5f %10.5f %10.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f\n',C'); %EDIT 8 AUG 2014 KES
%changed to matrix C and added a %11.5f to incoroporate new variable.
fclose(fid2);
