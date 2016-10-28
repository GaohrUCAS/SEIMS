
import random
import numpy

a = [5,1,2,3,2,4,0]
b = [5,6,7]
a.extend(b)
print a
c = list(set(a))
print c
