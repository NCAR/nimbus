
def qca(env):
	if env['PLATFORM'] == 'darwin':
		env.AppendUnique(FRAMEWORKPATH=['/usr/local/lib',])
		env.AppendUnique(FRAMEWORKS=['qca'])
		env.AppendUnique(CPPPATH=['/usr/local/lib/qca.framework/Headers'])
    
Export('qca')
