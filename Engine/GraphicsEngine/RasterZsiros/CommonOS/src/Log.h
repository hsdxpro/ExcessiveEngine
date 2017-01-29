// Log.h By ZsíroskenyérTeam 2013.10.22
// Win32 implementation of Log interface
#include "../../Core/src/ILog.h"

class cLog : public ILog {
public:
	void MsgBox(const zsString& str);
	void Log(const zsString& str);
};
