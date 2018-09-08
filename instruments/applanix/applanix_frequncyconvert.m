%Kelly Schick 13 August 2014
%Goal: write code that converts 10 Hz data into 1 Hz data
function [outmatrix]=applanix_frequncyconvert(infile)
%extract 10 data point from matrix/file. these will become the "old points"
inmatrix=infile;%dlmread(infile,'',1,0);
[rows,columns]=size(inmatrix);
endpoint=floor(rows/10)-1;
slope=zeros(1,columns-1);
outmatrix=zeros(endpoint,columns);
old_group=inmatrix(1:10,:);
%calculate averages of all variables -put into array (old_average)
old_averages=mean(old_group);
%enter iterative process

for i=1:endpoint
    %extract set of new points
    new_group =inmatrix((i*10)+1:(i+1)*10,:);
    %calculate averages of all variables in new set of 10
    new_averages = mean(new_group);
    % ??FIND TIME DESIRED (WHICH WHOLE SECOND?) ??
    time_desired = ceil((new_averages(1)+old_averages(1))/2); %edited 26 Jan 2015 to attempt to correct for applanix leading by 2 seconds
    %find slope of linear equation created by old & new averages
    time_run = new_averages(1)-old_averages(1);
    var_rise = new_averages(2:columns)-old_averages(2:columns);
    for j=1:(columns-1)
        slope(j)=var_rise(j)/time_run;
    end
%     size(old_averages)
%     size(new_averages)
%     size(slope)
    %solve for value at desired time
    %(y-y_avg)=slope*(time_desired-time_avg)
    outmatrix(i,1)=time_desired;
    outmatrix(i,2:columns)=(slope'*(old_averages(1)-time_desired)+new_averages(2:columns)')';
    %set new points to old
    old_averages=new_averages;
    %write values out at whole seconds to new file
    %repeat until end of file ??POSSIBLY limit number of iterations based on
    %divisions by ten so as to avoid out of bounds errors?
end