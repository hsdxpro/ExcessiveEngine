if( LINUX_BUILD )
  include( Externals/linux/lib.cmake )
endif()

if( OSX_BUILD )
  include( Externals/osx/lib.cmake )
endif()

if( WIN_BUILD )
  include( Externals/win/lib.cmake )
endif()

include_directories( include )
