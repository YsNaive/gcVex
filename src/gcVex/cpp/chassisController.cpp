#include "..\control\chassisController.h"
namespace gcVex{

    chassisController::chassisController(){
        startAccEnc=70;
        endAccEnc=70;
        allowErrorEnc = 10;
    }
    chassisController::chassisController(int leftMotorPort, int rightMotorPort, float3 minPid,float3 maxPid, vex::brain* mainBrainPtr, vex::inertial* mainInertialPtr){
        leftMotor = motorController(leftMotorPort);
        rightMotor = motorController(rightMotorPort);
        brainPtr = mainBrainPtr;
        inertialPtr = mainInertialPtr;
        startAccEnc=70;
        endAccEnc=70;
        allowErrorEnc = 10;
    }

    chassisController::~chassisController(){}


   
    
}