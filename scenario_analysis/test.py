# -*- coding: utf-8 -*-

import os
from readTextInfo import *
from pymongo import MongoClient
from matplotlib import pyplot as plt
import numpy
from preprocess.util import *

# cmdStr = "%s %s %d %d %s %d %d" % (model_Exe, model_Workdir, threadsNum, layeringMethod, HOSTNAME, PORT, 1)
# process = Popen(cmdStr, shell=True, stdout=PIPE)
# while(process.stdout.readline() != ""):
#     # line = process.stdout.readline().split("\n")
#     # if(line[0] != ""):
#     #     print line[0]
#     continue
# process.wait()
# if process.returncode == 0:
#     print "OK"

# client = MongoClient('192.168.6.55', 27017)
# db = client['BMP_Scenario_dianbu2_30m_longterm']
# collection = db.BMP_SCENARIOS
# # for i in range(10, 500):
# #     collection.remove({'ID': i})
# for i in collection.find({"ID":{"$gt":100}}):
#     # print "delete -- ID:", i["ID"]
#     collection.remove({"ID":i["ID"]})

# a = [1,2,3]
# print list(set(a[0:9]))
# for i in range(0):
#     a.append(4)
# print a

# x = numpy.array([-441571.5, -441571.5, -441571.5, -441571.5, -436185, -436185, -436185, -433998, -431811, -420025.5, -414396, -414396, -406822.5, -402327, -383980.5, -371911.5, -366282, -363771, -363771, -363771, -305937, -252477, -251019, -249480, -245308.5, -160663.5, -160663.5, 108428.5, 108428.5, 108428.5, 108428.5, 112600, 113815, 113815, 113815, 116002, 116002, 116002, 116002, 121388.5, 123818.5, 125762.5, 125762.5, 131392, 131392, 135604, 135604, 242929, 249773.5, 293351.5, 335755, 340129, 389336.5, 518612.5, 735248.5, 736666, 749788, 878821, 878821])
# y = numpy.array([52931.00339, 53003.7745, 53034.62467, 53068.26122, 52427.15561, 52648.68467, 52755.28563, 52649.64126, 52614.45263, 52586.91077, 52518.0422, 52701.1552, 52315.39673, 52595.05569, 52587.18609, 52417.63503, 52575.8233, 52306.81708, 52482.28307, 52545.37226, 52507.21331, 52491.62617, 52327.80031, 52140.46525, 52448.04452, 52376.97295, 52389.33154, 17909.11985, 18042.68096, 18156.24992, 18207.15535, 17803.81089, 17742.22948, 17838.59702, 17888.04761, 17708.2963, 17756.0589, 17875.34572, 17962.71452, 17780.45446, 17652.65199, 17410.36993, 17696.26499, 17555.97472, 17776.18686, 17612.00234, 17651.92225, 17627.15111, 17547.22489, 17518.53965, 17353.12383, 17587.39021, 17596.01176, 17557.72037, 17489.21132, 17568.34696, 17566.45517, 17375.00015, 17509.64248])
# Plot
# plt.ylim(10, 60)
# plt.title("Pareto frontier of Scenarios Optimization\n", color="#aa0903")
# plt.xlabel("Economic cost(Million Yuan)")
# plt.ylabel("Pollution load(t)")
# x /= 1000000.
# y /= 1000.
# plt.scatter(x, y, c="r", alpha=0.9, s=12)
# plt.title("\nPopulation: %d, Generation: %d" % (60, 50), color="green", fontsize=9, loc='right')
# plt.annotate('S_1', xy=(0.1, 126.5), xycoords='data', xytext=(-30, -30), textcoords='offset points',
#                 arrowprops=dict(arrowstyle="->"))
# plt.annotate('S_2', xy=(3.1, 93), xycoords='data', xytext=(-50, -20), textcoords='offset points',
#                 arrowprops=dict(arrowstyle="->"))
# plt.annotate('S_3', xy=(5.8, 65), xycoords='data', xytext=(0, 30), textcoords='offset points',
#                 arrowprops=dict(arrowstyle="->"))
# plt.show()

def getGenerationsInfo(f, gen):
    textArr = []
    if os.path.isfile(f):
        fieldfile_object = open(f, "r")
        for line in fieldfile_object:
            line = delSpecialStr(line)
            textArr.append(line.strip())

    genIDArr = []
    indexArr = []
    for i in range(len(textArr)):
        if textArr[i][0] == "#":
            iArr = textArr[i].split(" ")
            if len(iArr) == 3:
                genID = int(iArr[1].split("_")[1])
                genIDArr.append(genID)
                indexArr.append(i)
            if len(iArr) == 4:
                genIDArr.append(-1)
                indexArr.append(i)
    # print genIDArr
    # print indexArr

    allpoints = []
    points = []
    for j in range(len(genIDArr) - 1):
        cost = []
        benefit = []
        for m in range(indexArr[j] + 2, indexArr[j + 1] - 1):
            # print textArr[m].split("\t")[0]
            cost.append(float(textArr[m].split("\t")[0]))
            benefit.append(float(textArr[m].split("\t")[1]))
        points.append(cost)
        points.append(benefit)
        allpoints.append(points)
        points = []
    # print allpoints[0]
    genPlotData = []
    for g in gen:
        genPlotData.append(allpoints[g])
    return genPlotData

resultLog = r'C:\Users\dell\Desktop\sa\3_Gen_50_Pop_60\Gen_50_Pop_60_resultLog.txt'
genPlot = [5, 10, 20, 30, 40, -1]
markerlabel = [5, 10, 20, 30, 40, 50]
genPlotData = getGenerationsInfo(resultLog, genPlot)

fig, ax = plt.subplots(figsize=(12, 6))
plt.title("Pareto frontier of Scenarios Optimization\n", color="#aa0903")

colormap = plt.cm.Paired
plt.gca().set_color_cycle([colormap(i) for i in numpy.linspace(0, 0.8, 10)])

for f in range(len(genPlotData)):
    x = numpy.array(genPlotData[f][0]) / 1000000.
    y = numpy.array(genPlotData[f][1]) / 1000.

    plt.plot(x, y, marker='.', markersize=12, linestyle='none', alpha=0.9, label="Gen_" + str(markerlabel[f]))
    plt.legend(fontsize=12, loc=3)
    plt.grid(True)

plt.title("\nPopulation: %d, Generation: %d" % (60, 50), color="green", fontsize=9, loc='right')
plt.xlabel("Economic cost(Million Yuan)")
plt.ylabel("Pollution load(t)")
plt.show()

# Create scenario raster
# sa_workDir = r'C:\Users\dell\Desktop\sa\1_Gen_50_Pop_60'
# sa = 'S_1_3.tiff'
# fieldFile = r'D:\GaohrWS\GithubPrj\SEIMS\model_data\dianbu\data_prepare\spatial\mgtfield_t100_3.txt'
# fieldTiff = r'D:\GaohrWS\GithubPrj\SEIMS\model_data\dianbu\data_prepare\spatial\mgtfield_t100_3.tif'
# farm_type = [1, 33]
# fieldInfo = getFieldInfo(fieldFile)
# farm_lu = fieldInfo[1]
# sa_farm_arr = numpy.array([1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0])
# # print sa_farm[0:len(fieldInfo[1])]
#
# # read raster info
# field = ReadRaster(fieldTiff).data
# nRows = ReadRaster(fieldTiff).nRows
# nCols = ReadRaster(fieldTiff).nCols
# geotrans = ReadRaster(fieldTiff).geotrans
# srs = ReadRaster(fieldTiff).srs
# noDataVal = ReadRaster(fieldTiff).noDataValue
#
# sa_farm = numpy.zeros((nRows, nCols))
# print "Calculating..."
# for i in range(nRows):
#     for j in range(nCols):
#         if int(field[i][j]) in farm_lu:
#             sa_index = numpy.where(farm_lu == field[i][j])
#             if sa_farm_arr[sa_index] == 0:
#                 sa_farm[i][j] = 0.
#             else:
#                 sa_farm[i][j] = 1.
#         else:
#             if field[i][j] == noDataVal:
#                 sa_farm[i][j] = field[i][j]
#             else:
#                 sa_farm[i][j] = -1.
#
# outputFile = sa_workDir + os.sep + sa
# WriteGTiffFile(outputFile, nRows, nCols, sa_farm, geotrans, srs, noDataVal, gdal.GDT_Float32)
# print "Finished!"