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

#include <cstdio>
#include <string>
#include <vector>
#include <array>

#include <iostream>

#define RAPIDJSON_HAS_STDSTRING 1
#include <json11.hpp>
#include <rttr/type>

using namespace rttr;

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

namespace io
{

namespace detail
{

static bool to_json(const variant& var, json11::Json& jvalue);
static bool to_json_basic(const variant& var, json11::Json& jvalue);
static bool to_json_array(const variant_sequential_view& var, json11::Json& jvalue);
static bool to_json_map(const variant_associative_view& var, json11::Json& jvalue);
static bool to_json_class(const instance var, json11::Json& jvalue);

/////////////////////////////////////////////////////////////////////////////////////////
bool to_json(const variant& orig_var, json11::Json& jvalue)
{
    auto value_type = orig_var.get_type();
    auto data = value_type.is_wrapper()
            ? orig_var.extract_wrapped_ref_value() : orig_var;
    value_type = data.get_type();
    auto raw_type = value_type.get_raw_type();

    if (raw_type.is_arithmetic() || raw_type.is_enumeration() || raw_type == type::get<std::string>())
    {
        return to_json_basic(data, jvalue);
    }
    else if (raw_type.is_sequential_container())
    {
        return to_json_array(data.create_sequential_view(), jvalue);
    }
    else if (raw_type.is_associative_container())
    {
        return to_json_map(data.create_associative_view(), jvalue);
    }
    else if (raw_type.is_class())
    {
        return to_json_class(data, jvalue);
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool to_json_basic(const variant& orig_var, json11::Json& jvalue)
{
    if (!orig_var) { return false; }

    auto value_type = orig_var.get_type();
    auto data = value_type.is_wrapper()
            ? orig_var.extract_wrapped_ptr_value() : orig_var;
    value_type = data.get_type();
    data = value_type.is_pointer() 
            ? data.extract_pointer_value() : data;
    value_type = data.get_type();

    if (value_type.is_arithmetic())
    {
        if (value_type == type::get<bool>()) 
        {
            jvalue = json11::Json(data.to_bool());
            return true;
        }
        else if (value_type == type::get<int8_t>() || 
                 value_type == type::get<int16_t>() ||
                 value_type == type::get<int32_t>() ||
                 value_type == type::get<uint8_t>() ||
                 value_type == type::get<uint16_t>() ||
                 value_type == type::get<uint32_t>())
        {
            jvalue = json11::Json(data.to_int());
            return true;
        }
        else if (value_type == type::get<int64_t>() || 
                 value_type == type::get<uint64_t>() ||
                 value_type == type::get<float>() ||
                 value_type == type::get<double>()) 
        {
            jvalue = json11::Json(data.to_double());
            return true;
        }
    }
    else if (value_type.is_enumeration())
    {
        bool ok = false;
        auto strval = data.to_string(&ok);
        if (ok) {
            jvalue = json11::Json(strval);
            return true;
        }
        auto intval = data.to_uint64(&ok);
        if (ok) {
            jvalue = json11::Json(static_cast<double>(intval));
            return true;
        }
    }
    else if (value_type == type::get<std::string>())
    {
        jvalue = json11::Json(data.to_string());
        return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool to_json_array(const variant_sequential_view& view, json11::Json& jarray_)
{
    auto jarray = json11::Json::array();
    jarray.reserve(view.get_size());

    for (const auto& item : view)
    {
        json11::Json& jitem = jarray.emplace_back();
        variant wrapped_var = item.get_type().is_wrapper() 
                ? item.extract_wrapped_ptr_value() : item;

        to_json(wrapped_var, jitem);
    }
    jarray_ = jarray;
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool to_json_map(const variant_associative_view& view, json11::Json& jmap_)
{
    static const std::string key_name("key");
    static const std::string value_name("value");

    json11::Json::array jmap;
    jmap.reserve(view.get_size());
    if (view.is_key_only_type())
    {
        for (auto& item : view)
        {
            json11::Json& jitem = jmap.emplace_back();
            to_json(item.first, jitem);
        }
    }
    else
    {
        for (auto& item : view)
        {
            json11::Json& jitem = jmap.emplace_back();
            json11::Json jitem_key;
            json11::Json jitem_value;

            to_json(item.first, jitem_key);
            to_json(item.second, jitem_value);

            jitem = json11::Json::object{
                std::make_pair(key_name, jitem_key),
                std::make_pair(value_name, jitem_value),
            };
        }
    }
    jmap_ = jmap;
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool to_json_class(const instance orig_var, json11::Json& jvalue_)
{
    auto jvalue = json11::Json::object();
    auto value_type = orig_var.get_type();
    instance data = value_type.get_raw_type().is_wrapper() 
            ? orig_var.get_wrapped_instance() : orig_var;
    value_type = data.get_type();
    type raw_type = value_type.get_raw_type();

    if (!raw_type.is_class()) { return false; }

    instance obj = data;

    type derived_type = obj.get_derived_type();
    auto prop_list = derived_type.get_properties();
    for (auto prop : prop_list)
    {
        if (prop.get_metadata("NO_SERIALIZE"))
            continue;

        variant prop_value = prop.get_value(obj);
        if (!prop_value)
            continue; // cannot serialize, because we cannot retrieve the value

        const auto name = prop.get_name().to_string();
        json11::Json jobj_val;
        if (to_json(prop_value, jobj_val)) 
        {
            jvalue[name] = jobj_val;
        }
    }
    jvalue_ = jvalue;
    return true;
}

} // end namespace anonymous

/////////////////////////////////////////////////////////////////////////////////////////

std::string to_json(rttr::variant& obj)
{
    if (!obj.is_valid())
        return std::string();

    json11::Json jvalue;
    detail::to_json(obj, jvalue);

    return jvalue.dump();
}

std::string to_json(rttr::instance obj)
{
    if (!obj.is_valid())
        return std::string();

    json11::Json jvalue;
    detail::to_json_class(obj, jvalue);

    return jvalue.dump();
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace io
