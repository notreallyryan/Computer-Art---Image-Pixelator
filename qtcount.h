#ifndef _QTCOUNT_H_
#define _QTCOUNT_H_

#include <cmath>
#include <utility>
#include <stack>

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "quadtree.h"

using namespace std;
using namespace cs221util;

class qtcount : public quadtree {
/*
This methodology considers a node prunable if all of its children have pixel averages below or equal to the provided tolerance. 
*/
public:
    qtcount(PNG& im) : quadtree(im) {}

private:
    bool prunable(Node* node, const int tol) const;
    void getLeaves(stack<Node*>& x, Node* node) const;
};

#endif
