#include "pidController.h"

#pragma region /* Constructor and Destructor */

/// @brief Default construct with all val = 0
pidController::pidController(){
    error = 0;
    errorLast = 0;
    errorTotal = 0;
}

pidController::~pidController(){

}

#pragma endregion

void pidController::setPID(float3 newPID){
    pid = newPID;
}