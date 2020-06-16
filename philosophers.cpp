#include "philosophers.h"
#include <iostream>
#include <chrono>
#include "utils.h"
#include "semaphore.h"
#include <thread>

// Implémentation de la solution de Chandy/Misra avec des sémaphores

using namespace std::chrono_literals;
using namespace cppUtils;
using std::chrono::milliseconds;
using std::string;

Semaphore coutSem(1);
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
    leftChop.pickUP(id);
    print(name + " picked up left chopstick");
    rightChop.pickUP(id);
    print(name + " picked up right chopstick");
    print(name + " is eating");
    std::this_thread::sleep_for(milliseconds(1300));
    // On les dépose
    leftChop.release();
    rightChop.release();
}

void Philosopher::think() {
    print(name + " is thinking... ");
    // Think for
    std::this_thread::sleep_for(milliseconds(random < uint16_t > (1000, 1100)));
}

void Philosopher::leaveTable() {
    print(name + " is preparing to leave");
    dining = false;
    leftChop.release();
    rightChop.release();
    done.P();
    print(name + " left the table");
    thread.join();
}


void Chopstick::pickUP(int id) {
    // Si la demande est faite par qqun d'autre
    while (owner != id) {
        // Si le chopstick est sale, c'est que le philosophe l'a déjà utilisé.
        // Il doit donc le céder après avoir fini de manger
        if (dirty) {
            lock.P();
            dirty = false;
            owner = id;
        }
            // S'il est propre c'est que le philosophe qui l'a présentement n'a pas encore
            // mangé donc on doit attendre qu'il appelle release
        else {
            lock.P();
        }
    }
    // Si le philosophe a déjà la possession de la baguette et que personne ne l'a demandé, pas d'attente
}

void Chopstick::release() {
    // Lorsqu'on a fini avec la baguette, elle devient sale
    dirty = true;
    // On relâche la sémaphore pour qu'un voisin demandant puisse s'en servir
    lock.V();
}

Chopstick::Chopstick(int id, int owner) :
        lock(1), id(id), owner(owner), dirty(true) {}


int main() {

    Chopstick chopstick1(1, 1);
    Chopstick chopstick2(2, 2);
    Chopstick chopstick3(3, 3);
    Chopstick chopstick4(4, 4);
    Chopstick chopstick5(5, 1);

    Philosopher philosophers[5] = {
            Philosopher("Arendt", 1, chopstick1, chopstick2),
            Philosopher("Beauvoir", 2, chopstick2, chopstick3),
            Philosopher("Aspasie", 3, chopstick3, chopstick4),
            Philosopher("Butler", 4, chopstick4, chopstick5),
            Philosopher("Hypatie", 5, chopstick5, chopstick1)};

    std::this_thread::sleep_for(std::chrono::seconds(10));

    for (auto &phil : philosophers) {
        phil.leaveTable();
    }

    print("Dinner finished");
    return 0;


}
