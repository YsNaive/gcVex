/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       chuwe                                                     */
/*    Created:      2023/1/29 下午1:48:21                                     */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// 基本函式庫
#include "gcVex\gcVex.h"

using namespace vex;
using namespace gcVex;

vex::brain Brain;

int main() {
    tick::init();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nstart\n");
    motorSetting setting;
    setting.breakType = vex::brakeType::hold;
    setting.isResetPosition_init = true;
    setting.isReversed_init = true;
    setting.offRange = 3;
    setting.pid = float3(0.4, 0, 0.12);
    setting.port = PORT12;
    motorController lm = motorController(setting);
    setting.isReversed_init = false;
    setting.port = PORT7;
    motorController rm = motorController(setting);

    rm.onEnc(0, 200);
    lm.waitCmd()->onEnc(0, 200);
    vex::wait(100, timeUnits::msec);
    rm.onEnc(0, 200);
    lm.onEnc(0, 200);
    
    tick::debug();
    printf("finish\n");
}

