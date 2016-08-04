/*
 * 	Brandon Pickett
 * 	findSSRs.cpp
 *
 * 	This program's purpose is to test the FindSSRs class
 */

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <fstream>

#include "../../include/FindSSRs.h"
#include "../../include/Arguments.h"

using namespace std;

bool actualOutputMatchedExpected(const string &actual_fn, const string &expected_fn);
bool test1();
//bool test2();
//bool test3();
//bool test4();

int main()
{
	cout << '\n';
	
	int ret_val = 0; // also used to count the number of failed tests

	//vector<bool (*)()> tests = { test1, test2, test3, test4 };
	vector<bool (*)()> tests = { test1 };

	for (uint32_t i = 0; i < tests.size(); ++i)
	{
		cout << "FindSSRs Test #" << i + 1 << ":\t";
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

		optind = 1; // reset optind for next use of global getopt
	}
	
	string pass_or_fail_color = ret_val ? "\033[0;31m" : "\033[0;32m";
	cout << "\033[0;34mFindSSRs Passed:\t" << pass_or_fail_color << tests.size() - ret_val << " / " << tests.size() << "\033[0m" << endl;
	
	return ret_val;
}

bool actualOutputMatchedExpected(const string &actual_fn, const string &expected_fn)
{
	// sort each file
	// run diff
	// check diff results
}

bool test1()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 6;
		char* argv[] = {
			(char*)("test1"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/1.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/1.tsv")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs find_ssrs(args);
			ret_val = find_ssrs.run() == 0 ? true : false;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected();
			}
		}
	}
	catch(string error)
	{
		cerr << error << endl;
		ret_val = false;
	}
	catch(const char* error)
	{
		cerr << error << endl;
		ret_val = false;
	}
	
	delete args;

	return ret_val;
}

