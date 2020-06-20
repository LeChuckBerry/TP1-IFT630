/*
Test de l'impl�mentation d'un canal de communication de type mailbox
*/

#include <iostream>
#include <chrono>
#include <thread>
#include "string"
#include "sync_primitives/comm_channel.h"
#include "utils.h"
#include "sync_primitives/comm_channel.h"

using namespace std::chrono_literals;

size_t TOTAL_NB_ITEMS = 1000000;
Mailbox<int> items(5);
Mailbox<int> done(1);
MessageSender<int> itemSender(&items);
MessageReceiver<int> itemReceiver(&items);
MessageSender<int> doneSender(&done);
MessageReceiver<int> doneReceiver(&done);

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
