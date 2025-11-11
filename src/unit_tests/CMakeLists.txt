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

add_subdirectory(base_library)

add_subdirectory(plugin)

set(HPP_FILES "")
set(SRC_FILES "")

set(TARGET_NAME_DEFAULT rttr_unit_tests)
set(TARGET_NAME ${TARGET_NAME_DEFAULT})

message(STATUS "Scanning "  ${TARGET_NAME_DEFAULT} " module.")
message(STATUS "===========================")

loadFolder("unit_tests" HPP_FILES SRC_FILES)

if (USE_PCH)
  activate_precompiled_headers("pch.h" SRC_FILES)
endif()

add_executable(${TARGET_NAME} ${SRC_FILES} ${HPP_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE RTTR::Core)
target_link_libraries(${TARGET_NAME} PRIVATE rttr_unit_test_base)
target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

set_target_properties(${TARGET_NAME} PROPERTIES 
                      DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                      FOLDER "Rttr/Testing"
                      INSTALL_RPATH "${RTTR_EXECUTABLE_INSTALL_RPATH}"
                      CXX_STANDARD ${MAX_CXX_STANDARD})

set_compiler_warnings(${TARGET_NAME})

if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /bigobj)
endif()

add_dependencies(${TARGET_NAME} rttr_unit_test_plugin)

# run tests
#add_custom_target(run_tests ALL
#                  COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/lib "$<TARGET_FILE:unit_tests>"
#                  DEPENDS unit_tests
#                  COMMENT "Running unit_tests")

add_test(NAME unit_tests_all
         COMMAND ${TARGET_NAME}
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 为测试添加标签
set_tests_properties(unit_tests_all PROPERTIES 
    LABELS "rttr_unit;catch2"
    TIMEOUT 120
)


message(STATUS "Scanning " ${TARGET_NAME_DEFAULT} " module finished!")
message(STATUS "")