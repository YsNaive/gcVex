#ifndef GCVEX_MOTORSETTING_H_
#define GCVEX_MOTORSETTING_H_

#include <vex.h>

namespace gcVex{
    struct motorSetting
    {
        int port;
        int offRange;
        float3 pid;
        vex::brakeType breakType;
        
        bool isReversed_init;
        bool isResetPosition_init;

        motorSetting(){
            port = 0;
            offRange = 1;
            pid = float3(0,0,0);
            breakType = vex::brakeType::hold;
            isReversed_init = false;
            isResetPosition_init = true;
        }
    };
}

#endif