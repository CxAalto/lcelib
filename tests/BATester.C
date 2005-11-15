#define NDEBUG
#include <cassert>
#include <iostream>
#include "../Randgens.H"
//#include "bits/Array.H"
RandNumGen<> globalRandGen=RandNumGen<>();
#include "../Nets.H"
#include<ctime>

#define NET_SIZE 1000
#define net_m 10


int main() {
  typedef SymmNet<bool, ValueTable, ExplSumTreeTable> NetType;
  NetType theNet(NET_SIZE);
  std::cerr << "Constructing the seed";
  size_t i;
  for (i=0; i<net_m; ++i) {
    for (size_t j=0; j <net_m; ++j) {
      if (j!=i) theNet[i][j]=true;
    }
  }
  std::cerr << "Verifying the seed:\n";
  for (i=0; i<net_m; ++i) {
    std::cerr << i << ":" << theNet(i).size()  << ", weight:" 
	      << theNet(i).weight() << "->";
    for (NetType::const_edge_iterator j=theNet(i).begin(); 
	 !j.finished(); ++j) {
      std::cerr << "[" << (*j) << ": " 
		<< j.value() 
		<< "]";
    }
    std::cerr << "\n";
  }
  clock_t cpustart=clock();
  for (; i<NET_SIZE; ++i) {
    if ((i & ((1 << 16)-1))==0) std::cerr << i << "\n";
    //std::cerr << "Putting:" << i << "\n";
    AutoMap<size_t, bool> conns;
    while (conns.size() < net_m) 
      conns.put(theNet.weighedRandSlot(globalRandGen));
    theNet[i]=conns;
    
    //for (Set<size_t>::iterator j=conns.begin(); !j.finished(); ++j) 
    //theNet[i][*j]=1;  
  }
  std::cerr << "Time:" << (((float) (clock()-cpustart))/CLOCKS_PER_SEC) 
	    << "\n";
  //for (i=0; i<NET_SIZE; ++i) {
  //std::cout << i << ":" << theNet.c()[i].size() << "\n";
  //}
  std::cerr << "Ready. Let's construct the histogram\n";
  size_t max_deg=0;
  for (i=0; i<NET_SIZE; ++i) {
    if (theNet(i).size() > max_deg) max_deg=theNet(i).size();
  }
  size_t * dist=new size_t[max_deg+1];
  for (i=0; i<NET_SIZE; ++i) {
    dist[theNet(i).size()]++;
  }
  for (i=net_m; i<=100; ++i) {
    std::cerr << i << ":" << dist[i] << " vs. " 
	      << (((float) NET_SIZE)*net_m*2*(net_m+1)/(i*(i+1)*(i+2))) 
	      << "\n";
  }
}


