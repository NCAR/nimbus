/*
 * Program to read real-time flight database periodically (say 60 seconds)
 * and create a KML for google earth.
 *
 * COPYRIGHT: University Corporation for Atmospheric Research, 2005-2015
 */

#include "ACTrackDriver.hh"

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  ACTrackDriver driver;

  return driver.main(argc, argv);
}
