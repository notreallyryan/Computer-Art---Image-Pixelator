
#include "stats.h"

stats::stats(PNG& im) {
    for (unsigned i = 0; i <im.width(); i++){
        //create the temporary pixel value vectors for the column and their squared equivalents
        vector<long> tempr;
        vector<long> tempg;
        vector<long> tempb;
        vector<long> tempsqr;
        vector<long> tempsqg;
        vector<long> tempsqb;

        //access each cell in the column
        for(unsigned j = 0; j<im.height(); j++){
            RGBAPixel* pixel = im.getPixel(i,j);
            long r = pixel->r;
            long g = pixel->g;
            long b = pixel->b;
            long sqr = pow(pixel->r, 2);
            long sqg = pow(pixel->g, 2);
            long sqb = pow(pixel->b, 2);

            //mathematically, the sum of all pixel values in a rectangle from (0,0) to (i,j) is
            //the pixel value sum the rectangles from (0,0) to (i-1,j) and (0,0) to (i, j-1)
            //minus the rectangle from (0,0) to (i-1, j-1)
            if (i != 0){
                r += sumRed[i-1][j];
                g += sumGreen[i-1][j];
                b += sumBlue[i-1][j];
                sqr += sumsqRed[i-1][j];
                sqg += sumsqGreen[i-1][j];
                sqb += sumsqBlue[i-1][j];
            }
            if (j != 0){
                r += tempr[j-1];
                g += tempg[j-1];
                b += tempb[j-1];
                sqr += tempsqr[j-1];
                sqg += tempsqg[j-1];
                sqb += tempsqb[j-1];
            }
            if(j!=0 && i!=0){
                r -= sumRed[i-1][j-1];
                g -= sumGreen[i-1][j-1];
                b -= sumBlue[i-1][j-1];
                sqr -= sumsqRed[i-1][j-1];
                sqg -= sumsqGreen[i-1][j-1];
                sqb -= sumsqBlue[i-1][j-1];
            }
            //push onto temporary vector
            tempr.push_back(r);
            tempg.push_back(g);
            tempb.push_back(b);
            tempsqr.push_back(sqr);
            tempsqg.push_back(sqg);
            tempsqb.push_back(sqb);
        }
        //pus vector onto permenant vector to create matrix
        sumRed.push_back(tempr);
        sumGreen.push_back(tempg);
        sumBlue.push_back(tempb);
        sumsqRed.push_back(tempsqr);
        sumsqGreen.push_back(tempsqg);
        sumsqBlue.push_back(tempsqb);
    }
}

long stats::getSum(char channel, pair<int, int> ul, int dim) {
    //get correct channel
    vector<vector<long>>* toUse; 
    switch(channel) {
        case 'r':
            toUse = &sumRed;
            break;
        case 'g':
            toUse = &sumGreen;
            break;
        case 'b':
            toUse = &sumBlue;
            break;
    }
    //get bottom right coordinates of square
    int newx = ul.first + pow(2, dim)-1;
    int newy = ul.second+ pow(2, dim)-1;

    //get value from sum table, and subtract excess for cases where left corner is not (0,0)
    long value = (*toUse)[newx][newy];
    if (ul.first != 0){value -= (*toUse)[(int)ul.first-1][newy];}
    if (ul.second != 0){value -= (*toUse)[newx][(int)ul.second-1];}
    if (ul.first != 0 && ul.second != 0){value += (*toUse)[(int)ul.first-1][(int)ul.second-1];}
    return value;
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    //get correct channel
    vector<vector<long>>* toUse; 
    switch(channel) {
        case 'r':
            toUse = &sumsqRed;
            break;
        case 'g':
            toUse = &sumsqGreen;
            break;
        case 'b':
            toUse = &sumsqBlue;
            break;
    }
    //get bottom right coordinates of square
    int newx = ul.first + pow(2, dim)-1;
    int newy = ul.second+ pow(2, dim)-1;

    //get value from sum table, and subtract excess for cases where left corner is not (0,0)
    long value = (*toUse)[newx][newy];
    if (ul.first != 0){value -= (*toUse)[(int)ul.first-1][newy];}
    if (ul.second != 0){value -= (*toUse)[newx][(int)ul.second-1];}
    if (ul.first != 0 && ul.second != 0){value += (*toUse)[(int)ul.first-1][(int)ul.second-1];}
    return value;
}

long stats::rectArea(int dim) {
    //calculate and return no units in square of given dim.
    return pow(pow(2, dim), 2);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    double value = 0;

    //summation from mathematical simplification
    value += (double)getSumSq('r', ul, dim) - (pow(getSum('r', ul, dim),2))/(double)rectArea(dim);
    value += (double)getSumSq('g', ul, dim) - (pow(getSum('g', ul, dim),2))/(double)rectArea(dim);
    value += (double)getSumSq('b', ul, dim) - (pow(getSum('b', ul, dim),2))/(double)rectArea(dim);
    return value;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    //make pixel to return 
    RGBAPixel avg = RGBAPixel();

    //get and set averages
    avg.r = getSum('r',ul, dim)/rectArea(dim);
    avg.g = getSum('g',ul, dim)/rectArea(dim);
    avg.b = getSum('b',ul, dim)/rectArea(dim);
    return avg;
}