/*
 * 	Brandon Pickett
 * 	progressMeter.cpp
 *
 * 	This program's purpose is to do some basic testing of the ProgressMeter
 * 	class.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>

#include "../../include/ProgressMeter.h"

using namespace std;

int main()
{
	const uint32_t DATA_SIZE = 100; // 10,000
	const uint32_t DATA_INTERVAL = 1;
	const uint32_t DISPLAY_INTERVAL = 4;
	const bool UPDATE_DISPLAY = true;
	const bool DO_NOT_UPDATE_DISPLAY = false;
	
	ProgressMeter pm = ProgressMeter(true); // true means we're going to not completely disable the progress bar from working at all

	pm.initialize(DATA_SIZE);
	pm.updateProgress(0, UPDATE_DISPLAY);
	cerr << endl;

	for (uint32_t i = 1; i <= DATA_SIZE; i += DATA_INTERVAL)
	{
		if (i % DISPLAY_INTERVAL)
		{
			pm.updateProgress(DATA_INTERVAL, DO_NOT_UPDATE_DISPLAY);
		}
		else
		{
			pm.updateProgress(DATA_INTERVAL, UPDATE_DISPLAY);
			cerr << endl;
		}
	}

	cout << "Do you see several lines of progress bars incrementing by 4% each time and\nending with 100%? (Yes or No [default: yes])\n" << endl << ">>> ";

	string response;
	getline(cin, response);
	
	cout << "\n\033[0;34mProgressBar Test: ";

	if (response.size())
	{
		for (uint32_t i = 0; i < response.size(); ++i)
		{
			if (isalpha(response[i]))
			{
				if (toupper(response[i]) == 'N')
				{
					cout << "\033[0;31mFAIL\033[0m" << endl;
					return 0;
				}
			}
		}
	}

	cout << "\033[0;32mOK\033[0m" << endl;

	return 0;
}
