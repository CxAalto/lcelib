/* Wong_opt
   2007 May 31                                                               
   Author: Lauri Kovanen

Wong model generator for optimization of clustering coefficient.
Generates a Wong model according to input parameters and outputs only
the clustering coefficient of the net and the Pearson correlation coefficient.
                                            
To compile:     g++ -O -Wall Wong_opt.cpp -o Wong_opt

To run:         ./Wong_opt [params]

Example:        ./Wong_opt 1000 5 0.05 0.4 7092205

[params] = N k_mean H p_b randseed

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include <vector>
#include <memory>
#include <iomanip>
#include "../NetExtras.H" 
#include "../models/Wong.H" 
#include "../NetExtrasB.H"


typedef SymmNet<float> NetType;

int main(int argc, char* argv[]) { 
  
  struct WongArgs args;
  readWongArgs(args, argc, argv);
  
  RandNumGen<> generator(args.randseed); 
  NetType net(args.netSize); 

  // Generate the network
  Wong(net, args, generator);

  std::auto_ptr<NetType> netPointer2(findLargestComponent<NetType>(net)); 
  NetType& net2 = *netPointer2;  // Create a reference for easier handling of net.
  
  // Calculate average clustering coefficient
  double avg_clust = 0;
  for (size_t i = 0; i < net2.size(); i++)
      avg_clust += clusteringCoefficient(net2, i);

  size_t edges = numberOfEdges(net2);
  std::cout << net2.size() << " " << (double)2*edges/net2.size() << " " << avg_clust/net2.size() << " " << pearsonCoeff2(net2) << "\n";
}
