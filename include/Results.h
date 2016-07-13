/*
	Brandon Pickett
	Results.h
*/

#pragma once

#include <cassert> // assert
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <cmath> // abs
#include <algorithm> // min

#include "SingleResult.h"
#include "Arguments.h"
#include "OutputFile.h"

using namespace std;

class Results
{
private:
	set<SingleResult> results;
	vector<bool> available_start_positions;
	unordered_set<string>* enumerated_ssrs;
	//string blast_result;

public:
	Results(const uint32_t &sequence_length, unordered_set<string>* _enumerated_ssrs);
	~Results();
	bool isStartPositionAvailableAt(uint32_t index) const;
	bool hasResults() const;
	void initialize(const uint32_t &sequence_length);
	bool processComparison(const Arguments* args, const string &sequence,const uint32_t &left, const uint32_t &right, const uint32_t &lcp);
	//bool isSignificantSubSequence(const Arguments* args, const uint32_t &k, const uint32_t &r, const uint32_t &p, const uint32_t &lcp) const;
	bool isSignificantSubSequence(const Arguments* args, const uint32_t &k, const uint32_t &r) const;
	void addResult(const string &sequence, const uint32_t &k, const uint32_t &r, const uint32_t &p);
	void updateAvailableStartPositions(uint32_t k, uint32_t r, uint32_t p);
	uint32_t enumeratedSSRFilter(const string &ssr);
	//void writeToFile(bool include_zero, bool write_additional_output, const string &header, const string &sequence, OutputFile &out_file);
	void writeToFile(bool include_zero, bool write_additional_output, const string &header, const string &sequence, OutputFile &out_file, uint32_t ignore_chars_offset, bool write_overlaps);
	string toString(); 
};
