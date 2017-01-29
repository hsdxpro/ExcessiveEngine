////////////////////////////////////////////////////////////////////////////////
//	File: GraphicsEngine/src/Exception.h
//	2013.oct.10, Zsiroskenyer Team, Péter Kardos
////////////////////////////////////////////////////////////////////////////////
//	Collection of exception classes.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdexcept>
#include "../../Core/src/zsString.h"

class GraphicsEngineException {
};

template <int id>
class BaseException : public std::runtime_error, public GraphicsEngineException {
public:
	BaseException(const char* what_arg="no message") : runtime_error(what_arg) {};
	explicit BaseException(const zsString& what_arg) : runtime_error(what_arg) {};
};


typedef BaseException<1> FileNotFoundException;
typedef BaseException<2> InvalidCallException;
typedef BaseException<3> InvalidDataException;
typedef BaseException<4> UnknownErrorException;