# A library for network analysis (and some containers)

A simple undirected networks class is known as SymmNet. It is a template class that takes the type of data stored to edges as a template parameter. Following are probably the most useful in the beginning:
```
SymmNet<bool> net(size); // unweighted network
SymmNet<float> net(size); // weighted network (with float as a weight)
```

If you need to sample nodes from the network so that the sampling probability is proportional to the degree of the node or strength of the node (sum of the weights of edges attached to the node), then you should use the following:
```
SymmNet<*, ExplSumTreeTable>
SymmNet<*, ExplSumTreeTable, WeightSumTable> 
```

The first one uses degree as the sampling weight and the second one uses the strenght as the sampling weight. This is just a guess of what the user wants: if the weight of a node is stored explicitly (the latter option) the library assumes that you want to use it as a weight for the sampling. However, everything can be customized and the sampling weights are in the end produced according to a policy class. Note that * is replaced with the type of the edge data.

You can access the nodes of the network using the [] operator, which takes as parameters normal unsigned integers. For example:
```
net[i]
```

There is also an iterator, but you probably won't be needing it for networks of fixed size. Generic algorithms in the library of course use iterators, because we want them to be independent of the implementation of the network.

You can get the const reference to a node by using the () operator. This can be useful from time to time; although everything should also work with normal references. The const reference is a direct reference to the data structure describing the neigborhood of that node.

You can access the edge from node i to node j with the following notation:
```
net[i][j]
```

You can then do what ever you want with this edge: =, +=, method calls through the "->" opeartor etc. are all implemented. The network is kept symmetric (for SymmNet) automatically and all the auxiliary data structures are kept valid. If you set the edge to zero (or, in general, to default value) then the edge is automatically removed from the network. When you query for an edge that doesn't exist you are returned zero (or, in general, the default value).

The following give you a const reference to an edge:
```
net(i)[j] 
net(i,j)
```
These will also return the default value if the edge doesn't exist.

The edge iterator works as follows:
```
for (NetType::edge_iterator j=net[i].begin(); !j.finished(); ++j) {
   edge_destination=*j;
   edge_weight=j.value();
   j.value()+=1; // etc.
}
```

As you can guess from the previous example, "value()" givesyou a reference to the weight of an edge. The *-operator returns only const references: You have to move edges "manually" by copying the edge and then setting it to zero. Iterators also automatically make sure that invariants in the network are taken care of.

The "NetType" in the previous example is of course replaced with the type of the network, for example with SymmNet<bool>. You might want to add the following line to the beginning of your code:
```
typedef SymmNet<bool> NetType;
```
so you don't have to repeat yourself and to make it easy to change the implementation.

You can sample a random node with the following command:
```
node=net.weighedRandSlot(randgen); 
```
This returns a number: you can get a reference to the node with the [] or () operator if you want to access the node object (see above).

## Examples

Clustering coefficient of node i:
```
// Iterate through neighbors of i
for (NetType::const_edge_iterator j=net(i).begin();
     !j.finished();
     ++j) {
   // Iterate through neighbors of neighbors. 
   // Note that edge *j->i is not filtered out. 
   for (NetType::const_edge_iterator k=net(*j).begin();
     !k.finished();
     ++k) {
     if (net(*k)[i] != 0) sum++;    
}
result=((float) sum)/net(i).size() / ( net(i).size()-1 ); 
```
If the network is unweighted you can replace the if statement in the above code with 
```
sum+=net(*k,i);
```

The average degree of neighbors of node i:
```
size_t sum
for (NetType::const_edge_iterator j=net(i).begin();
     !j.finished();
     ++j) {
   sum+=net(*j).size();
}
result=((float) sum)/net(i).size();
```


