#ifndef LCE_EMB_BOOL_SELECTOR
#define LCE_EMB_BOOL_SELECTOR
#include <utility>

template<bool sizeEq> 
struct SelectHelper {
  typedef bool type;
};

template<>
struct SelectHelper<false> {
  typedef char type;
};

template<typename DataType>
struct EmbBoolSelector {
  typedef SelectHelper<sizeof(pair<DataType. bool>) 
    == sizeof(pair<DataType, char>)>::type type:
}

#endif
