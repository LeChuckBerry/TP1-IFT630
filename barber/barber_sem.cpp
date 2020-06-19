#include "../sync_primitives/semaphore.h"
#include"thread"
#include <iostream>
#include "../utils.h"
using std::string;
using std::cout;
using std::chrono::milliseconds;
using namespace cppUtils;
const int NB_OF_WAITING_CHAIRS = 5;
const int TOTAL_NUM_CLIENTS = 30; 
int customerCount;



Semaphore readyToCut(0);
Semaphore haircutDone(0);
Semaphore customerQueue(0);
Semaphore chair(0);
Semaphore customerMutex(1);
Semaphore coutSem(1);
Semaphore shopClosed(0);


std::thread customers[TOTAL_NUM_CLIENTS];

void print(const string &s) {
    coutSem.P();
    std::cout << s << std::endl;
    coutSem.V();
}

class Barber{

    Semaphore& readyToCut;
    Semaphore& haircutDone;
    Semaphore& customerQueue;
    Semaphore& chair;
    Semaphore& shopClosed;
    bool shopOpen = true;
    std::thread thread;

public:
    Barber(Semaphore &readyToCut, Semaphore &haircutDone, Semaphore &customerQueue, Semaphore &chair, Semaphore &shopClosed) :
            readyToCut(readyToCut), haircutDone(haircutDone), customerQueue(customerQueue),
            chair(chair), thread(&Barber::serveCustomers, this), shopClosed(shopClosed) {}

    void closeShop(){
        print("Shop is closing, serving last clients");
        shopOpen = false;
        if (customerCount == 0){
            customerQueue.V();
        }
        shopClosed.P();
        print("Shop closed");
        thread.join();
    }

private:

    void serveCustomers(){

        while(shopOpen || customerCount > 0){
            // Wait for a customer to show up
            if(customerCount == 0)
                print("Barber is asleep");
            customerQueue.P();

            if (!shopOpen && customerCount == 0){
                break;
            }

            // Signal next customer
            print("Barber calls next customer");
            readyToCut.V();
            // Waiting for next client to get on the chair
            print("Barber waits for costumer to sit on the chair");
            chair.P();
            // Executing haircut
            cutHair();

            // Waiting for client to vacate chair
            print("Barber waiting for client to leave chair");
            chair.P();
        }
        shopClosed.V();
    }

    void cutHair(){
        print("Barber is currently cutting hair");
        std::this_thread::sleep_for(milliseconds(random<uint16_t>(500, 3000)));
        haircutDone.V();
    }

};

void customer(int clientID){
    customerMutex.P();
    if(customerCount < NB_OF_WAITING_CHAIRS ){
       customerCount++;
       customerQueue.V();
       customerMutex.V();

       // Waiting for the barber to be ready
       print("Customer "  + std::to_string(clientID) + " entered the barber shop and takes a chair in the waiting room");
       readyToCut.P();

       print("Customer " + std::to_string(clientID) + " is sitting in the barber chair");
       chair.V();

       // Waiting for barber to finish haircut
       haircutDone.P();
       // Costumer
       customerCount --;
       print("Customer " + std::to_string(clientID) + " is leaving to barber shop");
       chair.V();
    }

    else{
        print("Shop full ... Customer " + std::to_string(clientID) + " is going away" );
        customerMutex.V();
    }
}




int main(){

    int customerID = 0;
    Barber barber = Barber(readyToCut, haircutDone, customerQueue, chair, shopClosed);

	// Simuler le barbier qui dors en attendant des clients 
	std::this_thread::sleep_for(milliseconds(20));

    for (int i = 0; i < TOTAL_NUM_CLIENTS; i++){
        // Pour simuler un grand achalandage et des clients qui quittent à la fin 
		if (i >= 5 && i<= TOTAL_NUM_CLIENTS - 8){
            std::this_thread::sleep_for(milliseconds(random<uint16_t>(1000, 2000)));
        }
        customers[i] = std::thread(customer, customerID++);
    }

    for (int i = 0; i < TOTAL_NUM_CLIENTS; i++){
        customers[i].join();
    }

	// Pour que le programme termine
    barber.closeShop();

    return 0;
}


