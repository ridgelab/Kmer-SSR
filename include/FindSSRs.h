/*	
	Brandon Pickett
	FindSSRs.h
*/

#pragma once

#include <iostream>
#include <cstdio>
#include <cstddef> // nullptr
#include <cstddef> // uint32_t
#include <unistd.h>
#include <sys/types.h>
#include <fstream> // ifstream,ofstream
#include <string> // string
#include <sstream> // stringstream
#include <vector> // vector
#include <semaphore.h> // sem_t
#include <pthread.h> // thread_t
#include <memory>

#include "Arguments.h"
#include "SSRcontainer.h"
#include "SSR.h"
#include "TaskQueue.h"
#include "Task.h"
#include "OutputFile.h"
#include "ProgressMeter.h"
#include "AtomicityChecker.h"

using namespace std;

class FindSSRs
{
private:
	Arguments* args;
	vector<pthread_t> threads;
	OutputFile* out_file; // thread-safe class
	ProgressMeter* progress_bar; // thread-safe class
	AtomicityChecker* atomicity_checker; // thread-safe class
	TaskQueue* tasks; // thread-safe class

private:
	uint32_t makeThreads();
	void joinAndForgetAllThreads();
	void splitStringOnIgnoredChars(vector<uint32_t> &starts, vector<uint32_t> &sizes, const string &sequence, uint32_t &actually_ignored_chars);
	void processInput();
	void processSequence(const string &header, const string &sequence);

public:
	FindSSRs(Arguments* _args);
	~FindSSRs();
	uint32_t run();
};
