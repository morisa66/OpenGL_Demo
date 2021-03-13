#ifndef __M_MATH_H__
#define __M_MATH_H__

#include "../_morisa.h"

MORISA_NAMESPACE_BEGIN

template<typename T>
inline const T clamp(T value, T max, T min,
	bool (*compare)(T, T) = [](T a, T b) -> bool {return a >= b; }) {
	if (compare(value, max)) return max;
	if (compare(min, value)) return min;
	return value;
}

MORISA_NAMESPACE_END

#endif
