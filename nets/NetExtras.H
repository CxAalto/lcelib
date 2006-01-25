 
// lcelib/nets/NetExtras.H
// Auxiliary functions for use with the SymmNet class.
// 
// Some of these functions, such as ClearNet or ConnectivityCheck,
// could be methods within the SymmNet class, but are now written
// using the interface. That's why the file is named NetExtras.H.
// 
// Authors: Riitta Toivonen, Jussi Kumpula



#ifndef NETEXTRAS_H
#define NETEXTRAS_H
#include "../Containers.H"
#include "../Nets.H"
#include "../Randgens.H"
#include "Dijkstrator.H"

#include <cassert>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <ctime>






/*

 Functions included 
 ------------------  

 readNet                (added Dec 6 2005, Riitta)
 outputEdges            (Nov 9 2005, Riitta)
 outputEdgesAndWeights  (Nov 9 2005, Riitta)
 copyNet                (Jussi) 
 numberOfEdges          (Jussi) 
 numberOfTriangles      (Jan 4 2006, Riitta) 
 ConnectivityCheck      (Riitta)
 ClearNet               (Riitta)



Feel free to add whatever useful bits you have that belong to this category.
A reasonable amount of testing is expected.

Suggestions, corrections etc. also welcomed. 

Usage examples for nontrivial functions should be found in 
lcelib/nets/Examples/

 */ 













/* function readNet */
/* Reads a net from stdin and returns a pointer to a new SymmNet
 * instance. The number of edges and nodes does not need to be 
 * known beforehand - the function deduces them from the input. 
 * This can be useful if you need to randomize a large ensemble of 
 * networks. 
 *  
 * For more info and an example on how to use this function, 
 * see lcelib/nets/Examples/readNetExample.cpp
 * 
 * EdgeDataType must be a type with an extractor (cin >> data).
 * 
 * If a copy-constructor is written for SymmNet, this function could
 * be written and called more neatly. Or, if a SymmNet was written that 
 * can handle changing numbers of nodes, and an extractor was written 
 * for it, we could throw away this function and just say 
 * 
 *    NetType net; 
 *    std::cin >> net;
 * 
 * (which means Jorkki would have done all the dirty work). 
 * In the meantime, use this laborious construction. 
 */



template<typename EdgeDataType>
SymmNet<EdgeDataType>* readNet()
{
  typedef SymmNet<EdgeDataType> MyNetType;

  // Read edge data into a temporary buffer.
  /*  struct Edge {
      size_t source;
      size_t dest;
      EdgeDataType data; 
      };
      std::vector<Edge> edges;
      Turned out to be impossible to make a vector of Edges.
      Making separate vectors for source, dest and data.
  */
  
  std::vector<size_t> edgeSource;
  std::vector<size_t> edgeDest;
  std::vector<EdgeDataType> edgeData;
  size_t nodeCount = 0;

  while (std::cin) {
    size_t source, dest;
    EdgeDataType data;
    std::string line;
    std::getline(std::cin, line);  // Read a line from input into a string.
    if (!line.empty()) {
      std::istringstream is(line);  // Extract data using a stringstream.
      is >> source;
      is >> dest;
      is >> data;
      if (!is) {
	std::cerr << "\nError in reading input.\n"
		  << "Possibly a line containing too few values, or a header line.\n\n"; 
	exit(1); 
      }

      // Track the maximum node index.
      if (source >= nodeCount)
	nodeCount = source + 1;
      if (dest >= nodeCount)
	nodeCount = dest + 1;
      
      edgeSource.push_back(source);
      edgeDest.push_back(dest);
      edgeData.push_back(data);
    }
  }

  // Construct the net.
  std::auto_ptr<MyNetType> netPointer(new MyNetType(nodeCount));
  MyNetType& net = *netPointer;  // Create a reference for easier access.
  
  // Add edges to the net.
  for (size_t i = 0; i < edgeSource.size(); ++i) {
    size_t source = edgeSource.at(i);
    size_t dest = edgeDest.at(i);
    EdgeDataType data = edgeData.at(i);
    
    if (source == dest) {     
      std::cerr << "\nInput file contains a loop edge.\n\n";
      exit(1);
    }
    if (net(source).contains(dest)) {
      std::cerr << "\nInput file contains same edge twice.\n\n";
      exit(1);
    }
    net[source][dest] = data;
    assert(net[source][dest] == data);
  }
  
  std::cerr << "\nreadNet: read in " << nodeCount << " nodes and ";
  std::cerr << edgeSource.size() << " links.\n";
  std::cerr << "Reading data complete.\n\n";
  
  return netPointer.release(); // release the pointer so that it is not destroyed
			       // (we want to return it)
}
// <--- readNet
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 








// readNet2 ------------------------------->
// This is almost a direct copy of the readNet-function.
// However, this function can handle all kinds of networks, for example  
// SymmNet<EdgeData, ValueTable, ExplSumTreeTable>.
// Furthermore, this function can read files which do not specify
// the weights (set parameter weights=0). If weights are given set parameter
// weights=1.  Also, files which contain on their first line some headers
// etc. can be read by setting himmeli=1, so that the first line is ignored.
// 
// call example:
// std::auto_ptr<NetType> netPointer(readNet2<NetType>(1,0));
// NetType& net = *netPointer;  // Create a reference for easier handling of net.

template<typename NetType>
NetType * readNet2(const size_t weights, const size_t himmeli)
{
  typedef typename NetType::EdgeData EdgeDataType;
  
  std::vector<size_t> edgeSource;
  std::vector<size_t> edgeDest;
  std::vector<EdgeDataType> edgeData;
  size_t nodeCount = 0;

  if (himmeli) {                   // first line in himmeli edge data file has to be skipped
    std::string line;
    std::getline(std::cin, line);  // Read a line from input and discard it...
  }

  while (std::cin) {

    size_t source, dest;
    EdgeDataType data;
    std::string line;
    std::getline(std::cin, line);   // Read a line from input into a string.
    if (!line.empty()) {
      std::istringstream is(line);  // Extract data using a stringstream.
      is >> source;
      is >> dest;
      if (weights) is >> data;      // if weights are given use them
      else data=1;                  // else set all weights to 1
      if (!is) {
	std::cerr << "\nError in reading input.\n"
		  << "Possibly a line containing too few values, or a header line.\n\n"; 
	exit(1); 
      }

      // Track the maximum node index.
      if (source >= nodeCount)
	nodeCount = source + 1;
      if (dest >= nodeCount)
	nodeCount = dest + 1;
      
      edgeSource.push_back(source);
      edgeDest.push_back(dest);
      edgeData.push_back(data);
    }
  }

  // Construct the net.
  std::auto_ptr<NetType> netPointer(new NetType(nodeCount));
  NetType& net = *netPointer;  // Create a reference for easier access.
  
  // Add edges to the net.
  for (size_t i = 0; i < edgeSource.size(); ++i) {
    size_t source = edgeSource.at(i);
    size_t dest = edgeDest.at(i);
    EdgeDataType data = edgeData.at(i);
    
    if (source != dest && !net(source).contains(dest)) {
      net[source][dest] = data;
      assert(net[source][dest] == data);
    }
  }
  
  return netPointer.release(); // release the pointer so that it is not destroyed
			       // (we want to return it)
}
// <---------------- readNet2 --------------------------








                                                              
/*  function outputEdgesAndWeights(NetType& theNet)                             
    Prints to std::cout the edges and weights of the network in the format      
    SOURCE DEST WEIGHT                                                          
*/                                                                              

template<typename NetType>                                                      
void outputEdgesAndWeights(NetType& theNet) {                                   
  
  // Print out edges and weights of the network (source dest weight):           
  for (size_t i=0; i<theNet.size(); ++i) {                                      
    for (typename NetType::const_edge_iterator j=theNet(i).begin();             
         !j.finished(); ++j) {                                                  
      if (i<(*j))                                                               
        std::cout << i <<  "\t" << (*j) << "\t" << j.value() << "\n";       
      //  std::cout << i+1 <<  "\t" << (*j)+1 << "\t" << j.value() << "\n";       
      //printing nodal indices as  1...N instead of 0...N-1                     
    }                                                                           
  }                                                                             
}                                                                               
// <--- outputEdgesAndWeights                                                   
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -           












/*  function outputEdges(NetType& theNet)                                       
    Prints to std::cout the edges of the network in the format                  
    SOURCE DEST                                                                 
*/

template<typename NetType>
void outputEdges(NetType& theNet) {

  // Print out edges of the network (source dest):                              
  for (size_t i=0; i<theNet.size(); ++i) {
    for (typename NetType::const_edge_iterator j=theNet(i).begin();
         !j.finished(); ++j) {
      if (i<(*j))
        std::cout << i <<  "\t" << (*j) << "\t" << "\n";
      //std::cout << i+1 <<  "\t" << (*j)+1 << "\t" << "\n";
      //printing nodal indices as  1...N instead of 0...N-1                     
    }
  }
}
// <--- outputEdges                                                             
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -           























/*function copyNet*/
/* copies net1 to net2, the nets have to be of the same size */

template<typename NetType>
void copyNet(NetType & net1, NetType & net2) {

  assert(net1.size() == net2.size() );
  size_t netSize = net1.size();
  ClearNet(net2, net2.size());

  for (size_t i=0; i<netSize; ++i) {
    for (typename NetType::const_edge_iterator j=net1(i).begin(); !j.finished(); ++j) {
      net2[i][*j] = net1(i)[*j];
    }
  }
}
// <--- copyNet
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  













/* function numberOfEdges*/
/* calculates the number of edges in the network */
template<typename NetType>
size_t numberOfEdges(NetType & net) {
  size_t links=0;

  for (size_t i=0; i<net.size(); ++i) {
    links += net(i).size();
  }
  links=links/2; // divide by two to get the number of links                                            
  return links;
}
// <--- numberOfEdges
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  










/* function numberOfTriangles*/
/* calculates the number of triangles in the network 
   (tested on 3-clique, 4-clique, and chain - gives correct answers 1, 4, 0)
*/
template<typename NetType>
size_t numberOfTriangles(NetType & net) {

  size_t trianglesum=0;
  
  // Go through each node in the network:
  for (size_t i=0; i<net.size(); ++i) {
    
    // Go through the neighbors of node i:
    for (typename NetType::const_edge_iterator j=net(i).begin();
	 !j.finished();
	 ++j) {
      
      for (typename NetType::const_edge_iterator k=net(*j).begin();
	   !k.finished();
	   ++k) {
	if (net(*k)[i] != 0) {
	  trianglesum++;
	}
      }
    }
  }
  
  return trianglesum/6; // each triangle was counted 6 times 
}
// <--- numberOfTriangles
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  














// ConnectivityCheck ---->                                                   
/*                                                                           
                                                                             
The function ConnectivityCheck uses the Dijkstra algorithm                   
to determine whether or not a path exists between the first                  
node in the network and all others - in other words, whether                 
or not the network is connected. In fact, it finds all shortest              
paths from this node, which takes an equivalent time to calculate.           
                                                                             
If the network is connected, all nodes are reachable from                    
any single node. We start the search with the first node                     
in the network, since it always exists for a nonempty network.               
                                                                             
To carry out the Dijkstra algorithm, we use the Dijkstrator                  
from lcelib/nets/Dijkstrator.H.                                
                                                                             
*/

template<typename NetType>
bool ConnectivityCheck(NetType & theNet) {

  /* Find all shortest paths from node 0 */
  Dijkstrator<NetType> paths(theNet,0);
  for (; !paths.finished(); ++paths) {
  }

  /* Check whether all other nodes were reachable */
  if (paths.getFoundSet().size() == theNet.size() )
    return true;
  else if (paths.getFoundSet().size() < theNet.size() )
    return false;
  else
    assert(false);
}
//  <---- ConnectivityCheck                                                  












// ClearNet ---->
/*   A function to clear all edges of a network.                            
     The number of nodes does not change. */

template <typename NetType>
void ClearNet(NetType& theNet, size_t netSize) {
  /* Go through each node */
  for (size_t i=0; i<netSize; ++i) {
    /* and remove the links to its neighbors */
    for (typename NetType::edge_iterator j=theNet[i].begin();
         !j.finished();
         ++j) {
      j.value()=0;  // don't use 'theNet[i][*j]=0;' to avoid messing up the iterator
    }
  }
}
//  <---- ClearNet   









template<typename NetType>
size_t findMaxDegree(NetType& theNet) {
  size_t kmax = 0;
  for (size_t i=0; i<theNet.size(); ++i) {
    size_t curr_deg=theNet(i).size(); // Read the degree of node i
    if (kmax < curr_deg ) {
      kmax = curr_deg;
    }
  }
  return kmax;
}
//  <----   findMaximumDegree 





// -------- find_k_core ------------>
// finds the k-core of the network. This should be the most central part
// the network in some sense. The algorithm takes away all nodes which have
// less than k neighbors so that in the remaining network all nodes have at least
// k neigbors. If you don't want to mess up your original network do a copy of it
// and run find_k_core on the copy.

template<typename NetType>
void find_k_core(NetType & theNet, size_t k) {

  if (k < 1) {
    std::cerr << "K-core degree limit too small, exiting...\n";
    exit(-1);
  }
  
  std::cerr << "Finding " << k << "-core...\n";
  std::cerr << "Rounds\t Min degree \t Edges remaining \n"; 
  size_t minDegree, currDegree;
  size_t i;
  size_t netSize = theNet.size();
  size_t rounds = 0;

  do {
    for (i=0; i<netSize; ++i) { // check all nodes in the network
      if (theNet(i).size() < k) {  // remove all links from nodes which have degree less than k
	for (typename NetType::edge_iterator j=theNet[i].begin(); !j.finished(); ++j)  j.value() = 0; // remove edge i - > j
      }
    }

    // find minimum degree in the net
    minDegree = findMaxDegree(theNet);
    for (i=0; i<netSize; ++i) {
      currDegree = theNet(i).size();
      if (currDegree < minDegree && currDegree > 0) minDegree = theNet(i).size();
    }
    ++rounds;
    std::cerr << rounds << "\t" << minDegree << "\t" << numberOfEdges(theNet)  << "\n";
  } while (minDegree < k && minDegree > 0);  // do this as long as small degree nodes remain
}
// <-------- find_k_core ------------




#endif //~ NETEXTRAS_H 
