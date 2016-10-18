'''
@summary: Implementation of the NSGA-II algorithm in Python.
@version: 1.0
@since: 2011-01-10
@author: Marcelo Pita, http://marcelopita.wordpress.com
@modified: Huiran Gao
@date: 2016-10-12
@contact: marcelo.souza.pita <at> gmail.com
@copyright: Copyright 2011 Marcelo Pita
@license:

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''
import math
import random
import time

from matplotlib import pyplot as plt

from nsga2 import NSGAII
from solution import Solution


class T1Solution(Solution):
    # Solution for the T1 function.
    def __init__(self, chro_size):
        # Constructor.
        Solution.__init__(self, num_objs)
        # self.min = 0.
        # self.max = 1.
        self.num_objectives = num_objs
        self.size = chro_size

        for _ in range(self.size):
            self.attributes.append(random.randint(1, 5))
        self.evaluate_solution()
        
    def evaluate_solution(self):
        # Implementation of method evaluate_solution() for T1 function.
        self.objectives[0] = sum(self.attributes)
        self.objectives[1] = -sum([math.pow(abs(x), 0.5)**3 for x in self.attributes])
        # print self.objectives[0]
        
    def crossover(self, other):
        # Crossover of T1 solutions.
        child_solution = T1Solution(self.size)
        cpoint = random.randint(0, self.size - 1)
        child_solution.attributes.extend(self.attributes[0 : cpoint])
        child_solution.attributes.extend(other.attributes[cpoint : self.size])
        return child_solution

    
    def mutate(self):
        # Mutation of T1 solution.
        mpoint = random.randint(0, self.size - 1)
        self.attributes[mpoint] = random.randint(1, 5)

if __name__ == '__main__':

    pop_size = 50   # size of populations
    chro_size = 20 # size of chrosome
    num_gens = 80   # number of generations

    muta_rate=0.05  # mutation rate
    crsr_rate=0.65  # crossover rate
    num_objs = 2    # number of objectives
    startT = time.clock()

    nsga2 = NSGAII(num_objs, muta_rate, crsr_rate)
    P = []
    for i in range(pop_size):
        P.append(T1Solution(chro_size))
    nsga2.run(P, pop_size, num_gens)

    endT = time.clock()
    print "Running time: %.2fs" % (endT - startT)

    obj1 = []
    obj2 = []
    for i in range(len(P)):
        obj1.append(P[i].objectives[0])
        obj2.append(P[i].objectives[1])

    plt.scatter(obj1, obj2, c="b")
    plt.show()
