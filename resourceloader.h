#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class CResourceLoader
{
public:
	enum class State
	{
		LOAD_OK,
		LOAD_ERROR_NO_FILE,
		LOAD_ERROR_BAD_FORMAT,

		SAVE_OK,
		SAVE_ERROR,
	};
	std::unordered_map<std::string, std::vector<char>> p;
public:
	State load(const std::string& Filename);
	bool readBytes(const std::string& Field, std::vector<char>& Bytes);
	bool read(const std::string& Field);

};