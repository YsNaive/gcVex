#include "../gcVex.h"

namespace gcVex{
    namespace test
    {
        void testPidController(){
            /*--- motor init ---*/
            motor leftMotor = motor(PORT12);
            motor rightMotor = motor(PORT7);
            leftMotor.resetPosition();
            rightMotor.resetPosition();
            leftMotor.setReversed(true);
            leftMotor.spin(directionType::fwd);
            rightMotor.spin(directionType::fwd);
            
            /*--- tank drive in straight line by using pidController ---*/
            pidController controller = pidController(
                float3(0.2, 0, 0.04),
                [&](void) -> float { // the action that give controller the error from
                    return leftMotor.position(rotationUnits::deg) - rightMotor.position(rotationUnits::deg); 
                    },
                [&](float fix) -> void { // apply the feedback on motor
                    leftMotor.setVelocity(40 - fix,percentUnits::pct);
                    rightMotor.setVelocity(40 + fix,percentUnits::pct);
                });

            /*--- run for 2 sec ---*/
            vex::timer time = vex::timer();
            while (time.time(vex::timeUnits::sec)<2)
            {
                controller.update();
                vex::wait(15, vex::timeUnits::msec);
            }

            /*--- off motor ---*/
            leftMotor.stop();
            rightMotor.stop();
            
            printf("Finish.\n");
        }
    }
}