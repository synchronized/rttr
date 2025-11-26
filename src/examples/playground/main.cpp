/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#include <iostream>
#include <type_traits>

#include <rttr/type>
using namespace rttr;

#define CLASS(CLASS_NAME) struct CLASS_NAME \
{ virtual ~CLASS_NAME() {} RTTR_ENABLE() virtual int getType() { return dummyIntValue; } int dummyIntValue = 0; };

#define CLASS_INHERIT(CLASS1, CLASS2) struct CLASS1 : CLASS2 \
{ virtual int getType() { return static_cast<int>(dummyDoubleValue); } RTTR_ENABLE(CLASS2) double dummyDoubleValue = 1; };

CLASS(ClassSingleBase)
CLASS_INHERIT(ClassSingle1A, ClassSingleBase)

int main(int /*argc*/, char** /*argv*/)
{
    std::cout << "platform \n";


    std::cout << "common_type<char, uint32_t>: "
              << typeid(std::common_type<char, uint32_t>::type).name() << std::endl;

    std::cout << "common_type<float, uint32_t>: "
              << typeid(std::common_type<float, uint32_t>::type).name() << std::endl;

    std::cout << "common_type<float, long long>: "
              << typeid(std::common_type<float, long long>::type).name() << std::endl;

    std::cout << "common_type<double, uint32_t>: "
              << typeid(std::common_type<double, uint32_t>::type).name() << std::endl;

    std::cout << "common_type<double, long long>: "
              << typeid(std::common_type<double, long long>::type).name() << std::endl;


    std::cout << "has_get_type_func: " << rttr::detail::has_get_type_func<ClassSingleBase*>::value << std::endl;

    std::cout << "has_get_type_func: " << typeid(rttr::detail::has_get_type_func<ClassSingleBase*>::type).name() << std::endl;

    return 0;
}

