#the platform independence layer should be included first

if( DEBUG_BUILD )
  add_definitions( "-DDEBUG_BUILD" )
  set( CMAKE_BUILD_TYPE Debug )
  message( "Debug mode" )
else()
  add_definitions( "-DRELEASE_BUILD" )
  set( CMAKE_BUILD_TYPE Release )
  message( "Release mode" )
endif()

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  message( "Linux build" )
  set( LINUX_BUILD true )
	add_definitions( "-DLINUX_BUILD" )
endif()

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  message( "OSX build" )
  set( OSX_BUILD true )
	add_definitions( "-DOSX_BUILD" )
endif()

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
  message( "Windows build" )
  set( WIN_BUILD true )
	add_definitions( "-DWIN_BUILD" )
endif()

if( MSVC )
  message( "MSVC compiler detected" )
  set( MSVC_COMPILER true )
	add_definitions( "-DMSVC_COMPILER" )
endif()

if( CMAKE_COMPILER_IS_GNUCXX )
  message( "G++ compiler detected" )
  set( GPP_COMPILER true )
	add_definitions( "-DGPP_COMPILER" )
endif()

if( LINUX_BUILD )
  if( ARCH_32BIT )
    message( "32 bit build" )
	  set( CMAKE_CXX_FLAGS "-m32" ) #compile, link, etc. with 32 bits
	  set( CMAKE_C_FLAGS "-m32" )
	  set( CMAKE_SHARED_LINKER_FLAGS "-m32" )
	  add_definitions( "-march=i686 -DARCH_32BIT" ) #go with generic 32 bits
  else()
    message( "64 bit build" )
	  add_definitions( "-march=x86-64" ) #go with generic 64 bits
  endif()

  #default to compiler optimizations
  add_definitions( "-mtune=nocona -mfpmath=sse" ) 

  #best practice
  add_definitions( "-Wall -Wno-long-long -ansi -pedantic -std=c++0x" )
else()
  if( ARCH_32BIT )
    message( "32 bit build" )
    add_definitions( "-DARCH_32BIT" )
  else()
    message( "64 bit build" )
  endif()
endif()

include_directories( ${CMAKE_CURRENT_LIST_DIR} )
