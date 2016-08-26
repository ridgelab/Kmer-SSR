/*
 * 	Brandon Pickett
 * 	threads.cpp
 *
 * 	This program's purpose is to test the FindSSRs class
 * 	to see that the output is the same (though not necessarily
 * 	in the same order) regardless of whether it is using 1
 * 	thread or many threads.
 */

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <fstream>

#include "../../include/FindSSRs.h"
#include "../../include/Arguments.h"

using namespace std;

bool test1(string threads);
bool test2(string threads);
bool test3(string threads);
bool test4(string threads);
bool test5(string threads);
bool test6(string threads);
bool test7(string threads);
bool test8(string threads);
bool test9(string threads);
bool test10(string threads);
bool test11(string threads);
bool test12(string threads);

int main()
{
	cout << '\n';
	
	int ret_val = 0; // also used to count the number of failed tests

	vector<bool (*)(string)> tests = {
		test1, test2, test3, test4, test5,
		test6, test7, test8, test9, test10,
		test11, test12
	};

	vector<string> threads = { "2", "3", "4", "5", "6", "7", "8" };

	for (uint32_t t = 0; t < threads.size(); ++t)
	{
		for (uint32_t i = 0; i < tests.size(); ++i)
		{
			cout << "Threads (" << threads[t] << ") Test #" <<  (i + 1) + (t * 12) << ":\t";
			//if (tests[i](threads))
			//if (tests[i]("1"))
			if (tests[i](threads[t]))
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
	}
	
	string pass_or_fail_color = ret_val ? "\033[0;31m" : "\033[0;32m";
	cout << "\033[0;34mThread Tests Passed:\t" << pass_or_fail_color << tests.size() * threads.size() - ret_val << " / " << tests.size() * threads.size() << "\033[0m" << endl;
	
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
bool test1(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/1_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test1"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/1.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("2-3"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};
		
		args = new Arguments(argc, argv);
		
		if (!args->helpOrVersionDisplayed())
		{
			//cerr << "checkpoint 1" << endl;
			FindSSRs* find_ssrs = new FindSSRs(args);
			//cerr << "checkpoint 2" << endl;
			ret_val = find_ssrs->run() == 0 ? true : false;
			//cerr << "checkpoint 3" << endl;
			delete find_ssrs;
			//cerr << "checkpoint 4" << endl;

			if (ret_val)
			{
				//cerr << "checkpoint 5" << endl;
				ret_val = actualOutputMatchedExpected("test/output/actual/1_" + threads + ".tsv", "test/output/expected/1.tsv");
				//cerr << "checkpoint 6" << endl;
			}
			//cerr << "checkpoint 7" << endl;
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
	//delete blah2;

	return ret_val;
}

bool test2(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/2_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test2"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/2.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1-3"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/2_" + threads + ".tsv", "test/output/expected/2.tsv");
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
bool test3(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/3_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test3"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/3.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1-7"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/3_" + threads + ".tsv", "test/output/expected/3.tsv");
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
bool test4(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/4_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test4"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/4.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1,2,3,4,5,6,7"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/4_" + threads + ".tsv", "test/output/expected/4.tsv");
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
bool test5(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/5_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test5"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/5.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/5_" + threads + ".tsv", "test/output/expected/5.tsv");
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
bool test6(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/6_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test6"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/6.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("2"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/6_" + threads + ".tsv", "test/output/expected/6.tsv");
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
bool test7(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/7_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test7"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/7.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("3"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/7_" + threads + ".tsv", "test/output/expected/7.tsv");
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
bool test8(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/8_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test8"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/8.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("4"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/8_" + threads + ".tsv", "test/output/expected/8.tsv");
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
bool test9(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/9_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test9"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/9.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("5"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/9_" + threads + ".tsv", "test/output/expected/9.tsv");
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
bool test10(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/10_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test10"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/10.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("6"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/10_" + threads + ".tsv", "test/output/expected/10.tsv");
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
bool test11(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/11_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test11"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/11.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("7"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/11_" + threads + ".tsv", "test/output/expected/11.tsv");
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
bool test12(string threads)
{
	bool ret_val = true;

	Arguments* args;

	string temp_output_file_name = "test/output/actual/12_" + threads + ".tsv";

	try
	{
		int argc = 14;
		char* argv[] = {
			(char*)("test12"),
			(char*)("-d"),
			(char*)("-i"),
			(char*)("test/input/12.fasta"),
			(char*)("-o"),
			const_cast<char*>(temp_output_file_name.c_str()),
			(char*)("-n"),
			(char*)("2"),
			(char*)("-l"),
			(char*)("2"),
			(char*)("-p"),
			(char*)("1-7"),
			(char*)("-t"),
			const_cast<char*>(threads.c_str())
		};

		args = new Arguments(argc, argv);

		if (!args->helpOrVersionDisplayed())
		{
			FindSSRs* find_ssrs = new FindSSRs(args);
			ret_val = find_ssrs->run() == 0 ? true : false;
			delete find_ssrs;

			if (ret_val)
			{
				ret_val = actualOutputMatchedExpected("test/output/actual/12_" + threads + ".tsv", "test/output/expected/12.tsv");
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

