#include <unordered_map>
#include <map>


// add you test function here & make a new .cpp for it
int PetiTestGeneric();
int CommonTest();
int PetiGraphics();


const std::map<std::string, int(*)(void)> test_cases = {
	// add you test here
		{"peti_test_generic", PetiTestGeneric},
		{"common_test", CommonTest},
		{"peti_graphics", PetiGraphics},
};