message( "-lib-osx" )

if( ARCH_32BIT )
  link_directories( ${CMAKE_CURRENT_LIST_DIR}/lib32 )
else()
  link_directories( ${CMAKE_CURRENT_LIST_DIR}/lib64 )
endif()
