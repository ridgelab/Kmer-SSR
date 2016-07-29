/*
	Brandon Pickett
	TaskQueue.h
*/

#pragma once

#include <iostream>
#include <string>
#include <sstream> // stringstream
#include <cstdlib> // exit
#include <cstdint>
#include <queue> // queue
#include <semaphore.h> // semaphore

#include "Task.h"

using namespace std;

class TaskQueue
{
private:
	queue<Task*>* tasks;
	//uint32_t max_capacity; // upper size limit on the queue [default: 1,000]
	sem_t lock;
	sem_t unfinished_business; // n, the number of tasks still waiting in the queue for analysis
	sem_t space_available; // e, the space available for the addition of more tasks

public:
	TaskQueue();
	TaskQueue(uint32_t max_capacity);
	~TaskQueue();
	void setUp(uint32_t max_capacity);
	uint32_t size() const;
	bool empty() const;
	void add(Task* task);
	Task* get();
	void clear();
	string toString();
	string toJson();
};
