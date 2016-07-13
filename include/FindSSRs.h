/*	
	Brandon Pickett
	FindSSRs.h
*/

#pragma once

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <semaphore.h>
#include <pthread.h>
#include <memory>

#include "Arguments.h"
#include "Results.h"
#include "SingleResult.h"
#include "FastaSequences.h"
#include "OutputFile.h"
#include "ProgressMeter.h"

using namespace std;

class FindSSRs
{
private:
	//Arguments args;
	Arguments* args;
	FastaSequences fasta_seqs; // thread-safe data structure
	vector<pthread_t> threads;
	uint32_t num_threads;
	OutputFile out_file; // thread-safe class
	sem_t n; // full space (for the consumer to take) in the buffer...if n increases/decreases, e should decrease/increase
	sem_t e; // empty space (for the producer to fill) in the buffer...if e increases/decreases, n should decrease/increase
	sem_t d; // finished semaphores
	sem_t s; // signal finished 
	uint32_t finished_threads;
	ProgressMeter progress_bar;
	ofstream out;
	sem_t lock;

	bool isGood(const string &base, uint32_t repeats, uint32_t position, const vector<bool> &filter, uint32_t offset) const;

public:
	FindSSRs(Arguments* _args);
	~FindSSRs();
	sem_t* getN() const;
	sem_t* getE() const;
	sem_t* getD() const;
	sem_t* getS() const;
	sem_t* getLock() const;
	uint32_t getFinishedThreadsCount() const;
	void incrementFinishedThreads();
	uint32_t run();
	uint32_t makeThreads();
	void joinAndForgetAllThreads();
	void processInput();
	void processSequence(const string &header, string &sequence);
	//void findSSRsInSequence(const string &header, const string &sequence);
	void findSSRsInSequence(const string &header, const string &sequence, uint32_t ignore_chars_offset);
	//void findSSRsInSA(const string &header, const string &sequence, const int *SA, const int *LCP);
	//void findSSRsInSA(const string &header, const string &sequence, const int *SA, const int *LCP, vector<Results> &res_vec);
	void findSSRsInSA(const string &header, const string &sequence, const int *SA, const int *LCP, uint32_t ignore_chars_offset);
	void splitStringOnIgnoredChars(vector<uint32_t> &starts, vector<uint32_t> &sizes, const string &sequence, uint32_t &actually_ignored_chars);
	void printExtraInformation(const string &header, const string &sequence, const int *SA, const int *LCP);
	static void* consume(void* find_ssrs);
};
