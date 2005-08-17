#ifndef LCE_NNET
#define LCE_NNET

#include"../Containers/Indices/LinearHash.H"
#include"../Containers/Tables/ValueTable.H"

struct Presynaptic {
  size_t target;
  size_t delay;
}

template<typename NeuronType> 
struct NetTriple {
  NeuronType neuron;
  Array<Presynaptic> axon;
  LinearHash<NeuronType::SynapseType> dendrites;
}

struct AxonPolicy:public MapContainerPolicy<size_t, size_t> {
  static const size_t MagicEmptyKey=UINT_MAX;
};

struct AxonPolicy:public DefaultContainerParams {
  typedef void StatusPolicy;
};

class Axon:public Map<size_t, size_t, Array, 

template<typename NeuronType, typename TimeType=size_t> 
class NNet {
  
}

#endif
