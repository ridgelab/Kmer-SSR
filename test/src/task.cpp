/*
 * 	Brandon Pickett
 * 	task.cpp
 *
 * 	This program's purpose is to do some basic testing of the Task
 * 	class.
 */

#include <iostream>
#include <string>
#include <cstdint>

#include "../../include/Task.h"

using namespace std;

int main()
{
	Task task1 = Task();
	Task task2("SomeSeqID", "ACGTGTACGATCGACTACGATCACACACGT", 237, 946);

	//cout << task1.toString() << endl;
	//cout << task2.toString() << endl;
	
	cout << "\n\033[0;34mTask Test: ";

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
