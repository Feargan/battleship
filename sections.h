#pragma once

#include <unordered_map>
#include <vector>
#include <istream>
#include <ostream>
#include <optional>

class CSections
{
public:
	enum class CState
	{
		LOAD_OK,
		LOAD_ERROR_NO_FILE,
		LOAD_ERROR_BAD_FORMAT,

		SAVE_OK,
		SAVE_ERROR,
	};
private:
	static constexpr char SECTION_HEADER[] = { 0x2, 0x1, 0x0 };
	static constexpr char FILE_HEADER[] = { 0x0, 0x0, 0x41, 0x52, 0x43, 0x48, 0x47, 0x0 };
	std::unordered_map<std::string, std::vector<char>> m_Sections;
public:
	CSections();
	~CSections();

	CState loadFromFile(const char* Filename);
	CState saveToFile(const char* Filename) const;
	void loadFromBuffer(const std::vector<char>& Buffer);
	void saveToBuffer(std::vector<char>& Buffer) const;

	const std::vector<char>* get(const char* Section) const;
	template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr> std::optional<T>  getInt(const char* Section) const
	{
		auto Vec = get(Section);
		if (!Vec || Vec->size() < sizeof(T))
			return {};
		T Value;
		readRaw(Value, &(*Vec)[0]);
		return Value;
	}
	template<typename T, typename std::enable_if<!std::is_unsigned<T>::value>::type* = nullptr> std::optional<T> getInt(const char* Section) const
	{
		auto Vec = get(Section);
		if (!Vec || Vec->size() < sizeof(T))
			return {};
		typename std::make_unsigned<T>::type Value;
		rawRead(Value, &(*Vec)[0]);
		return static_cast<T>(Value&(~(1 << (sizeof(T)*8-1)))) * ((Value>>(sizeof(T)*8-1))&1 ? -1 : 1);
	}

	void put(const char* Section, const std::vector<char>& Buffer);
	template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr> void putInt(const char* Section, T Value)
	{
		std::vector<char> Buffer(sizeof(T));
		rawWrite(Value, &Buffer[0]);
		put(Section, Buffer);

	}
	template<typename T, typename std::enable_if<!std::is_unsigned<T>::value>::type* = nullptr> void putInt(const char* Section, T Value)
	{
		std::vector<char> Buffer(sizeof(T));
		typename std::make_unsigned<T>::type Uvalue;
		int SignBit = Value >= 0 ? 0 : 1;
		Uvalue = std::abs(Value);
		Uvalue |= (SignBit << ((sizeof(T) * 8) - 1));
		rawWrite(Uvalue, &Buffer[0]);
		put(Section, Buffer);
	}

	template<typename T> static void rawRead(T& Output, const char* Buffer)
	{
		Output = 0;
		for (unsigned int i = 0; i < sizeof(T); i++)
		{
			Output <<= 8;
			Output |= Buffer[i] & 0xff;// reinterpret_cast<const unsigned char*>(Buffer)[i];
		}
	}
	template<typename T> static void rawWrite(T Value, char* Buffer)
	{
		for (unsigned int i = 0; i < sizeof(T); i++)
		{
			//Buffer[sizeof(T) - i - 1] = static_cast<char>(Value & 0xff);
			Buffer[sizeof(T) - i - 1] = static_cast<char>(Value & 0xff);
			Value >>= 8;
		}
	}
private:
	void createFromStream(std::istream& Input);
	void writeToStream(std::ostream& Output) const;
};
