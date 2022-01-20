#include "dict.hpp"
#include "dict_serialization.hpp"
#include "dict_json.hpp"

#include <cassert>
#include <sstream>

using namespace std;
using namespace utils;

////////////////////////////////////////
// smoke tests

dict_t make_sample_dict()
{
    dict_t dict =
    {
        {"five", 5},
        {"almost_zero", 0.01},
        {"boolean", true},
        {"inner_dict",
            {
                {"str_key", string("dict")},
                {"pi", 3.14}
            }
        }
    };

    return dict;
}

void test_copy_move()
{
    dict_t a = make_sample_dict();
    dict_t b = a;
    assert(b == a);

    dict_t c;
    c = a;
    assert(c == a);

    b = move(a);
    assert(a.empty());
    assert(b == c);

    dict_t d(move(b));
    assert(b.empty());
}

template<class exc, class func>
bool check_exception(func const& f)
{
    try
    {
        f();
    }
    catch (exc const&)
    {
        return true;
    }
    catch (...)
    {
        return false;
    }

    return false;
}

void test_put_n_get()
{
    dict_t d;

    d.put("one", 1);

    dict_t inner;

    inner.put("e_num", 2.71);
    d.put("inner", inner);

    assert(d.get<int>("one") == 1);
    assert(d.get_ptr<double>("one") == nullptr);

    assert(d.get<dict_t>("inner").get_ptr<double>("e") == nullptr);

    assert(check_exception<no_key_exception>([&d]()       { d.get<string>("string_key"); }));
    assert(check_exception<invalid_type_exception>([&d]() { d.get<string>("one"); }));
}

using composition_t = map<string, vector<map<string, double>>>;

composition_t get_composition()
{
    composition_t value =
    {
        {
            "one",
            {
                {},
                {
                    {"single", 3}
                },
                {
                    {"double_1", 1}, {"double_2", 2}
                }
            }
        },
        {
            "empty",
            {}
        }
    };

    return value;
}

void test_serialization()
{
    auto origin_value = get_composition();

    dict_t dict;
    write(dict, origin_value);

    composition_t read_value;
    read(dict, read_value);

    assert(read_value == origin_value);

    double& deeeep_value = dict.get<dict_t>("one").get<dict_t>("1").get<double>("single");
    assert(deeeep_value == 3);
}

void test_eq()
{
    auto value = get_composition();

    dict_t dict;
    write(dict, value);

    dict_t other_dict = dict;
    assert(dict == other_dict);

    double& deeeep_value = dict.get<dict_t>("one").get<dict_t>("1").get<double>("single");
    deeeep_value += 1;

    assert(dict != other_dict);
}

void test_json()
{
    auto dict = make_sample_dict();

    stringstream ss;
    save_to_json(ss, dict);

    ss.rdbuf()->pubseekpos(0);

    dict_t loaded_dict;
    load_from_json(ss, loaded_dict);

    assert(dict == loaded_dict);
}

void test_content()
{
    auto dict = make_sample_dict();

    assert(!dict.empty());
    assert(dict.is_dict("inner_dict"));
    assert(!dict.is_dict("five"));

    assert(dict.contains("five"));
    assert(!dict.contains("six"));
}

int main()
{
    auto d = make_sample_dict();
    (void)d;

    test_copy_move();
    test_put_n_get();
    test_serialization();
    test_eq();
    test_json();
    test_content();
}
