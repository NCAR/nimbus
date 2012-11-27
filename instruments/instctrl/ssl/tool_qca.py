
def qca(env):
	env.Require(['qt4'],)
	env.EnableQt4Modules(['QtCore'],)
	if env['PLATFORM'] == 'darwin':
		env.AppendUnique(FRAMEWORKPATH=['/usr/local/lib',])
		env.AppendUnique(FRAMEWORKS=['qca'])
		env.AppendUnique(CPPPATH=['/usr/local/lib/qca.framework/Headers'])
	elif env['PLATFORM'] in ['posix','linux', 'linux2']:
		env.AppendUnique(CPPPATH=['/usr/local/include/QtCrypto'])
		env.AppendUnique(LIBPATH=['/usr/local/lib'])
		env.AppendUnique(LIBS=['qca'])
	elif env['PLATFORM'] == 'win32':
		pass

Export('qca')
