/*
 * 	Brandon Pickett
 * 	arguments.cpp
 *
 * 	This program's purpose is to test the Arguments class
 */

#include <iostream>
#include <string>
#include <cstdint>

#include "../../include/Arguments.h"

using namespace std;

bool test1();

int main()
{
	cout << '\n';
	
	int ret_val = 0; // also used to count the number of failed tests

	vector<bool (*)()> tests = { test1 };

	for (uint32_t i = 0; i < tests.size(); ++i)
	{
		cout << "Arguments Test #" << i + 1 << ":\t";
		if (tests[i]())
		{
			cout << "OK";
		}
		else
		{
			++ret_val;
			cout << "FAIL";
		}
		cout << '\n';
	}
	
	string pass_or_fail_color = ret_val ? "\033[0;31m" : "\033[0;32m";
	cout << "\033[0;34mArguments Passed:\t" << pass_or_fail_color << tests.size() - ret_val << " / " << tests.size() << "\033[0m" << endl;
	
	return ret_val;
}

bool test1()
{
	try
	{
		int argc = 9;
		char* argv[] = {
			(char*)("test1"),
			(char*)("-i"),
			(char*)("example/input.fasta"),
			(char*)("-o"),
			(char*)("test/arg1.tsv"),
			(char*)("-p"),
			(char*)("1-7,18"),
			(char*)("-R"),
			(char*)("9")
		};
		
		Arguments* args = new Arguments(argc, argv);

		if (args->getInFileName() != "example/input.fasta")
		{
			return false;
		}
		if (args->getOutFileName() != "test/arg1.tsv")
		{
			return false;
		}
		if (args->getMaxRepeats() != 9)
		{
			return false;
		}
		if (!args->inPeriods(18))
		{
			return false;
		}
		for (uint32_t p = 1; p < 8; ++p)
		{
			if (!args->inPeriods(p))
			{
				return false;
			}
		}


		delete args;
	}
	catch(string error)
	{
		cerr << error << endl;
		return false;
	}
	catch(const char* error)
	{
		cerr << error << endl;
		return false;
	}

	return true; // success
}
