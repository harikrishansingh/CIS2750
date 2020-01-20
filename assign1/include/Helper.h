#ifndef _HELPER_
#define _HELPER_
#define PI 3.1415926535

#include "LinkedListAPI.h"
#include <ctype.h>

//TODO: Make one general function instead of all four
//Perform depth-first searches through the XML nodes to find each element
List* populateRects (xmlNode *, List *);
List* populateCircles (xmlNode *, List *);
List* populatePaths (xmlNode *, List *);
List* populateGroups (xmlNode *, List *);
List* populateAttr (xmlNode *, List *);
float split (char*, char*);

#endif //_HELPER_
