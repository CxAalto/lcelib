/* HolmeKim.cpp
   2005 Dec 30                                                               
   Author: Riitta Toivonen


                                            
To compile:     g++ -O -Wall HolmeKim.cpp -o HolmeKim

To run:         ./HolmeKim [params] > net.edg 2> net.log

Example:        ./HolmeKim 1000 3384 3 0.35 4 clique > net.edg 2> net.log

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include "../../Containers.H"
#include "../../Nets.H"
#include "../../Randgens.H"
#include "../Distributions.H" 

// instead of the #includes below, you can alternatively use #include Models.H
//#include "../NetExtras.H" 
#include "../models/HolmeKim.H" 



//typedef float EdgeData;  
//typedef SymmNet<EdgeData> NetType;
typedef SymmNet<float> /*, ValueTable, ExplSumTreeTable>*/ NetType;


int main(int argc, char* argv[]) { 
  
    struct HolmeKimArgs args;
    readHolmeKimArgs(args, argc, argv);
    outputHolmeKimArgs(args); 


    RandNumGen<> generator(args.randseed); 
    NetType net(args.netSize); 


    /* Generate the network and do something with it */

    HolmeKim(net, args, generator);
    size_t triangles=numberOfTriangles(net);
    std::cerr << "Triangles in the network: " << triangles << "\n";
    outputEdgesAndWeights(net);



}
