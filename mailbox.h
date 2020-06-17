#ifndef TP1_IFT630_MAILBOX_H
#define TP1_IFT630_MAILBOX_H

#include <mutex>
#include <condition_variable>
#include <queue>

/*
#include "receiver.h"
#include "sender.h"
*/

// Implémentation d'un canal de IPC style boitre aux lettres
// Grandement inspiré de l'article suivant:
// https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html

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
	std::mutex mtx;
	std::condition_variable cv;

public:

	mailbox(int n)
	{
		max_size = n;
	}
	~mailbox(){}

	// Essai de pousser un message
	bool try_push(Data const& data)
	{
		// On essaye de vérouiller la file
		if (mtx.try_lock())
		{
			// Si la file n'est pas pleine
			if (que.size() < max_size)
			{
				// Verouillage de la file
				//std::unique_lock<std::mutex> lock(mtx);

				// Envoi du message
				que.push(data);

				// Deverouillage et signalement
				mtx.unlock();
				cv.notify_one();

				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}
	}

	void push(Data const& data)
	{
		// Verouille la file
		std::unique_lock<std::mutex> lock(mtx);

		// Attendre qu'il y ait de la place dans la file
		while (que.size() >= max_size)
		{
			cv.wait(lock);
		}

		// Envoi du message
		que.push(data);

		// Deverouillage et signalement
		lock.unlock();
		cv.notify_one();
		
	}

	bool empty() const
	{
		std::unique_lock<std::mutex> lock(mtx);
		return que.empty();
	}

	bool full() const
	{
		std::unique_lock<std::mutex> lock(mtx);
		return que.size == max_size;
	}

	//Essai de retirer un message
	bool try_pop(Data& popped_value)
	{
		// Verouille la file
		std::unique_lock<std::mutex> lock(mtx);

		// Verifie si la file est vide
		if (que.empty())
		{
			return false;
		}
		else
		{
			popped_value = que.front();
			que.pop();
			return true;
		}
	}
	
	// Retirer un message 
	void pop(Data& popped_value)
	{
		// Verouille la file
		std::unique_lock<std::mutex> lock(mtx);

		// Attendre qu'il y ait un message
		while (que.empty())
		{
			cv.wait(lock);
		}
		popped_value = que.front();
		que.pop();
	}

};
#endif //TP1_IFT630_MAILBOX_H