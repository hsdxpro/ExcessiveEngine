#ifndef ITextureView_h
#define ITextureView_h

class ITextureView
{
  public:
    virtual void create() = 0; //TODO
    virtual void destroy() = 0;
    virtual void setWrapping() = 0; //TODO
    virtual void setFiltering() = 0; //TODO
};

#endif