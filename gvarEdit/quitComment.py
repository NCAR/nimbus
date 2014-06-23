#Julian Quick
#This function is called when the user exits the program.
#Prompts user to make changes, saves under ==== with timestatmp
import fileinput
import sys
from datetime import datetime
def comment(text):
   filename='groundvars'
   for line in fileinput.input(filename,inplace=1):
      if '===========' in line:
         sys.stdout.write(line)
         sys.stdout.write('\n')
         sys.stdout.write('# Comments from edit on (Y-M-D-H-M)  '+datetime.now().strftime('%y-%m-%H-%M')+'\n')
         line=line.replace(line,'#'+text+'\n')
      sys.stdout.write(line)
   fileinput.close()
   print 'commenting succesful'
   quit()
