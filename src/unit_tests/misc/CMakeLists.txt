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

set(TARGET_NAME rttr_unit_misc)

message(STATUS "Scanning "  ${TARGET_NAME} " module.")
message(STATUS "===========================")

file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(${TARGET_NAME} ${SRC_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ${RTTR_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} PRIVATE RTTR::Core)
target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(${TARGET_NAME} PRIVATE rttr_unit_test_base)
set_target_properties(${TARGET_NAME} PROPERTIES
                      DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                      FOLDER "Testing"
                      CXX_STANDARD ${MAX_CXX_STANDARD}
                      )

set_compiler_warnings(${TARGET_NAME})

if (USE_PCH)
    target_precompile_headers(${TARGET_NAME} PRIVATE "pch.h")
endif()

add_dependencies(${TARGET_NAME} rttr_unit_test_plugin)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:rttr_unit_test_plugin>"           # 源：DLL完整路径
    "$<TARGET_FILE_DIR:${TARGET_NAME}>"   # 目标：exe所在目录
    COMMENT "Copying rttr_unit_test_plugin DLL to executable directory"
)

# 为可执行文件设置 RPATH
set_target_properties(${TARGET_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    BUILD_WITH_INSTALL_RPATH TRUE  # 构建时也使用安装 RPATH，便于测试
)

add_dependencies(rttr_unit_test ${TARGET_NAME})

add_test(NAME ${TARGET_NAME}_test
         COMMAND ${TARGET_NAME}
         WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)

# 为测试添加标签
set_tests_properties(${TARGET_NAME}_test PROPERTIES
    LABELS "rttr_unit;catch2"
    TIMEOUT 120
)

message(STATUS "Scanning " ${TARGET_NAME} " module finished!")
message(STATUS "")