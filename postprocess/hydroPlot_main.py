# -*- coding: utf-8 -*-


# import util module located in SEIMS/preprocess
import sys
sys.path.append("..")
from preprocess.util import *
from hydroPlot import *

if __name__ == "__main__":
    ## @dataDir, data folder, superior directory of "OUTPUT"
    ## @vari_Sim, Variables list

    # dataDir = r'D:\SEIMS_model\Model_data\model_dianbu_30m_longterm'
    dataDir = r'E:\code\Hydro\SEIMS\model_data\dianbu\model_dianbu_30m_longterm'
    vari_Sim = ["Q", "SED", "SEDORGNTOCH", "SEDORGPTOCH"]
    #    "CH_NO3",  "CH_TNConc", "CH_TN", "CH_TP", "CH_COD", "CH_NH4", 
    xlsFile = r'SEIMS_db_vali_2016-09-09.xlsm'
    excelData = ReadObsfromExcel(currentPath() + os.sep + xlsFile)
    sim_date = excelData[0]
    ## observation
    preci_obs = excelData[1]
    flow_obs = excelData[2]
    sed_obs = excelData[3]
    tn_obs = excelData[4]
    tp_obs = excelData[5]
    cod_obs = excelData[6]
    dataobsList = (preci_obs, flow_obs, sed_obs, tn_obs, tp_obs, cod_obs)

    ## simulation
    dataSimList = []
    for i in range(len(vari_Sim)):
        txtData = ReadSimfromTxt(dataDir, vari_Sim[i])
        dataSimList.append(txtData)
    flow_sim = ReadSimfromTxt(dataDir, "Q")

    ## Creat multiple plot
    CreatPlot(sim_date, flow_sim, preci_obs, dataobsList, dataSimList, vari_Sim)
    print "Success!"
