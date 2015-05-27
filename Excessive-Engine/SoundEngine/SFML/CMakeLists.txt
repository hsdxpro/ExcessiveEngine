message( "-SoundEngineSFML" )

#########################################################################
# Files

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../Interfaces/*)

# Interface
SOURCE_GROUP(Interfaces FILES ${interfaces})

# Main
SOURCE_GROUP(Main FILES src/SoundEngineSFML.h src/SoundEngineSFML.cpp)

# Scene
SOURCE_GROUP(Scene FILES
src/SoundScene.h
src/SoundScene.cpp
src/Emitter.h
src/Emitter.cpp
src/Listener.h
src/Listener.cpp
src/SFMLCommonSoundEmitter.h
src/SFMLCommonSoundEmitter.cpp
src/SFMLSoundWrapper.h
src/SFMLSoundWrapper.cpp
src/SFMLMusicWrapper.h
src/SFMLMusicWrapper.cpp
)

# Resource management files filter
SOURCE_GROUP(Resources FILES
src/SoundData.h
src/SoundData.cpp
src/SFMLCommonSoundData.h
src/SFMLCommonSoundData.cpp
src/SFMLMusicData.h
src/SFMLMusicData.cpp
src/SFMLSoundData.h
src/SFMLSoundData.cpp
)

#########################################################################
# Project

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( SoundEngineSFML SHARED ${sources} ${headers} ${interfaces} )
	target_link_libraries( SoundEngineSFML "-Wl,--whole-archive" Common  "-Wl,--no-whole-archive")
else()
	add_library( SoundEngineSFML STATIC ${sources} ${headers} ${interfaces} )
endif()
