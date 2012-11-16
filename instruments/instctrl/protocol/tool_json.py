
def json(env):
	env.AppendUnique(CPPPATH = ['/usr/local/include',])
	env.AppendUnique(LIBPATH=['/usr/local/lib',])
	env.AppendUnique(LIBS=['json',])
    
Export('json')
	