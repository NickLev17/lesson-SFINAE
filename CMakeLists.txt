cmake_minimum_required(VERSION 3.12)


set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VERSION})



configure_file(version.h.in version.h)


 configure_file(Doxyfile.in
${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile.in COPYONLY)


add_executable(print_ip print_ip.cpp)



set_target_properties(print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)


target_include_directories(print_ip
    PRIVATE "${CMAKE_BINARY_DIR}"
)


if (MSVC)
    target_compile_options(print_ip PRIVATE /W4)
else()
    target_compile_options(print_ip PRIVATE -Wall -Wextra -pedantic) 
endif()

#option(BUILD_DOC "Build documentation" ON)

#find_package(Doxygen REQUIRED)

#if(DOXYGEN_FOUND AND BUILD_DOC)
    #set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    #set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    #configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    #add_custom_target(doc ALL
       # COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
       # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
       # COMMENT "Generating API documentation with Doxygen"
      # VERBATIM
    #)
#else()
 #  message(WARNING "Doxygen not found or BUILD_DOC disabled, documentation will not be generated")
#endif()


install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

