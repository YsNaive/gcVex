#ifndef GCVEX_MOTORCONTROL_H_
#define GCVEX_MOTORCONTROL_H_

#include <math.h>
#include <vex.h>

class motorController
{
private:

public:
    vex::motor motor = vex::motor(vex::PORT1);
    motorController();
    motorController(int port);
    ~motorController();

    void on(float power);
    void off();
    void off(bool isHold);
    void onEnc(float power, float enc);
    void turnToPosition(float power, float position);
};

#endif