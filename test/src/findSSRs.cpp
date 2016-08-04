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

void fillSSRSetFromFile(const string &fn, set<string>* ssrs)
{
	ifstream fd;
	fd.open(fn.c_str());

	string line;
	getline(fd, line); // skip past the header line
	while(getline(fd, line))
	{
		ssrs->insert(line);
	}

	fd.close();
}

bool actualOutputMatchedExpected(const string &actual_fn, const string &expected_fn)
{
	bool ret_val = true;
	
	set<string>* actual = new set<string>();
	set<string>* expected = new set<string>();
	
	fillSSRSetFromFile(actual_fn, actual);
	fillSSRSetFromFile(expected_fn, expected);

	if (actual->size() != expected->size())
	{
		ret_val = false;
	}
	else
	{
		set<string>::iterator a_itr = actual->begin();
		set<string>::iterator e_itr = expected->begin();
		while (e_itr != expected->end() && a_itr != actual->end()) // expected.size() == actual.size()
		{
			if (*e_itr != *a_itr)
			{
				ret_val = false;
				break;
			}

			++e_itr;
			++a_itr;
		}
	}

	delete actual;
	delete expected;

	return ret_val;
}

bool test1()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
			//(char*)("-e"),
		int argc = 8;
		char* argv[] = {
			(char*)("test1"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/1.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/1.tsv"),
			(char*)("-p"),
			(char*)("2-3")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/1.tsv", "test/output/expected/1.tsv");
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

