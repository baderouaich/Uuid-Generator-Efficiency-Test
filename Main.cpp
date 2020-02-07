#include <iostream>
#include "uuid4.h"
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>

#define MAX_UUIDS_TEST 100000


std::string generateUUID()
{
	char* uuid = new char[UUID4_LEN];
	uuid4_init();
	uuid4_generate(uuid);
	std::string uuidStr(uuid);
	delete uuid;
	return uuidStr;
}

/*
* Tests efficiancy of the UUID generator
*/
void TestUUIDsRepetition()
{
	auto time1 = std::chrono::system_clock::now();

	///
	// Generate and store UUIDs in uuids.txt
	std::ofstream outFile("uuids.txt", std::ios_base::app);
	std::vector<std::string> vUUIDs;
	for (size_t i = 0; i < MAX_UUIDS_TEST; i++)
	{
		std::string* uuid = new std::string(generateUUID());
		vUUIDs.push_back(*uuid);
		outFile << *uuid << '\n';
		uuid->clear();
		delete uuid;
	}
	outFile.close();


	//check if there is repeated uuids
	size_t numRepeated = 0, size = vUUIDs.size();
	for (size_t i = 0; i < size; i++)
	{
		//Test the current uuid with all other uuids
		for (size_t j = i + 1; j < size; j++)
		{
			if (vUUIDs[i] == vUUIDs[j])
				numRepeated++;
		}
		//once the uuid tested with all other uuids, clear it
		//deallocate some memory
		vUUIDs[i].clear();
	}
	///

	auto time2 = std::chrono::system_clock::now();
	auto diff = time2 - time1;
	std::cout << "Test Took: "<< std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() <<
		" milliseconds \n"<< MAX_UUIDS_TEST  << " UUIDs Tested\n"
		<< "Found " << numRepeated << " Repeated UUIDs." << std::endl;

	vUUIDs.clear();
}

int main()
{
	TestUUIDsRepetition();
	return 0;
}
/*
Test Took: 194370 milliseconds
100000 UUIDs Tested
Found 0 Repeated UUIDs.
*/