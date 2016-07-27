/*
	Brandon Pickett
	SingleResult.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // stringstream
#include <cstdlib> // exit

using namespace std;

class SingleResult
{
private:
	uint32_t k; // the k in k-mer
	uint32_t r; // r as in repeats
	uint32_t p; // p as in position (starting)

public:
	SingleResult();
	SingleResult(uint32_t k, uint32_t r, uint32_t p);
	~SingleResult();
	uint32_t getK() const;
	uint32_t getR() const;
	uint32_t getP() const;
	void setK(uint32_t k);
	void setR(uint32_t r);
	void setP(uint32_t p);
	void setAll(uint32_t k, uint32_t r, uint32_t p);
	//void writeToFile(bool write_additional_output, const string &header, const string &sequence, string &output) const;
	void writeToFile(bool write_additional_output, const string &header, const string &sequence, string &output, uint32_t ignore_chars_offset) const;
	string toString() const;
	bool operator<(const SingleResult &s) const;
};
