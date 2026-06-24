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

#include <catch2/catch_all.hpp>

#include <rttr/type>

using namespace rttr;

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::extract_pointer_value() - nullptr", "[variant]")
{
    SECTION("empty type")
    {
        variant a;
        variant b = a.extract_pointer_value();

        CHECK(b.is_valid() == false);
    }

    SECTION("valid type - lhs")
    {
        int* i = nullptr;
        variant a = i;
        variant b = a.extract_pointer_value();

        CHECK(a.is_valid() == true);
        CHECK(b.is_valid() == false);
    }

    SECTION("nullptr type - lhs")
    {
        int* i = nullptr;
        variant a = i;
        variant b = nullptr;

        CHECK(a.is_valid() == true);
        CHECK(b.is_valid() == true);
        CHECK((a == variant()) == false);
        CHECK((b == variant()) == false);
    }

}

/////////////////////////////////////////////////////////////////////////////////////////
