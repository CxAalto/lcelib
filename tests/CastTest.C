#include <iostream>

template <typename T>
class PuppaType {
private:
  T data;
public:
  T & get() {return data;}
  const T & get() const {return data;}
};

template<typename T>
const PuppaType<T> & const_cast<PuppaType<T> &>(T & src) {
  std::cerr << "Foo";
  return src;
}
  
int main() {
  PuppaType<int> foo;
  foo.get()=4;
  std::cerr << foo.get();
   
}
  

    
