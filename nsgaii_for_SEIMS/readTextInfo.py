# -*- coding: utf-8 -*-
# @Read BMP_Scenario text info
# @Author Huiran GAO
# @Date   2016-10-28

import os, sys
import random

def delSpecialStr(line):
    '''
    :param line:
    :return: line that remove ['\r\n', '\n\r', '\r', '\n']
    '''
    LFs = ['\r\n', '\n\r', '\r', '\n']
    for LF in LFs:
        if LF in line:
            line = line.split(LF)[0]
            break
    return line


###### farms ######
def getFieldInfo(fieldFile):
    '''
    :param fieldFile: full path of field text file
    :return: field number, field list
    '''
    # Get field info
    fieldTtextArr = []
    if os.path.isfile(fieldFile):
        fieldfile_object = open(fieldFile, "r")
        try:
            li = 0
            for line in fieldfile_object:
                line = delSpecialStr(line)
                if li != 0 and li != 2:
                    fieldTtextArr.append(line.strip())
                li += 1
        finally:
            fieldfile_object.close()

    # Get farm field index
    fieldsNum = int(fieldTtextArr[0])
    farmFields = []
    farmLU = []
    for i in range(1, fieldsNum):
        fieldInfo = fieldTtextArr[i].split('\t')
        if int(fieldInfo[3]) == 1 or int(fieldInfo[3]) == 33:
            farmFields.append(int(fieldInfo[0]))
            farmLU.append(int(fieldInfo[3]))
    # print farmFields
    return (fieldsNum, farmFields, farmLU)


def getPointSource(pointFile):
    '''
    :param pointFile: full path of point source distribute file
    :return: cattle points list, pig points list, sewage points list
    '''
    ###### points ######
    pointTextArr = []
    point_cattle = []
    point_pig = []
    point_sewage = []
    if os.path.isfile(pointFile):
        pointfile_object = open(pointFile, "r")
        try:
            for line in pointfile_object:
                line = delSpecialStr(line)
                if len(line) > 0:
                    if line[0] is not "#":
                        pointTextArr.append(line.strip())
                        # del pointTtextArr[0]
        finally:
            pointfile_object.close()
    # Get animal farms index
    for i in range(1, len(pointTextArr)):
        pointInfo = pointTextArr[i].split('\t')
        if int(pointInfo[0]) == 10000:
            point_cattle.append(pointInfo[1])
        elif int(pointInfo[0]) == 20000:
            point_pig.append(pointInfo[1])
        else:
            point_sewage.append(pointInfo[1])
    return (point_cattle, point_pig, point_sewage)


def getBMPsInfo(pointBMPsFile):
    '''
    :param pointBMPsFile: full path of points BMPs info file
    :return: farm BMPs, cattle BMPs, pig BMPs, sewage BMPs
    '''
    # Get BMPs info
    BMPs_farm_id = []
    BMPs_farm = []
    BMPs_cattle_id = []
    BMPs_cattle = []
    BMPs_pig_id = []
    BMPs_pig = []
    BMPs_sewage_id = []
    BMPs_sewage = []

    pointbmpsTextArr = []
    if os.path.isfile(pointBMPsFile):
        pointbmpsfile_object = open(pointBMPsFile, "r")
        try:
            for line in pointbmpsfile_object:
                line = delSpecialStr(line)
                if len(line) > 0:
                    if line[0] is not "#":
                        pointbmpsTextArr.append(line.strip())
        finally:
            pointbmpsfile_object.close()
    # Get animal farms' BMPs info
    for j in range(1, len(pointbmpsTextArr)):
        pointbmpsInfo = pointbmpsTextArr[j].split('\t')
        if int(float(pointbmpsInfo[0]) / 10000.) == 1:
            BMPs_cattle_id.append(pointbmpsInfo[0])
        elif int(float(pointbmpsInfo[0]) / 10000.) == 2:
            BMPs_pig_id.append(pointbmpsInfo[0])
        else:
            BMPs_sewage_id.append(pointbmpsInfo[0])

    BMPs_farm = [0, 1]
    for cc in range(len(BMPs_cattle_id)):
        BMPs_cattle.append(int(float(BMPs_cattle_id[cc]) % 10000.))
    for pp in range(len(BMPs_pig_id)):
        BMPs_pig.append(int(float(BMPs_pig_id[pp]) % 10000.))
    for ss in range(len(BMPs_sewage_id)):
        BMPs_sewage.append(int(float(BMPs_sewage_id[ss]) % 10000.))

    # print BMPs_farm,'\n',BMPs_cattle,'\n',BMPs_pig,'\n',BMPs_sewage
    return (BMPs_farm, BMPs_cattle, BMPs_pig, BMPs_sewage)

def selectBMPatRandom(arr):
    '''
    :param arr: BMPs list
    :return: select a BMP randomly
    '''
    aLen = len(arr)
    n = random.randint(0, aLen - 1)
    return arr[n]

def getPointConfig(scenario, bmps_point, point_source, start_index, end_index):
    '''
    :param scenario: scenario array
    :param bmps_point: BMPs list of point
    :param point_source: point source list
    :param start_index: point source start index in scenario array
    :param end_index: point source end index in scenario array
    :return: config info array
    '''
    pointConfig = []
    for bc in range(len(bmps_point)):
        bmp_index = []
        bmp_index.append(bmps_point[bc])
        for c in range(start_index, end_index):
            if scenario[c] == bmps_point[bc]:
                bmp_index.append(point_source[c - start_index])
            else:
                continue
        pointConfig.append(bmp_index)
    return pointConfig

def decodPointScenario(id, pointConfig, ptsrc):
    '''
    :param pointConfig: config info array obtained from function getPointConfig()
    :return: part of BMPs scenarios text
    '''
    scenario_Table = []
    for config in pointConfig:
        if len(config) > 1:
            scenario_Row = ""
            scenario_Row += str(id) + "\tsName\t1\t" + str(ptsrc + config[0]) + "\tARRAY|point_source_distribution|"\
                            + str(ptsrc) + "\tpoint_source_management\t"
            pidArr = ""
            for pid in range(1, len(config)):
                if pid == len(config) - 1:
                    pidArr += config[pid]
                else:
                    pidArr += config[pid] + ","
            scenario_Row += pidArr
            scenario_Table.append(scenario_Row)
    return scenario_Table
