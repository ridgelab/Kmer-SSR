/*
	Brandon Pickett
	FastaSeqeunces.h
*/

#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <cstddef>
#include <cstdint>
#include <semaphore.h>

using namespace std;

class FastaSequences
{
private:
	queue<string> headers;
	queue<string> sequences;
	queue<uint32_t> ignore_chars_offsets;
	sem_t lock;
	bool dried_up_source;
	uint32_t dry_marker;

public:
	FastaSequences();
	~FastaSequences();
	bool isDriedUp() const;
	bool empty();
	void dryUp();
	void add(string header, string sequence, uint32_t ignore_chars_offset);
	uint32_t get(string &header, string &sequence, uint32_t &ignore_chars_offset);
	uint32_t size() const;
};
