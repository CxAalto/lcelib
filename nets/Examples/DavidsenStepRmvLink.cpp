/* DavidsenStepRmvLink
   2007 July 25                                                               
   Author: Lauri Kovanen

To compile:     g++ -O -Wall DavidsenStepRmvLink.cpp -o DavidsenStepRmvLink

To run:         ./DavidsenStepRmvLink [params] > net.edg 2> net.log

Example:        ./DavidsenStepRmvLink 7000 0.04 38926 > net.edg 2> net.log

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

  DavidsenStepRmvLink(net, args, generator);

  outputEdgesAndWeights(net);
}
