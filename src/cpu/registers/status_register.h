//
// Created by zoso on 11/18/24.
//

#ifndef STATUS_REGISTER_H
#define STATUS_REGISTER_H

#include <cassert>

#include "../../types.h"
namespace cpu::registers {
    namespace {
        template<integer T>
        inline byte_t msb(const T value) {
            return (value >> (num_bits<T> - 1)) & 1; // Have to and out the rest for signed types
        }
    }

    enum class condition_code {
        t, f, hi, ls, cc, cs, ne, eq, vc, vs, pl, mi, ge, lt, gt, le
    };

    struct condition_code_helper {

        template<typename src, typename dest, bool add = true>
        inline static byte_t c(src s, dest d, dest result) {
            if constexpr(add) {
                return (msb(s) && msb(d)) || (!msb(result) && msb(d)) || (msb(s) && !msb(result));
                // apparently, the best thing is to use boolean logic on word_t values
            } else {
                return (msb(s) && !msb(d)) || (msb(result) && !msb(d)) || (msb(s) && msb(result));
            }
        }

        template<typename src, typename dest, bool add = true>
        inline static byte_t v(src s, dest d, dest result) {
            if constexpr (add) {
                return ((msb(s) && msb(d) && !msb(result)) || (!msb(s) && !msb(d) && msb(result))) << 1;
            } else {
                return ((!msb(s) && msb(d) && !msb(result)) || (msb(s) && !msb(d) && msb(result))) << 1;
            }
        }

        template<typename dest>
        inline static byte_t z(dest result) { return result == 0 ? 0b100 : 0; }

        template<typename dest>
        inline static byte_t n(dest result) {
            return msb(result) << 3;
        }

        inline static byte_t x(const word_t c) { return c << 4; }

        template<typename src, typename dest, bool add = true>
        inline static byte_t get_flags(const src s, const dest d, dest result) {
            const byte_t c = condition_code_helper::c<src, dest, add>(s, d, result);
            const byte_t v = condition_code_helper::v<src, dest, add>(s, d, result);
            const byte_t z = condition_code_helper::z<dest>(result);
            const byte_t n = condition_code_helper::n<dest>(result);
            const byte_t x = condition_code_helper::x(c);
            return x | n | z | v | c;
        }

        struct result_t {
            byte_t flags;
            word_t value;
        };

        // inline static result_t get_flags_2 (const word_t s, const word_t d) {
        //     bool carry, overflow, zero, negative;
        //     result_t result{};
        //     result.value = d;
        //     asm ("sub %5,%4":
        //                     "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative), "+r"(result.value):
        //                     "r"(s));
        //     result.flags = carry | (overflow << 1) | (zero << 2) | (negative << 3) | (carry << 4);
        //     return result;
        // }
    };

    struct status_register {
        status_register() : _value(0) {}

        template<bool arithmetic>
        inline void set_cc(const byte_t value) { // byte_t so no-one can "accidentally" overwrite the supervisor part
            const auto v = static_cast<word_t>(value);
            if constexpr(arithmetic) {
                _value = (_value & inverse_mask<word_t, 5>) | (v & mask<word_t, 5>);
            } else {
                _value = (_value & inverse_mask<word_t, 4>) | (v & mask<word_t, 4>);
            }
        }

        [[nodiscard]] inline byte_t get_cc() const {
            return static_cast<byte_t>(_value & 0b11111);
        }

        inline void set_sr(const word_t value) {
            assert((!supervisor_mode(value)) || supervisor_mode());
            _value = value;
        }

        static inline bool supervisor_mode(const word_t value) { return value & superuser_flag; }

        [[nodiscard]] inline bool supervisor_mode() const { return supervisor_mode(_value); }

        [[nodiscard]] inline bool trace_mode() const { return (_value & trace_flag); }

            /*
             * Condition code flags
             */

            [[nodiscard]] inline bool c() const { return _value & carry_flag; }

            [[nodiscard]] inline bool v() const { return _value & overflow_flag; }

            [[nodiscard]] inline bool z() const { return _value & zero_flag; }

            [[nodiscard]] inline bool n() const { return _value & negative_flag; }

            [[nodiscard]] inline bool x() const { return _value & extend_flag; }

            /*
             * Condition codes
             */

            inline constexpr bool t() { return true; }

            inline constexpr bool f() { return false; }

            // !c & !z
            [[nodiscard]] inline bool hi() const {
                word_t test = _value & 0b101;
                return (test == 0);
            }

            //inline bool ls() { return (c() || z()); }
            [[nodiscard]] inline bool ls() const {
                word_t test = _value & 0b101;
                return (test != 0);
            }

            [[nodiscard]] inline bool cc() const { return !c(); }

            [[nodiscard]] inline bool cs() const { return c(); }

            [[nodiscard]] inline bool ne() const { return !z(); }

            [[nodiscard]] inline bool eq() const { return z(); }

            [[nodiscard]] inline bool vc() const { return !v(); }

            [[nodiscard]] inline bool vs() const  { return v(); }

            [[nodiscard]] inline bool pl() const { return !n(); }

            [[nodiscard]] inline bool mi() const { return n(); }

            // (v & n) | (!v & !n) | z => 7 cycles, no branch/movec
            [[nodiscard]] inline bool ge() const {
                const word_t test = _value & 0b1110;
                return (test != 0b1000) && (test != 0b0010);
            }

            //inline bool lt() { return ((!v() && n() && !z()) || (v() && !n() && !z())); }
            [[nodiscard]] inline bool lt() const {
                const word_t test = _value & 0b1110;
                return (test == 0b10) || (test == 0b1000);
            }

            // !z & ((n & v) | (!n & !v))
            [[nodiscard]] inline bool gt() const {
                const word_t test = _value & 0b1110;
                return (test == 0) || (test == 0b1010);
            }

            // (!v & n) | (v & !n) | z => 7 cycles, no branch/movec!
            [[nodiscard]] inline bool le() const {
                const word_t test = _value & 0b1110;
                return (test != 0) && (test != 0b1010);
            }

            template<condition_code c_c>
            inline bool query_condition() {
                if constexpr(c_c == condition_code::t) {
                    return this->t();
                } else if constexpr(c_c == condition_code::f) {
                    return this->f();
                } else if constexpr(c_c == condition_code::hi) {
                    return this->hi();
                } else if constexpr(c_c == condition_code::ls) {
                    return this->ls();
                } else if constexpr(c_c == condition_code::cc) {
                    return this->cc();
                } else if constexpr(c_c == condition_code::cs) {
                    return this->cs();
                } else if constexpr(c_c == condition_code::ne) {
                    return this->ne();
                } else if constexpr(c_c == condition_code::eq) {
                    return this->eq();
                } else if constexpr(c_c == condition_code::vc) {
                    return this->vc();
                } else if constexpr(c_c == condition_code::vs) {
                    return this->vs();
                } else if constexpr(c_c == condition_code::pl) {
                    return this->pl();
                } else if constexpr(c_c == condition_code::mi) {
                    return this->mi();
                } else if constexpr(c_c == condition_code::ge) {
                    return this->ge();
                } else if constexpr(c_c == condition_code::lt) {
                    return this->lt();
                } else if constexpr(c_c == condition_code::gt) {
                    return this->gt();
                } else if constexpr(c_c == condition_code::le) {
                    return this->le();
                }
                // We are never going to see this, but apparently the compiler can't figure out we're complete
                return false;
            }

        private:
            word_t _value;

            static constexpr word_t carry_flag = 1;

            static constexpr word_t overflow_flag = 1 << 1;

            static constexpr word_t zero_flag = 1 << 2;

            static constexpr word_t negative_flag = 1 << 3;

            static constexpr word_t extend_flag = 1 << 4;

            static constexpr word_t superuser_flag = 1 << 13;

            static constexpr word_t trace_flag = 1 << 15;

        };
}
#endif //STATUS_REGISTER_H
