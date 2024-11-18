//
// Created by zoso on 11/18/24.
//

#ifndef TYPES_H
#define TYPES_H

#include <climits>
#include <type_traits>

using ram_size_t = unsigned long int;

using opcode_t = unsigned short int;
using address_t = unsigned int;

using byte_t = unsigned char;
using word_t = unsigned short int;
using long_t = unsigned int;
using sbyte_t = char;
using sword_t = short int;
using slong_t = int;

inline bool is_odd(const address_t address) {return (address & 1) != 0;}
inline bool is_even(const address_t address) {return (address & 1) == 0;}

template<typename T>
concept signed_integer  =
    std::is_same_v<sbyte_t, T> || std::is_same_v<sword_t, T> || std::is_same_v<slong_t, T>;

template<typename T>
concept unsigned_integer  =
std::is_same_v<byte_t, T> || std::is_same_v<word_t, T> || std::is_same_v<long_t, T>;
template<typename T>
concept integer = signed_integer<T> || unsigned_integer<T>;

template<typename T>
concept is_byte = std::is_same_v<T, byte_t> || std::is_same_v<T, sbyte_t>;

template<typename T>
concept is_word = std::is_same_v<T, word_t> || std::is_same_v<T, sword_t>;

template<typename T>
concept is_long = std::is_same_v<T, long_t> || std::is_same_v<T, slong_t>;

template<typename T>
concept big_integer = is_long<T> || is_word<T>;

template<typename T>
concept small_int = is_word<T> || is_byte<T>;

template<typename T>
constexpr int num_bits = sizeof(T) * CHAR_BIT;

template<integer T>
inline constexpr T sign_bit = static_cast<T>(1 << (num_bits<T>-1));

template<integer T>
constexpr bool is_negative(T value) {
    return value & sign_bit<T>;
}

template<is_long T>
inline long_t sign_extend(T value) {
    return value;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-count-overflow"
template <unsigned_integer T, int bits, int shift = 0>
T mask = ((1 << bits) - 1) << shift;
#pragma GCC diagnostic pop

template <unsigned_integer T, integer S, int shift = 0>
T type_mask = mask<T, num_bits<S>, shift>;

template <unsigned_integer T, int bits, int shift = 0>
T inverse_mask = ~mask<T, bits, shift>;

template <unsigned_integer T, integer S, int shift = 0>
T inverse_type_mask = ~type_mask<T, S, shift>;

template <unsigned_integer T, int mask_bits, int mask_shift = 0, int shift = mask_shift, bool shift_right = true>
T mask_and_shift(T value) {
    if constexpr (shift_right) {
        return (value & mask<T, mask_bits, mask_shift>) << shift;
    } else {
        return (value & mask<T, mask_bits, mask_shift>) >> shift;
    }
}

template<small_int T>
inline long_t sign_extend(T value) {
    return (sign_bit<T> & value) ? inverse_type_mask<long_t, T> | value : value;
}
#endif //TYPES_H
