#ifndef TP1_IFT630_MONITOR_H
#define TP1_IFT630_MONITOR_H
#include "semaphore.h"


struct ConditionVar {

    Semaphore &sem_wait;
    Semaphore &mutex;


    void wait() {
        mutex.V();
        sem_wait.P();
        mutex.P();
    }

    void signal() {
        sem_wait.V();
    }

public :
    ConditionVar(Semaphore &mutex, Semaphore &semWait)
            : mutex(mutex), sem_wait(semWait) {}
};
class Semaphore_Monitor{

    Semaphore mutex = Semaphore(1);
    Semaphore wait = Semaphore(0);
    bool busy = false;
    ConditionVar available;

    // Procedure to enter function
    void enter(){
        mutex.P();
    }
    // Procedure to leave function
    void leave(){
        mutex.V();
    }

public:

    void P(){
        enter();
        if (busy)
            available.wait();
        busy = true;
        leave();
    }

    void V(){
        enter();
        busy = false;
        available.signal();
        leave();
    }

    Semaphore_Monitor(): available(mutex, wait) {}

};





#endif //TP1_IFT630_MONITOR_H
