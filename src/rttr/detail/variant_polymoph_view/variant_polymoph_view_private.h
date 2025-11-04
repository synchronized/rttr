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

#ifndef RTTR_VARIANT_POLYMOPH_VIEW_PRIVATE_H_
#define RTTR_VARIANT_POLYMOPH_VIEW_PRIVATE_H_

#include "rttr/variant.h"
#include "rttr/argument.h"
#include "rttr/instance.h"
#include "rttr/polymoph_mapper.h"

namespace rttr
{

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

class RTTR_LOCAL variant_polymoph_view_private
{
    public:
        variant_polymoph_view_private() RTTR_NOEXCEPT :   
            m_type(get_invalid_type()),
            m_container(nullptr),
            m_get_is_empty_func(polymoph_container_empty::is_empty),
            m_clear_func(polymoph_container_empty::clear),
            m_create_func(polymoph_container_empty::create),
            m_set_value_func(polymoph_container_empty::set_value),
            m_get_value_func(polymoph_container_empty::get_value),
            m_get_func(polymoph_container_empty::get),
            m_get_value_type_func(polymoph_container_empty::get_value_type),
            m_get_value_ptr_type_func(polymoph_container_empty::get_value_ptr_type),
            m_get_real_type_func(polymoph_container_empty::get_real_type),
            m_get_real_ptr_type_func(polymoph_container_empty::get_real_ptr_type)
        {
        }

        template<typename T, typename RawType = raw_type_t<T>, typename ConstType = remove_pointer_t<T>>
        variant_polymoph_view_private(const T& container) RTTR_NOEXCEPT :  
            m_type(type::get<RawType>()),
            m_container(as_void_ptr(container)),
            m_get_is_empty_func(polymoph_container_mapper_wrapper<RawType, ConstType>::is_empty),
            m_clear_func(polymoph_container_mapper_wrapper<RawType, ConstType>::clear),
            m_create_func(polymoph_container_mapper_wrapper<RawType, ConstType>::create),
            m_set_value_func(polymoph_container_mapper_wrapper<RawType, ConstType>::set_value),
            m_get_value_func(polymoph_container_mapper_wrapper<RawType, ConstType>::get_value),
            m_get_func(polymoph_container_mapper_wrapper<RawType, ConstType>::get),
            m_get_value_type_func(polymoph_container_mapper_wrapper<RawType, ConstType>::get_value_type),
            m_get_value_ptr_type_func(polymoph_container_mapper_wrapper<RawType, ConstType>::get_value_ptr_type),
            m_get_real_type_func(polymoph_container_mapper_wrapper<RawType, ConstType>::get_real_type),
            m_get_real_ptr_type_func(polymoph_container_mapper_wrapper<RawType, ConstType>::get_real_ptr_type)
        {
        }

        RTTR_INLINE variant_polymoph_view_private(const variant_polymoph_view_private& other) = default;

        RTTR_INLINE ~variant_polymoph_view_private()
        {

        }

        RTTR_INLINE type get_type() const RTTR_NOEXCEPT
        {
            return m_type;
        }

        RTTR_INLINE bool is_empty() const RTTR_NOEXCEPT
        {
            return m_get_is_empty_func(m_container);
        }

        RTTR_INLINE void clear()
        {
            m_clear_func(m_container);
        }

        RTTR_INLINE bool create(std::string type_name, std::vector<argument> args)
        {
            return m_create_func(m_container, type_name, args);
        }

        RTTR_INLINE bool set_value(variant arg)
        {
            return m_set_value_func(m_container, arg);
        }

        RTTR_INLINE variant get_value() const
        {
            return m_get_value_func(m_container);
        }

        RTTR_INLINE variant get() const
        {
            return m_get_func(m_container);
        }

        RTTR_INLINE type get_value_type() const
        {
            return m_get_value_type_func(m_container);
        }

        RTTR_INLINE type get_value_ptr_type() const
        {
            return m_get_value_ptr_type_func(m_container);
        }

        RTTR_INLINE type get_real_type() const
        {
            return m_get_real_type_func(m_container);
        }

        RTTR_INLINE type get_real_ptr_type() const
        {
            return m_get_real_ptr_type_func(m_container);
        }

    private:

        using get_is_empty_func = bool(*)(void* container);
        using clear_func        = void(*)(void* container);
        using create_func       = bool(*)(void* container, std::string type_name, std::vector<argument> args);
        using set_value_func    = bool(*)(void* container, variant arg);
        using get_value_func    = variant(*)(void* container);
        using get_func          = variant(*)(void* container);
        using get_type_func     = type(*)(void* container);

        type                    m_type;
        void*                   m_container;
        get_is_empty_func       m_get_is_empty_func;
        clear_func              m_clear_func;
        create_func             m_create_func;
        set_value_func          m_set_value_func;
        get_value_func          m_get_value_func;
        get_func                m_get_func;
        get_type_func           m_get_value_type_func;
        get_type_func           m_get_value_ptr_type_func;
        get_type_func           m_get_real_type_func;
        get_type_func           m_get_real_ptr_type_func;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_POLYMOPH_VIEW_PRIVATE_H_
