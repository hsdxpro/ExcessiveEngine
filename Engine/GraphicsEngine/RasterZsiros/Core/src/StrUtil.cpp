#include "StrUtil.h"

#include <cassert>

void cStrUtil::ToAnsi(const zsString& str, char* dst, size_t nChars) {
	wcstombs(dst, str.c_str(), nChars);
}

size_t cStrUtil::ToUnsigned(const zsString& str) {
	std::wstringstream ss;
	ss << str.c_str();
	size_t res;
	ss >> res;
	return res;
}

void cStrUtil::ToUpper(std::list<zsString>& strs) {
	size_t strIdx = 0;
	for (auto it = strs.begin(); it != strs.end(); it++) {
		strIdx = 0;
		while ((*it)[strIdx] != '\0') {
			(*it)[strIdx] = toupper((*it)[strIdx]);
			strIdx++;
		}
	}
}

bool cStrUtil::Contains(const zsString& in, const zsString& that) {
	if (in.find(that.c_str()) != std::wstring::npos)
		return true;
	else
		return false;
}

bool cStrUtil::Contains(const std::list<zsString>& fileLines, const zsString& that) {
	auto iter = fileLines.begin();
	while (iter != fileLines.end()) {
		if (iter->find(that.c_str()) != std::wstring::npos)
			return true;
		iter++;
	}
	return false;
}

// returns - 1 : string not found
// return >= 0 : index of the founded str in zsString
int cStrUtil::Find(const zsString& in, const zsString& that) {
	return in.find(that.c_str());
}

// returns - 1 : string not found
// return >= 0 : index of the founded str in zsString
int cStrUtil::Find(const zsString& in, wchar_t that) {
	wchar_t const* s = in.c_str();
	size_t idx = 0;
	while (s[idx] != '\0')
		if (s[idx++] == that)
			return --idx;

	return -1;
}

int cStrUtil::Find(const char* in, char that, char rightBound) {
	char const* s = in;
	size_t idx = 0;
	while (s[idx] != '\0' && s[idx] != rightBound)
		if (s[idx++] == that)
			return --idx;

	return -1;
}

int cStrUtil::Find(const char* in, const char* that, char rightBound) {
	size_t mainIndex = 0;
	char const* self = in;
	char ch = '\0';
	while ((ch = self[mainIndex]) != '\0' && self[mainIndex] != rightBound) {
		if (that[0] == ch) {
			bool equal = true;
			size_t secIndex = 0;
			size_t fIndex = mainIndex;
			while (that[secIndex] != '\0') {
				ch = self[fIndex];
				if (that[secIndex] != ch) {
					equal = false;
					break;
				}
				fIndex++;
				secIndex++;
			}
			if (equal)
				return mainIndex;

		}
		mainIndex++;
	}
	return -1;
}

int cStrUtil::FindLeft(const zsString& in, int startPos, wchar_t that) {
	assert(startPos < (int)in.size() && startPos >= 0);

	wchar_t const* s = in.c_str();

	while (s[startPos] != that && startPos >= 0)
		startPos--;

	return startPos; // this will be the result, if not found it will be -1, nor 1
}

int cStrUtil::FindLeft(const char* in, int startPos, char that, char leftBound) {
	assert(startPos >= 0);

	char const* s = in;

	while (s[startPos] != that && startPos >= 0)
		startPos--;

	return startPos; // this will be the result, if not found it will be -1, nor 1
}

bool cStrUtil::Begins(const zsString& str, const zsString& withThat) {
	bool match = true;

	wchar_t const* tmp1 = str.c_str();
	wchar_t const* tmp2 = withThat.c_str();

	if (*tmp1 == '\0')
		match = false;

	while (*tmp2 != '\0')
	{
		if (*tmp1 != *tmp2)
		{
			match = false;
			break;
		}

		tmp1++;
		tmp2++;
	}

	return match;
}

void cStrUtil::CutNumberFromEnd(char* src) {
	// Move to end
	while (*src != '\0')
		src++;

	// terminate in numbers (cutting)
	while (isdigit(*--src))
		*src = '\0';
}

void cStrUtil::TrimBorder(std::list<zsString>& strs, const wchar_t* borderChars, size_t nChars) {
	
	size_t leftIdx;
	size_t rightIdx;
	size_t i = 0;

	// For each string
	for (auto it = strs.begin(); it != strs.end(); it++) {
		const wchar_t* str = it->c_str();
		leftIdx = 0;
		i = 0;

		// Trimming left, while found trimmable char
		while (i != nChars) {
			// match trimmable chars
			for (i = 0; i < nChars; i++)
				if (str[leftIdx] == borderChars[i])
				{
					leftIdx++; // Trimming occurs
					break;
				}
		}

		// End of string
		rightIdx = wcslen(str) - 1;

		// Trimming rght, while found trimmable char
		i = 0;
		while (i != nChars) {
			// match trimmable chars
			i = 0;
			for (; i < nChars; i++)
			if (str[rightIdx] == borderChars[i])
			{
				rightIdx--; // Trimming occurs
				break;
			}
		}
		*it = (*it).substr(leftIdx, rightIdx - leftIdx + 1);
	}
}

void cStrUtil::TrimBorder(zsString& strOut, wchar_t borderChar) {
	const wchar_t* str = strOut.c_str();

	// Trim from left
	size_t leftIdx = 0;
	while (str[leftIdx] == borderChar && leftIdx++);

	// Prepare for right trimming, (End of string)
	size_t rightIdx = wcslen(str) - 1;

	// Trim from right
	while (str[rightIdx] == borderChar && rightIdx--);

	strOut = strOut.substr(leftIdx, rightIdx - leftIdx + 1);
}

char const* cStrUtil::NextLine(char const* p) {
	while (*p != '\n' && *p != '\0')
		p++;
	(*p != '\0') ? p++ : p;
	return p;
}

// Gather string between left and right characters, for ex. zsString ex = _asdasd; ex.Between('-',';') returns asdasd   
void cStrUtil::Between(zsString& strOut, wchar_t left, wchar_t right) {
	wchar_t const* str = strOut.c_str();
	size_t leftIdx = 0;

	// Reach left bound
	while (str[leftIdx] != left || str[leftIdx] == '\0')
		leftIdx++;

	// Reach right bound
	size_t rightIdx = leftIdx + 1;
	while (str[rightIdx] != right || str[rightIdx] == '\0')
		rightIdx++;

	strOut = strOut.substr(leftIdx + 1, (rightIdx - 1) - leftIdx);
}

// Gather string between left and right strings, for ex. zsString ex = _asdasd; ex.Between('-',';') returns asdasd   
void cStrUtil::Between(zsString& strOut, const wchar_t* left, const wchar_t* right) {
	wchar_t const* str = strOut.c_str();

	size_t leftIdx = 0;
	size_t helperIdx = 0;
	size_t startIdx;

	// Reach index of var:left str in c_str()
	while (str[leftIdx] != '\0') {
		// Save left idx for BackUps
		startIdx = leftIdx;

		// Search var:left string in c_str() from current char
		helperIdx = 0;
		while (left[helperIdx] != '\0') {

			// left string part not equal with current c_str() char
			if (left[helperIdx] != str[leftIdx])
				break;

			helperIdx++;
			leftIdx++;
		}

		// Yeah found left string, terminate loop
		if (left[helperIdx] == '\0') {
			break;
		}

		// Backup left idx
		leftIdx = startIdx;

		leftIdx++;
	}



	// Reach index of var:left str in c_str()
	size_t rightIdx = leftIdx + 1;
	while (str[rightIdx] != '\0') {
		// Save left idx for BackUps
		startIdx = rightIdx;

		// Search var:left string in c_str() from current char
		helperIdx = 0;
		while (right[helperIdx] != '\0') {

			// left string part not equal with current c_str() char
			if (right[helperIdx] != str[rightIdx])
				break;

			helperIdx++;
			rightIdx++;
		}

		// Backup rightidx
		rightIdx = startIdx;

		// Yeah found var:right string, terminate loop
		if (right[helperIdx] == '\0') {
			break;
		}

		rightIdx++;
	}

	// ex   azt_bazt    (left:azt, right:bazt)
	//leftIdx = index of 't'
	// rightIDx = index of 'b'
	strOut = strOut.substr(leftIdx, rightIdx - leftIdx);
}

void cStrUtil::Between(zsString& strOut, wchar_t left, const wchar_t* rightDelims, size_t nRightDelims) {
	wchar_t const* str = strOut.c_str();
	size_t leftIdx = 0;

	// Reach left bound
	while (str[leftIdx] != left || str[leftIdx] == '\0')
		leftIdx++;

	// Reach right bound
	size_t rightIdx = leftIdx + 1;
	size_t i = 0;
	while (str[rightIdx] != '\0')
	{
		for (i = 0; i < nRightDelims; i++)
		if (str[rightIdx] == rightDelims[i])
			break;

		if (i != nRightDelims)
			break;

		rightIdx++;
	}

	strOut = strOut.substr(leftIdx + 1, (rightIdx - 1) - leftIdx);
}

zsString cStrUtil::SubStrLeft(const zsString& str, size_t startPos, wchar_t leftBound, size_t leftCutOffset /*= 0*/) {
	wchar_t const* strP = str.c_str() + startPos;
	size_t idxLeft = startPos;
	while (*strP != leftBound)
	{
		strP--;
		idxLeft--;
	}

	return str.substr(idxLeft + leftCutOffset, (startPos + 1) - idxLeft - leftCutOffset);
}

zsString cStrUtil::SubStrLeft(const char* str, size_t startPos, char leftBound, size_t leftCutOffset /*= 0*/) {
	char const* s = str + startPos;
	size_t idxLeft = startPos;
	while (*s != leftBound)
	{
		s--;
		idxLeft--;
	}

	// TODO CSINALD MEG MERT LASSSÚÚ
	size_t newSize = (startPos + 1) - idxLeft - leftCutOffset;
	char tmp[512];
	memcpy(tmp, str + idxLeft + leftCutOffset, newSize);
	tmp[newSize] = '\0'; // null terminate bullshit
	return tmp;
}

zsString cStrUtil::SubStrLeft(const zsString& str, size_t pos) {
	return str.substr(0, pos);
}


zsString cStrUtil::SubStrRight(const zsString& str, size_t pos, wchar_t rightBound, size_t rightCutOffset/* = 0*/) {
	wchar_t const* strP = str.c_str() + pos;
	size_t idxRight = pos;
	while (*strP != rightBound)
	{
		strP++;
		idxRight++;
	}

	return str.substr(pos, (idxRight + 1) - pos + rightCutOffset);
}


zsString cStrUtil::SubStrRight(const zsString& str, size_t pos) {
	size_t rightLength = 1;
	const wchar_t* s = str.c_str();
	while (*s++ != '\0' && rightLength++);
	return str.substr(pos, rightLength);
}

std::list<zsString> cStrUtil::SplitAt(const zsString& str, wchar_t ch) {
	std::list<zsString> result;
	int idx = cStrUtil::Find(str, ch);
	if (idx >= 0) {
		result.push_back(cStrUtil::SubStrLeft(str, idx - 1)); // left part
		result.push_back(cStrUtil::SubStrRight(str, idx + 1));
	}
	return result;
}

zsString cStrUtil::Between(const zsString& s, wchar_t left, const wchar_t* rightDelims, size_t nRightDelims) {
	wchar_t const* str = s.c_str();
	size_t leftIdx = 0;

	// Reach left bound
	while (str[leftIdx] != left || str[leftIdx] == '\0')
		leftIdx++;

	// Reach right bound
	size_t rightIdx = leftIdx + 1;
	size_t i = 0;
	while (str[rightIdx] != '\0')
	{
		for (i = 0; i < nRightDelims; i++)
		if (str[rightIdx] == rightDelims[i])
			break;

		if (i != nRightDelims)
			break;

		rightIdx++;
	}

	return s.substr(leftIdx + 1, (rightIdx - 1) - leftIdx);
}


std::list<size_t> cStrUtil::GetLines(const std::list<zsString>& strs, size_t startLineIdx, const zsString& containStr) {
	// Start strs iterating from startLineIdx
	size_t idx = 0;
	auto it = strs.begin();
	while (idx != startLineIdx) {
		idx++;
		it++;
	}

	// output
	std::list<size_t> result;
	while (it != strs.end()) {
		if (!cStrUtil::Contains(*it, containStr))
			result.push_back(idx);
		else
			break;

		it++;
		idx++;
	}

	return result;
}

// Gather string between left and right characters, for ex. zsString ex = _asdasd; ex.Between('-',';') returns asdasd   
zsString cStrUtil::Between(const zsString& s, wchar_t left, wchar_t right) {
	wchar_t const* str = s.c_str();
	size_t leftIdx = 0;

	// Reach left bound
	while (str[leftIdx] != left && str[leftIdx] != '\0')
		leftIdx++;

	// Reach right bound
	size_t rightIdx = leftIdx + 1;
	while (str[rightIdx] != right && str[rightIdx] != '\0')
		rightIdx++;

	return s.substr(leftIdx + 1, (rightIdx - 1) - leftIdx);
}

zsString cStrUtil::Between(const char* p, char left, char right, char rightBound) {
	char const* str = p;
	size_t leftIdx = 0;

	// Reach left bound
	while (str[leftIdx] != left && str[leftIdx] != '\0' && str[leftIdx] != rightBound)
		leftIdx++;

	// Reach right bound
	size_t rightIdx = leftIdx + 1;
	while (str[rightIdx] != right && str[rightIdx] != '\0' && str[rightIdx] != rightBound)
		rightIdx++;

	// TODO REPAIIIIR THAT SHIT PLLLLLSSSS
	char tmp[512];
	size_t newSize = (rightIdx - 1) - leftIdx;
	memcpy(tmp, p + leftIdx, newSize);
	tmp[newSize] = '\0'; // Null terminate
	return tmp;
}

// Gather string between left and right strings, for ex. zsString ex = _asdasd; ex.Between('-',';') returns asdasd   
zsString cStrUtil::Between(const zsString& s, const wchar_t* left, const wchar_t* right) {
	wchar_t const* str = s.c_str();

	size_t leftIdx = 0;
	size_t helperIdx = 0;
	size_t startIdx;

	// Reach index of var:left str in c_str()
	while (str[leftIdx] != '\0') {
		// Save left idx for BackUps
		startIdx = leftIdx;

		// Search var:left string in c_str() from current char
		helperIdx = 0;
		while (left[helperIdx] != '\0') {

			// left string part not equal with current c_str() char
			if (left[helperIdx] != str[leftIdx])
				break;

			helperIdx++;
			leftIdx++;
		}

		// Yeah found left string, terminate loop
		if (left[helperIdx] == '\0') {
			break;
		}

		// Backup left idx
		leftIdx = startIdx;

		leftIdx++;
	}



	// Reach index of var:left str in c_str()
	size_t rightIdx = leftIdx + 1;
	while (str[rightIdx] != '\0') {
		// Save left idx for BackUps
		startIdx = rightIdx;

		// Search var:left string in c_str() from current char
		helperIdx = 0;
		while (right[helperIdx] != '\0') {

			// left string part not equal with current c_str() char
			if (right[helperIdx] != str[rightIdx])
				break;

			helperIdx++;
			rightIdx++;
		}

		// Backup rightidx
		rightIdx = startIdx;

		// Yeah found var:right string, terminate loop
		if (right[helperIdx] == '\0') {
			break;
		}

		rightIdx++;
	}

	// ex   azt_bazt    (left:azt, right:bazt)
	//leftIdx = index of 't'
	// rightIDx = index of 'b'
	return  s.substr(leftIdx, rightIdx - leftIdx);
}

zsString cStrUtil::CutBack(const zsString& str, wchar_t ch) {
	assert(str.size() != 0);
	return str.substr(0, str.find_last_of(ch));
}

void cStrUtil::CutBack(zsString& str, wchar_t ch) {
	assert(str.size() != 0);
	str = str.substr(0, str.find_last_of(ch));
}

zsString cStrUtil::CutDirectory(const zsString& str) {
	const wchar_t *s = str.c_str();
	size_t newSize = str.size();
	if (s != '\0') {
		while (*s != '\0') s++; // Go back

		while (*s != '\\' && *s != '/') {
			newSize--;
			s--;
		}
		newSize++;
	}

	return str.substr(0, newSize);
}

void cStrUtil::CutFrontFromDelim(zsString& strOut, wchar_t ch) {
	const wchar_t *str = strOut.c_str();
	size_t idx = 0;
	while (str[idx] != '\0' && str[idx++] != ch);
	strOut = strOut.substr(idx, strOut.size() - idx);
}

void cStrUtil::GetNumberFromEnd(char* src, char* buf_out) {
	// Move to end
	while (*src != '\0')
		src++;

	while (isdigit(*--src))
		*buf_out++ = *src;
	*buf_out = '\0';
}

void cStrUtil::GetWordBetween(zsString& str, char left, char right, char* buf_out) {
	wchar_t const* strP = str.c_str();

	// Leave spaces from left and try reach left bound
	while ((char)*strP == ' ' || (char)*strP != left)
		strP++;
	strP++;

	// Leave spaces
	while ((char)*strP == ' ')
		strP++;

	// Copy characters while not end of string or right bound reached
	while ((char)*strP != right && (char)*strP != '\0')
		*buf_out++ = (char)*strP++;

	// Terminate string
	*buf_out = '\0';
}

void cStrUtil::GetFloats(const zsString& str, std::vector<float>& floats_out) {
	// Clear output if it's not clear
	if (floats_out.size() != 0)
		floats_out.clear();

	wchar_t const* strP = str.c_str();
	while (!isdigit(*strP))
		strP++;

	size_t offset = 0;
	wchar_t* end = nullptr;
	do {
		floats_out.push_back((float)wcstod(strP + offset, &end));
		offset = end - strP;
	} while (*end != '\0' && end != nullptr);
}

zsString cStrUtil::GetDirectory(const zsString& str) {
	const wchar_t* strP = str.c_str();
	if (strP[0] == '\0')
		return zsString();


	// Copy whole string
	zsString directory = strP;

	// Null terminate from right to left, while not reaching directory
	size_t i = directory.size() - 1;
	for (; directory[i] != '\\' && directory[i] != '/' && i > 0; i--);

	// Found directory sign
	if (i != 0)
		i++;

	directory.resize(i);

	return directory;
}

zsString cStrUtil::GetWordAfter(const std::list<zsString>& strs, const zsString& str) {
	size_t idx = 0;
	auto iter = strs.begin();
	while (iter != strs.end()) {
		size_t start_pos = iter->find(str);
		if (start_pos != std::wstring::npos) {
			start_pos += str.size();
			idx = start_pos;
			while ((*iter)[idx] != ' ') {
				idx++;
			}
			return zsString(iter->substr(start_pos, idx - start_pos));
		}
		iter++;
	}
	return zsString();
}

std::list<zsString> cStrUtil::GetLinesBetween(const std::list<zsString>& fileLines, const zsString& str, const zsString& endLine) {
	std::list<zsString> result;

	// For each line
	auto iter = fileLines.begin();
	while (iter != fileLines.end()) {
		size_t start_pos = iter->find(str);

		// Gound str
		if (start_pos != std::wstring::npos) {
			iter++; // Step further
			// Check for out of bound
			if (iter == fileLines.end())
				break;

			while (iter != fileLines.end()) {
				if (iter->find(endLine) != std::wstring::npos)
					break;
				result.push_back(*iter);
				iter++;
			}
			break;
		}
		iter++;
	}
	return result;
}

std::list<zsString> cStrUtil::GetLinesBeginsWith(const std::list<zsString>& fileLines, const zsString& str) {
	std::list<zsString> result;

	bool match = true;
	auto iter = fileLines.begin();
	while (iter != fileLines.end()) {
		match = true;
		wchar_t const* tmp1 = iter->c_str();
		wchar_t const* tmp2 = str.c_str();

		if (*tmp1 == '\0')
			match = false;

		while (*tmp2 != '\0') {
			if (*tmp1 != *tmp2) {
				match = false;
				break;
			}

			tmp1++;
			tmp2++;
		}

		if (match)
			result.push_back(*iter);

		iter++;
	}
	return result;
}

std::list<size_t> cStrUtil::GetLinesContainingAllStr(const std::list<zsString>& in, const zsString* those, size_t nThose) {
	std::list<size_t> result;

	size_t idx = 0;
	uint16_t i;

	for (auto str : in) {
		// if that line contains all of the "those" words, then push the string
		for (i = 0; i < nThose; i++)
		if (! cStrUtil::Contains(str, those[i]))
				break;

		if (i == nThose)
			result.push_back(idx);

		idx++;
	}

	return result;
}