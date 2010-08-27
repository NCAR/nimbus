/*
 * Scan an AVAPS D file for the first line item with a user supplied
 * pressure and ouput a KML placemark containing that information.
 * This works in conjunction with a wrapper script.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void outputKML(float lat, float lon, float wd, float ws)
{
  int iws = 0;

  if (ws > 2.5 && ws < 7.5) iws = 5;
  if (ws >= 7.5 && ws < 12.5) iws = 10;
  if (ws >= 12.5 && ws < 17.5) iws = 15;
  if (ws >= 17.5 && ws < 22.5) iws = 20;
  if (ws >= 22.5 && ws < 27.5) iws = 25;
  if (ws >= 27.5 && ws < 32.5) iws = 30;
  if (ws >= 32.5 && ws < 37.5) iws = 35;
  if (ws >= 37.5 && ws < 42.5) iws = 40;
  if (ws >= 42.5 && ws < 47.5) iws = 45;
  if (ws >= 47.5 && ws < 52.5) iws = 50;
  if (ws >= 52.5 && ws < 57.5) iws = 55;
  if (ws >= 57.5 && ws < 62.5) iws = 60;
  if (ws >= 62.5 && ws < 67.5) iws = 65;
  if (ws >= 67.5 && ws < 72.5) iws = 70;
  if (ws >= 72.5 && ws < 77.5) iws = 75;
  if (ws >= 77.5 && ws < 82.5) iws = 80;
  if (ws >= 82.5 && ws < 87.5) iws = 85;
  if (ws >= 87.5 && ws < 92.5) iws = 90;
  if (ws >= 92.5 && ws < 97.5) iws = 95;
  if (ws >= 97.5 && ws < 102.5) iws = 100;
  if (ws >= 102.5 && ws < 105.5) iws = 105;

  printf("  <Placemark>\n");
  printf("    <description><![CDATA[WD: %f<br>WS: %f]]></description>\n", wd, ws);
  printf("    <Style>\n");
  printf("      <IconStyle>\n");
  printf("        <color>ffffaa55</color>\n");
  printf("        <scale>3</scale>\n");
//  printf("        <heading>0</heading>\n");
  printf("        <Icon>\n");
  printf("          <href>http://acserver.raf.ucar.edu/flight_data/display/windbarbs/%03d/wb_%03d_%03d.png</href>\n", iws, iws, (int)wd);
  printf("        </Icon>\n");
//  printf("        <gx:headingMode>worldNorth</gx:headingMode>\n");
  printf("      </IconStyle>\n");
  printf("    </Style>\n");
  printf("    <Point>\n");
  printf("      <coordinates>%f,%f,0</coordinates>\n", lon, lat);
  printf("    </Point>\n");
  printf("  </Placemark>\n");
}

int main(int argc, char *argv[])
{
  int i;
  char buffer[2048];
  char *p;
  float wd, ws, lat, lon;
  float ps_cutoff = 850.0;

  if (argc > 1)
    ps_cutoff = atof(argv[1]);

  // Get away from begning of file.
  for (i = 0; i < 1000; ++i)
    fgets(buffer, 2048, stdin);

  while (fgets(buffer, 2048, stdin) )
  {
    if (strncmp(buffer, "AVAPS-D0", 8))
      continue;

    p = strtok(buffer, " ");
    p = strtok(NULL, " ");
    if (strncmp(p, "S0", 2))
      continue;

    // Skip to air pressure.
    for (i = 0; i < 4; ++i)
      p = strtok(NULL, " ");

    if (atof(p) > ps_cutoff && atof(p) < 1500)
    {
      p = strtok(NULL, " ");
      p = strtok(NULL, " ");
      p = strtok(NULL, " ");
      wd = atof(p);
      p = strtok(NULL, " ");
      ws = atof(p);
      p = strtok(NULL, " ");
      p = strtok(NULL, " ");
      lon = atof(p);
      p = strtok(NULL, " ");
      lat = atof(p);

      if (lon > 200 || lat > 90 || wd > 360 || ws > 300)
        continue;

      outputKML(lat, lon, wd, ws);
      exit(0);
    }
  }

  return 1;
}
