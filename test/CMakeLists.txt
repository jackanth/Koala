# CMake file for building koala-test
#------------------------------------------------------------------------------------------------------------------------------------------
# Compiler flags

# - set C++17 flag
if (NOT CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "-std=c++17")
endif()

include(CheckCXXCompilerFlag)
unset(COMPILER_SUPPORTS_CXX_FLAGS CACHE)
CHECK_CXX_COMPILER_FLAG(${CMAKE_CXX_FLAGS} COMPILER_SUPPORTS_CXX_FLAGS)

if(NOT COMPILER_SUPPORTS_CXX_FLAGS)
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support cxx flags ${CMAKE_CXX_FLAGS}")
endif()

if("${CMAKE_BUILD_TYPE} " STREQUAL "Debug ")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=4 -Wswitch-default -Wundef -Werror -Wconversion -O0 -ggdb ${CMAKE_CXX_FLAGS}")
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=4 -Wswitch-default -Wundef -Werror -Wconversion -O0 -ggdb ${CMAKE_CXX_FLAGS}")
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wno-maybe-uninitialized -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=4 -Wswitch-default -Wundef -Werror -Wconversion -O0 -ggdb -rdynamic ${CMAKE_CXX_FLAGS}")
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        message(FATAL_ERROR "Unsupported compiler: Intel")
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        message(FATAL_ERROR "Unsupported compiler: MSVC")
    endif()

elseif("${CMAKE_BUILD_TYPE} " STREQUAL "Release ")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=4 -Wswitch-default -Wundef -Werror -Wconversion -O3 ${CMAKE_CXX_FLAGS}")
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=4 -Wswitch-default -Wundef -Werror -Wconversion -O3 ${CMAKE_CXX_FLAGS}")
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wno-maybe-uninitialized -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=4 -Wswitch-default -Wundef -Werror -Wconversion -O3 ${CMAKE_CXX_FLAGS}")
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        message(FATAL_ERROR "Unsupported compiler: Intel")
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        message(FATAL_ERROR "Unsupported compiler: MSVC")
    endif()
endif()

#------------------------------------------------------------------------------------------------------------------------------------------
# Build products

# - start bringing all the include directories, compile definitions and libraries together
set(KOALA_TEST_LIBS stdc++fs)
set(KOALA_TEST_INCLUDE_DIRS ${KOALA_TEST_INCLUDE_DIRS} include ${PROJECT_BINARY_DIR} $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_INCLUDE_DIRECTORIES>)
set(KOALA_TEST_SYSTEM_INCLUDE_DIRS ${KOALA_TEST_SYSTEM_INCLUDE_DIRS} $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>)
set(KOALA_TEST_COMPILE_DEFINITIONS ${KOALA_TEST_COMPILE_DEFINITIONS} PROTOBUF_INLINE_NOT_IN_HEADERS=0 $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_COMPILE_DEFINITIONS>)

if("${CMAKE_BUILD_TYPE} " STREQUAL "Debug ")
    set(KOALA_TEST_COMPILE_DEFINITIONS ${KOALA_TEST_COMPILE_DEFINITIONS} KOALA_DEBUG)
endif()

# - link against threads
find_package(Threads)
set(KOALA_TEST_LIBS ${KOALA_TEST_LIBS} ${CMAKE_THREAD_LIBS_INIT})

# - link statically against libunwind
include(FindPkgConfig)
pkg_check_modules(UNWIND REQUIRED libunwind)
set(KOALA_TEST_COMPILE_OPTIONS ${KOALA_TEST_COMPILE_OPTIONS} ${UNWIND_CFLAGS_OTHER})

foreach(LIBNAME ${UNWIND_STATIC_LIBRARIES})
    find_library(${LIBNAME}_STATIC_LIB ${LIBNAME})
    set(KOALA_TEST_LIBS ${KOALA_TEST_LIBS} ${${LIBNAME}_STATIC_LIB})  
endforeach()

find_library(UNWIND_ARCH_LIBRARIES libunwind-x86_64.a)
set(KOALA_TEST_LIBS ${KOALA_TEST_LIBS} ${UNWIND_ARCH_LIBRARIES})

# - koala-test executable
file(GLOB_RECURSE KOALA_TEST_SRCS *.cxx)

set(KOALA_TEST_EXECUTABLE_NAME koala-test)
add_executable(${KOALA_TEST_EXECUTABLE_NAME} ${KOALA_TEST_SRCS})

target_include_directories(${KOALA_TEST_EXECUTABLE_NAME} PRIVATE ${KOALA_TEST_INCLUDE_DIRS})
target_include_directories(${KOALA_TEST_EXECUTABLE_NAME} SYSTEM PRIVATE ${JACAKL_TEST_SYSTEM_INCLUDE_DIRS} ${KOALA_TEST_EXECUTABLE_SYSTEM_INCLUDE_DIRS})
target_compile_definitions(${KOALA_TEST_EXECUTABLE_NAME} PRIVATE ${KOALA_TEST_COMPILE_DEFINITIONS} ${KOALA_TEST_EXECUTABLE_COMPILE_DEFINITIONS})
target_compile_options(${KOALA_TEST_EXECUTABLE_NAME} PRIVATE ${KOALA_TEST_COMPILE_OPTIONS})
target_link_libraries(${KOALA_TEST_EXECUTABLE_NAME} PRIVATE ${KOALA_TEST_LIBS})

#------------------------------------------------------------------------------------------------------------------------------------------
# Install products

# - directories
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX} DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(DIRECTORY DESTINATION bin DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# - executable
install(TARGETS ${KOALA_TEST_EXECUTABLE_NAME} DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
