#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

//simple structure to hold the bounding box lat/lon
typedef struct {
	double n,e,s,w; 
} bounds;

//struct to hold data parsed out of the input kml file
typedef struct {
	bool good;
	string name;
	double lat, lon;
	string description;
} placemark;

//function that returns the string contained inside of a cdata tag
string getInnerCDATA(string in) {
	int start, npos;
	start = in.find("<![CDATA[") + 9; //9 chars, '<![CDATA['
	npos = in.find("]]") - start;
	
	return in.substr(start,npos);
}

//function that returns the string contained inside of the element specified
//  by 'match', given an input string
string getInnerElement(string in, string match) {
	int start, npos;
	start = in.find("<"+match+">") + match.size() + 2; //2 chars, '<' and '>'
	npos = in.find("</"+match+">") - start;
	
	return in.substr(start,npos);
}

//function that returns the string contained inside of the element specified
// by 'match', given a filestream
string getElement(ifstream *file, string match) {

	string curline, full("");
	int state = 0;

	do {
		getline(*file, curline);
		if ( curline.find("<"+match+">")<curline.size() )
			state = 1;
		if ( (state==1) && (curline.find("</"+match+">")<curline.size()) )
			state = 2;

		if (! file->eof() ) {
			full += curline;
		} else {
			return "";
		}
		
	} while ( state < 2 );

	return getInnerElement(full, match);
}

//function that returns a placemark struct w/ data from input kml
placemark parsePlacemark(ifstream *file) {

	float junk;
	placemark p;
	string pData;

	pData = getElement(file, "Placemark");
	p.good = (pData.size() > 0);

	if (p.good) {	
		p.name = getInnerElement(pData, "name");
		p.description = getInnerCDATA(getInnerElement(pData, "description"));
		sscanf( getInnerElement(pData, "coordinates").c_str(), 
			"%lf,%lf,%f", 
			&p.lon, &p.lat, &junk);
	}

	return p;
}

//function that checks to see if the placemark, p is inside the bounding box, b
bool inBounds(placemark p, bounds b) {

	bool retval = false;
	if ( 
		(b.s < p.lat) && (p.lat < b.n) &&
		(b.w < p.lon) && (p.lon < b.e) 
	) retval = true;

	return retval;
}

//function that outputs correct KML for a google earth placemark
void outputPlacemark(ofstream *fout, placemark p) {
	*fout 
	<<"		<Placemark>\n"
	<<"			<name>" << p.name << "</name>\n"
	<<"			<description>\n"
	<<"<![CDATA[" << p.lat << "," << p.lon << "<br><br>" << p.description << "]]>\n" 
	<<"			</description>\n"
/*
	<<"			<LookAt>\n"
	<<"				<longitude>25.819942</longitude>\n"
	<<"				<latitude>-18.089461</latitude>\n"
	<<"				<altitude>0</altitude>\n"
	<<"				<range>200000</range>\n"
	<<"				<tilt>0</tilt>\n"
	<<"				<heading>1</heading>\n"
	<<"			</LookAt>\n"
*/
	<<"			<styleUrl>#default_copy0</styleUrl>\n"
	<<"			<Point>\n"
	<<"				<coordinates>\n"
	<<"				" << p.lon << "," << p.lat << ",0\n"

	<<"				</coordinates>\n"
	<<"			</Point>\n"
	<<"		</Placemark>\n";

}

//function that outputs correct KML headers, and style
void outputKMLstart(ofstream *fout, bounds bbox) {

	*fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
	<< "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n"
	<< "<Document>\n"
	<< "        <name>VORs ["
		<< bbox.n 
		<< "," << bbox.e 
		<< " " << bbox.s 
		<< "," << bbox.w 
	<<"]</name>\n"
	<< "        <Style id=\"default_copy0\">\n"
	<< "                <IconStyle>\n"
	<< "                        <color>ff0000ff</color>\n"
	<< "                        <scale>1.4</scale>\n"
	<< "                        <Icon>\n"
	<< "                                <href>http://maps.google.com/mapfiles/kml/shapes/triangle.png</href>\n"
	<< "                        </Icon>\n"
	<< "                </IconStyle>\n"
	<< "                <LabelStyle>\n"
	<< "                        <color>ff0055ff</color>\n"
	<< "                </LabelStyle>\n"
	<< "        </Style>\n"
	<< "        <Folder>\n"
	<< "                <name>DAFIF Navaid data</name>\n"
	<< "                <description>Created using Digital Aeronautical Flight Information File by Karl Naef</description>\n"
/*
	<< "                <LookAt>\n"
	<< "                        <longitude>9.099314912699301</longitude>\n"
	<< "                        <latitude>47.59889434313599</latitude>\n"
	<< "                        <altitude>0</altitude>\n"
	<< "                        <range>52674.45404210446</range>\n"
	<< "                        <tilt>1.690747985393871e-14</tilt>\n"
	<< "                        <heading>5.045075268829454</heading>\n"
	<< "                </LookAt>\n"
*/
	<<"\n";
}

//function to output KML that closes the file
void outputKMLend(ofstream *fout) {
	*fout << "	</Folder>\n"
	<<"</Document>\n"
	<<"</kml>";
}

//function that prints some help for the user
void printHelp(string s) {
	cout << "usage:\n" << s << " <input file> <output file> [<bounding box>] \n\tBounding Box values in this order: n e s w\n\t where (+) values => NE, (-) values => SW\n";
}

//function that prints out a bounding box to the screen
void printBBOX(bounds b) {
	cout << "N: " << b.n
	<< ", E: " << b.e
	<< ", S: " << b.s
	<< ", W: " << b.w;
}

int main(int argc, char *argv[]) {

	int count=0, rejects=0; //number of placemarks allowed, rejected
	placemark p;        //placemark structure used to hold parsed data
	bounds b;           //bounds structure to hold desired bounding box

	/* verify input arguments */
	if (argc == 3) {
		//no bounding box desired, maximize lat/lon
		b.n = 90; 
		b.e = 180; 
		b.s = -90; 
		b.w = -180; 
	} else if (argc == 7) {
		//read bounding box into b, if specified
		b.n = atof(argv[3]);
		b.e = atof(argv[4]);
		b.s = atof(argv[5]);
		b.w = atof(argv[6]);
	} else {
		printHelp(argv[0]);
		return 1;
	}

	/* open and test filestreams */
	ifstream infile(argv[1]);	
	ofstream outfile(argv[2]);

	if (!outfile.is_open()) {
		cout << "failed to open output file: " << argv[2] << "\n";
		return 1;
	}

	if (!infile.is_open()) {
		cout << "failed to open input file: " << argv[1] << "\n";
		return 1;
	}

	/* construct reduced KML file */
	outputKMLstart(&outfile, b);
	p.good = true;
	while (p.good) {	
		p = parsePlacemark(&infile);

		/* exclude anything w/ NDB in name or outside the bounding box, b */
		if (p.name.npos == p.name.find("NDB") && inBounds(p, b) ) {

			/* custom: remove VOR from name */
			p.name = p.name.substr(0, p.name.find(" VOR"));

			outputPlacemark(&outfile, p);
			count++;
		} else {
			rejects++;
		}
	} 
	outputKMLend(&outfile);

	//cout <<"rejected " << rejects << " placemarks\n";
	cout<<"wrote " << argv[2] << ", with " << count << " placemarks\n";

	infile.close();
	outfile.close();
	return 0;
}

