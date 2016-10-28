# -*- coding: utf-8 -*-

import datetime
import os

import matplotlib.dates as mdates
import matplotlib.pyplot as plt
import numpy

from config import *


def getDayByDay(timeStart, timeEnd):
    oneday = datetime.timedelta(days=1)
    timeArr = [timeStart]
    while timeArr[len(timeArr) - 1] < timeEnd:
        tempday = timeArr[len(timeArr) - 1] + oneday
        timeArr.append(tempday)
    return timeArr


## DateTime
def GetDateArr(timeStart, timeEnd):
    TIME_Start = datetime.datetime.strptime(timeStart, "%Y-%m-%d")
    TIME_End = datetime.datetime.strptime(timeEnd, "%Y-%m-%d")
    dateArr = getDayByDay(TIME_Start, TIME_End)
    # print dateArr
    return dateArr


## Precipatation
def GetPreciObs(timeStart, timeEnd, ClimateDB, SpatialDB):
    TIME_Start = datetime.datetime.strptime(timeStart, "%Y-%m-%d")
    TIME_End = datetime.datetime.strptime(timeEnd, "%Y-%m-%d")
    preci = []
    siteListstr = SpatialDB.SITELIST.find_one()['SITELISTP'].split(',')
    siteList = []
    allSiteValue = []
    for s in range(len(siteListstr)):
        siteList.append(int(siteListstr[s]))
        allSiteValue.append([])
    siteArr = numpy.array(siteList)
    for pdata in ClimateDB.DATA_VALUES.find({'LOCALDATETIME': {"$gte": TIME_Start, '$lte': TIME_End}, 'TYPE': 'P'}):
        if len(numpy.where(siteArr == pdata['STATIONID'])[0]) > 0:
            siteIndex = numpy.where(siteArr == pdata['STATIONID'])[0][0]
            allSiteValue[siteIndex].append(pdata['VALUE'])
            # print type(pdata['STATIONID'])
    ## Sum of all sites value and average
    for i in range(len(allSiteValue[0])):
        preci.append(sum([x[i] for x in allSiteValue]) / len(siteArr))
    # print preci
    return preci


## Search observed value
def SearchObs(timeStart, timeEnd, sim, ClimateDB):
    TIME_Start = datetime.datetime.strptime(timeStart, "%Y-%m-%d")
    TIME_End = datetime.datetime.strptime(timeEnd, "%Y-%m-%d")
    simNameArr = sim.split('_')
    # print simNameArr
    obsValue = []
    obsDate = []
    fieldList = ClimateDB.collection_names()
    if "MEASUREMENT" in fieldList:
        for obs in ClimateDB.MEASUREMENT.find({
            'LOCALDATETIME': {"$gte": TIME_Start, '$lte': TIME_End}, 'TYPE': simNameArr[len(simNameArr) - 1]}):
            # print obs['TYPE']
            obsValue.append(obs['VALUE'])
            obsDate.append(obs['LOCALDATETIME'])
        dateArr = GetDateArr(timeStart, timeEnd)
        obsValueArr = numpy.zeros(len(dateArr))
        if len(obsValue) > 0:
            for s in range(len(dateArr)):
                for t in range(len(obsDate)):
                    if dateArr[s] == obsDate[t]:
                        obsValueArr[s] = obsValue[t]
            # print obsValueArr
            return (obsValueArr, obsValue)
        else:
            return [[-1]]
    else:
        # print "None"
        return [[-1]]


LFs = ['\r\n', '\n\r', '\r', '\n']


def ReadSimfromTxt(timeStart, timeEnd, dataDir, sim):
    TIME_Start = datetime.datetime.strptime(timeStart, "%Y-%m-%d")
    TIME_End = datetime.datetime.strptime(timeEnd, "%Y-%m-%d")
    ## Read simulation txt
    simData = "%s/1_%s.txt" % (dataDir, sim)
    simulate = []
    if os.path.exists(simData):
        simfile = open(simData, "r")
        while True:
            line = simfile.readline()
            # print line[0]
            if line:
                for LF in LFs:
                    if LF in line:
                        line = line.split(LF)[0]
                        break
                strList = SplitStr(StripStr(line), spliters=" ")
                if len(strList) == 3:
                    dateStr = strList[0] + " " + strList[1]
                    simDatetime = datetime.datetime.strptime(dateStr, "%Y-%m-%d %H:%M:%S")
                    if simDatetime >= TIME_Start and simDatetime <= TIME_End:
                        simulate.append(float(strList[2]))
            else:
                break
        simfile.close()
        # print simulate
        return simulate
    else:
        raise (IOError, "%s is not exist" % simData)


## Calculate Nash coefficient
def NashCoef(qObs, qSimu, obsNum=9999):
    n = len(qObs)
    ave = sum(qObs) / n
    a1 = 0
    a2 = 0
    for i in range(n):
        if qObs[i] != 0:
            a1 = a1 + pow(float(qObs[i]) - float(qSimu[i]), 2)
            a2 = a2 + pow(float(qObs[i]) - ave, 2)
    if a2 == 0:
        a2 = 1.e-6
    if obsNum > 1:
        return "%.3f" % round(1 - a1 / a2, 3)
    else:
        return "NAN"


## Calculate R2
def RSquare(qObs, qSimu, obsNum=9999):
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
    RSquare = round(pow((obsPredMinusAvgs / yy), 2), 3)
    if obsNum > 1:
        return "%.3f" % RSquare
    else:
        return "NAN"


def CreatePlot(sim_date, flow, preci, simList, vari_Sim, model_dir, ClimateDB):
    # print datetime.datetime.strftime("%Y-%m-%:%Md %H:%S", sim_date[0])
    # print type(sim_date[0])
    # set ticks direction, in or out
    plt.rcParams['xtick.direction'] = 'out'
    plt.rcParams['ytick.direction'] = 'out'
    timeStart = datetime.date.strftime(sim_date[0], "%Y-%m-%d")
    timeEnd = datetime.date.strftime(sim_date[len(sim_date) - 1], "%Y-%m-%d")
    for i in range(len(vari_Sim)):
        # plt.figure(i)
        fig, ax = plt.subplots(figsize=(12, 4))
        if vari_Sim[i] == "Q":
            ylabelStr = vari_Sim[i]
            if vari_Sim[i] == "Q":
                ylabelStr += " (m$^3$/s)"
            else:
                ylabelStr += " (kg)"
            obs = SearchObs(timeStart, timeEnd, vari_Sim[i], ClimateDB)
            if obs[0][0] != -1:
                # Flow with observed value
                p1 = ax.bar(sim_date, obs[0], label="Observation", color="none", edgecolor='black',
                        linewidth=1, align="center", hatch="//")
                p2, = ax.plot(sim_date, simList[i], label="Simulation", color="black",
                        marker="o", markersize=2, linewidth=1)
                plt.xlabel('Date')
                # format the ticks date axis
                # autodates = mdates.AutoDateLocator()
                days = mdates.DayLocator(bymonthday=range(1, 32), interval=4)
                months = mdates.MonthLocator()
                dateFmt = mdates.DateFormatter('%m-%d')
                ax.xaxis.set_major_locator(months)
                ax.xaxis.set_major_formatter(dateFmt)
                ax.xaxis.set_minor_locator(days)
                ax.tick_params('both', length=5, width=2, which='major')
                ax.tick_params('both', length=3, width=1, which='minor')
                # fig.autofmt_xdate()

                plt.ylabel(ylabelStr)
                # plt.legend(bbox_to_anchor = (0.03, 0.85), loc = 2, shadow = True)
                ax.set_ylim(float(min(simList[i])) * 0.8, float(max(simList[i])) * 1.8)
                ax2 = ax.twinx()
                ax.tick_params(axis='x', which='both', bottom='on', top='off')
                ax2.tick_params('y', length=5, width=2, which='major')
                ax2.set_ylabel(r"Precipitation (mm)")
                p3 = ax2.bar(sim_date, preci, label="Rainfall", color="black", linewidth=0, align="center")
                ax2.set_ylim(float(max(preci)) * 1.8, float(min(preci)) * 0.8)
                ax.legend([p3, p1, p2], ["Rainfall", "Observation", "Simulation"], bbox_to_anchor=(0.03, 0.85), loc=2,
                  shadow=True)
                plt.title("Simulation of %s\n" % vari_Sim[i], color="#aa0903")
                plt.title("\nNash: %s, R$^2$: %s" %
                    (NashCoef(obs[0], simList[i], len(obs[1])),
                    RSquare(obs[0], simList[i], len(obs[1]))),
                    color="red", loc='right')
                plt.tight_layout()
                plt.savefig(model_dir + os.sep + vari_Sim[i] + ".png")
            else:
                # Flow without observed value
                p2, = ax.plot(sim_date, simList[i], label="Simulation", color="black",
                        marker="o", markersize=2, linewidth=1)
                plt.xlabel('Date')
                # format the ticks date axis
                # autodates = mdates.AutoDateLocator()
                days = mdates.DayLocator(bymonthday=range(1, 32), interval=4)
                months = mdates.MonthLocator()
                dateFmt = mdates.DateFormatter('%m-%d')
                ax.xaxis.set_major_locator(months)
                ax.xaxis.set_major_formatter(dateFmt)
                ax.xaxis.set_minor_locator(days)
                ax.tick_params('both', length=5, width=2, which='major')
                ax.tick_params('both', length=3, width=1, which='minor')
                # fig.autofmt_xdate()

                plt.ylabel(ylabelStr)
                # plt.legend(bbox_to_anchor = (0.03, 0.85), loc = 2, shadow = True)
                ax.set_ylim(float(min(simList[i])) * 0.8, float(max(simList[i])) * 1.8)
                ax2 = ax.twinx()
                ax.tick_params(axis='x', which='both', bottom='on', top='off')
                ax2.tick_params('y', length=5, width=2, which='major')
                ax2.set_ylabel(r"Precipitation (mm)")
                p3 = ax2.bar(sim_date, preci, label="Rainfall", color="black", linewidth=0, align="center")
                ax2.set_ylim(float(max(preci)) * 1.8, float(min(preci)) * 0.8)
                ax.legend([p3, p2], ["Rainfall", "Simulation"], bbox_to_anchor=(0.03, 0.85), loc=2,
                  shadow=True)
                plt.title("Simulation of %s\n" % vari_Sim[i], color="#aa0903")
                plt.tight_layout()
                plt.savefig(model_dir + os.sep + vari_Sim[i] + ".png")

        else:
            obs = SearchObs(timeStart, timeEnd, vari_Sim[i], ClimateDB)
            if obs[0][0] != -1:
                # Simulation with observed value
                plt.bar(sim_date, obs[0], label = "Observation", color = "green", linewidth = 0, align = "center")
                plt.plot(sim_date, simList[i], label = "Simulation", color = "black",
                         marker = "o", markersize = 1, linewidth = 1)
                plt.xlabel('Date')
                plt.ylabel(vari_Sim[i])
                plt.legend(bbox_to_anchor = (0.03, 0.85), loc = 2, shadow = True)
                ax.set_ylim(float(min(simList[i])) * 0.8, float(max(simList[i])) * 1.8)
                ax2 = ax.twinx()
                ax2.set_ylabel(r"Flow (m$^3$/s)")
                ax2.plot(sim_date, flow, label = "Flow", color = "blue", linewidth = 1)
                ax2.set_ylim(float(max(flow)) * 1.8, float(min(flow)) * 0.8)
                plt.title("Simulation of %s \n" % vari_Sim[i], color = "#aa0903")
                plt.title("\nNash: %s, R$^2$: %s" %
                          (NashCoef(obs[0], simList[i], len(obs[1])),
                           str(RSquare(obs[0], simList[i], len(obs[1])))), color = "red", loc = 'right')
            else:
                # Simulation without observed value
                plt.plot(sim_date, simList[i], label = "Simulation", color = "green",
                         marker = "o", markersize = 1, linewidth = 1)
                plt.xlabel('Date')
                plt.ylabel(vari_Sim[i])
                plt.legend(bbox_to_anchor = (0.03, 0.85), loc = 2, shadow = True)
                ax.set_ylim(0, float(max(simList[i])) * 1.8 + 1)
                ax2 = ax.twinx()
                ax2.set_ylabel(r"Flow (m$^3$/s)")
                ax2.plot(sim_date, flow, label = "Flow", color = "blue", linewidth = 1)
                ax2.set_ylim(float(max(flow)) * 1.8, float(min(flow)) * 0.8)
                plt.title("Simulation of %s \n" % vari_Sim[i], color = "#aa0905")
    plt.show()

def CreatePlot2(sim_date, preci, simList, vari_Sim, model_dir, ClimateDB):
    '''
    Create hydrographs of discharge, sediment, nutrient (amount or concentrate), etc.
    :param sim_date:
    :param preci:
    :param simList:
    :param vari_Sim:
    :param model_dir:
    :param ClimateDB:
    :return:
    '''
    # print datetime.datetime.strftime("%Y-%m-%:%Md %H:%S", sim_date[0])
    # print type(sim_date[0])
    # set ticks direction, in or out
    plt.rcParams['xtick.direction'] = 'out'
    plt.rcParams['ytick.direction'] = 'out'
    timeStart = datetime.date.strftime(sim_date[0], "%Y-%m-%d")
    timeEnd = datetime.date.strftime(sim_date[len(sim_date) - 1], "%Y-%m-%d")
    for i in range(len(vari_Sim)):
        # plt.figure(i)
        fig, ax = plt.subplots(figsize = (12, 4))
        # if vari_Sim[i] == "Q":
        ylabelStr = vari_Sim[i]
        if vari_Sim[i] == "Q":
            ylabelStr += " (m$^3$/s)"
        elif "CONC" in vari_Sim[i].upper(): # Concentrate
            ylabelStr += " (mg/L)"
        else: # amount
            ylabelStr += " (kg)"
        print ylabelStr
        obs = SearchObs(timeStart, timeEnd, vari_Sim[i], ClimateDB)
        # print obs
        p1 = ax.bar(sim_date, obs[0], label = "Observation", color = "none", edgecolor = 'black',
                    linewidth = 1, align = "center", hatch = "//")
        p2, = ax.plot(sim_date, simList[i], label = "Simulation", color = "black",
                      marker = "o", markersize = 2, linewidth = 1)
        plt.xlabel('Date')
        # format the ticks date axis
        # autodates = mdates.AutoDateLocator()
        days = mdates.DayLocator(bymonthday = range(1, 32), interval = 4)
        months = mdates.MonthLocator()
        dateFmt = mdates.DateFormatter('%m-%d')
        ax.xaxis.set_major_locator(months)
        ax.xaxis.set_major_formatter(dateFmt)
        ax.xaxis.set_minor_locator(days)
        ax.tick_params('both', length = 5, width = 2, which = 'major')
        ax.tick_params('both', length = 3, width = 1, which = 'minor')
        # fig.autofmt_xdate()

        plt.ylabel(ylabelStr)
        # plt.legend(bbox_to_anchor = (0.03, 0.85), loc = 2, shadow = True)
        ax.set_ylim(float(min(simList[i])) * 0.8, float(max(simList[i])) * 1.8)
        ax2 = ax.twinx()
        ax.tick_params(axis = 'x', which = 'both', bottom = 'on', top = 'off')
        ax2.tick_params('y', length = 5, width = 2, which = 'major')
        ax2.set_ylabel(r"Precipitation (mm)")
        p3 = ax2.bar(sim_date, preci, label = "Rainfall", color = "black", linewidth = 0, align = "center")
        ax2.set_ylim(float(max(preci)) * 1.8, float(min(preci)) * 0.8)
        ax.legend([p3, p1, p2], ["Rainfall", "Observation", "Simulation"], bbox_to_anchor = (0.03, 0.85), loc = 2,
                  shadow = True)
        plt.title("Simulation of %s\n" % vari_Sim[i], color = "#aa0903")
        plt.title("\nNash: %s, R$^2$: %s" %
                  (NashCoef(obs[0], simList[i], len(obs[1])),
                   RSquare(obs[0], simList[i], len(obs[1]))),
                  color = "red", loc = 'right')
        plt.tight_layout()
        plt.savefig(model_dir + os.sep + vari_Sim[i] + ".png")
    plt.show()
