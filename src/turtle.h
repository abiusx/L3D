#ifndef TURTLE_H
#define TURTLE_H
#include <stack>
#include "R2/R2.h"
#include "R3/R3.h"
#include "R3Mesh.h"
using namespace std;
struct Turtle 
{
  R3Vector position;
  R3Vector direction;
  R3Vector right;
  Turtle();
  void turnRight(float angle);
  void turnLeft(float angle);
  void pitchDown(float angle);
  void pitchUp(float angle);
  void rollLeft(float angle);
  void rollRight(float angle);
  void move(float distance);
  void turn180(float temp);
};
class TurtleSystem: public Turtle 
{
  stack<Turtle> state;
  R3Mesh *mesh;
public:
  float thickness;
  TurtleSystem(R3Mesh * m);
  void save();
  void restore();
  void draw(float param);
};

#endif