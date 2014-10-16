#include "tests.h"

#include <iostream>

#include <Graphics.h>

using namespace ge;

#ifdef _MSC_VER
#define _WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

using namespace std;


int PetiGraphics() {
#ifdef _MSC_VER
	// load the DLL
	HMODULE module = LoadLibraryA("GraphicsEngineRaster.dll");
	if (!module) {
		cout << "Failed to load module, code " << GetLastError() << endl;
		return 1;
	}
	// load function
	IGraphicsEngine*(*CreateGraphicsEngine)() = (IGraphicsEngine*(*)())GetProcAddress(module, "CreateGraphicsEngine");
	if (CreateGraphicsEngine == nullptr) {
		cout << "Could not find function in module, code " << GetLastError() << endl;
		return 2;
	}

	// do stuff :)
	cout << "Doing stuff... " << endl;

	IGraphicsEngine* engine = CreateGraphicsEngine();

	//IMesh* myMesh;
	//IMaterial* myMat;

	
	engine->release();

#else
	std::cout << "Currently only supports Windows systems." << std::endl;
#endif
	return 0;
}