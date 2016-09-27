# -*- coding: utf-8 -*-

from hydroPlot_main import *
import datetime
import matplotlib.pyplot as plt
import os

def getDayByDay(timeStart, timeEnd):
    oneday = datetime.timedelta(days=1)
    timeArr=[timeStart]
    while timeArr[len(timeArr) - 1] < timeEnd:
        tempday = timeArr[len(timeArr) - 1] + oneday
        timeArr.append(tempday)
    return timeArr

## DateTime
def GetDateArr(timeStart, timeEnd):
    TIME_Start = datetime.datetime.strptime(timeStart, "%Y-%m-%d")
    TIME_End = datetime.datetime.strptime(timeEnd, "%Y-%m-%d")
    dateArr =  getDayByDay(TIME_Start, TIME_End)
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
    for pdata in ClimateDB.DATA_VALUES.find({'LOCALDATETIME':{"$gte":TIME_Start, '$lte':TIME_End}, 'TYPE':'P'}):
        if len(numpy.where(siteArr == pdata['STATIONID'])[0]) > 0:
            siteIndex =  numpy.where(siteArr == pdata['STATIONID'])[0][0]
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
    for obs in ClimateDB.MEASUREMENT.find({ \
            'LOCALDATETIME':{"$gte":TIME_Start, '$lte':TIME_End}, 'TYPE':simNameArr[len(simNameArr) - 1]}):
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

LFs = ['\r\n', '\n\r', '\r', '\n']

def ReadSimfromTxt(timeStart, timeEnd, dataDir, sim):
    TIME_Start = datetime.datetime.strptime(timeStart, "%Y-%m-%d")
    TIME_End = datetime.datetime.strptime(timeEnd, "%Y-%m-%d")
    ## Read simulation txt
    simData = "%s/OUTPUT/1_%s.txt" % (dataDir, sim)
    if os.path.exists(simData):
        simfile = open(simData, "r")
        simulate = []
        while True:
            line = simfile.readline()
            # print line[0]
            if line:
                for LF in LFs:
                    if LF in line:
                        line = line.split(LF)[0]
                        break
                strList = SplitStr(StripStr(line), spliters = " ")
                if len(strList) == 3:
                    dateStr = strList[0] + " " + strList[1]
                    simDatetime = datetime.datetime.strptime(dateStr, "%Y-%m-%d %H:%M:%S")
                    if simDatetime >= TIME_Start and simDatetime <= TIME_End:
                        simulate.append(float(strList[2]))
            else:
                break
        simfile.close()
    else:
        print "%s is not exist" % simData
    # print simulate
    return simulate


## Calculate Nash coefficient
def NashCoef(qObs, qSimu):
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
    return round(1 - a1 / a2, 3)


## Calculate R2
def RSquare(qObs, qSimu, obsNum):
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


def CreatPlot(sim_date, flow, preci, simList, vari_Sim, ClimateDB):
    # print datetime.datetime.strftime("%Y-%m-%:%Md %H:%S", sim_date[0])
    # print type(sim_date[0])
    timeStart = datetime.date.strftime(sim_date[0], "%Y-%m-%d")
    timeEnd = datetime.date.strftime(sim_date[len(sim_date) - 1], "%Y-%m-%d")
    for i in range(len(vari_Sim)):
        #plt.figure(i)
        fig, ax = plt.subplots(figsize = (12, 4))
        if vari_Sim[i] == "Q":
            obs_flow = SearchObs(timeStart, timeEnd, 'Q', ClimateDB)
            # print obs_flow
            plt.bar(sim_date, obs_flow[0], label = "Observation", color = "green", linewidth = 0)
            plt.plot(sim_date, simList[i], label = "Simulation", color = "orange",
                    marker = "o", markersize = 1, linewidth = 1)
            plt.xlabel('Date')
            plt.ylabel('Flow(m3/s)')
            plt.legend(bbox_to_anchor = (0.03, 0.85), loc = 2, shadow = True)
            ax.set_ylim(0, float(max(simList[i])) * 1.5)
            ax2 = ax.twinx()
            ax2.set_ylabel(r"Precipitation (mm)")
            ax2.bar(sim_date, preci, color = "blue", linewidth = 0)
            ax2.set_ylim(float(max(preci)) * 1.5, 0)
            plt.title("Simulation of SEIMS-%s in Dianbu watershed\n" % vari_Sim[i], color = "#aa0903")
            plt.title("\nNash: %.3f, R2: %.3f" % \
                    (NashCoef(obs_flow[0], simList[i]), RSquare(obs_flow[0], simList[i], len(obs_flow[1]))),
                    color = "red", loc = 'right')

        else:
            obs = SearchObs(timeStart, timeEnd, vari_Sim[i], ClimateDB)
            if obs[0][0] != -1:
                plt.bar(sim_date, obs[0], label = "Observation", color = "green", linewidth = 0)
                plt.plot(sim_date, simList[i], label = "Simulation", color = "black",
                         marker = "o", markersize = 1, linewidth = 1)
                plt.xlabel('Date')
                plt.ylabel(vari_Sim[i])
                plt.legend(bbox_to_anchor = (0.03, 0.85), loc = 2, shadow = True)
                ax.set_ylim(0, float(max(simList[i])) * 1.5)
                ax2 = ax.twinx()
                ax2.set_ylabel(r"Flow (m3/s)")
                ax2.plot(sim_date, flow, label = "Flow", color = "blue", linewidth = 1)
                ax2.set_ylim(float(max(flow)) * 1.5, 0)
                plt.title("Simulation of SEIMS-%s in Dianbu watershed\n" % vari_Sim[i], color = "#aa0903")
                plt.title("\nNash: %.3f, R2: %s" % \
                          (NashCoef(obs[0], simList[i]), str(RSquare(obs[0], simList[i], len(obs[1])))), color = "red", loc = 'right')
            else:
                plt.plot(sim_date, simList[i], label = "Simulation", color = "green",
                     marker = "o", markersize = 1, linewidth = 1)
                plt.xlabel('Date')
                plt.ylabel(vari_Sim[i])
                plt.legend(bbox_to_anchor = (0.03, 0.85), loc = 2, shadow = True)
                ax.set_ylim(0, float(max(simList[i])) * 1.5 + 1)
                ax2 = ax.twinx()
                ax2.set_ylabel(r"Flow (m3/s)")
                ax2.plot(sim_date, flow, label = "Flow", color = "blue", linewidth = 1)
                ax2.set_ylim(float(max(flow)) * 1.5, 0)
                plt.title("Simulation of SEIMS-%s in Dianbu watershed\n" % vari_Sim[i], color = "#aa0905")
    plt.show()


