/*	
	Brandon Pickett
	FindSSRs.cpp
*/

#include "../include/FindSSRs.h"

using namespace std;

static
uint32_t calculateDataSizeFromFasta(ifstream &fasta);

FindSSRs::FindSSRs(Arguments* _args) : out_file(_args->getOutFileName())
{
	this->args = _args;
	this->num_threads = this->args->getNumThreads();
	sem_init(&(this->n),0,0);
	sem_init(&(this->e),0,(this->num_threads * 2));
	sem_init(&(this->d),0,1);
	sem_init(&(this->s),0,0);
	sem_init(&(this->lock),0,1);
	this->finished_threads = 1;
	
	this->progress_bar = ProgressMeter();
	this->atomicity_checker = AtomicityChecker();
}
FindSSRs::~FindSSRs()
{
	this->out.close();
	//sem_destroy(&(this->n));
	//sem_destroy(&(this->e));
}
sem_t* FindSSRs::getN() const
{
	return (sem_t*) &(this->n);
}
sem_t* FindSSRs::getE() const
{
	return (sem_t*) &(this->e);
}
sem_t* FindSSRs::getD() const
{
	return (sem_t*) &(this->d);
}
sem_t* FindSSRs::getS() const
{
	return (sem_t*) &(this->s);
}
sem_t* FindSSRs::getLock() const
{
	return (sem_t*) &(this->lock);
}
uint32_t FindSSRs::getFinishedThreadsCount() const
{
	//uint32_t temp;
	//sem_wait(&(this->d)); // acquire lock for finished threads
	//temp = this->finished_threads;
	//sem_post(&(this->d)); // release lock for finished threads
	//return temp;
	return this->finished_threads;
}
void FindSSRs::incrementFinishedThreads()
{
	sem_wait(&(this->d)); // acquire lock for finished threads
	++this->finished_threads;
	sem_post(&(this->d)); // release lock for finished threads
}
uint32_t FindSSRs::run()
{
	uint32_t error = this->makeThreads(); // set up consumers

	if (error) { return error; }

	try
	{
		this->processInput(); // produce (and consume if only main thread)
	}
	catch (string e)
	{
		cerr << e << endl;
		return 1;
	}

	//cerr << "About to start joining threads..." << endl;
	//this->joinAndForgetAllThreads(); // clean up consumers
	
	while (this->finished_threads < this->num_threads)
	{
		//cerr << "Waiting on threads! (" << this->finished_threads << "/" << this->num_threads << ")" << endl;
		sem_wait(&(this->s)); // wait for signal from a thread
	}
	
	//int nval, eval, dval, sval = 0;
	//sem_getvalue(&(this->n), &nval);
	//sem_getvalue(&(this->e), &eval);
	//sem_getvalue(&(this->d), &dval);
	//sem_getvalue(&(this->s), &sval);
	//cerr << "final n,e,d,s: " << nval << "," << eval << "," << dval << "," << sval << endl;

	return 0;
}
uint32_t FindSSRs::makeThreads()
{
	pthread_attr_t tattr;
	pthread_attr_init(&tattr);
	//pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE); // Maybe omit because this is already default?
	pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
	
	for (uint32_t i = 1; i < this->num_threads; ++i)
	{
		pthread_t thread;
		this->threads.push_back(thread);

		//if ( pthread_create(&thread,NULL,&consume,(void*) this) != 0 )
		if ( pthread_create(&thread,&tattr,&consume,(void*) this) != 0 )
		{
			perror("creating threads");
			return errno;
		}
	}

	pthread_attr_destroy(&tattr);

	return 0;
}
void FindSSRs::joinAndForgetAllThreads()
{
	for (uint32_t i = 0; i < this->threads.size(); ++i)
	{
		//cerr << "Attempting to join thread #" << i << " with ID=" << this->threads[i] << endl;
		long status = 0L;
		long* status_ptr = &status;

		int joinerr = 0;
		joinerr = pthread_join(this->threads[i],(void**) &status_ptr); //if ( pthread_join(this->threads[i],(void**) &status_ptr) != 0)
		if ( joinerr != 0 && joinerr != EINVAL)
		{
			perror("joining thread");
			//cerr << "joinerr,errno" << joinerr << "," << errno << endl;
		}
	}

	this->threads.clear();
}
void FindSSRs::processInput() // produce
{
	this->out.open(this->args->getOutFileName().c_str());
	
	ifstream species1_in_file;
	species1_in_file.open(this->args->getInFileName().c_str());

	// init (set the data_size of) the progress bar
	this->progress_bar.initialize(calculateDataSizeFromFasta(species1_in_file));

	string header = "";
	string sequence = "";
	string line = "";
	while (getline(species1_in_file, line))
	{
		if (line.size() > 0)
		{
			if (line[0] != '>') // line is a sequence
			{
				this->progress_bar.updateProgress(1, false); // +1 for the '\n'

				for (uint32_t i = 0; i < line.size(); ++i)
				{
					//sequence = sequence + (char) toupper(line[i]);
					sequence += (char) toupper(line[i]);
				}
			}
			else // line is a header
			{
				this->processSequence(header,sequence);

				header = line.substr(1, string::npos);
				sequence = "";
				
				this->progress_bar.updateProgress(header.size() + 2, false); // +1 for the '\n', +1 for the '>'
			}
		}
		else // empty line
		{
			this->progress_bar.updateProgress(1, false); // +1 for the '\n'
		}
		
		//if ( (line[0] != '>') && (line.size() >= this->args->getMinSequenceLength()) && (line.size() <= this->args->getMaxSequenceLength()) )
		//{
		//	line.append("$");
		//	switch (this->num_threads)
		//	{
		//		case 1:
		//			this->findSSRsInSequence(header, line);
		//			break;
		//		default:
		//			sem_wait(&(this->e)); // decrease num empty slots
		//			this->fasta_seqs.add(header, line); // fill a slot
		//			sem_post(&(this->n)); // increase num occupied slots
		//			break;
		//	}
		//}
		//else
		//{
		//	header = line;
		//}
	}
	
	this->processSequence(header,sequence); // catch the last sequence

	species1_in_file.close();

	switch (this->num_threads)
	{
		case 1:
			break;
		default:
			this->fasta_seqs.dryUp(); // tell the FastaSequences object it will never recieve more input
			for (uint32_t i = 1; i < this->num_threads; ++i)
			{
				sem_post(&(this->n)); // tell the consumers there's another thing to consume (which will be the stop code), aka, it will return a stop code.
			}
			break;
	}
}

void FindSSRs::processSequence(const string &header, string &sequence)
{
	if (sequence.size() == 0)
	{
		return;
	}
	
	//uint32_t seq_size = sequence.size();

	vector<uint32_t> starts = {0};
	vector<uint32_t> sizes;

	uint32_t ignored_chars_count = 0;
	splitStringOnIgnoredChars(starts, sizes, sequence, ignored_chars_count);
	this->progress_bar.updateProgress(ignored_chars_count, false);
	
	for (uint32_t i = 0; i < starts.size(); ++i)
	{
		string sub_seq = sequence.substr(starts[i], sizes[i]);

		switch (this->num_threads)
		{
			case 1:
				this->findSSRsInSequence(header, sub_seq, starts[i]);
				break;
			default:
				sem_wait(&(this->e)); // decrease num empty slots
				this->fasta_seqs.add(header, sub_seq, starts[i]); // fill a slot
				sem_post(&(this->n)); // increase num occupied slots
				break;
		}
	}
	
}

void FindSSRs::findSSRsInSequence(const string &header, const string &sequence, uint32_t ignore_chars_offset)
{
	
	vector<bool> filter(sequence.size(), false);
	vector<string> base_ssrs;
	vector<uint32_t> repeats;
	vector<uint32_t> positions;

	for (set<uint32_t>::iterator itr = this->args->periods->begin(); itr != this->args->periods->end(); ++itr)
	{
		// NOTE: "*itr" is the period size
		
		for (uint32_t i = 0; i < sequence.size(); ++i)
		{
			// helper
			string base = sequence.substr(i, *itr);
			string current = string(base);
			uint32_t count = 0;
			uint32_t start = i + *itr;
			while (start < sequence.size() && current == base)
			{
				++count;
				current = sequence.substr(start, *itr);
				start += *itr;
			}
			
			if (isGood(base, count, ignore_chars_offset + i, filter, ignore_chars_offset))
			{
				if ( (count >= this->args->getMinRepeats()) && (count <= this->args->getMaxRepeats()) && (base.size() * count >= this->args->getMinNucleotideLength()) && (base.size() * count <= this->args->getMaxNucleotideLength()) )
				{
					base_ssrs.push_back(base);
					repeats.push_back(count);
					positions.push_back(ignore_chars_offset + i);
				}

				for (uint32_t j = i; j < (base.size() * count + i); ++j)
				{
					filter[j] = true;
				}
				i += ((base.size() * count) - *itr - 1);
			}
		}
	}


	sem_wait(&(this->lock));
	for (uint32_t i = 0; i < base_ssrs.size(); ++i)
	{
		out << header << '\t' << base_ssrs[i] << '\t' << repeats[i] << '\t' << positions[i] << '\n';
	}
	sem_post(&(this->lock));
	this->progress_bar.updateProgress(sequence.size(), true);
}

bool FindSSRs::isGood(const string &base, uint32_t repeats, uint32_t position, const vector<bool> &filter, uint32_t offset)
{
	if (!this->args->enumerated_ssrs->empty() && this->args->enumerated_ssrs->count(base) == 0)
	{
		return false;
	}

	if (repeats < 2)
	{
		return false;
	}

	if (!this->atomicity_checker.isAtomic(base))
	{
		return false;
	}

	for (uint32_t i = position - offset; i < (base.size() * repeats + position - offset); ++i)
	{
		if (!filter[i])
		{
			return true;
		}
	}

	return false;
}

void FindSSRs::splitStringOnIgnoredChars(vector<uint32_t> &starts, vector<uint32_t> &sizes, const string &sequence, uint32_t &actually_ignored_chars)
{
	for (uint32_t i = 0; i < sequence.size(); ++i)
	{
		if (!this->args->alphabet->count(sequence[i])) // if it's a "bad" character
		{
			sizes.push_back(i - starts[starts.size() - 1]);
			++actually_ignored_chars;
		
			if (sizes[sizes.size() - 1] == 0)
			{
				starts.pop_back();
				sizes.pop_back();
			}

			starts.push_back(i + 1);
		}
	}

	sizes.push_back(sequence.size() - starts[starts.size() - 1]);

	if (sizes[sizes.size() - 1] == 0)
	{
		starts.pop_back();
		sizes.pop_back();
	}

	// Sanity check
	if (starts.size() != sizes.size())
	{
		cerr << "\n\tERROR! starts and sizes are not the same size!\n" << endl;
		exit(1);
	}
}
void FindSSRs::printExtraInformation(const string &header, const string &sequence, const int *SA, const int *LCP) // not thread safe!!
{
	out_file << "header: " << header << "\n";
	out_file << "sequence: " << sequence << "\n";
	out_file << " SA: ";
	for (uint32_t i = 0; i < sequence.size(); ++i)
	{
		string temp;
		stringstream strm;
		strm << SA[i];
		strm >> temp;
		out_file << temp << " ";
	}
	out_file << "\nLCP: - ";
	for (uint32_t i = 1; i < sequence.size(); ++i)
	{
		string temp;
		stringstream strm;
		strm << LCP[i];
		strm >> temp;
		out_file << temp << " ";
	}
	out_file << "\n" << "\n";
}
void* FindSSRs::consume(void* find_ssrs_vptr) // void* (*)(void* )
{
	bool go = true;
	FindSSRs* find_ssrs_ptr = (FindSSRs*) find_ssrs_vptr;
	string header;
	string sequence;
	uint32_t ignore_chars_offset = 0;
	while (go)
	{
		sem_wait(find_ssrs_ptr->getN()); // decrease num occupied slots
		if (!find_ssrs_ptr->fasta_seqs.get(header, sequence, ignore_chars_offset)) // take
		{
			sem_post(find_ssrs_ptr->getE()); // increase num empty slots
			find_ssrs_ptr->findSSRsInSequence(header, sequence, ignore_chars_offset); // consume (find the ssrs in the given sequence)
		}
		else
		{
			go = false;
		}
	}
	
	//cout << "I'm exiting!" << endl;
	//long* status = 0;
	//pthread_exit((void*) status);
	find_ssrs_ptr->incrementFinishedThreads();
	sem_post(find_ssrs_ptr->getS()); // send a signal to main thread
	return NULL;
}


// --------------- STATIC FUNCTIONS ------------------------------ ||

static
uint32_t calculateDataSizeFromFasta(ifstream &fasta)
{
	fasta.seekg(0, fasta.end); // move to the end
	uint32_t size = fasta.tellg(); // get position of the end
	fasta.seekg(0, fasta.beg); // move to the beginning
	fasta.clear(); // reset the error state flags, if necessary
	return size; // return the size
}

