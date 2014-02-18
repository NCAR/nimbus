%% CONTRAST combine CO and PICARRO DATA
%%info: RF4 (2014,01,19); RF5 (2014,01,21);
clc
clear all 
close all
flight = '11';
adBefore = 44;
adAfter = 44.26;

set(0, 'DefaultAxesFontSize',20)
set(0,'DefaultLineLineWidth',2)

picPath='/net/work/Projects/CONTRAST/PICARRO/matlab/';
rafPath = [picPath,'RF',flight];
cd(rafPath);

% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08
% cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
%% load CO DATA
load(['RF', flight, '_CO_data.mat'])

switch flight
    case '08'
        %% remove data from start and land as we turnd the pumps off then
START_LAND=find(GV.JD>32.16 & GV.JD<32.05);
TAKEOFF=find(GV.JD<32.16);%find(GV.PALTF<200);
    case '09'
START_LAND=find(GV.JD>36.002 & GV.JD<36.305);
TAKEOFF=find(GV.JD<36.002);%find(GV.PALTF<200);
takeoff_t=36.002;
land_t=36.305;
    case '11'
takeoff_t=44.00;
land_t=44.26;
START_LAND=find(GV.JD>44.00 & GV.JD<44.26)
TAKEOFF=find(GV.JD<44.26);%find(GV.PALTF<200);
end

GV.CORAW_AL=GV.CORAW_AL(START_LAND);
GV.DOY=GV.DOY(START_LAND);
GV.GGLAT=GV.GGLAT(START_LAND);
GV.INLETP_AL=GV.INLETP_AL(START_LAND);
GV.JD=GV.JD(START_LAND);
GV.PALTF=GV.PALTF(START_LAND);
GV.TIME=GV.TIME(START_LAND);


fig=figure()
plot(GV.JD,GV.CORAW_AL)
hold on
plot(GV.JD,GV.PALTF./300,'k')
legend('CO','PALT')
saveas(fig,'CORAW_JD','jpg')
saveas(fig,'CORAW_JD','fig')


% %% remove all the datapoints that are bad because of the reboot of CO necessary each hour in RF08
% reboot=find(GV.CORAW_AL==-32767);
% GV.CORAW_AL(reboot)=NaN;
% 
% reboot2=find(GV.CORAW_AL<60);
% GV.CORAW_AL(reboot2)=NaN;
% 
% AA=reboot2-[reboot2(2:end);reboot2(1)];
% BB=reboot2(find(AA<-1));%% uebergang zwischen cabin air and ambient air
% 
% 
% reboot3=[];
% for i=1:length(BB)
%     BB(i)
%     reboot3i=[BB(i)+1];%:1:BB(i)+25];
%     reboot3=[reboot3,reboot3i];
% end
% 
% GV.CORAW_AL(reboot3)=NaN;

%% load PICARRO DATA
load(['RF',flight,'_PICARRO_data'])

fig=figure()
plot(GV.JD,GV.CORAW_AL)
hold on
plot(PICARRO.JULIAN_DAYS,PICARRO.CO2_dry,'r')
legend('CO','CO2')
saveas(fig,'CO_CO2','jpg')
saveas(fig,'CO_CO2','fig')

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



fig=figure()
plot(GV.JD,GV.CORAW_AL)
hold on
plot(GV.JD,PICARRO_interp.CO2_dry,'r')
legend('CO','CO2')
plot(GV.JD,GV.PALTF./300,'k')
saveas(fig,'CO_CO2_interp','jpg')
saveas(fig,'CO_CO2_interp','fig')




% figure()
% plot(GV.PALTF)
% hold on
% plot(GV.PALTF(TAKEOFF),'rx')
%% remove all the data where we have backflow (where CO2_dry>420)

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
% BAD=BAD(find(BAD<length(GV.JD)));
% 
% figure()
% plot(GV.JD,GV.CORAW_AL,'bo')
% hold on
% plot(GV.JD,PICARRO_interp.CO2_dry,'ro')
% % legend('CO','CO2')
% hold on
% plot(GV.JD(BAD),GV.CORAW_AL(BAD),'kx')
% hold on
% plot(GV.JD(BAD),PICARRO_interp.CO2_dry(BAD),'kx')
% legend('CO','CO2')







%% pressure drop in RF05 remove that data
BAD_PRESSURE=find(PICARRO_interp.CavityPressure<151.3 | PICARRO_interp.CavityPressure>151.7);

fig=figure()
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CavityPressure,'ro')
hold on
plot(PICARRO_interp.JULIAN_DAYS(BAD_PRESSURE),PICARRO_interp.CavityPressure(BAD_PRESSURE),'bx')
saveas(fig,'CavityPressure','jpg')
saveas(fig,'CavityPressure','fig')

%% remove all bad data


all_fieldname=fieldnames(PICARRO_interp);

for i=1:length(fieldnames(PICARRO_interp))
fieldname=char(all_fieldname(i));
PICARRO_interp_i=PICARRO_interp.(fieldname);
if strcmp(fieldname,'JULIAN_DAYS')==1
PICARRO_interp_NANs.(fieldname)=PICARRO_interp.JULIAN_DAYS; 
else
% PICARRO_interp_i(BAD)=NaN;
PICARRO_interp_i(BAD_PRESSURE)=NaN;
% PICARRO_interp_i(BAD_STRANGE)=NaN;
PICARRO_interp_NANs.(fieldname)=PICARRO_interp_i;
end
end



%% find calibration times CO2 between 382 and 390 and CO between 168 and 178 and methane between 1.75 and 1.78

CO2_CAL=find(PICARRO_interp.CO2_dry>382 & PICARRO_interp.CO2_dry<390);
CH4_CAL=find(PICARRO_interp.CH4_dry>1.75 & PICARRO_interp.CH4_dry<1.78);
CO_CAL=find(GV.CORAW_AL>93 & GV.CORAW_AL<105);

PICARRO_CAL=intersect(CO2_CAL,CH4_CAL);

ALL_CAL=intersect(PICARRO_CAL,CO_CAL);


fig=figure()
subplot(3,1,1)
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CO2_dry,'gx')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),PICARRO_interp.CO2_dry(ALL_CAL),'kx')
%legend('CO2 dry','CO2 dry CAL')
subplot(3,1,2)
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CH4_dry,'b')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),PICARRO_interp.CH4_dry(ALL_CAL),'kx')
%legend('CH4 dry','CH4 dry CAL')
subplot(3,1,3)
plot(PICARRO_interp.JULIAN_DAYS,GV.CORAW_AL,'r')
hold on
plot(PICARRO_interp.JULIAN_DAYS(ALL_CAL),GV.CORAW_AL(ALL_CAL),'kx')
%legend('CO','CO CAL')
title('CALIBRATIONS')
saveas(fig,'cals','jpg')
saveas(fig,'cals','fig')

fig=figure()
plot(GV.JD,GV.CORAW_AL,'r')
hold on
plot(GV.JD(CO_CAL),GV.CORAW_AL(CO_CAL),'kx')
legend('CO','CO CAL')
saveas(fig,'CO_cal','jpg')
saveas(fig,'CO_cal','fig')

%% find all call times (and delete those points that are not real cals!

ZZ=[find(PICARRO_interp.JULIAN_DAYS<takeoff_t);find(PICARRO_interp.JULIAN_DAYS>land_t & PICARRO_interp.JULIAN_DAYS<takeoff_t)];
% ZZ=[find(PICARRO_interp.JULIAN_DAYS<19.027);find(PICARRO_interp.JULIAN_DAYS>19.195 & PICARRO_interp.JULIAN_DAYS<19.2)];
% ZZZ=setdiff(ALL_CAL,ZZ);
CAL=setdiff(ALL_CAL,ZZ);

fig=figure()
subplot(3,1,1)
plot(PICARRO_interp.JULIAN_DAYS(CAL),GV.CORAW_AL(CAL),'kx')
% xlim([19.059 19.312])
grid on
legend('CO')
subplot(3,1,2)
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2_dry(CAL),'rx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2(CAL),'bx')
% xlim([19.059 19.312])
grid on
legend('CO2 dry','CO2')
subplot(3,1,3)
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CH4_dry(CAL),'rx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CH4(CAL),'bx')
% xlim([19.059 19.312])
grid on
legend('CH4 dry','CH4')
title('CALIBRATIONS')
saveas(fig,'CH4_cal','jpg')
saveas(fig,'CH4_cal','fig')

%% find CO zeros

CO_BG=find(GV.CORAW_AL>-5 & GV.CORAW_AL<5);

fig=figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_BG),GV.CORAW_AL(CO_BG),'kx')
legend('CO')
title('CO BG MEASUREMENTS')
saveas(fig,'CO_bg','jpg')
saveas(fig,'CO_bg','fig')


%% CO calibration

%% rough calculation of sensitivity (mean of all call and zero values)
mean_CO_BG=mean(GV.CORAW_AL(CO_BG));
mean_CO_CAL=mean(GV.CORAW_AL(CO_CAL));




fig=figure()
plot(GV.JD,GV.CORAW_AL,'bx')
hold on
plot(GV.JD,(GV.CORAW_AL-mean_CO_BG).*141./(mean_CO_CAL-mean_CO_BG),'ro')
saveas(fig,'mean_CO_bg','jpg')
saveas(fig,'mean_CO_bg','fig')



%% better calibration using a linear interpolation between the different cals

%% Linear interpolation of calibration
% find each calibration separately
CO_CAL_2=[CO_CAL(2:end);CO_CAL(1)];

indexi=[1;find((CO_CAL-CO_CAL_2)<-100);length(CO_CAL)];

CO_CAL_individual=[];
CO_CAL_times=[];

for iii=2:(length(indexi)-2)
    iii
    CO_CAL_i=mean(GV.CORAW_AL(CO_CAL(indexi(iii)+3):CO_CAL(indexi(iii+1)-3)));
    CO_CAL_time_i=median(GV.JD(CO_CAL(indexi(iii)+3):CO_CAL(indexi(iii+1)-3)));
    
    CO_CAL_individual=[CO_CAL_individual,CO_CAL_i];
    CO_CAL_times=[CO_CAL_times,CO_CAL_time_i];
end


fig=figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_CAL),GV.CORAW_AL(CO_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')
saveas(fig,'CO_CAL_ind','jpg')
saveas(fig,'CO_CAL_ind','fig')

% clc;
% close all;
% clear all;

% Stützstellen
xs = CO_CAL_times;%[0, 35, 75, 110, 150];
ys = CO_CAL_individual;%[20, 0, -50, 40, 20];
timestep= GV.TIME(2)-GV.TIME(1);
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
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_CAL),GV.CORAW_AL(CO_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')
plot(xi, CO_CAL_interp, 'g')
saveas(fig,'CO_CAL_interp','jpg')
saveas(fig,'CO_CAL_interp','fig')

%add the first cal to the times before the first cal and the last cal
%number to after the last cal

before_first_cal=find(GV.JD<xi(1));
during_the_cals=find(GV.JD>=xi(1) & GV.JD<=xi(end));%<=
after_last_cal=find(GV.JD>xi(end));


CO_CAL_ALL=zeros(1,length(GV.JD))*NaN;

CO_CAL_ALL(before_first_cal)=ones(1,length(before_first_cal))*CO_CAL_individual(1);
CO_CAL_ALL(during_the_cals)=CO_CAL_interp(1:end);
CO_CAL_ALL(after_last_cal)=ones(1,length(after_last_cal))*CO_CAL_individual(end);

fig=figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_CAL),GV.CORAW_AL(CO_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')
plot(GV.JD, CO_CAL_ALL, 'g')
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
    CO_BG_i=mean(GV.CORAW_AL(CO_BG(indexi(iii)+3):CO_BG(indexi(iii+1)-3)));
    CO_BG_time_i=median(GV.JD(CO_BG(indexi(iii)+3):CO_BG(indexi(iii+1)-3)));
    
    CO_BG_individual=[CO_BG_individual,CO_BG_i];
    CO_BG_times=[CO_BG_times,CO_BG_time_i];
end


fig=figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_BG),GV.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
saveas(fig,'CO_BG_ind','jpg')
saveas(fig,'CO_BG_ind','fig')

% clc;
% close all;
% clear all;

% Stützstellen
xs = CO_BG_times;%[0, 35, 75, 110, 150];
ys = CO_BG_individual;%[20, 0, -50, 40, 20];
timestep= GV.TIME(2)-GV.TIME(1);
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
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_BG),GV.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
plot(xi, CO_BG_interp, 'g')
saveas(fig,'CO_BG_int','jpg')
saveas(fig,'CO_BG_int','fig')

%add the first cal to the times before the first cal and the last cal
%number to after the last cal

before_first_BG=find(GV.JD<xi(1));
during_the_BGs=find(GV.JD>=xi(1) & GV.JD<=xi(end));
after_last_BG=find(GV.JD>xi(end));


CO_BG_ALL=zeros(1,length(GV.JD))*NaN;

CO_BG_ALL(before_first_BG)=ones(1,length(before_first_BG))*CO_BG_individual(1);
CO_BG_ALL(during_the_BGs)=CO_BG_interp;
CO_BG_ALL(after_last_BG)=ones(1,length(after_last_BG))*CO_BG_individual(end);

fig=figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_BG),GV.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
plot(GV.JD, CO_BG_ALL, 'g')
saveas(fig,'CO_BG_ALL','jpg')
saveas(fig,'CO_BG_ALL','fig')
%%


GV.CO_PPB=(GV.CORAW_AL-CO_BG_ALL').*140./(CO_CAL_ALL'-CO_BG_ALL');

fig=figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD,GV.CO_PPB,'rx')
legend('CORAW','CO PPB')
saveas(fig,'CO_PPB','jpg')
saveas(fig,'CO_PPB','fig')

length(GV.JD)
length(GV.CO_PPB)

GV.CO_PPB(CAL)=NaN;
GV.CO_PPB(CO_BG)=NaN;
% GV.CO_PPB(BAD)=NaN;
GV.CO_PPB(BAD_PRESSURE)=NaN;
%GV.CO_PPB(TAKEOFF)=NaN;

length(GV.JD)
length(GV.CO_PPB)

fig=figure()
plot(GV.JD,GV.CO_PPB,'bx')
legend('CO PPBV')
saveas(fig,'CO_PPBV','jpg')
saveas(fig,'CO_PPBV','fig')


%% CO2 Calibration

% FILTER=[find(GV.JD>32.184 & GV.JD<32.1844);find(GV.JD>32.4341 & GV.JD<32.4358);find(GV.JD>32.4760 & GV.JD<32.4762);find(GV.JD>32.2263 & GV.JD<32.2264);find(GV.JD>32.3935 & GV.JD<32.3940);find(GV.JD>32.4765 & GV.JD<32.4777)]; % Data that was not filtered by any other reason but clearly is bg or cal data.
FILTER=[find(GV.JD<36);find(GV.JD>36.082&GV.JD<36.0837);find(GV.JD>36.2468 & GV.JD<36.2469);find(GV.JD>36.288 & GV.JD<36.2897)];
FILTER2=find(GV.CO_PPB<10);

PICARRO_CALIBRATED.CO2_dry_ppm=PICARRO_interp_NANs.CO2_dry+1.38;
PICARRO_CALIBRATED.CO2_dry_ppm([ALL_CAL;CO_BG;FILTER;FILTER2])=NaN;

PICARRO_CALIBRATED.CH4_dry_ppm=PICARRO_interp_NANs.CH4_dry+0.0339;
PICARRO_CALIBRATED.CH4_dry_ppm([ALL_CAL;CO_BG;FILTER;FILTER2])=NaN;


GV.CO_PPB([FILTER;FILTER2])=NaN;


fig=figure()
subplot(3,1,1)
plot(GV.JD,GV.CO_PPB,'bx')
% hold on
% plot(GV.JD,GV.PALTF./3/10,'k')
legend('CO [ppbv]')
% xlim([19.059 19.312])
ylim([60 160])
grid on
subplot(3,1,2)
plot(GV.JD,PICARRO_CALIBRATED.CO2_dry_ppm,'rx')
legend('CO2 [ppmv]')
% xlim([19.059 19.312])
ylim([394 420])
grid on
subplot(3,1,3)
plot(GV.JD,PICARRO_CALIBRATED.CH4_dry_ppm,'gx')
legend('CH4 [ppmv]')
grid on
% xlim([19.059 19.312])
ylim([1.79 1.88])
saveas(fig,'final','jpg')
saveas(fig,'final','fig')



%% save the pre_ICARTT file
TAKEOFF2=find(GV.PALTF>200);%take only data after takeoff

preICARTT.CO_PPBV=GV.CO_PPB(TAKEOFF2);
preICARTT.PALTF=GV.PALTF(TAKEOFF2);
preICARTT.TIME=GV.TIME(TAKEOFF2);
preICARTT.DOY=GV.DOY(TAKEOFF2);
preICARTT.CO2_dry_ppm=PICARRO_CALIBRATED.CO2_dry_ppm(TAKEOFF2);
preICARTT.CH4_dry_ppm=PICARRO_CALIBRATED.CH4_dry_ppm(TAKEOFF2);
preICARTT.H2O=PICARRO_interp_NANs.H2O(TAKEOFF2);




filename=['RF', flight, 'preICARTT_data.mat'];

% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08\
save(filename,['preICARTT'])
save(['RF', flight, 'quicklook.mat']);








