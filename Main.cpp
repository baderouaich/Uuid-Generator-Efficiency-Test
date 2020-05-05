/*
For Efficency:
https://docs.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support?view=vs-2019
#pragma omp parallel
creates a parallel region, and each individual thread executes what is in your loop.
In other words, you do the complete loop N times,
instead of N threads splitting up the loop and completing all iterations just once.

#pragma omp parallel for
 create one parallel region (aka one fork/join, which
 is expensive and therefore you don't want to do it for every loop)
 and run multiple loops in parallel within that region. Just make sure
 if you already have a parallel region you use #pragma omp for
 as opposed to #pragma omp parrallel for as the latter will mean that each of
 your N threads spawns N more threads to do the loop.

 To set this compiler option in the Visual Studio development environment

	Open the project's Property Pages dialog box. For details, see Set C++ compiler and build properties in Visual Studio.

	Expand the Configuration Properties > C/C++ > Language property page.

	Modify the OpenMP Support property.

*/


#if _MSVC_LANG >= 201703 || __cplusplus >= 201703
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <mutex>
#include <string>
#include <algorithm> // std::binary_search
#include <omp.h> //#pragma omp parallel for reduction(+:m_RepeatedUUIDs) private(i) private(j)
using namespace std;
#include "uuid4/uuid4.h"

mutex guard;

#define LOCK guard.lock();
#define UNLOCK guard.unlock();
//optimize
#define RUN_IN_PARALLEL 0
#define MAX_UUIDS_TEST 1000000
#define SAVE_UUIDS_TO_File
#if RUN_IN_PARALLEL
#define LOCK guard.lock();
#define UNLOCK guard.unlock();
#else
#define LOCK 
#define UNLOCK 
#endif
unsigned int m_RepeatedUUIDs = 0ULL;
vector <const char*> vecUUIDs;

void GenerateUUIDs()
{
	vecUUIDs.reserve(MAX_UUIDS_TEST);
	uuid4_init();

#if RUN_IN_PARALLEL
#pragma omp parallel for
#endif
	for (int i = 0; i < MAX_UUIDS_TEST; i++)
	{
		LOCK
		char* uuid = new char[UUID4_LEN];
		uuid4_generate(uuid);
		vecUUIDs.emplace_back(uuid);
		UNLOCK
	}
}

void CheckRepititions()
{
#if RUN_IN_PARALLEL
	#pragma omp parallel for
#endif	
	for (int i = 0; i < MAX_UUIDS_TEST; i++)
#if RUN_IN_PARALLEL
	#pragma omp parallel for
#endif
		for (int j = i + 1; j < MAX_UUIDS_TEST; j++)
		{
			LOCK
				m_RepeatedUUIDs += vecUUIDs[i] == vecUUIDs[j];
			UNLOCK
		}
}

void TestUUIDsRepetition()
{
	GenerateUUIDs();
	CheckRepititions();
}

#ifdef SAVE_UUIDS_TO_File
void SaveUUIDsToFile(const char* filename)
{
	ofstream ofs(filename, ios::app | ios::out);
	if (ofs)
	{
#if RUN_IN_PARALLEL
	#pragma omp parallel for
#endif		
		for (const char* uuid : vecUUIDs)
		{
			LOCK
			ofs << uuid << '\n';
			delete[] uuid;
			UNLOCK
		}
		ofs.close();
	}
}
#endif

int main()
{
	cout << "Test Running...\n";
	auto t1 = std::chrono::system_clock::now();
	TestUUIDsRepetition();
	auto t2 = std::chrono::system_clock::now();

	cout <<
		"Took: " << chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " seconds \n" << 
		MAX_UUIDS_TEST << " UUIDs Tested\n" <<
		m_RepeatedUUIDs <<" Repeated UUIDs Found.\n" <<
		"Test Finished.\n";


#ifdef SAVE_UUIDS_TO_File
	SaveUUIDsToFile((to_string(MAX_UUIDS_TEST) + string("-uuids.txt")).c_str());
#endif


#ifndef SAVE_UUIDS_TO_File
	cout << "Press anything to clean\n";
	cin.get();
	for (const char* uuid : vecUUIDs)
		delete[] uuid;
#endif


	cout << "Press anything to exit\n";
	cin.ignore();
	cin.get();
	return 0;
}
/*
Tested in Releasex64: Parallel optimization OFF
Took: 14 seconds
100000 UUIDs Tested
0 Repeated UUIDs Found.
===============================
Tested in Releasex64: Parallel optimization ON
Took: 14 seconds
100000 UUIDs Tested
0 Repeated UUIDs Found.
===============================
Tested in Releasex64: Parallel optimization ON
Took: 1728 seconds (28.8‬ minutes)
1000000 UUIDs Tested
0 Repeated UUIDs Found.
Test Finished.
==========================
Took: 1443 seconds (24mins)
1000000 UUIDs Tested
0 Repeated UUIDs Found.
*/

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