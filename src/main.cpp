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

vex::brain Brain;
chassisController chassis;
motorController shot1Motor;
motorController shot2Motor;
motorController clawMotor;
motorController upDownMotor;
vex::inertial brainInertial = vex::inertial();

void init(){
    // 底盤設定
    chassis = chassisController(PORT12,PORT7,float3(1,0,0.1),float3(3,0,0.2),&Brain,&brainInertial);
    chassis.leftMotor.motor.setReversed(true);
    chassis.minPower = 30;
    chassis.startAccEnc = 50;
    chassis.endAccEnc = 170;
    chassis.allowErrorEnc = 10;

    shot1Motor = motorController(PORT5);
    shot2Motor = motorController(PORT11);
    clawMotor = motorController(PORT6);
    upDownMotor = motorController(PORT8);
    //.....
}

void printGyroDeg(){
    printf("Deg: %d\n", (int)brainInertial.rotation(vex::rotationUnits::deg));
}
int main() {
    init();
    brainInertial.calibrate();
    brainInertial.resetHeading();
    brainInertial.resetRotation();
    chassis.turnGyro(-50);
    
   
}

