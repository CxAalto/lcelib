#include <iostream>
#include<ctime>
#define NUM_VALS 10000000

int main(int argc, char* argv[]) {
  int * vals;
  int hits=0;
  float sum=0;

  vals=new int[NUM_VALS];
  for (int i=0; i<NUM_VALS; i++) vals[i]=i;


  clock_t cpustart=clock();
  for (int j=0; j<10; j++) {
    for (int i=0; i<NUM_VALS; i++) {
      //if ((i & 31) == 0) {i++;}
      sum+=vals[i];
    }
  }
  std::cout << "Time:" << (float(clock()-cpustart) / CLOCKS_PER_SEC) << "\n";  
  std::cout << "Sum:" << sum << "\n";
}
