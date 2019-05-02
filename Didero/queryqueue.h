#ifndef _QUERY_QUEUE_H
#define _QUERY_QUEUE_H

#include "aliases.h"
#include "queryfactory.h"

//singleton

class QueryQueue {
	std::queue<QueryFactory> writeQueue;
	std::mutex mut;
	std::unique_lock<std::mutex> lock;
	std::condition_variable cv;
	//only one thing can write at a time
	bool available;

public:
	QueryQueue();
	void begin();
	void add(QueryFactory query);
};

#endif _QUERY_QUEUE_H