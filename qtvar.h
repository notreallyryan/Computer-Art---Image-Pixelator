#ifndef _QTVAR_H_
#define _QTVAR_H_

#include <cmath>
#include <utility>

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "quadtree.h"

using namespace std;
using namespace cs221util;

class qtvar : public quadtree {
/*
this methodology considers a node prunable if its own average is below or equal to the provided tolerance. Children are ignored. 
*/
public:
    qtvar(PNG& im) : quadtree(im) {}

private:
    bool prunable(Node* node, const int tol) const;
};

#endif
