
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG& imIn) {
    //create stats object, get all variables + build
    stats info = stats(imIn);
    pair<int,int> start(0,0);
    edge = pow(2, (int)log2(min(imIn.width(), imIn.height())));
    int dim = log2(edge);
    root = buildTree(info, start, dim);
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    //make Node at this point
    Node* point = new Node(ul,dim,s.getAvg(ul, dim),s.getVar(ul, dim));
    int newdim = dim-1;

    //if newdim < 0 (i.e. no pixels within subsquare), terminate. Else recursively get next node. 
    if (newdim >= 0){
        int newx = ul.first + pow(2, newdim);
        int newy = ul.second + pow(2, newdim);

        point->NW = buildTree(s, ul ,newdim);
        point->NE = buildTree(s, {newx, ul.second} ,newdim);
        point->SE = buildTree(s, {newx, newy} ,newdim);
        point->SW = buildTree(s, {ul.first, newy} ,newdim);
    }
    return point;
}

PNG quadtree::render() const {
    //create blank PNG object
    PNG image = PNG(pow(2, root->dim), pow(2, root->dim));
    stack<Node*> toDo;
    toDo.push(root);
    while (!toDo.empty()){
        Node* curr = toDo.top();
        toDo.pop();
        if (curr->NW == nullptr){ //if current node has no children
            int startx = curr->upLeft.first; //get upper left corner coordinates
            int starty = curr->upLeft.second;
            int f = pow(2, curr->dim); //get square dimensions

            //set all pixels in square to the average pixel.
            for(int i = startx; i < startx + f; i++){
                for(int j = starty; j < starty + f; j++){
                    *image.getPixel(i,j) = curr->avg;
                }
            }
        }
        else{ //if current node has children, use them instead.
            toDo.push(curr->NW);
            toDo.push(curr->NE);
            toDo.push(curr->SE);
            toDo.push(curr->SW);
        }
    }
    return image;
}

int quadtree::idealPrune(const int leaves) const {
    int low = 0;
    int max = 1;
    int tol = 0;
    int mid = 0;
    if(leaves == edge*edge){return 0;} //if same number of pixels in square image
    if (pruneSize(0) <= leaves){return 0;} //if lowest tolerance image is still lower quality than wanted
    while(pruneSize(max)>leaves){max *=2;} //update max tolerance until it makes leaves larger than leaves amount

    //keep updating midpoint to narrow in on ideal Prune tolerance.
    while(low<max){
        mid = ceil((max+low)/2);
        int curleaves = pruneSize(mid);
        if(max-low == 1){break;}
        else if (curleaves > leaves){low = mid;}
        else{max = mid;}
    }

    if(pruneSize(mid) != leaves){mid++;}//final update to account for tolerance underestimation (overestimation never occurs)
    return mid;
}

int quadtree::pruneSize(const int tol) const {
    return PShelper(tol, root);
}

int quadtree::PShelper(const int tol, Node* u) const{
    if(u->NW == nullptr || prunable(u, tol)){return 1;} //if no more children, or this node is prunable, terminate. 
    int subtotal = 0;
    subtotal += PShelper(tol, u->NW);
    subtotal += PShelper(tol, u->NE);
    subtotal += PShelper(tol, u->SE);
    subtotal += PShelper(tol, u->SW);
    return subtotal; //else return sum of remaining children. 
}

void quadtree::prune(const int tol) {
    pruneHelper(tol, root);
}

void quadtree::pruneHelper(const int tol, Node* u){
    if(u->dim == 0){return;} //if reached the end, terminate. 
    if (prunable(u, tol)){ //if prunable, prune and terminate. 
        clearHelper(u->NW); u->NW = nullptr;
        clearHelper(u->NE); u->NE = nullptr;
        clearHelper(u->SE); u->SE = nullptr;
        clearHelper(u->SW); u->SW = nullptr;
        return;
    }//else continue to children nodes.
    pruneHelper(tol, u->NW);
    pruneHelper(tol, u->NE);
    pruneHelper(tol, u->SE);
    pruneHelper(tol, u->SW);
}

void quadtree::clear() {
    clearHelper(root);
}

void quadtree::clearHelper(Node* u){
    if(u->NW != nullptr){
        //delete children.
        clearHelper(u->NW);
        clearHelper(u->NE);
        clearHelper(u->SE);
        clearHelper(u->SW);
    }
    delete u;
    return;
}

void quadtree::copy(const quadtree& orig) {
    root = copyConstructor(orig.root);
    edge = orig.edge;
}

quadtree::Node* quadtree::copyConstructor(Node* replace){
    Node* point = new Node(replace->upLeft,replace->dim,replace->avg,replace->var);
    if (replace->NW != nullptr){
        //copy children if they exist
        point->NW = copyConstructor(replace->NW);
        point->NE = copyConstructor(replace->NE);
        point->SE = copyConstructor(replace->SE);
        point->SW = copyConstructor(replace->SW);
    }
    return point;
}
