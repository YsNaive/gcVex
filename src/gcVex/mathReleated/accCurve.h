#ifndef GCVEX_ACCCURVE_H_
#define GCVEX_ACCCURVE_H_

class accCurve
{
private:
    accCurve();
    ~accCurve();

public:
    // calculate curve of y = x^pow
    float powerX(float x, float pow, curveDirection dir); 
};

enum curveDirection{
    up,
    down
};

#endif