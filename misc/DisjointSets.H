#include<cassert>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <deque>

/**
 * Data structures for disjoint sets forests. See the DisjointSets class for documentation.
 * 
 * This code is based on the older KruskalTree class, and that implementation is improved here.
 */




/**
 * Policy for NOT keeping track of lengths of the sets.
 */
template<typename IndexType>
class NoLengths{
protected:
  inline void updateLengths(IndexType set1, IndexType set2){}; 
  NoLengths(IndexType size){};
  ~NoLengths(){};
};



/**
 * Policy for keeping track of lengths of the sets.
 * 
 * Template parameters:
 *
 * IndexType    The integral type of node indices. Use the same values as 
 *              for DisjointSetsForest.
 * ValueType    The type of the values that are used to calculate lengths.
 * lenDist      Whether we are updating the length distribution of
 *              the sets while building it. This should not affect
 *              the time/space scaling of the operations that change
 *              the data structure, but makes queries about the length
 *              distribution much faster.
 */
template<typename IndexType,typename ValueType,bool lenDist=true>
class TrackLengths{
protected:
  ValueType * minValues;
  ValueType * maxValues;
  
public:
  std::map<ValueType,IndexType> * lenDistribution;

  TrackLengths(IndexType size){
    minValues = new ValueType[size];
    maxValues = new ValueType[size];
    if (lenDist){
      lenDistribution=new std::map<ValueType,IndexType>;
      (*lenDistribution)[0]=size;
    }
  };

  ~TrackLengths(){
    delete[] minValues;
    delete[] maxValues;
    if (lenDist){
      delete lenDistribution;
    }
  };

  void updateLengths(IndexType set_small, IndexType set_large){
    ValueType set_small_len=maxValues[set_small]-minValues[set_small];
    ValueType set_large_len=maxValues[set_large]-minValues[set_large];

    // Updating the min and max values of the large set.
    // Note that the small set is not updated.
    if (minValues[set_large] > minValues[set_small]){
      minValues[set_large]=minValues[set_small];
    }
    if (maxValues[set_large] < maxValues[set_small]){
      maxValues[set_large]=maxValues[set_small];
    }

    if (lenDist){
      (*lenDistribution)[set_small_len]--;
      (*lenDistribution)[set_large_len]--;
      
      if ((*lenDistribution)[set_small_len]==0){
	lenDistribution->erase(set_small_len);
      }
      if (set_small_len!=set_large_len){
	//If sets are equal size, "larger" set size is already removed.
	if ((*lenDistribution)[set_large_len]==0){
	  lenDistribution->erase(set_large_len);
	}
      }
      
      ++((*lenDistribution)[maxValues[set_large] - minValues[set_large]]); // default value for getter should be 0
    }
  };
  
  ValueType getLen(IndexType set){
    return maxValues[set]-minValues[set];
  }

  void printLenDist(){
    for (typename std::map<ValueType,IndexType>::const_iterator i = lenDistribution->begin();i!=lenDistribution->end();i++) {
      std::cout << i->first << " " << i->second << std::endl;
    }    
  }
  
  void printFileLenDist(std::string file){//print in a file the length distribution
    std::ofstream fileLenDist ((char*)file.c_str());
    for (typename std::map<ValueType,IndexType>::const_iterator i = lenDistribution->begin();i!=lenDistribution->end();i++) {
      fileLenDist << i->first << " " << i->second << std::endl;
    }
    fileLenDist.close();
  }
  
  float getGiantLen(){//get the giant length from the length distribution
    float giant=0;
    for (typename std::map<ValueType,IndexType>::const_iterator i = lenDistribution->begin();i!=lenDistribution->end();i++) {
      if(i->first > giant){
        giant = i->first;
      }
    }
    return giant;
  }
  
  float getScdGiantLen(float giant){//get the giant size from the size distribution
    float scdGiant=0;
    for (typename std::map<ValueType,IndexType>::const_iterator i = lenDistribution->begin();i!=lenDistribution->end();i++) {
      if(i->first < giant and i->first > scdGiant){
        scdGiant = i->first;
      }
    }
    return scdGiant; 
  }
  
  float getSuscLen(int giant){//compute the susceptibility
    float s_up=0;
    float s_down=0;
    for (typename std::map<ValueType,IndexType>::const_iterator i = lenDistribution->begin();i!=lenDistribution->end();i++) {
      s_down += (i->first)*(i->second);
      s_up += (i->first)*(i->first)*(i->second);
    }
    
    return (s_up - (giant*giant))/(s_down - giant);
  }

  void setValue(IndexType set, ValueType value){
    minValues[set]=value;
    maxValues[set]=value;
  };

};



/**
 * Implemenation of disjoint sets forest data structure. This data structure
 * is use to represent disjoint sets, i.e., sets where each element 
 * is in exactly one set. The operations for mergin sets and asking if
 * two elements are in the same set are fast. 
 *
 * Reference: Cormen et al. "Introduction to Algorithms", Section 22.3.
 *
 * Template parameters:
 *
 * IndexType    The integral type of node indices
 * packTree     Whether to link the nodes found on the 
 *              path to the root node of a cluster tree
 *              directly to the root. Breaks the shape of the tree 
 *              AND the node counts of subtrees, but speeds up the 
 *              futher operations considerably
 * sizeDist     Whether we are updating the size distribution of
 *              the sets while building it. This should not affect
 *              the time/space scaling of the operations that change
 *              the data structure, but makes queries about the size
 *              distribution much faster.
 * LengthPolicy The policy class for dealing with "lenghts" of the 
 *              sets. If there are no lenghts then choose NoLenghts
 *              (default), otherwise choose TrackLengths.
 */

template<typename IndexType=unsigned, bool packTree=true,bool sizeDist=true, typename LengthPolicy = NoLengths<IndexType> >
class DisjointSetsForest : public LengthPolicy {
protected:
  IndexType * fathers;
  IndexType * setSizes; /* Why to use the same type here? */
  //It would be faster to have a single array with all the data for each element instead of having multiple arrays.
  

  DisjointSetsForest() {}; /* Not allowed to be called: 
		     * only to be initialized to a specified size */ 

  // NOTE THAT THERE IS NO COPY CONSTRUCTOR!

  IndexType size;

public:
  std::map<IndexType,IndexType> * sizeDistribution;

  /** 
   * Constructor for the disjoint sets forest. The total number of elements, n, in all sets
   * is given as a parameter and the elements are numbered from 0 to n-1. In the beginning
   * each element is in its own set (i.e., there are n sets with only a single element).
   */
  DisjointSetsForest(IndexType sizeOfTheForest) : LengthPolicy(sizeOfTheForest) {
    size=sizeOfTheForest;
    fathers=new IndexType[size];
    setSizes=new IndexType[size];

    for (IndexType i=0; i<size; i++) {
      fathers[i]=i; /* Every node is its own cluster */
      setSizes[i]=1;
    }


    if (sizeDist){
      sizeDistribution=new std::map<IndexType,IndexType>;
      (*sizeDistribution)[1]=size;
    }

  }

  ~DisjointSetsForest() {
    delete[] fathers;
    delete[] setSizes;

    if (sizeDist){
      delete sizeDistribution;
    }

  }

  void printForest(){
    for (IndexType i=0; i<size; i++) {
      std::cout << fathers[i] << " ";
    }    
    std::cout << std::endl;
  }
  
  IndexType getForestSize(){
    return size;
  }
  
  IndexType getFather(IndexType i){
    return fathers[i];
  }
  
  void getFathersList(deque<IndexType>& fathersvect){
    for(IndexType i=0; i<size; i++){
      if(find(fathersvect.begin(), fathersvect.end(), fathers[i]) != fathersvect.end()){
        continue;
      }
      else{fathersvect.push_back(fathers[i]);}
    }
  }

  
  void printSizeDist(){
    for (typename std::map<IndexType,IndexType>::const_iterator i = sizeDistribution->begin();i!=sizeDistribution->end();i++) {
      std::cout << i->first << " " << i->second << std::endl;
    }    
  }
  
  void printFileSizeDist(std::string file){//print in a file the size distribution
    std::ofstream fileSizeDist ((char*)file.c_str());
    for (typename std::map<IndexType,IndexType>::const_iterator i = sizeDistribution->begin();i!=sizeDistribution->end();i++) {
      fileSizeDist << i->first << " " << i->second << std::endl;
    }
    fileSizeDist.close();  
  }
  
  int getGiantSize(){//get the giant size from the size distribution
    int giant=0;
    for (typename std::map<IndexType,IndexType>::const_iterator i = sizeDistribution->begin();i!=sizeDistribution->end();i++) {
      if(i->first > giant){
        giant = i->first;
      }
    }
    return giant; 
  }
  
  int getScdGiantSize(int giant){//get the giant size from the size distribution
    int scdGiant=0;
    for (typename std::map<IndexType,IndexType>::const_iterator i = sizeDistribution->begin();i!=sizeDistribution->end();i++) {
      if(i->first < giant and i->first > scdGiant){
        scdGiant = i->first;
      }
    }
    return scdGiant; 
  }
  
  float getSuscSize(int giant){//compute the susceptibility
    
    float s_up=0;
    float s_down=0;
    for (typename std::map<IndexType,IndexType>::const_iterator i = sizeDistribution->begin();i!=sizeDistribution->end();i++) {
      s_down += (i->first)*(i->second);
      s_up += (i->first)*(i->first)*(i->second);
    }
    
    return (s_up - (giant*giant))/(s_down - giant);
  }



  /* The basic, recursive method for traversing a tree toward the 
   * root. The packing operation becomes very elegant with the 
   * recursive implementation. If packing is not to be used,
   * the compiler should be able to eliminate the recursion 
   * as a tail recursion. 
   *
   * Params: 
   * 
   * nodeIndex   the node...
   * returns     ...and where it belongs to. */
  IndexType getSetID(const IndexType elementIndex) const {
    assert((elementIndex >= 0) && (elementIndex < size));
    if (fathers[elementIndex] == elementIndex) { /* The root of a subtree */
      return elementIndex;
    } else { /* Recurse up the tree */
      IndexType theRoot=getSetID(fathers[elementIndex]);
      if (packTree) { /* The compiler eliminates this clause based on
		       * the template parameter. */
	fathers[elementIndex]=theRoot; /* A direct link to the root */
      }
      return theRoot;
    }
  }

  /* Merge sets (if necessary). 
   *
   * If sets (trees) are joined, the one corresponding to the
   * large set becomes the "root", that is, the set ID
   * of that will be the set ID of the resulting set.
   *
   * Params: 
   *
   * set1,set2    The nodes associated with the edge 
   * returns      Whether a new cluster was formed */

  bool mergeSets(const IndexType element1, const IndexType element2) {
    assert((element1 >= 0) && (element1 < size));
    assert((element2 >= 0) && (element2 < size));
    
    IndexType set1=getSetID(element1);
    IndexType set2=getSetID(element2);

    assert((set1 >= 0) && (set1 < size));
    assert((set2 >= 0) && (set2 < size));

    if (set1==set2) {
      return false;
    } else {

      // Find out the which set is bigger
      IndexType smallSet,largeSet;
      if (setSizes[set1]>setSizes[set2]){
	smallSet=set2;
	largeSet=set1;
      }else{
	smallSet=set1;
	largeSet=set2;
      }

      //Update the distributions
      if (sizeDist){ //Should be optimized away by the compiler no size dists.   
	IndexType smallSetSize=setSizes[smallSet];
	IndexType largeSetSize=setSizes[largeSet];

	(*sizeDistribution)[smallSetSize] --;
	(*sizeDistribution)[largeSetSize]--;

	
	//we remove empty elements
	if ((*sizeDistribution)[smallSetSize]==0){
	  sizeDistribution->erase(smallSetSize);
	}
	if (smallSetSize!=largeSetSize){
	  //If sets are equal size, "larger" set size is already removed.
	  if ((*sizeDistribution)[largeSetSize]==0){
	    sizeDistribution->erase(largeSetSize);
	  }
	}

	++((*sizeDistribution)[smallSetSize+largeSetSize]); // default value for getter should be 0
	
      }
      
      // Update the data structures
      fathers[smallSet]=largeSet; /* Linking the small set to the large set */
      setSizes[largeSet]+=setSizes[smallSet];

      //This updates the lengths of the sets
      //Should be optimized away if NoLenghts is used?
      this->updateLengths(smallSet, largeSet);

      return true;
    }

  }



  /**
   * Returns the size of the cluster the nodei in question belongs to
   * 
   * nodeIndex   the node...
   * returns     ...and the size of the cluster it belongs to.
   */

  IndexType getSetSize(const IndexType elementIndex) const {
    /* Assertion included below */
    return setSizes[getSetID(elementIndex)];
  }
};

 


