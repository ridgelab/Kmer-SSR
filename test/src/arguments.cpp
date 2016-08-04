/*
 * 	Brandon Pickett
 * 	arguments.cpp
 *
 * 	This program's purpose is to test the Arguments class
 */

#include <iostream>
#include <string>
#include <cstdint>
#include <unistd.h> // getopt

#include "../../include/Arguments.h"

using namespace std;

bool test1();
bool test2();
bool test3();
bool test4();

int main()
{
	cout << '\n';
	
	int ret_val = 0; // also used to count the number of failed tests

	vector<bool (*)()> tests = { test1, test2, test3, test4 };

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

		optind = 1; // reset optind for next use of global getopt
	}
	
	string pass_or_fail_color = ret_val ? "\033[0;31m" : "\033[0;32m";
	cout << "\033[0;34mArguments Passed:\t" << pass_or_fail_color << tests.size() - ret_val << " / " << tests.size() << "\033[0m" << endl;
	
	return ret_val;
}

bool test1()
{
	try
	{
		int argc = 5;
		char* argv[] = {
			(char*)("test1"),
			(char*)("-i"),
			(char*)("example/input.fasta"),
			(char*)("-o"),
			(char*)("test/arg1.tsv")
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

bool test2()
{
	try
	{
		int argc = 1;
		char* argv[] = {
			(char*)("test2")
		};
		
		Arguments* args = new Arguments(argc, argv);

		if (args->getInFileName() != "/dev/stdin")
		{
			return false;
		}
		if (args->getOutFileName() != "/dev/stdout")
		{
			return false;
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

bool test3()
{
	try
	{
		int argc = 7;
		char* argv[] = {
			(char*)("test3"),
			(char*)("-i"),
			(char*)("example/input.fasta"),
			(char*)("-o"),
			(char*)("test/arg3.tsv"),
			(char*)("-p"),
			(char*)("1-7,18")
		};
		
		Arguments* args = new Arguments(argc, argv);

		if (args->getInFileName() != "example/input.fasta")
		{
			return false;
		}
		if (args->getOutFileName() != "test/arg3.tsv")
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

bool test4()
{
	try
	{
		int argc = 13;
		char* argv[] = {
			(char*)("test4"),
			(char*)("-i"),
			(char*)("example/input.fasta"),
			(char*)("-o"),
			(char*)("test/arg4.tsv"),
			(char*)("-n"),
			(char*)("5"),
			(char*)("-N"),
			(char*)("18"),
			(char*)("-r"),
			(char*)("4"),
			(char*)("-R"),
			(char*)("8")
		};
		
		Arguments* args = new Arguments(argc, argv);

		if (args->getInFileName() != "example/input.fasta")
		{
			return false;
		}
		if (args->getOutFileName() != "test/arg4.tsv")
		{
			return false;
		}
		if (args->getMinNucleotideLength() != 5)
		{
			return false;
		}
		if (args->getMaxNucleotideLength() != 18)
		{
			return false;
		}
		if (args->getMinRepeats() != 4)
		{
			return false;
		}
		if (args->getMaxRepeats() != 8)
		{
			return false;
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

