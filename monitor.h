#ifndef TP1_IFT630_MONITOR_H
#define TP1_IFT630_MONITOR_H
#include "semaphore.h"


class Semaphore_Monitor{

    Semaphore mutex;
    bool busy;
    ConditionVar available;

     struct ConditionVar{

        Semaphore sem_wait;
        Semaphore sem_next;
        int wait_count;
        int next_count;

        void wait(){
            wait_count++;
            if (next_count > 0)
                sem_next.V();
            else
                mutex.V();
            sem_wait.P();
            wait_count--;
        }

        void signal(){
            if(wait_count > 0){
                next_count++;
                sem_wait.V();
                sem_next.P();
                next_count--;
            }
        }



    };

    // Procedure to enter function
    void enter(){
        mutex.P();
    }
    // Procedure to leave function
    void leave(ConditionVar var){
        if (var.next_count > 0 )
            var.sem_next.V();
        else
            mutex.V();
    }

public:

    P(){
        enter();
        if (busy) available.wait();
        busy = true;
        leave(available);
    }

    V(){
        enter();
        occupe = faux;
        available.wait();
        leave(available);
    }

};


#endif //TP1_IFT630_MONITOR_H
