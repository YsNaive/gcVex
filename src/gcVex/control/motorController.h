#ifndef GCVEX_MOTORCONTROL_H_
#define GCVEX_MOTORCONTROL_H_

#include <math.h>
#include <vex.h>
#include <iq2_api.h>

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
    void onEnc(float power, float enc, bool waitForCompletion);
    void turnToPosition(float power, float position, bool waitForCompletion);
};

#endif