#ifndef IGapi_h
#define IGapi_h

#include "mymath/mymath.h"

class IGapi
{
  public:
    virtual void setDepthTest(bool val) = 0;
    virtual void setDepthRange(float near, float far) = 0;
    virtual void setDepthMask(bool val) = 0;

    virtual void setStencilTest(bool val) = 0;
    virtual void setStencilFunc() = 0; //TODO
    virtual void setStencilOp() = 0; //TODO
    virtual void setStencilMask(bool val) = 0;

    virtual void setBlendState(bool val) = 0;
    virtual void setBlendEquation() = 0; //TODO
    virtual void setBlendFunc() = 0; //TODO
    virtual void setBlendColor(mm::vec4 color) = 0;

    virtual void setSRGBWrites(bool val) = 0;
    virtual void setSeamlessCubeMaps(bool val) = 0;
    
    virtual void setViewport(int x, int y, unsigned w, unsigned h) = 0;

    virtual void setPolygonMode() = 0; //TODO
    virtual void setFaceCulling(bool val) = 0;
    virtual void setFrontFace() = 0; //TODO
    virtual void setCullMode() = 0; //TODO
    virtual void setColorMask(bool r, bool g, bool b, bool a) = 0;

    virtual void getError() = 0; //TODO
    virtual void setDebugOutput(bool val) = 0;
    virtual void setSyncDebugOutput(bool val) = 0;
};



#endif