/*
** $Id: ncar_imake.c.sed,v 1.1.1.1 1999/05/14 20:57:11 morreale Exp $
*/

/*****************************************************************************
 *                                                                           *
 *                                Porting Note                               *
 *                                                                           *
 * Add the value of BOOTSTRAPCFLAGS to the cpp_argv table so that it will be *
 * passed to the template file.                                              *
 *                                                                           *
 *****************************************************************************/


/*
**				S T O P ! ! !
**
**
**	Do not edit ncar_imake.c! It is created from the file ncar_imake.c.sed
**	Edit ncar_imake.c.sed instead or your changes will be lost
**
*/

/*
**	These defines are set by the Makefile at compile time
*/
#define OSMajorVersion	MAJOR
#define OSMinorVersion	MINOR
#define OSTeenyVersion	TEENY


/*
 * 
Copyright (c) 1985, 1986, 1987  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.
 * 
 * Original Author:
 *	Todd Brunhoff
 *	Tektronix, inc.
 *	While a guest engineer at Project Athena, MIT
 *
 * imake: the include-make program.
 *
 * Usage: imake [-Idir] [-Ddefine] [-T template] [-f imakefile ] [-C Imakefile.c ] [-s] [-e] [-v] [make flags]
 *
 * Imake takes a template file (Imake.tmpl) and a prototype (Imakefile)
 * and runs cpp on them producing a Makefile.  It then optionally runs make
 * on the Makefile.
 * Options:
 *		-D	define.  Same as cpp -D argument.
 *		-I	Include directory.  Same as cpp -I argument.
 *		-T	template.  Designate a template other
 *			than Imake.tmpl
 *		-f	specify the Imakefile file
 *		-C	specify the name to use instead of Imakefile.c
 *		-s[F]	show.  Show the produced makefile on the standard
 *			output.  Make is not run is this case.  If a file
 *			argument is provided, the output is placed there.
 *              -e[F]   execute instead of show; optionally name Makefile F
 *		-v	verbose.  Show the make command line executed.
 *
 * Environment variables:
 *		
 *		IMAKEINCLUDE	Include directory to use in addition to "."
 *		IMAKECPP	Cpp to use instead of /lib/cpp
 *		IMAKEMAKE	make program to use other than what is
 *				found by searching the $PATH variable.
 * Other features:
 *	imake reads the entire cpp output into memory and then scans it
 *	for occurences of "@@".  If it encounters them, it replaces it with
 *	a newline.  It also trims any trailing white space on output lines
 *	(because make gets upset at them).  This helps when cpp expands
 *	multi-line macros but you want them to appear on multiple lines.
 *	It also changes occurences of "XCOMM" to "#", to avoid problems
 *	with treating commands as invalid preprocessor commands.
 *
 *	The macros MAKEFILE and MAKE are provided as macros
 *	to make.  MAKEFILE is set to imake's makefile (not the constructed,
 *	preprocessed one) and MAKE is set to argv[0], i.e. the name of
 *	the imake program.
 *
 * Theory of operation:
 *   1. Determine the name of the imakefile from the command line (-f)
 *	or from the content of the current directory (Imakefile or imakefile).
 *	Call this <imakefile>.  This gets added to the arguments for
 *	make as MAKEFILE=<imakefile>.
 *   2. Determine the name of the template from the command line (-T)
 *	or the default, Imake.tmpl.  Call this <template>
 *   3. Determine the name of the imakeCfile from the command line (-C)
 *	or the default, Imakefile.c.  Call this <imakeCfile>
 *   3. Store three lines of input into <imakeCfile>:
 *		#define IMAKE_TEMPLATE		" <template> "
 *		#define INCLUDE_IMAKEFILE	< <imakefile> >
 *		#include IMAKE_TEMPLATE
 *	Start up cpp and provide it with this file.
 *	Note that the define for INCLUDE_IMAKEFILE is intended for
 *	use in the template file.  This implies that the imake is
 *	useless unless the template file contains at least the line
 *		#include INCLUDE_IMAKEFILE
 *   4. Gather the output from cpp, and clean it up, expanding @@ to
 *	newlines, stripping trailing white space, cpp control lines,
 *	and extra blank lines, and changing XCOMM to #.  This cleaned
 *	output is placed in a new file, default "Makefile", but can
 *	be specified with -s or -e options.
 *   5. Optionally start up make on the resulting file.
 *
 * The design of the template makefile should therefore be:
 *	<set global macros like CFLAGS, etc.>
 *	<include machine dependent additions>
 *	#include INCLUDE_IMAKEFILE
 *	<add any global targets like 'clean' and long dependencies>
 */
#include <stdio.h>
#include <ctype.h>
#include "Xosdefs.h"
#ifdef WIN32
#include "Xw32defs.h"
#endif
#ifndef X_NOT_POSIX
#define _POSIX_SOURCE
#endif
#include <sys/types.h>
#include <fcntl.h>
#ifdef X_NOT_POSIX
#ifndef WIN32
#include <sys/file.h>
#endif
#else
#include <unistd.h>
#endif
#if defined(X_NOT_POSIX) || defined(_POSIX_SOURCE)
#include <signal.h>
#else
#define _POSIX_SOURCE
#include <signal.h>
#undef _POSIX_SOURCE
#endif
#include <sys/stat.h>
#ifndef X_NOT_POSIX
#ifdef _POSIX_SOURCE
#include <sys/wait.h>
#else
#define _POSIX_SOURCE
#include <sys/wait.h>
#undef _POSIX_SOURCE
#endif
#define waitCode(w)	WEXITSTATUS(w)
#define waitSig(w)	WTERMSIG(w)
typedef int		waitType;
#else /* X_NOT_POSIX */
#ifdef SYSV
#define waitCode(w)	(((w) >> 8) & 0x7f)
#define waitSig(w)	((w) & 0xff)
typedef int		waitType;
#else /* SYSV */
#ifdef WIN32
#include <process.h>
typedef int		waitType;
#else
#include <sys/wait.h>
#define waitCode(w)	((w).w_T.w_Retcode)
#define waitSig(w)	((w).w_T.w_Termsig)
typedef union wait	waitType;
#endif
#endif
#ifndef WIFSIGNALED
#define WIFSIGNALED(w) waitSig(w)
#endif
#ifndef WIFEXITED
#define WIFEXITED(w) waitCode(w)
#endif
#endif /* X_NOT_POSIX */
#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#else
char *malloc(), *realloc();
void exit();
#endif
#if defined(macII) && !defined(__STDC__)  /* stdlib.h fails to define these */
char *malloc(), *realloc();
#endif /* macII */
#ifdef X_NOT_STDC_ENV
extern char	*getenv();
#endif
#include <errno.h>
#ifdef X_NOT_STDC_ENV
extern int	errno;
#endif
#include "imakemdep.h"


#define	TRUE		1
#define	FALSE		0

#ifdef FIXUP_CPP_WHITESPACE
int	InRule = FALSE;
#ifdef INLINE_SYNTAX
int	InInline = 0;
#endif
#endif
#ifdef MAGIC_MAKE_VARS
int xvariable = 0;
int xvariables[10];
#endif

/*
 * Some versions of cpp reduce all tabs in macro expansion to a single
 * space.  In addition, the escaped newline may be replaced with a
 * space instead of being deleted.  Blech.
 */
#ifdef FIXUP_CPP_WHITESPACE
void KludgeOutputLine(), KludgeResetRule();
#else
#define KludgeOutputLine(arg)
#define KludgeResetRule()
#endif

typedef	unsigned char	boolean;

#ifdef USE_CC_E
#ifndef DEFAULT_CC
#define DEFAULT_CC "cc"
#endif
#else
#ifndef DEFAULT_CPP
#ifdef CPP_PROGRAM
#define DEFAULT_CPP CPP_PROGRAM
#else
#define DEFAULT_CPP "/lib/cpp"
#endif
#endif
#endif

char *cpp = NULL;

char	*tmpMakefile    = "/tmp/Imf.XXXXXX";
char	*tmpImakefile    = "/tmp/IIf.XXXXXX";
char	*make_argv[ ARGUMENTS ] = {
#ifdef WIN32
    "nmake"
#else
    "make"
#endif
};

int	make_argindex;
int	cpp_argindex;
char	*Imakefile = NULL;
char	*Makefile = "Makefile";
char	*Template = "Imake.tmpl";
char	*ImakefileC = "Imakefile.c";
boolean haveImakefileC = FALSE;
char	*cleanedImakefile = NULL;
char	*program;
char	*FindImakefile();
char	*ReadLine();
char	*CleanCppInput();
char	*Strdup();
char	*Emalloc();
void	LogFatalI(), LogFatal();

boolean	verbose = FALSE;
boolean	show = TRUE;

main(argc, argv)
	int	argc;
	char	**argv;
{
	FILE	*tmpfd;
	char	makeMacro[ BUFSIZ ];
	char	makefileMacro[ BUFSIZ ];

	program = argv[0];
	init();
	SetOpts(argc, argv);

	Imakefile = FindImakefile(Imakefile);
	CheckImakefileC(ImakefileC);
	if (Makefile)
		tmpMakefile = Makefile;
	else {
		tmpMakefile = Strdup(tmpMakefile);
		(void) mktemp(tmpMakefile);
	}
	AddMakeArg("-f");
	AddMakeArg( tmpMakefile );
	sprintf(makeMacro, "MAKE=%s", program);
	AddMakeArg( makeMacro );
	sprintf(makefileMacro, "MAKEFILE=%s", Imakefile);
	AddMakeArg( makefileMacro );

	if ((tmpfd = fopen(tmpMakefile, "w+")) == NULL)
		LogFatal("Cannot create temporary file %s.", tmpMakefile);

	cleanedImakefile = CleanCppInput(Imakefile);
	cppit(cleanedImakefile, Template, ImakefileC, tmpfd, tmpMakefile);

	if (show) {
		if (Makefile == NULL)
			showit(tmpfd);
	} else
		makeit();
	wrapup();
	exit(0);
}

showit(fd)
	FILE	*fd;
{
	char	buf[ BUFSIZ ];
	int	red;

	fseek(fd, 0, 0);
	while ((red = fread(buf, 1, BUFSIZ, fd)) > 0)
		writetmpfile(stdout, buf, red, "stdout");
	if (red < 0)
	    LogFatal("Cannot read %s.", tmpMakefile);
}

wrapup()
{
	if (tmpMakefile != Makefile)
		unlink(tmpMakefile);
	if (cleanedImakefile && cleanedImakefile != Imakefile)
	    unlink(cleanedImakefile);
	if (haveImakefileC)
	    unlink(ImakefileC);
}

#ifdef SIGNALRETURNSINT
int
#else
void
#endif
catch(sig)
	int	sig;
{
	errno = 0;
	LogFatalI("Signal %d.", sig);
}

/*
 * Initialize some variables.
 */
init()
{
	register char	*p;

	make_argindex=0;
	while (make_argv[ make_argindex ] != NULL)
		make_argindex++;
	cpp_argindex = 0;
	while (cpp_argv[ cpp_argindex ] != NULL)
		cpp_argindex++;

	/*
	 * See if the standard include directory is different than
	 * the default.  Or if cpp is not the default.  Or if the make
	 * found by the PATH variable is not the default.
	 */
	if (p = getenv("IMAKEINCLUDE")) {
		if (*p != '-' || *(p+1) != 'I')
			LogFatal("Environment var IMAKEINCLUDE %s\n",
				"must begin with -I");
		AddCppArg(p);
		for (; *p; p++)
			if (*p == ' ') {
				*p++ = '\0';
				AddCppArg(p);
			}
	}
	if (p = getenv("IMAKECPP"))
		cpp = p;
	if (p = getenv("IMAKEMAKE"))
		make_argv[0] = p;

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, catch);
}

AddMakeArg(arg)
	char	*arg;
{
	errno = 0;
	if (make_argindex >= ARGUMENTS-1)
		LogFatal("Out of internal storage.", "");
	make_argv[ make_argindex++ ] = arg;
	make_argv[ make_argindex ] = NULL;
}

AddCppArg(arg)
	char	*arg;
{
	errno = 0;
	if (cpp_argindex >= ARGUMENTS-1)
		LogFatal("Out of internal storage.", "");
	cpp_argv[ cpp_argindex++ ] = arg;
	cpp_argv[ cpp_argindex ] = NULL;
}

SetOpts(argc, argv)
	int	argc;
	char	**argv;
{
	errno = 0;
	/*
	 * Now gather the arguments for make
	 */
	for(argc--, argv++; argc; argc--, argv++) {
	    /*
	     * We intercept these flags.
	     */
	    if (argv[0][0] == '-') {
		if (argv[0][1] == 'D') {
		    AddCppArg(argv[0]);
		} else if (argv[0][1] == 'I') {
		    AddCppArg(argv[0]);
		} else if (argv[0][1] == 'f') {
		    if (argv[0][2])
			Imakefile = argv[0]+2;
		    else {
			argc--, argv++;
			if (! argc)
			    LogFatal("No description arg after -f flag\n", "");
			Imakefile = argv[0];
		    }
		} else if (argv[0][1] == 's') {
		    if (argv[0][2])
			Makefile = ((argv[0][2] == '-') && !argv[0][3]) ?
			    NULL : argv[0]+2;
		    else {
			argc--, argv++;
			if (!argc)
			    LogFatal("No description arg after -s flag\n", "");
			Makefile = ((argv[0][0] == '-') && !argv[0][1]) ?
			    NULL : argv[0];
		    }
		    show = TRUE;
		} else if (argv[0][1] == 'e') {
		   Makefile = (argv[0][2] ? argv[0]+2 : NULL);
		   show = FALSE;
		} else if (argv[0][1] == 'T') {
		    if (argv[0][2])
			Template = argv[0]+2;
		    else {
			argc--, argv++;
			if (! argc)
			    LogFatal("No description arg after -T flag\n", "");
			Template = argv[0];
		    }
		} else if (argv[0][1] == 'C') {
		    if (argv[0][2])
			ImakefileC = argv[0]+2;
		    else {
			argc--, argv++;
			if (! argc)
			    LogFatal("No imakeCfile arg after -C flag\n", "");
			ImakefileC = argv[0];
		    }
		} else if (argv[0][1] == 'v') {
		    verbose = TRUE;
		} else
		    AddMakeArg(argv[0]);
	    } else
		AddMakeArg(argv[0]);
	}
#ifdef USE_CC_E
	if (!cpp)
	{
		AddCppArg("-E");
		cpp = DEFAULT_CC;
	}
#else
	if (!cpp)
		cpp = DEFAULT_CPP;
#endif
	cpp_argv[0] = cpp;
	AddCppArg(ImakefileC);
}

char *FindImakefile(Imakefile)
	char	*Imakefile;
{
	if (Imakefile) {
		if (access(Imakefile, R_OK) < 0)
			LogFatal("Cannot find %s.", Imakefile);
	} else {
		if (access("Imakefile", R_OK) < 0)
			if (access("imakefile", R_OK) < 0)
				LogFatal("No description file.", "");
			else
				Imakefile = "imakefile";
		else
			Imakefile = "Imakefile";
	}
	return(Imakefile);
}

void
LogFatalI(s, i)
	char *s;
	int i;
{
	/*NOSTRICT*/
	LogFatal(s, (char *)i);
}

void
LogFatal(x0,x1)
	char *x0, *x1;
{
#if !defined(WIN32) && !defined(linux)
	extern char	*sys_errlist[];
#endif
	static boolean	entered = FALSE;

	if (entered)
		return;
	entered = TRUE;

	fprintf(stderr, "%s: ", program);
	if (errno)
		fprintf(stderr, "%s: ", sys_errlist[ errno ]);
	fprintf(stderr, x0,x1);
	fprintf(stderr, "  Stop.\n");
	wrapup();
	exit(1);
}

showargs(argv)
	char	**argv;
{
	for (; *argv; argv++)
		fprintf(stderr, "%s ", *argv);
	fprintf(stderr, "\n");
}

#define TmplDef "#define IMAKE_TEMPLATE"
#define ImakeDef "#define INCLUDE_IMAKEFILE"
#define TmplInc "#include IMAKE_TEMPLATE"

#define OSMajorVersionDef    "#define OSMajorVersion"
#define OSMinorVersionDef    "#define OSMinorVersion"
#define OSTeenyVersionDef    "#define OSTeenyVersion"


CheckImakefileC(masterc)
	char *masterc;
{
	char mkcbuf[1024];
	FILE *inFile;

	if (access(masterc, F_OK) == 0) {
		inFile = fopen(masterc, "r");
		if (inFile == NULL)
			LogFatal("Refuse to overwrite: %s", masterc);
		if ((fgets(mkcbuf, sizeof(mkcbuf), inFile) &&
		     strncmp(mkcbuf, TmplDef, sizeof(TmplDef)-1)) ||
		    (fgets(mkcbuf, sizeof(mkcbuf), inFile) &&
		     strncmp(mkcbuf, ImakeDef, sizeof(ImakeDef)-1)) ||
		    (fgets(mkcbuf, sizeof(mkcbuf), inFile) &&
		     strncmp(mkcbuf, TmplInc, sizeof(TmplInc)-1)) ||
		    fgets(mkcbuf, sizeof(mkcbuf), inFile))
		{
			fclose(inFile);
			LogFatal("Refuse to overwrite: %s", masterc);
		}
		fclose(inFile);
	}
}

cppit(imakefile, template, masterc, outfd, outfname)
	char	*imakefile;
	char	*template;
	char	*masterc;
	FILE	*outfd;
	char	*outfname;
{
	FILE	*inFile;
	int	pid;
	waitType	status;

	haveImakefileC = TRUE;
	inFile = fopen(masterc, "w");
	if (inFile == NULL)
		LogFatal("Cannot open %s for output.", masterc);
	if (fprintf(inFile, "%s %d\n", OSMajorVersionDef, OSMajorVersion) < 0 ||
	    fprintf(inFile, "%s %d\n", OSMinorVersionDef, OSMinorVersion) < 0 ||
	    fprintf(inFile, "%s %d\n", OSTeenyVersionDef, OSTeenyVersion) < 0 ||
	    fprintf(inFile, "%s \"%s\"\n", TmplDef, template) < 0 ||
	    fprintf(inFile, "%s <%s>\n", ImakeDef, imakefile) < 0 ||
	    fprintf(inFile, "%s\n", TmplInc) < 0 ||
	    fclose(inFile))
	    LogFatal("Cannot write to %s.", masterc);
	/*
	 * Fork and exec cpp
	 */
#ifdef WIN32
	dup2(fileno(outfd), 1);
	status = _spawnvp(_P_WAIT, cpp, cpp_argv);
	if (status < 0)
		LogFatal("Cannot spawn.", "");
	if (status > 0)
		LogFatalI("Exit code %d.", status);
#else
	pid = fork();
	if (pid < 0)
		LogFatal("Cannot fork.", "");
	if (pid) {	/* parent */
		while (wait(&status) > 0) {
			errno = 0;
			if (WIFSIGNALED(status))
				LogFatalI("Signal %d.", waitSig(status));
			if (WIFEXITED(status) && waitCode(status))
				LogFatalI("Exit code %d.", waitCode(status));
		}
	}
	else {	/* child... dup and exec cpp */
		if (verbose)
			showargs(cpp_argv);
		dup2(fileno(outfd), 1);
		execvp(cpp, cpp_argv);
		LogFatal("Cannot exec %s.", cpp);
	}
#endif
	CleanCppOutput(outfd, outfname);
}

makeit()
{
	int	pid;
	waitType	status;

	/*
	 * Fork and exec make
	 */
#ifdef WIN32
	status = _spawnvp(_P_WAIT, make_argv[0], make_argv);
	if (status < 0)
		LogFatal("Cannot spawn %s.", make_argv[0]);
	if (status > 0)
		LogFatalI("Exit code %d.", status);
#else
	pid = fork();
	if (pid < 0)
		LogFatal("Cannot fork.", "");
	if (pid) {	/* parent... simply wait */
		while (wait(&status) > 0) {
			errno = 0;
			if (WIFSIGNALED(status))
				LogFatalI("Signal %d.", waitSig(status));
			if (WIFEXITED(status) && waitCode(status))
				LogFatalI("Exit code %d.", waitCode(status));
		}
	} else {	/* child... dup and exec cpp */
		if (verbose)
			showargs(make_argv);
		execvp(make_argv[0], make_argv);
		LogFatal("Cannot exec %s.", make_argv[0]);
	}
#endif
}

char *CleanCppInput(imakefile)
	char	*imakefile;
{
	FILE	*outFile = NULL;
	FILE	*inFile;
	char	*buf,		/* buffer for file content */
		*pbuf,		/* walking pointer to buf */
		*punwritten,	/* pointer to unwritten portion of buf */
		*ptoken,	/* pointer to # token */
		*pend,		/* pointer to end of # token */
		savec;		/* temporary character holder */
	int	count;
	struct stat	st;

	/*
	 * grab the entire file.
	 */
	if (!(inFile = fopen(imakefile, "r")))
		LogFatal("Cannot open %s for input.", imakefile);
	if (fstat(fileno(inFile), &st) < 0)
		LogFatal("Cannot stat %s for size.", imakefile);
	buf = Emalloc(st.st_size+3);
	count = fread(buf + 2, 1, st.st_size, inFile);
	if (count <= 0)
		LogFatal("Cannot read %s:", imakefile);
	fclose(inFile);
	buf[0] = '\n';
	buf[1] = '\n';
	buf[count + 2] = '\0';

	punwritten = pbuf = buf + 2;
	while (*pbuf) {
	    /* for compatibility, replace make comments for cpp */
	    if (*pbuf == '#' && pbuf[-1] == '\n' && pbuf[-2] != '\\') {
		ptoken = pbuf+1;
		while (*ptoken == ' ' || *ptoken == '\t')
			ptoken++;
		pend = ptoken;
		while (*pend && *pend != ' ' && *pend != '\t' && *pend != '\n')
			pend++;
		savec = *pend;
		*pend = '\0';
		if (strcmp(ptoken, "define") &&
		    strcmp(ptoken, "if") &&
		    strcmp(ptoken, "ifdef") &&
		    strcmp(ptoken, "ifndef") &&
		    strcmp(ptoken, "include") &&
		    strcmp(ptoken, "line") &&
		    strcmp(ptoken, "else") &&
		    strcmp(ptoken, "elif") &&
		    strcmp(ptoken, "endif") &&
		    strcmp(ptoken, "error") &&
		    strcmp(ptoken, "pragma") &&
		    strcmp(ptoken, "undef")) {
		    if (outFile == NULL) {
			tmpImakefile = Strdup(tmpImakefile);
			(void) mktemp(tmpImakefile);
			outFile = fopen(tmpImakefile, "w");
			if (outFile == NULL)
			    LogFatal("Cannot open %s for write.\n",
				tmpImakefile);
		    }
		    writetmpfile(outFile, punwritten, pbuf-punwritten,
				 tmpImakefile);
		    if (ptoken > pbuf + 1)
			writetmpfile(outFile, "XCOMM", 5, tmpImakefile);
		    else
			writetmpfile(outFile, "XCOMM ", 6, tmpImakefile);
		    punwritten = pbuf + 1;
		}
		*pend = savec;
	    }
	    pbuf++;
	}
	if (outFile) {
	    writetmpfile(outFile, punwritten, pbuf-punwritten, tmpImakefile);
	    fclose(outFile);
	    return tmpImakefile;
	}

	return(imakefile);
}

CleanCppOutput(tmpfd, tmpfname)
	FILE	*tmpfd;
	char	*tmpfname;
{
	char	*input;
	int	blankline = 0;

	while(input = ReadLine(tmpfd, tmpfname)) {
		if (isempty(input)) {
			if (blankline++)
				continue;
			KludgeResetRule();
		} else {
			blankline = 0;
			KludgeOutputLine(&input);
			writetmpfile(tmpfd, input, strlen(input), tmpfname);
		}
		writetmpfile(tmpfd, "\n", 1, tmpfname);
	}
	fflush(tmpfd);
#ifdef NFS_STDOUT_BUG
	/*
	 * On some systems, NFS seems to leave a large number of nulls at
	 * the end of the file.  Ralph Swick says that this kludge makes the
	 * problem go away.
	 */
	ftruncate (fileno(tmpfd), (off_t)ftell(tmpfd));
#endif
}

/*
 * Determine if a line has nothing in it.  As a side effect, we trim white
 * space from the end of the line.  Cpp magic cookies are also thrown away.
 * "XCOMM" token is transformed to "#".
 */
isempty(line)
	register char	*line;
{
	register char	*pend;

	/*
	 * Check for lines of the form
	 *	# n "...
	 * or
	 *	# line n "...
	 */
	if (*line == '#') {
		pend = line+1;
		if (*pend == ' ')
			pend++;
		if (*pend == 'l' && pend[1] == 'i' && pend[2] == 'n' &&
		    pend[3] == 'e' && pend[4] == ' ')
			pend += 5;
		if (isdigit(*pend)) {
		    	do {
			    pend++;
			} while (isdigit(*pend));
			if (*pend == '\n' || *pend == '\0')
				return(TRUE);
			if (*pend++ == ' ' && *pend == '"')
				return(TRUE);
		}
		while (*pend)
		    pend++;
	} else {
	    for (pend = line; *pend; pend++) {
		if (*pend == 'X' && pend[1] == 'C' && pend[2] == 'O' &&
		    pend[3] == 'M' && pend[4] == 'M' &&
		    (pend == line || pend[-1] == ' ' || pend[-1] == '\t') &&
		    (pend[5] == ' ' || pend[5] == '\t' || pend[5] == '\0'))
		{
		    *pend = '#';
		    strcpy(pend+1, pend+5);
		}
#ifdef MAGIC_MAKE_VARS
		if (*pend == 'X' && pend[1] == 'V' && pend[2] == 'A' &&
		    pend[3] == 'R')
		{
		    char varbuf[5];
		    int i;

		    if (pend[4] == 'd' && pend[5] == 'e' && pend[6] == 'f' &&
			pend[7] >= '0' && pend[7] <= '9')
		    {
			i = pend[7] - '0';
			sprintf(varbuf, "%0.4d", xvariable);
			strncpy(pend+4, varbuf, 4);
			xvariables[i] = xvariable;
			xvariable = (xvariable + 1) % 10000;
		    }
		    else if (pend[4] == 'u' && pend[5] == 's' &&
			     pend[6] == 'e' && pend[7] >= '0' &&
			     pend[7] <= '9')
		    {
			i = pend[7] - '0';
			sprintf(varbuf, "%0.4d", xvariables[i]);
			strncpy(pend+4, varbuf, 4);
		    }
		}
#endif
	    }
	}
	while (--pend >= line && (*pend == ' ' || *pend == '\t')) ;
	pend[1] = '\0';
	return (*line == '\0');
}

/*ARGSUSED*/
char *ReadLine(tmpfd, tmpfname)
	FILE	*tmpfd;
	char	*tmpfname;
{
	static boolean	initialized = FALSE;
	static char	*buf, *pline, *end;
	register char	*p1, *p2;

	if (! initialized) {
		int	total_red;
		struct stat	st;

		/*
		 * Slurp it all up.
		 */
		fseek(tmpfd, 0, 0);
		if (fstat(fileno(tmpfd), &st) < 0)
			LogFatal("cannot state %s for size\n", tmpMakefile);
		pline = buf = Emalloc(st.st_size+1);
		total_red = fread(buf, 1, st.st_size, tmpfd);
		if (total_red <= 0)
			LogFatal("cannot read %s\n", tmpMakefile);
		end = buf + total_red;
		*end = '\0';
		fseek(tmpfd, 0, 0);
#if defined(SYSV) || defined(WIN32)
		freopen(tmpfname, "w+", tmpfd);
#else	/* !SYSV */
		ftruncate(fileno(tmpfd), 0);
#endif	/* !SYSV */
		initialized = TRUE;
	    fprintf (tmpfd, "# Makefile generated by imake - do not edit!\n");
	    fprintf (tmpfd, "# %s\n",
		"$XConsortium: imake.c,v 1.87 94/04/17 20:10:28 rws Exp $");
	}

	for (p1 = pline; p1 < end; p1++) {
		if (*p1 == '@' && *(p1+1) == '@') { /* soft EOL */
			*p1++ = '\0';
			p1++; /* skip over second @ */
			break;
		}
		else if (*p1 == '\n') { /* real EOL */
#ifdef WIN32
			if (p1 > pline && p1[-1] == '\r')
				p1[-1] = '\0';
#endif
			*p1++ = '\0';
			break;
		}
	}

	/*
	 * return NULL at the end of the file.
	 */
	p2 = (pline == p1 ? NULL : pline);
	pline = p1;
	return(p2);
}

writetmpfile(fd, buf, cnt, fname)
	FILE	*fd;
	int	cnt;
	char	*buf;
	char	*fname;
{
	if (fwrite(buf, sizeof(char), cnt, fd) != cnt)
		LogFatal("Cannot write to %s.", fname);
}

char *Emalloc(size)
	int	size;
{
	char	*p;

	if ((p = malloc(size)) == NULL)
		LogFatalI("Cannot allocate %d bytes\n", size);
	return(p);
}

#ifdef FIXUP_CPP_WHITESPACE
void
KludgeOutputLine(pline)
	char	**pline;
{
	char	*p = *pline;
	char	quotechar = '\0';

	switch (*p) {
	    case '#':	/*Comment - ignore*/
		break;
	    case '\t':	/*Already tabbed - ignore it*/
	    	break;
	    case ' ':	/*May need a tab*/
	    default:
#ifdef INLINE_SYNTAX
		if (*p == '<' && p[1] == '<') { /* inline file close */
		    InInline--;
		    InRule = TRUE;
		    break;
		}
#endif
		/*
		 * The following cases should not be treated as beginning of 
		 * rules:
		 * variable := name	(GNU make)
		 * variable = .*:.*	(':' should be allowed as value)
		 *	sed 's:/a:/b:'	(: used in quoted values)
		 */
		for (; *p; p++) {
		    if (quotechar) {
			if (quotechar == '\\' ||
			    (*p == quotechar &&
#ifdef WIN32
			     quotechar != ')' &&
#endif
			     p[-1] != '\\'))
			    quotechar = '\0';
			continue;
		    }
		    switch (*p) {
		    case '\\':
		    case '"':
		    case '\'':
			quotechar = *p;
			break;
		    case '(':
			quotechar = ')';
			break;
		    case '{':
			quotechar = '}';
			break;
		    case '[':
			quotechar = ']';
			break;
		    case '=':
#ifdef REMOVE_CPP_LEADSPACE
			if (!InRule && **pline == ' ') {
			    while (**pline == ' ')
				(*pline)++;
			}
#endif
			goto breakfor;
#ifdef INLINE_SYNTAX
		    case '<':
			if (p[1] == '<') /* inline file start */
			    InInline++;
			break;
#endif
		    case ':':
			if (p[1] == '=')
			    goto breakfor;
			while (**pline == ' ')
			    (*pline)++;
			InRule = TRUE;
			return;
		    }
		}
breakfor:
		if (InRule && **pline == ' ')
		    **pline = '\t';
		break;
	}
}

void
KludgeResetRule()
{
	InRule = FALSE;
}
#endif /* FIXUP_CPP_WHITESPACE */

char *Strdup(cp)
	register char *cp;
{
	register char *new = Emalloc(strlen(cp) + 1);

	strcpy(new, cp);
	return new;
}
