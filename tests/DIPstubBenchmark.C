#define NDEBUG
//#define GNU_PREFETCH
#include<iostream>
#include<cstdlib>
#include"../Containers.H"
#include<cassert>
#include<ctime>
#include<climits>

#define START_SIZE 1
#define CHECKS 1000000;

using namespace std;

struct MyPolicy:public MapContainerPolicy<size_t, size_t> {
  static const size_t MagicEmptyKey=UINT_MAX;
};

struct MyParams:public DefaultContainerParams {
  typedef void StatusPolicy;
  //static const bool HASH_ORDERED=true;
  typedef SmallHashController<85> HashController;
};


typedef Map<size_t, size_t,LinearHash, ValueTable, MyPolicy, MyParams> MapType;


int main(int argc, char* argv[]) {
  
  unsigned hashSize;
  bool successful=false;

  //std::cerr << MyPolicy::MagicEmptyKey << "is magic.";
  
  successful=true;
  

  for (size_t initSize=4; initSize<5; initSize++) {
    std::cout << "\n";
    size_t log_num_hashes=12;
    size_t log_num_passes=12; /* Total: 2^27=128M*/

    size_t num_hashes=1 << log_num_hashes;
    size_t num_passes=1 << log_num_passes;
    //size_t maxSize=16*num_hashes*num_passes*4;
    hashSize=initSize;
    //std::cout << hashSize << " " << num_hashes << " " << num_passes << "\n";
    //std::cout << "Clocks per sec:" << CLOCKS_PER_SEC << "\n";
    //std::cout << "Max hash size in bytes=" << maxSize << "\n";
    do {
      clock_t totTime=0;
      for (size_t j=0;  j < num_hashes*10; ++j) {
	MapType hash(hashSize);
	//std::cerr << hash.getTableSize();
	size_t * randvals=new size_t[hashSize];
	if (successful) {
	  for (size_t i=0; i < hashSize; ++i) {
	    do {
	      randvals[i]=rand();
	    } while (hash[randvals[i]] != 0); /* Was present. */
	    hash[randvals[i]]=1;
	  }
	} else {
	  while (hash.size() < hashSize) hash[rand()]=1;
	  for (size_t i=0; i < hashSize; ++i) {
	    do {
	      randvals[i]=rand();
	    } while (hash.contains(randvals[i])); /* Was present. */
	  }
	}
	//std::cerr << hash.getTableSize();
	//std::cerr << "I " << j << " " << num_hashes << "\n";
	unsigned numFound=0;
	clock_t cpustart=clock();
	for (size_t k=0; k<num_passes; ++k) {
	  for (size_t i=0; i<hashSize; ++i) {
	    if ((hash[randvals[i]]+=1) == 0) hash.remove(randvals[i]);	  
	  }
	}
	totTime+=(clock()-cpustart);
	delete[] randvals;
      }
      std::cout << hashSize << " " 
		<< ((((float) totTime) 
		     / hashSize*100) / (num_hashes*num_passes)) << "\n";
      //<< " " << num_hashes << " " << num_passes << "\n";
      
      if (num_passes > num_hashes) { 
	num_passes >>= 1;
      } else {
	num_hashes >>= 1;
      }
      hashSize*=2;
    } while (num_hashes > 0);
  
    }
}

















