# analyze.py
# Ethan Peterson
# Jan 11, 2019
# Take Comma seperated actuator data and run analysis on it.

import subprocess

infile = open("data.txt", "r")
testString = infile.readline()

data = []

for i, line in enumerate(infile):
    if i > 0: # skip first line in reading process because data here is not useful
        #print(line)
        break
        pass
    pass

#parseLine(testString)

def parseLine(string):
    #print(string)
    parsed = string.split(',')
    parsed[2] = parsed[2].strip('\n')
    for i in range(0, len(parsed)):
        parsed[i] = int(parsed[i])
    print(parsed)
    data.append({
        "steering" : parsed[0],
        "actuator" : parsed[1],
        "intended" : parsed[2]
    })
parseLine(testString)
print(data)
exit()