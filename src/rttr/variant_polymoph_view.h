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

#ifndef RTTR_VARIANT_POLYMOPH_VIEW_H_
#define RTTR_VARIANT_POLYMOPH_VIEW_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/variant.h"
#include "rttr/detail/variant_polymoph_view/variant_polymoph_view_private.h"

#include <cstddef>
#include <vector>
#include <memory>

namespace rttr
{
    class type;
    class instance;
    class argument;

/*!
 * The \ref variant_polymoph_view describes a class that refers to an
 * <a target="_blank" href=https://en.wikipedia.org/wiki/Sequence_container_(C%2B%2B)>sequence container</a> (e.g: `std::vector`)
 * inside a \ref variant.
 * With an instance of that class you can set/get values of such container,
 * without having access to the type declaration of the type or it's elements.
 *
 * A \ref variant_polymoph_view can be created directly from a \ref variant with its member function \ref variant::create_polymoph_view() "create_polymoph_view()".
 * \remark The instance of an variant_polymoph_view is always valid as long as the referenced \ref variant is valid, otherwise accessing a variant_polymoph_view
 *         is undefined behaviour.
 *
 * Meta Information
 * ----------------
 *
 * RTTR recognize whether a type is an polymoph container or not with the help of the \ref polymoph_container_mapper class template.
 * This call can access different container types via one common interface.
 * At the moment there exist specializations for following types:
 * `std::vector<T>`, `std::array<T, std::size_t>`, `std::list<T>`, `std::deque<T>`, `std::initializer_list<T>` and raw arrays
 *
 * Copying and Assignment
 * ----------------------
 * A \ref variant_polymoph_view object can be copied and assigned,
 * however each copy will reference the data of same underlying \ref variant value.
 *
 * Typical Usage
 * ----------------------
 *
 * \code{.cpp}
 *  std::vector<int> my_vec = { 1, 2, 3, 4, 5};
 *  variant var = my_vec; // copies data into variant
 *  if (var.is_polymoph_container())
 *  {
 *      variant_polymoph_view view = var.create_polymoph_view();  // performs no copy of the underlying vector
 *      std::cout << view.get_size() << std::endl;  // prints: '5'
 *      for (const auto& item : view)
 *      {
 *          // remark that the value is stored inside a 'std::reference_wrapper', however there is an automatic conversion for wrapper classes implemented.
 *          std::cout << "data: " << item.to_string() << " ";
 *      }
 *  }
 * \endcode
 *
 * Output:
 * \code
 *  1 2 3 4 5
 * \endcode
 *
 * \see variant, type::is_polymoph_wapper()
 */
class RTTR_API variant_polymoph_view
{
    public:
        class const_iterator;

        /*!
         * \brief Constructs an invalid variant_polymoph_view object.
         *
         * \see is_valid()
         */
        variant_polymoph_view();

        /*!
         * \brief Constructs a copy of the given variant_polymoph_view \p other.
         */
        variant_polymoph_view(const variant_polymoph_view& other);

        /*!
         * \brief Destroys the variant_polymoph_view.
         *
         * \remark The underlying data is not destroyed.
         */
        ~variant_polymoph_view() RTTR_NOEXCEPT;

        /*!
         * \brief Assigns the value of the \a other variant_polymoph_view to this variant_polymoph_view.
         *
         * \return A reference to the variant_polymoph_view with the new data.
         */
        variant_polymoph_view& operator=(const variant_polymoph_view& other) RTTR_NOEXCEPT;

        /*!
         * \brief Returns true if this variant_polymoph_view is valid, that means the object is holding some data.
         *        When the variant_polymoph_view doesn't hold any data it will return false.
         *
         * \return True if this \ref variant_polymoph_view is valid, otherwise false.
         */
        bool is_valid() const RTTR_NOEXCEPT;

        /*!
         * \brief Convenience function to check if this \ref variant_polymoph_view is valid or not.
         *
         * \see is_valid()
         *
         * \return True if this \ref variant_polymoph_view is valid, otherwise false.
         */
        explicit operator bool() const RTTR_NOEXCEPT;

        /*!
         * \brief Swaps this variant_polymoph_view with the \a other variant_polymoph_view.
         */
        void swap(variant_polymoph_view& other) RTTR_NOEXCEPT;

        /*!
         * \brief Returns the \ref type object of this polymoph container.
         *
         * \remark When the view is not valid, this function will return an invalid type object.
         *
         * \return \ref type "Type" of the polymoph container.
         */
        type get_type() const RTTR_NOEXCEPT;

        /*!
         * \brief Returns the \ref type object from the value of this polymoph container.
         *
         * \remark When the view is not valid, this function will return an invalid type object.
         *
         * \return \ref type "Type" from the value of the polymoph container.
         */
        type get_value_type() const RTTR_NOEXCEPT;

        type get_wrapped_type() const RTTR_NOEXCEPT;

        /*!
         * \brief Checks if the container has no elements.
         *
         * \return `true` if container is empty, otherwise `false`.
         */
        bool is_empty() const RTTR_NOEXCEPT;

        /*!
         * \brief Removes all elements from the container.
         *
         * \remark Invalidates all references, pointers, or iterators referring to contained elements.
         */
        void clear();

        bool create(std::string type_name, std::vector<argument> args = {});

        /*!
         * \brief Set the content of the the argument \p arg
         *        into the underlying polymoph container.
         *
         * \return `true` if the value could be set, otherwise `false`.
         */
        bool set_value(variant arg);

        /*!
         * \brief Returns the current value.
         *
         * \return The data wrapped inside a `std::reference_wrapper<T>`.
         */
        variant get_value() const;

        variant get() const;

        std::string get_type_name() const;

    private:
        friend class variant;
        friend class argument;

        detail::variant_polymoph_view_private m_view;
};

} // end namespace rttr

#endif // RTTR_VARIANT_POLYMOPH_VIEW_H_
