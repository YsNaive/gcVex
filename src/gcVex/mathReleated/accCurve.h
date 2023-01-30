#ifndef GCVEX_ACCCURVE_H_
#define GCVEX_ACCCURVE_H_

enum curveDirection{
    up,
    down
};

namespace accCurve
{
    // calculate curve of y = x^pow
    float powerX(float x, float pow, curveDirection dir); 
    float upCurve(float x);     // pow as 0.4
    float downCurve(float x);   // pow as 2.3
};

#endif