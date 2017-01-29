// ISoundEngine.h By Zsíroskenyér Team 2013.10.23 22:38
#pragma once

#include "zsString.h"

class ISoundEngine {
public:
	virtual void Release() = 0;
	virtual bool GenWavFromMicrophone(const zsString& filePath, float stopAfterSeconds) = 0;
	virtual bool isMicrophoneConnected() const = 0;
};