#include <math.h>
#include "..\mathReleated\accCurve.h"

/// @brief input x between 0~1
/// @param dir up as 0 to 1, down as 1 to 0
/// @return y = x^pow
float accCurve::powerX(float x, float pow, curveDirection dir){
    float output = std::pow(x, pow);
    if( dir == curveDirection::down )
        output = 1.0 - output;
    
    return output;
}

float accCurve::upCurve(float x){
    return accCurve::powerX(x, 0.4f, curveDirection::up);
}

float accCurve::downCurve(float x){
    return accCurve::powerX(x, 0.2f, curveDirection::down);
}
