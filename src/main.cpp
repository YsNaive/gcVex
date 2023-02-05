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

void printGyroDeg(){
    printf("Deg: %d\n", (int)brainInertial.rotation(vex::rotationUnits::deg));
}
void setUp(){
    upDownMotor.on(-50);
    clawMotor.on(-50);
    vex::wait(1.5,vex::timeUnits::sec);
    upDownMotor.off();
    clawMotor.off();
    upDownMotor.motor.setPosition(0,vex::rotationUnits::deg);
    clawMotor.motor.setPosition(0,vex::rotationUnits::deg);
    upDownMotor.turnToPosition(100,270,false);
    clawMotor.turnToPosition(50,495, true);
    upDownMotor.off(true);
}
void shot(){
    upDownMotor.turnToPosition(80,140,true);
    upDownMotor.on(100);
    wait(1,timeUnits::sec);
    upDownMotor.turnToPosition(80,140,true);
    upDownMotor.on(100);
    wait(1,timeUnits::sec);
    upDownMotor.turnToPosition(80,140,true);
    clawMotor.turnToPosition(100,0,false);

    chassis.off(false);
    while(!touchLed.pressing()){
        upDownMotor.on(100);
        wait(0.7,timeUnits::sec);
        upDownMotor.turnToPosition(80,140,true);
        clawMotor.turnToPosition(100,300,false);
    }
    shot1Motor.off();
    shot2Motor.off();
    clawMotor.turnToPosition(100,495,false);
    upDownMotor.turnToPosition(80,270,true);
    chassis.off(false);


}
void leftPurpleBlue(){

    Brain.playSound(vex::soundType::alarm);
    chassis.encMove(195,50,false);
    chassis.arcMove(80,2.4,415);
    shot1Motor.on(100);
    shot2Motor.on(100);
    //chassis.arcMove(60,-1.06,100);
    chassis.turnGyro(-90);
    chassis.on(100,60);
    wait(4.5,vex::timeUnits::sec);
    shot1Motor.off();
    shot2Motor.off();
    chassis.arcMove(-50,-1.7,80);
    chassis.turnGyro(1);
    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.onForTime(-95,1,false);
    clawMotor.on(100);
    wait(2,timeUnits::sec);
    clawMotor.turnToPosition(100,507,true);
    chassis.arcMove(90,-1.45,170);
    chassis.arcMove(90,1.4,165);
    shot();
    touchLed.off();
    wait(1,vex::timeUnits::sec);
}
void rightPurpleBlue(){
    shotPower = 45;
    chassis.leftMotor.motor.setPosition(0, vex::rotationUnits::deg);
    chassis.rightMotor.motor.setPosition(0, vex::rotationUnits::deg);
    Brain.playSound(vex::soundType::alarm);
    chassis.encMove(100,-40,false);
    chassis.onForTime(-60,1.3,true);
    upDownMotor.on(100);
    wait(0.4,vex::timeUnits::sec);
    //chassis.on(-100,-100);
    chassis.off(false);
    upDownMotor.turnToPosition(100,185,true);
    wait(0.3,vex::timeUnits::sec);
    chassis.encMoveAcc(220 , 70);
    upDownMotor.turnToPosition(100,290,false);
    chassis.turnGyro(45);
    upDownMotor.turnToPosition(100,235,false);
    chassis.encMove(190,50,false);
    upDownMotor.turnToPosition(100,290,false);
    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.arcMove(70,1.4,460);
    chassis.on(70,70);

    upDownMotor.on(100);
    wait(1.1,timeUnits::sec);
    upDownMotor.turnToPosition(100,140,true);
    upDownMotor.on(100);
    wait(1.1,timeUnits::sec);
    upDownMotor.turnToPosition(100,140,true);
    upDownMotor.on(100);
    wait(1.1,timeUnits::sec);
    // upDownMotor.turnToPosition(100,140,true);
    // upDownMotor.on(100);
    // wait(1.1,timeUnits::sec);


    upDownMotor.turnToPosition(100,410,false);
    chassis.encMoveAcc(325,-70);
    chassis.turnGyro(109);
    shot1Motor.on(100);
    shot2Motor.on(100);
    upDownMotor.turnToPosition(100,270,false);
    chassis.onForTime(90,1.5,true);
    chassis.leftMotor.off(false);
    chassis.rightMotor.on(100);
    wait(1,vex::timeUnits::sec);
    chassis.rightMotor.off(true);
    //chassis.on(70,70);
    wait(2.5,vex::timeUnits::sec);
    chassis.off(true);
    chassis.encMove(100,-50, true);
    chassis.turnGyro(1);
    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.onForTime(-95,1,false);
    clawMotor.on(100);
    wait(2,timeUnits::sec);
    clawMotor.turnToPosition(100,507,true);
    chassis.encMove(100, 50, false);
    chassis.arcMove(90,1.6,190);
    chassis.arcMove(60,-1.3,160);
    shotPower = 60;
    shot();
    touchLed.off();
    wait(1,vex::timeUnits::sec); 
 }

int main() {
    init();
    setUp();
    brainInertial.setRotation(180, vex::rotationUnits::deg);
    upDownMotor.turnToPosition(80,225,true);
    touchLed.on(vex::color::green);
    while(!touchLed.pressing()){}
    rightPurpleBlue();
    while(!touchLed.pressing()){}
    touchLed.on(vex::color::green);
    brainInertial.resetRotation();
    brainInertial.resetHeading();
    brainInertial.setRotation(0, vex::rotationUnits::deg);
    leftPurpleBlue();  
 }

