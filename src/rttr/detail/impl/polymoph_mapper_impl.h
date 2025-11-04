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
    using value_type    = typename base_class::value_type;

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

    static bool create(void* container, std::string type_name, std::vector<argument> args)
    {
        return base_class::create(get_container(container), type_name, args);
    }

    /////////////////////////////////////////////////////////////////////////
    template<typename..., 
             typename C = ConstType, 
             enable_if_t<!std::is_const<C>::value, int> = 0>
    static bool set_value(void* container, variant value)
    {
        return base_class::set_value(get_container(container), value);
    }

    template<typename..., 
             typename C = ConstType, 
             enable_if_t<std::is_const<C>::value, int> = 0>
    static bool set_value(void* container, std::string type_name, argument& value)
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////

    static variant get_value(void* container)
    {
        return base_class::get_value(get_container(container));
    }

    template<typename..., 
             typename C = ConstType, 
             typename ReturnType = decltype(base_class::get(std::declval<C&>())),
             enable_if_t<!std::is_const<C>::value, int> = 0,
             enable_if_t<std::is_reference<ReturnType>::value, int> = 0,
             enable_if_t<!std::is_pointer<ReturnType>::value, int> = 0>
    static variant get(void* container)
    {
        return variant(std::ref(base_class::get(get_container(container))));
    }

    template<typename..., 
             typename C = ConstType, 
             typename ReturnType = decltype(base_class::get(std::declval<C&>())),
             enable_if_t<!std::is_const<C>::value, int> = 0,
             enable_if_t<!std::is_reference<ReturnType>::value, int> = 0,
             enable_if_t<std::is_pointer<ReturnType>::value, int> = 0>
    static variant get(void* container)
    {
        return variant(base_class::get(get_container(container)));
    }

    template<typename..., 
             typename C = ConstType, 
             typename ReturnType = decltype(base_class::get(std::declval<C&>())),
             enable_if_t<std::is_const<C>::value, int> = 0,
             enable_if_t<std::is_reference<ReturnType>::value, int> = 0,
             enable_if_t<!std::is_pointer<ReturnType>::value, int> = 0>
    static variant get(void* container)
    {
        return variant(std::cref(base_class::get(get_container(container))));
    }

    template<typename..., 
             typename C = ConstType, 
             typename ReturnType = decltype(base_class::get(std::declval<C&>())),
             enable_if_t<std::is_const<C>::value, int> = 0,
             enable_if_t<!std::is_reference<ReturnType>::value, int> = 0,
             enable_if_t<std::is_pointer<ReturnType>::value, int> = 0>
    static variant get(void* container)
    {
        return variant(base_class::get(get_container(container)));
    }

    static type get_value_type(void* container)
    {
        return base_class::get_value_type(get_container(container));
    }

    static type get_value_ptr_type(void* container)
    {
        return base_class::get_value_ptr_type(get_container(container));
    }

    static type get_real_type(void* container)
    {
        return base_class::get_real_type(get_container(container));
    }

    static type get_real_ptr_type(void* container)
    {
        return base_class::get_real_ptr_type(get_container(container));
    }
};

//////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct polymoph_container_dynamic
{
    using container_t   = T;
    using wrapped_type  = decltype(std::declval<T>().get());
    using value_type    = typename T::value_type;


    /////////////////////////////////////////////////////////////////////////////////////

    static void clear(container_t& container)
    {
        container.clear();
    }

    static bool is_empty(const container_t& container)
    {
        return container.empty();
    }

    static bool create(container_t& container, std::string type_name, std::vector<argument> args)
    {
        return container.create(type_name, args);
    }

    static bool create(const container_t& container, std::string type_name, std::vector<argument> args)
    {
        return false;
    }

    static bool set_value(container_t& container, variant val)
    {
        return container.set_variant(val);
    }

    static bool set_value(const container_t& container, variant val)
    {
        return false;
    }

    static variant get_value(container_t& container)
    {
        return container.get_variant();
    }

    static variant get_value(const container_t& container)
    {
        return container.get_variant();
    }

    static wrapped_type get(container_t& container)
    {
        return container.get();
    }

    static const wrapped_type get(const container_t& container)
    {
        return container.get();
    }

    static type get_value_type(const container_t& container)
    {
        return container.get_value_type();
    }

    static type get_value_ptr_type(const container_t& container)
    {
        return container.get_value_ptr_type();
    }

    static type get_real_type(const container_t& container)
    {
        return container.get_real_type();
    }

    static type get_real_ptr_type(const container_t& container)
    {
        return container.get_real_ptr_type();
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

    static bool create(void* container, std::string type_name, std::vector<argument> value)
    {
        return false;
    }

    static bool set_value(void* container, variant value)
    {
        return false;
    }

    static variant get_value(void* container)
    {
        return variant();
    }

    static variant get(void* container)
    {
        return variant();
    }

    static type get_value_type(void* container)
    {
        return get_invalid_type();
    }
    static type get_value_ptr_type(void* container)
    {
        return get_invalid_type();
    }
    static type get_real_type(void* container)
    {
        return get_invalid_type();
    }
    static type get_real_ptr_type(void* container)
    {
        return get_invalid_type();
    }
};

//////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_POLYMOPH_MAPPER_IMPL_H_
