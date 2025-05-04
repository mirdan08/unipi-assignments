# Data representation

When we get data it is raw we need to manipulate it for our purposes.

Three types of representation
- by correlation : according to correlation
- by neighborhood : i want to represent it in terms of similarities
- by manifold : we want to represent the data so that its manifold is preserved
## Principal Component Analysis
a.k.a by correlation

take the vvector space that can be generateed using a base of vectors *u* and *v* with coordinates alpha_u and alpha_v
![[Pasted image 20240924111957.png]]
Eigenvectors defined where data "moves" for each direction we have and eigenvalues that tells us how much they get stretched.

taking advantage of the spectracl decomposition we can get redefine data using the decompositon this gives a way of observing data.
PCA leverages these ideas to model linear dependence between features.

We want to disentagle such relationships
![[Pasted image 20240924113635.png]]
on the diagonal we have variance for each dimension and also allows us the measure covariance and the slope through the matrix multiplication $X X^T$ .

![[Pasted image 20240924115259.png]]
When collapsing to lower dimensions we would like to make sure to not just follow a direction but maintain spatial relationships e.g. clustering , distances and so one otherwise we get  overlapped items.

## Stochastic neighbor embedding

define  a similarity phase first in the original space and the map in a second space.

### similarity phase
question how are similar two items?we define with probability that :
we use a distribution using  a normal distribution 
![[Pasted image 20240924115805.png]]
use a neighbor  as a mean and sample probability of the other according to such distribution then observe a probability. in this case use a gaussian.
we can repeat the same process the other way around and take their mean as a final estimation.
variance is to be defined as a parameter for our algorithm, we can observe the classic "stretching" that comes from higher variances
![[Pasted image 20240924120156.png]]
![[Pasted image 20240924120221.png]]
NOTE : for the report make sure to use different perplexities and show extensive is the research in hyperparams values.

## uniform manifold representation
