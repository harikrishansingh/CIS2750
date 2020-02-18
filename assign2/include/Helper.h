/* Name: Nicholas Rosati
 * Student ID: 1037025
 * Email: nrosati@uoguelph.ca*/

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
void addRectsToXML(Node* elementNode, xmlNode* docHead);
void addCirclesToXML(Node* elementNode, xmlNode* docHead);
void addPathsToXML(Node* elementNode, xmlNode* docHead);
void addGroupsToXML(Node* elementNode, xmlNode* docHead);

#endif
