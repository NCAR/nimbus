/*
 * The purpose of this program is to collect and archive images from 
 * IEEE1394 scientific cameras.
 */

#include <libpq-fe.h>	//for postgres
#include <stdio.h>
#include <time.h> 		//for timestamp
#include <stdint.h>
#include <unistd.h> 	//for sleep, fork
#include <sys/wait.h> 	//for waitpid
#include <signal.h> 	//for catching kill signals
#include <string.h>
#include <dc1394/dc1394.h>
#include <stdlib.h>	
#include <inttypes.h>
#include "getIMG.h"

#define CONFIG_FILE "cameras.conf"
#define FILE_PREFIX "/scr/rafcam/flight_number_"

void finishUp();
void getTime(char*);
int initPostgres(PGconn *, camConf_t **, int);
int updatePostgres(PGconn *, const char *, long long, int); 

/* global var for interrupt handling */
int keepGoing = 1;

int main(int argc, char *argv[])
{
	/* declare vars */
    int i, useDB = 1, night=0;
    char image_file_name[100], directory[100], *flNum, timeStr[20];

	PGconn *conn; 					//postgresql database connection
	pid_t cpid;						//PID for timing process
	dc1394_t * d;					//dc1394 base class (for scanning the bus)
	dc1394camera_t * resCam;		//dc1394 camera class, used here for reset
    dc1394camera_list_t * list; 	
    dc1394error_t err;			
	
	/* get flight number */
	flNum = argv[1];

	/* set up libdc1394 opject */    	
    d = dc1394_new ();
    if (!d)
        return 1;
    err=dc1394_camera_enumerate (d, &list);
    DC1394_ERR_RTN(err,"Failed to enumerate cameras");

	/* check to make sure there is at least one camera on the bus */
    if (list->num == 0) {
        dc1394_log_error("No cameras found");
        return 1;
    }

	/* relase any old bandwith allocations */
	resCam = dc1394_camera_new (d, list->ids[0].guid);
    if (!resCam) {
        dc1394_log_error("Failed to initialize first camera [with guid %llx]",
						 list->ids[0].guid);
        return 1;
    }
	dc1394_iso_release_all(resCam);
    dc1394_camera_free(resCam); 

	/* allocate memory for struct pointers for each cam */
	camConf_t **camArray;
	camArray = malloc(sizeof(camConf_t*)*list->num); 

	/* fill structs with data from config file */
	for (i=0; i<list->num; i++){
		camArray[i] = malloc(sizeof(camConf_t)); //allocate config structures
		if (!getConf(CONFIG_FILE, list->ids[i].guid, camArray[i])){
			printf("Warning: no settings for camera: %llx in file: %s,using defaults.\n",
					list->ids[i].guid, CONFIG_FILE); 
		}
		strcpy(camArray[i]->flNum, flNum); 
	}

	/* connect to postgres db */
	conn = PQconnectdb("host=lenado.eol.ucar.edu dbname=real-time user=ads");
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s",
				 PQerrorMessage(conn));
    	PQfinish(conn);
		printf("continuing without DB updates\n");
	    useDB = 0;
    } else printf("connected to db\n");

	/* set up postgres db */
	if(useDB) initPostgres(conn, camArray, list->num);

	/* make image directory structure */
	sprintf(directory, "mkdir %s%s &> /dev/null",FILE_PREFIX, flNum); 
	system(directory);
	for (i=0; i<list->num; i++){
		sprintf(directory, "mkdir %s%s/%s &> /dev/null", FILE_PREFIX, 
				flNum, camArray[i]->direction); 
		system(directory);
	}

	/* set up handler for SIGINT (ctrl-c) signal */
	signal(SIGINT, &finishUp);

	/* keep getting pictures until SIGINT signal or night detection */
	while(keepGoing && night<10){

		/* spawn child process that waits one second, then die */
		if( !(cpid = fork()) ) {
			sleep(1);
			exit(0);
		}

		/* main process continues here, where it will
		   get image(s) and then wait for child process to die */
		getTime(timeStr);
		for (i=0; i<list->num; i++){
			sprintf(image_file_name, "%s%s/%s/%s", FILE_PREFIX, flNum, 
					camArray[i]->direction, timeStr); 
			night += getIMG(image_file_name, camArray[i], d);
			if(useDB) updatePostgres(conn, timeStr, camArray[i]->guid, night);
		}
		printf("night: %d\n",  night);
		waitpid(cpid, NULL, 0);
	}

	/* Will get here after SIGINT signal, or night detection
	   clean up memory and exit*/
	if (night>10) printf ("night detected, shutting down\n");
	else printf("sigINT signal captured, exiting cleanly\n");

	for (i=0; i<list->num; i++)
		free(camArray[i]);
	free(camArray);
	dc1394_camera_free_list (list);
	if(useDB) PQfinish(conn);

    return 0;
}

void finishUp(){
/* This function will be set as the SIGINT signal handler
   It simply breaks the main loop so that we can exit cleanly */
	keepGoing = 0;
}

void getTime(char* s){
/* This function returns the a string formatted with the date and time
   in the style YYMMDD-HHMMSS */
	struct tm *t;
	time_t rawtime;

	time(&rawtime);
	t = localtime(&rawtime);
//	t = gmtime(&rawtime);

	/* make time string; note that tm_mon is 0-11, not 1-12 */
	sprintf(s, "%2.2d%2.2d%2.2d-%2.2d%2.2d%2.2d", t->tm_year%100, t->tm_mon+1,
					 t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	return;
}

int initPostgres(PGconn * conn, camConf_t **camArray, int numCams){
/* This function clears the old camera table and sets up a new one */

	int i;
	char command[100];
	PGresult *res;

	/* drop existing camera table */
	res = PQexec(conn, "DROP TABLE camera");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "drop table failed: %s", PQerrorMessage(conn));
        PQclear(res);
	} else printf("old table dropped\n");

	/* create new camera table */
	res = PQexec(conn,
		 "CREATE TABLE camera ( guid text, direction text, latest text, night text)");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "create table failed: %s", PQerrorMessage(conn));
        PQclear(res);
	} else printf("new table created\n");

	/* fill in rows */
	for (i=0; i<numCams; i++) {
		sprintf(command, "INSERT INTO camera VALUES ('%llx', '%s', '0', '0')", 
				camArray[i]->guid, camArray[i]->direction);
		res = PQexec(conn, command);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    	    fprintf(stderr, "add row failed: %s", PQerrorMessage(conn));
        	PQclear(res);
		} else printf("new row added\n");
	}

    PQclear(res);
	return 1;
}

int updatePostgres(PGconn *conn, const char * img_name, long long guid, int night) {
/* This function updates the database with the newest image, etc */

	char command[100];
	PGresult *res;

	sprintf(command, "UPDATE camera SET latest ='%s.jpg', night='%d' WHERE guid = '%llx'",
			 img_name, night, guid);
	res = PQexec(conn, command);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "update table failed: %s", PQerrorMessage(conn));
        PQclear(res);
	} //else printf("table updated\n"); 
	
    PQclear(res);
	return 1;
}


