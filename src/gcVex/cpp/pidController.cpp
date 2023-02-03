#include "..\mathReleated\pidController.h"

#pragma region - Constructor and Destructor

/// @brief Default construct with all val = 0
pidController::pidController(){
    lastError = 0;
    totalError = 0;
}
pidController::pidController(float3 _minPid,float3 _maxPid){
    minPid = _minPid;
    maxPid = _maxPid;

}


pidController::~pidController(){

}

#pragma endregion
void pidController::setMinPID(float3 newMinPid){
    minPid = newMinPid;
}
void pidController::setMaxPID(float3 newMaxPid){
    maxPid = newMaxPid;
}
float3 pidController::calPID(float nowPower){
    return minPid +((maxPid-minPid)*(nowPower/100.0));
}

/// @brief give error to calculate next fixValue with last input power
float pidController::update(float error){
    return pidController::update(error, lastPower);
}
/// @brief give error to calculate next fixValue
float pidController::update(float error, float power){
    lastPower = power;
    totalError += error;
    fixValue = (float3(error, totalError, lastError-error) * pidController::calPID(power)).sum();
    lastError = error;

    return fixValue;
}

void pidController::reset(){
    lastError = 0;
    totalError = 0;
}