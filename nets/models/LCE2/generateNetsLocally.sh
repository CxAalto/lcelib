

thisRunFolder=$1
mkdir $thisRunFolder
cd $thisRunFolder
#ln -s  ~/c++/lcelib/nets/models/LCE2/socmodel4.cpp ./tmpCode.cpp
#g++ tmpCode.cpp -o tmpCode  
ln -s  ~/c++/lcelib/nets/models/LCE2/socmodel4 ./tmpCode

#netsize=50000; rounds=250000; 
#p_tri=0.026; delta=0.1; 
#netrounds=20; # this many networks 
#nimi2=net.; 
#offset=1; 
netsize=$2
rounds=$3
p_tri=$4
delta=$5
netrounds=$6
nimi=$7
offset=$8
./tmpCode   $netsize $rounds $p_tri $delta $netrounds $nimi $offset

rm tmpCode
