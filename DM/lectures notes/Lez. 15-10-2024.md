# Anomaly detection

basically something is not like other values e.g. 100 values only one is positive the other are negative the positive one is an outlier.
they are are fuzzy meaning the have degree of outlierness that ccan be thresholded.

They depend  on data since they are exceptions of it while outliers themselves define it

THEY ARE NOT NOISE, they are exceptional not random.

they can be mono or multidimensional , they can be outliers for one dimension or on multiple.

we can use a normal deistribution to define outliers by defiing a meanand a variance and calculating the value.

Sometimes we have unusual values sometimes we have extreme values.
- unusual means values outside our mode
- extreme means: not unusual but very above the mean

approac
- grading meaning define a function to quanitfy a sddegree of anomaly
- thresholding define a second function to map a deggree to a binary label.
![[Pasted image 20241015113233.png]]
It can be local meaning it is outside a local distributions or the global distribution.

is it sensible to some heavily impacted data with characteristics.

how interpretable is an instance of an outlier.

Three ways of defining outliers
![[Pasted image 20241015113546.png]]
the distribution way is the more natural, the data manifold is a function of he shape of the data.
Finally neighborhood way try to describe outliers w.r.t. neighbors.
![[Pasted image 20241015113943.png]]
**The +1 problem: how many outliers can i introduce in the data beofre they aren ot otuliers anymore?**

basically adding or removing extremes changes distribtution and also outliers then.

I ca keeping going on by removing ieratively outliers and seing how many outliers i can remove by "peeliing layers of outleirs"

This process is caputers using the grubbs test
![[Pasted image 20241015114447.png]]

![[Pasted image 20241015114654.png]]

## mixture models
We could model locally basically instead of relying on global statstics we could use for different subsets i.e. for example we coudl use extreme cases to model different kind of outliers.
![[Pasted image 20241015115130.png]]
![[Pasted image 20241015115432.png]]
thresholding distributions can be arbitrary.
![[Pasted image 20241015115740.png]]
(listen recording)

![[Pasted image 20241015120405.png]]
## data manifolds

