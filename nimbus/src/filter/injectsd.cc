//***********************************************/
//  injectsd.cpp
//******************************************/
#ifndef ninjectsdc
#define ninjectsdc


#include <iostream>
#include "nimbus.h"
#include "injectsd.h"

extern NR_TYPE          *AveragedData,*HighRateData,*SampledData;

vars tempvar;   //used with the widgits

extern bool SDF;
extern bool SDC;
extern bool SDP;

extern void FillListWidget();


bool SyntheticData::InjectSyntheticData(float pt)
{
  passedtime=pt;
  if((SDF)&(SDC)&(SDP))  //from file, function, and constant
    {
      CheckTime();
      InjectConstants();
      InjectFunctions();
    }
  else if((SDF)&&(SDC))   //from file and constant
    {
      CheckTime();
      InjectConstants();
    }
  else if((SDC)&&(SDP))    //from file and function
    {
      InjectConstants();
      InjectFunctions();
    }
    else  if(SDF)                   //synthetic data is being injected from a file
    {
  CheckTime();
    }
  if(SDC)                 // synthetic data is being injected via constants
    {
      InjectConstants();
    } 
  if(SDP)                 //data is being modified with functions
    {
      InjectFunctions();
    }
 return 0;
}


int SyntheticData::InitSynth(std::string k)
{
  return InitSynth(k.c_str());
}   


int SyntheticData::InitSynth(char* tt)
{
//this function should probably check to file format ???
  std::string temporary=tt;
  f.open(tt);  
  if (f.is_open())
    {
    SDF=true;
    f>>tf;     //reads the label time

    temp=f.get();   //get the space after time
      
    /* get the variable names and stores them in the array names
     */
    while (temp==' ')       //checks to see if a newline has been reached
      {
      f>>tempv.name;
      varnames.push_back(tempv);
      varcount++;
      temp=f.get();
      }
    
    //end get variable names code

    for (int i=0;i<varcount;i++)
      {
      indexr=SearchTable(raw,varnames[i].name.c_str());

      if(!((indexr==-1)||(indexr==ERR)))
        {
        raw[indexr]->DataQuality="Synthetic";
        raw[indexr]->Dirty=true;
        raw[indexr]->synthtype=sy_file;
        rt.push_back(raw[indexr]);      //push the variable on to the rawtable pointer vector
        varnames[i].type='r';
        varnames[i].index=ir;          //keep track of it's location in the raw  vectors 
        ir++;                          //increment the index counter
        FillListWidget();
        }
      else
        {
        printf("injectsd: Variable not found.\n");
        }
      }
  
    SynthData=true;
    }
  else
    {
    std::cout<<"A valid file name was not entered."<<std::endl;
    f.close();
    return 0;
    }

  return 1;
}



void SyntheticData::ReadLine()
{
  if (!(f.eof()))
  {
    // read the time stamp   
    f>>dummy;
	    
    if(dummy.size()==8)  //not a hirate
      {
      timestamp=(dummy[0]-48)*36000;
      timestamp+=(dummy[1]-48)*3600;
      timestamp+=(dummy[3]-48)*600;  //index 2 is skipped b/c it's the ":"  
      timestamp+=(dummy[4]-48)*60; 
      timestamp+=(dummy[6]-48)*10;  //index 5 is skipped b/c it is the ":"
      timestamp+=(dummy[7]-48);
      }
    if(dummy.size()==10)  // hirate ie 12:23:36:0
      {
      timestamp=(dummy[0]-48)*36000;
      timestamp+=(dummy[1]-48)*3600;
      timestamp+=(dummy[3]-48)*600;  //index 2 is skipped b/c it's the ":"  
      timestamp+=(dummy[4]-48)*60; 
      timestamp+=(dummy[6]-48)*10;  //index 5 is skipped b/c it is the ":"
      timestamp+=(dummy[7]-48);
      timestamp+=(float)((dummy[9]-48)/1000.00);
      }
    if(dummy.size()==11)
      {
      timestamp=(dummy[0]-48)*36000;
      timestamp+=(dummy[1]-48)*3600;
      timestamp+=(dummy[3]-48)*600;  //index 2 is skipped b/c it's the ":"  
      timestamp+=(dummy[4]-48)*60; 
      timestamp+=(dummy[6]-48)*10;  //index 5 is skipped b/c it is the ":"
      timestamp+=(dummy[7]-48);
      timestamp+=(float)(((dummy[9]-48)*10)+(dummy[10]-48))/1000.00;
      }
    if(dummy.size()==12)
      {
      timestamp=(dummy[0]-48)*36000;
      timestamp+=(dummy[1]-48)*3600;
      timestamp+=(dummy[3]-48)*600;  //index 2 is skipped b/c it's the ":"  
      timestamp+=(dummy[4]-48)*60; 
      timestamp+=(dummy[6]-48)*10;  //index 5 is skipped b/c it is the ":"
      timestamp+=(dummy[7]-48);
      timestamp+=(float)(((dummy[9]-48)*100)+((dummy[10]-48)*10)+(dummy[11]-48))/1000.00;
      }
      
    //  finished reading the timestamp
    for (int i=0;i<varcount;i++)
      {
      f>>varnames[i].value;
      }
    }
  else
    f.close();

}


void SyntheticData::CheckTime()
{
  if (!f.eof())
    {
    if (passedtime>timestamp)
    //read until the times match or you've gotten to the end of the synthetic data file;
      {
      ReadLine();
      CheckTime();
      }
    else if(passedtime<timestamp)  //
      {
      //do nothing, don't read from the synth data file, give passed time a chance to catch up
      }
    else if(passedtime==timestamp)
      {
      Inject();
      if (cfg.ProcessingRate() == Config::HighRate)
        {
        ReadhrLine();
        }
      }
    }
}

void SyntheticData::Inject()
{
  temptallyfunc=new float[varcount] ;

 for (int f=0;f<varcount;f++)
   temptallyfunc[f]=0;


  for (int i=0;i<varcount;i++)
    {
    if (varnames[i].type=='r')
      {
      SetAverageData('r',varnames[i].index,varnames[i].value);
	
      if(cfg.ProcessingRate() == Config::HighRate)
        {
        temptallyfunc[i]+=varnames[i].value;

        SetHighData('r',varnames[i].index,0,varnames[i].value);

        if (rt[varnames[i].index]->OutputRate ==rt[varnames[i].index]->SampleRate)
          {
          SetSampleData('r',varnames[i].index,0,varnames[i].value);
          }
        }
      }
    }
}

void SyntheticData:: registervar(struct v ttemp)
{
  SynthData=true;
  SDC=true;
 
  constvarnames.push_back(ttemp);

  if(ttemp.type=='r')
    {
    indexr=SearchTable(raw,ttemp.name.c_str()); 
    raw[indexr]->DataQuality="synthetic";
    raw[indexr]->Dirty=true;
    FillListWidget();
    raw[indexr]->synthtype=sy_constant;
    rt.push_back(raw[indexr]);
    constvarnames[constcount].index=ir;
    ir++;
    }
  constcount++;
}

SyntheticData::SyntheticData()
{
  indexr=-1;
  constcount=0;
  funccount=0;
  varcount=0;
  ir=0;
  temptally=0;
}

void SyntheticData::InjectConstants()
{
  for(int i=0;i<constcount;i++)
    {
    RAWTBL * rp;

    if(constvarnames[i].type=='r')
      {
      rp=rt[constvarnames[i].index];
      SetAverageData('r',constvarnames[i].index,constvarnames[i].value);

      if(cfg.ProcessingRate() == Config::HighRate)
        {
        for(int kk=0;kk<25;kk++)
          {
          SetHighData('r',constvarnames[i].index,kk,constvarnames[i].value);	
          if ((rp->OutputRate ==25)&&( rp->SampleRate==25))
            {
            SetSampleData('r',constvarnames[i].index,kk,constvarnames[i].value);
            }
          else if((rp->OutputRate ==1)&&( rp->SampleRate==1))
            {
            SetSampleData('r',constvarnames[i].index,0,constvarnames[i].value);
            }
          }
        }
      }
    }
}


void SyntheticData::InjectFunctions()
{
  float dummyfloat,df;
  temptally=0;
  for(int i=0;i<funccount;i++)
    {
    RAWTBL *rp;
    if(funcvarnames[i].function== "abs")
      {
      if(funcvarnames[i].type=='r')
        {
        rp=rt[funcvarnames[i].index];
        if(cfg.ProcessingRate() == Config::HighRate)
          {
          for(int a=0;a<25;a++)
            {
            dummyfloat=fabs(GetValue('h','r',funcvarnames[i].index,a));
            SetHighData('r',funcvarnames[i].index,a,dummyfloat);
            temptally+=dummyfloat;
            if ((rp->OutputRate==25)&&(rp->SampleRate==25))
              {
              df=fabs(GetValue('s','r',funcvarnames[i].index,a));
              SetSampleData('r',funcvarnames[i].index,a,df);
              }
            else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
              {
              df=fabs(GetValue('s','r',funcvarnames[i].index,0));
              SetSampleData('r',funcvarnames[i].index,0,df);
              }
            }
            SetAverageData('r',funcvarnames[i].index,temptally/25.0);

          }
        else
          {
          dummyfloat=fabs(GetValue('a','r',funcvarnames[i].index,0));
          SetAverageData('r',funcvarnames[i].index,dummyfloat);
          }
        }
      } //end if abs
    else if(funcvarnames[i].function== "acos")
      {
      if(funcvarnames[i].type=='r')
        {
        rp=rt[funcvarnames[i].index];
        if(cfg.ProcessingRate() == Config::HighRate)
          {
          for(int a=0;a<25;a++)
            {
            dummyfloat=acos(GetValue('h','r',funcvarnames[i].index,a));
            SetHighData('r',funcvarnames[i].index,a,dummyfloat);
            temptally+=dummyfloat;
            if ((rp->OutputRate==25)&&(rp->SampleRate==25))
              {
              df=acos(GetValue('s','r',funcvarnames[i].index,a));
              SetSampleData('r',funcvarnames[i].index,a,df);
              }
            else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
              {
              df=acos(GetValue('s','r',funcvarnames[i].index,0));
              SetSampleData('r',funcvarnames[i].index,0,df);
              }
            }
          SetAverageData('r',funcvarnames[i].index,temptally/25.0);
          }
        else
          {
          dummyfloat=acos(GetValue('a','r',funcvarnames[i].index,0));
          SetAverageData('r',funcvarnames[i].index,dummyfloat);
          }
        }
      }
    else if(funcvarnames[i].function=="asin")
      {
      if(funcvarnames[i].type=='r')
        {
        rp=rt[funcvarnames[i].index];
        if(cfg.ProcessingRate() == Config::HighRate)
          {
          for(int a=0;a<25;a++)
            {
            dummyfloat=asin(GetValue('h','r',funcvarnames[i].index,a));
            SetHighData('r',funcvarnames[i].index,a,dummyfloat);
            temptally+=dummyfloat;
            if ((rp->OutputRate==25)&&(rp->SampleRate==25))
              {
              df=asin(GetValue('s','r',funcvarnames[i].index,a));
              SetSampleData('r',funcvarnames[i].index,a,df);
              }
            else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
              {
              df=asin(GetValue('s','r',funcvarnames[i].index,0));
              SetSampleData('r',funcvarnames[i].index,0,df);
              }
            }
          SetAverageData('r',funcvarnames[i].index,temptally/25.0);
          }
        else
          {
          dummyfloat=asin(GetValue('a','r',funcvarnames[i].index,0));
          SetAverageData('r',funcvarnames[i].index,dummyfloat);
          }
        }
      }
    else if(funcvarnames[i].function=="atan")
      {
      if(funcvarnames[i].type=='r')
        {
        rp=rt[funcvarnames[i].index];
        if(cfg.ProcessingRate() == Config::HighRate)
          {
          for(int a=0;a<25;a++)
            {
            dummyfloat=atan(GetValue('h','r',funcvarnames[i].index,a));
            SetHighData('r',funcvarnames[i].index,a,dummyfloat);
            temptally+=dummyfloat;
            if ((rp->OutputRate==25)&&(rp->SampleRate==25))
              {
              df=atan(GetValue('s','r',funcvarnames[i].index,a));
              SetSampleData('r',funcvarnames[i].index,a,df);
              }
            else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
              {
              df=atan(GetValue('s','r',funcvarnames[i].index,0));
              SetSampleData('r',funcvarnames[i].index,0,df);
              }
            }
          SetAverageData('r',funcvarnames[i].index,temptally/25.0);
          }
        else
          {
          dummyfloat=atan(GetValue('a','r',funcvarnames[i].index,0));
          SetAverageData('r',funcvarnames[i].index,dummyfloat);
          }
        }
      }
    else if(funcvarnames[i].function=="ceil")
      {
      if(funcvarnames[i].type=='r')
        {
        rp=rt[funcvarnames[i].index];
        if(cfg.ProcessingRate() == Config::HighRate)
          {
          for(int a=0;a<25;a++)
            {
            dummyfloat=ceil(GetValue('h','r',funcvarnames[i].index,a));
            SetHighData('r',funcvarnames[i].index,a,dummyfloat);
            temptally+=dummyfloat;
            if ((rp->OutputRate==25)&&(rp->SampleRate==25))
              {
              df=ceil(GetValue('s','r',funcvarnames[i].index,a));
              SetSampleData('r',funcvarnames[i].index,a,df);
              }
            else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
              {
              df=ceil(GetValue('s','r',funcvarnames[i].index,0));
              SetSampleData('r',funcvarnames[i].index,0,df);
              }
            }
            SetAverageData('r',funcvarnames[i].index,temptally/25.0);
          }
        else
          {
          dummyfloat=ceil(GetValue('a','r',funcvarnames[i].index,0));
          SetAverageData('r',funcvarnames[i].index,dummyfloat);
          }
        }
      }
    else if(funcvarnames[i].function=="cos")
      {
      if(funcvarnames[i].type=='r')
        {
        rp=rt[funcvarnames[i].index];
        if(cfg.ProcessingRate() == Config::HighRate)
          {
          for(int a=0;a<25;a++)
            {
            dummyfloat=cos(GetValue('h','r',funcvarnames[i].index,a));
            SetHighData('r',funcvarnames[i].index,a,dummyfloat);
            temptally+=dummyfloat;
            if ((rp->OutputRate==25)&&(rp->SampleRate==25))
              {
              df=cos(GetValue('s','r',funcvarnames[i].index,a));
              SetSampleData('r',funcvarnames[i].index,a,df);
              }
            else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
              {
              df=cos(GetValue('s','r',funcvarnames[i].index,0));
              SetSampleData('r',funcvarnames[i].index,0,df);
              }
            }
          SetAverageData('r',funcvarnames[i].index,temptally/25.0);
          }
        else
          {
          dummyfloat=cos(GetValue('a','r',funcvarnames[i].index,0));
          SetAverageData('r',funcvarnames[i].index,dummyfloat);
          }
        }
      }
    else if(funcvarnames[i].function== "cosh")
      {
      if(funcvarnames[i].type=='r')
        {
        rp=rt[funcvarnames[i].index];
        if(cfg.ProcessingRate() == Config::HighRate)
          {
          for(int a=0;a<25;a++)
            {
            dummyfloat=cosh(GetValue('h','r',funcvarnames[i].index,a));
            SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=cosh(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=cosh(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=cosh(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }


	    
	    }
	  else if(funcvarnames[i].function=="exp")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=exp(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=exp(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=exp(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=exp(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }


	      
	    }
	  else if(funcvarnames[i].function=="fabs")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=fabs(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=fabs(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=fabs(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=fabs(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }


	    }
	  else if(funcvarnames[i].function=="floor")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=floor(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=floor(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=floor(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=floor(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }



	    }
	  else if(funcvarnames[i].function=="ln")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=log(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=log(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=log(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=log(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }


	    }
	  else if(funcvarnames[i].function=="log")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=log10(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=log10(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=log10(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=log10(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }


	    }
	  else if(funcvarnames[i].function=="pow")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=pow(GetValue('h','r',funcvarnames[i].index,a),2);
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=pow(GetValue('s','r',funcvarnames[i].index,a),2);
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=pow(GetValue('s','r',funcvarnames[i].index,0),2);
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=pow(GetValue('a','r',funcvarnames[i].index,0),2);
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }



	    }
	  else if(funcvarnames[i].function=="sin")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=sin(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=sin(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=sin(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=sin(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }


	    }
	  else if(funcvarnames[i].function=="sinh")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=sinh(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=sinh(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=sinh(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=sinh(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }

	    }
	  else if(funcvarnames[i].function=="sqrt")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=sqrt(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=sqrt(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=sqrt(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=sqrt(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }

	      
	    }
 else if(funcvarnames[i].function=="tan")
	    {
		if(funcvarnames[i].type=='r')
		  {
		    rp=rt[funcvarnames[i].index];
		    if(cfg.ProcessingRate() == Config::HighRate)
		      {
			 for(int a=0;a<25;a++)
			  {
			    dummyfloat=tan(GetValue('h','r',funcvarnames[i].index,a));
			    SetHighData('r',funcvarnames[i].index,a,dummyfloat);
			    temptally+=dummyfloat;
			    if ((rp->OutputRate==25)&&(rp->SampleRate==25))
				  { df=tan(GetValue('s','r',funcvarnames[i].index,a));
				    SetSampleData('r',funcvarnames[i].index,a,df);
				  }
			    else  if((rp->OutputRate==1)&&(rp->SampleRate==1))
				  { df=tan(GetValue('s','r',funcvarnames[i].index,0));
				    SetSampleData('r',funcvarnames[i].index,0,df);
				  }
			  }
			 SetAverageData('r',funcvarnames[i].index,temptally/25.0);

		      }
		    else
		      {
			 dummyfloat=tan(GetValue('a','r',funcvarnames[i].index,0));
			 SetAverageData('r',funcvarnames[i].index,dummyfloat);
		      }
		  }

	    }
    }}



void SyntheticData::registerfunc(struct v ttemp)
{
  SynthData=true;
  SDP=true;
  funcvarnames.push_back(ttemp);

  if(ttemp.type=='r')
    {
    indexr=SearchTable(raw,ttemp.name.c_str());
    raw[indexr]->DataQuality="synthetic";
    raw[indexr]->synthtype=sy_function;
    rt.push_back(raw[indexr]);
    funcvarnames[funccount].index=ir;
    ir++;
    }

  funccount++;
}

void SyntheticData::ReadhrLine()
{    
  for(int w=1;w<25;w++)
    {
    ReadLine();
    if(passedtime==(int)timestamp)
      Injecthr(w);
    else
      {
      std::cout<<"25 milliseconds worth of data was not provided"<<std::endl;
      exit(1);
      }
    }
}

void SyntheticData::Injecthr(int m)
{
  for(int i=0;i<varcount;i++)
    {
    if(varnames[i].type=='r')
      {
      if(cfg.ProcessingRate() == Config::HighRate)
        {
        SetHighData('r',varnames[i].index,m,varnames[i].value);

        temptallyfunc[i]+=varnames[i].value; 

        if (	(rt[varnames[i].index]->OutputRate == 25) &&
		(rt[varnames[i].index]->SampleRate == 25))
          SetSampleData('r',varnames[i].index,m,varnames[i].value);

        if (m==24)
          SetAverageData('r',varnames[i].index,temptallyfunc[i]/25.00); 
        }
      }
    }
}

void SyntheticData::SetHighData(char t,int ind,int offset,float val)  
{
  if (t == 'r')
    HighRateData[(rt[ind]->HRstart)+offset]=val;

  /*
ind= the current variables index in the appropriate table
offset= generally 0, however if a highrate data is being processed it will be 0-24 for the 25 milliseconds
val= the value that is to be stored

*/

}


void SyntheticData::SetSampleData(char t,int ind,int offset,float val)
{
  if(t == 'r')
    SampledData[(rt[ind]->SRstart)+offset]=val;
}

void SyntheticData::SetAverageData(char t,int ind,float val)
{
  if(t == 'r')
    AveragedData[rt[ind]->LRstart]=val;
}

float SyntheticData::GetValue(char k,char t,int ind,int offset)
{
  float z;
  if(k=='h')//from the highrate 
    {
    if(t=='r')
      z = HighRateData[(rt[ind]->HRstart)+offset];
    }
  else  if(k=='a')//from the averagerate
    {
    if(t=='r')
      z = AveragedData[(rt[ind]->LRstart)];
    }
  else  if(k=='s')//from the samplerate
    {
    if(t=='r')
      z= SampledData[(rt[ind]->SRstart)+offset];
    }

  return z;

}

#endif

// END  INJECTSD.CPP
