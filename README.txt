This is an extended 3D L-System mesh implementation that can generate trees and bushes very easily and rapidly.

It is written in C++ using OpenGL and glut, and is cross-platform. Everything is implemented as simply as possible, to allow modification to achieve any desired outcome.

The library supports L files (plain L-System 2D trees), L3D files (3D L-System trees) and L++ files (extended 3D L-System trees).

Two classes are specific to generation of L-System trees: Turtle and LSystem

Turtle implements a turtle graphics system, which is a graphics model where you have a turtle, you can move it forward, rotate, turn, pitch, roll, and make it draw while moving forward. It can also save its state, and return to that state. This is used to implement trees, where you save, turn right and draw a branch, restore, turn left and draw another. It allows for drawing fractal-based shapes and objects.

LSystem class implements an L-System, which is a set of rules generating a huge string of steps for the turtle, to draw a tree. L-System generates those strings from .l or .l3d or .l++ files and then feeds them to turtle to consume. The extended LSystem which is called LPlusSystem has a better parser which features many more commands and stockastic control over the rules.

A sample L (or L3D, they are the same) file is as follows:
#this is a comment line
3 #the number of recursions for generating rules from the axiom
12 #default number for rotations, turns, and etc.
5 #thickness of branches compared to their length in percent
F #the axiom, a tree starts with this
F=F[-&<F][<++&F]|F[-&>F][+&F] #a rule, this is recursively applied to the axiom to generate the tree string
# we can have multiple rules, all of them are evaluated in each recursion iteration
@ #end of L file


The R3Mesh class is responsible for generating this tree using LSystem and Turtle, and saving it to an Off or Off+ file. There is a Cylinder function in the R3Mesh class, which creates a Cylinder and returns its vertices. The cylinder is towards Y axis, with radius and length of 1. This cylinder is the building block of all trees. There is also a Leaf function which generates a leaf, for L++ trees.


L and L3D files typically work with an OFF file for input/output. Off file is a textual format containing vertices and faces. On the other hand, L++ files work on Off+ files, which on top of everything that Off has, contain texture coordinates for vertices, and a flag telling if a face is a leaf or not.

Instructions for L (or L3D) files are as follows (for generation of rules):
+ = turn right
- = turn left
& = pitch down
^ = pitch up
< or \ = roll left
> or / = roll right
| = turn 180 degree
f or F = draw branch (and go forward)
g = go forward
[ = save state
] = restore state

Any of those that require a number, work on the default number by default. You can provide another number by providing it in paranthesis, for example:

+(90)&(75.5)

The L++ format is slightly different, in the following ways:
> and < are not rolling
> : decrease thickness (by percent of its own)
< : increase thickness (by percent of its own)
= : set thickness (to percentage of length)
* : draw leaf
% : set thickness reduction (for every draw operation)

The major component of L++ is that it supports leafs, so it needs to save its output to OFF+ files to properly reflect leaf textures. Leaves are also affected by gravity, and their tip bends towards ground.

To run the system and generate a tree, simply do the following:

./run L++/tree.l++





The code relies on the C++ 3D code-set created by people at Princeton University (Connelly Barnes gave it to me).
