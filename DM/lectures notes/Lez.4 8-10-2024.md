We can have multiple comparitive measures to enable comparative results between to or more clusters, clusters can be obtained varying parameters or using certain features.

we can evaluate with correlation use a matreix to understand if ocrrelations make sene, we have similarity matrix or a proximity matrix. For any parise of data comupote the proximity then compute the ideal simmilairty matrix , each entry is a 1 if the belong to different clusters and 0 if they don't.

we compare the ideal similarity matrix and the proximity matrix high correlation means clusters in the same centroid are close but it useless for density based methods.

You can generate different random data  apply k-means and apply the correlation you should get completely different results

![[Pasted image 20241008112508.png]]

The ccorrelation is better if we are capturing actual partners.

![[Pasted image 20241008112548.png]]
For sensed correlation we get well separated clusterrs and we should thuis obtain a correlationh matrix between points in blocks a per the upper image

![[Pasted image 20241008112623.png]]

Using DBSCAN we can get this clustering which suports our problem from before

![[Pasted image 20241008112640.png]]

we can see here that using k-means doesn't get correct values 

we can compare also the strategies between eachother.

.............

ther analysis that we saw during data undesrtanding should bne focued during clustering, if you find some meaning to clusters.

We do not aim to characterize all clusters. Instead, we will generate a table displaying different combinations of clusters and select the one that is most suitable for further analysis. Only the selected cluster will undergo detailed characterization.

