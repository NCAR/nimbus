// an example:
//
//  NOYMR_CL             | 2008-03-27 21:14:11 | {-0.360266,1.06578}
//  A2D1600_CH0_2T       | 2007-08-09 17:46:10 | {-0.0282729,1.09077}
//  A2D008_CH0_2T        | 2007-08-09 17:46:10 | {-0.0282729,1.09077}
//
// <!-- A2D1600_CH0_2T       | 2007-08-09 17:46:10 | {-0.0282729,1.09077} -->
//                     <parameter name="gain"     value="4"       type="float"/> <parameter name="bipolar"      value="false"      type="bool"/>
//                     <parameter name="corSlope" value="1.09077" type="float"/> <parameter name="corIntercept" value="-0.0282729" type="float"/>
//
#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace std;

int main (void)
{
  map<string, string> longDesc;
  map<string, string> xmlConf;

  longDesc["1T"] = " [-10 to +10] volts\">";
  longDesc["2F"] = " [  0 to +10] volts\">";
  longDesc["2T"] = " [ -5 to  +5] volts\">";
  longDesc["4F"] = " [  0 to  +5] volts\">";

  xmlConf["1T"] = "<parameter name=\"gain\"     value=\"1\"       type=\"float\"/> <parameter name=\"bipolar\"      value=\"true\"       type=\"bool\"/>";
  xmlConf["2F"] = "<parameter name=\"gain\"     value=\"2\"       type=\"float\"/> <parameter name=\"bipolar\"      value=\"false\"      type=\"bool\"/>";
  xmlConf["2T"] = "<parameter name=\"gain\"     value=\"2\"       type=\"float\"/> <parameter name=\"bipolar\"      value=\"true\"       type=\"bool\"/>";
  xmlConf["4F"] = "<parameter name=\"gain\"     value=\"4\"       type=\"float\"/> <parameter name=\"bipolar\"      value=\"false\"      type=\"bool\"/>";

  char linebuf[1024];
  int c0,c1,c2, cc;
  bool generic_name;

  while (cin) {
    cin.getline(linebuf,1024);
    if (cin) {
      string line(linebuf);

      c0 = line.find("{");
      c1 = line.find(",");
      c2 = line.find("}");
      string corIntercept = line.substr(c0+1,c1-c0-1);
      string corSlope     = line.substr(c1+1,c2-c1-1);
      string cIpad("");
      string cSpad("");

      if (corIntercept.size() < 10)
        cIpad.resize (10-corIntercept.size(), ' ');

      if (corSlope.size() < 7)
        cSpad.resize (7-corSlope.size(), ' ');

      string a2dSN;
      string chnNum;
      string gainType;
      string xmlLine;

      generic_name = false;
      if (line.find("_1T") || line.find("_2F") || line.find("_2T") || line.find("_4F")) {
        generic_name = true;
        c0 = line.find("_");
        a2dSN    = line.substr(1,c0-1);
        chnNum   = line.substr(c0+3,1);
        gainType = line.substr(c0+5,2);
        xmlLine  = xmlConf[gainType];

// don't use... leave as an example
//      cc = xmlLine.rfind("type", xmlLine.length());
//      xmlLine.insert(cc,cIpad);
//      cc = xmlLine.rfind("type", cc);
//      xmlLine.insert(cc,cSpad);
      }
      generic_name = false;

      cout << "<!--" << line << " -->" << endl;
    if (generic_name) {
      cout << "                <variable name=\"" << a2dSN << "_CH" << chnNum << "_" << gainType << "\" units=\"V\" longname=\""
           << a2dSN << " channel " << chnNum << longDesc[gainType] << endl;
    }
      cout << "                    " << xmlLine << endl;
      cout << "                    <parameter name=\"corSlope\" value=\"" << corSlope << "\"" << cSpad
           << " type=\"float\"/> <parameter name=\"corIntercept\" value=\"" << corIntercept << "\"" << cIpad
           << " type=\"float\"/>" << endl;
    if (generic_name)
      cout << "                </variable>" << endl;
    }
  }
  return 0;
}
