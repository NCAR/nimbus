#!/usr/bin/python
# file SR.py
"""Read the data encoded from the flight-manual charts and produce a pickled dictionary
suitable for reading by Specs.py."""
import pickle


try:
    SRFile = open ('./SpecificRange.txt', 'r')
    SR = {}
    G = {}
    last_altitude = 0.
    for line in SRFile:
        if line[0] == '#': pass
        else:
            m = {}
            line = line.replace ('\n', '')
            ff = line.split (',')
#           print 'split line is ', ff
            nn = len (ff)
            altitude = float (ff[0])
            print 'altitude = ', altitude
            if altitude != last_altitude:
                SR[format (last_altitude*100., '.0f')] = G
                print 'Saving dictionary item for altitude ', last_altitude
#               print "G = ",G
                G = {}
                last_altitude = altitude
            gw = float (ff[1])
            print 'gw = ', gw
            for i in range (2, nn, 2):
                m[ff[i]] = float (ff[i+1])
            print 'M range is ', ff[2], ff[nn-2]
            G[str (gw)] = m
#           print 'm = ', m
    SR[format (last_altitude*100., '.0f')] = G
    print 'Saving dictionary item for altitude ', last_altitude
    SRFile.close ()
#   print 'SR=', SR
    TFile = open ('./SpecificRange.pickle','wb')
    pickle.dump (SR, TFile)
    TFile.close ()

except IOError as e:
    print \
            'There is no Specific-Range file.\n '\
          + 'ABORT'

