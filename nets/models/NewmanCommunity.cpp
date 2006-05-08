// NewmanCommunity.cpp 

/* Newman community net

Algorithm (roughly):

- predefine the number of communities
- decide the average number of links a node has within its community 
- and the average degree of a node (links within and out of the community
   summed up)
- randomly place the links so that the averages are met. 

This results in a random network of tightly bound clusters. 
The network was designed by Newman for test purposes. 




To compile:
 g++ -O NewmanCommunity.cpp -o NewmanCommunity

To run:
 ./NewmanCommunity 10 32 16 14 1

 The parameters are: number of communities, size of community, average degree,
 average number of links per node inside community, himmeli output (if 1,
 the header line HEAD TAIL WEIGHT is printed)

*/

//  Authors: Jussi Kumpula, minor additions by Riitta Toivonen



#define NDEBUG // to turn assertions off
#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>

#include "../NetExtras.H"
#include "../../Containers.H"



//typedef SymmNet<size_t> NetType;
typedef float EdgeData;
typedef  SymmNet<EdgeData, ValueTable, ExplSumTreeTable> NetType;
typedef Set<size_t> nodeSet; // stores the indexes of nodes
typedef Map<size_t, nodeSet > nodeMap;


int main(int argc, char* argv[]) {


  const size_t numberOfCommunities = atoi(argv[1]);
  const size_t sizeOfCommunity = atoi(argv[2]);
  const size_t netSize =  sizeOfCommunity*numberOfCommunities;
  const float aveDegree = atof(argv[3]);
  const float aveIn = atof(argv[4]);
  const size_t himmeliOut = atoi(argv[5]);
  const float aveOut = aveDegree - aveIn;
  const float pIn = aveIn/(sizeOfCommunity - 1.0);
  const float pOut = aveOut/(float)(netSize - sizeOfCommunity);
  size_t communityIndex[netSize];

  if (aveDegree < aveIn) {
    std::cerr << "In community degree larger than average degree, exiting...\n";
    exit(-1);
  }

  NetType net(netSize);

  int randseed=time(0); // some random number 
  RandNumGen<> generator(randseed);

  size_t i,j;
  size_t nodeIndex = 0;
  for (i=0; i < numberOfCommunities; ++i) {
    for (j=0; j < sizeOfCommunity; ++j) {
      communityIndex[nodeIndex] = i;        // label each node to correct community
      ++nodeIndex;
    }
  }


  float p=0;
  for (i = 0; i < netSize; ++i) {
     for (j = i+1; j < netSize; ++j) {
       if (communityIndex[i] == communityIndex[j]) p = pIn; // nodes are in the same community
       else p = pOut;                                       // different communities
       if (generator.next(1.0) < p) net[i][j] = 1;          // make link with this probability
     }
  }

  if (himmeliOut) std::cout << "HEAD" << "\t" << "TAIL\n";
  outputEdgesAndWeights(net);

  // std::cerr << "Ave degree: " << calculateAveDegree(net) << "\n";
}
