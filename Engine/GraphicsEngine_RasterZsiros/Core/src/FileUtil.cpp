#include "FileUtil.h"
#include "StrUtil.h"

// WINDOWS IMPLEMENTATION
#include <windows.h>

// For determining size of a file
#include <sys/stat.h>
#include "common.h"

bool cFileUtil::Clear(const zsString& path) {
	std::ofstream os(path.c_str(), std::ios::trunc);

	if (!os.is_open())
		return false;

	os.close();
	return true;
}

void cFileUtil::Delete(const zsString& path) 
{
	// TODO
	//boost::filesystem::remove(path.c_str());
}

void cFileUtil::ReplaceIncludeDirectives(const zsString& filePath, std::list<zsString>& fileLines_out, std::list<zsString>& includedFilesPaths_out/* = std::list<zsString>()*/) {
	auto fileLines = cFileUtil::GetLines(filePath);

	auto basePath = cStrUtil::GetDirectory(filePath);

	for (zsString s : fileLines) {
		// Search for "#include" in that line
		int chIdx = cStrUtil::Find(s, L"#include");

		// reach line that has "#include"
		if (chIdx >= 0) {
			// Included file
			zsString fileRelPath = cStrUtil::Between(s.c_str() + chIdx, '"', '"');

			auto it = std::find(includedFilesPaths_out.begin(), includedFilesPaths_out.end(), filePath);
			if (it == includedFilesPaths_out.end()) {
				zsString filePath = basePath + fileRelPath;
				includedFilesPaths_out.push_back(filePath);
				ReplaceIncludeDirectives(filePath, fileLines_out, includedFilesPaths_out);
			}
		}
		else {
			fileLines_out.push_back(s);
		}
	}
}

void cFileUtil::Write(std::ofstream& o, void* data, size_t size)
{
	o.write((const char*)data, size);
}

void cFileUtil::Read(std::ifstream& i, const void* dataOut, uint32_t size)
{
	i.read((char*)dataOut, size);
}

int cFileUtil::GetSize(const zsString& filePath) {
	char ansiPath[256];
	cStrUtil::ToAnsi(filePath, ansiPath, 256);

	struct stat results;
	if (stat(ansiPath, &results) == 0)
		return results.st_size;
	else
		return -1;
}

uint64_t cFileUtil::GetLastWriteTime(const zsString& filePath) 
{
	HANDLE hFile = CreateFileW(filePath.c_str(),                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_NEW,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template


	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		int asd = 5;
		asd++;
	}

	return (uint64_t)ftWrite.dwLowDateTime;
}

std::list<zsString> cFileUtil::GetLines(std::ifstream& file) {
	file.clear();
	file.seekg(0, std::ios::beg);

	std::list<zsString> lines;
	std::string str;
	while (getline(file, str))
		lines.push_back(str.c_str());
	return lines;
}

std::list<zsString> cFileUtil::GetLines(const zsString& filePath) {
	std::ifstream is(filePath.c_str());
	auto lines = cFileUtil::GetLines(is);
	is.close();
	return lines;
}

bool cFileUtil::isFileExits(const zsString& str) {
	std::wfstream is(str.c_str(), std::ios_base::in);
	bool isOpen = is.is_open();
	is.close();
	return isOpen;
}