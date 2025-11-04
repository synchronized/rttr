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

#ifndef RTTR_POLYMOPH_MAPPER_H_
#define RTTR_POLYMOPH_MAPPER_H_

#include "rttr/detail/base/core_prerequisites.h"

namespace rttr
{

/*!
 * The \ref polymoph_container_mapper class is a class template to access an associative container via one common interface.
 *
 * This class will be only used internally by RTTR via the variant_associative_view class to get access to elements of an associative container.
 * In order to use your own custom associative container type, you have to provide a specialization of this class.
 *
 * Out of the box, RTTR has specialization for following associative container types:
 * - \p `std::set<Key>`
 * - \p `std::map<Key, T>`
 * - \p `std::multiset<Key>`
 * - \p `std::multimap<Key, T>`
 * - \p `std::unordered_set<Key>`
 * - \p `std::unordered_map<Key, T>`
 * - \p `std::unordered_multiset<Key>`
 * - \p `std::unordered_multimap<Key, T>`
 *
 * Custom associative container
 * -----------------------------
 * For a specialization of the class \ref rttr::polymoph_container_mapper<T> "polymoph_container_mapper<T>"
 * you have to provide some nested alias templates:
 * 1. `using container_t = T;`
 * 2. `using key_t       = typename T::key_type;`
 * 3. `using value_t     = typename T::mapped_type;` \remark When you have a key-only container, like `std::set<T>`, use `void`; i.e. `using value_t = void;`
 * 4. `using itr_t       = typename T::iterator;`
 * 5. `using const_itr_t = typename T::const_iterator;`
 *
 * and following member functions:
 * 1. `static const key_t& get_key(const const_itr_t& itr);`
 * 2. `static value_t& get_value(itr_t& itr);`
 * 3. `static const value_t& get_value(const const_itr_t& itr);`
 * 4. `static itr_t begin(container_t& container);`
 * 5. `static const_itr_t begin(const container_t& container);`
 * 6. `static const_itr_t end(const container_t& container);`
 * 7. `static itr_t find(container_t& container, const key_t& key);`
 * 8. `static const_itr_t find(const container_t& container, const key_t& key);`
 * 9. `static std::pair<itr_t, itr_t> equal_range(container_t& container, const key_t& key);`
 * 10. `static std::pair<const_itr_t, const_itr_t> equal_range(const container_t& container, const key_t& key);`
 * 11. `static void clear(container_t& container);`
 * 13. `static bool is_empty(const container_t& container);`
 * 14. `static std::size_t get_size(const container_t& container);`
 * 15. `static std::size_t erase(container_t& container, const key_t& key);`
 * 16. `static std::pair<itr_t, bool> insert_key(container_t& container, const key_t& key);`
 *     \remark This method needs to be implemented only when you have a key-only container.
 * 17. `static std::pair<itr_t, bool> insert_key_value(container_t& container, const key_t& key, const value_t& value);`
 *      \remark This method needs to be implemented only when you have a key-value container.
 *
 * Following code example for the associative container <a target="_blank" href=http://doc.qt.io/qt-5/qhash.html>QHash<K, T></a>
 * illustrates how to add a specialization:
 *
 * \code{.cpp}
 * namespace rttr
 * {
 * template<typename K, typename T>
 * struct polymoph_container_mapper<QHash<K, T>>
 * {
 *     using container_t   = QHash<K, T>;
 *     using key_t         = typename QHash<K, T>::key_type;
 *     using value_t       = typename QHash<K, T>::value_type;
 *
 *     static void clear(container_t& container)
 *     {
 *         container.clear();
 *     }
 * 
 *     static bool is_empty(const container_t& container)
 *     {
 *         return container.empty();
 *     }
 * 
 *     static bool create(container_t& container, std::string type_name, std::vector<argument> args)
 *     {
 *         return container.create(type_name, args);
 *     }
 * 
 *     static void set_value(container_t& container, variant val)
 *     {
 *         return container.set_variant(type_name, val);
 *     }
 * 
 *     static variant get_value(container_t& container)
 *     {
 *         return container.get_variant();
 *     }
 * 
 *     static wrapped_type get(container_t& container)
 *     {
 *         return container.get();
 *     }
 * 
 *     static const wrapped_type get(const container_t& container)
 *     {
 *         return container.get();
 *     }
 * 
 *     static std::string get_type_name(const container_t& container)
 *     {
 *         return container.get_type_name();
 *     }
 *
 * };
 * } // end namespace rttr
 * \endcode
 *
 * \remark
 * Make sure you put your specialization inside the namespace `rttr`.
 * The best place for this code, is below the declaration of your custom associative container type.
 * When this is not possible, include your specialization code before registering your types to RTTR.
 */
template<typename T>
struct polymoph_container_mapper
{
#ifndef DOXYGEN
    using is_valid      = std::false_type;
    using container_t   = T;
    using value_type    = detail::invalid_type;
#else
    using container_t = T;                          //!< An alias declaration to the container type itself.
    using value_type    = typename T::value_type;    //!< An alias to the value type.

    /////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief Removes all elements from the container.
     */
    static void clear(container_t& container)
    {
        container.clear();
    }

    /*!
     * \brief Returns the number of elements in the container.
     */
    static bool is_empty(const container_t& container)
    {
        return container.empty();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static bool create(container_t& container, std::string type_name, std::vector<argument> args)
    {
        return container.create(type_name, args);
    }

    static void set_value(container_t& container, variant val)
    {
        return container.set_variant(type_name, val);
    }

    static variant get_value(container_t& container)
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

    static std::string get_type_name(const container_t& container)
    {
        return container.get_type_name();
    }
#endif
};

} // end namespace rttr

#include "rttr/detail/impl/polymoph_mapper_impl.h"

#endif // RTTR_POLYMOPH_MAPPER_H_
