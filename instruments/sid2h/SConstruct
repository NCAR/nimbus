# -*- python -*-

import os
env = Environment(platform = 'posix',ENV= os.environ)
env['JLOCAL'] = os.environ['JLOCAL']

# Targets are only to install files.

Alias('install', env.Install('$JLOCAL/bin', 'sid2h'))
Alias('install', env.Install('$JLOCAL/bin', 'procsid2h'))

Alias('install', env.Install('$JLOCAL/idl/lib', ['sid.pro', 'sid_autolevel.pro', 'sid_bin2dec64.pro',
	'sid_browse.pro', 'sid_datablock.pro', 'sid_date.pro', 'sid_dec2bin.pro', 'sid_export.pro',
	'sid_export_ncdf.pro', 'sid_footer.pro', 'sid_ft2sfm.pro', 'sid_header.pro',
	'sid_hms2sfm.pro', 'sid_legend.pro', 'sid_meandiam.pro', 'sid_mvdiam.pro',
	'sid_ncdf_getstartstop.pro', 'sid_pointers.pro', 'sid_process.pro', 'sid_properties.pro',
	'sid_read_next.pro', 'sid_reject.pro', 'sid_sfm2hms.pro', 'sid_size.pro', 'sid_tas.pro',
	'sid_unnormalize.pro', 'sid_update_op.pro', 'sid_windowplot.pro']))
