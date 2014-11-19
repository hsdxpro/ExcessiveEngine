message( "-externals" )

if( LINUX_BUILD )
  include( ${CMAKE_CURRENT_LIST_DIR}/linux/lib.cmake )
endif()

if( OSX_BUILD )
  include( ${CMAKE_CURRENT_LIST_DIR}/osx/lib.cmake )
endif()

if( WIN_BUILD )
  include( ${CMAKE_CURRENT_LIST_DIR}/win/lib.cmake )
endif()

include_directories( ${CMAKE_CURRENT_LIST_DIR}/include )
