#pragma once

#include "../internal/macro.h"

#define rep(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REP, __VA_ARGS__)
#define _MIHATSU_REP2(i, n) for (long long i = 0ll, __##i##__end = (n); i < __##i##__end; ++i)
#define _MIHATSU_REP3(i, a, b) for (long long i = (a), __##i##__end = (b); i < __##i##__end; ++i)
#define _MIHATSU_REP4(i, a, b, c) for (long long i = (a), __##i##__end = (b), __##i##__step = (c); __##i##__step > 0ll ? i < __##i##__end : i > __##i##__end; i += __##i##__step)
#define rep_rev(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REP_REV, __VA_ARGS__)
#define _MIHATSU_REP_REV2(i, n) for (long long i = static_cast<long long>(n) - 1; i >= 0; --i)
#define _MIHATSU_REP_REV3(i, a, b) for (long long i = static_cast<long long>(b) - 1, __##i##__end = (a); i >= __##i##__end; --i)
#define itr(...) _MIHATSU_ITR(MIHATSU_REWIND(__VA_ARGS__))
#define _MIHATSU_ITR(...) _MIHATSU_PRIMITIVE_ITR(__VA_ARGS__)
#define _MIHATSU_PRIMITIVE_ITR(v, ...) for (auto&& __VA_ARGS__ : (v))
#define itr_rev(...) _MIHATSU_ITR_REV(__COUNTER__, MIHATSU_REWIND(__VA_ARGS__))
#define _MIHATSU_ITR_REV(...) _MIHATSU_PRIMITIVE_ITR_REV(__VA_ARGS__)
#define _MIHATSU_PRIMITIVE_ITR_REV(id, v, ...)                                                      \
    if (auto&& __itr_rev_##id##__range = (v); true)                                                 \
        if (auto __itr_rev_##id##__begin = std::rbegin(__itr_rev_##id##__range); true)              \
            if (auto __itr_rev_##id##__end = std::rend(__itr_rev_##id##__range); true)              \
                for (; __itr_rev_##id##__begin != __itr_rev_##id##__end; ++__itr_rev_##id##__begin) \
                    if (auto&& __VA_ARGS__ = *__itr_rev_##id##__begin; true)
