/* Boguna_opt
   2007 June 29                                                               
   Author: Lauri Kovanen


                                            
To compile:     g++ -O -Wall Boguna_opt.cpp -o Boguna_opt

To run:         ./Boguna_opt [params]

Example:        ./Boguna_opt 8003 4.20442 1.91466 3892672

[params] = N <k> alpha randseen

*/



//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include "../models/Boguna.H" 
#include "../NetExtrasB.H"

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

  double avg_clust;
  size_t edges;

  /*
  std::cout << "Statistics for full network:\n";

  avg_clust = 0;
  for (size_t i = 0; i < net.size(); i++)
      avg_clust += clusteringCoefficient(net, i);

  edges = numberOfEdges(net);
  std::cout << net.size() << " " << (double)2*edges/net.size() << " " << avg_clust/net.size() << " " << pearsonCoeff2(net) << "\n";

  std::cout << "Statistics for largest component:\n";
  */

  avg_clust = 0;
  for (size_t i = 0; i < net2.size(); i++)
      avg_clust += clusteringCoefficient(net2, i);

  edges = numberOfEdges(net2);
  std::cout << net2.size() << " " << (double)2*edges/net2.size() << " " << avg_clust/net2.size() << " " << pearsonCoeff2(net2) << "\n";
}
