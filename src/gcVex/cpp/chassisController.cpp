#include "..\control\chassisController.h"

chassisController::chassisController()
{
    startAccEnc = 70;
    endAccEnc = 70;
    allowErrorEnc = 5;
}
chassisController::chassisController(int leftMotorPort, int rightMotorPort, float3 minPid, float3 maxPid, vex::brain *mainBrainPtr, vex::inertial *mainInertialPtr, vex::gyro *mainGyroPrt)
{
    leftMotor = motorController(leftMotorPort);
    rightMotor = motorController(rightMotorPort);
    pidController.setMinPID(minPid);
    pidController.setMaxPID(maxPid);
    brainPtr = mainBrainPtr;
    inertialPtr = mainInertialPtr;
    gyroPtr = mainGyroPrt;
    startAccEnc = 70;
    endAccEnc = 70;
    allowErrorEnc = 10;
}

chassisController::~chassisController() {}

// twoMotor with same power
void chassisController::on(float power)
{
    leftMotor.on(power);
    rightMotor.on(power);
}
// twoMotor with different power
void chassisController::on(float2 power)
{
    leftMotor.on(power.x);
    rightMotor.on(power.y);
}
void chassisController::on(float leftPower, float rightPower)
{
    leftMotor.on(leftPower);
    rightMotor.on(rightPower);
}

void chassisController::off()
{
    leftMotor.off();
    rightMotor.off();
}

void chassisController::off(bool isHold)
{
    leftMotor.off(isHold);
    rightMotor.off(isHold);
}

void chassisController::encMove(float enc, float power, bool isHold)
{
    bool arrived = false;
    pidController.reset();
    float leftStartPos = leftMotor.motor.position(vex::rotationUnits::deg);
    float rightStartPos = rightMotor.motor.position(vex::rotationUnits::deg);
    float gyroStartPos = inertialPtr->rotation();

    float nowEnc = 0;
    float leftEnc;
    float rightEnc;
    float error = 0;
    float fixValue;
    while (!arrived)
    {
        leftEnc = abs(leftMotor.motor.position(vex::rotationUnits::deg) - leftStartPos);
        rightEnc = abs(rightMotor.motor.position(vex::rotationUnits::deg) - rightStartPos);
        nowEnc = ((leftEnc + rightEnc) / 2);
        arrived = (nowEnc >= enc);
        // error = leftEnc-rightEnc;
        error = inertialPtr->rotation() - gyroStartPos;
        pidController.calPID(power);
        pidController.update(error);
        fixValue = pidController.fixValue;
        if (power > 0)
            fixValue *= -1;

        leftMotor.on(power + fixValue);
        rightMotor.on(power - fixValue);
        vex::wait(10, vex::timeUnits::msec);
    }
    if (isHold)
        chassisController::off(true);
    else
        chassisController::off();
}

/// @brief
/// @param x now enc
/// @param power
/// @param all
/// @return
float chassisController::accPower(float x, float power, float all)
{
    float error = (all - x);
    float output;
    if (x <= startAccEnc)
    {
        output = (accCurve::upCurve((x / startAccEnc)) * power);
    }
    else if (x <= (all - endAccEnc))
    {
        output = power;
    }
    else
    {
        output = (accCurve::downCurve(((x - (all - endAccEnc)) / endAccEnc)) * power);
    }
    if (std::abs(output) < minPower)
        return minPower * (power / std::abs(power));

    return output;
}

void chassisController::encMoveAcc(float enc, float power)
{
    bool arrived = false;
    pidController.reset();
    float leftStartPos = leftMotor.motor.position(vex::rotationUnits::deg);
    float rightStartPos = rightMotor.motor.position(vex::rotationUnits::deg);
    float gyroStartPos = inertialPtr->rotation();
    float nowEnc = 0;
    float leftEnc;
    float rightEnc;
    float error = 0;
    float fixValue;
    float nowPower;
    while (!arrived)
    {
        leftEnc = abs(leftMotor.motor.position(vex::rotationUnits::deg) - leftStartPos);
        rightEnc = abs(rightMotor.motor.position(vex::rotationUnits::deg) - rightStartPos);
        nowEnc = ((leftEnc + rightEnc) / 2);
        error = leftEnc - rightEnc;
        arrived = (abs(enc - nowEnc) < allowErrorEnc);
        // error = inertialPtr->rotation() - gyroStartPos;
        nowPower = chassisController::accPower(nowEnc, power, enc);
        pidController.update(error, nowPower);
        fixValue = pidController.fixValue;
        float lpowerError = leftMotor.motor.velocity(percentUnits::pct) - nowPower;
        float rpowerError = rightMotor.motor.velocity(percentUnits::pct) - nowPower;
        if (power > 0)
            fixValue *= -1;

        leftMotor.on(nowPower + fixValue);
        rightMotor.on(nowPower - fixValue);
        vex::wait(50, vex::timeUnits::msec);
    }
    // brainPtr->playSound(vex::soundType::alarm);
    chassisController::off(true);
}
/// @brief
/// @param power
/// @param r if (r < 0) :turn right .else turn left
/// @param enc
void chassisController::arcMove(float power, float r, float enc)
{
    bool arrived = false;
    float leftStartPos = leftMotor.motor.position(vex::rotationUnits::deg);
    float rightStartPos = rightMotor.motor.position(vex::rotationUnits::deg);
    float nowEnc = 0;
    float leftEnc = 0;
    float rightEnc = 0;
    float error = 0;
    float fixValue = 0;
    while (!arrived)
    {
        leftEnc = abs(leftMotor.motor.position(vex::rotationUnits::deg) - leftStartPos);
        rightEnc = abs(rightMotor.motor.position(vex::rotationUnits::deg) - rightStartPos);
        nowEnc = ((leftEnc + rightEnc) / 2);
        if (r > 0)
            leftEnc = abs(leftEnc * r);
        else
            rightEnc = abs(rightEnc * r);
        arrived = (nowEnc >= enc);
        error = leftEnc - rightEnc;

        pidController.setMaxPID(float3 (3,0,0.5));
        pidController.setMinPID(float3 (0.9,0,0.07));

        pidController.calPID(power);
        pidController.update(error);
        fixValue = pidController.fixValue;
        if (power > 0)
            fixValue *= -1;

        leftMotor.on(power + fixValue);
        rightMotor.on(power - fixValue);
        vex::wait(10, vex::timeUnits::msec);
    }
    chassisController::off(true);
    pidController.setMaxPID(float3 (0.5,0,0.15));
    pidController.setMinPID(float3 (0.3,0,0.1));
}

void chassisController::onForTime(float power, float time, bool PdorNot)
{
    brainPtr->Timer.reset();
    pidController.reset();
    float leftStartPos = leftMotor.motor.position(vex::rotationUnits::deg);
    float rightStartPos = rightMotor.motor.position(vex::rotationUnits::deg);
    vex::inertial brainInertial = vex::inertial();
    float gyroStartPos = inertialPtr->rotation();
    float nowEnc = 0;
    float leftEnc;
    float rightEnc;
    float error = 0;
    float fixValue;
    while ((brainPtr->Timer.value() <= time))
    {
        if (PdorNot)
        {
            leftEnc = abs(leftMotor.motor.position(vex::rotationUnits::deg) - leftStartPos);
            rightEnc = abs(rightMotor.motor.position(vex::rotationUnits::deg) - rightStartPos);
            nowEnc = ((leftEnc + rightEnc) / 2);
            error = leftEnc - rightEnc;
            // error = inertialPtr->rotation() - gyroStartPos;
            pidController.update(error);
            fixValue = pidController.fixValue;
            if (power > 0)
                fixValue *= -1;

            leftMotor.on(power + fixValue);
            rightMotor.on(power - fixValue);
            vex::wait(10, vex::timeUnits::msec);
        }
        else
        {
            chassisController::on(power, power);
        }
    }
    chassisController::off(true);
}
/// @brief
/// @param power
/// @param enc
/// @param dir 1 = left ; 0 = right
void chassisController::turnEnc(float power, float enc)
{
    float leftStartEnc = leftMotor.motor.position(rotationUnits::deg);
    float rightStartEnc = rightMotor.motor.position(rotationUnits::deg);
    float leftEnc = 0;
    float rightEnc = 0;
    while ((leftEnc + rightEnc / 2) < enc)
    {
        leftEnc = abs(leftMotor.motor.position(rotationUnits::deg) - leftStartEnc);
        rightEnc = abs(rightMotor.motor.position(rotationUnits::deg) - rightStartEnc);
        chassisController::on(power, -power);
    }
    chassisController::off(true);
    // brainPtr->playSound(vex::soundType::alarm);
}

void chassisController::turnGyro(float target)
{
    bool arrived = false;
    float3 pid = float3(0.45, 0.001, 0.35);
    float error;
    float lastError = 0;
    float totalError = 0;
    float power;
    int arrivedCount = 0;
    while (arrivedCount < 8)
    {
        printf("ERR %d\n", (int)error);
        printf("DEG %d\n", (int)inertialPtr->rotation(vex::rotationUnits::deg));
        error = (target - gyroPtr->rotation(rotationUnits::deg));
        totalError += error;
        if (std::abs(error) < 2)
            arrivedCount++;
        else
            arrivedCount = 0;

        power = (float3(error, totalError, lastError - error) * pid).sum();
        power += 2.6 * (power / std::abs(power));
        if (error < 50)
            power *= 0.75;
        else
            power *= 1.2;
        chassisController::on(-power, power);
        lastError = error;
        vex::wait(25, timeUnits::msec);
    }
    // brainPtr->playSound(vex::soundType::alarm);
    chassisController::off(true);
}