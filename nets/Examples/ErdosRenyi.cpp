/* ErdosRenyi
   2007 July 20                                                               
   Author: Lauri Kovanen

Erdos-Renyi network generator.
                                            
To compile:     g++ -O -Wall ErdosRenyi.cpp -o ErdosRenyi

To run:         ./ErdosRenyi [params]

Example:        ./ErdosRenyi 1000 0.2 7092205

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

  outputEdgesAndWeights(net);
}
