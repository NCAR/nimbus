/*
-------------------------------------------------------------------------
OBJECT NAME:    psql.c (PostgreS)

FULL NAME:      PostgreSQL database routines.

ENTRY POINTS:   InitSQL()
		WriteSQL()
		Write2dSQL()

STATIC FNS:     none

DESCRIPTION:

REFERENCES:     none

REFERENCED BY:  rtplot.c

NOTES:		

COPYRIGHT:      University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "ctape.h"
#include "vardb.h"
#include "UdpSocket.h"
#include <ctype.h>
#include <libpq-fe.h>

#include <errno.h>
#include <sys/time.h>
#include <unistd.h>

static char	sql_str[30000];		// SQL string
static char	sql2d_str[50000];	// 2D SQL string
static char	brd_str[30000];		// Broadcast string

static PGconn	*conn;

static UdpSocket	brdcst(2101, "128.117.84.255");

extern NR_TYPE	*SampledData, *AveragedData;
extern char	dateProcessed[];	// From netcdf.c

void GetPMS1DAttrsForSQL(RAWTBL *rp, char sql_buff[]);
void SQLcommand(char command[]);

/* -------------------------------------------------------------------- */
bool InitSQL()
{
  int		i, nVars = 0;
  PGresult	*res;
  char		temp[5000], name[32];
printf("InitSQL\n");
  if (Mode == REALTIME)
    brdcst.openSock(UDP_BROADCAST);

  conn = PQconnectdb("");

  /* check to see that the backend connection was successfully made
   */
  if (PQstatus(conn) == CONNECTION_BAD)
  {
    fprintf(stderr, "Connection to database failed, check environment variables.\n");
    fprintf(stderr, "%s", PQerrorMessage(conn));
    PQfinish(conn);
    conn = NULL;
    return(false);
  }

  PQsetnonblocking(conn, true);

  PQclear(PQexec(conn, "DROP TABLE Global_Attributes"));
  PQclear(PQexec(conn, "DROP TABLE Variable_List"));
  PQclear(PQexec(conn, "DROP TABLE PMS1D_list"));
  PQclear(PQexec(conn, "DROP TABLE PMS2D_list"));
  PQclear(PQexec(conn, "DROP TABLE RAF_1hz"));
//  PQclear(PQexec(conn, "DROP TABLE current")); superceeded by globa_attr:endtime

  PQclear(PQexec(conn, "CREATE TABLE Global_Attributes (key char(20) PRIMARY KEY, value char(120))"));
  PQclear(PQexec(conn, "CREATE TABLE Variable_List (Name char(20) PRIMARY KEY, Units char(16), long_name char(80), SampleRateTable char(16), vector_length int, category char(16), missing_value float, data_quality char(16))"));
  PQclear(PQexec(conn, "CREATE TABLE PMS1D_list (Name char(20), SerialNumber char(16), SampleRateTable char(16), FirstBin INT, LastBin INT, CellSizes FLOAT[])"));
  PQclear(PQexec(conn, "CREATE TABLE PMS2D_list (Name char(20), SerialNumber char(16))"));

  strcpy(sql_str, "CREATE TABLE RAF_1hz (datetime timestamp PRIMARY KEY, ");


  /* Add Global Attributes/Flight Data.
   */
  PQclear(PQexec(conn, "INSERT INTO global_attributes VALUES ('Source', 'NCAR Research Aviation Facility')"));
  PQclear(PQexec(conn, "INSERT INTO global_attributes VALUES ('Address', 'P.O. Box 3000, Boulder, CO 80307-3000')"));
  PQclear(PQexec(conn, "INSERT INTO global_attributes VALUES ('Phone', '(303) 497-1030')"));

  char	*p;
  GetAircraft(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('ProjectName', '%s')", p);
  PQclear(PQexec(conn, temp));
  GetProjectNumber(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('ProjectNumber', '%s')", p);
  PQclear(PQexec(conn, temp));
  GetFlightNumber(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('FlightNumber', '%s')", p);
  PQclear(PQexec(conn, temp));
  sprintf(temp, "INSERT INTO global_attributes VALUES ('DateProcessed', '%s')", dateProcessed);
  PQclear(PQexec(conn, temp));


  for (i = 0; i < nsdi; ++i)
    {
    strcat(sql_str, sdi[i]->name);
    strcat(sql_str, " FLOAT, ");
    ++nVars;

    sprintf(temp, "INSERT INTO Variable_List VALUES ('%s', '%s', '%s', 'RAF_%dhz', '%d', '%s', '-32767.0', 'Preliminary')",
	sdi[i]->name, VarDB_GetUnits(sdi[i]->name), VarDB_GetTitle(sdi[i]->name),
	sdi[i]->SampleRate, 1);
    PQclear(PQexec(conn, temp));
    fprintf(stderr, "%s", PQerrorMessage(conn));
    }

  for (i = 0; i < nraw; ++i)
    {
    if (isdigit(raw[i]->name[0]))	// Can't support vars starting with number.
      {
      name[0] = 'A';
      strcpy(&name[1], raw[i]->name);
      }
    else
      strcpy(name, raw[i]->name);

    if (strncmp(raw[i]->name, "A1D", 3) == 0)
      {
      char	name[16];

      strcpy(name, raw[i]->name);
      name[strlen(name)-1] = '\0';

      sprintf(temp, "INSERT INTO PMS2D_list VALUES ('%s', '%s')",
	name, raw[i]->SerialNumber);
      PQclear(PQexec(conn, temp));
      fprintf(stderr, "%s", PQerrorMessage(conn));

      sprintf(temp, "DROP TABLE %s", name);
      PQclear(PQexec(conn, temp));
      sprintf(temp, "CREATE TABLE %s (datetime time, msec int, nSlices int, particle int[])", name);
      PQclear(PQexec(conn, temp));
      fprintf(stderr, "%s", PQerrorMessage(conn));

//      sprintf(temp, "TRUNCATE TABLE %s", name);
//      PQclear(PQexec(conn, temp));
//      fprintf(stderr, "%s", PQerrorMessage(conn));
      }

    if (raw[i]->Length > 1)
      {
      sprintf(temp, "INSERT INTO PMS1D_list VALUES ('%s', '%s', 'RAF_%dhz'",
	&raw[i]->name[1], raw[i]->SerialNumber, raw[i]->SampleRate);

      GetPMS1DAttrsForSQL(raw[i], buffer);
      strcat(temp, buffer);
      strcat(temp, " )");
      PQclear(PQexec(conn, temp));
      fprintf(stderr, "%s", PQerrorMessage(conn));
//printf("%s - %d\n", raw[i]->name, raw[i]->Length);
      }

    strcat(sql_str, name);

    if (raw[i]->Length > 1)
      strcat(sql_str, " FLOAT[], ");
    else
      strcat(sql_str, " FLOAT, ");

    ++nVars;

    sprintf(temp, "INSERT INTO Variable_List VALUES ('%s', '%s', '%s', 'RAF_%dhz', '%d')",
	name, VarDB_GetUnits(raw[i]->name), VarDB_GetTitle(raw[i]->name),
	raw[i]->SampleRate, raw[i]->Length);
    PQclear(PQexec(conn, temp));
    fprintf(stderr, "%s", PQerrorMessage(conn));
    }

  for (i = 0; i < nderive; ++i)
    {
    strcat(sql_str, derived[i]->name);

    if (derived[i]->Length > 1)
      strcat(sql_str, " FLOAT[]");
    else
      strcat(sql_str, " FLOAT");

    if (i != nderive-1)
      strcat(sql_str, ", ");
    ++nVars;

    sprintf(temp, "INSERT INTO Variable_List VALUES ('%s', '%s', '%s', 'RAF_%dhz', '%d')",
	derived[i]->name, VarDB_GetUnits(derived[i]->name),
	VarDB_GetTitle(derived[i]->name), derived[i]->OutputRate, derived[i]->Length);
    PQclear(PQexec(conn, temp));
    fprintf(stderr, "%s", PQerrorMessage(conn));
    }

  strcat(sql_str, ")");
  PQclear(PQexec(conn, sql_str));
  fprintf(stderr, "%s", PQerrorMessage(conn));

/*  Superceeded by global_atrributes:endTime
  PQclear(PQexec(conn, "CREATE TABLE current (lastTime timestamp, cntr INT)"));
  fprintf(stderr, "%s", PQerrorMessage(conn));
  PQclear(PQexec(conn, "INSERT INTO current VALUES ('1970-01-01 00:00:00', 0)"));
  fprintf(stderr, "%s", PQerrorMessage(conn));
*/

  printf("nVars = %d\n", nVars);

  if (Mode == REALTIME)
    {
    PQclear(PQexec(conn,
     "CREATE RULE update AS ON UPDATE TO global_attributes DO NOTIFY current"));
    fprintf(stderr, "%s", PQerrorMessage(conn));
    }

  // Clear out an residual shit after restarting the DB.
  PQclear(PQexec(conn, "VACUUM FULL"));
  fprintf(stderr, "%s", PQerrorMessage(conn));

  return(true);

}	/* END INITSQL */

/* -------------------------------------------------------------------- */
void Start2dSQL(RAWTBL *rp)
{
  strcpy(sql2d_str, "BEGIN;");

}	/* END START2DSQL */

/* -------------------------------------------------------------------- */
void Write2dSQL(RAWTBL *rp, long time, long msec, ulong *p, int nSlices)
{
  int	i;
  char	name[16], temp[128];

  if (conn == NULL || nSlices < 2)
    return;

  strcpy(name, rp->name);
  name[strlen(name)-1] = '\0';

  sprintf(temp, "INSERT INTO %s VALUES ('%02d:%02d:%02d', %u, %d, '{%lu", name, time/3600, (time%3600)/60, time%60, msec, nSlices, p[0]);
  strcat(sql2d_str, temp);

  for (i = 1; i < nSlices; ++i)
    {
    sprintf(temp, ", %d", p[i]);
    strcat(sql2d_str, temp);
    }

  strcat(sql2d_str, "}');");

}	/* END WRITE2DSQL */

/* -------------------------------------------------------------------- */
void Submit2dSQL()
{
  // Overwrite trailing ',' with ending paren.
//  sql2d_str[strlen(sql2d_str)-1] = ')';
//printf("%s\n", sql2d_str);

  strcat(sql2d_str, "COMMIT;");
  SQLcommand(sql2d_str);

}	/* END SUBMIT2DSQL */

/* -------------------------------------------------------------------- */
void WriteSQL(char timeStamp[])
{
  int	i, j, len;
  char	*sql_p, *brd_p, temp[8192];

  static int	cntr = 0;

//printf("WriteSQL\n");

  if (cntr == 0)
    {
    sprintf(temp, "INSERT INTO global_attributes VALUES ('StartTime', '%s')",
	timeStamp);
    SQLcommand(temp);

    sprintf(temp, "INSERT INTO global_attributes VALUES ('EndTime', '%s')",
	timeStamp);
    SQLcommand(temp);
    }

  sprintf(brd_str, "RAF-TS %s ", timeStamp);
  brd_p = &brd_str[strlen(brd_str)];

  sprintf(sql_str, "INSERT INTO RAF_1hz VALUES ('%s', ", timeStamp);
  sql_p = &sql_str[strlen(sql_str)];

  for (i = 0; i < nsdi; ++i)
    {
    sprintf(buffer, "%e,", AveragedData[sdi[i]->LRstart]);
    if (buffer[0] == 'n' || buffer[0] == 'i')
      strcpy(buffer, "-32767.0,");

    strcpy(sql_p, buffer);
    sql_p += strlen(buffer);

    if (sdi[i]->Broadcast)
      {
      strcpy(brd_p, buffer);
      brd_p += strlen(buffer);
      }
    }

  for (i = 0; i < nraw; ++i)
    {
    if (raw[i]->Length > 1)
      {
      strcpy(temp, "'{");
//      sql_p += 2;

      for (j = 0; j < raw[i]->Length; ++j)
        {
        sprintf(buffer, "%e", AveragedData[raw[i]->LRstart+j]);
        if (j != raw[i]->Length-1)
          strcat(buffer, ",");
        strcat(temp, buffer);
//        sql_p += strlen(buffer);
        }

      strcat(temp, "}',");
      len = strlen(temp);
      strcpy(sql_p, temp);
      sql_p += len;

      strcpy(brd_p, &temp[1]);
      brd_p += len-3;
      strcpy(brd_p, ",");
//      sql_p += 3;
      continue;
      }

    sprintf(buffer, "%e,", AveragedData[raw[i]->LRstart]);
    if (buffer[0] == 'n' || buffer[0] == 'i')
      strcpy(buffer, "-32767.0,");

    strcpy(sql_p, buffer);
    sql_p += strlen(buffer);

    if (raw[i]->Broadcast)
      {
      strcpy(brd_p, buffer);
      brd_p += strlen(buffer);
      }
    }

  for (i = 0; i < nderive; ++i)
    {
    if (derived[i]->Length > 1)
      {
      strcpy(temp, "'{");
//      sql_p += 2;

      for (j = 0; j < derived[i]->Length; ++j)
        {
        sprintf(buffer, "%e", AveragedData[derived[i]->LRstart+j]);
        if (j != derived[i]->Length-1)
          strcat(buffer, ",");
        strcat(temp, buffer);
//        sql_p += strlen(buffer);
        }

      strcat(temp, "}',");
      len = strlen(temp);
      strcpy(sql_p, temp);
      sql_p += len;

      strcpy(brd_p, &temp[1]);
      brd_p += len-3;
      strcpy(brd_p, ",");
//      sql_p += 3;
      continue;
      }

    sprintf(buffer, "%e", AveragedData[derived[i]->LRstart]);
    if (buffer[0] == 'n' || buffer[0] == 'i')
      strcpy(buffer, "-32767.0");

    if (i != nderive-1)
      strcat(buffer, ",");

    strcpy(sql_p, buffer);
    sql_p += strlen(buffer);

    if (derived[i]->Broadcast)
      {
      strcpy(brd_p, buffer);
      brd_p += strlen(buffer);
      }
    }

  if (Mode == REALTIME)
    {
    strcpy(brd_p, "\n");
    brdcst.writeSock(brd_str, strlen(brd_str));
//    sprintf(buffer, "); UPDATE current SET lastTime='%s';", timeStamp);
    }
//  else
//    strcpy(buffer, ");");

  sprintf(buffer, "); UPDATE global_attributes SET value='%s' WHERE key='EndTime';", timeStamp);

  strcpy(sql_p, buffer);
  SQLcommand(sql_str);

//  if (++cntr < 10900 && cntr % 3600 == 0)     // every hour, 1st 3 hours
  if (++cntr % 3600 == 0)     // every hour
    {
fprintf(stderr, "Performing ANALYZE @ %s\n", timeStamp);
    SQLcommand("ANALYZE raf_1hz (datetime)");
    }

}	/* END WRITESQL */

/* -------------------------------------------------------------------- */
void SQLcommand(char command[])
{
  PGresult	*res;
//printf("SQLcommand\n");

  if (conn == NULL)
    return;

  while ( (res = PQgetResult(conn)) )
    PQclear(res);

  PQsendQuery(conn, command);
  fprintf(stderr, "%s", PQerrorMessage(conn));

  while ( (res = PQgetResult(conn)) )
    PQclear(res);

//  PQclear(PQexec(conn, command));

}	/* END CLOSESQL */

/* -------------------------------------------------------------------- */
void CloseSQL()
{
  if (conn)
    PQfinish(conn);

}	/* END CLOSESQL */

/* END PSQL.C */
