# -*- coding: utf-8 -*-

import os
import numpy
from readTextInfo import *

HOSTNAME = "127.0.0.1"
PORT = 27017
BMPScenarioDBName = "local"

fieldFile = r'D:\GaohrWS\SEIMS\model_data\dianbu\data_prepare\spatial\mgtfield_t100.txt'
pointFile = r'D:\GaohrWS\SEIMS\model_data\dianbu\data_prepare\management\point_source_distribution.txt'
pointBMPsFile = r'D:\GaohrWS\SEIMS\model_data\dianbu\data_prepare\management\point_source_management.txt'


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
