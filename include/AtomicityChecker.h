/*
	Brandon Pickett
	AtomicityChecker.h
*/

#pragma once

#include <iostream> // cout,cerr
#include <string> // string
#include <sstream> // stringstream
#include <cstddef>
#include <cstdint>
#include <semaphore.h> // semaphore
#include <unordered_set> // unordered_set
#include <unordered_map> // unordered_map

using namespace std;

class AtomicityChecker
{
private:
	unordered_map<uint32_t, unordered_set<uint32_t>*>* factors;
	sem_t lock;

private:
	void setUp();
	void initFactorsWithPreDefinedFactors(unordered_map<uint32_t, unordered_set<uint32_t>> _factors);
	void safeInsert(uint32_t x, unordered_set<uint32_t>* xfactors);
	void addFactor(uint32_t x, const unordered_set<uint32_t> &xfactors);
	unordered_set<uint32_t>* calculateFactors(uint32_t x);
	bool checkAtomicityAtFactor(uint32_t factor, const string &base_ssr, uint32_t period) const;

public:
	AtomicityChecker();
	AtomicityChecker(const unordered_map<uint32_t, unordered_set<uint32_t>> &_factors);
	~AtomicityChecker();
	bool isAtomic(const string &base_ssr);
	string toString() const;
};
