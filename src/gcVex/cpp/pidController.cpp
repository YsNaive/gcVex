#include "..\mathReleated\pidController.h"

namespace gcVex{

        pidController::pidController(){
            pid = float3(0,0,0);
            error = float3(0,0,0);
        }
        pidController::pidController(float3 pid){
            this->pid = pid;
            error = float3(0,0,0);
        }
        pidController::pidController(float3 pid,float (*onCalculateError)(),void (*onUpdated)(float)){
            this->pid = pid;
            error = float3(0,0,0);
            this->onCalculateError = onCalculateError;
            this->onUpdated = onUpdated;
        }
        pidController::~pidController(){}

        /// @brief calling before update(), you need to return error in this Func
        void pidController::setOnCalculateError (float (*onCalculateError)()){
            this->onCalculateError = onCalculateError;
        }

        /// @brief calling after update(), you can do something with feedback value by controller
        void pidController::setOnUpdated (void (*onUpdated)(float)){
            this->onUpdated = onUpdated;
        }

        void pidController::setPID(float3 pid){
            this->pid = pid;
        }
        void pidController::setPID(float p, float i, float d){
            setPID(float3(p,i,d));
        }

        float pidController::nextError(float nextError){
            error.x = nextError;
            error.y += error.x;
            error.z -= error.x;
            fixValue = (pid * error).sum();
            error.z = error.x;
            return fixValue;
        }
        /// @brief you need to define onCalculateError() and onUpdated() before calling update()
        void pidController::update(){
            nextError(onCalculateError());
            onUpdated(fixValue);
        }

        float3 pidController::getPID(){
            return this->pid;
        }
        float3 pidController::getError(){
            return this->error;
        }
        float pidController::getFixValue(){
            return this->fixValue;
        }
}