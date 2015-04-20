#!/usr/bin/python

import sys
import time
from random import randint

from adned_lib import adned_lib
from adned_globals import adned_globals
    
def main():

    pv = str(sys.argv[1])

    print "Testing multiple start/stop cycles on " + pv
    
    lib = adned_lib()
    g = adned_globals()
    
    cycles = range(1000)
    
    for i in cycles:
        print "Start/Stop test " + str(i)

        stat = lib.start(pv)
        if (stat == g.FAIL):
            sys.exit(lib.testComplete(g.FAIL))

        sleepTime = randint(1,5)
        print "count time: " + str(sleepTime)
        time.sleep(sleepTime)
        
        stat = lib.stop(pv)
        if (stat == g.FAIL):
            sys.exit(lib.testComplete(g.FAIL))

    sys.exit(lib.testComplete(g.SUCCESS))
   

if __name__ == "__main__":
        main()
