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
	this->usage_message = "USAGE: kmer-ssr [-a a1,..,aN] [-A] [-b|-g] [-B|-G] [-d] [-e] [-h] [-i file]\n                [-n int] [-N int] [-o file] [-p p1,..,pN] [-r int] [-R int]\n                [-s s1,..,sN] [-t int] [-v]\n";
	this->help_message = this->generateHelpMessage();

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
bool Arguments::isExhaustive() const
{
	return this->exhaustive;
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
		return "\033[0;31mUSAGE: " + this->usage_message + " (-h for help)\033[0m";
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
string Arguments::getInFileName() const
{
	return this->input_file_name;
}
string Arguments::getOutFileName() const
{
	return this->output_file_name;
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
	string min_nucleotides_str = "2";        // -n:
	string max_nucleotides_str = "10000";    // -N:
	string period_s = "4-6";              // -p:
	string max_task_queue_size_str = "1000"; // -Q:
	string min_repeats_str = "2";            // -r:
	string max_repeats_str = "1000";         // -R:
	string enumerated_ssrs_str = "";         // -s:
	string threads_str = "1";                // -t:
	
	int c;

	while ( (c = getopt(argc,argv,"a:AbBdegGhi:n:N:o:p:Q:r:R:s:t:v")) != -1 )
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
			case 'h':
				cerr << this->usage_message << endl << endl << this->help_message << endl << endl; break;
			case 'i': this->input_file_name = optarg; break;
			case 'n': min_nucleotides_str = optarg; break;
			case 'N': max_nucleotides_str = optarg; break;
			case 'o': this->output_file_name = optarg; break;
			case 'p': period_s = optarg; break;
			case 'Q': max_task_queue_size_str = optarg; break;
			case 'r': min_repeats_str = optarg; break;
			case 'R': max_repeats_str = optarg; break;
			case 's': enumerated_ssrs_str = optarg; break;
			case 't': threads_str = optarg; break;
			case 'v':
				cerr << "Version: " << this->version << endl; break;
			case '?':
				if (optopt == 'c')
				{
					fprintf(stderr,"Option -%c requires an argument.\n",optopt);
				}
				else if (isprint(optopt))
				{
					fprintf(stderr,"Unknown option `-%c'.\n",optopt);
				}
				else
				{
					fprintf(stderr,"Unknown option character `\\x%x'.\n",optopt);
				}
				return;
			default:
				abort();
		}
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
	this->min_nucleotides = parsePositiveIntegerArgument(min_nucleotides_str);
	this->max_nucleotides = parsePositiveIntegerArgument(max_nucleotides_str);
	this->min_repeats = parsePositiveIntegerArgument(min_repeats_str);
	this->max_repeats = parsePositiveIntegerArgument(max_repeats_str);
	this->max_task_queue_size = parsePositiveIntegerArgument(max_task_queue_size_str);
	this->addToEnumeratedSSRs(enumerated_ssrs_str);
	this->addToPeriods(period_s);

	cout << "threads: " << this->threads << endl;
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
string Arguments::generateHelpMessage() const
{
	return "Find SSRs in FASTA sequence data "
		""
		"Input:"
		"  -b, -bi, -BI, --bzipped2-input"
		"			Input fasta file is compressed with bzip2. If the file"
		"			name supplied to '-i' ends in '.bz2', the input is"
		"			assumed to be compressed with bzip2, regardless of"
		"			whether you supplied this option or not. If the"
		"			compressed file is supplied via STDIN, you should (a)"
		"			use the bzcat utility (part of the bzip2 package) and"
		"			(b) not use '-b'."
		"  -g, -gi, -GI, --gzipped-input"
		"			Input fasta file is gzipped. If the file name supplied"
		"			to '-i' ends in '.gz', the input is assumed to be"
		"			gzipped, regardless of whether you supplied this"
		"			option or not. If the compressed file is supplied via"
		"			STDIN, you should (a) use the zcat utility (part of"
		"			the gzip package) and (b) not use '-g'."
		"  -i in.fasta[.gz|.bz2], --input in.fasta[.gz|.bz2]"
		"			The input file in fasta format, may be compressed with"
		"			gzip or bzip2 (see '-g' and '-b'). All sequence"
		"			characters will be converted to uppercase. [default:"
		"			stdin]"
		""
		"Output:"
		"  -B, -bo, -BO, --bzip2-output"
		"			Compress the output file with bzip2. This will *not*"
		"			automatically add '.bz2' to the end of any file name"
		"			supplied by '-o'. If the file name supplied to '-o'"
		"			already ends in '.bz2', the output will be compressed"
		"			with bzip2, regardless of whether you supplied this"
		"			option or not. If the '-o' option is not used, use"
		"			this flag to compress everything sent to stdout with"
		"			bzip2."
		"  -G, -go, -GO, --gzip-output"
		"			Gzip the output file. This will *not* automatically"
		"			add '.gz' to the end of any file name supplied by"
		"			'-o'. If the file name supplied to '-o' already ends"
		"			in '.gz', the output will be gzipped, regardless of"
		"			whether you supplied this option or not. If the '-o'"
		"			option is not used, use this flag to gzip everything"
		"			sent to stdout."
		"  -o out.tsv[.gz|.bz2], --output out.tsv[.gz|.bz2]"
		"			The output file in tab-separated value (tsv) format,"
		"			may be compressed with gzip or bzip2 (see '-G' and"
		"			'-B'). [default: stdout]"
		""
		"Algorithmic:"
		"  -a a1,..,aN, --alphabet a1,..,aN"
		"			A comma-separated list of valid, uppercase characters"
		"			(nucleotides). [default=A,C,G,T]"
		"  -A, --allow-non-atomic"
		"			Report non-atomic SSRs (e.g., AT repeated 6 times will"
		"			also report an ATAT repeated 3 times and an ATATAT"
		"			repeated 2 times)."
		"  -e, --exhaustive      Disable all filters and SSR validation to report every"
		"			SSR. Similar to: -A -r 2 -R <big_number> -n 2 -N"
		"			<big_number>. This will override any options set for"
		"			-n, -N, -r, -R, and -s."
		"  -p p1,..,pN, --periods p1,..,pN"
		"			A comma-separated list of period sizes (i.e., kmer"
		"			lengths). Inclusive ranges are also supported using a"
		"			hyphen. [default=4-6]"
		"  -n int, --min-nucleotides int"
		"			Keep only SSRs with total length (number of"
		"			nucleotides) >= n [default: 2]"
		"  -N int, --max-nucleotides int"
		"			Keep only SSRs with total length (number of"
		"			nucleotides) <= N [default: 10,000]"
		"  -r int, --min-repeats int"
		"			Keep only SSRs that repeat >= r times [default: 2]"
		"  -R int, --max-repeats int"
		"			Keep only SSRs that repeat <= R times [default:"
		"			10,000]"
		"  -s s1,..,sN, --ssrs s1,..,sN"
		"			A comma-separated list of SSRs to search for; e.g."
		"			\"AC,GTTA,TTCTG,CCG\" or \"TGA\". Please note that other"
		"			options may prevent SSRs specified with this option"
		"			from appearing in the output. For example, if -p is"
		"			\"4-6\", then an SSR with a repeating \"AC\" will never be"
		"			displayed because \"AC\" has a period size of 2 (and, as"
		"			it turns out, 2 is not in the range 4-6)."
		""
		"Misc:"
		"  -d, --disable-progress-bar"
		"			Disable the progress bar that normally prints to"
		"			stderr. Will automatically be disabled if (a) reading"
		"			from stdin or (b) writing to stdout without"
		"			redirecting it to a file."
		"  -h, --help            Show this help message and exit"
		"  -Q int, --max-task-queue-size int"
		"			[default: 1,000]"
		"  -t int, --threads int"
		"			Number of threads [default: 1]"
		"  -v, --version         Show version number and exit";
}
