#ifndef _HELPER_
#define _HELPER_
#define PI 3.1415926535

#include "LinkedListAPI.h"
#include <ctype.h>

//TODO: Make one general function instead of all four
//Perform depth-first searches through the XML nodes to find each element
void addRectangle (xmlNode* node, List* list);
void addCircle (xmlNode* node, List* list);
void addPath (xmlNode* node, List* list);
void addGroup (xmlNode* node, List* list);
void addAttribute (xmlNode* node, List* list);

#endif //_HELPER_
