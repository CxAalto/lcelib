//#define NDEBUG
//#define GNU_PREFETCH
#include<iostream>
#include<cstdlib>
#include"../Containers.H"
#include<cassert>
#include<ctime>
#include<climits>

#define NUM_PASSES 8
#define DEFAULT_SIZE 10000000
#define LOG_NUM_RANDS 24 /* 16M randvals */

using namespace std;

struct MyPolicy:public SetContainerPolicy<size_t> {
  static const size_t MagicEmptyKey=UINT_MAX;
};

struct MyParams:public DefaultContainerParams {
  typedef void StatusPolicy;
  //typedef SmallHashController<80, 30, Pow2DivHashController<> > HashController;
};

int main(int argc, char* argv[]) {
  
  unsigned hashSize;
  unsigned numFound=0;
  unsigned numRands=1<<LOG_NUM_RANDS;

  //std::cerr << MyPolicy::MagicEmptyKey << "is magic.";
  
  if (argc < 2) {
    hashSize=DEFAULT_SIZE;
    std::cerr << "Using default hash size:" << hashSize << "\n";
  } else {  
    hashSize=atoi(argv[1]);
    std::cerr << "Number read:" << hashSize << "\n";
  }

  cerr << "In main\n";
  size_t * randvals=new size_t[numRands];
  cerr << "Entering constructor!\n";
  //Set<size_t, LinearHash, ValueTable, MyPolicy, MyParams> hash;
  //Set<size_t> hash;
  //AutoMap<size_t, bool, LinearHash, ValueTable, MyPolicy, MyParams> hash;
  AutoMap<size_t, int, LinearHash, ValueTable, MyPolicy, MyParams> hash;

  cerr << "Construction done! Filling with rands:\n";

  while (hash.size() < hashSize) {
    //hash.put(rand());
    hash[rand()]+=1;//true;
  }

  cerr << "Generating rands:\n";
  for (unsigned i=0; i<numRands; i++) {
    randvals[i]=rand();
  }
    
  cerr << "Done! Into the benchmark:\n";
  
  assert(hash.isLegal());
  clock_t cpustart=clock();
  for (unsigned pass=0; pass<NUM_PASSES; pass++) {
    for (unsigned i=0; i<numRands; ++i) { 
      if (hash[randvals[i]]) numFound++; 
      //if (randvals[i]==1) numFound++; 
    }
  }
  
  std::cerr 
    << "\nClocksPerFind:" 
    << ((float) (clock()-cpustart))/CLOCKS_PER_SEC*3000000000.0
    /(numRands*NUM_PASSES) 
    << "\n";
  std::cerr << "\nClocks" << (clock()-cpustart) << "Per sec:" 
	    <<  CLOCKS_PER_SEC;
  std::cerr << "Found:" << numFound << "\n"; 
  assert(hash.isLegal());
  std::cerr << hash.elemSize();
}

















