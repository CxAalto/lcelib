
/* readNetExample.cpp
   Dec 6 2005                                                                                                   
   Author: Riitta Toivonen

 

The function readNet (currently found in lcelib/nets/NetExtras.H )
reads a network from standard input into a SymmNet<NetType> 
(the datatype used in Jorkki's network library) that it creates. 

It does not require network size or number of edges as input, but instead
deduces these from the input file. This is different from JP's
otherwise excellent net reading function (also named readNet, but not
available on cvs as of Dec 6 2005). 

The input data needs to be in the format SOURCE DEST EDGEDATA
(on each row, the values source, dest, edgedata).
Whatever comes after edgedata on a line is ignored.

This example function shows how to use readNet. 
Function call example shown below. 


Note:
Since we don't know the number of nodes beforehand, we can't generate 
the SymmNet<NetType> before we call readNet. This causes calling readNet 
to look a bit mean ( std::auto_ptr<NetType> netPointer(readNet<EdgeData>()); ).
Simply copy the few lines of code from this file. 



To compile:     g++ -O -Wall readNetExample.cpp -o readNet

To run:         cat net.edg | ./readNet > netcopy.edg

(net.edg is a file where each row contains the values SOURCE DEST EDGEDATA.)

 */


//#define DEBUG  // for debugging code to be run 
//#define NDEBUG // to turn assertions off 



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


// typedef NDEdgeData<float, float> EdgeData;  
typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;






int main(void) {

  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read (..and a
     copy-constructor does not yet exist for SymmNet. If a
     copy-constructor is created for SymmNet, readNet can be rewritten
     more neatly and called with: NetType net = readNet<EdgeData>(); )  */

  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());

  NetType& net = *netPointer;  // Create a reference for easier handling of net.
  

  /* The network is now read in, and you can do whatever you like to it. */
  /* Let's just output it now. */ 

  outputEdgesAndWeights(net); 

}

