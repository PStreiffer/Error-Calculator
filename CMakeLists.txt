cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


project(ErrorCalculator)


enable_language(C ASM)
enable_language(C CXX)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


add_executable(ErrorCalculator
${CMAKE_CURRENT_SOURCE_DIR}/Src/main.cpp
)

target_sources( ErrorCalculator PRIVATE

${CMAKE_CURRENT_SOURCE_DIR}/Src/ErrorClass.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Src/funcconstructors.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Src/FunctionExecute.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Src/FunctionParse.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Src/funcvarlist.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Src/mathfuncs.cpp

)

target_include_directories(ErrorCalculator PRIVATE

${CMAKE_CURRENT_SOURCE_DIR}/Inc

)