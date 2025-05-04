# laboratory

in the report e have to think about what our results mean and must be meaningful.

can i estimate the compactness? -> look at the leaf i f i have as much leaf  as  i have transactions the compactness is low but thee is no firesure way of making an estimate(?)

# classification

in decision tree we can create subgroups that get represented using some statistical tests and create a tree like representation
![[Pasted image 20241105121504.png]]
we might get a subset of values that  can classify correctly using just that, if the certainty is high then we can employ a subdivision and see if we get a more certain accuracy.

however could have multiple decision trees on a certain dataset, 
## Hunt's algorithm

We can take advantage of the tree to dirscretize however the producted ranges on the continuous cvalues will be treated a   orinala values otehrwise we lose the meaning of the discretization and the ranges.

when splitting be careful, e.g. 
![[Pasted image 20241105125032.png]]
basically if you split on customer id you get a meaningless spittting it is the best when evaluated however it is also useless since when i have  new customer i cannot estimate isince im usinng the id which is unique and doesn't allow generalization.


