# Data cleaning

Different features different sets of features -> different results.

Anomalous values might be missing like NULL or ? , unknown without any real meaning and invalid values.

Use values that are outside of the normal domain to symbolize unknown values e.g. -1 for ages 0 to 99.

How do i manage missing values?

1. elimination of records
	- for lots of missing values i have a meaningless statistical analysis it is better to eliminate records
2. substitution of values
	- Otherwise a nice alternative is to use substitution values, we can infer values and a join operation to complete your data without approximations ! e.g. infer users gender from their twitter profiles, alternatively we can do some statistical inference for values you could use the mean or the mode for non numerical values use mode. 
	-  We try to maintain the distribution of the column  instead of brainlessly of substituting a single values that alter the statistical significance of the feature distribution.
	- You can build a model to predict missing values 
Discretization makes it easier to approximate normal shape which helps algorithms and reduces sparseness of my data.
![[Pasted image 20240926143158.png]]
Binning can destroy eventual patterns, e.g. defining wrongly thre intervals for binning and havinng all data end up in the same interval 
![[Pasted image 20240926143338.png]]
How do i pick intervals? with some domain dependent criterion
![[Pasted image 20240926143420.png]]
![[Pasted image 20240926143521.png]]
a simple example is natural binning however it can generate very unbalanced.
