%% combine CO and PICARRO DATA
%%info: RF4 (2014,01,19); RF5 (2014,01,21);
clc
clear all 
close all

set(0, 'DefaultAxesFontSize',20)
set(0,'DefaultLineLineWidth',2)

cd C:\Users\kaser\Documents\CONTRAST\matlab\RF04\resultsRF04
% cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
%% load CO DATA
load('RF04_CO_data.mat')


%% remove all the datapoints that are bad because of the reboot of CO necessary each hour in RF04
reboot=find(GV.CORAW_AL==-32767);
GV.CORAW_AL(reboot)=NaN;

reboot2=find(GV.CORAW_AL<60);
GV.CORAW_AL(reboot2)=NaN;

AA=reboot2-[reboot2(2:end);reboot2(1)];
BB=reboot2(find(AA<-1));%% uebergang zwischen cabin air and ambient air


reboot3=[];
for i=1:length(BB)
    BB(i)
    reboot3i=[BB(i)+1];%:1:BB(i)+25];
    reboot3=[reboot3,reboot3i];
end

GV.CORAW_AL(reboot3)=NaN;

%% load PICARRO DATA
load('RF04_PICARRO_data')

figure()
plot(GV.JD,GV.CORAW_AL)
hold on
plot(PICARRO.JULIAN_DAYS,PICARRO.CO2_dry,'r')
legend('CO','CO2')

%% interpolate all the PICARRO DATA to the CO time stamp
all_fieldname=fieldnames(PICARRO);

for i=1:length(fieldnames(PICARRO))
fieldname=char(all_fieldname(i));
if strcmp(fieldname,'timestamp')==1
    
else
before_interp=PICARRO.(fieldname);
after_interp=interp1(PICARRO.JULIAN_DAYS,before_interp,GV.JD);
PICARRO_interp.(fieldname)=after_interp;
end
end



figure()
plot(GV.JD,GV.CORAW_AL)
hold on
plot(GV.JD,PICARRO_interp.CO2_dry,'r')
legend('CO','CO2')



%% remove data from start and land as we turnd the pumps off then
START_LAND=find(GV.JD<18.965 | GV.JD>19.31);
TAKEOFF=find(GV.JD<19.059);%find(GV.PALTF<200);

% figure()
% plot(GV.PALTF)
% hold on
% plot(GV.PALTF(TAKEOFF),'rx')
%% remove all the data where we have backflow (where CO2_dry>420)

BAD1=find(PICARRO_interp.CO2_dry>420);
% also remove 3 datapoints before and 10 datapoints after
A=BAD1-[BAD1(2:end);BAD1(1)];
B=BAD1(find(A<-1));%% uebergang zwischen cabin air and ambient air
BAD2=[];
for i=1:(length(B)-1)
    B(i)
    BAD2i=[B(i):1:B(i)+50];
    BAD2=[BAD2,BAD2i];
end

C=BAD1-[BAD1(end);BAD1(1:end-1)];



D=BAD1(find(C>1));%% uebergang zwischen cabin air and ambient air
BAD3=[];
for i=1:length(D)
    D(i)
    BAD3i=[D(i)-10:1:D(i)];
    BAD3=[BAD3,BAD3i];
end



BAD=[START_LAND;TAKEOFF;BAD1;BAD2';BAD3'];
% 

BAD=BAD(find(BAD<length(GV.JD)));

figure()
plot(GV.JD,GV.CORAW_AL,'bo')
hold on
plot(GV.JD,PICARRO_interp.CO2_dry,'ro')
% legend('CO','CO2')
hold on
plot(GV.JD(BAD),GV.CORAW_AL(BAD),'kx')
hold on
plot(GV.JD(BAD),PICARRO_interp.CO2_dry(BAD),'kx')
legend('CO','CO2')







%% pressure drop in RF5 remove that data
BAD_PRESSURE=find(PICARRO_interp.CavityPressure<151.3 | PICARRO_interp.CavityPressure>151.7);

figure()
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CavityPressure,'ro')
hold on
plot(PICARRO_interp.JULIAN_DAYS(BAD_PRESSURE),PICARRO_interp.CavityPressure(BAD_PRESSURE),'bx')


%% remove all bad data


all_fieldname=fieldnames(PICARRO_interp);

for i=1:length(fieldnames(PICARRO_interp))
fieldname=char(all_fieldname(i));
PICARRO_interp_i=PICARRO_interp.(fieldname);
if strcmp(fieldname,'JULIAN_DAYS')==1
PICARRO_interp_NANs.(fieldname)=PICARRO_interp.JULIAN_DAYS; 
else
PICARRO_interp_i(BAD)=NaN;
PICARRO_interp_i(BAD_PRESSURE)=NaN;
% PICARRO_interp_i(BAD_STRANGE)=NaN;
PICARRO_interp_NANs.(fieldname)=PICARRO_interp_i;
end
end



%% find calibration times CO2 between 382 and 390 and CO between 168 and 178 and methane between 1.75 and 1.78

CO2_CAL=find(PICARRO_interp.CO2_dry>382 & PICARRO_interp.CO2_dry<390);
CH4_CAL=find(PICARRO_interp.CH4_dry>1.75 & PICARRO_interp.CH4_dry<1.78);
CO_CAL=find(GV.CORAW_AL>155 & GV.CORAW_AL<178);

PICARRO_CAL=intersect(CO2_CAL,CH4_CAL);

ALL_CAL=intersect(PICARRO_CAL,CO_CAL);

figure()
subplot(3,1,1)
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CO2_dry,'gx')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),PICARRO_interp.CO2_dry(ALL_CAL),'kx')
legend('CO2 dry','CO2 dry CAL')
subplot(3,1,2)
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CH4_dry,'b')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),PICARRO_interp.CH4_dry(ALL_CAL),'kx')
legend('CH4 dry','CH4 dry CAL')
subplot(3,1,3)
plot(PICARRO_interp.JULIAN_DAYS,GV.CORAW_AL,'r')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),GV.CORAW_AL(ALL_CAL),'kx')
legend('CO','CO CAL')
title('CALIBRATIONS')


%% find all call times (and delete those points that are not real cals!

ZZ=[find(PICARRO_interp.JULIAN_DAYS<19.027);find(PICARRO_interp.JULIAN_DAYS>19.195 & PICARRO_interp.JULIAN_DAYS<19.2)];
% ZZZ=setdiff(ALL_CAL,ZZ);
CAL=setdiff(ALL_CAL,ZZ);

figure()
subplot(3,1,1)
plot(PICARRO_interp.JULIAN_DAYS(CAL),GV.CORAW_AL(CAL),'kx')
xlim([19.059 19.312])
grid on
legend('CO')
subplot(3,1,2)
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2_dry(CAL),'rx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2(CAL),'bx')
xlim([19.059 19.312])
grid on
legend('CO2 dry','CO2')
subplot(3,1,3)
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CH4_dry(CAL),'rx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CH4(CAL),'bx')
xlim([19.059 19.312])
grid on
legend('CH4 dry','CH4')

title('CALIBRATIONS')

%% find CO zeros

CO_BG=find(GV.CORAW_AL>60 & GV.CORAW_AL<80);

figure()
plot(PICARRO_interp.JULIAN_DAYS(CO_BG),GV.CORAW_AL(CO_BG),'kx')
legend('CO')
title('CO BG MEASUREMENTS')


%% CO calibration


mean_CO_BG=mean(GV.CORAW_AL(CO_BG));
mean_CO_CAL=mean(GV.CORAW_AL(CAL));

figure()
plot(GV.JD,GV.CORAW_AL,'bx')
hold on
plot(GV.JD,(GV.CORAW_AL-mean_CO_BG).*146./(mean_CO_CAL-mean_CO_BG),'ro')

GV.CO_PPB=(GV.CORAW_AL-mean_CO_BG).*146./(mean_CO_CAL-mean_CO_BG);

GV.CO_PPB(CAL)=NaN;
GV.CO_PPB(CO_BG)=NaN;
GV.CO_PPB(BAD)=NaN;
GV.CO_PPB(BAD_PRESSURE)=NaN;
GV.CO_PPB(TAKEOFF)=NaN;

STRANGE_CO=[find(GV.JD>19.1962 & GV.JD<19.1965);find(GV.JD>19.283 & GV.JD<19.286);find(GV.JD>19.28 & GV.JD<19.2812)];
GV.CO_PPB(STRANGE_CO)=NaN;



figure()
plot(GV.JD,GV.CO_PPB,'bx')
legend('CO PPBV')


%% CO2 Calibration

FILTER=[find(GV.JD>19.1963 & GV.JD<19.1965);find(GV.JD>19.2394 & GV.JD<19.241);find(GV.JD>19.283 & GV.JD<19.286);find(GV.JD>19.28 & GV.JD<19.2812)]; % Data that was not filtered by any other reason but clearly is bg or cal data.

PICARRO_CALIBRATED.CO2_dry_ppm=PICARRO_interp_NANs.CO2_dry+1.38;
PICARRO_CALIBRATED.CO2_dry_ppm([ALL_CAL;CO_BG;FILTER;BAD;23949])=NaN;

PICARRO_CALIBRATED.CH4_dry_ppm=PICARRO_interp_NANs.CH4_dry+0.0339;
PICARRO_CALIBRATED.CH4_dry_ppm([ALL_CAL;CO_BG;FILTER;BAD;23949])=NaN;


figure()
subplot(3,1,1)
plot(GV.JD,GV.CO_PPB,'bx')
% hold on
% plot(GV.JD,GV.PALTF./3/10,'k')
legend('CO [ppbv]')
xlim([19.059 19.312])
ylim([60 160])
grid on
subplot(3,1,2)
plot(GV.JD,PICARRO_CALIBRATED.CO2_dry_ppm,'rx')
legend('CO2 [ppmv]')
xlim([19.059 19.312])
ylim([394 400])
grid on
subplot(3,1,3)
plot(GV.JD,PICARRO_CALIBRATED.CH4_dry_ppm,'gx')
legend('CH4 [ppmv]')
grid on
xlim([19.059 19.312])
ylim([1.79 1.88])



%% save the pre_ICARTT file
TAKEOFF2=find(GV.PALTF>200);%take only data after takeoff

RF04_forICARTT.CO_PPBV=GV.CO_PPB(TAKEOFF2);
RF04_forICARTT.PALTF=GV.PALTF(TAKEOFF2);
RF04_forICARTT.TIME=GV.TIME(TAKEOFF2);
RF04_forICARTT.DOY=GV.DOY(TAKEOFF2);
RF04_forICARTT.CO2_dry_ppm=PICARRO_CALIBRATED.CO2_dry_ppm(TAKEOFF2);
RF04_forICARTT.CH4_dry_ppm=PICARRO_CALIBRATED.CH4_dry_ppm(TAKEOFF2);




filename=['RF04_forICARTT_data.mat'];


cd C:\Users\kaser\Documents\CONTRAST\matlab\RF04\resultsRF04\
save(filename,'RF04_forICARTT')








