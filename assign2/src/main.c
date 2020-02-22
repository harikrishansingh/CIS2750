#include <Helper.h>
#include "SVGParser.h"

Circle* getTestCircle();
Path* getTestPath();
Group* getTestGroup();
Attribute* getTestAttr1();
Attribute* getTestAttr2();

int main() {
    SVGimage* image = createSVGimage("testFiles/hen_and_chicks.svg");
    List* list = getPaths(image);
    char* string = pathListToJSON(list);

    FILE* out = fopen("temp.svg", "w");
    fwrite(string, sizeof(char), strlen(string), out);
    fclose(out);

    free(string);
    freeList(list);
    deleteSVGimage(image);
}

Rectangle* getTestRect() {
    Rectangle* r = calloc(1, sizeof(Rectangle));
    r->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    r->x = 1;
    r->y = 1;
    r->width = 100;
    r->height = 150;
    strcpy(r->units, "px");
    insertBack(r->otherAttributes, getTestAttr1());
    insertBack(r->otherAttributes, getTestAttr2());

    return r;
}

Circle* getTestCircle() {
    Circle* c = calloc(1, sizeof(Circle));
    c->otherAttributes = initializeList(attributeToString, deleteCircle, compareCircles);
    c->cx = 10;
    c->cy = 10;
    c->r = 3;
    strcpy(c->units, "mm");
    insertBack(c->otherAttributes, getTestAttr1());
    insertBack(c->otherAttributes, getTestAttr2());

    return c;
}

Path* getTestPath() {
    Path* p = calloc(1, sizeof(Path));
    p->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    p->data = calloc(strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz12STOP") + 1, sizeof(char));
    strcpy(p->data, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz12STOP");
    insertBack(p->otherAttributes, getTestAttr1());
    insertBack(p->otherAttributes, getTestAttr2());

    return p;
}

Group* getTestGroup() {
    Group* g = calloc(1, sizeof(Group));
    g->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    g->circles = initializeList(circleToString, deleteCircle, compareCircles);
    g->paths = initializeList(pathToString, deletePath, comparePaths);
    g->groups = initializeList(groupToString, deleteGroup, compareGroups);
    g->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    insertBack(g->rectangles, getTestRect());
    insertBack(g->circles, getTestCircle());
    insertBack(g->paths, getTestPath());
    insertBack(g->otherAttributes, getTestAttr1());
    insertBack(g->otherAttributes, getTestAttr2());

    return g;
}

Attribute* getTestAttr1() {
    Attribute* attr = calloc(1, sizeof(Attribute));
    attr->name = calloc(64, sizeof(char));
    attr->value = calloc(64, sizeof(char));
    strcpy(attr->name, "fill");
    strcpy(attr->value, "#ff1199");
    return attr;
}

Attribute* getTestAttr2() {
    Attribute* attr = calloc(1, sizeof(Attribute));
    attr->name = calloc(64, sizeof(char));
    attr->value = calloc(64, sizeof(char));
    strcpy(attr->name, "stroke");
    strcpy(attr->value, "red");
    return attr;
}
