import os, sys
from pymongo import MongoClient
from pymongo.errors import ConnectionFailure
import glob
import os

def main():
    """ Connect to MongoDB """
    try:
        c = MongoClient(host="localhost", port=27017)
        print c
    except ConnectionFailure, e:
        sys.stderr.write("Could not connect to MongoDB: %s" % e)
        sys.exit(1)

if __name__ == "__main__":
    moudles_dir = r''
    main()