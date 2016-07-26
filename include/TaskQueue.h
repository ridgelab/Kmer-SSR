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
	uint32_t max_capacity; // upper size limit on the queue [default: 1,000]
	sem_t lock;

public:
	TaskQueue();
	TaskQueue(uint32_t size);
	~TaskQueue();
	void setUp();
	uint32_t size() const;
	bool empty() const;
	void add(Task* task);
	Task* get();
	void clear();
	string toString();
	string toJson();
};
