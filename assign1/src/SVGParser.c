#include "SVGParser.h"
#include "Helper.h"
#include <malloc.h>

//Public API

SVGimage* createSVGimage(char* fileName){
    SVGimage *image = calloc(1, sizeof(SVGimage));

    xmlDoc *document = xmlReadFile(fileName, NULL, 0);
    //Return NULL if the parsing failed
    if (document == NULL) return NULL;

    xmlNode *rootNode = xmlDocGetRootElement(document);

    //Get namespace
    strcpy(image->namespace, (char *)rootNode->nsDef->href);

    //Get title, if it exists
    xmlNode *titleNode = rootNode->children;
    do {
        titleNode = titleNode->next;
    } while (titleNode != NULL && strcmp("title", (char *)titleNode->name) != 0);
    strcpy(image->title,titleNode == NULL ? "" : (char *)titleNode->children->content);

    //Get description
    xmlNode *descNode = rootNode->children;
    do {
        descNode = descNode->next;
    } while (descNode != NULL && strcmp("desc", (char *)descNode->name) != 0);
    strcpy(image->description,descNode == NULL ? "" : (char *)descNode->children->content);
    //TODO: Generalize title and description functions

    image->rectangles = populateRects(rootNode);
    image->circles = populateCircles(rootNode);
    image->paths = populatePaths(rootNode);
    image->groups = populateGroups(rootNode);
    image->otherAttributes = populateAttr(rootNode);

    xmlFreeDoc(document);
    return image;
}

char* SVGimageToString(SVGimage* img){
    char *desc = calloc(1024, sizeof(char));

    strcat(desc, "Namespace: ");
    strcat(desc, img->namespace);
    strcat(desc, "\n");
    strcat(desc, "Title: ");
    strcat(desc, img->title);
    strcat(desc, "\n");
    strcat(desc, "Description: ");
    strcat(desc, img->description);
    strcat(desc, "\n");

    //TODO: Add descriptions for the lists

    return desc;
}

void deleteSVGimage(SVGimage* img){
    if (img == NULL) return;

    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->paths);
    freeList(img->groups);
    freeList(img->otherAttributes);
    free(img);
}

List* getRects(SVGimage* img){
    return img->rectangles;
}

List* getCircles(SVGimage* img){
    return img->circles;
}

List* getGroups(SVGimage* img){
    return img->groups;
}

List* getPaths(SVGimage* img){
    return img->paths;
}

int numRectsWithArea(SVGimage* img, float area){

    return 0;
}

int numCirclesWithArea(SVGimage* img, float area){

    return 0;
}

int numPathsWithdata(SVGimage* img, char* data){

    return 0;
}

int numGroupsWithLen(SVGimage* img, int len){

    return 0;
}

//Helper Functions

int numAttr(SVGimage* img){
    //TODO: Loop through rects, circles, paths, groups, attribute lists for this (INCLUDES SVG NODE ATTRIBUTES (NOT including the description, title, or namespace))
    return 0;
}

void deleteAttribute( void* data){

}

char* attributeToString( void* data){

    return NULL;
}

//Unused
int compareAttributes(const void *first, const void *second){
    return 0;
}

void deleteGroup(void* data){

}

char* groupToString( void* data){

    return NULL;
}

//Unused
int compareGroups(const void *first, const void *second){
    return 0;
}

void deleteRectangle(void* data){

}

char* rectangleToString(void* data){

    return NULL;
}

//Unused
int compareRectangles(const void *first, const void *second){
    return 0;
}

void deleteCircle(void* data){

}

char* circleToString(void* data){

    return NULL;
}

//Unused
int compareCircles(const void *first, const void *second){
    return 0;
}

void deletePath(void* data){

}

char* pathToString(void* data){

    return NULL;
}

//Unused
int comparePaths(const void *first, const void *second){
    return 0;
}

List* populateRects (xmlNode* rootNode){
    List* listHead = initializeList(rectangleToString, deleteRectangle, compareRectangles);

    return listHead;
}

List* populateCircles (xmlNode* rootNode){
    List* listHead = initializeList(circleToString, deleteCircle, compareCircles);

    return listHead;
}

List* populatePaths (xmlNode* rootNode){
    List* listHead = initializeList(pathToString, deletePath, comparePaths);

    return listHead;
}

List* populateGroups (xmlNode* rootNode){
    List* listHead = initializeList(groupToString, deleteGroup, compareGroups);

    return listHead;
}

List* populateAttr (xmlNode* rootNode){
    List* listHead = initializeList(attributeToString, deleteAttribute, compareAttributes);

    return listHead;
}
