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
pidController pid;
motorController shot1Motor;
motorController shot2Motor;
motorController clawMotor;
motorController upDownMotor;
vex::inertial brainInertial;
vex::touchled touchLed = vex::touchled(PORT2);
vex::colorsensor Color = colorsensor(PORT4);
vex::gyro Gyro = vex::gyro(PORT9);
vex::vision::signature Vision__DISC = vision::signature(1, 5349, 7747, 6548,-2313, -1607, -1960,2.5, 0);
vex::vision Vision3 = vex::vision(PORT3, 50, Vision__DISC);
float shotPower = 45;
float colorMid = 36 ;

void init(){
    brainInertial = vex::inertial();
    // brainInertial.resetRotation();
    // brainInertial.resetHeading();
 
    

    // 底盤設定
    chassis = chassisController(PORT12,PORT7,float3(0.3,0,0.05),float3(0.8,0,0.2),&Brain,&brainInertial,&Gyro);
    chassis.leftMotor.motor.setReversed(true);
    chassis.minPower = 12;
    chassis.startAccEnc = 50;
    chassis.endAccEnc = 350;
    chassis.allowErrorEnc = 10;

    shot1Motor = motorController(PORT5);
    shot2Motor = motorController(PORT11);
    clawMotor = motorController(PORT6);
    upDownMotor = motorController(PORT8);
    shot1Motor.motor.setReversed(true);
    upDownMotor.motor.setReversed(true);
    chassis.leftMotor.motor.setPosition(0, vex::rotationUnits::deg);
    chassis.rightMotor.motor.setPosition(0, vex::rotationUnits::deg);
    //.....
}


void printGyroDeg(){
    printf("Deg: %d\n", (int)Gyro.rotation(rotationUnits::deg));
}
void setUp(){
    chassis.leftMotor.motor.setMaxTorque(100, currentUnits::amp);
    chassis.rightMotor.motor.setMaxTorque(100, currentUnits::amp);

    Gyro.installed();
    Gyro.calibrate(gyroCalibrationType::calNormal, true);
    Gyro.setRotation(0, rotationUnits::deg);
    upDownMotor.on(-50);
    clawMotor.on(-50);
    vex::wait(1.5,vex::timeUnits::sec);
    upDownMotor.off();
    clawMotor.off();
    upDownMotor.motor.setPosition(70,vex::rotationUnits::deg);
    clawMotor.motor.setPosition(0 ,vex::rotationUnits::deg);
    upDownMotor.turnToPosition(100,270,false);
    clawMotor.turnToPosition(50,495, true);
    upDownMotor.off(true);
    Color.setLight(ledState::on);
    
}
void shot(int type){
    if (type == 1){
        chassis.on(70,70);
        upDownMotor.turnToPosition(100,80,true);
        upDownMotor.on(100);
        wait(1,timeUnits::sec);
        upDownMotor.turnToPosition(100,80,true);
        upDownMotor.on(100);
        wait(1,timeUnits::sec);
        upDownMotor.turnToPosition(100,200,true);
        clawMotor.turnToPosition(100,30,true);
        clawMotor.turnToPosition(100,500,true);
        int times = 0;
        while(times < 3){
        upDownMotor.on(100);
        wait(1,timeUnits::sec);
        upDownMotor.turnToPosition(100,80,true);
        times += 1;
        }
        shot1Motor.on((shotPower + 5));
        shot2Motor.on((shotPower + 5));
        upDownMotor.turnToPosition(100,240,true);
        clawMotor.turnToPosition(100,144,true);
        upDownMotor.on(100);
        wait(0.5,timeUnits::sec);
        clawMotor.turnToPosition(100,500,true);
        times = 0;
        while(times < 3){
            upDownMotor.on(100);
            wait(1,timeUnits::sec);
            upDownMotor.turnToPosition(100,80,true);       
            times += 1;
        }
    }
    else{
        upDownMotor.turnToPosition(100,80,true);
        upDownMotor.on(100);
        wait(1,timeUnits::sec);
        upDownMotor.turnToPosition(100,80,true);
        upDownMotor.on(100);
        wait(1,timeUnits::sec);

        clawMotor.turnToPosition(100,30,true);
        clawMotor.turnToPosition(100,500,true);
        shot1Motor.on((shotPower + 5));
        shot2Motor.on((shotPower + 5));
        int times = 0;
        while(times < 3){
        upDownMotor.on(100);
        wait(1,timeUnits::sec);
        upDownMotor.turnToPosition(100,80,true);
        times += 1;
        }
    }
    shot1Motor.off();
    shot2Motor.off();
    clawMotor.turnToPosition(100,495,false);
    upDownMotor.turnToPosition(80,270,true);
    chassis.off(false);
     
    
}


void yellowTower(float time){
    float power = -70;
    pid.reset();
    float leftStartPos = chassis.leftMotor.motor.position(vex::rotationUnits::deg);
    float rightStartPos = chassis.rightMotor.motor.position(vex::rotationUnits::deg);
    float nowEnc = 0;
    float leftEnc;
    float rightEnc;
    float error = 0;
    float fixValue;
    float nowPower;
    Brain.Timer.reset();
    chassis.startAccEnc = 200;
    chassis.endAccEnc = 0;
    while (Brain.Timer.value() < time)
    {   
        leftEnc = abs(chassis.leftMotor.motor.position(vex::rotationUnits::deg) - leftStartPos);
        rightEnc = abs(chassis.rightMotor.motor.position(vex::rotationUnits::deg) - rightStartPos);
        nowEnc = ((leftEnc + rightEnc) / 2);
        error = leftEnc - rightEnc;
        if (nowEnc >= 320){
            upDownMotor.on(100);
        }
        if (nowEnc <= chassis.startAccEnc)
            nowPower = (accCurve::upCurve((nowEnc / chassis.startAccEnc)) * power);
        else 
            nowPower = power;
        if(nowPower > -25)
            nowPower = -25;
        pid.update(error, nowPower);
        fixValue = pid.fixValue;
        if (power > 0)
            fixValue *= -1;

        chassis.leftMotor.on(nowPower + fixValue);
        chassis.rightMotor.on(nowPower - fixValue);
        vex::wait(50, vex::timeUnits::msec);
    }
    upDownMotor.turnToPosition(100, 200, true);
    chassis.off(true);
    wait(0.3, timeUnits::sec);
}

void purpleTower(float time, float p, float i, float d, float basicPower){

    float3 pid = float3( p ,i, d);
    float error;
    float totalError;
    float lastError;
    float fix;
    Brain.Timer.reset();
    while (Brain.Timer.value() < time)
    {
        Vision3.takeSnapshot(Vision__DISC);
        if (Vision3.objectCount > 0){
           error = ((double)Vision3.largestObject.centerX - 158);
        }
        else
            error = 0;
        printf("%d \n", (int)error);
        error *= -1;
        fix = (pid * float3(error, totalError, (error - lastError))).sum();
        chassis.on(basicPower-fix, basicPower+fix);
        lastError = error;
        vex::wait(10, timeUnits::msec);
    }
}

void extend(){
    // chassis.turnEnc(50,80);
    chassis.turnGyro(-50);
    chassis.arcMove(70,1.7,270);
    chassis.onForTime(100, 1, false);
    clawMotor.on(100);
    chassis.on(100,100);
 }

// while(1){
//     Color.setLight(ledState::on);
//     printf("Deg: %d\n", (int)Color.brightness());
//     wait(10,timeUnits::msec);
// }
void shake(int times){
    int time = 0;
    while(time < times){
        upDownMotor.turnToPosition(100, 100, true);
        upDownMotor.turnToPosition(100, 300, true);
        time += 1;
    }
    upDownMotor.turnToPosition(100, 260, false);
}

int main() {
    // main
    init();
    // purpleTower(10, 0.5, 0, 0.05, 50);
    setUp();
    shotPower = 50;
    Gyro.setRotation(180, vex::rotationUnits::deg);
    upDownMotor.turnToPosition(80,220,true);
    touchLed.on(vex::color::green);
    while(!touchLed.pressing()){}
    yellowTower(2.1);

    chassis.encMoveAcc(190 , 50);
    upDownMotor.turnToPosition(100,240,false);
    chassis.turnGyro(45);

    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.encMoveAcc(600, 60);
    upDownMotor.turnToPosition(100,200,true);
    upDownMotor.on(0);
    chassis.turnGyro(0);
    chassis.onForTime(-65,1.1,false);
    chassis.on(-80,-80);
    clawMotor.on(100);
    wait(1,timeUnits::sec); 
    clawMotor.turnToPosition(100,525,true);
    chassis.encMove(100,100,false);
    chassis.encMove(60,30,true);
    shot1Motor.on(100);
    shot2Motor.on(100);
    upDownMotor.turnToPosition(100,250,false);
    
    clawMotor.turnToPosition(100,550,false);
    chassis.turnGyro(130);
    chassis.encMove(150, 50, true);
    // chassis.turnGyro(95);
    chassis.turnEnc(70, 80);
    // chassis.on(50, -50);
    // wait(1, timeUnits::sec);
    // chassis.onForTime(70, 2, false);
    purpleTower(5, 0.5, 0, 0.05, 20);



    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    chassis.encMove(-50,60, true);
    chassis.turnEnc(-50, 60);

    chassis.arcMove(-70,-2.5,250);
    chassis.arcMove(-70,2.1,330);
    chassis.turnEnc(90,175);
    shot(1);





    shotPower -= 7;
    chassis.encMove(60,-50,true);
    chassis.turnGyro(-99);
    chassis.onForTime(50,0.5,true);
    chassis.onForTime(90,1.2,true);
    chassis.onForTime(20,1,true);
    Gyro.setRotation(-90, rotationUnits::deg);
    chassis.encMoveAcc(260,-70);
    chassis.turnGyro(0);
    upDownMotor.turnToPosition(100, 320, false);

    chassis.endAccEnc = 0;
    chassis.encMoveAcc(300,-60);
    chassis.endAccEnc = 200;
    chassis.onForTime(-60,0.3,true);
    chassis.on(-60,-60);
    clawMotor.on(100);
    wait(1,timeUnits::sec);
    clawMotor.turnToPosition(100,515,true);
    chassis.encMove(95,60,true);
    shot1Motor.on(100);
    shot2Motor.on(100);
    upDownMotor.turnToPosition(100,250,false);
    chassis.turnGyro(-95);
    clawMotor.turnToPosition(100,550,false);
    purpleTower(5,0.5, 0, 0.05,50);
    
    upDownMotor.turnToPosition(100, 270, false);
    chassis.encMove(180, -50, true);
    chassis.turnGyro(0);
    chassis.onForTime(60, 2, true);
    shot(2);
    chassis.arcMove(-50, -2, 250);

    extend();
    

 }
 



