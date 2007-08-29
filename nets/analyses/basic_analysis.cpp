/* basic_analysis.cpp
   2007 May 24                                                        
   Author: Lauri Kovanen


Reads a network from standard input and writes out analysed information
about it. For each node, the output includes:
[nodeID degree clustering_coeff]

To compile:     g++ -O -Wall basic_analysis.cpp -o full_analysis

To run:         cat net.edg | ./basic_analysis > analysis.txt
                                                 
(net.edg is a file where each row contains the values EDGE TAIL EDGECHARACTERISTIC)
(EDGECHARACTERISTIC for example edge weight)

*/


#define DEBUG  // for debugging code to be run 
//#define NDEBUG // to turn assertions off 


#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include "../../Containers.H"
#include "../../Nets.H"
#include "../NetExtrasB.H"


typedef SymmNet<float> NetType;


int main(int argc, char* argv[])
{

  /* Read network from stdin. Using a pointer to a network, since we
     don't know how many nodes and edges we will read */
  std::auto_ptr<NetType> netPointer(readNet<EdgeData>());
  NetType& net = *netPointer;  // Create a reference for easier handling of net.

  
  // Go through all nodes and calculate their degree and clustering coeff
  std::cerr << "Node_ID Degree Clustering_coeff\n";
  float degree = 0, avg_degree = 0;
  float clust = 0, avg_clust = 0;
  double neighbour_degree = 0;
  for (size_t i = 0; i < net.size(); i++)
    {
      // Degree
      degree = net(i).size();
      avg_degree += degree;

      // Clustering coefficient
      clust = clusteringCoefficient(net, i);
      avg_clust += clust;

      // Calculate average neighbour degree
      neighbour_degree = 0;
      for (NetType::edge_iterator j = net[i].begin();
	   !j.finished();
	   ++j)
	{
	  neighbour_degree += net(*j).size();
	}
      neighbour_degree /= net(i).size();
 
      std::cout << i << ' ' << degree << ' ' << neighbour_degree << ' ' << clust << '\n';
    }

  avg_degree /= net.size();
  avg_clust /= net.size();

  std::cout << net.size() << ' ' << numberOfEdges(net) << ' ' << avg_degree << " " << avg_clust << '\n';
}
