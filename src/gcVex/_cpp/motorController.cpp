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
    powerController.setPID(pid);
}
void motorController::setBreakType(vex::brakeType brakeType){
    setting.breakType = brakeType;
    motor.setStopping(setting.breakType);
}

motorController* motorController::waitCmd(){
    isNextCmdWait = true;
    return this;
}

//TODO cant use in addTask
bool motorController::update(){
    return true;
}

void motorController::init(){
    isClosed = false;
    motor = vex::motor(setting.port);
    motor.setStopping(setting.breakType);
    motor.setReversed(setting.isReversed_init);
    if(setting.isResetPosition_init)
        motor.resetPosition();

    powerController.setPID(setting.pid);
    powerController.setOnCalculateError([&](void)->float{return targetPower - motor.power();});
    powerController.setOnUpdated([&](float fix)->void{spin(targetPower + fix);});
    
    encController.setPID(setting.pid);
    encController.setOnCalculateError([&](void)->float{return targetPosition - getPosition();});
    encController.setOnUpdated([&](float fix)->void{spin(targetPower + fix);});

    if(task != nullptr)
        tick::colseTask(task);
    tick::addTask([&](void)->bool{
        if(state == motorState::none);
        else if (state == motorState::off);
        else if (state == motorState::on);
        else if (state == motorState::onForEnc){
            
            encController.update();
            if(encController.getError().x < 10)
                checkCount++;
            else
                checkCount = 0;
            if(checkCount > 5)
                off();
        }
        

        return isClosed;
    });
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
    powerController.reset();
    if(breakType != vex::brakeType::undefined)
        setBreakType(breakType);
    state = motorState::onForEnc;
    
    if(isNextCmdWait){
        while (checkCount <= 5)
        {
            encController.update();
            if(encController.getError().x < 10)
                checkCount++;
            else
                checkCount = 0;
        }
        off();
        isNextCmdWait = false;
    }
}
void motorController::onTime(int power, int sencend, vex::brakeType breakType){
    targetPower = power;
    targetTime = sencend;
    powerController.reset();
    if(breakType != vex::brakeType::undefined)
        setBreakType(breakType);
    state = motorState::onForTime;
}
void motorController::onToPosition(int power, int position, vex::brakeType breakType){
    targetPower = power;
    targetPosition = position;
    powerController.reset();
    if(breakType != vex::brakeType::undefined)
        setBreakType(breakType);
    state = motorState::onToPosition;
}
void motorController::close(){
    isClosed = true;
}

bool motorController::isOff(){return state == motorState::off;}

}