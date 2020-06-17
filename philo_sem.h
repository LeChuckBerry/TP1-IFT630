#ifndef TP1_IFT630_PHILO_SEM_H
#define TP1_IFT630_PHILO_SEM_H

#include <thread>
#include "semaphore.h"

class Chopstick{
    private:
        Semaphore lock;
        Semaphore doneUsing;
        const int id;
        int owner;
        bool dirty;

    public:
        void askOwnsership(const int id);
        void giveUpOwnership();
        void release();
        void take();
        Chopstick(int id, int owner);
};

    class Philosopher{
    private :
        const std::string name;
        const int id;
        Chopstick& leftChop;
        Chopstick& rightChop;
        std::thread thread;
        bool dining;
        Semaphore done;
    public:
        Philosopher(const std::string &name, int id, Chopstick &chopstick1, Chopstick &chopstick2);
        void dine();
        void eat();
        void think();
        void leaveTable();
};

#endif //TP1_IFT630_PHILO_SEM_H
