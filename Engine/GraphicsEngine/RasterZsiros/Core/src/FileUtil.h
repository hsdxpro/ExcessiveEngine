//FileUtil.h By Zsíroskenyér Team 2014.01.11 22:00
#pragma once

#include <fstream>
#include <list>
#include "zsString.h"

class cFileUtil {
public:
	static bool Clear(const zsString& path);
	static void Delete(const zsString& path);

	// fileLines_out will be the resulted "include expanded file" string list
	static void ReplaceIncludeDirectives(const zsString& filePath, std::list<zsString>& fileLines_out, std::list<zsString>& includedFilesPaths_out = std::list<zsString>());

// Binary writes
	static void Write(std::ofstream& o, void* data, size_t size);

	template<class T>
	static void Write(std::ofstream& o, const T& data);

	template<>
	static void Write<const wchar_t*>(std::ofstream& o, const wchar_t* const& str);

	template<>
	static void Write<zsString>(std::ofstream& o, const zsString& str);

// Binary reads
	static void Read(std::ifstream& i, const void* data_out, uint32_t size);

	template<class T>
	static void Read(std::ifstream& i, T& out);

	template<>
	static void Read<zsString>(std::ifstream& o, zsString& str_out);

// Conditional things
	static bool isFileExits(const zsString& str);

// Get infos
	static std::list<zsString> GetLines(std::ifstream& file);
	static std::list<zsString> GetLines(const zsString& filePath);
	static int GetSize(const zsString& path);
	static uint64_t GetLastWriteTime(const zsString& path);
};

template<class T>
static void cFileUtil::Write(std::ofstream& o, const T& data) {
	o.write((const char*)&data, sizeof(T));
}

template<>
static void cFileUtil::Write<const wchar_t*>(std::ofstream& o, const wchar_t* const& str) {
	uint32_t size = (wcslen(str) + 1) * sizeof(wchar_t);
	cFileUtil::Write(o, size);
	o.write((const char*)str, size);
}

template<>
static void cFileUtil::Write<zsString>(std::ofstream& o, const zsString& str) {
	uint32_t size = (str.size() + 1) * sizeof(wchar_t);
	cFileUtil::Write(o, size);
	o.write((const char*)str.c_str(), size);
}

template<class T>
static void cFileUtil::Read(std::ifstream& i, T& out) {
	i.read((char*)&out, sizeof(T));
}

template<>
static void cFileUtil::Read<zsString>(std::ifstream& is, zsString& str_out) {
	uint32_t size;
	cFileUtil::Read(is, size);
	// TODO PERFORMANCE WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	wchar_t* chars = new wchar_t[size];
	cFileUtil::Read(is, chars, size);
	str_out = chars;
	delete[] chars;
}