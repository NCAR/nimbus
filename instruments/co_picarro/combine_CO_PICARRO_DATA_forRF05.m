%% combine CO and PICARRO DATA
%%info: RF4 (2014,01,19); RF5 (2014,01,21);
clc
clear all 
close all

set(0, 'DefaultAxesFontSize',20)
set(0,'DefaultLineLineWidth',2)

cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
%% load CO DATA
load('RF05_CO_data.mat')


%% load PICARRO DATA
load('RF05_PICARRO_data')

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

%% find data where CO2_dry>450 (there was a flow back of cabin air. Data of
%% PICARRO needs to be removed. CO Data needs to be flagged.


BAD1=find(PICARRO_interp.CO2_dry>410);
% also remove 3 datapoints before and 10 datapoints after
A=BAD1-[BAD1(2:end);BAD1(1)];
B=BAD1(find(A<-1));%% uebergang zwischen cabin air and ambient air
BAD2=[];
for i=1:length(B)
    B(i)
    BAD2i=[B(i):1:B(i)+100];
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



BAD=[BAD1;BAD2';BAD3'];
% 



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

%% strange data for RF05
BAD_STRANGE1=find(PICARRO_interp.JULIAN_DAYS>22.304);
% BAD_STRANGE2=find(PICARRO_interp.JULIAN_DAYS>22.168 & PICARRO_interp.JULIAN_DAYS<22.21);

BAD_STRANGE=[BAD_STRANGE1];%;BAD_STRANGE2];
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
PICARRO_interp_i(BAD_STRANGE)=NaN;
PICARRO_interp_NANs.(fieldname)=PICARRO_interp_i;
end
end


%% CO2 calibrations!
%% find calibrations

CAL=find(PICARRO_interp_NANs.CO2_dry<392);
figure()
subplot(2,1,1)
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2_dry(CAL),'rx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2(CAL),'bx')
legend('CO2 dry','CO2')
subplot(2,1,2)
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CH4_dry(CAL),'rx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CH4(CAL),'bx')
legend('CH4 dry','CH4')
title('CALIBRATIONS')

%% enter calibration value here!
% CO2 offset CO2_dry+1.38ppm
% CH4 offset CH4_dry+0.0339ppm



figure()
subplot(2,1,1)
plot(PICARRO_interp_NANs.JULIAN_DAYS,PICARRO_interp_NANs.H2O,'bx')
subplot(2,1,2)

plot(GV.JD,GV.PALTF,'r')


%% write NaNs for cal
PICARRO_interp_NANs.CO2_dry(CAL)=NaN;

figure()
subplot(2,1,1)
plot(PICARRO_interp_NANs.JULIAN_DAYS,PICARRO_interp_NANs.CO2_dry,'bx')
subplot(2,1,2)
plot(PICARRO_interp_NANs.JULIAN_DAYS,PICARRO_interp_NANs.CH4_dry,'rx')

% figure()
% plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2(CAL),'ro')

figure()
plot(PICARRO_interp.JULIAN_DAYS,PICARRO_interp.CO2_dry,'ro')
hold on

figure()
plot(PICARRO_interp_NANs.JULIAN_DAYS,PICARRO_interp_NANs.CO2_dry,'bx')
hold on
plot(PICARRO_interp_NANs.JULIAN_DAYS(CAL),PICARRO_interp_NANs.CO2_dry(CAL),'kx')

PICARRO_CALIBRATED.CO2_dry_ppm=PICARRO_interp_NANs.CO2_dry+1.38;
PICARRO_CALIBRATED.CH4_dry_ppm=PICARRO_interp_NANs.CH4_dry+0.0339;
PICARRO_CALIBRATED.JD=PICARRO_interp_NANs.JULIAN_DAYS;
B=find(PICARRO_CALIBRATED.CH4_dry_ppm<1.798);

PICARRO_CALIBRATED.CH4_dry_ppm(B)=NaN;

filename=['RF05_PICARRO_calibrated_data.mat'];

cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
save(filename,'PICARRO_CALIBRATED')
%% CO DATA

%% find CO BG DATA
% GV.CORAW_AL(BAD_PRESSURE)=NaN;
GV.CORAW_AL(BAD_STRANGE1)=NaN;

CO_BG_i=find(GV.CORAW_AL<90);
ZZ=[CO_BG_i(end);CO_BG_i(1:end-1)]-CO_BG_i;

CO_BG_i=CO_BG_i(find(ZZ==-1));
mean_CO_BG=mean(GV.CORAW_AL(CO_BG_i));
mean_CO_CAL=mean(GV.CORAW_AL(CAL));

figure()
plot(GV.JD(CO_BG_i),GV.CORAW_AL(CO_BG_i),'rx')
hold on
plot(GV.JD(CAL),GV.CORAW_AL(CAL),'bo')
legend('CO BG','CO CAL')
grid on

figure()
plot(GV.JD,GV.CORAW_AL,'bx')
hold on
plot(GV.JD,(GV.CORAW_AL-mean_CO_BG).*146./(mean_CO_CAL-mean_CO_BG),'ro')

GV.CO_PPM=(GV.CORAW_AL-mean_CO_BG).*146./(mean_CO_CAL-mean_CO_BG);

GV.CO_PPM(BAD)=NaN;
GV.CO_PPM(BAD_PRESSURE)=NaN;
GV.CO_PPM(BAD_STRANGE1)=NaN;
GV.CO_PPM(CAL)=NaN;

SPIKES=find(GV.CO_PPM<60);
GV.CO_PPM(SPIKES)=NaN;

figure()
plot(GV.JD,GV.CO_PPM,'ro')
grid on
legend('CO [ppb]')


filename=['RF05_CO_calibrated_data.mat'];
CO=GV;

cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
save(filename,'CO')


% load NONO2O3

cd C:\Users\kaser\Documents\CONTRAST\infield\NONO2O3

% dirname='C:\Users\kaser\Documents\CONTRAST\infield\NONO2O3\';
% filename=uigetfile('*.ict','Select files','MultiSelect','off');



[UTC,ICTdata,VarHeader,ICTfileData]=read_ICARTT_files('CONTRAST-NONO2O3_GV_20140122_RA.ict');
A=find(ICTdata(:,3)>0);

figure()
plot(UTC(A),ICTdata(A,3))

TAKEOFF2=find(GV.PALTF>200);%take only data after takeoff



RF05_forICARTT.CO_PPBV=CO.CO_PPM(TAKEOFF2);
RF05_forICARTT.TIME=CO.TIME(TAKEOFF2);
RF05_forICARTT.DOY=CO.DOY(TAKEOFF2);
RF05_forICARTT.CO2_dry_ppm=PICARRO_CALIBRATED.CO2_dry_ppm(TAKEOFF2);
RF05_forICARTT.CH4_dry_ppm=PICARRO_CALIBRATED.CH4_dry_ppm(TAKEOFF2);

filename=['RF05_forICARTT_data.mat'];


cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
save(filename,'RF05_forICARTT')