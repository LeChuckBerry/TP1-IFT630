#ifndef TP1_IFT630_COMM_CHANNEL_H
#define TP1_IFT630_COMM_CHANNEL_H

#include <queue>
#include "semaphore.h"


template<typename Data>
class Mailbox;


template<typename Data>
class MessageReceiver
{
private:
    Mailbox<Data>* mbox;

public:
    MessageReceiver(Mailbox<Data>* box)
    {
        mbox = box;
    }
    ~MessageReceiver()
    {
        mbox = nullptr;
    }

    void receive(Data& popped_value)
    {
        mbox->pop(popped_value);
    }
};


template<typename Data>
class MessageSender
{
private:
    Mailbox<Data>* mbox;

public:
    MessageSender(Mailbox<Data>* box)
    {
        mbox = box;
    }
    ~MessageSender()
    {
        mbox = nullptr;
    }

    void send(Data const& data)
    {
        mbox->push(data);
    }
};

template<typename Data>
class Mailbox
{
private:
    int max_size;
    std::queue<Data> que;
    Semaphore queueMutex;
    Semaphore availableSpace;
    Semaphore availableMessage;

private :
    Data pop_from_queue(){
        queueMutex.P();
        Data popped_value = que.front();
        que.pop();
        queueMutex.V();
        return popped_value;
    }

    void push_in_queue(const Data message){
        queueMutex.P();
        que.push(message);
        queueMutex.V();
    }

public:

	// Par d�faut, tampon de 5 
	Mailbox() : max_size(5), queueMutex(Semaphore(1)),
		availableSpace(Semaphore(5)), availableMessage(Semaphore(0)){}

    Mailbox(int n): max_size(n),  queueMutex(Semaphore(1)),
                    availableSpace(Semaphore(n)), availableMessage(Semaphore(0)) {}

    ~Mailbox(){}

    void push(const Data data)
    {

        // On attend qu'il y ait de l'espace dispo
        availableSpace.P();
        push_in_queue(data);
        availableMessage.V();
    }

    // Retirer un message
    void pop(Data& popped_value)
    {
        // Attendre qu'il y ait un message
        availableMessage.P();
        // On va chercher la prochaine valeur dans la file
        popped_value = pop_from_queue();
        // On indique qu'un espace est libre
        availableSpace.V();

    }

};

template<typename Data>
class MailboxCommChannel {

    private :
        Mailbox<Data> mailbox;
        MessageSender<Data> sender;
        MessageReceiver<Data> receiver;


    public:

        MailboxCommChannel(int capacity) : mailbox(Mailbox<Data>(capacity)),
            sender(MessageSender(&mailbox)), receiver(MessageReceiver(&mailbox)) {}

        MessageReceiver<Data> * getReceiver(){
            return &receiver;
        }

        MessageSender<Data> * getSender(){
            return &sender;
        }


};



#endif //TP1_IFT630_COMM_CHANNEL_H
