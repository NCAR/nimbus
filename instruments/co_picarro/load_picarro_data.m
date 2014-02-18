%load PICARRO data
tic
clc
clear all
close all

%% insert RF number and flight day
%%info: RF4 (2014,01,19); RF5 (2014,01,21);
result=['10'];
doy_start=datenum(2014,02,08);


cd C:\Users\kaser\Documents\CONTRAST\infield\Picarro\RF10\Private

dirname='C:\Users\kaser\Documents\CONTRAST\infield\Picarro\RF10\Private\';
filename=uigetfile('*.h5','Select files','MultiSelect','on');

file=[dirname,char(filename(1))];
data = h5read(file,'/results');

all_fieldnames=fieldnames(data);
% 
for i=1:length(fieldnames(data))
fieldname=char(all_fieldnames(i));
DATA.(fieldname)=[];

end


for kk=1:length(filename)
clear data

file=[dirname,char(filename(kk))];


data = h5read(file,'/results');

% 
    for i=1:length(fieldnames(data))
    fieldname=char(all_fieldnames(i));
    data_i=data.(fieldname);
    DATA.(fieldname)=[DATA.(fieldname);data_i];
    
    end


end




PICARRO=DATA;
PICARRO.DOY=DATA.JULIAN_DAYS-1;
filename=['RF', result, '_PICARRO_data.mat'];

% cd C:\Users\kaser\Documents\CONTRAST\infield\matlab_data\
cd C:\Users\kaser\Documents\CONTRAST\matlab\RF10\resultsRF10
save(filename,'PICARRO')

figure()
plot(PICARRO.CO2)


figure()
plot(PICARRO.H2O)
toc
