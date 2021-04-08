#define __PRIMITIVE_CONCAT(a, b) a##b
#define ___CONCAT(a, b) __PRIMITIVE_CONCAT(a, b)

#define __SELECT_11TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define NARGS(...) __SELECT_11TH(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define NARGS_SELECTOR(prefix, ...)      \
    ___CONCAT(prefix, NARGS(__VA_ARGS__)) \
    (__VA_ARGS__)

#define __REWIND1(z) z
#define __REWIND2(a, z) z, a
#define __REWIND3(a, b, z) z, a, b
#define __REWIND4(a, b, c, z) z, a, b, c
#define __REWIND5(a, b, c, d, z) z, a, b, c, d
#define __REWIND6(a, b, c, d, e, z) z, a, b, c, d, e
#define __REWIND7(a, b, c, d, e, f, z) z, a, b, c, d, e, f
#define __REWIND8(a, b, c, d, e, f, g, z) z, a, b, c, d, e, f, g
#define __REWIND9(a, b, c, d, e, f, g, h, z) z, a, b, c, d, e, f, g, h
#define __REWIND10(a, b, c, d, e, f, g, h, i, z) z, a, b, c, d, e, f, g, h, i
#define REWIND(...) NARGS_SELECTOR(__REWIND, __VA_ARGS__)
