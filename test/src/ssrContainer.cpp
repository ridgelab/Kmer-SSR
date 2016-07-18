/*
 * 	Brandon Pickett
 * 	ssrContainer.cpp
 *
 * 	This program's purpose is to do some basic testing of the SSRContainer
 * 	class.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

#include "../../include/SSRcontainer.h"
#include "../../include/SSR.h"

using namespace std;

//int main(int argc, char** argv)
int main()
{
	SSRcontainer ssrs = SSRcontainer();
	//SSRcontainer ssrs();

	//cout << ssrs.toString() << endl;

	vector<string> ids;

	stringstream strm;
	string str;
	for (uint32_t j = 0; j < 4; ++j)
	{
		for (uint32_t i = 0; i < 48; ++i)
		{
			strm << "seq_" << i;
			strm >> str;
			ids.push_back(str);
			strm.clear();
		}
	}

	vector<string> base_ssrs( {
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",

		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",

		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG",
		"A", "AT", "ATC", "ATCG", "ATCGG", "ATCGGT", "ATCGGTC", "ATCGGTCG", "ATCGGTCGC", "ATCGGTCGCT", "ATCGGTCGCTC", "ATCGGTCGCTCG"
	} );

	vector<uint32_t> repeats( {
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
		
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
		
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
		
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9
	} );

	vector<uint32_t> positions( {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

		57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
		57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
		57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
		57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
		57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
		57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
		57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
		57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,

		3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649,
		3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649,
		3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649,
		3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649,
		3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649,
		3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649,
		3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649,
		3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649, 3649
	} );
		
	for (uint32_t i = 0; i < 192; ++i)
	{
		SSR* temp_ssr_ptr = new SSR(base_ssrs[i], repeats[i], positions[i]);
		ssrs.add(ids[i], temp_ssr_ptr);
	}

	
	bool pass = true;
	string pass_or_fail_msg = pass ? "\033[0;32mOK" : "\033[0;31mFAIL";
	cout << "\n\033[0;34mSSRcontainer Test: " << pass_or_fail_msg << "\033[0m" << endl;
	
	//cout << ssrs.toString() << endl;
	//cout << ssrs.toJson() << endl;

	return 0;
}
