#ifndef RTTR_POLYMOPH_PTR_IMPL_H_
#define RTTR_POLYMOPH_PTR_IMPL_H_

#include "rttr/variant.h"

#include <memory>

namespace rttr
{

template<typename T>
bool polymoph_ptr<T>::set_variant(variant var) { 
    if (!var) return false;

    type var_type = var.get_type();
    if (!var_type) return false;

    if (var.convert(type::get<std::shared_ptr<value_type>>())) {
        clear();
        m_type_name = var_type.get_wrapped_type().get_raw_type().get_name().to_string();
        m_value = var.get_value<std::shared_ptr<value_type>>();
        return true;
    }
    return false;
}

template<typename T>
variant polymoph_ptr<T>::get_variant() const {
    if (!is_valid()) {
        return {};
    }
    const rttr::type real_type = type::get_by_name(m_type_name+"*");
    variant var = m_value.get();
    if (var.convert(real_type)) {
        return var;
    }
    return {};
}

template<typename T>
bool polymoph_ptr<T>::create(std::string type_name, std::vector<argument> args) {

    auto real_type = rttr::type::get_by_name(type_name);
    if (!real_type.is_valid() || !real_type.is_derived_from(rttr::type::get<T>())) {
        return false;
    }

    variant var = real_type.create(args);
    if (!var) {
        return false;
    }
    return set_variant(var);
}

template<typename T>
polymoph_ptr<T> polymoph_ptr<T>::make(std::string type_name, std::vector<argument> args) {

    polymoph_ptr<T> result;
    if (result.create(type_name, args)) {
        return result;
    }
    return {};
}

} // end namespace rttr

#endif // RTTR_POLYMOPH_PTR_IMPL_H_