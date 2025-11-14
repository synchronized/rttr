####################################################################################
#                                                                                  #
#  Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           #
#                                                                                  #
#  This file is part of RTTR (Run Time Type Reflection)                            #
#  License: MIT License                                                            #
#                                                                                  #
#  Permission is hereby granted, free of charge, to any person obtaining           #
#  a copy of this software and associated documentation files (the "Software"),    #
#  to deal in the Software without restriction, including without limitation       #
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,        #
#  and/or sell copies of the Software, and to permit persons to whom the           #
#  Software is furnished to do so, subject to the following conditions:            #
#                                                                                  #
#  The above copyright notice and this permission notice shall be included in      #
#  all copies or substantial portions of the Software.                             #
#                                                                                  #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      #
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        #
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     #
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          #
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   #
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   #
#  SOFTWARE.                                                                       #
#                                                                                  #
####################################################################################

set(TARGET_NAME_DEFAULT rttr_bench_variant)
set(TARGET_NAME_SHARED ${TARGET_NAME_DEFAULT})
set(TARGET_NAME ${TARGET_NAME_SHARED})

message(STATUS "Scanning "  ${TARGET_NAME_DEFAULT} " module.")
message(STATUS "===========================")

file(GLOB_RECURSE SRC_FILES "*.cpp")

add_executable(${TARGET_NAME} ${SRC_FILES})
target_include_directories(${TARGET_NAME} PUBLIC ${RTTR_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE RTTR::Core)
target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

set_target_properties(${TARGET_NAME} PROPERTIES 
                      DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                      FOLDER "Rttr/Benchmarks"
                      INSTALL_RPATH "${RTTR_EXECUTABLE_INSTALL_RPATH}"
                      CXX_STANDARD ${MAX_CXX_STANDARD})
set_compiler_warnings(${TARGET_NAME})

if (USE_PCH)
    target_precompile_headers(${TARGET_NAME} PRIVATE "pch.h")
endif()

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /Zm200 /bigobj)
endif()

install(TARGETS ${TARGET_NAME}
        RUNTIME       DESTINATION ${RTTR_RUNTIME_INSTALL_DIR}
        LIBRARY       DESTINATION ${RTTR_LIBRARY_INSTALL_DIR}
        ARCHIVE       DESTINATION ${RTTR_ARCHIVE_INSTALL_DIR}
        FRAMEWORK     DESTINATION ${RTTR_FRAMEWORK_INSTALL_DIR})

###############################################################################

if (BUILD_STATIC)
    set(TARGET_NAME bench_variant_lib)
    add_executable(${TARGET_NAME} ${SRC_FILES} ${HPP_FILES})
    target_include_directories(${TARGET_NAME} PUBLIC ${RTTR_SOURCE_DIR})
    target_link_libraries(${TARGET_NAME} PRIVATE RTTR::Core_Lib)
    target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

    set_target_properties(${TARGET_NAME} PROPERTIES 
                          DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                          FOLDER "Rttr/Benchmarks"
                          INSTALL_RPATH "${RTTR_EXECUTABLE_INSTALL_RPATH}"
                          CXX_STANDARD ${MAX_CXX_STANDARD})

    set_compiler_warnings(${TARGET_NAME})

    if (USE_PCH)
        target_precompile_headers(${TARGET_NAME} PRIVATE "pch.h")
    endif()

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /Zm200 /bigobj)
    endif()

    install(TARGETS ${TARGET_NAME}
            RUNTIME       DESTINATION ${RTTR_RUNTIME_INSTALL_DIR}
            LIBRARY       DESTINATION ${RTTR_LIBRARY_INSTALL_DIR}
            ARCHIVE       DESTINATION ${RTTR_ARCHIVE_INSTALL_DIR}
            FRAMEWORK     DESTINATION ${RTTR_FRAMEWORK_INSTALL_DIR})

endif()

###############################################################################

if (BUILD_WITH_STATIC_RUNTIME_LIBS)
    set(TARGET_NAME bench_variant_s)
    add_executable(${TARGET_NAME} ${SRC_FILES} ${HPP_FILES})
    target_include_directories(${TARGET_NAME} PUBLIC ${RTTR_SOURCE_DIR})
    target_link_libraries(${TARGET_NAME} PRIVATE RTTR::Core_STL)
    target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

    set_target_properties(${TARGET_NAME} PROPERTIES 
                          DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                          FOLDER "Rttr/Benchmarks"
                          INSTALL_RPATH "${RTTR_EXECUTABLE_INSTALL_RPATH}"
                          CXX_STANDARD ${MAX_CXX_STANDARD})

    set_compiler_warnings(${TARGET_NAME})

    if (USE_PCH)
        target_precompile_headers(${TARGET_NAME} PRIVATE "pch.h")
    endif()

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /Zm200 /bigobj)
    endif()

    if (MSVC)
        target_compile_options(${TARGET_NAME} PUBLIC "/MT$<$<CONFIG:Debug>:d>")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        target_link_options(${TARGET_NAME} PUBLIC ${GNU_STATIC_LINKER_FLAGS})
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_link_options(${TARGET_NAME} PUBLIC ${CLANG_STATIC_LINKER_FLAGS})
    else()
        message(SEND_ERROR "Do now know how to statically link against the standard library with this compiler.")
    endif()

    install(TARGETS ${TARGET_NAME}
            RUNTIME       DESTINATION ${RTTR_RUNTIME_INSTALL_DIR}
            LIBRARY       DESTINATION ${RTTR_LIBRARY_INSTALL_DIR}
            ARCHIVE       DESTINATION ${RTTR_ARCHIVE_INSTALL_DIR}
            FRAMEWORK     DESTINATION ${RTTR_FRAMEWORK_INSTALL_DIR})

    if (BUILD_STATIC)
        set(TARGET_NAME bench_variant_lib_s)
        add_executable(${TARGET_NAME} ${SRC_FILES} ${HPP_FILES})
        target_include_directories(${TARGET_NAME} PUBLIC ${RTTR_SOURCE_DIR})
        target_link_libraries(${TARGET_NAME} PRIVATE RTTR::Core_Lib_STL)
        target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

        set_target_properties(${TARGET_NAME} PROPERTIES 
                              DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                              FOLDER "Rttr/Benchmarks"
                              INSTALL_RPATH "${RTTR_EXECUTABLE_INSTALL_RPATH}"
                              CXX_STANDARD ${MAX_CXX_STANDARD})

        set_compiler_warnings(${TARGET_NAME})

        if (USE_PCH)
            target_precompile_headers(${TARGET_NAME} PRIVATE "pch.h")
        endif()

        if(MSVC)
            target_compile_options(${TARGET_NAME} PRIVATE /Zm200 /bigobj)
        endif()

        if (MSVC)
            target_compile_options(${TARGET_NAME} PUBLIC "/MT$<$<CONFIG:Debug>:d>")
        elseif(CMAKE_COMPILER_IS_GNUCXX)
            target_link_options(${TARGET_NAME} PUBLIC ${GNU_STATIC_LINKER_FLAGS})
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_link_options(${TARGET_NAME} PUBLIC ${CLANG_STATIC_LINKER_FLAGS})
        else()
            message(SEND_ERROR "Do now know how to statically link against the standard library with this compiler.")
        endif()

        install(TARGETS ${TARGET_NAME}
                RUNTIME       DESTINATION ${RTTR_RUNTIME_INSTALL_DIR}
                LIBRARY       DESTINATION ${RTTR_LIBRARY_INSTALL_DIR}
                ARCHIVE       DESTINATION ${RTTR_ARCHIVE_INSTALL_DIR}
                FRAMEWORK     DESTINATION ${RTTR_FRAMEWORK_INSTALL_DIR})

    endif()
endif()

message(STATUS "Scanning " ${TARGET_NAME_DEFAULT} " module finished!")
message(STATUS "")
