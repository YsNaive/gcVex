#ifndef GCVEX_TICK_H_
#define GCVEX_TICK_H_

#include <functional>

namespace gcVex{
    class tickTask
    {
    private:
        tickTask();


    public:
        tickTask* nextTaskPtr;
        tickTask* lastTaskPtr;
        std::function<bool()> taskFunc;

        tickTask(std::function<bool()> func);
        ~tickTask();
    };
    

    class tick
    {
    private:
        tick();
        ~tick();

        // you can see default value in init() function
        static int realTick;
        static bool isInited;
        static float lastUpdateTime;
        static float updateFrequence; // msec
        static tickTask* updateRoot;
        static vex::timer timer;

        static int update(); // lopping by using task after calling init()

    public:

        static void init();
        static int getTick();
        static float getTime();
        static tickTask addTask(std::function<bool()> func);
        static void debug();
    };
    
}

#endif
