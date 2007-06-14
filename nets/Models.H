// Models.H
/* A file that includes all header files related to network models. */


#ifndef MODELS_H
#define MODELS_H

#include "NetExtras.H"   
#include "Distributions.H" 
#include "models/SimpleNets.H" 
#include "models/SeedNet.H" 
#include "models/HolmeKim.H" 
#include "models/ErdosRenyi.H" 
#include "models/CommunityNet.H"  // old version currently in lcelib/nets/OldModels.H
#include "models/BBV.H" 
// #include "models/BA.H" // perhaps Jussi will write this one
#include "models/Davidsen.H" 

#endif //~ MODELS_H



/* 
 The network model functions take in a reference to a network 
 of fixed size, whose edges they modify. The type of seed network 
 used for growing network models is specified with the enumerated 
 variables RANDOM (referring to an Erdös-Renyi random network) 
 CLIQUE (fully connected network), CHAIN or RING. 

 Each function also takes in a reference to a random number generator 
 of type RandNumGen (from lcelib/Randgens.H). 

 (This information may or may not change in the future as more
 network models  are included and more features added. It held 
 true at Aug 30 2005.) 
*/
