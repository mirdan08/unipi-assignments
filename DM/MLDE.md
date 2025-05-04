# Dynamical ensemble learning for multi-label classification

Most existing ensemble learning methods don't consider the difference between seen and unseen instances, and other methods that do don't explore label correlation.

MLDE (Multi-Label classification with Dynamic Ensemble learning) tries to tackle these issues, it selects the most competent ensemble of base classifiers to predict each unseen instance.

For multi-label problems we combine classification,accuracy and ranking loss which measure competence of the base classifiers:
- accuracy can discriminate the ability to differentiate of each classifier w.rt. different labels.
- ranking loss focuses on the overall performance of a classifier on the label set and fully considers the correlation between multiple labels.


## introduction

There are a lot of methods that tackle such problems mainly two categories:
1. Problem Transformation : transform multi-label into single-label, Binary Releveance , Classifier  Chains and Label Powerset. The ignore the effects of multiple labels and have limited performance.
2. Algorithm Adaptation : They extend  a specific single-label method to it's multi-label version and lack generality.
### the responsibility problem
Ensembles in general can enhance multi-label algorithms, a group of researchers tried to use multiple base learners with  diverse generalization and label correlation capabilities and combine them with bagging, with this method generalization improves but base learners all have the same weight even when they don't  give the same contribution.

This contribution problem a method proposed was to learn the distribution of weights using a sepcified objective function. Further development to this method has been done by extending AdaBoost to multi-label classification and realize another weighted ensemble.

All these methods obtain and additive model in the end but don't manage to distinguish differences between seen ad unseen instances.
### explanation to the responsibility problem
The problem is that once models are induced and weights learned the architecture is fixed, we need a dynamical approach like dynamical single label ensemble methods that select each time the most competent classifiers using a competence measure like accuracy.

The main point is that each model is specialized in  a sub-part of the feature space if contributions are equal the mixing of models cannot work well,  so assuming that a learner performs well on instances similar to the ones already seen. 

previous dynamical ensemble models:
- DIBR works for multi-label concept drifting data but doesn't take into account the correlation between labels during classifier selection.
- DECC generates CC classifiers for unseen instances with limited label correlation capabilities.
# MLDE
MLDE uses dynamc selection methodolog for some specific multi-label tasks.

The number of labels poses two problems:
1. the measure tends to ignore the difference between them
2. the measure decomposable ignores the correlation between them.
MLDE tackles this using the accuracy on the single labels for each classifier adthe ranking loss for a set of labels that are combined together.
## classifiers ensemble evaluation
1. accuracy for each classifier we evaluate separately on each label to distnguish differentiation capabilities.
2. ranking loss it used to measure the opposite relation between positive and negative labels
3. A lot of metrics are choosen to evaluate models here we use a noel mechanism to integrate outputs of all classifiers in a unified conclsion.

**Summary**:
- MLDE dynamically selects competent base classifiers for different unseen instances, and provides more promising prediction
results.
-  MLDE proposes a novel classifier selection approach specific to multi-label problem, considering classification accuracy on individual labels and ranking loss on label set simultaneously, which significantly improves the model accuracy and its ability on label correlation exploiting.
-  It provides a novel integration mechanism for multi-label dynamic selection learning that fuses the outputs of base classifiers selected by different measurements.

# related work
### notation
given:
- $X$: the d-dimensional feature space
- $Y$ being the labels set,and $D$ the the multi-label training set, with $y_i$ being a vector of labels in the labels space belonging to $x_i$.

# the method
The method is made three steps
1. base classifiers generation
2. base classifier selection
3. base classifier aggregation
![[Pasted image 20241206152548.png]]
## base classifier generation
We prefer simple classifiers for this reason we use BR classifiers, BR given $q$ values makes q single label binary classifiers. 
BR uses 4 kids of algorithms:
- logistic regression
- decision trees
- bayes networks
- SVMs

using the bootstrap method the datasets are resampled from $D$ in  order to train the base classifiers.

Now we have a classifier pool $P=\{C_1,\dots,C_r\}$ with each $C_i$ being one the classifiers. 

so assuming $P$ to be  generated we continue to the selection of classifiers.

Note:See what bootstraping is to be sure.
## classifiers selection

our new instance is $x_{new}$ .

Remember that we want diversity and accuracy. Diversity is obtained by training different single-label ad then selecting the most .

Selecton imples evaluation and to evaluation we need some metrics, we have three:

**Single label accuracy** for class $Y_j$
$$
\frac{1}{m}\sum^m_{i=1}I(h_j(x_i)=y_{ij})
$$
**Multi label accuracy** for all labels with equal weights

$$
\frac{1}{mq}\sum^m_{i=1}\sum_{j=1}^qI(h_j(x_i)=y_{ij})
$$
**ranking loss** explores opposition correlation between positive and negative labels 

$$
\frac{1}{m}\sum_{i=1}^m \frac{|\{ Y_j,Y_k \}f_k(x_i),y_{ij}=1,y_{ik}=0\} |}{|y_i|(q-|y_i|)}
$$
the lower term in the fraction is the multiplication of positive ad negative labels w.r.t. ground truth of $x_i$ , such a loss function achieves good result when the number of correctly predicted values is greater than 

### Single & multi label accuracy

it's used to evaluate a base classifier accuracy for single labels in unseen instances , it s needed since we might have cases where classifiers are unbalanced in precision for single labels and have very unbalanced scores that make for a high multi-label accuracy while having unbalanced single label accuracy making it harder to evaluate which labels are less discriminated.

## Ranking loss

we analyze the correlation between different labels using or ranking loss as above defined to analyze significant correlations between labels. 

Here is an example of the differences we highlighted before
![[Pasted image 20241207122909.png]]


## competence measure calculation

he competence measure is computed in a three step process:
1. for a test instance $x_{new}$  we pick he first $k$ neighbors noted as $N_k$ 
2. compute for a base classifier $C_i$ the metrics of the neighbors using ranking loss and single label accuracy and expect the well performing classifiers to perform well also on $x_{new}$  specifically we gather first $r$ most performing classifiers according on single-label accuracy and the top 50% of the most performing classifiers in the ranking loss 
   ![[Pasted image 20241207122947.png]]
   Here is a rapresentation of the selection part, next we have the aggregation
3. during selection we select $q$ pools of base classifiers $P_{acc}^1,\dots,P_{acc}^q$ with the single label metric and $P_{rank}$ using the ranking loss.
   For the j-th label of $x_{new}$ we have a prediction madre by each base classifier in $b$ which is an array of booleans and a real valued vector $r$ which are mixed together:
	- sum them up $br=b+r$ (b values are convertd to real valued integers 1 and 0)
	- normalize $br$ 
	- threshold $br$ with 0.5 to obtain the voting and put 1 if above 0.5 or 0 in the other case.
	- use the values in majority voting and get the final result
  Here is the pseudocode for the algorithm
  ![[Pasted image 20241207125552.png]]
# experimental studies
