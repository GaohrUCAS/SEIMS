#! /usr/bin/env python
# coding=utf-8
# @Configuration of Postprocess for SEIMS
#

import ConfigParser

from RelativeImportModules import import_parents

from ..preprocess.util import *

# import util module located in SEIMS/preprocess
if __package__ is None:
    __package__ = import_parents(level = 2)

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

# 4. Optional_Parameters
if 'OPTIONAL_PARAMETERS' in cf.sections():
    TIME_Start = cf.get('OPTIONAL_PARAMETERS', 'Time_start'.lower())
    TIME_End = cf.get('OPTIONAL_PARAMETERS', 'Time_end'.lower())
else:
    raise ValueError("[OPTIONAL_PARAMETERS] section MUST be existed in *.ini file.")
TIME_StartArray = datetime.datetime.strptime(TIME_Start, "%Y-%m-%d")
TIME_EndArray = datetime.datetime.strptime(TIME_End, "%Y-%m-%d")
if TIME_StartArray > TIME_EndArray:
    raise ValueError("Wrong time setted in [OPTIONAL_PARAMETERS]!")
