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

struct poly_base
{
    poly_base() {
        ctor_type = 1;
    }
    poly_base(const poly_base& other) 
    {
        ctor_type = 2;
        val = other.val;
    }
    poly_base(poly_base&& other) 
    {
        ctor_type = 3;
        val = other.val;
    }
    poly_base(int val) : val(val) {
        ctor_type = 4;
    }

    int ctor_type = 0;
    int val{};

    RTTR_ENABLE();
};

struct poly_sub: public poly_base
{
    poly_sub() {
        ctor_type = 101;
    }
    poly_sub(const poly_sub& other) 
    {
        ctor_type = 102;
        val = other.val;
        sub_val = other.sub_val;
    }
    poly_sub(poly_sub&& other) 
    {
        ctor_type = 103;
        val = other.val;
        sub_val = other.sub_val;
    }
    poly_sub(int val) : poly_base(val) {
        ctor_type = 104;
    }
    poly_sub(int val, int sub_val) : poly_base(val), sub_val(sub_val) {
        ctor_type = 105;
    }

    int sub_val{};

    RTTR_ENABLE(poly_base);
};

RTTR_REGISTRATION
{
    registration::class_<poly_base>("poly_base")
            .constructor()
            .constructor<int>()
            .property("val", &poly_base::val);

    registration::class_<poly_sub>("poly_sub")
            .constructor()
            .constructor<int>()
            .constructor<int, int>()
            .property("sub_val", &poly_sub::sub_val);

    type::register_wrapper_converter_for_base_classes<std::shared_ptr<poly_sub>>();
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("basic assign", "[polymoph]")
{

    SECTION("polymoph_ptr construct")
    {
        rttr::polymoph_ptr<poly_base> a = rttr::polymoph_ptr<poly_base>();
        CHECK(a.is_valid() == false);
        CHECK(a.get_real_type().is_valid() == false);
        CHECK(a.get() == nullptr);

        poly_base base1(1);
        auto b = rttr::polymoph_ptr<poly_base>(base1);
        CHECK(b.is_valid());
        CHECK(b.get_real_type().get_name() == "poly_base");
        CHECK(b.get() != nullptr);
        CHECK(b.get()->ctor_type == 2);
        CHECK(b.get()->val == 1);

        auto c = rttr::polymoph_ptr<poly_base>(new poly_base(2));
        CHECK(c.is_valid());
        CHECK(c.get_real_type().get_name() == "poly_base");
        CHECK(c.get() != nullptr);
        CHECK(c.get()->ctor_type == 4);
        CHECK(c.get()->val == 2);

        auto d = rttr::polymoph_ptr<poly_base>(poly_base(3));
        CHECK(d.is_valid());
        CHECK(d.get_real_type().get_name() == "poly_base");
        CHECK(d.get() != nullptr);
        CHECK(d.get()->ctor_type == 3);
        CHECK(d.get()->val == 3);

        auto e = rttr::polymoph_ptr<poly_base>(std::make_shared<poly_base>(4));
        CHECK(e.is_valid());
        CHECK(e.get_real_type().get_name() == "poly_base");
        CHECK(e.get() != nullptr);
        CHECK(e.get()->ctor_type == 4);
        CHECK(e.get()->val == 4);

        //-------------------------------------

        poly_sub sub1{1001, 1002};
        auto s1 = rttr::polymoph_ptr<poly_base>(sub1);
        CHECK(s1.is_valid());
        CHECK(s1.get_real_type().get_name() == "poly_sub");
        CHECK(s1.get()->ctor_type == 102);
        CHECK(s1.get()->val == 1001);
        CHECK(s1.get<poly_sub>() != nullptr);
        CHECK(s1.get<poly_sub>()->sub_val == 1002);

        auto s2 = rttr::polymoph_ptr<poly_base>(new poly_sub{1003, 1004});
        CHECK(s2.is_valid());
        CHECK(s2.get_real_type().get_name() == "poly_sub");
        CHECK(s2.get()->ctor_type == 105);
        CHECK(s2.get()->val == 1003);
        CHECK(s2.get<poly_sub>() != nullptr);
        CHECK(s2.get<poly_sub>()->sub_val == 1004);

        auto s3 = rttr::polymoph_ptr<poly_base>(poly_sub{1005, 1006});
        CHECK(s3.is_valid());
        CHECK(s3.get_real_type().get_name() == "poly_sub");
        CHECK(s3.get()->ctor_type == 103);
        CHECK(s3.get()->val == 1005);
        CHECK(s3.get<poly_sub>() != nullptr);
        CHECK(s3.get<poly_sub>()->sub_val == 1006);

        auto s4 = rttr::polymoph_ptr<poly_base>(std::make_shared<poly_sub>(1007, 1008));
        CHECK(s4.is_valid());
        CHECK(s4.get_real_type().get_name() == "poly_sub");
        CHECK(s4.get()->ctor_type == 105);
        CHECK(s4.get()->val == 1007);
        CHECK(s4.get<poly_sub>() != nullptr);
        CHECK(s4.get<poly_sub>()->sub_val == 1008);

        auto s10 = rttr::polymoph_ptr<poly_base>::make("poly_sub", {201, 202});
        CHECK(s10.is_valid());
        CHECK(s10.get_real_type().get_name() == "poly_sub");
        if (s10) {
            CHECK(s10.get()->ctor_type == 105);
            CHECK(s10.get()->val == 201);
            CHECK(s10.get<poly_sub>() != nullptr);
            CHECK(s10.get<poly_sub>()->sub_val == 202);
        }

        rttr::polymoph_ptr<poly_base> s11 = s10;
        CHECK(s11.is_valid());
        CHECK(s11.get_real_type().get_name() == "poly_sub");
        if (s11) {
            CHECK(s11.get()->ctor_type == 105);
            CHECK(s11.get()->val == 201);
            CHECK(s11.get<poly_sub>() != nullptr);
            CHECK(s11.get<poly_sub>()->sub_val == 202);
        }

        //---------------------------------------------

        auto sub2 = poly_sub{1009, 1010};
        rttr::polymoph_ptr<poly_base> s21 = sub2;
        CHECK(s21.is_valid());
        CHECK(s21.get_real_type().get_name() == "poly_sub");
        if (s21) {
            CHECK(s21.get()->ctor_type == 102);
            CHECK(s21.get()->val == 1009);
            CHECK(s21.get<poly_sub>() != nullptr);
            CHECK(s21.get<poly_sub>()->sub_val == 1010);
        }

        rttr::polymoph_ptr<poly_base> s22 = new poly_sub{1011, 1012};
        CHECK(s22.is_valid());
        CHECK(s22.get_real_type().get_name() == "poly_sub");
        if (s22) {
            CHECK(s22.get()->ctor_type == 105);
            CHECK(s22.get()->val == 1011);
            CHECK(s22.get<poly_sub>() != nullptr);
            CHECK(s22.get<poly_sub>()->sub_val == 1012);
        }

        rttr::polymoph_ptr<poly_base> s23 = poly_sub{1013, 1014};
        CHECK(s23.is_valid());
        CHECK(s23.get_real_type().get_name() == "poly_sub");
        if (s23) {
            CHECK(s23.get()->ctor_type == 103);
            CHECK(s23.get()->val == 1013);
            CHECK(s23.get<poly_sub>() != nullptr);
            CHECK(s23.get<poly_sub>()->sub_val == 1014);
        }

        rttr::polymoph_ptr<poly_base> s24 = std::make_shared<poly_sub>(1015, 1016);
        CHECK(s24.is_valid());
        CHECK(s24.get_real_type().get_name() == "poly_sub");
        if (s24) {
            CHECK(s24.get()->ctor_type == 105);
            CHECK(s24.get()->val == 1015);
            CHECK(s24.get<poly_sub>() != nullptr);
            CHECK(s24.get<poly_sub>()->sub_val == 1016);
        }
    }

    SECTION("polymoph_ptr set/get")
    {
        //---------------------------------------------

        auto sub3 = poly_sub{3009, 3010};
        rttr::polymoph_ptr<poly_base> s31;
        s31 = sub3;
        CHECK(s31.is_valid());
        CHECK(s31.get_real_type().get_name() == "poly_sub");
        if (s31) {
            CHECK(s31.get()->ctor_type == 102);
            CHECK(s31.get()->val == 3009);
            CHECK(s31.get<poly_sub>() != nullptr);
            CHECK(s31.get<poly_sub>()->sub_val == 3010);
        }

        rttr::polymoph_ptr<poly_base> s32;
        s32 = new poly_sub{3011, 3012};
        CHECK(s32.is_valid());
        CHECK(s32.get_real_type().get_name() == "poly_sub");
        if (s32) {
            CHECK(s32.get()->ctor_type == 105);
            CHECK(s32.get()->val == 3011);
            CHECK(s32.get<poly_sub>() != nullptr);
            CHECK(s32.get<poly_sub>()->sub_val == 3012);
        }

        rttr::polymoph_ptr<poly_base> s33;
        s33 = poly_sub{3013, 3014};
        CHECK(s33.is_valid());
        CHECK(s33.get_real_type().get_name() == "poly_sub");
        if (s33) {
            CHECK(s33.get()->ctor_type == 103);
            CHECK(s33.get()->val == 3013);
            CHECK(s33.get<poly_sub>() != nullptr);
            CHECK(s33.get<poly_sub>()->sub_val == 3014);
        }

        rttr::polymoph_ptr<poly_base> s34;
        s34 = std::make_shared<poly_sub>(3015, 3016);
        CHECK(s34.is_valid());
        CHECK(s34.get_real_type().get_name() == "poly_sub");
        if (s34) {
            CHECK(s34.get()->ctor_type == 105);
            CHECK(s34.get()->val == 3015);
            CHECK(s34.get<poly_sub>() != nullptr);
            CHECK(s34.get<poly_sub>()->sub_val == 3016);
        }


        //---------------------------------------------

        auto sub4 = poly_sub{3009, 3010};
        rttr::polymoph_ptr<poly_base> s41;
        s41.set(sub4);
        CHECK(s41.is_valid());
        CHECK(s41.get_real_type().get_name() == "poly_sub");
        if (s41) {
            CHECK(s41.get()->ctor_type == 102);
            CHECK(s41.get()->val == 3009);
            CHECK(s41.get<poly_sub>() != nullptr);
            CHECK(s41.get<poly_sub>()->sub_val == 3010);
        }

        rttr::polymoph_ptr<poly_base> s42;
        s42.set(new poly_sub{3011, 3012});
        CHECK(s42.is_valid());
        CHECK(s42.get_real_type().get_name() == "poly_sub");
        if (s42) {
            CHECK(s42.get()->ctor_type == 105);
            CHECK(s42.get()->val == 3011);
            CHECK(s42.get<poly_sub>() != nullptr);
            CHECK(s42.get<poly_sub>()->sub_val == 3012);
        }

        rttr::polymoph_ptr<poly_base> s43;
        s43.set(poly_sub{3013, 3014});
        CHECK(s43.is_valid());
        CHECK(s43.get_real_type().get_name() == "poly_sub");
        if (s43) {
            CHECK(s43.get()->ctor_type == 103);
            CHECK(s43.get()->val == 3013);
            CHECK(s43.get<poly_sub>() != nullptr);
            CHECK(s43.get<poly_sub>()->sub_val == 3014);
        }

        rttr::polymoph_ptr<poly_base> s44;
        s44.set(std::make_shared<poly_sub>(3015, 3016));
        CHECK(s44.is_valid());
        CHECK(s44.get_real_type().get_name() == "poly_sub");
        if (s44) {
            CHECK(s44.get()->ctor_type == 105);
            CHECK(s44.get()->val == 3015);
            CHECK(s44.get<poly_sub>() != nullptr);
            CHECK(s44.get<poly_sub>()->sub_val == 3016);
        }

        //---------------------------------------------
        auto s51 = rttr::polymoph_ptr<poly_base>(poly_sub(3015, 3016));
        CHECK(s51.is_valid());
        CHECK(s51.get_real_type().get_name() == "poly_sub");
        variant v51 = s51.get_variant();
        CHECK(v51.is_valid());
        type t51 = v51.get_type();
        std::string t51_type_name = t51.get_name().to_string();
        CHECK(t51_type_name == "poly_sub*");
        CHECK(t51 == rttr::type::get<poly_sub*>());

        //---------------------------------------------
        auto s52 = rttr::polymoph_ptr<poly_base>();
        variant v52 = type::get<poly_sub>().create({3015, 3016});
        s52.set_variant(v52);
        CHECK(s52.is_valid());
        CHECK(s52.get_real_type().get_name() == "poly_sub");
        if (s52) {
            CHECK(s52.get()->ctor_type == 105);
            CHECK(s52.get()->val == 3015);
            CHECK(s52.get<poly_sub>() != nullptr);
            CHECK(s52.get<poly_sub>()->sub_val == 3016);
        }
    }

    SECTION("variant_polymoph_view check")
    {
        variant a = rttr::polymoph_ptr<poly_base>(poly_sub{11, 12});

        [[maybe_unused]] bool b1 = detail::is_polymoph_container<polymoph_ptr<poly_base>>::value;
        
        CHECK(a.get_type() == type::get<polymoph_ptr<poly_base>>());
        CHECK(a.is_polymoph_container() == true);

        variant_polymoph_view view = a.create_polymoph_view();
        CHECK(view.get_type() == type::get<rttr::polymoph_ptr<poly_base>>());

        CHECK(view.get_value_type().get_name() == "poly_base");
        CHECK(view.get_value_ptr_type().get_name() == "poly_base*");

        CHECK(view.get_real_type().get_name() == "poly_sub");
        CHECK(view.get_real_ptr_type().get_name() == "poly_sub*");

        rttr::type value_type = view.get_value_type();
        std::string value_type_name = value_type.get_name().to_string();
        CHECK(value_type_name == "poly_base");
        CHECK(value_type == type::get<poly_base>());

        rttr::type wrapped_type = view.get_value_ptr_type();
        std::string wrapped_type_name = wrapped_type.get_name().to_string();
        CHECK(wrapped_type_name == "poly_base*");
        CHECK(wrapped_type == type::get<poly_base*>());

        variant val = view.get_value();
        rttr::type val_type = val.get_type();
        std::string val_type_name = val_type.get_name().to_string();
        CHECK(val.get_type() == type::get<poly_sub*>());
        CHECK(val_type_name == "poly_sub*");

        variant val_get = view.get();
        rttr::type val_get_type = val_get.get_type();
        std::string val_get_type_name = val_get_type.get_name().to_string();
        CHECK(val_get.get_type() == type::get<poly_base*>());
        CHECK(val_get_type_name == "poly_base*");

        poly_sub* sub_val2 = val.get_value<poly_sub*>();
        CHECK(sub_val2 != nullptr);
        CHECK(sub_val2->ctor_type == 103);
        CHECK(sub_val2->val == 11);
        CHECK(sub_val2->sub_val == 12);
    }


}

/////////////////////////////////////////////////////////////////////////////////////////
