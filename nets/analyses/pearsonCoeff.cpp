/* pearsonCoeff.cpp
   2007 May 24                                                        
   Author: Lauri Kovanen


Reads a network from standard input and calculates its Pearson coefficient
for neighbouring degrees. 

To compile:     g++ -O -Wall pearsonCoeff.cpp -o full_analysis

To run:         cat net.edg | ./pearsonCoeff > analysis.txt
                                                 
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
  std::auto_ptr<NetType> netPointer(readNet<float>());
  NetType& net = *netPointer;  // Create a reference for easier handling of net.

  std::cout << pearsonCoeff(net) << '\n';
}
