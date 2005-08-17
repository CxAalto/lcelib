#define NDEBUG
//#define GNU_PREFETCH
#include<iostream>
#include<cstdlib>
#include<cassert>
#include<ctime>
#include<climits>
#include<algorithm>
#include<ext/hash_set>

#define START_SIZE 1
#define CHECKS 1000000;

using namespace std;
using namespace __gnu_cxx;

int main(int argc, char* argv[]) {
  
  unsigned hashSize;

  //std::cerr << MyPolicy::MagicEmptyKey << "is magic.";
  
  if (argc < 2) {
    hashSize=START_SIZE;
    std::cerr << "Using default start hash size:" << hashSize << "\n";
  } else {  
    hashSize=atoi(argv[1]);
    std::cerr << "Number read:" << hashSize << "\n";
  }

  for (size_t initSize=8; initSize<14;) {
    std::cout << "\n";
    size_t log_num_hashes=11;
    size_t log_num_passes=11; /* Total: 2^27=128M*/

    size_t num_hashes=1 << log_num_hashes;
    size_t num_passes=1 << log_num_passes;
    //size_t maxSize=16*num_hashes*num_passes*4;
    hashSize=initSize;
    //std::cout << hashSize << " " << num_hashes << " " << num_passes << "\n";
    //std::cout << "Clocks per sec:" << CLOCKS_PER_SEC << "\n";
    //std::cout << "Max hash size in bytes=" << maxSize << "\n";
    do {
      clock_t totTime=0;
      for (size_t j=0;  j < num_hashes; ++j) {
	hash_set<size_t> hash(hashSize);
	//std::cerr << hash.getTableSize();
	while (hash.size() < hashSize) hash.insert(rand());
	
	//std::cerr << hash.getTableSize();
	//std::cerr << "I " << j << " " << num_hashes << "\n";
	unsigned numFound=0;
	const hash_set<size_t> & cset=hash;
	clock_t cpustart=clock();
	for (size_t k=0; k<num_passes; ++k) {
	  hash_set<size_t>::const_iterator end=hash.end();
	  for (hash_set<size_t>::const_iterator i=cset.begin();
	       i!=end; ++i) numFound++;
	}
	totTime+=(clock()-cpustart);
	if (numFound != num_passes*hashSize) std::cerr << "Foo!";
      }
      std::cout << hashSize << ":" 
		<< ((((float) totTime) 
		     / hashSize) / (num_hashes*num_passes)) << "\n";
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

















