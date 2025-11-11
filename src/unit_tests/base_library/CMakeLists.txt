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

set(TARGET_NAME rttr_unit_test_base)

message(STATUS "Scanning "  ${TARGET_NAME} " module.")
message(STATUS "===========================")

set(HPP_FILES "")
set(SRC_FILES "")

loadFolder("files" HPP_FILES SRC_FILES)

if (USE_PCH)
  activate_precompiled_headers("pch.h" SRC_FILES)
endif()

add_library(${TARGET_NAME} SHARED ${SRC_FILES} ${HPP_FILES})

target_compile_definitions(${TARGET_NAME} PRIVATE RTTR_DLL_EXPORTS)
target_compile_definitions(${TARGET_NAME} PUBLIC RTTR_DLL)

target_link_libraries(${TARGET_NAME} RTTR::Core)
set_target_properties(${TARGET_NAME} PROPERTIES 
                      DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                      FOLDER "Rttr/Testing"
                      CXX_STANDARD ${MAX_CXX_STANDARD}
                      )

message(STATUS "Scanning " ${TARGET_NAME} " module finished!")
message(STATUS "")