#!/usr/bin/env python3

"""
Test to check if max agents of train are working correctly. In this scenario 2 trains arrive
"""

import os
import sys

import pandas as pd
import math
utestdir = os.path.abspath(os.path.dirname(os.path.dirname(sys.path[0])))
from sys import *

sys.path.append(utestdir)
from JPSRunTest import JPSRunTestDriver
from utils import *

def runtest(inifile, trajfile):
    fps, N, traj = parse_file(trajfile)

    df = pd.DataFrame(data=traj, columns=['ID', 'FR', 'X', 'Y', 'Z'])
    trainTimes = [[5,15, 15], [35, 50, 20]]

    errorFPS = 5 * fps

    numPedsByFrame = df.groupby('FR').FR.agg('count').to_frame('count').reset_index()

    success = True
    for i, trainTime in enumerate(trainTimes):
        allowedAgents = N - trainTimes[i][2]
        timeDeparture = trainTimes[i][1]
        if (i < len(trainTimes)-1):
            timeArrival = trainTimes[i+1][0]
        else:
            timeArrival = 99999999

        fpsDeparture = timeDeparture * fps + errorFPS
        fpsArrival = timeArrival * fps - errorFPS

        dfBetweenTrain = numPedsByFrame[(numPedsByFrame.FR > fpsDeparture) & (numPedsByFrame.FR < fpsArrival)]

        maxAgents = dfBetweenTrain['count'].max()
        minAgents = dfBetweenTrain['count'].min()

        if math.isnan(minAgents) or math.isnan(minAgents):
            logging.critical("There are no pedestrians left, there should be %d pedestrians in the simulation" % allowedAgents)
            success = False
        elif maxAgents != minAgents:
            logging.critical("The number of pedestrians changes during waiting time from %d to %" % (maxAgents, minAgents))
            success = False
        elif allowedAgents != maxAgents:
            logging.critical("Train capacity exceeded, there should be %d pedestrians in the simulation but found max. %d pedestrians"
                             %(allowedAgents, maxAgents))
            success = False


        N = N - trainTimes[i][2]
    if not success:
        exit(FAILURE)


if __name__ == "__main__":
    test = JPSRunTestDriver(1, argv0=argv[0], testdir=sys.path[0], utestdir=utestdir, jpscore=argv[1])
    test.run_test(testfunction=runtest)
    logging.info("%s exits with SUCCESS" % (argv[0]))
    exit(SUCCESS)
