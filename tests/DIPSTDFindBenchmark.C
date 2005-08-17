#define NDEBUG
//#define GNU_PREFETCH
#include<iostream>
#include<cstdlib>
#include<cassert>
#include<ctime>
#include<climits>
#include<algorithm>
//#include<ext/hash_set>
#include"../Randgens.H"
#include<set>

#define START_SIZE 1

using namespace std;
//using namespace __gnu_cxx;

int main(int argc, char* argv[]) {
  
  RandNumGen<> generator;
  unsigned hashSize;
  bool successful=false;
  std::cerr << sizeof(short);

  // hash_set<size_t> testset;
//   testset.insert(0); testset.insert(1); testset.insert(3);
//   hash_set<size_t>::iterator tester=testset.begin();
//   const size_t * first=&(*tester);
//   tester++;
//   const size_t * second=&(*tester);
//   tester++;
//   const size_t * third=&(*tester);
//   std::cerr << "Size:" << (second-first) << "," << (third-first)
// 	    << ", check:" 
// 	    << (*first) 
// 	    << "," 
// 	    << (*second) 
// 	    << "," 
// 	    << (*third) 
// 	    << "\n";
  
  //std::cerr << MyPolicy::MagicEmptyKey << "is magic.";
  
  if (argc < 2) {
    hashSize=START_SIZE;
    std::cerr << "Using default start hash size:" << hashSize << "\n";
  } else {  
    hashSize=atoi(argv[1]);
    std::cerr << "Number read:" << hashSize << "\n";
  }

  if (argc < 3) {
    std::cerr << "Performing unsuccesful seeks\n";
  } else {
    std::cerr << "Performing succesful seeks\n";
    successful=true;
  }

  for (size_t initSize=1; initSize<8; initSize++) {
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
	set<size_t> hash;/*(hashSize);*/
	//std::cerr << hash.getTableSize();
	size_t * randvals=new size_t[hashSize];
	if (successful) {
	  while (hash.size() < hashSize) {
	    //std::cerr << hash.size() << "\n";
	    randvals[hash.size()]=rand();
	    hash.insert(randvals[hash.size()]);
	    //std::cerr << hash.size() << "\n";
	  }
	  /* The successive hash nodes are probably allocated in adjacent
	   * memory locations. We should therefore not query in the order
	   * of insertions. */ 
	  for (size_t i=0; i<hashSize; ++i) {
	    size_t loc=i+generator.next(hashSize-i);
	    size_t tempval=randvals[i];
	    randvals[i]=randvals[loc];
	    randvals[loc]=tempval;	    
	  }
	} else {
	  while (hash.size() < hashSize) hash.insert(rand());
	  for (size_t i=0; i < hashSize; ++i) {
	    do {
	      randvals[i]=generator.next(UINT_MAX);
	    } while (hash.find(randvals[i]) != hash.end());
	  }
	}
	//std::cerr << "I " << j << " " << num_hashes << "\n";
	unsigned numFound=0;
	clock_t cpustart=clock();
	for (size_t k=0; k<num_passes; ++k) {
	  for (size_t i=0; i<hashSize; ++i) {
	    if (hash.find(randvals[i]) != hash.end()) numFound++;
	  }
	}
	totTime+=(clock()-cpustart);
	assert((successful && (numFound==hashSize*num_passes)) || 
	       ((!successful) && (numFound==0)));
	delete[] randvals;
      }
      std::cout << hashSize << " " 
		<< ((((float) totTime) 
		     / hashSize)*1000 / (num_hashes*num_passes)) 
		<< "\n";
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

















