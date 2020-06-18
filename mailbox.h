#ifndef TP1_IFT630_MAILBOX_H
#define TP1_IFT630_MAILBOX_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include "semaphore.h"
/*
#include "receiver.h"
#include "sender.h"
*/

//Foward Declaration
template<typename Data>
class sender;
template<typename Data>
class receiver;


template<typename Data>
class mailbox
{
private:
	int max_size;
	std::queue<Data> que;
	Semaphore pushMutex;
	Semaphore popMutex;
	Semaphore availableSpace;
	Semaphore availableMessage;

public:

	mailbox(int n): max_size(n), pushMutex(Semaphore(1)), popMutex(Semaphore(1)),
	                availableSpace(Semaphore(n - 1)), availableMessage(Semaphore(0)) {}

	~mailbox(){}


	void push(Data data)
	{
		// Verouille la file
		pushMutex.P();
        availableSpace.P();
		// Mise en file du message

		que.push(data);
		availableMessage.V();
		pushMutex.V();
	}

	// Retirer un message 
	void pop(Data& popped_value)
	{
		popMutex.P();

		// Attendre qu'il y ait un message
		availableMessage.P();
		popped_value = que.front();
		que.pop();
        availableSpace.V();

		popMutex.V();
	}

};
#endif //TP1_IFT630_MAILBOX_H