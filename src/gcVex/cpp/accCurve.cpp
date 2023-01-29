#include <math.h>
#include "..\mathReleated\accCurve.h"

/// @brief input x between 0~1
/// @param dir up as 0 to 1, down as 1 to 0
/// @return y = x^pow
float powerX(float x, float pow, curveDirection dir){
    float output = std::pow(x, pow);
    if( dir == curveDirection::down )
        output = 1.0 - output;
    
    return output;
}