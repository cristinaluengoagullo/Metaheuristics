#!/usr/bin/python

import random

instanceOPL = open("testOPL.dat", 'w')
instanceMH = open("testMH.dat", 'w')

nOffices = 80;
nCenters = 160;
nSegments = 4;

line = "nOffices = " + str(nOffices) + ";\n";
instanceOPL.write(line);
line = "no " + str(nOffices) + "\n";
instanceMH.write(line);
line = "nCenters = " + str(nCenters) + ";\n";
instanceOPL.write(line);
line = "nc " + str(nCenters) + " .\n";
instanceMH.write(line);
line = "nSegments = " + str(nSegments) + ";\n";
instanceOPL.write(line);

maxCapacity = 0;
k = "";
for n in range(0,nCenters):
    r = random.randint(1,50);
    if r > maxCapacity:
        maxCapacity = r;
    k += str(r) + " ";
instanceOPL.write("k = [" + k + "];\n");
instanceMH.write("k " + k + ". \n");

r = random.randint(1,maxCapacity/2);
d = str(r) + " ";
minDemmand = d;
for n in range(1,nOffices):
    r = random.randint(1,maxCapacity/2);
    d += str(r) + " ";
    if(r < minDemmand):
        minDemmand = r;
instanceOPL.write("d = [" + d + "];\n");
instanceMH.write("d " + d + ". \n");

f = "";
for n in range(0,nCenters):
    r = random.randint(1,20);
    f += str(r) + " ";
instanceOPL.write("f = [" + f + "];\n");
instanceMH.write("f " + f + ". \n");

s = "";
n = 5*nSegments;
while(n > 0):
    s += str(n) + " ";
    n -= 5;
instanceOPL.write("s = [" + s + "];\n");
instanceMH.write("s " + s + ".\n");

m = "0 ";
for n in range(1,nSegments):
    m += str((maxCapacity/nSegments)*n) + " ";
instanceOPL.write("m = [" + m + "];\n");
instanceMH.write("m " + m + ".\n");

uOPL = "u = [";
uMH = "u ";
subu = [[0 for x in range(0,nCenters)] for y in range(0,nOffices)];
for n in range(0,nOffices) :
    indexs = range(0,nCenters);
    i = nCenters;
    while (i > nCenters/2):
        r = random.randint(0,len(indexs)-1);
        subu[n][indexs[r]] = 1;
        indexs.pop(r);
        i -= 1;

for n in range(0,nCenters):
    uOPL += "[";
    for m in range(0,nOffices):
        r = subu[m][n];
        uOPL += str(r) + " ";
        uMH += str(r) + " ";
    uOPL += "]";
    if (n != nCenters-1) :
        uMH += ", ";
uOPL += "];\n";
uMH += ". ";
instanceOPL.write(uOPL);
instanceMH.write(uMH);

instanceOPL.close();
 

