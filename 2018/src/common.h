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

#include "../../2019/src/input.h"
#include "../../2019/src/result.h"

#include "../../2019/src/common/str.h"
#include "../../2019/src/common/vector.h"
#include "../../2019/src/common/set.h"
#include "../../2019/src/common/vec2.h"

inline int32_t min(int32_t a, int32_t b)
{
	return a < b ? a : b;
}

inline int32_t max(int32_t a, int32_t b)
{
	return a > b ? a : b;
}

#endif
