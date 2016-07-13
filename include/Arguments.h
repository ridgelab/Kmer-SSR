/*
	Brandon Pickett
	Arguments.h
*/

#pragma once

#include <cassert> // assert
#include <typeinfo> // typeid
#include <unistd.h> // getopt
#include <iostream> // cout,cerr
#include <string> // string
#include <unordered_set> // unordered_set
#include <vector> // vector
#include <set> // set
#include <cstring> // strcmp
#include <sstream> // stringstream
#include <cstdlib> // exit
#include <climits> // LONG_MAX, etc.

using namespace std;

class Arguments
{
private:
	// fields
	bool allow_non_atomic;                  // -A
	bool bzipped2_input;                    // -b
	bool bzipped2_output;                   // -B
	bool display_progress_bar;              // -d
	bool exhaustive;                        // -e
	bool gzipped_input;                     // -g
	bool gzipped_output;                    // -G
	string usage_message;                   // -h
	string help_message;                    // -h
	string input_file_name;                 // -i:
	uint32_t min_nucleotides;               // -n:
	uint32_t max_nucleotides;               // -N:
	string output_file_name;                // -o:
	uint32_t max_task_queue_size;           // -Q:
	uint32_t min_repeats;                   // -r:
	uint32_t max_repeats;                   // -R:
	uint32_t threads;                       // -t:
	string version;                         // -v
	
	// functions
	void processArgs(int argc, char* argv[]);
	uint32_t parsePositiveIntegerArgument(string argument);
	uint32_t parsePositiveIntegerArgument(char* argument);
	void addToPeriods(string period_s);
	void addToEnumeratedSSRs(string enumerated_ssrs_str);
	vector<string> split(string str, char sep);
	void addToAlphabet(string alphabet_str);
	string generateHelpMessage() const;

public:
	// fields
	// none
	unordered_set<char>* alphabet;          // -a:
	set<uint32_t, bool(*)(uint32_t,uint32_t)>* periods;                 // -p:
	unordered_set<string>* enumerated_ssrs; // -s:
	
	// functions
	Arguments(int argc, char* argv[]);
	~Arguments();
	bool isExhaustive() const;
	uint32_t getMinNucleotideLength() const;
	uint32_t getMaxNucleotideLength() const;
	uint32_t getMinRepeats() const;
	uint32_t getMaxRepeats() const;
	uint32_t getNumThreads() const;
	string getUsageStatement(bool error) const;
	unordered_set<string>* getEnumeratedSSRs() const;
	unordered_set<char>* getAlphabet() const;
	string getInFileName() const;
	string getOutFileName() const;
};
