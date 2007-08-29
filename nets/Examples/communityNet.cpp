/* communityNet.cpp
   2006 Jan 10
   Author: Riitta Toivonen


                                            
To compile:     g++ -O -Wall communityNet.cpp -o communityNet

To run:         ./communityNet [params] > net.edg 2> net.log

Example:        ./communityNet    1000 2349  2 0.95 1   3 0.25 0.5 0.75 1  0 1 10 random 2 > net.edg 2> net.log

(params: N randseed Nrandmax [cumulative probabilities of Nrand]  Nwalksmax [cumulative probabilities of Nwalks]   delta w0 seedSize seedType optional:k_ave


Note: distributions for number of initial and secondary connections are given in cumulative form: 
  Nrandmax [cumulative probabilities of Nrand] 
  Nwalksmax [cumulative probabilities of Nwalks].


Note that zero initial contacts are not allowed. Therefore, 
Nrandmax [cumulative probabilities of Nrand]
2         0.95       1

means 
  p(nrand=1) = 0.95,  
  p(nrand=2) = 0.05.


For the number of secondary connections, zero is allowed.  As an example, the discrete distribution U[0,3] is given as
   3  0.25  0.5  0.75  1

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
#include "../NetExtras.H" 
#include "../models/CommunityNet.H" 


typedef SymmNet<float> /*, ValueTable, ExplSumTreeTable>*/ NetType;


int main(int argc, char* argv[]) { 
  
    struct CommNetArgs args;
    readCommNetArgs(args, argc, argv);
    outputCommNetArgs(args);  // Prints parameters to std::cerr verbosely
    //outputCommNetArguments2(argc, argv); // Prints command line to std::cout

    RandNumGen<> generator(args.randseed); 
    NetType net(args.netSize); 

    communityNetCustomCdfsFaster(net, args, generator);
    outputEdgesAndWeights(net);
}
