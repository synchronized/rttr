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

#include <catch/catch.hpp>

#include <rttr/type>
#include <rttr/registration>

using namespace rttr;

struct simple_type2
{
    simple_type2() {
        ctor_type = 1;
    }
    simple_type2(int val) : val(val) {
        ctor_type = 2;
    }
    simple_type2(const simple_type2& other) 
    {
        ctor_type = 3;
        moved = other.moved;
        moved_from = other.moved_from;
        val = other.val;
    }
    simple_type2(simple_type2&& other) 
    {
        ctor_type = 4;
        moved = true;
        val = other.val;
        other.moved_from = true;
    }

    int ctor_type = 0;
    bool moved = false;
    bool moved_from = false;
    int val{};
};

RTTR_REGISTRATION
{
    registration::class_<simple_type2>("simple_type2")
            .constructor()
            .constructor<int>()
            .property("val", &simple_type2::val);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("basic operator*()", "[variant]")
{
    SECTION("simple_type operator*()")
    {
        variant a = type::get<simple_type2>().create({10});
        CHECK(a.get_type() == type::get<std::shared_ptr<simple_type2>>());
        
        auto& ptr_a = a.get_value<std::shared_ptr<simple_type2>>();
        CHECK(ptr_a->val == 10);

        variant b = *a;
        CHECK(b.get_type() == type::get<simple_type2*>());
        CHECK(b.get_value<simple_type2*>()->val == 10);
        simple_type2* ptr_b = b.get_value<simple_type2*>();
        CHECK(ptr_b->val == 10);

        variant c = *b;
        CHECK(ptr_a->val == 10);
        CHECK(ptr_b->val == 10);

        CHECK(c.is_valid() == true);
        CHECK(c.get_type() == type::get<simple_type2>());

        simple_type2& d = c.get_value<simple_type2>();
        CHECK(d.val == 10);

        variant e = new simple_type2(13);
        auto e_type = e.get_type();
        CHECK(e_type == type::get<simple_type2*>());
        bool is_convert = e.convert(type::get<simple_type2>());
        CHECK(is_convert);
        auto e_type2 = e.get_type();
        CHECK(e_type2 == type::get<simple_type2>());
    }


    SECTION("base operator*()")
    {
        variant a = std::make_shared<int>(10);
        CHECK(a.get_type() == type::get<std::shared_ptr<int>>());
        
        auto& ptr_a = a.get_value<std::shared_ptr<int>>();
        CHECK(*ptr_a == 10);

        variant b = *a;
        CHECK(b.get_type() == type::get<int*>());
        CHECK(*(b.get_value<int*>()) == 10);
        int* ptr_b = b.get_value<int*>();
        CHECK(*ptr_b == 10);

        variant c = *b;
        CHECK(*ptr_a == 10);
        CHECK(*ptr_b == 10);

        int& d = c.get_value<int>();
        CHECK(c.is_valid() == true);
        CHECK(c.get_type() == type::get<int>());
        CHECK(d == 10);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
