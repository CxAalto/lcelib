/* ~/c++/lcelib/nets/analyses/pathLengths.cpp
   2006 Nov 24
   Author: Riitta Toivonen


                                            
To compile:     g++ -O -Wall pathLengths.cpp -o pathLengths

Example:        cat net.edg | ./pathLengths  0.1 (optional: 132351235) > averagePathLengthAndDiameter.txt

(params:       fraction of nodes to start finding shortest paths from as an argument, random seed (integer)

   Tested average path length and longest shortest paths: ...

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
#include "../Dijkstrator.H" 
#include "../NetExtras.H" 
#include "../models/SimpleNets.H" 
#include "../models/CommunityNet.H" 




typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;
  
  /* function shortestPaths(net, Nstarts, generator)
     Starting from  Nstarts randomly chosen nodes, find shortest paths.
     Output the longest found path and the average over all found shortest path 
     lengths to standard output */
  template<typename NetType, typename Generator>
  void shortestPaths(NetType &net, float fraction, Generator &generator) {
    
    float maxlength=0; 
    double sumlengths=0; 
    long long Ndistances=0; 
    size_t NStartNodes=(size_t) ceil(fraction* ((float) net.size() ) );
    
    /* * * * shuffle node indices from 0 to N-1, then take NStartNodes
       indices from the beginning. This way, we are able to pick
       NStartNodes unique indices. The operation is O(N) (linear
       with N). */
    std::vector<size_t> order;  for (size_t i=0; i<net.size(); ++i) { order.push_back(i); };  
    shuffle(order,generator);   
    
    for (size_t m=0; m<NStartNodes; ++m) {
      size_t startingPoint=order[m]; 
      std::cerr << "\r\rStarting to find shortest paths from node id " << startingPoint << "...\n";
      Dijkstrator<NetType> paths(net,startingPoint);
      for (; !paths.finished(); ++paths) {
	sumlengths += (*paths).getWeight();
	Ndistances++;
	if (maxlength <  (*paths).getWeight() ) 
	  maxlength =  (*paths).getWeight();	
      } 
#ifdef DEBUG
      std::cerr << "startingPoint: \t" << startingPoint << "\n";
      std::cerr << "sumlengths: \t" << sumlengths << "\n";
      std::cerr << "Ndistances: \t" << Ndistances << "\n";
      std::cerr << "netSize: \t" << net.size() << "\n";
#endif //~ DEBUG      
    } // end of loop for choosing Nstarts starting nodes
    // Calculate and output longest shortest path and average shortest path length for this run      
    std::cerr << "Outputting: average path length \t maximum path length.\n";
    std::cout <<  sumlengths/Ndistances << "\t" << maxlength << "\n";
  }






typedef SymmNet<float> /*, ValueTable, ExplSumTreeTable>*/ NetType;

int main(int argc, char* argv[]) { 
  

  // read in arguments
  
  size_t bookmark=1;

  if ( (size_t) argc < (bookmark+1)) { std::cerr << "Please give arguments: the fraction of nodes to start finding shortest paths from, and optionally a random seed (integer).\n"; exit(1);}
  float fraction=atof(argv[bookmark]);  bookmark++;
  size_t randseed;
  if ( (size_t) argc < (bookmark+1)) { randseed = time(0);} // some random number 
  else { randseed = atoi(argv[bookmark]);  bookmark++;}
  
  
  RandNumGen<> generator(randseed);   
  
  
  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read */
  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());
  NetType& net = *netPointer;  // Create a reference for easier handling of net.
   
  shortestPaths(net,fraction,generator);     /* Find all shortest paths starting from NStartNodes randomly chosen nodes: */
  
}

