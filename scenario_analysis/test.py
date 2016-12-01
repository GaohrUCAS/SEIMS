
from pymongo import MongoClient

# cmdStr = "%s %s %d %d %s %d %d" % (model_Exe, model_Workdir, threadsNum, layeringMethod, HOSTNAME, PORT, 1)
# process = Popen(cmdStr, shell=True, stdout=PIPE)
# while(process.stdout.readline() != ""):
#     # line = process.stdout.readline().split("\n")
#     # if(line[0] != ""):
#     #     print line[0]
#     continue
# process.wait()
# if process.returncode == 0:
#     print "OK"

# client = MongoClient('192.168.6.55', 27017)
# db = client['BMP_Scenario_dianbu2_30m_longterm']
# collection = db.BMP_SCENARIOS
# for i in range(7, 500):
#     collection.remove({'ID': i})

import numpy as np
import matplotlib.pyplot as plt


N = 50
x = np.random.rand(N)
y = np.random.rand(N)
colors = np.random.rand(N)
area = np.pi * (15 * np.random.rand(N))**2  # 0 to 15 point radiuses

plt.scatter(x, y, s=12, c='r', alpha=0.8)
plt.show()