#include "../models/BA.H"

// to compile: g++ -O BAnetworks.cpp -o BAnetworks
// to run: ./BAnetworks netSize connectionsToAddPerNewNode seedSize 
// example run: ./BAnetworks 10000 3 10 > testi.txt




typedef SymmNet<float, ValueTable, ExplSumTreeTable> NetType;


int main(int argc, char* argv[]) { 
  
  struct SeedArgs seedArgs;
  const size_t netSize=atoi(argv[1]);
  const size_t connectionsToAdd = atoi(argv[2]);
  seedArgs.seedSize = atoi(argv[3]);
  seedArgs.seedType = CHAIN;          // use chain
  seedArgs.netSize = netSize;

  int randseed=time(0); // some random number 
  RandNumGen<> generator(randseed); 

  NetType net(netSize); 

  BAnet_const_addition(net, connectionsToAdd, seedArgs, generator);
  //const float k_ave = 4.0;
  //BAnet_geometric(net, k_ave, seedArgs, generator);

  outputEdgesAndWeights(net);   // outputs the connections in the network
}

