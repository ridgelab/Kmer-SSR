/*
	Brandon Pickett
	ProgressMeter.cpp
*/

#include "../include/ProgressMeter.h"

using namespace std;

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------     PUBLIC   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

ProgressMeter::ProgressMeter(bool _display)
{
	this->display = _display;
	this->setUp();
}
ProgressMeter::~ProgressMeter()
{
	return;
}
void ProgressMeter::initialize(uint32_t _data_size)
{
	sem_wait(&(this->lock));

	this->data_size = _data_size;

	if (this->display)
	{
		cerr << "\nFinding SSRs:" << endl;
	}

	sem_post(&(this->lock));
}
void ProgressMeter::updateProgress(uint32_t additional_completed, bool update_display)
{
	sem_wait(&(this->lock));

	this->data_done += additional_completed;
	this->progress = (1.0 * this->data_done) / this->data_size;

	if (this->display && update_display)
	{
		this->updateMeter();
	}
	
	sem_post(&(this->lock));
}

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------    PRIVATE   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

void ProgressMeter::setUp()
{
	this->bar_width = this->calculateWidth() - 7; // 7 because of "[" and "]" and " 100%" (5 chars) at max.  At min, " 4%" (3 chars). In the middle, " 76%" (4 chars).
	
	this->data_done = 0;
	this->data_size = 0;

	this->progress = 0.0;

	sem_init(&(this->lock),0,1);
}
uint32_t ProgressMeter::calculateWidth() const
{
	uint32_t screen_width = 0;

	screen_width = 80;

	return screen_width;
}
void ProgressMeter::updateMeter()
{
	cerr << "[";
	
	uint32_t pos = this->progress * this->bar_width;
	for (uint32_t i = 0; i < this->bar_width; ++i)
	{
		if (i <= pos)
		{
			cerr << "#";
		}
		else
		{
			cerr << " ";
		}
	}

	cerr << "] " << (uint32_t) (this->progress * 100) << "%";

	if (this->progress < 1.0)
	{
		cerr << "\r";
	}
	else //if (this->progress == 1.0)
	{
		cerr << "\n\n";
	}
	//else
	//{
	//	cerr << "\nsomething's wrong!  progress,datadone,datasize: " << this->progress << "," << this->data_done << "," << this->data_size << "\n\n";
	//}

	cerr.flush();
}
