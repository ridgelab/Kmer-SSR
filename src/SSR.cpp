/*
	Brandon Pickett
	SSR.cpp
*/

#include "../include/SSR.h"

SSR::SSR()
{
	this->repeats = 0;
	this->position = 0;
}
SSR::SSR(string _base_ssr, uint32_t _repeats, uint32_t _position)
{
	this->base_ssr = _base_ssr;
	this->repeats = _repeats;
	this->position = _position;
}
SSR::~SSR()
{
	return;
}
uint32_t SSR::getPeriod() const
{
	return this->base_ssr.size();
}
uint32_t SSR::getRepeats() const
{
	return this->repeats;
}
uint32_t SSR::getStartPosition() const
{
	return this->position;
}
uint32_t SSR::getLength() const
{
	return this->base_ssr.size() * this->repeats;
}
uint32_t SSR::getExclusiveEndPosition() const
{
	return this->position + this->getLength();
}
void SSR::writeToFile(ofstream &ofd) const
{
	ofd << this->base_ssr << '\t' << this->repeats << '\t' << this->position << '\n';
}
void SSR::writeToFile(OutputFile &ofd) const
{
	ofd << this->base_ssr << '\t' << this->repeats << '\t' << this->position << '\n';
}
string SSR::toString() const
{
	stringstream strm;
	strm << "SSR: { base: " << this->base_ssr << ", repeats: " << this->repeats << ", position: " << this->position << " }";
	return strm.str();
}
