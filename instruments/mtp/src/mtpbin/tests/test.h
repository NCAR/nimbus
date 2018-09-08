/*! \file test.h
 * \brief Set all constants for tests in one location
 **
 ** HIPPO-5 values were determined by running the VB program MTPBin in the debugger
 ** and checking input and output values there.  
 */

// HIPPO-5
float flightLevelsKmH[]={14.5,13.0,12.0,11.0,10.0,9.0,8.0,7.0,6.0,5.0,4.0,3.0,2.0,1.0,0.0};
std::string RCFdirH = "../RCF/";
int numFlightLevelsH=(sizeof(flightLevelsKmH)/sizeof(float));


// HIPPO-5 test 1
// ob(1)-ob(30) values for scan at 152700 HIPPPO-5 flight date 20110809
std::string FlightDescH1 = "HIPPO-5 flt on 8/9/2011 at 152700 hours";
double ACAltKmH1 = 12.49947; // Aircraft Altitude (km)
// Brightness Temperatures from MTP scan
std::vector<float> scanBTsH1 = {210.9473, 213.5502, 215.0358, 216.6606, 217.8942,
                   219.3694, 219.8724, 221.0873, 223.0352, 225.3323,
                   214.6268, 214.9759, 216.3311, 217.3704, 218.7520, 
                   219.3694, 219.3790, 219.9641, 220.7292, 222.0580,
                   216.1557, 216.8213, 217.1181, 217.9902, 218.5839,
                   219.3694, 219.3111, 219.1711, 219.6915, 220.3420};

// HIPPO-5 test 2
// Now a low level leg of the flight to assure altitudes turn out ok
double ACAltKmH2 = 2.205186;
std::vector<float> scanBTsH2 = {295.9447, 294.9844, 295.2653, 295.7357, 295.5644,
                   293.9797, 292.0251, 292.5367, 293.3295, 293.7696,
                   297.2932, 297.3831, 296.7744, 296.6493, 295.8458,
                   293.9797, 292.5577, 294.1493, 295.0679, 295.0130,
                   296.9786, 296.3578, 296.4440, 296.1351, 295.6343,
                   293.9797, 292.0367, 292.6659, 293.1209, 293.1770};

// HIPPO-5 test 3
// ob(1)-ob(30) values for scan at 220214 HIPPPO-5 flight date 20110901
std::string FlightDescH3 = "HIPPO-5 flt on 9/1/2011 at 220214 hours";
double ACAltKmH3 = 2.050817;
std::vector<float> scanBTsH3 = {262.1815, 262.0143, 262.2418, 263.3536, 264.2274,
                   264.8418, 264.9377, 265.8375, 266.8653, 268.4430,
                   263.2054, 263.5550, 263.7445, 264.3099, 264.4827,
                   264.8418, 264.9814, 265.5672, 266.2898, 267.8371,
                   262.9489, 263.1401, 263.4613, 264.2195, 264.6548,
                   264.8418, 265.1116, 265.6754, 266.2920, 267.0385};

/*****************************************************************************/
// DEEPWAVE test 1 20140606 (RF01) 06:33:33 (23613)
/*****************************************************************************/

//*** For calibrator test ***//
// From the CAL file
std::vector<float> cnd0={86.926,76.711,80.198};
std::vector<float> gof={40.6,0.0,0.0};
std::vector<float> gec1={19.87,23.10,26.05};
std::vector<float> gec2={-0.1,-0.1,-0.1};

// From the RAW file
float OAT = 222.53; // Outside air temperature (Kelvin) from the raw file A line.
// The raw file stores the scan as [3 channels, 10 angles], but the code needs [10 angles, 3 channels].
// Manually invert array before typing here.
std::vector<int> ScanCounts = {17103,17128,17146,17178,17215,17255,17285,17315,17363,17402,
    18227,18238,18257,18295,18331,18369,18381,18407,18431,18459,
    17524,17537,17546,17568,17592,17614,17640,17661,17690,17716};

std::vector<int> PltWireCnts= {2173,13809,13808,7809,13344,13296,13230,14456};
std::vector<int> TargetCnts= {20806,22251,22084,19101,20508,20031};


//*** For retreiver test ***//
// Found flightLevelsKm in /h/eol/haggerty/data/DEEPWAVE/Boon/Setup/DEEPWAVE_RCS.txt.
float flightLevelsKmD[]={14.5,13.5,13.0,12.5,12.0,11.0,9.0,7.0,5.0,3.5,2.0,1.0,0.01};
std::string RCFdirD = "../DEEPWAVE/RCF/"; /// Directory where RCF files can be found.
int numFlightLevelsD=(sizeof(flightLevelsKmD)/sizeof(float));

double ACAltKmD1 = 9.305; // from PALT(time)
// calculated from counts using calibrator.cc
std::vector<float> scanBTsD1 = { 214.913, 216.166, 217.068, 218.671, 220.526,
	           222.53, 224.033, 225.537, 227.942, 229.896,
	           216.405, 216.88, 217.699, 219.338, 220.891,
	           222.53, 223.048, 224.169, 225.204, 226.412,
	           219.086, 219.584, 219.928, 220.77, 221.688,
	           222.53, 223.525, 224.328, 225.438, 226.433};

/*****************************************************************************/
// DEEPWAVE test 2 20140625 (RF10) 08:10:26 (29426)
/*****************************************************************************/

//*** For calibrator test ***//
// From the CAL file
//std::vector<float> cnd0={87.941,77.026,81.603};
//std::vector<float> gof={40.6,0.0,0.0};
//std::vector<float> gec1={19.17,22.47,25.17};
//std::vector<float> gec2={-0.1,-0.1,-0.1};

// From the RAW file
//float OAT = 220.39; // Outside air temperature (Kelvin) from the raw file A line.
// The raw file stores the scan as [3 channels, 10 angles], but the code needs [10 angles, 3 channels].
// Manually invert array before typing here.
//std::vector<int> ScanCounts = {17157,17206,17221,17246,17227,17299,17194,17198,17207,17213,18428,18448,18454,18466,18455,18448,18444,18432,18417,18411,17662,17679,17674,17671,17692,17748,17671,17635,17627,17609};

//std::vector<int> PltWireCnts= {2170,13804,13798,6804,13395,13360,13126,14460};
//std::vector<int> TargetCnts= {20809,22379,22186,19118,020636,020133};


//*** For retreiver test ***//
// Found flightLevelsKm in /h/eol/haggerty/data/DEEPWAVE/Boon/Setup/DEEPWAVE_RCS.txt.
//float flightLevelsKmD[]={14.5,13.5,13.0,12.5,12.0,11.0,9.0,7.0,5.0,3.5,2.0,1.0,0.01};
//std::string RCFdirD = "../DEEPWAVE/RCF/"; /// Directory where RCF files can be found.
//int numFlightLevelsD=(sizeof(flightLevelsKmD)/sizeof(float));

//double ACAltKmD1 = 12.181; // from PALT(time)
// calculated from counts using calibrator.cc
//std::vector<float> scanBTsD1 = { 212.996,215.548,216.329,217.63,216.641,220.39,214.923,215.131,215.6,215.912,219.501,220.39,220.657,221.19,220.701,220.39,220.212,219.679,219.013,218.746,216.978,217.652,217.454,217.335,218.168,220.39,217.335,215.907,215.589,214.875};

/*****************************************************************************/
// CSET test 1 20150701 (RF01) 21:00:14 (75614)
/*****************************************************************************/

//*** For calibrator test ***//
// From the CAL file
//std::vector<float> cnd0={85.685,76.213,79.617};
//std::vector<float> gof={40.6,0.0,0.0};
//std::vector<float> gec1={19.17,22.47,25.17};
//std::vector<float> gec2={-0.1,-0.1,-0.1};

// From the RAW file
//float OAT = 248.06;
//std::vector<int> ScanCounts = {17636, 17674, 17702, 17740, 17783, 17822, 17848, 17872, 17900, 17940,
//    18957, 18977, 18999, 19041, 19076, 19117, 19131, 19155, 19166, 19193,
//    18247, 18276, 18296, 18311, 18340, 18372, 18393, 18411, 18438, 18457};
//std::vector<int> PltWireCnts = {2166,13820,13808,9440,13391,13396,13196,14448};
//std::vector<int> TargetCnts= {20774,22421,22220,19144,20683,20166};

// Can't find flight levels, so use DEEPWAVE levels to start
//float flightLevelsKmC[]={14.5,13.5,13.0,12.5,12.0,11.0,9.0,7.0,5.0,3.5,2.0,1.0,0.01};
//std::string RCFdirC = "../CSET/RCF/"; /// Directory where RCF files can be found.
//int numFlightLevelsC=(sizeof(flightLevelsKmC)/sizeof(float));

//double ACAltKmC1 = 8.206;
// calculated from counts using calibrator.cc
//std::vector<float> scanBTsC1 = {238.371,240.351,241.809,243.789,246.028,248.06,249.414,250.665,252.123,254.207,240.948,241.837,242.815,244.682,246.238,248.06,248.682,249.749,250.238,251.438,243.099,244.25,245.044,245.639,246.79,248.06,248.893,249.608,250.679,251.433};
