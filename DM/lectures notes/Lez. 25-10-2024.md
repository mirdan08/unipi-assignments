# Sequential pattern mining

Some times we don't think of items a s sets but as sequence of sets 
![[Pasted image 20241025092027.png]]
This gives us some sort of sequence like historical purchases of a customer,sequence of events.

We can also analise at different granularity we pick e.g. we have pages that can be taken by day so we have sets of days or by timestamp creating a single set sequence.

We could try to understand frquent patterns acoss sequence for a definite set of items.

![[Pasted image 20241025092611.png]]
in this case we might care about patterns frequent across items or across items.
![[Pasted image 20241025092721.png]]
in this case instead we care about sequences for custmoers

i can extract sequential patterns for each customer.
![[Pasted image 20241025092825.png]]
in this case we can see that we have a recurring sequential pattern.

users have different behaviors even with a recurring pattern so a pattern can repeat across users in this case we don't care about an exact sequence of transaction but at the fact that an item is bought after another one regardless of their "distance" across.

![[Pasted image 20241025093101.png]]


![[Pasted image 20241025093533.png]]

eahc itemm inside the events across itme is counted as a different item , a sequence that contians k items regardless of repetition is a k-sequence.
![[Pasted image 20241025093916.png]]
repetation across timess of same elemtns e.g. buying bread each tie is interisting , we need temporal behavior not just once in a time events e.g. buying 10 tomatoes.

we might care about less freqent pattern e.g. i don't buy every time but i do on particular days like one per week or once a year.

![[Pasted image 20241025095123.png]]

Given a sequence we can define it to be a subsequence however to be such we need to make sure.

- we can have multiple people that can arrive together.
- lower position yields high delta (i.e. if you are before someone else in position you should have lower delta).
- we can assume by default that there is some tarmac (false,false false) case.
- we have different cyclist in the same position.
- we have different categories of races
- check if have ME and not ME races for the same years we can assume them to be the same years.
- points are warded from the cyclist that gets most than go down
- No person correlation for climb_total,profile look at Kendall-Tau and Spearman correlations.
