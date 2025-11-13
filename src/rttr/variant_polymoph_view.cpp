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


#include "rttr/variant_polymoph_view.h"

#include "rttr/argument.h"
#include "rttr/instance.h"



namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////

variant_polymoph_view::variant_polymoph_view()
{
}

/////////////////////////////////////////////////////////////////////////////////

variant_polymoph_view::variant_polymoph_view(const variant_polymoph_view& other)
:   m_view(other.m_view)
{

}

/////////////////////////////////////////////////////////////////////////////////

variant_polymoph_view::~variant_polymoph_view() RTTR_NOEXCEPT
{
}

/////////////////////////////////////////////////////////////////////////////////

void variant_polymoph_view::swap(variant_polymoph_view& other) RTTR_NOEXCEPT
{
    std::swap(m_view, other.m_view);
}

/////////////////////////////////////////////////////////////////////////////////

variant_polymoph_view& variant_polymoph_view::operator=(const variant_polymoph_view& other) RTTR_NOEXCEPT
{
    variant_polymoph_view(other).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_polymoph_view::is_valid() const RTTR_NOEXCEPT
{
    return m_view.get_type().is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant_polymoph_view::operator bool() const RTTR_NOEXCEPT
{
    return m_view.get_type().is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

type variant_polymoph_view::get_type() const RTTR_NOEXCEPT
{
    return m_view.get_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type variant_polymoph_view::get_value_type() const RTTR_NOEXCEPT
{
    return m_view.get_value_type();
}

type variant_polymoph_view::get_value_ptr_type() const RTTR_NOEXCEPT
{
    return m_view.get_value_ptr_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type variant_polymoph_view::get_real_type() const RTTR_NOEXCEPT
{
    return m_view.get_real_type();
}

type variant_polymoph_view::get_real_ptr_type() const RTTR_NOEXCEPT
{
    return m_view.get_real_ptr_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_polymoph_view::is_empty() const RTTR_NOEXCEPT
{
    return m_view.is_empty();
}

/////////////////////////////////////////////////////////////////////////////////////////

void variant_polymoph_view::clear()
{
    m_view.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_polymoph_view::create(std::string type_name, std::vector<argument> args) {
    return m_view.create(type_name, args);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_polymoph_view::set_value(variant arg)
{
    return m_view.set_value(arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_polymoph_view::get_value() const
{
    return m_view.get_value();
}

variant variant_polymoph_view::get() const
{
    return m_view.get();
}

} // end namespace rttr

