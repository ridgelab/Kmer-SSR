/*	
	Brandon Pickett
	main.cpp -- Main
*/

// include statements
#include <pthread.h>
#include "../include/FindSSRs.h"
#include "../include/Arguments.h"

// using statements
using namespace std;

// main
int main(int argc, char* argv[])
{
	int ret_val = 0;
	
	Arguments* args;

	try
	{
		args = new Arguments(argc, argv);

		if(!args->helpOrVersionDisplayed())
		{
			FindSSRs find_ssrs(args);
			ret_val = find_ssrs.run();
		}
	}
	catch (string error)
	{
		cerr << error << endl;
		return 1;
	}
	catch (const char* error)
	{
		cerr << error << endl;
		return 1;
	}
	
	delete args;

	return ret_val;
	//pthread_exit(NULL);
}
