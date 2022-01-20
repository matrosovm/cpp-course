#include "serialize.hpp"

namespace cls_05 {

Object::Object(std::vector<Attribute> attributes)
    : attributes_(std::move(attributes)) {}

PointObject::PointObject(GeoPoint point, std::vector<Attribute> attributes)
    : Object(std::move(attributes)), point_(point) {}

LineObject::LineObject(GeoLine line, std::vector<Attribute> attributes)
    : Object(std::move(attributes)), line_(std::move(line)) {}

const std::vector<Attribute>& Object::GetAttributes() const {
  return attributes_;
}

std::vector<Attribute>& Object::GetAttributes() {
  return attributes_;
}

}// namespace cls_05