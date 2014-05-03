#include <iostream>
#include <stack>
#include "R2/R2.h"
#include "R3/R3.h"
#include "turtle.h"
#include <string>
#include <map>
#include "lsystem.h"
using namespace std;
class LPlusSystem: public LSystem
{
public:
	bool isPlus;
	LPlusSystem(R3Mesh *m);
	string generateFromFile(const char * filename,const int iterationsOverride );
	virtual void run(const char command,const float param);
};