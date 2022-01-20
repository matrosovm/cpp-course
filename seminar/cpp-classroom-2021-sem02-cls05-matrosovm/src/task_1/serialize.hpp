#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <boost/serialization/access.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>


namespace cls_05 {

    struct GeoPoint {
        double lat = 0;
        double lon = 0;

        bool operator==(const GeoPoint& other) const {
            return lat == other.lat && lon == other.lon;
        }

    private:
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int /* version */) {
            ar & lat;
            ar & lon;
        }
    };

    struct GeoLine {
        std::vector<GeoPoint> points{};

        bool operator==(const GeoLine& other) const {
            return points == other.points;
        }

    private:
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int /* version */) {
            ar & points;
        }
    };

    struct Attribute {
        std::string name;
        std::string value;

        bool operator==(const Attribute& other) const {
            return name == other.name && value == other.value;
        }

    private:
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int /* version */) {
            ar & name;
            ar & value;
        }
    };

    class Object {
    public:
        Object() = default;
        explicit Object(std::vector<Attribute> attributes);

        const std::vector<Attribute>& GetAttributes() const;
        std::vector<Attribute>& GetAttributes();

    private:
        std::vector<Attribute> attributes_;
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int /* version */) {
            ar & attributes_;
        }
    };

    class PointObject : public Object {
    public:
        PointObject() = default;
        PointObject(GeoPoint point, std::vector<Attribute> attributes);

        bool operator==(const PointObject& other) const {
            return point_ == other.point_ && GetAttributes() == other.GetAttributes();
        }

    private:
        GeoPoint point_;
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int /* version */) {
            ar & GetAttributes();
            ar & point_;
        }
    };

    class LineObject : public Object {
    public:
        LineObject() = default;
        LineObject(GeoLine line, std::vector<Attribute> attributes);

        bool operator==(const LineObject& other) const {
            return line_ == other.line_ && GetAttributes() == other.GetAttributes();
        }

    private:
        GeoLine line_;
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int /* version */) {
            ar & GetAttributes();
            ar & line_;
        }
    };

    template <class T>
    void load(T&& object, std::stringstream& oss) {
        boost::archive::text_oarchive oa(oss);
        oa << std::forward<T>(object);
    }

    template <class T>
    void upload(T& object, std::stringstream& iss) {
        boost::archive::text_iarchive ia(iss);
        ia >> object;
    }
}