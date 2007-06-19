/* clique.cpp
   2006 Aug 1
   Author: Riitta Toivonen


                                            
To compile:     g++ -O -Wall clique.cpp -o clique

To run:         ./clique netSize  > net.edg


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
// network models, including SimpleNets.H, included by Models.H
#include "../Models.H" 

// typedef NDEdgeData<float, float> EdgeData;  
typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;


int main(int argc, char* argv[]) { 
  
  //if ( (size_t) argc < (2+1)) { std::cerr << "Please give input arguments: network size N, random seed (integer).\n"; exit(1);}
if ( (size_t) argc < (1+1)) { std::cerr << "Please give input argument: network size N\n"; exit(1);}  

  size_t netSize=atoi(argv[1]);
  NetType net(netSize); 
  
  Clique(net);  //generate the net
  // ...and output the edges and weights
  outputEdgesAndWeights(net);  

}
