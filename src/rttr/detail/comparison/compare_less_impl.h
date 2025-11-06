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

#ifndef RTTR_COMPARE_LESS_IMPL_H_
#define RTTR_COMPARE_LESS_IMPL_H_

#include "rttr/type.h"
#include "rttr/detail/comparison/compare_array_less.h"
#include <type_traits>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

// 定义标签
struct cmp_less_function_ptr_tag {};
struct cmp_less_array_tag {};
struct cmp_less_comparable_tag {};

template<typename T>
struct compare_trait {
    using type = conditional_t<
            is_function_ptr<T>::value, cmp_less_function_ptr_tag,
        conditional_t<
            std::is_array<T>::value, cmp_less_array_tag,
        conditional_t<
            is_less_than_comparable<T>::value, cmp_less_comparable_tag,
            void
            >
        >
    >;
};

template<typename T>
using compare_trait_t = typename compare_trait<T>::type;

template<typename T, typename Tp = void>
struct compare_less_than_impl {
    static bool cmp(const T& lhs, const T& rhs, int& result) {
        return compare_types_less_than(&lhs, &rhs, type::get<T>(), result);
    }
};

template<typename T>
struct compare_less_than_impl<T, cmp_less_function_ptr_tag>{
    static bool cmp(const T& lhs, const T& rhs, int& result) {
        uint64_t lhs_addr = (uint64_t)(void*)lhs;
        uint64_t rhs_addr = (uint64_t)(void*)rhs;
        result = (lhs_addr < rhs_addr ? - 1 : ((rhs_addr < lhs_addr) ? 1 : 0));
        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct compare_less_than_impl<T, cmp_less_array_tag>{
    static bool cmp(const T& lhs, const T& rhs, int& result)
    {
        result = compare_array_less(lhs, rhs) ? -1 : 1;
        return true;
    }
};

template<typename T>
struct compare_less_than_impl<T, cmp_less_comparable_tag>{
    static bool cmp(const T& lhs, const T& rhs, int& result)
    {
        result = (lhs < rhs ? - 1 : ((rhs < lhs) ? 1 : 0));
        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool compare_less_than(const T& lhs, const T& rhs, int& result)
{
    return compare_less_than_impl<T, compare_trait_t<T>>::cmp(lhs, rhs, result);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_COMPARE_LESS_IMPL_H_
