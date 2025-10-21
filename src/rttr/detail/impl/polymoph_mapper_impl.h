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

#ifndef RTTR_POLYMOPH_MAPPER_IMPL_H_
#define RTTR_POLYMOPH_MAPPER_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/iterator_wrapper.h"

#include "rttr/variant.h"
#include "rttr/polymoph_ptr.h"

#include <type_traits>
#include <vector>
#include <list>
#include <deque>
#include <array>
#include <initializer_list>

namespace rttr
{
namespace detail
{


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename ConstType>
struct polymoph_container_mapper_wrapper 
{
    using base_class    = polymoph_container_mapper<T>;
    using value_t       = typename base_class::value_t;

    static ConstType& get_container(void* container)
    {
        return *reinterpret_cast<ConstType*>(container);
    }

    static bool is_empty(void* container)
    {
        return base_class::is_empty(get_container(container));
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename..., typename C = ConstType, enable_if_t<!std::is_const<C>::value, int> = 0>
    static void clear(void* container)
    {
        base_class::clear(get_container(container));
    }

    template<typename..., typename C = ConstType, enable_if_t<std::is_const<C>::value, int> = 0>
    static void clear(void* container)
    {
        // cannot clear a const container...
    }

    /////////////////////////////////////////////////////////////////////////
    template<typename..., 
             typename C = ConstType, 
             typename ReturnType = decltype(base_class::get_value(std::declval<C&>())),
             enable_if_t<!std::is_const<C>::value &&
                         !std::is_const<remove_reference_t<ReturnType>>::value, int> = 0>
    static bool set(void* container, std::string type_name, argument& value)
    {
        if (value.get_type() == ::rttr::type::get<value_t>())
        {
            value_t val = value.get_value<value_t>();
            return base_class::set(get_container(container), type_name, std::move(val));
        }
        else if (value.get_type() == ::rttr::type::get<value_t*>()) 
        {
            return base_class::set(get_container(container), type_name, value.get_value<value_t*>());
        }
        else
        {
            return false;
        }
    }

    template<typename..., 
             typename C = ConstType, 
             typename ReturnType = decltype(base_class::get_value(std::declval<C&>())),
             enable_if_t<std::is_const<C>::value ||
                         std::is_const<remove_reference_t<ReturnType>>::value, int> = 0>
    static bool set(void* container, std::string type_name, argument& value)
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename..., 
             typename C = ConstType, 
             typename ReturnType = decltype(base_class::get_value(std::declval<C&>())),
             enable_if_t<std::is_reference<ReturnType>::value, int> = 0,
             enable_if_t<!std::is_pointer<ReturnType>::value, int> = 0>
    static variant get_value(void* container)
    {
        return variant(std::ref(base_class::get_value(get_container(container))));
    }

    template<typename..., 
             typename C = ConstType, 
             typename ReturnType = decltype(base_class::get_value(std::declval<C&>())),
             enable_if_t<!std::is_reference<ReturnType>::value, int> = 0,
             enable_if_t<std::is_pointer<ReturnType>::value, int> = 0>
    static variant get_value(void* container)
    {
        return variant(static_cast<value_t*>(base_class::get_value(get_container(container))));
    }

    static std::string get_type_name(void* container)
    {
        return base_class::get_type_name(get_container(container));
    }
};

//////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct polymoph_container_dynamic
{
    using container_t   = T;
    using wrapped_type  = decltype(std::declval<T>().get());
    using value_t       = typename T::value_type;


    /////////////////////////////////////////////////////////////////////////////////////

    static void clear(container_t& container)
    {
        container.clear();
    }

    static bool is_empty(const container_t& container)
    {
        return container.empty();
    }

    static bool set(container_t& container, std::string type_name, value_t* val)
    {
        container.set(type_name, val);
        return true;
    }

    static bool set(const container_t& container, std::string type_name, value_t* val)
    {
        return false;
    }

    static bool set(container_t& container, std::string type_name, value_t&& val)
    {
        container.set(type_name, std::forward<value_t>(val));
        return true;
    }

    static bool set(const container_t& container, std::string type_name, value_t&& val)
    {
        return false;
    }

    static wrapped_type get_value(container_t& container)
    {
        return container.get();
    }

    static const wrapped_type get_value(const container_t& container)
    {
        return container.get();
    }

    static std::string get_type_name(const container_t& container)
    {
        return container.get_type_name();
    }
};

} // end namespace detail

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct polymoph_container_mapper<polymoph_ptr<T>> : detail::polymoph_container_dynamic<polymoph_ptr<T>> {};


//////////////////////////////////////////////////////////////////////////////////////

namespace detail
{

struct polymoph_container_empty
{

    static bool is_empty(void* container)
    {
        return true;
    }

    static void clear(void* container)
    {
    }

    static bool set(void* container, std::string type_name, argument& value)
    {
        return false;
    }

    static variant get_value(void* container)
    {
        return variant();
    }

    static std::string get_type_name(void* container)
    {
        return "";
    }
};

//////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_POLYMOPH_MAPPER_IMPL_H_
