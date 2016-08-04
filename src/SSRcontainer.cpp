/*
	Brandon Pickett
	SSRcontainer.cpp
*/

#include "../include/SSRcontainer.h"

SSRcontainer::SSRcontainer()
{
	this->ssrs = new unordered_map<string, vector<SSR*>*>();
}
SSRcontainer::~SSRcontainer()
{
	this->clear();
	delete this->ssrs;
}
uint32_t SSRcontainer::size() const
{
	return this->ssrs->size();
}
bool SSRcontainer::empty() const
{
	return this->ssrs->empty();
}
void SSRcontainer::add(const string &identifier, SSR* ssr)
{
	if (this->ssrs->count(identifier)) // if the key (and thus the associated vector) already exists in the map
	{
		this->ssrs->at(identifier)->push_back(ssr);
	}
	else // if it doesn't yet exist in the map
	{
		vector<SSR*>* temp_ptr = new vector<SSR*>( { ssr } ); // create the vector
		std::pair<string, vector<SSR*>*> temp_pair(identifier, temp_ptr); // create the pair
		this->ssrs->insert(temp_pair); // add the pair to the map
	}
}
void SSRcontainer::add(const string &identifier, vector<SSR*>* ssrs_vec)
{
	if (ssrs_vec != nullptr)
	{
		if (this->ssrs->count(identifier)) // if the key (and thus the associated vector) already exists in the map
		{
			vector<SSR*>* temp_ptr = this->ssrs->at(identifier); // get the vector

			// add each SSR* in ssrs_vec into the vector pointed to by the identifier in the map (this->ssrs)
			for (uint32_t i = 0; i < ssrs_vec->size(); ++i)
			{
				temp_ptr->push_back(ssrs_vec->at(i));
			}
		}
		else // if it doesn't yet exist in the map
		{
			std::pair<string, vector<SSR*>*> temp_pair(identifier, ssrs_vec);
			this->ssrs->insert(temp_pair);
		}
	}
}
void SSRcontainer::clear()
{
	for (unordered_map<string, vector<SSR*>*>::iterator itr = this->ssrs->begin(); itr != this->ssrs->end(); ++itr)
	{
		//delete itr->first;
		
		for (uint32_t i = 0; i < itr->second->size(); ++i)
		{
			delete itr->second->at(i);
		}

		itr->second->clear();

		delete itr->second;
	}

	this->ssrs->clear();
}
void SSRcontainer::writeToFile(ofstream &ofd) const
{
	for (unordered_map<string, vector<SSR*>*>::iterator itr = this->ssrs->begin(); itr != this->ssrs->end(); ++itr)
	{
		for (uint32_t i = 0; i < itr->second->size(); ++i)
		{
			ofd << itr->first << '\t';
			itr->second->at(i)->writeToFile(ofd);
		}
	}
}
void SSRcontainer::writeToFile(OutputFile &ofd, bool block) const
{
	if (ofd.obtainLock(block))
	{
		for (unordered_map<string, vector<SSR*>*>::iterator itr = this->ssrs->begin(); itr != this->ssrs->end(); ++itr)
		{
			for (uint32_t i = 0; i < itr->second->size(); ++i)
			{
				ofd << itr->first << '\t';
				itr->second->at(i)->writeToFile(ofd);
			}
		}

		ofd.unlock();
	}
}
void SSRcontainer::writeToFile(OutputFile* ofd, bool block) const
{
	if (ofd->obtainLock(block))
	{
		for (unordered_map<string, vector<SSR*>*>::iterator itr = this->ssrs->begin(); itr != this->ssrs->end(); ++itr)
		{
			for (uint32_t i = 0; i < itr->second->size(); ++i)
			{
				*ofd << itr->first << '\t';
				itr->second->at(i)->writeToFile(*ofd);
			}
		}

		ofd->unlock();
	}
}
string SSRcontainer::toString() const
{
	string output = "SSRcontainer: { ";
	
	for (unordered_map<string, vector<SSR*>*>::iterator itr = this->ssrs->begin(); itr != this->ssrs->end(); ++itr)
	{
		if (itr != this->ssrs->begin())
		{
			output += ", ";
		}
		
		output += itr->first + ": { ";

		for (uint32_t i = 0; i < itr->second->size(); ++i)
		{
			if (i != 0)
			{
				output += ", ";
			}
			
			output += itr->second->at(i)->toString();
		}

		output += " }";
	}

	output += " }";

	return output;
}
string SSRcontainer::toJson() const
{
	string json = "SSRcontainer: {";

	if (this->size() != 0)
	{
		json += "\n\t";
		
		for (unordered_map<string, vector<SSR*>*>::iterator itr = this->ssrs->begin(); itr != this->ssrs->end(); ++itr)
		{
			if (itr != this->ssrs->begin())
			{
				json += ",\n\t";
			}
			
			json += itr->first + ": {";

			if (itr->second->size() != 0)
			{
				json += "\n\t\t";
				
				for (uint32_t i = 0; i < itr->second->size(); ++i)
				{
					if (i != 0)
					{
						json += ",\n\t\t";
					}
					
					json += itr->second->at(i)->toString();
				}
		
				json += "\n\t";
			}
			else
			{
				json += ' ';
			}

			json += "}";
		}

		json += '\n';
	}
	else
	{
		json += ' ';
	}

	json += '}';

	return json;
}
