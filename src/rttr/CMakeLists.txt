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

set(TARGET_NAME_DEFAULT rttr_core)
set(TARGET_NAME ${TARGET_NAME_DEFAULT})

message(STATUS "Scanning "  ${TARGET_NAME_DEFAULT} " module.")
message(STATUS "===========================")

generateLibraryVersionVariables(
    ${RTTR_VERSION_MAJOR} ${RTTR_VERSION_MINOR} ${RTTR_VERSION_PATCH}
    "RTTR" "Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>" "MIT License")

loadFolder("rttr" HPP_FILES SRC_FILES)

if (${BUILD_RTTR_DYNAMIC})
    add_library(${TARGET_NAME} SHARED ${SRC_FILES} ${HPP_FILES})
    add_library(RTTR::Core ALIAS ${TARGET_NAME})
    
    target_compile_definitions(${TARGET_NAME} PRIVATE RTTR_DLL_EXPORTS)
    target_compile_definitions(${TARGET_NAME} PUBLIC RTTR_DLL)

    set_target_properties(${TARGET_NAME} PROPERTIES
                          VERSION ${RTTR_VERSION} SOVERSION ${RTTR_VERSION}
                          EXPORT_NAME Core
                          DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                          CXX_STANDARD ${MAX_CXX_STANDARD}
                          CXX_VISIBILITY_PRESET hidden
                          VISIBILITY_INLINES_HIDDEN 1
                          FOLDER "Rttr/Core"
                          )
    
    target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})
                          
    target_include_directories(${TARGET_NAME} PUBLIC
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
                               $<INSTALL_INTERFACE:include>)

    set_compiler_warnings(${TARGET_NAME})

    if (USE_PCH)
        target_precompile_headers(${TARGET_NAME} PRIVATE "detail/base/pch.h")
    endif()

    target_compile_options(${TARGET_NAME} PUBLIC
            $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
            $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
            )
    
    if (BUILD_INSTALLER)
        install(TARGETS ${TARGET_NAME} EXPORT rttr_targets
                RUNTIME       DESTINATION ${RTTR_RUNTIME_INSTALL_DIR}
                LIBRARY       DESTINATION ${RTTR_LIBRARY_INSTALL_DIR}
                ARCHIVE       DESTINATION ${RTTR_ARCHIVE_INSTALL_DIR}
                FRAMEWORK     DESTINATION ${RTTR_FRAMEWORK_INSTALL_DIR})

        # install also pdb file
        install_pdb_files(${TARGET_NAME})
    endif()

endif()

if (BUILD_STATIC)
    set(TARGET_NAME rttr_core_lib)
    add_library(${TARGET_NAME} STATIC ${SRC_FILES} ${HPP_FILES})
    add_library(RTTR::Core_Lib ALIAS ${TARGET_NAME})

    set_target_properties(${TARGET_NAME} PROPERTIES
                          VERSION ${RTTR_VERSION} SOVERSION ${RTTR_VERSION}
                          EXPORT_NAME Core_Lib
                          CXX_STANDARD ${MAX_CXX_STANDARD}
                          DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                          FOLDER "Rttr/Core"
                          )
                          
    target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})
    
    target_include_directories(${TARGET_NAME} PUBLIC
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
                                $<INSTALL_INTERFACE:include>)
    
    set_compiler_warnings(${TARGET_NAME})

    if (USE_PCH)
        target_precompile_headers(${TARGET_NAME} PRIVATE "detail/base/pch.h")
    endif()

    target_compile_options(${TARGET_NAME} PUBLIC
            $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
            $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
            )

    if (BUILD_INSTALLER)
        install(TARGETS ${TARGET_NAME} EXPORT rttr_targets
                RUNTIME       DESTINATION ${RTTR_RUNTIME_INSTALL_DIR}
                LIBRARY       DESTINATION ${RTTR_LIBRARY_INSTALL_DIR}
                ARCHIVE       DESTINATION ${RTTR_ARCHIVE_INSTALL_DIR}
                FRAMEWORK     DESTINATION ${RTTR_FRAMEWORK_INSTALL_DIR})
    endif()
endif()

if (BUILD_WITH_STATIC_RUNTIME_LIBS)
    set(TARGET_NAME rttr_core_s)
    add_library(${TARGET_NAME} SHARED ${SRC_FILES} ${HPP_FILES})
    add_library(RTTR::Core_STL ALIAS ${TARGET_NAME})
    
    target_compile_definitions(${TARGET_NAME} PRIVATE RTTR_DLL_EXPORTS)
    target_compile_definitions(${TARGET_NAME} PUBLIC RTTR_DLL)

    target_include_directories(${TARGET_NAME} PUBLIC
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
                               $<INSTALL_INTERFACE:include>)

    target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})
                               
    set_target_properties(${TARGET_NAME} PROPERTIES
                          VERSION ${RTTR_VERSION} SOVERSION ${RTTR_VERSION}
                          EXPORT_NAME Core_STL
                          CXX_STANDARD ${MAX_CXX_STANDARD}
                          DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                          CXX_VISIBILITY_PRESET hidden
                          VISIBILITY_INLINES_HIDDEN 1
                          FOLDER "Rttr/Core"
                          )

    set_compiler_warnings(${TARGET_NAME})

    if (USE_PCH)
        target_precompile_headers(${TARGET_NAME} PRIVATE "detail/base/pch.h")
    endif()

    target_compile_options(${TARGET_NAME} PUBLIC
            $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
            $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
            )

    if (MSVC)
        target_compile_options(${TARGET_NAME} PUBLIC "/MT$<$<CONFIG:Debug>:d>")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        target_link_options(${TARGET_NAME} PUBLIC ${GNU_STATIC_LINKER_FLAGS})
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_link_options(${TARGET_NAME} PUBLIC ${CLANG_STATIC_LINKER_FLAGS})
    else()
        message(SEND_ERROR "Do not know how to statically link against the standard library with this compiler.")
    endif()

    if (BUILD_INSTALLER)
        install_pdb_files(${TARGET_NAME})
        install(TARGETS ${TARGET_NAME} EXPORT rttr_targets
                RUNTIME       DESTINATION ${RTTR_RUNTIME_INSTALL_DIR}
                LIBRARY       DESTINATION ${RTTR_LIBRARY_INSTALL_DIR}
                ARCHIVE       DESTINATION ${RTTR_ARCHIVE_INSTALL_DIR}
                FRAMEWORK     DESTINATION ${RTTR_FRAMEWORK_INSTALL_DIR})
    endif()
    
    if (BUILD_STATIC)
        set(TARGET_NAME rttr_core_lib_s)
        add_library(${TARGET_NAME} STATIC ${SRC_FILES} ${HPP_FILES})
        add_library(RTTR::Core_Lib_STL ALIAS ${TARGET_NAME})

        target_include_directories(${TARGET_NAME} PUBLIC
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
                                   $<INSTALL_INTERFACE:include>)

        set_target_properties(${TARGET_NAME} PROPERTIES
                              VERSION ${RTTR_VERSION} SOVERSION ${RTTR_VERSION}
                              EXPORT_NAME Core_Lib_STL
                              CXX_STANDARD ${MAX_CXX_STANDARD}
                              DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                              FOLDER "Rttr/Core"
                              )
                              
        target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})
            
        set_compiler_warnings(${TARGET_NAME})

        if (USE_PCH)
            target_precompile_headers(${TARGET_NAME} PRIVATE "detail/base/pch.h")
        endif()

        target_compile_options(${TARGET_NAME} PUBLIC
                $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
                $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
                )

        if (MSVC)
            target_compile_options(${TARGET_NAME} PUBLIC "/MT$<$<CONFIG:Debug>:d>")
        elseif(CMAKE_COMPILER_IS_GNUCXX)
            target_link_options(${TARGET_NAME} PUBLIC ${GNU_STATIC_LINKER_FLAGS})
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_link_options(${TARGET_NAME} PUBLIC ${CLANG_STATIC_LINKER_FLAGS})
        else()
            message(SEND_ERROR "Do not know how to statically link against the standard library with this compiler.")
        endif()
        
        if (BUILD_INSTALLER)
            install(TARGETS ${TARGET_NAME} EXPORT rttr_targets
                    RUNTIME       DESTINATION ${RTTR_RUNTIME_INSTALL_DIR}
                    LIBRARY       DESTINATION ${RTTR_LIBRARY_INSTALL_DIR}
                    ARCHIVE       DESTINATION ${RTTR_ARCHIVE_INSTALL_DIR}
                    FRAMEWORK     DESTINATION ${RTTR_FRAMEWORK_INSTALL_DIR})
        endif()

    endif()
endif()

if(BUILD_INSTALLER)
	install(EXPORT rttr_targets
		DESTINATION ${RTTR_CMAKE_CONFIG_INSTALL_DIR}
		NAMESPACE RTTR::
		FILE rttr-config.cmake)
endif()


message(STATUS "Scanning " ${TARGET_NAME_DEFAULT} " module finished!")
message(STATUS "")
