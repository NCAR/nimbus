.\"qc man page -- by marc anderson
'\"macro stdmacro
.nr X
.TH qc l "April 30 1995"
.SH NAME
qc \- Quality control for WINPUT data collection system
.SH SYNOPSIS
.LP
.nf
.ft 3
#include <qc.h>
.ft
.fi
.B "int qc_rt_init(void)"
.br
.B "int qc_rt_exec(void)"
.br
.B "void qc_rt_cleanup(void)"
.SH DESCRIPTION
The 
.B "qc"
system, also called the Valkyrie project, was developed in the Fall of '94 
through the Spring of '95 by four University of Colorado students (see 
.B "AUTHORS"
section below), as their Computer Science senior project.
Its purpose is to rest on top of the 
.B "WINPUT"
data collection system, provide realtime validation for the data 
.B "WINPUT"
collects, and to represent error information meaningfully to the technicians
aboard the aircraft.

The 
.B "qc"
system consists of two major components:  (1) a validation library -- 
a collection of C++ classes that perform various sorts of tests on
.B "WINPUT"
variables, and (2) a graphical user interface (GUI) -- which provides
an X11 display of validation errors as they occur.  The validation library
code is linked into the 
.B "WINPUT"
process and thus executes on the same thread, whereas
the GUI is forked off as a separate process.  The validation library
communicates with the GUI through a BSD non-blocking socket interface.

There are four specific types of validation that are performed by the
.B "qc"
system:  range checking, flatline checking, spike detection, and
a level shift test.  These tests are explained in more detail in the
.B "EXAMPLE CONFIGURATION"
section of this file. 

The entire system's functionality is encapsulated into three C function calls
that are available to the 
.B "WINPUT"
system:

.B "int qc_rt_init(void)"

This initializes the validation library and forks off the GUI process.

The validation library is setup externally through a configuration
file (qc-config) which specifies the 
.B "WINPUT"
variables to perform 
validation on, the types of validation to be performed, and specific 
parameters required for each validation test.

After reading and parsing the configuration file,
.B "qc_rt_init()"
opens a disk log in which all validation errors are recorded throughout
the flight.

The 
.B "qc_rt_init()"
procedure also forks off the GUI process so that it
can initialize itself.  In order for this step to be successful, the
DISPLAY variable must be set locally, and, if displaying onto another
machine, the "xhost" utility must be set up to allow access to the
remote display.  The GUI functionality will be briefly summarized in the
section labeled 
.B "GUI OVERVIEW."

.B "qc_rt_init()"
returns the WINPUT-defined constant OK on success, or ERR on any of several
initialization failure conditions:  (1) the configuration file was 
determined invalid, (2) the GUI execution failed, or (3) the socket was 
not bound correctly.

.B "int qc_rt_exec(void)"

This performs one cycle of validation through the variables that were
configured.  If any of the validation tests fail, a disk log entry is
stored, and a socket message is passed to the GUI to be displayed.

If during this cycle the GUI terminates execution (i.e. the user 
presses the "Quit" button on the main screen), then
.B "qc_rt_exec()"
returns the constant ERR so that WINPUT can clean up the 
.B "qc"
system gracefully (with
.B "qc_rt_cleanup()" 
).  Otherwise, this function returns the OK constant.

.B "void qc_rt_cleanup(void)"

This function gracefully cleans up the 
.B "qc"
system.  It should be called in the 
.B "WINPUT"
termination sequence, or if 
.B "qc_rt_exec()"
returns ERR.  It
performs general cleanup of the validation system and instructs the
GUI to terminate if it is still running.  The disk log is timestamped 
and is then closed.
.br
.SH EXAMPLE CONFIGURATION
The configuration file specifies what validation tests 
.B "qc"
should perform.  A comment can be placed anywhere on a line, as long as 
the comment
is preceeded with a "#" character, and 
either tabs or spaces can be used to separate columns.  The 
first column of this file specifies a variable name, the second 
specifies the validation test to be run, and the subsequent columns
are reserved for parameters that depend on the specific test.

Examples of how to configure each of the four validation tests are presented
below.  Note that for purposes of optimization, the level shift test
and spike detection test are combined into a single algorithm, and thus their
parameters are combined into a single line in the configuration file.

.B "# range tests          low     high     high delta"
.br
.B "# -----------          -----   ----     ----------"
.br
.B "GLON      range        -90.0   90.0     0.5       "
.br

The range test is the simplest of the 
.B "qc"
validation tests.  It makes sure
that a variable stays within the ranges specified in the "low" and "high" columns.

The 
.B "qc"
system has a useful feature of distinguishing "low priority" errors from
"high priority" errors (the GUI treats these sorts of errors differently -- 
this is explained more in the 
.B "GUI OVERVIEW"
section).  The "high delta" parameter of the range
test is an additional threshold used to determine which range test errors
are high priority errors.  In the above example, a value greater than or equal 90.5 or
less than or equal to -90.5 will generate a high priority range error.  If one
would always want to generate high priority errors when "low" or "high" are
exceeded, the "high delta" parameter can be set to 0.  Alternatively,
if one would never want to generate high priority errors, the "high delta"
parameter could just be set to a very large number.

.B "# flatline tests       minvarlow   minvarhigh"
.br
.B "# --------------       ---------   ----------"
.br
.B "GLAT     flatline      0.001       0.0001    "
.br

The flatline detector, similarly, is straightforward.  The 
.B "qc"
system detects the flatline condition by calculating a standard statistical 
variance
over a fixed number of seconds (currently 10 seconds of averaged data).  
If the variance is less then "minvarhigh," a high priority flatline 
error is issued.  If the variance is less than "minvarlow," a low priority 
flatline error is issued to the GUI.  Never will both a low and high
priority error be generated:  if the value is less than "minvarlow" and
"minvarhigh," then only a high priority flatline error will be issued.

.B "# level/spike tests    minslope minshift peakmaxint rtnslack"
.br
.B "# -----------------    -------- -------- ---------- --------"
.br
.B "GLAT     level         0.1      0.2      1          0.4     "
.br

In the 
.B "qc"
system, level shift/spike detection is the most complicated of the
three validation tests. 
Spikes and level shifts are closely related to one another:  a spike
is two level shifts in opposite directions, which rationalizes why
their detection is combined into a single algorithm.  

The "minslope" parameter defines the minimum slope that a variable's value
must exceed, initially, in order for a level shift or spike to be considered.
The slope is the ratio of the rise in value over
the run in number of samples (the sample rate is defined by 
.B "WINPUT"
and varies between variables).
The slope parameter can be exceeded in the positive or negative direction,
so that positive and negative spikes/level shifts are both detected.

"Minshift" defines the minimum value the variable must change, at
the slope specified by "minslope."  This is required because slope, alone,
doesn't determine the magnitude of a spike or level shift.

"Peakmaxint" and "rtnslack" are used to distinguish spikes from level shifts.
After "minshift" is exceeded at the slope specified by "minslope," in order for
the algorithm to detect a spike instead of a level shift, the variable's value
must return to its baseline region within "peakmaxint" samples.  The baseline
region is defined as the value before "minslope" was exceeded, plus or minus
the "rtnslack" parameter.  If "peakmaxint" samples go by, or if the value
doesn't return to the baseline region, a level shift, instead of spike, will 
be signaled.
.SH GUI OVERVIEW
The 
.B "qc"
GUI (executable named qc-gui) maintains an X11 display of validation
errors as they are detected.  There are two routes by which errors are 
displayed by the GUI:  low priority and high priority errors are displayed
on the "main screen,"
while high priority errors are also displayed by the GUI in "warning boxes."
The distinction between low priority and high priority errors is made
in the validation library (see
.B "EXAMPLE CONFIGURATION"
section of this man page).

The
main screen
consists of four windows containing space for the four types of errors
that the validation library produces, and a button bar with options for
the user.  The "Pause" button allows the user to pause the GUI for a period 
of time (when paused, the button name changes to "Resume").  The "Snooze 10 
min." button will cause validation to cease for all variables over the next
10 minutes.  The "Reset" button clears all validation errors present
in the four windows on the 
main screen.
The "Clear Boxes" button will dismiss all 
warning boxes
(see below) that are currently being displayed.  The "Quit" button
will terminate the GUI and have the validation library (linked into
the WINPUT process) stop using CPU cycles for validation.

A
warning box
is displayed when a high priority error message is generated by the
validation library.  Separate 
warning boxes
are generated for each variable that has high priority errors.  A
warning box
lists all errors for a given variable, and
also has a button bar of options available to the user.  The "Snooze
10 min." button causes validation for the variable associated with
the warning box to be disabled for the next 10 minutes.  Similarly,
the "Snooze 2 min." button disables validation for 2 minutes.  
The "Clear" button will erase all accumulated warning
messages from the selected warning box, and also remove messages for
the variable associated with the warning box, from the main screen.
Finally, the "Dismiss" button causes the warning box to be removed
without effecting the main screen.
.SH FILES
.B $PROJ_DIR/[project number]/qc-config"
-- full path of qc configuration file for a specific research project 
.SH SEE ALSO
.I "Valkyrie Project Book"
:  An assortment of papers written through the course of this project.
.SH BUGS
The socket link between the validation library and the GUI relies on a 
hardcoded binding port number.  Occasionally after crashing the system, 
the port number needs to be re-assigned.  This can be done by changing the 
Makefile present in the 'socklib/' directory of the source distribution,
and then by rebuilding the entire system by typing "make clean all" at the 
root level of the source distribution.
.SH AUTHORS
Marc Anderson <andersom@colorado.edu>
.br
Alexandra Bokinsky <bokinsky@colorado.edu>
.br
Edward Stautler <stautler@colorado.edu>
.br
Kent Larson <larsonk@colorado.edu>

