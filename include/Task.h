/*
	Brandon Pickett
	Task.h
*/

#pragma once

#include <iostream>
#include <string>
#include <sstream> // stringstream
#include <cstdlib> // exit
#include <cstdint>

using namespace std;

class Task
{
private:
	string identifier; // the fasta id or header
	string sequence; // the (possibly sub-)sequence to search in
	uint32_t global_pos; // the global position where this sequence starts in the entire sequence
	uint32_t global_seq_length; // the length of the entire sequence from which this sequence comes from

	string getShortenedID() const;
	string getBegAndEndOfSeq() const;

public:
	Task();
	Task(const string &_identifier, const string &_sequence, uint32_t _global_pos, uint32_t _global_seq_length);
	~Task();
	string getID() const;
	string getSequence() const;
	uint32_t getGlobalPosition() const;
	uint32_t getGlobalSequenceLength() const;
	bool equalTo(Task* other) const;
	string toString() const;
};
