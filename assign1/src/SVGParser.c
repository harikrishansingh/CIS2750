#include "SVGParser.h"
#include "Helper.h"
#include <math.h>
#include <strings.h>

SVGimage* createSVGimage(char* fileName) {
    xmlDoc* document = xmlReadFile(fileName, NULL, 0);
    //Return NULL if the parsing failed
    if (document == NULL) {
        xmlCleanupParser();
        return NULL;
    }

    xmlNode* rootNode = xmlDocGetRootElement(document);

    SVGimage* image = calloc(1, sizeof(SVGimage));

    //Get namespace
    strcpy(image->namespace, (char*)rootNode->ns->href);

    image->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    image->circles = initializeList(circleToString, deleteCircle, compareCircles);
    image->paths = initializeList(pathToString, deletePath, comparePaths);
    image->groups = initializeList(groupToString, deleteGroup, compareGroups);
    image->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    for (xmlNode* currNode = rootNode->children; currNode != NULL; currNode = currNode->next) {
        if (strcasecmp((char*)currNode->name, "rect") == 0) {
            addRectangle(currNode, image->rectangles);
        } else if (strcasecmp((char*)currNode->name, "circle") == 0) {
            addCircle(currNode, image->circles);
        } else if (strcasecmp((char*)currNode->name, "path") == 0) {
            addPath(currNode, image->paths);
        } else if (strcasecmp((char*)currNode->name, "g") == 0) {
            addGroup(currNode, image->groups);
        } else if (strcasecmp((char*)currNode->name, "title") == 0) {
            strcpy(image->title, (char*)currNode->children->content);
        } else if (strcasecmp((char*)currNode->name, "desc") == 0) {
            strcpy(image->description, (char*)currNode->children->content);
        }
    }

    for (xmlAttr* attrNode = rootNode->properties; attrNode != NULL; attrNode = attrNode->next) {
        insertBack(image->otherAttributes, makeAttribute(attrNode));
    }

    xmlFreeDoc(document);
    xmlCleanupParser();
    return image;
}

char* SVGimageToString(SVGimage* img) {
    char* desc = calloc(strlen(img->namespace) + strlen(img->title) + strlen(img->description) + 512, sizeof(char)); //242 extra bytes for \0's and extra words in the next line
    sprintf(desc, "[BEGIN SVG]\n[NAMESPACE]\n%s\n[TITLE]\n%s\n[DESCRIPTION]\n%s", img->namespace, img->title, img->description);

    char* listDesc = NULL;
    if (img->otherAttributes->length > 0) {
        listDesc = toString(img->otherAttributes);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    if (img->rectangles->length > 0) {
        listDesc = toString(img->rectangles);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    if (img->circles->length > 0) {
        listDesc = toString(img->circles);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

    if (img->paths->length > 0) {
        listDesc = toString(img->paths);
        desc = realloc(desc, strlen(desc) + strlen(listDesc) + 8);
        strcat(desc, listDesc);
        free(listDesc);
    }

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

void deleteSVGimage(SVGimage* img) {
    if (img == NULL) return;
    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->paths);
    freeList(img->groups);
    freeList(img->otherAttributes);
    free(img);
}

//TODO: Possibly initialize delete functions to NULL to prevent double free
//TODO
List* getRects(SVGimage* img) {
    return NULL;
}

//TODO
List* getCircles(SVGimage* img) {
    return NULL;
}

//TODO
List* getGroups(SVGimage* img) {
    return NULL;
}

//TODO
List* getPaths(SVGimage* img) {
    return NULL;
}

int numRectsWithArea(SVGimage* img, float area) {
    int count = 0;

    //TODO: Use rectangles getter
    if (img != NULL && img->rectangles->length > 0) {
        for (Node* currentRect = img->rectangles->head; currentRect != NULL; currentRect = currentRect->next) {
            if (ceil(((Rectangle*)currentRect->data)->width * ((Rectangle*)currentRect->data)->height) == ceil(area)) {
                count++;
            }
        }
    }

    return count;
}

int numCirclesWithArea(SVGimage* img, float area) {
    int count = 0;

    //TODO: Use circles getter
    if (img != NULL && img->circles->length > 0) {
        for (Node* currentCircle = img->circles->head; currentCircle != NULL; currentCircle = currentCircle->next) {
            if (ceil(((Circle*)currentCircle->data)->r * ((Circle*)currentCircle->data)->r * PI) == ceil(area)) {
                count++;
            }
        }
    }

    return count;
}

int numPathsWithdata(SVGimage* img, char* data) {
    int count = 0;

    //TODO: Use path getter
    if (img != NULL && img->paths->length > 0) {
        for (Node* currentPath = img->paths->head; currentPath != NULL; currentPath = currentPath->next) {
            if (strcasecmp(((Path*)currentPath)->data, data) == 0) {
                count++;
            }
        }
    }

    return count;
}

int numGroupsWithLen(SVGimage* img, int len) {
    int count = 0;

    //TODO: use groups getter
    if (img != NULL && img->groups->length > 0) {
        for (Node* currentGroup = img->groups->head; currentGroup != NULL; currentGroup = currentGroup->next) {
            int currentGroupLength = ((Group*)currentGroup->data)->rectangles->length +
                                     ((Group*)currentGroup->data)->circles->length +
                                     ((Group*)currentGroup->data)->paths->length +
                                     ((Group*)currentGroup->data)->groups->length;

            if (currentGroupLength == len) count++;
        }
    }

    return count;
}

int numAttr(SVGimage* img) {
    //TODO: Loop through ALL nodes and count (NOT including the description, title, or namespace))
    return 0;
}

void deleteAttribute(void* data) {
    free(((Attribute*)data)->name);
    free(((Attribute*)data)->value);
    free(data);
}

char* attributeToString(void* data) {
    char* tmpDesc = calloc(strlen(((Attribute*)data)->name) + strlen(((Attribute*)data)->value) + 64, sizeof(char));
    sprintf(tmpDesc, "[BEGIN ATTRIBUTE]\nname: %s\nvalue: %s\n[END ATTRIBUTE]\n", ((Attribute*)data)->name, ((Attribute*)data)->value);
    return tmpDesc;
}

//Unused
int compareAttributes(const void* first, const void* second) {
    return 0;
}

void deleteGroup(void* data) {
    if (((Group*)data)->rectangles != NULL) freeList(((Group*)data)->rectangles);
    if (((Group*)data)->circles != NULL) freeList(((Group*)data)->circles);
    if (((Group*)data)->paths != NULL) freeList(((Group*)data)->paths);
    if (((Group*)data)->groups != NULL) freeList(((Group*)data)->groups);
    if (((Group*)data)->otherAttributes != NULL) freeList(((Group*)data)->otherAttributes);
    free(data);
}

char* groupToString(void* data) {
    char* temp = calloc(64, sizeof(char));
    strcpy(temp, "<GROUP PLACEHOLDER>\n");
    return temp;
}

//Unused
int compareGroups(const void* first, const void* second) {
    return 0;
}

void deleteRectangle(void* data) {
    freeList(((Rectangle*)data)->otherAttributes);
    free(data);
}

char* rectangleToString(void* data) {
    char* tmpDesc = calloc(128, sizeof(char));
    sprintf(tmpDesc, "[BEGIN RECTANGLE]\nx: %f\ny: %f\nwidth: %f\nheight: %f\nunits: %s\n", ((Rectangle*)data)->x, ((Rectangle*)data)->y, ((Rectangle*)data)->width, ((Rectangle*)data)->height, ((Rectangle*)data)->units);

    if (((Rectangle*)data)->otherAttributes->length > 0) {
        char* listDesc = toString(((Rectangle*)data)->otherAttributes);
        tmpDesc = realloc(tmpDesc, sizeof(char) * (strlen(tmpDesc) + strlen(listDesc) + 64));
        strcat(tmpDesc, listDesc);
        free(listDesc);
    }

    strcat(tmpDesc, "[END RECTANGLE]\n");

    return tmpDesc;
}

//Unused
int compareRectangles(const void* first, const void* second) {
    return 0;
}

void deleteCircle(void* data) {
    freeList(((Circle*)data)->otherAttributes);
    free(data);
}

char* circleToString(void* data) {
    char* tmpDesc = calloc(128, sizeof(char));
    sprintf(tmpDesc, "[BEGIN CIRCLE]\ncx: %f\ncy: %f\nr: %f\nunits: %s", ((Circle*)data)->cx, ((Circle*)data)->cy, ((Circle*)data)->r, ((Circle*)data)->units);

    if (((Circle*)data)->otherAttributes->length > 0) {
        char* listDesc = toString(((Circle*)data)->otherAttributes);
        tmpDesc = realloc(tmpDesc, sizeof(char) * (strlen(tmpDesc) + strlen(listDesc) + 64));
        strcat(tmpDesc, listDesc);
        free(listDesc);
    }

    strcat(tmpDesc, "[END CIRCLE]\n");

    return tmpDesc;
}

//Unused
int compareCircles(const void* first, const void* second) {
    return 0;
}

void deletePath(void* data) {
    free(((Path*)data)->data);
    freeList(((Path*)data)->otherAttributes);
    free(data);
}

char* pathToString(void* data) {
    char* tmpDesc = calloc(strlen(((Path*)data)->data) + 64, sizeof(char));
    sprintf(tmpDesc, "[BEGIN PATH]\nd: %s\n[END PATH]\n", ((Path*)data)->data);

    if (((Path*)data)->otherAttributes->length > 0) {
        char* listDesc = toString(((Path*)data)->otherAttributes);
        tmpDesc = realloc(tmpDesc, sizeof(char) * (strlen(tmpDesc) + strlen(listDesc) + 64));
        strcat(tmpDesc, listDesc);
        free(listDesc);
    }

    strcat(tmpDesc, "[END PATH]\n");

    return tmpDesc;
}

//Unused
int comparePaths(const void* first, const void* second) {
    return 0;
}

void addRectangle(xmlNode* node, List* list) {
    Rectangle* rectToAdd = calloc(1, sizeof(Rectangle));
    rectToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    //Needed for strtof
    char* units = NULL;

    for (xmlAttr* attrNode = node->properties; attrNode != NULL; attrNode = attrNode->next) {
        if (strcasecmp((char*)attrNode->name, "x") == 0) {
            /*This first case gives strtof the `units` field because we only care about the first element having units.
              If the fist has no units, we assume none do. And if the first has units, we assume the same for all elements.*/
            rectToAdd->x = strtof((char*)attrNode->children->content, &units);
        } else if (strcasecmp((char*)attrNode->name, "y") == 0) {
            rectToAdd->y = strtof((char*)attrNode->children->content, NULL);
        } else if (strcasecmp((char*)attrNode->name, "width") == 0) {
            rectToAdd->width = strtof((char*)attrNode->children->content, NULL);
        } else if (strcasecmp((char*)attrNode->name, "height") == 0) {
            rectToAdd->height = strtof((char*)attrNode->children->content, NULL);
        } else {
            insertBack(rectToAdd->otherAttributes, makeAttribute(attrNode));
        }
    }
    if (units != NULL) strcpy(rectToAdd->units, units);

    insertBack(list, rectToAdd);
}

void addCircle(xmlNode* node, List* list) {
    Circle* circleToAdd = calloc(1, sizeof(Circle));
    circleToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    //Needed for strtof
    char* units = NULL;

    for (xmlAttr* attrNode = node->properties; attrNode != NULL; attrNode = attrNode->next) {
        if (strcasecmp((char*)attrNode->name, "cx") == 0) {
            /*This first case gives strtof the `units` field because we only care about the first element having units.
              If the fist has no units, we assume none do. And if the first has units, we assume the same for all elements.*/
            circleToAdd->cx = strtof((char*)attrNode->children->content, &units);
        } else if (strcasecmp((char*)attrNode->name, "cy") == 0) {
            circleToAdd->cy = strtof((char*)attrNode->children->content, NULL);
        } else if (strcasecmp((char*)attrNode->name, "r") == 0) {
            circleToAdd->r = strtof((char*)attrNode->children->content, NULL);
        } else {
            insertBack(circleToAdd->otherAttributes, makeAttribute(attrNode));
        }
    }
    if (units != NULL) strcpy(circleToAdd->units, units);

    insertBack(list, circleToAdd);
}

void addPath(xmlNode* node, List* list) {
    Path* pathToAdd = calloc(1, sizeof(Path));
    pathToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    for (xmlAttr* attrNode = node->properties; attrNode != NULL; attrNode = attrNode->next){
        if (strcasecmp((char*)attrNode->name, "d") == 0) {
            pathToAdd->data = calloc(strlen((char*)attrNode->children->content) + 1, sizeof(char));
            strcpy(pathToAdd->data, (char*)attrNode->children->content);
        } else {
            insertBack(pathToAdd->otherAttributes, makeAttribute(attrNode));
        }
    }

    insertBack(list, pathToAdd);
}

void addGroup(xmlNode* node, List* list) {
    Group* groupToAdd = calloc(1, sizeof(Group));
    groupToAdd->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    groupToAdd->circles = initializeList(circleToString, deleteCircle, compareCircles);
    groupToAdd->paths = initializeList(pathToString, deletePath, comparePaths);
    groupToAdd->groups = initializeList(groupToString, deleteGroup, compareGroups);
    groupToAdd->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    for (xmlNode* currNode = node->children; currNode != NULL; currNode = currNode->next) {
        if (strcasecmp((char*)currNode->name, "rect") == 0) {
            addRectangle(currNode, groupToAdd->rectangles);
        } else if (strcasecmp((char*)currNode->name, "circle") == 0) {
            addCircle(currNode, groupToAdd->circles);
        } else if (strcasecmp((char*)currNode->name, "path") == 0) {
            addPath(currNode, groupToAdd->paths);
        } else if (strcasecmp((char*)currNode->name, "g") == 0) {
            addGroup(currNode, groupToAdd->groups);
        /*} else if (strcasecmp((char*)currNode->name, "title") == 0) {
            strcpy(image->title, (char*)currNode->children->content);
        } else if (strcasecmp((char*)currNode->name, "desc") == 0) {
            strcpy(image->description, (char*)currNode->children->content);*/
        //Not sure if the above should be put in the "other attributes" list or not
        }
    }

    for (xmlAttr* attrNode = node->properties; attrNode != NULL; attrNode = attrNode->next) {
        insertBack(groupToAdd->otherAttributes, makeAttribute(attrNode));
    }

    insertBack(list, groupToAdd);
}

Attribute* makeAttribute(xmlAttr* attrNode) {
    Attribute* attrToAdd = calloc(1, sizeof(Attribute));
    attrToAdd->name = calloc(strlen((char*)attrNode->name) + 1, sizeof(char));
    attrToAdd->value = calloc(strlen((char*)attrNode->children->content) + 1, sizeof(char));
    strcpy(attrToAdd->name, (char*)attrNode->name);
    strcpy(attrToAdd->value, (char*)attrNode->children->content);
    return attrToAdd;
}
