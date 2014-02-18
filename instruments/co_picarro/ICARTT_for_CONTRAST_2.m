clc
clear all
close all
%% to calculate UTC time: http://www.mbari.org/staff/rich/utccalc.htm
%% this segment reads data where the first row is a header
% data are stored in rf1 array
% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08\
% dirname='C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08\';
flight = '09';
slash = '/';
rafPath='/Users/campos/Documents/macBak/campos/chemData/2014contrast/ifpRAF/';
cd(rafPath);
if ~exist('pix')
    mkdir('pix');
end
dirname=rafPath;
filename=uigetfile('*.mat','Select files','MultiSelect','off');


% for kk=1%:length(filename)
% 
% if length(filename)==23;%27;
    load([dirname,filename])    
% else load([dirname,char(filename(kk))])
% end


if preICARTT.TIME(1)<43200 
date_start=datevec(preICARTT.DOY(1)+datenum('12-31-2013')+1);%% add 1 for the day if take off is after Midnight UTC (==10AM in Guam)
else
date_start=datevec(preICARTT.DOY(1)+datenum('12-31-2013'));%% add 1 for the day if take off is after Midnight UTC (==10AM in Guam)    
end



date2=num2str(date_start(1:3));
datestr=date_start(1:3);

if datestr(2)<10 & datestr(3)>9
date3=[num2str(datestr(1)),'0',num2str(datestr(2)),num2str(datestr(3))];
elseif datestr(2)<10 & datestr(3)<10
date3=[num2str(datestr(1)),'0',num2str(datestr(2)),'0',num2str(datestr(3))];
end

doy=preICARTT.DOY;
tag=floor(doy(1));
%% calculate utc
utc=[];
for i=1:length(doy)
    
 A=datevec(doy(i)-tag);
utc_i=A(3)*60*60*24+A(4)*60*60+A(5)*60+A(6);%

utc=[utc,utc_i];
end





conc_data=[preICARTT.CO_PPBV,preICARTT.CO2_dry_ppm,preICARTT.CH4_dry_ppm];
%% make a time trace with no missing values, missing seconds in the ICARTT format does not work


utc_test=floor(utc);
utc_2=utc_test(1):1:utc_test(end);

aa=ismember(utc_2,utc_test);


all_data_2=NaN(size(aa,2),3);
for i=1:length(aa)
    
    if aa(i)==1
        bb=find(utc_2(i)==utc_test);
        all_data_2(i,:)=conc_data(bb(1),:);
    elseif aa(i)==0
            all_data_2(i,:)=NaN(1,3);
    end
end


figure()
subplot(4,1,1)
plot(utc,preICARTT.CO_PPBV,'g.')
legend('CO [ppbv]')
grid on
xlabel('utc')
ylabel('ppbv')
xlim([utc(1) utc(end)])
ylim([min(preICARTT.CO_PPBV) max(preICARTT.CO_PPBV)])
subplot(4,1,2)
plot(utc,preICARTT.CO2_dry_ppm,'b.')
legend('CO2 [ppmv]')
grid on
xlabel('utc')
ylabel('ppmv')
xlim([utc(1) utc(end)])
ylim([min(preICARTT.CO2_dry_ppm) max(preICARTT.CO2_dry_ppm)])
subplot(4,1,3)
plot(utc,preICARTT.CH4_dry_ppm,'r.')
legend('CH_4 [ppmv]')
grid on
xlabel('utc')
ylabel('ppmv')
xlim([utc(1) utc(end)])
ylim([min(preICARTT.CH4_dry_ppm) max(preICARTT.CH4_dry_ppm)])
subplot(4,1,4)
plot(utc,preICARTT.H2O,'k.')
legend('H_2O [vol%]')
grid on
xlabel('utc')
ylabel('vol%')
xlim([utc(1) utc(end)])
ylim([min(preICARTT.H2O) max(preICARTT.H2O)])
set(gcf, 'Color', 'white'); % white bckgr
screen_size = get(0, 'ScreenSize');
set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08\
saveas(gcf, ...
    ['pix',slash,'CO_CO2_CH4_RF',flight,'.jpg'],'jpg');
% export_fig( gcf, ...      % figure handle
%     ['pix',slash,'CO_CO2_CH4_RF',flight],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi

saveas(gcf, ...
    ['pix',slash,'CO_CO2_CH4_RF08'],... % name of output file without extension
'fig');    


conc_data2=all_data_2;



conc_data2(isnan(conc_data2))=-9999;
clear rf1
rf1=[utc_2',conc_data2];

figure()
plot(rf1(:,1),rf1(:,2))

%% open new file and write data to file
% change filename!

filenamexp=[rafPath,'COCO2CH4_GV_',date3,'_RA.ict'];

fid = eval(['fopen(''',filenamexp ''',''w+'')'])

fprintf(fid,'%s\n','35, 1001'); %change number of header lines
fprintf(fid,'%s\n','Campos, T.L.; Kaser, L.; Stell, M.; Munnerlyn, J.; Weinheimer, A.J.; Flocke, F.M;');
fprintf(fid,'%s\n','NCAR');
fprintf(fid,'%s\n','NCAR Aerolaser');
fprintf(fid,'%s\n','CONTRAST');
fprintf(fid,'%s\n','1, 1');%?
fprintf(fid,'%s\n',[date3(1:4),', ',date3(5:6),', ',date3(7:8),', ','2014, 02, 06']); %change dates: %UTC date when data begin, UTC date of data reduction or revision - comma delimited (yyyy, mm, dd, yyyy, mm, dd).
fprintf(fid,'%s\n','1');%Data interval 1s --> 1???
fprintf(fid,'%s\n','UTC, seconds'); %needs to be in UTC
fprintf(fid,'%s\n','3'); %change - number of columns - this would be 13 columns (first start time column is not included here)
fprintf(fid,'%s\n','1,1,1'); % add or delete 1s according to number of columns
fprintf(fid,'%s\n','-9999,-9999,-9999'); % add or delete -9999s according to number of columns
% fprintf(fid,'%s\n','stop_time, seconds');% , sec');
% fprintf(fid,'%s\n','mid_time, seconds');% , sec');
fprintf(fid,'%s\n','CO, ppbv'); % name of first species after the 3 time columns
fprintf(fid,'%s\n','CO2, ppmv'); 
fprintf(fid,'%s\n','CH4, ppmv'); 
fprintf(fid,'%s\n','1');% was macht diese Zeile? hier stand 1 jetzt 0
fprintf(fid,'%s\n','Data is reported in 1s.'); % replace the time by averaging time here
fprintf(fid,'%s\n','17'); % this is the number of lines after this line including this line
fprintf(fid,'%s\n','PI_CONTACT_INFOs: campos@ucar.edu, kaser@ucar.edu');
fprintf(fid,'%s\n','PLATFORM: NCAR GV');
fprintf(fid,'%s\n','LOCATION: Latitude, Longitude and Altitude of the GV can be obtained from other files');
fprintf(fid,'%s\n','ASSOCIATED_DATA: N/A');%aircraft data
fprintf(fid,'%s\n','INSTRUMENT_INFO: CO AEROLASER');								
fprintf(fid,'%s\n','DATA_INFO: units are given in ppmv');
fprintf(fid,'%s\n','UNCERTAINTY: CO: 2ppb +/-3%; CO2: 0.3 ppmv; CH4: 30ppb'); % change!!!
fprintf(fid,'%s\n','ULOD_FLAG: N/A');										
fprintf(fid,'%s\n','ULOD_VALUE: N/A');										
fprintf(fid,'%s\n','LLOD_FLAG: N/A');	% typically people like -8888, but here we use a flag for everything (missing + LOD) and say it is missing -9999									
fprintf(fid,'%s\n','LLOD_VALUE: N/A');	% you could choose something here or leave as is -									
fprintf(fid,'%s\n','DM_CONTACT_INFOs: N/A');										
fprintf(fid,'%s\n','PROJECT_INFO: CONTRAST');										
fprintf(fid,'%s\n','STIPULATIONS_ON_USE: PRELIMINARY DATA. DO NOT USE WITHOUT TALKING TO PIs');										
fprintf(fid,'%s\n','OTHER_COMMENTS: N/A');
% fprintf(fid,'%s\n','REVISION: R1');
% fprintf(fid,'%s\n','R1: Some previously missing datapoints added');
fprintf(fid,'%s\n','REVISION: RA');
% fprintf(fid,'%s\n','RA: Preliminary Data.');	
% important down below is the format
fprintf(fid,'%s','utc,');
% fprintf(fid,'%s',' stop_time,');% , sec');
% fprintf(fid,'%s',' mid_time,');% , sec');
fprintf(fid,'% s','CO,');
fprintf(fid,'% s','CO2,');
fprintf(fid,'% s\n','CH4');


% 
% for i=1:size(rf1,1) %zeilen
%     fprintf(fid,'%8.0f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f, %8.4f\n', rf1(i,:));
% %     fprintf(fid,'%8.2f %8.2f %8.2f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f\n', rf1(i,:));
%     % first 3 columns are start-time, end-time, and mid-time - then VOCs as
%     % labeled above - make sure format neds with \n - which means return to
%     % next line
% end

% figure()
% plot(utc,'rx')

% utc_test=floor(utc);
% utc_2=utc_test(1):1:utc_test(end);
% 
% aa=ismember(utc_2,utc_test);
% 
% % figure()
% % plot(aa,'bo')
% 
% all_data_2=NaN(size(aa,2),15);
% for i=1:length(aa)
%     
%     if aa(i)==1
%         bb=find(utc_2(i)==utc_test);
%         all_data_2(i,2:end)=rf1(bb(1),2:end);
%     elseif aa(i)==0
%             all_data_2(i,2:end)=NaN(1,14);
%     end
% end
% 
% 
% all_data_2(isnan(all_data_2))=-9999;
% 
% all_data_2(:,1)=utc_2;

for i=1:size(rf1,1) %zeilen
    fprintf(fid,'%5.0f, %8.4f, %8.4f, %8.4f\n', rf1(i,:));
%     fprintf(fid,'%8.2f %8.2f %8.2f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f\n', rf1(i,:));
    % first 3 columns are start-time, end-time, and mid-time - then VOCs as
    % labeled above - make sure format neds with \n - which means return to
    % next line
end

fclose(fid);

% end

% 

% % load NONO2O3
% 
% cd C:\Users\kaser\Documents\CONTRAST\infield\NONO2O3
% 
% dirname='C:\Users\kaser\Documents\CONTRAST\infield\NONO2O3\';
% filename=uigetfile('*.ict','Select files','MultiSelect','off');
% 
% 
% % 
% [UTC,ICTdata,VarHeader,ICTfileData]=read_ICARTT_files(filename);
% UTC=UTC([1:27085,27087:end]);
% ICTdata=ICTdata([1:27085,27087:end],:);
% 
% 
% A=find(ICTdata(:,3)>0);
% 
% figure()
% plot(UTC(A),ICTdata(A,3))


% 
% % % A = [7 18 27 42 65 49 54 65 78 82 87 98];
% % [n, bin] = histc(UTC, unique(UTC));
% % multiple = find(n > 1);
% % index    = find(ismember(bin, multiple));
% % % 
% O3_interp=interp1(unique(UTC(A)),ICTdata(A,3),utc);

O3_interp=interp1(GV.TIME,GV.FO3,utc);

set(0,'DefaultLineLineWidth',2)
set(0, 'DefaultAxesFontSize',20)


figure()
plotyy(utc,preICARTT.CO_PPBV,utc,O3_interp)
legend('CO','O3')
% xlim([utc(1) utc(end)])
grid on
set(gcf, 'Color', 'white'); % white bckgr
screen_size = get(0, 'ScreenSize');
set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08\
saveas(gcf, ...
    ['pix',slash,'O3_CO_RF',flight,'.jpg'],'jpg');

% export_fig( gcf, ...      % figure handle
%     ['pix',slash,'O3_CO_RF',flight,'.jpg'],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi
saveas(gcf, ...
    ['pix',slash,'O3_CO_RF',flight,'.fig'],... % name of output file without extension
'fig');

figure()
scatter(double(preICARTT.CO_PPBV),double(O3_interp),50,double(preICARTT.PALTF))
xlabel('CO [ppbv]')
ylabel('O_3 [ppbv]')
colorbar;
grid on
set(gcf, 'Color', 'white'); % white bckgr
screen_size = get(0, 'ScreenSize');
set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% cd C:\Users\kaser\Documents\CONTRAST\matlab\RF08\resultsRF08\
% export_fig( gcf, ...      % figure handle
%     ['pix',slash,'O3_CO_scatter_with_height_RF08'],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi
% saveas(gcf, ...
%     ['pix',slash,'O3_CO_scatter_with_height_RF',flight,'.jpg'],... % name of output file without extension
% 'jpg');
saveas(gcf, ...
    ['pix',slash,'O3_CO_scatter_with_height_RF',flight,'.fig'],... % name of output file without extension
'fig');
% 
% 
% O3_interp=O3_interp';
% XX=find(O3_interp>22 & CO.CO_PPM<80);
% figure()
% plot(utc,O3_interp,'b')
% hold on
% plot(utc,CO.CO_PPM,'g')
% plot(utc,CO.PALTF/3/100,'r')
% plot(utc(XX),O3_interp(XX),'kx')
% hold on
% 
% plot(utc(XX),CO.CO_PPM(XX),'kx')
% 
% legend('O_3 [ppbv]','CO [ppbv]','PALT [m/1000]')
% grid on
% xlabel('utc')
% ylabel('ppbv')
% set(gcf, 'Color', 'white'); % white bckgr
% screen_size = get(0, 'ScreenSize');
% set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% export_fig( gcf, ...      % figure handle
%     ['O3_CO_height'],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi
% 
% figure()
% scatter(double(CO.CO_PPM),double(O3_interp),50,double(CO.PALTF))
% hold on
% plot(double(CO.CO_PPM(XX)),double(O3_interp(XX)),'kx')
% hold on
% xlabel('CO [ppbv]')
% ylabel('O_3 [ppbv]')
% colorbar;
% grid on
% set(gcf, 'Color', 'white'); % white bckgr
% screen_size = get(0, 'ScreenSize');
% set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% export_fig( gcf, ...      % figure handle
%     ['O3_CO_scatter_with_height_2'],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi
% 
% 
% 
% % load PICARRO
% 
% 
% 
% load('C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\RF05_PICARRO_calibrated_data.mat')
% 
% figure()
% plot(utc,PICARRO_CALIBRATED.CO2_dry_ppm)
% legend('CO2 [ppmv]')
% grid on
% xlabel('utc')
% ylabel('ppmv')
% set(gcf, 'Color', 'white'); % white bckgr
% screen_size = get(0, 'ScreenSize');
% set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% export_fig( gcf, ...      % figure handle
%     ['CO2_RF05'],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi
% 
% 
% 
% figure()
% plot(utc,PICARRO_CALIBRATED.CH4_dry_ppm,'r')
% legend('CH_4 [ppmv]')
% grid on
% xlabel('utc')
% ylabel('ppmv')
% set(gcf, 'Color', 'white'); % white bckgr
% screen_size = get(0, 'ScreenSize');
% set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% export_fig( gcf, ...      % figure handle
%     ['CH4_RF05'],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi
% 
% figure()
% plot(utc,PICARRO_CALIBRATED.CH4_dry_ppm,'r')
% hold on
% plot(utc,CO.CO_PPM./100,'g')
% 
% figure()
% plot(PICARRO_CALIBRATED.CH4_dry_ppm,CO.CO_PPM,'bo')
% grid on
% xlabel('CH_4 [ppmv]')
% ylabel('CO [ppbv]')
% set(gcf, 'Color', 'white'); % white bckgr
% screen_size = get(0, 'ScreenSize');
% set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% export_fig( gcf, ...      % figure handle
%     ['CH4_CO'],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi
% 
% 
% figure()
% plot(utc,CO.CO_PPM,'g')
% legend('CO [ppbv]')
% grid on
% xlabel('utc')
% ylabel('ppbv')
% set(gcf, 'Color', 'white'); % white bckgr
% screen_size = get(0, 'ScreenSize');
% set(gcf, 'Position', [0 0 screen_size(3) screen_size(4) ] );
% export_fig( gcf, ...      % figure handle
%     ['CO_RF05'],... % name of output file without extension
%     '-painters', ...      % renderer
%     '-pdf', ...           % file format
%     '-r300' );             % resolution in dpi


figure()
plot(utc)
hold on
plot(UTC,'r')