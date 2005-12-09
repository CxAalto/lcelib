#include <iostream>
#include "../Bindata.H"


using namespace std;

int main(void){
  double startEdge = 12;
  double endEdge = 30;
  int numberOfBins = 5;
  int numberOfValues = 10;
  double x[10] = {8,12,14000,15.333,29,12.5,13,15,18,18.5};
  double y[10] = {13,21,14,15.333,2900,122.5,3,44.7,12,15.8};
  double x2[10] = {3,5,5.4,12,29,12.5,13,15,18,18.5};

  // Linear bining "forward" with x
  double * bin_edges= new double[numberOfBins+1];
  double * bin_centers = new double[numberOfBins];
  double * bin_centers_log = new double[numberOfBins];
  double * bin_means = new double[numberOfBins];
  
  cout << "Linear" << endl << "Forward: ";
  bin_edges = spacing("lin",startEdge,endEdge,numberOfBins);
  bin_centers = giveBinCenters(bin_edges,numberOfBins);
  bin_means = meansc("forward", bin_edges,x,numberOfBins,numberOfValues);
  
  cout << "Edges: ";
  for(int i=0; i<numberOfBins+1;i++){
    cout << " " << bin_edges[i];
  }
  
  cout << endl << "Centers: ";
  for(int i=0; i<numberOfBins;i++){
    cout << " " << bin_centers[i];
  }

  cout << endl << "Means (forward): ";
  for(int i=0; i<numberOfBins;i++){
    cout << " " << bin_means[i];
  }

  // Linear bining "backward" with x
  cout << endl << "Backward: " << endl;
  bin_means = meansc("backward",bin_edges,x,numberOfBins,numberOfValues);
  cout << " Means (backward): ";
  for(int i=0; i<numberOfBins;i++){
    cout << " " << bin_means[i];
  }
  cout << endl;
  
  // Logarithmic bining "forward" with x2 and y
  startEdge = 3; endEdge = 40; 
  bin_edges = spacing("log",startEdge,endEdge,numberOfBins);
  bin_centers = giveBinCenters(bin_edges,numberOfBins); // = giveBinCenters("lin",bin_edges,numberOfBins);
  
  // Let´s show how two centering methods differ.
  // For details, see Bindata.H.
  bin_centers_log = giveBinCenters("log",bin_edges,numberOfBins);
  bin_means = meansc("forward", bin_edges,x2,y,numberOfBins,numberOfValues);
  
  cout << "Logarithmic" << endl << "Edges: ";
  for(int i=0; i<numberOfBins+1;i++){
    cout << " " << bin_edges[i];
  }
  
  cout << endl << "Centers(normal): ";
  for(int i=0; i<numberOfBins;i++){
    cout << " " << bin_centers[i];
  }
  
  cout << endl << "Centers(log): ";
  for(int i=0; i<numberOfBins;i++){
    cout << " " << bin_centers_log[i];
  }
  
  cout << endl << "Means (forward): ";
  for(int i=0; i<numberOfBins;i++){
    cout << " " << bin_means[i];
  }
  cout << endl;
}
