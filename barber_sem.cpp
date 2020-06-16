#include "semaphore.h"
#include"thread"
#include <iostream>
#include "utils.h"
using std::string;
using std::cout;
using std::chrono::milliseconds;
using namespace cppUtils;
const int MAX_COSTUMER = 5;
int customerCount;

Semaphore coutSem(1);
void print(const string &s) {
    coutSem.P();
    std::cout << s << std::endl;
    coutSem.V();
}

Semaphore readyToCut(0);
Semaphore haircutDone(0);
Semaphore customerQueue(0);
Semaphore chair(0);
Semaphore customerMutex(1);


class Barber{

    Semaphore& readyToCut;
    Semaphore& haircutDone;
    Semaphore& customerQueue;
    Semaphore& chair;
    std::thread thread;

public:
    Barber(Semaphore &readyToCut, Semaphore &haircutDone, Semaphore &customerQueue, Semaphore &chair):
    readyToCut(readyToCut), haircutDone(haircutDone), customerQueue(customerQueue),
                                        chair(chair), thread(&Barber::serveCustomers, this) {}

private:

    void serveCustomers(){

        while(true){
            // Wait for a customer to show up
            if(customerCount == 0
            ){
                print("Barber is asleep");
            }
            customerQueue.P();

            // Signal next customer
            print("Barber calls next customer");
            readyToCut.V();
            // Waiting for next client to get on the chair
            print("Barber waits for costumer to sit on the chair ");
            chair.P();
            // Executing haircut
            cutHair();

            // Waiting for client to vacate chair
            print("Barber waiting for client to leave chair");
            chair.P();
        }
    }

    void cutHair(){
        print("Barber is currently cutting hair");
        std::this_thread::sleep_for(milliseconds(500));
        haircutDone.V();
    }

};




void customer(int clientID){
    customerMutex.P();
    if(customerCount < MAX_COSTUMER ){
       customerCount++;
       customerQueue.V();
       customerMutex.V();

       // Waiting for the barber to be ready
       print("Customer "  + std::to_string(clientID) + " is waiting for the barber to call");
       readyToCut.P();

       print("Customer " + std::to_string(clientID) + "is sitting in the barber chair");
       chair.V();

       // Waiting for barber to finish haircut
       haircutDone.P();
       // Costumer
       customerCount --;
       print("Customer " + std::to_string(clientID) + "is leaving to barber shop");
       chair.V();
    }

    else{
        print("Shop full ... Customer " + std::to_string(clientID) + " is going away" );
        customerMutex.V();
    }

}


int main(){

    int customerID = 0;

    Barber barber(readyToCut, haircutDone, customerQueue, chair);

    std::thread customers[25];
    // Fill the waiting room

    for (int i = 0; i <= 25; i++){
        if (i >= 5){
            std::this_thread::sleep_for(milliseconds(random<uint16_t>(100, 2000)));
        }
        customers[i] = std::thread(customer, customerID++);
    }


    for(auto i = 0; i < 25; i++){
        customers[i].join();
    }
    std::this_thread::sleep_for(milliseconds(200000));
    return 0;
}


