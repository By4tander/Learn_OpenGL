cmake_minimum_required(VERSION 3.16)
project(LearnOpenGL VERSION 1.0.1 DESCRIPTION "Learn OpenGL From Scratch" LANGUAGES CXX)

set(LEARN_OPENGL_DEPS_DIR deps)

# load dependencies
add_subdirectory("${LEARN_OPENGL_DEPS_DIR}/glfw3/" "${CMAKE_CURRENT_BINARY_DIR}/glfw3")
add_subdirectory("${LEARN_OPENGL_DEPS_DIR}/glad/" "${CMAKE_CURRENT_BINARY_DIR}/glad")
add_subdirectory("${LEARN_OPENGL_DEPS_DIR}/glm/" "${CMAKE_CURRENT_BINARY_DIR}/glm")
set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
set(ASSIMP_BUILD_PLY_IMPORTER TRUE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
set(ASSIMP_BUILD_GLTF_IMPORTER TRUE)
set(ASSIMP_BUILD_3DS_IMPORTER TRUE)
set(ASSIMP_BUILD_STL_IMPORTER TRUE)
set(ASSIMP_BUILD_BLEND_IMPORTER TRUE)
set(ASSIMP_BUILD_COLLADA_EXPORTER TRUE)
add_subdirectory("${LEARN_OPENGL_DEPS_DIR}/assimp/" "${CMAKE_CURRENT_BINARY_DIR}/assimp")
target_include_directories(glad PUBLIC ${LEARN_OPENGL_DEPS_DIR})
if (APPLE)
    target_compile_options(assimp PRIVATE -Wno-deprecated-declarations)
endif ()

add_subdirectory(chapter1)
add_subdirectory(chapter2)