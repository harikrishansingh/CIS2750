#include "SVGParser.h"
#include <malloc.h>

/*Public API*/

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

    image->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    image->circles = initializeList(circleToString, deleteCircle, compareCircles);
    image->paths = initializeList(pathToString, deletePath, comparePaths);
    image->groups = initializeList(groupToString, deleteGroup, compareGroups);
    image->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

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

// Function that returns a list of all rectangles in the image.  
List* getRects(SVGimage* img){
    return img->rectangles;
}

// Function that returns a list of all circles in the image.  
List* getCircles(SVGimage* img){
    return img->circles;
}

// Function that returns a list of all groups in the image.  
List* getGroups(SVGimage* img){
    return img->groups;
}

// Function that returns a list of all paths in the image.  
List* getPaths(SVGimage* img){
    return img->paths;
}

// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(SVGimage* img, float area){

    return 0;
}

// Function that returns the number of all circles with the specified area
int numCirclesWithArea(SVGimage* img, float area){

    return 0;
}

// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(SVGimage* img, char* data){

    return 0;
}

// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(SVGimage* img, int len){

    return 0;
}

/*Helper Functions*/

int numAttr(SVGimage* img){

    return 0;
}

void deleteAttribute( void* data){

}

char* attributeToString( void* data){

    return (char *)'c';
}

int compareAttributes(const void *first, const void *second){

    return 0;
}

void deleteGroup(void* data){

}

char* groupToString( void* data){

    return (char *)'c';
}

int compareGroups(const void *first, const void *second){

    return 0;
}

void deleteRectangle(void* data){

}

char* rectangleToString(void* data){

    return (char *)'c';
}

int compareRectangles(const void *first, const void *second){

    return 0;
}

void deleteCircle(void* data){

}

char* circleToString(void* data){

    return (char *)'c';
}

int compareCircles(const void *first, const void *second){

    return 0;
}

void deletePath(void* data){

}

char* pathToString(void* data){

    return (char *) 'c';
}

int comparePaths(const void *first, const void *second){

    return 0;
}
