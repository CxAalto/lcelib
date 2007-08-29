/* mutualEdges.cpp
   2007 June 7                                                        
   Author: Lauri Kovanen

Reads a directed network from standard input and turns it into an
undirected network so that there is an edge between nodes i and j only
if both edges (i,j) and (j,i) exist in the original network.

To compile:     g++ -O -Wall mutualEdges.cpp -o mutualEdges.cpp

To run:         cat net.edg | ./mutualEdges > net_mutual.edg
                                                 
(net.edg is a file where each row contains the values EDGE TAIL EDGECHARACTERISTIC)
(EDGECHARACTERISTIC for example edge weight)

*/

#define DEBUG  // for debugging code to be run 
//#define NDEBUG // to turn assertions off 

#include "../../Containers.H"
#include "../../Nets.H"
#include "../NetExtras.H"

typedef SymmNet<float> NetType;

int main(int argc, char* argv[])
{
  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read */
  std::auto_ptr<NetType> netPointer(readNet_mutual<NetType>(1, 0));
  NetType& net = *netPointer;  // Create a reference for easier handling of net.

  outputEdgesAndWeights(net);
}
