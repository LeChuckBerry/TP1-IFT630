#include "semaphore.h"
#include"thread"
#include <iostream>
#include "utils.h"
#include "monitor.h"

using std::string;
using std::chrono::milliseconds;
using std::chrono::seconds;
using namespace cppUtils;

const int TOTAL_PASSENGERS = 5;
int passengerCount = 0;

Semaphore_Monitor coutSem;
void print(const string &s) {
    coutSem.P();
    std::cout << s << std::endl;
    coutSem.V();
}

Semaphore_Monitor emptySeats;
Semaphore_Monitor carEmpty;
Semaphore_Monitor carFull;
Semaphore_Monitor passengerMutex;
Semaphore_Monitor rideOver;
Semaphore_Monitor canGetOut;

void leaveCar(int pNum){
    passengerMutex.P();
    print("Passenger : " + std::to_string(pNum) + " is leaving car");
    std::this_thread::sleep_for(milliseconds(random<uint16_t>(500, 1000)));
    passengerCount--;
    if(passengerCount == 0){
        carEmpty.V();
    }
    passengerMutex.V();
}

void passenger(int pNum){
    do {
        print("Passenger" + std::to_string(pNum) + "is waiting in line");
        emptySeats.P();
        passengerMutex.P();
        print("Passenger " + std::to_string(pNum) + " is entering the ride");
        std::this_thread::sleep_for(milliseconds(random<uint16_t>(500, 1000)));
        passengerCount++;
        if(passengerCount == TOTAL_PASSENGERS){
            carFull.V();
        }
        passengerMutex.V();
        canGetOut.P();
        leaveCar(pNum);
    }
    while (true);

}


void rollerCoaster(){
    while (true) {
        carFull.P();
        print("Car is full. Departure");
        for (int i = 0; i < 5; i++) {
            print("Lap n. " + std::to_string(i));
            std::this_thread::sleep_for(seconds(1));
        }
        rideOver.V();
    }
}

void fillSeats(int numberOfSeats){
    for (int i = 0; i <numberOfSeats; i++){
        emptySeats.V();
    }

}

int main(){

    // Create passengers;
    auto totalClients = random<uint16_t>(TOTAL_PASSENGERS +1 , TOTAL_PASSENGERS*3);
    std::thread clients[totalClients];
    for (int i = 0; i < totalClients; i++){
        clients[i] = std::thread(passenger, i);
    }
    std::thread roller_coaster = std::thread(rollerCoaster);
    while (true){
        carEmpty.P();
        print("Car is now empty. Beginning passenger boarding");
        fillSeats(TOTAL_PASSENGERS);
        //
        rideOver.P();
        print("Ride is over. Passengers can now safely get out of the car");
        for (int i = 0; i <TOTAL_PASSENGERS; i++){
            canGetOut.V();
        }

    }

}

