/* ~/c++/lcelib/nets/analyses/pathLengths.cpp
   2006 Nov 24
   Author: Riitta Toivonen


                                            
To compile:     g++ -O -Wall pathLengths.cpp -o pathLengths

Example:        cat net.edg | ./pathLengths  0.1 132351235 > averagePathLengthAndDiameter.txt

(params:       fraction of nodes to start finding shortest paths from as an argument, random seed (integer)


Note: distributions for number of initial and secondary connections are given in cumulative form: 
  Nrandmax [cumulative probabilities of Nrand] 
  Nwalksmax [cumulative probabilities of Nwalks].


Note that zero initial contacts are not allowed. Therefore, 
Nrandmax [cumulative probabilities of Nrand]
2         0.95       1

means 
  p(nrand=1) = 0.95,  
  p(nrand=2) = 0.05.


For the number of secondary connections, zero is allowed.  As an example, the discrete distribution U[0,3] is given as
   3  0.25  0.5  0.75  1



   Tested average path length and longest shortest paths:
   For a 3-clique: OK
    Average shortest path length:   1
    Longest shortest path found:    1
   For a 4-ring (from each point, 2 nodes at distance 1 and 1 at distance 2):  OK
    Average shortest path length:   4/3 = 1.33333
    Longest shortest path found:    2
    For a 5-ring (5 paths of length 1, 5 of length 2, that s all) OK
  Average shortest path length:   1.5 
  Longest shortest path found:    2 

*/  
/*    ~/c++/lcelib/nets/Examples/findDiameter.cpp

 Starting from a given fraction of nodes, the program finds shortest
 paths to all other nodes in the network which it reads in.
 It aoutputs the average shortest path length  and the maximum 
 path length it found.

 Note: The program does not  avoid the possibility of finding shortest 
       paths from the same node more than once.
       In a large network, this is not a problem because it is not very 
       likely that the same node is chosen twice. 

 
 Input:   [source] [dest] [payload]

 Output:  max path length 

 Example: cat net.edg | ./findDiameter > tmpDiameter.txt

 24.11.2006 / Riitta Toivonen: Original version

To compile: g++ -Wall findDiameter.cpp -o findDiameter

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

// instead of the #includes below, you can alternatively use #include Models.H
//#include ../Models.H
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
  void shortestPaths(NetType &net, size_t Nstarts, Generator &generator) {
    float maxlength=0; 
    double sumlengths=0; 
    long long Ndistances=0; 
    for (size_t m=0; m<Nstarts; ++m) {
      size_t startingPoint=generator.next(net.size()); /* choose random node
        Note: not avoiding the possibility of starting again from the same node...
        This could be fixed. In a large network, it is not a problem though
        because it is not very likely that the same node is chosen twice. */
      std::cerr << "Starting to find shortest paths from node id " << startingPoint << "...\n";
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

  if ( (size_t) argc < (bookmark+1)) { std::cerr << "Please give arguments: the fraction of nodes to start finding shortest paths from, and a random seed (integer).\n"; exit(1);}
  float fraction=atof(argv[bookmark]);  bookmark++;
    
  if ( (size_t) argc < (bookmark+1)) { std::cerr << "Please give arguments: the fraction of nodes to start finding shortest paths from, and a random seed (integer).\n"; exit(1);}
  size_t randseed=atoi(argv[bookmark]);  bookmark++;
 
  
  RandNumGen<> generator(randseed);   
  
  
  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read */
  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());
  NetType& net = *netPointer;  // Create a reference for easier handling of net.
   
  
  std::cout <<  "Output: average shortest path length and the longest found shortest path.\n"; 
  size_t NStartNodes=(size_t) ceil(fraction* ((float) net.size() ) );

  shortestPaths(net,NStartNodes,generator);     /* Find all shortest paths starting from 100 randomly chosen nodes: */
  
}

