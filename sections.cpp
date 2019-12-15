#include "sections.h"

constexpr char CSections::SECTION_HEADER[];
constexpr char CSections::FILE_HEADER[];

using CState = CSections::CState;

#include <fstream>
#include <cstring>
#include <algorithm>
#include <cinttypes>
#include <string>
#include <sstream>

CSections::CSections()
{
}


CSections::~CSections()
{
}

CState CSections::loadFromFile(const char* Filename)
{
	std::ifstream File(Filename, std::ios_base::binary);
	if (!File)
		return CState::LOAD_ERROR_NO_FILE;
	char Header[sizeof(FILE_HEADER)];
	File.read(Header, sizeof(FILE_HEADER));
	if (!File || std::memcmp(Header, FILE_HEADER, sizeof(FILE_HEADER)))
		return CState::LOAD_ERROR_BAD_FORMAT;
	createFromStream(File);

	return CState::LOAD_OK;
}

CState CSections::saveToFile(const char * Filename) const
{
	static char ZeroBuffer[4 * sizeof(uint32_t)] = { 0 };
	std::ofstream File;
	File.open(Filename, std::ios::binary);
	if (!File)
		return CState::SAVE_ERROR;
	File.write(FILE_HEADER, sizeof(FILE_HEADER));
	writeToStream(File);
	return CState::SAVE_OK;
}

void CSections::loadFromBuffer(const std::vector<char>& Buffer)
{
	std::istringstream Stream(std::string(Buffer.begin(), Buffer.end()));
	createFromStream(Stream);
}

void CSections::saveToBuffer(std::vector<char>& Buffer) const
{
	Buffer.clear();
	std::ostringstream Stream;
	writeToStream(Stream);
	std::string StrBuffer(Stream.str());
	std::copy(StrBuffer.begin(), StrBuffer.end(), std::back_inserter(Buffer));
}

const std::vector<char>* CSections::get(const char * Section) const
{
	auto It = m_Sections.find(std::string(Section));
	if (It != m_Sections.end())
		return &It->second;
	return nullptr;
}

void CSections::put(const char * Section, const std::vector<char>& Buffer)
{
	m_Sections.emplace(std::string(Section), Buffer);
}

void CSections::remove(const char * Section)
{
	m_Sections.erase(Section);
}

void CSections::list(std::function<void(const std::string&, int)> Fn)
{
	for (auto& p : m_Sections)
		Fn(p.first, p.second.size());
}

void CSections::createFromStream(std::istream& Input)
{
	m_Sections.clear();
	char Header[sizeof(SECTION_HEADER)];
	while (Input.read(Header, sizeof(SECTION_HEADER)) && Input.gcount() == sizeof(SECTION_HEADER))
	{
		if (std::memcmp(Header, SECTION_HEADER, sizeof(SECTION_HEADER)))
			break;

		char UintBuffer[sizeof(uint32_t)];
		Input.read(UintBuffer, sizeof(uint32_t));
		unsigned int Size;
		rawRead(Size, UintBuffer);
		Input.ignore(sizeof(uint32_t) * 4);
		std::string Name;
		std::getline(Input, Name, '\0');
		auto It = m_Sections.emplace(Name, std::vector<char>(Size));
		Input.read(&It.first->second[0], Size);
	}
}

void CSections::writeToStream(std::ostream & Output) const
{
	static char ZeroBuffer[4 * sizeof(uint32_t)] = { 0 };
	for (auto &p : m_Sections)
	{
		Output.write(SECTION_HEADER, sizeof(SECTION_HEADER));
		char UintBuffer[sizeof(uint32_t)];
		rawWrite(p.second.size(), UintBuffer);
		Output.write(UintBuffer, sizeof(uint32_t));
		Output.write(ZeroBuffer, sizeof(ZeroBuffer));
		Output << p.first << '\0';
		Output.write(&p.second[0], p.second.size());
	}
}
