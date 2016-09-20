/*	
	Brandon Pickett
	FindSSRs.cpp
*/

#include "../include/FindSSRs.h"

static uint32_t calculateDataSizeFromFasta(ifstream &fasta);
static void* consume(void* consumer_args_vptr);
static bool isGoodSSR(SSR* ssr, uint32_t global_pos, const vector<bool> &filter, Arguments* args, AtomicityChecker* atomicity_checker);
static bool keepSSR(SSR* ssr, Arguments* args);
static vector<SSR*>* findSSRsExhaustively(Task* task, set<uint32_t, bool(*)(uint32_t,uint32_t)>* periods);
static vector<SSR*>* findSSRsNormally(Task* task, Arguments* args, AtomicityChecker* atomicity_checker);
static vector<SSR*>* findSSRs(Task* task, Arguments* args, AtomicityChecker* atomicity_checker);

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------    STRUCTS   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

struct ConsumerArguments
{
	TaskQueue* tasks_ptr;
	ProgressMeter* progress_bar_ptr;
	OutputFile* ofd_ptr;
	AtomicityChecker* atomicity_checker_ptr;
	Arguments* arguments_ptr;

	ConsumerArguments(TaskQueue* _tasks_ptr, ProgressMeter* _progress_bar_ptr, OutputFile* _ofd_ptr, AtomicityChecker* _atomicity_checker_ptr, Arguments* _arguments_ptr)
	{
		tasks_ptr = _tasks_ptr;
		progress_bar_ptr = _progress_bar_ptr;
		ofd_ptr = _ofd_ptr;
		atomicity_checker_ptr = _atomicity_checker_ptr;
		arguments_ptr = _arguments_ptr;
	}
	~ConsumerArguments()
	{
		/*
		 * 	We are intentionally NOT freeing the memory for the pointers that
		 * 	make up the members of this struct. The duty to free that memory
		 * 	will fall upon those who initially created those objects.
		 */
		return;
	}
};


// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------     PUBLIC   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

//FindSSRs::FindSSRs(Arguments* _args) : out_file(_args->getOutFileName())
FindSSRs::FindSSRs(Arguments* _args)
{
	this->args = _args;
	this->out_file = new OutputFile(this->args->getOutFileName());
	this->out_file->write("#Sequence_Name\tSSR\tRepeats\tPosition\n");
	this->progress_bar = new ProgressMeter(this->args->displayProgressBar());
	this->atomicity_checker = new AtomicityChecker();
	this->tasks = new TaskQueue(this->args->getMaxTaskQueueSize());
}
FindSSRs::~FindSSRs()
{
	delete this->out_file;
	delete this->progress_bar;
	delete this->atomicity_checker;
	delete this->tasks;
}

uint32_t FindSSRs::run()
{
	// set up consumers
	uint32_t error = this->makeThreads(); // set up consumers

	if (error)
	{
		return error;
	}

	// find the ssrs in the sequences
	try
	{
		this->processInput(); // produce (and consume if only main thread)
	}
	catch (string e)
	{
		cerr << e << endl;
		return 1;
	}
	catch (const char* e)
	{
		cerr << e << endl;
		return 1;
	}

	// join threads
	//cerr << "About to start joining threads..." << endl;
	//usleep(3000000);
	this->joinAndForgetAllThreads(); // clean up consumers

	// we're done!
	return 0;
}


// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------    PRIVATE   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

uint32_t FindSSRs::makeThreads()
{
	pthread_attr_t tattr;
	pthread_attr_init(&tattr);
	pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
	//pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
	
	for (uint32_t i = 1; i < this->args->getNumThreads(); ++i)
	{
		pthread_t thread;
		this->threads.push_back(thread);

		if ( pthread_create(&(this->threads[this->threads.size() - 1]), &tattr, &consume, (void*) new ConsumerArguments(this->tasks, this->progress_bar, this->out_file, this->atomicity_checker, this->args)) != 0 )
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

void FindSSRs::splitStringOnIgnoredChars(vector<uint32_t> &starts, vector<uint32_t> &sizes, const string &sequence, uint32_t &actually_ignored_chars)
{
	for (uint32_t i = 0; i < sequence.size(); ++i)
	{
		//if (!this->args->alphabet->count(sequence[i])) // if it's a "bad" character
		if (!this->args->inAlphabet(sequence[i])) // if it's a "bad" character
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

void FindSSRs::processInput() // produce
{
	ifstream fasta_file;
	fasta_file.open(this->args->getInFileName().c_str());

	// init (set the data_size of) the progress bar
	uint32_t init_size = this->args->getInFileName() == "/dev/stdin" ? 0 : calculateDataSizeFromFasta(fasta_file);
	this->progress_bar->initialize(init_size);

	string header = "";
	string sequence = "";
	string line = "";
	while (getline(fasta_file, line))
	{
		if (line.size())
		{
			if (line[0] != '>') // line is a sequence
			{
				this->progress_bar->updateProgress(1, false); // +1 for the '\n'

				for (uint32_t i = 0; i < line.size(); ++i) // -1 because we don't want to add the '\n' to the sequence
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
				
				this->progress_bar->updateProgress(header.size() + 2, false); // +1 for the '\n', +1 for the '>'
			}
		}
		else // empty line
		{
			this->progress_bar->updateProgress(1, false); // +1 for the '\n'
		}
		
	}
	
	this->processSequence(header,sequence); // catch the last sequence

	fasta_file.close(); // close the input file

	// put some NULL tasks in the TaskQueue, signalling the threads they can quit when they encounter one
	switch (this->args->getNumThreads())
	{
		case 1:
			break;
		default:
			for (uint32_t i = 1; i < this->args->getNumThreads(); ++i)
			{
				this->tasks->add(nullptr);
			}
			break;
	}

	// if there are plenty of tasks left (more than 2 tasks, excluding the dummy tasks, per thread)
	if (this->tasks->size() > (this->args->getNumThreads() * 3))
	{
		// the main thread becomes a consumer for a while
		SSRcontainer* ssrs = new SSRcontainer();
		Task* task = nullptr;

		// but only until each thread will have 2 tasks left (excluding the dummy task)
		while (this->tasks->size() > (this->args->getNumThreads() * 3))
		{
			// get a task
			task = this->tasks->get(); // get a task
		
			// fulfill the task, add the results to the SSRcontainer, attempt to write them to file
			ssrs->add(task->getID(), findSSRs(task, this->args, this->atomicity_checker));
			ssrs->writeToFile(this->out_file, false, true); // false means it won't block if it can't write, true means it will clear the SSRcontainer if it wrote
		
			// let the user know we've made some progress
			progress_bar->updateProgress(task->size(), true);

			// delete the ask now that we've completed it
			delete task;
		}

		// if we didn't write everything, let's do it now
		if (!ssrs->empty())
		{
			ssrs->writeToFile(this->out_file, true, false); // true means it will block until it can write, false means it won't clear the SSRcontainer when finished (because we're about to delete the thing anyway right after this)
		}

		delete ssrs;
	}
}

void FindSSRs::processSequence(const string &header, const string &sequence)
{
	if ( (sequence.size() == 0) || (sequence.size() < this->args->getMinSequenceLength()) || (sequence.size() > this->args->getMaxSequenceLength()) )
	{
		this->progress_bar->updateProgress(sequence.size(), true);
		return;
	}
	
	vector<uint32_t> starts = {0};
	vector<uint32_t> sizes;
	uint32_t ignored_chars_count = 0;

	splitStringOnIgnoredChars(starts, sizes, sequence, ignored_chars_count);

	this->progress_bar->updateProgress(ignored_chars_count, false); // false means don't update the display

	switch (this->args->getNumThreads())
	{
		case 1:
		{
			SSRcontainer* ssrs = new SSRcontainer();
			Task* task = nullptr;

			for (uint32_t i = 0; i < starts.size(); ++i)
			{
				// create the task
				task = new Task(header, sequence.substr(starts[i], sizes[i]), starts[i], sequence.size());
				
				// fulfill the task, add the results to the SSRcontainer so we can write them to file
				ssrs->add(task->getID(), findSSRs(task, this->args, this->atomicity_checker));
				ssrs->writeToFile(this->out_file, true, true); // true means it will block if it can't write (it will always be able to write bcuz it has no competition), the second true means clear the SSRcontainer after writing
				
				// let the user know we've made some progress
				progress_bar->updateProgress(task->size(), true);
			
				// delete the ask now that we've completed it
				delete task;
			}

			delete ssrs;

			break;
		}
		default:
		{
			for (uint32_t i = 0; i < starts.size(); ++i)
			{
				// add a new task to the TaskQueue (so another thread can process it)
				this->tasks->add(new Task(header, sequence.substr(starts[i], sizes[i]), starts[i], sequence.size()));
			}
			
			break;
		}
	}
}



// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------     STATIC   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

static
uint32_t calculateDataSizeFromFasta(ifstream &fasta)
{
	fasta.seekg(0, fasta.end); // move to the end
	uint32_t size = fasta.tellg(); // get position of the end
	fasta.seekg(0, fasta.beg); // move to the beginning
	fasta.clear(); // reset the error state flags, if necessary
	return size; // return the size
}

static
void* consume(void* consumer_args_vptr)
{
	// cast void pointer to correct pointer type
	ConsumerArguments* consumer_args = (ConsumerArguments*) consumer_args_vptr;

	// Extract the arguments out of the struct
	TaskQueue* tasks = consumer_args->tasks_ptr;
	ProgressMeter* progress_bar = consumer_args->progress_bar_ptr;
	OutputFile* ofd = consumer_args->ofd_ptr;
	AtomicityChecker* atomicity_checker = consumer_args->atomicity_checker_ptr;
	Arguments* args = consumer_args->arguments_ptr;

	// create a container for ssrs
	SSRcontainer* ssrs = new SSRcontainer();
	Task* task = nullptr;

	// while we have work to do
	while (true)
	{
		// get a task to perform
		task = tasks->get();
	
		// if the task is NULL, it's a signal that we're all done here
		if (task == nullptr)
		{
			break; // stop working because there isn't any work left to do
		}

		// add the ssrs to the container and *attempt* to write all ssrs in the container to file
		ssrs->add(task->getID(), findSSRs(task, args, atomicity_checker));
		ssrs->writeToFile(ofd, false, true); // false means it won't block if it can't write, true means clear the SSRcontainer (if it wrote)

		// update the progress bar with the work done
		progress_bar->updateProgress(task->size(), true);

		delete task;
	}

	// if we haven't printed out all the ssrs yet, do it
	if (!ssrs->empty())
	{
		ssrs->writeToFile(ofd, true, false); // true means it will block until it can write, false don't means clear the SSRcontainer (because we're about to delete it anyway)
	}

	// delete the arguments pointer and the SSRcontainer
	delete consumer_args;
	delete ssrs;

	return nullptr;
	//pthread_exit(NULL);
}

static
SSR* seekSinglePeriodSizeSSRatIndex(const string &sequence, uint32_t index, uint32_t global_pos, uint32_t period)
{
	//assert((period * 2) <= sequence.size());
	
	string base = sequence.substr(index, period);
	string next = base;
	uint32_t repeats = 0;
	uint32_t pos = index;

	while ( (base == next) && (pos < (sequence.size() - period + 1)) )
	{
		++repeats;
		pos += period;
		next = sequence.substr(pos,  period);
	}

	return new SSR(base, repeats, index + global_pos);
}

static
bool isGoodSSR(SSR* ssr, uint32_t global_pos, const vector<bool> &filter, Arguments* args, AtomicityChecker* atomicity_checker)
{
	// check enumerated SSRs
	//if (args->ssrs->size() && !args->ssrs->count(ssr->getBaseSSR()))
	if (args->areEnumeratedSSRs() && !args->baseSSRinEnumeratedSSRs(ssr->getBaseSSR()))
	{
		return false; // the user enumerated SSRs and this one wasn't one of them
	}

	// check the number of repeats
	if (ssr->getRepeats() < 2)
	{
		return false; // this ssr doesn't repeat, making it not an ssr
	}

	// check atomicity
	if (!args->allowNonAtomic() && !atomicity_checker->isAtomic(ssr->getBaseSSR()))
	{
		return false; // this SSR was not atomic (e.g., ATAT instead of AT)
	}

	// check overlap of entire SSR
	for (uint32_t i = ssr->getStartPosition() - global_pos; i < ssr->getExclusiveEndPosition() - global_pos; ++i)
	{
		if (!filter[i])
		{
			return true;
		}
	}

	return false;
}

static
bool keepSSR(SSR* ssr, Arguments* args)
{
	// check the number of repeats
	if ( (ssr->getRepeats() < args->getMinRepeats()) || (ssr->getRepeats() > args->getMaxRepeats()) )
	{
		return false;
	}

	// check the entire ssr length
	if ( (ssr->getLength() < args->getMinNucleotideLength()) || (ssr->getLength() > args->getMaxNucleotideLength()) )
	{
		return false;
	}

	return true;
}

static
vector<SSR*>* findSSRsExhaustively(Task* task, set<uint32_t, bool(*)(uint32_t,uint32_t)>* periods)
{
	vector<SSR*>* ssrs = new vector<SSR*>();

	string seq = task->getSequence();
	//vector<uint32_t> periods = task->getPeriods();

	//for (uint32_t i = 0; i < periods.size(); ++i)
	for (set<uint32_t, bool(*)(uint32_t,uint32_t)>::iterator itr = periods->begin(); itr != periods->end(); ++itr)
	{
		for (uint32_t index = 0; index < task->size(); ++index)
		{
			//ssrs.push_back(seekSinglePeriodSizeSSRatIndex(task->getSequenceRef(), index, task->getGlobalPosition(), periods[i]));
			//ssrs->push_back(seekSinglePeriodSizeSSRatIndex(seq, index, task->getGlobalPosition(), periods[i]));
			ssrs->push_back(seekSinglePeriodSizeSSRatIndex(seq, index, task->getGlobalPosition(), *itr));
		}
	}

	return ssrs;
}

static
vector<SSR*>* findSSRsNormally(Task* task, Arguments* args, AtomicityChecker* atomicity_checker)
{
	vector<bool>* filter = new vector<bool>(task->size(), false);

	vector<SSR*>* ssrs = new vector<SSR*>();

	string seq = task->getSequence();
	set<uint32_t, bool(*)(uint32_t,uint32_t)>* periods = args->getPeriods();
	SSR* ssr = nullptr;
	
	//for (uint32_t i = 0; i < periods.size(); ++i)
	for (set<uint32_t, bool(*)(uint32_t,uint32_t)>::iterator itr = periods->begin(); itr != periods->end(); ++itr)
	{
		if ( (*itr * 2) <= seq.size() )
		{
			for (uint32_t index = 0; index < task->size(); ++index)
			{
				//ssr = seekSinglePeriodSizeSSRatIndex(seq, index, task->getGlobalPosition(), periods[i]);
				ssr = seekSinglePeriodSizeSSRatIndex(seq, index, task->getGlobalPosition(), *itr);

				if (isGoodSSR(ssr, task->getGlobalPosition(), *filter, args, atomicity_checker))
				{
					for (uint32_t j = (ssr->getStartPosition() - task->getGlobalPosition()); j < (ssr->getExclusiveEndPosition() - task->getGlobalPosition()); ++j)
					{
						filter->at(j) = true;
					}

					//index += (ssr->getLength() - periods[i] - 1);
					index += (ssr->getLength() - *itr - 1);

					if (keepSSR(ssr, args))
					{
						ssrs->push_back(ssr);
					}
					else
					{
						delete ssr;
					}
				}
				else
				{
					delete ssr;
				}
			}
		}
	}

	filter->clear();
	delete filter;

	return ssrs;
}

static
vector<SSR*>* findSSRs(Task* task, Arguments* args, AtomicityChecker* atomicity_checker)
{
	if (args->doExhaustiveSearch())
	{
		return findSSRsExhaustively(task, args->getPeriods());
	}

	return findSSRsNormally(task, args, atomicity_checker);
}
