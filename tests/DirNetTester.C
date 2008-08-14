#include<iostream>
#include<cstdlib>
#include"../Nets.H"
#define DEBUG
#include<cassert>

using namespace std;

typedef int DataType;
typedef WeightPair<DataType> WeightType;
typedef DirNet<DataType> NetType;

int main() {

  // Create a new net of fixed size.
  int netSize = 3;
  auto_ptr<NetType> netPointer(new NetType(netSize));
  NetType& net = *netPointer;
  
  // This would trigger a bug that exists already in SymmNet.
  // See tests/SymmNetBug.C for more info.
  //cerr << net[0][1] << " " << net[1][0] << "\n";

  // Basic testing:
  net[0][1] = 10;
  cerr << "===== Output: " << net[0][1] << " " << net[1][0] << "\n";

  //net[0][1] = false;
  //cerr << "===== Output: " << net[0][1] << " " << net[1][0] << "\n";

  net[1][0] = 12;
  cerr << "===== Output: " << net[0][1] << " " << net[1][0] << "\n";

  net[1][0] *= 0;
  cerr << "===== Output: " << net[0][1] << " " << net[1][0] << "\n";
  /*
  net[0][1] <<= 1;
  cerr << "===== Output: " << net[0][1] << " " << net[1][0] << "\n";
  */
  net[0][1]++;
  net[1][0]++;
  cerr << "===== Output: " << net[0][1] << " " << net[1][0] << "\n";

  DataType val = net[0][1] + net[1][0];
  cerr << "val = " << val << "\n";

  // Test iterator:
  net[0][1] = WeightType(1,10);
  net[1][2] = WeightType(12,21);
  net[0][2] = WeightType(2,20);
  /*
  net[0][1] = WeightType(0.1,1.0);
  net[1][2] = WeightType(1.2,2.1);
  net[0][2] = WeightType(0.2,2.0);
  */
  /*
  net[0][1] = WeightType(true,false);
  net[1][2] = WeightType(true,false);
  net[0][2] = WeightType(false,true);
  */

  DataType total_weight = 0;
  for (size_t i = 0; i < net.size(); ++i) {
    for (NetType::edge_iterator j = net[i].begin(); !j.finished(); ++j) {
      cerr << "From " << i << " to " << *j << ", weight = (" << j.value().out() << ", " << j.value().in() << ")\n";
      total_weight += j.value().out();
    }
  }
  cerr << "Total weight: " << total_weight << "\n";

  /*
  WeightPair<DataType> wp;
  wp.out() = 6;
  wp.in() = 34;
  cerr << wp << "\n";
  wp = 8;
  cerr << wp << "\n";
  wp.out() = 0;
  wp.in() = 0;
  cerr << wp << "\n";
  cerr << WeightPair<DataType>() << "\n";
  assert( wp == WeightPair<DataType>() );
  */

  cerr << "Done!\n";
}
