#include "IGapi.h"

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

const char* vshd = 
"#version 440 core \n"
"uniform mat4 mvp; \n"
"in vec4 in_vertex; \n"
"void main() \n"
"{ \n"
"gl_Position = mvp * in_vertex; \n"
"} \n";

const char* pshd = 
"#version 440 core \n"
"uniform mat4 mvp; \n"
"out vec4 color; \n"
"void main() \n"
"{ \n"
"color = vec4(1); \n"
"} \n";

int main( int argc, char** args )
{
  sf::Window w;
  w.create( sf::VideoMode( 512, 512 ), "Graphics Api GL Test" );
  w.setVerticalSyncEnabled( true );

  IGapi* gapi = new Gapi();
  auto sp = gapi->createShaderProgram();
  sp->addShader(vshd, VERTEX_SHADER);
  sp->addShader(pshd, PIXEL_SHADER);

  bool run = false;

  while( run )
  {
    w.display();
  }

  w.close();
}