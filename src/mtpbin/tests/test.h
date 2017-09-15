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


// DEEPWAVE test 1
float flightLevelsKmD[]={14.5,13.5,13.0,12.5,12.0,11.0,9.0,7.0,5.0,3.5,2.0,1.0,0.01};
std::string RCFdirD = "../DEEPWAVE/RCF/"; /// Directory where RCF files can be found.
int numFlightLevelsD=(sizeof(flightLevelsKmD)/sizeof(float));

double ACAltKmD1 = 9.305;
// calculated from counts using calibrator.cc
std::vector<float> scanBTsD1 = { 214.913, 216.166, 217.068, 218.671, 220.526,
	           222.53, 224.033, 225.537, 227.942, 229.896,
	           216.405, 216.88, 217.699, 219.338, 220.891,
	           222.53, 223.048, 224.169, 225.204, 226.412,
	           219.086, 219.584, 219.928, 220.77, 221.688,
	           222.53, 223.525, 224.328, 225.438, 226.433};
