//
// Filename:  qc.cc
// Author:    Marc Anderson
// Compiler:  GNU C++ 2.6.0
// Copyright 1995, All Rights Reserved
//
// Description: 
//    this contains the procedures that WINPUT calls to access the validation
//    library
//
// Methods:
//
// Global Functions:
//    qc_rt_init()
//
//    qc_rt_exec()
//
//    qc_rt_dumpstate()
//
//    qc_rt_cleanup()
//
//    qc_pp_init()
//
//    qc_pp_exec()
//
//    qc_pp_cleanup()
//
// Local Functions:
//    rt_parse_configfile()
//
//    rt_parse_sleepmsg()
//

#define QC_C

#include <time.h>
#include <unistd.h>
#include <fstream>
#include "stddefs.h"
#include "val.h"
#include "qc.h"


// path to configuration file -- in new code, construct path dynamically
// with project number, etc.

//static const char *QC_CONFIGFULLPATH = "/home/local/dev/nimbus/src/"
//                                       "qc/vallib/qc-config";  
// (old method -- config file used to be fixed)

static const char *QC_CONFIGNAME       ="qc-config"; 



#ifdef VAL_DISKLOG
// ultimately construct this dynamically like configuration 
static const char *QC_DISKLOGNAME       = "/home/local/dev/nimbus/src/qc/vallib/qc-disklog";
#endif

static const u_int8 QC_CONFIG_COLS      = 80;

static const char QC_CONFIG_COMMENT     = '#'; // must be 1st character of line


// static const char *GUI_MACHINE = "escalante";   // (future remote execution)


#ifdef VAL_USEGUI
// string used by gui to indicate 'sleep all' action
static const char *QC_SLEEPALL = "all";

// string used by gui to indicate system should shut down
static const char *QC_QUIT = "quit";

static boolean quit_received = FALSE;
#endif



//
// static definitions
//

static boolean rt_parse_configfile(void);

#ifdef VAL_USEGUI
static boolean rt_parse_sleepmsg(Sleep_Message &msg);
#endif

static const u_int16 RT_BATCHES_MAXNUM = 200;  
static u_int16 rt_batches_num   = 0;
static ValRealtimeTestbatch rt_batches[RT_BATCHES_MAXNUM];
static boolean global_enable;


int     SearchTable(std::vector<RAWTBL *> &table, const char target[]),
        SearchTable(std::vector<DERTBL *> &table, const char target[]);



///////////////////////////////////////////////////////////////////////////////
//
// realtime mode
//


//
// qc_rt_init()
//
// initialize the validation library for realtime use
//
// return error codes that chris uses..  OK for success or ERR on failure
//
int qc_rt_init(void)
{
    u_int16 batch_index;

#ifdef VAL_DISKLOG
    time_t current_time;
#endif

#ifdef VAL_USEGUI
    std::cout << "QC: GUI use is enabled and will be executed\n";
#else
    std::cout << "QC: GUI is _disabled_\n";
#endif

    std::cout << "QC: winput: reading configuration file:\n";

    if(rt_parse_configfile() == FALSE)
      return ERR;

#ifdef VAL_USEGUI
    // kludge to handle case when gui sends us a quit message, so we don't
    // send it a quit message back.
    quit_received = FALSE;

    // fork off GUI process
    if(!fork())
    { // then child process -- run GUI here
        sleep(1);
	
	// executes gui; defines errors that gui recognizes
        if(execl(VAL_GUIPATH, VAL_GUIPATH,
		 VAL_RANGESTR,
		 VAL_LEVELSTR,
		 VAL_SPIKESTR,
		 VAL_FLATLINESTR,
		 NULL) == -1)
	{
	    std::cerr << "QC: qc_rt_init(): GUI execution failed: path="
		      << VAL_GUIPATH << std::endl;
	    exit(1);
	}

        // haven't got this code to work reliably yet:
        // execl("rsh", VAL_GUIMACHINE ,VAL_GUIPATH);
    }


    // instantiate socket
    val_socket = new Report;

    if(!val_socket)
    {
	std::cerr << "QC: qc_rt_init(): error allocating Report class\n";

	return ERR;    // abort
    }

    if(val_socket->error_constructing)
    {
	std::cerr << "QC: qc_rt_init(): error in socket construction\n";
	return ERR;    // abort 
    }
      

    std::cout << "QC: winput: waiting for gui to come up\n";
    sleep(2); 
#endif


#ifdef VAL_DISKLOG
    val_disklog = new std::ofstream(QC_DISKLOGNAME);

    if(!val_disklog)
    {
	std::cerr << "QC: qc_rt_init():  error allocating disklog class\n";

#ifdef VAL_USEGUI
	delete val_socket;
#endif

	return ERR;   // not a totally graceful exit
    }

    time(&current_time);
    val_disklog << "qc validation library disk log\n";
    val_disklog->form("opened:  %s\n\n", ctime(&current_time));
#endif

    // explicitly enable all validation test batches that were configured
    for(batch_index=0; batch_index<rt_batches_num; ++batch_index)
      rt_batches[batch_index].enabled = TRUE;

    // this flag is disabled when 'snooze all' occurs.
    global_enable = TRUE;

    return OK;    // use chris's return codes
}



//
// qc_rt_exec()
//
// execute validation tests when in realtime mode
//
// above all other code at this level, this needs to be optimized
//
// returns ERR if we're quitting or OK if we're not quitting
//
int qc_rt_exec(void)
{
    u_int16 batch_index, test_index;

    // variables used to look up time from chris's code
    int hour, minute, second;
    int index;
    char current_timestr[SOCK_TSLEN];
    int current_elapsed;


#ifdef VAL_USEGUI
    Sleep_Message *msg;

    // parse sleep messages from the socket code
    while(msg = val_socket->nextSleepMessage())
    {
	if(rt_parse_sleepmsg(*msg) == FALSE)
	{
	    quit_received = TRUE;
	    return ERR;    //  use chris's return codes (this indicates QUIT)
        }
    }
#endif


    // this flag is disabled temporarily when the 'snooze all' feature
    // is used
    if(global_enable)
    {
	// look up UTC time from WINPUT database (needed for last2min counter
        // maintenance) -- this is ugly.  i wish we would have access to 
        // something analogous to time_t.
        if((index = SearchTable(raw, "HOUR")) == ERR)
	{
	    std::cerr << "QC: qc_rt_exec(): couldn't look up HOUR\n";
	    return OK; // not ok, but 'ERR' indicates quit so we can't use it
        }
	else
	  hour = (int) AveragedData[raw[index]->LRstart];

	if((index = SearchTable(raw, "MINUTE")) == ERR)
	{
	    std::cerr << "QC: qc_rt_exec(): couldn't look up MINUTE\n";
	    return OK; // not ok, but 'ERR' indicates quit so we can't use it
        }
	else
	  minute = (int) AveragedData[raw[index]->LRstart];

	if((index = SearchTable(raw, "SECOND")) == ERR)
	{
	    std::cerr << "QC: qc_rt_exec(): couldn't look up SECOND\n";
	    return OK; // not ok, but 'ERR' indicates quit so we can't use it
        }
	else
	  second = (int) AveragedData[raw[index]->LRstart];

	// form time string with 'hour', 'minute', and 'second'
        sprintf(current_timestr, "%02d:%02d:%02d", hour, minute, second);

	// this WINPUT routine returns the number of seconds so far elapsed
        // (in collected data -- not our current time!)
        current_elapsed = GetShmemElapsedTime();


	// go through and execute each testbatch.
        for(batch_index=0; batch_index<rt_batches_num; ++batch_index)
	{
	    ValRealtimeTestbatch &batch = rt_batches[batch_index];

	    if(batch.enabled)
	      for(test_index=0; test_index<batch.num_tests; ++test_index)
		batch.tests[test_index]->test(current_timestr, 
					      current_elapsed);
#ifdef VAL_DIAGS
	    else
	    {
		std::cout << "QC: " << batch.varref->get_name()
		<< ": (tests disabled -- sleeping)\n",
	    }
#endif 
	}
    } // end of 'global_enable' test

#ifdef VAL_USEGUI
    val_socket->flushQ();
#endif

    return OK;    // use chris's return codes  (this indicates not quitting)
}




//
// qc_rt_dumpstate()
//
// dump state of the world (useful for diagnostics and debugging)
//
// (dumps to 'std::cout')
//
void qc_rt_dumpstate(void)
{
    char buffer[200];
    u_int16 batch_index, test_index;
    WinputSRindex sample_index, num_samples;

    for(batch_index=0; batch_index<rt_batches_num; ++batch_index)
    {
	ValRealtimeTestbatch &batch = rt_batches[batch_index];

	// print name and latest value from 'average' database
        sprintf(buffer, "QC: %-10s: avg=[%5.3f] sr=[ ", batch.varref->get_name(),                  batch.varref->get_winput_avg());
	std::cout << buffer;

	// print out entries from 'SR' database
	num_samples = batch.varref->get_winput_SR_freq();

	for(sample_index=0; sample_index<num_samples; ++sample_index)
        {
          sprintf(buffer, "%5.3f ", batch.varref->get_winput_SR(sample_index));
	  std::cout << buffer;
        }

	std::cout << "]\n";

	// print each of the tests and their arguments
	for(test_index=0; test_index<batch.num_tests; ++test_index)
	{
	    std::cout << "     " << batch.tests[test_index]->get_name() << "( ";

	    batch.tests[test_index]->out_params(std::cout);
	    
	    std::cout << " )\n";
	}
    }
}








//
// qc_rt_cleanup()
//
// cleanup the system when in realtime mode
//
void qc_rt_cleanup(void)
{
    u_int16 batch_index, test_index;

#ifdef VAL_DISKLOG
    time_t current_time;
#endif

    for(batch_index=0; batch_index<rt_batches_num; ++batch_index)
    {
	ValRealtimeTestbatch &batch = rt_batches[batch_index];
	
	delete batch.varref;

	for(test_index=0; test_index<batch.num_tests; ++test_index)
	  delete batch.tests[test_index];
	
	// reinitialize count of tests
	batch.num_tests = 0;
    }

    // reinitialize count of batches
    rt_batches_num = 0;


    // total hack -- in termination sequence, send quit message to GUI & make 
    // sure it gets out...  the way to avoid such a hack is to implement
    // a real handshaking protocol.
#ifdef VAL_USEGUI
    if(!quit_received)    // only send if we haven't received a quit message
    {
	Message *socket_msg = new Message;
	u_int8 count = 0;

	strncpy(socket_msg->type, VAL_QUITSTR, SOCK_TYPELEN);
	val_socket->Send(socket_msg);
    
	// don't waste more than 5 seconds trying this..
	while(val_socket->Q.size() && (count++ < 5))
	{
	    val_socket->flushQ();
	    sleep(1);
	}
    }
#endif

    // dispose of socket
#ifdef VAL_USEGUI
    delete val_socket;
#endif

#ifdef VAL_DISKLOG
    time(&current_time);
    val_disklog->form("qc validation library disk log\n");
    val_disklog->form("closed:  %s\n\n", ctime(&current_time));

    delete val_disklog;
#endif
}




//
// rt_parse_configfile()
//
// this reads the configuration file and parses it, forming a table of 
// individual variables mapped to multiple tests
//
// !! this should probably be split up into procedures -- it's monsterous!!
//
static boolean rt_parse_configfile(void)
{
    char varname[QC_CONFIG_COLS];
    char testname[QC_CONFIG_COLS];
    ValTest *newtest;
    ValRealtimeTestbatch *batch;
    u_int16 batch_index;

    // for searching winput databases:
    WinputVarIndex db_index;
    boolean found;
    WinputVarIndex found_index;
    WinputVarType found_db;

    // configuration file..   (old way -- location is static):
    // std::ifstream configfile(strcat(QC_CONFIGDIR, QC_CONFIGNAME));

    // (new way of doing it -- dynamically determine config file pathname):
    char project_configname[100];
    char *project_directory = getenv("PROJ_DIR");
    
    sprintf(project_configname, "%s/%s/%s", project_directory, 
	    proj_number, QC_CONFIGNAME);

    std::cout << "QC: configuration file full path is: "
	<< project_configname << std::endl;

    std::ifstream configfile(project_configname);

    if(!configfile)
    {
	std::cerr << "QC: error opening configuration file!\n";
	return FALSE;
    }


    // parse until we reach EOF
    while(!configfile.eof())
    {      
	configfile >> varname;

	if(varname[0] == QC_CONFIG_COMMENT)
	  // extract comment and ignore
	  configfile.getline(varname, QC_CONFIG_COLS);

	else if(strlen(varname) != 0)  // ignore blank lines
	{
	    // we found variable name -- search for it, etc.

	    // search for variable name token in the three WINPUT databases
            found = FALSE;


	    // search for variable in each of the two WINPUT databases
	    db_index = 0;
	    while((!found) && (db_index < raw.size()))
	      if(strcmp(raw[db_index]->name, varname) == 0)
	      {
		  found = TRUE;
		  found_index = db_index;
		  found_db = WINPUT_RAW;
	      }
	    else
	      ++db_index;

	    // !! do we do any realtime tests on derived variables?
	    db_index = 0;
	    while((!found) && (db_index < derived.size()))
	      if(strcmp(derived[db_index]->name, varname) == 0)
	      {
		  found = TRUE;
		  found_index = db_index;
		  found_db = WINPUT_DERIVED;
	      }
	    else
	      ++db_index;


	    if(!found)
	    {
		std::cerr << "QC: qc_rt_init(): unknown variable in config file: " << varname << std::endl;
		continue;
	    }


	    // at this point look up a suitable testbatch, if one doesn't
	    // exist, allocate a new one
	    batch = NULL;
	    batch_index=0;
	    while((!batch) && (batch_index < rt_batches_num))
	      if((rt_batches[batch_index].varref->get_type() == found_db) &&
		 (rt_batches[batch_index].varref->get_index() == found_index))
	      { // then we found a match!
		  batch = &rt_batches[batch_index]; 
	      }
	      else
		++batch_index;


	    // couldn't find existing batch, initialize a new one
	    if(!batch) 
	    {
		if(rt_batches_num == RT_BATCHES_MAXNUM)
		{
		    std::cerr << "QC: qc_rt_init(): too many testbatches!\n";
		    return FALSE;
		}

		rt_batches[rt_batches_num].varref = new 
		  ValVarref(found_db, found_index);

		if(!rt_batches[rt_batches_num].varref)
		{
		    std::cerr << "QC: qc_rt_init(): error alloc varref memory\n";
		    return FALSE;
		}
		
		batch = &rt_batches[rt_batches_num];

		++rt_batches_num;
	    }


	    std::cout << "QC:   " << varname << " ";


	    // now get validation test type and construct validation test class
	    configfile >> testname;

	    // i can't think of an easy way to do error checking here
	    // (for invalid arguments, etc.) -- probably a way
	    // with c++ streams, but I'm not going to worry about it now


	    // see if this is a range check
	    if(strcmp(testname, VAL_RANGESTR) == 0)
	    {
		WinputDatavalue low_thresh, high_thresh, highprio_delta;

		configfile >> low_thresh >> high_thresh >> highprio_delta;

		if(low_thresh >= high_thresh)
		{
		    std::cout << "test: " << testname << " -- error: invalid rngs! (" << (float)low_thresh << ", " << (float)high_thresh << std::endl;
		    return FALSE;
		}
		else if(highprio_delta < 0)
		{
		    std::cout << "test: " << testname << " -- error: highprio_del negative(" << (float)highprio_delta << ")\n";
		    return FALSE;
		}
		else
		{
		    newtest = new ValTestRange(*(batch->varref), low_thresh, 
					       high_thresh, highprio_delta); 

		    std::cout << "test: " << testname << " (" << low_thresh << ", " << high_thresh << ", " << highprio_delta << ")\n";
		}
	    }

	    // see if this is a flatline check 
	    else if(strcmp(testname, VAL_FLATLINESTR) == 0)
	    {
		ValVariance min_var_low, min_var_high;

		configfile >> min_var_low >> min_var_high;
		
		if(min_var_low <= min_var_high)
		{
		    std::cout << "test: " << testname << " -- error: invalid variances(" << (float)min_var_low << ", " << (float)min_var_high << ")\n";
		    return FALSE;
		}
		else
		{
		    newtest = new ValTestFlatline(*(batch->varref), 
					   min_var_low, min_var_high);

		    std::cout << "test: " << testname << " (" << min_var_low << ", " << min_var_high;
		}
	    }

	    // see if this is a level shift/spike check
            //
	    // !! remember that we don't have a specific spike detection
	    // test as far as the configuration file is concerned, hence,
	    // we don't do anything with QC_SPIKESTR in this function...
	    else if(strcmp(testname, VAL_LEVELSTR) == 0)
	    {
		ValSlope min_slope;
		WinputDatavalue min_shift;
		WinputSRindex spike_peak_maxintvls;
		WinputDatavalue spike_rtn_slack;
		
		configfile >> min_slope >> min_shift >> spike_peak_maxintvls >>
		  spike_rtn_slack;

		if(min_slope < 0)
		{
		    std::cout << "test: " << testname << " -- error: min_slope negative? (" << (float)min_slope << ")\n";
		    return FALSE;
		}
		else if(min_shift < 0)
		{
		    std::cout << "test: " << testname << " -- error: min_shift negative? (" << (float)min_shift << ")\n";
		    return FALSE;
		}
		else if(spike_rtn_slack < 0)
		{
		    std::cout << "test: " << testname << " -- error: rtn_slack negative? (" << (float)spike_rtn_slack << ")\n";
		    return FALSE;
		}
		else
		{
		    newtest = new ValTestLevel(*(batch->varref), min_slope, 
			    min_shift, spike_peak_maxintvls, spike_rtn_slack);

		    std::cout << "test: " << testname << " (" << min_slope << ", " << min_shift << ", " << spike_peak_maxintvls << ", " << spike_rtn_slack << ")\n";
		}
	    }

	    else // invalid realtime validation test type
	    {
		std::cout << "unknown test -- aborting parse operation: " <<
		  testname << std::endl;
		return FALSE;
	    }

	    // make sure memory for test was allocated correctly
	    if(!newtest)
	    {
		std::cerr << "QC: qc_rt_init(): out of mem alloc test object\n";
		return FALSE;
	    }


	    // now add test to batch, if there is space for it
	    if(batch->num_tests == QC_MAX_RT_TESTS)
	    {
		delete(newtest);

		std::cerr << "QC: qc_rt_init(): too many tests for variable [" <<
		  varname << "]\n";

		return FALSE;  // could omit return and just keep going though
	    }
	    else
	      // store away new test
	      batch->tests[batch->num_tests++] = newtest;
	}
    } // end outer parser "while"

    return TRUE; // success
}



#ifdef VAL_USEGUI
//
// rt_parse_sleepmsg()
//
// parse a 'sleep' message that we obtained from the socket code
//
// returns FALSE if gui wants us to quit, else returns TRUE
//
static boolean rt_parse_sleepmsg(Sleep_Message &msg)
{
    boolean new_enable, found;
    u_int16 batch_index;


    // hack:  if user presses 'quit' in GUI, it sends QUIT message and we're
    // supposed to shut down as well..
    if(strcmp(msg.name, QC_QUIT) == 0)
    {
        std::cout << "QC:  rcvd 'quit' msg -- shutting validation system down\n";
	return FALSE;
    }
    

    switch(msg.mode)
    {
      case SLEEP:
	new_enable = FALSE;
	break; 

      case UNSLEEP:
	new_enable = TRUE;
	break;
       
      default:
	std::cerr << "QC: rt_parse_sleepmsg(): invalid sleep mode\n";
	return TRUE;
    }


    // this is a messy kludge compared to the way that was specified in the
    // design spec (i.e. 'sleep all' was enumerated vs "all" passed as 
    // string).  
    if(strcmp(msg.name, QC_SLEEPALL) == 0)
    {
	// do 'sleep all' action -- old way
	global_enable = new_enable;

	// do 'sleep all' action -- old way
        //for(batch_index=0; batch_index<rt_batches_num; ++batch_index)
        //  rt_batches[batch_index].enabled = new_enable;
    }
    else
    {
	// sleep for individual variable -- look up variable first
     
	found = FALSE;
	batch_index = 0;

	// search for variable in rt_batches[] with name of msg.name
        while((!found) && (batch_index<rt_batches_num))
	  if(strcmp(rt_batches[batch_index].varref->get_name(), msg.name) == 0)
	    found = TRUE;
	  else
	    ++batch_index;

	if(found)
	  rt_batches[batch_index].enabled = new_enable;
	else // (not found)
        {   
	    std::cerr << "QC: rt_parse_sleepmsg(): invalid sleep var name:" << 
	      msg.name << "\n";
	    return TRUE;
	}
    }

    return TRUE;
}
#endif



///////////////////////////////////////////////////////////////////////////////
//
// postprocessing mode (!! later  -- not in our senior project?)
//


//
// qc_pp_init()
//
// initialize the validation library for postprocessing use
//
int qc_pp_init(void)
{
    return OK;
}



//
// qc_pp_exec()
//
// execute validation tests when in postprocessing mode
//
int qc_pp_exec(void)
{
    return OK;
}




//
// qc_pp_cleanup()
//
// cleanup the system when in postprocessing mode
//
void qc_pp_cleanup(void)
{
}
