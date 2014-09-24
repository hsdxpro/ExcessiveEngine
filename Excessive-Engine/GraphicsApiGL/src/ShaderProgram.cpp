#include "ShaderProgram.h"

//TODO add error checking in debug mode maybe?

//should be indexed by an eShaderType enum
GLenum shader_types[] = 
{
  GL_VERTEX_SHADER, GL_TESSELLATION_CONTROL_SHADER, GL_TESSELLATION_EVALUATION_SHADER,
  GL_GEOMETRY_SHADER, GL_PIXEL_SHADER, GL_COMPUTE_SHADER
};

#define INFOLOG_SIZE 4096

void ShaderProgram::destroy()
{
  glDeleteProgram( id );    
  id = 0;
}

void ShaderProgram::addShader(const char* src, eShaderType type)
{
  static GLchar infolog[INFOLOG_SIZE];
  
  GLuint shader_id = glCreateShader( shader_types[type] );
  glShaderSource( shader_id, 1, &c, 0 );
  glCompileShader( shader_id );
  glAttachShader( id, shader_id );
  glDeleteShader( id );
}

void ShaderProgram::link()
{
  glLinkProgram( id );
}

void ShaderProgram::getBinary(char** data, unsigned* size)
{
  //no nullptr, stupid!
  if( data && size )
  {
    vector<char> buf;
    GLenum format = 0;
    
    GLint bufsize = 0;
    glGetProgramiv( id, GL_PROGRAM_BINARY_LENGTH, &bufsize );
    
    if( bufsize )
    {
      buf.resize( bufsize );
    
      glGetProgramBinary( id, bufsize, 0, &format, (void*)buf.data() );
      
      //add space for the enum and the size
      *size = bufsize + sizeof(GLint) + sizeof(GLenum);
      *data = new char[*size];
      
      char* ptr = *data;
      static_cast<GLint*>(ptr)[0] = bufsize;
      ptr += sizeof(GLint);
      static_cast<GLenum*>(ptr)[0] = format;
      ptr += sizeof(GLenum);
      memcpy( ptr, buf.data(), bufsize );
    }
  }
}

void ShaderProgram::loadFromBinary(char* data, unsigned size)
{
  if( data )
  {
    char* ptr = data;
    GLint size = static_cast<GLint*>(ptr)[0];
    ptr += sizeof(GLint);
    GLenum format = static_cast<GLenum*>(ptr)[0];
    ptr += sizeof(GLenum);
    
    glProgramBinary( id, format, ptr, size );
  }
}