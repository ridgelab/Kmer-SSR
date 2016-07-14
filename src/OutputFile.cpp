/*
	Brandon Pickett
	OutputFile.cpp
*/

#include "../include/OutputFile.h"

using namespace std;

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------     PUBLIC   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

OutputFile::OutputFile()
{
	this->setUp();
}
OutputFile::OutputFile(const string &file_name)
{
	this->setUp(file_name);
}
OutputFile::OutputFile(const string &file_name, const char* header)
{
	this->setUp(file_name);
	this->writeHeader(header);
}
OutputFile::OutputFile(const string &file_name, const string &header)
{
	this->setUp(file_name);
	this->writeHeader(header);
}
OutputFile::OutputFile(const string &file_name, const vector<string> &headers)
{
	this->setUp(file_name);
	this->writeHeaders(headers);
}
OutputFile::~OutputFile()
{
	this->out_file.close();
}
void OutputFile::changeFile(const string &file_name)
{
	sem_wait(&(this->lock));
	if (this->out_file.is_open())
	{
		this->out_file.close();
	}
	this->out_file.open(file_name.c_str());
	if (!this->out_file.is_open())
	{
		exit(1);
	}
	sem_post(&(this->lock));
}
void OutputFile::changeFile(const string &file_name, const vector<string> &headers)
{
	this->changeFile(file_name);
	this->writeHeaders(headers);
}
OutputFile& OutputFile::write(const char* output)
{
	sem_wait(&(this->lock));
	this->out_file << output;
	sem_post(&(this->lock));
	return *this;
}
OutputFile& OutputFile::write(const string &output)
{
	sem_wait(&(this->lock));
	this->out_file << output;
	sem_post(&(this->lock));
	return *this;
}
OutputFile& OutputFile::write(const vector<string> &output)
{
	sem_wait(&(this->lock));
	for (uint32_t i = 0; i < output.size(); i++)
	{
		this->out_file << output[i];
	}
	sem_post(&(this->lock));
	return *this;
}
OutputFile& OutputFile::write(stringstream &output)
{
	sem_wait(&(this->lock));
	this->out_file << output.str();
	sem_post(&(this->lock));
	return *this;
}
OutputFile& OutputFile::operator<<(const char* output)
{
	return this->write(output);
}
OutputFile& OutputFile::operator<<(const string &output)
{
	return this->write(output);
}
OutputFile& OutputFile::operator<<(const vector<string> &output)
{
	return this->write(output);
}
OutputFile& OutputFile::operator<<(stringstream &output)
{
	return this->write(output);
}

// --------------------------------------------------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------    PRIVATE   ----------------------------------- ||
// --------------------------              ----------------------------------- ||
// --------------------------------------------------------------------------- ||

void OutputFile::setUp()
{
	sem_init(&(this->lock),0,1);
}
void OutputFile::setUp(const string &file_name)
{
	this->setUp();

	this->out_file.open(file_name.c_str());
	
	if (!this->out_file.is_open())
	{
		exit(1);
	}
}
void OutputFile::writeHeader(const char* header)
{
	sem_wait(&(this->lock));
	this->out_file << header;
	sem_post(&(this->lock));
}
void OutputFile::writeHeader(const string &header)
{
	sem_wait(&(this->lock));
	this->out_file << header;
	sem_post(&(this->lock));
}
void OutputFile::writeHeaders(const vector<string> &headers)
{
	sem_wait(&(this->lock));
	for (uint32_t i = 0; i < headers.size(); i++)
	{
		this->out_file << headers[i];
		if ( (headers[i].size() > 0) && (headers[i].at(headers[i].size() - 1) != '\n') )
		{
			this->out_file << endl;
		}
	}
	sem_post(&(this->lock));
}
