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
bool test2();
bool test3();
bool test4();
bool test5();
bool test6();
bool test7();
bool test8();
bool test9();
bool test10();
bool test11();
bool test12();

int main()
{
	cout << '\n';
	
	int ret_val = 0; // also used to count the number of failed tests

	vector<bool (*)()> tests = {
		test1, test2, test3, test4, test5,
		test6, test7, test8, test9, test10,
		test11, test12
	};

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

// tests 1-2 are really just examples to make sure our testing scheme works
bool test1()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test1"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/1.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/1.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
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

bool test2()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test2"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/2.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/2.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1-3")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/2.tsv", "test/output/expected/2.tsv");
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

// tests 3-11 will be for testing the -p parameter 
// tests 3-11 will use the same input file

// test 3 tests periods 1-7 at once
bool test3()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test3"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/3.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/3.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1-7")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/3.tsv", "test/output/expected/3.tsv");
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

// test 4 tests periods 1-7 at once, using commas
bool test4()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test4"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/4.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/4.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1,2,3,4,5,6,7")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/4.tsv", "test/output/expected/4.tsv");
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

// test 5 tests period 1
bool test5()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test5"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/5.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/5.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/5.tsv", "test/output/expected/5.tsv");
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

// test 6 tests period 2
bool test6()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test6"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/6.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/6.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("2")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/6.tsv", "test/output/expected/6.tsv");
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


// test 7 tests period 3
bool test7()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test7"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/7.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/7.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("3")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/7.tsv", "test/output/expected/7.tsv");
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

// test 8 tests period 4
bool test8()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test8"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/8.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/8.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("4")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/8.tsv", "test/output/expected/8.tsv");
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

// test 9 tests period 5
bool test9()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test9"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/9.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/9.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("5")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/9.tsv", "test/output/expected/9.tsv");
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

// test 10 tests period 6
bool test10()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test10"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/10.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/10.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("6")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/10.tsv", "test/output/expected/10.tsv");
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

// test 11 tests period 7
bool test11()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test11"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/11.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/11.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("7")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/11.tsv", "test/output/expected/11.tsv");
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

// test 12 tests sesquence edge cases
// edge cases are as follows:
//   - beginning of sequence
//   - end of sequence
//   - ssr followed by an ssr
//   - ssr within an ssr
//   - ssrs overlapping within last period (first one is bigger period size)
//   - ssrs overlapping entire last period (second one is bigger period size)
//   - ssrs overlapping more than last period (second one is bigger period size)
//   - ssrs overlapping within first period (second one is bigger period size)
//   - ssrs overlapping entire first period (first one is bigger period size)
//   - ssrs overlapping more than first period (first one is bigger period size)
//   - imperfect ssr (ATAGATAT should not register as ATAG x2)
//   - atomicity checks
//   - identical ssrs separated by n nucleotides, where n is 1-7
//   - report first of ATATATAT vs TATATATA in seq. ...ATATATATA...
//   - ...SSR1SSR2SSSR3SSR2SSR1...
bool test12()
{
	bool ret_val = true;

	Arguments* args;

	try
	{
		int argc = 12;
		char* argv[] = {
			(char*)("test12"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/12.fasta"),
			(char*)("-o"),
			(char*)("test/output/actual/12.tsv"),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1-7")
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/12.tsv", "test/output/expected/12.tsv");
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

