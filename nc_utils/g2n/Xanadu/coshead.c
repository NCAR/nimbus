#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

union buffers {
    short int shint_type[32768];
	unsigned short int ushint_type[32768];
    int int_type[16384];
    char char_type[65536];
    float float_type[16384];
};

void testswap_();
void swap_(), swapf_();

static int byteswap = 0;

#define NVARSZ 1000
#define VNAME_SIZE 11

    union buffers buffer;
    unsigned short int ibuf[128];
    char cbuf[128];
    /* static char fname[]="/h/guest/cooperw/data/f001"; */
    static char fname[]="../../data/f001"; 
    char tname[6];
    char ch;
    char sp[16];
    int lunit=4;
    int nunit;
    int stat;
    int mode=1;
    static int nbytes;
    int jflag=1;
    int nbuf=3312; /* int nbuf=32768;*/
    static int ierr=0,i,k,j,n,is;
    int nrec=0;
    int idiag=0;
    int ienabl=1;
    static int skip=0, nbits=6, nskip=0, repeat=200, nw=75;
    static int vskip=0, vnbits=20, vnskip=0, vrepeat=148, vnw=185;
    int nc;
    static int outbuf[200];
    float values[200];
    char charbuf[200];
    static char trans[64]=":ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-*/()$= ,.#[]%\"_!&'?<>@\\^;";
    float value, P, A;
    float PX[200], AX[200];
    int test;
    char NAMES[200][7];
    int m;
    char name[7];
    int count;
    int idy, iyr;
    char month[3];
    int mon = 0;

coshead_(newnames, newvars, idate, iac, iflt, iprj, ideltt)
    char newnames[NVARSZ][VNAME_SIZE+1];
    int *newvars, *idate, *iflt, *iprj, *ideltt;
    char *iac;
   
/* char            newnames[NVARSZ][VNAME_SIZE + 1]; */
{

    int hskip=0, hnbits=6, hnskip=0, hrepeat=100, hnw=75, hlines = 11;
    char *s;
    testswap_(&byteswap);
    printf(" swap switch set=%d\n", byteswap);


#if(0)
    strcpy(fname,"/home/cooperw/cosutils/f001");
    strcpy(fname,"/data/workdata/f001.1");
    strcpy(fname,"/data/workdata/f001");
#endif

    copen_(&lunit,&nunit,fname,&mode,&ierr,&idiag); 
    printf(" return from copen, fname=%s, ierr=%d\n",fname,ierr);
    if(ierr != 0) printf(" open error, ierr=%d\n",ierr);
		/* read header record */
    nbuf = 825;
    bnread_(&nunit,buffer.int_type,&nbuf,&nbytes,&ierr,&idiag);
    if (byteswap) {
        for (is = 0; is < nbuf; is++)
            swapf_(buffer.int_type+is);
    }
    printf(" wd1=%x ",buffer.ushint_type[0]);
    printf(" header record read, %d bytes, error code=%d\n",nbytes,ierr);
    for (is = 0; is < 10; is++) {
	printf(" %x", buffer.int_type[is]);
    }
    k = 0;
    while(k < nbytes) {
        gbytes_(&buffer.ushint_type, &outbuf, &hskip, &hnbits, &hnskip, &hrepeat);
        nc = 0;
        for (j = 0; j < hrepeat; j++) {
            charbuf[j] = trans[outbuf[j]];
            printf("%c", charbuf[j]);
        }
        printf("\n", k);
		/* for first line, read proj, flight, and date */
	if (k == 0) {
	    sscanf(charbuf, "%3d", iprj);
	    s = charbuf;
	    s = strpbrk(s, "-");
	    s++;
	    sscanf(s, "%d", iflt);
	    s = strpbrk(s, " ");
	    s++;
	    s = strpbrk(s, " ");
	    s++;
	    sscanf(s, "%d", &idy);
	    s = strpbrk(s, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	    strncpy(month, s, 3);
	    s += 3;
	    sscanf(s, "%d", &iyr);
	    if (iyr < 10)
	        iyr += 1970;
	    else
	        iyr += 1900;
	    printf(" project %d flight %d day %d month %c%c%c year %d\n", *iprj, 
		*iflt, idy, month[0], month[1], month[2], iyr);
	}
        k += hnw;
	hskip += hrepeat * hnbits;
    }
    printf(" **** end of text header ****\n");
		/* now read variable listing */
    for (i = 0; i < 8; i++) {
        hskip=0;
        nbuf = 825;
        bnread_(&nunit,buffer.int_type,&nbuf,&nbytes,&ierr,&idiag);
        if (byteswap) {
            for (is = 0; is < nbuf; is++)
                swapf_(buffer.int_type+is);
        }
        k = 0;
        while(k < nbytes) {
            gbytes_(&buffer.ushint_type, &outbuf, &hskip, &hnbits, &hnskip, &hrepeat);
            nc = 0;
            for (j = 0; j < hrepeat; j++) {
                charbuf[j] = trans[outbuf[j]];
                printf("%c", charbuf[j]);
            }
            printf("\n", k);
            k += hnw;
	    hskip += hrepeat * hnbits;
		/* decode translation information [value = (counts / P) - A] */
	    sscanf(charbuf, "%3d", &m);
	    strncpy(name, &(charbuf[56]), 6);
	    sscanf(&(charbuf[79]), "%7f", &P);
#if(0)
	    printf(" charbuf=%c%c%c%c%c%c%c, P=%f\n", charbuf[79], charbuf[80], charbuf[81], charbuf[82], charbuf[83], charbuf[84], charbuf[85], P);
#endif
	    sscanf(&(charbuf[89]), "%7f", &A);
	    PX[m-1] = P;
	    AX[m-1] = A;
	    strncpy(NAMES[m-1], name, 6);
	    printf(" variable %d = %s, P=%7.1f and A=%7.1f\n", m, NAMES[m-1], PX[m-1], AX[m-1]);
        }
    }
    printf(" **** end of translation header ****\n");

		/* fill in rest of translation arrays: */
    for (m = 88; m < 148; m++) {
	PX[m] = 1.;
	AX[m] = 0.;
    }
    for (m = 0; m < 16; m++) {
	sprintf(NAMES[m+84], "%s%02d", "ASAS", m);
	sprintf(NAMES[m+100], "%s%02d", "ASBS", m);
	sprintf(NAMES[m+116], "%s%02d", "CSSP", m);
	sprintf(NAMES[m+132], "%s%02d", "FSSP", m);
    }
    *newvars = 148;
    iac = "308D";
    for (m = 0; m < *newvars; m++) {
	strncpy(newnames[m], NAMES[m], 6);
	strncpy(newnames[m]+6, "              ", (VNAME_SIZE-6));
    }
    iyr -= 1900;
    if(!strncmp(month, "NOV", 3))
	mon = 11;
    else if (!strncmp(month, "JAN", 3))
	mon = 1;
    else if (!strncmp(month, "FEB", 3))
	mon = 2;
    else if (!strncmp(month, "MAR", 3))
	mon = 3;
    else if (!strncmp(month, "APR", 3))
	mon = 4;
    else if (!strncmp(month, "MAY", 3))
	mon = 5;
    else if (!strncmp(month, "JUN", 3))
	mon = 6;
    else if (!strncmp(month, "JUL", 3))
	mon = 7;
    else if (!strncmp(month, "AUG", 3))
	mon = 8;
    else if (!strncmp(month, "SEP", 3))
	mon = 9;
    else if (!strncmp(month, "OCT", 3))
	mon = 10;
    else if (!strncmp(month, "DEC", 3))
	mon = 12;
    *idate = iyr * 10000 + mon * 100 + idy;
    *ideltt = 1000;
    printf(" setting date = %d, project=%d and flt = %d\n", *idate, *iprj, *iflt);
    return(1);
}

static int needread = 1;
static int firstread = 1;

int cosdata_(outvalues)
    float *outvalues;

{
    char bufnames[NVARSZ][VNAME_SIZE+1];
    int bufvars, bdate, bflt, bprj, bdeltt;
    char bac[4];

		/* now unpack the data and translate into values for the output file */
		/* (next is arbitrary skip to try to synchronize records) */
		/* (found right skip using search.c to look for appropriate seconds-past-midnight integer) */
RECYCLE:
    if (firstread) {
        nbuf = 14855;
        nbuf = 7;
        bnread_(&nunit,buffer.int_type,&nbuf,&nbytes,&ierr,&idiag); 
        printf(" wd1=%x ",buffer.ushint_type[0]);
        printf(" record read, %d bytes, error code=%d\n",nbytes,ierr);
        firstread = 0;
    }
    if (needread) {
		/* don't understand this, but it works. I would think 3705=148*10 20-bit words + 16 bits left... */
	nbuf = 3712;
        bnread_(&nunit,buffer.int_type,&nbuf,&nbytes,&ierr,&idiag); 
        if (byteswap) {
            for (is = 0; is < nbuf; is++)
                swapf_(buffer.int_type+is);
        }
	printf(" wd1=%x ",buffer.ushint_type[0]);
        printf(" record read, %d bytes, error code=%d\n",nbytes,ierr);
	vskip = 0; 
        nrec++;
        k = 0;
	needread = 0;
    }
    gbytes_(buffer.int_type, &outbuf, &vskip, &vnbits, &vnskip, &vrepeat);
    k += 1;
    vskip += vrepeat * vnbits;
    if (k >= 10) {	
        needread = 1;
        printf(" k=%d, nbytes=%d, nrec=%d => read needed next call\n", k, nbytes, nrec);
    }
    for (m = 0; m < vrepeat; m++) {
        values[m] = outbuf[m] / PX[m] - AX[m];
        outvalues[m] = values[m];
#if(0)
        printf(" variable %d integer %d value %f P=%7.1f A=%7.1f name %c%c%c%c%c%c\n", m, outbuf[m], values[m], PX[m], AX[m], 
        NAMES[m][0], NAMES[m][1], NAMES[m][2], NAMES[m][3], NAMES[m][4], NAMES[m][5]);
	if (m < 30) {
        printf(" variable %d integer %d value %f P=%7.1f A=%7.1f name %c%c%c%c%c%c\n", m, outbuf[m], outvalues[m], PX[m], AX[m], 
        NAMES[m][0], NAMES[m][1], NAMES[m][2], NAMES[m][3], NAMES[m][4], NAMES[m][5]);
	}
#endif
    }
#if(0)
    printf(" ierr=%d, nbytes=%d outvalues[0] = %f\n", ierr, nbytes, outvalues[0]);
#endif
    if(ierr != 0 || nbytes <= 0 || outvalues[0] <= 0.1) {
	cclose_(&nunit, &stat, &ierr);
	/* fname[25]++; */
	fname[14]++; 
	if (!(access(fname, R_OK))) {
	    printf(" file %s exists\n", fname);
	    coshead_(bufnames, &bufvars, &bdate, bac, &bflt, &bprj, &bdeltt);
	    firstread = 1;
	    needread = 1;
	    goto RECYCLE;
	} else {
	    outvalues[0] = -999999.;
	}
    }
    return(1);
}
