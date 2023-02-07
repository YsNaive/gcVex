#ifndef GCVEX_TICK_H_
#define GCVEX_TICK_H_

#include <functional>
#include <vex.h>

namespace gcVex{
    struct tickTask
    {
        tickTask* nextTaskPtr;
        tickTask* lastTaskPtr;
        std::function<bool()> taskFunc;
        bool isPause;
    };

    class tick
    {
    private:
        tick();
        ~tick();

        // you can see default value in init() function
        static int realTick;
        static float lastUpdateTime;
        static float updateFrequence; // msec
        static bool isClosed;
        static tickTask* updateRoot;
        static tickTask* lastestUpdateFunc;
        static vex::timer timer;
        static vex::task updateTask;

        static int update(); // lopping by using task after calling init()

    public:

        static tickTask* newTask(std::function<bool()> func);
        static void pauseTask(tickTask* task);
        static void resumeTask(tickTask* task);
        static void colseTask(tickTask* task);

        static void init();
        static int getTick();
        static int getTime();
        static int countTask();
        static tickTask* addTask(std::function<bool()> func);
        static tickTask* addTask(tickTask* task);
        static void waitForAllTask();
        static void debug();
    };
    
}

#endif
