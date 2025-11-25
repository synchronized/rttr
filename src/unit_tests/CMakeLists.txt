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

add_custom_target(rttr_unit_test ALL
                  COMMENT "compile all unit tests")
                                    
set_target_properties(rttr_unit_test PROPERTIES 
                      FOLDER "Testing")

add_subdirectory(base_library)
add_subdirectory(constructor)
add_subdirectory(destructor)
add_subdirectory(enumeration)
add_subdirectory(instance)
add_subdirectory(method)
add_subdirectory(misc)
add_subdirectory(property)
add_subdirectory(type)
add_subdirectory(variant)
add_subdirectory(variant_associative_view)
add_subdirectory(variant_polymoph_view)
add_subdirectory(variant_sequential_view)
add_subdirectory(visitor)
add_subdirectory(plugin)