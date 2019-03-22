/**
 * Author: Alexander Rhodes
 * March 21, 2019
 */
#include <mutex>
#include <vector>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id) :
		id(id) {
}

Baker::~Baker() {
}

void Baker::bake_and_box(ORDER &anOrder) {
	// # of boxes = # of donuts / 12, +1 due to rounding
	int numBoxes = (anOrder.number_donuts / 12) + 1;
	int numDonuts = anOrder.number_donuts;
	//For each box, add donuts until full, then move to the next box
	vector<Box> tempVect;
	for (int i = 0; i < numBoxes; i++) {
		Box box;
		DONUT donut;
		//Add until out of donuts or box is full
		while (numDonuts > 0 && box.addDonut(donut))
			numDonuts--;
		//Add to the order's completed boxes
		//tempVect.push_back(box);
		anOrder.boxes.push_back(box); //TODO ERROR SECTION
	}
	//anOrder.boxes = tempVect; //TODO check why you can't access vector directly
}

void Baker::beBaker() {
	//Wait until there's an order or waiter is finished
	while(!order_in_Q.empty() || !b_WaiterIsFinished) {
		//Lock to wait for CV
		unique_lock<mutex> lockOrderIn(mutex_order_inQ);

		//Wait until there is an order or waiter is done
		while (order_in_Q.empty() && !b_WaiterIsFinished)
			cv_order_inQ.wait(lockOrderIn);

		//If there is an order, bake time
		if (!order_in_Q.empty()) {
			//Bake first order
			bake_and_box(order_in_Q.front());

			//Lock out vector changes
			unique_lock<mutex> lockOrderOut(mutex_order_outQ);
			order_out_Vector.push_back(order_in_Q.front());
			lockOrderOut.unlock();

			order_in_Q.pop();

			//Unlock and notify
			lockOrderIn.unlock();
			cv_order_inQ.notify_all();
		}
	}
}
