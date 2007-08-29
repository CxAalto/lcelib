/* Marsili
   2007 May 22                                                               
   Author: Lauri Kovanen


                                            
To compile:     g++ -O -Wall Marsili.cpp -o Marsili

To run:         ./Marsili [params] > net.edg 2> net.log

Example:        ./Marsili 8003 0.01 0.003 0.05 3892672 [1500] > net.edg 2> net.log

[params] = N lambda eta xi randseed maxiter
(maxiter is an optional parameter. If not given, 
the number of iterations will be determined dynamically.)

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include "../models/Marsili.H" 

//typedef float EdgeData;
//typedef SymmNet<EdgeData> NetType;
typedef SymmNet<float, ValueTable, ExplSumTreeTable> NetType;

int main(int argc, char* argv[]) { 
  
  struct MarsiliArgs args;
  readMarsiliArgs(args, argc, argv);
  //outputMarsiliArgs(args); 
  
  RandNumGen<> generator(args.randseed); 
  NetType net(args.netSize); 

  /* Generate the network and do something with it */

  Marsili(net, args, generator);

  //size_t edges = numberOfEdges(net);
  //std::cerr << "Edges in the network: \t" << edges << "\n";
  //std::cerr << "Mean degree k: \t\t  " << (double)2*edges/args.netSize << "\n";
  outputEdgesAndWeights(net);
}
