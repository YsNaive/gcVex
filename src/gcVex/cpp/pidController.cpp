#include "..\mathReleated\pidController.h"

#pragma region - Constructor and Destructor

/// @brief Default construct with all val = 0
pidController::pidController(){
    lastError = 0;
    totalError = 0;
}

pidController::pidController(float3 newPID){
    pid = newPID;
}

pidController::~pidController(){

}

#pragma endregion

void pidController::setPID(float3 newPID){
    pid = newPID;
}

/// @brief give error to calculate next fixValue
float pidController::update(float error){
    totalError += error;
    fixValue = (error * pid.x) + (totalError * pid.y) + ((lastError - error) * pid.z);
    lastError = error;

    return fixValue;
}