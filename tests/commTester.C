//#define NDEBUG
#include <cassert>
#include <iostream>
#include "../Randgens.H"
#include "../Nets.H"
#include<ctime>

int main() {
  RandNumGen<> generator;
  size_t seedSize=3;
  size_t netSize=10000000;
  float p_r=0.95;
  float p_t=0.9;
  float delta=0.5;
  float w_0=1;

  typedef SymmNet<float> NetType;
  /* A map from nodes in R to nodes in T. */
  typedef Map<size_t, Set<size_t> > ConnMap;
 
  NetType theNet(netSize);
 
  size_t i;

  /* Fully connected seed: */

  for (i=0; i<seedSize; ++i) {
    for (size_t j=0; j<i; ++j) {
      theNet[i][j]=1;
    }
  }

  /* Growth loop. i is the node to be inserted.*/

  for (; i<netSize; ++i) {
    ConnMap RtoT;
    /* The loop for constructiong the initial neighbours. */
    do {
      size_t newNeighbour;
      if (RtoT.size()==i) {
	std::cerr << "Bummer! You did not think of that, did you";
	break;
      }
      do {
	/* Choose a new initial neighbour. Duplicates do not count,
	 * hence the loop.*/
	newNeighbour=generator.next(i);
      } while (RtoT.contains(newNeighbour));
      /* We init the member in the map. The stdlib-style behaviour 
       * seems rather useful in some cases, after all.
       */
      RtoT[newNeighbour];
    } while (generator.nextNormed() >= p_r);

    /* The loop for neighbours' neighbours*/
    
    while (generator.nextNormed() >= p_t) {
      /* A random node from R */
      size_t neighbour=RtoT.randKey(generator);      
      /* If some node's neighbourhood gets "full", we break. */
      if (theNet(neighbour).size()==RtoT[neighbour].size()) break;
      /* A single step of (weighed) random walk: */
      size_t neighsNeigh=theNet(neighbour).weighedRandKey(generator);
      /* Store the result. */
      RtoT[neighbour].put(neighsNeigh);
    }
   
    for (ConnMap::iterator j=RtoT.begin(); !j.finished(); ++j) {
      theNet[i][*j]=w_0;
      for (Set<size_t>::iterator k=RtoT[*j].begin(); !k.finished(); ++k) {
	theNet[i][*k]=w_0;
	theNet[i][*j]+=delta;
	theNet[*j][*k]+=delta;
      }
    }
  }

  /* Printout */
  //for (i=0; i<netSize; ++i) {
  //for (NetType::const_edge_iterator j=theNet.c()[i].begin(); 
  // !j.finished(); ++j) {
  //  //if ((*j) > i) {
  //std::cout << i <<  " " << (*j) << " " << j.value() << "\n";
  ////}
  //}
  //}

  /* Degree dist*/
  size_t max_deg=0;
  for (i=0; i<netSize; ++i) {
    if (theNet(i).size() > max_deg) max_deg=theNet(i).size();
  }
  size_t * dist=new size_t[max_deg+1];
  for (i=0; i<netSize; ++i) {
    dist[theNet(i).size()]++;
  }
  for (i=1; i<=max_deg; ++i) {
    std::cerr << i << ":" << dist[i] << " vs. " 
      //<< (((float) NET_SIZE)*net_m*2*(net_m+1)/(i*(i+1)*(i+2))) 
	      << "\n";
  }
  delete[] dist;
  
}

