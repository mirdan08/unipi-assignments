# data understanding
We assume to collect data from different sources but first we need to understand what kind of data we are dealing with.
We can have
records in a matrix like fashion,graphs,....
![[Pasted image 20240920091606.png]]
We cannot apply any kind of distance here but we preprocessing or we can create an attribute vector using a term document matrix.(Like in HLT)
Transacction data insteadd stores sets of items for each object however here we don't have a matrix since a the sets do not have a fixed length unlike term document matrixes.
Graphs structures of data.
we have sequences of data that are sequences of sets. we don't care about the order inside the sets while we care only about the order of the sets.
we have ordered data that has single elements with single ordered elements. e.g. spatio-temporal data.
We can have different kind of types for each attribute:
- nominal categorical an attribute of values within a finite domain
- binary : can be symmetric binary or asymmetric basically the values can have equally important values or one might be better than the other
- ordinal: a domain with a meaningful ordering
we can have algorithms that have better performances for certain type and hybrid types like a nominal and ordinal.

Numerical attributes can quantity

NOISE= WRONG data that doesn't report the correct value
using a median is more robust w.r.t. outliers that make data distribution skewed.

Feature engineering