/* Davidsen_step
   2007 May 22                                                               
   Author: Lauri Kovanen


                                            
To compile:     g++ -O -Wall DavidsenStep.cpp -o DavidsenStep

To run:         ./DavidsenStep [params] > net.edg 2> net.log

Example:        ./DavidsenStep 7000 0.04 38926 > net.edg 2> net.log

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


//typedef float EdgeData;  
//typedef SymmNet<EdgeData> NetType;
typedef SymmNet<float> NetType;

int main(int argc, char* argv[]) { 
  
  struct DavidsenArgs args;
  readDavidsenArgs(args, argc, argv);
  outputDavidsenArgs(args); 
  
  RandNumGen<> generator(args.randseed); 
  NetType net(args.netSize); 

  /* Generate the network and do something with it */

  DavidsenStep(net, args, generator);

  // Analyse only the largest component.
  std::auto_ptr<NetType> netPointer2(findLargestComponent<NetType>(net)); 
  NetType& net2 = *netPointer2;  // Create a reference for easier handling of net.

  //size_t edges = numberOfEdges(net);
  //std::cerr << "Edges in the network: \t" << edges << "\n";
  //std::cerr << "Mean degree k: \t\t  " << (double)2*edges/args.netSize << "\n";
  outputEdgesAndWeights(net2);
}
