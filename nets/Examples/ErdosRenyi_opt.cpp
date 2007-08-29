/* ErdosRenyi_opt
   2007 July 20
   Author: Lauri Kovanen

ErdosRenyi model generator for optimization of clustering coefficient.
Generates an ER network according to input parameters and outputs the
size of the largest component, <k>, average clustering the Pearson
correlation coefficient.
                                            
To compile:     g++ -O -Wall ErdosRenyi_opt.cpp -o ErdosRenyi_opt

To run:         ./ErdosRenyi_opt [params]

Example:        ./ErdosRenyi_opt 1000 0.2 7092205

[params] = N p randseed

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include <vector>
#include <memory>
#include <iomanip>
#include "../NetExtras.H" 
#include "../models/ErdosRenyi.H" 
#include "../NetExtrasB.H"


typedef SymmNet<float> NetType;

int main(int argc, char* argv[]) { 
  
  if ( argc < 4 )
    {
      std::cerr << "\nPlease specify arguments: \n\tN, p, randseed\n\n"; exit(1);
    }

  size_t netSize = atoi(argv[1]);
  float p = atof(argv[2]);
  size_t randseed = atoi(argv[3]);
  
  RandNumGen<> generator(randseed); 
  NetType net(netSize); 

  // Generate the network
  ErdosRenyi2(net, p, generator);

  std::auto_ptr<NetType> netPointer2(findLargestComponent<NetType>(net)); 
  NetType& net2 = *netPointer2;  // Create a reference for easier handling of net.

  // Calculate average clustering coefficient
  double avg_clust = 0;
  for (size_t i = 0; i < net.size(); i++)
      avg_clust += clusteringCoefficient(net, i);

  size_t edges = numberOfEdges(net2);
  std::cout << net2.size() << " " << (double)2*edges/net2.size() << " " << avg_clust/net2.size() << " " << pearsonCoeff(net2) << "\n";
}
