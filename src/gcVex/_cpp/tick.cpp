#include "../control/tick.h"

namespace gcVex{
    
    int tick::realTick;
    float tick::lastUpdateTime;
    float tick::updateFrequence; // msec
    bool tick::isClosed;
    tickTask* tick::updateRoot;
    tickTask* tick::lastestUpdateFunc;
    vex::timer tick::timer;
    vex::task tick::updateTask;

    #pragma region TickTask

    tickTask* tick::newTask(std::function<bool()> func){
        tickTask* newTask = (tickTask*) malloc(sizeof(tickTask));
        newTask->isPause = false;
        newTask->nextTaskPtr = nullptr;
        newTask->lastTaskPtr = nullptr;
        newTask->taskFunc = func;
        return newTask;
    }
    void tick::pauseTask(tickTask* task){task->isPause = true;}
    void tick::resumeTask(tickTask* task){task->isPause = false;}
    void tick::colseTask(tickTask* task){
        if(countTask()==1){
            updateRoot = nullptr;
            lastestUpdateFunc = nullptr;
        }
        else{
            tickTask* last = task->lastTaskPtr;
            tickTask* next = task->nextTaskPtr;
            if(last != nullptr)
                last->nextTaskPtr = next;
            if(next != nullptr)
                next->lastTaskPtr = last;
        }
        free(task);
    }

    #pragma endregion


    int tick::update(){
        tickTask* updating;
        while (!isClosed)
        {
            if((timer.time(vex::timeUnits::msec) - lastUpdateTime) > updateFrequence){
                realTick++;
                lastUpdateTime += updateFrequence;
                updating = updateRoot;
                while (updating != nullptr){
                    tickTask* next = updating->nextTaskPtr;
                    if(!updating->isPause)
                        if(updating->taskFunc())
                            colseTask(updating);

                    updating = next;
                }
            }
            vex::task::yield();
        }
        return 0;
    }

    void tick::init(){
        realTick = 0;
        lastUpdateTime = 0;
        updateFrequence = 10; // msec
        isClosed = false;
        updateRoot = nullptr;
        lastestUpdateFunc = nullptr;
        timer = vex::timer();
        timer.reset();

        updateTask = vex::task(update);
    }

    int tick::getTick(){return realTick;}
    int tick::getTime(){return (int)timer.time(vex::timeUnits::msec);}
    int tick::countTask(){
        tickTask* temp = updateRoot;
        int output = 0;
        while (temp != nullptr){
            output++;
            temp = temp->nextTaskPtr;
        }
        return output;
    }

    tickTask* tick::addTask(std::function<bool()> func){
        tickTask* task = newTask(func);
        if(lastestUpdateFunc == nullptr){
            updateRoot = task;
            lastestUpdateFunc = task;
        }
        else{
            lastestUpdateFunc->nextTaskPtr = task;
            task->lastTaskPtr = lastestUpdateFunc;
            lastestUpdateFunc = task;
        }
        return task;
    }

    void tick::debug(){
        printf("-----Tick System State-----\n");
        printf("> Task Count   : %d\n",countTask());
        printf("> Current Time : %d\n",(int)timer.time(vex::timeUnits::sec));
        printf("> Current Tick : %d\n",realTick);
    }
}