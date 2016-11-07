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

import sys


class Solution:
    # Abstract solution. To be implemented.

    def __init__(self, num_objectives):
        # Constructor. Parameters: number of objectives.
        self.num_objectives = num_objectives
        self.objectives = []
        for _ in range(num_objectives):
            self.objectives.append(None)
        self.attributes = []
        self.rank = sys.maxint
        self.distance = 0.

    def evaluate_solution(self):
        # Evaluate solution, update objectives values.
        raise NotImplementedError("Solution class have to be implemented.")

    def crossover(self, other):
        # Crossover operator.
        raise NotImplementedError("Solution class have to be implemented.")

    def mutate(self):
        # Mutation operator.
        raise NotImplementedError("Solution class have to be implemented.")

    def __rshift__(self, other):
        # True if this solution dominates the other (">>" operator).
        dominates = False

        for i in range(len(self.objectives)):
            if self.objectives[i] > other.objectives[i]:
                return False

            elif self.objectives[i] < other.objectives[i]:
                dominates = True

        return dominates

    def __lshift__(self, other):
        # True if this solution is dominated by the other ("<<" operator).
        return other >> self

def crowded_comparison(s1, s2):
    # Compare the two solutions based on crowded comparison.
    if s1.rank < s2.rank:
        return 1

    elif s1.rank > s2.rank:
        return -1

    elif s1.distance > s2.distance:
        return 1

    elif s1.distance < s2.distance:
        return -1

    else:
        return 0
