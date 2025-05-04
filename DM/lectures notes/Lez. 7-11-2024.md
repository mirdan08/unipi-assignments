We need to be able to determine the best local decision upon a split to avoid useless splitting of nodes.
# determine the best split
the greedy approach is based on nodes with a more purer / homogeneous class the purity for us the omegeneity of the classes upon choosing a certain split
![[Pasted image 20241107142124.png]]
![[Pasted image 20241107142208.png]]
These are the most used ones
as ofr isclassifcation it measures how much error i can make applying  certain model by seeing the most miscalssified classes n the above example we get 1-1/10 if you pick the right one.
Entropy is well known and gini is just the miscallsifcation error bbut with apower of two.
![[Pasted image 20241107142607.png]]
How do i find the best split? i start pick the splits i want to evaluate then is i compare the measures above before the split like A and B above.
![[Pasted image 20241107142651.png]]

you can calculate the measures and split , mrge the into a siingle one and then calculate the Gain by taking the difference
![[Pasted image 20241107142735.png]]

here is an example for andode t

![[Pasted image 20241107142830.png]]
the GINI index 


for a two cclass problem we have this landscape for the values that get calculated
![[Pasted image 20241107142906.png]]

all relationship still maintain consistency for the values fo the probabilities used

Now we have the measures for a split wee need to alcualte the values for a selection of nodes

![[Pasted image 20241107143137.png]]
![[Pasted image 20241107143316.png]]
remmber that featues with a lot of values also mmeans a lot of complexity , we also have to keep in acccount hte nuber of samples per values / class tht could be meaningless in case of lowe frequency we could use discretization guided by frequency using also measures like gini index and apply some preprocessing.

when doing a binary values we just pick threshold , in this case we need different threshold to create ranges.

a way to do so is to test aany possible split and test the measures s above specified.

![[Pasted image 20241107144017.png]]

![[Pasted image 20241107144122.png]]


The GINI measure can be applied on splitted nodes and weights the result w.r.t. the occurency of the values in the node same gose for the entropy.

usually higher nummber of childrens are preferred but we also havve too much complexity like that some algorithm try to balance this accitng like regularization taking in to consideration the number of partitions

![[Pasted image 20241107144419.png]]

## determine when to stop splitting

we must evaluate the goodness of our model to employ  stopping criteria.

we can decide to :
- stop expanding a node when all records belong to the sae calss
- stop when attribute becomes "similar"
- stop before reaching single class leaves (?)
![[Pasted image 20241107144955.png]]
being irrelevant we get evenly splitted class which makes have very low values in the mesures mening gain is low (low discriminative power) hence it won't be selected a s much(low purity with splitting).

when testing splitting conditions we shoould gain similar purity and the algorithm has an approach that chooses one ove the other e.g. pick the first one given and ordering.

.......

we can keep into account also the missing values and use that to calculate the split, remember that the impurity takes into account missing values if computed like normal the missing values have weight that makes it sum to one be can be computed anyway.

we can yutn the already present values probbilities and then pick the missing values attribute usnig such probability, it is sotchastic process of course.

![[Pasted image 20241107152825.png]]
Disadvantages
![[Pasted image 20241107152959.png]]

........

We might get the same subtrees for different branches , in this case 

INtersting note : oovercomplicated trees might be more complex and etter at generalizing (if they are not in oerfitting) however simpler trees are much more interpretable.