# file KML.py
#
"Produces a kml file from the track as defined in ModuleConstructor.Track."
# Strategy here is to produce two .kml files, one that references
# google.com and one that references acserver.raf.ucar.edu, the latter
# for use on the aircraft to avoid remote connections to google.com
# in flight. The latter is named PlanAC.kml, the former Plan.kml.
#
# This is awkward code that writes many things repeatedly where I'm sure
# there is an efficient way to do this. Someday should clean this up --
# but it works, so leave it for now. It was copied from a Google-Earth-
# constructed representation of the track, so I'm just taking all the
# kml that was in that file and duplicating it without understanding what
# I'm doing...

import Specs
WaypointNumber = 0
KMLFileName = 'Plan.kml'
lonx = Specs.TakeoffLocation()[0]
latx = Specs.TakeoffLocation()[1]
galtx = Specs.TakeoffLocation()[2]
# header info for .kml file
def KMLHeader(KMLFileName):
    "Opens the file and writes the required header."
#					XXXX fix this
    global WaypointNumber		# changed here so needs to be global
    KMLACFileName = KMLFileName.replace ('Plan', 'PlanAC')
    print 'kml file name: ', KMLFileName, ', new name is: ', KMLACFileName
    KMLFile = open(KMLFileName,'w')
    KMLACFile = open(KMLACFileName,'w')
    KMLFile.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
    KMLFile.write("<kml xmlns=\"http://earth.google.com/kml/2.2\">\n")
    KMLFile.write("<Document>\n")
# might need to replace .kml with .kmz here?
    KMLFile.write("\t <name>"+KMLFileName+"</name>\n")
    KMLFile.write("\t<StyleMap id=\"msn_triangle_copy1\">\n")
    KMLFile.write("\t\t<Pair>\n")
    KMLFile.write("\t\t\t<key>normal</key>\n")
    KMLFile.write("\t\t\t<styleUrl>#sn_triangle_copy1"\
                  + "</styleUrl>\n")
    KMLFile.write("\t\t</Pair>\n")
    KMLFile.write("\t\t<Pair>\n")
    KMLFile.write("\t\t\t<key>highlight</key>\n")
    KMLFile.write("\t\t\t<styleUrl>#sh_triangle_copy1"\
                  +"</styleUrl>\n")
    KMLFile.write("\t\t</Pair>\n")
    KMLFile.write("\t</StyleMap>\n")
    KMLFile.write("\t <Style id=\"sh_triangle_copy1\">\n")
    KMLFile.write("\t\t <IconStyle>\n")
    KMLFile.write("\t\t\t <color>ff0000ff</color>\n")
    KMLFile.write("\t\t\t <scale>0.8</scale>\n")
    KMLFile.write("\t\t\t <Icon>\n")
#   KMLFile.write("\t\t\t\t <href>http://acserver.raf.ucar.edu/flight_data/display/triangle.png</href>\n")
    KMLFile.write("\t\t\t\t <href>http://maps.google.com/mapfiles/kml/shapes/placemark_square.png</href>\n")
    KMLFile.write("\t\t\t </Icon>\n")
    KMLFile.write("\t\t </IconStyle>\n")
    KMLFile.write("\t\t <LabelStyle>\n")
    KMLFile.write("\t\t\t <color>ff0000ff</color>\n")
    KMLFile.write("\t\t </LabelStyle>\n")
    KMLFile.write("\t\t <LineStyle>\n")
    KMLFile.write("\t\t\t <color>ff00aaff</color>\n")
    KMLFile.write("\t\t\t <width>2</width>\n")
    KMLFile.write("\t\t </LineStyle>\n")
    KMLFile.write("\t\t <ListStyle>\n")
    KMLFile.write("\t\t </ListStyle>\n")
    KMLFile.write("\t </Style>\n")
    KMLFile.write("\t <Style id=\"sn_triangle_copy1\">\n")
    KMLFile.write("\t\t <IconStyle>\n")
    KMLFile.write("\t\t\t <color>ff0000ff</color>\n")
    KMLFile.write("\t\t\t <scale>0.8</scale>\n")
    KMLFile.write("\t\t\t <Icon>\n")
#   KMLFile.write("\t\t\t\t <href>http://acserver.raf.ucar.edu/flight_data/display/triangle.png</href>\n")
    KMLFile.write("\t\t\t\t <href>http://maps.google.com/mapfiles/kml/shapes/placemark_square.png</href>\n")
    KMLFile.write("\t\t\t </Icon>\n")
    KMLFile.write("\t\t </IconStyle>\n")
    KMLFile.write("\t\t <LabelStyle>\n")
    KMLFile.write("\t\t\t <color>ff0000ff</color>\n")
    KMLFile.write("\t\t </LabelStyle>\n")
    KMLFile.write("\t\t <LineStyle>\n")
    KMLFile.write("\t\t\t <color>ff00aaff</color>\n")
    KMLFile.write("\t\t\t <width>2</width>\n")
    KMLFile.write("\t\t </LineStyle>\n")
    KMLFile.write("\t\t <ListStyle>\n")
    KMLFile.write("\t\t </ListStyle>\n")
    KMLFile.write("\t </Style>\n")
    KMLACFile.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
    KMLACFile.write("<kml xmlns=\"http://earth.google.com/kml/2.2\">\n")
    KMLACFile.write("<Document>\n")
# might need to replace .kml with .kmz here?
    KMLACFile.write("\t <name>"+KMLACFileName+"</name>\n")
    KMLACFile.write("\t<StyleMap id=\"msn_triangle_copy1\">\n")
    KMLACFile.write("\t\t<Pair>\n")
    KMLACFile.write("\t\t\t<key>normal</key>\n")
    KMLACFile.write("\t\t\t<styleUrl>#sn_triangle_copy1"\
                  + "</styleUrl>\n")
    KMLACFile.write("\t\t</Pair>\n")
    KMLACFile.write("\t\t<Pair>\n")
    KMLACFile.write("\t\t\t<key>highlight</key>\n")
    KMLACFile.write("\t\t\t<styleUrl>#sh_triangle_copy1"\
                  +"</styleUrl>\n")
    KMLACFile.write("\t\t</Pair>\n")
    KMLACFile.write("\t</StyleMap>\n")
    KMLACFile.write("\t <Style id=\"sh_triangle_copy1\">\n")
    KMLACFile.write("\t\t <IconStyle>\n")
    KMLACFile.write("\t\t\t <color>ff0000ff</color>\n")
    KMLACFile.write("\t\t\t <scale>0.8</scale>\n")
    KMLACFile.write("\t\t\t <Icon>\n")
#   KMLACFile.write("\t\t\t\t <href>http://acserver.raf.ucar.edu/flight_data/display/triangle.png</href>\n")
    KMLACFile.write("\t\t\t\t <href>http://acserver.raf.ucar.edu/flight_data/display/placemark_square.png</href>\n")
    KMLACFile.write("\t\t\t </Icon>\n")
    KMLACFile.write("\t\t </IconStyle>\n")
    KMLACFile.write("\t\t <LabelStyle>\n")
    KMLACFile.write("\t\t\t <color>ff0000ff</color>\n")
    KMLACFile.write("\t\t </LabelStyle>\n")
    KMLACFile.write("\t\t <LineStyle>\n")
    KMLACFile.write("\t\t\t <color>ff00aaff</color>\n")
    KMLACFile.write("\t\t\t <width>2</width>\n")
    KMLACFile.write("\t\t </LineStyle>\n")
    KMLACFile.write("\t\t <ListStyle>\n")
    KMLACFile.write("\t\t </ListStyle>\n")
    KMLACFile.write("\t </Style>\n")
    KMLACFile.write("\t <Style id=\"sn_triangle_copy1\">\n")
    KMLACFile.write("\t\t <IconStyle>\n")
    KMLACFile.write("\t\t\t <color>ff0000ff</color>\n")
    KMLACFile.write("\t\t\t <scale>0.8</scale>\n")
    KMLACFile.write("\t\t\t <Icon>\n")
    KMLACFile.write("\t\t\t\t <href>http://acserver.raf.ucar.edu/flight_data/display/placemark_square.png</href>\n")
#   KMLACFile.write("\t\t\t\t <href>http://maps.google.com/mapfiles/kml/shapes/triangle.png</href>\n")
    KMLACFile.write("\t\t\t </Icon>\n")
    KMLACFile.write("\t\t </IconStyle>\n")
    KMLACFile.write("\t\t <LabelStyle>\n")
    KMLACFile.write("\t\t\t <color>ff0000ff</color>\n")
    KMLACFile.write("\t\t </LabelStyle>\n")
    KMLACFile.write("\t\t <LineStyle>\n")
    KMLACFile.write("\t\t\t <color>ff00aaff</color>\n")
    KMLACFile.write("\t\t\t <width>2</width>\n")
    KMLACFile.write("\t\t </LineStyle>\n")
    KMLACFile.write("\t\t <ListStyle>\n")
    KMLACFile.write("\t\t </ListStyle>\n")
    KMLACFile.write("\t </Style>\n")
    WaypointNumber = 0
    return(KMLFile, KMLACFile)

def KMLclose(KMLFile, KMLACFile):
    "Adds trailer to the .kml file and then closes it."
    KMLFile.write("</Document>\n")
    KMLFile.write("</kml>\n")
    KMLFile.close()
    KMLACFile.write("</Document>\n")
    KMLACFile.write("</kml>\n")
    KMLACFile.close()

def PlotPoints (KMLFile, KMLACFile, points):
    "Plot the set of points on the .kml file"
    KMLFile.write("\t <Placemark>\n")
    KMLFile.write("\t\t <styleUrl>#msn_triangle_copy1</styleUrl>\n")
    KMLFile.write("\t\t <LineString>\n")
    KMLFile.write("\t\t\t <tessellate>1</tessellate>\n")
    KMLFile.write("\t\t\t <coordinates>\n")
    for x in points:
        KMLFile.write ("\t\t\t\t " + format (x[0], 'f') + ','\
                      + format (x[1], 'f') + ','\
                      + format (x[2], 'f') + ' \n')
    KMLFile.write("\t\t\t </coordinates>\n")
    KMLFile.write("\t\t\t <altitudeMode>absolute</altitudeMode>\n")
    KMLFile.write("\t\t </LineString>\n")
    KMLFile.write("\t </Placemark>\n")
    KMLACFile.write("\t <Placemark>\n")
    KMLACFile.write("\t\t <styleUrl>#sh_triangle_copy1</styleUrl>\n")
    KMLACFile.write("\t\t <LineString>\n")
    KMLACFile.write("\t\t\t <tessellate>1</tessellate>\n")
    KMLACFile.write("\t\t\t <coordinates>\n")
    for x in points:
        KMLACFile.write ("\t\t\t\t " + format (x[0], 'f') + ','\
                      + format (x[1], 'f') + ','\
                      + format (x[2], 'f') + ' \n')
    KMLACFile.write("\t\t\t </coordinates>\n")
    KMLACFile.write("\t\t </LineString>\n")
    KMLACFile.write("\t </Placemark>\n")

def PlotWaypoint (KMLFile, KMLACFile, wp, label='', symbol = 'triangle'):
    "Adds waypoint symbol to the .kml file for plotting on Google Earth etc."
#		Copy from a Google-Earth-generated example
#		(I don't understand all this; it's just copied verbatim here.
#		 It's likely this could be made more compact.)
    global WaypointNumber, lonx, latx, galtx
#		These are global because they are saved in order to
#		draw lines from the last point to this one.
    longitude = wp[0]
    latitude  = wp[1]
    altitude  = wp[2]
    WaypointNumber += 1
    if (label == ''): label="WP"+format(WaypointNumber,'d')
    KMLFile.write("\t <Placemark>\n")
    KMLFile.write("\t\t <name>"+label+"</name>\n")
    KMLFile.write("\t\t <description>WayPoint "\
                  +format(round(altitude/(100))*100.,'.0f')+' ft'+"</description>\n")
    KMLFile.write("\t\t <styleUrl>#msn_triangle_copy1"\
                  +"</styleUrl>\n")
    KMLFile.write("\t\t <Point>\n")
    KMLFile.write("\t\t\t <coordinates>"+format(longitude,'f')\
        +','+format(latitude,'f')+','+format(altitude,'f')+"</coordinates>\n")
    KMLFile.write("\t\t\t <altitudeMode>absolute</altitudeMode>\n")
    KMLFile.write("\t\t </Point>\n")
    KMLFile.write("\t </Placemark>\n")
    KMLFile.write("\t <Placemark>\n")
    KMLFile.write("\t\t <name>"+"Path"+format(WaypointNumber,'d')+"</name>\n")
    KMLFile.write("\t\t <styleUrl>#msn_triangle_copy1</styleUrl>\n")
    KMLFile.write("\t\t <LineString>\n")
    KMLFile.write("\t\t\t <tessellate>1</tessellate>\n")
    KMLFile.write("\t\t\t <coordinates>\n")
    KMLFile.write("\t\t\t\t "+format(lonx,'f')+','+format(latx,'f')+','\
                  +format(galtx,'f')+' '+format(longitude,'f')+','\
                  +format(latitude,'f')+','+format(altitude,'f')+'\n')
#   print 'Waypoint'+format(WaypointNumber,'d')+' '+format(longitude, '.2f')\
#          +','+format(latitude, '.2f')+',' + format(round(altitude/100.)*100., '.0f')
    KMLFile.write("\t\t\t </coordinates>\n")
    KMLFile.write("\t\t\t <altitudeMode>absolute</altitudeMode>\n")
    KMLFile.write("\t\t </LineString>\n")
    KMLFile.write("\t </Placemark>\n")
    KMLACFile.write("\t <Placemark>\n")
    KMLACFile.write("\t\t <name>"+label+"</name>\n")
    KMLACFile.write("\t\t <description>WayPoint "\
                  +format(round(altitude/(100))*100.,'.0f')+' ft'+"</description>\n")
    KMLACFile.write("\t\t <styleUrl>#msn_triangle_copy1"\
                  +"</styleUrl>\n")
    KMLACFile.write("\t\t <Point>\n")
    KMLACFile.write("\t\t\t <coordinates>"+format(longitude,'f')\
        +','+format(latitude,'f')+','+format(altitude,'f')+"</coordinates>\n")
    KMLACFile.write("\t\t </Point>\n")
    KMLACFile.write("\t </Placemark>\n")
    KMLACFile.write("\t <Placemark>\n")
    KMLACFile.write("\t\t <name>"+"Path"+format(WaypointNumber,'d')+"</name>\n")
    KMLACFile.write("\t\t <styleUrl>#msn_triangle_copy1</styleUrl>\n")
    KMLACFile.write("\t\t <LineString>\n")
    KMLACFile.write("\t\t\t <tessellate>1</tessellate>\n")
    KMLACFile.write("\t\t\t <coordinates>\n")
    KMLACFile.write("\t\t\t\t "+format(lonx,'f')+','+format(latx,'f')+','\
                  +format(galtx,'f')+' '+format(longitude,'f')+','\
                  +format(latitude,'f')+','+format(altitude,'f')+'\n')
#   print 'Waypoint'+format(WaypointNumber,'d')+' '+format(longitude, '.2f')\
#          +','+format(latitude, '.2f')+',' + format(round(altitude/100.)*100., '.0f')
    KMLACFile.write("\t\t\t </coordinates>\n")
    KMLACFile.write("\t\t </LineString>\n")
    KMLACFile.write("\t </Placemark>\n")
    lonx = longitude
    latx = latitude
    galtx = altitude
    return()

