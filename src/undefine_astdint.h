#ifndef UNDEFINE_ASTDINT_H
#define	UNDEFINE_ASTDINT_H
#pragma once

// prevent compiler errors

// Undefine no matter what
#undef int8_t
#undef int16_t
#undef int32_t
#undef intptr_t
#undef uint8_t
#undef uint16_t
#undef uint32_t
#undef uintptr_t
#undef int64_t
#undef uint64_t

// Mostly just for self-documenting, /Zc:__cplusplus doesn't exist until VS 2017.
// Can't be set below C++14
// In any case the result is the same
#if (__cplusplus < 201103L)
// A bit hackish but old language versions are a PITA
namespace boost {

// have it as big 

} // namespace boost


#include <boost/cstdint.hpp>
using boost::int8_t;
using boost::int16_t;
using boost::int32_t;
using boost::int64_t;
using boost::intptr_t;
using boost::uint8_t;
using boost::uint16_t;
using boost::uint32_t;
using boost::uint64_t;
using boost::uintptr_t;
#else
#include <cstdint>
#endif // (__cplusplus < 201103L)

#endif // !UNDEFINE_ASTDINT_H
