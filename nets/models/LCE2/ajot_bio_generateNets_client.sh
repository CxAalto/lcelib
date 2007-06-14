#!/bin/bash

thisRunUniqueName=genNet_$1
homeFolder=/proj/networks/Jussi
codeFolder=/proj/networks/Jussi/models

cd $codeFolder; g++ -O socmodel4.cpp -o binFor_$thisRunUniqueName;

echo $1 $2 $3 $4 $5 $6 $7 $8
./binFor_$thisRunUniqueName $2 $3 $4 $5 $6 $7 $8

rm -f binFor_$thisRunUniqueName