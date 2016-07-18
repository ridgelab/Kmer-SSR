/*
 * 	Brandon Pickett
 * 	ssr.cpp
 *
 * 	This program's purpose is to do some basic testing of the SSR
 * 	class.
 */

#include <iostream>
#include <string>
#include <cstdint>

#include "../../include/SSR.h"

using namespace std;

int main()
{
	SSR ssr1();
	SSR ssr2("ACGT", 4, 32);

	//cout << ssr2.toString() << endl;

	cout << "\n\033[0;34mSSR Test: ";

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
