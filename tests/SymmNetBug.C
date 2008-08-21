#include<iostream>
#include<cstdlib>
#include"../Nets.H"
#define NDEBUG
#include<cassert>

// This file triggers a bug in SymmNet. The bug occurs when both edge
// stubs of the same non-existing edge (i->j and j->i) exist at the
// same time: destructor of the second stub fails.
//
// Lauri Kovanen, 13.8.2008

typedef SymmNet<int> NetType;

int main() {

  // Create a new net of fixed size.
  int netSize = 3;
  std::auto_ptr<NetType> netPointer(new NetType(netSize));
  NetType& net = *netPointer;

  std::cerr << net[0][2] << "\n";
  std::cerr << net[1][0] << "\n"; 

  // This will trigger the bug.
  std::cerr << net[0][1] << " " << net[1][0] << "\n"; 
}
