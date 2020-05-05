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
#include <string>
#include <unordered_set> 
using namespace std;
#include "uuid4/uuid4.h"

#define MAX_UUIDS_TEST 10000000
#define SAVE_UUIDS_TO_File 0

unordered_set<const char*> m_RepeatedUUIDs;
vector <const char*> vecUUIDs;

void GenerateUUIDs()
{
	vecUUIDs.reserve(MAX_UUIDS_TEST);
	uuid4_init();

	for (int i = 0; i < MAX_UUIDS_TEST; i++)
	{
		char* uuid = new char[UUID4_LEN];
		uuid4_generate(uuid);
		vecUUIDs.emplace_back(uuid);
	}
}

void CheckRepititions()
{
	for (const char* uuid : vecUUIDs)
		m_RepeatedUUIDs.insert(uuid);
}

void TestUUIDsRepetition()
{
	GenerateUUIDs();
	CheckRepititions();
}

#if SAVE_UUIDS_TO_File
void SaveUUIDsToFile(const char* filename)
{
	ofstream ofs(filename, ios::app | ios::out);
	if (ofs)
	{
		for (const char* uuid : vecUUIDs)
			ofs << uuid << '\n';
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
		"Took: " << chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() <<
		" milliseconds \n" << 
		MAX_UUIDS_TEST << " UUIDs Tested\n" <<
		vecUUIDs.size() - m_RepeatedUUIDs.size() <<
		" Repeated UUIDs Found.\n" <<
		"Test Finished.\n";


#if SAVE_UUIDS_TO_File
	SaveUUIDsToFile((to_string(MAX_UUIDS_TEST) + string("-uuids.txt")).c_str());
#endif




	cout << "Press anything to clean\n";
	cin.get();
	for (const char* uuid : m_RepeatedUUIDs)
		if(uuid) delete[] uuid;
	vecUUIDs.clear();
	m_RepeatedUUIDs.clear();


	cout << "Press anything to exit\n";
	cin.ignore();
	cin.get();
	return 0;
}

/*
Latest test on 05-05-2020 11:26pm
Build Type:			Release x64
Optimization:		ON
UUIDs Generated:	10,000,000 (100 Million)
UUIDs Repeated:		0
Test Duration:		14063 milliseconds
Memory Allocated due test: Each uuid has 37 bytes, so
37 * 10,000,000 uuids = 370,000,000‬ bytes
370,000,000‬ bytes / 1024 / 1024 = 352 MB

~352MB for vecUUDs which holds generated uuids
~352MB for m_RepeatedUUIDs which holds generated uuids minus repeated ones
~300MB for uuid generator library and other stuff

Approximately the entire test takes ~1GO of memory
*/

#else
#error Enable C++17 or remove this macro check
#endif
