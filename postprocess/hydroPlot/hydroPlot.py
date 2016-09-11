# -*- coding: utf-8 -*-

#from __future__ import print_function
import numpy
import matplotlib
from matplotlib.ticker import FuncFormatter
import matplotlib.pyplot as plt
import matplotlib.dates
from matplotlib.dates import bytespdate2num
import os
import numpy
import xlrd, time, datetime
from util import *
from hydroPlot_main import  *

def ReadObsfromExcel(xlsfile):
    ## hydro
    bk = xlrd.open_workbook(xlsfile)
    sim_date = []
    preci = []
    flow = []
    sed = []
    tn = []
    tp = []
    cod = []

    #for sh in bk.sheets():
    ish = 0
    for sh in bk.sheets():
        sheet_name = bk._sheet_names[ish]
        ish += 1
        if(sheet_name == "sim_obs"):
            for row in range(366, sh.nrows):
                sim_date.append(datetime.datetime(*xlrd.xldate_as_tuple(sh.cell(row, 1).value, 0)))
                flowobs_value = sh.cell(row, 14).value
                sedobs_value = sh.cell(row, 15).value
                tnobs_value = sh.cell(row, 22).value
                tpobs_value = sh.cell(row, 23).value
                codobs_value = sh.cell(row, 24).value
                Definezero(flowobs_value, flow)
                Definezero(sedobs_value, sed)
                Definezero(tnobs_value, tn)
                Definezero(tpobs_value, tp)
                Definezero(codobs_value, cod)
            #print cod

        ## Precipatation
        if(sheet_name == "rainfall"):
            for row in range(366, sh.nrows):
                preci_value = sh.cell(row, 8).value
                preci.append(preci_value)
    return (sim_date, preci, flow, sed, tn, tp, cod)

def Definezero(value, arr):
    if value == 42:
        value = 0
    arr.append(value)

def selectType(sim):
    if sim is "Q":
        return 1
    elif sim is "SED":
        return 2
    elif sim is "CH_TN":
        return 3
    elif sim is "CH_TP":
        return 4
    elif sim is "CH_COD":
        return 5
    else:
        return -1


def ReadSimfromTxt(dataDir, data_Sim):
    ## Read simulation txt
    simData = "%s/OUTPUT/1_%s.txt" % (dataDir, data_Sim)
    simfile = open(simData, "r")
    simulate = []
    while True:
        line = simfile.readline()
        #print line[0]
        if line:
            
            if str(line[0:4]) == "2014":
                #print line[0:4]
                strList = SplitStr(StripStr(line), spliters=" ")
                simulate.append(strList[2])
                #print strList
        else:
            break
    simfile.close()
    return simulate

## Caiculate Nash coefficient
def NashCoef(qObs, qSimu):
    n = len(qObs)
    ave = sum(qObs)/n
    a1 = 0
    a2 = 0
    for i in range(n):
        if qObs[i] != 0:
            a1 = a1 + pow(float(qObs[i]) - float(qSimu[i]), 2)
            a2 = a2 + pow(float(qObs[i]) - ave, 2)
    if a2 == 0:
        a2 = 1.e-6
    return 1 - a1/a2

## Calculate R2
def RSquare(qObs, qSimu):
    n = len(qObs)
    sim = []
    for k in range(n):
        sim.append(float(qSimu[k]))
    obsAvg = sum(qObs) / n
    predAvg = sum(sim) / n
    obsMinusAvgSq = 0
    predMinusAvgSq = 0
    obsPredMinusAvgs = 0
    for i in range(n):
        if qObs[i] != 0:
            obsMinusAvgSq = obsMinusAvgSq + pow((qObs[i] - obsAvg), 2)
            predMinusAvgSq = predMinusAvgSq + pow((sim[i] - predAvg), 2)
            obsPredMinusAvgs = obsPredMinusAvgs + (qObs[i] - obsAvg) * (sim[i] - predAvg)
    ## Calculate RSQUARE
    yy = (pow(obsMinusAvgSq, 0.5) * pow(predMinusAvgSq, 0.5))
    if yy == 0:
        yy = 1.e-6
    RSquare = pow((obsPredMinusAvgs / yy), 2)
    return RSquare

def CreatPlot(sim_date, flow, hydro, obsList, simList, vari_Sim):
    for i in range(len(vari_Sim)):
        plt.figure(i)
        fig, ax = plt.subplots(figsize=(12,4))
        #fig.autofmt_xdate()
        type = selectType(vari_Sim[i])
        if type != -1:
            if type == 1:
                plt.bar(sim_date, obsList[type], label = "Observation")
                plt.plot(sim_date, simList[i], label = "Simulation", color="orange",
                         marker = "o", markersize = 1, linewidth = 1)
                plt.xlabel('Date')
                plt.ylabel('Flow(m3/s)')
                plt.legend(bbox_to_anchor=(0.03,0.85), loc = 2, shadow = True)
                ax.set_ylim(0, float(max(simList[i])) * 4)
                ax2 = ax.twinx()
                ax2.set_ylabel(r"Precipitation (mm)")
                ax2.bar(sim_date, hydro, color = "blue", linewidth = 1)
                ax2.set_ylim(float(max(hydro)) * 4 ,0)
                plt.title("Simulation of SEIMS-%s in Dianbu watershed\n" % vari_Sim[i],color = "#aa0903")
                plt.title("\nNash: %f, R2: %f" % \
                          (NashCoef(obsList[type], simList[i]), RSquare(obsList[type], simList[i])),
                          color = "red", loc='right')
                #print min(sim_date)
            else:
                plt.bar(sim_date, obsList[type], label = "Observation", color = "green")
                plt.plot(sim_date, simList[i], label = "Simulation", color="black",
                         marker = "o", markersize = 1, linewidth = 1)
                plt.xlabel('Date')
                plt.ylabel(vari_Sim[i])
                plt.legend(bbox_to_anchor=(0.03,0.85), loc = 2, shadow = True)
                ax.set_ylim(0, float(max(simList[i])) * 6)
                ax2 = ax.twinx()
                ax2.set_ylabel(r"Flow (m3/s)")
                ax2.plot(sim_date, flow, label = "Flow", color="blue", linewidth = 1)
                ax2.set_ylim(float(max(flow)) * 6 ,0)
                plt.title("Simulation of SEIMS-%s in Dianbu watershed\n" % vari_Sim[i], color = "#aa0903")
                plt.title("\nNash: %f, R2: %f" % \
                          (NashCoef(obsList[type], simList[i]), RSquare(obsList[type], simList[i])),
                          color = "red", loc='right')
        else:
            plt.plot(sim_date, simList[i], label = "Simulation", color = "green",
                         marker = "o", markersize = 1, linewidth = 1)
            plt.xlabel('Date')
            plt.ylabel(vari_Sim[i])
            plt.legend(bbox_to_anchor=(0.03,0.85), loc = 2, shadow = True)
            ax.set_ylim(0, float(max(simList[i])) * 6 + 10)
            ax2 = ax.twinx()
            ax2.set_ylabel(r"Flow (m3/s)")
            ax2.plot(sim_date, flow, label = "Flow", color="blue", linewidth = 1)
            ax2.set_ylim(float(max(flow)) * 6 ,0)
            plt.title("Simulation of SEIMS-%s in Dianbu watershed\n" % vari_Sim[i], color = "#aa0905")
            plt.title("\nNash: %f, R2: %f" % \
                          (NashCoef(obsList[type], simList[i]), RSquare(obsList[type], simList[i])),
                          color = "red", loc='right')

    plt.show()

def currentPath():
    path = sys.path[0]
    if os.path.isdir(path):
        return path
    elif os.path.isfile(path):
        return os.path.dirname(path)
