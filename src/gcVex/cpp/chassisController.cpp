#include "..\robotControl\chassisController.h"

    chassisController::chassisController(int leftMotorPort, int rightMotorPort, float3 pid){
        leftMotor = motorController(leftMotorPort);
        rightMotor = motorController(rightMotorPort);
        pidController.setPID(pid);
    }

    chassisController::~chassisController(){}

    void chassisController::setPID(float3 pid){
        pidController.setPID(pid);
    }

    // twoMotor with same power
    void chassisController::on(float power){
        leftMotor.on(power);
        rightMotor.on(power);
    }
    // twoMotor with different power
    void chassisController::on(float2 power){
        leftMotor.on(power.x);
        rightMotor.on(power.y);
    }
    void chassisController::on(float leftPower, float rightPower){
        leftMotor.on(leftPower);
        rightMotor.on(rightPower);
    }
    
    void chassisController::off(){
        leftMotor.off();
        rightMotor.off();
    }

    void chassisController::off(bool isHold){
        leftMotor.off(isHold);
        rightMotor.off(isHold);
    }
    
    void chassisController::encMove(float power, float enc);
    void chassisController::encMoveAcc(float power, float enc);
    void chassisController::arcMove(float power, float r, float enc);
    void chassisController::onForTime(float power, float time);
    void chassisController::turnEnc(float power, float enc, turnDirection dir);
    void chassisController::turnGyro(float power, float angle);