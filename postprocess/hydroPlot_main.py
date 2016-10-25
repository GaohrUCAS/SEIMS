# -*- coding: utf-8 -*-

from pymongo import MongoClient
from pymongo.errors import ConnectionFailure

from config import *
from hydroPlot import *

if __name__ == "__main__":
    LoadConfiguration(GetINIfile())
    ## @dataDir, data folder, superior directory of "OUTPUT"
    ## @PLOT_VARS, Variables list

    # dataDir = r'D:\SEIMS_model\Model_data\model_dianbu_30m_longterm'
    # MODEL_DIR = r'E:\code\Hydro\SEIMS\model_data\dianbu\model_dianbu_30m_longterm'
    #PLOT_VARS = ["Q", "SED"]
    #    "CH_NO3",  "CH_TNConc", "CH_TN", "CH_TP", "CH_COD", "CH_NH4", "SEDORGNTOCH", "SEDORGPTOCH"

    ClimateDB = MongoClient(host=HOSTNAME, port=PORT)[ClimateDBName]
    SpatialDB = MongoClient(host=HOSTNAME, port=PORT)[SpatialDBName]
    ## Date Array
    dateArr = GetDateArr(TIME_Start, TIME_End)
    ## Precipatation
    preci = GetPreciObs(TIME_Start, TIME_End, ClimateDB, SpatialDB)

    ## simulation
    dataSimList = []
    for i in range(len(PLOT_VARS)):
        txtData = ReadSimfromTxt(TIME_Start, TIME_End, MODEL_DIR, PLOT_VARS[i])
        dataSimList.append(txtData)
    sim_flow = ReadSimfromTxt(TIME_Start, TIME_End, MODEL_DIR, "Q")
    SearchObs(TIME_Start, TIME_End, 'Q', ClimateDB)

    ## Creat multiple plot
    CreatPlot(dateArr, sim_flow, preci, dataSimList, PLOT_VARS, MODEL_DIR, ClimateDB)
    print "Success!"
