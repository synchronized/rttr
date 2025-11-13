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

####################################################################################
# Search and install 3rd party libraries
#
####################################################################################

MESSAGE(STATUS ${LIBRARY_OUTPUT_DIRECTORY})
MESSAGE(STATUS "Finding 3rd party libs...")
MESSAGE(STATUS "===========================")

# Find json11
if(NOT TARGET json11)
    set(JSON11_DIR ${RTTR_3RD_PARTY_DIR}/json11)
    add_library(json11 ${JSON11_DIR}/json11.cpp)
    target_include_directories(json11 PUBLIC ${JSON11_DIR})
    set_target_properties(json11 PROPERTIES
        FOLDER "Rttr/3rdParty/json11"
        )
endif()

# Find Catch2
if(NOT TARGET Catch2::Catch2)
    set(CATCH_INCLUDE_DIR ${RTTR_3RD_PARTY_DIR}/catch-v3.11.0)
    add_library(Catch2 INTERFACE)
    add_library(Catch2::Catch2 ALIAS Catch2)
    set_target_properties(Catch2 PROPERTIES
        FOLDER "Rttr/3rdParty/Catch2"
        )
    target_include_directories(Catch2 INTERFACE ${CATCH_INCLUDE_DIR})
    add_library(Catch2WithMain STATIC ${CATCH_INCLUDE_DIR}/catch2/catch_amalgamated.cpp)
    add_library(Catch2::Catch2WithMain ALIAS Catch2WithMain)
    set_target_properties(Catch2WithMain PROPERTIES
        FOLDER "Rttr/3rdParty/Catch2"
        )
endif()

# Find chai script
if(NOT TARGET chaiscript)
    set(CHAISCRIPT_INCLUDE_DIR ${RTTR_3RD_PARTY_DIR}/chaiscript-develop)
    add_library(chaiscript INTERFACE)
    target_include_directories(chaiscript INTERFACE ${CHAISCRIPT_INCLUDE_DIR}/include)
    set_target_properties(chaiscript PROPERTIES
        FOLDER "Rttr/3rdParty/chaiscript"
        )
endif()

MESSAGE(STATUS "Finished finding 3rd party libs!")
