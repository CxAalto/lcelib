#include<iostream>
#include<cstdlib>
#include"./myutil.H"
#define NDEBUG
#include<cassert>
#define NUM_RANDS 60000

using namespace std;

int main() {
  int randvals[NUM_RANDS];

  int counter;
  int duplicates=0;
  int inputIter;
  int remIter;
  unsigned foo;
  cerr << "Entering constructor\n";
  LinProbPow2HashCore<int> hash(16);
  cerr << "Construction done! Generating rands\n";
  
  for (int i=0; i < NUM_RANDS; i++) {
    randvals[i]=rand();
  }

  cerr << "Done! Starting to fill\n";

  for (int i=0; i< NUM_RANDS; i++) {
    if (hash.put(randvals[i], foo)) duplicates++;  
    assert(hash.getNumKeys() == i-duplicates+1);
  }

  assert(hash.isLegal());
  cerr << "Total duplicates:" << duplicates << "\n";
  
  for (int i=0; i<NUM_RANDS; i++) {
    if (!hash.removeKey(randvals[i])) duplicates--;
    assert(hash.getNumKeys()+duplicates+1 == NUM_RANDS-i);
  }
  
  assert(hash.getNumKeys()==0);

  assert(hash.isLegal());

}
