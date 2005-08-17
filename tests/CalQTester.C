
//#define NDEBUG
#define USE_FIBO

#ifdef USE_FIBO
#include "./FiboHeap.H"
#endif

#include <iostream>
#include <cmath>
#include "./calqueue.H"
#include "./randgens.H"
#include "./hashes.H"
#include <ctime>

#define NUM_RANDS 10000000
#define Q_SIZE 1000000
#define expec 10000000.0
#include <cassert>

class TestEvent: public CalQEvent<TestEvent> {
public:
  void setTime(const unsigned long eventTime) {time=eventTime;}
  TestEvent(const unsigned long time): CalQEvent<TestEvent>(time) {}
};

int main() {
  Ranmar<float> myRand;
  MyCalQueue<TestEvent> queue(0,22);
#ifndef NDEBUG
  HashMultiKeySet<unsigned long> hash;
#endif

#ifdef USE_FIBO
  FiboHeap<unsigned long> heap;
  unsigned long heapTime;
#endif

  float * randvals;
  unsigned * addTimes;

  TestEvent * currEvent;
  int i;
  clock_t cpustart;

  unsigned long lastTime=0;;
  float recinvlambda=-expec;  

  std::cerr << "To rands\n";

  randvals=myRand.array(NUM_RANDS);
  addTimes=new unsigned[NUM_RANDS]; /* Unsigned is nice in case of overflows */

  std::cerr << "To calc\n";
  for (int i=0; i<NUM_RANDS; i++) {
    addTimes[i]=(unsigned) (recinvlambda*log(randvals[i]));
  }

  delete[] randvals;

  std::cerr << "INto the test\n";

  for (i=0; i<Q_SIZE; i++) {
    assert(hash.size()==i);
    assert(queue.getNumEvents()==i);
    if (addTimes[i] < 0) std::cerr << "!:" << addTimes[i] << "\n";
    queue.push(new TestEvent(addTimes[i]));
#ifndef  NDEBUG
    hash.put(addTimes[i]);
#endif
#ifdef USE_FIBO
    heap.push(addTimes[i]);
#endif
    
  }
  std::cerr << "PUTS DONE\n";

  cpustart=clock();
  for (i=0; i<NUM_RANDS; i++) {
    assert(hash.size()==Q_SIZE);
    assert(queue.getNumEvents()==Q_SIZE);
    currEvent=queue.pop();
    assert(currEvent->getTime() >= lastTime);
    lastTime=currEvent->getTime();
    assert(hash.remove(currEvent->getTime()));
#ifdef USE_FIBO
    assert (heap.getNumElems()==Q_SIZE);
    heapTime=heap.pop();
    //std::cerr << i << " " << heapTime << " " << lastTime << "\n";
    assert (heapTime==currEvent->getTime());
    
#endif    
    currEvent->setTime(currEvent->getTime() + addTimes[i]);
    queue.push(currEvent);
#ifdef USE_FIBO
    heap.push(currEvent->getTime());
#endif 

#ifndef  NDEBUG
    hash.put(currEvent->getTime());
#endif
  }
  
  std::cerr << "\nClocks" << (clock()-cpustart) << "Per sec:" << CLOCKS_PER_SEC;
  std::cerr << "\nHold per sec:" 
       << ((float) (NUM_RANDS)*CLOCKS_PER_SEC)/(clock()-cpustart) 
       << "\n";

  for (i=Q_SIZE-1; i>=0; i--) {
    currEvent=queue.pop();
    assert(currEvent->getTime() >= lastTime);
    lastTime=currEvent->getTime();
    assert(hash.remove(currEvent->getTime()));
    assert(hash.size()==i);
    assert(queue.getNumEvents()==i);
#ifdef USE_FIBO
    heapTime=heap.pop();
    assert (heap.getNumElems()==i);
    assert (heapTime==lastTime);
    
#endif 
  }

  assert(queue.pop()==0);
  

  std::cerr << "All done\n";

}
  









