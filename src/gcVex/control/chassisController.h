#ifndef GCVEX_CHASSISCONTROLLER_H_
#define GCVEX_CHASSISCONTROLLER_H_

#include <string.h>
#include <vex.h>
#include "..\struct.inc"
#include "..\mathReleated.inc"
#include "..\control\motorController.h"

using namespace vex;

enum turnDirection{
    left,
    right
};

class chassisController
{
private:

    pidController pidController;
    float accPower(float x, float power,float all);
    vex::brain* brainPtr;
    vex::inertial* inertialPtr;
    vex::gyro *gyroPtr;

public:
    float minPower;
    float startAccEnc;
    float endAccEnc;
    float allowErrorEnc;
    motorController leftMotor = motorController(PORT12), rightMotor = motorController(PORT7);

    chassisController();
    chassisController(int leftMotorPort, int rightMotorPort, float3 minPid,float3 maxPid,vex::brain* mainBrainPtr, vex::inertial* mainInertialPtr, vex::gyro *mainGyroPrt);
    ~chassisController();

    // twoMotor with same power
    void on(float power);
    // twoMotor with different power
    void on(float2 power);
    void on(float leftPower, float rightPower);
    void off();
    void off(bool isHold);
    
    void encMove(float enc, float power, bool isHold);
    void encMoveAcc(float enc, float power);
    void arcMove(float power, float r, float enc);
    void onForTime(float power, float time, bool PdorNot);
    void turnEnc(float power, float enc);
    void turnGyro(float target);
};

#endif