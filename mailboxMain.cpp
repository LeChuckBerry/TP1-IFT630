/*
Test de l'impl�mentation d'un canal de communication de type mailbox
*/

#include <iostream>
#include <chrono>
#include <thread>
#include "mailbox.h"
#include "sender.h"
#include "receiver.h"
#include "utils.h"
using namespace std::chrono_literals;

size_t TOTAL_NB_ITEMS = 1000000;
mailbox<int> items(5);
mailbox<int> done(1);
sender<int> itemSender(&items);
receiver<int> itemReceiver(&items);
sender<int> doneSender(&done);
receiver<int> doneReceiver(&done);

Semaphore coutSem = Semaphore(1);

void print(std::string message){
    coutSem.P();
    std::cout << message << std::endl;
    coutSem.V();
}


void consume()
{
	for (size_t i = 0; i < TOTAL_NB_ITEMS; i++)
	{
		int n;
		itemReceiver.receive(n);
        std::this_thread::sleep_for(2s);
        print("consumed : " + std::to_string(n));

    }
	doneSender.send(1);
}

void produce() 
{
	for (size_t i = 0; i < TOTAL_NB_ITEMS; i++)
	{
		itemSender.send(i);
        print("produced : " + std::to_string(i));

    }
}

int main()
{
	std::cout << "Staring" << std::endl;
	std::thread t2(consume);
	std::thread t3(produce);
	int p;
	doneReceiver.receive(p);
	std::cout << "Done" << std::endl;
	t2.join();
	t3.join();
	return 0;
}
