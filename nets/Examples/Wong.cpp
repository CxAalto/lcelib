/* Wong
   2007 May 29                                                               
   Author: Lauri Kovanen


                                            
To compile:     g++ -O -Wall Wong.cpp -o Wong

To run:         ./Wong [params] > net.edg 2> net.log

Example:        ./Wong 1000 5 0.05 0.4 7092205 > net.edg 2> net.log

[params] = N k_mean H p_b randseed

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include <vector>
#include <memory>
#include <iomanip>
#include "../NetExtras.H" 
#include "../models/Wong.H" 

typedef SymmNet<float> NetType;

int main(int argc, char* argv[]) { 
  
  struct WongArgs args;
  readWongArgs(args, argc, argv);
  outputWongArgs(args); 
  
  RandNumGen<> generator(args.randseed); 
  NetType net(args.netSize); 

  // Generate the network
  Wong(net, args, generator);

  outputEdgesAndWeights(net);
}
