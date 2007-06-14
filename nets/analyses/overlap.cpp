/* overlap.cpp
   2006 Apr 4                                                        
   Author: Riitta Toivonen


Reads a network from standard input and writes out the edges as 
HEAD TAIL OVERLAP (third column containing the overlap for each edge).
If run with option 0, the program outputs the real overlap value 
between [0,1] for each edge. With option 1 it multiplies this value 
by 10000 (this way, the numbers are safer to sort with a shell command
than if the values are in decimal numbers).
                                            
To compile:     g++ -O -Wall overlap.cpp -o overlap

To run:         cat net.edg | ./overlap 1 > overlap.txt
                or, multiplying edge weights by 10000:    cat net.edg | ./overlap  > overlap.txt
                                                 
(net.edg is a file where each row contains the values EDGE TAIL EDGECHARACTERISTIC
(EDGECHARACTERISTIC for example edge weight)

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
#include "../NetExtras.H"



typedef float EdgeData;  
typedef SymmNet<EdgeData> NetType;



int main(int argc, char* argv[]) {


  bool integerOutput=true; /* when this option is set to true, overlap values will be multiplied by 10 000 and truncated to integers. This way, sorting the output file with a shell command is safer than with decimal numbers. If this parameter is set to false, the output will be the real overlap value within [0,1] */ 

  
  if (argc < 1+1) {
    std::cerr << "\nFunction call: ./overlap 0 < net.edg.\n";
    std::cerr << "\nPlease give the network edge file from standard input.\n";
    std::cerr << "Arguments: 1 if overlap values are to be multiplied by 10000 \n";
    std::cerr << "             and rounded to nearest integer (integers are \n";
    std::cerr << "             safer for sorting by overlap).\n";
    std::cerr << "           0 if overlap values in (0...1)\n\n";
    exit(1);
  }

  if (argc > 1) { 
    if ( atoi(argv[1]) == 1 ) {integerOutput=true;} 
    else {integerOutput=false;}
  }
  
  
  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read */
  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());
  NetType& net = *netPointer;  // Create a reference for easier handling of net.
  
  outputOverlap(net,integerOutput,true);

}
