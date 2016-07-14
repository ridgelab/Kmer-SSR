/*
	Brandon Pickett
	AtomicityChecker.cpp
*/

#include "../include/AtomicityChecker.h"

using namespace std;

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------     PUBLIC   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

AtomicityChecker::AtomicityChecker()
{
	this->setUp();
	unordered_map<uint32_t, unordered_set<uint32_t>> default_factors( {
		{ 1, unordered_set<uint32_t>( {1} ) },
		{ 2, unordered_set<uint32_t>( {1,2} ) },
		{ 3, unordered_set<uint32_t>( {1,3} ) },
		{ 4, unordered_set<uint32_t>( {1,2,4} ) },
		{ 5, unordered_set<uint32_t>( {1,5} ) },
		{ 6, unordered_set<uint32_t>( {1,2,3,6} ) },
		{ 7, unordered_set<uint32_t>( {1,7} ) },
		{ 8, unordered_set<uint32_t>( {1,2,4,8} ) },
		{ 9, unordered_set<uint32_t>( {1,3,9} ) },
		{ 10, unordered_set<uint32_t>( {1,2,5,10} ) }
	} );
	this->initFactorsWithPreDefinedFactors(default_factors);
}
AtomicityChecker::AtomicityChecker(const unordered_map<uint32_t, unordered_set<uint32_t>> &_factors)
{
	this->setUp();
	this->initFactorsWithPreDefinedFactors(_factors);
}
AtomicityChecker::~AtomicityChecker()
{
	for (unordered_map<uint32_t, unordered_set<uint32_t>*>::iterator itr = this->factors->begin(); itr != this->factors->end(); ++itr)
	{
		delete itr->second;
	}
	this->factors->clear();
	delete this->factors;
}
bool AtomicityChecker::isAtomic(const string &base_ssr)
{
	uint32_t period = base_ssr.size();

	// get the factors (calculate them if necessary)
	unordered_set<uint32_t>* period_factors = this->calculateFactors(period);

	// check if it's atomic
	for (unordered_set<uint32_t>::iterator itr = period_factors->begin(); itr != period_factors->end(); ++itr)
	{
		if ( (*itr != period) && (!this->checkAtomicityAtFactor(*itr, base_ssr, period)) )
		{
			return false; // basically, skip the check if *itr == period. Given that, if it's not atomic, return false
		}
	}

	return true;
}
string AtomicityChecker::toString() const
{
	stringstream strm;
	strm << "AtomicityChecker: { factors: { ";

	for (unordered_map<uint32_t, unordered_set<uint32_t>*>::iterator itr = this->factors->begin(); itr != this->factors->end(); ++itr)
	{
		if (itr != this->factors->begin())
		{
			strm << ", ";
		}

		strm << itr->first << ": { ";

		for (unordered_set<uint32_t>::iterator inner_itr = itr->second->begin(); inner_itr != itr->second->end(); ++inner_itr)
		{
			if (inner_itr != itr->second->begin())
			{
				strm << ", ";
			}
			strm << *inner_itr;
		}

		strm << " }";
	}

	strm << " } }";
	return strm.str();
}

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------    PRIVATE   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

void AtomicityChecker::setUp()
{
	sem_init(&(this->lock),0,1);
	this->factors = new unordered_map<uint32_t, unordered_set<uint32_t>*>();
}
void AtomicityChecker::initFactorsWithPreDefinedFactors(unordered_map<uint32_t, unordered_set<uint32_t>> _factors)
{
	for (unordered_map<uint32_t, unordered_set<uint32_t>>::iterator orig_itr = _factors.begin(); orig_itr != _factors.end(); ++orig_itr)
	{
		this->addFactor(orig_itr->first, orig_itr->second);
	}
}
void AtomicityChecker::addFactor(uint32_t x, const unordered_set<uint32_t> &xfactors)
{
	unordered_set<uint32_t>* temp_ptr = new unordered_set<uint32_t>(xfactors);
	this->safeInsert(x, temp_ptr);
}
void AtomicityChecker::safeInsert(uint32_t x, unordered_set<uint32_t>* xfactors)
{
	sem_wait(&(this->lock));
	if (!this->factors->count(x)) // if x is not already in this->factors
	{
		std::pair<uint32_t, unordered_set<uint32_t> * > temp_pair(x, xfactors);
		this->factors->insert(temp_pair); // add it
	}
	else if (this->factors->at(x) != xfactors) // else if x is already in this->factors, but has a different set than xfactors
	{
		delete xfactors;
	}
	sem_post(&(this->lock));
}
unordered_set<uint32_t>* AtomicityChecker::calculateFactors(uint32_t x)
{
	if (this->factors->count(x))
	{
		return this->factors->at(x);
	}

	if (x == 1)
	{
		this->safeInsert(1, new unordered_set<uint32_t>( {1} ) );
		return this->factors->at(x);
	}
	
	unordered_set<uint32_t>* xfactors = new unordered_set<uint32_t>( {1, x} );

	for (uint32_t i = 2; i <= (x / 2); ++i)
	{
		if ( (x % i) == 0) // english: if i is a factor of x
		{
			xfactors->insert(i); // add i to xfactors

			if (!this->factors->count(i)) // if i is not in this->factors
			{
				this->safeInsert(i, this->calculateFactors(i)); // add it
			}
		}
	}

	this->safeInsert(x, xfactors);
	return this->factors->at(x);
}
bool AtomicityChecker::checkAtomicityAtFactor(uint32_t factor, const string &base_ssr, uint32_t period) const
{
	uint32_t count = 1;

	for (uint32_t i = 0; i < period; i += factor)
	{
		if ( base_ssr.substr(i, factor) != base_ssr.substr(i + factor, factor) )
		{
			break;
		}
		++count;
	}

	return count != (period / factor);
}
