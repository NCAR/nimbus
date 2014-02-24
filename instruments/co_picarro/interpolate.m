function CO_ALL = interpolate(CO,CORAW_AL,JD,TIME,type)
  CO_2=[CO(2:end);CO(1)];
  indexi=[1;find((CO-CO_2)<-100);length(CO)];

  CO_individual=[];
  CO_times=[];

  for iii=1:(length(indexi)-1)
    iii
    CO_i=mean(CORAW_AL(CO(indexi(iii)+3):CO(indexi(iii+1)-3)));
    CO_time_i=median(JD(CO(indexi(iii)+3):CO(indexi(iii+1)-3)));
    
    CO_individual=[CO_individual,CO_i];
    CO_times=[CO_times,CO_time_i];
  end

%-------------------------------------------------------------------------------
% Stützstellen (grid points)
%-------------------------------------------------------------------------------
  xs = CO_times;
  ys = CO_individual;
  timestep= TIME(2)-TIME(1);

%-------------------------------------------------------------------------------
% Interpolationspunkte (Interpolation)
%-------------------------------------------------------------------------------
% The units in this command are mixed. CO_times are in decimal days, timestep 
% is in seconds. Convert timestep to decimal days.
  timestep=timestep/(3600*24);
  xi = CO_times(1):timestep:CO_times(end);
  CO_interp = interp1(xs, ys, xi);

  fig=figure()
  plot(JD,CORAW_AL,'b')
  hold on
  plot(JD(CO),CORAW_AL(CO),'rx')
  hold on
  plot(CO_times,CO_individual,'ko')
  hold on
  plot(xi, CO_interp, 'gx')
  title(['individual ',type,' interpolations'])
  legend('CORAW',['CO ',type],['CO ',type,' ind'],['CO ',type,' interp'])
  saveas(fig,['CO_',type,'_interp'],'jpg')
  saveas(fig,['CO_',type,'_interp'],'fig')

%-------------------------------------------------------------------------------
% Add the first cal to the times before the first cal and the last cal
% number to after the last cal
%-------------------------------------------------------------------------------
  before_first_cal=find(JD<xi(1));
  during_the_cals=find(JD>=xi(1) & JD<=xi(end));
  after_last_cal=find(JD>xi(end));

  CO_ALL=zeros(1,length(JD))*NaN;
  CO_ALL(before_first_cal)=ones(1,length(before_first_cal))*CO_individual(1);
  CO_ALL(during_the_cals)=CO_interp(1:end-1);
  CO_ALL(after_last_cal)=ones(1,length(after_last_cal))*CO_individual(end);

  fig=figure()
  plot(JD,CORAW_AL,'b')
  hold on
  plot(JD(CO),CORAW_AL(CO),'rx')
  hold on
  plot(CO_times,CO_individual,'ko')
  plot(JD, CO_ALL, 'g')
  title(['CO ',type,' interpolation fit'])
  legend('CORAW',['CO ',type],['CO ',type,' ind'],['CO ',type,' fit'])
  saveas(fig,['CO_',type,'_ALL'],'jpg')
  saveas(fig,['CO_',type,'_ALL'],'fig')

end
