#include "Texture.h"

#include "custom_assert.h"

//not final, may change
GLenum texture_formats[] = 
{
  GL_RED,
  GL_RED,
  GL_RG,
  GL_RG,
  GL_RED,
  GL_RED,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGB,
  GL_RGBA,
  GL_RED,
  GL_RG,
  GL_RGB,
  GL_RGBA,
  GL_RED,
  GL_RG,
  GL_RGB,
  GL_RGBA,
  GL_RGB,
  GL_RGBA,
  GL_RED,
  GL_RED,
  GL_RED,
  GL_RED,
  GL_RED,
  GL_RED,
  GL_RG,
  GL_RG,
  GL_RG,
  GL_RG,
  GL_RG,
  GL_RG,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_DEPTH_COMPONENT,
  GL_DEPTH_COMPONENT,
  GL_DEPTH_COMPONENT,
  GL_DEPTH_COMPONENT,
  GL_DEPTH_COMPONENT, //DS???
  GL_DEPTH_COMPONENT, //DS???
  GL_STENCIL_INDEX,
  GL_STENCIL_INDEX,
  GL_STENCIL_INDEX,
  GL_STENCIL_INDEX
};

//not final, may change
GLenum texture_types[] = 
{
  GL_UNSIGNED_BYTE,
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_BYTE,
  GL_UNSIGNED_SHORT,
  GL_SHORT,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_SHORT,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_FLOAT,
  GL_FLOAT,
  GL_FLOAT,
  GL_FLOAT,
  GL_FLOAT,
  GL_FLOAT,
  GL_FLOAT,
  GL_FLOAT,
  GL_FLOAT,
  GL_UNSIGNED_BYTE,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT,
  GL_FLOAT,
  GL_UNSIGNED_INT,
  GL_FLOAT,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT
};

unsigned texture_sizes[] = 
{
  8,
  8,
  16,
  16,
  16,
  16,
  8,
  12,
  15,
  24,
  24,
  30,
  36,
  48,
  8,
  16,
  16,
  32,
  32,
  32,
  32,
  48,
  64,
  32,
  32,
  16,
  32,
  48,
  64,
  32,
  64,
  96,
  128,
  32,
  32,
  8,
  8,
  16,
  16,
  32,
  32,
  16,
  16,
  32,
  32,
  64,
  64,
  24,
  24,
  48,
  48,
  96,
  96,
  32,
  32,
  64,
  64,
  128,
  128,
  16,
  24,
  32,
  32,
  32,
  40,
  1,
  4,
  8,
  16
};

GLenum texture_internal_formats[] = 
{
  GL_R8,
  GL_R8_SNORM,
  GL_RG8,
  GL_RG8_SNORM,
  GL_R16,
  GL_R16_SNORM,
  GL_R3_G3_B2,
  GL_RGB4,
  GL_RGB5,
  GL_RGB8,
  GL_RGB8_SNORM,
  GL_RGB10,
  GL_RGB12,
  GL_RGB16_SNORM,
  GL_RGBA2,
  GL_RGBA4,
  GL_RGB5_A1,
  GL_RGBA8,
  GL_RGBA8_SNORM,
  GL_RGB10_A2,
  GL_RGB10_A2UI,
  GL_RGBA12,
  GL_RGBA16,
  GL_SRGB8,
  GL_SRGB8_ALPHA8,
  GL_R16F,
  GL_RG16F,
  GL_RGB16F,
  GL_RGBA16F,
  GL_R32F,
  GL_RG32F,
  GL_RGB32F,
  GL_RGBA32F,
  GL_R11F_G11F_B10F,
  GL_RGB9_E5,
  GL_R8I,
  GL_R8UI,
  GL_R16I,
  GL_R16UI,
  GL_R32I,
  GL_R32UI,
  GL_RG8I,
  GL_RG8UI,
  GL_RG16I,
  GL_RG16UI,
  GL_RG32I,
  GL_RG32UI,
  GL_RGB8I,
  GL_RGB8UI,
  GL_RGB16I,
  GL_RGB16UI,
  GL_RGB32I,
  GL_RGB32UI,
  GL_RGBA8I,
  GL_RGBA8UI,
  GL_RGBA16I,
  GL_RGBA16UI,
  GL_RGBA32I,
  GL_RGBA32UI,
  GL_DEPTH_COMPONENT16,
  GL_DEPTH_COMPONENT24,
  GL_DEPTH_COMPONENT32,
  GL_DEPTH_COMPONENT32F,
  GL_DEPTH24_STENCIL8,
  GL_DEPTH32F_STENCIL8,
  GL_STENCIL_INDEX1,
  GL_STENCIL_INDEX4,
  GL_STENCIL_INDEX8,
  GL_STENCIL_INDEX16
};

void Texture::destroy()
{
  glDeleteTextures( 1, &id );
  id = 0;
}

void Texture::setSamplerState(const rTextureSamplerData* data)
{
  ASSERT( data && target )
  {
    //GL needs a glTextureParameterx so that no stupid binding is needed
    //if( currently_bound_textures[0] != id )
    {
      //glBindTexture( target, id );
      //glBindTextureUnit( 0, id );
      //currently_bound_textures[0] = id;
    }
  
    if( data->is_anisotropic )
    {
      glTextureParameterf( id, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy );
    }
    
    if( data->is_clamped )
    {
      glTextureParameteri( id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
      glTextureParameteri( id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
      glTextureParameteri( id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    }
    else
    {
      glTextureParameteri( id, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTextureParameteri( id, GL_TEXTURE_WRAP_T, GL_REPEAT );
      glTextureParameteri( id, GL_TEXTURE_WRAP_R, GL_REPEAT );
    }
    
    if( data->is_bilinear )
    {
      if( data->is_mipmapped )
      {
        glTextureParameteri( id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      }
      
      glTextureParameteri( id, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
      if( data->is_mipmapped )
      {
        glTextureParameteri( id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
      }
      
      glTextureParameteri( id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    }
  }
}

void Texture::update(const rTextureUpdateData* data)
{
  ASSERT( data )
  {
	  if (dim == eDimensions::ONE)
    {
      glTextureSubImage1D( id, data->level, data->x_offset, data->width, texture_formats[data->format], texture_types[data->format], data->data );
    }
    else if( dim == eDimensions::TWO )
    {
      glTextureSubImage2D( id, data->level, data->x_offset, data->y_offset, data->width, data->height, texture_formats[data->format], texture_types[data->format], data->data );
    }
    else //threesome
    {
      glTextureSubImage3D( id, data->level, data->x_offset, data->y_offset, data->z_offset, data->width, data->height, data->depth, texture_formats[data->format], texture_types[data->format], data->data );
    }
  }
}

void Texture::getSubData(const rTextureUpdateData* data)
{
  ASSERT( data )
  {
    glGetTextureSubImage( id, data->level, data->x_offset, data->y_offset, data->z_offset, data->width, data->height, data->depth, texture_formats[data->format], texture_types[data->format], ((data->width - data->x_offset) * (data->height - data->y_offset) * (data->depth - data->z_offset) * texture_sizes[data->format]) / 8, data->data );
  }
}

rTextureData Texture::getFormat()
{
  return d;
}

void Texture::genMipChain()
{
  glGenerateTextureMipmap( id );
}