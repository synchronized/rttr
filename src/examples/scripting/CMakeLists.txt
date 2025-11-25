####################################################################################
#                                                                                  #
#  Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           #
#                                                                                  #
#  This file is part of the examples of RTTR (Run Time Type Reflection)            #
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

set(TARGET_NAME rttr_scripting_example)

message(STATUS "Scanning "  ${TARGET_NAME} " module.")
message(STATUS "===========================")

file(GLOB_RECURSE SRC_FILES "*.cpp")

find_package(Threads REQUIRED)

add_executable(${TARGET_NAME} ${SRC_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ${RTTR_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})
target_link_libraries(${TARGET_NAME} PRIVATE RTTR::Core)
target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads)
target_link_libraries(${TARGET_NAME} PRIVATE chaiscript)

set_target_properties(${TARGET_NAME} PROPERTIES 
                      DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                      FOLDER "Examples"
                      INSTALL_RPATH "${RTTR_EXECUTABLE_INSTALL_RPATH}"
                      CXX_STANDARD ${MAX_CXX_STANDARD}
                      )

set_compiler_warnings(${TARGET_NAME})

if (USE_PCH)
    target_precompile_headers(${TARGET_NAME} PRIVATE "pch.h")
endif()

if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /bigobj) 
endif()

add_dependencies(rttr_examples ${TARGET_NAME})

message(STATUS "Scanning " ${TARGET_NAME} " module finished!")
message(STATUS "")
