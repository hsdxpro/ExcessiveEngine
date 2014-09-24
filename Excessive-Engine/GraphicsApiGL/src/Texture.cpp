#include "Texture.h"

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

void Texture::destroy()
{
  glDeleteTextures( 1, &id );
  id = 0;
}

void Texture::setSamplerState(const rTextureSamplerData* data)
{
  if( data && target )
  {
    //GL needs a glTextureParameterx so that no stupid binding is needed
    if( currently_bound_textures[0] != id )
    {
      glBindTextureUnit( 0, id );
      currently_bound_textures[0] = id;
    }
  
    if( data->is_anisotropic )
    {
      glTexParameterf( target, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy );
    }
    
    if( data->is_clamped )
    {
      glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
      glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
      glTexParameteri( target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    }
    else
    {
      glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
      glTexParameteri( target, GL_TEXTURE_WRAP_R, GL_REPEAT );
    }
    
    if( data->is_bilinear )
    {
      if( data->is_mipmapped )
      {
        glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      }
      
      glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
      if( data->is_mipmapped )
      {
        glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
      }
      
      glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    }
  }
}

void Texture::update(const rTextureUpdateData* data)
{
  if( data && target )
  {
    if( dim == ONE )
    {
      glTextureSubImage2D( id, target, data->level, data->x_offset, data->width, texture_formats[data->format], texture_types[data->format], data->data );
    }
    else if( dim == TWO )
    {
      glTextureSubImage2D( id, target, data->level, data->x_offset, data->y_offset, data->width, data->height, texture_formats[data->format], texture_types[data->format], data->data );
    }
    else //threesome
    {
      glTextureSubImage3D( id, target, data->level, data->x_offset, data->y_offset, data->z_offset, data->width, data->height, data->depth, texture_formats[data->format], texture_types[data->format], data->data );
    }
  }
}

void Texture::getSubData(const rTextureUpdateData* data)
{
  if( data && target )
  {
    if( dim == ONE )
    {
      glTextureSubImage2D( id, target, data->level, data->x_offset, data->width, texture_formats[data->format], texture_types[data->format], ((data->width - data->x_offset) * texture_sizes[data->format]) / 8, data->data );
    }
    else if( dim == TWO )
    {
      glTextureSubImage2D( id, target, data->level, data->x_offset, data->y_offset, data->width, data->height, texture_formats[data->format], texture_types[data->format], ((data->width - data->x_offset) * (data->height - data->y_offset) * texture_sizes[data->format]) / 8, data->data );
    }
    else //threesome
    {
      glTextureSubImage3D( id, target, data->level, data->x_offset, data->y_offset, data->z_offset, data->width, data->height, data->depth, texture_formats[data->format], texture_types[data->format], ((data->width - data->x_offset) * (data->height - data->y_offset) * (data->depth - data->z_offset) * texture_sizes[data->format]) / 8, data->data );
    }
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