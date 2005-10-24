#include <cassert>
#include "../bits/FiboHeapCore.H"
#ifndef FIBO_HEAP
#define FIBO_HEAP

template <typename KeyType, typename ValueType>
class FiboHeapMap {
public:
  typedef FiboHeapNode<KeyType, ValueType> NodeType;
private:
  FiboHeapCore<NodeType> heap;
  KeyType lastPopped;
public:
  /* Pushes a key-value pair to the heap. Returns a pointer 
   * to the node so generated, which can be used for decreasing key, 
   * deletion, setting values again etc. */
  NodeType * push(const KeyType key, const ValueType value=0) {
    NodeType * retval;
    retval=new NodeType(key);
    retval->value()=value;
    heap.push(retval);
    return retval;
  }

  /**
   * Pops the next key from the heap. The value is returned by reference.
   */
  KeyType pop(ValueType & value) {
    KeyType retval;
    NodeType * popped;
    popped=heap.pop();
    assert(popped!=0);
    value=popped->value();
    retval=popped->getKey();    
    delete popped;
    lastPopped=retval;
    return retval;
  }

  bool isEmpty() {return heap.isEmpty();}

  KeyType getMinKey() const {return heap.getMinKey();}
  
  void decreaseKey(NodeType * subject, KeyType newKey) {
    subject->decreaseKey(newKey, heap);
  } 
    
  unsigned getNumElems() {
    return heap.getNumElems();
  }

  bool isValid(bool justPopped=true) {
    return heap.rootListValid(true, justPopped); /* Check it all, 
						  * check degree list only 
						  * if just popped. */
  }
};

template <typename KeyType>
class FiboHeap {
 typedef FiboHeapNode<KeyType, void> NodeType;
private:
  KeyType lastPopped;  
  FiboHeapCore<NodeType> heap;
public:
  /* Pushes a key to the heap. */
  NodeType * push(const KeyType key) {
    NodeType * retval;
    retval=new NodeType(key);
    heap.push(retval);
    return retval;
  }

  /**
   * Pops the next key from the heap.
   */
  KeyType pop() {
    KeyType retval;
    NodeType * popped;
    popped=heap.pop();
    assert(popped!=0);
    retval=popped->getKey();
    delete popped;
    lastPopped=retval;
    return retval;
  }
  
  KeyType getLastTime() {return lastPopped;}

  KeyType getMinKey() const {
    assert (heap.getRoot()); /* !=0, that means. !=NULL, in C. */
    return heap.getRoot()->getKey();
  }
  
  void decreaseKey(NodeType * subject, KeyType newKey) {
    subject->decreaseKey(newKey,heap);
  } 
    
  unsigned getNumElems() {
    return heap.getNumElems();
  }

  bool isEmpty() {return heap.isEmpty();}

  bool isValid() {return heap.rootListValid(true);}

};

#endif




