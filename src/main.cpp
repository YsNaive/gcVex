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
float colorMid = 36 ;

void init(){
    brainInertial = vex::inertial();
    brainInertial.resetRotation();
    brainInertial.resetHeading();

    // 底盤設定
    chassis = chassisController(PORT12,PORT7,float3(0.2,0,0.05),float3(0.4,0,0.1),&Brain,&brainInertial);
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
    clawMotor.motor.setPosition(0 ,vex::rotationUnits::deg);
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
        // Brain.resetTimer();
        int times = 0;
        while(times < 3){
        upDownMotor.on(100);
        wait(1,timeUnits::sec);
        upDownMotor.turnToPosition(80,140,true);
        clawMotor.turnToPosition(100,300,false);
        times += 1;
        }
        shot1Motor.off();
        shot2Motor.off();
        clawMotor.turnToPosition(100,495,false);
        upDownMotor.turnToPosition(80,270,true);
        chassis.off(false);
     
    }
}
void purpleTower(float time, float p, float d){

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
    shotPower = 53;
    // 1.0
    // chassis.encMove(180,50,false);
    // shot1Motor.on(100);
    // shot2Motor.on(100);
    // chassis.arcMove(80,2.58,455);
    // chassis.turnGyro(-90);
    // chassis.on(80,72);
    // wait(5,vex::timeUnits::sec);


    //2.0

    chassis.encMove(250,50,false);
    shot1Motor.on(100);
    shot2Motor.on(100);

    chassis.arcMove(55,2.72,305);
    
    colorMid = 32 ;
    purpleTower(1, 0.15, 0.02);
    colorMid = 31 ;
    purpleTower(1.7, 0.1, 0.01);
    chassis.on(50,50);
    wait(2.5,vex::timeUnits::sec);

    shot1Motor.off();
    shot2Motor.off();
    chassis.arcMove(-60,-27,55);
    chassis.encMove(82,-30,true);
    upDownMotor.turnToPosition(100,290,false);
    upDownMotor.off(true);
    chassis.turnGyro(0);
    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.onForTime(-95,1,false);
    clawMotor.on(100);
    wait(1.5,timeUnits::sec);
    clawMotor.turnToPosition(100,507,true);
    chassis.arcMove(70,-1.62,150);
    // chassis.arcMove(90,1.6,195);
    // chassis.on(50,50);

    shot(false);
}
void rightPurpleBlue(){
    shotPower = 45;
    chassis.leftMotor.motor.setPosition(0, vex::rotationUnits::deg);
    chassis.rightMotor.motor.setPosition(0, vex::rotationUnits::deg);


    chassis.encMove(100,-40,false);
    chassis.onForTime(-60,1,true);
    upDownMotor.on(100);
    wait(1.1,vex::timeUnits::sec);
    //chassis.on(-100,-100);
    chassis.off(false);
    upDownMotor.turnToPosition(100,185,true);
    // wait(0.3,vex::timeUnits::sec);
    chassis.encMoveAcc(190 , 60);
    upDownMotor.turnToPosition(100,275,false);

    chassis.turnGyro(42);

    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.encMoveAcc(595, 70);
    chassis.turnGyro(0);
    chassis.onForTime(-60,0.8,false);
    chassis.on(-80,-80);
    clawMotor.on(100);
    wait(1,timeUnits::sec);
    clawMotor.turnToPosition(100,507,true);

    chassis.arcMove(80,1.7,118);

    upDownMotor.on(100);
    wait(1.1,timeUnits::sec);
    upDownMotor.turnToPosition(100,110,true);
    upDownMotor.on(80);
    wait(1.1,timeUnits::sec);
    upDownMotor.turnToPosition(100,110,true);
    upDownMotor.on(80);
    
    upDownMotor.turnToPosition(100,140,true);
    upDownMotor.on(100);
    wait(1.1,timeUnits::sec);

    upDownMotor.turnToPosition(100,270,false); //


    //chassis.turnGyro(94.5);
    chassis.turnEnc(70,150);
    while(Color.brightness() > 30){
     chassis.on(20,-20);
    }
    chassis.off(true);
    upDownMotor.turnToPosition(100,270,false);
    // chassis.leftMotor.onEnc(-30, 10, true);
    // chassis.leftMotor.off(true);
    chassis.turnEnc(-30,10);
    //wait(1,timeUnits::sec);

    // upDownMotor.turnToPosition(100,270,false);
    //purpleTower(8, 0.4, 0.02);
    shot1Motor.on(100);
    shot2Motor.on(100);
    colorMid = 30.5 ;
    purpleTower(1.7, 0.3, 0.05);
    chassis.on(100,100);
    wait(3,vex::timeUnits::sec);

    shotPower = 57;
    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.arcMove(-60,1.5,140);
    //chassis.turnGyro(0);
    upDownMotor.turnToPosition(100,300,false);
    chassis.turnEnc(-70,317);
    
    shot(true);
    touchLed.off();
    wait(1,vex::timeUnits::sec); 
 }

 void extend(){
    chassis.turnEnc(-70,80);
    chassis.encMove(235,70,false);
    clawMotor.on(100);
    chassis.arcMove(90, -3.6, 90);
   
    chassis.on(100,100);
 }

// while(1){
//     Color.setLight(ledState::on);
//     printf("Deg: %d\n", (int)Color.brightness());
//     wait(10,timeUnits::msec);
// }

int main() {

    // main

    init();
    Color.setLight(ledState::on);
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

    // //right blue and purple

    // while(!touchLed.pressing()){}
    // upDownMotor.motor.setPosition(270,vex::rotationUnits::deg);
    // clawMotor.motor.setPosition(470 ,vex::rotationUnits::deg);
    // chassis.encMove(125, -40, true); // rightTower() 裡面要改
    // rightPurpleBlue();
    // while(!touchLed.pressing()){}
    // touchLed.on(vex::color::green);
    // brainInertial.resetRotation();
    // brainInertial.resetHeading();
    // brainInertial.setRotation(0, vex::rotationUnits::deg);
    // leftPurpleBlue(); 

    // extend();


    // // left purple and blue

    // upDownMotor.motor.setPosition(270,vex::rotationUnits::deg);
    // clawMotor.motor.setPosition(470 ,vex::rotationUnits::deg);
    // upDownMotor.off(true);
    // Color.setLight(ledState::on);
    // while(!touchLed.pressing()){}
    // touchLed.on(vex::color::green);
    // brainInertial.resetRotation();
    // brainInertial.resetHeading();
    // brainInertial.setRotation(0, vex::rotationUnits::deg);
    // leftPurpleBlue(); 
    // extend();

    // // XXX Blue
    // init();
    // clawMotor.on(100);



 }



