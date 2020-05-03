#if _MSVC_LANG >= 201703 || __cplusplus >= 201703
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
using namespace std;

#include "uuid4/uuid4.h"

#define MAX_UUIDS_TEST 100

unsigned long long m_RepeatedUUIDs = 0ULL;
void TestUUIDsRepetition()
{

}


int main()
{
	auto t1 = std::chrono::system_clock::now();
	TestUUIDsRepetition();
	auto t2 = std::chrono::system_clock::now();

	cout <<
		"Test Took: " << chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " seconds \n" << 
		MAX_UUIDS_TEST << " UUIDs Tested\n" <<
		"Found " << m_RepeatedUUIDs << " Repeated UUIDs." <<
		std::endl;

	cin.ignore();
	cin.get();
	return 0;
}

#else
#error Enable C++17 or remove this macro check
#endif

/*
Test Took: 194370 milliseconds (3.2395 minutes)
100000 UUIDs Tested
Found 0 Repeated UUIDs.
*/


/*
std::string generateUUID()
{
	char* uuid = new char[UUID4_LEN];
	uuid4_init();
	uuid4_generate(uuid);
	std::string uuidStr(uuid);
	delete uuid;
	return uuidStr;
}


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
	std::cout << "Test Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() <<
		" milliseconds \n" << MAX_UUIDS_TEST << " UUIDs Tested\n"
		<< "Found " << numRepeated << " Repeated UUIDs." << std::endl;

	vUUIDs.clear();
}
*/