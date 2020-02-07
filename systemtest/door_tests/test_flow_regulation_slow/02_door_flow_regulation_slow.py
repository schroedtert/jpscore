#!/usr/bin/env python3
#################################################################################
# Check if the door is opened only opened after the times specified in events
# file. Also check if the flow fits the outflow specified in the ini file.
#################################################################################
import os
from sys import argv, path
import logging
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

utestdir = os.path.abspath(os.path.dirname(os.path.dirname(path[0])))
path.append(utestdir)

from utils import SUCCESS, FAILURE
from JPSRunTest import JPSRunTestDriver

import xml.etree.cElementTree as ET
import re


def readFlow(outflow):
    ioutflow = int(outflow)
    regexString = 'rate_{:.2f}'.format(outflow)
    regex = re.compile(regexString)

    for root, dirs, files in os.walk('results'):
        for file in files:
            if regex.search(file):
                flow = pd.read_csv(os.path.join('results', file),
                                   comment='#',
                                   sep='\s+',
                                   skip_blank_lines=True,
                                   header=None,
                                   engine='python')
                flow.columns = ['time', 'peds']
                return flow

    logging.error('Could not find flow file to expected flow {}.'.format(outflow))
    exit(FAILURE)


def readOutflow(inifile):
    tree = ET.parse(inifile)
    root = tree.getroot()
    for tc in root.iter("traffic_constraints"):
        for door in tc.iter("door"):
            return float(door.attrib["outflow"])

    logging.error('Could not read inifile for outflow: {}.'.format(inifile))
    exit(FAILURE)


def readMaxAgents(inifile):
    tree = ET.parse(inifile)
    root = tree.getroot()
    for tc in root.iter("traffic_constraints"):
        for door in tc.iter("door"):
            return int(door.attrib["max_agents"])

    logging.error('Could not read inifile for max agents: {}.'.format(inifile))
    exit(FAILURE)


def readStartingTimes():
    times = []
    tree = ET.parse('results/events.xml')
    root = tree.getroot()
    for event in root.iter("event"):
        if event.attrib["state"] == "open" or event.attrib["state"] == "reset":
            times.append(float(event.attrib["time"]))

    if times:
        return times
    else:
        logging.error('Could not read starting times from events file: {}.'.format('results/events.xml'))
        exit(FAILURE)


def checkFlow(data, expectedFlow):
    success = True

    flowError = 0.15
    relativeError = flowError * expectedFlow

    first = data.iloc[0]
    last = data.iloc[-1]
    numAgents = data.shape[0]

    time = last.time - first.time
    flow = numAgents / time

    if (np.abs(flow - expectedFlow) > relativeError):
        success = False

    return success, flow


def runtest(inifile, trajfile):
    success = True

    outflow = readOutflow(inifile)
    data = readFlow(outflow)

    [flowCorrect, measuredFlow] = checkFlow(data, outflow)
    if not flowCorrect:
        success = False
        logging.error('Outflow rate is not as expected. Expected {}, but are {}.'.format(outflow, measuredFlow))

    if not success:
        exit(FAILURE)


if __name__ == "__main__":
    test = JPSRunTestDriver(2, argv0=argv[0], testdir=path[0], utestdir=utestdir, jpscore=argv[1])
    test.run_test(testfunction=runtest)
    logging.info("%s exits with SUCCESS" % (argv[0]))
    exit(SUCCESS)
