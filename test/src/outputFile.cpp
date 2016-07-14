/*
 * 	Brandon Pickett
 * 	outputFile.cpp
 *
 * 	This program's purpose is to do some basic testing of the OutputFile
 * 	class.
 */

#include <iostream>
#include <string>
#include <cstdint>

#include "../../include/OutputFile.h"

using namespace std;

int main()
{
	OutputFile of();

	cout << "\nThe outputFile test has not yet been implemented." << endl;
	
	cout << "\n\033[0;34mOutputFile Test: ";

	bool pass = true;

	if (pass)
	{
		cout << "\033[0;32mOK\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	return 0;
}
