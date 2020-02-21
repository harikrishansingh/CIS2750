/* Name: Nicholas Rosati
 * Student ID: 1037025
 * Email: nrosati@uoguelph.ca*/
#include <libxml/tree.h>
#include "LinkedListAPI.h"
#include "SVGParser.h"

#ifndef _HELPER_
#define _HELPER_
#define PI 3.1415926535

//TODO: Condense ALL of the add* functions into one variadic function
void addRectangle (xmlNode* node, List* list);
void addCircle (xmlNode* node, List* list);
void addPath (xmlNode* node, List* list);
void addGroup (xmlNode* node, List* list);
void getGroupsHelper (List* masterList, Group* groupRoot);
Attribute* makeAttribute(xmlAttr* attrNode);
void dummy();
xmlDoc* imageToXML(SVGimage* image);
bool validateRects (List* list);
bool validateCircles (List* list);
bool validatePaths (List* list);
bool validateGroups (List* list);
bool validateAttributes (List* list);
bool fileExists (char* fileName);
int validateXMLwithXSD(xmlDoc* xml, char* xsdFile);
void addAttributesToXML(List* elementList, xmlNode* node);
void addRectsToXML(List* elementList, xmlNode* docHead);
void addCirclesToXML(List* elementList, xmlNode* docHead);
void addPathsToXML(List* elementList, xmlNode* docHead);
void addGroupsToXML(List* elementList, xmlNode* docHead);
Attribute* existsInList(List* list, Attribute* attribute);

#endif
