%-------------------------------------------------------------------------------
% Combine CO and PICARRO DATA
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
cd ('../src');
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
set(0,'DefaultAxesFontSize',20)
set(0,'DefaultLineLineWidth',2)

rafPath = [picPath,'RF',flightNum];
cd(rafPath);

%%------------------------------------------------------------------------------
% load CO DATA
%-------------------------------------------------------------------------------
load(['RF', flightNum, '_CO_data.mat'])

%-------------------------------------------------------------------------------
%% remove data from start and land as we turned the pumps off then
%-------------------------------------------------------------------------------
takeoff_t=adBefore;
land_t=adAfter;
START_LAND=find(AC.JD>takeoff_t & AC.JD<land_t)
TAKEOFF=find(AC.JD<land_t);%find(AC.PALTF<200);

AC.CORAW_AL=AC.CORAW_AL(START_LAND);
AC.DOY=AC.DOY(START_LAND);
AC.GGLAT=AC.GGLAT(START_LAND);
AC.INLETP_AL=AC.INLETP_AL(START_LAND);
AC.JD=AC.JD(START_LAND)+offset;
AC.PALTF=AC.PALTF(START_LAND);
AC.TIME=AC.TIME(START_LAND);

fig=figure()
plot(AC.JD,AC.CORAW_AL)
hold on
plot(AC.JD,AC.PALTF./300,'k')
legend('CO','PALT')
title('CO (CORAW JD)')
saveas(fig,'CORAW_JD','jpg')
saveas(fig,'CORAW_JD','fig')

%%------------------------------------------------------------------------------
% remove all the datapoints that are bad because of the reboot of CO necessary
% each hour in RF08
%-------------------------------------------------------------------------------
switch flightNum
    case '08'
        reboot=find(AC.CORAW_AL==-32767);
        AC.CORAW_AL(reboot)=NaN;
 
        reboot2=find(AC.CORAW_AL<60);
        AC.CORAW_AL(reboot2)=NaN;
 
        AA=reboot2-[reboot2(2:end);reboot2(1)];
        BB=reboot2(find(AA<-1));%% uebergang zwischen cabin air and ambient air

        reboot3=[];
        for i=1:length(BB)
            BB(i)
            reboot3i=[BB(i)+1];%:1:BB(i)+25];
            reboot3=[reboot3,reboot3i];
        end
 
        AC.CORAW_AL(reboot3)=NaN;
end
%%------------------------------------------------------------------------------
%% load PICARRO DATA
%-------------------------------------------------------------------------------
load(['RF',flightNum,'_PICARRO_data'])

fig=figure()
plot(AC.JD,AC.CORAW_AL)
hold on
plot(PICARRO.JULIAN_DAYS,PICARRO.CO2_dry,'r')
legend('CO','CO2')
title('CO2 dry from PICARRO (CO CO2)')
saveas(fig,'CO_CO2','jpg')
saveas(fig,'CO_CO2','fig')

%%------------------------------------------------------------------------------
%% interpolate all the PICARRO DATA to the CO time stamp
%-------------------------------------------------------------------------------
all_fieldname=fieldnames(PICARRO);

for i=1:length(fieldnames(PICARRO))
fieldname=char(all_fieldname(i));
if strcmp(fieldname,'timestamp')==1
    
else
before_interp=PICARRO.(fieldname);
after_interp=interp1(PICARRO.JULIAN_DAYS,before_interp,AC.JD);
PICARRO_interp.(fieldname)=after_interp;
end
end

fig=figure()
plot(AC.JD,AC.CORAW_AL)
hold on
plot(AC.JD,PICARRO_interp.CO2_dry,'r')
plot(AC.JD,AC.PALTF./300,'k')
legend('CO','CO2','PALTF')
title('CO2 Interpolated to CO timestamp (CO CO2 interp)')
saveas(fig,'CO_CO2_interp','jpg')
saveas(fig,'CO_CO2_interp','fig')

%%------------------------------------------------------------------------------
%% remove all the data where we have backflow (where CO2_dry>420)
%-------------------------------------------------------------------------------

% BAD1=find(PICARRO_interp.CO2_dry>420);
% % also remove 3 datapoints before and 10 datapoints after
% A=BAD1-[BAD1(2:end);BAD1(1)];
% B=BAD1(find(A<-1));%% uebergang zwischen cabin air and ambient air
% BAD2=[];
% for i=1:(length(B)-1)
%     B(i)
%     BAD2i=[B(i):1:B(i)+50];
%     BAD2=[BAD2,BAD2i];
% end
% 
% C=BAD1-[BAD1(end);BAD1(1:end-1)];
% 
% 
% 
% D=BAD1(find(C>1));%% uebergang zwischen cabin air and ambient air
% BAD3=[];
% for i=1:length(D)
%     D(i)
%     BAD3i=[D(i)-10:1:D(i)];
%     BAD3=[BAD3,BAD3i];
% end
% 
% 
% 
% BAD=[BAD1;BAD2';BAD3'];
% % 
% 
% BAD=BAD(find(BAD<length(AC.JD)));
% for i=1:length(fieldnames(PICARRO_interp))
%    fieldname=char(all_fieldname(i));
%    PICARRO_interp_i=PICARRO_interp.(fieldname);
%    PICARRO_interp_i(BAD)=NaN;
%    AC.CORAW_AL(BAD)=NaN;
% end
% 
% figure()
% plot(AC.JD,AC.CORAW_AL,'bo')
% hold on
% plot(AC.JD,PICARRO_interp.CO2_dry,'ro')
% % legend('CO','CO2')
% hold on
% plot(AC.JD(BAD),AC.CORAW_AL(BAD),'kx')
% hold on
% plot(AC.JD(BAD),PICARRO_interp.CO2_dry(BAD),'kx')
% legend('CO','CO2')

%%------------------------------------------------------------------------------
%% pressure drop in CONTRAST RF05 
% pressure spike in CONTRAST RF14
% remove that data
%-------------------------------------------------------------------------------
if strcmp(project,'CONTRAST')==1
  switch flightNum
    case '14'
        BAD_PRESSURE=find(PICARRO_interp.CavityPressure>154.0);
        BAD_PRESSURE
        for i=1:length(fieldnames(PICARRO_interp))
            fieldname=char(all_fieldname(i));
            if strcmp(fieldname,'timestamp')==1
    
            else
                PICARRO_interp_i=PICARRO_interp.(fieldname);
                if exist('BAD_PRESSURE','var')
                    PICARRO_interp_i(BAD_PRESSURE)=NaN;
                    AC.CORAW_AL(BAD_PRESSURE)=NaN;
                end
            end
        end 
    case '05'
        BAD_PRESSURE=find(PICARRO_interp.CavityPressure<151.3 | PICARRO_interp.CavityPressure>151.7);
        for i=1:length(fieldnames(PICARRO_interp))
            fieldname=char(all_fieldname(i));
            if strcmp(fieldname,'timestamp')==1
    
            else
                PICARRO_interp_i=PICARRO_interp.(fieldname);
                if exist('BAD_PRESSURE','var')
                    PICARRO_interp_i(BAD_PRESSURE)=NaN;
                    AC.CORAW_AL(BAD_PRESSURE)=NaN;
                end
            end
        end
    end
end

fig=figure()
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CavityPressure,'ro')
if exist('BAD_PRESSURE','var')
    hold on
    plot(PICARRO_interp.JULIAN_DAYS(BAD_PRESSURE),PICARRO_interp.CavityPressure(BAD_PRESSURE),'bx')
end
title('Cavity Pressure')
saveas(fig,'CavityPressure','jpg')
saveas(fig,'CavityPressure','fig')

%%------------------------------------------------------------------------------
%% set up interp_NAN arrays from which to remove all bad data
%-------------------------------------------------------------------------------
all_fieldname=fieldnames(PICARRO_interp);

for i=1:length(fieldnames(PICARRO_interp))
    fieldname=char(all_fieldname(i));
    PICARRO_interp_i=PICARRO_interp.(fieldname);
    if strcmp(fieldname,'JULIAN_DAYS')==1
        PICARRO_interp_NANs.(fieldname)=PICARRO_interp.JULIAN_DAYS; 
    else
        PICARRO_interp_NANs.(fieldname)=PICARRO_interp_i;
    end
end

%%------------------------------------------------------------------------------
% find CO, CH4, CO2 calibration times based on ranges set in constants file
% UPGRADE: This misses cal values on the way up and down that should be eliminated
%-------------------------------------------------------------------------------

CO2_CAL=find(PICARRO_interp.CO2_dry>CO2calLow & PICARRO_interp.CO2_dry<CO2calHigh);
CH4_CAL=find(PICARRO_interp.CH4_dry>CH4calLow & PICARRO_interp.CH4_dry<CH4calHigh);
CO_CAL=find(AC.CORAW_AL>COcalLow & AC.CORAW_AL<COcalHigh);

% This was intersect, but the if found a cal value in CO2, but not CH4, it
% wasn't removed. Should be union so that if we identify it in any var,
% it's removed from all.
PICARRO_CAL=union(CO2_CAL,CH4_CAL);
ALL_CAL=union(PICARRO_CAL,CO_CAL);


fig=figure()
subplot(3,1,1)
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CO2_dry,'gx')
title('CALIBRATIONS')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),PICARRO_interp.CO2_dry(ALL_CAL),'kx')
%legend('CO2 dry','CO2 dry CAL')
subplot(3,1,2)
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CH4_dry,'b')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),PICARRO_interp.CH4_dry(ALL_CAL),'kx')
%legend('CH4 dry','CH4 dry CAL')
subplot(3,1,3)
plot(PICARRO_interp.JULIAN_DAYS,AC.CORAW_AL,'r')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),AC.CORAW_AL(ALL_CAL),'kx')
%legend('CO','CO CAL')
saveas(fig,'cals','jpg')
saveas(fig,'cals','fig')

fig=figure()
plot(AC.JD,AC.CORAW_AL,'r')
hold on
plot(AC.JD(CO_CAL),AC.CORAW_AL(CO_CAL),'kx')
legend('CO','CO CAL')
title('CO Calibrations')
saveas(fig,'CO_cal','jpg')
saveas(fig,'CO_cal','fig')

%%------------------------------------------------------------------------------
%% remove cals that are before takeoff or after landing
%-------------------------------------------------------------------------------

ZZ=[find(PICARRO_interp.JULIAN_DAYS<takeoff_t);find(PICARRO_interp.JULIAN_DAYS>land_t & PICARRO_interp.JULIAN_DAYS<takeoff_t)];
CAL=setdiff(ALL_CAL,ZZ);

fig=figure()
subplot(3,1,1)
plot(PICARRO_interp.JULIAN_DAYS(CAL),AC.CORAW_AL(CAL),'kx')
title('CALIBRATIONS')
grid on
legend('CO')
subplot(3,1,2)
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2_dry(CAL),'rx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2(CAL),'bx')
grid on
legend('CO2 dry','CO2')
subplot(3,1,3)
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CH4_dry(CAL),'rx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CH4(CAL),'bx')
grid on
legend('CH4 dry','CH4')
saveas(fig,'CH4_cal','jpg')
saveas(fig,'CH4_cal','fig')

%%------------------------------------------------------------------------------
% find CO zeros based on ranges set in constants file
% UPGRADE: This misses cal values on the way up and down that should be eliminated
%-------------------------------------------------------------------------------
CO_BG=find(AC.CORAW_AL>zeroLow & AC.CORAW_AL<zeroHigh);

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_BG),AC.CORAW_AL(CO_BG),'kx')
legend('CO','CO BG')
title('CO BG identification')
saveas(fig,'CO_bg','jpg')
saveas(fig,'CO_bg','fig')

%%------------------------------------------------------------------------------
%% rough calculation of CO sensitivity (mean of all call and zero values)
%-------------------------------------------------------------------------------
mean_CO_BG=mean(AC.CORAW_AL(CO_BG));
mean_CO_CAL=mean(AC.CORAW_AL(CO_CAL));

%-------------------------------------------------------------------------------
% Plot the mean and cal_COMR adjusted CO, but don't save it anywhere (!!!!)
% cal_COMR is a fn of the canister and is defined in the constants file
%-------------------------------------------------------------------------------
fig=figure()
plot(AC.JD,AC.CORAW_AL,'bx')
hold on
plot(AC.JD,(AC.CORAW_AL-mean_CO_BG).*cal_COMR./(mean_CO_CAL-mean_CO_BG),'ro')
legend('CO','CO-BG_(mean)')
title('CO minus mean BG, scaled to cal gas')
saveas(fig,'mean_CO_bg','jpg')
saveas(fig,'mean_CO_bg','fig')

%%------------------------------------------------------------------------------
%% better calibration of CO using a linear interpolation between the different 
% cals instead of a simple mean.
%-------------------------------------------------------------------------------
%-------------------------------------------------------------------------------
% Linear interpolation of calibration
% find each calibration separately
%-------------------------------------------------------------------------------
CO_CAL_2=[CO_CAL(2:end);CO_CAL(1)];

indexi=[1;find((CO_CAL-CO_CAL_2)<-100);length(CO_CAL)];

CO_CAL_individual=[];
CO_CAL_times=[];

for iii=1:(length(indexi)-1)
    iii
    CO_CAL_i=mean(AC.CORAW_AL(CO_CAL(indexi(iii)+3):CO_CAL(indexi(iii+1)-3)));
    CO_CAL_time_i=median(AC.JD(CO_CAL(indexi(iii)+3):CO_CAL(indexi(iii+1)-3)));
    
    CO_CAL_individual=[CO_CAL_individual,CO_CAL_i];
    CO_CAL_times=[CO_CAL_times,CO_CAL_time_i];
end

%-------------------------------------------------------------------------------
% Stützstellen (grid points)
%-------------------------------------------------------------------------------
xs = CO_CAL_times;
ys = CO_CAL_individual;
timestep= AC.TIME(2)-AC.TIME(1);

%-------------------------------------------------------------------------------
% Interpolationspunkte (Interpolation)
%-------------------------------------------------------------------------------
% The units in this command are mixed. CO_CAL_times are in decimal days, timestep 
% is in seconds. Convert timestep to decimal days.
timestep=timestep/(3600*24);
xi = CO_CAL_times(1):timestep:CO_CAL_times(end);
CO_CAL_interp = interp1(xs, ys, xi);

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_CAL),AC.CORAW_AL(CO_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')
hold on
plot(xi, CO_CAL_interp, 'gx')
title('individual CAL interpolations')
legend('CORAW','CO cals','CO cal ind','CO CAL interp')
saveas(fig,'CO_CAL_interp','jpg')
saveas(fig,'CO_CAL_interp','fig')

%-------------------------------------------------------------------------------
% Add the first cal to the times before the first cal and the last cal
% number to after the last cal
%-------------------------------------------------------------------------------
before_first_cal=find(AC.JD<xi(1));
during_the_cals=find(AC.JD>=xi(1) & AC.JD<=xi(end));%<=
after_last_cal=find(AC.JD>xi(end));

CO_CAL_ALL=zeros(1,length(AC.JD))*NaN;
CO_CAL_ALL(before_first_cal)=ones(1,length(before_first_cal))*CO_CAL_individual(1);
CO_CAL_ALL(during_the_cals)=CO_CAL_interp(1:end-1);
CO_CAL_ALL(after_last_cal)=ones(1,length(after_last_cal))*CO_CAL_individual(end);

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_CAL),AC.CORAW_AL(CO_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')
plot(AC.JD, CO_CAL_ALL, 'g')
title('CO CAL interpolation fit')
legend('CORAW','CO cals','CO cal ind','CO CAL fit')
saveas(fig,'CO_CAL_ALL','jpg')
saveas(fig,'CO_CAL_ALL','fig')


%%------------------------------------------------------------------------------
% Linear interpolation of CO background
% find each calibration separately
%-------------------------------------------------------------------------------
CO_BG_2=[CO_BG(2:end);CO_BG(1)];

indexi=[1;find((CO_BG-CO_BG_2)<-100);length(CO_BG)];

CO_BG_individual=[];
CO_BG_times=[];

for iii=1:(length(indexi)-1)
    iii
    CO_BG_i=mean(AC.CORAW_AL(CO_BG(indexi(iii)+3):CO_BG(indexi(iii+1)-3)));
    CO_BG_time_i=median(AC.JD(CO_BG(indexi(iii)+3):CO_BG(indexi(iii+1)-3)));
    
    CO_BG_individual=[CO_BG_individual,CO_BG_i];
    CO_BG_times=[CO_BG_times,CO_BG_time_i];
end

%-------------------------------------------------------------------------------
% Stützstellen (grid points)
%-------------------------------------------------------------------------------
xs = CO_BG_times;
ys = CO_BG_individual;
timestep= AC.TIME(2)-AC.TIME(1);

%-------------------------------------------------------------------------------
% Interpolationspunkte (Interpolation)
%-------------------------------------------------------------------------------
% The units in this command are mixed. CO_CAL_times are in decimal days, timestep 
% is in seconds. Convert timestep to decimal days.
timestep=timestep/(3600*24);
xi = CO_BG_times(1):timestep:CO_BG_times(end);
CO_BG_interp = interp1(xs, ys, xi);

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_BG),AC.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
hold on
plot(xi, CO_BG_interp, 'gx')
title('individual CAL interpolations')
legend('CORAW','CO BG','CO BG ind','interp')
saveas(fig,'CO_BG_int','jpg')
saveas(fig,'CO_BG_int','fig')

%-------------------------------------------------------------------------------
% Add the first cal to the times before the first cal and the last cal
% number to after the last cal
%-------------------------------------------------------------------------------
before_first_BG=find(AC.JD<xi(1));
during_the_BGs=find(AC.JD>=xi(1) & AC.JD<=xi(end));
after_last_BG=find(AC.JD>xi(end));

CO_BG_ALL=zeros(1,length(AC.JD))*NaN;
CO_BG_ALL(before_first_BG)=ones(1,length(before_first_BG))*CO_BG_individual(1);
CO_BG_ALL(during_the_BGs)=CO_BG_interp(1:end-1);
CO_BG_ALL(after_last_BG)=ones(1,length(after_last_BG))*CO_BG_individual(end);

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_BG),AC.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
plot(AC.JD, CO_BG_ALL, 'g')
legend('CORAW','CO BG','CO BG ind','CO BG fit')
saveas(fig,'CO_BG_ALL','jpg')
saveas(fig,'CO_BG_ALL','fig')


%-------------------------------------------------------------------------------
%% Correct the CO for the interpolated cal values and the cal gas canister ppb.
% cal_COMR is a fn of the canister and is defined in the constants file
%-------------------------------------------------------------------------------
AC.CO_PPB=(AC.CORAW_AL-CO_BG_ALL').*cal_COMR./(CO_CAL_ALL'-CO_BG_ALL');

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD,AC.CO_PPB,'rx')
legend('CORAW','CO PPB')
saveas(fig,'CO_PPB','jpg')
saveas(fig,'CO_PPB','fig')

%-------------------------------------------------------------------------------
%% Blank out the zeros and the cals, and make sure the array length didn't change.
%-------------------------------------------------------------------------------
before=length(AC.JD)
after=length(AC.CO_PPB)

AC.CO_PPB(CAL)=NaN;
AC.CO_PPB(CO_BG)=NaN;
%AC.CO_PPB(TAKEOFF)=NaN;

length(AC.JD)
length(AC.CO_PPB)

if (before ~= length(AC.JD)) 
	error('CO array length changed when cal vals removed')
end
if (before ~= length(AC.CO_PPB)) 
	error('CO array length changed when bg vals removed')
end

fig=figure()
plot(AC.JD,AC.CO_PPB,'bx')
title('CO PPBV - still need to remove remaining cals')
saveas(fig,'CO_PPBV','jpg')
saveas(fig,'CO_PPBV','fig')


%%------------------------------------------------------------------------------
%% This is a final kludge. If we didn't catch and eliminate all the cal points
% try to get more by simply drawing a straight line above and below the good data
% and removing all points outside that.
% This misses values on the way in and out of 
% the cal, so... UPGRADE: Change this part of the code to look for the steep 
% changes that mark the start and end of a cal.
%-------------------------------------------------------------------------------
FILTER=find(AC.CO_PPB>filterHigh);
FILTER2=find(AC.CO_PPB<filterLow);

AC.CO_PPB([FILTER;FILTER2])=NaN;

%%------------------------------------------------------------------------------
%% Now can do the CO2 and CH4 Calibrations based on the CO calibration
%-------------------------------------------------------------------------------
PICARRO_CALIBRATED.CO2_dry_ppm=PICARRO_interp_NANs.CO2_dry+1.38;
PICARRO_CALIBRATED.CO2_dry_ppm([ALL_CAL;CO_BG;FILTER;FILTER2])=NaN;

PICARRO_CALIBRATED.CH4_dry_ppm=PICARRO_interp_NANs.CH4_dry+0.0339;
PICARRO_CALIBRATED.CH4_dry_ppm([ALL_CAL;CO_BG;FILTER;FILTER2])=NaN;

if exist('BAD_PRESSURE','var')
    PICARRO_CALIBRATED.CO2_dry_ppm([BAD_PRESSURE])=NaN;
    PICARRO_CALIBRATED.CH4_dry_ppm([BAD_PRESSURE])=NaN;
end

fig=figure()
subplot(3,1,1)
plot(AC.JD,AC.CO_PPB,'bx')
title('Final Data')
% hold on
% plot(AC.JD,AC.PALTF./3/10,'k')
legend('CO [ppbv]')
% ylim([60 160])
grid on
subplot(3,1,2)
plot(AC.JD,PICARRO_CALIBRATED.CO2_dry_ppm,'rx')
legend('CO2 [ppmv]')
% ylim([394 420])
grid on
subplot(3,1,3)
plot(AC.JD,PICARRO_CALIBRATED.CH4_dry_ppm,'gx')
legend('CH4 [ppmv]')
grid on
% ylim([1.79 1.88])
saveas(fig,'final','jpg')
saveas(fig,'final','fig')



%%------------------------------------------------------------------------------
%% save the pre_ICARTT file
%-------------------------------------------------------------------------------
TAKEOFF2=find(AC.PALTF>200);%take only data after takeoff

preICARTT.CO_PPBV=AC.CO_PPB(TAKEOFF2);
preICARTT.PALTF=AC.PALTF(TAKEOFF2);
preICARTT.TIME=AC.TIME(TAKEOFF2);
preICARTT.DOY=AC.DOY(TAKEOFF2);
preICARTT.CO2_dry_ppm=PICARRO_CALIBRATED.CO2_dry_ppm(TAKEOFF2);
preICARTT.CH4_dry_ppm=PICARRO_CALIBRATED.CH4_dry_ppm(TAKEOFF2);
preICARTT.H2O=PICARRO_interp_NANs.H2O(TAKEOFF2);




filename=['RF', flightNum, 'preICARTT_data.mat'];

save(filename,['preICARTT'])
save(['RF', flightNum, 'quicklook.mat']);
