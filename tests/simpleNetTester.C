#include <cassert>
#include <iostream>
#include "../Nets.H"

#define CONN_DIST 2
#define NET_SIZE 8 /* Power of two, please */

/* Tester using common unsigned int:s as payload. 
 * Network topology is a ring with given connection distance. */

unsigned getNodeIndex(int index) {
  return (index & (NET_SIZE-1));
}

// struct intPayload {
//   int data;
//   intPayload(int val=0): data(val) {};
//   intPayload& operator+=(intPayload& src) {
//     data+=src.data;
//     return *this;
//   }
//   intPayload& operator-=(intPayload& src) {
//     data-=src.data;
//     return *this;
//   }
//   bool operator==(intPayload& cmp) {return cmp.data==data;}
  
//   intPayload& operator=(const intPayload& src) {
//     data=src.data;
//     return *this;
//   }
  
//   operator int() {return data;}
//   operator 

// };

int main() {
  typedef SymmNet<int ,WeightSumTable, ExplSumTreeTable> NetType;
  NetType theNet(NET_SIZE);
  /* Construct the network */
  int currValue;
  for (int i=0; i<NET_SIZE; i++) {
    for (int j=i-CONN_DIST; j<=i+CONN_DIST; j++) {
      currValue=getNodeIndex(j);
      if (i!=j) {
	std::cerr << i << "<->" << getNodeIndex(j) << " "; 
	std::cerr << theNet.c()[i][currValue] << ", ";
	std::cerr << "("  << theNet.c()[i].weight() << "," 
		  << theNet.c()[currValue].weight() << ") ";
	theNet[i][currValue]+=1;
	std::cerr << theNet.c()[i][currValue] << ", ";
	std::cerr << "("  << theNet.c()[i].weight() << "," 
		  << theNet.c()[currValue].weight() << ")\n";
      }
      //std::cerr << "Leg assert...";
      assert(theNet.isLegal());
      //std::cerr << "LA done\n";
    }
  }
  
  std::cerr << "\nInited, reading:\n";

  for (NetType::NodeIter i=theNet.begin(); !i.finished(); ++i) {
    std::cerr << "\nNode:" << *i << ", size:" 
	      << i.value()->size()  << ", weight:" << i.value()->weight()
	      << "  -> ";
    for (NetType::edge_iterator j=i.value().begin(); 
	 !j.finished();) {
      std::cerr << "[" << (*j) << ": " 
		<< j.value() 
		<< "]";
      ++j;
      //std::cerr << "Leg assert...";
      assert(theNet.isLegal());
      //std::cerr << "LA done\n";

    }
  }

  std::cerr << "\n\nRead, substracting:\n";
  
  for (NetType::NodeIter i=theNet.begin(); !i.finished(); ++i) {
     std::cerr << "\nNode:" << *i << ", size:" 
	       << i.value()->size() << ", weight:" << i.value()->weight()
	       << "  -> \n";
     //std::cerr << "Leg assert...";
     assert(theNet.isLegal());
     //std::cerr << "LA done\n";
     for (NetType::edge_iterator j=i.value().begin(); 
	  !j.finished(); ++j) {
       std::cerr << (*j) << ": "; 
       std::cerr << (j.value()-=1) << "\n";
     }
     //std::cerr << "Leg assert...";
     assert(theNet.isLegal());
     //std::cerr << "LA done\n";
  }
  std::cerr << "\nSubstracted, cheking\n";
  assert(theNet.isLegal());
  for (NetType::NodeIter i=theNet.begin(); !i.finished(); ++i) {
     std::cerr << "\nNode:" << *i << ", size:" 
	      << i.value()->size() << ", weight:" << i.value()->weight()
	      << "  -> ";
     for (NetType::edge_iterator j=i.value().begin(); 
	  !j.finished(); ++j) {
       std::cerr << (*j) << ": " 
		 << j.value() 
		 << ", ";
     }
     assert(theNet.isLegal());
  }
  std::cerr << "\n\n";
  
}


