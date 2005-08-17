#define NDEBUG
#include<iostream>
#include<cstdlib>
#include"../Containers.H"
#include<cassert>
#include<ctime>
#define NUM_RANDS 1000000
#define HASH_SIZE 100000


using namespace std;

struct MyPolicy:public SetContainerPolicy<size_t> {
  static const size_t MagicEmptyKey=UINT_MAX;
};

struct MyParams:public DefaultContainerParams {
  typedef void StatusPolicy;
  //static const bool HASH_ORDERED=true;
  //typedef SmallHashController<80, 30, Pow2DivHashController<> > HashController;
};

int main() {
  cerr << "In main\n";
  int * randvals=new int[NUM_RANDS];
  cerr << "Entering contructor!\n";
  Set<size_t, LinearHash, ValueTable, MyPolicy, MyParams> hash;
  size_t duplicates=0;
  size_t inputIter=0;
  size_t remIter;
  clock_t cpustart;

  cerr << "Construction done! Generating rands\n";
  
  for (int i=0; i < NUM_RANDS; i++) {
    randvals[i]=rand();
  }

  cerr << "Done! Starting to fill";

  while (hash.size() < HASH_SIZE) {
    if(hash.put(randvals[inputIter++])) {
      duplicates++;
      std::cerr << "A dup:" << randvals[inputIter-1] << "\n";
    }
  }

  cerr << "Grown! Duplicates:" << duplicates << "\n";

  remIter=0;

  /* Let's try to get an iterator: */

  Set<size_t, LinearHash, ValueTable, MyPolicy, MyParams>::iterator myIter=hash.begin();
  cerr << "A couple of values:\n" << *myIter << " at " << myIter.getLoc();
  ++myIter;
  cerr << *myIter << " at " << myIter.getLoc() << "\n"; 
  ++myIter;
  cerr << *myIter << " at " << myIter.getLoc() << "\n"; 
  ++myIter;
  cerr << *myIter << " at " << myIter.getLoc() << "\n"; 
  ++myIter;
  cerr << *myIter << " at " << myIter.getLoc() << "\n"; 
  
  assert(hash.isLegal());
  cpustart=clock();
  do {
    assert(hash.size()==HASH_SIZE);
    assert(inputIter-remIter==HASH_SIZE+duplicates);
    while (!hash.remove(randvals[remIter++])) {
      duplicates--; 
      cerr << "D";
    }
    while ((inputIter<NUM_RANDS) && (hash.put(randvals[inputIter++]))) {
      duplicates++;
      cerr << "d";
    }
  } while (inputIter < NUM_RANDS);

  std::cerr << "\nClocks" << (clock()-cpustart) << "Per sec:" << CLOCKS_PER_SEC;
  std::cerr << "\nHold per sec:" 
       << ((float) (NUM_RANDS-HASH_SIZE)*CLOCKS_PER_SEC)/(clock()-cpustart) 
       << "\n";
  assert(hash.isLegal());

  while (remIter < NUM_RANDS) {
    if (!hash.remove(randvals[remIter++])) duplicates--; 
    assert(NUM_RANDS-remIter == hash.size()+duplicates);
  }

  std::cerr << "Empty bugger!\n";
  
  assert(hash.size()==0);
  assert(hash.isLegal());

  std::cerr << sizeof(Set<int>) << "\n";
  std::cerr << "Last line!\n";
}

















