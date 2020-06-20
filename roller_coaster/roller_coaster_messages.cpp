#include <iostream>
#include "../sync_primitives/comm_channel.h"
#include"thread"
#include "../utils.h"

using std::string;
using std::chrono::milliseconds;
using std::chrono::seconds;
using namespace cppUtils;

const int TOTAL_PASSENGERS = 5;
const int TOTAL_CLIENTS = 10;

int passengerCount = 0;

MailboxCommChannel<bool> emptySeat(TOTAL_CLIENTS);
MailboxCommChannel<bool> carEmpty(1);
MailboxCommChannel<bool> carFull(1);
MailboxCommChannel<bool> passengerSync(1);
MailboxCommChannel<bool> rideOver(1);
MailboxCommChannel<bool> canGetOut(TOTAL_CLIENTS);

Printer_Sem printer = Printer_Sem();

bool b;

void leaveCar(int pNum)
{
    passengerSync.getReceiver()->receive(b);
    printer.printLine("Passenger " + std::to_string(pNum) + " is leaving car");
    passengerCount--;
    if (passengerCount == 0)
    {
        carEmpty.getSender()->send(b);
    }
    passengerSync.getSender()->send(b);

}

void passenger(int pNum)
{
    do
    {
        printer.printLine("Passenger " + std::to_string(pNum) + " is waiting in line");
        emptySeat.getReceiver()->receive(b);
        passengerSync.getReceiver()->receive(b);
        printer.printLine("Passenger " + std::to_string(pNum) + " is entering the ride");
        std::this_thread::sleep_for(milliseconds(random<uint16_t>(500, 1000)));
        passengerCount++;

        if (passengerCount == TOTAL_PASSENGERS)
        {
            carFull.getSender()->send(b);
        }
        passengerSync.getSender()->send(b);
        canGetOut.getReceiver()->receive(b);
        leaveCar(pNum);
    } while (true);
}

void rollerCoaster()
{
    while (true)
    {
        carFull.getReceiver()->receive(b);
        printer.printLine("Car is full. Departure");
        for (int i = 0; i < 5; i++)
        {
            printer.printLine("Lap n. " + std::to_string(i));
            std::this_thread::sleep_for(seconds(1));
        }
        rideOver.getSender()->send(b);
    }
}

void fillSeats(int numberOfSeats)
{
    for (int i = 0; i < numberOfSeats; i++)
    {
        emptySeat.getSender()->send(b);
    }
}

int main()
{
    std::thread clients[TOTAL_CLIENTS];
    for (int i = 0; i < TOTAL_CLIENTS; i++)
    {
        clients[i] = std::thread(passenger, i);
    }
    std::thread roller_coaster = std::thread(rollerCoaster);

    // Pas exactement certain pourquoi mais sans ces 2 send, il y a un deadlock
    carEmpty.getSender()->send(b);
    passengerSync.getSender()->send(b);

    while (true)
    {
        carEmpty.getReceiver()->receive(b);
        printer.printLine("Car is now empty. Beginning passenger boarding");
        fillSeats(TOTAL_PASSENGERS);

        rideOver.getReceiver()->receive(b);
        printer.printLine("Ride is over. Passengers can now safely get out of the car");
        for (int i = 0; i < TOTAL_PASSENGERS; i++)
        {
            canGetOut.getSender()->send(b);
        }
    }
}
