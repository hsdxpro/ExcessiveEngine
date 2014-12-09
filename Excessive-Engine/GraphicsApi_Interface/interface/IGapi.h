#pragma once

#include "mymath/mymath.h"

#include "Common.h"
#include "ITextureView.h"
#include "ITexture.h"
#include "IBuffer.h"
#include "IShaderProgram.h"
#include "IInputLayout.h"

#include <cstdint>


class IGapi
{
  public:
    //virtual IShaderProgram* createShaderProgram() = 0;
	virtual IShaderProgram* createShaderProgram(const rShaderPaths& data) = 0;
	virtual IShaderProgram* createShaderProgram(const rShaderSources& data) = 0;

	virtual IUniformBuffer* createUniformBuffer(const IUniformBuffer::rDesc& data) = 0;
	virtual IVertexBuffer*	createVertexBuffer(const IVertexBuffer::rDesc& data) = 0;
	virtual ITextureView*	createTextureView(const ITextureView::rDesc& data) = 0;
	virtual IIndexBuffer*	createIndexBuffer(const IIndexBuffer::rDesc& data) = 0;
	virtual ITexture*		createTexture(const rTexture& data) = 0;
	virtual ITexture*		createTexture(const std::string& path) = 0;
	
	virtual void WriteTexture(ITexture* t, const rTextureUpdate& d) = 0;
  
    virtual void setDepthState(const rDepthState& state) = 0;
    virtual void setStencilState(const rStencilState& state) = 0;
    virtual void setBlendState(const rBlendState& state) = 0;
	virtual void setRasterizationState(const rRasterizerState& state) = 0;

    virtual void setSRGBWrites(bool val) = 0;
    virtual void setSeamlessCubeMaps(bool val) = 0;
    
    virtual void setViewport(int x, int y, unsigned w, unsigned h) = 0;    

    virtual bool getError() = 0;
    virtual void setDebugOutput(bool val) = 0;
    virtual void setSyncDebugOutput(bool val) = 0;

    //pass input/output to shader
    virtual void setShaderProgram(IShaderProgram* sp) = 0;
    virtual void setTextureView(ITextureView* tex, unsigned index) = 0;
	virtual void setRenderTargets(const rRenderTargetInfo* render_targets, unsigned size) = 0;
    virtual void setUniformBuffer(IUniformBuffer* buf, unsigned index) = 0;
	virtual void setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, unsigned num_buffers) = 0;
    virtual void setIndexBuffer(IIndexBuffer* ibo) = 0;
    
    //draw stuff
    virtual void draw(unsigned num_indices) = 0;

	// input layout & vertex streams
#pragma message("Marci: ezt is implementálnod kéne [setVertexBuffer]")

	/*
	@see IInputLayout.h	
	*/
	virtual IInputLayout* createInputLayout(InputElement* elements, size_t num_elements) = 0;
	virtual void setInputLayout(IInputLayout* layout) = 0;

	/*
	Egyszerûen bindeli a vertex buffereket a megfelelõ 'slot'-ba.
	Ha több buffert adunk meg, akkor azokat a start_slot, start_slot+1, start_slot+2 helyekre bindeli.
	A nullptr buffer unbindeli az adott slotból a buffert.
	*/
	virtual void setVertexStreams(
		IVertexBuffer** buffers, // buffers to bind
		unsigned* strides, // size of one vertex in bytes; for each buffer
		unsigned* offsets, // how many bytes the 0th vertex is offseted from start of buffer
		unsigned start_slot, // bind 1st buffer here
		unsigned num_buffers) = 0; 
};