
import random
import numpy
from subprocess import *
from config import *
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

client = MongoClient(HOSTNAME, PORT)
db = client[BMPScenarioDBName]
collection = db.BMP_SCENARIOS
# for i in range(7, 15):
collection.remove({'ID': '8', 'BMPID':'1'})
