/* largestComponent.cpp
   2006 Jun 14                                             
   Author: Lauri Kovanen


Reads a network from standard input and writes out largest
component. The nodes will be re-indexed from 0 to N, where N is the
size of the largest component.
                                            
To compile:     g++ -O -Wall largestComponent.cpp -o largestComponent

To run:         cat net.edg | ./largestComponent > net_largest.edg
                                                 
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

  std::auto_ptr<NetType> netPointer2(findLargestComponent<NetType>(net)); 
  NetType& net2 = *netPointer2;  // Create a reference for easier handling of net.

  outputEdgesAndWeights(net2);
}
