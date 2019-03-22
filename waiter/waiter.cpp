#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id, string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder) {
	return myIO.getNext(anOrder);
}

void Waiter::beWaiter() {
	//Take order, if one exists
	ORDER order;
	while (getNext(order) == SUCCESS) {
		//Lock vector changes
		unique_lock<mutex> lockInLoop(mutex_order_inQ);
		order_in_Q.push(order); //Adding order to queue
		lockInLoop.unlock();
		cv_order_inQ.notify_all(); //Tell bakers that there's a new order
	}

	//Notify bakers that the waiter is done
	unique_lock<mutex> lockOutLoop(mutex_order_inQ);
	b_WaiterIsFinished = true;
	cv_order_inQ.notify_all();
}

