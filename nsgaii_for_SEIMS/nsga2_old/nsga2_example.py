'''
@summary: Implementation of the NSGA-II algorithm in Python.
@version: 1.0
@since: 2011-01-10
@author: Marcelo Pita, http://marcelopita.wordpress.com
@modified: Huiran Gao
@date: 2016-10-12
@contact: marcelo.souza.pita <at> gmail.com
@copyright: Copyright 2011 Marcelo Pita
'''
import math
import random
import time
from config import *
from matplotlib import pyplot as plt
from nsga2 import NSGAII
from solution import Solution
from scenario import *
from readTextInfo import *


class T1Solution(Solution):
    # Solution for the T1 function.
    def __init__(self, bmps_sce):
        # Constructor.
        Solution.__init__(self, num_objs)
        # self.min = 0.
        # self.max = 1.
        self.num_objectives = num_objs
        bmps_sce.getIdfromMongo()
        bmps_sce.create()
        bmps_sce.decoding()
        bmps_sce.importoMongo(HOSTNAME, PORT, BMPScenarioDBName)
        bmps_sce.cost()
        bmps_sce.benefit()
        self.attributes = bmps_sce.attributes
        self.size = len(self.attributes)
        # print self.size
        self.cost_eco = bmps_sce.cost_eco
        self.benefit_env = bmps_sce.benefit_env
        self.evaluate_solution()

    def evaluate_solution(self):
        self.objectives[0] = self.cost_eco
        self.objectives[1] = self.benefit_env
        # print self.attributes

    def crossover(self, other):
        # Crossover of T1 solutions.
        child_solution = T1Solution(Scenario())
        cpoint = random.randint(0, self.size - 1)
        # for i in range(cpoint):
        #     child_solution.attributes[i] = self.attributes[i]
        # for j in range(cpoint, self.size):
        #     child_solution.attributes[j] = other.attributes[j]
        child_solution.attributes = []
        child_solution.attributes.extend(self.attributes[0 : cpoint])
        child_solution.attributes.extend(other.attributes[cpoint : self.size])
        return child_solution

    def mutate(self):
        # Mutation of T1 solution.
        # chromosome section
        field_index = farm_Num - 1
        point_cattle_index = point_cattle_Num + field_index
        point_pig_index = point_pig_Num + point_cattle_index
        # point_sewage_index = point_sewage_Num + point_pig_index

        mpoint = random.randint(0, self.size - 1)
        if mpoint <= field_index:
            self.attributes[mpoint] = selectBMPatRandom(bmps_farm)
        elif mpoint <= point_cattle_index:
            self.attributes[mpoint] = selectBMPatRandom(bmps_cattle)
        elif mpoint <= point_pig_index:
            self.attributes[mpoint] = selectBMPatRandom(bmps_pig)
        else:
            self.attributes[mpoint] = selectBMPatRandom(bmps_sewage)

if __name__ == '__main__':

    pop_size = 2   # size of populations
    # chro_size = 20 # size of chrosome
    num_gens = 1   # number of generations

    muta_rate=0.25  # mutation rate
    crsr_rate=0.65  # crossover rate
    num_objs = 2    # number of objectives

    print "### START TO SCENARIOS OPTIMIZING ###"
    startT = time.clock()
    nsga2 = NSGAII(num_objs, muta_rate, crsr_rate)
    P = []

    for i in range(pop_size):
        S = Scenario()
        P.append(T1Solution(S))

    # Run
    B = nsga2.run(P, pop_size, num_gens)

    endT = time.clock()
    print "Running time: %.2fs" % (endT - startT)
    # Result
    obj1 = []
    obj2 = []
    for i in range(len(B)):
        obj1.append(B[i].objectives[0])
        obj2.append(B[i].objectives[1])
        print B[i].attributes

    # Plot
    plt.title("Scenarios Optimizing\n", color="#aa0903")
    plt.xlabel("cost(Yuan)")
    plt.ylabel("contaminants(t)")
    plt.scatter(obj1, obj2, c="b")
    plt.title("\nPopulation: %d, Generation: %d" % (pop_size, num_gens), color="green", fontsize=9, loc='right')
    plt.savefig("result_Scatter1.png")
    plt.show()