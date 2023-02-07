#include "..\control\motorController.h"

namespace gcVex{
    

motorController::motorController(){init();}
motorController::motorController(int port){
    setting.port = port;
    init();
}
motorController::motorController(motorSetting setting){
    this->setting = setting;
    init();
}
motorController::~motorController(){}

void motorController::spin(int power){
    motor.setVelocity(power, vex::percentUnits::pct);
    motor.spin(vex::directionType::fwd);
}

vex::motor* motorController::getMotorPtr(){return &motor;}
motorState motorController::getState(){return state;}
int motorController::getPosition(){return motor.position(vex::rotationUnits::deg);}

/// @brief Notice that this func will re-init the controller.
void motorController::setSetting(motorSetting setting){
    this->setting = setting;
    init();
}
void motorController::setPID(float3 pid){
    controller.setPID(pid);
}
void motorController::setBreakType(vex::brakeType brakeType){
    setting.breakType = brakeType;
    motor.setStopping(setting.breakType);
}

void motorController::init(){
    isClosed = false;
    motor = vex::motor(setting.port);
    motor.setStopping(setting.breakType);
    motor.setReversed(setting.isReversed_init);
    if(setting.isResetPosition_init)
        motor.resetPosition();
    controller.setPID(setting.pid);

    // task = vex::task(update);
    // TODO tick apply
}
void motorController::off(){
    motor.stop();
    state = motorState::off;
}
void motorController::off(vex::brakeType breakType){
    setBreakType(breakType);
    off();
}
void motorController::on(int power){
    
    targetPower = power;
    state = motorState::on;
}
void motorController::onEnc(int power, int enc, vex::brakeType breakType){
    targetPower = power;
    targetPosition = getPosition() + enc;
    if(breakType != vex::brakeType::undefined)
        setBreakType(breakType);
    state = motorState::onForEnc;
}
void motorController::onTime(int power, int sencend, vex::brakeType breakType){
    targetPower = power;
    targetTime = sencend;
    if(breakType != vex::brakeType::undefined)
        setBreakType(breakType);
    state = motorState::onForTime;
}
void motorController::onToPosition(int power, int position, vex::brakeType breakType){
    targetPower = power;
    targetPosition = position;
    if(breakType != vex::brakeType::undefined)
        setBreakType(breakType);
    state = motorState::onToPosition;
}
void motorController::close(){
    isClosed = true;
}

bool motorController::isOff(){return state == motorState::off;}
void motorController::wait(){while (!isOff());}

}