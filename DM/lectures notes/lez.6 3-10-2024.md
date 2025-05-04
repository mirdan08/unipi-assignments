# k-means(cont.)
Initial setting is quite important to reach optimal solutions for the clusters centers initial values.
understanding quality of clustering is quite difficult you can't only use the metric e.g. changing from manhattan distance to euclidean changes results and their meaning.

K means is better w.r.t. hierarchical clustering for matters of time complexity.

Outliers are difficult to deal with beause outliers might move the centroid having lower overall loss values.

different densities might yield strange behavior for k-means, also clusters might have very different size.

it cannot capture shapes that are non globular e.g. elongated sets.
![[Pasted image 20241003142906.png]]
question: volume or number of points ? -> number of points because we don't have any statistic for clusters of say 2-3 points.
![[Pasted image 20241003143014.png]]
data should be somehow grouped but if data points are all mixed up it is difficult to find clusters.

in this image you can see that the clustering is influenced by the sparseness of the data in the original the red one is big but sparse hence we might have clusters that are subdivided.

K-means is not applicable in such cases but you can do that and then apply post processing after applying some output, one exampe could be to merge clusters that are very close.

![[Pasted image 20241003143508.png]]

a merge heuristic could be to pick the clusters to have close points (not centroids just the point in a cluster that is closest to a point in another cluster).

Main problem: how do i pick K?

![[Pasted image 20241003144424.png]]

high K yield very low SSE with no semantic behind it, however it is useful to run post-processing to find meaning full lower ks clusters using the previously found clusters.

![[Pasted image 20241003144435.png]]

Basically increase K and merge what is very close.

![[Pasted image 20241003144908.png]]

In this case the most corret approach would be to find a clustering with high K's then use the contiguity strategy to merge clusters.

Question : i cannot visualize the data how do i get if i have this? use PCA or watch SSE and observe well separation.

We might get empty clusters i.e. K with some clusters not assigned.

(rewatch empty clusters slide)

pre-processing comprises of normalizing data,duplicate elimination and outliers elimination.

post processing can be elimination of small clusters or the splitting

![[Pasted image 20241003150713.png]]

Note: MIN,MAX and average are equivalent there is no worst case between them!

Note: for distance function always use the MIN one! for similarity use MAX !