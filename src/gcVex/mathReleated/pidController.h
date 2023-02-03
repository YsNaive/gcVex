#ifndef GCVEX_PIDCONTROLLER_H_
#define GCVEX_PIDCONTROLLER_H_

#include "..\struct\floatExtention.h"
class pidController
{
private:
    float3 minPid;
    float3 maxPid;
    float lastPower;
    float lastError, totalError;
public:
    /// @brief after called update(), this value will reflash as return
    float fixValue;

    pidController();
    pidController(float3 minPid, float3 maxPid);
    ~pidController();

    void setMinPID(float3 newMinPid);
    void setMaxPID(float3 newMaxPid);
    float3 calPID(float nowPower);
    float update(float error); // calculate next fixValue with last input power
    float update(float error, float power); // calculate next fixValue
    void reset();
};

#endif