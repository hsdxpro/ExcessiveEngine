#include <unordered_map>
#include <map>


// Add you test function here & make a new .cpp for it
int PetiTestGeneric();
int CommonTest();
int PetiGraphics();
int Ricsi();
int breakpadTest();
int physFsTest();
int sqliteTest();
int artur();

const std::map<std::string, int(*)(void)> test_cases = {
	// Add you test here
		{"peti_test_generic",  PetiTestGeneric},
		{"peti_graphics",      PetiGraphics },
		{"common_test",        CommonTest},
		{"ricsi",              Ricsi },
		{"breakpad_test",      breakpadTest },
		{"physfs_test",        physFsTest },
		{"sqlite_test",        sqliteTest },
		{"artur",              artur },
};
