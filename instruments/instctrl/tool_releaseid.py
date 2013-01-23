import sys
import subprocess

def ReleaseId(env):
	# Create an identifier that can be used to distinguish
	# between operating systems. 
	#
	# Aspen.3.1-7456.el6.x86_64
	# Name-
    id = 'unknown'
    if sys.platform in ['posix', 'linux', 'linux2']:
        id = 'linux'
        # determine the architecture
        unameproc = subprocess.Popen(['uname', '-m'], stdout=subprocess.PIPE)
        arch = unameproc.communicate()[0]
        arch = arch[0:len(arch)-1]

  	    # determine the distributer id (e.g. Scientific)
        distributerproc = subprocess.Popen(['lsb_release', '-i', '-s'], stdout=subprocess.PIPE)
        distributer = distributerproc.communicate()[0]
        distributer = distributer[0:len(distributer)-1]
	
        # determine the release number
        relproc = subprocess.Popen(['lsb_release', '-r', '-s'], stdout=subprocess.PIPE)
        release = relproc.communicate()[0]
        release = release[0:len(release)-1]
        
        id = distributer+'-'+'release'+'-'+arch   
        
    if sys.platform == 'darwin':
        child = Popen(['/usr/bin/uname', '-r'], stdout=PIPE)
        release = child.communicate()[0]
        tokens = split(release, '.')
        if len(tokens) > 0:
        	major = tokens[0]
        if major == '9':
        	id = 'leopard'
        if major == '10':
        	id = 'snow-leopard'
        if major == '11':
        	id = 'lion'
        if major == '12':
        	id = 'mountain-lion'
        
    if sys.platform == 'win32':
        id = 'win'
    
    return id
    
def releaseid(env):
    env.AddMethod(ReleaseId, 'ReleaseId')
    
Export('releaseid')

    