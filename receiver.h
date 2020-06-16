#pragma once

/*
// Foward Declaration
template<typename Data>
class mailbox;
*/
#include "mailbox.h"

template<typename Data>
class receiver
{
private:
	mailbox<Data>* mbox;

public:
	receiver(mailbox<Data>* box)
	{
		mbox = box;
	}
	~receiver()
	{
		mbox = nullptr;
	}

	void receive(Data& popped_value)
	{
		mbox->pop(popped_value);
	}
};