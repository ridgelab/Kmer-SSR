/*
	Brandon Pickett
	Task.cpp
*/

#include "../include/Task.h"

Task::Task()
{
	this->global_pos = 0;
	this->global_seq_length = 0;
}
Task::Task(const string &_identifier, const string &_sequence, uint32_t _global_pos, uint32_t _global_seq_length)
{
	this->identifier = _identifier;
	this->sequence = _sequence;
	this->global_pos = _global_pos;
	this->global_seq_length = _global_seq_length;
}
Task::~Task()
{
	return;
}
string Task::getShortenedID() const
{
	return this->identifier.size() > 20 ? this->identifier.substr(0, 20) : this->identifier;
}
string Task::getBegAndEndOfSeq() const
{
	return this->sequence.size() > 20 ? this->sequence.substr(0, 10) + ".." + this->sequence.substr(this->sequence.size() - 10, string::npos) : this->sequence;
}
string Task::getID() const
{
	return this->identifier;
}
string Task::getSequence() const
{
	return this->sequence;
}
uint32_t Task::getGlobalPosition() const
{
	return this->global_pos;
}
uint32_t Task::getGlobalSequenceLength() const
{
	return this->global_seq_length;
}
bool Task::equalTo(Task* other) const
{
	if (this->identifier != other->getID())
	{
		return false;
	}
	if (this->sequence != other->getSequence())
	{
		return false;
	}
	if (this->global_pos != other->getGlobalPosition())
	{
		return false;
	}
	if (this->global_seq_length != other->getGlobalSequenceLength())
	{
		return false;
	}

	return true;
}
string Task::toString() const
{
	stringstream strm;
	strm << "Task: { id: " << this->getShortenedID() << ", sequence: " << this->getBegAndEndOfSeq() << ", global position: " << this->global_pos << ", global sequence length: " << this->global_seq_length << " }";
	return strm.str();
}
