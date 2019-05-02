#include "queryqueue.h"

QueryQueue::QueryQueue() : lock(mut) {
	available = true;
}

//launch a thread that waits for both an item in the queue and the db connection to be free
//when both of these are true, it launches a thread that writes the first item to the db
//when done, this child thread notifies the parent which then goes back to the first state
void QueryQueue::begin() {
	while (true) {

	}
}

void QueryQueue::add(QueryFactory query) {

}