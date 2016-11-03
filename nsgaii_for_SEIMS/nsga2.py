'''
@summary: Implementation of the NSGA-II algorithm in Python.
@version: 1.0
@since: 2011-01-07
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

import random
import sys

from solution import *


class NSGAII:
    # Implementation of NSGA-II algorithm.
    current_evaluated_objective = 0

    def __init__(self, num_objectives, mutation_rate=0.05, crossover_rate=0.65):
        '''
        Constructor.
        @num_objectives: number of objectives,
        @mutation_rate: mutation rate (default value 5%) and
        @crossover_rate: crossover rate (default value 65%).
        '''
        self.num_objectives = num_objectives
        self.mutation_rate = mutation_rate
        self.crossover_rate = crossover_rate
        random.seed();
        
    def run(self, P, population_size, num_generations):
        '''
        Run NSGA-II.
        @P: chromosomes
        @population_size: number of chromosome
        @num_generations: iteration number of genetic
        '''
        for s in P:
            s.evaluate_solution()
        Q = []
        for i in range(num_generations):
            print "###### Iteration: %d ######" % i
            R = []
            R.extend(P)
            R.extend(Q)
            fronts = self.fast_nondominated_sort(R)
            # print len(fronts[1])
            del P[:]
            for front in fronts.values():
                if len(front) == 0:
                    break
                self.crowding_distance_assignment(front);
                P.extend(front)
                if len(P) >= population_size:
                    break
            
            self.sort_crowding(P)
            if len(P) > population_size:
                del P[population_size:]
            Q = self.make_new_pop(P)

        # for f in fronts.values():
        #     for info in f:
        #         print info.__dict__['attributes']
        # print fronts.values()[0][0].__dict__
        # for ss in range(len(Q)):
        #     print Q[ss].__dict__['attributes']

        return P

    def sort_ranking(self, P):
        for i in range(len(P) - 1, -1, -1):
            for j in range(1, i + 1):
                s1 = P[j - 1]
                s2 = P[j]
                if s1.rank > s2.rank:
                    P[j - 1] = s2
                    P[j] = s1
                    
    def sort_objective(self, P, obj_idx):
        for i in range(len(P) - 1, -1, -1):
            for j in range(1, i + 1):
                s1 = P[j - 1]
                s2 = P[j]
                if s1.objectives[obj_idx] > s2.objectives[obj_idx]:
                    P[j - 1] = s2
                    P[j] = s1
                    
    def sort_crowding(self, P):
        for i in range(len(P) - 1, -1, -1):
            for j in range(1, i + 1):
                s1 = P[j - 1]
                s2 = P[j]
                if crowded_comparison(s1, s2) < 0:
                    P[j - 1] = s2
                    P[j] = s1
                
    def make_new_pop(self, P):
        # Make new population Q, offspring of P.
        Q = []
        while len(Q) != len(P):
            selected_solutions = [None, None]
            while selected_solutions[0] == selected_solutions[1]:
                for i in range(2):
                    s1 = random.choice(P)
                    s2 = s1
                    while s1 == s2:
                        s2 = random.choice(P)
                    if crowded_comparison(s1, s2) > 0:
                        selected_solutions[i] = s1
                    else:
                        selected_solutions[i] = s2
            
            if random.random() < self.crossover_rate:
                child_solution = selected_solutions[0].crossover(selected_solutions[1])
                if random.random() < self.mutation_rate:
                    child_solution.mutate()
                child_solution.evaluate_solution()
                Q.append(child_solution)
        return Q
        
    def fast_nondominated_sort(self, P):
        # Discover Pareto fronts in P, based on non-domination criterion.
        fronts = {}
        S = {}
        n = {}
        for s in P:
            S[s] = []
            n[s] = 0
        fronts[1] = []
        
        for p in P:
            for q in P:
                if p == q:
                    continue
                if p >> q:
                    S[p].append(q)
                elif p << q:
                    n[p] += 1
            if n[p] == 0:
                fronts[1].append(p)
        i = 1

        while len(fronts[i]) != 0:
            next_front = []
            for r in fronts[i]:
                for s in S[r]:
                    n[s] -= 1
                    if n[s] == 0:
                        next_front.append(s)
            i += 1
            fronts[i] = next_front
        return fronts
        
    def crowding_distance_assignment(self, front):
        # Assign a crowding distance for each solution in the front.
        for p in front:
            p.distance = 0
        
        for obj_index in range(self.num_objectives):
            self.sort_objective(front, obj_index)
            front[0].distance = float('inf')
            front[len(front) - 1].distance = float('inf')
            for i in range(1, len(front) - 1):
                front[i].distance += (front[i + 1].distance - front[i - 1].distance)
