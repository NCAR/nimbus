#ifndef _ProjectInfo_hh_
#define _ProjectInfo_hh_

#include <string>

class ProjectInfo
{
public:
  ProjectInfo() : 
    groundFeedDataRate(1)
  { };

  std::string platform;
  std::string projectName;
  std::string flightNumber;
  std::string landmarks;
  int groundFeedDataRate;
};

#endif // _ProjectInfo_hh_
