#pragma once

#define _MIHATSU_PRIMITIVE_CONCAT(a, b) a##b
#define _MIHATSU_CONCAT(a, b) _MIHATSU_PRIMITIVE_CONCAT(a, b)

#define _MIHATSU_SELECT_11TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define _MIHATSU_NARGS(...) _MIHATSU_SELECT_11TH(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define MIHATSU_NARGS_SELECTOR(prefix, ...)      \
    _MIHATSU_CONCAT(prefix, _MIHATSU_NARGS(__VA_ARGS__)) \
    (__VA_ARGS__)

#define _MIHATSU_REWIND1(z) z
#define _MIHATSU_REWIND2(a, z) z, a
#define _MIHATSU_REWIND3(a, b, z) z, a, b
#define _MIHATSU_REWIND4(a, b, c, z) z, a, b, c
#define _MIHATSU_REWIND5(a, b, c, d, z) z, a, b, c, d
#define _MIHATSU_REWIND6(a, b, c, d, e, z) z, a, b, c, d, e
#define _MIHATSU_REWIND7(a, b, c, d, e, f, z) z, a, b, c, d, e, f
#define _MIHATSU_REWIND8(a, b, c, d, e, f, g, z) z, a, b, c, d, e, f, g
#define _MIHATSU_REWIND9(a, b, c, d, e, f, g, h, z) z, a, b, c, d, e, f, g, h
#define _MIHATSU_REWIND10(a, b, c, d, e, f, g, h, i, z) z, a, b, c, d, e, f, g, h, i
#define MIHATSU_REWIND(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REWIND, __VA_ARGS__)
