#ifndef _HELPER_
#define _HELPER_
#define PI 3.1415926535

#include "LinkedListAPI.h"
#include <ctype.h>

void addRectangle (xmlNode* node, List* list);
void addCircle (xmlNode* node, List* list);
void addPath (xmlNode* node, List* list);
void addGroup (xmlNode* node, List* list);
Attribute* makeAttribute(xmlAttr* attrNode);

#endif //_HELPER_
