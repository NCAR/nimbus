/*
-------------------------------------------------------------------------
OBJECT NAME:	vdb2ncml.cc

FULL NAME:	Convert VarDB to NcML.

DESCRIPTION:	Converts VarDB, Categories, StandardNames, DerivedNames,
		DependTable, ModVars and AircraftSpecs to NcML.

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netcdf.h>
#include <libgen.h>

#include <raf/vardb.h>

#include <cstdio>
#include <sys/types.h>

#include <netcdf.h>
#include <raf/portable.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>

static char defaultProjDir[1000];
static char projDir[1000];

static const char COMMENT = '#';
static const float fill_value = -32767.0;

extern long	VarDB_nRecords;

/* -------------------------------------------------------------------- */
void checkModVars(FILE* vdb,int varID, const char *varName)
{
  char fileName[500], buffer[1000], name[100];
  strcpy(fileName, defaultProjDir);
  strcat(fileName, "ModVars");

  FILE *fp;
  if ((fp = fopen(fileName, "r")) == NULL)
  {
    return;
  }

  float vals[2];
  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    sscanf(buffer, "%s %f %f", name, &vals[0], &vals[1]);

    if (strcmp(name, varName) == 0)
      fprintf(vdb,"      <modulus_range>%f %f</modulus_range>\n",vals[0],vals[1]);
//      nc_put_att_float(ncid, varID, "modulus_range", NC_FLOAT, 2, vals);
  }

  fclose(fp);
}

/* -------------------------------------------------------------------- */
void checkDerivedNames(FILE* vdb,int ncid, int varID, const char *varName)
{
  char fileName[500], buffer[1000], *p;
  strcpy(fileName, defaultProjDir);
  strcat(fileName, "DerivedNames");

  FILE *fp;
  if ((fp = fopen(fileName, "r")) == NULL)
  {
    return;
  }

  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    p = strtok(buffer, " \t");

    if (strcmp(p, varName) == 0)
    {
      p = strtok(NULL, "\n");
      while (isspace(*p)) ++p;
        fprintf(vdb,"      <derive>%s</derive>\n",p);
        return void();
//      nc_put_att_text(ncid, varID, "derive", strlen(p)+1, p);
    }
  }

  fclose(fp);
}

/* -------------------------------------------------------------------- */
void checkDependencies(FILE* vdb,int ncid, int varID, const char *varName)
{
  char fileName[500], buffer[1000], *p;
  strcpy(fileName, projDir);
  strcat(fileName, "DependTable");

  FILE *fp;
  if ((fp = fopen(fileName, "r")) == NULL)
  {
    return;
  }

  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    p = strtok(buffer, " \t\n");

    if (p && strcmp(p, varName) == 0)
    {
      if ( (p = strtok(NULL, "\n")) )
      {
        while (isspace(*p)) ++p;
          fprintf(vdb,"      <depends>%s</depends>\n",p);
//        nc_put_att_text(ncid, varID, "Dependencies", strlen(p)+1, p);
      }
    //  else
//        nc_put_att_text(ncid, varID, "Dependencies", 1, "");
    }
  }

  fclose(fp);
}
void addHeader(FILE* vdb,std::string name,std::string iter,std::string infoFile)
{
  fprintf(vdb,"  <%s>\n",name.c_str());
  std::fstream cats;
  cats.open(infoFile.c_str());
  std::string raw_line,definition,useless_info,noSpace;
  int i;
  bool added=false;
  while(std::getline(cats,raw_line))
  {
    noSpace="";
    if (raw_line[0]!='#' and !raw_line.empty())
    {
      useless_info="";
      definition="";
      std::stringstream iss(raw_line);
      std::getline(iss,useless_info,',');
      std::getline(iss,definition,',');
      i=0;
      added=false;
      while(definition[i]!='\0'){
       if(definition[i]!=' ' or added==true){
          noSpace+=definition[i];
          added=true;
        }
      i++;
      }
      if (noSpace!="None")
      {
        fprintf(vdb,"    <%s name=\"%s\"/>\n",iter.c_str(),noSpace.c_str());
      }
    }
  }
  fprintf(vdb,"  </%s>\n",name.c_str());
}
/* -------------------------------------------------------------------- */
void schemaMaker(std::string dictionaryLoc)
{
 FILE* sch=fopen("VDBSchema.xsd","w");
 fprintf(sch,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
 fprintf(sch,"   <xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\" attributeFormDefault=\"unqualified\">\n");
 fprintf(sch,"         <xs:element name=\"VarDB\">\n");
 fprintf(sch,"               <xs:complexType>\n");
 fprintf(sch,"                     <xs:sequence>\n");
 fprintf(sch,"                           <xs:element name=\"categories\">\n");
 fprintf(sch,"                                 <xs:complexType>\n");
 fprintf(sch,"                                       <xs:sequence>\n");
 fprintf(sch,"                                             <xs:element name=\"category\" maxOccurs=\"unbounded\">\n");
 fprintf(sch,"                                                   <xs:complexType>\n");
 fprintf(sch,"                                                         <xs:attribute name=\"name\" type=\"xs:string\"></xs:attribute>\n");
 fprintf(sch,"                                                   </xs:complexType>\n");
 fprintf(sch,"                                             </xs:element>\n");
 fprintf(sch,"                                       </xs:sequence>\n");
 fprintf(sch,"                                 </xs:complexType>\n");
 fprintf(sch,"                           </xs:element>\n");
 fprintf(sch,"                           <xs:element name=\"standard_names\">\n");
 fprintf(sch,"                                 <xs:complexType>\n");
 fprintf(sch,"                                       <xs:sequence>\n");
 fprintf(sch,"                                             <xs:element name=\"standard_name\" maxOccurs=\"unbounded\">\n");
 fprintf(sch,"                                                   <xs:complexType>\n");
 fprintf(sch,"                                                         <xs:attribute name=\"name\" type=\"xs:string\"></xs:attribute>\n");
 fprintf(sch,"                                                   </xs:complexType>\n");
 fprintf(sch,"                                             </xs:element>\n");
 fprintf(sch,"                                       </xs:sequence>\n");
 fprintf(sch,"                                 </xs:complexType>\n");
 fprintf(sch,"                           </xs:element>\n");
 fprintf(sch,"                           <xs:element name=\"variableCatalog\">\n");
 fprintf(sch,"                                 <xs:complexType>\n");
 fprintf(sch,"                                       <xs:sequence>\n");
 fprintf(sch,"                                             <xs:element name=\"variable\" maxOccurs=\"unbounded\">\n");
 fprintf(sch,"                                                   <xs:complexType>\n");
 fprintf(sch,"                                                         <xs:sequence>\n");


 std::fstream dictionary;
 dictionary.open(dictionaryLoc.c_str());
 std::string raw_line,named,definition;
 while(std::getline(dictionary,raw_line))
 {
   if (raw_line[0]!='#' and !raw_line.empty())
   {
     named="";
     definition="";
     std::stringstream iss(raw_line);
     std::getline(iss,named,',');
     std::getline(iss,definition,',');
     fprintf(sch,"                                                               <xs:element name=\"%s\" type=\"xs:string\" minOccurs=\"0\" ></xs:element>\n",named.c_str());
   }
 }

 fprintf(sch,"                                                         </xs:sequence>\n");
 fprintf(sch,"                                                         <xs:attribute name=\"name\" type=\"xs:string\"></xs:attribute>\n");
 fprintf(sch,"                                                   </xs:complexType>\n");
 fprintf(sch,"                                             </xs:element>\n");
 fprintf(sch,"                                       </xs:sequence>\n");
 fprintf(sch,"                                 </xs:complexType>\n");
 fprintf(sch,"                           </xs:element>\n");
 fprintf(sch,"                           <xs:element name=\"Dictionary\">\n");
 fprintf(sch,"                                 <xs:complexType>\n");
 fprintf(sch,"                                       <xs:sequence>\n");
 fprintf(sch,"                                             <xs:element name=\"definition\" maxOccurs=\"unbounded\" >\n");
 fprintf(sch,"                                                 <xs:complexType>\n");
 fprintf(sch,"                                                     <xs:simpleContent>\n");
 fprintf(sch,"                                                         <xs:extension base=\"xs:string\">\n");
 fprintf(sch,"                                                             <xs:attribute name=\"name\" type=\"xs:string\"></xs:attribute>\n");
 fprintf(sch,"                                                         </xs:extension>\n");
 fprintf(sch,"                                                     </xs:simpleContent>\n");
 fprintf(sch,"                                                 </xs:complexType>\n");
 fprintf(sch,"                                             </xs:element>\n");
 fprintf(sch,"                                       </xs:sequence>\n");
 fprintf(sch,"                                 </xs:complexType>\n");
 fprintf(sch,"                           </xs:element>\n");
 fprintf(sch,"                     </xs:sequence>\n");
 fprintf(sch,"               </xs:complexType>\n");
 fprintf(sch,"         </xs:element>\n");
 fprintf(sch,"   </xs:schema>\n");
 fclose(sch);
}
/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	i = 1, ncid, timeDim, varID, noDim;
  const char *p;
  char outFile[512];

  if (argc < 2)
  {
    fprintf(stderr, "Usage: vdbdump [-a] [proj_num | VarDB_filename]\n");
    return(1);
  }

  FILE* vdb;
  vdb=fopen("VDB.xml","w");
  fprintf(vdb,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?><VarDB xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"VDBSchema.xsd\">\n");
  addHeader(vdb,"categories","category","Categories");
  addHeader(vdb,"standard_names","standard_name","StandardNames");
  fprintf(vdb,"  <variableCatalog>\n");

  if (InitializeVarDB(argv[i]) == ERR)
  {
    fprintf(stderr, "vdb2xml: Initialize failure.\n");
    return(1);
  }

  strcpy(defaultProjDir, getenv("PROJ_DIR"));
  strcat(defaultProjDir, "/Configuration/raf/");

  if (strchr(argv[i], '/'))
  {
    strcpy(projDir, argv[i]);
    dirname(projDir);
    strcat(projDir, "/");
  }
  else
    projDir[0] = '\0';

  strcpy(outFile, argv[i]);
  strcat(outFile, ".nc");

  if (nc_create(outFile, 0, &ncid) != NC_NOERR)
  {
    fprintf(stderr, "vdb2ncml: Can't create output file %s.\n", outFile);
    return(1);
  }

  for (i = 0; i < VarDB_nRecords; ++i)
  {
    struct var_v2 * vp = &((struct var_v2 *)VarDB)[i];

    if (isdigit(vp->Name[0]))
    {
      continue;
    }

    fprintf(vdb,"    <variable name=\"%s\">\n",vp->Name);
    fprintf(vdb,"      <units>%s</units>\n",vp->Units);
    fprintf(vdb,"      <long_name>%s</long_name>\n",vp->Title);
    if (ntohl(vp->is_analog)!=0)
    {
      fprintf(vdb,"      <is_analog>true</is_analog>\n");
      fprintf(vdb,"      <voltage_range>%d %d</voltage_range>\n",ntohl(vp->voltageRange[0]),ntohl(vp->voltageRange[1]));
      fprintf(vdb,"      <default_sample_rate>%d</default_sample_rate>\n",ntohl(vp->defaultSampleRate));
    }else
    {
      fprintf(vdb,"      <is_analog>false</is_analog>\n");
    }
    if(vp->MaxLimit-vp->MinLimit!=0)
    {
      fprintf(vdb,"      <min_limit>%f</min_limit>\n",ntohf(vp->MinLimit));
      fprintf(vdb,"      <max_limit>%f</max_limit>\n",ntohf(vp->MaxLimit));
    }
  
    p = VarDB_GetCategoryName(vp->Name);
    if (strcmp(p, "None"))
      fprintf(vdb,"      <category>%s</category>\n", p);

    checkModVars(vdb,varID, vp->Name);
    checkDerivedNames(vdb,ncid, varID, vp->Name);
    checkDependencies(vdb,ncid, varID, vp->Name);

    p = VarDB_GetStandardNameName(vp->Name);
    if (strcmp(p, "None"))
      fprintf(vdb,"      <standard_name>%s</standard_name>\n", p);

    if(ntohl(vp->reference)!=0)
    {
      fprintf(vdb,"      <reference>true</reference>\n");
    }else
    {
      fprintf(vdb,"      <reference>false</reference>\n");
    }
    fprintf(vdb,"    </variable>\n");



// What about Spikes & Lags?  Put in aircraft specific?
// What about default global_attrs; coordinates, etc.
  }


  ReleaseVarDB();


  fprintf(vdb,"  </variableCatalog>\n");
  fprintf(vdb,"  <Dictionary>\n");
  std::string dictionaryLocation="Dictionary";
  std::fstream dictionaryNames;
  dictionaryNames.open(dictionaryLocation.c_str());
  
  //Create xml schema
  schemaMaker(dictionaryLocation);

  std::string raw_line,named,definition;
  while(std::getline(dictionaryNames,raw_line))
  {
    if (raw_line[0]!='#' and !raw_line.empty())
    {
      named="";
      definition="";
      std::stringstream iss(raw_line);
      std::getline(iss,named,',');
      std::getline(iss,definition,',');
      //std::cout<<named<<":: "<<definition<<"\n";
      fprintf(vdb,"    <definition name=\"%s\">%s</definition>\n",named.c_str(),definition.c_str());
    }
  }
  fprintf(vdb,"  </Dictionary>\n");
  fprintf(vdb,"</VarDB>");
  fclose(vdb);

  return(0);

}	/* END MAIN */

/* END VDB2NCML.CC */
