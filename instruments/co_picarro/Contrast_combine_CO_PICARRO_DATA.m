%-------------------------------------------------------------------------------
% Combine CO and PICARRO DATA
%-------------------------------------------------------------------------------

%-------------------------------------------------------------------------------
% When working in interactive mode, you want to clear everything before you 
% rerun. Makes debugging alot easier if declared vars aren't hanging around 
% with their previous values.
%-------------------------------------------------------------------------------
clc
clear all 
close all

%%------------------------------------------------------------------------------
% Set project- and flight-specific constants
% UPGRADE: This section should be in an input file that all three matlab scripts% can call.
%-------------------------------------------------------------------------------

project=['CONTRAST'];
year=[2014];
%flight = '11';
%adBefore = 44;
%adAfter = 44.26;
% filterLow=10;
%flight = '12';
%adBefore = 48.998;
%adAfter = 49.232;
picPath='/net/work/Projects/CONTRAST/PICARRO/';
%filterHigh=100;
%filterLow=50);
flight = '13';
adBefore = 50.709;
adAfter = 51.016;
picPath='/Users/janine/Desktop/CONTRAST/PICARRO/';
% apply brute force filtering of CO if nothing else worked
filterLow=60;
filterHigh=120;

% offset needs to be +1 if start after UTC 00 (e.g. for RF05), zero otherwise
offset=0;

% ranges to look for cal values. This misses values on the way in and out of the cal, so...
% UPGRADE: Change this part of the code to look for the steep changes that mark the start and
% end of a cal.
zeroLow=-5;
zeroHigh=50;
COcalLow=91;
COcalHigh=110;
CH4calLow=1.75;
CH4calHigh=1.78;
CO2calLow=382;
CO2calHigh=390;



set(0,'DefaultAxesFontSize',20)
set(0,'DefaultLineLineWidth',2)

rafPath = [picPath,'RF',flight];
cd(rafPath);

%%------------------------------------------------------------------------------
% load CO DATA
%------------------------------------------------------------------------------
load(['RF', flight, '_CO_data.mat'])

%% remove data from start and land as we turnd the pumps off then

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
saveas(fig,'CORAW_JD','jpg')
saveas(fig,'CORAW_JD','fig')

%%------------------------------------------------------------------------------
% remove all the datapoints that are bad because of the reboot of CO necessary
% each hour in RF08
%------------------------------------------------------------------------------
switch flight
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
% load PICARRO DATA
%------------------------------------------------------------------------------
load(['RF',flight,'_PICARRO_data'])

fig=figure()
plot(AC.JD,AC.CORAW_AL)
hold on
plot(PICARRO.JULIAN_DAYS,PICARRO.CO2_dry,'r')
legend('CO','CO2')
saveas(fig,'CO_CO2','jpg')
saveas(fig,'CO_CO2','fig')

%%------------------------------------------------------------------------------
% interpolate all the PICARRO DATA to the CO time stamp
%------------------------------------------------------------------------------
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
title('CO CO2 Interpolated')
saveas(fig,'CO_CO2_interp','jpg')
saveas(fig,'CO_CO2_interp','fig')

%%------------------------------------------------------------------------------
% remove all the data where we have backflow (where CO2_dry>420)
%------------------------------------------------------------------------------

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
% pressure drop in RF05 remove that data
%------------------------------------------------------------------------------
% switch flight
%     case '05'
        BAD_PRESSURE=find(PICARRO_interp.CavityPressure<151.3 | PICARRO_interp.CavityPressure>151.7);
% end

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
% remove all bad data
%------------------------------------------------------------------------------

all_fieldname=fieldnames(PICARRO_interp);

for i=1:length(fieldnames(PICARRO_interp))
fieldname=char(all_fieldname(i));
PICARRO_interp_i=PICARRO_interp.(fieldname);
if strcmp(fieldname,'JULIAN_DAYS')==1
PICARRO_interp_NANs.(fieldname)=PICARRO_interp.JULIAN_DAYS; 
else
    if exist('BAD_PRESSURE','var')
        % PICARRO_interp_i(BAD)=NaN;
        PICARRO_interp_i(BAD_PRESSURE)=NaN;
    end
% PICARRO_interp_i(BAD_STRANGE)=NaN;
PICARRO_interp_NANs.(fieldname)=PICARRO_interp_i;
end
end


%%------------------------------------------------------------------------------
% find calibration times: constants are set at top of code 
%------------------------------------------------------------------------------

CO2_CAL=find(PICARRO_interp.CO2_dry>CO2calLow & PICARRO_interp.CO2_dry<CO2calHigh);
CH4_CAL=find(PICARRO_interp.CH4_dry>CH4calLow & PICARRO_interp.CH4_dry<CH4calHigh);
CO_CAL=find(AC.CORAW_AL>COcalLow & AC.CORAW_AL<COcalHigh);

PICARRO_CAL=intersect(CO2_CAL,CH4_CAL);

ALL_CAL=intersect(PICARRO_CAL,CO_CAL);


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

%% find all call times (and delete those points that are not real cals!

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

%% find CO zeros

CO_BG=find(AC.CORAW_AL>zeroLow & AC.CORAW_AL<zeroHigh);

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_BG),AC.CORAW_AL(CO_BG),'kx')
legend('CO','CO BG')
title('CO BG identification')
saveas(fig,'CO_bg','jpg')
saveas(fig,'CO_bg','fig')


%% CO calibration

% rough calculation of sensitivity (mean of all call and zero values)
mean_CO_BG=mean(AC.CORAW_AL(CO_BG));
mean_CO_CAL=mean(AC.CORAW_AL(CO_CAL));




fig=figure()
plot(AC.JD,AC.CORAW_AL,'bx')
hold on
plot(AC.JD,(AC.CORAW_AL-mean_CO_BG).*141./(mean_CO_CAL-mean_CO_BG),'ro')
legend('CO','CO-BG')
saveas(fig,'mean_CO_bg','jpg')
saveas(fig,'mean_CO_bg','fig')



%% better calibration using a linear interpolation between the different cals

% Linear interpolation of calibration
% find each calibration separately
CO_CAL_2=[CO_CAL(2:end);CO_CAL(1)];

indexi=[1;find((CO_CAL-CO_CAL_2)<-100);length(CO_CAL)];

CO_CAL_individual=[];
CO_CAL_times=[];

for iii=2:(length(indexi)-2)
    iii
    CO_CAL_i=mean(AC.CORAW_AL(CO_CAL(indexi(iii)+3):CO_CAL(indexi(iii+1)-3)));
    CO_CAL_time_i=median(AC.JD(CO_CAL(indexi(iii)+3):CO_CAL(indexi(iii+1)-3)));
    
    CO_CAL_individual=[CO_CAL_individual,CO_CAL_i];
    CO_CAL_times=[CO_CAL_times,CO_CAL_time_i];
end


fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_CAL),AC.CORAW_AL(CO_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')
title('individual CO CALs')
legend('CORAW','CALS','CO CAL ind')
saveas(fig,'CO_CAL_ind','jpg')
saveas(fig,'CO_CAL_ind','fig')

% clc;
% close all;
% clear all;

% Stützstellen
xs = CO_CAL_times;%[0, 35, 75, 110, 150];
ys = CO_CAL_individual;%[20, 0, -50, 40, 20];
timestep= AC.TIME(2)-AC.TIME(1);
% Interpolationspunkte
xi = CO_CAL_times(1):timestep:CO_CAL_times(end);

% Interpolation
CO_CAL_interp = interp1(xs, ys, xi);

% % Plotten
% figure()
% plot(xs, ys, 'or')
% hold on
% plot(xi, yi, '.b')
% % axis equal

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

%add the first cal to the times before the first cal and the last cal
%number to after the last cal

before_first_cal=find(AC.JD<xi(1));
during_the_cals=find(AC.JD>=xi(1) & AC.JD<=xi(end));%<=
after_last_cal=find(AC.JD>xi(end));


CO_CAL_ALL=zeros(1,length(AC.JD))*NaN;

CO_CAL_ALL(before_first_cal)=ones(1,length(before_first_cal))*CO_CAL_individual(1);
CO_CAL_ALL(during_the_cals)=CO_CAL_interp(1:end);
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


%% Linear interpolation of BG
% find each calibration separately
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


fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_BG),AC.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
legend('CORAW','CO BG','CO BG ind')
saveas(fig,'CO_BG_ind','jpg')
saveas(fig,'CO_BG_ind','fig')

% clc;
% close all;
% clear all;

% Stützstellen
xs = CO_BG_times;%[0, 35, 75, 110, 150];
ys = CO_BG_individual;%[20, 0, -50, 40, 20];
timestep= AC.TIME(2)-AC.TIME(1);
% Interpolationspunkte
xi = CO_BG_times(1):timestep:CO_BG_times(end);

% Interpolation
CO_BG_interp = interp1(xs, ys, xi);

% % Plotten
% figure()
% plot(xs, ys, 'or')
% hold on
% plot(xi, yi, '.b')
% % axis equal

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD(CO_BG),AC.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
hold on
plot(xi, CO_BG_interp, 'gx')
legend('CORAW','CO BG','CO BG ind','interp')
saveas(fig,'CO_BG_int','jpg')
saveas(fig,'CO_BG_int','fig')

%add the first cal to the times before the first cal and the last cal
%number to after the last cal

before_first_BG=find(AC.JD<xi(1));
during_the_BGs=find(AC.JD>=xi(1) & AC.JD<=xi(end));
after_last_BG=find(AC.JD>xi(end));


CO_BG_ALL=zeros(1,length(AC.JD))*NaN;

CO_BG_ALL(before_first_BG)=ones(1,length(before_first_BG))*CO_BG_individual(1);
CO_BG_ALL(during_the_BGs)=CO_BG_interp;
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
%%


AC.CO_PPB=(AC.CORAW_AL-CO_BG_ALL').*140./(CO_CAL_ALL'-CO_BG_ALL');

fig=figure()
plot(AC.JD,AC.CORAW_AL,'b')
hold on
plot(AC.JD,AC.CO_PPB,'rx')
legend('CORAW','CO PPB')
saveas(fig,'CO_PPB','jpg')
saveas(fig,'CO_PPB','fig')

length(AC.JD)
length(AC.CO_PPB)

AC.CO_PPB(CAL)=NaN;
AC.CO_PPB(CO_BG)=NaN;
% AC.CO_PPB(BAD)=NaN;
if exist('BAD_PRESSURE','var')
    AC.CO_PPB(BAD_PRESSURE)=NaN;
end
%AC.CO_PPB(TAKEOFF)=NaN;

length(AC.JD)
length(AC.CO_PPB)

fig=figure()
plot(AC.JD,AC.CO_PPB,'bx')
title('CO PPBV')
saveas(fig,'CO_PPBV','jpg')
saveas(fig,'CO_PPBV','fig')


%% CO2 Calibration

FILTER=find(AC.CO_PPB>filterHigh);
FILTER2=find(AC.CO_PPB<filterLow);


PICARRO_CALIBRATED.CO2_dry_ppm=PICARRO_interp_NANs.CO2_dry+1.38;
PICARRO_CALIBRATED.CO2_dry_ppm([ALL_CAL;CO_BG;FILTER;FILTER2])=NaN;

PICARRO_CALIBRATED.CH4_dry_ppm=PICARRO_interp_NANs.CH4_dry+0.0339;
PICARRO_CALIBRATED.CH4_dry_ppm([ALL_CAL;CO_BG;FILTER;FILTER2])=NaN;


AC.CO_PPB([FILTER;FILTER2])=NaN;


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



%% save the pre_ICARTT file
TAKEOFF2=find(AC.PALTF>200);%take only data after takeoff

preICARTT.CO_PPBV=AC.CO_PPB(TAKEOFF2);
preICARTT.PALTF=AC.PALTF(TAKEOFF2);
preICARTT.TIME=AC.TIME(TAKEOFF2);
preICARTT.DOY=AC.DOY(TAKEOFF2);
preICARTT.CO2_dry_ppm=PICARRO_CALIBRATED.CO2_dry_ppm(TAKEOFF2);
preICARTT.CH4_dry_ppm=PICARRO_CALIBRATED.CH4_dry_ppm(TAKEOFF2);
preICARTT.H2O=PICARRO_interp_NANs.H2O(TAKEOFF2);




filename=['RF', flight, 'preICARTT_data.mat'];

% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08\
save(filename,['preICARTT'])
save(['RF', flight, 'quicklook.mat']);
