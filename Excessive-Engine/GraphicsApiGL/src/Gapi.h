#pragma once

#include "IGapi.h"

#include "GL/glew.h"

class Gapi : public IGapi
{
  public:
    unsigned global_vao;
  
    IShaderProgram* createShaderProgram();
    ITexture* createTexture(rTextureData* data);
    ITextureView* createTextureView(rTextureViewData* data);
    IVertexBuffer* createVertexBuffer(rAllocData* data);
    IIndexBuffer* createIndexBuffer(rAllocData* data);
    IUniformBuffer* createUniformBuffer(rAllocData* data);
  
    void setDepthState(rDepthState* state);

    void setStencilState(rStencilState* state);

    void setBlendState(rBlendState* state);

    void setSRGBWrites(bool val);
    void setSeamlessCubeMaps(bool val);
    
    void setViewport(int x, int y, unsigned w, unsigned h);

    void setRasterizationState(rRasterizerState* state);

    bool getError();
    void setDebugOutput(bool val);
    void setSyncDebugOutput(bool val);
    
    //pass input/output to shader
    void setShaderProgram(IShaderProgram* sp);

    void passTextureView(ITextureView* tex, unsigned index);
    void passRenderTargets(rTargetData* render_targets, unsigned size);
    void passUniformBuffer(IUniformBuffer* buf, unsigned index);
    void passVertexBuffers(IVertexBuffer** vbos, rVertexAttrib* attrib_data, unsigned num_vbos);
    void passIndexBuffer(IIndexBuffer* ibo);
    
    //draw stuff
    void draw(unsigned num_indices);
};