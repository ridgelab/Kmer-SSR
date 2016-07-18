/*
	Brandon Pickett
	SSR.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // stringstream
#include <cstdlib> // exit
#include <cstdint>

#include "OutputFile.h"

using namespace std;

class SSR
{
private:
	//string* identifier; // the fasta id or header
	string base_ssr; // base_ssr
	uint32_t repeats; // number of repeats
	uint32_t position; // starting position of the SSR

public:
	SSR();
	SSR(string _base_ssr, uint32_t _repeats, uint32_t _position);
	~SSR();
	//string* getIDPtr() const;
	//string getID() const;
	uint32_t getPeriod() const;
	uint32_t getRepeats() const;
	uint32_t getStartPosition() const;
	uint32_t getLength() const;
	uint32_t getExclusiveEndPosition() const;

	void writeToFile(ofstream &ofd) const;
	void writeToFile(OutputFile &ofd) const;
	string toString() const;
};
