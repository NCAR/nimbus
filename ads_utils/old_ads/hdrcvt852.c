/*
-------------------------------------------------------------------------
OBJECT NAME:	hdrcvt852.c

FULL NAME:		Header Convert (Proj 852)

ENTRY POINTS:	main()

DESCRIPTION:	This program converts the old new header format to the
				newest new header format.  (i.e. There should only be a
				few exabytes that this works for).

INPUT:		

OUTPUT:		

REFERENCES:		none

REFERENCED BY:	none

COPYRIGHT:		University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <raf/ctape.h>
#include "/h/9/cjw/wrk/tape/hdr_api-1.1/header.h"

#define TAPE_BLK_SIZE   32767

typedef unsigned char   bool;



struct old_Fl	*old_header;	/* Pointer where data is/will be stored	*/

char	*p_name = "hdrcvt852";
char	*filename;
bool	HeaderInitialized = FALSE;
int		taperr;			/* Tape Error, analogous to errno	*/
int		name_ptr;		/* Index to current variable		*/


/* -------------------------------------------------------------------- */
main(argc, argv)
int		argc;
char	*argv[];
{
	if (argc < 3)
		{
		fprintf(stderr, "usage: %s input_file output_file\n", p_name);
		exit(1);
		}

	filename = argv[2];

	if (InitFlightHeader(argv[1], CLOSE) == ERR)
		{
		fprintf(stderr, "hdrdump: init error, taperr = %d\n", taperr);
		exit(1);
		}

}	/* END MAIN */

/* -------------------------------------------------------------------- */
InitFlightHeader(name)
char	name[];
{
	int		fd;
	char	buffer[256];
	bool	tape = FALSE;
	int	rc;

	HeaderInitialized = FALSE;

	/* Check for Exabyte Tape Names of ex0 or ex1
	 */
	if (strcmp(name, "ex0") == 0 || strcmp(name, "ex1") == 0)
		{
		strcpy(buffer, "/dev/r");
		strcat(buffer, name);
		tape = TRUE;
		}
	else
		strcpy(buffer, name);

	/* Open File/Tape
	 */
	if ((fd = open(buffer, O_RDONLY)) == NULL)
		{
		taperr = errno;
		return(ERR);
		}

	if ((old_header = (struct old_Fl *)malloc((unsigned)TAPE_BLK_SIZE)) == NULL)
		{
		taperr = errno;
		return(ERR);
		}

	/* If it's a tape then skip past the first record
	 */
	if (tape)
		{
		read(fd, (char *)old_header, TAPE_BLK_SIZE);

		if (strcmp((char *)old_header, "ADS DATA TAPE") != 0)
			{
			taperr = BADHDR;
			return(ERR);
			}
		}


	read(fd, (char *)old_header, TAPE_BLK_SIZE);
	close(fd);


	/* Check if valid file		*/
	if (strcmp(TAPEHDR_STR, old_header->thdr) != 0)
		{
		taperr = BADHDR;
		return(ERR);
		}

	/* Setup the var pointers and init the var name list
	 */
	if ((rc = setup_var_list()) == OK)
		HeaderInitialized = TRUE;

	return(rc);

}	/* END INITFLIGHTDATA */

/* -------------------------------------------------------------------- */
ReleaseFlightHeader()
{
	free(old_header);
	HeaderInitialized = FALSE;
}

/* -------------------------------------------------------------------- */
static setup_var_list()
{
	int	i, type, rc = OK;
	char	*p;
	FILE	*fp;
	struct Fl	flight;

        if ((fp = fopen(filename, "wb")) == NULL)
                {
                fprintf(stderr, "%s: can't open output file %s\n",
                                                        p_name, filename);
                exit(1);
                }

        strcpy(flight.thdr, "THDR");
        flight.item_len = sizeof(struct Fl);
        strcpy(flight.version, VERSION_STR);
        memcpy(flight.prnum, old_header->prnum, 76);
        fwrite((char *)&flight, sizeof(struct Fl), 1, fp);


	/* loop
	 *	- Get Struct Type
	 *	- Add var name to list of var names
	 *	- Set Pointer into heap
	 */
	p = (char *)old_header + sizeof(struct old_Fl);

	for (i = 0; i < old_header->n_items; ++i)
		{
		if ((type = write_struct((struct old_Blk *)p, fp)) == ERR)
			{ rc = ERR; break; }

		switch (type)
			{
			case SH:
				p += sizeof(struct old_Sh);
				break;

			case DME:
				p += sizeof(struct old_Dme);
				break;

			case INS:
				p += sizeof(struct old_Ins);
				break;

			case IRS:
				p += sizeof(struct old_Irs);
				break;

			case PMS1D:
				p += sizeof(struct old_Pms1);
				break;

			case EVENT:
				p += sizeof(struct old_Evt);
				break;

			case BLK:
				p += sizeof(struct old_Blk);
				break;

			case ASYNC:
				p += sizeof(struct old_Asy);
				break;

			default:
				fprintf(stderr, "Unrecognized type\n");
			}

		}

	fclose(fp);
	return(rc);

}	/* END SETUP_VAR_LIST */

/* -------------------------------------------------------------------- */
static write_struct(p, fp)
struct old_Blk	*p;
FILE		*fp;
{
	char	*type;
	struct Sh	sh;
	struct Asy	async;
	struct Dme	dme;
	struct Ins	ins;
	struct Irs	irs;
	struct Pms1	pms1;
	struct Evt	event;
	struct Blk	blk;

	type = (char *)p;

	/* Determine Struct Type
	 */
	switch (type[0])
		{
		case 'A':
			do_async(&async, p);
			fwrite(&async, sizeof(struct Asy), 1, fp);
			return(ASYNC);

		case 'C': case 'G': case 'L':
			do_blk(&blk, p);
			fwrite(&blk, sizeof(struct Blk), 1, fp);
			return(BLK);

		case 'D':
			if (strcmp(type, DME_STR) == 0)
				{
				do_dme(&dme, p);
				fwrite(&dme, sizeof(struct Dme), 1, fp);
				return(DME);
				}
			else
				if(strcmp(type, DIGOUT_STR) == 0)
					{
					do_sh(&sh, p);
					fwrite(&sh, sizeof(struct Sh), 1, fp);
				 	return(SH);
					}
			break;

		case 'E':
			do_event(&event, p);
			fwrite(&event, sizeof(struct Evt), 1, fp);
			return(EVENT);

		case 'H':
			if (strcmp(type, HDR_STR) == 0)
				{
				do_blk(&blk, p);
				fwrite(&blk, sizeof(struct Blk), 1, fp);
				return(BLK);
				}
			else
				if(strcmp(type, HSKP_STR) == 0)
					{
					do_sh(&sh, p);
					fwrite(&sh, sizeof(struct Sh), 1, fp);
				 	return(SH);
					}
			break;

		case 'I':
			if (strcmp(type, IRS_STR) == 0)
				{
				do_event(&irs, p);
				fwrite(&irs, sizeof(struct Irs), 1, fp);
				return(IRS);
				}
			else
				if(strcmp(type, INS_STR) == 0)
					{
					do_ins(&ins, p);
					fwrite(&ins, sizeof(struct Ins), 1, fp);
				 	return(INS);
					}
			break;

		case 'P':
			do_pms1(&pms1, p);
			fwrite(&pms1, sizeof(struct Pms1), 1, fp);
			return(PMS1D);

		case 'S':
			do_sh(&sh, p);
			fwrite(&sh, sizeof(struct Sh), 1, fp);
			return(SH);
		}

	taperr = BADTYPE;
	return(ERR);

}	/* END GETITEMTYPE */

/* -------------------------------------------------------------------- */
do_async(new, old)
struct Asy	*new;
struct old_Asy	*old;
{
	strcpy(new->item_type, old->blkid);
	strcpy(new->name, old->name);
	new->item_len = sizeof(struct Asy);
	new->lrlen = old->lrlen;
	new->lrppr = old->lrppr;

	printf("Item_type = [%s]\n", new->item_type);

}

/* -------------------------------------------------------------------- */
do_blk(new, old)
struct Blk	*new;
struct old_Blk	*old;
{
	strcpy(new->item_type, old->name);
	new->item_len = sizeof(struct Blk);
	new->start = old->start;
	new->length = old->length;

	printf("Item_type = [%s]\n", new->item_type);

}

/* -------------------------------------------------------------------- */
do_pms1(new, old)
struct Pms1	*new;
struct old_Pms1	*old;
{
	strcpy(new->item_type, old->name);
	strcpy(new->name, old->prb_name);
	strcpy(new->locn, "");
	new->item_len = sizeof(struct Pms1);
	new->start = old->start;
	new->length = old->length;
	new->rate = old->rate;
	new->adsaddr = old->adsaddr;
	new->auxaddr = old->auxaddr;

	printf("Item_type = [%s]\n", new->item_type);

}

/* -------------------------------------------------------------------- */
do_ins(new, old)
struct Ins	*new;
struct old_Ins	*old;
{
	strcpy(new->item_type, old->name);
	new->item_len = sizeof(struct Ins);
	new->start = old->start;
	new->length = old->length;
	new->rate = old->rate;
	new->adsadr1 = old->adsadr1;
	new->adsadr2 = old->adsadr2;

	printf("Item_type = [%s]\n", new->item_type);

}

/* -------------------------------------------------------------------- */
do_event(new, old)
struct Evt	*new;
struct old_Evt	*old;
{
	strcpy(new->item_type, old->name);
	strcpy(new->locn, old->locn);
	new->item_len = sizeof(struct Evt);
	new->start = old->start;
	new->length = old->length;

	printf("Item_type = [%s]\n", new->item_type);

}

/* -------------------------------------------------------------------- */
do_sh(new, old)
struct Sh	*new;
struct old_Sh	*old;
{
	int	i;

	strcpy(new->item_type, old->name);
	strcpy(new->name, old->vname);
	strcpy(new->type, old->type);
	new->item_len = sizeof(struct Sh);
	new->start = old->start;
	new->length = old->length;
	new->rate = old->rate;
	new->adsaddr = old->adsaddr;
	new->convert = old->convert;
	new->order = old->order;

	switch (new->rate)
		{
		case 250:
			new->offset = 0x06;
			break;

		case 50:
			new->offset = 0x1e;
			break;

		case 5:
			new->offset = 0x3a;
			break;

		case 1:
			new->offset = 0;
			break;

		default:
			fprintf(stderr, "Invalid rate = %d\n", new->rate);
			break;
		}


	for (i = 0; i < new->order; ++i)
		new->cof[i] = old->cof[i];

	for (; i < 10; ++i)
		new->cof[i] = 0.0;

	printf("Item_type = [%s]\n", new->item_type);

}

/* -------------------------------------------------------------------- */
do_dme(new, old)
struct Dme	*new;
struct old_Dme	*old;
{
	strcpy(new->item_type, old->name);
	new->item_len = sizeof(struct Dme);
	new->start = old->start;
	new->length = old->length;
	new->rate = old->rate;
	new->rdadr1 = old->rdadr1;
	new->rdadr2 = old->rdadr2;
	new->wradr = old->wradr;

	printf("Item_type = [%s]\n", new->item_type);

}

/* END HDRCVT852.C */
