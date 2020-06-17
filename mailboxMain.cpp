/*
Test de l'impl�mentation d'un canal de communication de type mailbox
*/

#include <iostream>
#include <chrono>
#include <thread>
#include "mailbox.h"
#include "sender.h"
#include "receiver.h"

using namespace std::chrono_literals;

size_t TOTAL_NB_ITEMS = 1000000;
mailbox<int> items(1);
mailbox<int> done(1);
sender<int> itemSender(&items);
receiver<int> itemReceiver(&items);
sender<int> doneSender(&done);
receiver<int> doneReceiver(&done);

// send() = V()
// receive() = P()


void consume()
{
	for (size_t i = 0; i < TOTAL_NB_ITEMS; i++)
	{
		int n;
		itemReceiver.receive(n);
		std::cout << "consuming" << std::endl;
	}
	doneSender.send(1);
}

void produce() 
{
	for (size_t i = 0; i < TOTAL_NB_ITEMS; i++)
	{
		std::cout << "producing" << std::endl;
		std::this_thread::sleep_for(2s);
		itemSender.send(1);
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
