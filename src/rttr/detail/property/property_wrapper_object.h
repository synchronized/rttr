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

#ifndef RTTR_PROPERTY_WRAPPER_OBJECT_H_
#define RTTR_PROPERTY_WRAPPER_OBJECT_H_

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read write

template<typename Declaring_Typ, typename C, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<object_ptr, Declaring_Typ, C*, void, Acc_Level, return_as_copy, set_value, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using prop_type     = C;
    using policy_type   = prop_type;

    public:
        property_wrapper(string_view name,
                         C* pointer, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_accessor(pointer)
        {
            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()        const RTTR_NOEXCEPT { return true; }
        bool is_readonly()     const RTTR_NOEXCEPT { return false; }
        bool is_static()       const RTTR_NOEXCEPT { return true; }
        type get_type()        const RTTR_NOEXCEPT { return type::get<prop_type>(); }
        type get_policy_type() const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            if (arg.is_type<C>())
                return property_accessor<C>::set_value(*m_accessor, arg.get_value<C>());
            else
                return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(*m_accessor));
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, return_as_copy, C*>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker(obj));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read_only

template<typename Declaring_Typ, typename C, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<object_ptr, Declaring_Typ, C*, void, Acc_Level, return_as_copy, read_only, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using prop_type     = C;
    using policy_type   = prop_type;

    public:
        property_wrapper(string_view name,
                         C* pointer, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()        const RTTR_NOEXCEPT { return true; }
        bool is_readonly()     const RTTR_NOEXCEPT { return true; }
        bool is_static()       const RTTR_NOEXCEPT { return true; }
        type get_type()        const RTTR_NOEXCEPT { return type::get<prop_type>(); }
        type get_policy_type() const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(*m_accessor));
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, return_as_copy, C*>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker<read_only>(obj));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read write

template<typename Declaring_Typ, typename C, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<object_ptr, Declaring_Typ, C*, void, Acc_Level, return_as_ptr, set_as_ptr, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{

    using prop_type     = C;
    using policy_type   = add_pointer_t<prop_type>;

    public:
        property_wrapper(string_view name,
                         C* pointer, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
            static_assert(!std::is_pointer<C>::value, "The given type is already a pointer type!");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()        const RTTR_NOEXCEPT { return true; }
        bool is_readonly()     const RTTR_NOEXCEPT { return false; }
        bool is_static()       const RTTR_NOEXCEPT { return true; }
        type get_type()        const RTTR_NOEXCEPT { return type::get<prop_type>(); }
        type get_policy_type() const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            if (arg.is_type<C*>())
            {
                *m_accessor = *arg.get_value<C*>();
                return true;
            }
            else
            {
                return false;
            }
        }

        variant get_value(instance& object) const
        {
            return (variant(m_accessor));
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, return_as_ptr, C*>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker(obj));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read_only

template<typename Declaring_Typ, typename C, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<object_ptr, Declaring_Typ, C*, void, Acc_Level, return_as_ptr, read_only, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using prop_type     = add_const_t<C>;
    using policy_type   = add_pointer_t<prop_type>;

    public:
        property_wrapper(string_view name,
                         C* pointer, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()        const RTTR_NOEXCEPT { return true; }
        bool is_readonly()     const RTTR_NOEXCEPT { return true; }
        bool is_static()       const RTTR_NOEXCEPT { return true; }
        type get_type()        const RTTR_NOEXCEPT { return type::get<prop_type>(); }
        type get_policy_type() const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(m_accessor));
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, return_as_ptr, C*>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker<read_only>(obj));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read write

template<typename Declaring_Typ, typename C, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<object_ptr, Declaring_Typ, C*, void, Acc_Level, get_as_ref_wrapper, set_as_ref_wrapper, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using prop_type     = C;
    using policy_type   = std::reference_wrapper<prop_type>;

    public:
        property_wrapper(string_view name,
                         C* pointer, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
            static_assert(!std::is_pointer<C>::value, "The given type is already a pointer type!");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()        const RTTR_NOEXCEPT { return true; }
        bool is_readonly()     const RTTR_NOEXCEPT { return false; }
        bool is_static()       const RTTR_NOEXCEPT { return true; }
        type get_type()        const RTTR_NOEXCEPT { return type::get<prop_type>(); }
        type get_policy_type() const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            if (arg.is_type<std::reference_wrapper<C>>())
                return property_accessor<C>::set_value(*m_accessor, arg.get_value<std::reference_wrapper<C>>().get());
            else
                return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(std::ref(*m_accessor)));
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, get_as_ref_wrapper, C*>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker(obj));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read_only

template<typename Declaring_Typ, typename C, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<object_ptr, Declaring_Typ, C*, void, Acc_Level, get_as_ref_wrapper, read_only, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using prop_type     = add_const_t<C>;
    using policy_type   = std::reference_wrapper<prop_type> ;

    public:
        property_wrapper(string_view name,
                         C* pointer, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()        const RTTR_NOEXCEPT { return true; }
        bool is_readonly()     const RTTR_NOEXCEPT { return true; }
        bool is_static()       const RTTR_NOEXCEPT { return true; }
        type get_type()        const RTTR_NOEXCEPT { return type::get<prop_type>(); }
        type get_policy_type() const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(std::cref(*m_accessor)));
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, get_as_ref_wrapper, C*>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker<read_only>(obj));
        }

    private:
        C* m_accessor;
};


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read write

template<typename Declaring_Typ, typename C, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<object_ptr, Declaring_Typ, C*, void, Acc_Level, most_get_as_ptr, most_set_as_ptr, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using prop_type     = C;
    using policy_type   = add_pointer_t<prop_type>;

    using get_policy = most_get_as_ptr;;
    using set_policy = most_set_as_ptr;

    public:
        property_wrapper(string_view name,
                         C* pointer, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
            static_assert(!std::is_pointer<C>::value, "The given type is already a pointer type!");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()        const RTTR_NOEXCEPT { return true; }
        bool is_readonly()     const RTTR_NOEXCEPT { return false; }
        bool is_static()       const RTTR_NOEXCEPT { return true; }
        type get_type()        const RTTR_NOEXCEPT { return type::get<prop_type>(); }
        type get_policy_type() const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            //pointer
            if (arg.is_type<C*>()) {
                *m_accessor = *arg.get_value<C*>();
                return true;
            }
            //reference
            if (arg.is_type<std::reference_wrapper<C>>()) {
                *m_accessor = arg.get_value<std::reference_wrapper<C>>().get();
                return true;
            }
            //raw value
            if (arg.is_type<C>()) {
                *m_accessor = arg.get_value<C>();
                return true;
            }
            return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(m_accessor));
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, get_policy, C*>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker(obj));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read_only

template<typename Declaring_Typ, typename C, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<object_ptr, Declaring_Typ, C*, void, Acc_Level, most_get_as_ptr, read_only, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using prop_type     = add_const_t<C>;
    using policy_type   = add_pointer_t<prop_type>;

    using get_policy = most_get_as_ptr;;
    using set_policy = read_only;

    public:
        property_wrapper(string_view name,
                         C* pointer, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()        const RTTR_NOEXCEPT { return true; }
        bool is_readonly()     const RTTR_NOEXCEPT { return true; }
        bool is_static()       const RTTR_NOEXCEPT { return true; }
        type get_type()        const RTTR_NOEXCEPT { return type::get<prop_type>(); }
        type get_policy_type() const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(m_accessor));
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, get_policy, C*>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker<read_only>(obj));
        }

    private:
        C* m_accessor;
};

#endif // RTTR_PROPERTY_WRAPPER_OBJECT_H_
