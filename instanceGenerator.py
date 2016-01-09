#!/usr/bin/python

import random

instanceOPL = open("testOPL.dat", 'w')
instanceMH = open("testMH.dat", 'w')

nOffices = 8;
nCenters = 16;
nSegments = 1;

line = "nOffices = " + str(nOffices) + ";\n";
instanceOPL.write(line);
line = "no " + str(nOffices) + "\n";
instanceMH.write(line);
line = "nCenters = " + str(nCenters) + ";\n";
instanceOPL.write(line);
line = "nc " + str(nCenters) + "\n";
instanceMH.write(line);
line = "nSegments = " + str(nSegments) + ";\n";
instanceOPL.write(line);

maxCapacity = 0;
k = "";
for n in range(1,nCenters+1):
    r = random.randint(1,10);
    if r > maxCapacity:
        maxCapacity = r;
    k += str(r) + " ";
instanceOPL.write("k = [" + k + "];\n");
instanceMH.write("k " + k + ". \n");

maxDemmand = 0;
d = "";
for n in range(1,nOffices+1):
    r = random.randint(1,maxCapacity/2);
    if r > maxCapacity:
        maxCapacity = r;
    d += str(r) + " ";
instanceOPL.write("d = [" + d + "];\n");
instanceMH.write("d " + d + ". \n");

f = "";
for n in range(1,nCenters+1):
    r = random.randint(1,20);
    f += str(r) + " ";
instanceOPL.write("f = [" + f + "];\n");
instanceMH.write("f " + f + ". \n");

s = "";
for n in range(1,nSegments+1):
    r = random.randint(1,20);
    s += str(r) + " ";
instanceOPL.write("s = [" + s + "];\n");
instanceMH.write("s " + s + ".\n");

i = maxDemmand/nSegments;
j = 1;
m = "";
for n in range(1,nSegments+1):
    m += str(i*j) + " ";
    j += 1;
instanceOPL.write("m = [" + m + "];\n");
instanceMH.write("m " + m + ".\n");

uOPL = "u = [";
uMH = "u ";
for n in range(1,nCenters+1):
    uOPL += "[";
    for m in range(1,nOffices+1):
        r = random.randint(0,1);
        uOPL += str(r) + " ";
        uMH += str(r) + " ";
    uOPL += "]";
    if (n != nCenters) :
        uMH += ", ";
uOPL += "];\n";
uMH += ". ";
instanceOPL.write(uOPL);
instanceMH.write(uMH);

instanceOPL.close();
 

