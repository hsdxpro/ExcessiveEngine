#include "Gapi.h"

#include "ShaderProgram.h"
#include "Texture.h"
#include "TextureView.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <iostream>
using namespace std;

static Gapi* gapi = 0;

EXPORT IGapi* getGapi()
{
  if( !gapi )
  {
    gapi = new Gapi();

    GLenum glew_error = glewInit();
    glGetError(); //ignore glew errors

    if( glew_error != GLEW_OK )
    {
      cerr << "Error initializing GLEW: " << glewGetErrorString( glew_error ) << endl;
    }

    if( !GLEW_VERSION_4_4 )
    {
      cerr << "Error: GL 4.4 is required" << endl;
    }
  }

  return gapi;
}

GLenum func_data[] =
{
  GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS
};

GLenum stencil_op_data[] = 
{
  GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT 
};

GLenum blend_eq_data[] =
{
  GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX
};

GLenum blend_func_data[] =
{
  GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA
};

GLenum raster_mode_data[] = 
{
  GL_POINT, GL_LINE, GL_FILL
};

GLenum raster_face_data[] = 
{
  GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
};

GLenum raster_order_data[] = 
{
  GL_CW, GL_CCW
};

IShaderProgram* Gapi::createShaderProgram()
{
  ShaderProgram* sp = new ShaderProgram();
  sp->id = glCreateProgram();
  return sp;
}

ITexture* Gapi::createTexture(rTextureData* data)
{
  Texture* tex = new Texture();
  glGenTextures( 1, &tex->id );
  tex->target = 0;
  
  if( data )
  {
    if( data->height > 1 )
    {
      if( data->depth > 1 )
      {
        //3D
        tex->dim = THREE;
        
        if( data->is_layered )
        {
          if( data->is_cubemap )
          {
            tex->target = GL_TEXTURE_CUBE_MAP_ARRAY;
          }
          else
          {
            tex->target = GL_TEXTURE_2D_ARRAY;
          }
        }
        else
        {
          tex->target = GL_TEXTURE_3D;
        }
        
        glTexStorage3D( tex->target, data->num_levels, texture_internal_formats[data->format], data->width, data->height, data->depth );        
      }
      else
      {
        //2D
        tex->dim = TWO;
        
        if( data->is_layered )
        {
          tex->target = GL_TEXTURE_1D_ARRAY;
        }
        else if( data->is_cubemap )
        {
          tex->target = GL_TEXTURE_CUBE_MAP;
        }
        else
        {
          tex->target = GL_TEXTURE_2D;
        }
        
        glTexStorage2D( tex->target, data->num_levels, texture_internal_formats[data->format], data->width, data->height );
      }
    }
    else
    {
      //1D
      tex->dim = ONE;
      
      tex->target = GL_TEXTURE_1D;
      glTexStorage1D( tex->target, data->num_levels, texture_internal_formats[data->format], data->width );
    }
    
    tex->d = *data;
  }
  
  return tex;
}

ITextureView* Gapi::createTextureView(rTextureViewData* data)
{
  TextureView* tex = new TextureView();
  glGenTextures( 1, &tex->id );
  
  if( data )
  {
    tex->dim = data->dim;
  
    if( data->dim == ONE )
    {
      tex->target = GL_TEXTURE_1D;
    }
    else if( data->dim == TWO )
    {
      if( data->is_layered )
      {
        tex->target = GL_TEXTURE_1D_ARRAY;
      }
      else if( data->is_cubemap )
      {
        tex->target = GL_TEXTURE_CUBE_MAP;
      }
      else
      {
        tex->target = GL_TEXTURE_2D;
      }
    }
    else
    {
      if( data->is_layered )
      {
        if( data->is_cubemap )
        {
          tex->target = GL_TEXTURE_CUBE_MAP_ARRAY;
        }
        else
        {
          tex->target = GL_TEXTURE_2D_ARRAY;
        }
      }
      else
      {
        tex->target = GL_TEXTURE_3D;
      }
    }
  
    glTextureView(	tex->id,
    tex->target,
    static_cast<Texture*>(data->base_tex)->id,
    texture_internal_formats[data->format],
    data->start_level,
    data->num_levels,
    data->start_layer,
    data->num_layers );
  }
  
  return tex;
}

IVertexBuffer* Gapi::createVertexBuffer(rAllocData* data)
{
  VertexBuffer* vbo = new VertexBuffer();
  glGenBuffers( 1, &vbo->id );
  
  
  glNamedBufferStorage( vbo->id, data->size, 0, GL_DYNAMIC_STORAGE_BIT | 
                                             (data->is_readable ? GL_MAP_READ_BIT : 0) |
                                             (data->is_writable ? GL_MAP_WRITE_BIT : 0) |
                                             (data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
                                             (data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );

  return vbo;
}

IIndexBuffer* Gapi::createIndexBuffer(rAllocData* data)
{
  IndexBuffer* ibo = new IndexBuffer();
  glGenBuffers( 1, &ibo->id );
  
  glNamedBufferStorage( ibo->id, data->size, 0, GL_DYNAMIC_STORAGE_BIT | 
                                           (data->is_readable ? GL_MAP_READ_BIT : 0) |
                                           (data->is_writable ? GL_MAP_WRITE_BIT : 0) |
                                           (data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
                                           (data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );
  
  return ibo;
}

IUniformBuffer* Gapi::createUniformBuffer(rAllocData* data)
{
  UniformBuffer* ubo = new UniformBuffer();
  glGenBuffers( 1, &ubo->id );
  
  glNamedBufferStorage( ubo->id, data->size, 0, GL_DYNAMIC_STORAGE_BIT | 
                                           (data->is_readable ? GL_MAP_READ_BIT : 0) |
                                           (data->is_writable ? GL_MAP_WRITE_BIT : 0) |
                                           (data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
                                           (data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );
  
  return ubo;
}

void Gapi::setDepthState(rDepthState* state)
{
  if( state )
  {
    if( state->depth_test )
    {
      glEnable( GL_DEPTH_TEST );
    }
    else
    {
      glDisable( GL_DEPTH_TEST );
    }
    
    glDepthMask( state->depth_mask );
    
    glDepthRangef( state->near, state->far );
    
    glDepthFunc( func_data[state->func] );
  }
}

void Gapi::setStencilState(rStencilState* state)
{
  if( state )
  {
    if( state->stencil_test )
    {
      glEnable( GL_STENCIL_TEST );
    }
    else
    {
      glDisable( GL_STENCIL_TEST );
    }
    
    glDepthMask( state->stencil_mask );
    
    glStencilMask( state->mask );
    
    glStencilFunc( func_data[state->func], state->reference_stencil_value, state->func_mask );
    
    glStencilOp( stencil_op_data[state->on_stencil_fail], stencil_op_data[state->on_stencil_pass_depth_fail], stencil_op_data[state->on_stencil_pass_depth_pass] );
  }
}

void Gapi::setBlendState(rBlendState* state)
{ 
  if( state )
  {
    if( state->blend_test )
    {
      glEnable( GL_BLEND );
    }
    else
    {
      glDisable( GL_BLEND );
    }
    
    glBlendColor( state->blend_color.x, state->blend_color.y, state->blend_color.z, state->blend_color.w );
    
    glBlendEquation( blend_eq_data[state->equation] );
    
    glBlendFunc( blend_func_data[state->src_func], blend_func_data[state->dst_func] );
  }
}

void Gapi::setSRGBWrites(bool val)
{
  if( val )
  {
    glEnable( GL_FRAMEBUFFER_SRGB );
  }
  else
  {
    glDisable( GL_FRAMEBUFFER_SRGB );
  }
}

void Gapi::setSeamlessCubeMaps(bool val)
{
  if( val )
  {
    glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
  }
  else
  {
    glDisable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
  }
}

void Gapi::setViewport(int x, int y, unsigned w, unsigned h)
{
  glViewport( x, y, w, h );
}

void Gapi::setRasterizationState(rRasterizerState* state)
{
  glPolygonMode(GL_FRONT_AND_BACK, raster_mode_data[state->mode]);
  glFrontFace( raster_order_data[state->vertex_order] );
  glCullFace( raster_face_data[state->face] );
  glColorMask( state->r_mask, state->g_mask, state->b_mask, state->a_mask );
}

bool Gapi::getError() //true if error
{
  return glGetError != GL_NO_ERROR;
}

void Gapi::setDebugOutput(bool val)
{
  if( val )
  {
    glEnable(GL_DEBUG_OUTPUT);
  }
  else
  {
    glDisable(GL_DEBUG_OUTPUT);
  }
}

void Gapi::setSyncDebugOutput(bool val)
{
  if( val )
  {
    glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
  }
  else
  {
    glDisable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
  }
}

void Gapi::passTextureView(IShaderProgram* s, ITextureView* tex, unsigned index)
{
  glBindTextureUnit( index, static_cast<TextureView*>(tex)->id );
}

void Gapi::passRenderTargets(IShaderProgram* s, rTargetData* render_targets, unsigned size)
{
  //TODO
}

void Gapi::passUniformBuffer(IShaderProgram* s, IUniformBuffer* buf)
{
  //TODO
}

void Gapi::passVertexBuffer(IShaderProgram* s, IVertexBuffer* vbos, unsigned num_vbos)
{
  //TODO
}

void Gapi::draw(IShaderProgram* s, unsigned num_indices)
{
  //TODO
}
