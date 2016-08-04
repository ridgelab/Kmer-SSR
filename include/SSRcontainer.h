/*
	Brandon Pickett
	SSRcontainer.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // stringstream
#include <cstdlib> // exit
#include <cstdint>
#include <unordered_map> // unordered_map

#include "SSR.h"
#include "OutputFile.h"

using namespace std;

class SSRcontainer
{
private:
	static const uint32_t ID_INDEX = 0;
	static const uint32_t BASE_SSRcontainer_INDEX = 1;
	static const uint32_t REPEATS_INDEX = 2;
	static const uint32_t START_POS_INDEX = 3;

	unordered_map<string, vector<SSR*>*>* ssrs;

public:
	SSRcontainer();
	~SSRcontainer();
	uint32_t size() const;
	bool empty() const;
	void add(const string &identifier, SSR* ssr);
	void add(const string &identifier, vector<SSR*>* ssrs_vec);
	void clear();
	void writeToFile(ofstream &ofd) const;
	void writeToFile(OutputFile &ofd, bool block) const;
	void writeToFile(OutputFile* ofd, bool block) const;
	string toString() const;
	string toJson() const;
};
