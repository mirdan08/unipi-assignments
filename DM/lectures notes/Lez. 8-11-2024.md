Repeats ML stuff about validation and training.

![[Pasted image 20241108091530.png]]
Here is an exampe of the ML pipeline we should use.
## perssimitic error estimation
We have al ot of ways to evaluate the model
pessimistic approach : we have Pessimistic Error Edtimate
![[Pasted image 20241108091923.png]]
this allows us to minimize the loss i.e. learn and obtain the generalization

in this case we might decided to have limited amount of leaves to avoid having high errors.

![[Pasted image 20241108092200.png]]
Here we have on T_L 4/24 errors on the right one we have less errors, on teh left but the second term of my estimation we add the penalty
we find taht the T_L gets the higher error beacuse of it's complexity.

## optimistic error estimation

We could use minimum description length, we use the number of used for the encoding the misclassifcation error + a cost the uses the node encoding (# of chidlrens).

In our case we assume binomial distribution.

## rule-based classifiers

