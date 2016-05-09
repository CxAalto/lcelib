#include <iostream>
#include "../DisjointSets.H"

using namespace std;


template<typename T>
void printTree(T & ds){
  std::cout << "Tree:" << std::endl;
  ds.printForest();
}

template<typename T>
void printTreeSize(T & ds){
  std::cout << "Tree:" << std::endl;
  ds.printForest();
  std::cout << "Size dist:" << std::endl;
  ds.printSizeDist();
}

template<typename T>
void printTreeSizeLen(T & ds){
  std::cout << "Tree:" << std::endl;
  ds.printForest();
  std::cout << "Size dist:" << std::endl;
  ds.printSizeDist();
  std::cout << "Len dist:" << std::endl;
  ds.printLenDist();
}


template<typename T>
void testMerge(T & ds, void(*pf)(T&)){
  std::cout << "Initial state." << std::endl;
  (*pf)(ds);
  std::cout << "Merging 0 and 1." << std::endl;
  ds.mergeSets(0,1);
  (*pf)(ds);
  std::cout << "Merging 1 and 2." << std::endl;
  ds.mergeSets(1,2);
  (*pf)(ds);
  std::cout << "Merging 3 and 4." << std::endl;
  ds.mergeSets(3,4);
  (*pf)(ds);
  std::cout << "Merging 0 and 4." << std::endl;
  ds.mergeSets(0,4);
  (*pf)(ds);
  std::cout << "Merging 5 and 6." << std::endl;
  ds.mergeSets(5,6);
  (*pf)(ds);
}

int main(void){
  std::cout << "Creating tree with 8 elements." << std::endl;
  DisjointSetsForest<unsigned,true,false> ds1(8);  
  testMerge(ds1,&printTree);
  std::cout << std::endl;

  std::cout << "Creating tree with 8 elements, and size distribution." << std::endl;
  DisjointSetsForest<unsigned,true,true > ds_size(8);  
  testMerge(ds_size,&printTreeSize);
  std::cout << std::endl;

  std::cout << "Creating tree with 8 elements, and size distribution, and length dist." << std::endl;
  DisjointSetsForest<unsigned,true,true,TrackLengths<unsigned,float,true> > ds_sizelen(8);  
  ds_sizelen.setValue(0,0.1);
  ds_sizelen.setValue(1,1.0);
  ds_sizelen.setValue(2,2.0);
  ds_sizelen.setValue(3,3.0);
  ds_sizelen.setValue(4,4.0);
  ds_sizelen.setValue(5,5.0);
  ds_sizelen.setValue(6,6.0);
  ds_sizelen.setValue(7,7.0);
  ds_sizelen.setValue(8,8.0);
  testMerge(ds_sizelen,&printTreeSizeLen);
}
