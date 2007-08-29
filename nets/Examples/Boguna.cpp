/* Boguna
   2007 June 29                                                               
   Author: Lauri Kovanen


                                            
To compile:     g++ -O -Wall Boguna.cpp -o Boguna

To run:         ./Boguna [params] > net.edg 2> net.log

Example:        ./Boguna 8003 4.20442 1.91466 3892672 > net.edg 2> net.log

[params] = N <k> alpha randseen

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include "../models/Boguna.H" 

//typedef float EdgeData;
//typedef SymmNet<EdgeData> NetType;
typedef SymmNet<float, ValueTable, ExplSumTreeTable> NetType;

int main(int argc, char* argv[]) { 
  
  struct BogunaArgs args;
  readBogunaArgs(args, argc, argv);
  //outputBogunaArgs(args); 
  
  RandNumGen<> generator(args.randseed); 
  NetType net(args.netSize); 

  /* Generate the network and do something with it */

  Boguna(net, args, generator);

  std::auto_ptr<NetType> netPointer2(findLargestComponent<NetType>(net)); 
  NetType& net2 = *netPointer2;  // Create a reference for easier handling of net.

  //size_t edges = numberOfEdges(net);
  //std::cerr << "Edges in the network: \t" << edges << "\n";
  //std::cerr << "Mean degree k: \t\t  " << (double)2*edges/args.netSize << "\n";
  outputEdgesAndWeights(net2);
}
