#ifndef RTTR_POLYMOPH_PTR_IMPL_H_
#define RTTR_POLYMOPH_PTR_IMPL_H_

#include "rttr/variant.h"

#include <memory>

namespace rttr
{

template<typename T>
polymoph_ptr<T> polymoph_ptr<T>::create(std::string type_name, std::vector<argument> args) {

    auto real_type = rttr::type::get_by_name(type_name);
    if (!real_type.is_valid() || !real_type.is_derived_from(rttr::type::get<T>())) {
        return {};
    }

    variant var = real_type.create(args);
    if (!var) {
        return {};
    }
    rttr::type val_type = var.get_type();
    if (var.convert(type::get<std::shared_ptr<value_type>>())) {
        auto var_ptr = var.get_value<std::shared_ptr<value_type>>();
        return polymoph_ptr<T>{type_name, var_ptr};
    }
    if (var.convert(type::get<value_type*>())) {
        return polymoph_ptr<T>{type_name, var.get_value<value_type*>()};
    }
    return {};
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

} // end namespace rttr

#endif // RTTR_POLYMOPH_PTR_IMPL_H_