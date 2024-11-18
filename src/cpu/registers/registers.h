//
// Created by zoso on 11/18/24.
//

#ifndef REGISTERS_H
#define REGISTERS_H

#include "../../types.h"

namespace cpu::registers {
    struct base_register {
        base_register() : _value(0) {}

        [[nodiscard]] inline long_t l() const { return _value; }

        [[nodiscard]] inline word_t w() const { return static_cast<word_t>(_value); }

    protected:
        long_t _value;
    };


    struct data_register : public base_register {
        template<integer T>
        inline data_register &operator=(const T &other) {
            _value = (_value & inverse_type_mask<long_t, T>) | static_cast<long_t>(other);
            return *this;
        }

        [[nodiscard]] inline byte_t b() const { return static_cast<byte_t>(_value); }

        template<integer T>
        [[nodiscard]] inline T read() const {
            if constexpr(is_long<T>) {
                return this->l();
            } else if constexpr(is_word<T>) {
                return this->w();
            } else if constexpr(is_byte<T>) {
                return this->b();
            }
            throw std::runtime_error("Compiler came up with additional types");
        }
    };

    struct address_register : public base_register {
        template<big_integer T>
        inline address_register &operator=(const T &other) {
            if constexpr(is_long<T>) {
                _value = other;
            } else if constexpr(is_word<T>) {
                _value = sign_extend(other);
            }
            return *this;
        }

        template<big_integer T>
        [[nodiscard]] inline T read() const {
            if constexpr(is_long<T>) {
                return this->l();
            } else if constexpr(is_word<T>) {
                return this->w();
            }
            throw std::runtime_error("Compiler came up with additional types");
        }
    };
}
#endif //REGISTERS_H
