
#define NDEBUG
#define GNU_PREFETCH

#include <iostream>
#include <cmath>
#include "../misc/EventQueue.H"
#include "../Randgens.H"
#include "../Containers.H"
#include <ctime>

#define NUM_RANDS 10000000
#define Q_SIZE 1000000
#define expec 10000000.0
#include <cassert>

class tst: public EvQueue<unsigned>::Event {
public:
  virtual bool foo() {return false;}
};

class tst2: public tst {
public:
  virtual bool foo() {return true;}
};


typedef tst Event;
typedef Event * EventPtr;


int main() {
  std::cerr << sizeof(Event) << " " << sizeof(tst);
  EvQueue<unsigned> queue(0,20);
#ifndef  NDEBUG
  AutoMap<unsigned, unsigned> hash;
#endif
  
  unsigned * addTimes;

  clock_t cpustart;

  unsigned lastTime=0;
  float recinvlambda=-expec;

  tst test;
  std::cerr << (&test) << " ";
  std::cerr << (&(test.next)) << "\n";

  EvQueue<unsigned>::Event test2;
  std::cerr << (&test2) << " ";
  std::cerr << (&(test2.next)) << "\n";

  std::cerr << "To rands\n";

  addTimes=new unsigned[NUM_RANDS];
  for (unsigned i=0; i<NUM_RANDS; ++i) {
    addTimes[i]=((unsigned) (recinvlambda*log(((float) rand())/RAND_MAX)));
  }
  std::cerr << "Into the test\n";

  unsigned i;

  for (i=0; i<Q_SIZE; i++) {
    //std::cerr << "#";
    EventPtr curr;
    if (i & 1 == 0) 
      curr=new tst();
    else 
      curr=new tst2();
    
    assert(queue.getNumEvents()==i);
    queue.push(curr,addTimes[i]);
    //queue.printout();
#ifndef  NDEBUG
    ++hash[addTimes[i]];
    if ((i & ((1 << 16)-1)) == 0) std::cerr << i << "\n";
#endif
  }
  std::cerr << "PUTS DONE\n";

  //queue.printout();

  unsigned acc=0;

  for (; i<NUM_RANDS; i++) {
    unsigned currTime;
    assert(queue.getNumEvents()==Q_SIZE);
    //std::cerr << "#";
    EventPtr ptr=(EventPtr) queue.pop(currTime);
    acc+=ptr->foo();
    //delete ptr;
    //std::cerr << "Pop:" << currEvent << "\n";
    assert(currTime >= lastTime);
    lastTime=currTime;
    assert(hash[currTime]-- > 0); /* Was not empty*/
    currTime+=addTimes[i];
    //ptr=new Event();
    queue.push(ptr, currTime);
    //std::cerr << "+";
    assert(++hash[currTime] > 0);
    if (i && ((1 << 10)-1) == 0) std::cerr << i << "\n";
  }
  
  std::cerr << "To meas. Acc:" << acc << "\n";
  cpustart=clock(); 
  std::cerr << cpustart << "\n";

  for (i=0; i<NUM_RANDS; i++) {
    unsigned currTime;
    assert(queue.getNumEvents()==Q_SIZE);
    //std::cerr << "#";
    EventPtr ptr=(EventPtr) queue.pop(currTime);
    //delete ptr;
    //std::cerr << "Pop:" << currEvent << "\n";
    assert(currTime >= lastTime);
    lastTime=currTime;
    assert(hash[currTime]-- > 0); /* Was not empty*/
    currTime+=addTimes[i];
    //ptr=new Event();
    queue.push(ptr, currTime);
    //std::cerr << "+";
    assert(++hash[currTime] > 0);
  }
  
  std::cerr << "\nClocks" << (clock()-cpustart) << "Per sec:" << CLOCKS_PER_SEC;
  std::cerr << clock() << "\n";
  std::cerr << "\nHold per sec:" 
	    << ((float) (NUM_RANDS)*CLOCKS_PER_SEC)/(clock()-cpustart) 
	    << "\n";

  for (i=Q_SIZE; i>0; i--) {
    unsigned currTime;
    assert(queue.getNumEvents()==i);
    //std::cerr << "#";
    EventPtr ptr=(EventPtr) queue.pop(currTime);
    delete ptr;
    assert(currTime >= lastTime);
    lastTime=currTime;
    assert(hash[currTime]-- > 0); /* Was not empty*/
  }
  assert(hash.size()==0);

  std::cerr << "All done\n";

}
  
