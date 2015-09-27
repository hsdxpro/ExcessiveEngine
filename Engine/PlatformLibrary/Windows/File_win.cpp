#include "File_win.h"
#include <assert.h>

File::File(const std::string& filePath, eIO flag)
{
	int nativeFlag = 0;
	switch (flag)
	{
	case OUT_BIN:		nativeFlag = std::ios::out | std::ios::binary; break;
	case OUT_BIN_TRUNC: nativeFlag = std::ios::out | std::ios::binary | std::ios::trunc; break;
	case IN_BIN:		nativeFlag = std::ios::in | std::ios::binary; break;
	default: assert(0); break;
	}
	
	fileStream.open(filePath, nativeFlag);
}