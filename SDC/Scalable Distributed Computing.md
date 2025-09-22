From the course name we start discussing the basic concepts:
- Distributed is the spreading of tasks and resources across multiple machines/locations. This should enhance resilience and efficiency using decentralization.
- Scalable is the ability to grow the size of a system to handle increasing workload without compromising performance. In this case efficiency DOES NOT GROW but is maintained while the system scales while maintaining efficiency.

Distributed computing involves the studies of systems where multiple autonomous computers communicate over a network to achieve a common goal, it's main characteristics are:
- decentralized architecture
- resource sharing
- scalability and fault tollerance
In this discipline there are a few key concepts:
- nodes: indipendent  entities in the system, e.g. a computers/server
- communication: the communication method like exchanginig mesages RPC etc.etc.
- synchronization: ensuring coordination between nodes
- fault tolerance: resilience against node failure
- scalability: ability to handle growing workload effectively
From this list of concepts we focus on scalability, in particular such concept can be specialized:
- Vertical scalability:Adding more  power to a single node
- Horizontal scalability:adding more nodes
A scalable system has:
- performance: maintain efficiency as demand increases
- elasticity: scale up or down based on current needs

Scalability is needed since we have growing data/number of users and the necessity to keep efficiency with low latency,high through put and improved reliability. As we know these kind of system are distributed geographically and low latency and high availability in such context is difficult to ensure.

Scalability comes in two variants:
- Strong:More resources means faster execution,without incrementing  the workload per node. Ideally his should be done for systems where we can handle twice the load with the same efficiency.
	- Ideal when workload can be evenly distributed across many nodes.
- Weak:Performances don't degrade as the number of users/workload increases. Performance increases as the system grows but only when the workload per node increases proportionally to the resources added. Good for systems where the workload grows alongside the system's resources.
With this kind of system we target HPC,cloud and distributed architectures and IOT.

The main differences from parallel computing are that we have once computer against multiple, multiple processors against multiple computers that multiple operations. In the distriuted field we only have distributed memory as in paralle computing we have also shared memory. The data exchange hjappens with a bus while on the distributed filed we only have message passing.

## Architectural styles elements and challenges

For now we can deal with a simplified definitio of a distributed system:
- A distributed system is a collection of computing elements that appears to its users as a single coherent system.
In such definiton each element can behave independently w.r.t. one another and the end users will act as if the are dealing with a single system.
To give such impression nodes will need to collaborate and such collaboration is very important to create such systems.

The collaboration happens through the exchanging of messages with each other and each node with react to messages wit other messages through message passing. When you have a collection of nodes we need to organize it by providing each member with a list of all the other nodes it knows or can communicate with.

A system of this kind should be coherent meaning the user does not notice such an organization and only sees a unified application, but developing such a system is difficult since we get relevant issues like partial failures inherent to any complex system and they are very difficult to hide like partia lfailures.

Developing distributed applications can be very complex, to make it feasible we have separate layers of software on top of the respective OS's called __middleware__, it manages resources s.t. the y can be efficiently shared and deployed across a network.
- a good example can be **Remote Procedure Call** where an application can invoke a function executed on a remote server as if it was locally available. 
- They also enable service composition to develop new applications by taking existing programs and gluing them together like in Web services.
- The horus toolkit instead grants the capability of building applications asa group of processes s.t. any message sent by one process is guaranteed to be received by all or no other processes, this simplifies the distributed applications.
- Many applications use multiple services distributed among several computers, usually with the all or nothing methodology (a.k.a. an atomic transaction)
- Using a standardized protocol the middleware makes sures that every srvice is invoked or none at all.

Creating a distributed system should be worth the effort, 4 criteria should be met inn this case:
## Resource accessibility
resources can be virtually anything and connecting users to resources should make easier to collborate and exchange information, accessing and sharing is very important.
### Hiding distribution
 There are various concepts behind this, first is **distribution transparency**: hide the fact that processes and resources are physically distributed across multiple computers and separated by large distances.
In particular we can distinguish seven flavors of transparancy:
- ACCESS: hide differences in data representation and how an object is located
- LOCATION: hide where an object is located
- RELOCATION: hied the fact that an object might be moved from point A to point B WHILE IN USE
- MIGRATION: hide the fact that an object is moved to another location(included above)
- REPLICATION: hide object replication
- CONCURRENCY: hide the fact that an object might have multiple concurrent users and indepedent users at the same time
- FAILURE: hide failure and recovery of an object(very difficult)
### Access transparency
We have different  representations and way of accessing data for different systems,a good distributed system should hide differences in physical machines and data representation by different OSs.
### Location & relocation transparency
Users can access resources even when they don't know where an object is physically located in the system, in this context **naming** allows giving logical names to resources without providing information about physical location  e.g. The URL pointing to the UNIPI website.

### Migration transparency
Relocation is only about changing position of an entity within the distrbiuted system while migration is offered when mobility of processes and resources initiated by users is supported and does not affect ongoing communication and operations.

Replication transparency
Resources may be replicated to increase availability/imporve performance by placing a copy close to the place it is accessed. Such thing should be hidden s.t. when a process operating in lockstep mode with another or the various copies can repladce each other in case of failures.

Concurrency transparency

Users of the system must not notice each other presence and usage of sahred resources,using concurrently a shared resource should leave such resource in a consistent state and consistency achieved using a locking mechanism gives users and exclusive access to a resource. 
This method could also be achieved using transactions but this will have a strong impact on scalability given the need for synchronization.

### Failure transparency
The user will not notice when a piece of the system failed and that the system will recover from that. 
This is one of the hardest thing to achieve in distributed systems and might impossible under certain assumptions.

Mainly we have a hard time distinguishing from a dead process and a slowly responding one.
## Degree of distribution transparency

There are cases where we can relax the transparency of system for an higher performance and in general some situations instead require avoiding hiding the distribution from users.
We can state a few cases:
- internet application try multiple times to contact a server before going to another however this may slow down the whole system.
- Replicas across very large distances,e.g. continents must be consistent all the time but it can be very costly.
- Mobile devices are starting to rely on location and context awareness
- real time documents editing could hinder cooperation
After all these examples we can conclude that full distribution transprency is impossible and in some cases it is better to make it explicit so that user and applications developer are not tricked into beliving there is and users understadn much better the behavior of a distributed system.

## Openness

It mean that a system offers components that can be easily used or integrated into other systems, an open distributed system will consist of components that originate from elsewhere with two key enablers:
- interoperability,composability and extensibility
- separation of policies from mechanisms
We need to adhere to standard rules descrbiing components usually relying on an Interface Definition Language and interface ddefiniton allows an arbitrary process that need a creating interface to interact with another process providing that interface.

A proper specification of such interface is complete and neutral, meaning tha everything that is necessary has been specified. However many interface definitions are not complete and it necessary for a developer to add implementation specific  details.

A specification should not prescribe what an implementation should look like meaning it should be neutral.

Completeness and neutrality are important for inter operability and portability
- Interoperability characterize the extent by which two implementations of system from different manifacturers can work together, it can happen by relying on each other services or a common standard
- Portability characterizes to what extent an application developed for a given distribution can be executed. It can be execurted without modification or on a different distributed system implementing the same interfaces.

The system should be easy to configure out of different components(and different developers) and adding/replace components should be easy i.e. it should extensible e.g.. changing OS or replacing a file system.

To achieve flexibility the system must be divided into small easily replaceable / adaptable components. This require to provide definitions for interfaces at high level and the internal components and describe how they interact.

Scalability can be seen across three dimensions:
- Size: a system can be scalalble w.r.t. it's size meaning we can add more users an resources without loss in performance
- Geographical: users and resources can be distant but it is hardly noticed.
- Administrative: it can be easily managed even if it spans multiple administrative organizations.

## Size scalability
When many users are supported centralized services are limited. When many services are centralized we have a sinlge server running on a machine or a group of collboratring servers located on a cluster in the same location. THe problem is obious: the server ir group of servers can become a bottleneck :
- limite compuitationa capacity (CPU bound)
- limited sotrage capacity (I/O bound)
- network between users and centralized service (network bound)
## Geographical scalability
When we have distributed systems in wide-area networks using sycnrhonous communication can be hunderds of millisiconds instead o a few hudred of microseconds, this mean that buildfing sycrhnous communication in wide are systemws requires great care and a rich interaction pattern.
In WANs communication is much less reliable and bandiwidtth is very limited and with few facilities fo multipoint communication while LANs support efficient broadcasting mechanisms.

Solutions for LANs cannot always be easily ported to  wide-area system.

## Administrative scalability
We need to deal wioth multiple,independent administrative domaisn and a major problem is that  policies can conflict w.r.t. resource uasage,payment,management and security.

We want users within a domain to trust the components to keep operating within that same domain but it should not expand across domain boundaries. 
When a distributed system expands to another domain it has to protect itself from malicious attacks coming from  the new domain and the new domain has to protect itself from attacks coming from the distributed system.

There are various ways to scale
- scale up meaning you add more power
- scale out meaning you add more machines
	- hide communication latencies when more machines are added
	- disitribute work between the new machihes
	- replicate replicate in cases of failures for processes /resources
	The hiding of latencies applies in geographical scalability since we want to avoid waiting for responses to remote service requests and a service 