
/* degClustNndeg.cpp 
   Aug 1 2006                                                                                                   
   Author: Riitta Toivonen


To compile:     g++ -O -Wall degClustNndeg.cpp -o degClustNndeg

To run:         cat net.edg | degClustNndeg./  > degClustNndeg.txt

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
#include "../../Randgens.H"
#include "../NetExtras.H"
#include "../Distributions.H"


typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;



int main(void) {

  /* Read network from stdin. */
  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());
  NetType& net = *netPointer;  // Create a reference for easier handling of net.

  
  /* Average degree, max degree  */
  size_t kmax = 0;
  size_t degreesum=0;
  for (size_t i=0; i<net.size(); ++i) { 
    degreesum += net(i).size();
    if (net(i).size() > kmax) {kmax = net(i).size();}
  }
  std::cerr << "\nAverage degree in the network:  " <<  ((float) degreesum) / net.size() << "\n\n";
  

  outputDistributions2(net);
  std::cerr << "\n\nPrinted out for each node i: \n";
  std::cerr << "column 1: degree k(i)\n";
  std::cerr << "column 2: clustering c(i) = (# triangles around i) / ( k*(k-1)/2 )  \n";
  std::cerr << "column 3: nearest neighbor degree knn(i)  k\n";
  std::cerr << "\n Each column will have N elements, where N is the number of nodes in the network.\n\n";
}

