#include <stdio.h>
#include "header.h"

main(int argc, char *argv[])
{
	FILE	*fp;
	Fl	flInfo;

	if (argc < 2)
		{
		fprintf(stderr, "Usage: fldump header_file_name.\n");
		return(1);
		}

	if ((fp = fopen(argv[1], "rb")) == NULL)
		{
		fprintf(stderr, "Can't open %s.\n", argv[1]);
		return(1);
		}

	fread((char *)&flInfo, sizeof(Fl), 1, fp);
	fclose(fp);

	printf("Fl.thdr\t\t%s\n", flInfo.thdr);
	printf("Fl.item_len\t%d\n", flInfo.item_len);
	printf("Fl.version\t%s\n", flInfo.version);
	printf("Fl.prnum\t%s\n", flInfo.prnum);
	printf("Fl.acraft\t%s\n", flInfo.acraft);
	printf("Fl.n_items\t%d\n", flInfo.n_items);
	printf("Fl.lrlen\t%d\n", flInfo.lrlen);
	printf("Fl.lrppr\t%d\n", flInfo.lrppr);
	printf("Fl.thdrlen\t%d\n", flInfo.thdrlen);
	printf("Fl.ads_type\t%s\n", flInfo.ads_type);

}
