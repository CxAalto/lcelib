#!/bin/bash

rounds=20000 #25000
netsize=$1             # verkon koko
offset=$2              # verkkojen numerointi alkaa t‰st‰ luvusta
netrounds=$3           # kuinka monta verkkoa tehd‰‰n
p_tri=$4
delta=$5
p_jump=$6

# find available machines:
list=$(cat ~/Klusteri/AvailableMachinesForRunningLcelibWithSGE_updated1March2007.txt)

# join them with commas
list2=$( for f in $list; do echo -ne $f\,; done)

qsub -l cluster=TRUE -l mmxext -l 3dnow -cwd -N genNet_$netsize"_"$offset ajot_optimize_client.sh $netsize"_"$offset $netsize $rounds $p_tri $delta $netrounds $p_jump
#qsub -q $list2 -cwd -N genNet_$netsize"_"$offset ajot_optimize_client.sh $netsize"_"$offset $netsize $rounds $p_tri $delta $netrounds

