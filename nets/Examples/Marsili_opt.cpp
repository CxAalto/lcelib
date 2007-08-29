/* Marsili_opt
   2007 May 22                                                               
   Author: Lauri Kovanen

Function for the optimization of Marsili et al. model. Generates a
Marsili network with given parameters and returns a line containing
[mean_degree clustering_coeff pearson_corr_coeff]
                                            
To compile:     g++ -O -Wall Marsili_opt.cpp -o Marsili_opt

To run:         ./Marsili_opt [params]

Example:        ./Marsili_opt 8003 0.01 0.003 0.05 3892672 1400

[params] = N lambda eta xi randseed [maxiter]
(maxiter is an optional parameter. If not given, 
the number of iterations will be determined dynamically.)

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include "../models/Marsili.H" 
#include "../NetExtras.H"
#include "../NetExtrasB.H"
#include <time.h>

//typedef float EdgeData;
//typedef SymmNet<EdgeData> NetType;
typedef SymmNet<float, ValueTable, ExplSumTreeTable> _NetType;

int main(int argc, char* argv[]) { 
  
  struct MarsiliArgs args;
  readMarsiliArgs(args, argc, argv);
  
  RandNumGen<> generator(args.randseed); 
  _NetType net(args.netSize); 

  //time_t starttime = time(NULL);

  // Generate the network
  Marsili(net, args, generator);

  //time_t endtime = time(NULL);
  //std::cerr << "Time taken by generation: " << difftime(endtime, starttime) << " s\n";

  std::auto_ptr<_NetType> netPointer2(findLargestComponent<_NetType>(net)); 
  _NetType& net2 = *netPointer2;  // Create a reference for easier handling of net.

  double avg_clust = 0;
  for (size_t i = 0; i < net2.size(); i++)
      avg_clust += clusteringCoefficient(net2, i);

  size_t edges = numberOfEdges(net2);
  std::cout << net2.size() << " " << (double)2*edges/net2.size() << " " << avg_clust/net2.size() << " " << pearsonCoeff2(net2) << "\n";
}
