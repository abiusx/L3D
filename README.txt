The code uses an L-system to generate a tree or bush from a descriptions file (.l or .l3d). The generated tree or bush is a single mesh.

Two classes are specific to this task: Turtle and LSystem
The first one implements a turtle graphics system, which is a graphics model where you have a turtle, you can move it forward, rotate, turn, pitch, roll, and make it draw while moving forward. It can also save its state, and return to that state. This is used to implement trees, where you save, turn right and draw a branch, save, turn right and draw another. Return and turn left this time instead, and so on.

The second implements an L-System, which is essentially a set of rules generating a huge string of orders for the turtle, to draw a tree. L-System generates those strings from .l or .l3d files (same format) and then feeds them to turtle to consume.
A sample L file is as follows:

#this is a comment line
3
#3 means that L-System has to populate the axiom three times by the rules. Essentially it is the exponent of the growth, so be careful going above 6
12	
#the default rotate, pitch, turn angle in degrees. Can be ovverridden by (angle) in the rules
5
#the thickness of branches based on their length (percentage). 5 is very narrow, suitable for weeds and narrow trees. 20 is for rigid trees.
F
#the axiom, a tree starts with this
F=F[-&<F][<++&F]|F[-&>F][+&F]
#set of rules. we have only one here. check L folder for more
@
#end of L file

There is a Cylinder function in the R3Mesh class, which creates a Cylinder and returns its vertices. The cylinder is towards Y axis, with radius and length of 1. You can provide a ratio to this, defining how big should the top of cylinder be compared to its bottom. A default ratio of .8 is used for trees and bushes (branches get narrower as they progress).




The code relies on the C++ 3D library created by people at princeton (Connelly Barnes gave it to me).
