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
vex::colorsensor Color1 = colorsensor(PORT10);
vex::colorsensor Color2 = colorsensor(PORT1);
vex::gyro Gyro = vex::gyro(PORT9);
vex::vision::signature Vision__DISC = vision::signature(1, 7587, 8583, 8085, -2759, -2185, -2472, 2.8, 0);
vex::vision Vision3 = vex::vision(PORT3, 50, Vision__DISC);
float shotPower = 45;
float colorMid = 36;

void init()
{
    brainInertial = vex::inertial();
    // brainInertial.resetRotation();
    // brainInertial.resetHeading();

    // 底盤設定
    chassis = chassisController(PORT12, PORT7, float3(0.2, 0, 0.05), float3(0.6, 0, 0.1), &Brain, &brainInertial, &Gyro);
    chassis.leftMotor.motor.setReversed(true);
    chassis.minPower = 25;
    chassis.startAccEnc = 50;
    chassis.endAccEnc = 500;
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

void printGyroDeg()
{
    printf("Deg: %d\n", (int)Gyro.rotation(rotationUnits::deg));
}
void setUp()
{
    chassis.leftMotor.motor.setMaxTorque(100, currentUnits::amp);
    chassis.rightMotor.motor.setMaxTorque(100, currentUnits::amp);

    Gyro.installed();
    Gyro.calibrate(gyroCalibrationType::calNormal, true);
    Gyro.setRotation(0, rotationUnits::deg);
    upDownMotor.on(-50);
    clawMotor.on(-50);
    vex::wait(1.5, vex::timeUnits::sec);
    upDownMotor.off();
    clawMotor.off();
    upDownMotor.motor.setPosition(40, vex::rotationUnits::deg);
    clawMotor.motor.setPosition(0, vex::rotationUnits::deg);
    upDownMotor.turnToPosition(100, 270, false);
    clawMotor.turnToPosition(50, 490, true);
    upDownMotor.off(true);
}
void shot(int type)
{
    if (type == 1)
    {
        chassis.on(100, 100);
        int times = 0;
        upDownMotor.turnToPosition(100, 110, true);
        upDownMotor.on(100);
        wait(0.8, timeUnits::sec);
        // upDownMotor.turnToPosition(100, 110, true);
        // upDownMotor.on(100);
        // wait(0.75, timeUnits::sec);
        while (times < 1)
        {
            upDownMotor.turnToPosition(100, 160, true);
            upDownMotor.on(100);
            wait(1, timeUnits::sec);
            times += 1;
        }
        upDownMotor.turnToPosition(100, 110, true);
        clawMotor.turnToPosition(100, 320, true);
        clawMotor.turnToPosition(100, 500, false);
        upDownMotor.on(100);
        chassis.encMove(30, -100, true);
        chassis.on(100, 100);
        shotPower += 4;
        upDownMotor.turnToPosition(100, 110, true);
        times = 0;
        while (times < 2)
        {
            upDownMotor.on(100);
            // chassis.encMove(30, -100, true);
            // chassis.on(100, 100);
            wait(0.85, timeUnits::sec);
            upDownMotor.turnToPosition(100, 110, true);
            times += 1;
        }
        shot1Motor.on((shotPower + 3));
        shot2Motor.on((shotPower + 3));
        upDownMotor.turnToPosition(100, 200, true);
        clawMotor.on(-100);
        // wait(1,timeUnits::sec);
        chassis.encMove(30, -100, true);
        chassis.on(100, 100);
        wait(0.9, timeUnits::sec);
        clawMotor.turnToPosition(100, 500, false);
        upDownMotor.on(100);
        wait(0.7, timeUnits::sec);
        chassis.encMove(50, -100, true);
        chassis.on(100, 100);
        times = 0;
        while (times < 3)
        {
            upDownMotor.on(100);
            wait(0.75, timeUnits::sec);
            upDownMotor.turnToPosition(100, 110, true);
            times += 1;
        }
    }
    else
    {
        upDownMotor.on(100);
        wait(0.7, timeUnits::sec);
        upDownMotor.turnToPosition(100, 110, true);
        // chassis.encMove(80, -70, true);

        int times = 0;
        while (times < 2)
        {
            upDownMotor.on(100);
            wait(0.75, timeUnits::sec);
            upDownMotor.turnToPosition(100, 110, true);
            times += 1;
        }
        clawMotor.on(-100);
        chassis.encMove(30, -70, true);
        chassis.on(100, 100);
        wait(0.9, timeUnits::sec);
        upDownMotor.on(100);
        // chassis.encMove(95, -70, true);
        clawMotor.turnToPosition(100, 500, false);
        shot1Motor.on((shotPower + 5));
        shot2Motor.on((shotPower + 5));
        times = 0;
        while (times < 2)
        {
            upDownMotor.on(100);
            wait(0.75, timeUnits::sec);
            upDownMotor.turnToPosition(100, 110, true);
            times += 1;
        }
        shot1Motor.off();
        shot2Motor.off();
    }
    clawMotor.turnToPosition(100, 495, false);
    upDownMotor.turnToPosition(80, 270, false);
    chassis.off(false);
}

void yellowTower(float time)
{
    float power = -80;
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
        if (nowEnc >= 320)
        {
            upDownMotor.on(100);
        }
        if (nowEnc <= chassis.startAccEnc)
            nowPower = (accCurve::upCurve((nowEnc / chassis.startAccEnc)) * power);
        else
            nowPower = power;
        if (nowPower > -25)
            nowPower = -25;
        pid.update(error, nowPower);
        fixValue = pid.fixValue;
        if (power > 0)
            fixValue *= -1;

        chassis.leftMotor.on(nowPower + fixValue);
        chassis.rightMotor.on(nowPower - fixValue);
        vex::wait(50, vex::timeUnits::msec);
    }
    upDownMotor.turnToPosition(100, 200, false);
    chassis.off(true);
    wait(0.3, timeUnits::sec);
}

void purpleTower(float time, float p, float i, float d, float basicPower)
{

    float3 pid = float3(p, i, d);
    float error;
    float totalError;
    float lastError;
    float fix;
    Brain.Timer.reset();
    while (Brain.Timer.value() < time)
    {
        Vision3.takeSnapshot(Vision__DISC);
        if (Vision3.objectCount > 0)
        {
            error = ((double)Vision3.largestObject.centerX - 158);
        }
        else
            error = 0;
        printf("%d \n", (int)error);
        // error *= -1;
        fix = (pid * float3(error, totalError, (error - lastError))).sum();
        chassis.on(basicPower - fix, basicPower + fix);
        lastError = error;
        vex::wait(10, timeUnits::msec);
    }
}

void extend()
{
    chassis.encMove(185, 90, false);
    chassis.on(-10, 100);
    wait(1, timeUnits::sec);
    clawMotor.on(100);
    chassis.on(100, 100);
    wait(0.8, timeUnits::sec);
    chassis.turnEnc(100, 30);
}

// while(1){
//     Color.setLight(ledState::on);
//     printf("Deg: %d\n", (int)Color.brightness());
//     wait(10,timeUnits::msec);
// }
void shake(int times)
{
    int time = 0;
    while (time < times)
    {
        upDownMotor.turnToPosition(100, 100, true);
        upDownMotor.turnToPosition(100, 300, true);
        time += 1;
    }
    upDownMotor.turnToPosition(100, 260, false);
}

int main()
{

    // main
    init();
    setUp();
    
    // Gyro.installed();
    // Gyro.calibrate(gyroCalibrationType::calNormal, true);
    // Gyro.setRotation(0, rotationUnits::deg);
    // chassis.turnGyro(90);
    // Brain.playSound(soundType::alarm2);
    shotPower = 45;
    Gyro.setRotation(180, vex::rotationUnits::deg);
    upDownMotor.turnToPosition(80, 220, true);
    touchLed.on(vex::color::green);
    while (!touchLed.pressing())
    {
    }
    yellowTower(2.1);

    chassis.encMoveAcc(190, 50);
    upDownMotor.turnToPosition(100, 240, false);
    chassis.turnEnc(60, 360);
    wait(0.1, timeUnits::sec);

    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    Color1.setLight(ledState::on);
    chassis.encMove(300, 70, false);
    while (Color1.brightness() > 20)
    {
        chassis.on(60, 60);
    }
    chassis.encMove(200, 50, true);
    Color1.setLight(ledState::off);

    wait(0.3, timeUnits::sec);
    upDownMotor.turnToPosition(100, 275, true);
    upDownMotor.on(0);
    chassis.turnGyro(3);
    chassis.onForTime(-80, 1.1, false);
    chassis.on(-80, -80);
    clawMotor.on(100);
    wait(1, timeUnits::sec);
    clawMotor.turnToPosition(100, 505, true);
    chassis.leftMotor.motor.setPosition(0, rotationUnits::deg);
    chassis.rightMotor.motor.setPosition(0, rotationUnits::deg);
    chassis.on(0, 0);
    chassis.encMove(70, 70, false);
    chassis.encMove(60, 30, true);

    shot1Motor.on(100);
    shot2Motor.on(100);
    upDownMotor.turnToPosition(100, 250, false);

    clawMotor.turnToPosition(100, 550, false);

    chassis.turnEnc(-70, 310);
    chassis.encMove(180, 70, true);
    chassis.off(true);
    Vision3.takeSnapshot(Vision__DISC);
    while (!((Vision3.objectCount > 0) && (Vision3.largestObject.centerX >= 40) && ((Vision3.largestObject.width * Vision3.largestObject.height) > 100)))
    {
        wait(0.05, timeUnits::sec);
        Vision3.takeSnapshot(Vision__DISC);
        chassis.rightMotor.on(-(Vision3.largestObject.centerX + 55) * 0.6);
    }

    chassis.off(true);
    purpleTower(2.1, 0.31, 0.0012, 0.05, 40);

    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    upDownMotor.turnToPosition(100, 280, false);
    chassis.encMove(100, -70, true);
    chassis.turnEnc(-70, 200);

    chassis.arcMove(-70, 1.57, 550);

    chassis.rightMotor.on(-100);
    wait(0.25, timeUnits::sec);
    chassis.turnEnc(100, 120);
    chassis.encMove(30, -70, true);
    shot(1);

    shotPower -= 10;
    chassis.encMove(30, -70, true);
    chassis.turnEnc(100, 235);
    // chassis.turnGyro(-110);
    // chassis.onForTime(50,0.7,true);
    // chassis.onForTime(90,1.2,true);
    // chassis.onForTime(60,1,true);

    // Gyro.setRotation(-90, rotationUnits::deg);
    // chassis.encMoveAcc(280,-40);
    Color2.setLight(ledState::on);
    chassis.encMove(300, 60, false);
    while (Color2.brightness() > 20)
    {
        chassis.on(30, 30);
    }
    // chassis.encMove(10, 30, true);
    chassis.off(true);
    Color2.setLight(ledState::off);

    chassis.turnGyro(0);
    upDownMotor.turnToPosition(100, 280, false);

    chassis.onForTime(-90, 1.2, true);
    chassis.on(-80, -80);
    clawMotor.on(100);
    wait(1, timeUnits::sec);
    clawMotor.turnToPosition(100, 500, true);
    // wait(0.5, timeUnits::sec);
    chassis.on(0, 0);
    chassis.encMove(90, 60, true);
    shot1Motor.on(100);
    shot2Motor.on(100);
    upDownMotor.turnToPosition(100, 290, false);
    chassis.turnGyro(-100);
    while (!((Vision3.objectCount > 0) && (Vision3.largestObject.centerX >= 170) && ((Vision3.largestObject.width * Vision3.largestObject.height) > 100)))
    {
        wait(0.05, timeUnits::sec);
        Vision3.takeSnapshot(Vision__DISC);
        float power = (Vision3.largestObject.centerX + 70) * 1.5;
        chassis.on(power, -(power*1.5));
    }

    chassis.off(true);
    clawMotor.turnToPosition(100, 550, false);
    purpleTower(1.2, 0.2, 0, 0.03, 40);
    chassis.on(100, 100);
    wait(1.2, timeUnits::sec);
    // chassis.onForTime(70, 2, true);

    upDownMotor.turnToPosition(100, 280, false);
    chassis.encMove(190, -60, true);
    shot1Motor.on(shotPower);
    shot2Motor.on(shotPower);
    // chassis.arcMove(-50, 2.5, 215);
    // chassis.turnGyro(0);
    chassis.turnEnc(-70, 270);
    chassis.on(60, 60);
    // upDownMotor.on(100);
    // wait(0.5, timeUnits::sec);
    // clawMotor.turnToPosition(100, 320, true);
    // clawMotor.turnToPosition(100, 500, true);
    upDownMotor.turnToPosition(100, 110, true);
    clawMotor.turnToPosition(100, 320, true);
    clawMotor.turnToPosition(100, 500, false);
    upDownMotor.on(100);
    shot(2);
    // chassis.arcMove(-70, -2, 250);
    chassis.encMove(80, -70, true);
    chassis.on(0, 0);
    chassis.turnEnc(100, 150);

    extend();
}
