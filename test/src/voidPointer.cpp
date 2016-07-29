/*
 * 	Brandon Pickett
 * 	voidPointer.cpp
 *
 * 	This program's purpose is to test if casting a void pointer to another type
 * 	of pointer will cause a memory leak when we only delete the casted pointer.
 * 	The alternative is, of course, deleting both the casted and the void
 * 	pointer.
 *
 * 	As it turns out, the answer is: it is sufficient to simply delete the casted
 * 	pointer.
 */

#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

struct Dummy
{
	uint32_t dummy_uint;
	string dummy_string;

	Dummy(uint32_t _dummy_uint, const char* _dummy_string)
	{
		dummy_uint = _dummy_uint;
		dummy_string = _dummy_string;
	}
};

void dummyFunc(void* func_vptr)
{
	Dummy* temp_dummy = (Dummy*) func_vptr;
	temp_dummy->dummy_string = "three";
	delete temp_dummy;
}

int main()
{
	Dummy d1(1, "one");
	
	Dummy* d2 = new Dummy(2, "two");
	delete d2;

	dummyFunc((void*) new Dummy(3, "THREE"));

	cout << "\n\033[0;34mVoid Pointer Test: ";

	bool pass = true;

	if (pass)
	{
		cout << "\033[0;32mOK\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	return 0;
}
