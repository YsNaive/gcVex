#ifndef GCVEX_CHASSISCONTROLLER_H_
#define GCVEX_CHASSISCONTROLLER_H_

#include <vex.h>
#include "..\struct.inc"
#include "..\mathReleated.inc"
#include "..\robotControl\motorController.h"

using namespace vex;

enum turnDirection{
    left,
    right
};

class chassisController
{
private:
    chassisController();
    pidController pidController;

public:
    motorController leftMotor = motorController(PORT12), rightMotor = motorController(PORT7);
    float startAccEnc;
    float endAccEnc;

    chassisController(int leftMotorPort, int rightMotorPort, float3 pid);
    ~chassisController();

    void setPID(float3 pid);

    // twoMotor with same power
    void on(float power);
    // twoMotor with different power
    void on(float2 power);
    void on(float leftPower, float rightPower);
    void off();
    void off(bool isHold);
    
    void encMove(float power, float enc);
    void encMoveAcc(float power, float enc);
    void arcMove(float power, float r, float enc);
    void onForTime(float power, float time);
    void turnEnc(float power, float enc, turnDirection dir);
    void turnGyro(float power, float angle);
};

#endif