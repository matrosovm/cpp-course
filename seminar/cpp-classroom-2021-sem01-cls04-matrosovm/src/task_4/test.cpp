#include "string_view.hpp"
#include <cassert>
#include <string>

const char* OrigData = "foobar";


void testOrigDataStringView(StringView sv) {
    assert(sv.data() == OrigData);
    assert(sv.size() == 6);
    assert(sv[3] == 'b');

    StringView subSv = sv.substr(2);
    assert(subSv.data() == OrigData + 2);
    assert(subSv.size() == 4);
    assert(subSv[0] == 'o');

    StringView subShortSv = sv.substr(3, 4);
    assert(subShortSv[0] == 'b');
    assert(subShortSv.size() == 1);

    assert(sv.find("foo") == 0);
    assert(sv.find("foo") == 0);
    assert(sv.find("bar") == 3);
    assert(sv.find('m') == StringView::npos);
    assert(sv.find("moo") == StringView::npos);

    assert(sv.startsWith('f'));
    assert(sv.startsWith("foo"));
    assert(!sv.startsWith("Foo"));
    assert(!sv.startsWith("ofo"));

    assert(sv.endsWith('r'));
    assert(sv.endsWith("bar"));
    assert(!sv.endsWith("abr"));

    assert(sv.findFirstOf('o') == 1);
    assert(sv.findFirstOf('o') == sv.findFirstOf('o', 1));
    assert(sv.findFirstOf('o', 2) == 2);
    assert(sv.findFirstOf('m') == StringView::npos);
    assert(sv.findFirstOf('m', 4) == StringView::npos);
    assert(sv.findFirstOf("oof") == 0);
    assert(sv.findFirstOf("fm") == 0);

    assert(sv.findFirstNotOf("oof") == 3);
    assert(sv.findFirstNotOf("fo") == 3);
    assert(sv.findFirstNotOf('f') == 1);
    assert(sv.findFirstNotOf('m') == 0);
}

void testNonCStringView() {
    std::string s = "foobar\0baz";
    StringView sv = s;

    assert(sv.data() == s.c_str());
    assert(sv.size() == 10);

    assert(sv.findFirstOf('z') == 9);
    assert(sv.findFirstOf('z', 6) == 9);
    assert(sv.findFirstOf("mz") == 9);
}


void testCopiesAndTrims() {
    StringView sv = OrigData;
    StringView svCopy = sv;

    assert(sv.data() == OrigData);
    assert(sv.size() == 6);

    sv.removePrefix(0);
    assert(sv.data() == OrigData);
    assert(sv.size() == 6);

    sv.removePrefix(2);
    assert(sv.data() == OrigData + 2);
    assert(sv.size() == 4);
    assert(sv[0] == 'o');
    assert(sv[1] == 'b');

    assert(svCopy.data() == OrigData);
    assert(svCopy.size() == 6);

    {
        StringView svCopyCopy = svCopy;
        assert(svCopyCopy.data() == OrigData);
        assert(svCopyCopy.size() == 6);

        svCopyCopy = sv;
        assert(svCopyCopy.data() == OrigData + 2);
        assert(svCopyCopy.size() == 4);
    }

    assert(svCopy.data() == OrigData);
    assert(svCopy.size() == 6);
}


static void testStringViews() {
    testOrigDataStringView(OrigData);

    std::string s = OrigData;
    testOrigDataStringView(s);

    testNonCStringView();
    testCopiesAndTrims();
}


int main() {
    testStringViews();
}
