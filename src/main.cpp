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
vex::colorsensor Color = colorsensor(PORT4);
float shotPower = 45;

void init(){
    brainInertial = vex::inertial();
    brainInertial.resetRotation();
    brainInertial.resetHeading();

    // 底盤設定
    chassis = chassisController(PORT12,PORT7,float3(0.3,0,0.1),float3(0.5,0,0.15),&Brain,&brainInertial);
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
    upDownMotor.motor.setPosition(10,vex::rotationUnits::deg);
    clawMotor.motor.setPosition(0,vex::rotationUnits::deg);
    upDownMotor.turnToPosition(100,270,false);
    clawMotor.turnToPosition(50,495, true);
    upDownMotor.off(true);
    Color.setLight(ledState::on);
}
void shot(bool isReset){
    upDownMotor.turnToPosition(80,140,true);
    upDownMotor.on(100);
    wait(1,timeUnits::sec);
    upDownMotor.turnToPosition(80,140,true);
    upDownMotor.on(100);
    wait(1,timeUnits::sec);
    upDownMotor.turnToPosition(80,140,true);
    // clawMotor.turnToPosition(100,0,false);
    clawMotor.on(-100);
    if(isReset){
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
    else{
        Brain.resetTimer();
        while(Brain.timer(vex::timeUnits::sec) < 6){
        upDownMotor.on(100);
        wait(1,timeUnits::sec);
        upDownMotor.turnToPosition(80,140,true);
        clawMotor.turnToPosition(100,300,false);
     
        }
        shot1Motor.off();
        shot2Motor.off();
        clawMotor.turnToPosition(100,495,false);
        upDownMotor.turnToPosition(80,270,true);
        chassis.off(false);
    }
}
void purpleTower(float time, float p, float d){

    int colorMax = 52, colorMin = 10;

    float colorMid =( (colorMax + colorMin)/2.0) + 5;

    float3 pid = float3( p ,0, d);
    float3 error = float3(0 ,0, 0);
    int fix;
    Brain.Timer.reset();
    while (Brain.Timer.value() < time)
    {
        error.x = Color.brightness() - colorMid;
        error.y += error.x;
        error.y -= error.x;

        fix = (pid * error).sum();
        chassis.on(18-fix, 18+fix);
        // printf("%d\n", color.brightness());

        error.z = error.x;

        vex::wait(10, timeUnits::msec);
    }
}

void leftPurpleBlue(){
    shotPower = 56.5;
    // 1.0
    // chassis.encMove(180,50,false);
    // shot1Motor.on(100);
    // shot2Motor.on(100);
    // chassis.arcMove(80,2.58,455);
    // chassis.turnGyro(-90);
    // chassis.on(80,72);
    // wait(5,vex::timeUnits::sec);


    //2.0

    chassis.encMove(220,50,false);
    chassis.arcMove(70,2.7,310);
    shot1Motor.on(100);
    shot2Motor.on(100);
    purpleTower(2.7, 0.1, 0.02);
    chassis.on(50,50);
    wait(2.5,vex::timeUnits::sec);

    shot1Motor.off();
    shot2Motor.off();
    chassis.arcMove(-60,-30,60);
    chassis.encMove(70,-30,true);
    upDownMotor.turnToPosition(100,290,false);
    chassis.turnGyro(-1);
    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.onForTime(-95,1,false);
    clawMotor.on(100);
    wait(1.5,timeUnits::sec);
    clawMotor.turnToPosition(100,507,true);
    chassis.arcMove(70,-1.5,150);
    // chassis.arcMove(90,1.6,195);
    // chassis.on(50,50);

    shot(false);
}
void rightPurpleBlue(){
    shotPower = 58;
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
    chassis.encMoveAcc(210 , 60);
    upDownMotor.turnToPosition(100,270,false);
    chassis.turnGyro(48);

    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.encMoveAcc(600 , 70);
    chassis.turnGyro(0);
    chassis.onForTime(-50,0.7,false);
    chassis.on(-80,-80);
    clawMotor.on(100);
    wait(1,timeUnits::sec);
    clawMotor.turnToPosition(100,507,true);

    chassis.arcMove(80,-1.35,95);

    upDownMotor.on(100);
    wait(1.1,timeUnits::sec);
    upDownMotor.turnToPosition(100,110,true);
    upDownMotor.on(80);
    wait(1.1,timeUnits::sec);
    upDownMotor.turnToPosition(100,110,true);
    upDownMotor.on(80);
    
    // upDownMotor.turnToPosition(100,140,true);
    // upDownMotor.on(100);
    // wait(1.1,timeUnits::sec);
    upDownMotor.turnToPosition(100,405,false);


    //chassis.turnGyro(94.5);
    chassis.turnEnc(50,100);
    while(Color.brightness() > 30){
     chassis.on(20,-20);
    }
    chassis.off(true);
    // chassis.leftMotor.onEnc(-30, 10, true);
    // chassis.leftMotor.off(true);
    chassis.turnEnc(-30,10);
    //wait(1,timeUnits::sec);

    upDownMotor.turnToPosition(100,270,false);
    //purpleTower(8, 0.4, 0.02);
    shot1Motor.on(100);
    shot2Motor.on(100);
    purpleTower(1.7, 0.15, 0.02);
    chassis.on(50,50);
    wait(3,vex::timeUnits::sec);

    shotPower = 52;
    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.arcMove(-60,1.5,190);
    //chassis.turnGyro(0);
    chassis.turnEnc(-50,290);
    shotPower = 50;
    shot(true);
    touchLed.off();
    wait(1,vex::timeUnits::sec); 
 }

 void extend(){
    chassis.turnEnc(-50,80);
    chassis.encMove(220,70,false);
    chassis.arcMove(80, -3.2, 90);
    clawMotor.on(100);
    chassis.on(100,100);
 }

int main() {
    init();
    // Color.setLight(ledState::on);
    // purpleTower(10);

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

    extend();
 }



