
// VDEB.cpp
/* A growing variant of the Davidsen, Ebel, Bornholdt 2002 model for
social networks, proposed by A. Vazquez 2003 (Growing networks with
local rules). Here, at each time step with prob. u a link is formed
between two non-neighboring nodes which share at least one common
neighbor. With prob. 1-u, a new node is added to the network, joining
it to a random node.
*/


/*

To compile:
 g++ -O VDEB.cpp -o VDEB

To run:
 ./VDEB > net.edg 

 The parameters are: 
    network size
    probability u of creating a link where there is a possible link

*/

//  Authors: Riitta Toivonen
//  Aug 21 2007


#define NDEBUG // to turn assertions off
#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>

#include "../NetExtras.H"
#include "../../Containers.H"
#include "VDEB.H"


//typedef SymmNet<size_t> NetType;
typedef float EdgeData;
typedef  SymmNet<EdgeData, ValueTable, ExplSumTreeTable> NetType;

int main(int argc, char* argv[]) {


  if ( (size_t) argc < (2+1)) { std::cerr << "Please give arguments: N and  u.\n"; exit(1);}
  const size_t N = atoi(argv[1]);
  const float u = atof(argv[2]);


  NetType net(N);

  size_t randseed=time(0); // some random number 
  RandNumGen<> generator(randseed);

  VDEB(net, u, generator);
  outputEdgesAndWeights(net);
}
