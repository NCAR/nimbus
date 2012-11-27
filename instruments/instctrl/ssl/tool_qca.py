
def qca(env):
	env.Require(['qt4'],)
	env.EnableQt4Modules(['QtCore'],)
	if env['PLATFORM'] == 'darwin':
		env.AppendUnique(FRAMEWORKPATH=['/usr/local/lib',])
		env.AppendUnique(FRAMEWORKS=['qca'])
		env.AppendUnique(CPPPATH=['/usr/local/lib/qca.framework/Headers'])
    
Export('qca')
