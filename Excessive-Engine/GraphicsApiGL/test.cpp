#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "IGapi.h"

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

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

std::string get_app_path();
#ifdef WIN_BUILD
char* realpath( const char* path, char** ret );
#endif

int main( int argc, char** args )
{
  string app_path = get_app_path();

  sf::Window w;
  w.create( sf::VideoMode( 512, 512 ), "Graphics Api GL Test" );

  if( !w.isOpen() )
  {
    cerr << "Couldn't initialize SFML." << endl;
  }

  w.setVerticalSyncEnabled( true );

  string dll_path = app_path + "Debug/GraphicsApiGL.dll";

  fstream f;
  f.open( dll_path.c_str() );

  if( !f.is_open() )
    cerr << "couldn't open dll." << endl;
  else
  {
    cerr << "Dll opened" << endl;
    f.close();
  }

  std::replace( dll_path.begin(), dll_path.end(), '/', '\\' );
  auto dll = LoadLibrary( dll_path.c_str() );
  getGapiType getGapi = 0;

  if( dll )
  {
    getGapi = (getGapiType)GetProcAddress( dll, "getGapi" );
  }

  IGapi* gapi = getGapi();
  auto sp = gapi->createShaderProgram();
  sp->addShader(vshd, VERTEX_SHADER);
  sp->addShader(pshd, PIXEL_SHADER);

  /*bool run = false;

  while( run )
  {
    w.display();
  }*/

  FreeLibrary(dll);

  w.close();
}

#ifdef WIN_BUILD
char* realpath( const char* path, char** ret )
{
  char* the_ret = 0;

  if( !ret )
  {
    the_ret = new char[MAX_PATH];
  }
  else
  {
    if( !*ret )
    {
      *ret = new char[MAX_PATH];
    }
    else
    {
      unsigned long s = strlen( *ret );

      if( s < MAX_PATH )
      {
        delete [] *ret;
        *ret = new char[MAX_PATH];

      }

      the_ret = *ret;
    }
  }

  unsigned long size = GetFullPathNameA( path, MAX_PATH, the_ret, 0 );

  if( size > MAX_PATH )
  {
    //too long path
    cerr << "Path too long, truncated." << endl;
    delete [] the_ret;
    return "";
  }

  if( ret )
  {
    *ret = the_ret;
  }

  return the_ret;
}
#endif

std::string get_app_path()
{

  char fullpath[4096];
  std::string app_path;

  /* /proc/self is a symbolic link to the process-ID subdir
    * of /proc, e.g. /proc/4323 when the pid of the process
    * of this program is 4323.
    *
    * Inside /proc/<pid> there is a symbolic link to the
    * executable that is running as this <pid>.  This symbolic
    * link is called "exe".
    *
    * So if we read the path where the symlink /proc/self/exe
    * points to we have the full path of the executable.
    */

#ifdef LINUX_BUILD
  int length;
  length = readlink( "/proc/self/exe", fullpath, sizeof( fullpath ) );

  /* Catch some errors: */

  if( length < 0 )
  {
    my::log << my::lock << "Couldnt read app path. Error resolving symlink /proc/self/exe." << my::endl << my::unlock;
    loop::get().shutdown();
  }

  if( length >= 4096 )
  {
    my::log << my::lock << "Couldnt read app path. Path too long. Truncated." << my::endl << my::unlock;
    loop::get().shutdown();
  }

  /* I don't know why, but the string this readlink() function
    * returns is appended with a '@'.
    */
  fullpath[length] = '\0';       /* Strip '@' off the end. */

#endif

#ifdef WIN_BUILD

  if( GetModuleFileName( 0, ( char* )&fullpath, sizeof( fullpath ) - 1 ) == 0 )
  {
    cerr << "Couldn't get the app path." << endl;
    return "";
  }

#endif

  app_path = fullpath;

#ifdef WIN_BUILD
  app_path = app_path.substr( 0, app_path.rfind( "\\" ) + 1 );
#endif

#ifdef __unix__
  config::get().app_path = config::get().app_path.substr( 0, config::get().app_path.rfind( "/" ) + 1 );
#endif

  app_path += "../";

  char* res = 0;

  res = realpath( app_path.c_str(), 0 );

  if( res )
  {
#if WIN_BUILD
    app_path = std::string( res );
    delete [] res;
#endif

#if __unix__
    config::get().app_path = std::string( res ) + "/";
    free( res ); //the original linux version of realpath uses malloc
#endif
  }

  std::replace( app_path.begin(), app_path.end(), '\\', '/' );

  return app_path;
}