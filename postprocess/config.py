#! /usr/bin/env python
# coding=utf-8
# @Configuration of Postprocess for SEIMS
#

import ConfigParser
# import util module located in SEIMS/preprocess
import sys
sys.path.append("..")
from preprocess.text import *
from preprocess.util import *

# Load model configuration from *.ini file
cf = ConfigParser.ConfigParser()
cf.read(GetINIfile())
# 1. Directories
MODEL_DIR = None
if 'PATH' in cf.sections():
    MODEL_DIR = cf.get('PATH', 'MODEL_DIR'.lower())
else:
    raise ValueError("[PATH] section MUST be existed in *.ini file.")
if not isPathExists(MODEL_DIR):
    raise IOError("Please Check Directories defined in [PATH]")
# 2. MongoDB
if 'MONGODB' in cf.sections():
    HOSTNAME = cf.get('MONGODB', 'HOSTNAME'.lower())
    PORT = cf.getint('MONGODB', 'PORT'.lower())
    ClimateDBName = cf.get('MONGODB', 'ClimateDBName'.lower())
    SpatialDBName = cf.get('MONGODB', 'SpatialDBName'.lower())
else:
    raise ValueError("[MONGODB] section MUST be existed in *.ini file.")
if not isIPValid(HOSTNAME):
    raise ValueError("HOSTNAME illegal defined in [MONGODB]!")

# 3. Parameters
PLOTVAR_STRING = ''
PLOT_VARS = []
if 'PARAMETERS' in cf.sections():
    PLOTVAR_STRING = cf.get('PARAMETERS', 'PLOT_VARIABLES'.lower())
else:
    raise ValueError("[PARAMETERS] section MUST be existed in *.ini file.")
if PLOTVAR_STRING != '':
    PLOT_VARS = SplitStr(StripStr(PLOTVAR_STRING))
else:
    raise ValueError("PLOT_VARIABLES illegal defined in [PARAMETERS]!")