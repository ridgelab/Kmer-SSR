/*
	Brandon Pickett
	Results.cpp
*/

//#define NDEBUG

#include "../include/Results.h"

using namespace std;

Results::Results(const uint32_t &sequence_length, unordered_set<string>* _enumerated_ssrs)
{
	this->enumerated_ssrs = _enumerated_ssrs;
	initialize(sequence_length);
}
Results::~Results()
{
	this->enumerated_ssrs = NULL; // the actual set will be destroyed by a different class
}
bool  Results::isStartPositionAvailableAt(uint32_t index) const
{
	return available_start_positions[index];	
}
bool Results::hasResults() const
{
	return (this->results.empty() == true) ? false : true;
}
void Results::initialize(const uint32_t &sequence_length)
{
	this->available_start_positions.assign(sequence_length, true);
	// shrink to fit???
}
//void  Results::processComparison(Arguments* args, const string &sequence, const uint32_t &left, const uint32_t &right, const uint32_t &lcp)
bool  Results::processComparison(const Arguments* args, const string &sequence, const uint32_t &left, const uint32_t &right, const uint32_t &lcp)
{
	assert (lcp > 0); // Technically lcp == 0 is okay, but completely useless.

	//cerr << "left,right: " << left << "," << right << endl;

	uint32_t k = (uint32_t ) abs((int) (right - left));
	uint32_t r = lcp / k;
	uint32_t p = min(left, right);

	//if (isSignificantSubSequence(args,k,r,p,lcp) == true)
	if (isSignificantSubSequence(args,k,r) == true)
	{
		addResult(sequence,k,r,p);
	}
	else if (r == 0) // a.k.a., r == 0 AND the subsequence was insignificant
	{
		return false; // a.k.a., don't stop, keep incrementing j to find repeats
	}
	
	return true; // a.k.a., (you've found something) OR (r > 0 AND the subsequence was insignificant)
}
//bool Results::isSignificantSubSequence(const Arguments* args, const uint32_t &k, const uint32_t &r, const uint32_t &p, const uint32_t &lcp) const
bool Results::isSignificantSubSequence(const Arguments* args, const uint32_t &k, const uint32_t &r) const
{
	if (r == 0) // check that there is actually a repeat occuring
	{
		return false;
	}
	else if (k * r + k < args->getMinNucleotideLength()) // check min nucleotide length
	{
		return false;
	}
	//else if (k < args->getMinSSRLength()) // check min ssr length
	//{
	//	return false;
	//}
	//else if (k > args->getMaxSSRLength()) // check max ssr length
	//{
	//	return false;
	//}
	else if ( (r + 1) < args->getMinRepeats() ) // check min repeats
	{
		return false;
	}
	else if ( (r + 1) > args->getMaxRepeats() ) // check max repeats
	{
		return false;
	}
	return true;
}
void  Results::addResult(const string &sequence, const uint32_t &k, const uint32_t &r, const uint32_t &p)
{
	if (enumeratedSSRFilter(sequence.substr(p,k)) == 1)
	{
		results.insert(SingleResult(k, r, p));
		//updateAvailableStartPositions(k, r, p);
	}
}
void  Results::updateAvailableStartPositions(uint32_t k, uint32_t r, uint32_t p)
{
	//for (uint32_t i = (p + k); i < available_start_positions.size() && i < (p + k * r); i++)
	for (uint32_t i = p; i < available_start_positions.size() && i < (p + k * (r + 1)); i++)
	//for (uint32_t i = p; i < available_start_positions.size() && i < (p + k * r); i++)
	{
		available_start_positions[i] = false;
	}
}
uint32_t Results::enumeratedSSRFilter(const string &ssr)
{
	if (enumerated_ssrs->empty() == true)
	{
		return 1;
	}
	return enumerated_ssrs->count(ssr);
}
//void Results::writeToFile(bool include_zero, bool write_additional_output, const string &header, const string &sequence, OutputFile &out_file)
void Results::writeToFile(bool include_zero, bool write_additional_output, const string &header, const string &sequence, OutputFile &out_file, uint32_t ignore_chars_offset, bool write_overlaps)
{
	string output;
	if (!results.empty())
	{
		for (set<SingleResult>::iterator itr = this->results.begin(); itr != this->results.end(); ++itr)
		{
			if (isStartPositionAvailableAt(itr->getP()) == true)
			{
				itr->writeToFile(write_additional_output, header, sequence, output, ignore_chars_offset);
				//output = output + "\n";
				output += "\n";
				
				if (!write_overlaps)
				{
					updateAvailableStartPositions(itr->getK(), itr->getR(), itr->getP());
				}
			}
		}
	}
	else
	{
		if (include_zero)
		{
			//output = output + header.substr(1,string::npos);
			output += header.substr(1,string::npos);
			if (write_additional_output)
			{
				//output = output + "\t-";
				output += "\t-";
			}
			//output = output + "\t-\t0\t0\n";
			output += "\t-\t0\t0\n";
		}
	}
	out_file << output;
}
string  Results::toString()
{
	string temp;
	stringstream strm;
	temp = "Results\n=======\n";
	
	//temp = temp + "Position Availability[ ";
	temp += "Position Availability[ ";
	for (uint32_t i = 0; i < available_start_positions.size(); i++)
	{
		if (i > 0)
		{
			//temp = temp + ", ";
			temp += ", ";
		}
		
		if (available_start_positions[i] == true)
		{
 			//temp = temp + "true";
 			temp += "true";
		}
		else
		{
			//temp = temp + "false";
			temp += "false";
		}
	}
	//temp = temp + " ]\n";
	temp += " ]\n";

	for (set<SingleResult>::iterator itr = results.begin(); itr != results.end(); ++itr)
	{
		if (isStartPositionAvailableAt(itr->getP()) == true)
		{
			//temp = temp + itr->toString() + "\n";
			temp += itr->toString() + "\n";
			updateAvailableStartPositions(itr->getK(), itr->getR(), itr->getP());
		}
	}

	return temp;
}
