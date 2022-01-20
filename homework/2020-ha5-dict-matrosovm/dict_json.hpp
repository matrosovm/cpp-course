#pragma once

#include <sstream>

#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/rapidjson.h"

#include "dict.hpp"


namespace utils {

    namespace details {

        void serialize(const dict_t& dict, rapidjson::Writer<rapidjson::StringBuffer>& writer) {
            writer.StartObject();
            for (const auto& [key, value] : dict) {
                writer.Key(key.data());
                if (dict.is_dict(key)) {
                    serialize(std::any_cast<dict_t>(value.storage), writer);
                    continue;
                }
                auto& type = value.storage.type();
                if (type == typeid(bool)) {
                    writer.Bool(std::any_cast<bool>(value.storage));
                }  else if (type == typeid(unsigned short)) {
                    writer.Int(std::any_cast<unsigned short>(value.storage));
                } else if (type == typeid(short)) {
                    writer.Int(std::any_cast<short>(value.storage));
                } else if (type == typeid(int)) {
                    writer.Int(std::any_cast<int>(value.storage));
                } else if (type == typeid(unsigned int)) {
                    writer.Int(std::any_cast<unsigned int>(value.storage));
                } else if (type == typeid(long)) {
                    writer.Int(std::any_cast<long>(value.storage));
                } else if (type == typeid(unsigned long)) {
                    writer.Int(std::any_cast<unsigned long >(value.storage));
                } else if (type == typeid(float)) {
                    writer.Double(std::any_cast<float>(value.storage));
                } else if (type == typeid(double)) {
                    writer.Double(std::any_cast<double>(value.storage));
                } else if (type == typeid(std::string)) {
                    writer.String(std::any_cast<std::string>(value.storage).c_str());
                }
            }
            writer.EndObject();
        }

        bool deserialize(dict_t& dict, const auto& obj) {
            for (const auto& [key, value] : obj) {
                if (value.IsBool()) {
                    dict.put(key.GetString(), value.GetBool());
                } else if (value.IsInt()) {
                    dict.put(key.GetString(), value.GetInt());
                } else if (value.IsDouble()) {
                    dict.put(key.GetString(), value.GetDouble());
                } else if (value.IsString()) {
                    dict.put(key.GetString(), std::string(value.GetString()));
                } else if (value.IsObject()) {
                    dict_t tmp;
                    deserialize(tmp, value.GetObject());
                    dict.put(key.GetString(), tmp);
                } else {
                    return false;
                }
            }
            return true;
        }

    }

    void save_to_json(std::ostream& os, const dict_t& dict) {
        rapidjson::StringBuffer sb;
        rapidjson::Writer writer{sb};
        details::serialize(dict, writer);
        os << sb.GetString();
    }

    bool load_from_json(std::istream& is, dict_t& dict) {
        rapidjson::Document document;
        rapidjson::IStreamWrapper isw{is};
        document.ParseStream(isw);
        return document.IsObject() && details::deserialize(dict, document.GetObject());
    }

}