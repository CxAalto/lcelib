
//  ./socmodel4 1000 25000 0.03 0.3 2 testiverkko 5

/*
Weighted community network (LCE2)

Parameters:

netSize        - network size (num nodes)
maxRounds      - how many simulation rounds, use 25000 if don't know otherwise
p_tri          - triangle probability, this is used to tune the average degree after delta has been fixed
delta          - increment from initial weight 1; usually values between 0 and 1 
netRoundsLimit - how many networks to generate
namebase       - folder path and the base of the network name to be written out
counterOffset  - normally the program generates network names from zero, but this value can be used to change it


The code generates the network and outputs the full network and the largest component in the files
   namebase_full.edg     namebase_largest.edg  

Note: node indices in the file  namebase_largest.edg may run from 0 to N-1 
(or counterOffset to N+counterOffset-1)  even if the network contains fewer than N nodes. 


To compile: 
  g++ -O socmodel4.cpp -o lce2

To run: 
  ./lce2 netsize maxRounds p_tri delta netRoundsLimit namebase offset

Example: 
 mkdir testnets;  ./lce2  100 1000 0.3 0.5 3 testnets/net  0
  
Author: Jussi Kumpula
*/


#define NDEBUG // to turn assertions off
#define PI 3.14159265
//#include "lcelib/nets/Models.H" 

#include <vector>
#include "../../../../lcelib/nets/NetExtras.H"
#include "../../../../lcelib/nets/models/SeedNet.H"
#include "../../../../lcelib/Containers.H"
#include "../../../../lcelib/Nets.H" 
#include "../../../../lcelib/Randgens.H"
#include "../../../../lcelib/misc/KruskalTree2.H"
// #include <math.h>

//#include "../deviations/apuja.H"
//#include "socmodel.H"
// #include "socmodel3.cpp"

//#include "../lcelib/nets/Randomizer.H"


typedef float EdgeData;
//typedef  SymmNet<EdgeData, ValueTable, ExplSumTreeTable> NetType;
typedef  SymmNet<EdgeData> NetType;
typedef  Set<size_t> nodeSet;


template <typename Generator>
float normRandNumber(const float mean, const float std, Generator & generator) {
  float u1, u2, r;
  u1 = generator.next(1.0);
  u2 = generator.next(1.0);

  r = mean + std*( sqrt(-2.0*log(u2))*cos(2.0*PI*u1) );
  return r;
}

template <typename NetType>
float calculateAveDegree(NetType & theNet){
  
  const size_t netSize=theNet.size();
  size_t degreeSum = 0;

  // Go through each node in the network: 
  for (size_t i=0; i<netSize; ++i) {
    degreeSum += theNet(i).size();
  }
  
  return degreeSum/(float) netSize;
}






// makes a fast random permutation of the numbers in the array
template <typename Generator>
void fastSuffle(size_t deck[], const size_t N, Generator & generator) {

  size_t temp, randIndex, last;
  for (last = N; last > 1; --last) {
    randIndex = generator.next(last); // uniform random integer from 0 to N-1
    if (randIndex >= last) std::cerr << "väärä indeksi!\n";
    temp = deck[randIndex];
    deck[randIndex] = deck[last-1];
    deck[last-1] = temp;
  }
}



// takes a random step weighed with the link weights excluding the previous node
template <typename NetType, typename Generator>
size_t weighedStepExcludingOld(NetType & net, const size_t currentNode, const size_t prevNode, Generator & generator) {
  
  const size_t degree = net(currentNode).size();
  if ( degree < 2 ) std::cerr << "degreetä ei riittävästi\n";
  float weights[degree-1]; // the weight to prevNode is not stored
  size_t indexes[degree-1]; 
  size_t counter = 0;
  for (typename NetType::const_edge_iterator j=net(currentNode).begin(); !j.finished(); ++j) {
    if ( *j != prevNode) {
      weights[counter] = j.value();
      indexes[counter] = *j;
      ++counter;
    }
  }

  if (counter != degree - 1) std::cerr << "painotetussa valinnassa häikkää\n";

  float wSum = 0;
  for (size_t i = 0; i < degree-1; ++i) wSum += weights[i];
  float cumWSum[degree-1];
  cumWSum[0] = weights[0];
  for (size_t i = 1; i < degree-1; ++i) cumWSum[i] = cumWSum[i-1] + weights[i];
  float p = generator.next(wSum); // float between 0 and wSum

  size_t luckyIndex = 0;
  while ( cumWSum[luckyIndex] < p ) ++luckyIndex;
  return indexes[luckyIndex];
}



// takes a random step weighed with the link weights
template <typename NetType, typename Generator>
size_t weighedStep(NetType & net, const size_t currentNode, Generator & generator) {
  
  const size_t degree = net(currentNode).size();
  if ( degree < 1 ) std::cerr << "degreetä ei riittävästi\n";
  float weights[degree];
  size_t indexes[degree]; 
  size_t counter = 0;
  for (typename NetType::const_edge_iterator j=net(currentNode).begin(); !j.finished(); ++j) {
      weights[counter] = j.value();
      indexes[counter] = *j;
      ++counter;
  }

  if (counter != degree) std::cerr << "painotetussa valinnassa häikkää\n";

  float wSum = 0;
  for (size_t i = 0; i < degree; ++i) wSum += weights[i];
  float cumWSum[degree];
  cumWSum[0] = weights[0];
  for (size_t i = 1; i < degree; ++i) cumWSum[i] = cumWSum[i-1] + weights[i];
  float p = generator.next(wSum); // float between 0 and wSum

  size_t luckyIndex = 0;
  while ( cumWSum[luckyIndex] < p ) ++luckyIndex;
  return indexes[luckyIndex];
}





void   printParameters(size_t netSize, size_t maxRounds, float p_jump, float p_walk, float p_tri, float p_death, 
		       float delta,size_t  stepLimit, size_t netRoundsLimit, int randSeed) {

  std::cout << "netSize=" <<  netSize << ";\n";
  std::cout << "maxRounds=" <<  maxRounds << ";\n";
  std::cout << "p_jump=" <<  p_jump << ";\n";
  std::cout << "p_walk=" <<  p_walk << ";\n";
  std::cout << "p_tri=" <<  p_tri << ";\n";
  std::cout << "p_death=" <<  p_death << ";\n";
  std::cout << "delta=" <<  delta << ";\n";
  std::cout << "stepLimit=" <<  stepLimit << ";\n";
  std::cout << "netRoundsLimit=" <<  netRoundsLimit << ";\n";
  std::cout << "randSeed=" <<  randSeed << ";\n";

}




// runs the social network model according to parameters below
template <typename NetType, typename Generator>
void socModel7(NetType & net, Generator & generator, 
	       const float p_jump, const float p_walk, const float p_tri, const float p_death,
	       const float delta,  const size_t stepLimit, const size_t maxRounds) {
  
  const size_t netSize = net.size();
  const float w0 = 1.0;
  
 
  NetType net2(netSize); // initialize empty network
   
  size_t currentNode, nextNode, secondNode, randNode;
  for ( size_t rounds=0; rounds < maxRounds; ++rounds) {       // run the simulation over many rounds (time steps)
    
    // net2 should be empty always when starting a new round...
   
    for ( size_t i = 0; i < netSize; ++i) {                    // loop over all nodes
      currentNode = i;

      if ( generator.next(1.0) < p_jump || net(currentNode).size() == 0 ) {                     // choose random node
		do {
		  randNode = generator.next(netSize);
		} while ( randNode == currentNode );    
      if ( net(currentNode)[randNode] == 0 ) 
		  net2[currentNode][randNode] = w0; // connect in net2 if not already connected
      }
	
      // perform walk from current node if possible
      if ( net(currentNode).size() > 0 ) {    // this is not always true as the previous link is not yet efective     
		if ( generator.next(1.0) < p_walk ) { // decide whether to do the walk or not
	  
		nextNode = weighedStep(net, currentNode, generator);
        if ( delta > 0 )
              net2[currentNode][nextNode] = net2[currentNode][nextNode] + delta; // increase weight in net2
	  
    	if ( net(nextNode).size() > 1 ) {                                // if possible, take step avoiding old node
		    secondNode = weighedStepExcludingOld(net, nextNode, currentNode, generator);
            if ( delta > 0 )
                net2[nextNode][secondNode] = net2[nextNode][secondNode] + delta;   // increase weight in net2
	    
	    // second neighbor has been found
	    if (secondNode != currentNode) {     // this should be always true... 
	      if ( net(currentNode)[secondNode] == 0 ) {     // there is not link, i.e., we are at distance 2
			if (  generator.next(1.0) < p_tri ) net2[currentNode][secondNode] = w0;
	      }
	      else {
                if ( delta > 0 )
                        net2[currentNode][secondNode] = net2[currentNode][secondNode] + delta;
		      }
		    }
		  }
		}
      }
    }
    // end of node loop
      
    // copy changes from net2 to net
    for ( size_t i = 0; i < netSize; ++i) {
       for (typename NetType::edge_iterator neigh=net2[i].begin(); !neigh.finished(); ++neigh) {
          if ( *neigh > i ) { // take each link only once
                net[i][*neigh] = net[i][*neigh] + neigh.value(); // add changes
                neigh.value() = 0; // remove edge so that net2 will eventually be empty
          }
       }
    }
      
    // remove nodes  
    for ( size_t i = 0; i < netSize; ++i) { 
      if ( generator.next(1.0) < p_death ) { // delete this node
		  for (typename NetType::edge_iterator neigh=net[i].begin(); !neigh.finished(); ++neigh) {
		    neigh.value() = 0;
		  }
      }
    }
   
    if ( rounds%1000 == 0 ) {
      std::cerr << rounds << "\t" <<  calculateAveDegree(net) << "\n";
    }
    
  } // end of rounds loop 
}







template<typename NetType>
NetType * findLargestComponent(NetType & net)
{
  
  // find the largest component
  const size_t netSize = net.size();
  KruskalTree<true> kpuu(netSize);
  for (size_t source=0; source<netSize; ++source) {
    for (typename NetType::const_edge_iterator target=net(source).begin(); !target.finished(); ++target) {
      if ( *target > source) 	kpuu.addEdge(source,*target);
    }
  }
  // std::cerr << kpuu.getClusterID(10) << "\t" <<  kpuu.getClusterSize(10) << "\n";
  size_t giantSize = 0;
  size_t currentClusterSize;
  size_t maxClusterIndex = 0;
  size_t node = 0;
  while (giantSize < netSize/2 && node < netSize) {
    currentClusterSize = kpuu.getClusterSize( node );
    if ( currentClusterSize > giantSize ) {
      maxClusterIndex = kpuu.getClusterID( node );
      giantSize = currentClusterSize;
    }
    ++node;
  }
  if ( giantSize <= netSize/2 ) std::cout << "Largest component less than half of the whole network!\n";
  else std::cerr << "Largest component size is: " << giantSize << "\n";
  
  // Construct the net.
  std::auto_ptr<NetType> netPointer(new NetType(giantSize));
  NetType& net2 = *netPointer;  // Create a reference for easier access.

  size_t newIndexes[netSize];
  size_t counter=0;
  // 
  for (size_t source=0; source<netSize; ++source) {
    if ( kpuu.getClusterID(source) == maxClusterIndex ) {
      newIndexes[source] = counter;                        // rename nodes which belong to giant component
      counter++;
    }
  }
  
  // counter should be now giantSize;
  if (counter != giantSize) std::cerr << "Something wrong with largest component!\n";
  for (size_t source=0; source<netSize; ++source) {
    if ( kpuu.getClusterID(source) == maxClusterIndex ) {
      for (typename NetType::const_edge_iterator target=net(source).begin(); !target.finished(); ++target) {
	if ( kpuu.getClusterID(*target) == maxClusterIndex ) {
	  net2[newIndexes[source]][newIndexes[*target]] = net(source)[*target]; // these nodes belong to same cluster
	}
      }
    }
  }


  return netPointer.release(); // release the pointer so that it is not destroyed

}










int main(int argc, char* argv[]) {


  std::string argInfo =  "netSize        - network size (num nodes)\n maxRounds      - how many simulation rounds, use 25000 if don't know otherwise \n p_tri          - triangle probability, this is used to tune the average degree after delta has been fixed \n delta          - increment from initial weight 1; usually values between 0 and 1  \n netRoundsLimit - how many networks to generate \n namebase       - folder path and the base of the network name to be written out \n counterOffset  - normally the program generates network names from zero, but this value can be used to change it \n\nThe code outputs the full network and the largest component in the files \n     namebase_full.edg     namebase_largest.edg  \n\nNote: node indices in the file  namebase_largest.edg may run from 0 to N-1 \n(or counterOffset to N+counterOffset-1) even if the network contains fewer than N nodes.\n\n";


  if ( (size_t) argc < (1+1)) { std::cerr << "\nPlease give arguments: \n " << argInfo << "\\n"; exit(1);}
  const size_t netSize = atoi(argv[1]); // network size (num nodes)
  const size_t maxRounds = atoi(argv[2]); // how many simulation rounds, use 25000 if don't know otherwise
  const float p_jump = 0.0005;  // amount of random links, 0.0005 is usual but also 0.00025 is fine
  const float p_walk = 1;   // do not change this!
  const float p_tri = atof(argv[3]);  // triangle probability, this is used to tune the average degree after delta has been fixed
  const float p_death = 0.001; // do not change this!
  const float delta = atof(argv[4]);   // values between 0 and 1 usual
  const size_t netRoundsLimit = atoi(argv[5]);  // how many networks to generate
  size_t counterOffset = 0;    
  if ( argc > 7 ) counterOffset = atoi(argv[7]);    // normally the program generates network names from zero, but this value can be used to change it

  int randseed=time(0); // some random number 
  printParameters(netSize, maxRounds, p_jump, p_walk, p_tri, p_death, delta, 0, netRoundsLimit, randseed);

  RandNumGen<> generator(randseed);






  // loop over several realisations of the network
  for (size_t netRounds = 0; netRounds < netRoundsLimit; netRounds++) {
    
    NetType net(netSize);  // initialize some nodes in the network
    
    // these are needed for non-exponential life times, socmodel8
    float deathTimes[netSize];
    float meanLifeTime = 1.0/p_death;
    float stdLifeTime = 1.0/p_death;
    for (size_t i=0; i < netSize; ++i) {
	 	deathTimes[i] = meanLifeTime*generator.next(1.0); // init from uniform distribution
    }
    
    // run the model
    socModel7(net, generator, p_jump, p_walk, p_tri, p_death, delta, 0, maxRounds);

    std::auto_ptr<NetType> netPointer(findLargestComponent<NetType>(net)); 
    NetType& net2 = *netPointer;  // Create a reference for easier handling of net.


    char buffer[50];
    sprintf(buffer,"%s%d_full.edg",argv[6], netRounds+counterOffset);
    
    //      std::ofstream myFile(argv[8] + str);
    std::ofstream myFile(buffer);
    
    if (! myFile) {
      std::cerr << "Error opening output file\n";
      exit(-1);
    }
    myFile << "HEAD\tTAIL\tWEIGHT\n";
    outputEdgesAndWeights2(net, 0, myFile);  // whole network
    myFile.close();


    sprintf(buffer,"%s%d_largest.edg",argv[6], netRounds+counterOffset);
    std::ofstream myFile2(buffer);
    if (! myFile2) {
      std::cerr << "Error opening output file\n";
      exit(-1);
    }
    myFile2 << "HEAD\tTAIL\tWEIGHT\n";
    outputEdgesAndWeights2(net2, 0, myFile2);  // largest component
    myFile2.close();


  }  // time to build a new network



}


















/****************************************

REMOVED CODE


*****************************************/




// life times of nodes are normally distributed
/*
template <typename NetType, typename Generator>
void socModel8(NetType & net, Generator & generator, 
	       const float p_jump, const float p_walk, const float p_tri, const float p_death,
	       const float delta,  const size_t stepLimit, const size_t maxRounds, 
	       float deathTimes[], const float meanLifeTime, const float stdLife ) {
  
  const size_t netSize = net.size();
  const float w0 = 1.0;
  
 
  NetType net2(netSize); // initialize empty network
   
  size_t currentNode, nextNode, secondNode, randNode;
  for ( size_t rounds=0; rounds < maxRounds; ++rounds) {       // run the simulation over many rounds (time steps)
    
    // net2 should be empty always when starting a new round...
   
    for ( size_t i = 0; i < netSize; ++i) {                    // loop over all nodes
      currentNode = i;

      if ( generator.next(1.0) < p_jump || net(currentNode).size() == 0 ) {                     // choose random node
		do {
		  randNode = generator.next(netSize);
		} while ( randNode == currentNode );    
      if ( net(currentNode)[randNode] == 0 ) 
		  net2[currentNode][randNode] = w0; // connect in net2 if not already connected
      }
	
      // perform walk from current node if possible
      if ( net(currentNode).size() > 0 ) {    // this is not always true as the previous link is not yet efective     
		if ( generator.next(1.0) < p_walk ) { // decide whether to do the walk or not
	  
		nextNode = weighedStep(net, currentNode, generator);
        if ( delta > 0 )
              net2[currentNode][nextNode] = net2[currentNode][nextNode] + delta; // increase weight in net2
	  
    	if ( net(nextNode).size() > 1 ) {                                // if possible, take step avoiding old node
		    secondNode = weighedStepExcludingOld(net, nextNode, currentNode, generator);
            if ( delta > 0 )
                net2[nextNode][secondNode] = net2[nextNode][secondNode] + delta;   // increase weight in net2
	    
	    // second neighbor has been found
	    if (secondNode != currentNode) {     // this should be always true... 
	      if ( net(currentNode)[secondNode] == 0 ) {     // there is not link, i.e., we are at distance 2
			if (  generator.next(1.0) < p_tri ) net2[currentNode][secondNode] = w0;
	      }
	      else {
                if ( delta > 0 )
                        net2[currentNode][secondNode] = net2[currentNode][secondNode] + delta;
		      }
		    }
		  }
		}
      }
    }
    // end of node loop
      
    // copy changes from net2 to net
    for ( size_t i = 0; i < netSize; ++i) {
       for (typename NetType::edge_iterator neigh=net2[i].begin(); !neigh.finished(); ++neigh) {
          if ( *neigh > i ) { // take each link only once
                net[i][*neigh] = net[i][*neigh] + neigh.value(); // add changes
                neigh.value() = 0; // remove edge so that net2 will eventually be empty
          }
       }
    }
      
    // remove nodes  
    for ( size_t i = 0; i < netSize; ++i) { 
      if ( rounds > deathTimes[i] ) { // if time is larger than this node death time
	      do {
	      	deathTimes[i] = rounds + normRandNumber(meanLifeTime, stdLife, generator);
      	  } while ( deathTimes[i] < rounds ); 
		  for (typename NetType::edge_iterator neigh=net[i].begin(); !neigh.finished(); ++neigh) {
		    neigh.value() = 0;
		  }
      }
    }
   
    if ( rounds%1000 == 0 ) {
      std::cerr << rounds << "\t" <<  calculateAveDegree(net) << "\n";
    }
    
  } // end of rounds loop 
}
*/










// runs the social network model according to parameters below
/*
template <typename NetType, typename Generator>
void socModel2(NetType & net, Generator & generator, 
	       const float p_jump, const float p_walk, const float p_tri, const float p_death,
	       const float delta,  const size_t stepLimit, const size_t maxRounds ) {
  
  const size_t netSize = net.size();
  const float w0 = 1.0;
  
  
  size_t indexArray[netSize];
  for (size_t i = 0; i < netSize; ++i) indexArray[i] = i;      // generate linear ordering

  size_t currentNode, nextNode, secondNode, randNode;
  for ( size_t rounds=0; rounds < maxRounds; ++rounds) {       // run the simulation over many rounds (time steps)

    fastSuffle(indexArray, netSize, generator);                // take the nodes in random order at each time step
    
    for ( size_t i = 0; i < netSize; ++i) {                    // loop over all nodes
      currentNode = indexArray[i];

      if ( generator.next(1.0) < p_jump || net(currentNode).size() == 0 ) {                     // choose random node
	do {
	  randNode = generator.next(netSize);
	} while ( randNode == currentNode );    
	if ( net(currentNode)[randNode] == 0 ) 
	  net[currentNode][randNode] = w0; // connect if not already connected
      }
	
      // perform walk from current node if possible
      if ( net(currentNode).size() > 0 ) {    // this should be always true...         
	
	if ( generator.next(1.0) < p_walk ) { // decide whether to do the walk or not
	  
	  nextNode = weighedStep(net, currentNode, generator);
	  net[currentNode][nextNode] = net[currentNode][nextNode] + delta; // increase weight
	  
	  if ( net(nextNode).size() > 1 ) {                                // if possible, take step avoiding old node
	    secondNode = weighedStepExcludingOld(net, nextNode, currentNode, generator);
	    net[nextNode][secondNode] = net[nextNode][secondNode] + delta;   // increase weight
	    
	    // second neighbor has been found
	    if (secondNode != currentNode) {     // this should be always true... 

	      if ( net(currentNode)[secondNode] == 0 ) {     // there is not link, i.e., we are at distance 2
		if (  generator.next(1.0) < p_tri ) net[currentNode][secondNode] = w0;
	      }
	      else {
		net[currentNode][secondNode] = net[currentNode][secondNode] + delta;
	      }
	    }
	  }

	}

      }
    }
    // end of node loop
      
    for ( size_t i = 0; i < netSize; ++i) { 
      if ( generator.next(1.0) < p_death) {
	for (typename NetType::edge_iterator neigh=net[i].begin(); !neigh.finished(); ++neigh) {
	  neigh.value() = 0;
	}
      }
    }

    
    if ( rounds%1000 == 0 ) {
      std::cerr << rounds << "\t" <<  calculateAveDegree(net) << "\n";
    }
    
  } // end of rounds loop 
}
*/



// runs the social network model according to parameters below
/*
template <typename NetType, typename Generator>
void socModel6(NetType & net, NetType & net2, Generator & generator, 
	       const float p_jump, const float p_walk, const float p_tri, const float p_death,
	       const float delta,  const size_t stepLimit, const size_t maxRounds ) {
  
  const size_t netSize = net.size();
  const float w0 = 1.0;
  bool net2equaltonet1 = false;
  
  if (!net2equaltonet1) copyNet(net, net2); // net 2 is the network where all changes go
  
  size_t indexArray[netSize];
  for (size_t i = 0; i < netSize; ++i) indexArray[i] = i;      // generate linear ordering

  size_t currentNode, nextNode, secondNode, randNode;
  for ( size_t rounds=0; rounds < maxRounds; ++rounds) {       // run the simulation over many rounds (time steps)

    fastSuffle(indexArray, netSize, generator);                // take the nodes in random order at each time step
    
    for ( size_t i = 0; i < netSize; ++i) {                    // loop over all nodes
      currentNode = indexArray[i];

      if ( generator.next(1.0) < p_jump || net(currentNode).size() == 0 ) {                     // choose random node
	do {
	  randNode = generator.next(netSize);
	} while ( randNode == currentNode );    
	if ( net(currentNode)[randNode] == 0 ) 
	  net2[currentNode][randNode] = w0; // connect in net2 if not already connected
      }
	
      // perform walk from current node if possible
      if ( net(currentNode).size() > 0 ) {    // this is not always true as the previous link is not yet efective     
	
	if ( generator.next(1.0) < p_walk ) { // decide whether to do the walk or not
	  
	  nextNode = weighedStep(net, currentNode, generator);
	  net2[currentNode][nextNode] = net2[currentNode][nextNode] + delta; // increase weight in net2
	  
	  if ( net(nextNode).size() > 1 ) {                                // if possible, take step avoiding old node
	    secondNode = weighedStepExcludingOld(net, nextNode, currentNode, generator);
	    net2[nextNode][secondNode] = net2[nextNode][secondNode] + delta;   // increase weight in net2
	    
	    // second neighbor has been found
	    if (secondNode != currentNode) {     // this should be always true... 
	      if ( net(currentNode)[secondNode] == 0 ) {     // there is not link, i.e., we are at distance 2
		if (  generator.next(1.0) < p_tri ) net2[currentNode][secondNode] = w0;
	      }
	      else {
		net2[currentNode][secondNode] = net2[currentNode][secondNode] + delta;
	      }
	    }
	  }
	}
      }
    }
    // end of node loop
      
    for ( size_t i = 0; i < netSize; ++i) { 
      if ( generator.next(1.0) < p_death) {
	for (typename NetType::edge_iterator neigh=net2[i].begin(); !neigh.finished(); ++neigh) {
	  neigh.value() = 0;
	}
      }
    }

    copyNet(net2, net);
    net2equaltonet1 = true;
    
    if ( rounds%1000 == 0 ) {
      std::cerr << rounds << "\t" <<  calculateAveDegree(net) << "\n";
    }
    
  } // end of rounds loop 
}
*/








// takes a random step weighed with the link weights excluding the previous node
/*
template <typename NetType, typename Generator>
size_t weighedStepExcludingOld2(NetType & net, const size_t currentNode, 
								const size_t prevNode, const float alpha, Generator & generator) {
  
  const size_t degree = net(currentNode).size();
  if ( degree < 2 ) std::cerr << "degreetä ei riittävästi\n";
  float weights[degree-1]; // the weight to prevNode is not stored
  size_t indexes[degree-1]; 
   float maxWeight=0;
  size_t counter = 0;
  for (typename NetType::const_edge_iterator j=net(currentNode).begin(); !j.finished(); ++j) {
    if ( *j != prevNode) {
      weights[counter] = j.value();
      indexes[counter] = *j;
      if ( weights[counter] > maxWeight ) maxWeight = weights[counter]; 
      ++counter;
    }
  }

  if (counter != degree - 1) std::cerr << "painotetussa valinnassa häikkää\n";

  float wSum = 0;
  for (size_t i = 0; i < degree-1; ++i) weights[i] = pow(weights[i]/maxWeight, alpha); // all numbers are between zero and one
  for (size_t i = 0; i < degree-1; ++i) wSum += weights[i];
  float cumWSum[degree-1];
  cumWSum[0] = weights[0];
  for (size_t i = 1; i < degree-1; ++i) cumWSum[i] = cumWSum[i-1] + weights[i];
  float p = generator.next(wSum); // float between 0 and wSum

  size_t luckyIndex = 0;
  while ( cumWSum[luckyIndex] < p ) ++luckyIndex;
  return indexes[luckyIndex];
}
*/




// takes a random step weighed with the link weights to power alpha!
/*
template <typename NetType, typename Generator>
size_t weighedStep2(NetType & net, const size_t currentNode, const float alpha, Generator & generator) {
  
  const size_t degree = net(currentNode).size();
  if ( degree < 1 ) std::cerr << "degreetä ei riittävästi\n";
  float weights[degree];
  float maxWeight=0;
  size_t indexes[degree]; 
  size_t counter = 0;
  for (typename NetType::const_edge_iterator j=net(currentNode).begin(); !j.finished(); ++j) {
      weights[counter] = j.value();
      indexes[counter] = *j;
      if ( weights[counter] > maxWeight ) maxWeight = weights[counter]; 
      ++counter;
  }

  if (counter != degree) std::cerr << "painotetussa valinnassa häikkää\n";

  float wSum = 0;
  for (size_t i = 0; i < degree; ++i) weights[i] = pow(weights[i]/maxWeight, alpha); // all numbers are between zero and one
  for (size_t i = 0; i < degree; ++i) wSum += weights[i];
  float cumWSum[degree];
  cumWSum[0] = weights[0];
  for (size_t i = 1; i < degree; ++i) cumWSum[i] = cumWSum[i-1] + weights[i];
  float p = generator.next(wSum); // float between 0 and wSum

  size_t luckyIndex = 0;
  while ( cumWSum[luckyIndex] < p ) ++luckyIndex;
  return indexes[luckyIndex];
}
*/






/*

template <typename Generator>
float gammaDistributedRandomNumber(const size_t k, const float gamma, Generator & generator) {
  float r = 0;
  float u;
  for (size_t i = 0; i < k; ++i) {
    u = generator.next(1.0);
    r += log(u);
  }

  r = -1.0/gamma*r;
  return r;
}

*/

