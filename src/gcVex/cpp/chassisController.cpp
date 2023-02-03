#include "..\control\chassisController.h"

    chassisController::chassisController(){
        startAccEnc=70;
        endAccEnc=70;
        allowErrorEnc = 10;
    }
    chassisController::chassisController(int leftMotorPort, int rightMotorPort, float3 minPid,float3 maxPid, vex::brain* mainBrainPtr, vex::inertial* mainInertialPtr){
        leftMotor = motorController(leftMotorPort);
        rightMotor = motorController(rightMotorPort);
        pidController.setMinPID(minPid);
        pidController.setMaxPID(maxPid);
        brainPtr = mainBrainPtr;
        inertialPtr = mainInertialPtr;
        startAccEnc=70;
        endAccEnc=70;
        allowErrorEnc = 10;
    }

    chassisController::~chassisController(){}


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
    
    void chassisController::encMove(float enc, float power){
        bool arrived = false;
        float leftStartPos = leftMotor.motor.position(vex::rotationUnits::deg);
        float rightStartPos = rightMotor.motor.position(vex::rotationUnits::deg);

        vex::inertial brainInertial = vex::inertial();
        float gyroStartPos = brainInertial.rotation();

        float nowEnc = 0;
        float leftEnc;
        float rightEnc;
        float error=0;
        float fixValue;
        while(!arrived){
            leftEnc = abs(leftMotor.motor.position(vex::rotationUnits::deg)-leftStartPos);
            rightEnc = abs(rightMotor.motor.position(vex::rotationUnits::deg)-rightStartPos);
            nowEnc=((leftEnc+rightEnc)/2);
            arrived = (nowEnc >= enc);
            //error = leftEnc-rightEnc;
            error = brainInertial.rotation() - gyroStartPos;
            pidController.update(error);
            fixValue=pidController.fixValue;
            if(power>0){
                leftMotor.on(power+fixValue);
                rightMotor.on(power-fixValue);
            }
                
            else{
                leftMotor.on(power-fixValue);
                rightMotor.on(power+fixValue);
            }
            vex::wait(10,vex::timeUnits::msec);
        }
        chassisController::off(true);
    }

    /// @brief 
    /// @param x now enc
    /// @param power 
    /// @param all 
    /// @return 
    float chassisController::accPower(float x, float power,float all){
        float error=(all-x);
        float output;
        if(x <= startAccEnc){
            output = (accCurve::upCurve((x/startAccEnc))*power);
        }
        else if(x <= (all-endAccEnc)){
            output = power;
        }
        else{
            output = (accCurve::downCurve(((x-(all-endAccEnc) )/endAccEnc))*power);
        }
        if(std::abs(output)< minPower)
                return  minPower * (power/std::abs(power));

        return output;
    }

    void chassisController::encMoveAcc(float enc, float power){
        bool arrived = false;
        float leftStartPos = leftMotor.motor.position(vex::rotationUnits::deg);
        float rightStartPos = rightMotor.motor.position(vex::rotationUnits::deg);
        vex::inertial brainInertial = vex::inertial();
        float gyroStartPos = brainInertial.rotation();
        float nowEnc = 0;
        float leftEnc;
        float rightEnc;
        float error=0;
        float fixValue;
        float nowPower;
        while(!arrived){
            leftEnc = abs(leftMotor.motor.position(vex::rotationUnits::deg)-leftStartPos);
            rightEnc = abs(rightMotor.motor.position(vex::rotationUnits::deg)-rightStartPos);
            nowEnc=((leftEnc+rightEnc)/2);
            error = leftEnc-rightEnc;
            arrived =(abs(enc-nowEnc)< allowErrorEnc);
            //error = brainInertial.rotation() - gyroStartPos;
            nowPower=chassisController::accPower(nowEnc,power,enc);
            pidController.update(error);
            fixValue=pidController.fixValue;
            float lpowerError = leftMotor.motor.velocity(percentUnits::pct) - nowPower;
            float rpowerError = rightMotor.motor.velocity(percentUnits::pct) - nowPower;
            if(power>0)
                fixValue*=-1;

            leftMotor.on(nowPower+fixValue);
            rightMotor.on(nowPower-fixValue);
            vex::wait(50,vex::timeUnits::msec);
        }
        brainPtr->playSound(vex::soundType::alarm);
        chassisController::off(true);
    }
    /// @brief 
    /// @param power 
    /// @param r if (r > 0) :turn right .else turn left
    /// @param enc 
    void chassisController::arcMove(float power, float r, float enc){
        bool arrived = false;
        float leftStartPos = leftMotor.motor.position(vex::rotationUnits::deg);
        float rightStartPos = rightMotor.motor.position(vex::rotationUnits::deg);
        float nowEnc = 0;
        float leftEnc;
        float rightEnc;
        float error=0;
        float fixValue;
        while(!arrived){
            leftEnc = abs(leftMotor.motor.position(vex::rotationUnits::deg)-leftStartPos);
            rightEnc = abs(rightMotor.motor.position(vex::rotationUnits::deg)-rightStartPos);
            nowEnc=((leftEnc+rightEnc)/2);
            if(r>0)
                leftEnc*r;
            else
                rightEnc*r;
            arrived = (nowEnc >= enc);
            error = leftEnc-rightEnc;
            pidController.update(error);
            fixValue=pidController.fixValue;
            if(power>0){
                leftMotor.on(power+fixValue);
                rightMotor.on(power-fixValue);
            }
                
            else{
                leftMotor.on(power-fixValue);
                rightMotor.on(power+fixValue);
            }
            vex::wait(10,vex::timeUnits::msec);
        }
        chassisController::off(true);

    }

    void chassisController::onForTime(float power, float time, bool PdorNot){
        brainPtr->Timer.reset();
        float leftStartPos = leftMotor.motor.position(vex::rotationUnits::deg);
        float rightStartPos = rightMotor.motor.position(vex::rotationUnits::deg);
        vex::inertial brainInertial = vex::inertial();
        float gyroStartPos = brainInertial.rotation();
        float nowEnc = 0;
        float leftEnc;
        float rightEnc;
        float error=0;
        float fixValue;
        while((brainPtr->Timer.value() >= time)){
            if(PdorNot){
                leftEnc = abs(leftMotor.motor.position(vex::rotationUnits::deg)-leftStartPos);
                rightEnc = abs(rightMotor.motor.position(vex::rotationUnits::deg)-rightStartPos);
                nowEnc=((leftEnc+rightEnc)/2);
                //error = leftEnc-rightEnc;
                error = brainInertial.rotation() - gyroStartPos;
                pidController.update(error);
                fixValue=pidController.fixValue;\
                if(power>0){
                    leftMotor.on(power+fixValue);
                    rightMotor.on(power-fixValue);
                }
                
                else{
                    leftMotor.on(power-fixValue);
                    rightMotor.on(power+fixValue);
                }   
                vex::wait(10,vex::timeUnits::msec);
            }
            else{
                chassisController::on(power,power);
            }
        }
        chassisController::off(true);
    }

    void chassisController::turnEnc(float power, float enc, turnDirection dir){}

    int sgn(double d)
    {
        if (d<0) return -1;
        else if (d==0) return 0;
        else return 1;
    }

    void chassisController::turnGyro(float target ){
        bool arrived = false;
        // TODO rewrite turn
        while(!arrived){
            printf("%d\n",(int)inertialPtr->rotation());
        }
        chassisController::off(true);
    }    