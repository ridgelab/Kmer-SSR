/*
	Brandon Pickett
	Arguments.cpp
*/

//#define NDEBUG

#include "../include/Arguments.h"

using namespace std;

bool reverseOrder(uint32_t lhs, uint32_t rhs) { return lhs > rhs; }
bool (*reverseOrderPointer) (uint32_t, uint32_t) = reverseOrder;

// constuctor / destructor
Arguments::Arguments(int argc, char* argv[])
{
	this->input_file_name = "/dev/stdin";
	this->output_file_name = "/dev/stdout";

	this->allow_non_atomic = false;
	this->display_progress_bar = true;
	this->exhaustive = false;

	this->bzipped2_input = false;
	this->bzipped2_output = false;
	this->gzipped_input = false;
	this->gzipped_output = false;

	this->threads = 1;

	this->help_displayed = false;
	this->version_displayed = false;

	this->usage_message = USAGE_MESSAGE; // defined in usage.h
	this->help_message = HELP_MESSAGE;   // defined in help.h
	this->version = VERSION;             // defined in version.h

	this->alphabet = new unordered_set<char>();
	this->periods = new set<uint32_t, bool(*)(uint32_t,uint32_t)>(reverseOrderPointer);
	this->enumerated_ssrs = new unordered_set<string>();

	this->processArgs(argc, argv);
}
Arguments::~Arguments()
{
	this->alphabet->clear();
	delete this->alphabet;

	this->periods->clear();
	delete this->periods;
	
	this->enumerated_ssrs->clear();
	delete this->enumerated_ssrs;
}

// public
bool Arguments::helpOrVersionDisplayed() const
{
	return this->help_displayed | this->version_displayed;
}
bool Arguments::isExhaustive() const
{
	return this->exhaustive;
}
bool Arguments::doExhaustiveSearch() const
{
	return this->exhaustive;
}
bool Arguments::allowNonAtomic() const
{
	return this->allow_non_atomic;
}
bool Arguments::displayProgressBar() const
{
	return this->display_progress_bar;
}
uint32_t Arguments::getMaxTaskQueueSize() const
{
	return this->max_task_queue_size;
}
uint32_t Arguments::getMinSequenceLength() const
{
	return this->min_sequence_length;
}
uint32_t Arguments::getMaxSequenceLength() const
{
	return this->max_sequence_length;
}
uint32_t Arguments::getMinNucleotideLength() const
{
	return this->min_nucleotides;
}
uint32_t Arguments::getMaxNucleotideLength() const
{
	return this->max_nucleotides;
}
uint32_t Arguments::getMinRepeats() const
{
	return this->min_repeats;
}
uint32_t Arguments::getMaxRepeats() const
{
	return this->max_repeats;
}
uint32_t Arguments::getNumThreads() const
{
	return this->threads;
}
string Arguments::getUsageStatement(bool error) const
{
	if (error)
	{
		return "\033[0;31m" + this->usage_message + " (-h for help)\033[0m";
	}
	
	return this->usage_message;
}
unordered_set<string>* Arguments::getEnumeratedSSRs() const
{
	return this->enumerated_ssrs;
}
unordered_set<char>* Arguments::getAlphabet() const
{
	return this->alphabet;
}
bool Arguments::inAlphabet(const char c) const
{
	return this->alphabet->count(c) ? true : false;
}
bool Arguments::inPeriods(uint32_t p) const
{
	return this->periods->count(p) ? true : false;
}
set<uint32_t, bool (*)(uint32_t, uint32_t)>* Arguments::getPeriods() const
{
	return this->periods;
}
string Arguments::getInFileName() const
{
	return this->input_file_name;
}
string Arguments::getOutFileName() const
{
	return this->output_file_name;
}
bool Arguments::baseSSRinEnumeratedSSRs(string base_ssr) const
{
	return this->enumerated_ssrs->count(base_ssr) ? true : false;
}
bool Arguments::areEnumeratedSSRs() const
{
	return !this->enumerated_ssrs->empty();
}


// private ------------------------------------------------------------------------------- |
uint32_t Arguments::parsePositiveIntegerArgument(char* argument)
{
	return this->parsePositiveIntegerArgument(string(argument));
}
uint32_t Arguments::parsePositiveIntegerArgument(string argument)
{
	long int num = strtol(argument.c_str(), NULL, 10);

	if (num != 0L and num != LONG_MAX and num != LONG_MIN and num > 0)
	{
		return (uint32_t) num;
	}
	else
	{
		throw "ERROR: One of your arguments should have been a positive integer, but it wasn't.";
	}
}

void Arguments::processArgs(int argc, char* argv[])
{
	// parse arguments
	string alphabet_str = "A,C,G,T";         // -a:
	string min_seq_len_str = "100";            // -l:
	string max_seq_len_str = "500000000";    // -L: // 500,000,000 (500 million)
	string min_nucleotides_str = "16";        // -n:
	string max_nucleotides_str = "10000";    // -N:
	string period_s = "4-8";                 // -p:
	string max_task_queue_size_str = "1000"; // -Q:
	string min_repeats_str = "2";            // -r:
	string max_repeats_str = "1000";         // -R:
	string enumerated_ssrs_str = "";         // -s:
	string threads_str = "1";                // -t:
	
	int c;

	if (argc > 1)
	{
		// search for -v
		while ( (c = getopt(argc,argv,"+:v")) != -1 )
		{
			switch (c)
			{
				case 'v':
					this->version_displayed = true;
					cerr << "Version: " << this->version << endl;
					return;
				case ':': break;
				case '?': break;
				default: break;
			}
		}
		
		optind = 1; // reset optind to search for -h

		// search for -h
		while ( (c = getopt(argc,argv,"+:h")) != -1 )
		{
			switch (c)
			{
				case 'h':
					this->help_displayed = true;
					cerr << this->usage_message << endl << endl << this->help_message << endl << endl;
					return;
				case ':': break;
				case '?': break;
				default: break;
			}
		}

		optind = 1; // reset optind to search for everything else
	
		// search for everything else
		while ( (c = getopt(argc,argv,"a:AbBdegGhi:l:L:n:N:o:p:Q:r:R:s:t:v")) != -1 )
		{
			switch (c)
			{
				case 'a': alphabet_str = optarg; break;
				case 'A': this->allow_non_atomic = true; break;
				case 'b': this->bzipped2_input = true; break;
				case 'B': this->bzipped2_output = true; break;
				case 'd': this->display_progress_bar = false; break;
				case 'e': this->exhaustive = true; break;
				case 'g': this->gzipped_input = true; break;
				case 'G': this->gzipped_output = true; break;
				case 'h': break;
				case 'i': this->input_file_name = optarg; break;
				case 'l': min_seq_len_str = optarg; break;
				case 'L': max_seq_len_str = optarg; break;
				case 'n': min_nucleotides_str = optarg; break;
				case 'N': max_nucleotides_str = optarg; break;
				case 'o': this->output_file_name = optarg; break;
				case 'p': period_s = optarg; break;
				case 'Q': max_task_queue_size_str = optarg; break;
				case 'r': min_repeats_str = optarg; break;
				case 'R': max_repeats_str = optarg; break;
				case 's': enumerated_ssrs_str = optarg; break;
				case 't': threads_str = optarg; break;
				case 'v': break;
				case '?':
					if (optopt == 'c')
					{
						fprintf(stderr,"Option -%c requires an argument. Run with -h for help.\n",optopt);
						this->help_displayed = true;
					}
					else if (isprint(optopt))
					{
						fprintf(stderr,"Unknown option `-%c'. Run with -h for help.\n",optopt);
						this->help_displayed = true;
					}
					else
					{
						fprintf(stderr,"Unknown option character `\\x%x'. Run with -h for help.\n",optopt);
						this->help_displayed = true;
					}
					return;
				default:
					cout << "abort, c: " << (char) c << endl;
					abort();
			}
		}
	}
	else if (argc == 1)
	{
		cerr << "Note: you are receiving input from STDIN and sending output to STDOUT\n" << endl;
	}

	// check for positional parameters (which is an error)
	if (optind < argc)
	{
		string error = "ERROR: You may not supply any positional parameters. Offenders:";

		for (uint32_t i = optind; i < (uint32_t) argc; i++)
		{
			error += " " + string(argv[i]);
		}

		throw error;
	}

	// assign member variables now that we've parsed the arguments
	this->threads = parsePositiveIntegerArgument(threads_str);
	this->addToAlphabet(alphabet_str);
	this->min_sequence_length = parsePositiveIntegerArgument(min_seq_len_str);
	this->max_sequence_length = parsePositiveIntegerArgument(max_seq_len_str);
	this->min_nucleotides = parsePositiveIntegerArgument(min_nucleotides_str);
	this->max_nucleotides = parsePositiveIntegerArgument(max_nucleotides_str);
	this->min_repeats = parsePositiveIntegerArgument(min_repeats_str);
	this->max_repeats = parsePositiveIntegerArgument(max_repeats_str);
	this->max_task_queue_size = parsePositiveIntegerArgument(max_task_queue_size_str);
	this->addToEnumeratedSSRs(enumerated_ssrs_str);
	this->addToPeriods(period_s);

	// adjust arguments for speed reasons
	this->adjustMinSequenceLength();

	// sanity check
	this->sanityCheckArguments();

	this->autoDetectCompressedInput();
	this->autoDetectCompressedOutput();
	this->display_progress_bar = this->input_file_name != "/dev/stdin" ? this->display_progress_bar : false; // if stdin..we can't determine size of data
}

void Arguments::autoDetectCompressedInput()
{
	if (this->input_file_name.size() >= 3 && this->input_file_name.substr(this->input_file_name.size() - 3, 3) == ".gz")
	{
		this->gzipped_input = true;
	}
	if (this->input_file_name.size() >= 4 && this->input_file_name.substr(this->input_file_name.size() - 4, 4) == ".bz2")
	{
		this->bzipped2_input = true;
	}
	if (this->bzipped2_input && this->gzipped_input)
	{
		throw "ERROR: Your input cannot be compressed with both gzip and bzip2.";
	}
}

void Arguments::autoDetectCompressedOutput()
{
	if (this->output_file_name.size() >= 3 && this->output_file_name.substr(this->output_file_name.size() - 3, 3) == ".gz")
	{
		this->gzipped_output = true;
	}
	if (this->output_file_name.size() >= 4 && this->output_file_name.substr(this->output_file_name.size() - 4, 4) == ".bz2")
	{
		this->bzipped2_output = true;
	}
	if (this->bzipped2_output && this->gzipped_output)
	{
		throw "ERROR: Your output cannot be compressed with both gzip and bzip2.";
	}
}

void Arguments::sanityCheckArguments() const
{
	if (this->min_nucleotides > this->max_nucleotides)
	{
		throw "ERROR: -n must not be > than -N";
	}
	
	if (this->min_repeats > this->max_repeats)
	{
		throw "ERROR: -r must not be > than -R";
	}

	if (this->min_sequence_length > this->max_sequence_length)
	{
		throw "ERROR: -l must not be > than -L";
	}
}

void Arguments::adjustMinSequenceLength()
{
	// if the minimum period size we're searching for cannot repeat in
	// the minimum sequence size, why even look in those sequences?
	if (this->min_sequence_length < *this->periods->rbegin() * 2)
	{
		this->min_sequence_length = *this->periods->rbegin() * 2;
	}
}

void Arguments::addToAlphabet(string alphabet_str)
{
	for (uint32_t i = 0; i < alphabet_str.size(); ++i)
	{
		if (!isalpha(alphabet_str[i]) && alphabet_str[i] != ',')
		{
			throw string("ERROR: '") + alphabet_str[i] + "' is not a valid alphabet character.";
		}

		this->alphabet->insert(toupper(alphabet_str[i]));
	}
}
vector<string> Arguments::split(string str, char sep)
{
	vector<string> v;
	vector<uint32_t> starts = {0};
	vector<uint32_t> ends;

	for (uint32_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == sep)
		{
			ends.push_back(i);

			if (starts[starts.size() - 1] == ends[ends.size() - 1])
			{
				starts.pop_back();
				ends.pop_back();
			}

			starts.push_back(i + 1);
		}
	}

	ends.push_back(str.size());

	if (starts[starts.size() - 1] >= str.size())
	{
		starts.pop_back();
		ends.pop_back();
	}

	if (starts.size() != ends.size())
	{
		throw "ERROR: You should never see this, but starts and ends had different lengths in Arguments::split.";
	}

	for (uint32_t i = 0; i < starts.size(); ++i)
	{
		v.push_back(str.substr(starts[i], ends[i] - starts[i]));
	}

	return v;
}
void Arguments::addToEnumeratedSSRs(string enumerated_ssrs_str)
{
	if (enumerated_ssrs_str == "")
	{
		return;
	}
	
	for (uint32_t i = 0; i < enumerated_ssrs_str.size(); ++i)
	{
		if ( !(isalpha(enumerated_ssrs_str[i]) or enumerated_ssrs_str[i] == ',') )
		{
			throw string("ERROR: '") + enumerated_ssrs_str[i] + "' is not a valid character for the -s option.";
		}

		enumerated_ssrs_str[i] = toupper(enumerated_ssrs_str[i]);
	}

	vector<string> ssrs = split(enumerated_ssrs_str, ',');

	for (uint32_t i = 0; i < ssrs.size(); ++i)
	{
		if (!ssrs[i].empty())
		{
			this->enumerated_ssrs->insert(ssrs[i]);
		}
	}
}
void Arguments::addToPeriods(string period_s)
{
	for (uint32_t i = 0; i < period_s.size(); ++i)
	{
		if (period_s[i])
		{
			if ( !(isdigit(period_s[i]) or period_s[i] == ',' or period_s[i] == '-') )
			{
				throw string("ERROR: '") + period_s[i] + "' is not a valid character for the -p option.";
			}

			period_s[i] = toupper(period_s[i]);
		}
	}

	vector<string> pers = split(period_s, ',');

	for (uint32_t i = 0; i < pers.size(); ++i)
	{
		if (!pers.empty())
		{
			vector<string> per = split(pers[i], '-');

			if (per.size() == 2)
			{
				for (uint32_t j = strtol(per[0].c_str(), NULL, 10); j <= strtol(per[1].c_str(), NULL, 10); ++j)
				{
					this->periods->insert(j);
				}
			}
			else if (per.size() == 1)
			{
				this->periods->insert(strtol(per[0].c_str(), NULL, 10));
			}
			else
			{
				throw "ERROR: A range can only contain 2 numbers separated by a hyphen.";
			}
		}
	}
}
