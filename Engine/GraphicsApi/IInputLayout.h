#pragma once


#include "Common.h"


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


struct rInputElement 
{
	char name[65]; ///< variable name (or semantic) in shader
	eVertexAttribType type; ///< \see eVertexAttribType
	u32 num_components; ///< vector's dimension
	u32 offset; ///< this element begins $ bytes offseted from start of vertex
	u32 stream_index; ///< in which vertex stream

	/// Helper for Setting name.
	void SetName(const char* str) { // just to make life easier
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
	/// Get the number of vertex elements.
	virtual size_t GetNumElements() = 0;
	/// Get the indexth vertex element.
	virtual const rInputElement& GetElement(size_t index) = 0;

	virtual void Release() = 0;
};