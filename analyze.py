# analyze.py
# Ethan Peterson
# Jan 11, 2019
# Take Comma seperated actuator data and run analysis on it.

import subprocess
import sys
import statistics
import json
import csv


infile = open("data.txt", "r")

data = []

def parseLine(string):
    #print(string)
    parsed = string.split(',')
    parsed[2] = parsed[2].strip('\n')
    for i in range(0, len(parsed)):
        parsed[i] = int(parsed[i])
    #print(parsed)
    data.append({
        "steering" : parsed[0],
        "actuator" : parsed[1],
        "intended" : parsed[2]
    })


# Collect data into dictionary
for i, line in enumerate(infile):
    if i > 0: # skip first line in reading process because data here is not useful
        parseLine(line)

# Process Data
total = 0
diffs = []
for i in data:
    currentDiff = abs(i['actuator'] - i['intended'])
    total = total + currentDiff
    diffs.append(currentDiff)


avg = total / len(data)
# Display Results
print("Average Diff: " + str(avg))
print("Standard Dev: " + str(statistics.stdev(diffs)))

# File Output

# Export as csv when the system arg flag is set
if len(sys.argv) > 1 and str(sys.argv[1] == 'csv'):
    csvFile = open('output.csv', 'w')
    with csvFile:
        fields = ['steering', 'actuator', 'intended']

        writer = csv.DictWriter(csvFile, fieldnames=fields)
        writer.writeheader()
        writer.writerows(data)


exit()