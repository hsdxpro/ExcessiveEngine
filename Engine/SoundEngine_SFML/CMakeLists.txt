#-------------------------------------------------------------------------------
# SOUND ENGINE SFML
#
# SFML based implementation of the sound engine.
#-------------------------------------------------------------------------------

message( "-SoundEngineSFML" )

#---------------------------------------
# Files

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )
FILE( GLOB_RECURSE interfaces "../SoundEngine/*.h*")

# Interface
SOURCE_GROUP(Interfaces FILES ${interfaces})

# Main
SOURCE_GROUP(Main FILES SoundEngineSFML.h SoundEngineSFML.cpp)

# Scene
SOURCE_GROUP(Scene FILES
SoundScene.h
SoundScene.cpp
Emitter.h
Emitter.cpp
Listener.h
Listener.cpp
SFMLCommonSoundEmitter.h
SFMLCommonSoundEmitter.cpp
SFMLSoundWrapper.h
SFMLSoundWrapper.cpp
SFMLMusicWrapper.h
SFMLMusicWrapper.cpp
)

# Resource management files filter
SOURCE_GROUP(Resources FILES
SoundData.h
SoundData.cpp
SFMLCommonSoundData.h
SFMLCommonSoundData.cpp
SFMLMusicData.h
SFMLMusicData.cpp
SFMLSoundData.h
SFMLSoundData.cpp
)

add_library( SoundEngine_SFML STATIC ${sources} ${headers} ${interfaces} )
target_link_libraries(SoundEngine_SFML PlatformLibrary SupportLibrary ${EXC_LINK_LIBRARY_SFML})