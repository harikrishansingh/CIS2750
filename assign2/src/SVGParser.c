/* Name: Nicholas Rosati
 * Student ID: 1037025
 * Email: nrosati@uoguelph.ca*/

#include "SVGParser.h"
#include "Helper.h"
#include <math.h>

/**
 * Creates an SVGimage from a SVG file.
 * @post A SVGimage struct is created and returned if the given file was valid XML. NULL otherwise.
 * @param fileName A path to a svg file.
 * @return A fully populated SVGimage struct.
 */
SVGimage* createSVGimage(char* fileName) {
    xmlDoc* document = xmlReadFile(fileName, NULL, 0);
    //Return NULL if the parsing failed
    if (document == NULL) {
        xmlCleanupParser();
        return NULL;
    }

    xmlNode* rootNode = xmlDocGetRootElement(document);
    SVGimage* image = calloc(1, sizeof(SVGimage));

    //Use strncpy to leave the null terminator
    strncpy(image->namespace, (char*)rootNode->ns->href, 255);

    image->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    image->circles = initializeList(circleToString, deleteCircle, compareCircles);
    image->paths = initializeList(pathToString, deletePath, comparePaths);
    image->groups = initializeList(groupToString, deleteGroup, compareGroups);
    image->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    for (xmlNode* currNode = rootNode->children; currNode != NULL; currNode = currNode->next) {
        if (strcmp((char*)currNode->name, "rect") == 0) {
            addRectangle(currNode, image->rectangles);
        } else if (strcmp((char*)currNode->name, "circle") == 0) {
            addCircle(currNode, image->circles);
        } else if (strcmp((char*)currNode->name, "path") == 0) {
            addPath(currNode, image->paths);
        } else if (strcmp((char*)currNode->name, "g") == 0) {
            addGroup(currNode, image->groups);
        } else if (strcmp((char*)currNode->name, "title") == 0) {
            //Use strncpy to leave the null terminator
            strncpy(image->title, (char*)currNode->children->content, 255);
        } else if (strcmp((char*)currNode->name, "desc") == 0) {
            //Use strncpy to leave the null terminator
            strncpy(image->description, (char*)currNode->children->content, 255);
        }
    }

    for (xmlAttr* attrNode = rootNode->properties; attrNode != NULL; attrNode = attrNode->next) {
        insertBack(image->otherAttributes, makeAttribute(attrNode));
    }

    xmlFreeDoc(document);
    xmlCleanupParser();
    return image;
}

/**
 * Creates a string representation of a SVGimage using other *toString functions.
 * @pre im should not be NULL.
 * @post A string describing the image is created.
 * @param img A pointer to a SVGimage struct.
 * @return A string describing the image.
 */
char* SVGimageToString(SVGimage* img) {
    char* desc = calloc(strlen(img->namespace) + strlen(img->title) + strlen(img->description) + 512,
                        sizeof(char)); //Extra bytes for \0's and extra words in the next line
    sprintf(desc, "[BEGIN SVG]\n[NAMESPACE]\n%s\n[TITLE]\n%s\n[DESCRIPTION]\n%s", img->namespace, img->title,
            img->description);

    char* listDesc = NULL;
    //Get description of the root node attributes (the svg node)
    if (img->otherAttributes->length > 0) {
        listDesc = toString(img->otherAttributes);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    //Get description for the rectangles in the base image
    if (img->rectangles->length > 0) {
        listDesc = toString(img->rectangles);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    //Get description for the circles in the base image
    if (img->circles->length > 0) {
        listDesc = toString(img->circles);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    //Get description for the paths in the base image
    if (img->paths->length > 0) {
        listDesc = toString(img->paths);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    //Get description for the groups. This is recursive to groups in groups.
    if (img->groups->length > 0) {
        listDesc = toString(img->groups);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    desc = realloc(desc, strlen(desc) + 16);
    strcat(desc, "[END SVG]\n");
    return desc;
}

/**
 * Frees an entire SVG image.
 * @pre img can be NULL or point to a populated SVGimage struct.
 * @post img and all its content is freed.
 * @param img A pointer to a SVGimage.
 */
void deleteSVGimage(SVGimage* img) {
    if (img == NULL) return;
    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->paths);
    freeList(img->groups);
    freeList(img->otherAttributes);
    free(img);
}

/**
 * Creates a list of ALL the circles in the image.
 * @pre img can be NULL or point to a SVGimage struct.
 * @post A list containing 0 or more Nodes of all the circles in the image.
 * @param img pointer to a SVGimage struct.
 * @return A list containing all of the circles in the image.
 */
List* getRects(SVGimage* img) {
    if (img == NULL) return NULL;

    List* allRectangles = initializeList(rectangleToString, dummy, compareRectangles);

    //Start with the circles in the img
    for (Node* node = img->rectangles->head; node != NULL; node = node->next) {
        insertBack(allRectangles, node->data);
    }

    //Go through the groups and add their rectangles
    List* groups = getGroups(img);
    for (Node* node = groups->head; node != NULL; node = node->next) {
        if (((Group*)(node->data))->rectangles->length > 0) {
            for (Node* rectangleNode = ((Group*)(node->data))->rectangles->head; rectangleNode !=
                                                                                 NULL; rectangleNode = rectangleNode->next) {
                insertBack(allRectangles, rectangleNode->data);
            }
        }
    }
    freeList(groups);
    return allRectangles;
}

/**
 * Creates a list of ALL the circles in the image.
 * @pre img can be NULL or point to a SVGimage struct.
 * @post A list containing 0 or more Nodes of all the circles in the image.
 * @param img pointer to a SVGimage struct.
 * @return A list containing all of the circles in the image.
 */
List* getCircles(SVGimage* img) {
    if (img == NULL) return NULL;

    List* allCircles = initializeList(circleToString, dummy, compareCircles);

    //Start with the circles in the img
    for (Node* node = img->circles->head; node != NULL; node = node->next) {
        insertBack(allCircles, node->data);
    }

    //Go through the groups and add their circles
    List* groups = getGroups(img);
    for (Node* node = groups->head; node != NULL; node = node->next) {
        if (((Group*)(node->data))->circles->length > 0) {
            for (Node* circleNode = ((Group*)(node->data))->circles->head; circleNode !=
                                                                           NULL; circleNode = circleNode->next) {
                insertBack(allCircles, circleNode->data);
            }
        }
    }
    freeList(groups);
    return allCircles;
}

/**
 * Creates a list of ALL the groups in the image. This function calls a recursive function to get groups that are in groups.
 * @pre img can be NULL or point to a SVGimage struct.
 * @post A list containing 0 or more Nodes of all the Groups in the image.
 * @param img A SVGimage struct.
 * @return A list containing all of the Groups in the image. If none, Will return an empty list.
 */
List* getGroups(SVGimage* img) {
    if (img == NULL) return NULL;

    List* allGroups = initializeList(groupToString, dummy, compareGroups);
    //Starts with the groups in the img
    ListIterator iterator = createIterator(img->groups);
    Group* g = nextElement(&iterator);
    while (g != NULL) {
        getGroupsHelper(allGroups, g);
        insertBack(allGroups, g);
        g = nextElement(&iterator);
    }
    return allGroups;
}

/**
 * Helper to the above getGroups function. This function is recursive to get groups in groups [...].
 * @param masterList Main Groups list to add more groups to.
 * @param groupRoot Node in a list to act as a root node. Other groups nodes will become a groupNode when called recursively.
 */
void getGroupsHelper(List* masterList, Group* groupRoot) {
    ListIterator iterator = createIterator(groupRoot->groups);
    Group* g = nextElement(&iterator);
    while (g != NULL) {
        getGroupsHelper(masterList, g);
        insertBack(masterList, g);
        g = nextElement(&iterator);
    }
}

/**
 * Creates a list of ALL the paths in the image.
 * @pre img can be NULL or point to a SVGimage struct.
 * @post A list containing 0 or more Nodes of all the Paths in the image.
 * @param img A SVGimage struct.
 * @return A list containing all of the Paths in the image. If none, Will return an empty list.
 */
List* getPaths(SVGimage* img) {
    if (img == NULL) return NULL;

    List* allPaths = initializeList(pathToString, dummy, comparePaths);

    //Start by adding paths in the img
    for (Node* node = img->paths->head; node != NULL; node = node->next) {
        insertBack(allPaths, node->data);
    }

    //Go through the groups and add their paths
    List* groups = getGroups(img);
    for (Node* node = groups->head; node != NULL; node = node->next) {
        if (((Group*)(node->data))->paths->length > 0) {
            for (Node* pathNode = ((Group*)(node->data))->paths->head; pathNode != NULL; pathNode = pathNode->next) {
                insertBack(allPaths, pathNode->data);
            }
        }
    }
    freeList(groups);
    return allPaths;
}

/**
 * Find the number of rectangles that have the same (rounded) area as given.
 * @pre img can be NULL or point to a SVGimage struct. area is an area to compare with.
 * @post Number of rectangles that have the same (rounded) given area is calculated.
 * @param img A pointer to a SVGimage struct.
 * @param area Float value of an area to compare with.
 * @return The number of rectangles that have the same (rounded) area as the given value.
 */
int numRectsWithArea(SVGimage* img, float area) {
    if (img == NULL) return 0;

    int count = 0;
    List* allRectangles = getRects(img);
    for (Node* currentRect = allRectangles->head; currentRect != NULL; currentRect = currentRect->next) {
        if (ceilf(((Rectangle*)currentRect->data)->width * ((Rectangle*)currentRect->data)->height) == ceilf(area)) {
            count++;
        }
    }
    freeList(allRectangles);
    return count;
}

/**
 * Find the number of circles that have the same (rounded) area as given.
 * @pre img can be NULL or point to a SVGimage struct. area is an area to compare with.
 * @post Number of circles that have the same (rounded) given area is calculated.
 * @param img A pointer to a SVGimage struct.
 * @param area Float value of an area to compare with.
 * @return The number of circles that have the same (rounded) area as the given value.
 */
int numCirclesWithArea(SVGimage* img, float area) {
    if (img == NULL) return 0;

    int count = 0;
    List* allCircles = getCircles(img);

    //This feels gross but its the only way I could get it to work
    //Gets the ceiling of the given area and area of a circle, then rounded to ints because floats are weird.
    int areaRound = ceilf(area);
    for (Node* currentCircle = allCircles->head; currentCircle != NULL; currentCircle = currentCircle->next) {
        int radSquareRound = ceilf(((Circle*)currentCircle->data)->r * ((Circle*)currentCircle->data)->r * PI);
        if (radSquareRound == areaRound) count++;
    }
    freeList(allCircles);
    return count;
}

/**
 * Finds the number of paths that match the given data string.
 * @pre img can be NULL or point to a SVGimage struct . data is a string of data to compare with.
 * @post Number of paths with the given data is calculated.
 * @param img A pointer to a SVGimage struct.
 * @param data A string to compare with.
 * @return The number of paths that have the same data as the given data.
 */
int numPathsWithdata(SVGimage* img, char* data) {
    if (img == NULL) return 0;

    int count = 0;
    List* allPaths = getPaths(img);
    for (Node* currentPath = allPaths->head; currentPath != NULL; currentPath = currentPath->next) {
        if (strcmp(((Path*)(currentPath->data))->data, data) == 0) {
            count++;
        }
    }
    freeList(allPaths);
    return count;
}

/**
 * Finds the number of groups that match the given length.
 * @pre img can be NULL or point to a SVGimage struct. len can be any integer.
 * @post Number of groups with the given lengths is calculated.
 * @param img A pointer to a SVGimage struct.
 * @param len The length of a group to compare with.
 * @return The number of groups in the image that have the same length as len.
 */
int numGroupsWithLen(SVGimage* img, int len) {
    if (img == NULL) return 0;

    int count = 0;
    List* allGroups = getGroups(img);
    for (Node* currentGroup = allGroups->head; currentGroup != NULL; currentGroup = currentGroup->next) {
        //Add the number of rectangles, circles, paths, and groups of the node.
        int currentGroupLength = ((Group*)currentGroup->data)->rectangles->length +
                                 ((Group*)currentGroup->data)->circles->length +
                                 ((Group*)currentGroup->data)->paths->length +
                                 ((Group*)currentGroup->data)->groups->length;

        if (currentGroupLength == len) count++;
    }
    freeList(allGroups);
    return count;
}

/**
 * Counts the number of other attributes in the entire SVG image.
 * @pre img can be NULL or point to a SVGimage struct.
 * @post The number of other attributes in the image is calculated.
 * @param img A pointer to a SVGimage struct.
 * @return The number of other attributes in the SVG image, or 0 if img is NULL.
 */
int numAttr(SVGimage* img) {
    if (img == NULL) return 0;

    //Start the count of attributes with the number in the base image.
    int count = img->otherAttributes->length;

    //Add the number of attributes in the rectangles
    List* list = getRects(img);
    for (Node* node = list->head; node != NULL; node = node->next) {
        count += ((Rectangle*)(node->data))->otherAttributes->length;
    }
    freeList(list);

    //Add the number of attributes in the circles
    list = getCircles(img);
    for (Node* node = list->head; node != NULL; node = node->next) {
        count += ((Circle*)(node->data))->otherAttributes->length;
    }
    freeList(list);

    //Add the number of attributes in the paths
    list = getPaths(img);
    for (Node* node = list->head; node != NULL; node = node->next) {
        count += ((Path*)(node->data))->otherAttributes->length;
    }
    freeList(list);

    //Add the number of attributes in the groups (and their subgroups)
    list = getGroups(img);
    for (Node* node = list->head; node != NULL; node = node->next) {
        count += ((Group*)(node->data))->otherAttributes->length;
    }

    freeList(list);
    return count;
}

/**
 * Frees a Attribute from memory.
 * @pre data should point to a Attribute struct.
 * @post the information contained in data should be freed, and then data itself should be freed.
 * @param data void pointer to a Attribute struct.
 */
void deleteAttribute(void* data) {
    free(((Attribute*)data)->name);
    free(((Attribute*)data)->value);
    free(data);
}

/**
 * C equivalent of a Java toString, but for Attributes
 * @pre data should point to a Attribute struct.
 * @post A dynamically sized string is created to hold the string representation of the Attribute.
 * @param data void pointer to Attribute struct.
 * @return A string representation of the given Attribute.
 */
char* attributeToString(void* data) {
    char* tmpDesc = calloc(strlen(((Attribute*)data)->name) + strlen(((Attribute*)data)->value) + 64, sizeof(char));
    sprintf(tmpDesc, "[BEGIN ATTRIBUTE]\nname: %s\nvalue: %s\n[END ATTRIBUTE]\n", ((Attribute*)data)->name,
            ((Attribute*)data)->value);
    return tmpDesc;
}

/**Unused Attribute compare*/
int compareAttributes(const void* first, const void* second) {
    return 0;
}

/**
 * Frees a group from memory.
 * @pre data should point to a Group struct.
 * @post the information contained in data should be freed, and then data itself should be freed.
 * @param data void pointer to a Group struct.
 */
void deleteGroup(void* data) {
    if (((Group*)data)->rectangles != NULL) freeList(((Group*)data)->rectangles);
    if (((Group*)data)->circles != NULL) freeList(((Group*)data)->circles);
    if (((Group*)data)->paths != NULL) freeList(((Group*)data)->paths);
    if (((Group*)data)->groups != NULL) freeList(((Group*)data)->groups);
    if (((Group*)data)->otherAttributes != NULL) freeList(((Group*)data)->otherAttributes);
    free(data);
}

/**
 * C equivalent of a Java toString, but for Groups
 * @pre data should point to a Group struct.
 * @post A dynamically sized string is created to hold the string representation of the Group.
 * @param data void pointer to Group struct.
 * @return A string representation of the given Group.
 */
char* groupToString(void* data) {
    char* desc = calloc(32, sizeof(char));
    sprintf(desc, "[BEGIN GROUP]");
    char* listDesc = NULL;

    //Do toString for rectangles
    if (((Group*)data)->rectangles->length > 0) {
        listDesc = toString(((Group*)data)->rectangles);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    //Do toString for circles
    if (((Group*)data)->circles->length > 0) {
        listDesc = toString(((Group*)data)->circles);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    //Do toString for paths
    if (((Group*)data)->paths->length > 0) {
        listDesc = toString(((Group*)data)->paths);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    //Do toString for nested groups
    if (((Group*)data)->groups->length > 0) {
        listDesc = toString(((Group*)data)->groups);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    desc = realloc(desc, strlen(desc) + 16);
    strcat(desc, "[END GROUP]\n");
    return desc;
}

/**Unused Group compare*/
int compareGroups(const void* first, const void* second) {
    return 0;
}

/**
 * Frees a Rectangle from memory.
 * @pre data should point to a Rectangle struct.
 * @post the information contained in data should be freed, and then data itself should be freed.
 * @param data void pointer to a Rectangle struct.
 */
void deleteRectangle(void* data) {
    freeList(((Rectangle*)data)->otherAttributes);
    free(data);
}

/**
 * C equivalent of a Java toString, but for Rectangles
 * @pre data should point to a Rectangle struct.
 * @post A dynamically sized string is created to hold the string representation of the Rectangle.
 * @param data void pointer to Rectangle struct.
 * @return A string representation of the given Rectangle.
 */
char* rectangleToString(void* data) {
    char* tmpDesc = calloc(128, sizeof(char));
    sprintf(tmpDesc, "[BEGIN RECTANGLE]\nx: %.2f\ny: %.2f\nwidth: %.2f\nheight: %.2f\nunits: %s\n",
            ((Rectangle*)data)->x, ((Rectangle*)data)->y, ((Rectangle*)data)->width, ((Rectangle*)data)->height,
            ((Rectangle*)data)->units);

    //Get string descriptions for other attributes, dynamic sizing
    if (((Rectangle*)data)->otherAttributes->length > 0) {
        char* listDesc = toString(((Rectangle*)data)->otherAttributes);
        tmpDesc = realloc(tmpDesc, sizeof(char) * (strlen(tmpDesc) + strlen(listDesc) + 32));
        strcat(tmpDesc, listDesc);
        free(listDesc);
    }
    strcat(tmpDesc, "[END RECTANGLE]\n");
    return tmpDesc;
}

/**Unused rectablge compare*/
int compareRectangles(const void* first, const void* second) {
    return 0;
}

/**
 * Frees a Circle from memory.
 * @pre data should point to a Circle struct.
 * @post the information contained in data should be freed, and then data itself should be freed.
 * @param data void pointer to a Circle struct.
 */
void deleteCircle(void* data) {
    freeList(((Circle*)data)->otherAttributes);
    free(data);
}

/**
 * C equivalent of a Java toString, but for Circles
 * @pre data should point to a Circle struct.
 * @post A dynamically sized string is created to hold the string representation of the Circle.
 * @param data void pointer to Circle struct.
 * @return A string representation of the given Circle.
 */
char* circleToString(void* data) {
    char* tmpDesc = calloc(128, sizeof(char));
    sprintf(tmpDesc, "[BEGIN CIRCLE]\ncx: %.2f\ncy: %.2f\nr: %.2f\nunits: %s\n", ((Circle*)data)->cx,
            ((Circle*)data)->cy, ((Circle*)data)->r, ((Circle*)data)->units);

    //Get string descriptions for other attributes, dynamic sizing
    if (((Circle*)data)->otherAttributes->length > 0) {
        char* listDesc = toString(((Circle*)data)->otherAttributes);
        tmpDesc = realloc(tmpDesc, sizeof(char) * (strlen(tmpDesc) + strlen(listDesc) + 32));
        strcat(tmpDesc, listDesc);
        free(listDesc);
    }
    strcat(tmpDesc, "[END CIRCLE]\n");
    return tmpDesc;
}

/**Unused circle compare*/
int compareCircles(const void* first, const void* second) {
    return 0;
}

/**
 * Frees a Path from memory.
 * @pre data should point to a Path struct.
 * @post the information contained in data should be freed, and then data itself should be freed.
 * @param data void pointer to a Path struct.
 */
void deletePath(void* data) {
    free(((Path*)data)->data);
    freeList(((Path*)data)->otherAttributes);
    free(data);
}

/**
 * C equivalent of a Java toString, but for Paths
 * @pre data should point to a Path struct.
 * @post A dynamically sized string is created to hold the string representation of the Path.
 * @param data void pointer to Path struct.
 * @return A string representation of the given Path.
 */
char* pathToString(void* data) {
    char* tmpDesc = calloc(strlen(((Path*)data)->data) + 64, sizeof(char));
    sprintf(tmpDesc, "[BEGIN PATH]\nd: %s", ((Path*)data)->data);

    //Get string descriptions for other attributes, dynamic sizing
    if (((Path*)data)->otherAttributes->length > 0) {
        char* listDesc = toString(((Path*)data)->otherAttributes);
        tmpDesc = realloc(tmpDesc, sizeof(char) * (strlen(tmpDesc) + strlen(listDesc) + 32));
        strcat(tmpDesc, listDesc);
        free(listDesc);
    }
    strcat(tmpDesc, "[END PATH]\n");
    return tmpDesc;
}

/**Unused path compare*/
int comparePaths(const void* first, const void* second) {
    return 0;
}

/** Adds a rectangle to a list.
 * @pre node and list cannot be NULL.
 * @post A Rectangle is created, filled, and appended to the list.
 * @param node xmlNode of a path element.
 * @param list List of rectangles to add the new Path to.
 */
void addRectangle(xmlNode* node, List* list) {
    Rectangle* rectToAdd = calloc(1, sizeof(Rectangle));
    rectToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    //Needed for strtof
    char* units = NULL;

    for (xmlAttr* attrNode = node->properties; attrNode != NULL; attrNode = attrNode->next) {
        if (strcmp((char*)attrNode->name, "x") == 0) {
            /*This first case gives strtof the `units` field because we only care about the first element having units.
              If the fist has no units, we assume none do. And if the first has units, we assume the same for all elements.*/
            rectToAdd->x = strtof((char*)attrNode->children->content, &units);
        } else if (strcmp((char*)attrNode->name, "y") == 0) {
            rectToAdd->y = strtof((char*)attrNode->children->content, NULL);
        } else if (strcmp((char*)attrNode->name, "width") == 0) {
            rectToAdd->width = strtof((char*)attrNode->children->content, NULL);
        } else if (strcmp((char*)attrNode->name, "height") == 0) {
            rectToAdd->height = strtof((char*)attrNode->children->content, NULL);
        } else {
            insertBack(rectToAdd->otherAttributes, makeAttribute(attrNode));
        }
    }
    //Use strncpy to leave the null terminator
    if (units != NULL) strncpy(rectToAdd->units, units, 49);

    insertBack(list, rectToAdd);
}

/**
 * Adds a circle to a list.
 * @pre node and list cannot be NULL.
 * @post A Circle is created, filled, and appended to the list.
 * @param node xmlNode of a circle element.
 * @param list List of circles to add the new Path to.
 */
void addCircle(xmlNode* node, List* list) {
    Circle* circleToAdd = calloc(1, sizeof(Circle));
    circleToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    //Needed for strtof
    char* units = NULL;

    for (xmlAttr* attrNode = node->properties; attrNode != NULL; attrNode = attrNode->next) {
        if (strcmp((char*)attrNode->name, "cx") == 0) {
            /*This first case gives strtof the `units` field because we only care about the first element having units.
              If the fist has no units, we assume none do. And if the first has units, we assume the same for all elements.*/
            circleToAdd->cx = strtof((char*)attrNode->children->content, &units);
        } else if (strcmp((char*)attrNode->name, "cy") == 0) {
            circleToAdd->cy = strtof((char*)attrNode->children->content, NULL);
        } else if (strcmp((char*)attrNode->name, "r") == 0) {
            circleToAdd->r = strtof((char*)attrNode->children->content, NULL);
        } else {
            insertBack(circleToAdd->otherAttributes, makeAttribute(attrNode));
        }
    }
    //Use strncpy to leave the null terminator
    if (units != NULL) strncpy(circleToAdd->units, units, 49);

    insertBack(list, circleToAdd);
}

/**
 * Adds a path to a list.
 * @pre node and list cannot be NULL.
 * @post A Path is created, filled, and appended to the list.
 * @param node xmlNode of a path element.
 * @param list List of paths to add the new Path to.
 */
void addPath(xmlNode* node, List* list) {
    Path* pathToAdd = calloc(1, sizeof(Path));
    pathToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    for (xmlAttr* attrNode = node->properties; attrNode != NULL; attrNode = attrNode->next) {
        if (strcmp((char*)attrNode->name, "d") == 0) {
            pathToAdd->data = calloc(strlen((char*)attrNode->children->content) + 1, sizeof(char));
            strcpy(pathToAdd->data, (char*)attrNode->children->content);
        } else {
            insertBack(pathToAdd->otherAttributes, makeAttribute(attrNode));
        }
    }

    insertBack(list, pathToAdd);
}

/**
 * Adds a group to a list, recursively.
 * @pre node and list cannot be NULL.
 * @post A Group struct is created, filled, and appended to the list.
 * @param node xmlNode of a group element.
 * @param list List of groups to add the new Group to.
 */
void addGroup(xmlNode* node, List* list) {
    Group* groupToAdd = calloc(1, sizeof(Group));
    groupToAdd->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    groupToAdd->circles = initializeList(circleToString, deleteCircle, compareCircles);
    groupToAdd->paths = initializeList(pathToString, deletePath, comparePaths);
    groupToAdd->groups = initializeList(groupToString, deleteGroup, compareGroups);
    groupToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    for (xmlNode* currNode = node->children; currNode != NULL; currNode = currNode->next) {
        if (strcmp((char*)currNode->name, "rect") == 0) {
            addRectangle(currNode, groupToAdd->rectangles);
        } else if (strcmp((char*)currNode->name, "circle") == 0) {
            addCircle(currNode, groupToAdd->circles);
        } else if (strcmp((char*)currNode->name, "path") == 0) {
            addPath(currNode, groupToAdd->paths);
        } else if (strcmp((char*)currNode->name, "g") == 0) {
            addGroup(currNode, groupToAdd->groups);
        } else if (strcmp((char*)currNode->name, "title") == 0) {
            /*currNode casted to xmlAttr to avoid compiler warnings.
              Both xmlAttr and xmlNode have a `name` and `children` field though,
              making it perfectly fine to do this.*/
            insertBack(groupToAdd->otherAttributes, makeAttribute((xmlAttr*)currNode));
        } else if (strcmp((char*)currNode->name, "desc") == 0) {
            insertBack(groupToAdd->otherAttributes, makeAttribute((xmlAttr*)currNode));
        }
    }

    for (xmlAttr* attrNode = node->properties; attrNode != NULL; attrNode = attrNode->next) {
        insertBack(groupToAdd->otherAttributes, makeAttribute(attrNode));
    }

    insertBack(list, groupToAdd);
}

/**
 * Fully populates an Attribute struct, given the associated xmlAttr node.
 * @pre attrNode cannot be NULL.
 * @post Attribute struct allocated and populated.
 * @param attrNode xmlAttr node.
 * @return Populated Attribute struct.
 */
Attribute* makeAttribute(xmlAttr* attrNode) {
    Attribute* attrToAdd = calloc(1, sizeof(Attribute));
    attrToAdd->name = calloc(strlen((char*)attrNode->name) + 1, sizeof(char));
    attrToAdd->value = calloc(strlen((char*)attrNode->children->content) + 1, sizeof(char));
    strcpy(attrToAdd->name, (char*)attrNode->name);
    strcpy(attrToAdd->value, (char*)attrNode->children->content);
    return attrToAdd;
}

/**
 * Dummy function, used in getter delete list initializers.
 */
void dummy() {}

/**
 * Creates a valid SVG image struct.
 * @param fileName File name for the XML document.
 * @param schemaFile Schema file to validate the xml file against. Expected to be an SVG schema file.
 * @return A valid SVGimage struct if the given XML document is valid SVG, NULL otherwise.
 */
SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {
    /*Return NULL if:
        -fileName or schemaFile is NULL
        -fileName does not have a .svg extension
        -schemaFile does not have a .xsd extension*/
    if ((fileName == NULL || schemaFile == NULL) ||
        (strcmp(".xsd", schemaFile + (strlen(schemaFile) - 4)) != 0) ||
        (strcmp(".svg", fileName + (strlen(fileName) - 4)) != 0)) return NULL;

    //Declaring all the XML variables
    xmlSchemaParserCtxt* parserContext = NULL;
    xmlSchema* schema = NULL;
    xmlDoc* docToValidate = NULL;
    xmlSchemaValidCtxt* validator = NULL;
    SVGimage* image = NULL;

    //This is all for XML validation against a inputted schema files
    parserContext = xmlSchemaNewParserCtxt(schemaFile);
    if (parserContext == NULL) goto end;

    schema = xmlSchemaParse(parserContext);
    if (schema == NULL) goto end;

    docToValidate = xmlReadFile(fileName, NULL, 0);
    if (docToValidate == NULL) goto end;

    validator = xmlSchemaNewValidCtxt(schema);
    if (validator == NULL) goto end;

    int ret = xmlSchemaValidateDoc(validator, docToValidate);
    if (ret == 0) /*SVG file is valid*/ image = createSVGimage(fileName);

    end:
    if (parserContext != NULL) xmlSchemaFreeParserCtxt(parserContext);
    if (schema != NULL) xmlSchemaFree(schema);
    if (docToValidate != NULL) xmlFreeDoc(docToValidate);
    if (validator != NULL) xmlSchemaFreeValidCtxt(validator);
    xmlSchemaCleanupTypes();
    xmlCleanupParser();
    return image;
}

/**
 * Validate an SVGimage against a XSD file.
 * @param image The SVGimage to validate.
 * @param schemaFile File name for the XML XSD file to use.
 * @return True is the SVGimage is valid, false otherwise
 */
bool validateSVGimage(SVGimage* image, char* schemaFile) {
    //TODO
    return -1;
}

//TODO: Testing
/**
 * Writes the SVGimage to a SVG image file.
 * @param image SVGimage struct to write.
 * @param fileName Filename to write to.
 * @return True if completed successfully, false otherwise.
 */
bool writeSVGimage(SVGimage* image, char* fileName) {
    if (image == NULL || fileName == NULL) return false;
    xmlDoc* imageXML = imageToXML(image);
    if (imageXML == NULL) return false;
    return (xmlSaveFormatFileEnc(fileName, imageXML, "UTF-8", 1) == -1 ? false : true);
}

/**
 * Creates an XML tree from an SVGimage.
 * @param image The SVGimage struct to create an XML tree from.
 * @return A XML document based on the given SVGimage.
 */
xmlDoc* imageToXML(SVGimage* image) {
    if (image == NULL) return NULL;
    xmlDoc* imageXML = xmlNewDoc((xmlChar*)"1.0");
    xmlNode* rootNode = xmlNewNode(NULL, (xmlChar*)"svg");
    xmlNs* namespace = xmlNewNs(rootNode, (xmlChar*)image->namespace, NULL);
    xmlSetNs(rootNode, namespace);
    xmlDocSetRootElement(imageXML, rootNode);

    addRectsToXML(image->rectangles, xmlDocGetRootElement(imageXML));
    addCirclesToXML(image->circles, xmlDocGetRootElement(imageXML));
    addPathsToXML(image->paths, xmlDocGetRootElement(imageXML));
    addGroupsToXML(image->groups, xmlDocGetRootElement(imageXML));

    return imageXML;
}

/**
 * Adds elements in a Rectangle node to the XML document.
 * @param elementList The head rectangle node.
 * @param docHead The XML root node to add do.
 */
void addRectsToXML(List* elementList, xmlNode* docHead) {
    ListIterator iterator = createIterator(elementList);
    Rectangle* rect = NULL;
    char* name = calloc(5, sizeof(char));
    strcpy(name, "rect");

    while ((rect = nextElement(&iterator)) != NULL) {
        xmlNode* newNode = xmlNewNode(docHead->ns, (xmlChar*)name);

        //Adds all the properties to the newly created XML node
        char* value = calloc(10240, sizeof(char));
        sprintf(value, "%.2f", rect->x);
        xmlNewProp(newNode, (xmlChar*)"x", (xmlChar*)value);
        sprintf(value, "%.2f", rect->y);
        xmlNewProp(newNode, (xmlChar*)"y", (xmlChar*)value);
        sprintf(value, "%.2f", rect->width);
        xmlNewProp(newNode, (xmlChar*)"width", (xmlChar*)value);
        sprintf(value, "%.2f", rect->height);
        xmlNewProp(newNode, (xmlChar*)"height", (xmlChar*)value);
        //TODO: Go through the otherAttributes list

        //Adds the new node to the SVG doc
        xmlAddChild(docHead, newNode);
        free(value);
        //TODO: Check for memory leaks if I should free the node I made
    }

    free(name);
}

/**
 * Adds elements in a Circle node to the XML document.
 * @param elementList The head rectangle node.
 * @param docHead The XML root node to add do.
 */
void addCirclesToXML(List* elementList, xmlNode* docHead) {

}

/**
 * Adds elements in a Path node to the XML document.
 * @param elementList The head rectangle node.
 * @param docHead The XML root node to add do.
 */
void addPathsToXML(List* elementList, xmlNode* docHead) {

}

/**
 * Adds elements in a Group node to the XML document.
 * @param elementList The group node to treat as current root.
 * @param docHead The XML root node to add do.
 */
void addGroupsToXML(List* elementList, xmlNode* docHead) {
    for(Node* node = elementList; node != NULL; node = node->next) {
        //TODO: Add rects, circles, paths, groups; in that order. Groups are recursive with this function.
    }
}