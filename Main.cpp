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
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_set> 
#include <format> 
#include <cassert>
#include <fstream>
#include <filesystem>
#include <windows.h>
extern "C" {
#include "uuid4/uuid4.h"
}

constexpr const std::size_t MAX_UUIDS_TEST = 1'000'000'000; // 1 Billion
static std::ifstream ifs{ "uuids.txt", std::ios::in };
static std::ofstream ofs{ "uuids.txt", std::ios::out};
/*
tellp // returns the output position indicator 
seekp // sets the output position indicator 
tellg // returns the input position indicator
seekg // sets the input position indicator 
*/
void save_uuid(const std::string& uuid)
{
#if _DEBUG
	std::cout << "saving " << uuid << "...\n";
#else
	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
		std::cout << "saving " << uuid << "...\n";
#endif
	ofs.seekp(0, std::ios::end);
	ofs << uuid << '\n';
}
const bool uuid_exists(const std::string& uuid, const std::size_t i)
{
#if _DEBUG
		std::cout << "checking for existence of " << uuid << "...\n";
#else
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		std::cout << "checking for existence of " << uuid << "...\n";
#endif

	// go to begining of ifs to iterate over all saved uuids and search for match
	ifs.clear();
	ifs.seekg(0, std::ios::beg);
	while (!ifs.eof())
	{
		std::string in_uuid{};
		std::getline(ifs, in_uuid);
#if _DEBUG
		std::cout << "getline = " << in_uuid << std::endl;
#else
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		std::cout << i << '/' << MAX_UUIDS_TEST << ": getline = " << in_uuid << std::endl;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		ifs.clear();
		ifs.close();
		ofs.clear();
		ofs.close();
		std::exit(EXIT_SUCCESS);
		break;
	}
#endif
		if(!in_uuid.empty()) // can be the '\n' we add after saving uuid
			if (uuid == in_uuid)
				return true;
	}
	return false;
}

void GenerateUUIDs()
{
	for (std::size_t i = 0; i < MAX_UUIDS_TEST; i++)
	{
		std::string uuid(UUID4_LEN - 1, '\000'); // UUID4_LEN includes last \000 too
		uuid4_generate(uuid.data());

		save_uuid(uuid);

		if (uuid_exists(uuid, i))
		{
			std::cout << "found one repeated uuid = "<< uuid << " after " << i << " generated uuids!\n";
			break;
		}
	}
}

int main()
{
	if(!std::filesystem::exists("uuids.txt"))
		return EXIT_FAILURE;
	if (uuid4_init() != UUID4_ESUCCESS)
		return EXIT_FAILURE;

	std::cout << "Test Running...\n";
	auto t1 = std::chrono::steady_clock::now();
	GenerateUUIDs();
	auto t2 = std::chrono::steady_clock::now();

	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(t2 - t1).count();
	std::cout << std::format("Test Took {} minutes.\n", minutes);

	
	ifs.close();
	ofs.close();

	return EXIT_SUCCESS;
}


#if 0
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_set> 
#include <format> 
#include <cassert>
extern "C" {
	#include "uuid4/uuid4.h"
}

constexpr const std::size_t MAX_UUIDS_TEST = 10'000'000;
std::unordered_set<std::string> UUIDs;

void GenerateUUIDs()
{
	UUIDs.reserve(MAX_UUIDS_TEST);
	for (std::size_t i = 0; i < MAX_UUIDS_TEST; i++)
	{
		//char* uuid = new char[UUID4_LEN];
		//std::array<char, UUID4_LEN> uuid{0};
		std::string uuid(UUID4_LEN, '\000');
		uuid4_generate(uuid.data());
		//std::cout << uuid << std::endl;
		UUIDs.insert(std::move(uuid));
	}
}

int main()
{
	if (uuid4_init() != UUID4_ESUCCESS)
		return EXIT_FAILURE;

	std::cout << "Test Running...\n";
	auto t1 = std::chrono::steady_clock::now();
	GenerateUUIDs();
	auto t2 = std::chrono::steady_clock::now();

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	auto num_repititions = MAX_UUIDS_TEST - UUIDs.size();

	std::cout << std::format("Took {} ms\n{} UUIDs Tested\n{} Repititions found.\n", ms, MAX_UUIDS_TEST, num_repititions);

	std::cout << "Press anything to clean\n";
	std::cin.get();
	//for (const char* uuid : UUIDs)
	//		delete[] uuid;
	UUIDs.clear();


	std::cout << "Press anything to exit\n";
	std::cin.ignore();
	std::cin.get();
	return EXIT_SUCCESS;
}
#endif




#if 0
#if _MSVC_LANG >= 201703 || __cplusplus >= 201703
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_set> 
#include "uuid4/uuid4.h"
using namespace std;

constexpr const std::size_t MAX_UUIDS_TEST = 1'000'000;
constexpr const bool SAVE_UUIDS_TO_File = false;

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
		vecUUIDs.emplace_back(std::move(uuid));
	}
}

void CheckRepititions()
{
	for (const char*& uuid : vecUUIDs)
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
#endif