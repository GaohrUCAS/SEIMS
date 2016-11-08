# -*- coding: utf-8 -*-

import ConfigParser
from readTextInfo import *

# Load model configuration from *.ini file
cf = ConfigParser.ConfigParser()
cf.read(GetINIfile())
# 1. Text files directories
MODEL_DIR = None
if 'PATH' in cf.sections():
    MODEL_DIR = cf.get('PATH', 'MODEL_DIR'.lower())
    fieldFile = cf.get('PATH', 'fieldFile'.lower())
    pointFile = cf.get('PATH', 'pointFile'.lower())
    pointBMPsFile = cf.get('PATH', 'pointBMPsFile'.lower())
else:
    raise ValueError("[PATH] section MUST be existed in *.ini file.")
if not isPathExists(MODEL_DIR):
    raise IOError("Please Check Directories defined in [PATH]")

# 2. NSGA
if 'NSGA' in cf.sections():
    GenerationsNum = int(cf.get('NSGA', 'GenerationsNum'))
    PopulationSize = int(cf.getint('NSGA', 'PopulationSize'))
    CrossoverRate = float(cf.get('NSGA', 'CrossoverRate'))
    MutateRate = float(cf.get('NSGA', 'MutateRate'))
else:
    raise ValueError("[MONGODB] section MUST be existed in *.ini file.")

# 3. MongoDB
if 'MONGODB' in cf.sections():
    HOSTNAME = cf.get('MONGODB', 'HOSTNAME')
    PORT = int(cf.getint('MONGODB', 'PORT'))
    BMPScenarioDBName = cf.get('MONGODB', 'BMPScenarioDBName'.lower())
else:
    raise ValueError("[MONGODB] section MUST be existed in *.ini file.")
if not isIPValid(HOSTNAME):
    raise ValueError("HOSTNAME illegal defined in [MONGODB]!")

# 3. SEIMS_Model
if 'SEIMS_Model' in cf.sections():
    model_Exe = cf.get('SEIMS_Model', 'model_Exe'.lower())
    model_Workdir = cf.get('SEIMS_Model', 'model_Workdir'.lower())
    threadsNum = int(cf.get('SEIMS_Model', 'threadsNum'))
    layeringMethod = int(cf.get('SEIMS_Model', 'layeringMethod'))
    timeStart = cf.get('SEIMS_Model', 'timeStart')
    timeEnd = cf.get('SEIMS_Model', 'timeEnd')
else:
    raise ValueError("[SEIMS_Model] section MUST be existed in *.ini file.")

# Scenario
field_farm = getFieldInfo(fieldFile)[1]
field_lu = getFieldInfo(fieldFile)[2]
point_cattle = getPointSource(pointFile)[0]
point_pig = getPointSource(pointFile)[1]
point_sewage = getPointSource(pointFile)[2]

# farm_Num = len(getFieldInfo(fieldFile)[1])
farm_Num = 1
point_cattle_Num = len(getPointSource(pointFile)[0])
point_pig_Num = len(getPointSource(pointFile)[1])
point_sewage_Num = len(getPointSource(pointFile)[2])

bmps_farm = getBMPsInfo(pointBMPsFile)[0]
bmps_cattle = numpy.sort(getBMPsInfo(pointBMPsFile)[1])
bmps_pig = numpy.sort(getBMPsInfo(pointBMPsFile)[2])
bmps_sewage = numpy.sort(getBMPsInfo(pointBMPsFile)[3])

bmps_farm_cost = [208., 166.]
bmps_cattle_cost = getBMPsInfo(pointBMPsFile)[4]
bmps_pig_cost = getBMPsInfo(pointBMPsFile)[5]
bmps_sewage_cost = getBMPsInfo(pointBMPsFile)[6]