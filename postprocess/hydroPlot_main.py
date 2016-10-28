# -*- coding: utf-8 -*-

from pymongo import MongoClient
from pymongo.errors import ConnectionFailure

from config import *
from hydroPlot import *

if __name__ == "__main__":
    LoadConfiguration(GetINIfile())

    ClimateDB = MongoClient(host=HOSTNAME, port=PORT)[ClimateDBName]
    SpatialDB = MongoClient(host=HOSTNAME, port=PORT)[SpatialDBName]
    # Date Array
    dateArr = GetDateArr(TIME_Start, TIME_End)
    # Precipatation
    preci = GetPreciObs(TIME_Start, TIME_End, ClimateDB, SpatialDB)

    # simulation
    dataSimList = []
    for i in range(len(PLOT_VARS)):
        txtData = ReadSimfromTxt(TIME_Start, TIME_End, MODEL_DIR, PLOT_VARS[i])
        dataSimList.append(txtData)

    # # Create multiple plot
    # sim_flow = ReadSimfromTxt(TIME_Start, TIME_End, MODEL_DIR, "Q")
    # SearchObs(TIME_Start, TIME_End, 'Q', ClimateDB)
    # CreatePlot(dateArr, sim_flow, preci, dataSimList, PLOT_VARS, MODEL_DIR, ClimateDB)

    # Create multi hydrographs, updated by LJ
    CreatePlot2(dateArr, preci, dataSimList, PLOT_VARS, MODEL_DIR, ClimateDB)

