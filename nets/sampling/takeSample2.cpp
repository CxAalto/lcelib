// g++ -O takeSample2.cpp -o takeSample2
// ./takeSample2 verkot/community_largest.edg 0




#define NDEBUG // to turn assertions off

#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <ctime>
#include <cstdlib>

#include "../../../lcelib/Containers.H"
#include "../../../lcelib/Nets.H"
#include "../../../lcelib/Randgens.H"

#include "../../../lcelib/nets/NetExtras.H"
#include "../../../lcelib/nets/Dijkstrator.H"


typedef float EdgeData;
typedef  SymmNet<EdgeData, ValueTable, ExplSumTreeTable> NetType;
typedef Set<size_t> nodeSet; // stores the indexes of nodes
typedef Map<size_t, nodeSet > nodeMap;



NetType * readNet3(const size_t weights, const size_t himmeli, char * fileName)
{
  typedef NetType::EdgeData EdgeDataType;
  
  std::vector<size_t> edgeSource;
  std::vector<size_t> edgeDest;
  std::vector<EdgeDataType> edgeData;
  size_t nodeCount = 0;

  std::string line;
  std::ifstream myfile(fileName);
  if (myfile.is_open()) {

    if (himmeli) {                   // first line in himmeli edge data file has to be skipped
      getline(myfile,line);
    }

    while (!myfile.eof()) {
      
      size_t source, dest;
      EdgeDataType data;
      getline(myfile, line);   // Read a line from input into a string.
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

    myfile.close();
    
    // Construct the net.
    std::auto_ptr<NetType> netPointer(new NetType(nodeCount));
    NetType& net = *netPointer;  // Create a reference for easier access.
    std::cerr << "Reading in network...\n";
    std::cerr << "Network has " << nodeCount << " nodes.\n";
    
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

  std::cerr << "Unable to open file " << fileName << "\n";
  return NULL; // Unable to open file.
}
// <---------------- readNet3 --------------------------



 
int main(int argc, char* argv[]) {
  
  if ( argc < 3 )
    {
      std::cerr << "Please give arguments: \n\tedge file to read in,\n\t1 if input file contains a header and 0 otherwise\n\n";
      exit(0);
    }

  const size_t himmeli_in = atoi(argv[2]); // use 0 if input file does not contain himmeli-headers, otherwise use 1

  int randseed=time(0); // some random number 
  RandNumGen<> generator(randseed);

  // read net from file
  std::auto_ptr<NetType> netPointer(readNet3(0,himmeli_in,argv[1]));
  NetType& net = *netPointer;  // Create a reference for easier handling of net.

  const size_t netSize = net.size();

  if (ConnectivityCheck(net)) {
    std::cerr << "Net connected\n";
  }
  else {
    std::cerr << "Net disconnected\n";
    exit(-1);
  }



  size_t distanceLimit = 0;
  int givenNode = 0;
  size_t vertexColor[netSize];
  do {
    nodeSet selectedNodes;
    std::cout << "\nThe visualization was written to the file Sample_0001.eps and can be viewed with the command \n\t\t\tgv Sample_0001.eps\n";
    std::cout << "\nPlease give maximum distance for snowball sample or type Ctrl-c to quit:\n";
    std::cin >> distanceLimit;
    std::cout << "\nPlease give starting node or type -1 to pick a random node:\n";
    std::cin >> givenNode; 
    size_t selectedNode = 0;
    if ( (givenNode > -1) && ((size_t)givenNode < net.size()) ) {
      selectedNode = (size_t) givenNode;  // use the given node 
      std::cerr << "\nStarting snowball sample from given node " << selectedNode << "\n";
    }
    else{
      selectedNode = generator.next(netSize);  // ...or choose random node
      std::cerr << "\nStarting snowball sample from random node " << selectedNode << "\n";
    }
    
    size_t distance = 0;
    selectedNodes.put( selectedNode );
    vertexColor[selectedNode] = 2; // color code for the starting node of the sample
    Dijkstrator<NetType> paths(net,selectedNode);  // find paths from the selected begin node
    do {
      distance = (size_t) (*paths).getWeight();
      if (distance <= distanceLimit) {        
	size_t currentNode =  (size_t)(*paths).getDest();
	// std::cerr << currentNode << "\t" << distance << "\n";
	selectedNodes.put( currentNode );
	if (distance < distanceLimit) vertexColor[currentNode] = 0; // color code for inner nodes in the sample
	else vertexColor[currentNode] = 1;  // color code for nodes on the boundary of the sample
      }
      ++paths;
    } while (distance <= distanceLimit && !paths.finished() ); 
    
    std::cerr << "Sample size: " << selectedNodes.size() << "\n";
    // for (nodeSet::iterator i = selectedNodes.begin(); !i.finished(); ++i) std::cerr << *i << " ";
    // std::cerr << "\n";

    size_t tiedostoon;
    std::cout << "Please type \n\t0 \tto output the edges of this sample to the file \n\t\tsample.edg and run Himmeli, \n\t1\t to take another sample, or\n\tCtrl-c\tto quit.\n\n";
    std::cin >> tiedostoon;
    if (!tiedostoon) {
      std::ofstream myFile("sample.edg");
      if (! myFile) {
	std::cerr << "Error opening output file\n";
	exit(-1);
      }
      
      myFile << "HEAD" << "\t" << "TAIL" << "\t" << "WEIGHT" << "\t" << "COLOR" << "\n"; // color the last layer edges gray
      for (nodeSet::iterator i = selectedNodes.begin(); !i.finished(); ++i) {
	for (NetType::const_edge_iterator neighbor=net(*i).begin(); !neighbor.finished(); ++neighbor) { 
	  /* ADD: check whether the edge is linked to one of the nodes on the boundary ... */
	  if ( selectedNodes.contains(*neighbor) && *i < *neighbor ) { // only include links within the sample
	    if (  (vertexColor[*i] == 1) || ( vertexColor[*neighbor] == 1) ) { // if one or both of the nodes are in the outer layer (boundary) of the sample, make the links thin and gray 
	      myFile << *i << "\t" << *neighbor << "\t" << 0.2 << "\t" << "666666"<< "\n";
	    }  
	    else {  // otherwise, make the links thick and black 
	      myFile << *i << "\t" << *neighbor << "\t" << 1 << "\t" << "000000"<< "\n";
	    }
	  }
	}
      }
      myFile.close();

      std::ofstream myFile2("sample_vertex.txt");
      if (! myFile2) {
	std::cerr << "Error opening output file\n";
	exit(-1);
      }
      myFile2 << "NAME\tCOLOR\tSHAPE\tSIZE\n";
      for (nodeSet::iterator i = selectedNodes.begin(); !i.finished(); ++i) {
	if (  vertexColor[*i] == 0 ) {	myFile2 << *i << "\t" << "000000" << "\t" << "1" << "\t" << "1.0" << "\n";} // set color, shape and size for inner nodes  (1 - circle)
	else if (  vertexColor[*i] == 2 )   myFile2 << *i << "\t" << "886600" << "\t" << "3" << "\t" << "1.5" << "\n"; // set color, shape and size for the center node (3 - cross)
	else 	myFile2 << *i << "\t" << "666666"  << "\t" << "7" << "\t" << "0.2" << "\n"; // set color, shape and size for nodes on the boundary  (7 - square)
      }
      myFile2.close();

      
      system("./himmeli sample.cfg");
    }
    
  }  while (distanceLimit);

}
