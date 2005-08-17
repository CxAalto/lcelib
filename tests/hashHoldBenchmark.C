#define NDEBUG
#include<iostream>
#include<cstdlib>
#include"./Containers.H"
#include"bits/ExplSumTreeTable.H"
#include"bits/WeightSumTable.H"
#include<cassert>
#include<ctime>

#define NUM_PASSES 1
#define DEF_LOG_HASH_SIZE 24 /* 16M */

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
  unsigned numRands;
  unsigned mask;
  
  if (argc < 2) {
    hashSize=1 << DEF_LOG_HASH_SIZE;
    std::cerr << "Using default hash size:" << hashSize << "\n";
  } else {  
    hashSize= 1 << atoi(argv[1]);
    std::cerr << "Size read:" << hashSize << "\n";
  }
  numRands=2*hashSize;
  mask=numRands-1;

  cerr << "In main\n";
  size_t * randvals=new size_t[numRands];
  size_t * randweights=new size_t[numRands];
  cerr << "Entering constructor!\n";
  //Set<size_t, LinearHash, ValueTable, MyPolicy, MyParams> hash;
  //Set<size_t> hash;
  Map<size_t, size_t, LinearHash, ExplSumTreeTable, MyPolicy, MyParams> hash;
  //AutoMap<size_t, bool, LinearHash, ValueTable, MyPolicy, MyParams> hash;
  //AutoMap<size_t, int, LinearHash, ValueTable, MyPolicy, MyParams> hash;

  cerr << "Construction done! Filling with rands:\n";
  
  /* First fill...*/
  {
    Set<size_t> tempHash;
    for (size_t  i=0; i < numRands; ++i) {
      assert(tempHash.size()==i);
      do {
	randvals[i]=rand();
      } while (tempHash.put(randvals[i])); /* We do not accept duplicates. */
      randweights[i]=rand();
    }
    
    assert(tempHash.size()==numRands);    
    assert(tempHash.isLegal());
    std::cerr << "Hope we destroy...";
  }
  std::cerr << "...before this. Init fill...";
  for (size_t  i=0; i < hashSize; i++) {
    assert(hash.size()==i);
    hash[randvals[i]]=randweights[i];
  }
  assert(hash.size()==hashSize);
  assert(hash.isLegal());

  /* Status: no duplicates to be encountered in the standard loop. 
   * Hash filled with the vals in the head of the randvals[]. */


  cerr << "...done. Begin holdttest\n";
  clock_t cpustart=clock();

  for (unsigned i=0; i<numRands*NUM_PASSES; ++i) {
    assert(hash.contains(randvals[i & mask]));
    assert(hash[randvals[i & mask]]==randweights[i & mask]);
    assert(hash[randvals[i & mask]]==randweights[i & mask]);
    //std::cerr << "#";
    hash.remove(randvals[i & mask]);
    //std::cerr << "!";
    hash[randvals[(i+hashSize) & mask]]=randweights[(i+hashSize) & mask];
    assert(hash.size()==hashSize);
    /* Check the legality once per pass. */
    assert(((i & mask) != 0) 
	   || (hash.isLegal()));
  }
  
  std::cerr << "\nHolds per sec:" 
	    << ((((float) (numRands))*NUM_PASSES*CLOCKS_PER_SEC)
		/(clock()-cpustart)) 
	    << "\n";
  assert(hash.isLegal());
  std::cerr << hash.elemSize() << "\n";
}

















