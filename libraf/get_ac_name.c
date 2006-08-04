/*
-------------------------------------------------------------------------
OBJECT NAME:    get_ac_name.c

FULL NAME:      Get Aircraft Name

DESCRIPTION:    Return a string containing the aircraft name/tail number
		combo which represents the directory name below the project
		name.

COPYRIGHT:      University Corporation for Atmospheric Research, 1996-2006
-------------------------------------------------------------------------
*/

static const char * ac_names[] = {
        "",
        "C130_N130AR",
        "KingAir_N312D",
        "P3_NRL-P3",
        "",
        "",
        "",
        "Saberliner_N307D",
        "Electra_N308D",
        "Sailplane_N9929J"
        };

const char * getAircraftName(int num)
{
  return ac_names[num/100];
}

