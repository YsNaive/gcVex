#ifndef GCVEX_PIDCONTROLLER_H_
#define GCVEX_PIDCONTROLLER_H_

#include "..\struct\floatExtention.h"
class pidController
{
private:
    float3 pid;
    float lastError, totalError;
public:
    /// @brief after called update(), this value will reflash as return
    float fixValue;

    pidController();
    pidController(float3 newPID);
    ~pidController();
    
    void setPID(float3 newPID);
    float update(float error); // calculate next fixValue
};

#endif