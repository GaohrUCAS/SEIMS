# -*- coding: utf-8 -*-

import os, sys
from pymongo import MongoClient
from pymongo.errors import ConnectionFailure
import numpy, datetime
import os

a = "2013-10-10 23:40:00"
# 方法:先转换为时间数组,然后转换为其他格式
timeArray = datetime.datetime.strptime(a, "%Y-%m-%d %H:%M:%S")
otherStyleTime = datetime.date.strftime(timeArray, "%Y/%m/%d")

print otherStyleTime