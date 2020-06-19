#include <iostream>
#include "../sync_primitives/comm_channel.h"
#include"thread"
#include "../utils.h"

using std::string;
using std::chrono::milliseconds;
using std::chrono::seconds;
using namespace cppUtils;

const int TOTAL_PASSENGERS = 5;
const int TOTAL_CLIENTS = 20;

int passengerCount = 0;

MailboxCommChannel<bool> emptySeat(TOTAL_CLIENTS);
MailboxCommChannel<bool> carEmpty(1);
MailboxCommChannel<bool> carFull(1);
MailboxCommChannel<bool> passengerSync(1);
MailboxCommChannel<bool> canGetOut(TOTAL_CLIENTS);
MailboxCommChannel<string> printChannel(10);

int main(){
    std::cout << "Nom implemente" << std::endl;
}



