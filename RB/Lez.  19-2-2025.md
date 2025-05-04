# Robotics mechanics and kinematics

Proprioception is the capability of getting feedback on our body, we have sensors that give information about the current state and position and body. 

A robot is an autonomous system which exists in he physcial world and can sene its environment.  
Two types of robots:
- A robot is autonomous and can act on it's own decisions and is not controlled by a human. 
- A robot is non-autonomous when it is controlled by an operator, it is semi-autonomous when it needs some input from the user and control is controlled by the user itself which is still different from tele-operating from the distance the robot.
A robot can exist in the physical world, which means it is subject to the laws of physics that are different from simulations, in the physical world interactions and physical laws are simulated and somehow approximated. 
A robot has sensors which are used to perceive information from the world, i.e. it SENSES it's world. 
A robot can also ACT on it's world and uses effectors and actuators to achieve what is desired.
and is also intelligent meaning it has some reasoning capability. 

The components of he robot are then:
- Physical body
- sensors
- effectors/actuators
- controller

![[Pasted image 20250219094727.png]]
The effectors themselves are of various types like in the image below

![[Pasted image 20250219094852.png]]

The first definition we need is a *degree of freedom*
Definition: It is the configuration of a robot is complete specification of the position of every point of he robot in a mathematical sense it is the number of real value coordinates/parameters used to represent the configuration of the robot.

![[Pasted image 20250219095324.png]]
here we have some examples that combine springs, pendulums and linear motions.

![[Pasted image 20250219095440.png]]
Other real life examples in this case we define coordinates in the world not motion.

For rigid bodies we have bot movement and orientation, see the helicopter below for an example.

![[Pasted image 20250219095606.png]]
Now we have objects in space, we don't have full control on all the degree of freedom on the space. an eamples is below since the car cannot rotate along Z or move along Z on it's own, a helicopt can and has full control on the degrees of freedom.

![[Pasted image 20250219095716.png]]

We have a more refined definition of this part:
- holonomic: a rigid body is when the total number of controllable DOF is equal to the DOFs of the robot.
- non-holonomic: is when the controllable DOFs are less the total DOFs.
- redundant is when the robot has more controllable DOF than it can control.
## Joints and DOFs
Joints are set of two surfaces that can slide keeping contact one to the another, the link between the two it the DOF of the robot, links are enumerated and link 0 is the support base and origin of the reference coordinates frame for robot motion.
![[Pasted image 20250219100701.png]]
bserve that a link can have more degrees of freedom, it can revolute (R), slide (P), it con rotate (H), it can be cylindrical(C), universal (U) and spherical (S).

The most commonly used are the revolutional and the primastic ones.

![[Pasted image 20250219102405.png]]

a robot manipulator is nothing more then a link of revolutonal and prismatic robots.
It is a open kinematic chain, the sequence of rigid elements or links connected with revolute or translational oints actuated by a motor.

we have various kinds depending on their type, see the image below

![[Pasted image 20250219102947.png]]

The joint space is the space in which the $q$ vector of joint variables are defined its dimension is indicated with $N$, and it contains an element for each joint defining it's rotation.

$x=(p,\Phi)^T$ is a value in the cartesian space which  defies the end-effector position an it's dimension is indicated with $M$. And $p$ is the cartesian coordinates while $\Phi$ is the orientation of the end effector.
for $p$ you have $(x,y,z)$ coordinates while for $\Phi$ you have $(roll,pitch,yaw)$,thus $x=(x,y,z,roll,pitch,yaw)$. 
The *robot workspace* is the region described by the origin and the end effector is when the robot joints execute all possible motions.

In robotics it is essential to be able to reconstruct the end position and angulation of a certain movement on a certain robot, o do so we need a *reference frame* thta allows us to build the kinematic chain needed.
![[Pasted image 20250219104428.png]]
differented reference frames , represented as $\{a\}$ in the image above, give a reference to reconstruct the position $p$, whatever reerence we use is ok we just need to have stationary position.
![[Pasted image 20250219104618.png]]
such reference frame is stationary,$\{s\}$, and can be used to describe the distance from other reference frames that are used to describe the distance from other non-stationary reernce frames, $\{b\}$ , that are attached to a moving rigid body. we don't need stuff like the center of mass.

We can derive the body-frame origin $p$ and we can express it as 
$$
p=p_x\hat{x}_s + p_y\hat{y}_s
$$
