#!/usr/bin/python
#file: SeeTrack.py
"Examine the details stored in a saved track."
import pickle
t = open('Plan.Test', 'r')
track = pickle.load (t)
t.close()
keylist = track.keys()
keylist.sort ()
for mkey in keylist:
    if 'Module' in mkey and 'Number' not in mkey:
        m = track[mkey]
        keys = m.keys ()
        keys.sort ()
#       print 'Keys:', keys
        print '%s Type %s Time %f' % (mkey,m['Type'],m['Time'])
        print 'Anchor %.1f, %.1f, %.0f, Dimension %s' % (\
              float (m['Anchor'][0]), float (m['Anchor'][1]), \
              float (m['Anchor'][2]), m['Dimension'])
        print 'Orientation %s Legs %s drift %s' % (m['Orientation'],\
               m['Legs'], m['Drift']) 
        print 'Other ',m['Other'], 'ModuleNumber ', m['ModuleNumber'],\
              'ManeuverNumber ',  m['ManeuverNumber']
        print 'FlightDirection ',m['FlightDirection'],' AC ',m['AC']
        print 'Comment ',m['Comment']
        for k in keys:
            if 'Manvr' in k:
                mv = m[k]
                print '    ',k, ', Type = ', mv['Type']
                print '    Orientation: ', mv['Orientation']
                print '    Anchor: ', mv['Anchor']
                print '    EndPt : ', mv['Anchor']
                print '    Flight Direction : ', mv['FlightDirection']
                print '    Time and Fuel: ', mv['Time'], mv['Fuel']
