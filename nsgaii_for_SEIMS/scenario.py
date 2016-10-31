# -*- coding: utf-8 -*-
# @Class Scenario
# @Author Huiran GAO
# @Date   2016-10-29

import os, sys
import random
from pymongo import MongoClient
from config import *
from readTextInfo import *

class Scenario:
    def __init__(self):
        self.id = None
        self.attributes = []
        self.field_Num = farm_Num
        self.point_cattle_Num = point_cattle_Num
        self.point_pig_Num = point_pig_Num
        self.point_sewage_Num = point_sewage_Num
        self.sce_list = []

    def getIdfromMong(self):
        client = MongoClient(HOSTNAME, PORT)
        db = client[BMPScenarioDBName]
        collection = db.BMP_SCENARIOS
        idsList = []
        for s in collection.find():
            idsList.append(int(s['ID']))
        idList = list(set(idsList))
        self.id = idList[-1] + 1

    def create(self):
        # Create a scenario numeric string
        for _ in range(self.field_Num):
            self.attributes.append(selectBMPatRandom(bmps_farm))
        for _ in range(self.point_cattle_Num):
            self.attributes.append(selectBMPatRandom(bmps_cattle))
        for _ in range(self.point_pig_Num):
            self.attributes.append(selectBMPatRandom(bmps_pig))
        for _ in range(self.point_sewage_Num):
            self.attributes.append(selectBMPatRandom(bmps_sewage))

    def decoding(self):
        # scenario section
        field_index = self.field_Num
        point_cattle_index = self.point_cattle_Num + field_index
        point_pig_index = self.point_pig_Num + point_cattle_index
        point_sewage_index = self.point_sewage_Num + point_pig_index
        # farm field
        for f in range(len(bmps_farm)):
            scenario_Row = ""
            scenario_Row += str(self.id) + "\tsName\t12\t"
            farm_BMP_do = False
            for i in range(0, field_index):
                if self.attributes[i] == 1:
                    farm_BMP_do = True
                else:
                    farm_BMP_do = False
            if farm_BMP_do:
                scenario_Row += str(bmps_farm[f] + 2) + "\t"
            else:
                scenario_Row += str(bmps_farm[f]) + "\t"
            scenario_Row += "RASTER|MGT_FIELDS\tplant_management\tALL"
            self.sce_list.append(scenario_Row)
        # point source
        cattleConfig = getPointConfig(self.attributes, bmps_cattle, point_cattle, field_index, point_cattle_index)
        pigConfig = getPointConfig(self.attributes, bmps_pig, point_pig, point_cattle_index, point_pig_index)
        sewageConfig = getPointConfig(self.attributes, bmps_sewage, point_sewage, point_pig_index, point_sewage_index)
        self.sce_list.extend(decodPointScenario(self.id, cattleConfig, 10000))
        self.sce_list.extend(decodPointScenario(self.id, pigConfig, 20000))
        self.sce_list.extend(decodPointScenario(self.id, sewageConfig, 40000))

    def importoMongo(self):
        '''
        Import scenario list to MongoDB
        :return:
        '''
        client = MongoClient(HOSTNAME, PORT)
        db = client[BMPScenarioDBName]
        collection = db.BMP_SCENARIOS
        keyarray = ["ID", "NAME", "BMPID", "SUBSCENARIO", "DISTRIBUTION", "COLLECTION", "LOCATION"]
        for line in self.sce_list:
            conf = {}
            li_list = line.split('\t')
            for i in range(len(li_list)):
                conf[keyarray[i]] = li_list[i]
            collection.insert(conf)

if __name__ == "__main__":
    Sce = Scenario()
    Sce.getIdfromMong()
    Sce.create()
    Sce.decoding()
    Sce.importoMongo()
