# -*- coding: utf-8 -*-

import os
import numpy
from readTextInfo import *

# MongoDB
HOSTNAME = "192.168.6.55"
PORT = 27017
BMPScenarioDBName = "BMP_Scenario_dianbu2_30m_longterm"

# Files
fieldFile = r'D:\GaohrWS\GithubPrj\SEIMS\model_data\dianbu\data_prepare\spatial\mgtfield_t100.txt'
pointFile = r'D:\GaohrWS\GithubPrj\SEIMS\model_data\dianbu\data_prepare\management\point_source_distribution.txt'
pointBMPsFile = r'D:\GaohrWS\GithubPrj\SEIMS\model_data\dianbu\data_prepare\management\point_source_management.txt'

# SEIMS Model
model_Exe = r'D:\SEIMS_model\SEIMS\Release\seims_omp'
model_Workdir = r'D:\SEIMS_model\Model_data\model_dianbu2_30m_longterm'
threadsNum = 8
layeringMethod = 0

timeStart = "2014-01-01"
timeEnd = "2014-12-31"

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