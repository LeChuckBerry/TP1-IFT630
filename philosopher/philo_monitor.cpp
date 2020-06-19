#include "philo_sem.h"
#include <iostream>
#include <chrono>
#include "../utils.h"
#include "../sync_primitives/monitor.h"
#include <thread>

// Implémentation de la solution de Chandy/Misra avec des sémaphores

using namespace std::chrono_literals;
using namespace cppUtils;
using std::chrono::milliseconds;
using std::string;

Semaphore_Monitor coutSem;
void print(const string &s) {
    coutSem.P();
    std::cout << s << std::endl;
    coutSem.V();
}


Philosopher::Philosopher(const string &name, int id, Chopstick &chopstick1, Chopstick &chopstick2) :
        name(name), id(id), leftChop(chopstick1), rightChop(chopstick2), thread(&Philosopher::dine, this), dining(true),
        done(0) {}

void Philosopher::dine() {
    do {
        think();
        eat();
    } while (dining);
    done.V();
}

void Philosopher::eat() {
    print(name + " is hungry");
    // On demande les deux baguettes
    leftChop.askOwnsership(id);
    rightChop.askOwnsership(id);

    leftChop.take();
    rightChop.take();

    print(name + " started eating");
    std::this_thread::sleep_for(milliseconds(1300));
    print(name + " finished eating");

    leftChop.giveUpOwnership();
    rightChop.giveUpOwnership();

    leftChop.release();
    rightChop.release();
}

void Philosopher::think() {
    print(name + " is thinking... ");
    // Think for
    std::this_thread::sleep_for(milliseconds(random < uint16_t > (1000, 1100)));
}

void Philosopher::leaveTable() {
    print(name + " is preparing for the last round");
    dining = false;
    leftChop.giveUpOwnership();
    rightChop.giveUpOwnership();
    done.P();
    print(name + " left the table");
    thread.join();
}


void Chopstick::askOwnsership(int id) {
    // Si la demande est faite par qqun d'autre
    while (owner != id) {
        // Si la baguette est sale, c'est que le philosophe la possédant présentement l'avais l'a déjà utilisée.
        // Ce dernier dois donc  céder après avoir fini de manger
        if (dirty) {
            lock.P();
            dirty = false;
            owner = id;
            lock.V();
        }
            // S'il est propre c'est que le philosophe qui l'a présentement n'a pas encore
            // mangé. À ce moment, on attend qu'il soit sale, c'est-
        else {
            doneUsing.P();
        }
    }
}

void Chopstick::giveUpOwnership() {
    // Lorsqu'on a fini avec la baguette, elle devient sale
    dirty = true;
    // On relâche la sémaphore pour qu'un voisin demandant puisse s'en servir
    doneUsing.V();
}

Chopstick::Chopstick(int id, int owner) :
        lock(1), id(id), owner(owner), dirty(true) {}


void Chopstick::release() {
    lock.V();
}

void Chopstick::take() {
    lock.P();
}


int main() {

    Chopstick chopstick1(1, 1);
    Chopstick chopstick2(2, 2);
    Chopstick chopstick3(3, 3);
    Chopstick chopstick4(4, 4);
    Chopstick chopstick5(5, 1);

    Philosopher philosophers[5] = {
            Philosopher("1", 1, chopstick1, chopstick2),
            Philosopher("2", 2, chopstick2, chopstick3),
            Philosopher("3", 3, chopstick3, chopstick4),
            Philosopher("4", 4, chopstick4, chopstick5),
            Philosopher("5", 5, chopstick5, chopstick1)};

    std::this_thread::sleep_for(std::chrono::seconds(10));

    for (auto &phil : philosophers) {
        phil.leaveTable();
    }

    print("Dinner finished");
    return 0;


}

