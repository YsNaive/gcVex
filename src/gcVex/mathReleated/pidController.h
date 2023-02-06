/*----------------------------------------------------------------------------*/
//    Module:       pidController
//    Author:       YsNaive
//      This class has 2 ways to use
//      1. Use it as common calculater
//          a. call nextError() and use return as fixValue feedback
//      
//      2. Use it with only calling Update()
//          a. you need to define:
//              i. float OnCalculateError();
//             ii. void OnUpdated(float fixValue);
//          b. call Update(), and its calling steps as below
//             OnCalculateError ==(error)==> Update ==(fixValue)==> OnUpdated
/*----------------------------------------------------------------------------*/


#ifndef GCVEX_PIDCONTROLLER_H_
#define GCVEX_PIDCONTROLLER_H_

#include "..\struct\floatExtention.h"

namespace gcVex{
    class pidController
    {
    private:
        float3 pid;     // { P, I, D }
        float3 error;   // { Error, AccumulateError, LastError }
        float fixValue; // give back value from pidController
        float (*onCalculateError)();
        void (*onUpdated)(float);
    public:
        pidController();
        pidController(float3 pid);
        pidController(float3 pid,float (*onCalculateError)(),void (*onUpdated)(float));
        ~pidController();

        /// @brief calling before update(), you need to return error in this Func
        void setOnCalculateError (float (*onCalculateError)());

        /// @brief calling after update(), you can do something with feedback value by controller
        void setOnUpdated (void (*onUpdated)(float));

        void setPID(float3 pid);
        void setPID(float p, float i, float d);

        float nextError(float nextError);
        /// @brief you need to define onCalculateError() and onUpdated() before calling update()
        void update();

        float3 getPID();
        float3 getError();
        float getFixValue();
    };
}

#endif