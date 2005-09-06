#ifndef LCE_NNET
#define LCE_NNET

#include"../Containers/Indices/LinearHash.H"
#include"../Containers/Tables/ValueTable.H"

template<typename Synapse> 
class Presynaptic {
  
}

template<typename Synapse> 
class Axon: public Array<size_t, Synapse>


template<typename Soma, typename OutSynapse, typename TimeType=size_t>
class Neuron: public Array<size_t, Pair<TimeType,Synapse> > {
protected:
  AutoMap<size_t, size_t> dendritic;
  Soma soma;
}

template<typename GenCell, typename TimeType=size_t>
class SpikeNet: public Map<size_t, GenCell,  


#endif
