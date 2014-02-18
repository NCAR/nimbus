%% combine CO and PICARRO DATA
%%info: RF4 (2014,01,19); RF5 (2014,01,21);
clc
clear all 
close all

set(0, 'DefaultAxesFontSize',20)
set(0,'DefaultLineLineWidth',2)

cd C:\Users\kaser\Documents\CONTRAST\matlab\RF10\resultsRF10
% cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
%% load CO DATA
load('RF10_CO_data.mat')


figure()
plot(GV.JD,GV.CORAW_AL)
hold on
plot(GV.JD,GV.PALTF./300,'k')

%% remove data from start and land as we turnd the pumps off then
START_LAND=find(GV.JD>39.015 & GV.JD<39.30);
TAKEOFF=find(GV.JD<39.31);%find(GV.PALTF<200);


GV.CORAW_AL=GV.CORAW_AL(START_LAND);
GV.DOY=GV.DOY(START_LAND);
GV.GGLAT=GV.GGLAT(START_LAND);
GV.INLETP_AL=GV.INLETP_AL(START_LAND);
GV.JD=GV.JD(START_LAND);
GV.PALTF=GV.PALTF(START_LAND);
GV.TIME=GV.TIME(START_LAND);

figure()
plot(GV.JD,GV.CORAW_AL)
% hold on
% plot(GV.PALTF(TAKEOFF),'rx')
%%


% %% remove all the datapoints that are bad because of the reboot of CO necessary each hour in RF10
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
load('RF10_PICARRO_data')

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
plot(GV.JD,GV.PALTF./300,'k')




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
% PICARRO_interp_i(BAD)=NaN;
PICARRO_interp_i(BAD_PRESSURE)=NaN;
% PICARRO_interp_i(BAD_STRANGE)=NaN;
PICARRO_interp_NANs.(fieldname)=PICARRO_interp_i;
end
end



%% find calibration times CO2 between 382 and 390 and CO between 168 and 178 and methane between 1.75 and 1.78

CO2_CAL=find(PICARRO_interp.CO2_dry>382 & PICARRO_interp.CO2_dry<390);
CH4_CAL=find(PICARRO_interp.CH4_dry>1.75 & PICARRO_interp.CH4_dry<1.78);
CO_CAL=find(GV.CORAW_AL>85 & GV.CORAW_AL<105);

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


figure()
plot(GV.JD,GV.CORAW_AL,'r')
hold on
plot(GV.JD(ALL_CAL),GV.CORAW_AL(ALL_CAL),'kx')
legend('CO','CO CAL')

%% find all call times (and delete those points that are not real cals!

ZZ=[find(PICARRO_interp.JULIAN_DAYS<39)];%find(PICARRO_interp.JULIAN_DAYS>19.195 & PICARRO_interp.JULIAN_DAYS<19.2)];
% ZZZ=setdiff(ALL_CAL,ZZ);
CAL=setdiff(ALL_CAL,ZZ);

figure()
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
set(gcf, 'Color', 'white'); % white bckgr
screen_size = get(0, 'ScreenSize');
set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
cd C:\Users\kaser\Documents\CONTRAST\matlab\RF10\resultsRF10\
export_fig( gcf, ...      % figure handle
    ['CO_CO2_CH4_CALIBRATIONS'],... % name of output file without extension
    '-painters', ...      % renderer
    '-pdf', ...           % file format
    '-r300' );             % resolution in dpi

%% find CO zeros

CO_BG=find(GV.CORAW_AL>-5 & GV.CORAW_AL<5);

figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_BG),GV.CORAW_AL(CO_BG),'kx')
legend('CO')
title('CO BG MEASUREMENTS')


%% CO calibration

%% rough calculation of sensitivity (mean of all call and zero values)
mean_CO_BG=mean(GV.CORAW_AL(CO_BG));
mean_CO_CAL=mean(GV.CORAW_AL(CO_CAL));




figure()
plot(GV.JD,GV.CORAW_AL,'bx')
hold on
plot(GV.JD,(GV.CORAW_AL-mean_CO_BG).*141./(mean_CO_CAL-mean_CO_BG),'ro')



%% better calibration using a linear interpolation between the different cals

%% Linear interpolation of calibration
% find each calibration separately
CO_CAL_2=[ALL_CAL(2:end);ALL_CAL(1)];

indexi=[1;find((ALL_CAL-CO_CAL_2)<-100);length(ALL_CAL)];

CO_CAL_individual=[];
CO_CAL_times=[];

for iii=1:(length(indexi)-1)
    iii
    CO_CAL_i=mean(GV.CORAW_AL(ALL_CAL(indexi(iii)+3):ALL_CAL(indexi(iii+1)-3)));
    CO_CAL_time_i=median(GV.JD(ALL_CAL(indexi(iii)+3):ALL_CAL(indexi(iii+1)-3)));
    
    CO_CAL_individual=[CO_CAL_individual,CO_CAL_i];
    CO_CAL_times=[CO_CAL_times,CO_CAL_time_i];
end


figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(ALL_CAL),GV.CORAW_AL(ALL_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')

% clc;
% close all;
% clear all;

% Stützstellen
xs = CO_CAL_times;%[0, 35, 75, 110, 150];
ys = CO_CAL_individual;%[20, 0, -50, 40, 20];
timestep= GV.JD(2)-GV.JD(1);

xs=xs(~isnan(ys));
ys=ys(~isnan(ys));
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

figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(ALL_CAL),GV.CORAW_AL(ALL_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')
plot(xi, CO_CAL_interp, 'g')

%add the first cal to the times before the first cal and the last cal
%number to after the last cal

before_first_cal=find(GV.JD<xi(1));
during_the_cals=find(GV.JD>=xi(1) & GV.JD<=xi(end));%<=
after_last_cal=find(GV.JD>xi(end));


CO_CAL_ALL=zeros(1,length(GV.JD))*NaN;

CO_CAL_ALL(before_first_cal)=ones(1,length(before_first_cal))*CO_CAL_individual(1);
CO_CAL_ALL(during_the_cals)=CO_CAL_interp(1:(end-1));
CO_CAL_ALL(after_last_cal)=ones(1,length(after_last_cal))*CO_CAL_individual(end);

figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(ALL_CAL),GV.CORAW_AL(ALL_CAL),'rx')
hold on
plot(CO_CAL_times,CO_CAL_individual,'ko')
plot(GV.JD, CO_CAL_ALL, 'g')

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


figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_BG),GV.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')

% clc;
% close all;
% clear all;

% Stützstellen
xs = CO_BG_times;%[0, 35, 75, 110, 150];
ys = CO_BG_individual;%[20, 0, -50, 40, 20];
timestep= GV.JD(2)-GV.JD(1);
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

figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_BG),GV.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
plot(xi, CO_BG_interp, 'g')

%add the first cal to the times before the first cal and the last cal
%number to after the last cal

before_first_BG=find(GV.JD<xi(1));
during_the_BGs=find(GV.JD>=xi(1) & GV.JD<=xi(end));
after_last_BG=find(GV.JD>xi(end));


CO_BG_ALL=zeros(1,length(GV.JD))*NaN;

CO_BG_ALL(before_first_BG)=ones(1,length(before_first_BG))*CO_BG_individual(1);
CO_BG_ALL(during_the_BGs)=CO_BG_interp(1:(end-1));
CO_BG_ALL(after_last_BG)=ones(1,length(after_last_BG))*CO_BG_individual(end);

figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD(CO_BG),GV.CORAW_AL(CO_BG),'rx')
hold on
plot(CO_BG_times,CO_BG_individual,'ko')
plot(GV.JD, CO_BG_ALL, 'g')
%%


GV.CO_PPB=(GV.CORAW_AL-CO_BG_ALL').*141./(CO_CAL_ALL'-CO_BG_ALL');

figure()
plot(GV.JD,GV.CORAW_AL,'b')
hold on
plot(GV.JD,GV.CO_PPB,'rx')

GV.CO_PPB(CAL)=NaN;
GV.CO_PPB(CO_BG)=NaN;
% GV.CO_PPB(BAD)=NaN;
GV.CO_PPB(BAD_PRESSURE)=NaN;
% GV.CO_PPB(TAKEOFF)=NaN;





figure()
plot(GV.JD,GV.CO_PPB,'bx')
legend('CO PPBV')


%% CO2 Calibration

FILTER=[find(GV.JD>39.099 & GV.JD<39.1);find(GV.JD>39.2666 & GV.JD<39.2684)];%find(GV.JD>32.4760 & GV.JD<32.4762);find(GV.JD>32.2263 & GV.JD<32.2264);find(GV.JD>32.3935 & GV.JD<32.3940);find(GV.JD>32.4765 & GV.JD<32.4777)]; % Data that was not filtered by any other reason but clearly is bg or cal data.

FILTER2=find(GV.CO_PPB<65);

PICARRO_CALIBRATED.CO2_dry_ppm=PICARRO_interp_NANs.CO2_dry+1.38;
PICARRO_CALIBRATED.CO2_dry_ppm([ALL_CAL;CO_BG;FILTER;FILTER2])=NaN;

PICARRO_CALIBRATED.CH4_dry_ppm=PICARRO_interp_NANs.CH4_dry+0.0339;
PICARRO_CALIBRATED.CH4_dry_ppm([ALL_CAL;CO_BG;FILTER;FILTER2])=NaN;


GV.CO_PPB([FILTER;FILTER2])=NaN;


figure()
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
ylim([394 400])
grid on
subplot(3,1,3)
plot(GV.JD,PICARRO_CALIBRATED.CH4_dry_ppm,'gx')
legend('CH4 [ppmv]')
grid on
% xlim([19.059 19.312])
ylim([1.79 1.88])



%% save the pre_ICARTT file
TAKEOFF2=find(GV.PALTF>200);%take only data after takeoff

RF10_forICARTT.CO_PPBV=GV.CO_PPB(TAKEOFF2);
RF10_forICARTT.PALTF=GV.PALTF(TAKEOFF2);
RF10_forICARTT.TIME=GV.TIME(TAKEOFF2);
RF10_forICARTT.DOY=GV.DOY(TAKEOFF2);
RF10_forICARTT.CO2_dry_ppm=PICARRO_CALIBRATED.CO2_dry_ppm(TAKEOFF2);
RF10_forICARTT.CH4_dry_ppm=PICARRO_CALIBRATED.CH4_dry_ppm(TAKEOFF2);




filename=['RF10_forICARTT_data.mat'];


cd C:\Users\kaser\Documents\CONTRAST\matlab\RF10\resultsRF10\
save(filename,'RF10_forICARTT')








