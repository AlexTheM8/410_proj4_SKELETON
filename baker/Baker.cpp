/**
 * Author: Alexander Rhodes
 * March 21, 2019
 */
#include <mutex>

#include "../includes/baker.h"
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
	for (int i = 0; i < numBoxes; i++) {
		Box box;
		DONUT donut;
		//Add until out of donuts or box is full
		while (numDonuts > 0 && box.addDonut(donut)) {
			numDonuts--;
		}
		//Add to the order's completed boxes
		anOrder.boxes.push_back(box);
	}

}

void Baker::beBaker() {

}
