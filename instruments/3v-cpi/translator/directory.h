#pragma once
#include <sys/stat.h>
#include "log.h"
namespace sp
{
	void MakeDirectory(const std::string& name)
	{
#ifdef _WIN32
		std::string n = "mkdir " + name;
		int result = system(n.c_str());
#else
		int result = mkdir(name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	//	mkdir(n);
#endif
		
	//	
		//if(result != 1)
		//{
		//	g_Log <<"Make Directory failed on \"" << name <<"\"\n";
		//}
	}

	void DeleteDirectory(const std::string& name)
	{

#ifdef _WIN32
		std::string n = "rmdir /s /q " + name;
		int result = system(n.c_str()); //windows delete temp directory
	//	g_Log <<"WIN32\n";
#else
		//std::string n = "sudo rm -r " + name;
		std::string n = "/s /q " + name;
		int result= rmdir(name.c_str());
		//int result = system(n.c_str());	  //linux delete all temps
	//	g_Log <<"NO WIN32\n";
#endif
	/*	if(result < 1)
		{
			g_Log <<"Delete Directory failed on \"" << name <<"\" result =" << result << "\n";
		}*/
	}
}
