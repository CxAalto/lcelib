/* removeDuplicates.cpp
   2007 May 24                                                        
   Author: Lauri Kovanen


Simply reads a network from standard input, saves into the network
data structure and writes it back. The internal network presentation
takes care not to include any duplicate edges.

To compile:     g++ -O -Wall removeDuplicates.cpp -o removeDuplicates.cpp

To run:         cat net.edg | ./removeDuplicates > newnet.edg
                                                 
(net.edg is a file where each row contains the values EDGE TAIL EDGECHARACTERISTIC)
(EDGECHARACTERISTIC for example edge weight)

*/

#define DEBUG  // for debugging code to be run 
//#define NDEBUG // to turn assertions off 


#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include "../../Containers.H"
#include "../../Nets.H"
#include "../NetExtras.H"

typedef SymmNet<float> NetType;

int main(int argc, char* argv[])
{
  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read */
  std::auto_ptr<NetType> netPointer(readNet2<NetType>(1, 0));
  NetType& net = *netPointer;  // Create a reference for easier handling of net.

  outputEdgesAndWeights(net);
}
