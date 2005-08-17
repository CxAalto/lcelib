#include <iostream>

template <typename T>
class inheritor: public T {
  T* operator->() {return this;}
}

template <typename T>
class PuppaArray {
public:
  inheritor<T> * array;
  PuppaArray(unsigned size) {array=new inheritor<T>[size];}
  ~PuppaArray() {delete[] array;}
 
private:
  class Stub {
    friend class PuppaArray;
    PuppaArray & ref;
    unsigned index;
    Stub() {}
    Stub(PuppaArray & src, unsigned where): 
      ref(src), index(where) {}
  public:
    ~Stub() {
      std::cerr << "del ";
    }

    inheritor<T> & operator->() {return ref.array[where];}
  
  };
  
  friend class Stub;

public:

  Stub operator[](const unsigned where) {
    std::cerr << "Returning a stub\n"; 
    return Stub(*this, where);
  }

  const T & operator()(const unsigned where) const {
    std::cerr << "Returning data\n";
    return array[where];
  }
};

class MyBloodyInt {
  int val;
public:
  MyBloodyInt(const int src=0): val(src) {}
  int getValue() {return val;}
  void setValue(const int src) {val=src;}
};

  
int main() {
  //PuppaArray<wrapper<int> > koe(10);
  //PuppaArray<intWrapper> koe(10);
  //PuppaArray<int> koe(10);
  PuppaArray<MyBloodyInt> koe(10);
  //int test;
  std::cerr << "\n";
  //for (unsigned i=0; i<10; i++) {std::cerr << koe[i].getValue() << "\n";}
  koe[5];/*.setValue(4);*/
  //for (unsigned i=0; i<10; i++) {std::cerr << "IN" << koe[i].getValue() << "OUT" << koe[i].getValue() << "OUTAGAIN" << "\n";}
   
}
  

    
