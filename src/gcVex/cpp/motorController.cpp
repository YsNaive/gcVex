#include "..\robotControl\motorController.h"

motorController::motorController(int port){
    motor = vex::motor(port);
}
motorController::motorController(){}
motorController::~motorController(){}

void motorController::on(float power){
    motor.setVelocity(power, vex::percentUnits::pct);
    motor.spin(vex::directionType::fwd);
}

void motorController::off(){
    motor.stop();
}

void motorController::off(bool isHold){
    if(isHold)
        motor.setStopping(vex::brakeType::hold);
    else
        motor.setStopping(vex::brakeType::coast);

    off();
}

// void motorController::onEnc(float power, float enc);
// void motorController::turnToPosition(float power, float position);