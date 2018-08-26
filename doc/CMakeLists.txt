# CMake file for building koala documentation
#------------------------------------------------------------------------------------------------------------------------------------------

find_package(Doxygen)

if(DOXYGEN_FOUND)
    # build documentation out-of-source
    set(DOC_SRC_DIR "${PROJECT_SOURCE_DIR}/doc")
    set(DOC_BIN_DIR "${PROJECT_BINARY_DIR}/docbuild")
    set(DOC_PROJECT_NUMBER "v${${PROJECT_NAME}_VERSION}")
    set(DOC_PROJECT_BRIEF "A thread-safe header-only C++17 library for object lifetime and relationship management")
    set(DOC_PROJECT_LOGO "$ENV{KOALA_DIR}/logo/logo_40dpi.png")
    
    # directories to search for documentation
    set(DOX_INPUT "$ENV{KOALA_DIR}/include/koala")

    configure_file(${DOC_SRC_DIR}/Doxyfile.in ${DOC_BIN_DIR}/Doxyfile @ONLY)

    # add doc target
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${DOC_BIN_DIR}/Doxyfile WORKING_DIRECTORY ${DOC_BIN_DIR} COMMENT "Building documentation using Doxygen" VERBATIM)

else()
    message(STATUS "Doxygen not found -- BUILD_DOCUMENTATION set to OFF")
    set(BUILD_DOCUMENTATION OFF)
endif()

if(BUILD_DOCUMENTATION)
    # make sure doxygen is executed (make doc) before make install
    install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} doc)")

    install(DIRECTORY ${DOC_BIN_DIR}/html DESTINATION share/doc/${PROJECT_NAME})
    install(DIRECTORY ${DOC_BIN_DIR}/latex DESTINATION share/doc/${PROJECT_NAME})
    install(DIRECTORY ${DOC_BIN_DIR}/man DESTINATION share/doc/${PROJECT_NAME})
endif()
