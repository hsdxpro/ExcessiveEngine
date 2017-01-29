////////////////////////////////////////////////////////////////////////////////
//	CreateGAPI.h
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Link to this library, and include this file, so that you can use the GAPI's
//	CreateGraphicsApi function to seamlessly create IGrapicsApi from nearby DLLs.
////////////////////////////////////////////////////////////////////////////////


#pragma once
#include <vector>


enum class eGraphicsApiType {
	GRAPHICS_API_OPENGL43	= 1,
	GRAPHICS_API_D3D11		= 2,
};

class IGraphicsApi;
class IWindow;

IGraphicsApi* CreateGraphicsApi(IWindow* targetWindow, unsigned backBufferWidth, unsigned backBufferHeight, eGraphicsApiType type);
std::vector<eGraphicsApiType> EnumGraphicsApiTypes();