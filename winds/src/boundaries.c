/* boundaries.c -- draw in geographic, political boundaries as designated
   in database
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <xview/xview.h>

#include <constants.h>
#include <track.h>
#include <macros.h>
#include <files.h>
#include <proto.h>

static int TotalNumberOfLines;
static float *LatLonData=0;
static int *CoordsInLine=0;


/************************  INIT_BOUNDARY_DATA()  *****************************/

init_boundary_data(char boundary_filename[])
{
FILE *WINDSfopen(),*boundaries;
float ValuesInSection; /* number of individual float values (lat or lon) in line 
		- declared float because data file is hosed, pads with
		float zeroes when we expect to read in an integer! */
int latlon;	/* a (lat,lon) pair of float values */
int lineid;
float maxlat,minlat,maxlon,minlon;
int TotalIncludedValues;	/* total number of float values (lat or lon) in file */
float latitude,longitude;
int SizeOfLatLonData,ValuesInCurrentLine;
static float MaxLat, MaxLon, MinLat, MinLon;
static char KeyWord[32];
#define MaxRecordLength 256
static char record[MaxRecordLength+1];
enum modes {USA,World,WorldGeoPol};
static int mode=USA;
/*
read in file defining the region rectangle to draw
*/
 if ( (boundaries=WINDSfopen(boundary_filename,"r")) == NULL) {
  (void)fprintf(stderr,"WINDS Terrain: no '%s' file found; ergo no Terrain\n",
   boundary_filename);
  return;
 }
 while (fgets(record,MaxRecordLength,boundaries)!=NULL) {
  if (record[0]!='#') {
   if (StringInString("USA",record,FALSE)) {
    mode=USA;
    fprintf(stderr,"WINDS Terrain: using USA geo-political outlines only\n"); 
   } else if (StringInString("WorldGeoPol",record,FALSE)) {
    mode=WorldGeoPol;
    fprintf(stderr,"WINDS Terrain: using World geo-political outlines\n"); 
   } else if (StringInString("World",record,FALSE)){
    mode=World;
    fprintf(stderr,"WINDS Terrain: using World geographical outlines only\n"); 
   } else {
    sscanf(record,"%s %f %s %f %s %f %s %f",
     KeyWord,&MaxLat,KeyWord,&MaxLon,KeyWord,&MinLat,KeyWord,&MinLon);
    fprintf(stderr,
     "WINDS Terrain: MinLat %6.1f MaxLat %6.1f MinLon %6.1f MaxLon %6.1f\n",
     MinLat,MaxLat,MinLon,MaxLon); 
   }
  }
 }
 (void)WINDSfclose(boundaries);
/*
now get geopolitical database ready for perusal
*/
 (void)sprintf(boundary_filename,COASTLINES,ProjectDirectory);
 if ( (boundaries=WINDSfopen(boundary_filename,"r")) == NULL) {
  (void)fprintf(stderr,"WINDS Terrain: no '%s' file found; ergo no Terrain\n",
    boundary_filename);
  return;
 }
 if (LatLonData)
  free((char *)LatLonData);
 if (CoordsInLine)
  free((char *)CoordsInLine);
/* 
read through geopolitical database 
*/
 TotalIncludedValues=SizeOfLatLonData=0;
 while (fscanf(boundaries,"%f",&ValuesInSection)!=EOF) {
/*
skip sections that have zero for number of values...
*/
  while (ValuesInSection==0)
   if (fscanf(boundaries,"%f",&ValuesInSection)==EOF) {
    (void)WINDSfclose(boundaries);
    return;
   }
  (void)fscanf(boundaries,"%d %f %f %f %f",
  &lineid,&maxlat,&minlat,&maxlon,&minlon);
/*
extract only those sections that have at least the possibility of having
a point or more within the defined rectangle. It appears that using only
those sections with line-ID of 2 gives good resolution geo-political outlines 
w/o the clutter of the coarse outlines.
*/
  if (((mode==USA && lineid==2)
   ||  (mode==World && lineid==1)
   ||  (mode==WorldGeoPol && (lineid==3 || lineid==1)))
   &&  (maxlat>MinLat && minlat<MaxLat && maxlon>MinLon && minlon<MaxLon)){
/*
allocate or re-allocate memory as needed
*/
   if (LatLonData) {
    if (SizeOfLatLonData <= TotalIncludedValues+(int)ValuesInSection) {
     LatLonData=(float *)realloc((void *)LatLonData,
      (size_t)((TotalIncludedValues+(int)ValuesInSection)*sizeof(float)));
     SizeOfLatLonData=TotalIncludedValues+ValuesInSection;
    }
   } else {
    LatLonData=(float *)malloc((unsigned)((int)ValuesInSection*sizeof(float)));
    SizeOfLatLonData=ValuesInSection;
   }
/*
examine each point in section for inclusion in defined rectangle
*/
   ValuesInCurrentLine=0;
   for (latlon=0; latlon<(int)ValuesInSection/2; latlon++) {
    fscanf(boundaries,"%f %f", &latitude, &longitude);
    if (within_boundaries(latitude,longitude,MinLat,MinLon,MaxLat,MaxLon)) {
     LatLonData[TotalIncludedValues]=latitude;
     LatLonData[TotalIncludedValues+1]=longitude;
     ValuesInCurrentLine+=PTS_PER_COORD;
     TotalIncludedValues+=PTS_PER_COORD;
    } else {
     if (ValuesInCurrentLine) {
/*
a line has been started, but now we've gone outside it; time to start a new
one
*/
      ChangeLines(ValuesInCurrentLine);  
      ValuesInCurrentLine=0;
     }
    }
   }
/*
end of points in current section; change lines
*/
   ChangeLines(ValuesInCurrentLine);  
  } else {
/*
it's not possible that this section has any points in defined rectangle. Scan
thru its points to get to next section instead. 
*/
   for (latlon=0; latlon<(int)ValuesInSection/2; latlon++)
    fscanf(boundaries,"%f %f",&latitude,&longitude);
  }
 }
/*
done
*/
 (void)WINDSfclose(boundaries);
}

/************************  ChangeLines()  *****************************/

ChangeLines(int numvalues)
{
 if (!numvalues)
  return;
 TotalNumberOfLines++;
 if (CoordsInLine)
  CoordsInLine=(int *)realloc((void *)CoordsInLine,
   (size_t)(TotalNumberOfLines*sizeof(int)));
 else
  CoordsInLine=(int *)malloc((unsigned)(sizeof(int)));
 CoordsInLine[TotalNumberOfLines-1]=(int)numvalues/2;
}

/************************  REFRESH_BOUNDARIES()  *****************************/

refresh_boundaries(int quadrant)
{
 set_boundary_context(quadrant,FALSE);
 draw_boundaries(quadrant);
 (*UnsetClipping[GetDrawType()])();
 (*SetOffset[GetDrawType()])(0,0);
}

/************************  DRAW_BOUNDARIES()  ********************************/

draw_boundaries(int quadrant)
{
int totalcoords,coord_,line;
int x,y,dummy=0;
XPoint *ptlist;
/***
static int debug=0, Debug=0;
int savex,savey,maxx,maxxy,maxy,maxyx,Maxx=0,Maxy=0;
***/
 
 totalcoords=0;
/***
 if (debug || Debug) {
  fprintf(stderr,"\n\nDEBUG: draw %d boundary lines\n",TotalNumberOfLines);
 }
***/
 for (line=0; line<TotalNumberOfLines; line++) {
/***
  if (Debug) {
   maxx=maxy=0;
  }
***/
  ptlist=(XPoint *)
   (malloc((unsigned)(sizeof(short)*PTS_PER_COORD*CoordsInLine[line])));
  for (coord_=0; coord_<CoordsInLine[line]; coord_++) {

/* assuming data is in (lat,lon) order, and that latitude is on the y-axis
   ...also, boundary data is plotted against the 0'th lat/lon variables...
*/
   y= (*GetYOffset[GetDrawType()])() +
    get_y_coord(quadrant,0,LatLonData[totalcoords+LAT_COORD]);
   x=(*GetXOffset[GetDrawType()])() +
    get_x_coord(quadrant,LatLonData[totalcoords+LON_COORD]);
/***
   if (debug) { savex=x; savey=y; }
***/
/*
elements of ptlist are short ints, but return values from get_y_coord and
get_x_coord can easily exceed limits of that datatype. Check for that here
before assigning something to ptlist members that will be misunderstood.
*/
   X11LimitCoords(&x,&y,&dummy,&dummy);
/***
   if (debug) {
    if (savex!=x || savey!=y)
     fprintf(stderr,"limited (%d,%d) to (%d,%d)\n",savex,savey,x,y);
    if (ABS(y) > Maxy) Maxy=y; if (ABS(x) > Maxx) Maxx=x;
   }
   if (Debug) {
    if (ABS(y) > maxy) { maxy=y; maxyx=x; }
    if (ABS(x) > maxx) { maxx=x; maxxy=y; }
   }
***/
   ptlist[coord_].x=x; ptlist[coord_].y=y;
   totalcoords+=PTS_PER_COORD;
  }
  if (ptlist) {
/***
   if (Debug) {
    fprintf(stderr,
     "extremes of %d coordinates in line %d: (%d,%d) and (%d,%d)\n",
     CoordsInLine[line],line,maxx,maxxy,maxy,maxyx);
   }
***/
   (*DrawPolyline[GetDrawType()])(CoordsInLine[line],ptlist);
   free((char *)ptlist);
  }
 }
/***
 if (debug) {
  fprintf(stderr,"extreme coordinates: (%d,%d)\n",Maxx,Maxy);
 }
***/
}

