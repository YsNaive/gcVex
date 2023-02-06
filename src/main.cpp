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
vex::inertial brainInertial;
vex::touchled touchLed = vex::touchled(PORT2);
float shotPower = 55;

void init(){
    brainInertial = vex::inertial();
    brainInertial.resetRotation();
    brainInertial.resetHeading();

    // 底盤設定
    chassis = chassisController(PORT12,PORT7,float3(0.8,0,0.2),float3(1.6,0,0.2),&Brain,&brainInertial);
    chassis.leftMotor.motor.setReversed(true);
    chassis.minPower = 30;
    chassis.startAccEnc = 50;
    chassis.endAccEnc = 170;
    chassis.allowErrorEnc = 10;

    shot1Motor = motorController(PORT5);
    shot2Motor = motorController(PORT11);
    clawMotor = motorController(PORT6);
    upDownMotor = motorController(PORT8);
    upDownMotor.motor.setReversed(true);
    shot1Motor.motor.setReversed(true);
    chassis.leftMotor.motor.setPosition(0, vex::rotationUnits::deg);
    chassis.rightMotor.motor.setPosition(0, vex::rotationUnits::deg);
    //.....
}

int main() {
    init();

    colorsensor color = colorsensor(PORT4);
    color.setLight(ledState::on);
    int colorMax = 52, colorMin = 13;
    pidController controller;
    controller = pidController(float3(1, 0, 0.4), float3(0.4, 0, 0.2));

    float colorMid = (colorMax + colorMin)/2.0;

    float3 pid = float3(0.2 ,0, 0.08);
    float3 error = float3(0 ,0, 0);
    int fix;
    shot1Motor.on(100);
    shot2Motor.on(100);
    while (true)
    {
        error.x = color.brightness() - colorMid;
        error.y += error.x;
        error.y -= error.x;

        fix = (pid * error).sum();
        chassis.on(18-fix, 18+fix);
        // printf("%d\n", color.brightness());

        error.z = error.x;
        vex::wait(25, timeUnits::msec);
    }
    
}

