#pragma once
#include "gcVex\\struct\\core.h"
class pidController
{
private:
    float3 pid;
    float error, errorLast, errorTotal;
public:
    pidController();
    ~pidController();
    
    void setPID(float3 newPID);
};