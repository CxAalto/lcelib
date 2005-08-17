//#define NDEBUG
#include <cassert>
#include <iostream>
#include "../Randgens.H"
//#include "bits/Array.H"
RandNumGen<> generator=RandNumGen<>();
#include "../Nets.H"
#include<ctime>

int main() {
  size_t seedSize=3;
  size_t netSize=1000;
  float pr=0.95;
  float pt=0.6;
  float delta=0.5;
  typedef SymmNet<float> NetType;
  /* The type of the map from the indices of the initial connections of a 
   * new node to their neigbours. ExplSTT is included in order to make
   * it possible to choose neighbours' neighbours unweightedly. See below.*/
  typedef Map<size_t, Set<size_t>, LinearHash, ExplSumTreeTable> ConnMap;
  std::cerr << "Net\n";
  NetType theNet(netSize);
  std::cerr << "Constructing the seed";
  size_t i;
  for (i=0; i<seedSize; ++i) {
    for (size_t j=0; j<i; ++j) {
      std::cerr << i << "<-> " << j << "\n";
      theNet[i][j]=1;
    }
  }

  std::cerr << "Done.\n";

  for (; i<netSize; ++i) {
    //std::cerr << "\nIn Node:" << i << "\n";
    /* This shall map the new node's neigbours to sets of their neighbours. 
     * We use the ExplSumTreeTable in order to be able to randomly select
     * form the set of the neighbour's neighbours. (or really, 
     * the set of edges originating from them: a node can be a neighbour or
     * several neigbours. See below. */
    ConnMap connCandidates;
    //std::cerr << "ConnCandidates is:" << (&connCandidates) << "\n";
    /* The loop for constructiong the initial neighbours. */
    do {
      size_t newNeighbour;
      do {
	/* Choose a new inital neighbour. Duplicates do not count,
	 * hence the loop.*/
	newNeighbour=generator.next(i);
      } while (connCandidates.contains(newNeighbour));
      /* The "c()" gives us a const reference. Speeds up a bit. 
       * Copies the edge set of the new negighbour selected in the 
       * "snapshot" structure. += for sets adds all the elements. 
       * = would do fine also, though. */
      (connCandidates[newNeighbour])+=theNet.c()[newNeighbour];
      //std::cerr << "Put done.\n";
    } while (connCandidates.size() && generator.nextNormed() >= pr);

    /* Now, the snapshot has been saved. We are free to begin modifying
     * the net itself. */

    for (ConnMap::iterator j=connCandidates.begin(); !j.finished(); ++j) {
      //std::cerr << " " << (*j);
      theNet[i][*j]=1;
    }
    //std::cerr << "\n";

    //std::cerr << "\nTo the triangles:\n";

    while (connCandidates.weight() != 0 && generator.nextNormed() >= pt) {
      //std::cerr << "0";
      /* As we are picking a random edge originating form any of the
       * neighbours of the new node, we must first select the neighbour
       * randomly, _weighed by the degree_. This would certainly work 
       * with the standard Map, but we demonstrate the usage of the 
       * ExplSumTreeTable here. The neighbourhood sets are so small that this 
       * is by no means necessary. */
      size_t randNeighbour=connCandidates.weighedRandKey(generator);
      //std::cerr << "1";
      /* Then, the neighbours neighbour. */
      size_t neighsNeigh=connCandidates[randNeighbour]->randKey(generator);
      //std::cerr << "2";
      theNet[randNeighbour][neighsNeigh]+=delta;
      //std::cerr << "3";
      theNet[i][randNeighbour]+=delta;
      //std::cerr << "4";
      /* We also += into the new edge, as we could have already manipulated
       * it through some other neighbour. */
      theNet[i][neighsNeigh]=1; 
      //std::cerr << "5";
      /* And in order not to select the same fellow twice, we get rid of it.
       * This automagically affects the random selection on the following 
       * rounds. */
      connCandidates[randNeighbour]->remove(neighsNeigh);
      //std::cerr << "6";
    }
    //std::cerr << "\nReloop\n";
  }

  for (i=0; i<netSize; ++i) {
    for (NetType::const_edge_iterator j=theNet.c()[i].begin(); 
	 !j.finished(); ++j) {
      if ((*j) > i) {
	std::cout << i <<  " " << (*j) << " " << j.value() << "\n";
      }
    }
  }
}


