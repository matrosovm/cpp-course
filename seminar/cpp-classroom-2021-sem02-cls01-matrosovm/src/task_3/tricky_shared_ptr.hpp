#pragma once

#include <memory>
#include <functional>


namespace cls01 {
    template<class ObjectType, class FieldType>
    std::shared_ptr<FieldType> field_restricted_shared_ptr(
            std::shared_ptr<ObjectType>& obj,
            std::function<FieldType*(ObjectType& obj)>& f) {
        if (obj != nullptr) {
            return std::shared_ptr<FieldType>(obj, f(*obj));
        }
        return std::shared_ptr<FieldType>();
    }
}