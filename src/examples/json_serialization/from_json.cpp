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

#include <json11.hpp>
#include <rttr/type>

#include "def_shape.h"

using namespace rttr;



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

namespace io
{

namespace detail
{

static bool from_json(variant& data, const json11::Json& jvalue);
static bool from_json_basic(variant& data, const json11::Json& jvalue);
static bool from_json_array(variant_sequential_view& view, const json11::Json& jvalue);
static bool from_json_map(variant_associative_view& view, const json11::Json& jvalue);
static bool from_json_class(instance data, const json11::Json& jvalue);

/////////////////////////////////////////////////////////////////////////////////////////

static bool is_basic_type(type raw_type) {
    return (raw_type.is_arithmetic() || raw_type.is_enumeration() || raw_type == type::get<std::string>());
}

bool from_json(variant& orig_data, const json11::Json& jvalue)
{
    type data_type = orig_data.get_type();
    variant data = data_type.is_wrapper()
            ? orig_data.extract_wrapped_ptr_value() : orig_data;
    variant& data_ref = data_type.is_wrapper()
            ? data : orig_data;
    data_type = data.get_type();
    type raw_type = data_type.get_raw_type();

    if (!orig_data || is_basic_type(raw_type)) {
        return from_json_basic(orig_data, jvalue);
    }

    if (raw_type.is_sequential_container()) {
        auto view = data.create_sequential_view();
        return from_json_array(view, jvalue);
    }

    if (raw_type.is_associative_container()) {
        auto view = data.create_associative_view();
        return from_json_map(view, jvalue);
    }

    if (raw_type.is_class()) {
        return from_json_class(data_ref, jvalue);
    }

    return false;
}


/////////////////////////////////////////////////////////////////////////////////////////

bool from_json_basic(variant& data, const json11::Json& jvalue)
{
    variant var;
    switch(jvalue.type()) 
    {
        case json11::Json::Type::NUL:
        {
            break;
        }
        case json11::Json::Type::BOOL:
        {
            bool bool_val = jvalue.bool_value();
            var = bool_val;
            break;
        }
        case json11::Json::Type::STRING:
        {
            std::string str_value = jvalue.string_value();
            var = str_value;
            break;
        }
        case json11::Json::Type::NUMBER:
        {
            double num_val = jvalue.number_value();
            var = num_val;
            break;
        }
        // we handle only the basic types here
        case json11::Json::Type::ARRAY:
        case json11::Json::Type::OBJECT:
        {
            break;
        }
    }

    if (!var) { return false; }

    type var_type = var.get_type();
    if (!data) {
        data = var;
        return true;
    }
    const type data_type = data.get_type();
    const type data_raw_type = data_type.get_raw_type();
    if (var_type == data_type) {
        data = var;
        return true;
    }
    if (!var.convert(data_raw_type)) {
        return false;
    }

    if (data_raw_type != data_type) {
        if (!var.convert(data_type.get_raw_type())) {
            return false;
        }
    }

    data = var;
    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////

bool from_json_array(variant_sequential_view& view, const json11::Json& jvalue)
{
    if (!jvalue.is_array()) { return false; }

    json11::Json::array jarray = jvalue.array_items();
    view.set_size(jarray.size());

    const type orig_value_type = view.get_value_type();
    for (size_t i = 0; i < jarray.size(); ++i)
    {
        variant view_item = view.get_value(i);
        auto& jitem = jarray[i];
        if (!from_json(view_item, jitem)) { continue; }

        if (!view_item.convert(orig_value_type)) { continue; }

        view.set_value(i, view_item);
    }
    return true;
}

bool from_json_map(variant_associative_view& view, const json11::Json& json_array_value)
{
    const json11::Json::array jarray = json_array_value.array_items();
    if (view.is_key_only_type()) 
    {
        const type orig_key_type = view.get_key_type();

        for (size_t i = 0; i < jarray.size(); ++i)
        {
            variant key = orig_key_type.create();
            if (!is_basic_type(orig_key_type.get_raw_type())) {
                if (!key) { continue; }
            }

            auto& jitem = jarray[i];
            if (!from_json(key, jitem)) { continue; }

            if (!key.convert(orig_key_type)) { continue; }

            view.insert(key);
        }
    } 
    else 
    {
        const type orig_key_type = view.get_key_type();
        const type orig_value_type = view.get_value_type();

        for (size_t i = 0; i < jarray.size(); ++i)
        {
            auto& jitem = jarray[i];
            if (!jitem.is_object()) { continue; }
            
            auto& jitem_key = jitem["key"];
            variant key = orig_key_type.create();
            if (!is_basic_type(orig_key_type.get_raw_type())) {
                if (!key) { continue; }
            }

            if (!from_json(key, jitem_key)) { continue; }

            if (!key.convert(orig_key_type)) { continue; }

            auto& jitem_value = jitem["key"];
            variant value = orig_value_type.create();
            if (!is_basic_type(orig_value_type.get_raw_type())) {
                if (!value) { continue; }
            }

            if (!from_json(value, jitem_value)) { continue; }

            if (!value.convert(orig_value_type)) { continue; }

            view.insert(key, value);
        }

    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool from_json_class(instance orig_data, const json11::Json& jobject)
{
    type data_type = orig_data.get_type();
    instance obj = data_type.get_raw_type().is_wrapper() 
            ? orig_data.get_wrapped_instance() : orig_data;

    type derived_type = obj.get_derived_type();

    const auto prop_list = derived_type.get_properties();
    const json11::Json::object json_object = jobject.object_items();
    for (auto prop : prop_list)
    {
        auto prop_name = prop.get_name().to_string();
        auto ret = json_object.find(prop_name);
        if (ret == json_object.end())
            continue;
        
        const type prop_type = prop.get_type();
        variant prop_value = prop.get_value(obj);
        auto& json_value = ret->second;
        if (!from_json(prop_value, json_value)) { continue; }

        if (!prop_value.convert(prop_type)) { continue; }

        prop.set_value(obj, prop_value);
    }
    return true;
}

} // end namespace anonymous

bool from_json(const std::string& json, rttr::variant& obj)
{

    // "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
    std::string err;
    json11::Json jvalue = json11::Json::parse(json, err);
    if (!err.empty()) {
        std::cout << "json::parse() err:" << err << std::endl;
        return false;
    }

    return detail::from_json(obj, jvalue);
}

bool from_json(const std::string& json, rttr::instance obj)
{

    // "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
    std::string err;
    json11::Json jvalue = json11::Json::parse(json, err);
    if (!err.empty()) {
        std::cout << "json::parse() err:" << err << std::endl;
        return false;
    }

    return detail::from_json_class(obj, jvalue);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace io
