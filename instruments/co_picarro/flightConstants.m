project=['CONTRAST'];
picPath=['/Users/janine/Desktop/', project, '/PICARRO/'];
%picPath=['/net/work/Projects/', project, '/PICARRO/'];
% offset needs to be +1 if start after UTC 00 (e.g. for RF05), zero otherwise
offset=0;

% mixing ratio of the CO cal gas (ppb)
% (This was defined as 141 for a plot, but the result was not saved anywhere
% so I am assuming 140 is correct).
cal_COMR=140;


% ------ RF04 ---------
% flightNum='04';
% flightYear=2014;
% flightMonth=01;
% flightDay=19;

% ------ RF05 ---------
% flightNum='05';
% flightYear=2014;
% flightMonth=01;
% flightDay=21;
% offset=1;

% ------ RF08 ---------
% flightNum='08';
% flightYear=2014;
% flightMonth=02;
% flightDay=01;

% ------ RF11 ---------
% flightNum='11';
% flightYear=2014;
% flightMonth=02;
% flightDay=12;
% adBefore = 44;
% adAfter = 44.26;
% filterLow=10;

% ------ RF12 ---------
% flightNum='12';
% flightYear=2014;
% flightMonth=02;
% flightDay=17;
% adBefore = 48.998;
% adAfter = 49.232;
% filterHigh=100;
% filterLow=50;

% ------ RF13 ---------
flightNum='13';
flightYear=2014;
flightMonth=02;
flightDay=19;
adBefore = 50.709;
adAfter = 51.016;
% apply brute force filtering of CO if nothing else worked
filterHigh=120;
filterLow=60;

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
