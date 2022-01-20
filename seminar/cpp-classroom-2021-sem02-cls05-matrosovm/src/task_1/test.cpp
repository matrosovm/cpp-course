#include "serialize.hpp"

#include <gtest/gtest.h>


TEST(serialize, TestGeoPoint) {
    using cls_05::GeoPoint;
    {
        GeoPoint gp{1, 2};
        std::stringstream ssm;
        cls_05::load(gp, ssm);
        GeoPoint res;
        cls_05::upload(res, ssm);
        ASSERT_EQ(gp.lat, res.lat);
        ASSERT_EQ(gp.lon, res.lon);
    }
    {
        GeoPoint gp{1.000234, -2.1234123};
        std::stringstream ssm;
        cls_05::load(gp, ssm);
        GeoPoint res;
        cls_05::upload(res, ssm);
        ASSERT_EQ(gp.lat, res.lat);
        ASSERT_EQ(gp.lon, res.lon);
    }
}

TEST(serialize, TestGeoLine) {
    using cls_05::GeoLine;
    using cls_05::GeoPoint;
    {
        GeoLine gl{{GeoPoint{2, 3},
                    GeoPoint{1.234054123, 2.23745023485},
                    GeoPoint{-1.09887342541234123421, 2.09874305324}}};
        std::stringstream ssm;
        cls_05::load(gl, ssm);
        GeoLine res;
        cls_05::upload(res, ssm);
        ASSERT_EQ(gl.points.size(), res.points.size());
        for (size_t i = 0; i < gl.points.size(); ++i) {
            ASSERT_EQ(gl.points[i].lat, res.points[i].lat);
            ASSERT_EQ(gl.points[i].lon, res.points[i].lon);
        }
    }
    {
        std::vector<GeoPoint> filler(100);
        for (size_t i = 0; i < filler.size(); ++i) {
            filler[i] = {i - 0.0001, i + 0.0001};
        }
        GeoLine gl{filler};
        std::stringstream ssm;
        cls_05::load(gl, ssm);
        GeoLine res;
        cls_05::upload(res, ssm);
        ASSERT_EQ(gl.points.size(), res.points.size());
        for (size_t i = 0; i < gl.points.size(); ++i) {
            ASSERT_EQ(gl.points[i].lat, res.points[i].lat);
            ASSERT_EQ(gl.points[i].lon, res.points[i].lon);
        }
    }
}

TEST(serialize, TestAttribute) {
    using cls_05::Attribute;
    {
        Attribute attr{"vouiwhenorsa", "ajfmpochwwwcinufwo iuowc"};
        std::stringstream ssm;
        cls_05::load(attr, ssm);
        Attribute res;
        cls_05::upload(res, ssm);
        ASSERT_EQ(attr.name.size(), res.name.size());
        ASSERT_EQ(attr.value.size(), res.value.size());
    }
}

TEST(serialize, TestObject) {
    using cls_05::Attribute;
    using cls_05::Object;
    {
        std::vector<Attribute> filler(100);
        for (size_t i = 0; i < filler.size(); ++i) {
            filler[i] = {std::to_string(i - 0.0001), std::to_string(i + 0.001)};
        }
        Object obj{filler};
        std::stringstream ssm;
        cls_05::load(obj, ssm);
        Object res{{}};
        cls_05::upload(res, ssm);
        const std::vector<Attribute>& obj_data = obj.GetAttributes();
        const std::vector<Attribute>& res_data = res.GetAttributes();
        ASSERT_EQ(obj_data.size(), res_data.size());
        for (size_t i = 0; i < obj_data.size(); ++i) {
            ASSERT_EQ(obj_data[i].name, res_data[i].name);
            ASSERT_EQ(obj_data[i].value, res_data[i].value);
        }
    }
}

TEST(serialize, TestPointObject) {
    using cls_05::Attribute;
    using cls_05::PointObject;
    {
        std::vector<Attribute> filler(100);
        for (size_t i = 0; i < filler.size(); ++i) {
            filler[i] = {std::to_string(i - 0.0001), std::to_string(i + 0.001)};
        }
        PointObject pobj{{1, 2}, {filler}};
        std::stringstream ssm;
        cls_05::load(pobj, ssm);
        PointObject res{{}, {}};
        cls_05::upload(res, ssm);
        const std::vector<Attribute>& obj_data = pobj.GetAttributes();
        const std::vector<Attribute>& res_data = res.GetAttributes();
        ASSERT_EQ(obj_data.size(), res_data.size());
        for (size_t i = 0; i < obj_data.size(); ++i) {
            ASSERT_EQ(obj_data[i].name, res_data[i].name);
            ASSERT_EQ(obj_data[i].value, res_data[i].value);
        }
    }
}

TEST(serialize, TestLineObject) {
    using cls_05::Attribute;
    using cls_05::LineObject;
    using cls_05::GeoPoint;
    {
        std::vector<Attribute> filler_attr(100);
        for (size_t i = 0; i < filler_attr.size(); ++i) {
            filler_attr[i] = {std::to_string(i - 0.0001), std::to_string(i + 0.001)};
        }
        std::vector<GeoPoint> filler_line(100);
        for (size_t i = 0; i < filler_line.size(); ++i) {
            filler_line[i] = {i - 0.0001, i + 0.0001};
        }
        LineObject pobj{{filler_line}, {filler_attr}};
        std::stringstream ssm;
        cls_05::load(pobj, ssm);
        LineObject res{{}, {}};
        cls_05::upload(res, ssm);
        const std::vector<Attribute>& obj_data = pobj.GetAttributes();
        const std::vector<Attribute>& res_data = res.GetAttributes();
        ASSERT_EQ(obj_data.size(), res_data.size());
        for (size_t i = 0; i < obj_data.size(); ++i) {
            ASSERT_EQ(obj_data[i].name, res_data[i].name);
            ASSERT_EQ(obj_data[i].value, res_data[i].value);
        }
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}