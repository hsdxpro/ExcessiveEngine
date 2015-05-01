#include <unordered_map>
#include <map>


// add you test function here & make a new .cpp for it
int PetiTestGeneric();
int CommonTest();
int PetiGraphics();
int Ricsi();
int breakpadTest();
int physFsTest();

const std::map<std::string, int(*)(void)> test_cases = {
	// add you test here
		{"peti_test_generic",	PetiTestGeneric},
		{"peti_graphics",		PetiGraphics },
		{"common_test",			CommonTest},
		{"ricsi",				Ricsi },
    {"breakpad_test",				breakpadTest },
		{"physfs_test",				physFsTest },
};
