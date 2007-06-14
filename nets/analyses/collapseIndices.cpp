/* collapseIndices.cpp
   2006 Jun 14                                             
   Author: Lauri Kovanen

Reads a network from standard input and re-indexes it so that the
index numbers run from 0 to N, where N is the number of nodes in the
network.
                                            
To compile:     g++ -O -Wall collapseIndices.cpp -o collapseIndices

To run:         cat net.edg | ./collapseIndices > net_collapsed.edg
                                                 
(net.edg is a file where each row contains the values EDGE TAIL EDGECHARACTERISTIC
(EDGECHARACTERISTIC for example edge weight)

*/




//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off 

#include "../../Containers.H"
#include "../../Nets.H"
#include "../NetExtras.H"



typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;

int main(int argc, char* argv[]) {

  std::auto_ptr<NetType> netPointer(readNet2<NetType>(1,0)); 
  NetType& net = *netPointer;  // Create a reference for easier handling of net.

  std::auto_ptr<NetType> netPointer2(collapseIndices(net)); 
  NetType& net2 = *netPointer2;  // Create a reference for easier handling of net.

  outputEdgesAndWeights(net2);
}
