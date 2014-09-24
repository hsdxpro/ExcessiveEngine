#pragma once

#include "IGapi.h"

class Gapi : public IGapi
{
  public:
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
    void passTextureView(IShaderProgram* s, ITextureView* tex, unsigned index);
    void passRenderTargets(IShaderProgram* s, rTargetData* render_targets, unsigned size);
    void passUniformBuffer(IShaderProgram* s, IUniformBuffer* buf);
    void passVertexBuffer(IShaderProgram* s, IVertexBuffer* vbos, unsigned num_vbos);
    
    //draw stuff
    void draw(IShaderProgram* s, unsigned num_indices);
};