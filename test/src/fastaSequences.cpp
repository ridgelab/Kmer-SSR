/*
 * 	Brandon Pickett
 * 	fastaSequences.cpp
 *
 * 	This program's purpose is to do some basic testing of the FastaSequences
 * 	class.
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#include "../../include/FastaSequences.h"

using namespace std;

int main()
{
	FastaSequences fs = FastaSequences();
	
	cout << "\n\033[0;34mFastaSequences Test: ";

	bool pass = true;

	if (pass)
	{
		cout << "\033[0;32mOK\033[0m";
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m";
	}
	
	cout << " (The fastaSequences test has not yet been implemented)" << endl;

	return 0;
}
