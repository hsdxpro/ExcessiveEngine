// Windows implementation of File
#pragma once
#include "SupportLibrary\BasicTypes.h"
#include <string>
#include <fstream>
#include <assert.h>
#include <sys/stat.h>

enum eIO
{
	OUT_BIN,
	OUT_BIN_TRUNC,
	IN_BIN,
};

class File
{
public:
	File(const std::string& filePath, eIO flag);

	_inline void Close() { fileStream.close(); }

	template<class T>
	void Write(T* ptr, size_t byteSize)
	{
		assert(fileStream.is_open());
		fileStream.write((char*)ptr, byteSize);
	}

	template<class T>
	void operator << (const T& data)
	{
		assert(fileStream.is_open());
		fileStream.write((char*)&data, sizeof(T));
	}

	template<>
	void operator << <std::string>(const std::string& data)
	{
		assert(fileStream.is_open());
		size_t strLenWithTerminator = data.length() + 1;
		*this << strLenWithTerminator;
		fileStream.write((char*)data.c_str(), strLenWithTerminator);
	}

	template<>
	void operator << <std::wstring>(const std::wstring& data)
	{
		assert(fileStream.is_open());
		size_t strLenWithTerminator = data.length() + 1;
		*this << strLenWithTerminator;
		fileStream.write((char*)data.c_str(), strLenWithTerminator * sizeof(wchar_t));
	}

	template<class T>
	T Read()
	{
		assert(fileStream.is_open());

		T data_out;
		fileStream.read((char*)&data_out, sizeof(T));
		return data_out;
	}

	template<class T>
	void Read(T* ptr, size_t byteSize)
	{
		assert(fileStream.is_open());
		fileStream.read((char*)ptr, byteSize);
	}

	template<class T>
	void operator >> (T& data_out)
	{
		assert(fileStream.is_open());
		fileStream.read((char*)&data_out, sizeof(T));
	}

	template<>
	void operator >> <std::string>(std::string& data_out)
	{
		assert(fileStream.is_open());
		size_t strLenWithTerminator;
		fileStream.read((char*)&strLenWithTerminator, sizeof(size_t));

		data_out.resize(strLenWithTerminator - 1);
		fileStream.read((char*)data_out.c_str(), strLenWithTerminator);
	}

	template<>
	void operator >> <std::wstring>(std::wstring& data_out)
	{
		assert(fileStream.is_open());
		size_t strLenWithTerminator;
		fileStream.read((char*)&strLenWithTerminator, sizeof(size_t));

		data_out.resize(strLenWithTerminator - 1);
		fileStream.read((char*)data_out.c_str(), strLenWithTerminator * sizeof(wchar_t));
	}

	 static i64 GetSize(const std::string& filePath)
	{
		struct stat results;
		if (stat(filePath.c_str(), &results) == 0)
			return (i64)results.st_size;
		else
			return -1;
	}

	 static bool IsExists(const std::string& filePath)
	{
		std::fstream is(filePath.c_str(), std::ios_base::in);
		bool isOpen = is.is_open();
		is.close();
		return isOpen;
	}


	template<class T>
	friend void operator << (T& data, File& file)
	{
		file >> data;
	}

protected:
	std::fstream fileStream;
};