cmake_minimum_required( VERSION 3.2 )

project( AdvancedVisualization )

if( ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" )
    message( STATUS "Found Clang" )
    # no built in variable for Clang so make one
    set (CMAKE_COMPILER_IS_CLANG TRUE)

    if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang" )
        message( STATUS "Compiler: Apple Clang" )
        # no built in variable for Apple Clang so make one
        set (CMAKE_COMPILER_IS_APPLE_CLANG TRUE)
    endif()
endif()

if( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG )
    set( CMAKE_COMPILER_IS_GNUCXX_LIKE TRUE )
endif()

# determine bitness
set( IS_64_BIT FALSE )

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( IS_64_BIT TRUE )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

# determine processor architecture directory
if( WIN32 )
    # directory names for 32 and 64 bit
    if( IS_64_BIT )
        set( ARCH_DIR "x64" )
        message(FATAL_ERROR "Windows only suports 32 bit version of the project, run again cmake and select 32 bit platform.")
    else()
        set( ARCH_DIR "x86" )
    endif()
else()
    # on non windows systems use processor as architecture
    set( ARCH_DIR ${CMAKE_SYSTEM_PROCESSOR} )
endif()

message( STATUS "Using architecture ${ARCH_DIR}" )

# determine compiler directory
if( MSVC )
    set ( COMPILER_DIR "vc14" )
elseif(APPLE)
    set ( COMPILER_DIR "xcode" )
else()
    set ( COMPILER_DIR "gnu" )
endif()

message( STATUS "Using Compiler IDE: ${COMPILER_DIR}" )

# determine platform directory
if( MSVC )
  set ( PLATFORM_DIR "windows" )
elseif(APPLE)
    set ( PLATFORM_DIR "Apple" )
else()
  set ( PLATFORM_DIR "linux" )
endif()

message( STATUS "Using platform ${PLATFORM_DIR}" )

function( copy_resources targetName )
    # TODO copy only files for debug or release rather than all DLLs
    message( "${CMAKE_BUILD_TYPE}" )
    if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        message("debug mode")
    endif()

    set( COPY_RES )

    if( MSVC )
        # collect a list of all the DLLs to copy
        #

        file( GLOB files "3rdParty/freeglut/bin/${ARCH_DIR}/*.dll" )
        set( COPY_RES ${COPY_RES} ${files} )

        file( GLOB files "3rdParty/GLEW/bin/${ARCH_DIR}/*.dll" )
        set( COPY_RES ${COPY_RES} ${files} )

        file( GLOB files "3rdParty/SDL2/bin/${ARCH_DIR}/*.dll" )
        set( COPY_RES ${COPY_RES} ${files} )
    endif( MSVC )

    file( GLOB files "res/*.ASE" )
    set( COPY_RES ${COPY_RES} ${files} )
	
	file( GLOB files "res/*.obj" )
    set( COPY_RES ${COPY_RES} ${files} )

    file( GLOB files "res/*.bmp" )
    set( COPY_RES ${COPY_RES} ${files} )

    file( GLOB files "src/shaders/*.glsl" )
    set( COPY_RES ${COPY_RES} ${files} )

    foreach( RES_FILE ${COPY_RES} )
        add_custom_command( TARGET ${targetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${RES_FILE}" $<TARGET_FILE_DIR:${targetName}> )
    endforeach()
endfunction()


if( MSVC )
    message( STATUS "Adding freeglut includes ..." )

    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/freeglut/include )
    set( FREEGLUT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/freeglut/lib/${COMPILER_DIR}/${ARCH_DIR} )


    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLEW/include )
    set( GLEW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLEW/lib/${COMPILER_DIR}/${ARCH_DIR} )

    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SDL2/include )
    set( SDL2_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SDL2/lib/${COMPILER_DIR}/${ARCH_DIR} )

    message ( STATUS "Visual Studio compiler" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS" )
endif()

set(CMAKE_CXX_STANDARD 11)

set( ALL_FILES )



set( MAIN
    src/includes.h
    src/main.cpp
)
source_group( "Main" FILES ${MAIN} )
set( ALL_FILES ${ALL_FILES} ${MAIN} )

set( Framework
    src/Application.cpp
    src/Application.h
    src/Camera.cpp
    src/Camera.h
    src/Maths.cpp
    src/Maths.h
    src/Mesh.cpp
    src/Mesh.h
    src/Utils.cpp
    src/Utils.h
    src/Texture.cpp
    src/Texture.h
    src/Shader.h
    src/Shader.cpp
)
source_group( "Framework" FILES ${Framework} )
set( ALL_FILES ${ALL_FILES} ${Framework} )

set( Extra
    src/extra/TextParser.cpp
    src/extra/TextParser.h
    src/extra/ImageLoader.cpp
    src/extra/ImageLoader.h
)
source_group( "Framework\\Extra" FILES ${Extra} )
set( ALL_FILES ${ALL_FILES} ${Extra} )


set( Shaders
    src/shaders/Fragment_1.glsl
    src/shaders/Fragment_2.glsl
    src/shaders/Fragment_3.glsl
	src/shaders/Fragment_4.glsl
	src/shaders/Fragment_5.glsl
	src/shaders/Fragment_6.glsl
	src/shaders/Fragment_7.glsl
	src/shaders/Fragment_8.glsl
	src/shaders/Fragment_9.glsl
    src/shaders/Vertex_1.glsl
    src/shaders/Vertex_2.glsl
    src/shaders/Vertex_3.glsl
	src/shaders/Vertex_4.glsl
	src/shaders/Vertex_5.glsl
	src/shaders/Vertex_6.glsl
	src/shaders/Vertex_7.glsl
	src/shaders/Vertex_8.glsl
	src/shaders/Vertex_9.glsl
)
source_group( "Framework\\Shaders" FILES ${Shaders} )
set( ALL_FILES ${ALL_FILES} ${Shaders} )

set( Lighting
	src/Light.h
	src/Light.cpp
	src/Material.h
	src/Material.cpp
)
source_group( "Lighting" FILES ${Lighting} )
set( ALL_FILES ${ALL_FILES} ${Lighting} )


include_directories( src )
include_directories( src/extra )

set( ProjectName "Practice3")

message( STATUS "Creating Practice 3 project." )
add_executable( ${ProjectName} ${ALL_FILES} )

copy_resources(${ProjectName})

if( MSVC )
    target_link_libraries( ${ProjectName} opengl32 )
    target_link_libraries( ${ProjectName} glu32 )
    target_link_libraries( ${ProjectName} ${GLEW_LIB_DIR}/glew32s.lib )

    target_link_libraries( ${ProjectName} ${SDL2_LIB_DIR}/SDL2.lib )
    target_link_libraries( ${ProjectName} ${SDL2_LIB_DIR}/SDL2main.lib )
elseif( CMAKE_COMPILER_IS_GNUCXX_LIKE )
    find_package(OpenGL 3.2 REQUIRED)

    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(${ProjectName} ${OPENGL_LIBRARIES})

    message(STATUS "OPENGL_INCLUDE_DIR: ${OPENGL_INCLUDE_DIR}")
    message(STATUS "OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")

    find_package(GLUT REQUIRED)

    include_directories(${GLUT_INCLUDE_DIR})
    target_link_libraries(${ProjectName} ${GLUT_LIBRARY})

    message(STATUS "GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")
    message(STATUS "GLUT_LIBRARY: ${GLUT_LIBRARY}")

    if(APPLE)
        set(SDL2_INCLUDE_DIRS "/opt/local/include")
        set(SDL2_LIBRARIES "/opt/local/lib/libSDL2.dylib") 
    else()
        find_package(GLEW REQUIRED)

        include_directories(${GLEW_INCLUDE_DIR})
        target_link_libraries(${ProjectName} ${GLEW_LIBRARY})

        message(STATUS "GLEW_INCLUDE_DIR: ${GLEW_INCLUDE_DIR}")
        message(STATUS "GLEW_LIBRARY: ${GLEW_LIBRARY}")

        include(FindPkgConfig)
        PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    endif()

    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(${ProjectName} ${SDL2_LIBRARIES})

    message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
    message(STATUS "SDL2_LIBRARY: ${SDL2_LIBRARIES}")
endif()

