#ifndef TP1_IFT630_MONITOR_H
#define TP1_IFT630_MONITOR_H
#include "semaphore.h"


struct ConditionVar {

    Semaphore &sem_wait;
    Semaphore &sem_next;
    Semaphore &mutex;
    int &wait_count;
    int &next_count;

    void wait() {
        wait_count++;
        if (next_count > 0)
            sem_next.V();
        else
            mutex.V();
        sem_wait.P();
        wait_count--;
    }

    void signal() {
        if (wait_count > 0) {
            next_count++;
            sem_wait.V();
            sem_next.P();
            next_count--;
        }
    }

public :
    ConditionVar(Semaphore &mutex, Semaphore &semNext, Semaphore &semWait, int &nextCount, int &waitCount)
            : mutex(mutex), sem_next(semNext), sem_wait(semWait), next_count(nextCount), wait_count(waitCount) {}
};
class Semaphore_Monitor{

    Semaphore mutex = Semaphore(1);
    Semaphore next = Semaphore(0);
    Semaphore wait = Semaphore(0);
    bool busy = false;
    ConditionVar available;
    int nextCount = 0;
    int waitCount = 0;


    // Procedure to enter function
    void enter(){
        mutex.P();
    }
    // Procedure to leave function
    void leave(ConditionVar var){
        if (nextCount > 0 )
            next.V();
        else
            mutex.V();
    }

public:

    void P(){
        enter();
        if (busy) available.wait();
        busy = true;
        leave(available);
    }

    void V(){
        enter();
        busy = false;
        available.signal();
        leave(available);
    }

    Semaphore_Monitor(): available(mutex, next, wait,  nextCount, waitCount) {}

};





#endif //TP1_IFT630_MONITOR_H
