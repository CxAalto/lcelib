#define NDEBUG
#include <cassert>
#include <iostream>
#include "../Randgens.H"
//#include "bits/Array.H"
RandNumGen<> globalRandGen;
#include "../Nets.H"
#include<ctime>

#define m 2

int main() {
  typedef SymmNet<bool, ValueTable, WeightSumTable> NetType;
  for (size_t netSize=8; netSize < (1<<21) ; netSize <<= 1) {
    clock_t totTime=0;
    size_t numNets=(1<<20) / netSize;
    for (size_t pass=0; pass<numNets; pass++) {
      NetType theNet(netSize);
      assert(theNet.isLegal());
      size_t i;
      for (i=0; i< m; ++i) {
	for (size_t j=0; j < m; ++j) {
	  if (j!=i) theNet[i][j]=true;
	}
      }
      clock_t cpustart=clock();
      for (; i<netSize; ++i) {
	//std::cerr << "Putting:" << i << "\n";
	AutoMap<size_t, bool> conns;
	while (conns.size() < m) 
	  conns.put(theNet.weighedRandSlot(globalRandGen));
	theNet[i]=conns;
      }    
      totTime+=(clock()-cpustart);
    }
    std::cerr << netSize << " " 
	      << (((float) (totTime))/1000000/numNets) << "\n";
	   
  //for (i=0; i<NET_SIZE; ++i) {
  //std::cout << i << ":" << theNet.c()[i].size() << "\n";
  //}
  }
}


