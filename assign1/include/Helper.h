#ifndef _HELPER_
#define _HELPER_
#define PI 3.1415926535

#include "LinkedListAPI.h"

//TODO: Make one general function instead of all four
//Perform depth-first searches through the XML nodes to find each element
List* populateRects (xmlNode *);
List* populateCircles (xmlNode *);
List* populatePaths (xmlNode *);
List* populateGroups (xmlNode *);
List* populateAttr (xmlNode *);

#endif //_HELPER_
