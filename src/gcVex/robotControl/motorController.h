#include <math.h>
#include <vex.h>

class motorController
{
private:
    motorController();
    bool lastBreakMode;
    
public:
    vex::motor motor = vex::motor(vex::PORT1);
    motorController(int port);
    ~motorController();

    void on(float power);
    void off();
    void off(bool isBreak);
    void onEnc(float power, float enc);
    void turnToPosition(float power, float position);
};
