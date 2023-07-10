#include "qtcount.h"

bool qtcount::prunable(Node* node, const int tol) const {
    //create stack for leaves and get average pixel values
    stack<Node*> leaves;
    getLeaves(leaves, node);
    int rAvg = node->avg.r;
    int gAvg = node->avg.g;
    int bAvg = node->avg.b;

    while(!leaves.empty()){
        //check if the average of all children of the node are within the tolerated variance from their parents.
        Node* current = leaves.top();
        leaves.pop();
        int distance = pow((current->avg.r - rAvg),2) + pow((current->avg.g - gAvg),2) + pow((current->avg.b - bAvg),2);
        if (distance > tol){return false;}
    }
    return true; //return true if all are within tolerance.
}

void qtcount::getLeaves(stack<Node*>& x, Node* node) const{
    if(node->NW != nullptr){
        getLeaves(x, node->NW);
        getLeaves(x, node->NE);
        getLeaves(x, node->SE);
        getLeaves(x, node->SW);
    }
    else{x.push(node);}
}