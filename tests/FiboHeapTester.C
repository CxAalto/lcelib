//#define NDEBUG
#include <iostream>
#include <algorithm>
#include "../misc/FiboHeap.H"
#include "../Randgens.H"

#define NUM_RANDS 10000000
#include <cassert>

int main() {
  RandNumGen<> myRand;
  FiboHeap<float> myHeap;
  
  float * randvals;
  float popped;
  unsigned i;
  
  std::cerr << "To rands\n";

  randvals=new float[NUM_RANDS];
  for (i=0; i<NUM_RANDS; ++i) randvals[i]=myRand.nextNormed();

  std::cerr << "Into heap\n";

  for (i=0; i<NUM_RANDS; i++) {
    assert (myHeap.size()==i);
    myHeap.push(randvals[i]);
  }

  std::cerr << "Sorting rands\n";
  
  std::sort(&(randvals[0]), &(randvals[NUM_RANDS]));

  std::cerr << "The poptest:";
  for (i=0; i<NUM_RANDS; i++) {
    assert(myHeap.size() == NUM_RANDS-i);
    //std::cerr << "p";
    
    // assert(myHeap.isValid());
    //std::cerr << "P";
    assert(*myHeap == randvals[i]);
    ++myHeap;
    if (i % 10000 == 0) std::cerr << i << "\n";
  }

  std::cerr << "All done\n";

  delete[] randvals;

  std::cerr << "Now, deleting the heap\n";

}
  









