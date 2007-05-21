 
// lcelib/nets/NetExtras2.H
// 
// More network functions
// Authors: Riitta Toivonen, Jussi Kumpula



#ifndef NETEXTRAS2_H
#define NETEXTRAS2_H
#include "../Containers.H"
#include "../Nets.H"
#include "../Randgens.H"
#include "Dijkstrator.H"
#include "NetExtras.H"

#include <cassert>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <ctime>
#include <fstream>






/*

 Functions included 
 ------------------  


 findMaxDegree              (Jussi)
 findMaxStrength            (Jussi)
 find_k_core                (Jussi)
 find_s_core                (Jussi)

 */ 






// -------- findMaxDegree ------------>
// finds the maximum degree in the network
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
//  <----   findMaxDegree 





// -------- findMaxStrength ------------>
// finds the maximum strength in the network
template<typename NetType>
typename NetType::EdgeData findMaxStrength(NetType& theNet) {
  typedef typename NetType::EdgeData EdgeDataType;
  EdgeDataType smax = 0;
  for (size_t i=0; i<theNet.size(); ++i) {
    EdgeDataType curr_strength=theNet(i).weight(); // Read the degree of node i
    if (smax < curr_strength ) {
      smax = curr_strength;
    }
  }
  return smax;
}
//  <----   findMaxStrength








// -------- find_k_core ------------>
// finds the k-core of the network. This should be the most central part
// the network in some sense. The algorithm takes away all nodes which have
// less than k neighbors so that in the remaining network all nodes have at least
// k neigbors. If you don't want to mess up your original network make a copy of it
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








// -------- find_s_core ------------>
// finds the s-core of the network. This should be the most central part
// the network in some sense. The algorithm takes away all nodes which have strength
// less than s so that in the remaining network all nodes have at least strength
// s. If you don't want to mess up your original network do a copy of it
// and run find_s_core on the copy.

/* find_s_core(net, s_core);
  outputEdgesAndWeights(net); */
  

template<typename NetType>
void find_s_core(NetType & theNet, typename NetType::EdgeData s) {

  typedef typename NetType::EdgeData EdgeDataType;

  if (s < 0) {
    std::cerr << "S-core degree limit too small, exiting...\n";
    exit(-1);
  }
  
  std::cerr << "Finding " << s << "-core (strength)...\n";
  std::cerr << "Rounds\t Min degree \t Edges remaining \n"; 
  EdgeDataType minStrength, currStrength;
  size_t i;
  size_t netSize = theNet.size();
  size_t rounds = 0;

  do {
    for (i=0; i<netSize; ++i) { // check all nodes in the network
      if (theNet(i).weight() < s) {  // remove all links from nodes which have degree less than k
	for (typename NetType::edge_iterator j=theNet[i].begin(); !j.finished(); ++j)  j.value() = 0; // remove edge i - > j
      }
    }

    // find minimum degree in the net
    minStrength = findMaxStrength(theNet);
    for (i=0; i<netSize; ++i) {
      currStrength = theNet(i).weight();
      if (currStrength < minStrength && currStrength > 0) minStrength = theNet(i).weight();
    }
    ++rounds;
    std::cerr << rounds << "\t" << minStrength << "\t" << numberOfEdges(theNet)  << "\n";
  } while (minStrength < s && minStrength > 0);  // do this as long as small degree nodes remain
}
// <-------- find_s_core ------------




#endif //~ NETEXTRAS2_H 
