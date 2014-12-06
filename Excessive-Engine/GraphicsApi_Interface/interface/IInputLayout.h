#pragma once

#include <Common.h>
#include <cstdint>


#pragma message("Marci: ezt a szart kéne implementálnod [InputLayout]")

/*
Na ezt sikerült összehozni vertex format név alatt.

A lényeg röviden:
Meg kell mondani, hogy a bindelt vertex streamekben hol található a shader bemenete.
Minden shader bemenetet egy 'InputElement' ír le. Remélem a forrás alapján ez világos.
A 'type'-ot és 'num_components'-et alakítsd át, ahogy jól esik.
A teljes shader bemnentét egy InputElement[] array írja le.
Az array alapján a Gapi létrehoz egy 'IInputLayout*'-ot, amiben optimalizált, natív formában
tárolja a Gapi a dolgokat. Utána ezt az InputLayoutot lehet bindelni a Gapihoz a VertexBufferek
és a ShaderProgram mellett.

Ha nem megfelelõ InputLayout és ShaderProgram kerül egyidejû bindelésre:
Leszarjuk, nem rajzolunnk, majd rájön a programozó, hogy faszsággal eteti a Gapit.
*/


struct InputElement 
{
	char name[65]; // variable name (or semantic) in shader
	eVertexAttribType type; // @see eVertexAttribType
	uint32_t num_components; // vector's dimension
	uint32_t offset; // this element begins $ bytes offseted from start of vertex
	uint32_t stream_index; // in which vertex stream

	void setName(const char* str) { // just to make life easier
		int i = 0;
		while (str[i] != '\0' && i<65) {
			name[i] = str[i];
			i++;
		}
		name[i] = '\0';
	}
};


class IInputLayout 
{
public:
	virtual size_t getNumComponents() = 0;
	virtual InputElement getElement(size_t index) = 0;
	virtual void release() = 0;
};