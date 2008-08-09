/*
 * Program to read an Air Force HDOB (high-density observation) file 
 * and create a KMZ for Google Earth.
 */
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

std::map<ptime, float> _lat, _lon, _alt, _temp, _wSpd, _wDir, _wMax;
std::map<ptime, float> _surfWMax, _rainRate;

/* -------------------------------------------------------------------- */
std::string
annotateLastPoint()
{
    std::stringstream e;
    ptime lastTime = _lat.rbegin()->first;

    e << "<![CDATA[";
    e << lastTime << 
        "<br>Alt: " << _alt[lastTime] << 
        "<br>Temp: " << _temp[lastTime] << 
        "<br>WS: " << _wSpd[lastTime] << 
        "<br>WD: " << _wDir[lastTime];
    e << "]]>";

    return e.str();
}

/* -------------------------------------------------------------------- */
/* Copies verbatim from a text file called "include.kml", this allows for
 * user-defined stuff to be added.
 */
void WriteSpecialInclude(std::ofstream & googleEarth)
{
    std::ifstream inc;

    inc.open("include.kml");
    if (inc.is_open())
    {
        char buff[1028];
        while (!inc.eof())
        {
            inc.getline(buff, 1028);
            googleEarth << buff;
        }
        inc.close();
    }
}


/* -------------------------------------------------------------------- */
void WriteGoogleEarthKMZ(std::string & file)
{
    std::ofstream kml;

    std::stringstream tmpFileName;
    tmpFileName << "/tmp/" << time(0) << ".kml";
    kml.open(tmpFileName.str().c_str());
    if (kml.is_open() == false)
    {
        std::cerr << "Failed to open output file " << file << std::endl;
        return;
    }

    kml <<
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" <<
        "<kml xmlns=\"http://earth.google.com/kml/2.1\">\n" <<
        "<Document>\n" <<
        " <name>" << "HDOB track" << "</name>\n" <<
        " <Style id=\"PM1\">\n" <<
        "  <IconStyle>\n" <<
        "   <scale>0.5</scale>\n" <<
        "   <Icon>\n" <<
        "    <href>http://www.eol.ucar.edu/flight_data/images/red.png</href>\n" <<
        "   </Icon>\n" <<
        "  </IconStyle>\n" <<
        " </Style>\n" <<
        " <Style id=\"PM2\">\n" <<
        "  <IconStyle>\n" <<
        "   <scale>0.5</scale>\n" <<
        "   <Icon>\n" <<
        "    <href>http://www.eol.ucar.edu/flight_data/images/white.png</href>\n" <<
        "   </Icon>\n" <<
        "  </IconStyle>\n" <<
        " </Style>\n" <<
        " <Style id=\"BOUND\">\n" <<
        "  <LineStyle>\n" <<
        "   <color>ffff0000</color>\n" <<
        "  </LineStyle>\n" <<
        " </Style>\n" <<
        " <Style id=\"TRACK\">\n" <<
        "  <LineStyle>\n" <<
        "   <color>ff00ffff</color>\n" <<
        "  </LineStyle>\n" <<
        "  <PolyStyle>\n" <<
        "   <color>7f00ff00</color>\n" <<
        "  </PolyStyle>\n" <<
        " </Style>\n" <<
        " <LookAt>\n" <<
        "  <longitude>" << _lon.rbegin()->second << "</longitude>\n" <<
        "  <latitude>" << _lat.rbegin()->second << "</latitude>\n" <<
        "  <range>100000</range>\n" <<
        "  <tilt>55</tilt>\n" <<
        "  <heading>0</heading>\n" <<
        " </LookAt>\n" <<
        " <Folder>\n" <<
        "  <name>" << "HDOB" << "</name>\n" <<
        "  <open>1</open>\n" <<
        "  <Placemark>\n" <<
        "   <name>Track</name>\n" <<
        "   <styleUrl>#TRACK</styleUrl>\n" <<
        "   <visibility>1</visibility>\n" <<
        "   <open>1</open>\n" <<
        "   <LineString>\n" <<
        "    <extrude>1</extrude>\n" <<
        "    <tessellate>1</tessellate>\n" <<
        "    <altitudeMode>absolute</altitudeMode>\n" <<
        "    <coordinates>\n";

    std::map<ptime, float>::iterator it;
    for (it = _lat.begin(); it != _lat.end(); it++) {
        ptime t = it->first;
        kml << _lon[t] << "," << _lat[t] << "," << (int)_alt[t] << "\n";
    }

    kml << 
        "    </coordinates>\n" << 
        "   </LineString>\n" << 
        "  </Placemark>\n";
    
    kml << 
        "  <Placemark>\n" <<
        "   <name>Plane</name>\n" << 
        "   <description>" << annotateLastPoint() << "</description>\n" <<
        "   <styleUrl>#PM1</styleUrl>\n" <<
        "   <Point>\n" << 
        "    <altitudeMode>absolute</altitudeMode>\n" <<
        "    <coordinates>" << _lon.rbegin()->second << "," << 
            _lat.rbegin()->second << "," << _alt.rbegin()->second << 
            "</coordinates>\n" <<
        "   </Point>\n" <<
        "  </Placemark>\n";

    kml <<
        "</Folder>\n" <<
        "</Document>\n" <<
        "</kml>\n";

    kml.close();

    // Remove the destination kmz (if any)
    remove(file.c_str());
    
    // Create the destination kmz
    char buffer[1024];
    sprintf(buffer, "zip -q %s %s", file.c_str(), tmpFileName.str().c_str());
    system(buffer);

    // remove the temporary kml
    remove(tmpFileName.str().c_str());
}

/* -------------------------------------------------------------------- */
// Parse data from an Air Force HDOB (high-density observation) message file
void ReadDataFromHDOB(const std::string & fileName)
{
    std::ifstream hdobFile(fileName.c_str());

    if (hdobFile.fail())
    {
        std::cerr << "Failed to open HDOB file " << fileName << std::endl;
        return;
    }

    std::string line;
    int nread;
    
    std::cout << "Reading " << fileName << std::endl;
    
    while (1) {
        // Get (and skip) the WMO header line
        getline(hdobFile, line);
        if (hdobFile.eof())
            break;

        // Get the HDOB header line
        getline(hdobFile, line);

        char missionId[32];
        char str[80];
        unsigned int yyyymmdd;
        unsigned int obsNum;
        nread = sscanf(line.c_str(), "%31c%s%d%d", missionId, str, 
                &obsNum, &yyyymmdd);
        
        if (nread != 4 || strcmp(str, "HDOB") != 0) {
            std::cerr << "Bad HDOB header line in file: " << fileName << "." <<
                "  Skipping file." << std::endl;
            break;
        }
        
        missionId[31] = '\0';
        
        date obDate(yyyymmdd / 10000, (yyyymmdd % 10000) / 100, 
                    yyyymmdd % 100);
        

        // Get the list of individual observations in this chunk
        while (1) {
            getline(hdobFile, line);
            
            if (line.substr(0, 2) == "$$")
                return;

            unsigned int hhmmss;
            unsigned int ddmmLat;
            char latHem;
            unsigned int ddmmLon;
            char lonHem;
            unsigned int iPres;
            unsigned int iGeoAlt;    // geopotential altitude, m
            unsigned int xxxx;
            char tempSign;
            unsigned int iTemp;
            char dpSign;
            unsigned int iDP;
            unsigned int dddsssWind;    // wind direction and speed
            unsigned int iWMax;
            unsigned int iSurfWMax;
            unsigned int iRainRate;
            unsigned int ff;
            nread = sscanf(line.c_str(), "%d%d%c%d%c%d%d%d%c%d%c%d%d%d%d%d%d",
                    &hhmmss, &ddmmLat, &latHem, &ddmmLon, &lonHem,
                    &iPres, &iGeoAlt, &xxxx, &tempSign, &iTemp, 
                    &dpSign, &iDP, &dddsssWind, &iWMax, &iSurfWMax,
                    &iRainRate, &ff);
            
            if (nread == 0 || nread == EOF)
                break;
            if (nread != 17) {
                std::cerr << "BARF on data line: " << nread << " " << line << std::endl;
                exit(1);
            }

            ptime obTime(obDate, 
                         time_duration(hhmmss / 10000, (hhmmss % 10000) / 100,
                                       hhmmss %  100));
                        
            float lat = (ddmmLat / 100) + ((ddmmLat % 100) / 60.0);
            if (latHem == 'S')
                lat *= -1;
            _lat[obTime] = lat;

            float lon = (ddmmLon / 100) + ((ddmmLon % 100) / 60.0);
            if (lonHem == 'W')
                lon *= -1;
            _lon[obTime] = lon;

            float pres = iPres * 0.1;
            if (pres < 100.0)
                pres += 1000.0;

            float geoAlt = iGeoAlt;
            _alt[obTime] = geoAlt;

            float temp = iTemp * 0.1;
            if (tempSign == '-')
                temp *= -1;
            _temp[obTime] = temp;

            float dp = iDP * 0.1;
            if (dpSign == '-')
                dp *= -1;

            unsigned wDir = dddsssWind / 1000;  // degrees
            _wDir[obTime] = wDir;
            
            float wSpd = (dddsssWind % 1000) * 0.514444444;  // knots -> m/s
            _wSpd[obTime] = wSpd;

            float wMax = iWMax * 0.514444444;
            _wMax[obTime] = wMax;

            float surfWMax = iSurfWMax * 0.514444444;
            _surfWMax[obTime] = surfWMax;

            float rainRate = iRainRate;
            _rainRate[obTime] = rainRate;
        }

        if (hdobFile.eof())
            break;
    }
    
    hdobFile.close();
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "Usage:" << argv[0] << " <kmz_file> <hdob_file> [<hdob_file>  ...]\n" << 
            std::endl;
        return 1;
    }

    std::string outputKmz(argv[1]);

    for (int i = 2; i < argc; i++)
        ReadDataFromHDOB(std::string(argv[i]));

    if (_lat.size() > 0)
        WriteGoogleEarthKMZ(outputKmz);
    return 0;
}
