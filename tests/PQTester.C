
#define NDEBUG

#include <iostream>
#include <cmath>
#include "./packedqueue.H"
#include "./randgens.H"
#include "./hashes.H"
#include <ctime>

#define NUM_RANDS 10000000
#define Q_SIZE 1000000
#define expec 10000000.0
#include <cassert>

int main() {
  Ranmar<float> myRand;
  MyPackedCalQueue<TestEvent> queue;
#ifndef  NDEBUG
  HashMultiKeySet<unsigned long> hash;
#endif
  
  float * randvals;
  unsigned * addTimes;

  unsigned i;
  clock_t cpustart;

  unsigned long lastTime=0;
  float recinvlambda=-expec;  
  TestEvent currEvent(0,true);

  std::cerr << "To rands\n";

  randvals=myRand.array(NUM_RANDS);
  addTimes=new unsigned[NUM_RANDS]; /* Unsigned is nice in case of overflows */

  std::cerr << "To calc\n";
  for (i=0; i<NUM_RANDS; i++) {
    addTimes[i]=(unsigned) (recinvlambda*log(randvals[i]));
  }

  delete[] randvals;

  std::cerr << "Into the test\n";

  for (i=0; i<Q_SIZE; i++) {
    //std::cerr << "#";
    assert(hash.size()==i);
    assert(queue.getNumEvents()==i);
    currEvent.time=addTimes[i];
    queue.push(currEvent);
    //queue.printout();
#ifndef  NDEBUG
    hash.put(addTimes[i]);
#endif
  }
  std::cerr << "PUTS DONE\n";

  queue.printout();
  for (; i<NUM_RANDS; i++) {
    assert(hash.size()==Q_SIZE);
    assert(queue.getNumEvents()==Q_SIZE);
    //std::cerr << "#";
    currEvent=queue.pop();
    //std::cerr << "Pop:" << currEvent << "\n";
    assert(currEvent.time >= lastTime);
    lastTime=currEvent.time;
    assert(hash.remove(currEvent.time));
    currEvent.time+=addTimes[i];
    queue.push(currEvent);
    //std::cerr << "+";
#ifndef  NDEBUG
    hash.put(currEvent.time);
#endif
  }
  
  std::cerr << "To meas:";
  cpustart=clock();

  for (i=0; i<NUM_RANDS; i++) {
    assert(hash.size()==Q_SIZE);
    assert(queue.getNumEvents()==Q_SIZE);
    //std::cerr << "#";
    currEvent=queue.pop();
    //std::cerr << "Pop:" << currEvent << "\n";
    assert(currEvent.time >= lastTime);
    lastTime=currEvent.time;
    assert(hash.remove(currEvent.time));
    currEvent.time+=addTimes[i];
    queue.push(currEvent);
    //std::cerr << "+";
#ifndef  NDEBUG
    hash.put(currEvent.time);
#endif
  }

  std::cerr << "\nClocks" << (clock()-cpustart) << "Per sec:" << CLOCKS_PER_SEC;
  std::cerr << "\nHold per sec:" 
       << ((float) (NUM_RANDS-Q_SIZE)*CLOCKS_PER_SEC)/(clock()-cpustart) 
       << "\n";

  for (i=Q_SIZE-1; i>=0; i--) {
    currEvent=queue.pop();
    assert(currEvent.time >= lastTime);
    lastTime=currEvent.time;
    assert(hash.remove(currEvent.time));
    assert(hash.size()==i);
    assert(queue.getNumEvents()==i);
  }

  std::cerr << "All done\n";

}
  
