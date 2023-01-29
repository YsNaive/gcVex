#ifndef GCVEX_CHASSISCONTROLLER_H_
#define GCVEX_CHASSISCONTROLLER_H_

#include <vex.h>
#include "..\struct.inc"
#include "..\mathReleated.inc"

using namespace vex;

class chassisController
{
private:
    chassisController();
    pidController pidController;

public:
    motor leftMotor, rightMotor;

    chassisController(int leftMotorPort, int rightMotorPort, float3 pid);
    ~chassisController();

    void setPID(float3 pid);

    // twoMotor with same power
    void on(float power);
    // twoMotor with different power
    void on(float2 power);
    void on(float leftPower, float rightPower);
    void off();
    
    void moveEnc(float power, float enc);
    void moveEnc(float power, float enc, void* callBack);
    void arcMoveEnc(float power, float r, float enc);
    void arcMoveEnc(float power, float r, float enc, void* callBack);
    void moveTime(float power, float time);
};

#endif