#include "test.h"

#include "set.h"

void test_stringset_from_string(void **state)
{
    StringSet *s = StringSetFromString("one,two, three four", ',');

    assert_true(StringSetContains(s, "one"));
    assert_true(StringSetContains(s, "two"));
    assert_true(StringSetContains(s, " three four"));

    assert_true(StringSetSize(s) == 3);

    StringSet *s2 = StringSetAddAll(s, s);

    assert_true(StringSetSize(s2) == 6);

    assert_true(s == s2);

    StringSetDestroy(s);
}

int main()
{
    PRINT_TEST_BANNER();
    const UnitTest tests[] =
    {
        unit_test(test_stringset_from_string)
    };

    return run_tests(tests);
}
