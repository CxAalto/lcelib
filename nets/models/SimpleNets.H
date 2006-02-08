
// lcelib/nets/models/SimpleNets.H
// The simplest of networks: clique, chain, ring, etc.

// Dec 2006
// Authors: Riitta Toivonen




/*

 Functions included 
 ------------------  

 Clique                 (added Jan 4 2006, Riitta) 
 Chain                  (added Jan 4 2006, Riitta) 
 Ring                   (added Jan 4 2006, Riitta) 
 Star                   (added Feb 8 2006, Riitta)  
Feel free to add whatever useful bits you have that belong to this category.
A reasonable amount of testing is expected.

Suggestions, corrections etc. also welcomed. 

Usage examples for nontrivial functions should be found in 
lcelib/nets/Examples/

 */ 








template<typename NetType>
void Clique(NetType& net) {
#ifdef DEBUG
  std::cerr << "Forming fully connected network. Links: \n";
#endif // DEBUG 
  for (size_t i=0; i<net.size(); ++i) {
    for (size_t j=0; j<i; ++j) {
      net[i][j]=1;
#ifdef DEBUG
      std::cerr << "\t" << i << " to " << j << "\n";
#endif // DEBUG 
    }
  }
}
// <--- Clique                                                      
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -           





  template<typename NetType> 
  void Star(NetType& net) {
#ifdef DEBUG
    std::cerr << "Forming star network. Links: \n";
#endif // DEBUG 
    for (size_t i=1; i<net.size(); ++i) {
      net[0][i]=1;
#ifdef DEBUG
      std::cerr << "\t" << 0 << " to " << i << "\n";
#endif // DEBUG 
    }
  }

// <--- Star
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 






template<typename NetType>
void Chain(NetType& net) {
#ifdef DEBUG
  std::cerr << "Forming chain network. Links: \n";
#endif // DEBUG 
  for (size_t i=0; i<net.size()-1; ++i) {
    net[i][i+1]=1;
#ifdef DEBUG 
    std::cerr << "Linked " << "\t" << i << " to " << i+1 << "\n";
#endif // DEBUG 
  }
}
// <--- Chain                                                    
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -           




template<typename NetType>
void Ring(NetType& net) {
#ifdef DEBUG
  std::cerr << "Forming ring network. Links: \n";
#endif // DEBUG 
  for (size_t i=0; i<net.size()-1; ++i) {
    net[i][i+1]=1;
#ifdef DEBUG 
    std::cerr << "Linked " << "\t" << i << " to " << i+1 << "\n";
#endif // DEBUG 
  }
  net[net.size()-1][0]=1; // the edge that will make the loop
#ifdef DEBUG
  std::cerr << "Linked " << "\t" << net.size()-1 << " to " << 0 << "\n";
#endif // DEBUG 
}
// <--- Ring 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  

