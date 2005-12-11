#ifndef _INJECTSD_H
#define _INJECTSD_H

/****************/
//injectsd.h

#include <iostream>
#include <fstream>
#include "nimbus.h"
#include <string>
#include <vector>

extern NR_TYPE          *AveragedData;


struct v 
{
  std::string name;
  char type;
  int index;
  float value;
  std::string function;
};

typedef struct v vars;

class SyntheticData
{
 private:
  std::vector<RAWTBL *> rt;              //holds pointers to the raw variables to be modified
  int varcount;                     //variable count for variables to be modified from a file
  int constcount;                   // variable count for variables to be modifed with constant values
  int funccount;                    //variable count for variables to be modified with functions
  int ir;                           //rt vector counter
  std:: vector<vars> varnames;            //hold the info for variables to be modified from a file 
  std:: vector<vars> constvarnames;       //holds the info for variables to be modified by constant values
  std::vector<vars> funcvarnames;        //holds the info for variables to be modified by functions
  std::string fname;                //hold the name of the file that contains synthetic data
  float passedtime;                 //holds the time that nimubs is currently processing
  float timestamp;                  //holds the time from the datafile
  std::string dummy;                //temporary storage for timestamp
  std::ifstream f;                       //the input file stream that reads data from the 
  int indexr;                       //holds the location of the current variable in the raw table
  char temp;
 std::string tf;                   //temporary string
  vars tempv;                       //temporary value that is constatly changing
  float temptally;                  //used with highrate data, this value is stored in averaged data if data is modified by a constant or a function
  float *temptallyfunc;     //used with highrate data, this value is stored in averaged data if data is modified from a file
 protected:
   void ReadLine();                 //reads a line of values  from the file opened with InitSynth and stores the values in values[] 
   void Inject();                   // checks to see if time stamps match if they do then replace the info with the synthetic data
   void CheckTime();                //checks to see if the times match, if they do it calls inject, if they don't it takes appropriate action
   void InjectConstants();          //replaces the current value with a constant
   void InjectFunctions();          //modifies the current value with a function
   void Injecthr(int);
   void ReadhrLine();
   void SetHighData(char t,int ind,int offset,float val);  
   void SetSampleData(char t,int ind,int offset,float val);
   void SetAverageData(char t,int ind,float val);
   float GetValue(char k,char t,int ind,int offset);
 public:
   SyntheticData();                 //default constructor, it initializes counters etc.
   bool InjectSyntheticData(float);   //called by lrloop and hrloop to inject data
   int InitSynth(std::string );     //  function is passed the filename to be opened as a string it calls InitSynth(char *)
   int InitSynth(char*);            //function is passed the filename that contains synthetic data and returns -1 if the file doesn't exits and 1 if it does and is opened
   void registervar(struct v);      // a variable is to be modified by a constanst, make a note of the variable and it's constant value
   void registerfunc(struct v);     // a variable is to be modified by a function, make a note of the variable and also the functions
      
};



//SyntheticData sd;   //hopefully I don't get a multiple definition error
#endif  /* _INJECTSD_H*/


