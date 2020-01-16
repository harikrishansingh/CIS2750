#include "SVGParser.h"

/*Public API*/

SVGimage* createSVGimage(char* fileName);

char* SVGimageToString(SVGimage* img);

void deleteSVGimage(SVGimage* img);

// Function that returns a list of all rectangles in the image.  
List* getRects(SVGimage* img);

// Function that returns a list of all circles in the image.  
List* getCircles(SVGimage* img);

// Function that returns a list of all groups in the image.  
List* getGroups(SVGimage* img);

// Function that returns a list of all paths in the image.  
List* getPaths(SVGimage* img);

// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(SVGimage* img, float area);

// Function that returns the number of all circles with the specified area
int numCirclesWithArea(SVGimage* img, float area);

// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(SVGimage* img, char* data);

// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(SVGimage* img, int len);

/*Helper Functions*/

int numAttr(SVGimage* img);

void deleteAttribute( void* data);

char* attributeToString( void* data);

int compareAttributes(const void *first, const void *second);

void deleteGroup(void* data);

char* groupToString( void* data);

int compareGroups(const void *first, const void *second);

void deleteRectangle(void* data);

char* rectangleToString(void* data);

int compareRectangles(const void *first, const void *second);

void deleteCircle(void* data);

char* circleToString(void* data);

int compareCircles(const void *first, const void *second);

void deletePath(void* data);

char* pathToString(void* data);

int comparePaths(const void *first, const void *second);
