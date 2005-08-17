#define NDEBUG
#include <iostream>
#include <algorithm>
#include "./FiboHeap.H"
#include "./randgens.H"

#define NUM_RANDS 10000000
#include <cassert>

int main() {
  Ranmar<float> myRand;
  FiboHeap<float> myHeap;
  
  float * randvals;
  float popped;
  unsigned i;
  
  std::cerr << "To rands\n";

  randvals=myRand.array(NUM_RANDS);
  
  std::cerr << "Into heap\n";

  for (i=0; i<NUM_RANDS; i++) {
    assert (myHeap.getNumElems()==i);
    myHeap.push(randvals[i]);
  }

  std::cerr << "Sorting rands\n";
  
  std::sort(&(randvals[0]), &(randvals[NUM_RANDS]));

  std::cerr << "The poptest:";
  for (i=0; i<NUM_RANDS; i++) {
    assert(myHeap.getNumElems() == NUM_RANDS-i);
    assert(myHeap.getMinKey() == randvals[i]);
    //std::cerr << "p";
    popped=myHeap.pop();
    // assert(myHeap.isValid());
    //std::cerr << "P";
    assert(popped == randvals[i]);
    if (i % 10000 == 0) std::cerr << i << "\n";
  }

  std::cerr << "All done\n";

  delete[] randvals;

  std::cerr << "Now, deleting the heap\n";

}
  









