import os, platform


sysstr = platform.system()
if sysstr == "Windows":
    LF = '\r\n'
elif sysstr == "Linux":
    LF = '\n'

def StripStr(str):
    # @Function: Remove space(' ') and indent('\t') at the begin and end of the string
    oldStr = ''
    newStr = str
    while oldStr != newStr:
        oldStr = newStr
        newStr = oldStr.strip('\t')
        newStr = newStr.strip(' ')
    return newStr


def SplitStr(str, spliters=None):
    # @Function: Split string by spliter space(' ') and indent('\t') as default
    # spliters = [' ', '\t']
    # spliters = []
    # if spliter is not None:
    #     spliters.append(spliter)
    if spliters is None:
        spliters = [' ', '\t']
    destStrs = []
    srcStrs = [str]
    while True:
        oldDestStrs = srcStrs[:]
        for s in spliters:
            for srcS in srcStrs:
                tempStrs = srcS.split(s)
                for tempS in tempStrs:
                    tempS = StripStr(tempS)
                    if tempS != '':
                        destStrs.append(tempS)
            srcStrs = destStrs[:]
            destStrs = []
        if oldDestStrs == srcStrs:
            destStrs = srcStrs[:]
            break
    return destStrs

def isPathExists(path):
    if os.path.isdir(path):
        if os.path.exists(path):
            return True
        else:
            return False
    else:
        return False

def WriteLog(logfile, contentlist, MODE='replace'):
    if os.path.exists(logfile):
        if MODE == 'replace':
            os.remove(logfile)
            logStatus = open(logfile, 'w')
        else:
            logStatus = open(logfile, 'a')
    else:
        logStatus = open(logfile, 'w')
    if isinstance(contentlist, list) or isinstance(contentlist,tuple):
        for content in contentlist:
            logStatus.write("%s%s" % (content, LF))
    else:
        logStatus.write(contentlist)
    logStatus.flush()
    logStatus.close()