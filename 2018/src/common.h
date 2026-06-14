#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>

#include "../../2019/src/input.h"
#include "../../2019/src/result.h"

#include "../../2019/src/common/str.h"
#include "../../2019/src/common/vector.h"
#include "../../2019/src/common/set.h"
#include "../../2019/src/common/heap.h"
#include "../../2019/src/common/vec2.h"

#define SFMT_INT32_T "%" SCNd32
#define SFMT_UINT32_T "%" SCNu32
#define SFMT_INT64_T "%" SCNd64
#define SFMT_UINT64_T "%" SCNu64

inline int32_t min(int32_t a, int32_t b)
{
	return a < b ? a : b;
}

inline int32_t max(int32_t a, int32_t b)
{
	return a > b ? a : b;
}

inline int64_t min_i64(int64_t a, int64_t b)
{
	return a < b ? a : b;
}

inline int64_t max_i64(int64_t a, int64_t b)
{
	return a > b ? a : b;
}

#endif
