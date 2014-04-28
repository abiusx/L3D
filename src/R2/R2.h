// Include files for R2 package
#ifndef R2_INCLUDED
#define R2_INCLUDED


// Standard dependency include files 

#if defined(_MSC_VER)
# pragma warning(disable:4996)
#endif
#ifdef _WIN32
# define NOMINMAX
# include <windows.h>
#endif
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cmath>
#include <climits>
#include <algorithm>
using namespace std;



// Class declarations

class R2Point;
class R2Vector;
class R2Line;
class R2Segment;



// Geometry include files

#include "R2Point.h"
#include "R2Vector.h"
#include "R2Line.h"
#include "R2Segment.h"

// Image include files

#include "R2Pixel.h"
#include "R2Image.h"


// Utility include files

#include "R2Distance.h"



#endif
