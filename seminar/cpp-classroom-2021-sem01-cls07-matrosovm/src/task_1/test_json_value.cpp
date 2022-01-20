#include <cassert>
#include "json_value.hpp"

using cls07::Json::JsonValue;

static void test_json() {
    JsonValue j;
    assert(j.IsNull());
    assert(!j.IsString());

    j = "hello";
    assert(j.IsString());
    assert(j.GetString() < "world");

    j.GetInt() = 42;
    assert(j.GetInt() + 24 == 66);

    j.Append(42);
    j.Append("foo");
    j[0] = "ok";
    j[1].Append(',');
    j.Append(" bye");

    cls07::Json::JsonValue::StringType sum = j[0].GetString() + j[1][0].GetString() + j[2].GetString();
    assert(sum == "ok, bye");
}


int main() {
    test_json();
    return 0;
}

