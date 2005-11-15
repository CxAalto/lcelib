
//#define NDEBUG
//#define GNU_PREFETCH

#include <iostream>
#include <cmath>
#include "../misc/EventQueue.H"
#include "../Randgens.H"
#include "../Containers.H"
#include <ctime>

#define NUM_RANDS 10000000
#define STREAK 1000
#define Q_SIZE 1000000
#define expec 10000000.0
#include <cassert>

typedef unsigned /*long*/ TimeType;


class tst: public EvQueue<TimeType>::Event {
public:
  virtual bool foo() {
    //std::cerr << "Foo!";
    return false;
  }
};

class tst2: public tst {
public:
  virtual bool foo() {
    //std::cerr << "BAr!";
    return true;
  }
};


//typedef EvQueue<TimeType>::Event Event;
typedef tst Event;
typedef Event * EventPtr;

int main() {
  assert(false);
  std::cerr << sizeof(Event) << " " << sizeof(tst);
  EvQueue<TimeType> queue(0,20);
#ifndef  NDEBUG
  AutoMap<TimeType, unsigned> hash;
#endif
  
  TimeType * addTimes;

  clock_t cpustart;
  //clock_t tempcpu=0;
  clock_t pushTot=0;
  clock_t popTot=0;

  TimeType lastTime=0;
  float recinvlambda=-expec;

  tst test;
  std::cerr << (&test) << " ";
  std::cerr << (&(test.next)) << "\n";
  
  EvQueue<TimeType>::Event * ptr=(&test);
  std::cerr << ptr << "\n";

  tst * foo=(tst *) ptr;
  std::cerr << foo << "\n";

  EvQueue<TimeType>::Event test2;
  std::cerr << (&test2) << " ";
  std::cerr << (&(test2.next)) << "\n";

  std::cerr << "To rands\n";
  addTimes=new TimeType[NUM_RANDS];
  for (unsigned i=0; i<NUM_RANDS; ++i) {
    addTimes[i]=((TimeType) (recinvlambda*log(((float) rand())/RAND_MAX)));
  }
  std::cerr << "Into the test\n";

  unsigned i;

  for (i=0; i<Q_SIZE; i++) {
    //std::cerr << "#";
    EventPtr curr=new Event;;
    if ((i & 1) == 0) {
      curr=new tst();
    } else { 
      curr=new tst2();
    }    

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

    TimeType currTime;
    assert(queue.getNumEvents()==Q_SIZE);
    //std::cerr << "#";
    EventPtr ptr=(EventPtr) queue.pop(currTime);
    //std::cout << currTime << "\n";
    acc+=ptr->foo();
    //delete ptr;
    //std::cerr << "Pop:" << currEvent << "\n";
    assert(currTime >= lastTime);
    lastTime=currTime;
    assert(hash[currTime]-- > 0); /* Was not empty*/
    currTime+=addTimes[i];
    //currTime+=((TimeType) (recinvlambda*log(((float) rand())/RAND_MAX)));
    //ptr=new Event();
    queue.push(ptr, currTime);
    //std::cerr << "+";
    assert(++hash[currTime] > 0);
    if (i && ((1 << 10)-1) == 0) std::cerr << i << "\n";
  }
  
  std::cerr << "To meas. Acc:" << acc << "\n";
  cpustart=clock(); 
  std::cerr << cpustart << "\n";

//   for (i=0; i<NUM_RANDS; i+=STREAK) {
//     TimeType currTime;
//     tempcpu=clock();
//     for (unsigned j=i; j<i+STREAK; ++j) {
//       delete queue.pop(currTime);
//     }
//     //std::cerr << i << "\n";
//     popTot+=clock();
//     popTot-=tempcpu;
//     tempcpu=clock();
//     for (unsigned j=i; j<i+STREAK; ++j) {
//       queue.push(new Event(), currTime+addTimes[j]);
//       //std::cerr << "+";
//     }
//     pushTot += clock();
//     pushTot -= tempcpu;
//   }

  for (i=0; i<NUM_RANDS; i++) {
    TimeType currTime;
    assert(queue.getNumEvents()==Q_SIZE);
    //std::cerr << "#";
    EventPtr ptr=(EventPtr) queue.pop(currTime);
    //acc+=ptr->foo();
    //delete ptr;
    //std::cerr << "Pop:" << currEvent << "\n";
    assert(currTime >= lastTime);
    lastTime=currTime;
    assert(hash[currTime]-- > 0); /* Was not empty*/
    //currTime+=((TimeType) (recinvlambda*log(((float) rand())/RAND_MAX)));
    currTime+=addTimes[i];
    //ptr=new Event();
    queue.push(ptr, currTime);
    //std::cerr << "+";
    assert(++hash[currTime] > 0);
  }

  std::cerr << "\nClocks" << (clock()-cpustart) << "Per sec:" << CLOCKS_PER_SEC;
  std::cerr << "\n" << clock() << "\n";
  std::cerr << "\nHolds per musec" 
	    << (((float) (NUM_RANDS))/(clock()-cpustart))
	    << "\n";

  std::cerr << "Pushes per musec" << (((float) (NUM_RANDS))/pushTot) << "\n";
  std::cerr << "Pops per musec" << (((float) (NUM_RANDS))/popTot) << "\n";

  for (i=Q_SIZE; i>0; i--) {
    TimeType currTime;
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
  
