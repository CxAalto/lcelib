#!/bin/bash

thisRunUniqueName=genNet_$1
codeFolder=/u/lkovanen/Koodit/C++/lcelib/nets/models/LCE2

#cd $codeFolder; nice -19 g++ socmodel4_opt.cpp -o binFor_$thisRunUniqueName;
cd $codeFolder; g++ -O socmodel4_opt.cpp -o binFor_$thisRunUniqueName;

echo $1 $2 $3 $4 $5 $6 $7 
#nice -19 ./binFor_$thisRunUniqueName $2 $3 $4 $5 $6 $7
./binFor_$thisRunUniqueName $2 $3 $4 $5 $6 $7

rm -f binFor_$thisRunUniqueName