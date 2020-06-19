#include"thread"
#include <iostream>
#include "../utils.h"
#include "../sync_primitives/monitor.h"

using std::string;
using std::chrono::milliseconds;
using std::chrono::seconds;
using namespace cppUtils;

const int TOTAL_PASSENGERS = 5;
const int TOTAL_CLIENTS = 20;
int passengerCount = 0;


Semaphore_Monitor emptySeats = Semaphore_Monitor();
Semaphore_Monitor carEmpty = Semaphore_Monitor();
Semaphore_Monitor carFull = Semaphore_Monitor();
Semaphore_Monitor passengerMutex = Semaphore_Monitor();
Semaphore_Monitor rideOver = Semaphore_Monitor();
Semaphore_Monitor canGetOut = Semaphore_Monitor();
Printer_Mon printer = Printer_Mon();

void leaveCar(int pNum){
    passengerMutex.P();
    printer.printLine("Passenger : " + std::to_string(pNum) + " is leaving car");
    std::this_thread::sleep_for(milliseconds(random<uint16_t>(500, 1000)));
    passengerCount--;
    if(passengerCount == 0){
        carEmpty.V();
    }
    passengerMutex.V();
}

void passenger(int pNum){
    do {
        printer.printLine("Passenger " + std::to_string(pNum) + " is waiting in line");
        emptySeats.P();
        passengerMutex.P();
        printer.printLine("Passenger " + std::to_string(pNum) + " is entering the ride");
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
        printer.printLine("Car is full. Departure");
        for (int i = 0; i < 5; i++) {
            printer.printLine("Lap n. " + std::to_string(i));
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
    // Les sémaphores devraient êtres à zéro au départ;
    emptySeats.P();
    carFull.P();
    rideOver.P();
    canGetOut.P();

    // Create passengers;
    std::thread clients[TOTAL_CLIENTS];
    for (int i = 0; i < TOTAL_CLIENTS; i++){
        clients[i] = std::thread(passenger, i);
    }
    std::thread roller_coaster = std::thread(rollerCoaster);

    while (true){
        carEmpty.P();
        printer.printLine("Car is now empty. Beginning passenger boarding");
        fillSeats(TOTAL_PASSENGERS);
        //
        rideOver.P();
        printer.printLine("Ride is over. Passengers can now safely get out of the car");
        for (int i = 0; i <TOTAL_PASSENGERS; i++){
            canGetOut.V();
        }

    }

}

