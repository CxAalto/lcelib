
/*  strengths.cpp
   Aug 1 2006                                                                                                   
   Author: Riitta Toivonen


To compile:     g++ -O -Wall strengths.cpp -o strengths

To run:         cat net.edg | ./strengths  > strengths.txt

net.edg is a file where each row contains the values SOURCE DEST EDGEDATA.)


*/



#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include "../../Containers.H"
#include "../../Nets.H"
#include "../NetExtras.H"


// typedef NDEdgeData<float, float> EdgeData;  
typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;



int main(void) {

  /* Read network from stdin.  */
  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());
  NetType& net = *netPointer;  // Create a reference for easier handling of net.

  /* strength of each node and average strength  */
  float strengthSum=0;
  for (size_t i=0; i<net.size(); ++i) { 
    std::cout  << net(i).weight() << "\n";
    strengthSum += net(i).weight();
  }
  std::cerr << "\nAverage strength in the network:  " <<  strengthSum / net.size() << "\n\n";

}

