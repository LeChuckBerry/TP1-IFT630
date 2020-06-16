#pragma once

/*
// Foward Declaration
template<typename Data>
class mailbox;
*/
#include "mailbox.h"

template<typename Data>
class sender
{
private:
	mailbox<Data>* mbox;

public:
	sender(mailbox<Data>* box)
	{
		mblox = box;
	}
	~sender() 
	{
		mbox = nullptr;
	}

	void send(Data const& data)
	{
		mbox->push(data);
	}
};

