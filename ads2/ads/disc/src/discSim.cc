/*
-------------------------------------------------------------------------
OBJECT NAME:	discSim.cc

FULL NAME:	DSM Simulator

DESCRIPTION:	

INPUT:		

OUTPUT:		

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include <signal.h>
#include <stdio.h>
#include <time.h>

#include <DsmConfig.h>
#include <DsmNet.h>

char	buffer[10000];

Hdr_blk	hbp[MAX_DSM];
int	len[MAX_DSM] = {7352, 5272, 2668, 3644, 0, 0, 0, 0};

DsmConfig dsmConfig;
DsmNet *dsm_net[MAX_DSM];

void catchPipe(int);
static void netMessage(int, char *, char *);


/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
	int	i, stat;
	struct timespec	rqtp;

	rqtp.tv_sec = 0;
	rqtp.tv_nsec = 100000000;

	if (argc < 2) {
		fprintf(stderr, "Usage: discSim CTL_HOST\n");
		return(1);
	}


	for (i = 0, stat = (int)dsmConfig.firstDsm(); stat;
				++i, stat = (int)dsmConfig.nextDsm()) {
		if (!dsmConfig.isDsmLocation())
			continue;

		dsm_net[i] = new DsmNet(TCP_CONNECT, DSM_MSG_MAX_LEN,
			dsmConfig.port(), argv[1], argv[1], netMessage);

		hbp[i].id = 0x8681;

		hbp[i].hour = 00;
		hbp[i].minute = 00;
		hbp[i].second = 00;
		hbp[i].year = 1996;
		hbp[i].month = 04;
		hbp[i].day = 24;
		strcpy(hbp[i].dsm_locn, dsmConfig.location());
	}

#ifdef SVR4
	if ((int)sigset (SIGPIPE, catchPipe) < 0) {
#else
	if ((int)signal (SIGPIPE, catchPipe) < 0) {
#endif
		perror("discSim: sigset SIGPIPE");
		exit (ERROR);
	}


	printf("discSim: At Main Loop.\n");
	while (1) {
		for (i = 0, stat = (int)dsmConfig.firstDsm(); stat;
				++i, stat = (int)dsmConfig.nextDsm()) {

			if (!dsmConfig.isDsmLocation())
				continue;

			if (!dsm_net[i]->connected())
				dsm_net[i]->connectNet();

			memcpy(buffer, (char *)&hbp[i], sizeof(Hdr_blk));
			dsm_net[i]->writeNet(buffer, len[i], DSM_SYNC_DATA);

			if (++hbp[i].second > 59) {
				hbp[i].second = 0;
				if (++hbp[i].minute > 59) {
					hbp[i].minute = 0;
					if (++hbp[i].hour > 23)
						hbp[i].hour = 0;
				}
			}
		}

		nanosleep(&rqtp, NULL);
//		sleep(1);
	}

}	/* END MAIN */

/* -------------------------------------------------------------------- */
static void netMessage(int action, char *name, char *msg_str)
{
	fprintf(stderr, msg_str);
}

/* -------------------------------------------------------------------- */
void catchPipe (int stat)
{
	fprintf(stderr, "discSim: sigPipe caught.\n"); fflush(stderr);
}

/* END DISCSIM.CC */
