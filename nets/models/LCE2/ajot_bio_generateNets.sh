#!/bin/bash

rounds=25000
netCounterOffset=0
netsize=$1             # verkon koko
offset=$2              # verkkojen numerointi alkaa t‰st‰ luvusta
netrounds=$3           # kuinka monta verkkoa tehd‰‰n
commonpart=$4          # nimi verkoille (jokainen tiedosto alkaa t‰ll‰ nimell‰), tiedostot menee hakemistoon testiverkkoja (ks. alla)

deltas=( 0 0.1 0.2 0.3 0.4 0.5 0.001 0.005 0.01 0.05 0.003 0.007 0.02 0.03 0.07 0.15 0.8 1 )
# alkuper‰inen! ptris2=( 0.0106 0.026 0.032 0.035 0.038 0.0395 0.0109 0.0121 0.0135 0.0205 0.0115 0.0127 0.016 0.018 0.0235 0.029 0.042 0.044 ) # ave_k = 10
ptris2=( 0.0106 0.026 0.032 0.035 0.038 0.0395 0.0109 0.0121 0.0135 0.0205 0.0115 0.0127 0.016 0.018 0.0235 0.029 0.042 0.044 ) # ave_k = 10

takeTheseCases=( 0 1 3 5 6 8 9 13 15 17 )




netCounter=$netCounterOffset
while [ $netCounter -lt ${#takeTheseCases[*]} ]; do
	netID=${takeTheseCases[$netCounter]} 
    p_tri=${ptris2[$netID]}
    delta=${deltas[$netID]}
    name="_""$netID""net"
    
    nimi2="testiverkkoja/""$commonpart""$name""_""$netsize""_"
      
    qsub -l cluster=TRUE -l mmxext -l 3dnow -cwd -N genNet_$netsize"_"$netID"_"$offset ajot_bio_generateNets_client.sh $netsize"_"$netID"_"$offset $netsize $rounds $p_tri $delta $netrounds $nimi2 $offset

    let netCounter=netCounter+1
done


