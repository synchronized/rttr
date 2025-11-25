#ifndef RTTR_POLYMOPH_PTR_H_
#define RTTR_POLYMOPH_PTR_H_

#include "rttr/type.h"

#include <memory>
#include <vector>

namespace rttr
{

class variant;
class argument;
template<typename T>
class polymoph_ptr;

template<typename T>
class polymoph_ptr {

    template<typename U>
    friend class polymoph_ptr;

public:
    using value_type = detail::raw_type_t<T>;

    using is_polymoph_type = std::true_type;

    friend void swap(polymoph_ptr<T> &a, polymoph_ptr<T> &b) noexcept
    {
        using std::swap; // 这一步允许编译器基于ADL寻找合适的swap函数
        swap(a.m_real_type, b.m_real_type);
        swap(a.m_real_ptr_type, b.m_real_ptr_type);
        swap(a.m_value, b.m_value);
    }

public:
    polymoph_ptr() noexcept {}
    polymoph_ptr(const polymoph_ptr<T>&) noexcept = default;
    polymoph_ptr(polymoph_ptr<T>&& rhs) noexcept
    {
        swap(*this, rhs);
    };
    polymoph_ptr<T>& operator=(const polymoph_ptr<T>&) noexcept = default;
    polymoph_ptr<T>& operator=(polymoph_ptr<T>&& rhs) noexcept {
        if (this == std::addressof(rhs)) {
            return *this;
        }
        swap(*this, rhs);
        return *this;
    }

    polymoph_ptr(const value_type& value) {
        m_real_type = type::get<value_type>();
        m_real_ptr_type = type::get<value_type*>();
        if (m_real_type) {
            m_value = std::make_shared<value_type>(value);
        }
    }
    polymoph_ptr(value_type* value) {
        m_real_type = type::get<value_type>(); 
        m_real_ptr_type = type::get<value_type*>();
        if (m_real_type) {
            m_value.reset(value); 
        }
    }
    polymoph_ptr(value_type&& value) {
        m_real_type = type::get<value_type>(); 
        m_real_ptr_type = type::get<value_type*>();
        if (m_real_type) {
            m_value = std::make_shared<value_type>(std::forward<value_type>(value));
        }
    }
    polymoph_ptr(std::shared_ptr<value_type> value) {
        m_real_type = type::get<value_type>(); 
        m_real_ptr_type = type::get<value_type*>();
        if (m_real_type) {
            m_value = value;
        }
    }

    polymoph_ptr(type real_type, T* value) {
        set(real_type, value);
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    polymoph_ptr(const U& value) {
        m_real_type = type::get<U>(); 
        m_real_ptr_type = type::get<U*>();
        if (m_real_type) {
            m_value = std::reinterpret_pointer_cast<value_type>(std::make_shared<U>(value));
        }
    }
    template<typename U, typename TP = detail::enable_if_t<std::is_base_of<value_type, U>::value>>
    polymoph_ptr(U* value) { 
        m_real_type = type::get<U>(); 
        m_real_ptr_type = type::get<U*>();
        if (m_real_type) {
            m_value.reset(reinterpret_cast<value_type*>(value)); 
        }
    }
    template<typename U, typename TP = detail::enable_if_t<std::is_base_of<value_type, U>::value>>
    polymoph_ptr(U&& value) {
        m_real_type = type::get<U>(); 
        m_real_ptr_type = type::get<U*>();
        if (m_real_type) {
            m_value = std::reinterpret_pointer_cast<value_type>(std::make_shared<U>(std::forward<U>(value)));
        }
    }
    template<typename U, typename TP = detail::enable_if_t<std::is_base_of<value_type, U>::value>>
    polymoph_ptr(std::shared_ptr<U> value) {
        m_real_type = type::get<U>(); 
        m_real_ptr_type = type::get<U*>();
        if (m_real_type) {
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

    type get_value_type() const { return type::get<value_type>(); }
    type get_value_ptr_type() const { return type::get<value_type*>(); }
    type get_real_type() const { return m_real_type; }
    type get_real_ptr_type() const { return m_real_ptr_type; }

    bool operator==(const value_type* ptr) const { return (get() == ptr); }
    bool operator!=(const value_type* ptr) const { return !(get() == ptr); }
    bool operator==(const polymoph_ptr<value_type>& rhs_ptr) const { return (m_real_type == rhs_ptr.m_real_type && m_value == rhs_ptr.m_value); }
    bool operator!=(const polymoph_ptr<value_type>& rhs_ptr) const { return !(*this == rhs_ptr); }

    template<typename T1, typename TP = detail::enable_if_t<std::is_base_of<value_type, T1>::value>>
    T1* get() const {
        return reinterpret_cast<T1*>(get()); 
    }

    template<typename T1, typename TP = detail::enable_if_t<std::is_base_of<value_type, T1>::value>>
    explicit operator T1*() const { return get<T1>(); }

    template<typename T1>
    explicit operator polymoph_ptr<T1>() const { return polymoph_ptr<T1>(*this); }

    bool is_valid() const { return m_real_type.is_valid() && m_value ? true : false ; }
    operator bool() const { return is_valid(); }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    bool set(const U& value) {
        clear();
        m_real_type = type::get<U>(); 
        m_real_ptr_type = type::get<U*>();
        if (m_real_type) {
            m_value = std::reinterpret_pointer_cast<value_type>(std::make_shared<U>(value));
            return true;
        }
        return false;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    bool set(U* value) {
        clear();
        m_real_type = type::get<U>(); 
        m_real_ptr_type = type::get<U*>();
        if (m_real_type) {
            m_value.reset(reinterpret_cast<value_type*>(value)); 
            return true;
        }
        return false;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    bool set(U&& value) {
        clear();
        m_real_type = type::get<U>(); 
        m_real_ptr_type = type::get<U*>();
        if (m_real_type) {
            m_value = std::reinterpret_pointer_cast<value_type>(std::make_shared<U>(std::forward<U>(value)));
            return true;
        }
        return false;
    }

    template<typename U, 
             detail::enable_if_t<std::is_base_of<value_type, U>::value, bool> = true>
    bool set(std::shared_ptr<U> value) {
        clear();
        m_real_type = type::get<U>(); 
        m_real_ptr_type = type::get<U*>();
        if (m_real_type) {
            m_value = std::reinterpret_pointer_cast<value_type>(value);
            return true;
        }
        return false;
    }

    bool set(type real_type, T* value) {
        value_type* raw_value_ptr = static_cast<value_type*>(detail::raw_addressof(value));

        m_real_type = real_type.get_raw_type();
        m_real_ptr_type = m_real_type.get_add_ptr_type();
        if (m_real_ptr_type.is_valid() && raw_value_ptr != nullptr) {
            m_value.reset(raw_value_ptr); 
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
        m_real_type = detail::get_invalid_type(); 
        m_real_ptr_type = detail::get_invalid_type();
        m_value.reset(); 
    }
    bool empty() const { return is_valid(); }

    bool create(std::string type_name, std::vector<argument> args = std::vector<argument>());

    static polymoph_ptr<T> make(std::string type_name, std::vector<argument> args = std::vector<argument>());

private:
    type                        m_real_type { detail::get_invalid_type() };
    type                        m_real_ptr_type { detail::get_invalid_type() };
    std::shared_ptr<value_type> m_value;
};

} // end namespace rttr

#include "rttr/detail/impl/polymoph_ptr_impl.h"

#endif // RTTR_POLYMOPH_PTR_H_