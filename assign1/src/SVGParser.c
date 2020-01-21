#include "SVGParser.h"
#include "Helper.h"
#include <malloc.h>

SVGimage* createSVGimage(char* fileName) {
    SVGimage* image = calloc(1, sizeof(SVGimage));

    xmlDoc* document = xmlReadFile(fileName, NULL, 0);
    //Return NULL if the parsing failed
    if (document == NULL) return NULL;

    xmlNode* rootNode = xmlDocGetRootElement(document);

    //Get namespace
    strcpy(image->namespace, (char*)rootNode->nsDef->href);

    //Get title, if it exists
    xmlNode* titleNode = rootNode->children;
    do {
        titleNode = titleNode->next;
    } while (titleNode != NULL && strcmp("title", (char*)titleNode->name) != 0);
    strcpy(image->title, titleNode == NULL ? "" : (char*)titleNode->children->content);

    //Get description
    xmlNode* descNode = rootNode->children;
    do {
        descNode = descNode->next;
    } while (descNode != NULL && strcmp("desc", (char*)descNode->name) != 0);
    strcpy(image->description, descNode == NULL ? "" : (char*)descNode->children->content);
    //TODO: Generalize title and description functions

  image->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
  image->circles = initializeList(circleToString, deleteCircle, compareCircles);
  image->paths = initializeList(pathToString, deletePath, comparePaths);
  image->groups = initializeList(groupToString, deleteGroup, compareGroups);
  image->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    //TODO: Make sure im not forgetting the other attributes thing
    for (xmlNode *currNode = rootNode->children; currNode != NULL; currNode = currNode->next) {
      if (strcmp((char*)currNode->name, "rect") == 0) {
        populateRects(currNode, image->rectangles);
      } else if (strcmp((char*)currNode->name, "circle") == 0) {
        populateCircles(currNode, image->circles);
      } else if (strcmp((char*)currNode->name, "path") == 0) {
        populatePaths(currNode, image->paths);
      } else if (strcmp((char*)currNode->name, "g") == 0) {
        populateGroups(currNode, image->groups);
      }
    }

    xmlFreeDoc(document);
    xmlCleanupParser();
    return image;
}

char* SVGimageToString(SVGimage* img) {
    char* desc = calloc(5120, sizeof(char));
    sprintf(desc, "Namespace: %s\nTitle: %s\nDescription: %s", img->namespace, img->title, img->description);

    char* listDesc;
    if (img->rectangles->length > 0) {
        listDesc = toString(img->rectangles);
        strcat(desc, listDesc);
        free(listDesc);
    }

    if (img->circles->length > 0) {
        listDesc = toString(img->circles);
        strcat(desc, listDesc);
        free(listDesc);
    }

    if (img->paths->length > 0) {
        listDesc = toString(img->paths);
        strcat(desc, listDesc);
        free(listDesc);
    }

    if (img->groups->length > 0) {
        listDesc = toString(img->groups);
        strcat(desc, listDesc);
        free(listDesc);
    }

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

List* getRects(SVGimage* img) {
    return img == NULL ? NULL : img->rectangles;
}

List* getCircles(SVGimage* img) {
    return img == NULL ? NULL : img->circles;
}

List* getGroups(SVGimage* img) {
    return img == NULL ? NULL : img->groups;
}

List* getPaths(SVGimage* img) {
    return img == NULL ? NULL : img->paths;
}

int numRectsWithArea(SVGimage* img, float area) {
    int count = 0;

    if (img != NULL && img->rectangles->length > 0) {
        Node* currentRect = img->rectangles->head;
        for (int i = img->rectangles->length; i >= 0; i--) {
            if ((((Rectangle*)currentRect->data)->width * ((Rectangle*)currentRect->data)->height) == area) {
                count++;
            }
            //TODO: Make sure this is next and not previous
            currentRect = currentRect->next;
        }
    }

    return count;
}

int numCirclesWithArea(SVGimage* img, float area) {
    int count = 0;

    if (img != NULL && img->circles->length > 0) {
        Node* currentCircle = img->circles->head;
        for (int i = img->circles->length; i >= 0; i--) {
            if ((((Circle*)currentCircle->data)->r * ((Circle*)currentCircle->data)->r * PI) == area) {
                count++;
            }
            //TODO: Make sure this is next and not previous
            currentCircle = currentCircle->next;
        }
    }

    return count;
}

int numPathsWithdata(SVGimage* img, char* data) {
    int count = 0;

    if (img != NULL && img->paths->length > 0) {
        Node* currentPath = img->paths->head;
        for (int i = img->paths->length; i >= 0; i--) {
            if (strcmp(((Path*)currentPath)->data, data) == 0) {
                count++;
            }
            //TODO: Make sure this is next and not previous
            currentPath = currentPath->next;
        }
    }

    return count;
}

int numGroupsWithLen(SVGimage* img, int len) {
    int count = 0;

    if (img != NULL && img->groups->length > 0) {
        Node* currentGroup = img->groups->head;
        for (int i = img->paths->length; i >= 0; i--) {
            int currentGroupLength = ((Group*)currentGroup->data)->rectangles->length +
                                     ((Group*)currentGroup->data)->circles->length +
                                     ((Group*)currentGroup->data)->paths->length +
                                     ((Group*)currentGroup->data)->groups->length;

            if (currentGroupLength == len) {
                count++;
            }
            //TODO: Make sure this is next and not previous
            currentGroup = currentGroup->next;
        }
    }

    return count;
}

int numAttr(SVGimage* img) {
    //TODO: Loop through rects, circles, paths, groups, attribute lists for this (INCLUDES SVG NODE ATTRIBUTES (NOT including the description, title, or namespace))
    return 0;
}

void deleteAttribute(void* data) {

}

char* attributeToString(void* data) {
    char* temp = calloc(14, sizeof(char));
    strcpy(temp, "<PLACEHOLDER>");
    return temp;
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
    free(data);
}

char* groupToString(void* data) {
    char* temp = calloc(14, sizeof(char));
    strcpy(temp, "<PLACEHOLDER>");
    return temp;
}

//Unused
int compareGroups(const void* first, const void* second) {
    return 0;
}

void deleteRectangle(void* data) {
    free(data);
}

char* rectangleToString(void* data) {
    char* temp = calloc(14, sizeof(char));
    strcpy(temp, "<PLACEHOLDER>");
    return temp;
}

//Unused
int compareRectangles(const void* first, const void* second) {
    return 0;
}

void deleteCircle(void* data) {
    free(data);
}

char* circleToString(void* data) {
    char* temp = calloc(14, sizeof(char));
    strcpy(temp, "<PLACEHOLDER>");
    return temp;
}

//Unused
int compareCircles(const void* first, const void* second) {
    return 0;
}

void deletePath(void* data) {
    freeList(((Path*)data)->otherAttributes);
    free(data);
}

char* pathToString(void* data) {
    char* temp = calloc(14, sizeof(char));
    strcpy(temp, "<PLACEHOLDER>");
    return temp;
}

//Unused
int comparePaths(const void* first, const void* second) {
    return 0;
}

//Make it just make a new rectangle and insert back
List* populateRects(xmlNode* rootNode, List* list) {
    for (xmlNode* node = rootNode; node != NULL; node = node->next) {
        if (strcmp((char*)node->name, "rect") != 0) continue;

        Rectangle* rectToAdd = calloc(1, sizeof(Rectangle));

        char* value = (char*)xmlGetProp(node, (xmlChar*)"x");
        rectToAdd->x = value == NULL ? 0 : split(value, NULL);
        xmlFree(value);

        value = (char*)xmlGetProp(node, (xmlChar*)"y");
        rectToAdd->y = value == NULL ? 0 : split(value, NULL);
        xmlFree(value);

        value = (char*)xmlGetProp(node, (xmlChar*)"width");
        rectToAdd->width = value == NULL ? 0 : split(value, NULL);
        xmlFree(value);

        value = (char*)xmlGetProp(node, (xmlChar*)"height");
        rectToAdd->height = value == NULL ? 0 : split(value, NULL);
        xmlFree(value);

        value = (char*)xmlGetProp(node, (xmlChar*)"units");
        if (value != NULL) split(value, rectToAdd->units);
        xmlFree(value);

        //TODO: Attributes
//        rectToAdd->otherAttributes ;

        insertBack(list, rectToAdd);
    }

    return list;
}

List* populateCircles(xmlNode* rootNode, List* list) {
    for (xmlNode* node = rootNode; node != NULL; node = node->next) {
        if (strcmp((char*)node->name, "circle") != 0) continue;

        Circle* circleToAdd = calloc(1, sizeof(Circle));

        char* value = (char*)xmlGetProp(node, (xmlChar*)"cx");
        circleToAdd->cx = value == NULL ? 0 : split(value, NULL);
        xmlFree(value);

        value = (char*)xmlGetProp(node, (xmlChar*)"cy");
        circleToAdd->cy = value == NULL ? 0 : split(value, NULL);
        xmlFree(value);

        value = (char*)xmlGetProp(node, (xmlChar*)"r");
        circleToAdd->r = value == NULL ? 0 : split(value, NULL);
        xmlFree(value);

        value = (char*)xmlGetProp(node, (xmlChar*)"units");
        if (value != NULL) split(value, circleToAdd->units);
        xmlFree(value);

        //TODO: Attributes
//        circleToAdd->otherAttributes ;

        insertBack(list, circleToAdd);
    }

    return list;
}

List* populatePaths(xmlNode* rootNode, List* list) {
    for (xmlNode* node = rootNode; node != NULL; node = node->next) {
        if (strcmp((char*)node->name, "path") != 0) continue;

        Path* pathToAdd = calloc(1, sizeof(Path));

        char* value = (char*)xmlGetProp(node, (xmlChar*)"d");
        pathToAdd->data = calloc(strlen(value) + 1, sizeof(char));
        strcpy(pathToAdd->data, value);
        xmlFree(value);

        //TODO: Attributes
//        pathToAdd->otherAttributes ;

        insertBack(list, pathToAdd);
    }

    return list;
}

List* populateGroups(xmlNode* rootNode, List* list) {
    //TODO: Make sure im not forgetting the other attributes thing
    //TODO: Make work
    for (xmlNode *currNode = rootNode->children; currNode != NULL; currNode = currNode->next) {
      if (strcmp((char*)currNode->name, "rect") == 0) {
//        populateRects(currNode, image->rectangles);
      } else if (strcmp((char*)currNode->name, "circle") == 0) {
//        populateCircles(currNode, image->circles);
      } else if (strcmp((char*)currNode->name, "path") == 0) {
//        populatePaths(currNode, image->paths);
      } else if (strcmp((char*)currNode->name, "g") == 0) {
//        populateGroups(currNode, image->groups);
      }
    }

    return list;
}

List* populateAttr(xmlNode* rootNode, List* list) {

    return list;
}

float split(char* input, char* units) {
    return (float)strtol(input, &units, 10);
}
