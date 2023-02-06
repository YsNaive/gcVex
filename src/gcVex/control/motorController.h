#ifndef GCVEX_MOTORCONTROL_H_
#define GCVEX_MOTORCONTROL_H_

#include <math.h>
#include <functional>
#include <vex.h>
#include "../mathReleated/pidController.h"
#include "../struct/motorSetting.h"

namespace gcVex{
    enum motorState{
        off,
        on,
        onForEnc,
        onForTime,
        onToPosition
    };

    class motorController
    {
    private:
        int targetPower    = 0;
        int targetPosition = 0;
        float targetTime   = 0;
        bool isClosed        = true;
        bool isSettingHotFix = false;
        motorSetting setting;
        pidController controller;
        motorState state;
        vex::motor motor = vex::motor(vex::PORT1);
        vex::timer timer;
        vex::task task;
        std::function<void()> callBack;

        void spin(int power);

    public:
        motorController();
        motorController(int port);
        motorController(motorSetting setting);
        ~motorController();

        vex::motor* getMotorPtr();
        motorState getState();
        int getPosition();

        void setSetting(motorSetting setting);
        void setPID(float3 pid); // let motor spin to target speed faster
        void setBreakType(vex::brakeType brakeType);
        
        void update();
        void init();
        void off();                          // breakMode depend on setting, and it will stop any "on()" request.
        void off(vex::brakeType breakType);  // break by type at once, it doesn't change setting
        void on(int power);                  // power = [0,100]
        void onEnc(int power, int enc, vex::brakeType breakType = vex::brakeType::undefined);             // on for enc
        void onTime(int power, int sencend, vex::brakeType breakType = vex::brakeType::undefined);        // on for time
        void onToPosition(int power, int position, vex::brakeType breakType = vex::brakeType::undefined);  // hold on position
        void close();

        bool isOff();
        void wait();

    };
}

#endif