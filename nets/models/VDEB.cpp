
// VDEB.cpp
/* 
A growing variant of the Davidsen, Ebel, Bornholdt 2002 model for social networks
(Davidsen, J., Ebel, H., and Bornholdt, S., 2002.
Emergence of a Small World from Local Interactions: Modeling
Acquaintance Networks. Phys. Rev. Lett. 88 (12) 128701-1.)

proposed by A. Vazquez, 2003 
(Growing networks with local rules: Preferential attachment, clustering
hierarchy, and degree correlations. Phys. Rev. E 67, 056104)

In this variant, at each time step 
  1) with prob. u a link is formed between two non-neighboring nodes 
     which share at least one common neighbor (in Vazquez's words,  
     a "potential edge" is converted into an edge)
  2) with prob. 1-u, a new node is added to the network, joining it to a random node.

  I keep track of potential edges by storing them in a set (Jorkki's hash set).
  Whenever a new edge is added to the network, I add any new potential edges to this set.
  When a potential edge is converted to an edge, I remove it from the list. 

*/

/* 

To compile:
 g++ -O VDEB.cpp -o VDEB

To run:
 ./VDEB > net.edg 

 The parameters are: 
    N - network size
    u - probability  of creating a link where there is a "potential edge"

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

  std::string infoText="\n\nA growing variant of the Davidsen, Ebel, Bornholdt 2002 model for social networks\n(Davidsen, J., Ebel, H., and Bornholdt, S., 2002.\nEmergence of a Small World from Local Interactions: Modeling\nAcquaintance Networks. Phys. Rev. Lett. 88 (12) 128701-1.)\n\nproposed by A. Vazquez, 2003 \n(Growing networks with local rules: Preferential attachment, clustering\nhierarchy, and degree correlations. Phys. Rev. E 67, 056104)\n\nIn this model, beginning with a single node and no edges, at each time step \n  1) with prob. u a link is formed between two non-neighboring nodes \n     which share at least one common neighbor (in Vazquez's words,  \n     a 'potential edge' is converted into an edge)\n  2) with prob. 1-u, a new node is added to the network, joining it to a random node.\n\n  I keep track of potential edges by storing them in a set (Jorkki's hash set).\n  Whenever a new edge is added to the network, I add any new potential edges to this set.\n  When a potential edge is converted to an edge, it is removed from the set. \n\n";


  if ( (size_t) argc < (2+1)) { std::cerr << infoText << "\nPlease give arguments: N and  u.\n\n\n\n"; exit(1);}
  const size_t N = atoi(argv[1]);
  const float u = atof(argv[2]);


  NetType net(N);

  size_t randseed=time(0); // some random number 
  RandNumGen<> generator(randseed);

  VDEB(net, u, generator);
  outputEdgesAndWeights(net);
} 
