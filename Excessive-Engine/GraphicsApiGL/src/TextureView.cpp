#include "TextureView.h"

#include "custom_assert.h"
#include "Texture.h"

void TextureView::destroy()
{
  glDeleteTextures( 1, &id );
  id = 0;
}

//TODO move these to a common function maybe?
void TextureView::setSamplerState(const rTextureSamplerData* data)
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

void TextureView::update(const rTextureUpdateData* data)
{
  ASSERT( data )
  {
    if( dim == eDimensions::ONE )
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

void TextureView::getSubData(const rTextureUpdateData* data)
{
  ASSERT( data )
  {
    glGetTextureSubImage( id, data->level, data->x_offset, data->y_offset, data->z_offset, data->width, data->height, data->depth, texture_formats[data->format], texture_types[data->format], ((data->width - data->x_offset) * (data->height - data->y_offset) * (data->depth - data->z_offset) * texture_sizes[data->format]) / 8, data->data );
  }
}

auto TextureView::getDesc() -> rDesc
{
  return d;
}

void TextureView::genMipChain()
{
  glGenerateTextureMipmap( id );
}