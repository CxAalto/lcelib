
/* connectivityCheckExample.cpp
   Dec 12 2005                                                                                                   
   Author: Riitta Toivonen



This example function shows how to check the connectivity of a network. 

The function readNet (currently found in lcelib/nets/NetExtras.H )
is used to read the network from standard input into a SymmNet<NetType>
(the datatype used in Jorkki's network library) that it creates. 
The input data needs to be in the format SOURCE DEST EDGEDATA
(on each row, the values source, dest, edgedata).
Whatever comes after edgedata on a line is ignored.



To compile:     g++ -O -Wall connectivityCheckExample.cpp -o checkConnectivity

To run:         cat net.edg | ./checkConnectivity

(net.edg is a file where each row contains the values SOURCE DEST EDGEDATA.)

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


typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;


int main(void) {

  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read  */
  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());
  NetType& net = *netPointer;  // Create a reference for easier handling of net. 
  /* The network is now read in, and you can do whatever you like to it. */


  std::cerr << "Checking for connectivity...\n";
  if ( ConnectivityCheck(net) ) 
    std::cerr << "\tThe network is connected.\n\n";
  else
    std::cerr << "\tThe network is disconnected.\n\n";

}

