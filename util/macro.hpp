#pragma once

#include "../internal/macro.h"

#define rep(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REP, __VA_ARGS__)
#define _MIHATSU_REP2(i, n) for (long long i = 0ll, _##i##_end = (n); i < _##i##_end; ++i)
#define _MIHATSU_REP3(i, a, b) for (long long i = (a), _##i##_end = (b); i < _##i##_end; ++i)
#define _MIHATSU_REP4(i, a, b, c) for (long long i = (a), _##i##_end = (b), _##i##_step = (c); _##i##_step > 0ll ? i < _##i##_end : i > _##i##_end; i += _##i##_step)
#define rep_rev(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REP_REV, __VA_ARGS__)
#define _MIHATSU_REP_REV2(i, n) for (long long i = static_cast<long long>(n) - 1; i >= 0; --i)
#define _MIHATSU_REP_REV3(i, a, b) for (long long i = static_cast<long long>(b) - 1, _##i##_end = (a); i >= _##i##_end; --i)
#define itr(...) _MIHATSU_ITR(MIHATSU_REWIND(__VA_ARGS__))
#define _MIHATSU_ITR(...) _MIHATSU_PRIMITIVE_ITR(__VA_ARGS__)
#define _MIHATSU_PRIMITIVE_ITR(v, ...) for (auto&& __VA_ARGS__ : (v))
#define itr_rev(...) _MIHATSU_ITR_REV(__COUNTER__, MIHATSU_REWIND(__VA_ARGS__))
#define _MIHATSU_ITR_REV(...) _MIHATSU_PRIMITIVE_ITR_REV(__VA_ARGS__)
#define _MIHATSU_PRIMITIVE_ITR_REV(id, v, ...)                                                \
    if (auto&& _itr_rev_##id##_range = (v); true)                                             \
        if (auto _itr_rev_##id##_begin = std::rbegin(_itr_rev_##id##_range); true)            \
            if (auto _itr_rev_##id##_end = std::rend(_itr_rev_##id##_range); true)            \
                for (; _itr_rev_##id##_begin != _itr_rev_##id##_end; ++_itr_rev_##id##_begin) \
                    if (auto&& __VA_ARGS__ = *_itr_rev_##id##_begin; true)
