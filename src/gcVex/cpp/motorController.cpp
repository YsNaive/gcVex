#include "..\robotControl\motorController.h"

motorController::motorController(int port){
    motor = vex::motor(port);
}
motorController::~motorController(){}

void motorController::on(float power){
    motor.setVelocity(power, vex::percentUnits::pct);
    motor.spin(vex::directionType::fwd);
}

void motorController::off(){
    motor.stop(breakMode);
}

void motorController::off(bool isHold){
    if(isHold)
        breakMode = vex::brakeType::hold;
    else
        breakMode = vex::brakeType::brake;

    off();
}

// void motorController::onEnc(float power, float enc);
// void motorController::turnToPosition(float power, float position);