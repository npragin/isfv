cmake_minimum_required(VERSION 3.5)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(SciVis)

set(UI imgui)
set(SRC learnply)
set(SOURCES 
    ${SRC}/learnply.cpp
    ${SRC}/ply.cpp
    ${SRC}/polyhedron.cpp
    ${SRC}/ibfv.cpp
    ${SRC}/tmatrix.cpp
    ${SRC}/trackball.cpp
    ${SRC}/drawUtil.cpp
    ${SRC}/project1.cpp
    ${SRC}/project2.cpp
    ${SRC}/project3.cpp
)
set(HEADERS
    ${SRC}/ply.h
    ${SRC}/polyhedron.h
    ${SRC}/ibfv.h
    ${SRC}/tmatrix.h
    ${SRC}/trackball.h
    ${SRC}/drawPoly.h
    ${SRC}/drawUtil.h
    ${SRC}/project1.h
    ${SRC}/glError.h
    ${SRC}/icMatrix.h
    ${SRC}/icVector.h
    ${SRC}/ply_io.h
    ${SRC}/project2.h
    ${SRC}/project3.h
)

file(GLOB_RECURSE SRC_UI
    "${UI}/*.cpp"
    "${UI}/*.h"
)

add_executable(learnply ${SOURCES} ${HEADERS} ${SRC_UI})
source_group("ui"            FILES ${SRC_UI})

if(WIN32)
	set(FREEGLUT ${CMAKE_CURRENT_SOURCE_DIR}/libraries/freeglut-3.0.0)
	set(GLEW ${CMAKE_CURRENT_SOURCE_DIR}/libraries/glew-2.1.0)

	target_include_directories(learnply PRIVATE
		${FREEGLUT}/include
		${GLEW}/include
                ${UI}
	)

	target_link_directories(learnply PRIVATE
		${FREEGLUT}/lib
		${GLEW}/lib
	)

	target_link_libraries(learnply PRIVATE freeglut glew32)

    # Copy DLLs to build directory
    set(TO_COPY
        ${FREEGLUT}/bin/freeglut.dll
        ${GLEW}/bin/glew32.dll
    )

    add_custom_command(
        TARGET learnply POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TO_COPY}
        ${CMAKE_CURRENT_BINARY_DIR}
    )
else()
	find_package(OpenGL REQUIRED)
	find_package(GLUT REQUIRED)
	find_package(GLEW REQUIRED)

	target_include_directories(learnply PRIVATE
		${OPENGL_INCLUDE_DIRS}
                ${GLUT_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
                ${UI}
	)
    
    target_link_libraries(learnply PRIVATE
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
        GLEW::GLEW
    )

	# Silence OpenGL deprecation warnings on Mac
    if(APPLE)
        add_definitions(-DGL_SILENCE_DEPRECATION)
    endif()
endif()

