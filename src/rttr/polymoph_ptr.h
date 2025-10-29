#ifndef RTTR_POLYMOPH_PTR_H_
#define RTTR_POLYMOPH_PTR_H_

#include "rttr/type.h"

#include <memory>
#include <vector>

namespace rttr
{

namespace detail {

    template<typename T, typename = void>
    struct is_polymoph_ptr : std::false_type {};

    template<typename T>
    struct is_polymoph_ptr<T, std::void_t<typename T::is_polymoph_type>> : std::true_type {};

    template<typename T, typename Decayed = decay_except_array_t<T>>
    using decay_polymoph_ptr_t = enable_if_t<!is_polymoph_ptr<Decayed>::value, Decayed>;
}

class variant;
class argument;
template<typename T>
class polymoph_ptr;

template<typename T>
class polymoph_ptr {

    template<typename U>
    friend class polymoph_ptr;

public:
    using value_type = T;

    using is_polymoph_type = std::true_type;

public:
    polymoph_ptr() {}
    polymoph_ptr(const polymoph_ptr<value_type>&) = default;
    polymoph_ptr(polymoph_ptr<value_type>&&) = default;
    polymoph_ptr<value_type>& operator=(const polymoph_ptr<value_type>&) = default;
    polymoph_ptr<value_type>& operator=(polymoph_ptr<value_type>&&) = default;

    polymoph_ptr(const value_type& value) {
        m_type_name = type::get<value_type>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = std::make_shared<value_type>(value);
        }
    }
    polymoph_ptr(value_type* value) {
        m_type_name = type::get<value_type>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value.reset(value); 
        }
    }
    polymoph_ptr(value_type&& value) {
        m_type_name = type::get<value_type>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = std::make_shared<value_type>(std::forward<value_type>(value));
        }
    }
    polymoph_ptr(std::shared_ptr<value_type> value) {
        m_type_name = type::get<value_type>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = value;
        }
    }

    polymoph_ptr(std::string type_name, value_type* value) {
        m_type_name = type_name; 
        if (m_type_name.size() > 0) {
            m_value.reset(value); 
        }
    }
    polymoph_ptr(std::string type_name, std::shared_ptr<value_type> value) {
        m_type_name = type_name; 
        if (m_type_name.size() > 0) {
            m_value = value;
        }
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    polymoph_ptr(const U& value) {
        m_type_name = type::get<U>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = std::reinterpret_pointer_cast<value_type>(std::make_shared<U>(value));
        }
    }
    template<typename U, typename TP = detail::enable_if_t<std::is_base_of<value_type, U>::value>>
    polymoph_ptr(U* value) { 
        m_type_name = type::get<U>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value.reset(reinterpret_cast<value_type*>(value)); 
        }
    }
    template<typename U, typename TP = detail::enable_if_t<std::is_base_of<value_type, U>::value>>
    polymoph_ptr(U&& value) {
        m_type_name = type::get<U>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = std::reinterpret_pointer_cast<value_type>(std::make_shared<U>(std::forward<U>(value)));
        }
    }
    template<typename U, typename TP = detail::enable_if_t<std::is_base_of<value_type, U>::value>>
    polymoph_ptr(std::shared_ptr<U> value) {
        m_type_name = type::get<U>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = std::reinterpret_pointer_cast<value_type>(value);
        }
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    polymoph_ptr<value_type>& operator=(const U& value) {
        set<U>(value);
        return *this;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    polymoph_ptr<value_type>& operator=(U* value) {
        set<U>(value);
        return *this;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    polymoph_ptr<value_type>& operator=(U&& value) {
        set<U>(std::forward<U>(value));
        return *this;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    polymoph_ptr<value_type>& operator=(std::shared_ptr<U> value) {
        set<U>(value);
        return *this;
    }

    std::string get_type_name() const { return m_type_name; }
    void set_type_name(std::string name) { m_type_name = name; }

    bool operator==(const value_type* ptr) const { return (get() == ptr); }
    bool operator!=(const value_type* ptr) const { return !(get() == ptr); }
    bool operator==(const polymoph_ptr<value_type>& rhs_ptr) const { return (m_value == rhs_ptr.m_value); }
    bool operator!=(const polymoph_ptr<value_type>& rhs_ptr) const { return !(*this == rhs_ptr); }

    template<typename T1, typename TP = detail::enable_if_t<std::is_base_of<value_type, T1>::value>>
    T1* get() const {
        return reinterpret_cast<T1*>(get()); 
    }

    template<typename T1, typename TP = detail::enable_if_t<std::is_base_of<value_type, T1>::value>>
    explicit operator T1*() const { return get<T1>(); }

    template<typename T1>
    explicit operator polymoph_ptr<T1>() const { return polymoph_ptr<T1>(*this); }

    bool is_valid() const { return m_type_name.size() > 0 && m_value ? true : false ; }
    operator bool() const { return is_valid(); }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    bool set(const U& value) {
        clear();
        m_type_name = type::get<U>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = std::reinterpret_pointer_cast<value_type>(std::make_shared<U>(value));
            return true;
        }
        return false;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    bool set(U* value) {
        clear();
        m_type_name = type::get<U>().get_name().to_string();
        if (m_type_name.size() > 0) {
            m_value.reset(reinterpret_cast<value_type*>(value)); 
            return true;
        }
        return false;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    bool set(U&& value) {
        clear();
        m_type_name = type::get<U>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = std::reinterpret_pointer_cast<value_type>(std::make_shared<U>(std::forward<U>(value)));
            return true;
        }
        return false;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    bool set(std::shared_ptr<U> value) {
        clear();
        m_type_name = type::get<U>().get_name().to_string(); 
        if (m_type_name.size() > 0) {
            m_value = std::reinterpret_pointer_cast<value_type>(value);
            return true;
        }
        return false;
    }

    value_type* get() const { return m_value.get(); }

    value_type* operator->() const { return get(); }

    value_type& operator*() const { return *get(); }

    std::shared_ptr<value_type> get_shared_ptr() const { return m_value; }

    bool set_variant(variant var);
    variant get_variant() const;

    void clear() { 
        m_type_name = ""; 
        m_value.reset(); 
    }
    bool empty() const { return is_valid(); }

    bool create(std::string type_name, std::vector<argument> args = std::vector<argument>());

    static polymoph_ptr<T> make(std::string type_name, std::vector<argument> args = std::vector<argument>());

private:
    std::string                 m_type_name {""};
    std::shared_ptr<value_type> m_value;
};

} // end namespace rttr

#include "rttr/detail/impl/polymoph_ptr_impl.h"

#endif // RTTR_POLYMOPH_PTR_H_