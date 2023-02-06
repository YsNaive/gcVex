/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       chuwe                                                     */
/*    Created:      2023/1/29 下午1:48:21                                     */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// 基本函式庫
#include <stdio.h>
#include <vex.h>
#include <iq2_api.h>
#include "gcVex\gcVex.h"

using namespace vex;
using namespace gcVex;

vex::brain Brain;

int main() {
    motor leftMotor = motor(PORT12);
    motor rightMotor = motor(PORT7);
    leftMotor.resetPosition();
    rightMotor.resetPosition();
    leftMotor.spin(directionType::fwd);
    rightMotor.spin(directionType::fwd);
    // (float *f1)() = [&](void)->float{
    //         return leftMotor.position(rotationUnits::deg) - rightMotor.position(rotationUnits::deg);
    //     };
    // void (*f2)(float) = [&](float fix){
    //         leftMotor.setVelocity(40 + fix,percentUnits::pct);
    //         rightMotor.setVelocity(40 - fix,percentUnits::pct);
    //     };
    auto f1 = [&](void) -> float { return leftMotor.position(rotationUnits::deg) - rightMotor.position(rotationUnits::deg); };
    auto f2 = [&](float fix) -> void { 
        leftMotor.setVelocity(40 + fix,percentUnits::pct);
        rightMotor.setVelocity(40 - fix,percentUnits::pct);
        };

    pidController controller = pidController(
        float3(0.2, 0, 0.04),f1,f2
    );
    

    while ((true))
    {
        controller.update();
    }
    
}

