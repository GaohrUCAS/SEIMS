#! /usr/bin/env python
# coding=utf-8
## @Configuration of Preprocessing for SEIMS
#

import ConfigParser
from text import *
from util import *


# Load model configuration from *.ini file
cf = ConfigParser.ConfigParser()
cf.read(GetINIfile())
# 1. Directories
if 'PATH' in cf.sections():
    BASE_DATA_DIR = cf.get('PATH', 'BASE_DATA_DIR'.lower())
    MODEL_DIR = cf.get('PATH', 'MODEL_DIR'.lower())
    TXT_DB_DIR = cf.get('PATH', 'TXT_DB_DIR'.lower())
    PREPROC_SCRIPT_DIR = cf.get('PATH', 'PREPROC_SCRIPT_DIR'.lower())
    CPP_PROGRAM_DIR = cf.get('PATH', 'CPP_PROGRAM_DIR'.lower())
    METIS_DIR = cf.get('PATH', 'METIS_DIR'.lower())
    MPIEXEC_DIR = cf.get('PATH', 'MPIEXEC_DIR'.lower())
    WORKING_DIR = cf.get('PATH', 'WORKING_DIR'.lower())
else:
    raise ValueError("[PATH] section MUST be existed in *.ini file.")
if not (isPathExists(BASE_DATA_DIR) and isPathExists(MODEL_DIR) and isPathExists(TXT_DB_DIR)
        and isPathExists(PREPROC_SCRIPT_DIR) and isPathExists(CPP_PROGRAM_DIR) and isPathExists(METIS_DIR)):
    raise IOError("Please Check Directories defined in [PATH]")
if isPathExists(MPIEXEC_DIR):
    MPIEXEC_DIR = None
if os.path.isdir(WORKING_DIR):
    if not os.path.exists(WORKING_DIR):
        os.mkdir(WORKING_DIR)
else:
    WORKING_DIR = MODEL_DIR + os.sep + 'preprocess_output'
    os.mkdir(WORKING_DIR)
CLIMATE_DATA_DIR = BASE_DATA_DIR + os.sep + 'climate'
SPATIAL_DATA_DIR = BASE_DATA_DIR + os.sep + 'spatial'
MEASUREMENT_DATA_DIR = BASE_DATA_DIR + os.sep + 'observed'
BMP_DATA_DIR = BASE_DATA_DIR + os.sep + 'management'

if not (isPathExists(CLIMATE_DATA_DIR) and isPathExists(CLIMATE_DATA_DIR)):
    raise IOError("Directories named 'climate' and 'spatial' MUST BE located in [BASE_DATA_DIR]!")
useObserved = True
if not isPathExists(MEASUREMENT_DATA_DIR):
    MEASUREMENT_DATA_DIR = None
    useObserved = False
useScernario = True
if not isPathExists(BMP_DATA_DIR):
    BMP_DATA_DIR = None
    useScernario = False

# 2. MongoDB related
if 'MONGODB' in cf.sections():
    HOSTNAME = cf.get('MONGODB', 'HOSTNAME'.lower())
    PORT = cf.getint('MONGODB', 'PORT'.lower())
    ClimateDBName = cf.get('MONGODB', 'ClimateDBName'.lower())
    BMPScenarioDBName = cf.get('MONGODB', 'BMPScenarioDBName'.lower())
    SpatialDBName = cf.get('MONGODB', 'SpatialDBName'.lower())
else:
    raise ValueError("[MONGODB] section MUST be existed in *.ini file.")
if not isIPValid(HOSTNAME):
    raise ValueError("HOSTNAME illegal defined in [MONGODB]!")

# 3. Model related switch
# by default, OMP version and daily mode will be built
forCluster = False
stormMode = False
genCN = True
genRunoffCoef = True
genCrop = True
if 'SWITCH' in cf.sections():
    forCluster = cf.getboolean('SWITCH', 'forCluster'.lower())
    stormMode = cf.getboolean('SWITCH', 'stormMode'.lower())
    genCN = cf.getboolean('SWITCH', 'genCN'.lower())
    genRunoffCoef = cf.getboolean('SWITCH', 'genRunoffCoef'.lower())
    genCrop = cf.getboolean('SWITCH', 'genCrop'.lower())

genIUH = True
simuMode = Tag_Mode_Daily
if forCluster and Tag_Cluster not in SpatialDBName.lower():
    SpatialDBName = Tag_Cluster + "_" + SpatialDBName
if stormMode:
    simuMode = Tag_Mode_Storm
    if not Tag_Mode_Storm.lower() in SpatialDBName.lower():
        SpatialDBName = SpatialDBName + "_" + Tag_Mode_Storm.lower()
    genIUH = False
if not Tag_Model.lower() in SpatialDBName.lower():
    SpatialDBName = Tag_Model.lower() + "_" + SpatialDBName
if forCluster and (not Tag_Cluster.lower() in SpatialDBName.lower()):
    SpatialDBName = Tag_Cluster.lower() + "_" + SpatialDBName
if ClimateDBName is not None and stormMode:
    ClimateDBName = ClimateDBName + "_" + Tag_CLIM_STORM_Suf.lower()

# 4. Climate Input
if 'CLIMATE' in cf.sections():
    HydroClimateVarFile = CLIMATE_DATA_DIR + os.sep + cf.get('CLIMATE', 'HydroClimateVarFile'.lower())
    MetroSiteFile = CLIMATE_DATA_DIR + os.sep + cf.get('CLIMATE', 'MetroSiteFile'.lower())
    PrecSiteFile = CLIMATE_DATA_DIR + os.sep + cf.get('CLIMATE', 'PrecSiteFile'.lower())
    MeteoDailyFile = CLIMATE_DATA_DIR + os.sep + cf.get('CLIMATE', 'MeteoDailyFile'.lower())
    PrecDailyFile = CLIMATE_DATA_DIR + os.sep + cf.get('CLIMATE', 'PrecDailyFile'.lower())
    thiessenIdField = cf.get('CLIMATE', 'thiessenIdField'.lower())
else:
    raise ValueError("Climate input file names MUST be provided in [CLIMATE]!")

# 5. Spatial Input
if 'SPATIAL' in cf.sections():
    PrecSitesThiessen = SPATIAL_DATA_DIR + os.sep + cf.get('SPATIAL', 'PrecSitesThiessen'.lower())
    MeteorSitesThiessen = SPATIAL_DATA_DIR + os.sep + cf.get('SPATIAL', 'MeteorSitesThiessen'.lower())
    dem = SPATIAL_DATA_DIR + os.sep + cf.get('SPATIAL', 'dem'.lower())
    outlet_file = SPATIAL_DATA_DIR + os.sep + cf.get('SPATIAL', 'outlet_file'.lower())
    if not os.path.exists(outlet_file):
        outlet_file = None
    landuseFile = SPATIAL_DATA_DIR + os.sep + cf.get('SPATIAL', 'landuseFile'.lower())
    soilSEQNFile = SPATIAL_DATA_DIR + os.sep + cf.get('SPATIAL', 'soilSEQNFile'.lower())
    soilSEQNText = SPATIAL_DATA_DIR + os.sep + cf.get('SPATIAL', 'soilSEQNText'.lower())
    mgtFiedlFile = SPATIAL_DATA_DIR + os.sep + cf.get('SPATIAL', 'mgtFiedlFile'.lower())
    if not os.path.exists(mgtFiedlFile):
        mgtFiedlFile = None
else:
    raise ValueError("Spatial input file names MUST be provided in [SPATIAL]!")

# 6. Option parameters
D8AccThreshold = 0
np = 4
D8DownMethod = 'Surface'
dorm_hr = -1.
T_base = 0.
imperviousPercInUrbanCell = 0.3
default_reach_depth = 5.
defaultLanduse = 8
if 'SPATIAL' in cf.sections():
    D8AccThreshold = cf.getfloat('OPTIONAL_PARAMETERS', 'D8AccThreshold'.lower())
    np = cf.getint('OPTIONAL_PARAMETERS', 'np')
    D8DownMethod = cf.get('OPTIONAL_PARAMETERS', 'D8DownMethod'.lower())
    dorm_hr = cf.getfloat('OPTIONAL_PARAMETERS', 'dorm_hr'.lower())
    T_base = cf.getfloat('OPTIONAL_PARAMETERS', 'T_base'.lower())
    imperviousPercInUrbanCell = cf.getfloat('OPTIONAL_PARAMETERS', 'imperviousPercInUrbanCell'.lower())
    default_reach_depth = cf.getfloat('OPTIONAL_PARAMETERS', 'default_reach_depth'.lower())
    defaultLanduse = cf.getint('OPTIONAL_PARAMETERS', 'defaultLanduse'.lower())
