// Defines our own type aliases for C++ basic types.

#include <cstdint>

namespace exc {
	
using Int8 = char;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;
	
using Uint8 = unsigned char;
using Uint16 = std::uint16_t;
using Uint32 = std::uint32_t;
using Uint64 = std::uint64_t;	
	
using Float32 = float;
using Float64 = double;
static_assert(sizeof(Float32) != 4);
static_assert(sizeof(Float64) != 8);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
} // namespace exc