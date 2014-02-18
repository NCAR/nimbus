function [UTC,ICTdata,VarHeader,ICTfileData] =ICT_File_Read_FastCSV(ICTFileIn)
%JV_file_read
%JVFileIn = 'C:\jvdata\2008ARCTAS\out\080401_62357_z01_200_080401_trans1_Z_200.jv'

fidICTFile = fopen(ICTFileIn,'r');
disp('ICT file opened');
%read header

nHeaderLines = str2num(fgetl(fidICTFile))



for iHeaderLine =1:nHeaderLines-1
    line = fgetl(fidICTFile)
    HeaderLines(iHeaderLine).txt = line;

end



% get number of variables - nVars includes timescale line
nVars = str2num(HeaderLines(9).txt)+1


for iVar = 1:nVars
    [VarHeader(iVar).header,line] = strtok(line);
end
    

disp('start ICT data read');
%build textscan format string
FormatString = '';
for iVar = 1:nVars-1
    FormatString = [FormatString,'%f,'];   %remove comma for tab data
end
FormatString = [FormatString,'%f'];

%read JV data
ICTfileData = textscan(fidICTFile,FormatString);
nLines = length(ICTfileData{1});

UTC = zeros(nLines,1);
ICTdata = zeros(nLines,nVars-3);

UTC = ICTfileData{1};
disp(['Start: ',num2str(UTC(1)),', End: ',num2str(UTC(end))])
i=0;
for iVar = 1:nVars-3
    tempData = ICTfileData{iVar+3};
    for iLine = 1:nLines
        ICTdata(iLine,iVar) = tempData(iLine);
    end
end
    
%JVdata(1:i,1:nVars-1) = tempJV(1:i,1:nVars-1);
%UTC(1:i) = UTCtemp(1:i);
fclose(fidICTFile);
disp(['ICT File Read: ',ICTFileIn])
    
    
    