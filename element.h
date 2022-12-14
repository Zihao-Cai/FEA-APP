#ifndef ELEMENT_H
#define ELEMENT_H
#include "node.h"

class Element
{
public:
    Element();
    Node* nodes;
    Node eq_nodes[3];
    double delta[6];
    double epson[3];
    double sigma[3];
    double eq_a[3];
    double eq_b[3];
    double eq_c[3];
    double E;
    double v;
    double t;
    double A;
};

#endif // ELEMENT_H
