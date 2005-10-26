#define TEST_DIJK
#include <cassert>
#include <iostream>
#include "../Nets.H"
#include "../Containers.H"
#include "../nets/Dijkstrator.H"
#include "../Randgens.H"

#define CONN_DIST 2
#define NET_SIZE 64 /* Power of two, please */

/* Tester for Dijkstra's algorithm. A really stoopid version
 * is included for comparison. */

template <typename NetworkType,
	  template <typename> class Policy=WeightPolicy, 
	  template <typename, typename> class HeapType=FiboHeap> 
class StoopidDijkstrator {
  typedef Policy<typename NetworkType::EdgeData> MyPolicy;
  typedef typename MyPolicy::WeightType WeightType;
  typedef StoopidDijkstrator<NetworkType, Policy, HeapType> MyType;
  typedef HeapType<WeightType, size_t> MyHeapType;
  typedef typename MyHeapType::NodeType HeapNodeType;
  typedef HeapNodeType * HeapNodePtr;
  typedef typename NetworkType::const_edge_iterator EdgeIter;
private:
  const NetworkType & myNet;
  MyHeapType myHeap;
  Set<size_t> found; /* The nodes, shortest route to which 
		              	* have been found */
  
  StoopidDijkstrator() {}; /* Has to get the start node as a param */
  
  /* Variable needed for pointer syntax */
  
  struct FooRouteType {
    WeightType weight;
    size_t dest;
    
    size_t getDest() const {
      return dest;
    }
    WeightType getWeight() const {return weight;}
  };

  FooRouteType currRoute;
  
  bool done;
  
public:
  StoopidDijkstrator(const NetworkType & net, const size_t start): 
    myNet(net), done(false) {
    assert (myNet.contains(start));
    /* Standard constructors should be fine for internal structs. */

    /* We init the structures by simply putting the start node to the 
     * heap with weight zero, which should be the default.  */
    currRoute.dest=start;
    currRoute.weight=0;
    found.put(start);
    ++(*this);
  }
  
  /* Standard destructor should be fine. */

  MyType & operator++() {
    FooRouteType newRoute;
    assert (!done); /* Iteration not completed. */
    /* Iterate through the current node's siblings */
    for (EdgeIter iter=myNet(currRoute.getDest()).begin();
	 !iter.finished();
	 ++iter) {
      if (!found.contains(*iter)) {
	myHeap.push(currRoute.getWeight() + 
		    MyPolicy::getWeight(iter.value()), 
		    *iter);
      }
    } /* Edge iteration */
    /* Then, let's pop the new shortest route: */

    while (!myHeap.finished()) {
      newRoute.weight=*myHeap;
      newRoute.dest=myHeap.value();
      ++myHeap;
      if (!found.contains(newRoute.dest)) {
	found.put(newRoute.dest);
	currRoute=newRoute;
	return (*this);
      }
    }
    
    done=true;
    return (*this);
  }

  const FooRouteType & operator*() const {
    assert (!done);
    return currRoute;
  }
  
  bool finished() const {return done;}
    
 };


unsigned getNodeIndex(int index) {
  return (index & (NET_SIZE-1));
}

float getConnStrength(int diff) {
  if (diff==1) return 1;
  else if (diff==2) return 2.5;
  else return 4.5;
}

int main() {
  RandNumGen<> myRand;
  SymmNet<float> theNet(NET_SIZE);
  float randval;
  
  //  for (int i=0; i<NET_SIZE;i++) {
//      for (int j=1; j<=CONN_DIST; j++) {
//        std::cerr << i << "-" << getNodeIndex(i+j) << ":" 
//  		<< getConnStrength(j) <<"\n";
//        theNet.addToEdge(i, getNodeIndex(i+j), getConnStrength(j));
//      }
//    }

  for (int i=0; i<NET_SIZE; i++) {
    for (int j=i+1; j<NET_SIZE; j++) {
      randval=myRand.nextNormed();
      theNet[i][j]+=randval;
      //std::cerr << i << "-" << j << ":" << randval << "\n";
    }
  }

  Dijkstrator<SymmNet<float> > routeIter(theNet, 0);
#ifdef TEST_DIJK
  std::cerr << "Running tests\n";
  StoopidDijkstrator<SymmNet<float> > testIter(theNet, 0);
#endif
  int i=1;
  for (; !routeIter.finished(); ++routeIter) {
    std::cerr << i << "Found route:(" << (*routeIter).getSource() << "->" 
	      << (*routeIter).getDest() << "), len: " 
	      << (*routeIter).getWeight() << "\n";
#ifdef TEST_DIJK
    assert ((*testIter).getWeight() == (*routeIter).getWeight());
    if ((*testIter).getDest() != (*routeIter).getDest()) {
      std::cerr << ". Test weight OK, dest:" << (*testIter).getDest();
    }    
    ++testIter;
#endif
    ++i;
  }
#ifdef TEST_DIJK
  assert (testIter.finished());
#endif
}




