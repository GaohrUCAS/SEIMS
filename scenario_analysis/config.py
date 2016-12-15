# -*- coding: utf-8 -*-

import ConfigParser
from readTextInfo import *
# import util module located in SEIMS/preprocess
if __package__ is None:
    __package__ = import_parents(level = 2)
from ..preprocess.util import *

# Load model configuration from *.ini file
cf = ConfigParser.ConfigParser()
cf.read(GetINIfile())
# 1. Text files directories
MODEL_DIR = None
# print cf.sections()
if 'PATH' in cf.sections():
    MODEL_DIR = cf.get('PATH', 'MODEL_DIR'.lower())
    fieldFile = cf.get('PATH', 'fieldFile'.lower())
    pointFile = cf.get('PATH', 'pointFile'.lower())
    pointBMPsFile = cf.get('PATH', 'pointBMPsFile'.lower())
    scenariosInfo = cf.get('PATH', 'scenariosInfo'.lower())
else:
    raise ValueError("[PATH] section MUST be existed in *.ini file.")
if not isPathExists(MODEL_DIR):
    raise IOError("Please Check Directories defined in [PATH]")

# 2. NSGA-II
if 'NSGAII' in cf.sections():
    GenerationsNum = int(cf.get('NSGAII', 'GenerationsNum'))
    PopulationSize = int(cf.getint('NSGAII', 'PopulationSize'))
    CrossoverRate = float(cf.get('NSGAII', 'CrossoverRate'))
    MutateRate = float(cf.get('NSGAII', 'MutateRate'))
    SelectRate = float(cf.get('NSGAII', 'SelectRate'))
else:
    raise ValueError("[NSGAII] section MUST be existed in *.ini file.")

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

# 4. BMPs
bmps_farm = []
bmps_cattle = []
bmps_pig = []
bmps_sewage = []
bmps_farm_cost = []
# bmps_cattle_cost = []
# bmps_pig_cost = []
# bmps_sewage_cost = []
if 'BMPs' in cf.sections():
    bmps_farm_STR = cf.get('BMPs', 'bmps_farm'.lower())
    bmps_cattle_STR = cf.get('BMPs', 'bmps_cattle'.lower())
    bmps_pig_STR = cf.get('BMPs', 'bmps_pig'.lower())
    bmps_sewage_STR_1 = cf.get('BMPs', 'bmps_sewage_1'.lower())
    bmps_sewage_STR_2 = cf.get('BMPs', 'bmps_sewage_2'.lower())
    bmps_sewage_STR_3 = cf.get('BMPs', 'bmps_sewage_3'.lower())
    bmps_sewage_STR_4 = cf.get('BMPs', 'bmps_sewage_4'.lower())
    bmps_farm_cost_STR = cf.get('BMPs', 'bmps_farm_cost'.lower())
    # bmps_cattle_cost_STR = cf.get('BMPs', 'bmps_cattle_cost'.lower())
    # bmps_pig_cost_STR = cf.get('BMPs', 'bmps_pig_cost'.lower())
    # bmps_sewage_cost_STR = cf.get('BMPs', 'bmps_sewage_cost'.lower())
    bmps_farm = StrtoIntArr(SplitStr(StripStr(bmps_farm_STR)))
    bmps_cattle = StrtoIntArr(SplitStr(StripStr(bmps_cattle_STR)))
    bmps_pig = StrtoIntArr(SplitStr(StripStr(bmps_pig_STR)))
    bmps_sewage_1 = StrtoIntArr(SplitStr(StripStr(bmps_sewage_STR_1)))
    bmps_sewage_2 = StrtoIntArr(SplitStr(StripStr(bmps_sewage_STR_2)))
    bmps_sewage_3 = StrtoIntArr(SplitStr(StripStr(bmps_sewage_STR_3)))
    bmps_sewage_4 = StrtoIntArr(SplitStr(StripStr(bmps_sewage_STR_4)))
    bmps_sewage.append(bmps_sewage_1)
    bmps_sewage.append(bmps_sewage_2)
    bmps_sewage.append(bmps_sewage_3)
    bmps_sewage.append(bmps_sewage_4)
    # farm_area = float(cf.get('BMPs', 'farm_area'.lower()))
    bmps_farm_cost = StrtoFltArr(SplitStr(StripStr(bmps_farm_cost_STR)))
    # bmps_cattle_cost = StrtoFltArr(SplitStr(StripStr(bmps_cattle_cost_STR)))
    # bmps_pig_cost = StrtoFltArr(SplitStr(StripStr(bmps_pig_cost_STR)))
    # bmps_sewage_cost = StrtoFltArr(SplitStr(StripStr(bmps_sewage_cost_STR)))
else:
    raise ValueError("[BMPs] section MUST be existed in *.ini file.")

# Scenario
fieldInfo = getFieldInfo(fieldFile)
pointSource = getPointSource(pointFile)
bmpsInfo = getBMPsInfo(pointBMPsFile, pointFile)

field_farm = fieldInfo[1]
field_lu = fieldInfo[2]
farm_area = fieldInfo[3]
point_cattle = pointSource[0]
point_pig = pointSource[1]
point_sewage = pointSource[2]

farm_Num = len(field_farm)
# farm_Num = 1
point_cattle_Num = len(point_cattle)
point_pig_Num = len(point_pig)
point_sewage_Num = len(point_sewage)

# BMP cost
bmps_cattle_cost = bmpsInfo[4]
bmps_pig_cost = bmpsInfo[5]
bmps_sewage_cost = bmpsInfo[6]

# livestock farm size
point_cattle_size = pointSource[3]
point_pig_size = pointSource[4]
