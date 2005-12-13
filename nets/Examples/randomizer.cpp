
/* randomizer.cpp
   2005 Dec 6                                                               
   Authors: Jussi Kumpula, Riitta Toivonen, JP Onnela

randomizer.cpp reads a network from standard input in the format SOURCE DEST EDGEDATA,
randomizes the network keeping the degree distribution intact,
and writes the network to standard output in the same format. 


Note: with assertions off, Dijkstrator runs in O(N logN) time,
otherwise O(N^2).  Randomizer uses Dijkstator, so turn assertions
off for larger networks.

The function randomize() (currently found in lcelib/nets/Randomizer.H)
does the randomization part.  One round consists of making L switches,
where L is the number of links in the network (a switch means swapping
the end nodes of two edges). The number of rounds is given as input.

It is important to keep the network connected. Since checking the
connectivity of the whole network is very time consuming, we use an
adaptive procedure. It is based on the fact that a component that
becomes disconnected in a switch is generally very small.

After switching two edges, we run Dijkstrator from each of them to
see whether a small component broke apart from the network.  The
parameter 'limit' determines how far from the starting node we will
look. If a disconnection is detected, the switch is immediately
reversed, and we try a different switch.  

For more detailed info, see the comments above the function
randomize() in Randomizer.H.


                                            
To compile:     g++ -O -Wall randomizer.cpp -o randomizer

To run:         cat net.edg | ./randomizer <rounds> <limit> <randseed>  > netr.edg

Example:        cat net.edg | ./randomizer 100 15 0 > netr.edg

*/


//#define DEBUG  // for debugging code to be run 
#define NDEBUG // to turn assertions off

#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include "../../Containers.H"
#include "../../Nets.H"
#include "../../Randgens.H"
#include "../NetExtras.H"
#include "../Randomizer.H"



// typedef NDEdgeData<float, float> EdgeData;  
typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;

 
int main(int argc, char* argv[]) {

  /* read command line arguments */
  const size_t rounds=atoi(argv[1]);  // Average number of swithes per edge
   size_t limit=atoi(argv[2]); /* For 'limit', use 15 if no better guess available. 
   After switching two edges, we run Dijkstrator from each of them to see 
   whether a small component broke apart from the network.  
   The parameter 'limit' determines how far from the starting node we will look.  */
  size_t randseed=atoi(argv[3]);  // Seed for random number generator



  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read */
  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());
  NetType& net = *netPointer;  // for easier handling of net



  RandNumGen<> generator(randseed);

  randomize(net, generator, rounds, limit);
  
  outputEdgesAndWeights(net); 
}

