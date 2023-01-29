#include "..\\gcVex\\struct\\core.h"
#pragma once
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