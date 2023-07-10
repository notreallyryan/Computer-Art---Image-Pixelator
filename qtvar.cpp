#include "qtvar.h"

bool qtvar::prunable(Node* node, const int tol) const {
    return (node->var < tol);
}
