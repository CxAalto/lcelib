#ifndef LCE_VALUE_TABLE
#define LCE_VALUE_TABLE
#include<cassert>
#include "../ArrayBase.H"
#include "./StubBase.H"
#include "../Pair.H"
#include "../ContainerPolicies.H"
#include "../WeightPolicy.H"
#ifndef NDEBUG
#include<iostream>
#endif

/**
 * This is the ValueTable that lurks behind each and every container 
 * implementation. Even if the value is void.
 */

template<typename KeyType, typename _ValueType, 
	 typename Policy, typename Params, typename Index>
class ValueTable:public ArrayBase<Pair<KeyType, _ValueType> > {
private:
  typedef ValueTable<KeyType, _ValueType, Policy, Params, Index> MyType;
  typedef ArrayBase<Pair<KeyType, _ValueType> > super;

public:
  typedef _ValueType ValueType;

  /** The Pair masks the void references away */
  typedef typename Pair<KeyType, ValueType>::second_reference  value_reference;
  typedef typename Pair<KeyType, ValueType>::const_second_reference 
                   const_value_reference;
  typedef typename Pair<KeyType, ValueType>::first_reference key_reference;
  typedef typename Pair<KeyType, ValueType>::const_first_reference 
                   const_key_reference;


  typedef ArrayStub<MyType, ArrayRefStub>  value_stub;
  friend class ArrayRefStub<MyType>;
  friend class value_stub;
  typedef typename Policy::WeightType WeightType;

  typedef CountWeightPolicy<MyType> DefaultWeightPolicy;

public: 

  /* Now, the CRTP buggers. Public so that you can define your own 
   * policies. */

  size_t sizeByCRTP() const {
    //std::cerr << "CRTP:" << ((Index *) this)->getTableSize() << "\n";
    if (!base_empty()) {
      return ((Index *) this)->getTableSize();
    } else {
      return 0;
    }
  }

  size_t elemsByCRTP() const {
    if (!base_empty()) {      
      return ((Index *) this)->size();
    } else {
      return 0;
    }
  }  

protected: 
  
  /**
   * This uses CRTP, but is prtected because else you could break something.
   */
  bool isUsed(const size_t i) const {return ((Index *) this)->isUsed(i);}  

private: 
  /** Only for the stub. Your friends can access your private parts. */
  value_reference directRefToValue(const size_t slot) {
    assert(slot < sizeByCRTP());
    return refTo(slot).second();
  }

protected:

  ValueTable(const size_t size=0):super(size) {}

  value_reference refToVal(const size_t loc) {
    //assert(loc < sizeByCRTP());
    return refTo(loc).second();
  }

  const_value_reference constRefToVal(const size_t loc) const {
    //assert(loc < sizeByCRTP());
    return constRefTo(loc).second();
  }

  key_reference refToKey(const size_t loc) {
    //assert(loc < sizeByCRTP());
    return refTo(loc).first();
  }

  const_key_reference constRefToKey(const size_t loc) const {
    //assert(loc < sizeByCRTP());
    return constRefTo(loc).first();
  }

  WeightType weightAt(const size_t i) const {
    return Policy::getWeight(constRefTo(i).second());
  }

  /** Needed only by the implicit table */
  
  WeightType & refToWeight(const size_t i) {
    return Policy::refToWeight(refTo(i).second());
  }

  void moveOrSwap(const size_t to, const size_t from) {
    assert(to < sizeByCRTP());
    assert(from < sizeByCRTP());
    super::copy(to, from);
  }

  void swapValues(const size_t to, const size_t from) {
    /* IN order to avoid destructors etc... */
    char tempVal[sizeof(ValueType)];
    assert(to < sizeByCRTP());
    assert(from < sizeByCRTP());
    memcpy(tempVal, &refToVal(to), sizeof(ValueType));
    memcpy(&refToVal(to), &refToVal(from), sizeof(ValueType));
    memcpy(&refToVal(from), tempVal, sizeof(ValueType));
  }

  void removeVal(const size_t loc) {
    //std::cerr << "Removing...";
    refTo(loc).removeSecond();
    //std::cerr << "Done.\n";
  }

  void clearVal(const size_t loc) {
    //std::cerr << "Clearing.";
    refTo(loc).clearSecond();
    //std::cerr << "Done.\n";
  }

  
  
  /** This class does not need clearing as such. */

  void remove_stage_1(const size_t loc) {
    removeVal(loc);
  }

  
  void remove_stage_2(const size_t loc) {}

  /** 
   * Destroying the container. This should be OK for all the bases, too. 
   */

  void final_remove(const size_t loc) {
    //std::cerr << "Final...";
    refTo(loc).~Pair();
    //std::cerr << "Done.";
  }


  /* No need to specify the stub more accurately. */

  void swapVals(const size_t to, const size_t from) {
    ValueType tempValue;
    memcpy(&tempValue, &directRefTo(to), sizeof(ValueType));
    memcpy(&directRefTo(to), &directRefTo(from), sizeof(ValueType));
    memcpy(&directRefTo(from), &tempValue, sizeof(ValueType));
  }

  /* The swap itself is inherited. */

  size_t weighedSlotSelect(WeightType val) const {
    assert(!base_empty());
    //std::cerr << "WSS. Val:" << val;
    size_t i=0;
    while (!(val < weightAt(i))) { /* Clearer this way. */
      //std::cerr << "Skip " << i << "with w:" << weightAt(i);
      val-=weightAt(i);
      //std::cerr << ", Val left:" << val << "\n";
      ++i;
      assert(i<sizeByCRTP());
    }
    return i;
  }

  /** Nothing to do. */
  void assemble() {}
  /** Nothing to do. */
  void disassemble() {}

  bool localLegal(const size_t i) const {return true;}

  void setValue(const size_t loc, const_value_reference value) {
    refToVal(loc)=value;
  }

  bool isLegal() const {return true;}

  void pushAt(const size_t loc) {
    assert(loc < sizeByCRTP()+1);
    super::pushAt(loc, sizeByCRTP());
    //clearVal(loc);    
  }

  void pushToTail() {
    super::pushAt(sizeByCRTP(), sizeByCRTP());
  }

  void pullFrom(const size_t loc) {
    assert(loc < sizeByCRTP()); 
    //weightSum-=weightAt(loc);
    super::pullFrom(loc, sizeByCRTP());
  }
  
public:
  
  WeightType weight(size_t len) const {
    WeightType retval=WeightType();
    //std::cerr << "Weights:\n";
    for (size_t i=0; i<len; ++i) {
      retval+=Policy::getWeight(constRefToVal(i));
      //std::cerr << i << ":";
      //std::cerr << Policy::getWeight(constRefToVal(i));
      //std::cerr << "\n";
    }
    return retval;
  }

  WeightType weight() const {return weight(sizeByCRTP());}

 //  template<typename RandSource>
//   size_t weighedRandSlot(RandSource & src=globalRandSource) const {
//     WeightType value=src.next(this->weight());
//     return weighedSlotSelect(value);
//   }

  /**
   * In order to avoid unnecessary calcs when the table is not full. 
   */
  
  template<typename RandSource>
  size_t weighedRandSlot(RandSource & src=globalRandSource) const {
    WeightType value=src.next(this->weight());
    return weighedSlotSelect(value);
  }
  
 
};

#endif
