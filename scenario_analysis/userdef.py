# -*- coding: utf-8 -*-
'''
User defined tools
'''
# @Class userdef
# @Author Huiran GAO
# @Date   2016-11-08

import uuid
from math import *
from scenario import *

# Model
def __uniqueid__():
    id = int(str(uuid.uuid4().fields[-1])[:8])
    while True:
        yield id
        id += 1

def calBenefitandCost(individual):
    Sce = Scenario()

    # random.seed()
    # ms = float(random.randint(0, 1000))
    # time.sleep(ms / 1000.)

    #Sce.getIdfromMongo()
    Sce.setId(__uniqueid__().next())
    Sce.attributes = individual
    Sce.decoding()
    Sce.importoMongo(HOSTNAME, PORT, BMPScenarioDBName)
    # Calculate benefit and cost
    Sce.cost()
    Sce.benefit()
    # Save scenarios information in file
    Sce.saveInfo(scenariosInfo)
    f1 = Sce.cost_eco
    f2 = Sce.benefit_env
    return f1, f2

def test(individual):
    g = sum(individual) / len(individual)
    f1 = sum(individual)
    f2 = sin(g) * (1 - sqrt(f1))
    return f1, f2

######################################
# GA Mutations                       #
######################################

def mutModel(individual, indpb):
    sceSize = farm_Num + point_cattle_Num + point_pig_Num + point_sewage_Num
    field_index = farm_Num - 1
    point_cattle_index = point_cattle_Num + field_index
    point_pig_index = point_pig_Num + point_cattle_index
    # point_sewage_index = point_sewage_Num + point_pig_index
    if random.random() < indpb:
        mpoint_num = int(len(individual) / 10)
        if mpoint_num == 0:
            mpoint_num = 1
        for _ in range(mpoint_num):
            mpoint = random.randint(0, sceSize - 1)
            if mpoint <= field_index:
                individual[mpoint] = selectBMPatRandom(bmps_farm)
            elif mpoint <= point_cattle_index:
                individual[mpoint] = selectBMPatRandom(bmps_cattle)
            elif mpoint <= point_pig_index:
                individual[mpoint] = selectBMPatRandom(bmps_pig)
            else:
                for i in range(len(bmps_sewage)):
                    if individual[mpoint] in bmps_sewage[i]:
                        b_s_index = i
                        break
                individual[mpoint] = selectBMPatRandom(bmps_sewage[b_s_index])
    return individual
