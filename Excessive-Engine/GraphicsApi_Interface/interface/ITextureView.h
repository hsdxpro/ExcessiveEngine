#pragma once

#include "Common.h"
#include "ITexture.h"

class ITextureView
{
public:
	//specifices what type of texture you'd like to allocate
	struct rDesc
	{
		unsigned start_level; //mipmap levels
		unsigned num_levels;
		unsigned start_layer;
		unsigned num_layers;
		eFormatType format; //texture format
		ITexture* base_tex;
		//new texture target
		unsigned dim;
		bool is_layered;
		bool is_cubemap;
	};
public:
    virtual void destroy() = 0;
    virtual void setSamplerState(const rTextureSamplerData* data) = 0;
    virtual void update(const rTextureUpdateData* data) = 0;
    virtual void getSubData(const rTextureUpdateData* data) = 0; //the pointer should be updated
    virtual rDesc getDesc() = 0;
    virtual void genMipChain() = 0;
};