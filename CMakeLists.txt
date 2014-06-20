# The name of our project is "HELLO". CMakeLists files in this project can 
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and 
# to the root binary directory of the project as ${HELLO_BINARY_DIR}. 
cmake_minimum_required (VERSION 2.6)

project( Unnamed-Team )

set( EXECUTABLE_OUTPUT_PATH ../Bin )
set( LIBRARY_OUTPUT_PATH ../Bin )

# Engine
add_subdirectory( Unnamed-Engine/Core )
add_subdirectory( Unnamed-Engine/GapiGL )
add_subdirectory( Unnamed-Engine/GraphicsEngineRaster )

# Game
add_subdirectory( Games/fps )