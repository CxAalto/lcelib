/* Davidsen_step_opt
   2007 May 22                                                               
   Author: Lauri Kovanen

Returns the # of nodes and the mean degree of the largest component of
Davidsen model.
                                            
To compile:     g++ -O -Wall Davidsen_step_opt.cpp -o Davidsen_step_opt

To run:         ./Davidsen_step_opt [params]

Example:        ./Davidsen_step_opt 7000 0.04 38926

[params] = N p randseed

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include <vector>
#include <memory>
#include <iomanip>

// instead of the #includes below, you can alternatively use #include Models.H
//#include "../NetExtras.H" 
#include "../models/Davidsen.H"
#include "../NetExtrasB.H"

//typedef float EdgeData;  
//typedef SymmNet<EdgeData> NetType;
typedef SymmNet<float> NetType;

int main(int argc, char* argv[]) { 
  
  struct DavidsenArgs args;
  readDavidsenArgs(args, argc, argv);
  
  RandNumGen<> generator(args.randseed); 
  NetType net(args.netSize); 

  /* Generate the network and do something with it */
  Davidsen_step(net, args, generator);

  // Analyse only the largest component.
  std::auto_ptr<NetType> netPointer2(findLargestComponent<NetType>(net)); 
  NetType& net2 = *netPointer2;  // Create a reference for easier handling of net.

  double avg_clust = 0;
  for (size_t i = 0; i < net2.size(); i++)
      avg_clust += clusteringCoefficient(net2, i);

  size_t edges = numberOfEdges(net2);
  std::cout << net2.size() << " " << (double)2*edges/net2.size() << " " << avg_clust/net2.size() << " " << pearsonCoeff2(net2) << "\n";
}
