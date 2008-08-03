/*
 * Program to read an Air Force HDOB (high-density observation) file 
 * and create a KMZ for Google Earth.
 */
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

// Output directory for KMZ files
static std::string _kmzDir = "/net/www/docs/flight_data/";

static std::string _inputFile, _outputKML;

std::vector<ptime> _time;
std::vector<float> _lat, _lon, _alt, _temp, _wSpd, _wDir, _wMax;
std::vector<float> _surfWMax, _rainRate;

/* -------------------------------------------------------------------- */
std::string
annotateLastPoint()
{
    std::stringstream e;
    ptime lastTime = _time[_time.size()-1];
    int lastPoint = _lat.size() - 1;

    e << "<![CDATA[";
    e << lastTime << 
        "<br>Alt: " << _alt[lastPoint] << 
        "<br>Temp: " << _temp[lastPoint] << 
        "<br>WS: " << _wSpd[lastPoint] << 
        "<br>WD: " << _wDir[lastPoint];
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

    kml.open(file.c_str());
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
        "  <longitude>" << _lon[_lon.size()-1] << "</longitude>\n" <<
        "  <latitude>" << _lat[_lat.size()-1] << "</latitude>\n" <<
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

    for (size_t i = 0; i < _lat.size(); ++i)
        kml << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "\n";

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
        "    <coordinates>" << _lon[_lon.size()-1] << "," << 
            _lat[_lat.size()-1] << "," << _alt[_alt.size()-1] << 
            "</coordinates>\n" <<
        "   </Point>\n" <<
        "  </Placemark>\n";

    kml <<
        "</Folder>\n" <<
        "</Document>\n" <<
        "</kml>\n";

    kml.close();
    
    char buffer[1024];
    std::string tmptmp = _kmzDir + "/tmp.kmz";

    sprintf(buffer, "zip %s %s", tmptmp.c_str(), file.c_str());
    system(buffer);

    sprintf(buffer, "chmod g+w %s", tmptmp.c_str());
    system(buffer);

    std::string temp = _kmzDir + "real-time.kmz";
    rename(tmptmp.c_str(), temp.c_str());
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

    while (1) {
        // Get (and skip) the WMO header line
        getline(hdobFile, line);
        if (hdobFile.eof())
            break;

        // Get the HDOB header line
        getline(hdobFile, line);

        char str[80];
        unsigned int yyyymmdd;
        unsigned int obsNum;
        nread = sscanf(line.c_str(), "%s%s%s%s%d%d", str, str, str, str, 
                &obsNum, &yyyymmdd);
        
        date obDate(yyyymmdd / 10000, (yyyymmdd % 10000) / 100, 
                    yyyymmdd % 100);
        

        if (nread != 6 || strcmp(str, "HDOB") != 0) {
            std::cerr << "BARF on header line: " << nread << " " << str << std::endl;
            exit(1);
        }

        // Get the list of individual observations in this chunk
        while (1) {
            getline(hdobFile, line);

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

            ptime obTime(obDate, 
                         time_duration(hhmmss / 10000, (hhmmss % 10000) / 100,
                                       hhmmss %  100));
            _time.push_back(obTime);
                        
            float lat = (ddmmLat / 100) + ((ddmmLat % 100) / 60.0);
            if (latHem == 'S')
                lat *= -1;
            _lat.push_back(lat);

            float lon = (ddmmLon / 100) + ((ddmmLon % 100) / 60.0);
            if (lonHem == 'W')
                lon *= -1;
            _lon.push_back(lon);

            float pres = iPres * 0.1;
            if (pres < 100.0)
                pres += 1000.0;

            float geoAlt = iGeoAlt;
            _alt.push_back(geoAlt);

            float temp = iTemp * 0.1;
            if (tempSign == '-')
                temp *= -1;
            _temp.push_back(temp);

            float dp = iDP * 0.1;
            if (dpSign == '-')
                dp *= -1;

            unsigned wDir = dddsssWind / 1000;  // degrees
            _wDir.push_back(wDir);
            
            float wSpd = (dddsssWind % 1000) * 0.514444444;  // knots -> m/s
            _wSpd.push_back(wSpd);

            float wMax = iWMax * 0.514444444;
            _wMax.push_back(wMax);

            float surfWMax = iSurfWMax * 0.514444444;
            _surfWMax.push_back(surfWMax);

            float rainRate = iRainRate;
            _rainRate.push_back(rainRate);

            if (nread == 0 || nread == EOF)
                break;
            if (nread != 17) {
                std::cerr << "BARF on data line: " << nread << " " << line << std::endl;
                exit(1);
            }
        }

        if (hdobFile.eof())
            break;
    }
}

/* -------------------------------------------------------------------- */
int usage(const char* argv0)
{
    std::cerr << "Usage:" << argv0 << 
        " [-o <dir>] <hdob_file> <kmz_file>\n" << std::endl;
    return 1;
}

/* -------------------------------------------------------------------- */
int parseRunstring(int argc, char** argv)
{
    extern char *optarg;    /* set by getopt() */
    extern int optind;      /* "  "     "     */
    int opt_char;           /* option character */

    while ((opt_char = getopt(argc, argv, "o:")) != -1)
    {
        switch (opt_char)
        {
        case 'o':	// output directory
            _kmzDir = optarg;
            break;
        case '?':
            return usage(argv[0]);
            break;
        }
    }

    if ((optind + 2) != argc)
        return usage(argv[0]);

    _inputFile = argv[optind];
    _outputKML = argv[optind+1];

    return 0;
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    int rc;

    if (argc > 1)
    {
        if ((strstr(argv[1], "usage") || strstr(argv[1], "help")))
            return usage(argv[0]);

        if ((rc = parseRunstring(argc,argv)) != 0)
            return rc;
    }
    
    std::cout << "\n  Output directory : " << _kmzDir << "\n\n";

    if (_inputFile.length() > 0)
    {
        ReadDataFromHDOB(_inputFile);
        if (_lat.size() > 0)
            WriteGoogleEarthKMZ(_outputKML);
        return 0;
    } else {
        return usage(argv[0]);
    }
}
