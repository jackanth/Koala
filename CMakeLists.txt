# CMake file for building koala
#-------------------------------------------------------------------------------------------------------------------------------------------
# Set up
cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
cmake_policy(VERSION 3.5.1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

# - project name
project(koala VERSION 0.1.0 LANGUAGES C CXX)

# - build type
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Set build type") 
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo)
option(TARGET_CEREAL_SUPPORT "Whether to include definitions for enabling cereal in the target" OFF)
option(BUILD_TESTS "Whether to build tests" OFF)
option(USE_LIBUNWIND "Whether to use libunwind" ON)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

# - some settings
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})

set(EXPORT_TARGETS_NAME koala-targets)
set(EXPORT_TARGETS_FILE_NAME ${EXPORT_TARGETS_NAME}.cmake)
set(EXPORT_TARGETS_DIR ${LIB_INSTALL_DIR})
set(EXPORT_TARGETS_FILE_PATH ${EXPORT_TARGETS_DIR}/${EXPORT_TARGETS_FILE_NAME})

# - build the cmake configure file
include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)

set(CONFIG_FILE_BINARY_LOCATION ${PROJECT_BINARY_DIR}/cmake/koala-config.cmake)
set(CONFIG_VERSION_FILE_BINARY_LOCATION ${PROJECT_BINARY_DIR}/cmake/koala-config-version.cmake)
set(CONFIG_FILE_INSTALL_DESTINATION share/cmake/${PROJECT_NAME})

configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/koala-config.cmake.in ${CONFIG_FILE_BINARY_LOCATION}
                              INSTALL_DESTINATION ${CONFIG_FILE_INSTALL_DESTINATION}
                              PATH_VARS INCLUDE_INSTALL_DIR EXPORT_TARGETS_FILE_PATH)
                              
write_basic_package_version_file(${CONFIG_VERSION_FILE_BINARY_LOCATION} VERSION ${${PROJECT_NAME}_VERSION} COMPATIBILITY SameMajorVersion)

#-------------------------------------------------------------------------------------------------------------------------------------------
# Construct library

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
                           $<INSTALL_INTERFACE:include>)

# - link against thread library
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE ${CMAKE_THREAD_LIBS_INIT})

# - link against Cereal (header-only)
if(TARGET_CEREAL_SUPPORT)
    find_package(Cereal REQUIRED)
    target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE ${Cereal_INCLUDE_DIR})
    target_compile_definitions(${PROJECT_NAME} INTERFACE KOALA_ENABLE_CEREAL)
endif()

# - link against libunwind
if(USE_LIBUNWIND)
    target_compile_definitions(${PROJECT_NAME} INTERFACE USE_LIBUNWIND)
endif()

# - temporary link to stdc++fs
target_link_libraries(${PROJECT_NAME} INTERFACE stdc++fs)

option(BUILD_DOCUMENTATION "Build documentation for ${PROJECT_NAME}" OFF)
if(BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Install products

# - directories
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX} DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(DIRECTORY DESTINATION include DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(DIRECTORY DESTINATION share DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(DIRECTORY DESTINATION share/cmake DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(DIRECTORY DESTINATION share/cmake/${PROJECT_NAME} DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

if(BUILD_DOCUMENTATION)
    install(DIRECTORY DESTINATION share/doc DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    install(DIRECTORY DESTINATION share/doc/${PROJECT_NAME} DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()

install(DIRECTORY DESTINATION ${${PROJECT_NAME}_SETTINGS_FILE_DIR} DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# - files
install(TARGETS ${PROJECT_NAME} EXPORT ${EXPORT_TARGETS_NAME})
install(EXPORT ${EXPORT_TARGETS_NAME} DESTINATION ${EXPORT_TARGETS_DIR} FILE ${EXPORT_TARGETS_FILE_NAME})

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}" DESTINATION include FILES_MATCHING PATTERN "*" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(FILES ${CONFIG_FILE_BINARY_LOCATION} ${CONFIG_VERSION_FILE_BINARY_LOCATION} DESTINATION ${CONFIG_FILE_INSTALL_DESTINATION} RENAME PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
