# Robotics mechanics and kinematics 

A robot is an autonomous system which exists in the physcial world and can sense it's environment and is able to act on it to achieve some goals. 
## definition of robot
Robot can either need or not need interaction with a person:
- A robot is *autonomous* when it can act on the basis of it's own decisions and is not controlled by a human. 
- A robot is *non autonomous* when it is controlled by an operator step by step
- A robot is *semi-autonomous* when control is shared between robot and users, it possible at different levels of autonomy.
A robot can exist in the physical world, which means it is subject to the laws of physics that are different from simulations, in the physical world interactions and physical laws are simulated and somehow approximated.
A robot has sensors which are used to perceive information from the world, i.e. it SENSES it's world. 
A robot can also ACT on it's world and uses *effectors* and *actuators* to achieve what is desired.
and is also intelligent meaning it has some reasoning capability. 
At last robot is intelligent in the sense that it has a reasoning capability to some degree.
## robot components
The components of he robot are then.
- Physical body, which is in the physical world.
- Sensors, so it can perceive its environment.
- Effectors/actuators, so it can take actions.
- Controller so it can be autonomous.

![[Pasted image 20250219094727.png]]
The effectors themselves are of various types like in the image below.

![[Pasted image 20250219094852.png]]
An *effector* is the part of the system which interacts with the environment to achieve a task and is composed also by *actuators* that put energy motion.
## Degree of freedom

The first definition we need is a *degree of freedom*.
Definition: the configuration of a robot is complete specification of the position of every point of the robot. In a mathematical sense it is the minimum number of real valued coordinates/parameters used to represent the configuration of the robot and is called *degree of freedom*.

![[Pasted image 20250219095324.png]]
here we have some examples that combine springs, pendulums and linear motions that can be used to describe completely their state with different relationships with one another.

![[Pasted image 20250219095440.png]]
Other real life examples in this case we define coordinates in the world not motion, as you can see it heavily relies on how you model a problem and how much information you need.

For rigid bodies we have both movement and orientation, see the helicopter below for an example.
![[Pasted image 20250219095606.png]]

In a 3D space we have at most 6 DOFs, 3 are used for translation and movement description and the other for orientation as shown in both the images above and below. However not all objects can move on all these degrees at their will, for example a car can only move on the x and y axis but not z, i.e. it can't move horizontally, and also it cannot rotate on all directions while the helicopter can actually.
![[Pasted image 20250219095716.png]]
Formally we have DOFs controlled by an actuator called *controllable DOFS* ($CDOF$) and the others that aren't. Total number of DOFS is indicated as $TDOF$ and depending on their number we can derive three situations:
- *holonomic* $(CDOF=TDOF)$ : the robot controls all its DOFs.
- *non-holonomic* $(CDOF<TDOF)$ : the robot controls only a part.
- *redundant* $(CDOF>TDOF)$: the robot has more ways to control the DOFs it can control.
## Joints and DOFs
*Joints*, in the mathematical sense, are set of two surfaces that can slide keeping contact one to the another. The link between the two joints is the DOF of the robot and *link 0* is the support base and origin of the reference coordinate frame for robot motion.
![[Pasted image 20250219100701.png]]
Observe that a link can have more degrees of freedom depending on the kind of movement it does as shown in the image above:
- it can revolute, called **Revolute** (R) where it revolves.
- it can slide, called **Prismatic** (P) where it slides.
- it can rotate, called **Helical** (H) where it has an helical movement around an axe.
- it can be **Cylindrical**(C) where it slides and rotate along a certain axe.
- it can be **Universal** (U) , where it moves rotates along two axes
- it can be **Spherical** (S) where the movement can be along a certain sphere with a defined surface.
Most common robot joints are the revolute but can also prismatic with different variations like linear,sliding,telescopic etc.etc.
![[Pasted image 20250219214647.png]]

Now we can see different joint types, in the image below we have two important points:
- $\{0\}$ which is the base frame of the model
- $\{E\}$ which is  the coordinate from of the end-effector where the combination of those joints ends up.

![[Pasted image 20250219102405.png]]
In this case we have $(x_0,y_0)$ that are the base coordinates for the base frame and $x_E,y_E$ that  are the coordinates of the end effector coordinates.
The joint variables for angles or primastic extension are denoted with $q$ and the length for fixed size parts are denoted using $a$.
## robot manipulator
![[Pasted image 20250219215534.png]]
A *robot manipulator* is nothing more then an open kinematic chain.
An open kinematic chain is a sequence of rigid segments or links that are connected using revolute or translational joints actuated by a motor.
An extremity is connected to a support base and the other ones is free and equipped with a tool called end effector.

The human arm is quite difficult to model, it has seven DOFs nad the arm itself has only three joints: shoulder,elbow and wrist with those it controls alls the seven DOF. in particular the shoulder is quite difficult to model as is the hand given the biologic formation.
![[Pasted image 20250219220457.png]]
At its most simple incarnation, as in the image above, a robot manipulator consists of *a robot arm*,*a wrist* and a *grippler* into an arbitrary pose.

Its task to to place an object grabbed by the grippler into an arbitrary position, this kind of robot uses one degree of freedom less so six instead of the seven from the classical arm we have seen before. The arm also gives us the position of the robot end point. The wristr enables the orientation needed for the object to be grabbed by the robot gripper.
We can have various kinds of joints, usually we have three of them and either revolute or translational and can be annotated with a three word label composed by R's and T's like in the image below.

![[Pasted image 20250219102947.png]]
## Joint & cartesian space
The *joint space* is the space in which the $q$ vector of joint variables are defined its dimension is indicated with $N$ that indicates the number of joints in the robot.
The *cartesian space* is where $x=(p,\Phi)^T$ resides and it defines the end-effector position. the dimension of such space is indicated with $M$. And $p$ is the cartesian coordinates while $\Phi$ is the orientation of the end effector.
- $q$ is the vector of the robot positon in the joint space and it contains the joint variables epxressed in degrees.
- $x=(p,\Phi)^T$ is the vector of the robot position in the cartesian space with the cartesian coodrinates $p=(x,y,z)$ and $\Phi=(roll,pitch,yaw)$ is the vector of orientation of the end effector.
The *robot workspace* is the region described by the origin and the end effector is when the robot joints execute all possible motions.

![[Pasted image 20250219230128.png]]
This the PUMA robot manipulator, here we have two main subgroups wihch are the support structure  and the wrist in this case we have 6 joint with a shoulder a trunk which offsets the shoulder and the base frame point and affect the end effector position, and also the wrist position.

The *Robot workspace* indicates the region described by the origin of the end effector when robot joints execute all possible motions
The *Reachable workspace* is the region that can be reached by the end-effector with at least one orientation.
The *Dextrous workspace* is the region that the end-effector can reach with more than one orientation.

Such worksaces are all depending on link lengths and joint ranges of motion, an axemple can be seen in the image below.

![[Pasted image 20250220090307.png]]

For a robot ARM-types ca be of different kinds, below you can see various types

![[Pasted image 20250220090402.png]]

## Representing position and orientation

A point $p\in \mathbb{R}^n$ can be represented as a vector from the reference frame point $\{a\}$ origin to $p$ with the vector $p_a$ . We can have different reference frames in the image below we can see $\{a\}$ and $\{b\}$, for each of them we have unit coordinates $\hat{x},\hat{y}$ .
![[Pasted image 20250219104428.png]]
different reference frames , represented as $\{a\}$ in the image above, give a reference to reconstruct the position $p$, whatever reference we use is ok we just need to have stationary position.

Reference frames can be placed anywhere to have a valid representation of the space in our case we assume to have always a single stationary frame called *fixed frame* denoted with $\{s\}$ also called *space frame*. We can also assume that at least one frame has been attached to a rigid moving body, it is called *body frame* and is denoted with $\{b\}$ and is always assumed to be attached to a rigid body. In the image below we see an example with the fixed frame being static and fixed from which the axes of the space are originating and then we have the second point which can also be represented as the vector $p$ and it is assumed to be attached to an L-shaped body, in this case it also has an angle $\theta$.
Also note that the body frame is attached to some important point of the body but it is not necessary.
![[Pasted image 20250219104618.png]]
such reference frame is stationary,$\{s\}$, and can be used to describe the distance from other reference frames that are used to describe the distance from other non-stationary reference frames, $\{b\}$ , that are attached to a moving rigid body. we don't need stuff like the center of mass but it can be useful.
## Rigid body motions in the plane
The body-frame origin $p$ can be expressed as: 
$$
p=p_x\hat{x}_s + p_y\hat{y}_s
$$
Now we want to descrbe the unit vectors used for the orientation of the body which are $\hat{x}_b,\hat{y}_b$ to do so we can use the following formulas 

$$
\hat{x}_b= \cos \theta \hat{x}_s + \sin \theta \hat{y}_s 
$$

$$
\hat{y}_b= - \cos \alpha \hat{x}_s + \sin \alpha \hat{y}_s 
$$
It becomes clearer when you use a raphical explanation of the formulas, take the following intutions :
![[Pasted image 20250220093426.png]]

The $\cos$ indicates the horizontal component of the unit vectors and $\sin$ indicates the vertical component, their sum allows you to obtain the the respectve unit vectors and you also have their orientation with simple trigonometric formulas where $$\alpha=(\pi -\frac{\pi}{2}-\theta)=\frac{\pi}{2}-\theta$$
so in this case it's a just 90 degrees rotation of $\theta$ and a rotation of $\alpha$ degrees of the unit vectors of the reference frame. From this you can take out $\alpha$ from $\hat{y}_b$, since $\cos(\frac{\pi}{2}-\theta)=sin(\theta)$ and $\sin(\frac{\pi}{2}-\theta)=\cos(\theta)$ you have that
$$
\hat{y}_b=- \sin \theta \hat{x}_s+\cos \theta \hat{y}_s
$$
and in case $\Theta=0$ we have that 

$$
\hat{x}_b=\cos(0)\hat{x}_s+\sin(0)\hat{y}_s=1*\hat{x}_s+0*\hat{y}_s=\hat{x}_s
$$
$$
\hat{y}_b=-\sin(0)\hat{x}_s+\cos(0)\hat{y}_s=0*\hat{x}_s+1*\hat{y}_s=\hat{y}_s
$$
so this is the situation now:
![[Pasted image 20250220094612.png]]
So orientation can be described using only $\theta$  relatively to the fixed frame while the position $p$  expresses the position w.r.t. the reference frame and is just a column vector:
$$
p=\begin{bmatrix}
p_x\\p_y
\end{bmatrix}
$$
we can also use a rotation matrix $P$ to have the same result:
$$
P=[\hat{x}_b\text{ }\hat{y}_b]=\begin{bmatrix}
\cos \theta & - \sin \theta \\
\sin \theta & \cos \theta
\end{bmatrix}
$$
for P we have that: 1. each column must be a unit vetor 2. the two columns must be orthogonal and 3. the remaining DOF is parametrized by $\theta$  as happens with $P$.

So the pair $(P,p)$ provides a description of the orientation and position of $\{b\}$ w.r.t. $\{s\}$ and can be expressed as $(P,p): \{ s \rightarrow b \}$.

Ok now we have a framewor for such cases, say ou have the following situation
![[Pasted image 20250220095535.png]]

For the previous notation $(Q,q): \{ c \rightarrow b \}$ and $(P,p): \{ b \rightarrow s \}$ in the image above.
So here we have a single reference frame and two body frames, we can convert from a body frame to another in this case we want $\{c\}$ relative to $\{ s\}$  and we can have it by converting $Q$ to the reference frame, to do so $R=PQ$ and $r=Pq+p$ and now we have $(R,r):\{c \rightarrow s\}$.
### 3D rigid body motions in the plane
We need to define also orientation and axes direction in 3D, we say that a space is right-handed when we have it organized lke in the image below

![[Pasted image 20250220102605.png]]

While a rotation is said to be positve when it rotates going left like in the image below.
![[Pasted image 20250220102621.png]]


All our reference frames are right-handed and the unit axes $\{\hat{x},\hat{y},\hat{z}\}$ and always satisfy $\hat{x} \times \hat{y} = \hat{z}$ the fixed frame has unit axes $\{\hat{x}_s,\hat{y}_s,\hat{z}_s\}$ while for a body frame we have the unit axes

$$
\begin{bmatrix}
\hat{x}_b \\ \hat{y}_b \\\hat{z}_b
\end{bmatrix}= R
\begin{bmatrix}
\hat{x}_s \\ \hat{y}_s \\\hat{z}_s
\end{bmatrix}
$$
the $p$ vector from the fixed frame has now the formulation:
$$
p=
\begin{bmatrix}
p_1\\p_2\\p_3
\end{bmatrix}
=p_1\hat{x}_s+p_2\hat{y}_s+p_3\hat{z}_s
$$
and $R$ is expressed as:
$$
R=\begin{bmatrix}
r_{11} &r_{12} &r_{13} \\
r_{21} &r_{22} &r_{23} \\
r_{31} &r_{32} &r_{33} \\

\end{bmatrix}=
\begin{bmatrix}
\hat{x}_b&\hat{y}_b&\hat{z}_b
\end{bmatrix}
$$

in 3D a description of the rigid body's position and orientation is described by 12 parameters defined by $p \in \mathbb{R}^3$ and $R \in \mathbb{R}^{3 \times 3}$ where each oftheir values can vary.

![[Pasted image 20250220105655.png]]


A rotation matrix can:
- represent an orientation
- change reference frame in which a vector or a another frame is represented
- rotate a vector or a frame
![[Pasted image 20250225223021.png]]
We assume a fixed frame space $\{s\}$ which is aligned with $\{a\}$ the orientations of the three frames can be written as 
$$
R_a=\begin{bmatrix}
1&0&0\\
0&1&0\\
0&0&1\\
\end{bmatrix},R_b=\begin{bmatrix}
0&-1&0\\
1&0&0\\
0&0&1\\
\end{bmatrix},
R_c=\begin{bmatrix}
0&-1&0\\
0&0&-1\\
1&0&0\\
\end{bmatrix}
$$

in those frames we can write the location of a point $p$ is written as 
$$
p_a=\begin{bmatrix}
1\\1\\0
\end{bmatrix},
p_b=\begin{bmatrix}
1\\-1\\0
\end{bmatrix},
p_c=\begin{bmatrix}
0\\-1\\-1
\end{bmatrix}
$$

Rotations are considered positive when the revolve around the selected axis in a count-clockwise manner when looking the the perspective in the iamge below.

![[Pasted image 20250225223240.png]]

When rotating we can look first at the rotation around a single axis-x in this case we get that x and x' (the result of rotating) are collinear while the other ar obtained by just rotating the reference frame x-y-z for the angle $\alpha$.
![[Pasted image 20250225223856.png]]

A rotational displacement can be described by an homogenous transformation matrix, the first three rows of the matrix corespond to x , y and z of the reference frame while the first three columns refere to the x'-y'-z' axes of the rotated frame, the upper 3x3 matrix is the rotation matrix $H$ and are the cosines o the angles between the axes given by corresponding column and row

$$
Rot(x,\alpha)=\begin{bmatrix}
\cos 0 &\cos 90 &\cos 90 &0\\
\cos 90 &\cos \alpha &\cos (90+\alpha) &0\\
\cos 90 &\cos (90 - \alpha) &\cos \alpha &0\\
 0 & 0 & 0 &1\\
\end{bmatrix}=\begin{bmatrix}
\cos 0 & 0 & 0 &0\\
0 &\cos \alpha & -\sin \alpha &0\\
0 & \sin \alpha &\cos \alpha &0\\
 0 & 0 & 0 &1\\
\end{bmatrix}
$$

This is a useful exercise to better understand rotation matrices, we can do the same thing with y imposing that $y=y'$ and then that
$$
x=x'\cos \beta + z' \sin \beta
$$
$$
z=-x'\sin \beta + z' \cos \beta
$$
This takes the form of the following matrix

$$
Rot(y,\beta)=\begin{bmatrix}
\cos \beta & 0 & \sin \beta & 0 \\
0 & 1 & 0 & 0 \\
-\sin \beta & 0 & \cos \beta & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
$$
The situation is as described in the image below

![[Pasted image 20250225225740.png]]

We can then use the complete formulation for the three axes and get a sequence of rotations around axes $R$.
![[Pasted image 20250226093057.png]]
And the rotation matrix combining them is defined as

$$R=R_{x,\alpha}R_{y,\phi}R_{z,\theta}$$
We can use a rotation matrix $R$ to represent a body frame $\{b\}$ in the fixed frame $\{s\}$ and a vector $p$ which represent the origin of $\{b\}$ in $\{s\}$, instead o identifying $R$ and $p$ we package them into a matrix 
$$
T=\begin{bmatrix}
R & p \\
0&1\\
\end{bmatrix}=
\begin{bmatrix}
r_{11}&r_{12}&r_{13}&p_1\\
r_{21}&r_{22}&r_{23}&p_2\\
r_{31}&r_{32}&r_{33}&p_3\\
0&0&0&1\\
\end{bmatrix}
$$
And its inverse is also a transformation matrix 
$$
T^{-1}=\begin{bmatrix}
R&p\\
0&1
\end{bmatrix}^{-1}
=\begin{bmatrix}
R^T & -R^Tp\\
0 & 1 \\
\end{bmatrix}
$$

and also the product of a transformation matrix is a transformation matrix.

we can also pack translation into a matrix too

$$
T_{tran}=\begin{bmatrix}
1&0&0&dx\\
0&1&0&dy\\
0&0&1&dz\\
0&0&0&1\\
\end{bmatrix}
$$
It can be used to obtain the coordinates of a point w.rt. different points in space by just multypling it.

$$P_{xyz}=T_{tran}P_{vuw}$$
![[Pasted image 20250226095833.png]]

The image above explains well the concept.

As for transformation composition we can do that through matrix multiplication

$$^AT_C=^AT_B\ ^AT_C$$
![[Pasted image 20250226100347.png]]
## Robot arm kinematics

We can use *kinematics* to make a robot arm move, we have to ways of doing that:
- Direct kinematics: computing the end-effector position in the cartesian space given the robot position in the joint space.
- Inverse kinematics: compute the joint positions to obtain a desired position of the end effector.
![[Pasted image 20250226100821.png]]

In the direct approach for a given robot arm and a vector of joint angles $q$ and link geometric parameters find the position and orientation of end effector or mathematical sense find the vectorial non linear function $K$ s.t.

$$
x=K(q)
$$
In inverse kinematics for the given robot arm with a desired position and orientation of the end effector w.r.t. reference coordinate frame find the corresponding joint variables.
In thi case find a non-linear vectorial function 
$$
q=K^{-1}(x)
$$

Basically you either find the joint variables rom the desired point or find he the result point from the joint variable.

Since ne number of DOFS s higher than the one needed to characterize the joint space, such number of redundancy is obtained as $R=N-M$, thise means multiple solutions at the cost of control and computational complexity.

Inverse kinematics show a series of problems to deal with:
- non linar equations
- not always possible to fin an analytical solutions
- solutions can be multiple,infinite or not possible for give arm kinematica structures, their existence is grante if the desired position and orientation belong the the dextrous workspace of the robot.

Direct kinematics are quite easy to grasp, just use the homogenous matrices you have seen before the multily them to get the result, see the image bew for an example

![[Pasted image 20250226104027.png]]

A homogenous transformation matrix can be either used to describe the pose of a frame w.r.t. a reference frame or the displacement of a frame into a new pose. The difference is given by interpretation, for the first case the upper-left 3x3 matrix is the orientation of the object while the riht hand 3x1 column describes the position, for the second case the matrix corresponds to a rotation and the riht-hand column is the translation.
![[Pasted image 20250226120359.png]]
This is an example to obtain the rotation matrix switch from a reference to another nad we can see that it is justthe combination of various rotations toruh an inhomogenous matrix.

We can start by studying the geometric manipulator model.

We have  matrix $^O H_3 = (^O H_1 D_1)(^1 H_2D_2)(2^ H_3 D_3)$ with matrices $^O H_1,^O H_2,^O H_3,$ that describe the pose of ech joint rame w.r.t. the preceding frame. 
![[Pasted image 20250226121047.png]]
This is the kind of robo we are working with.

We an start by analyzing wht is happening within the parenthesis

$$
^O H_1 D_1 = \begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & l_1 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
\begin{bmatrix}
c1 & -s1 & 0 & 0 \\
s1 & c1 & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}=
\begin{bmatrix}
c1 & -s1 & 0 & 0 \\
s1 & c1 & 0 & 0 \\
0 & 0 & 1 & l_1 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
$$
$$
\sin(\theta_1)=s1,\cos(\theta_1)=c1
$$
We undestand that the $D_1$ matrix represents rotation around the positive $z_1$ axis and the product is the displacement and pose and displacement of the first joint.

For the second joint w have a rotation around the $z_2$ axis.

$$
^O H_2 D_2 = \begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & l_2 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
\begin{bmatrix}
c2 & -s2 & 0 & 0 \\
s2 & c2 & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}=
\begin{bmatrix}
c1 & -s1 & 0 & 0 \\
s1 & c1 & 0 & l_2 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
$$
And it still is a rotation like before just on a different xis, now we see a translaton for a change

$$
^O H_3 D_3 = \begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & l_3 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & -d_3 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}=
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & l_3 \\
0 & 0 & 1 & -d_3 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
$$

All those combined give the following result

$$
^O H_3 = \begin{bmatrix}
c12 & -s12 & 0 & -l_3s12  -l_2s1\\
s12 & -s12 & 0 & -l_3c12 -l_2c1\\
0 & 0 & 1 & -l_1 - d_3\\
0 & 0 & 0 & 1\\
\end{bmatrix}
$$
with $$
c12= \cos (\theta_1 + \theta_2)=c1c2-s1s2
$$
$$
s12= \sin (\theta_1 + \theta_2)=s1c2+c1s2
$$
## The Universal Robot Description Format

The URDFs an XML file format used by the Robot Operating System to describe kinematics, inertial properties and link of geometric robots in a tree manner.
The URDFdescribes joints and links of a robot:
- Joints: it's used to connect two links a parent link and a child link. A joint can be of different types that are the types we already discussed. 
  Each one has an *origin frame* that defines a position and orientation of the child link frame relative to the parent link frame when the joint variable is zero and we have join which is on the joint's axis.
  Each Joint has a 3-axis vector, a unit vector expressed i the child link frame which is in the direction of positive rotation for a revolute joint or positive translation for a prismatic joint.
- Links: we define mass properties, the elements of a link among the others also include its mass, an origin frame that defies position and orientation of a frame at the link's center of mass relative to its joint frame and an inertia matrix relative to the link center of mass frame.
A URDF file can represent any robot with a tree structure including serial-chain robot arms and hands.

## The Denavit-Hartenberg representation
the D-H representation is an approach to forward kinematics to attach reference frames to each link of the open chain and then derive the forward kinematics frm the knowledge of the relative displacement between adjacent link frames. We use the matrix based method for describing the the relations between adjacent links, the D-H representation consists of homogenous 4x4 transformation matrices which represent the reference frame w.r.t. the previous link. the position of the end effector can be expressed in the base frame coordinates.
![[Pasted image 20250226220955.png]]
This is the model in question.

For link we have 4 geomtric parameters associated to each link:
- 2 describe relative position of adjacent link(the joint parameters)
- 2 describe the link structure
On those geometric parametrs depend the homogenous transformation matrices but only one set is known, we have a joint rotation axis defined by the 2 links connected by the joint. For each axes we have 2 normal lines defined one for each link.

![[Pasted image 20250226221851.png]]

From the kinematics POV a link keeps a fixed cofiguration between 2 joits, he structre of link $i$ is characterized with the length and the angle of the rotation axis of join $i$.
In the picture above:
- $a_i$ = minimum distance along the comon normal line between two joint axes, **LINK LENGHT**
- $\alpha_i$ which is the angle between two joint axes on a plane normal to $a_i$. called **TWIST**
- The position of the $i$-th link w.r.t. the $(i-1)$-th link can be expressedby measuringthe distance an the angle between two adjacent links. 
- The distance $d_i$ = distance between normal lines along the $(i-1)$-th joint axis, called **LINK OFFSET**
- $\theta_i$ = angle between two normal lines on a pan of the normal axis, called **JOINT ANGLE**.

In the D-H representation for a 6DOF arm wehave 7 coordinate frames:
- $z_{i-1}$= motion axis of joint i
- $z_i$=motion axis of joint i+1
- $x_i$=normal to axis $z_{i-1}$ and $z_i$ 
- $y_i$=completes the frame with the right hand rule
we can express the end effecotr position in the end effector frame can be expèressed in the base frame with a sequence of transformations.
THe algorithm to do so goes as follows
1. fix a base coordinate frame
2. for each joint sets
	- joint axis
	- origina of coordinate frame
	- the x axis
	- the y axis
3. fix end effector coordinate frame
4. for each joint and each link set
	- joint parameters
	- link parameters

we can see an example
![[Pasted image 20250416123541.png]]

we can now build an homogenous transformation matrix describing relations between adjacent frames.
The matrix is built through rotations and traslations:
- rotate $x_i$ around  angle $\alpha_i$  to align the z-axes
- translate $\alpha_i$ along $x_i$ 
- translate $d_i$ along $z_{i-1}$ to overlap the 2 origins
- rotate around $z_{i-1}$ for and angle $\theta_i$ to align the $x$ axes

the D-H transformation can be expressed with a homogenous trasnformation matrix

$$
^{i-1}A_i=R_{\theta}T_{z,d}T_{x,a}R_{x,\alpha}
$$
we use 4 parameters one for each link which describe completely all joints wehter they are revolute or prismatic:
- for a revolute joint: $d_i,a_i,\alpha_i$ are joint parameters and only $\theta_i$ varies
- for a prismatic joint: $\theta_i,a_i,\alpha_i$ are fixed and only $d_i$ varies.
the resulting matrix encodes:
- rotation of $z_{i-1}$ by $\theta_i$
- translation along $x_i$ by $a_i$
- rotation about $x_i$ by $\alpha_i$
$$
r_{i-1} = {}^{i-1}A_i p_1 =
\begin{bmatrix}
\cos(\vartheta_i) & -\cos(\alpha_i)\sin(\vartheta_i) & \sin(\alpha_i)\sin(\vartheta_i) & a_i \cos(\vartheta_i) \\
\sin(\vartheta_i) & \cos(\alpha_i)\cos(\vartheta_i) & -\sin(\alpha_i)\cos(\vartheta_i) & a_i \sin(\vartheta_i) \\
0 & \sin(\alpha_i) & \cos(\alpha_i) & d_i \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

so here the $i$ index refers to the frames, the matrix is thus  transformation from a frame to another frame in the open chain representation.
$p_1$ is a homogenous coordinate vector expressed in frame $i$ which gest transformed by the matrix to be expressed in fram $r_{i-1}$ i.e. we use the coordinates to reach the same point but changing the frame and calculate the coordinates to reach the same point with a frame that is linked to the previous one.

in this context a homogenous matrix $T$ describes the n-th frame with respect to the base frame is is the product of sequence of transformation matrices $^{i-1}A_i$ is expressed as
$$
^0T_n=^0A_1 \text{ }^1\dots^{n-1}A_n
$$

we can see it in another way

$$
^0T_n=\begin{bmatrix}
X_i & Y_i & Z_i & p_i\\
0 & 0& 0&1
\end{bmatrix}
$$
with $X_i,Y_i,Z_i$ describing the orientation of the n-th frame with respect to the base frame.

then we have $P_i$ being the position vector pointing from the origin of the base frame to the origin of the n-th frame and $R$ being the matrix describing the roll, pitch and yaw angles for the n-th frame

and we can pack everything in a more compact way

$$
^0T_n=\begin{bmatrix}
^0R_n & ^0p_n\\
0&1\\
\end{bmatrix}
$$
# Differential kinematics

in this part we explore differential kinematics. As we know forward kinematics map from the joint positions to the pose of the end effector in our space or any other part of the robot i.e. $x=f(q)$ which we have already seen.
the position is in $\mathbb{R}^3$ so it is easy while orientation is in $\mathbb{SO}^3$ with many notation we can apply like rotation matrix,euler angles, unit quaternions, axis angles and so on.

The uni quaterions are expresse as a two part notation
$$
x_o=(\cos(\frac{\theta}{2}),\hat{v}\sin(\frac{\theta}{2}))=(w,\text{ }x,y,z) 
$$
the first cosine is the scalar part, the sine part is the vector part and together they describe orientation.
Assume to be using a RRBot as in the following image

![[Pasted image 20250511124837.png]]

we compute $(x_1,x_2)$ using the forward kinematics function from before using for example the denavit harten berg representation.

![[Pasted image 20250511125025.png]]

Here the matrix $^{n-1}T_n$ is the conjunction between link $i$ and link $i-1$  which fives from links $i-1$ end effector position and orientation the position and orientation of end effector for link $i$.

In differential forward kinematics we map joint positions and speeds to the velocity of the end effector, to do so we use a jacobian matrix
$$
\dot x = \frac{df(q)}{dt}=\frac{\partial f(q)}{\partial q}\frac{dq}{dt}=J(q)\dot{q}
$$
note that in this case such velocity actually depends also on the joint configuration q.

Alternatively we can express the relationship between motion of the joint and the velocity of the EE by linearly composing velocities generated by the joints.

We compute the twist of the EE as a produt of a matrix depending only on $q$ calledd the geometrical jacobian and the joint speeds inside $\omega$.

$$
\xi = \begin{pmatrix}\dot x_p \\ \omega \end{pmatrix} = J(q)\dot q
$$

![[Pasted image 20250511142334.png]]

This is an example where $\dot x$ is just the sum of the velocities of the joints , $\dot q_1$ and $\dot q_2$.

Now two objects are describing EE movements given speed and rotation of the joints.

One is the geometrical jacobian the other is the analytical jacobian:
$$
\dot x_A=\begin{pmatrix}\dot x_p \\ \dot x_o\end{pmatrix}=\frac{dx}{dt}=\frac{\partial f(q)}{\partial q}\frac{dq}{dt}=J_A(q)\dot q
$$
the positioning part is always equal for the two of them but the analytical jacobian $J_A$ depends on how we parametrize orientation as deriving orientation is not the same thing as the angular velocity.

The notations used to express rotations allows us to switch between the two versions :
- Rotation matrix: 
  $$
  \dot R = SR=\begin{pmatrix}
  0 &-\omega_z&\omega_y \\
  \omega_z & 0&\omega_x \\
- \omega_y &-\omega_x&0 \\
  \end{pmatrix}R$$
- Quaternions:
  $$
  \dot q = \frac{1}{2}\begin{pmatrix}0 \\ \omega \end{pmatrix} \otimes q\
  $$
- Euler's angles:
$$
\omega = \dot{\alpha} u_{\alpha}+ \dot{\beta} u_{\beta}+\dot{\gamma} u_{\gamma}
$$

For inverse kinematics we know the trajectory for the EE bu we must make the robot follow the movement accordingly, the relation is the inverse of the forward kinematics:
$$
\dot q = J_A^{-1}\dot x_q
$$
but that is not correct, in this case we replicate the derivative of the trajectory we desired if we start with some error we conserve it.Thus we need to crrect the joint speeds considering the magnitude of the error

$$
\dot q = J_A^{-1}(\dot x_{A,d}+K_e)
$$

and we obtain 
$$
\dot x_A= J_A(q)\dot q=J_A(q)J_A^{-1}(\dot x _{A,d}+K_e)
$$

We obtain this relationship which means we get the following relationship

$$
\dot e=-K_e
$$

and 
$$
e(t)=\dot x_d(t)-x(t)
$$
$$
\dot e(t)=\dot x_d(t)-\dot x_A(t)
$$
meaning we can model the error and make it disappear adding acorection through $K_e$ but the jacobian we use is not always invertible so we use pseudoinverse of the jacobian

$$
J^{\dagger}=J^T(JJ^T)^{-1}
$$
In this representation we have to use the analytical jacobian is to be sued as there is no relation between angular velocity and derivative of the EE orientation, a quaternioni representation allows to use the geometrical jacobian by using a different definition of the orientation error

$$
e=\begin{pmatrix}
x_{p,d}- x_p\\
(x_{o,d}\otimes x_o^*)_v
\end{pmatrix}=\begin{pmatrix}
e_p\\e_{o,v}
\end{pmatrix}
$$
using the appropriate Lyapunov function and using the relation between the derivative of a quaternion and the angular velocity.
Depending on what control target we are trying to achieve we can have settings where we have more DOFs and requires to strictly solve inverse kinematics, in this case we infinite solution for our original problem but we can exploit this redundancy to follow another secondary objective.

for example say you want to perform gradient descent, that is a secondary arbitrary cost function
$$
\dot q_0=-k_0\nabla H(q)
$$
in this example it serves to keep joints in their central positions
$$
H(q)=\frac{1}{2}\sum(q_i)^2=\frac{1}{2}q^Tq
$$
this of course influences the motion of the EE and is bad for US! we can actual projects trhese joint speeds in the jacobian null space 
$$
\dot q = J^{-1}(\dot x_d+K_e)+P\dot q_0
$$
where $P=(I-J^{\dagger}J)$.

A singular configuration of the robot makes the jacobian matrix decrease its rank, when we are close to singularities the pseudoinverse solution becomes unstable and the robot configuration is close to singularities, for this reason a common practice is to use damped least square solution
$$
J^*=J^T(JJ^T+\rho^2)^{-1}
$$
with $\rho << 1$ .

The reason for instability is actually shown using Singular Value Decomposition, we use singular values , below a short recaèp just remember that they are the apporximation of eigen values to non square matrices

![[Pasted image 20250511153328.png]]

Now we can actually apply that to compute the pseudoinverse of the matrix 
![[Pasted image 20250511153415.png]]

now when $\sigma_r \rightarrow 0$ we approach a singularity.

If we apply then the damped LS solution we get dampend inverse singular values on $D$

![[Pasted image 20250511154002.png]]

now we don't get 0-ed out values but we are less accurate. We now need to compute joint positions corresponding to it , to control a robot we have to follow desired trajecotyr we use close loop iterative solutions, they provide a way of solving this problem. we converge to the deisred position by following continuous small steps trejacotries,(they are not we are just doing very small steps). Given a $q$ desired pose we want to incrementally update it untile we reach the point we want
$$
q_{k+1}=q_k+\Delta_k q
$$
that rule will be applied to a controller acting as in discrete time, the common practice us use Euler's forward integration  so the value of K in the inverse kincematic loop is limited by the sampling time to avoid instability of the system 
$$
q_{k+1}=q_k+\dot q_kT_s
$$
with $K < \frac{2}{T_s}$ of course we cannot actually influence the real position or speed of the actuator just how much torque is applied to it , as such this control strategy depends on a lower level controller that can bring each joint to a desired position. 
# Robot behaviour

In industrial robotics you have a structured environment that is easy for robots to interact with then we switched to service robots that could operate in an unstructured environment with the development of techniques and theories for perception & action control.

Robot behaviour is divided into two levels:
- high level
- low level
![[Pasted image 20250224213552.png]]
The interaction happens with a UI tht produces inputs to the behaviour control which itself controls controllers that ake robot act, the robot will then inform controllers and behaviour control with extrnal information and results of the actions.

## Robotic paradigms

We need to study the paradigms we can apply to drive robots and wich approaces suit best some situations. Three main paradigms:
- hierarchical
- reactive
- hybrid
All three differ i the way the accepted "primitives" are organized, commonly we accept the following primitives:
- SENSE: take the information from the robot sensors and produce and output to other functions
- PLAN: takes information from the SENSE or from a wold model and produces task for the robot
- ACT: takes the task for PLAN and produces output commands for the robot actuators
The paradigms are implemented using "architectures", and they can be described in 2 ways:
- by the relationships between the 3 commonly accepted primitives of robotics.
- by the way sensory data is processed and distributed through the system.
For each primitive we ca describe the information flow
![[Pasted image 20250224214311.png]]
Robotics architectures give a principled way of organizing a control system, hey impose constraints on the way the control problem can be solved and describe a set of architectural components and how they interacts i.e. they are the building blocks of programming a robot.

An architecture is evaluated by:
- modularity
- portability
- robustness
this is the way primitives are tied together, we sense plan based on the sensed information, such planning produces directives that are acted upon by producing actuator commands.
![[Pasted image 20250224214913.png]]
The loop is closed by the fact that we have that we the sense the world again seeing our changes. Also note that ACT can not only receive directives but also sensed information, this makes it so that we could have an arrow from sense to arrow creating another paradigm called "reactive".

Basically in the way we connect the primitives we have different paradigms, we start with the hierarchical ones
### hierarchical architectures
Hierarchical architectures usually provide cognition between perception and action where we elaborate data between percepting it and acting after reasoning on it

![[Pasted image 20250224215502.png]]
Cognition interprets perception and plans robot tasks:
- SENSE primitives generate a world description
- PLAN uses such description to produce a sequence of tasks
- ACT executes the tasks produced
perception is used to establish an maintain correspondence between the internal world model and external world, a typical world model cotains:
- a representation apriori of the environment of where the robot operates
- a perceived sensory information
- more information needed for task execution
each time world representation is modified when the robot perceives the environment and the action plan is established based on it.

Processessing informations is very important and we have to divide and distribute logically and functionally the tasks.

Horizontal and sequential decompositions of the chain of information processed by the central system
![[Pasted image 20250224220144.png]]
So we have sensors -> perception -> modeling -> planning -> execute motor control then actuators will act on physical world. Information flow is sequential.

Generally PLAN  is structured in 3 levels:
- strategic 
- tactical
- executive
The *strategic* level generates the strategy on the basis of the task to accomplish.
The *tactical* level generates the commands by interpreting instructions from the strategic level.
The *executive* level receives macro-commands generated by the intermediate level ad takes care of real-time control of actuators.
We can see a very easy to understand view of this concepts:
![[Pasted image 20250224220908.png]]
A controller is structured around primitives
![[Pasted image 20250224220953.png]]

This shows better than scheme above and in particular we see that SENSE and ACT are quite simple  the PLAN is more complex and we can see better where its levels come into the scene:
![[Pasted image 20250224221154.png]]
basically we first use sensed information to produce an intermediate representation that can be process to produce actions for the actuators.

We have a lot of drawbacks:
- Time-scale: it takes a lot of time to produce a plan for a real environment
- Space: we don't have the physical memory to compute the plan
- Information: generating a plan for a real environment require updating world model which takes a lot of time
- executing a plan when available is not trivial.
### reactive paradigm

We can derive the reactive paradigm now which can skip planning basically which makes us waste a lot of time and computational power.

So we can do a switch and go from a model-based view to a reactive behaviour based architecture.
![[Pasted image 20250224222210.png]]
We don't use cognition anymore perception and action are fused now.

We ca see that actually complex behaviour can be obtained by simple sensors like heat for example and we can use clever linkings between them. Now our architecture is modified as follows:
![[Pasted image 20250224222548.png]]
World representation is cut off: it is not modeled nor stored in a memory but it is extraced i real time through sensors. And give world model not existing also a priori planning does not exist.

![[Pasted image 20250224223617.png]]
reaction can be treated sing reactive rules through *reactive rules* where we represent each time a rule that given a certain sensor produces an action. the way such actions interacts depends on the action selection mechanism, as shown in the image below:
![[Pasted image 20250224223250.png]]

Basically we either select the action, in this case we have *arbitration* or rules could combine to produce actions, in this case we have a *fusion* selection.


Actually we can use more complex rules to define behaviours based on the sensors
![[Pasted image 20250224223724.png]]
This will produce a set of behaviours that will be concerned with a specific aspect of the overall behaviour, each one is a FSM and work asynchronously and in parallel with the others and uses multiple information flows each one related to a specific robot function.
Logically we see a BEHAVIOUR as a SENSE-ACT reactive couple and we have a set of behaviours.
![[Pasted image 20250224224002.png]]

For behaviour based architectures we have a few assumptions to make:
- We have a *situated agent* which is operating in an ecological niche, it is part o the world and changes it upon receiving new sensory inputs.
- The robo is *behaviour-based* behaviours serve as thebuilding blocks for robotic actions and abstract representational behaviour of the robot, such behaviours are independent computational entities and operate concurrently.
- We have *locality* and behaviour specific sensing using an abstract representational knowledge in perceptual processing is avoided .
- Lastly we have *indipendence* between the behaviours so a shared world model is not possible.
Behaviour based architectures yield high adaptability to environment changes, low computational complexity in each behaiour and the overall computational cost is low, parallelism is possible and heaviours are easily extensible with no world model required. On the other hand the overall beahviour is difficul to predict, managment of concurrency is needed and when increasing the number of behaviours we have the complexity of concurrency management also increasing with consequent difficulty in conflict resolution.
#### subsumption architecture
Behaviours themselves are organized in an architecture based on levels, with *control levels* corresponding to the competence levels of vertical decomposition:
- lower levels concern stuff like basic functions like obstacle avoidance
- higher levels concern more goal-directed actions
Basically higher levels subsume lower ones and level work in an independent and concurrent way.

![[Pasted image 20250224230104.png]]
Here each level is a behaviourm detail we can see below:
![[Pasted image 20250224230519.png]]
Each behaviour has input and output lines, the output lines of a behaviour can be connected to input and output lines of other behaviours, an input signal can be suppressed and replaced with the signal that suppressed it while an output signal can be inhibited.

One way of handling behaviours is using *potential field behaviours*, here vectors represented behaviours and vector summation combines vectors from different behaviours to produce an emergent behaviour.

### hybrid paradigm
Here robot behaviour has plan to some degree that interferees between SENSE and ACT as represented in the imagee below
![[Pasted image 20250225220842.png]]
In the hybrid paradigm directives go to a hybrid SENSE-ACT primitive that produces then actuator commands.

We have a PLAN primitive the has only the strategic and tactical level, the strategic planner makes long-term plans by identifying the sequence of sub-tasks needed to reach the goal and it provides results for the tactical planner which then initializes and monitors the behaviours by coordinating them in time. Thus you now have the situation described in he image below.
![[Pasted image 20250225221610.png]]
This can actually take many forms.
![[Pasted image 20250225221639.png]]

# Sensors

In this section we take care of sensors and perception and sensors in general.

Sensors allow for two kind of perceptions:
- Proprioception: measure variables internal to the system e.g. joint position
- Extroception: measure variables from the working environment e.g. dstance,force etc.etc.

They aim at building an internal model of the model and robot itself, such models influences robot's brain complexity.

## transducers

The first thing we define is a *transducer* which transforms energy of a kind into a energy of another kid and can work as both sensor and actuator, a *sensor* is a device sensitive to physical quantities and transforms them into a measurable and transferable signal by means of a transducer. It acts as both sensor and actuator ad can be of two types:
- Passive transducer: requires external energy for energy conversion.
- Active transducer: convert directly input energy in output without external energy sources.
Based on the kind on input,output and auxiliary energy we can classify transducers:
- Radiant: intensity,frequency,polarization and phase.
- Mechanical: position, velocity, dimension, compliance an force.
- Thermal: temperature, gradient of temperature and heat.
- Electrical: voltage, current, resistivity and capacity.
- Magnetic: field intensity, flow density and permeability.
- chemical: concentrations, crystal structure and aggregation state.
all these kinds types of energy can be either input,output or auxiliary
## Sensor's fundamental properties

A sensor is then defined as a device sensitive to physical quantities that can tranaform it in a transferable signal by means of a transducer, it inherits transducers properties and can be classified as:
- proprio or extero ceptive
- active or passive
- w.r.t. a physical quantity

There are quite a lot of properties useful to describe and understand for a sensor.
- transfer function: it is the relation between quantity of measure and output of the sensor.
- calibration:measuring outputs for known quantities , it can also be a *calibration cycle* where we do a trial to see the whole working range of a sensor usually divided into a part with increasing values and a part with decreasing values.
- hysteresis: a sensor might have for the same input value diverse outputs which depends on the fact that the input value can either increase or decrease while varying. It is measured as the maximum difference between two output curves of the sensor during the calibration cycle, epxressed as % maximum value for he transfer function see the image below.
![[Pasted image 20250305093729.png]]
- accuracy: is the maximum error between actual values and measured values by the sensors
- repeatability: when the same input values applies to a sensors we can have variability in output such variability is called *repeatability*.
Accuracy and repeatability are actually correlated and are used to evaluate a sensors capability.

![[Pasted image 20250305094234.png]]

For  $x_m=\text{average value}$ and $x_v=\text{actual value}$ we can express accuracy as 
$$accuracy=100(x_m-x_v)/x_v$$
so we just measure how much dispersion a sensor can have.

- resolution: is the minimum variation of the input which gives a variation of the output of the sensor.
- sensitiveness: a small variation in input causes small variation in output, this measure is the ratio between input and output variation.
- noise. is the amount of signal in the sensor output which is not given by the input
- stability: how much the sensor can epp its working characteristics for a long time, it either long,medium or short.
There are much more parameters.

This is just an introduction well see in detail

### resolution & sensitivity
With **resolution** we mean the smallest amount of change we can detect in the input that can be detected and accurately indicated by the sensor.
With **sensitivity** we measure the change in output relative to a unit change in the input i.e. the measured quantity.

![[Pasted image 20250325104535.png]]

Here we see batter what happens, the sensitivity is the distance between the two values between the two consecutive steps i.e. the unit change in the input.
### Sensor calibration
Sensors need to be calibrated, the **calibration** is a procedure where we measure sensor's output for known quantities.

We do a **calibration cycle** is the trials that overs the whole working range of a sensor, we have two parts:
1. one with increasing values
2. one with decreasing values
Below we see an example of cycle results for a linear fitting experiment

![[Pasted image 20250325105123.png]]
This is the measurement of a temperature sensor showing value results at the variation of temperature related to the voltage output with a linear fit, in this case relationship is linear and is quite easy to describe and understand.
The curve generated is the *calibration curve*  and it used to measure **linearity**
### Linearity
The linearity is measured as the maximum difference expressed in % of the maximum value between characteristic curve and the reference line.

The linearity is referred to the static calibration plot of the curves obtained when we show output and input amplitude under static conditions, its degree of resemblance to a straight line is the linearity.
![[Pasted image 20250325111707.png]]

the ideal response here is the reference line and he actual response is the response of the parameter. Basically the more we have linearity the easier it is to invert the relationship and obtained back the measured parameter when using the sensor.

### Hysteresis
If a sensor has hysteresis we have that for the same input value output varies, we measure it as the difference between the two output curves of the sensor during the calibration cycle and is expressed as a % of the maximum value for the transfer function

![[Pasted image 20250325112103.png]]
Usually it happens for variations of sensors in he full cycle of measurement.

### repeatability and stability
We use **repeatability** to see how much our sensor is reliable over time, it is defined as deviation between measurements in sequence when the tested object approaches the same value form the same direction each time usually expressed as percentage in the sensor range.

The **stability** is how much a sensor can keep its working characteristics over time , usually classified as wither *short*,*medium* or *long* w.r.t. the time.


## Sensor's role in a robot

The sensor's have two different roles:
- Perception of external state *(extroception)*: measure variable to characterize the working environment
- Perception of the internal state *(proprioception)* : measured variables in the internal system used to control the robot e.g. joint position.
Simple example with two wheeled robot
![[Pasted image 20250305095242.png]]

## position sensors
We can start by examining position sensors
### switches

mechanical switches are the most simple ones with binart data that is either contact or no contact.

![[Pasted image 20250305095443.png]]

applications robotics are for impact sensors, contact sensors and endstop sensors for manipulator joints.

### optical encoders
Measures the angular rotation of a shaft/axle

![[Pasted image 20250305095606.png]]
This is what we are working with, basically in the image the motor revolves around the axles, we have a slotted disk attached to the axle and an emitter shooting a signal to a detector interrupted periodically by the disk,by this mean we can deduce motor rotation velocity.

Motors themselves can have different configurations depending on where the sensor is placed, see the image below to understand better.

![[Pasted image 20250305100024.png]]

We have a motor and a reducer that reduces the number of rotation of an axle from original motor generating movement, a sensor can be placed:
- before motor+reducer and knows the rotation of the original motor.
- after motor+reducer where it knows the final joint rotation.
In the image above you also see some variable to take car of, basically we want to model the relation ships between the join angular position and the actual motor position.
the relationship is expressed as $$\theta=\frac{\theta_m}{k}$$ and we can also consider the sensor error, remember it is expressed as 
$$
\frac{d\theta}{d\theta_m}=\frac{1}{k} \implies d\theta=\frac{1}{k}d\theta_m
$$
The sensor error is reduced by a factor $k$.

The actual rotation is obtained by counting pulses and knowing the number of disks steps and is just the angle betwen different agular positon of out joint, more specifically we can see the situation described before as

![[Pasted image 20250305102613.png]]

Now for 
- $q=\text{joint angular position}$
- $\theta=\text{join position in encoder steps}$
- $k=\text{motor reduction ratio}$
- $R=\text{encoder resolution=\# of steps per turn}$  
$$
q=\frac{\theta \times 360^o}{R \times k}
$$
And the frequency of the pulse train is proportional to angular velocity.
As for the joint angula positon $\theta$ and $R$ determine the percentage of rotation done then $360^o$ converts it to degrees, finally k applies the reduction of rotation by the reducer.

### absolute encoders

We can have a absolute rotation angle and each position is uniquely determined

![[Pasted image 20250305105528.png]]

this is the optical disk used, it has
- k photoswitches
- k code tracks
- and can represent $2^k$ different orientations with an angular resolution of $360^o / 2^k$ 

This is a photo for the actual piece mounted on a motor

![[Pasted image 20250305110315.png]]

In this case the absolute encoder is mounted before motor+reducer.

Positions are encoded through fixed sensors and follow the positive track motion from 0 to 360 degrees as in the image below.

![[Pasted image 20250305111738.png]]

As you can see each position is uniquely determined, we use the gray code to obtain the position as reported in the table below
![[Pasted image 20250305111858.png]]

the special thing is that only two consecutive position have a different character basically.

We can see an example below

![[Pasted image 20250305112303.png]]

### potentiometers

We need to understand first some electronic basics


![[Pasted image 20250305112508.png]]
This is the classical tension applied to a circuit with a resistor with a certain direction given the positive ad negative signs and current by flowing in that direction and a resistor in the circuit.
These components are tied by the following relationship 
$$
v=Ri
$$

A potentiometer is just a variable resistor 
![[Pasted image 20250305112742.png]]
We have a resistance, 3 endpoints and a movable sliders, by moving he slider by hand we actually vary the Variable Voltage Output  in the sensor i.e. the output voltage, the working principle is that the output varies w.r.t. the resistance applied that is more or less present in the circuit.

We divided the full resistance elelment into $L_T$ and $R_T$ ,where $L_T$ is the part left out and $R_T$ the one present. The influenced part is also divided in $L_1$ and $R_1$ .
The relationship betwen them is 
$$
\frac{L_1}{R_1}=\frac{L_T}{R_T}
$$

we have the full tension $v_{ref}=R_Ti$ between endpoint 1 and 3 and we measure the output tension $v_{out}=R_1i$ . Also the relation between the $L$ components is that of a proportion expressed as below.

$$
L_1=L_T\frac{R_1}{R_T}=L_T\frac{v_{out}}{v_{ref}}
$$
### Hall effect sensors
this kind of sensor uses the *hall-effect principle*: where a current $I$ flows it is immersed in a magnetic field of intensity $B$ and voltage $V$ which originate in the direction normal both to current field and to the magnetic field.

![[Pasted image 20250325113300.png]]

such principle is used in much more complicated stuff like gloves and so on
![[Pasted image 20250325114018.png]]

the issue is that we don't get much more data thanthe direction of movement or the activation or not of the sensor or the fact that movement is happening with some degree of intensity.

### Force sensor

When a force is applied to a free body such force gives the body an acceleration in a direction fo the force, it is a vector value. There two classes of sensors for such a thing:
- **quantitative sensors**: measure force proportionally and repsent its value with electrical signals 
- **qualitative sensors**: indicate whether a sufficiently strong force is applied or not

### strain gauges
The *strain* is the deformation of the physical body under the action of applied forces, the strain gauge is the resisitive elastic sensor whose resistance is function of the applied strain, when a conductor is stretched its resistance increases and when compressed it descreases. See the image below for an example

![[Pasted image 20250504173611.png]]

In (A) you see the strain in normal conditions, in (B) you see tension applied s.t. area increase and in (C) the compression applied s.y. area decreases.

These facts allows us to calculate the gauge factor as 
$$
GR=\frac{\Delta R / R}{\epsilon}
$$
Its the reaction of fractional change in electrical resistance $R$ and the fractional change in length $\epsilon$ (the strain).

#### Load cells
Whenever *pressure* is measured it requires the measurement of *force*, which in terms is measured when dealing with solids, while pressure is measured with fluids. Force is considered when action is applied to a spot and pressure is measured when force is distributed over a relatively large area. A **Load cell** is a highly accurate device use to measure the weight or force in a number of different applications and it can be used to measure compression,tension and bending or shear forces.

This is an example for load cells
![[Pasted image 20250504174532.png]]
We can classify them in a simple way:
- **mono-axial load cells**:to measure either compression or tension forces along the normal axis up to a certain capacity
- **tri-axial**: to measyre vectorial forces in 3D
- **six-axial**: to  measure both vectorial and rotational forces
The basically use the same principle of the strain gauge and vary only in their usage

#### LVDT sensors

The **Linear Variable Differential Transformer** is a transducer used to measure linear displacements, it has three solenoidal coils placed end to end around a tube, the center coil si the primary, (A) in the photo, the other two are top and bottom , indicated with (B) in the photo.

![[Pasted image 20250504174921.png]]

the core is cylindrical and ferromagnetic attached to a object whose position is to be measured and slides along the axis of the tube. As the core moves the A's linkage and the B coils changes and causes the induced voltages to change, the coils are connected so that the output voltage is the difference between top secondary voltage and bottom secondary voltage.

The voltage produced by the LVDT sensor is proportional to the applied force withing the linear range of the spring

![[Pasted image 20250504175402.png]]

Here we can take advtange of Hooke's Law
$$
x=F \times k
$$
where $x$ is the force being applied and $k$ is the length of the spring current extension, $F$ is the actual force which we can calculate back. This is the proportional force we are measuring.


#### Switch sensor

Its the most simple tactile sensor, ti just produces and "on-off" output formed by a coil and a spacer

![[Pasted image 20250504180005.png]]


### Tactile sensor

The requirements for tactile sensor come from biology actually, below you can see an example che sheet of various tactile stimuluses and how we perceive them

![[Pasted image 20250504180429.png]]

there are various ways on how we use very different parts to perceive them with various stimulus intesity and the kind of signals it sends to the body(afferent response) which is composed of various frequence generated by different situations.

Tactile sensor can be:
- **intrinsic tactile**: sensing uses pure forces and torque measurements plus geometric calculation to deduce contact conditons at the finger/object interface
- **extrinsic tactile**: uses arrays of 1 dimensional pressure sensors distributed over the sensor sruface to extract tactile iamge of the finger/object interface
Different kind of signals provide information at different time scales:
- *constant forces* used low frequences
- *vibrations* used high frequences

There are various ways of transducing such intervals into measurable signals:
- piezoresistibity:measures resistance when teh material is deformed from a force $F$
- capacitance: measured resistibity when a material is deformed from a force $F$
- piezoelectricity: measured the voltage when a material is deformed from a force $F$
![[Pasted image 20250504181245.png]]
This image shows the three forces.

#### Vision based tactile sensors

Tactile information is encoded in srface deforamtion while interqacting objects, it can be tracked with visual gathering through on board cameras.
![[Pasted image 20250504181511.png]]

DIGIT tecnology is an interesting application of this which can also work without light, not trivial in computer vision, it applied RGB sensing to a tactile surface. See the image below

![[Pasted image 20250504181740.png]]

the TacTip family uses instead markers and trackes their deformation to uderstand where pressure is being applied and how much. See the the image below.

![[Pasted image 20250504182050.png]]

Also these two tecnologies can be compined together using also some ML techniques

![[Pasted image 20250504182127.png]]

### Time of flight measurement

We use sensor to gain a sense of ditance from a transducer to an object, this is the situation we are working with
![[Pasted image 20250325114841.png]]

Our transducer emits a pulse , the pulse is the reflected and echoed back at the transducer. by measuring the distance between these  three events we gain information on the distance.
In formulas this is expressed as 
$$d=\frac{t_ev}{2}$$
where 
$$v=\text{avg speed of signals emitted},t_e=\text{time between signal emitted and echo arrived}$$
Such technology can be used for ultrasound sensors .

![[Pasted image 20250325221848.png]]

These are some examples of ultrasound where we can use the logic from before to estimate the distance.

### Laser range sensors

We can user laser waves to measure ranges, in particular we put a pin-hole in short range finding sensors, we use a linear photo-diode as an array detector. The range from a sensor to the object is afunction of th position nd the maximum detected light along the array

![[Pasted image 20250325223523.png]]



![[Pasted image 20250325223726.png]]

### Proximity sensors

We basically detect an object in a spatial neighborhood with no distance measurement:
- passive proximity sensors: detect perturbations of the environment
- active proximity sensors: emit signals and get it back


## manipulation task phases

As we will see manipulating an object is quite complex for a robot what we will do is study the various phases that come with manipulating an object.

![[Pasted image 20250504182238.png]]
This is a scheme of the manipuilation task phase, we will have previous elaboration with preshaping so that we can conform to the object shape and grasp it, we have a contact or we could have acrtive exploration to understand how its made if necessary. After this part we have stable holding where we also have to deal with possible slippages and then depending on our needs we could have in-hand manipulation and then release of an object.

End-effectors used to solve this task can take many shapes and use many sensors. We have:
- proximity: detect object proximity
- texture/stiffness: perceive texture formation or keep contact stable with the object
- contact/force: apply force or make contact
- slip: slip prevents slippages

The sensorization for interaction tasks varies also depending on the knowledge about the object to be grasped, we can identigy three main objects in this context:
- geometric-uncertain
- physical-uncertain
- unknown objects
Below we have a grid showing what kind of approach you can take in the various phases

![[Pasted image 20250504182921.png]]

Its quite simple to understand, the columns indicate the situation in which you are the rows indicate the task phase and each intersection for row-column indicates the information you can take advantage of.

#### tactile exploration

This is the part regarfing the active explorartion part in our task phases. The robot workspace might not be known in advance or can change dinamically so it requires the active sensing of the external perturbations to generate a virtual model of the surrounding.
We have to ways of doing that plus the hybrid combination of the two:
- **local tactile**: focuses on the object surface properties
- **global tactile**:involves the detection and shape estimation

**Local exploration** is enabled by extrinsic tactile sensors mounted at or near the contact inerface, they generate a tactile map used to derive a valuable manipulation parameter, the sensor area is usually smaller than the explored surfce, this is a tecnological problem leasing ot the acquisition of multiple tactile iamges which are subsequently fused to generate wise pictures. Extrinsic sensors give many advntages but are also prone to cross-talk phenomena which might affect tactile images generation an limit object features extraction.

Tactile maps are elaborated but we need to be careful with interpreations, in some cases we might be needing to do some elaborations, see the image below

![[Pasted image 20250504185144.png]]
The normal case would be the simple centroid, however depending on the kind of snesors we migh know that some parts are artifacts or noise, we could then add a thrshould or put to 0 other parts and if needed we finally use a peak detection algorithm to actually correct peaks that might not make sense and smooth them out when we know its right to do so.

In **Global exploration** we cannot only rely on tactile sensing as it lacks the capacity of extracting from local data we use proprio ception or vision, it can be implemented as a behaviour based task.

![[Pasted image 20250504185456.png]]

This is the general framework we will employ and fuse the two methods together to produce an actuator command, the global exploration also uses memory and stores stuff like object positon shape and analysis of its properties while local exploration is much more volatile and does not have memory it only gives use surface properties. The two can produce an actuation command for thje robot actuators which will then give feedback of what happened after the action is done.

When you slide over unknown surface you are basically doing objhect exploration, **tactile servoing** requests a mapping froom tactile elements located on the robot surface and specific internal structures and a model to link sensors and contact states to close the loop to derive future actions. This kind of feedback is done by touch and not vision as it is very high frequency and difficult to do using vision.

#### Sensor modeling

The **inverse sensor model** maps sensory data to the contact state and the optimal solution has to be carefully constrained to avoid ambiguity or degeneration.

The **forward sensor model** maps a fiven contact state into the expected numerically computed tactile image, the model works in combination with the *tactile jacobian* an object the receives residual tactile vectors genrated by desire along with actual tactile iamges anbd outputs the amount of correction necessary to enslave the target in the contact space.

##### Tactile servo controllers

![[Pasted image 20250504191015.png]]

in the image (a) we see a tactile servo, as we see it gets a desired contact state a sensor data and outputs a corrective contact motion in order to acomodate the motion w.r.t the desire and sensor data, in (b) we have amore specialized version where we extract the interesting feature.e.g. the rotation and give an inversion sensor model t ectract the corrective on the desired contact in order to output the difference, creating this inverse model is difficult as we have map all the possible sensing information to all the possible contacts we can desire. This leads us to (c) which we can do easily: map the contact state to sensor data, use a feature extractor on both of them, one tells you how to move to get the desired contact state the other applies corrections based on sensor data the result is the difference which you can feed into the Tactile Jacobian and you will get the corrective motion used to approach the desired contact state.

### pre-shaping objects

In the process of grasping objects end effectors approac the target to establish contact, the impact of tactile impariment on **hand synergies** used to grab something can affect time taken by a lot, but it doesn't influence the overall movements required. Impaired individuals exercise larger farces compared to unimpaired ones inertial sensing instead reduces in such cases the forces applied to external object. 

This part of the task does not rely on tactile information as sensors canno gather data until contact is established but it relies also on proximity information to play a crucial role.

### object holding

The effective object holding allows control for stable grasping, precise manipulation and safe transportation of objects. Its similar to tactile exploration as tactile maps are put into play to plan out grasping, the discrepancy between real time tactile images and the expected ones acts as a loss that must be minimized to enhance the control of the grasp.

Grasping novel objects can pose difficult challenges as we don't have prior experience and we need to exploit optimization of grasping metrics and object classification. 

### Object slippage

THe slippage sesing is essential for grasping with minimal force and preventing falling of objects, slippage detecting remains a challenge since it may result from either slipperiness of the object or an external disturbance. Avoiding slippage is a strategy that improves grip stability by regulating gripping forces without squeezing the objhects, concerning contact mechanics the onset and direction of slip can be inferred by changes in shape and size of the area that sticks to the surface, micro and macro phenomena can be sensed using vibration, pressure and vision sensors. The occurrence of slippage can be divided in two distinct phasses:
1. **incipient slip**: object is on the verge of falling, in this phase the tangential and normal forces provide a prexise inforamtion on the surface friction coefficent aiding the robot's controller in rpeventing major slip by applying suitable contact forces.
2. **gross slip**: the object moves from the current grasping configuration, planning is involved to readjust the grasp.
![[Pasted image 20250504230616.png]]

This is our situation, we take sensors data, use a grasping planner that estimation gross slip situations and feeds the result into a configuration planner, the result is a reference configuration that is feed with the proprio ception to the grasping controler, the inverse mapping is fed into the low level contro,in the grasping controller we have incipient slipping estimator which gives information to the slippage suppressor, in this case we have both corrections being summed and fed into the low level controller.

For slipping the friction is quite important, primary controller overlook frinction forces usually other models assume a known constant friction coefficient to mitigate the impact of a normal finger force. We can do online friction estimation but it is not always possible to offer closed-form solutions and necessitate to use data-driven methods
$$
f_{ext}\le \mu f_0
$$

## Active learning

In activep perception the strategy is to purposefully select and exect action to improve quality,quantity and relevance of sensory obersvations, reducing thus the uncertainty in the staes of teh world. In passive systems sensors are fixed to collect whatever data arrives while in actrive systems the agent chooses what to look for or touch. 

This approach has multiple benefits:
1. **Resolve occlusions & ambiguties**: changing viewpoints or porbing objectives reveals hidden surfaces and reduces perceptual aliasing.
2. **Allocate sensing resources efficiently**: instead of exhaustive scanning,active agents focue on the morst informative actions
3. **Robustness in noisy environments**: by combining multiple targeted observations agetns can average out sensor noise and detect subtle features
4. **Essential for embodied AI**: real world robots operate under partial observability and must decide what to sense and how to act.

An active perception system has the following components:
- Action planner: uses a utility metric to propose candidate actions
- Sensor/Observation model: predicts probability of future sensor readings given current belief and candidate actions
- Belief estimator: Maintains probabilities distribution over hidden states
- Control loop integration: executes the selected action updates belief  with new observations an repeats planning
See the image below

![[Pasted image 20250511115638.png]]
This is the loop happening we control information from the previous iteration, the current perceptual input, from these we can use the beliefs of the agent(that are influenced by the past observations and actions taken),and we can output the new state to the policy which picks the actual actions. 

We study this approach using various case studies, the first one is learning visual representations via physical interactions. 
### Learning visual representations via Physical interactions

Traditional deep vision models rely on large labeled data captured passively. We want a robot to ber able to collect self-supervised visual data with physical interactions and learn features to transfer to downstream tasks.

The robot & data collected for this staudy case is Rethnk RObotics Baxter:

![[Pasted image 20250511120314.png]]


The network architecture uses a shared ConvNet root with five conv layers extractiong mid-level features. Then you have task specific branches:
- Grasp branch: 18 independent binary classifiers prediuct success probability at 0 to 170 degrees
- Push branch: siames architectures regresses start an end positions of push action
- Poke branch: Fully Connect network regressing linear fit parameters of pressure-time curves.
- Multi-view embedding:embedding head trained with cosine similarity loss to pull views of the same object together.

With activation maximization we reveak shape selective from interaction data, we are actually performing trasnfer learning as on a 25-class household object classifciation models pretrained with interaction data imrpoe accuracy by 10% over ImageNet from scratch. And bu doing an ablation study we see that removing the grasp branch we have large drops in transfer performance so we understand the imporatnce of affordance based signals.


We now deal with a second case study

### Active Tactile Perception through belief space control

Robots often need to infer unobservable physical properties like mass,friction shape and through interaction. Traditional methods rely on hand designed exploratory action.

![[Pasted image 20250511121715.png]]


The goal of this case study is learning end to end both predictive world model and information driven controllers that automatically sequences both actions to reduce uncertainty about these hidden properties.

The heart of this experiment is its generative world mode, it disposes of a concatenated vector encoding a kinematic state (position,velocity) and latent object properties.
It uses then:
- a dynamics model obtained with a GRU:processes previous state and action to predict next state distribution capturing temporal dependencies
- an observation model with a MLP: it gives the current states and predicts the expected sensor reading pluise noise
The training object is the evidence lower bound combining reconstruction likelihood of obervations and regularization on property posterior.

![[Pasted image 20250511122919.png]]

The information gathering controller:
- Model predictive control: sample candidate action sequences, simulate beliefs forward using the generative fieltrr
- utility metric: sum of variance, or any entropy proxy, in the posterior over target properties across the simulated trajectory
in action selection choose a sequence minimizing expected future uncertainty and execute only hte first action and replan.



# Robotic middlewares and ROS

We have a lot of diverse platform components for platforms that compose them depending on the kind of personal device

![[Pasted image 20250305092244.png]]

Basically we have the hardware that allow the SO to run which gives the user the possiblity to have an application and the dev to develop an application. For robot an application is some kind of interaction in the physical world, thw So is ROS and other parts while  HW is obsvious.

A platform is then divided into SW and HW platform.

a SW robot platform includes a lot of tools used to develop application programs such as:
- HW abstraction
- low-level device
- control
- sensing,recognition
- SLAM
- navigation
- manipulaton and package management
- libraries
- debugging
A robot SW platform gives an HW abstraction that combined with SW platforms allows us to develop application programs using software platforms without knowing the HW.

![[Pasted image 20250305092735.png]]
This is the part we are talking about for now.

a robot SW platform gives us 
- reusability
- communication
- availability of support for dev tools
- active community
## Robot Operating System

ROS is a meta OS meaning that it performs scheduling loading monitoring and uses a virtualization layer between applications and distributed computing resources, it acts as a middleware and uses an existing SO, thus meta SO and not SO.

ROS focuses on code reuse maxmization in robotics, we have a set of useful characteristics:
 - distributed process
 - package management
 - public repository + APIs
 - different languages to program
## Yet Another Robot Platform
YARP is a set of libraries, protocols and tools to keep modules and devices to decouple them and like ROS is a meta OS and and has the same objective. 
It is composed by the following components:
- OS: the interface with the OS to support easy data streaming, written to be OS neutral using the opensource ACE (ADAPTIVE Communication Environment) library portable across a very broad range of environments and inehrits portability, almost entirely written in C++.
- sig: signal processing tasks to interface with common used libraries
- dev: interfaces with comon devices used in robotics as framegrabbers digital cameras etc.etc.
in YARP we have a nameserverwhich serves the purpose of maintaining a list of all YARP ports and how to connect to them like a ROS master.

The name is a YARP port usually named "/root" all other programs communicate with the name serer thorugh this port. It is abstracted away using YARP library calls but sometime client don't use yarp libraries thus the need.

Conecting to it is like connecting ot any other port but we need to find the name server and socket port number, an idea is o siply fix the name server on a known machine or you have the server record its contact informaation in config file and other YARP programs will check it to see hwo to reach the name serer if it doesn't work use multicast to discover such server.

A *Port* is an object that can read and write values to peer objects spread throughtout a network of computers and it is possible to create, add and remove connections either from that progra, the command line or another program, ports are speacialized into streaming communciation such as camera images or motor commands and you can swithc networks progtocls for the conections without changing a single line of code. The YARP library supports transmission of a stream of user data across varius protocls or other means like shared memory insulating a user of the library from the detals of the netowrk tecnolgoy.

We use *Connections* to make named entities called *Ports* communicatetogehter they forma YARP networkwhere ports are a nodes and connectons are dgs.

Ports want to move *Content* which is sequences of bytes representing user data, from one thread to another across pocess and machine boundaries, the flow of data can be manipulated an montored externally. A por can send *Content* from a thread to another or across machine boundaries and process boundaries and glow o data can be manipulated and monitored externally. A part can send Content to any other Port and can receive Content from any other number o other ports, if one port is configured to send content the have a Connecton which can be freely added or removed.

The YARP name sere tracks information bout ports, it indexes the information by name like a DNS , to communicate with a port the porperties of that port need to beknown the YARP name server offers a conveniente place to store thee propertiesso that only the name ofthe port is needed to recover them.

A YARP network is made of he following entities:
1. a set of orts
2. a set of connections
3. a set of names
4. a name server
5. a set of registrations
Each port has its unique name, each connecion has a source port and a target port, eachport maintains a list of all connections for which it is the target port. 

In the netowrk we have a single name server and the name server maintains a list of registrations each registration contais ifomration abotu a single port identified by the name.

Communication can occur between:
- two ports
- a port and a name server
- a name server and an external entity

To communicate there must be a Connection betwen them, the connections involvig a certain port can be crete destroyed or queired by commuication between the external entity and that port, usually done by sending "port commands" using the YARP connection protocol.

Ports comunicate with the name server using the "YARP name server protocol"  and such comunication method is used to create.remove,and query registrations.

Here you can see an example of a YARP network

![[Pasted image 20250315113944.png]]

# Navigation

As of now we know that we can assume the robot to be placed in the space, but we need to be sure of how to locate the root in the space.

we want a reference coordinate system, a first idea would be to place it in space as below.
![[Pasted image 20250325234012.png]]
 the second approach is the use the robot itself.
 ![[Pasted image 20250325234041.png]]
 

in the second case we just treat everything relatively to robot and usa polar coordinates to represent ostacles, see the iamge below for an example.

![[Pasted image 20250325234203.png]]

We can also transform using polar coordinates to switch at will from world reference and robot reference frame in the velocity space

![[Pasted image 20250326093536.png]]

The navigation problem can be stated as reaching a final position from a starting position in geometric/sensory terms while avoiding obstacles.

The way we can tackle this is with these instruments:
- Localization: we have a geometric position $(x,y,\theta)$ in an absolute reference system or sensory state in the robot environment.
- Maps/Models: formalization and representation of an environment
- Planning: planning robot movements in the environment

### Maps
A map can be of two kinds:
- Metric/Geometric Maps: they represent the objects in the world using their size and coordinates in the absolute reference frame
- Topological Maps: the represent the objects in the world on the basis of their characteristics and relations among them

The relevant known methods are occupancy grids and geometric descriptions. 
For the occupancy grid the environment is represented on a bidimensional grid usually in a cell fashion where we discretize the world in a simple grid of swaures, see below:

![[Pasted image 20250326094800.png]]

Grids can have a fixed size  like above and below

![[Pasted image 20250326094954.png]]
or variable size to for more accuracy

![[Pasted image 20250326095029.png]]

In the geometric description instead we get an environment represented with geometrical means like segments,obstacles and free space
![[Pasted image 20250326095219.png]]

As for topological maps we have the environment defined in terms of points of interest for the robot and relations among points of interest and the relations between them.
We say a an object to be a *point of interest* when it is relevant to the navigation or the task of the robot.
A point of interest is either define by a position in the robot space or a sensory state.
![[Pasted image 20250326095938.png]]
we can combine a metric map with object recognition and get a meaningful oject labeling for locations on the map.
![[Pasted image 20250326100113.png]]

An easy to understand example for a complex home cis mapping the home envronemnt with points of intereset, first map the in the space
![[Pasted image 20250326100209.png]]
enumerate rooms and for each room number walls clock wise and for each wall number the points clockwise.
![[Pasted image 20250326100232.png]]
finally we get our graph of relationsships.
![[Pasted image 20250326100307.png]]

We can also mix the geometrical and map approaches 

![[Pasted image 20250326100542.png]]

## planning

We now deal with path planning after describin the enviroment around us, the objective is the one described from before, in this case we start a fro a configuration to reach another and avoid obstacles in the mean time.

A simple way of doing so is to consider the robot size, increase the object size virtually and treat the root as a point .
![[Pasted image 20250326100815.png]]

Planning is roughly divide into:
- planning of the path: find the trajectory s.t. they avoid obstacles and reach the goal configuration.
- following of the path: execute the trajectory and avoid unexpected obstacles.

The configuration reachable from the robot are called *robot space* indicated as $C_{space}$ , in this space the robot is a point and obstacles are represented and occupy a ragion called $C_{obstacle}$ while the free region is called $C_{free}$ the path is a trajectory betwen two configs $q_{init}$ and $q_{goal}$ in $C_{space}$ and it belongs to $C_{free}$.

Path planning is done in three ways
### Roadmaps
We connect points in the free space in a network, called *roadmap* made of unidimensional curves in the free space. We just want to connect them along this roadmap.

In the Roadmap we can use two methods:
- the visibility graph
- and the Voronoi diagram

#### Visibility graph
Here we take a visibility graph $G$ whose nodes are the intitial configurations $q_{init}$ and $q_{goal}$ and all vertxes of  the polygons which represent obstales in the map, the edges of $G$ are all the segments that  connect wo nodes in G and don't intersect with obstacle polygons.
![[Pasted image 20250326102121.png]]

A wight can be associated to the edges and corresponding to the distance between the nodes that they connect, a path from start anf stop is found in the graph with alogirthm for minimum paths wihich minimizes the distance.

#### Voronoi diagram

We define free confiurations in teh free space as equidistant from the obstacle region, if the obstacles are polygons the disgram consists ofa finte set of segments and parabolic curves i.e. our roadmap.
![[Pasted image 20250326102427.png]]

We then find a path the goes along those curves to reach our end goal.

### Cell decomposition

It consists of decomposing the free space in regions, named cells, s.t. a path netween to adjacent ones is found and the map represened is called connectivity graph. Two nodes are connecte iff the twocells tha they repsent are connected.
![[Pasted image 20250326102856.png]]

In this case we just use the vertexes to draw vertical lines, each line stops whe nit encounters an obstacle or a wall, when this is one we can treat each one as a nodes in a graph and get our connectivity graph, in the map we can calculate the path and the result of this rocess is a sequence of cells called *canal*, the final path is obtained by taking the mid point of the cell boundaries.
![[Pasted image 20250326103220.png]]#
### potential fields

The robot si a point in space that moves under the influence of an artificial potential produced by the goal configuration and obstacles, the final configuration hsa an attractive potential which pulls the robot towad the goal and obtacles have a repulsive potential which pushes the robot away from them. By summing attractive and repulsive potentials we generate a foce moving the robot toward the goals while avoiding obstacles.

![[Pasted image 20250326103839.png]]

![[Pasted image 20250326103858.png]]
 This is the examples we where toaking about, now we go deeper on potential fields.
We take differentiable potential function $U$ with local minima in $q_{goal}$ , we then pick two function 
$$
U(q)_{att}=\text{attractive potential function},U(q)_{rep}=\text{repulsive potential function}
$$
And our final potential function then becomes
$$
U(q)=U(q)_{att}+U(q)_{rep}
$$
and for point in space the motion of direction is given by
$$
F(q)=-\nabla U(q)=-\nabla U(q)_{att}-\nabla U(q)_{rep}=\begin{bmatrix} \frac{\delta U}{\delta x} \\ \frac{\delta U}{\delta y}  \end{bmatrix}
$$


We need to pick attractive potential, with the requiremnt to have a local minima in $q_{goal}$.
The function we use is the parabolic potential.
$$
U(q)_{att}=\frac{1}{2}k_{att}\rho^2_{goal}(q)
$$
With $\rho_{goal}(q)=||q-q_{goal}||$ i.e. the distance from the goal.

and 
$$F_{att}(q)=-\nabla U(q)_{att}=- k_{att} (q-q_{goal})$$
Now for the repulsive potential we want a protective barrier around the obstacle region to avoid robot contact with the obstacles and not affect it when it is far from obastacles.

$$
U(q)_{rep}=\begin{cases}
\frac{1}{2}k_{rep}( \frac{1}{\rho(q)}-\frac{1}{\rho_0} )^2 \text{ if } p(q)\le \rho_0\\\\
0 \text{ otherwise}
\end{cases}
$$
with $\rho(q)=\text{minimum distance from the object.}$

Now we can place the desired goal and its obstacles with the potential we just discussed, for the current robot confiuration just:
1. sum total force vectors $F(q)$ generate by the potential fields
2. set desired robot velocity $(v,w)$ proportional to $F(q)$.

We can have local minimias occuring when repulsive forces nullifies the attractive force in points different from the goal.

### Path planning

Now we deal with path planning, it differs depending on the representation of space we use.
For topological maps:
- find path in the graph
- translate it into instructions
### Path following
Now we have to stick to our plan, this is the diagram
![[Pasted image 20250326111319.png]]

The result of the planning in a series of points in space $(x_1,y_1,\theta_1),\dots,(x_n,y_n,\theta_n)$ , we find trajecotry and time laws that the robot will follow between each couple of points and te controller makes the robot execute such trajectory.

![[Pasted image 20250326111536.png]]
We use a trajecotory planner to actually follow the the sequence of points, Its is just an interpolation of points 
![[Pasted image 20250326111657.png]]

Now we deal with the hardware architecture of a mobile robot.

![[Pasted image 20250326112332.png]]


The actuatore control has two ways of controlling the actuators:
- position control: setting a position to reach, and it is the rbot finding acclerations and velocities to actally set motors to reach the desired positions
- velocity contrls: it consists in setting a velocity and an acceleration to the wheel motors
![[Pasted image 20250326112914.png]]
This is te Proportional, Integrative and Derivaive control, as we see this is just the 


# Neural robot brain

We can use a nerual network to learn a function for direct or inverse kinematic.
![[Pasted image 20250327215407.png]]

Basically we can learn a function that learns either joint angles from end effector position or another hat does viceversa, of course the most difficult one is the inverse kinematics since we need to define joint angles from end effector that could be way more and is difficult to predict basically for a lot of configurations we have the same end effector and the network will not be able to generalize to that.

We can teach the network to do a certain position by just ding random deltas to the joints movements which could be a way to make learn a movement but we don't control it.

So in short we create a dataset of $<\text{joint\_positions},\text{effector\_positions}>$ using direct kinematics and use effector position as a way to predict the joint positions, we go ass follows.

![[Pasted image 20250331161833.png]]

this is the situation we are dealing with where the netowrk structure is unknown.

Such learning method is always possible, in the case of "soft continuum robots" we can exploit the relation between cable tendion and end effector position to control the robot tip.

The robot is shown in the image below

![[Pasted image 20250331162314.png]]

Te soft arm is controlled by learning the inverse model that allows us to control the end effector position through the cable tension, we can collect points and exploit approximation capability capability of the NN as for the rigid robots.

![[Pasted image 20250331162507.png]]

A neural network can be used to solve such problem and thus we mode the relation as 
$$
x = f(q)
$$
With $x \in \mathbb{R}^m$ which is the orientation and postion vector of the robot and $q \in \mathbb{R}^n$ which is the joint vector and $f$ is a surjective function.

What we do is using a feedforward NN to learn the following relation
$$
(q_i,x_i,x_{i+1}) \rightarrow (q_{i+1}
)
$$
i.e. we use the current end effector postion,and joint positions and next end effector position to learn how to place the joint vector next.

The samples as we have said are generated in this case we have to make sure that $|q_{i+1}-q_i| \lt \epsilon$ for each sample $(q_i,q_{i+1},x_{i+1})$ .

the values of $x_i,x_{i+1}$ are generated using the direct model.

for the training phase we have to generate a sequence like this 
$$(q_{i-1},x_i,x_{i-1}),(q_i)$$
which is our sample to learn for the relation above, then in the test phase(or evaluation phase) we use the same points but in a different logic the relatio becomes 
$$
(q_i,x_i,x_{i+1}),(q_{i+1})
$$
basically in the first part you just set up where you want to be from where you are predicting what the next move is starting from the next point, the actual joint position and actual real position . At test time you just give the NN the same information knowing where you want it to be and it will try to predict the corect joint positions.
### the static setting 

We can develop a local representation by linearizing the function on a point $q^o$ and we get 
$$
\Delta x \simeq J(q^o)\Delta q
$$
With $J(q)$ which is the jacobian at point $q$ and we have the infinitesimal changes $\Delta x$ and $\Delta q$ in $x$ and $q$ bascailly we assume changes to be very easy to approximate as they are too small. 
We use the learned mapping from before and we manage to make the controller learn the actual mapping starting from a default state.

![[Pasted image 20250413120757.png]]
![[Pasted image 20250413120806.png]]

This is an example of robots that uses such techqnique.

The results are quite good as for:
- line following
- disturbance rejection
- line following with fixed oreintation
- line following with obstacles
the robot displayed very good behaviour and very low error by using position information only or by also giving orientaion information
![[Pasted image 20250413121015.png]]
### dynamic trajectory generation
This is a static task where we just end up in a fixed position, we can actually now study a dynamical trajectory generation.

![[Pasted image 20250413121054.png]]
To do so we must first generate the points to learn a relation as before, in this case the procedure is as follows but what we doo is creating a time series of positions with different lengths.

the method is always the same just transform the trajectories from $T_X^{tar}$ from the task space to the actuation space using the inverse kinematic model like before and in this ways we create the equivalent in actuation space indicated as $T_{\tau}$ 
![[Pasted image 20250413121402.png]]

In this case there is no possiblity to have a steady state assumption like before in the static setting so we can actually use the manipulator and record  its actual trajectories but it won't be very good

![[Pasted image 20250413121530.png]]

Note: here $T^{ee}_x$ is still in the end effector position generated by the manipulator.
the actual training regime is implemented as indicated by the following schema
![[Pasted image 20250413121658.png]]

This is the trainin pipeline, we first gather the generated trajectories, translate them into the actuation space , i.e. $T_{\tau}$, and use it as ground truth and the inputs upon which we learn are the values outputed from the robot,i.e. $T_x^{ee}$, the controller is a LSTM RNN.

Then we do the testing, in this case it is done by feeding trajectories like in the scheme below
![[Pasted image 20250413122845.png]]

basically we feed the next plus the previous 2 and actual position to the O.L controller which generates the position which will be passed to the actuator and we obtain the end effectorp position $\hat{x}_{t+1}$ , the resulted is calcualted as the difference from what it should be $x_{t+1}$ and what it is indicated as $\Delta x$ .

In this case the tests happen on 15 random trajectories not seen in trainnig, the controller outperforms simpler IK controler by 4mm in the cartesian space.

![[Pasted image 20250413142309.png]]

another approach is to feed the NN with a value at time $t$ , indicated as $y(t)$ and feed a NN a past time value $y(t-\Delta)$ and we can use it to make the network learn the correlation between $y(t)$ and $y(t-\Delta)$ as shown below

![[Pasted image 20250413142923.png]]

then at test time we get an estimation of $y(t+\Delta)$ 
![[Pasted image 20250413142948.png]]

Note that we could have interferences, if we have a sine regression pericoducally we get the same values depending on the point at which sin is evaluated thius creating confusion in this case we shoudl be making more comlex input by adding the time or time embeddings depending on our case.
![[Pasted image 20250413143144.png]]

prediction can also be enhanced by feeding multiple delays up to n times backward as indicated in this figure
![[Pasted image 20250413143221.png]]

a prediction can also be generated with any apriori knowledge of the signal obtaining faster convergence with less inputs seen. we use a simple model basedf on a single neuron perceiving as input the current and past values .

![[Pasted image 20250413143532.png]]

this the model perceiving current and past values and makes a prediction using a linear layer, what we  train is a set of current and past values to predict future values up to $n$ point in the future or in notation our dataset is made of $\langle X1-X10,X20\rangle,\dots,\langle Xn-Xn+10,Xn+20\rangle$  and the network output will be $y10,\dots,yn+10$ . in shorte the idea is to feed the difference as happens for the sine regression and schema we have seen  before instead of trying to preidct based on the timestamp and we care about the diffrence on the assumption of some kind of repetition.

## ballistic tasks

We can now dwelve in the ballistic tasks which involve throwing objects after picking them up. for this we have to follow the usual pipeline: 
- describe the experimental setup qith equipment and throwing specification
- describe the necesasry controllers with their architecture
- see results with performance evaluation
We see an example of someting by the teacher for a paper here.

this is the experiment setup, first we see the phsical parts involved:

![[Pasted image 20250413144327.png]]

the task to solve is desribed in the following schema:

![[Pasted image 20250413144423.png]]

this is bascailly the actuation space on the left and the robot's trajecotory on the right.

As before you have to generate a trajectory from before so we do that in this case we have sling so we generate somertinhg regarding centripetal force where we have spiral at different amplitudes and different velocities gnerateing diffrent xyz cartesian coordinates

![[Pasted image 20250413144632.png]]

we also record the landing positions of two objects for each trajectory after throwing objects, theyr are two in this case and a re indicated s blue and orange points in the right graph.

For this experiment the release depends on the weight of the object and it has na intrinsic variability, it is considered released when its distance from the end effector is 5% greater than before the release command. we have opening delay of $\Delta=0.10s$, which how much it takes to relase the object, and considering this values and the actuaton frequency 20Hz the object on which this delay is valid are those with weights 45 and 60 g.
![[Pasted image 20250413145230.png]]
this is the empirical results of what we have said before.


This is the control scheme implementing the robot movement

![[Pasted image 20250413145735.png]]

we can see the results of the model selection process

![[Pasted image 20250413145756.png]]

in this case the model with the lower error among all possible history differences.

# Robot control

For this part we start going deeper on robot control, our main subject will be kinmeatics and we refresh notation for this part

$$
x=k(q),q=k^{-1}(x),k(\dots)=\text{equation of direct kinematics}
$$
and the two point we care about will be 

$$
x=\begin{bmatrix}
x\\y\\q\\\phi\\\theta\\\psi
\end{bmatrix}=\text{cartesian coords + orientation},q = \begin{bmatrix}
q_0\\q_1\\q_2\\q_3\\q_4\\q_5
\end{bmatrix}=\text{coordinates in the actuation space}
$$

We can take into account also the velocity of the end effector and the joints which we express as
$$
\dot{x}=J(q)\dot{q},\dot{q}=J^{-1}(q)\dot{x}
$$
with $J(q)=\text{jacobian matrix}$ , this is the *velocity space*. basically we need the velocity of the joints and the jacobian of the joints to retrivee the end effecotr position. Conversely given the jacobian inverse and the end effector velocity to get the joint velocity. This is used when we want a dynamical robot.

## Trajectory planning

The objective here is to generate reference inpouts for the rpobot ocntrol system which will ensure the rbot end effector to follow the desired trajectory when moving from $x_{start}$ to $x_{f}$ , our final destination. Our path is formed by a sted of points, be it on joint or operational space, that the robot as to reach in order ti perform the desired movement, a trajectory is a path with a specified time course meaning we have velocity and accelleration at each point.

In input we get: path definition, path constraints plus constraints from the robot dynamics(certain movements cannot be performed). The output will be in the joint space, joint trajectories, and in the operational space, end effector trajectory.  The vecotrs we care about will be two sets, for the joint space we have:
- $q(t)$= position
- $\dot q(t)$= velocity
-  $\ddot q(t)$= angular velocity
As for the operational space we have:
- $p(t)$= position
- $\Phi(t)$=orientation
- $v(t)$=linear velocity
- $\Omega(t)$=angular velocity
To be albe to displace the robot between points in a certain time interval $t$ we need to pick profiles that do the interpolation for us in order to set the course of motion,we can have a trapezoidal velocity profile or the following functions:
- cubyc polynomial: allows to set initial and final values of joint variables $q_i$ and $q_d$ , the initial and final velocities
- fifth-degree ploynomial:allows setting everything a cubyc polynomial does plus the initial and final accelerations.

For this model we have a set of phases where velocity adn accelration changes, start by looking at the image below:
![[Pasted image 20250413160407.png]]

we have a starting point $q_i$, in that point we ramp up veloity and accelration is constant, after the starting point we are in the segment $[t_c,t_f-t_c]$ where velocity is constant and no acceleration is present and finally we reach the last segment where acceleration is negative as velocity goes down.

The trajectory point (Position) is a mixed polynomial composed of a linear segment and two parabolic parts near the initial and final positions.

The three parts act as a three distinct polynomial and the final function $q(t)$ is seen as the combination of these three functions:
$$
q(t)=
\begin{cases}
q_1(t)= q_i + \frac{1}{2}a_ct^2 \text{ } 0 \le t \le t_c\\
q_2(t)=q_i + a_ct_c(t-\frac{t_c}{2}) \text{ } t_c \le t \lt (t_f-t_c)\\
q_3(t)=q_f - \frac{1}{2}a_c(t-t_f)^2 \text{ } (t_f-t_c) \lt t \le t_f 
\end{cases}
$$

this serves the purpose of describing velocity adn acceleration now we would like to do interpolation by $n$ points.

Our rajectory is a sequence of linear segments that decribe movement between two segments and parabolic segments represententing the transitions, in notation we express this as:
- a set of points $\{q_1,\dots,q_n\}$
- a set off time intervals $\{t_1,\dots,t_n\}$ 
then we have an interpolation function 
$$
q(t)=\begin{cases}
	a_{1,k}(t-t_k)+a_{0,k} \text{    } t_{k+1}+\frac{\Delta t_k}{2} \lt t \lt t_{
k+1}- \frac{\Delta t_{k+1}}{2}	\\
b_{2,k}(t-t_k)^2+b_{1,k}(t-t_k)+b_{0,k} \text{ } t_{k}-\frac{\Delta t_k}{2} \lt t \lt t_{k}+ \frac{\Delta t_{k+1}}{2}
\end{cases}
$$
With polynomial coffiecents $a_{0,k},a_{1,k},b_{0,k},b_{1,k},b_{2,k}$ to be deterimned
![[Pasted image 20250413163617.png]]
basically the first case is the linear interpolation between points and the second case in the quadratic interpolation whch maintain the correspondence with linear interpolations.

## Motion control in joint space

We use such interpolation methods from before to move the end-effector from $x_i$ to $x_d$ without taking into account the trajectory. The final position is just tranformed into the joint space as $q_d$ with the the inverse kinematic transformation as $q_d=K^{-1}(x_d)$ , the same goes for $x_i$ and then we move the current positon $q_i$ to the desired position $q_d$.

![[Pasted image 20250414091250.png]]

This is the general robot control scheme, starting from the trajectory planning we can use the inverse kinematics to convert the desired $x_r$ from operational space  into the joint space equivalent $q_r$then we feed this into. 
In this tage the trajecotry  of the end effector in the operational space is not predictable due to the non linear effects of direct kinematics.

On the other end of the pipeline we have the $x$ vector describing the actual pose of the end effector usually made of six variables i.e. 
$$
x=[x\text{ }y\text{ }z\text{ }\phi\text{ }\theta\text{ }\psi]^T
$$
the position is determined by the the vector from the origin of the world coordiate fram eto the robot end point. while orientation is determined by the usual three variables:
- $\phi$ is the the variable around the $z$ axis
- $\theta$ is the variable around the $y$ axis
- $\psi$ is the variable around the $x$ axis

The loop for us "closes" with $q$ which is the joint coordinates given back by the robot, the result is fed to the sensors which measure if it is correct w.r.t. the real world position and then we can calculate the results as $\tilde{q}=q_r-q$  which is fed to the control, we can compare basically $q_r$ with the actual joint configuration by the difference.

The control block will then output the final $u$ value which is fed to the actuators that enforce the forces or torques necessary for the robot motion.


The interesting part for us is the position control.
![[Pasted image 20250414093333.png]]

The control is based on calculation of the positional error and determination of control parameters which enable reduction or suppression of the error. such positional error is reduced for each joint separately which many that there are as many controllers as there are DOFs!

the reference position is obtaied as the difference betwen the previous $q$ and the desired $q_r$, what we actual miss is the computation of the $u$ signal which is done as 
$$
u=K_p(q_r-q)-K_d\dot q
$$
in this scheme we also use the velocity of the joints in the model along with the difference, $K_p$ is a diagonal matrix of the gains of all the joint controller which is called the *proportional position gain*. using such matrix amplifies the error $\tilde q$. this provokes a movement in the direction of the reduction of the positional error, since it is proportional we could have overshooting which not allowed for our purposes as it could result in collisions. This issue is solved by closing the loop with negative sign i.e. subtratcting $K_d\dot q$ which dampens the system where $\dot q$ is the velocity of of the joints and $K_d$ is the diagonal matrix of velocity gains.

all these serve the purpose of controlling the robot at higher velocities and bring stability into the system.

This dampens the system in the fastest part of the system which is not actually necessary so we can upgraded the PD controller to refrence the velocity signal of the desired position.

$$
\dot {\tilde q}=\dot{q}_r-\dot{q}
$$
and we see below the modified scheme.
![[Pasted image 20250414095132.png]]

the control lgorithm then becomes 

$$
u=K_p(q_r-q)+K_d(\dot{q}_r-\dot{q})
$$
we use the difference in velocities instead of the total velocity which reduces the damping effect.

It seems obvious that we should set appropriately $K_p$ and $K_d$ :
- for $K_p$ set high values when you want fast responses
- for $K_d$ sets the damping and garantuess a fast response without oscillations
For both K's te parametrs aare set indipendently for each joint.

A robot mechanism is influenced by many forces: Inertial Coriolis, centripetal and gravitational.

Also frictional forces happen in robot joints and must be included in the robot dynamic model, for a simplified model we take ino accout only viscous friction which i s proportional to the joint velocity encoded into $F_v=\text{diagonal matrix of joint friction coefficents}$.

All these forces must be overcome bu robot actuators and we can express this in the following equation
$$
\tau = B(q) \ddot q + C(q,\dot q)\dot q + F_v\dot q+g(q)
$$
where the terms have the following meaning:
- $\tau$ is the torque
- $B$ is the inertia term
- $C$ is the coriolis term
- $F_v$ friction of coefficents
- $g$ is the gravity term

We hae to go term by term here:
- $B(q)\ddot q$ : this is the inertia term where the more mass / the faster it is accellerating the more more torque we need.
- $C(q,\dot q)\dot q$: basically these forces are influenced by the rotation of joints that influence other joints that might be attached, usually ignorable in other settings but not for the dynamic ones, the more we are rotating faster the more torque is needed.
- $F_v \dot q$ is the viscuous friction which is related to internal riction like internal damping or velocity related like air, the more we have the more torque is needed.
- $g(q)$ which is related to gravity, the more it is te more torque we need.

all these are not included in our regular controller. knowingthe dynamic model we can predict the necessary forces to perform motion and are enerated regardless of the position error signal.

in quasi-static conditions, i.e. robot not moving or moving very slowly, we can assume zero accelerations $\ddot q \sim 0$ and velocities $\ddot q \sim 0$ so we get that the dynamic model is $\tau \sim g(q)$ .

we can add such gravity compensation in the scheme from before
![[Pasted image 20250414103508.png]]

in the new scheme $\hat g(q)$ we have another force which is the torque , in our case is just the gravity approximation, in this way the PD controller doesn't ave to reduce errors caused by gravity.

## Motion control in the operational space

All the control schemes are based on internal coordinates(joint positions). what we care about in the end is the end-effector posiion instead of robot joints diplacements.
In the movement from $x_i$ to $x_d$ the robot follow a trajecotyr in the operational space according to a planed time law.

Our robot follows a trajecotyr determined by thefollowing functions
$$
(t,p(t),\Phi(t),\dot p(t),\omega(t))
$$
we have the time $t$, a position $p(t)$ , a set of joint velocities $\Phi(t)$ a set of accelerations$\dot p(t)$ and the set f currents and voltages to give to the motors to in order to apply to the joints the velocities and accelration calculated.

The velocities and accelrations are calculated usiga jacobian and expressed in cartesian cooridants and also voltages and currents give the motors.

### Motion control based on the jacobian matrix

We can connect the forces acting on the robot end-effector and the joint torques using the jacobian matrix 

$$
\tau=J^T(q)f
$$
with $\tau$ being the joint torques and $f$ the force at the robot end point, our aim is to control the pose of the robot end effector defined by $x_r$.
This is a different model that is based on the torques baically, it still produces $u$ but uses another method to do so.

![[Pasted image 20250414213733.png]]


the $x$ values are still made of the classical 6 vvalues like before, in this case we don't use a sensor to assess our actual position and modify the position w.r.t. the error and it is usually is the case with robots. in this case we can only use equation of the direct kinematic model i.e. $x=K(q)$ to determined the difference from the desired positition.
$$
\tilde{x}=x_r-x=x_r-K(q)
$$
such error must be reduced to 0, first we use a proportial control system with teh control matrix:
$$
f=K_p\tilde{x}
$$
this produces the $f$ we use to compute $\tau$ before which will be used to computer the final values fed into the robot i.e.
$$
u=J^T(q)f
$$
so we calculate the desired joint torques. the reason for the jacobian is easy to understand if you take the assumption that you have a simple mape from q to x as a function, what you want to know is how much it varies if you move a little bit the values in the joint space and what effectos it produces on the task space,the jacobian tells you extacly that and makes it so that you move accordingly when error is high and the jacobian is very different from 0 you move a lot in allora other cases you don't, so this translates into high movements when there is alot of torque available and low ones when it is not and you get near 0 values. Another view is that you move by the the derivative direction of decreasing the difference using $f$ mediated by the actual torque you can do in that point for the joints.

$$J(q)=\frac{\partial k(q)}{\partial q}$$
$J$ is basically the derivative of the direct kinematic model which makes sense and form that model we can estiamate the difference and use it to give the feedback on how much we can actually exert torque mediated by the vicinity to our error.
## principles of motor control

Human can showcase very complex movement behaviours, this works as we have various regions taking care of control issues like  disturbance rejections,state-estimation,predictions and internal models about the body and the world as shown in the image below.

![[Pasted image 20250414223639.png]]
this showcases the thing we just said as we have various mechanisms of thought that take action in the brain and act on the arm/muscles. 

we refer to the part of the central nervous sutem involved in the generation of moevemmtns as the motor system, it recevies sensory inputs from multiople sensory receptors and provides motor outputs to multipled muscles. Thje mammalian motor system is divided into four interacting subsystems each playing a rol in the control of movements.

![[Pasted image 20250414225543.png]]

In articular here we focus on the *Cerebellum* which handle the adaptive aspect of the movement, iut uses similar computations for diferent cognitive and motor functions usually explained by a framework of adaptive filters.

![[Pasted image 20250414225740.png]]

We use a simple feedback based model 
![[Pasted image 20250414225836.png]]

basically we have the brain pickinig what to do and combined with where to do which is the result of delay in our capacity to observe and the feedbackj form proprioception on our muscles we send omotor commands.

A very problematic issue is using a simple feedback loop to control with proportional feedback gain, basically we see tha following a trejecotyr goes as planned when we can observe some disturbance but there is now delay, the system becomes disturbed

![[Pasted image 20250414230628.png]]

if we are to have some delay we obtain some oscillatory movement in the response

![[Pasted image 20250414230700.png]]

internal models can deal with such delays and disturbances.

![[Pasted image 20250414230726.png]]

The inverse model can take the desired input and produce commands that move accordingly the body, we then add a feedback controller that computes some adjustments based on the senosry system delayed feedback of where you are, this is used to compute an adjustment to move the body.

this is our internal model it can be straightforward or forward and take a copy of the feedback integrate it with the integration module and get back value that subtracted to the original gives the correct movement to the feedback controller.
In particular a forward model should take a copy of the muscle commands from the feedback controler and generate predictions of the current or future state of the body.
![[Pasted image 20250414231405.png]]

in the forward model we have an integrator that should continuously integrate the predictions of the internal models with the respective delayed sensory feedback and produce the most likely body state.

this requires the use of some probability

![[Pasted image 20250414231835.png]]

in short the forward model estimates the probability of the original state $y'$ starting from $u$ ,$p(y'|u)$, and then we have a posterior estimate of the original movment plus some information taken from the measurements i.e. $p(y''|y',z)$ .


What we aim is emulating the human capability of aapting while moving, to do so we can use the inverse and the direct model to adapt by mean of motor errors, in this case we show first the inverse model approac:

![[Pasted image 20250414232736.png]]

in thi case we just improve using the output of the feedback controller and learn from it in order to combine a feedforward motor command with a feedforward motor command, in training we get that the feedback controller error should become lower and lower as the the inverse model learns to correct the errors by itself.


The other approach is using the forward model that use the feedback controller output to store copies of the command and outputs a correction the gets fed into the feedback controllerand we use the combnined desired trajectory, the delayed actual trajecotry and the previous output to correct the feedback controller output and lower again the difference between the desired trajectory and the actual trajectory. basically we have a recurrent connection from the cerebellum to the pre-motor region.
![[Pasted image 20250415113103.png]]

Another biologic system we can study is the vestivular system, it is made of two components:
1. the semicircular canal system, detecting rotational movements
2. otolithic organs which detect linear accelerations
![[Pasted image 20250415114238.png]]

these two informations are tinegrated with the retinal motion information to allow correct representation of head position thus it contributes to our balance and sense of spatial orientation.
![[Pasted image 20250415114248.png]]

the actual neural mechanism for the perceptio from the eayes is done bu the Vestibul Ocualr Reflex neural mechanism wihich allows the movements of the eyes.

![[Pasted image 20250415114411.png]]

this can actually be emulated thorugh a network

![[Pasted image 20250415114528.png]]

# robot vision

The main pipeline of a compute vision process goes as follows

![[Pasted image 20250415115416.png]]

we start with capturing illumations and material properties which generate reflections i.e. light on the scene, such light allows for image formation which is captured by a sensor getting the light, transduction from such sensor gives back a digital image which we can process on aperforme feature extraction for various applications.

## Light and color

In general light is a mixture of many wave lengths represented as a function of the wave length $\lambda$ which is a function $E(\lambda)$ which is the intensity.

This is a mathematical model our own perception is in term of subjective quantities like brightness and color, the light we ccan actually see lies between 400nm and 700nm of wavelength as shown in the image below.

![[Pasted image 20250415120428.png]]

For the retina of the human eye we use a *foveal* region which is 0.6mm in diameter,witha 5 degree field of view and contains most 6 million cone cells:
1. 65% sense red
2. 33% sense green
3. 2% sense blue
unconsciously scan our high resolution fovea over to world to build a large scale mental image of our surrounds in addition withg 120 million rod cells that are motion sensitive and distributed over the retina.

A digital camera sensor is analogous to the retina,but we have an array of lightr-sensitive photosites on a silicon chip, each one of the order of 1-10um sqyare abd givesa signal proportional to the intensity of the light falling over its area.

for a camera photosites are covored by color filters which pass either red, green or blue to the photosites. such photosites should be arranged on a singular grid to do so a common way of disposing them is the Bayer pattern which uses 2x2 filters:
- twop green filters
- one red
- one blue

below an example

![[Pasted image 20250415121146.png]]


## image formation

we treat an image as a function $f(x,y)$ where $(x,y)$ are the spatial coordinates and $f(x,y)$ is the grey level/light value.

for a colored image the value is actually a set of three values 
$$
f(x,y)=\{f_{red}(x,y),f_{green}(x,y),f_{blue}(x,y)\}
$$
The actual image formation happens with the following pipeline more or less

![[Pasted image 20250415121413.png]]


what we get is the image digitized through the technqiue we have said before and get light levels plus the colors using the color formation methods.

a digital image is sampled by discretizing spatial coordinates, then we quantize light by discretizing grey level s and finally we get a pixel which is the digital image element.

For an image we have the classical informations:
- resolution which is the number of pixels
- image depth which is the number of colours for the pixels
![[Pasted image 20250415121721.png]]

Color images are actually made of three channel usually the most common are forme by a RGB triplet
![[Pasted image 20250415121945.png]]

the more a channel has bits the more we have detail, we can have binaryu image base of 1 bit i.e. 1 or 0, we can have grey levels which are 8 bits or 24 bits which is the true color.
also we can vary the resolution of the images

![[Pasted image 20250415122055.png]]

for our purposes we just focus ono the image processing part.

## image processing

we wish to turn one or more images $f(x,y)$ into an image $g(x,y)$ with various methods
![[Pasted image 20250415122157.png]]

there are various tecniques of various kinds, each of hem serve a purpose,see the scheme below
![[Pasted image 20250415122244.png]]
### image processing operators

operators can work at various levels:
- point/pixel: the value of point $O(i,j)$ depending on the input images values $I(i,j)$.
- local/spatial: the output image value in point $O(i,j)$  depends on input image values in a neighborhood of point $I(i,j)$
- global operators: the output value in point $O(i,j)$ depends on the whole input image values.

Monadic operators are operators that do image-processing operations producing as a result an image of the same size as the input and the output is a function of the correpsonding input

$$
O[u,v]=f(I[u,v]) \text{ }\forall (u,v) \in I
$$
we can use a particular clas of monadic operator to change the type of pixel data, a monodic operator  to convert a color to a greyscale image with each input corresponding to the luminance.

### image histograms

We can analyzer the distribution of pixel values to provide useful information about the quality of the image and composition of the scene, the histograms gives us a histogram of how many times a pixel value occurs. We can use them to obtain info like under exposure if the values of the image are closer to the right we get that we have shifted histogram on the left while for over exposure we are shift to the right.

![[Pasted image 20250415215702.png]]

here distribution is not uniform, we have various peaks which are depending on the scale at which we consider data.

An histogram can be coded as a a piar of vecotrs $[n,v]$ where the elements of $n$ are the number of times a pixel occurs with the value of the corresponding element of $v$.

in this case peaks are defined as the points where the number of occurences is greather tha ntheir immediate neighbors in a raidus of neighbours which we have to set.

usually the peaks corrosponde to the histogram corresponding to particular populations of pixels in the image while the lowest ones correspond to ground and roof. middle peak correspons to sky pixels nad highest to white walls.

Of course an histogram is not aware of spatial distribution.

### monadic operations

a common monaidc operation is threhsolding, it is a logical operation that turns values of pix els into two possible values.

other operations to do are converting from color to grey and lightening and darkening.

![[Pasted image 20250415220716.png]]

another possiblity is inverting grey level

![[Pasted image 20250415220742.png]]

another possiblity is a trasnformation to a binary image with black 0 or white 255 values.

$$
\begin{cases}
255 \text{ }if\text{ } f(x,y) \gt T\\
0 \text{ }if\text{ } f(x,y) \le T
\end{cases}
$$
we can have T be variable or constant and can be determined by empirical methods.

### diadic operators
We have two input images result in one output image all three of the sam size, each output pixel is a function of the corresponding pixels of the two images

$$
O[u,v]=f(I_1[u,v],I_2[u,v]) \text{ }\forall(u,v) \in I_1

$$
some examples can be binary arithmetic like addition subratatction multiplication elements wise et.cetc.

One example is chroma keying, we can use it to super impose images of objects, we just put the subject on a blue or green background and use the distribution on the green channelto distringuish between the background and the subject, the the high pick contains the poplation fo the background everything else is the subject in question and then we can create a masjk corresponding to such pixels in the background region by selection anything with green less than a certain theshold which is just before reaching the peak corresponding the the background in our case. after that we can do what we want withit.


this is aa very special case but in genral what is background and what is not is illdefined and application specific

One of the most useful processes we can perform is partitioning an iamge into application meaningful regions, we aim to separate pixel represent objects from all other pixels representing other  stuff. 

We can process an image sequence and estiamte backgrounds even when objects are moving in the scene, we use a recursive algorithm to update the estimated background image, we do an estiamte recursively as follows with $\hat{B}$ beign the background estimator

$$
\hat{B}\langle k+1 \rangle \leftarrow \hat{B} \langle k \rangle + c(I\langle k\rangle - \hat{B}\langle k \rangle)
$$
with $k$ beign the time step and $c(\dots)$ the monadic image saturation function 

$$
\begin{cases}
\sigma, &x\gt \sigma\\
x, & -\sigma \le x\le \sigma\\
-\sigma, &x\lt -\sigma\\
\end{cases}
$$

this makes for moving objects to remain a blur while the static background remains still.

### local/spatial operators

in this case each pixel output is a function of all pixels in a region surrounding the corresponding pixel in the input image

$$
O[u,v]=f(I[u+i,v+j]) \text{    } \forall(i,j)\in W,\forall (u,v) \in I
$$
with $W$ being known as the window, usually a $w \times w$ square region with a odd side length $w=2h+1$ and $h \in \mathbb{Z}^{+}$ is the half width. all pixels are included in such window.

![[Pasted image 20250415225728.png]]

spatia loperator are of various kind, a linear spatial operator can be

$$
O[u,v]=\sum_{(i,j)\in W}I[u+i,v+j]K[i,j],\text{ } \forall (u,v) \in I 
$$
$K \in \mathbb{R}^{w \times w}$ is the kernel and all the elements are called filtert coefficents, we just multiply each element in K by the correponding element in the image for each K in the window. we consider the cnedter as the coordinate $(0,0)$ and $(i,j) \in  [-h,h] \subset Z \times Z$ .

we can written such operator as $O=K \otimes I$.

One issue is spatial operations when the window is close to the edge of the input image as shown in figure, basically we have to define what values should usch out of boundary pixels have:
- pick 0 so that they don't matter
- we just don't pick such points and get back a result of size $(W-2h)\times(H-2h)$ slithly smaller than the input.
![[Pasted image 20250415230415.png]]

#### smoothing
another important necessity is to smooth values a famous kernel to apply is the gaussian kernel.

$$
G(u,v)=\frac{1}{2\pi\sigma^2}e^{-\frac{u^2+v^2}{2\sigma^2}}
$$
we can control blurredness by the sigma value.

another simple idea is just to use an average  of the neighborhooud for the kernel
$$
g(x,y)=\frac{1}{P}\sum f(n,m)
$$
with $P$ number of pixels in the region.

another point is to take the median of a set of values in the neighborhood:
$$
g(x,y)=\text{median of }(x,y)
$$
we defiend as mean the value s.t. half values are below and half are above it.
very useful when you have salt and pepper issues and use it to remove noise.

we have at some point to make a decision between one or the other of course, the average based smooting creates new grey level  but all high spatial frequencies do as well and blurs the light/dark discontinuities, median based smootginf does not affect discontinuities but cancel peake with bases small enough w.r.t. the mask size.


The method is heavily reliant on scene illumination change or viewpoint change.



#### edge detection

Anothe interesting task to solve is edge detection i.e. find edges relatively to objects. and edge is a point where grey levels quickly vary(discontinuities) and edge operator is a operato detecting such edge.

![[Pasted image 20250416104045.png]]

here we can see better what we mean, it is better to look at the pixel values in a 1 dimensional profile thorugh the image. 

an informative gain we can have is to look at the imagethoriugh the eyes of a 1D profile though the image, for $v$=360 in the image above we can see that transpsing the grey value on a chart reveals some kind of strucutre, loook at the figure b in the image , in c we just havea closeup of the spike between  560 and 610 and finally image d shows a discrete derivative of such point showing where tinhs go up and then down signal a point.
the derivative is calculated as 
$$
p'[v]=p[v]-p[v-1]
$$

we can use different ways of doing so e.g.
$$
p'[v]=\frac{1}{2}(p[v+1]-p[v-1])
$$
its asymmetrical derivativein this case and is equivalent to a convolution with a 1D kernel, such convolution causes the following transformation

![[Pasted image 20250416105017.png]]

in figure a we see that edges are clearly distinguished by very negative or very positive values for the vertical aplication, of we do so horizontally we have what happens in figure b , such two transofrmations can be used to obtained figure d by threating the values as direction/vecors and we can use figure their magnitude and observe strucutre in figure c.

the previous kernel is obtained as 
$$
K=(+\frac{1}{2}\text{ }0\text{ }-\frac{1}{2})
$$
the notation for such value is
$$
I_u=D*I
$$
$$
I_v=D^T*I
$$
with D being a derivative kernel, there are many kernels possible one is $K$ from before but it is not the only possibility.

one interesting applications is template matching where we consider the kernel as an image and we widh to find an image that is very similar to that template.

$$
O[u,v]=s(T,W) \text{ } \forall(u,v) \in I
$$
with $T$ and $w \times w$ with the usual $w=2h+1$ length nad $W$ is centered at $(u,v)$. the function $s(I_1,I_2)$ is a sclaar measure describing similarity of two equal sized images $I_1$ and $I_2$.

### image feature extraction

image segmentation is considered as three subproblems:
- classification: we cllassify each pixel to be in one of $C$ classes $c \in \{0,\dots,C-1\}$, when $C=2$ we have a binary classification. always application specific
- representation: we have adjacent pixel in the same class connect to form sptial assets $S_1,\dots,S_m$ either repsented by a label for each pixel or a list for eahc class contraingt the pixel coordinates defining a boundary on the conected set,
- all $S_i$ are described in terms of compact scalr of vecrtor valued featyres like size,position and shape.

We start with the classification issue, in particualr we will studi binary classification.

the easiest part is applyinga monadic operator
$$
c[u,v]\begin{cases}
0, if\text{ } I[u,v]< t\\
1, if\text{ } I[u,v]\ge t
\end{cases}
\text{ }
\forall(u,v) \in I
$$
the decision is taken on the value of the pixel $I$ , usually called thesholding witth $t$ being the threshold. it has to be tried with trial and error.

we also analyze the histogram of the image and see that  by pickng values between peaks we can have better discrimination with bimodal distributions.

![[Pasted image 20250416110618.png]]

we now want to form our sptial sets $S_1,\dots,S_m$ bu connecting adjacent pixels of the smae class. a blob is a pstaily contiguous region of pixels of the same class connected to each other.

![[Pasted image 20250416111155.png]]

in this case we have a binary image,we can label each blob with a class and connecte them returing a label matrix wth each element $s \in \{1,\dots,m\}$ corresponding to the input pixel and we can see ther result of giving each one a color.

We can use a graph based impleemnteation and see everything  as a avertex with 8 edges connecting to its nieghtborhood pixel. each edge is weight as the non negative measure of dissimilaritybetween two pixels i.e. the abosulute difference ino color, the algorithm starts as each vertex in its own set at each iteration the edge weights are examined and if the different set have and edge at weight below the thresols we merge them. The threshold is a funtion of the size and set of a globla parameter k which sets the sacle for the segmentation al rger value leads to prefrecne for large valued connected components.

We can do segmentation also by global operators, two elements to do so:
- boundaries: represent a discontinuity
- regions:elements of asegmented image based on homogeneity

we can follow the edges to trace such boundaries:
1. scan image left to right and top to bottom until you get an edge
2. link the edge with the previous one and search for other edges in its neighbourhood
3. if there is 1 edge got to 2 , if 2 are present pick one and store the others if no neightbors are present it is a boundary end , if others are stored pick one a and go to 2 again or go to 1.

## ROS

ROS is a meta operating system and works as a middleware, it gives:
- distributed processing
- package management
- public repository and API
- suport to different languages like python,C++,java and matlab
We have a ROS master acting as a name serve fora nod to node connection and manages communication between nodes, each node registers at startup with the master and communication without the master is impossible.
 A node is the smallest unit of processor running in ROS, usually one node for purpose is recommended. the communication happens with XMLRPC communicating with the master and uses XMLRPC/TCPROS of TCP/IP when communicating between nodes.

nodes are organized in packages that are individually compiled ,executed and managed and communicate like we have said before.

a package is the basic unit of ROS, a ROS application is developed on a package basis and the package contains either configuration file to launch other packages or nodes. Each package contains the file snecesary to run it including ROS dependency libraries for running processes,dataseta and configuration files.

a node sends or receives data between nodes via a message and mesages are variable like integer,floating point and boolean. a nested message structure ccontains another messages or an array of messages that can be used in the message.

message delivery happens using TCPROS and UDPROS and topic is used as unidirectiona message delivery while service is used in bidirectional message delivery where request and response are involved.

We use publish-subscribe approach , apublisher publishes information in atopic with the master which send a mossage to the subscriber nodes interested inthe same topic.

analagously the term subscriber reive message to from the topic it is subscribed to and register its iinformation to a topic with the master and recevies information when it is pulbished in the topic.

this is an asynchronous communication method based on these two actoers, usually sensor data is sent.

