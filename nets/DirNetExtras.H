#ifndef DIRNETEXTRAS_H
#define DIRNETEXTRAS_H
#include <cassert>
#include <vector>
#include <sstream>
#include <cmath>
#include "../Nets.H"

#define DEBUG

/** 
 * Methods for manipulating and processing directed networks:
 *
 * readDirNet
 * outputEdgesAndWeights
 * edge_reciprocity
 * node_reciprocity
 * out_degree
 * in_degree
 * node_entropy
 */


/** readDirNet ------------------------------------------------------>
 *
 * Function for reading in a directed network. 
 *
 * The first two columns of the input must be the edge source and
 * destination index, in that order. The indices must be integers from
 * 0 to N-1. Edges to self are automatically excluded.
 *
 * The column used as weight is specified in the first parameter, the
 * first column being column 0. If this parameter is missing or less
 * than 2 (the first column after the edge source and destination), a
 * non-weighted network is assumed.
 *
 * If the file contains headers, use the second parameter to tell the
 * number of header lines. If the second parameter is missing or zero,
 * the input is assumed to have no headers.
 *
 * Call example with weight in the third column and no headers:
 *
 * std::auto_ptr<NetType> netPointer(readDirNet<NetType>(3));
 * NetType& net = *netPointer;  // Create a reference for easier handling of net.
 *
 * Lauri Kovanen, 13.8.2008 (from similar method for SymmNet)
 */

template<typename NetType>
NetType * readDirNet(const size_t weightColumn = 0, const int header = 0){

  typedef typename NetType::EdgeData EdgeDataType;
  
  std::vector<size_t> edgeSource;
  std::vector<size_t> edgeDest;
  std::vector<EdgeDataType> edgeData;
  size_t nodeCount = 0;

  // Skip headers
  for (int i = 0; i < header; ++i) {
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
      if (weightColumn > 1) {
	for (int col = 1; col < weightColumn; ++col) {
	  is >> data;      // if weights are given use them
	}
      }
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

      //std::cerr << "Added to vector: " << source << "->" << dest << " : " << data << "\n";
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
    
    if (source != dest) {
      net[source][dest] = data;
      //std::cerr << "Writing to net: " << source << "->" << dest << " : " << data << "\n";
    }
  }
  
  return netPointer.release(); // release the pointer so that it is not destroyed
			       // (we want to return it)
}
// <----- readDirNet -------------------------------------------------

/**  outputEdgesAndWeights ------------------------------------------>
 *
 *  Prints to std::cout the edges and weights of the network in the format      
 *    SOURCE DEST WEIGHT                                                          
*/                                                                              

template<typename NetType>                                                      
void outputEdgesAndWeights(NetType& theNet) {                                   
  typedef typename NetType::EdgeData EdgeDataType;

  // Print out edges and weights of the network (source dest weight):           
  for (size_t i = 0; i < theNet.size(); ++i) {                                      
    for (typename NetType::const_edge_iterator j = theNet(i).begin();             
         !j.finished(); ++j) {
      // Print only edges with non-zero weights.
      if (j.value() != EdgeDataType()) {
	    std::cout << i <<  "\t" << (*j) << "\t" << j.value() << "\n";       
      }
    }                                                                           
  }                                                                             
}                                                                               
// <----- outputEdgesAndWeights --------------------------------------


/**  edge_reciprocity ----------------------------------------------->
 *
 * Calculates the reciprocity of a single edge. Reciprocity is defined as
 *       r_{ij} = (w_{ij} - w_{ji} / (0.5 * (w_{ij} + w_{ji})
 * Naturally r_{ij} == -r_{ji} and r_{ij} \in [0,2] if the weights are 
 * non-negative.
 *
 * Usage:
 *   double r = edge_reciprocity(net, i, j);
 *
 * Lauri Kovanen, 13.8.2008
*/                                            

template<typename NetType>
double edge_reciprocity(const NetType & net, const size_t i, const size_t j) {
  typedef typename NetType::EdgeData EdgeDataType;
  EdgeDataType w_out = net[i][j].out();
  EdgeDataType w_in = net[i][j].in();
  return 2*(w_out - w_in)/(w_out + w_in);
}                                                                               
// <----- edge_reciprocity -------------------------------------------


/**  node_reciprocity ----------------------------------------------->
 *
 * Calculates the reciprocity of a single node. Reciprocity is defined as
 * the weighted sum of incident edge reciprocities, weighted by the total
 * weights (out+in) of the edges.
 *
 * Usage:
 *     float r = node_reciprocity(net, i);
 *
 * To sum the absolute values of edge reciprocities, add `true' after
 * the node index:
 *     float r = node_reciprocity(net, i, true);
 *
 * Lauri Kovanen, 13.8.2008
*/                                            

template<typename NetType>
double node_reciprocity(const NetType & net, const size_t i, const bool absolute = false) {
  typedef typename NetType::EdgeData EdgeDataType;
  
  double r = 0; // Reciprocity
  EdgeDataType w_total = 0; // Total weight of edges
  EdgeDataType w; // For the weight of a single edge

  for (typename NetType::const_edge_iterator j = net(i).begin();             
       !j.finished(); ++j) {
    w = net[i][*j].out() + net[i][*j].in();
    r += w*(absolute ? fabs(edge_reciprocity(net, i, *j)) : edge_reciprocity(net, i, *j));
    w_total += w;
    }                                                   

  return r/w_total;
}                                                                               
// <----- edge_reciprocity -------------------------------------------


/**  out_degree ----------------------------------------------------->
 *
 * Gives the out-degree of given node.
 *
 * Lauri Kovanen, 14.8.2008
*/                                            

template<typename NetType>
size_t out_degree(const NetType & net, const size_t i) {
  typedef typename NetType::EdgeData EdgeDataType;
  size_t out_degree = 0;

  for (typename NetType::const_edge_iterator j = net(i).begin();             
       !j.finished(); ++j) {
    out_degree += (net[i][*j].out() != EdgeDataType());
    }                                                   

  return out_degree;
}                                                                               
// <----- out_degree -------------------------------------------------


/**  in_degree ------------------------------------------------------>
 *
 * Gives the in-degree of given node.
 *
 * Lauri Kovanen, 14.8.2008
*/                                            

template<typename NetType>
size_t in_degree(const NetType & net, const size_t i) {
  typedef typename NetType::EdgeData EdgeDataType;
  size_t in_degree = 0;

  for (typename NetType::const_edge_iterator j = net(i).begin();             
       !j.finished(); ++j) {
    in_degree += (net[i][*j].in() != EdgeDataType());
    }                                                   

  return in_degree;
}                                                                               
// <----- in_degree --------------------------------------------------


/**  node_entropy --------------------------------------------------->
 *
 * Calculates the entropy of a node, defined as the entropy of
 * probabilities (fractions) 
 *          p_ij = w_ij / sum_j(w_ij)
 * where 
 *          w_ij = w_out(ij) + w_in(ij)
 *
 * Because the entropy depends on the number of elements, with maximum
 * of N with N equally probable elements, the entropy is normalized
 * with its maximum value log(N). This corresponds to calculating the
 * entropy with an N-based logarithm and normalizes the resuls to [0,1].
 *
 * Lauri Kovanen, 14.8.2008
*/                                            

template<typename NetType>
double node_entropy(const NetType & net, const size_t i) {
  typedef typename NetType::EdgeData EdgeDataType;
  std::vector<EdgeDataType> weights;
  EdgeDataType w;
  EdgeDataType w_total = 0;

  // Calculate weight of each edge and the total weight.
  for (typename NetType::const_edge_iterator j = net(i).begin();             
       !j.finished(); ++j) {
    w = net[i][*j].out() + net[i][*j].in();
    weights.push_back(w);
    w_total += w;
    //std::cerr << i << "->" << *j << " : w_tot = " << w << "\n";
    }                                                   
  
  double entropy = 0.0;
  double N = net(i).size();
  //std::cerr << "w_total = " << w_total << ", N = " << N << "\n";
  if (N > 1) {
    for (size_t j = 0; j < weights.size(); ++j) {
      double p = ((double)weights.at(j))/w_total;
      //std::cerr << j << ", p = " << p << "\n";
      entropy -= p*log(p)/log(N);
    }
  }

  return entropy;
}                                                                               
// <----- node_entropy -----------------------------------------------


#endif
