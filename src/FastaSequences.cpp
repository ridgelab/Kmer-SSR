/*
	Brandon Pickett
	FastaSequences.cpp
*/

#include "../include/FastaSequences.h"

using namespace std;

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------     PUBLIC   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

FastaSequences::FastaSequences()
{
	sem_init(&(this->lock),0,1);
	this->dried_up_source = false;
	this->dry_marker = 0;
}
FastaSequences::~FastaSequences()
{
	//return;
}
bool FastaSequences::isDriedUp() const
{
	bool temp = this->dried_up_source;
	return temp;
}
bool FastaSequences::empty()
{
	sem_wait(&(this->lock));
	bool empty = this->headers.empty();
	sem_post(&(this->lock));
	return empty;
}
void FastaSequences::dryUp()
{
	sem_wait(&(this->lock));
	this->dried_up_source = true;
	sem_post(&(this->lock));
}
void FastaSequences::add(string header, string sequence, uint32_t ignore_chars_offset)
{
	sem_wait(&(this->lock));
	this->headers.push(header);
	this->sequences.push(sequence);
	this->ignore_chars_offsets.push(ignore_chars_offset);
	if (!this->dried_up_source) { this->dry_marker++; }
	sem_post(&(this->lock));
}
uint32_t FastaSequences::get(string &header, string &sequence, uint32_t &ignore_chars_offset)
{
	uint32_t retval = 0;

	sem_wait(&(this->lock));
	
	if (this->dry_marker > 0)
	{
		header = this->headers.front();
		sequence = this->sequences.front();
		ignore_chars_offset = this->ignore_chars_offsets.front();
		this->headers.pop();
		this->sequences.pop();
		this->ignore_chars_offsets.pop();
		this->dry_marker--;
	}
	else
	{
		retval = 1;
	}
	sem_post(&(this->lock));
	
	return retval;
}

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------    PRIVATE   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||
