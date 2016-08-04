/*
	Brandon Pickett
	ProgressMeter.h
*/

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <semaphore.h>
#include <cstdlib>

using namespace std;

class ProgressMeter
{
private:
	uint32_t bar_width;
	uint32_t data_done;
	uint32_t data_size;
	float progress;
	sem_t lock;
	bool display;

private:
	void setUp();
	uint32_t calculateWidth() const;
	void updateMeter();

public:
	ProgressMeter(bool _display);
	~ProgressMeter();
	void initialize(uint32_t _data_size);
	void updateProgress(uint32_t additional_completed, bool update_display);
};
